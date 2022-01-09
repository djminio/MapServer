#include "..\stdAfx.h"

#include "DefaultHeader.h"
#include "NPC_Pattern.h"
#include "Map.h"
#include "Scrp_exe.h"

#include "Kein_Item.h"

#include "..\LowerLayers\MyLog.h"
#include "Ability.h"
#include "FileMgr.h"
#include "CItem.h"
#include "WeatherControl.h"//020610 lsw
#include "dungeon.h"
#include "eventmgr.h"
#include "teambattle.h"
#include "LogManager.h"
#include "UserManager.h"
#include "DualManager.h"
#include "GuildManager.h"
#include "ArenaManager.h"
#include "RegenManager.h"
#include "WarStartInfo.h"
#include "StepMigration.h"
#include "NationSys.h"

#include "..\\lowerlayers\\servertable.h"
#include "..\\lowerlayers\\servertable2.h"

#define BLOCK_SIZE 6000

//class CGuardianGuild;
CGuardianGuild g_CGuardianGuild;

HENV  hEnv  = NULL;
HDBC  hDBC  = NULL;
HENV  hEnv_ChrLogDB= NULL;
HDBC  hDBC_ChrLogDB= NULL;

extern CServerTable* g_pServerTable;

extern cNation* g_pNation;
extern void CheckFightMap( char *mapname, short int &x, short int &y, CHARLIST *ch );		// 0910 YGI
extern int GetTotalItemWeight( CHARLIST *ch );
extern int update_BinaryData_to_Chr_Info	(UCHAR *ws, UCHAR *pa,  UCHAR *akill, UCHAR *skill_exp, UCHAR *tac_skill_exp, UCHAR *script_var, UCHAR *inv,  UCHAR *equip, UCHAR *quick, UCHAR *party,UCHAR *relation,UCHAR *employment,UCHAR *itemindex,char *, char *) ;	//1219 YGI
extern int update_BinaryData_to_Chr_Info2	(UCHAR *bankitem, char *login_id, char *name); // 1225

void displaySQLError(SQLHSTMT) ;
int UpdateCharID_SQL( int char_id );
int InsertBBS( char *name, char *title, char *contents );
int	GetTitleBBS( int count, char date[10][ FILENAME_MAX], char name[10][ FILENAME_MAX], char title[10][ FILENAME_MAX] );
int	GetContentsBBS( char *date, char *con );

// 030919 HK YGI
bool CanSaveUserData(LPCHARLIST ch, const int iCallType)
{	//< CSD-040212
	Dbg_Assert(ch != NULL);

	int iErrorType = 0;

	if (ch->GetLevel() == 0)
	{
		iErrorType = 1;
	}

	if (ch->HpMax == 0)
	{
		iErrorType = 2;
	}

	if (ch->ManaMax == 0)
	{
		iErrorType = 3;
	}

	if (ch->HungryMax == 0)
	{
		iErrorType = 4;
	}

	if (ch->updatable == 0)
	{
		iErrorType = 5;
	}
	// 030915 HK YGI
	if( strcmp( ch->Name, connections[ch->GetServerID()].name ) != 0 )
	{
		iErrorType = 6;
	}
	// 추가는 이곳에
	// 030923 HK YGI
	if (iErrorType == 0)
	{
		extern void SaveLoginLogoutLogByKein( CHARLIST *ch, int type, int );
		SaveLoginLogoutLogByKein( ch, 0, iCallType );
		return true;
	}		

	MyLog(LOG_IMPORTANT,
		  "CanSaveUserData() Failed : Name(%s), CallType(%d), ErrorType(%d)",
		  ch->Name, iCallType, iErrorType);
	return false;
}	//> CSD-040212

void ConvertLoad(char* pData, int nSize = MAX_STEP)
{	//< CSD-TW-030620
	CStepMigration stepMigration;
	stepMigration.Load(pData);
}	//> CSD-TW-030620

void ConvertSave(char* pData, int nSize = MAX_STEP)
{	//< CSD-TW-030620
	CStepMigration stepMigration;
	stepMigration.Save(pData);
}	//> CSD-TW-030620
int Init_SQL(LPSTR szDSN, LPSTR szUID, LPSTR szUPW)
{
	RETCODE		retCode;
	
	SQLAllocEnv(&hEnv);
	SQLAllocConnect(hEnv, &hDBC);
	
	retCode = SQLConnect(hDBC, (UCHAR *)szDSN, SQL_NTS, (UCHAR *)szUID, SQL_NTS, (UCHAR *)szUPW, SQL_NTS);
	
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{
		return(1);
	}
	
	return(0);
};

// acer 2
int Release_ChrLogDB_SQL(void)
{	
	if(hDBC_ChrLogDB != NULL)	SQLDisconnect(hDBC_ChrLogDB );
	if(hDBC_ChrLogDB != NULL)	SQLFreeConnect(hDBC_ChrLogDB);
	if(hEnv_ChrLogDB != NULL)	SQLFreeEnv(hEnv_ChrLogDB);
	
	return(1);
};	
// acer 2
int Release_SQL(void)
{
	if(hDBC != NULL)
		SQLDisconnect(hDBC);
	
	SQLFreeConnect(hDBC);
	SQLFreeEnv(hEnv);
	
	Release_ChrLogDB_SQL();
	
	return(1);
};

// 020430 acer 2
int Querry_SQL(LPSTR szQuerry, HDBC hdbc /*= NULL */)
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	if( hdbc == NULL ) hdbc = hDBC;
	
	SQLAllocStmt(hdbc, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return(1);
	}
	else {
		MyLog( LOG_DEBUG, "\n SQL: %s", szQuerry) ;
		displaySQLError(hStmt) ;
		SQLFreeStmt(hStmt, SQL_DROP);
		return(-1);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
// 함수설명: 캐릭터의 binary(크기가 큰배열들은 binary로 저장하고 binary로
//				읽어온다)를 저장(update)하는 함수.
//
// 파라미터:	ws; 캐릭터 정보중 WS[200]배열의 *, ps; PS[200]배열의 *
//				inv; inv[3][3][8]배열의 *
//				equip, quick; 해당 배열의 *
//				(참고: inv, equip, quick은 ItemAttr Structure의 배열이다)
//
// 리턴값: update의 성공 여부; 성공(1), 실패(ret < 0)
//
/////////////////////////////////////////////////////////////////////////////

int update_BinaryData_to_Chr_Info(UCHAR *ws, UCHAR *ps, UCHAR *skill, UCHAR *skill_exp, UCHAR *tac_skillEXP, UCHAR *script_var,  UCHAR *inv, UCHAR *equip, UCHAR *quick, 
								  UCHAR *party,
								  UCHAR *relation,
								  UCHAR *employment,
								  UCHAR *itemindex,
								  char *login_id, char *name)		// 1219 YGI
{
	SQLINTEGER  cbdata = 200, cbvalue = SQL_DATA_AT_EXEC ;
	SQLINTEGER	cbwslen, cbpslen, cbskilllen, cbskillexplen,cbtacskillexplen, cbscriptvarlen, cbinvlen, cbquick, cbequip, cbparty, cbrelation, cbemployment, cbitemindex;
	SQLRETURN	retcode;
	SQLHSTMT	hstmt;
	SQLPOINTER	pParamData;
	int			n=0;
	char		aaa=20;
	char		query[256]={0,};
	
	sprintf_s(query, sizeof(query), "UPDATE chr_info SET ws=?, ps=?,skill=?, skill_exp=?, tac_skillEXP=?, script_var=?, inventory=?, equip=?, quick=?, party=?, relation=?, employment=?, itemindex=?  WHERE name='%s'", name) ;
	
	//printf("\nSQL: %s", query) ;
	SQLAllocStmt(hDBC, &hstmt);
	retcode = SQLPrepare(hstmt, (UCHAR *)query, SQL_NTS);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		
		retcode= SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_BINARY,SQL_LONGVARBINARY, SIZE_OF_WS,			0, ws,			0, &cbwslen); 
		retcode= SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_BINARY,SQL_LONGVARBINARY, SIZE_OF_PS,			0, ps,			0, &cbpslen); 
		retcode= SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_BINARY,SQL_LONGVARBINARY, SIZE_OF_SKILL,			0, skill,		0, &cbskilllen); 
		retcode= SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_BINARY,SQL_LONGVARBINARY, SIZE_OF_SKILL_EXP,		0, skill_exp,	0, &cbskillexplen); 
		retcode= SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_BINARY,SQL_LONGVARBINARY, SIZE_OF_TAC_SKILL_EXP,	0, tac_skillEXP,0, &cbtacskillexplen); 
		retcode= SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_BINARY,SQL_LONGVARBINARY, SIZE_OF_SCRIPT_VAR,	0, script_var,	0, &cbscriptvarlen); 
		retcode= SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_BINARY,SQL_LONGVARBINARY, SIZE_OF_INV,			0, inv,			0, &cbinvlen); 
		retcode= SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_BINARY,SQL_LONGVARBINARY, SIZE_OF_EQUIP,			0, equip,		0, &cbequip); 
		retcode= SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_BINARY,SQL_LONGVARBINARY, SIZE_OF_QUICK,			0, quick,		0, &cbquick); 
		retcode= SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_BINARY,SQL_LONGVARBINARY, SIZE_OF_PARTY,		0, party,		0, &cbparty     ); 
		retcode= SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_BINARY,SQL_LONGVARBINARY, SIZE_OF_RELATION,		0, relation,	0, &cbrelation  ); 
		retcode= SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_BINARY,SQL_LONGVARBINARY, SIZE_OF_EMPLOYMENT,	0, employment,	0, &cbemployment); 
		retcode= SQLBindParameter(hstmt, 13, SQL_PARAM_INPUT, SQL_C_BINARY,SQL_LONGVARBINARY, SIZE_OF_ITEMINDEX,	0, itemindex,	0, &cbitemindex); 
		
		
		if(retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
		{
			SQLFreeStmt(hstmt, SQL_DROP);
			printf("\nBinding FAIL!!!") ;
			return -5 ;
		}
		
		cbequip			= SQL_LEN_DATA_AT_EXEC(0);
		cbquick			= SQL_LEN_DATA_AT_EXEC(0);
		cbpslen			= SQL_LEN_DATA_AT_EXEC(0);
		cbwslen			= SQL_LEN_DATA_AT_EXEC(0);
		cbskilllen		= SQL_LEN_DATA_AT_EXEC(0);
		cbskillexplen	= SQL_LEN_DATA_AT_EXEC(0);
		cbtacskillexplen= SQL_LEN_DATA_AT_EXEC(0);
		cbscriptvarlen	= SQL_LEN_DATA_AT_EXEC(0);
		cbinvlen		= SQL_LEN_DATA_AT_EXEC(0);
		
		cbparty			= SQL_LEN_DATA_AT_EXEC(0);
		cbrelation		= SQL_LEN_DATA_AT_EXEC(0);
		cbemployment	= SQL_LEN_DATA_AT_EXEC(0);
		cbitemindex		= SQL_LEN_DATA_AT_EXEC(0);
		
		retcode = SQLExecute(hstmt) ;
		
		while(retcode == SQL_NEED_DATA) 
		{      
			retcode = SQLParamData(hstmt, &pParamData) ;
			if(retcode == SQL_NEED_DATA) 
			{
				if(pParamData == ws) 				SQLPutData(hstmt, ws, SIZE_OF_WS);
				else if(pParamData == ps) 			SQLPutData(hstmt, ps, SIZE_OF_PS);      
				else if(pParamData == skill) 		SQLPutData(hstmt, skill, SIZE_OF_SKILL);      
				else if(pParamData == skill_exp) 	SQLPutData(hstmt, skill_exp, SIZE_OF_SKILL_EXP );      
				else if(pParamData == tac_skillEXP)	SQLPutData(hstmt, tac_skillEXP, SIZE_OF_TAC_SKILL_EXP );      
				else if(pParamData == script_var ) 	SQLPutData(hstmt, script_var, SIZE_OF_SCRIPT_VAR );
				else if(pParamData == inv) 			SQLPutData(hstmt, inv, SIZE_OF_INV);
				else if(pParamData == equip) 		SQLPutData(hstmt, equip, SIZE_OF_EQUIP);
				else if(pParamData == quick) 		SQLPutData(hstmt, quick, SIZE_OF_QUICK );
				
				else if(pParamData == party ) 		SQLPutData(hstmt, party,		SIZE_OF_PARTY );
				else if(pParamData == relation) 	SQLPutData(hstmt, relation,		SIZE_OF_RELATION);
				else if(pParamData == employment ) 	SQLPutData(hstmt, employment,	SIZE_OF_EMPLOYMENT );
				else if(pParamData == itemindex ) 	SQLPutData(hstmt, itemindex,	SIZE_OF_ITEMINDEX );
				
			}
			else break ;
		}
		
		retcode = SQLExecute(hstmt) ;
		
		if(retcode == SQL_NEED_DATA)
		{
			retcode= SQLCancel(hstmt) ;// 020620 YGI
			//retcode= SQLTransact(SQL_NULL_HENV, hDBC, SQL_COMMIT);
			retcode= SQLFreeStmt(hstmt, SQL_DROP );
			if(retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
				MyLog(LOG_NORMAL, "Transact commit Error") ;
			}
			//printf("\nTransact & free") ;
			return(1); // succeed
		}
		else
		{
			displaySQLError(hstmt) ;
			//SQLTransact(SQL_NULL_HENV, hDBC, SQL_ROLLBACK);
			SQLFreeStmt(hstmt, SQL_DROP);
			return(-3); 
		}
	}
	
	SQLFreeStmt(hstmt, SQL_DROP);
	return 1;
}



int update_BinaryData_to_Chr_Info2(	 UCHAR *bankitem, char *login_id, char *name) // 1225
{
	SQLINTEGER  cbdata = 200, cbvalue = SQL_DATA_AT_EXEC ;
	SQLINTEGER	cbbankitemlen;
	SQLRETURN	retcode;
	SQLHSTMT	hstmt;
	SQLPOINTER	pParamData;
	int			n=0;
	char		aaa=20;
	char		query[256]={0,};
	
	sprintf_s(query, sizeof(query), "UPDATE chr_info2 SET bankitem=? WHERE name='%s'", name) ;
	
	SQLAllocStmt(hDBC, &hstmt);
	retcode = SQLPrepare(hstmt, (UCHAR *)query, SQL_NTS);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) 
	{
		retcode= SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_BINARY,SQL_LONGVARBINARY, SIZE_OF_BANKITEM,			0, bankitem,			0, &cbbankitemlen );
		
		if(retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
		{
			SQLFreeStmt(hstmt, SQL_DROP);
			printf("\nBinding FAIL!!!") ;
			return -5 ;
		}
		
		cbbankitemlen	= SQL_LEN_DATA_AT_EXEC(0);
		
		retcode = SQLExecute(hstmt) ;
		while(retcode == SQL_NEED_DATA) 
		{      
			retcode = SQLParamData(hstmt, &pParamData) ;
			if(retcode == SQL_NEED_DATA) 
			{
				if(pParamData == bankitem ) 				SQLPutData(hstmt, bankitem , SIZE_OF_BANKITEM );
			}
			else break ;
		}
		
		retcode = SQLExecute(hstmt) ;
		
		if(retcode == SQL_NEED_DATA)
		{
			retcode= SQLCancel(hstmt) ;// 020620 YGI
			//retcode= SQLTransact(SQL_NULL_HENV, hDBC, SQL_COMMIT);
			retcode= SQLFreeStmt(hstmt, SQL_DROP );
			
			if(retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
				MyLog(LOG_NORMAL, "Transact commit Error") ;
			}
			//printf("\n-----Transact & free----") ;
			return(1); // succeed
		}
		else
		{
			displaySQLError(hstmt) ;
			//SQLTransact(SQL_NULL_HENV, hDBC, SQL_ROLLBACK);
			SQLFreeStmt(hstmt, SQL_DROP);
			return(-3); 
		}
	}
	
	SQLFreeStmt(hstmt, SQL_DROP);
	return 1;
}

int updateCharacterStatus(t_connection c[], int char_id)
{		
	CHARLIST *chr= &c[char_id].chrlst;
	if (!chr) return -1;
	if(!::CanSaveUserData(chr,1)){return -1;}
		
	SQLRETURN retcode;
	SQLHSTMT  hstmt;
	char query[2048] = {0,};

	if( strcmp( chr->Name, c[ char_id].name ) )
	{
		MyLog( LOG_NORMAL, "Error :(a) chr->Name = %s   c[ char_id].name = %s", chr->Name, c[ char_id].name );
		return -1;
	}
	
	CheckFightMap(chr->MapName, chr->X, chr->Y, chr );		// 0910 YGI
	//< CSD-HK-030829
	DWORD temp_NWCharacter;
	memcpy(&temp_NWCharacter,&chr->NWCharacter,sizeof(DWORD));
	//> CSD-HK-030829
	char aStepInfo[20];
	chr->GetStepInfo(aStepInfo, sizeof(aStepInfo));
	ConvertSave(aStepInfo); // 저장을 위한 데이타 변형(각 필드에 1를 더함)
	// 퀴리문 생성
	sprintf_s(query, sizeof(query), 
		"UPDATE chr_info "
		"SET lev = %d, spritvalue = %d, social_status = %d, fame = %d, fame_pk = %d, guildname = '%s' "
		      "WHERE name= '%s'",
			  chr->GetLevel(),
			  chr->GetGuildCode(),
			  chr->social_status,
			  chr->fame,
			  temp_NWCharacter, // CSD-HK-030829
			  aStepInfo,
			  c[char_id].name);  // CSD-030806
	SQLAllocStmt(hDBC, &hstmt);
	retcode = SQLExecDirect(hstmt, (UCHAR *)query, SQL_NTS);
	if(retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) 
	{	
		displaySQLError(hstmt) ;
		//SQLTransact(SQL_NULL_HENV, hDBC, SQL_ROLLBACK);
		SQLFreeStmt(hstmt, SQL_DROP);
		return -1 ;
	}	
	else {
		//SQLTransact(SQL_NULL_HENV, hDBC, SQL_COMMIT);
		SQLFreeStmt(hstmt, SQL_DROP) ;
	}	
	
	DWORD nation;		//1004 YGI
	memcpy( &nation, &chr->name_status, sizeof( DWORD ) );
	
	sprintf_s(query, sizeof(query), "UPDATE chr_info SET"
		" Str= %d, Con= %d, Dex= %d, Wis= %d, "
		" Int= %d, MoveP= %d, Char= %d, Endu= %d, Moral= %d, Luck= %d, wsps= %d, tactics= %d, nation = %d, Money= %d, Hp= %d, HpMax = %d, "
		" mana= %d, manamax= %d, condition= %d, mapname='%s', sight= %d, Age= %d, bAlive= %d "
		" where name= '%s'"
		, chr->Str, chr->Con, chr->Dex, chr->Wis,
		chr->Int, chr->MoveP, chr->Char, chr->Endu, chr->Moral, chr->Luck, chr->wsps, chr->Tactics, nation, chr->Money, chr->Hp, chr->HpMax,		//1004 YGI
		chr->Mana, chr->ManaMax, 
		chr->GetState(), 
		chr->MapName, 
		chr->Sight, 
		chr->Age, 
		chr->bAlive, 
		c[char_id].name);
	SQLAllocStmt(hDBC, &hstmt) ;
	retcode = SQLExecDirect(hstmt, (UCHAR *)query, SQL_NTS);
	if(retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
		displaySQLError(hstmt) ;
		//SQLTransact(SQL_NULL_HENV, hDBC, SQL_ROLLBACK);
		SQLFreeStmt(hstmt, SQL_DROP);
		return -1 ;
	}	
	else {
		//SQLTransact(SQL_NULL_HENV, hDBC, SQL_COMMIT);
		SQLFreeStmt(hstmt, SQL_DROP) ;
	}	
	
	sprintf_s(query, sizeof(query), "UPDATE chr_info SET"
		" hungry= %d, hungrymax= %d, killmon= %d, "
		" killanimal= %d, killpc = %d, resist_poison= %d, resist_stone= %d, resist_magic= %d, resist_fire= %d, "
		" resist_ice= %d, resist_elect= %d, x=%d, y=%d, "
		" acc_equip1= %d, acc_equip2= %d, acc_equip3= %d, acc_equip4= %d "
		" where name= '%s'"
		,chr->Hungry, chr->HungryMax, chr->killmon,
		chr->killanimal, chr->killpc, 
		
		chr->GetBasicResist(RT_POISON), // 저주계열 마법에 대한 저항력
		chr->GetBasicResist(RT_CURSE),	 //	저주계열 마법에 대한 저항력
		chr->GetBasicResist(RT_HOLY),    // 신력계열 공격 마법에 대한 저항력
		chr->GetBasicResist(RT_FIRE),     //	불계열 공격 마법에 대한 저항력
		chr->GetBasicResist(RT_ICE) ,      //	얼음계열 공격 마법에 대한 저항력
		chr->GetBasicResist(RT_ELECT),   //	전격계열 공격 마법에 대한 저항력
		chr->X, chr->Y,
		chr->accessory[0], chr->accessory[1], chr->accessory[2], chr->accessory[3], 
		c[char_id].name) ;  
	
	SQLAllocStmt(hDBC, &hstmt) ;
	retcode = SQLExecDirect(hstmt, (UCHAR *)query, SQL_NTS);
	if(retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
		displaySQLError(hstmt) ;
		//SQLTransact(SQL_NULL_HENV, hDBC, SQL_ROLLBACK);
		SQLFreeStmt(hstmt, SQL_DROP);
		return -1 ;
	}	
	else {
		//SQLTransact(SQL_NULL_HENV, hDBC, SQL_COMMIT);
		SQLFreeStmt(hstmt, SQL_DROP) ;
	}	
	
	
	// 010406 YGI
	// 010531 KHS
	DWORD win_defeat;
	memcpy( &win_defeat, &chr->WinLoseScore, sizeof(DWORD) );
	
	sprintf_s(query, sizeof(query), "UPDATE chr_info SET"
		" openhouse = %d, reserved_point= %d,"
		" bankmoney= %u, win_defeat = %d, LastLoan= %d, exp = %u, "
		" disease1=%d, disease2=%d, disease3=%d, disease4=%d, disease5=%d, disease6=%d, viewtype= %d, "
		" ladderscore = %d, nut1=%d, nut2=%d, nut3=%d "
		" where name= '%s'",
		chr->openhouse,
		chr->GetReservedPoint(),
		chr->GetBankMoney(),
		win_defeat,
		chr->LastLoan,
		//chr->LastLoan_time,
		chr->Exp,
		0, 0, 0, 0, 0, 0,
		chr->viewtype,
		chr->LadderScore,
		chr->nk[ N_VYSEUS], 
		chr->nk[ N_ZYPERN], 
		chr->nk[ N_YLLSE], 
		c[char_id].name);  
	
	SQLAllocStmt(hDBC, &hstmt) ;
	retcode = SQLExecDirect(hstmt, (UCHAR *)query, SQL_NTS);
	if(retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
		printf("\n Update Character: Exec Direct Error ; %s", query) ;
		displaySQLError(hstmt) ;
		//SQLTransact(SQL_NULL_HENV, hDBC, SQL_ROLLBACK);
		SQLFreeStmt(hstmt, SQL_DROP);
		return -1 ;
	}
	else {
		//SQLTransact(SQL_NULL_HENV, hDBC, SQL_COMMIT);
		SQLFreeStmt(hstmt, SQL_DROP) ;
	}
	
	
	///////////////////////// 0218 YGI ////////////////////////
	
	EndsetPartyMenberForUpdate( chr );
	
	if( update_BinaryData_to_Chr_Info(	(UCHAR *)chr->Ws, 
		(UCHAR *)chr->Ps, 
		(UCHAR *)chr->Skill, 
		(UCHAR *)chr->skillexp, 
		(UCHAR *)chr->tac_skillEXP,
		(UCHAR *)var[ char_id],
		(UCHAR *)chr->inv, 
		(UCHAR *)chr->equip, 
		(UCHAR *)chr->quick, 
		(UCHAR *)chr->party_str, 
		(UCHAR *)chr->relation_str, 
		(UCHAR *)chr->employment_str, 
		(UCHAR *)chr->Item, 
		c[char_id].id, c[char_id].name) < 0) {		//1219 YGI
		printf("\n Update Character: Update BIN Error") ;
		return -2 ;
	}
	else 
	{
		if( update_BinaryData_to_Chr_Info2(  (UCHAR *)chr->bank, c[char_id].id, c[char_id].name) < 0) 
			return -2;
	}
	
	return 1 ;
}

void updateCharacterVeryImportantStatusToLoginServer( t_connection c[], int char_id )
{
	LPCHARLIST ch = &c[ char_id].chrlst;
	
	if(!::CanSaveUserData(ch,2)){return;}

	t_packet p;
	t_update_very_important_status *tp = NULL;
	p.h.header.type = CMD_UPDATE_VERY_IMPORTANT_STATUS;
	p.h.header.size = sizeof( t_update_very_important_status );
	tp = &p.u.update_very_important_status;
	
	strcpy_s( tp->name, sizeof(tp->name),	c[char_id].name );
	
	tp->Level = ch->GetLevel(); // CSD-030806
	
	tp->Str  				= ch->Str;
	tp->Con  				= ch->Con;
	tp->Dex  				= ch->Dex;
	tp->Wis  				= ch->Wis;
	tp->Int  				= ch->Int;
	tp->MoveP				= ch->MoveP;
	tp->Char 				= ch->Char;
	tp->Endu 				= ch->Endu;
	tp->Moral				= ch->Moral;
	tp->Luck 				= ch->Luck;
	tp->wsps 				= ch->wsps;
	tp->HpMax				= ch->HpMax ;
	tp->ManaMax				= ch->ManaMax;
	tp->HungryMax			= ch->HungryMax;
	tp->reserved_point = ch->GetReservedPoint();
	tp->Exp					= ch->Exp;
	
	QueuePacket( connections, DB_DEMON, &p, 1 );
}

void updateCharacterVeryImportant_TacticsSkillExp_ToLoginServer( t_connection c[], int char_id )
{
	LPCHARLIST ch = &c[ char_id].chrlst;
	
	if(!::CanSaveUserData(ch,3)){return;}
	
	t_packet p;
	t_update_very_important_tactics *tp = NULL;

	p.h.header.type = CMD_UPDATE_VERY_IMPORTANT_TACTICS;
	p.h.header.size = sizeof( t_update_very_important_tactics );
	tp = &p.u.update_very_important_tactics;
	
	strcpy_s( tp->name, sizeof(tp->name),	c[char_id].name );
	memcpy( tp->tac_skillEXP, c[ char_id].chrlst.tac_skillEXP, SIZE_OF_TAC_SKILL_EXP );
	
	QueuePacket( connections, DB_DEMON, &p, 1 );
}							


//acer5-----------------------------------
int AddCRC( void *pSource, int size, int step )
{
	int count = 0;
	for( int i=0; i<size; i+=step )
	{
		count += ((char*)pSource)[i];
	}
	return count;
}
// --------------------------------------------

void updateCharacterStatusToLoginServer( t_connection c[], int char_id)
{
	t_packet p;
	t_update_char_db *tp;
	LPCHARLIST ch = &c[ char_id].chrlst;
	
	if(!::CanSaveUserData(ch,4)){return;}
	
	p.h.header.type = CMD_UPDATE_CHAR_DB;
	tp = &p.u.update_char_db;
	
	strcpy_s( tp->id, sizeof(tp->id),		c[char_id].id );
	strcpy_s( tp->name, sizeof(tp->name),	c[char_id].name );
	
	tp->Level = ch->GetLevel(); // CSD-030806
	tp->nGuildCode		= ch->GetGuildCode(); // CSD-030324
	tp->social_status	= ch->social_status;
	tp->fame			= ch->fame;
	tp->NWCharacter		= ch->NWCharacter;			// 011015 LTS
	
	tp->Str  				= ch->Str;
	tp->Con  				= ch->Con;
	tp->Dex  				= ch->Dex;
	tp->Wis  				= ch->Wis;
	tp->Int  				= ch->Int;
	tp->MoveP				= ch->MoveP;
	tp->Char 				= ch->Char;
	tp->Endu 				= ch->Endu;
	tp->Moral				= ch->Moral;
	tp->Luck 				= ch->Luck;
	tp->wsps 				= ch->wsps;
	
	tp->Tactics 			= ch->Tactics;				// 선택한 전투기술 (보여주기 위함)
	memcpy( &tp->nation, &ch->name_status, sizeof( DWORD ) );		// 1004 YGI
	tp->Money = ch->Money;
	tp->Hp = ch->Hp;
	tp->HpMax = ch->HpMax ;
	tp->Mana = ch->Mana;					// 마법력
	tp->ManaMax	= ch->ManaMax;
	tp->Hungry = ch->Hungry;	
	tp->HungryMax = ch->HungryMax;

	tp->Condition	= ch->GetState();
	memcpy( tp->MapName, MapName, NM_LENGTH );
	tp->Sight = ch->Sight;
	tp->Age = ch->Age;
	tp->bAlive		= ch->bAlive;					// 캐릭터의 생사여부(	REMOVE_:0 / ALIVE_:1 / DEAD_:2 / BUFE_:3)
	
	tp->killmon = ch->killmon;						// 1계열몬스터 킬링스
	tp->killanimal = ch->killanimal;				// 동물 킬링수
	tp->killpc = ch->killpc;						// 사람,NPC 킬링수
	//030227 lsw
	tp->nPoison = ch->GetBasicResist(RT_POISON); // 저주계열 마법에 대한 저항력
	tp->nCurse	= ch->GetBasicResist(RT_CURSE);	 //	저주계열 마법에 대한 저항력
	tp->nHoly	= ch->GetBasicResist(RT_HOLY);    // 신력계열 공격 마법에 대한 저항력
	tp->nFire	= ch->GetBasicResist(RT_FIRE);     //	불계열 공격 마법에 대한 저항력
	tp->nIce	= ch->GetBasicResist(RT_ICE);       //	얼음계열 공격 마법에 대한 저항력
	tp->nElect	= ch->GetBasicResist(RT_ELECT);   //	전격계열 공격 마법에 대한 저항력
	
	tp->X = ch->X;
	tp->Y = ch->Y;
	
	tp->accessory[0] = ch->accessory[0];
	tp->accessory[1] = ch->accessory[1];
	tp->accessory[2] = ch->accessory[2];
	tp->accessory[3] = ch->accessory[3];
	
	tp->openhouse		= ch->openhouse;
	tp->reserved_point  = ch->GetReservedPoint();
	tp->BankMoney		= ch->GetBankMoney();
	// 010406 YGI
	memcpy( &tp->win_defeat, &ch->WinLoseScore, sizeof( DWORD ) );
	tp->LadderScore	=	ch->LadderScore;
	
	tp->LastLoan		= ch->LastLoan;
	//tp->LastLoan_time	= ch->LastLoan_time;
	tp->Exp				= ch->Exp;
	tp->viewtype		= ch->viewtype;
	
	tp->nk3				= ch->nk[N_VYSEUS];
	tp->nk4				= ch->nk[N_ZYPERN];
	tp->nk6				= ch->nk[N_YLLSE];
	ch->GetStepInfo(tp->aStepInfo, sizeof(tp->aStepInfo));
	p.h.header.size = sizeof( t_update_char_db);
	CheckFightMap(tp->MapName, tp->X, tp->Y, ch );		// 0910 YGI
	tp->check_crc = AddCRC(p.u.data, p.h.header.size-4, 10 ); // acer5
	QueuePacket( connections, DB_DEMON, &p, 1 );
}

void updateBinaryDataToLoaginServer( t_connection c[], int id )
{														
	t_packet p;										
	LPCHARLIST ch = &c[id].chrlst;			
	
	if(!::CanSaveUserData(ch,5)){return;}

	p.h.header.type = CMD_UPDATE_BINARY_DATA0;														
	memcpy(p.u.server_update_binary_data0.name, c[id].name, NM_LENGTH);
	memcpy(p.u.server_update_binary_data0.id, c[id].id,  ID_LENGTH );
	memcpy(p.u.server_update_binary_data0.Ws,				(UCHAR *)ch->Ws,			SIZE_OF_WS );
	memcpy(p.u.server_update_binary_data0.Ps,				(UCHAR *)ch->Ps,			SIZE_OF_PS );
	memcpy(p.u.server_update_binary_data0.Skill,			(UCHAR *)ch->Skill,			SIZE_OF_SKILL );
	memcpy(p.u.server_update_binary_data0.skillexp,		(UCHAR *)ch->skillexp,		SIZE_OF_SKILL_EXP );
	memcpy(p.u.server_update_binary_data0.tac_skillEXP,	(UCHAR *)ch->tac_skillEXP,	SIZE_OF_TAC_SKILL_EXP );
	p.h.header.size = sizeof( t_server_update_binary_data0 );
	p.u.server_update_binary_data0.check_crc = AddCRC(p.u.data, p.h.header.size-4, 10 ); // acer5
	QueuePacket( c, DB_DEMON, &p, 1 );
	
	
	p.h.header.type = CMD_UPDATE_SCRIPT_DATA;
	memcpy(p.u.server_update_script_data.name, c[id].name, NM_LENGTH);
	memcpy(p.u.server_update_script_data.id, c[id].id,  ID_LENGTH );
	memcpy(p.u.server_update_script_data.script_var,			(UCHAR *)var[ id],			SIZE_OF_SCRIPT_VAR );
	p.h.header.size = sizeof( t_server_update_script_data );
	p.u.server_update_script_data.check_crc = AddCRC(p.u.data, p.h.header.size-4, 10 ); // acer5
	QueuePacket( c, DB_DEMON, &p, 1 );
	
	//////////////////////////// 0218 YGI //////////////////////
	EndsetPartyMenberForUpdate( ch );		
	
	p.h.header.type = CMD_UPDATE_BINARY_DATA1;	
	memcpy(p.u.server_update_binary_data1.name, c[id].name, NM_LENGTH);
	memcpy(p.u.server_update_binary_data1.id, c[id].id,  ID_LENGTH );
	memcpy(p.u.server_update_binary_data1.equip,			(UCHAR *)ch->equip,			SIZE_OF_EQUIP);
	memcpy(p.u.server_update_binary_data1.quick,			(UCHAR *)ch->quick,			SIZE_OF_QUICK);
	memcpy(p.u.server_update_binary_data1.party,			(UCHAR *)ch->party_str,			SIZE_OF_PARTY );
	memcpy(p.u.server_update_binary_data1.relation,			(UCHAR *)ch->relation_str,		SIZE_OF_RELATION);
	memcpy(p.u.server_update_binary_data1.employment,		(UCHAR *)ch->employment_str,	SIZE_OF_EMPLOYMENT);
	p.h.header.size = sizeof( t_server_update_binary_data1 );
	p.u.server_update_binary_data1.check_crc = AddCRC(p.u.data, p.h.header.size-4, 10 ); // acer5
	QueuePacket( c, DB_DEMON, &p, 1 );
	
	p.h.header.type = CMD_UPDATE_INV_DATA;		
	memcpy(p.u.server_update_inv_data.name, c[id].name, NM_LENGTH);
	memcpy(p.u.server_update_inv_data.id,   c[id].id,  ID_LENGTH );
	memcpy(p.u.server_update_inv_data.inv,			(UCHAR *)ch->inv,				SIZE_OF_INV );
	p.h.header.size = sizeof( t_server_update_inv_data );	
	p.u.server_update_inv_data.check_crc = AddCRC(p.u.data, p.h.header.size-4, 10 ); // acer5
	QueuePacket( c, DB_DEMON, &p, 1 );
	
	p.h.header.type = CMD_UPDATE_ITEM_DATA;		
	memcpy(p.u.server_update_item_data.name, c[id].name, NM_LENGTH);
	memcpy(p.u.server_update_item_data.id,   c[id].id,  ID_LENGTH );
	memcpy(p.u.server_update_item_data.Item,			(UCHAR *)ch->Item,			SIZE_OF_ITEMINDEX );
	p.h.header.size = sizeof( t_server_update_item_data	);		
	p.u.server_update_item_data.check_crc = AddCRC(p.u.data, p.h.header.size-4, 10 ); // acer5
	QueuePacket( c, DB_DEMON, &p, 1 );
	
	p.h.header.type = CMD_UPDATE_BANKITEM_DATA;
	memcpy(p.u.server_update_bankitem_data.name,	c[id].name, NM_LENGTH);
	memcpy(p.u.server_update_bankitem_data.id,		c[id].id,  ID_LENGTH );
	strcpy_s(p.u.server_update_bankitem_data.mapname, sizeof(p.u.server_update_bankitem_data.mapname), MapName );
	
	memcpy(p.u.server_update_bankitem_data.bankitem,		(UCHAR *)ch->bank,		SIZE_OF_BANKITEM );
	p.h.header.size = sizeof( t_server_update_bankitem_data	);		
	p.u.server_update_bankitem_data.check_crc = AddCRC(p.u.data, p.h.header.size-4, 10 ); // acer5
	QueuePacket( c, DB_DEMON, &p, 1 );
}

void displaySQLError(SQLHSTMT hstmt)
{
	SQLCHAR err[80] ;
	SQLINTEGER nep ;
	SQLCHAR msg[255] ;
	SQLSMALLINT num ;
	
	SQLGetDiagRec(SQL_HANDLE_STMT, hstmt, 1, err, &nep, msg, 254, &num) ;
	//YGI acer
	MyLog( 0, "\nSQL Error MSG: %s; %s", err, msg) ;
}


int AddBoxItem_SQL(int id)
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	char		szQuerry[255];
	
	sprintf_s(szQuerry, sizeof(szQuerry), "insert into box_item(id) values (%d)", id);
	
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{
		//SQLTransact(SQL_NULL_HENV, hDBC, SQL_COMMIT);
		SQLFreeStmt(hStmt, SQL_DROP);
		return(1); // succeed
	}
	else
	{
		//SQLTransact(SQL_NULL_HENV, hDBC, SQL_ROLLBACK);
		SQLFreeStmt(hStmt, SQL_DROP);
		return(-3); // unknown error
	}
};

int DeleteBoxItem_SQL(int id)
{
	HSTMT hStmt = NULL;
	RETCODE retCode;
	char szQuerry[255];
	
    sprintf_s(szQuerry, sizeof(szQuerry), "delete from box_item where id=%d", id);
	
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{
		retCode = SQLFetch(hStmt);
		SQLFreeStmt(hStmt, SQL_DROP);
		return(1);
	}
	SQLFreeStmt(hStmt, SQL_DROP);
	return -1;
};
//-----------------------------------------
int TotalConnections( void )
{
	return 1;
}


//---------------------------------------------


DiseaseTable	disease_tbl[6];
// 질병 테이블 ~~~		0104
int GetDisease_SQL( )
{	
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
	char		szQuerry[2047];
	DWORD		t;
	//char		name[31];
	
	for( int a=0; a<6; a++ )
	{
	/*		sprintf(szQuerry, "Select disease_number, han_name, name, lv, nut1_1, nut2_1, nut3_1, probability_1, andor_1, checktime_1, contact_char, talk_char, fuck_char,"
	"contact_obj,eat_obj,nut1_2,nut2_2,nut3_2,probability_2,checktime_2,andor_2,dec_str,dec_dex,dec_con,dec_wis,dec_int,dec_movp,dec_char,"
	"dec_endu,dec_mor,dec_luck,dec_ws,dec_ps,recoverable,life,health,mp,dp,max_,damage,att_rate,depen_rate,ac,mad,poison,stony,sluggish,active,"
	"paralysis,active_time,maintain_time,natural_recovery,nut1_3,nut2_3,nut3_3,probability_3,checktime_3,andor_3,price_heal,price_prevent"
	"From DiseaseMain Where disease_number=%d ", a+1);
		*/
		sprintf_s(szQuerry, sizeof(szQuerry), "Select *"
			"From DiseaseMain Where disease_number=%d ", a+1);
		SQLAllocStmt(hDBC, &hStmt);
		retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
		if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
		{
			retCode = SQLFetch(hStmt);
			if(retCode == SQL_SUCCESS)
			{
				int cn = 1;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].disease_number	= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_CHAR,	 disease_tbl[a].han_name, 31, &cbValue);		
				retCode = SQLGetData(hStmt,  cn++, SQL_C_CHAR,  disease_tbl[a].name, 31, &cbValue);			
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].lv		= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].nut1_1		= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].nut2_1		= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].nut3_1		= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].probability_1= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].andor_1		= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].checktime_1	= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].contact_char	= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].talk_char	= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].fuck_char	= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].contact_obj	= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].eat_obj		= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].nut1_2		= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].nut2_2		= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].nut3_2		= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].probability_2= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].checktime_2	= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].andor_2		= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].dec_str		= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].dec_dex		= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].dec_con		= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].dec_wis		= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].dec_int		= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].dec_movp		= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].dec_char		= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].dec_endu		= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].dec_mor		= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].dec_luck		= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].dec_ws		= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].dec_ps		= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].recoverable	= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].life			= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].health		= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].mp			= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].dp			= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].max_			= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].damage		= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].att_rate		= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].depen_rate	= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].ac			= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].mad			= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].poison		= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].stony		= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].sluggish		= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].active		= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].paralysis	= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].active_time	= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].maintain_time	= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].natural_recovery	= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].nut1_3			= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].nut2_3			= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].nut3_3			= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].probability_3	= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].checktime_3		= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].andor_3			= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].price_heal		= (short )t;
				retCode = SQLGetData(hStmt,  cn++, SQL_C_ULONG, &t, 0, &cbValue);	disease_tbl[a].price_prevent	= (short )t;
				
				
				SQLFreeStmt(hStmt, SQL_DROP);
				
				//				 return(1);
			}
			else
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return(-2);
			}
		}
		else
		{
			SQLFreeStmt(hStmt, SQL_DROP);
			return(-1);
		}
	}
	
	FILE *fp;
	
	fp = fopen( "./Output/disease_tbl.bin", "wb" );
	if( fp )
	{
		fwrite( disease_tbl, 6, sizeof(DiseaseTable), fp );
		fclose(fp);
	}
	
	return 1;
}

int UpdateCharID_SQL( int char_id )
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	char		szQuerry[ 128];
	
	sprintf_s(szQuerry, sizeof(szQuerry), "update CharIDCount set CharIDCount=%d", char_id);
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return(1);
	}
	else 
	{
		//SQLTransact(SQL_NULL_HENV, hDBC, SQL_ROLLBACK);
		SQLFreeStmt(hStmt, SQL_DROP);
		return(-2);
	}
};



///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

// BBS관리 소스...

//#define MAX_DATA  5000
//	char enter[ MAX_DATA];
//	FILE *fp = fopen( "t.txt", "rt" );
//	DWORD filelen = filelength( fileno(fp));
//	fread(	enter, filelen, 1, fp );
//	fclose(fp);

int InsertBBS( char *name, char *title, char *contents )
{
	HSTMT hStmt = NULL;
	RETCODE retCode;
	char szQuerry[128];
	
	sprintf_s(szQuerry, sizeof(szQuerry),	"insert into bbs ( date, name, title ) "
		"values ('%d/%d/%d %d:%d:%d', '%s', '%s' )", 
		g_mon+1, g_day, g_year - 2000, g_hour, g_min, g_sec, name, title );
	
	// 캐릭터의 일반 Data를 chr_info에 생성하고 ( 1-1단계 )
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{
		//SQLTransact(SQL_NULL_HENV, hDBC, SQL_COMMIT);
		SQLFreeStmt(hStmt, SQL_DROP);
	}
	else
	{
		//SQLTransact(SQL_NULL_HENV, hDBC, SQL_ROLLBACK);
		SQLFreeStmt(hStmt, SQL_DROP);
		return(0); // unknown error
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	SQLINTEGER  cbdata = 200, cbvalue = SQL_DATA_AT_EXEC ;
	SQLINTEGER	cbcontentslen;
	SQLRETURN	retcode;
	SQLHSTMT	hstmt;
	SQLPOINTER	pParamData;
	int			n=0;
	char		aaa=20;
	DWORD		filelen;
	
	filelen = strlen( contents );
	
	sprintf_s(szQuerry, sizeof(szQuerry), "UPDATE bbs SET con=? WHERE date='%d/%d/%d %d:%d:%d'", g_mon+1, g_day, g_year - 2000, g_hour, g_min, g_sec );
	
	//printf("\nSQL: %s", query);
	SQLAllocStmt(hDBC, &hstmt);
	retcode = SQLPrepare(hstmt, (UCHAR *)szQuerry, SQL_NTS);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) 
	{
		retcode= SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR,SQL_LONGVARCHAR, filelen,	0, contents,			0, &cbcontentslen); 
		
		if(retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
		{
			SQLFreeStmt(hstmt, SQL_DROP);
			printf("\nBinding FAIL!!!") ;
			return -5 ;
		}
		
		cbcontentslen		= SQL_LEN_DATA_AT_EXEC(0);
		retcode = SQLExecute(hstmt) ;
		
		while(retcode == SQL_NEED_DATA) 
		{      
			retcode = SQLParamData(hstmt, &pParamData);
			if(retcode == SQL_NEED_DATA) 
			{
				if(pParamData == contents ) 	SQLPutData(hstmt, contents, filelen );
			}
			else break ;
		}
		
		retcode = SQLExecute(hstmt) ;
		
		if(retcode == SQL_NEED_DATA)
		{
			retcode= SQLCancel(hstmt) ;// 020620 YGI
			retcode= SQLFreeStmt(hstmt, SQL_RESET_PARAMS);
			//retcode= SQLTransact(SQL_NULL_HENV, hDBC, SQL_COMMIT);
			if(retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
				MyLog(LOG_NORMAL, "Transact commit Error") ;
			}
			//printf("\nTransact & free") ;
			return(1); // succeed
		}
		else
		{
			displaySQLError(hstmt) ;
			//SQLTransact(SQL_NULL_HENV, hDBC, SQL_ROLLBACK);
			SQLFreeStmt(hstmt, SQL_DROP);
			return(-3); 
		}
	}	
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	return 0;
}		


// 0305 YGI
int GetTitleBBS( int count, int ct[10], TIMESTAMP_STRUCT date[10], char name[10][ FILENAME_MAX], char title[10][ FILENAME_MAX] )
{		
	SDWORD		cbValue;
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	char		szQuerry[128];
	DWORD		bbs_count=0;		// 공지사항의 총 개수
	int max=10;		
	
	
	sprintf_s( szQuerry, sizeof(szQuerry), "SELECT no FROM bbs ORDER BY no DESC" );
	SQLAllocStmt(hDBC, &hStmt);
	
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{	
		retCode = SQLFetch( hStmt );
		retCode = SQLGetData(hStmt, 1, SQL_C_ULONG,  &bbs_count, 0, &cbValue);
		SQLFreeStmt(hStmt, SQL_DROP);
		
		bbs_count -= count*10;
		if( bbs_count > 0 && bbs_count < 10 )
		{
			max = bbs_count;
		}
		else if( bbs_count < 0 ) return 0;
		
		sprintf_s( szQuerry, sizeof(szQuerry), "select no, name, title, date from bbs order by no DESC" );
		SQLAllocStmt(hDBC, &hStmt);
		retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
		if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
		{	
			for( int i = 0 ; i < max ; i ++)
			{
				retCode = SQLFetch( hStmt );
				retCode = SQLGetData(hStmt, 1, SQL_C_ULONG,		&ct[i],	0, &cbValue);
				retCode = SQLGetData(hStmt, 2, SQL_C_CHAR,		&name[i],  FILENAME_MAX, &cbValue);
				retCode = SQLGetData(hStmt, 3, SQL_C_CHAR,		&title[i], FILENAME_MAX, &cbValue);
				retCode = SQLGetData(hStmt, 4, SQL_C_TIMESTAMP, &date[i],  sizeof( TIMESTAMP_STRUCT ), &cbValue);
			}
		}
		
		if(retCode == SQL_SUCCESS)
		{
			SQLFreeStmt(hStmt, SQL_DROP);
			return ( max );			// 가져온 데이터의 개수를 리턴해준다.
		}
		else 
		{
			SQLFreeStmt(hStmt, SQL_DROP);		// 0308 YGI
			return( 0 );
		}
	}	
	SQLFreeStmt(hStmt, SQL_DROP);		// 0308 YGI
	return( 0 );
}		



int GetContentsBBS( char *date, char *con )
{
	SDWORD		cbValue;
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	char		szQuerry[128];
	
	sprintf_s( szQuerry, sizeof(szQuerry), "select con from bbs where date = '%s'", date );
	
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{	
		//if( count == 0 )	
		retCode = SQLFetch( hStmt );
		retCode = SQLGetData(hStmt, 1, SQL_C_CHAR,		con, 0, &cbValue);
		
		if(retCode == SQL_SUCCESS)
		{
			SQLFreeStmt(hStmt, SQL_DROP);
			return (1);
		}
		else 
		{
			SQLFreeStmt(hStmt, SQL_DROP);		// 0308 YGI
			return( 0 );
		}
	}
	SQLFreeStmt(hStmt, SQL_DROP);		// 0308 YGI
	return 1;
}	







SkillMain SkillTbl[ MAX_SKILLMAIN];

int InitSkillTable( void )
{
	if( MAX_SKILLMAIN <= 0) return -4; 
	
	HSTMT hStmt= NULL ;
	RETCODE ret ;
	char query_stmt[80]= {0, } ;
	SDWORD cbValue ;
	int c;
	
	SQLAllocStmt(hDBC, &hStmt);
	
	strcpy_s(query_stmt, sizeof(query_stmt), "select * from SkillMain") ;
	ret= SQLExecDirect(hStmt, (UCHAR *)query_stmt, SQL_NTS) ;
	
	if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) {
		MyLog( LOG_NORMAL, "SkillMain: ExecDirect Error ") ;
		SQLFreeStmt(hStmt, SQL_DROP);
		return -1 ;
	}
	
	//SQLNumResultCols(hStmt, &nCols) ;
	ret= SQLFetch(hStmt) ;
	if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) 
	{
		MyLog( LOG_NORMAL, "SkillMain:  Fetch Error ") ;
		return -1 ;
	}
	
	int tno;
	
	c = 1;
	while(c < MAX_SKILLMAIN && ret == SQL_SUCCESS)
	{
		ret = SQLGetData(hStmt,  1, SQL_C_LONG,	&tno,			 0, &cbValue);
		SkillTbl[tno].NO = tno;
		ret = SQLGetData(hStmt,  2, SQL_C_CHAR,		SkillTbl[tno].Skill_Minute,		30, &cbValue);
		EatRearWhiteChar( SkillTbl[tno].Skill_Minute );
		ret = SQLGetData(hStmt,  3, SQL_C_LONG,	&SkillTbl[tno].Class_Warrior,	0, &cbValue);
		ret = SQLGetData(hStmt,  4, SQL_C_LONG,	&SkillTbl[tno].Class_Thief	,	0, &cbValue);
		ret = SQLGetData(hStmt,  5, SQL_C_LONG,	&SkillTbl[tno].Class_Archery,	0, &cbValue);
		ret = SQLGetData(hStmt,  6, SQL_C_LONG,	&SkillTbl[tno].Class_Wizard	,	0, &cbValue);
		ret = SQLGetData(hStmt,  7, SQL_C_LONG,	&SkillTbl[tno].Class_Priest	,	0, &cbValue);
		ret = SQLGetData(hStmt,  8, SQL_C_LONG,	&SkillTbl[tno].Select_Farmer,			 0, &cbValue);
		ret = SQLGetData(hStmt,  9, SQL_C_LONG,	&SkillTbl[tno].Select_miner	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 10, SQL_C_LONG,	&SkillTbl[tno].Select_fisher,			 0, &cbValue);
		ret = SQLGetData(hStmt, 11, SQL_C_LONG,	&SkillTbl[tno].Select_Lumberjack,		 0, &cbValue);
		ret = SQLGetData(hStmt, 12, SQL_C_LONG,	&SkillTbl[tno].Select_Butcher,			 0, &cbValue);
		ret = SQLGetData(hStmt, 13, SQL_C_LONG,	&SkillTbl[tno].Select_Herbdigger,		 0, &cbValue);
		ret = SQLGetData(hStmt, 14, SQL_C_LONG,	&SkillTbl[tno].Select_herdman,			 0, &cbValue);
		ret = SQLGetData(hStmt, 15, SQL_C_LONG,	&SkillTbl[tno].Select_Carpenter	,		 0, &cbValue);
		ret = SQLGetData(hStmt, 16, SQL_C_LONG,	&SkillTbl[tno].Select_Blacksmith,		 0, &cbValue);
		ret = SQLGetData(hStmt, 17, SQL_C_LONG,	&SkillTbl[tno].Select_Cooking,			 0, &cbValue);
		ret = SQLGetData(hStmt, 18, SQL_C_LONG,	&SkillTbl[tno].Select_Tailoring	,		 0, &cbValue);
		ret = SQLGetData(hStmt, 19, SQL_C_LONG,	&SkillTbl[tno].Select_Bowcraft_Fletcher, 0, &cbValue);
		ret = SQLGetData(hStmt, 20, SQL_C_LONG,	&SkillTbl[tno].Select_Alchemy	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 21, SQL_C_LONG,	&SkillTbl[tno].Select_Candlemaker	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 22, SQL_C_LONG,	&SkillTbl[tno].Select_Merchant	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 23, SQL_C_LONG,	&SkillTbl[tno].Howto	,			 0, &cbValue);   
		ret = SQLGetData(hStmt, 24, SQL_C_LONG,	&SkillTbl[tno].mouse_change	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 25, SQL_C_LONG,	&SkillTbl[tno].Interface1	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 26, SQL_C_LONG,	&SkillTbl[tno].Interface2	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 27, SQL_C_LONG,	&SkillTbl[tno].Success_interface	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 28, SQL_C_LONG,	&SkillTbl[tno].Fail_Interface	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 29, SQL_C_LONG,	&SkillTbl[tno].Work_target_self	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 30, SQL_C_LONG,	&SkillTbl[tno].Work_target_othercharacter,			 0, &cbValue);
		ret = SQLGetData(hStmt, 31, SQL_C_LONG,	&SkillTbl[tno].Work_target_object	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 32, SQL_C_LONG,	&SkillTbl[tno].Work_target_Item	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 33, SQL_C_LONG,	&SkillTbl[tno].Work_target_earth	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 34, SQL_C_LONG,	&SkillTbl[tno].Work_target_water	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 35, SQL_C_LONG,	&SkillTbl[tno].Work_target_forest	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 36, SQL_C_LONG,	&SkillTbl[tno].Work_target_interface	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 37, SQL_C_LONG,	&SkillTbl[tno].Tool	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 38, SQL_C_LONG,	&SkillTbl[tno].Object_Workshop	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 39, SQL_C_LONG,	&SkillTbl[tno].Object_target	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 40, SQL_C_LONG,	&SkillTbl[tno].Obj_Attribute	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 41, SQL_C_LONG,	&SkillTbl[tno].Resource1	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 42, SQL_C_LONG,	&SkillTbl[tno].Resource1_quantity	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 43, SQL_C_LONG,	&SkillTbl[tno].Resourece2	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 44, SQL_C_LONG,	&SkillTbl[tno].Resource2_quantity	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 45, SQL_C_LONG,	&SkillTbl[tno].Resource3	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 46, SQL_C_LONG,	&SkillTbl[tno].Resource3_quantity	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 47, SQL_C_LONG,	&SkillTbl[tno].Produce_type	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 48, SQL_C_LONG,	&SkillTbl[tno].Produce_Fix	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 49, SQL_C_LONG,	&SkillTbl[tno].Ability_Fix	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 50, SQL_C_LONG,	&SkillTbl[tno].Time_judge	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 51, SQL_C_LONG,	&SkillTbl[tno].Time_Maintain	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 52, SQL_C_LONG,	&SkillTbl[tno].Ability_min	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 53, SQL_C_LONG,	&SkillTbl[tno].Ability_max	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 54, SQL_C_LONG,	&SkillTbl[tno].Plus_element	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 55, SQL_C_LONG,	&SkillTbl[tno].Plus_type	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 56, SQL_C_LONG,	&SkillTbl[tno].Plus_Rate	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 57, SQL_C_LONG,	&SkillTbl[tno].Plus_min	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 58, SQL_C_LONG,	&SkillTbl[tno].SP_demand	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 59, SQL_C_LONG,	&SkillTbl[tno].Success	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 60, SQL_C_LONG,	&SkillTbl[tno].effectCharobj_status	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 61, SQL_C_LONG,	&SkillTbl[tno].Change_enermy_status	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 62, SQL_C_LONG,	&SkillTbl[tno].Change_enermy_status2	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 63, SQL_C_LONG,	&SkillTbl[tno].charvisible	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 64, SQL_C_LONG,	&SkillTbl[tno].Effectobj_status	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 65, SQL_C_LONG,	&SkillTbl[tno].Changeobj_attribute	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 66, SQL_C_LONG,	&SkillTbl[tno].changeobj_visible	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 67, SQL_C_LONG,	&SkillTbl[tno].Skill2job	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 68, SQL_C_LONG,	&SkillTbl[tno].effectChar_hide	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 69, SQL_C_LONG,	&SkillTbl[tno].Increase_mov	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 70, SQL_C_LONG,	&SkillTbl[tno].Increse_Life	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 71, SQL_C_LONG,	&SkillTbl[tno].Increse_mana	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 72, SQL_C_LONG,	&SkillTbl[tno].Increse_health	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 73, SQL_C_LONG,	&SkillTbl[tno].Change_mor	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 74, SQL_C_LONG,	&SkillTbl[tno].Obj_attribute1	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 75, SQL_C_LONG,	&SkillTbl[tno].ChangeObj_attribute1	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 76, SQL_C_LONG,	&SkillTbl[tno].Display_LV	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 77, SQL_C_LONG,	&SkillTbl[tno].DisPlay_Wis	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 78, SQL_C_LONG,	&SkillTbl[tno].Display_Int	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 79, SQL_C_LONG,	&SkillTbl[tno].DisPlay_Life	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 80, SQL_C_LONG,	&SkillTbl[tno].DisPlay_Health	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 81, SQL_C_LONG,	&SkillTbl[tno].DisPlay_Damage	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 82, SQL_C_LONG,	&SkillTbl[tno].DisPlay_AC	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 83, SQL_C_LONG,	&SkillTbl[tno].DisPlay_inventory	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 84, SQL_C_LONG,	&SkillTbl[tno].Display_Foot	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 85, SQL_C_LONG,	&SkillTbl[tno].DisPlay_Item_Changeall	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 86, SQL_C_LONG,	&SkillTbl[tno].DisPlay_royalty	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 87, SQL_C_LONG,	&SkillTbl[tno].DisPlay_Item_Status	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 88, SQL_C_LONG,	&SkillTbl[tno].DisPlay_name	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 89, SQL_C_LONG,	&SkillTbl[tno].DisPlay_item_Price	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 90, SQL_C_LONG,	&SkillTbl[tno].effect	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 91, SQL_C_LONG,	&SkillTbl[tno].effect_wave	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 92, SQL_C_LONG,	&SkillTbl[tno].effect_race	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 93, SQL_C_LONG,	&SkillTbl[tno].Success_range	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 94, SQL_C_LONG,	&SkillTbl[tno].Damage	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 95, SQL_C_LONG,	&SkillTbl[tno].Status_self	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 96, SQL_C_LONG,	&SkillTbl[tno].Effect1	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 97, SQL_C_LONG,	&SkillTbl[tno].Effect_wave1	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 98, SQL_C_LONG,	&SkillTbl[tno].Enemy_status	,			 0, &cbValue);
		ret = SQLGetData(hStmt, 99, SQL_C_LONG,	&SkillTbl[tno].Range	,			 0, &cbValue);
		ret = SQLGetData(hStmt,100, SQL_C_LONG,	&SkillTbl[tno].Enermy_alram	,			 0, &cbValue);
		ret = SQLGetData(hStmt,101, SQL_C_LONG,	&SkillTbl[tno].map	,			 0, &cbValue);
		ret = SQLGetData(hStmt,102, SQL_C_LONG,	&SkillTbl[tno].Curser1	,			 0, &cbValue);
		ret = SQLGetData(hStmt,103, SQL_C_LONG,	&SkillTbl[tno].Curser2	,			 0, &cbValue);
		ret = SQLGetData(hStmt,104, SQL_C_LONG,	&SkillTbl[tno].Curser3	,			 0, &cbValue);
		ret = SQLGetData(hStmt,105, SQL_C_LONG,	&SkillTbl[tno].point	,			 0, &cbValue);
		ret = SQLGetData(hStmt,106, SQL_C_LONG,	&SkillTbl[tno].inclusive ,			 0, &cbValue);
		ret = SQLGetData(hStmt,107, SQL_C_LONG,	&SkillTbl[tno].money ,				 0, &cbValue);
		ret = SQLGetData(hStmt,108, SQL_C_LONG,	&SkillTbl[tno].need_exp,			 0, &cbValue);
		ret = SQLGetData(hStmt,109, SQL_C_LONG,	&SkillTbl[tno].MotherSkillType,		 0, &cbValue);
		ret = SQLGetData(hStmt,110, SQL_C_LONG,	&SkillTbl[tno].MotherSkillExp,		 0, &cbValue);
		
		// 0619.......
		ret = SQLGetData(hStmt,111, SQL_C_ULONG,	&SkillTbl[tno].k1,		 0, &cbValue);
		ret = SQLGetData(hStmt,112, SQL_C_ULONG,	&SkillTbl[tno].k2,		 0, &cbValue);
		
		if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) { MyLog( LOG_NORMAL, "SkillMain: Error!!! (%d)", ret) ; return -1 ;}
		c++ ;
		ret= SQLFetch(hStmt) ;
	}	
	
	SQLFreeStmt(hStmt, SQL_DROP) ;
	
	FILE *fp;
	int  i;
	char * tt = (char *)SkillTbl;
	char crc = 0;
	for( i = 0 ; i < MAX_SKILLMAIN * sizeof(SkillMain) ; i ++ )
	{	
		*tt += ( 200 + i );
		crc  += ((*tt + *tt ) - i);
		tt ++;
	}	
	
	fp = fopen( "./Output/skilltable.bin", "wb" );
	if( fp )
	{	
		fwrite( &crc, 1,1, fp );
		fwrite( SkillTbl, MAX_SKILLMAIN, sizeof(SkillMain), fp );
		fclose(fp);
	}	
	
	tt = (char *)SkillTbl;
	for( i = 0 ; i < MAX_SKILLMAIN * sizeof(SkillMain) ; i ++ )
	{	
		*tt -= ( 200 + i );
		tt ++;
	}	
	
	MyLog( LOG_NORMAL, "	.'SkillMain'            %4d data Loaded", c) ;
	
	return c ;			
}							






t_SkillMapTable SkillMapTable[ MAX_SKILLMAPTABLE];
int             SkillMapTableCount;

int InitSkillMapTable( void )
{
	HSTMT hStmt= NULL ;
	RETCODE ret ;
	SDWORD cbValue ;
	int c;
	
	SQLAllocStmt(hDBC, &hStmt);
	ret= SQLExecDirect(hStmt, (UCHAR *)"select * from SkillMapTable order by No", SQL_NTS) ;
	
	if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) {
		MyLog( LOG_NORMAL, "'SkillMapTable' : ExecDirect Error ") ;
		SQLFreeStmt(hStmt, SQL_DROP);
		return -1 ;
	}	
	
	//SQLNumResultCols(hStmt, &nCols) ;
	ret= SQLFetch(hStmt) ;
	if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) 
	{
		MyLog( LOG_NORMAL, "'SkillMapTable' : Fetch Error ") ;
		return -1 ;
	}	
	
	int tno;
	
	c = 0;
	while(c < MAX_SKILLMAPTABLE && ret == SQL_SUCCESS)
	{	
		ret = SQLGetData(hStmt,  1, SQL_C_LONG,	&tno							,   0, &cbValue);
		SkillMapTable[tno].NO = tno;
		//< CSD-030324
		// thai2 YGI
		//ret = SQLGetData(hStmt,  2, SQL_C_CHAR,	SkillMapTable[tno].han_name		,  30, &cbValue);
		//EatRearWhiteChar( SkillMapTable[tno].han_name );
		//> CSD-030324
		ret = SQLGetData(hStmt,  3, SQL_C_LONG,	&SkillMapTable[tno].Item_id		,	0, &cbValue);
		ret = SQLGetData(hStmt,  4, SQL_C_LONG,	&SkillMapTable[tno].Skill_type	,	0, &cbValue);
		ret = SQLGetData(hStmt,  5, SQL_C_LONG,	&SkillMapTable[tno].demand		,	0, &cbValue);
		ret = SQLGetData(hStmt,  6, SQL_C_LONG,	&SkillMapTable[tno].table[0]	,	0, &cbValue);
		ret = SQLGetData(hStmt,  7, SQL_C_LONG,	&SkillMapTable[tno].table[1]	,	0, &cbValue);
		ret = SQLGetData(hStmt,  8, SQL_C_LONG,	&SkillMapTable[tno].table[2]	,	0, &cbValue);
		ret = SQLGetData(hStmt,  9, SQL_C_LONG,	&SkillMapTable[tno].table[3]	,	0, &cbValue);
		ret = SQLGetData(hStmt, 10, SQL_C_LONG,	&SkillMapTable[tno].table[4]	,	0, &cbValue);
		ret = SQLGetData(hStmt, 11, SQL_C_LONG,	&SkillMapTable[tno].table[5]	,	0, &cbValue);
		ret = SQLGetData(hStmt, 12, SQL_C_LONG,	&SkillMapTable[tno].table[6]	,	0, &cbValue);
		ret = SQLGetData(hStmt, 13, SQL_C_LONG,	&SkillMapTable[tno].table[7]	,	0, &cbValue);
		ret = SQLGetData(hStmt, 14, SQL_C_LONG,	&SkillMapTable[tno].table[8]	,	0, &cbValue);
		ret = SQLGetData(hStmt, 15, SQL_C_LONG,	&SkillMapTable[tno].table[9]	,	0, &cbValue);
		ret = SQLGetData(hStmt, 16, SQL_C_LONG,	&SkillMapTable[tno].table[10]	,	0, &cbValue);
		
		//011014 lsw >
		ret = SQLGetData(hStmt, 17, SQL_C_LONG,	&SkillMapTable[tno].new_item[0]	,	0, &cbValue);
		ret = SQLGetData(hStmt, 18, SQL_C_LONG,	&SkillMapTable[tno].new_item_pro[0]	,	0, &cbValue);
		ret = SQLGetData(hStmt, 19, SQL_C_LONG,	&SkillMapTable[tno].new_item[1]	,	0, &cbValue);
		ret = SQLGetData(hStmt, 20, SQL_C_LONG,	&SkillMapTable[tno].new_item_pro[1]	,	0, &cbValue);
		ret = SQLGetData(hStmt, 21, SQL_C_LONG,	&SkillMapTable[tno].new_item[2]	,	0, &cbValue);
		ret = SQLGetData(hStmt, 22, SQL_C_LONG,	&SkillMapTable[tno].new_item_pro[2]	,	0, &cbValue);
		//011014 lsw <
		
		
		if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) { MyLog( LOG_NORMAL, "'SkillMapTable' : Error!!! (%d)", ret) ; return -1 ;}
		c++ ;
		ret= SQLFetch(hStmt) ;
	}	
	
	SQLFreeStmt(hStmt, SQL_DROP) ;
	
	MyLog( LOG_NORMAL, "	.SkillMapTable		%4d data Loaded", c) ;
	
	SkillMapTableCount = c;
	
	return c ;
}								


char * GetMessageBBS_public( int count )			// 0306 YGI
{		
	SDWORD		cbValue;
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	char		szQuerry[128];
	int			len;
	char		*msg=NULL;
	
	sprintf_s( szQuerry, sizeof(szQuerry), "SELECT DataLength(con), con FROM bbs WHERE no = %d", count );
	SQLAllocStmt(hDBC, &hStmt);
	
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{	
		retCode = SQLFetch( hStmt );
		retCode = SQLGetData(hStmt, 1, SQL_C_ULONG, &len, 0, &cbValue);
		if( !len ) 
		{
			SQLFreeStmt(hStmt, SQL_DROP);
			return msg;
		}
		msg = new char[len+1];
		memset( msg, 0, len+1 );
		retCode = SQLGetData(hStmt, 2, SQL_C_CHAR, msg, len+1, &cbValue);
		SQLFreeStmt(hStmt, SQL_DROP);
		
		return msg;
	}
	SQLFreeStmt(hStmt, SQL_DROP);		// 0308 YGI
	return msg;
}		






int GetBelieveAndFaith_SQL( int &believe, int &faith, char *name )		// 0405 YGI
{	
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
	char		szQuerry[255];
	
	sprintf_s(szQuerry, sizeof(szQuerry), "select believe_god , faith from chr_info2 where name='%s'", name );		// 0414 YGI
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{	
		retCode = SQLFetch(hStmt);
		
		retCode = SQLGetData(hStmt, 1, SQL_C_ULONG, &believe,	0, &cbValue);
		retCode = SQLGetData(hStmt, 2, SQL_C_ULONG, &faith,		0, &cbValue);
	}		
	else 
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return 0;
	}
	SQLFreeStmt(hStmt, SQL_DROP);
	return 1;
}

int UpdateFaith_SQL( short int faith, char *name )		// 0405 YGI
{	
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	char		szQuerry[255];
	
	sprintf_s(szQuerry, sizeof(szQuerry), "Update chr_info2 SET faith = %d where name='%s'", faith, name );
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	SQLFreeStmt(hStmt, SQL_DROP);
	
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) return 1;
	else return 0;
}

int UpdateEvaName( char *my_name, char *eva_name )		// 전도한 사람 이름을 디비에 업데이트 한다. // 0405 YGI
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	char		szQuerry[255];
	
	sprintf_s(szQuerry, sizeof(szQuerry), "Update chr_info SET evangelist = '%s' where name='%s'", eva_name, my_name );
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	SQLFreeStmt(hStmt, SQL_DROP);
	
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) return 1;
	else return 0;
}

int GetEvangelist( char *my_name, char *eva_name )		// 나를 전도한 사람 이름 가져오기
{
	SDWORD		cbValue;
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	char		szQuerry[128];
	
	sprintf_s( szQuerry, sizeof(szQuerry), "select evangelist from chr_info where name = '%s'", my_name);
	
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{	
		retCode = SQLFetch( hStmt );
		retCode = SQLGetData(hStmt, 1, SQL_C_CHAR,	eva_name, 0, &cbValue);
		
		if(retCode == SQL_SUCCESS)
		{
			SQLFreeStmt(hStmt, SQL_DROP);
			return (1);
		}
		else 
		{
			SQLFreeStmt(hStmt, SQL_DROP);		// 0308 YGI
			return( 0 );
		}
	}
	SQLFreeStmt(hStmt, SQL_DROP);		// 0308 YGI
	return 1;
}

void SubtractFaith( char *name )		// 삭제를 할 경우 자기를 전도한 사람의 신앙심을 깍는다.
{
	char eva_name[20] = {0, };
	if( GetEvangelist( name, eva_name ) )
	{
		if( eva_name[0] )
		{
			int believe, faith;
			GetBelieveAndFaith_SQL( believe, faith, eva_name );
			faith -= 10;
			if( faith < 0 ) faith = 0;
			UpdateFaith_SQL( (short int )faith, eva_name );
		}
	}
}


//// 0405 YGI
int GetGodFood_SQL( int &god_food, DWORD &god_food_date, char *name )
{
	SDWORD		cbValue;
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	char		szQuerry[128];
	char		date[10];
	
	sprintf_s( szQuerry, sizeof(szQuerry), "SELECT god_food, god_food_date FROM chr_info2 WHERE name = '%s'", name);
	
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{	
		retCode = SQLFetch( hStmt );
		retCode = SQLGetData(hStmt, 1, SQL_C_ULONG,	&god_food, 0, &cbValue);
		retCode = SQLGetData(hStmt, 2, SQL_C_CHAR,	date, 9, &cbValue); god_food_date = (DWORD) atof( date );
		
		if(retCode == SQL_SUCCESS)
		{
			SQLFreeStmt(hStmt, SQL_DROP);
			return (1);
		}
		else 
		{
			SQLFreeStmt(hStmt, SQL_DROP);		
			return( 0 );
		}
	}
	SQLFreeStmt(hStmt, SQL_DROP);	
	return 1;
}



char *GodIndex2Name[] = { "EDELBLHOY", "TEFFERY", "LETTY", };	// 0410 YGI		에델브로이, 테페리, 그랑엘베르, 유피넬, 레티, 헬카네스, 화렌차
int GetGodMeetingTime( TIMESTAMP_STRUCT &day, TIMESTAMP_STRUCT &month, TIMESTAMP_STRUCT &year, int god_index )
{		
	SDWORD		cbValue;
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	char		szQuerry[128];
	DWORD		bbs_count=0;
	
	// char *index2name[] = { "EDELBLHOY", };			// 0410 YGI
	
	sprintf_s( szQuerry, sizeof(szQuerry), "SELECT day_time, month_time, year_time FROM god_meeting_time WHERE god_name = '%s'", GodIndex2Name[god_index-1] );
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{	
		retCode = SQLFetch( hStmt );
		
		retCode = SQLGetData(hStmt, 1, SQL_C_TIMESTAMP,	&day,	sizeof( TIMESTAMP_STRUCT ), &cbValue);
		retCode = SQLGetData(hStmt, 2, SQL_C_TIMESTAMP,	&month, sizeof( TIMESTAMP_STRUCT ), &cbValue);
		retCode = SQLGetData(hStmt, 3, SQL_C_TIMESTAMP,	&year,  sizeof( TIMESTAMP_STRUCT ), &cbValue);
	}
	
	if(retCode == SQL_SUCCESS)
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return ( 1 );
	}
	else 
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return( 0 );
	}
}

int GetMeetingMessage( int god_type, int text_type, int text_num, char *msg )
{		
	SDWORD		cbValue;
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	char		szQuerry[128];
	
	char		text_field[20];
	
	switch( text_type )
	{
	case 300/*YEAR_MEETING */:		sprintf_s( text_field, sizeof(text_field), "year_text%d", text_num ); break;
	case 200/*MONTH_MEETING */:		sprintf_s( text_field, sizeof(text_field), "month_text%d", text_num ); break;
	case 100/*DAY_MEETING */:		sprintf_s( text_field, sizeof(text_field), "day_text%d", text_num ); break;
	default : return NULL;
	}
	
	sprintf_s( szQuerry, sizeof(szQuerry), "SELECT %s FROM god_meeting_time WHERE god_name = '%s'", text_field,text_field, GodIndex2Name[god_type-1] );
	SQLAllocStmt(hDBC, &hStmt);
	
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{	
		retCode = SQLFetch( hStmt );
		retCode = SQLGetData(hStmt, 1, SQL_C_CHAR, msg, MAX_GOD_TEXT, &cbValue);
		msg[MAX_GOD_TEXT-1] = 0;
		EatRearWhiteChar( msg );
		SQLFreeStmt(hStmt, SQL_DROP);
		return 1;
	}
	SQLFreeStmt(hStmt, SQL_DROP);
	return 0;
}		

int UpdateCharStatusByKein( t_connection c[], short int cn )		// 0410 YGI
{
	CHARLIST *ch = &c[cn].chrlst;
	if( !ch ) return 0; 
	
	k_char_update_data p;
	
	p.believe_god		= ch->believe_god;
	p.faith				= ch->faith;
	p.god_cast_level	= ch->god_cast_level;
	sprintf_s( p.name, sizeof(p.name), "%s", ch->Name );
	
	return ( UpdateCharStatusByKein( &p ) );
}

int UpdateCharStatusByKein( k_char_update_data *ch )		// 0410 YGI
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	char		szQuerry[255];
	
	
	sprintf_s(szQuerry, sizeof(szQuerry), "UPDATE chr_info2 SET god_cast_level=%d, believe_god=%d, faith=%d WHERE name='%s'", 
		ch->god_cast_level, ch->believe_god, ch->faith, ch->name );
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	SQLFreeStmt(hStmt, SQL_DROP);
	
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) return 1;
	else return 0;
}


static char		szmailQuerry[MAIL_BUF];		// 0507 YGI
int GetCharDataStatusByKein( k_get_char_info *ch, char *name )		// 0410 YGI
{		
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
	DWORD		t;
	
	sprintf_s(szmailQuerry, sizeof(szmailQuerry), 
		"SELECT god_cast_level, believe_god, faith "		// 0414 YGI
		"FROM chr_info2 WHERE name='%s'", name );
	
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szmailQuerry, SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{		
		retCode = SQLFetch(hStmt);
		if(retCode == SQL_SUCCESS)
		{	
			retCode = SQLGetData(hStmt, 1, SQL_C_ULONG, &t, 0, &cbValue);			ch->god_cast_level	= (short int) t;
			retCode = SQLGetData(hStmt, 2, SQL_C_ULONG, &t, 0, &cbValue);			ch->believe_god		= (char) t;
			retCode = SQLGetData(hStmt, 3, SQL_C_ULONG, &t, 0, &cbValue);			ch->faith			= (short int) t;
			
			SQLFreeStmt(hStmt, SQL_DROP);
			return(1);
		}
	}	
	
	SQLFreeStmt(hStmt, SQL_DROP);
	return(0);
}			

struct today
{
	int Year;
	int Mon;
	int Day;
};

typedef struct 
{
	int No;
	int item_id;
	int map_num;
	int sx, sy, ex, ey;
	int NpcNo;
	int time;
	int weather;
	int day;
	int howmuch;
	int howmany;
	int thowmany;
	int howget;
	int max_store;
	//1018 zhh
	int now_store;
	int Date;			//8자리로 받는 부분
	int add_Day;		//아이템 주는 날에 더하는 수
	today now;
}	sGen_SpecialItem;

#define MAX_GEN_SPECIAL_ITEM_		500
sGen_SpecialItem GenSpcItem[ MAX_GEN_SPECIAL_ITEM_];
int MaxGenSpcItem;

int UpdateSpecialItemReleaseNumber(int i)
{	
	int temp=1;
	//	for(int i=0;i<MaxGenSpcItem;i++)
	{
		SQLRETURN retcode;
		SQLHSTMT  hstmt;
		char query[ FILENAME_MAX];
		
		sprintf_s(query, sizeof(query), "UPDATE Special_item SET Now_Store = %d , S_Day = %d where No= '%d'" , GenSpcItem[i].now_store,
			GenSpcItem[i].Date ,i+1);
		
		SQLAllocStmt(hDBC, &hstmt) ;
		retcode = SQLExecDirect(hstmt, (UCHAR *)query, SQL_NTS);
		if(retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
			displaySQLError(hstmt) ;
			//SQLTransact(SQL_NULL_HENV, hDBC, SQL_ROLLBACK);
			SQLFreeStmt(hstmt, SQL_DROP);			
			temp=-1;
		}	
		else {
			//SQLTransact(SQL_NULL_HENV, hDBC, SQL_COMMIT);
			SQLFreeStmt(hstmt, SQL_DROP) ;			
		}
	}
	return temp;
}	

int SendUpdateSpecialItemReleaseNumber( int i );
// Marking by chan78
void UpdateSpecialItem(  int i )
{	
	//	if( connections[DB_DEMON].dwAgentConnectionIndex == 0 ) // LoginServer와 연결이 끊어진 상태라면..	
	//		UpdateSpecialItemReleaseNumber(i);		
	//	else 
	SendUpdateSpecialItemReleaseNumber( i );
}	

int SendUpdateSpecialItemReleaseNumber( int i )
{	
	{
		t_packet p;
		p.h.header.type = CMD_SPECIAL_ITEM;
		{
			// YGI acer
			p.u.server_special_item.No = GenSpcItem[i].No;
			p.u.server_special_item.Now_Store = GenSpcItem[i].now_store;
			p.u.server_special_item.S_Day = GenSpcItem[i].Date;
		}
		p.h.header.size = sizeof( t_server_special_item );
		QueuePacket(connections, DB_DEMON, &p, 1);		
	}
	return 0;
}	



//1027 zhh
int SaveGiveSpecialItem(int NPCno,int dx,int dy,int MapNo,int How,int i)
{	
	FILE *fp;
	char name[100];
	wsprintf(name,"c:\\스페셜아이템로그%d.txt",MapNo);
	fp = fopen(name,"at+");
	if(fp==NULL)
		return 0;
	
	
	
	
	
	fprintf(fp,"%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t",		NPCno, dx, dy, MapNo, How,		g_year,g_mon+1,g_day,g_hour);
	fprintf(fp,"%d\t%d\t%d\t%d\t%d\t%d\t",i,GenSpcItem[i].NpcNo,i,GenSpcItem[i].now_store,i,		GenSpcItem[i].max_store);
	fprintf(fp,"%d\t%d\t%d\t%d\t%d\t%d\t",i,GenSpcItem[i].map_num,i,GenSpcItem[i].time,i,GenSpcItem[i].day);
	fprintf(fp,"%d\t%d\t%d\t%d\t%d%d\n",i,GenSpcItem[i].now.Year,i,GenSpcItem[i].now.Mon,		i,GenSpcItem[i].now.Day);
	
	/*
	fprintf(fp,"SprNo:%d x:%d y:%d MapNo:%d How:%d g_year:%d g_mon+1:%d g_day:%d g_hour:%d\n",
	NPCno, dx, dy, MapNo, How,
	g_year,g_mon+1,g_day,g_hour);
	
	  fprintf(fp,"[%d].NpcNo:%d [%d].now_store:%d [%d].max_store:%d ",i,GenSpcItem[i].NpcNo,i,GenSpcItem[i].now_store,i,
	  GenSpcItem[i].max_store);
	  fprintf(fp,"[%d].map_num:%d [%d].time:%d [%d].day:%d ",i,GenSpcItem[i].map_num,i,GenSpcItem[i].time,i,GenSpcItem[i].day);
	  fprintf(fp,"[%d].now.Year:%d [%d].now.Mon:%d [%d].now.Day:%d \n",i,GenSpcItem[i].now.Year,i,GenSpcItem[i].now.Mon,
	  i,GenSpcItem[i].now.Day);
	*/
	
	fclose(fp);
	
	return 1;
	
}

//2001/09/19 zhh
int EndDay[13]={ 0 , 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
void ChangeSpecialItemDate(int i)
{
	if( GenSpcItem[i].add_Day == 0)
		return;
	
	if( GenSpcItem[i].now_store % GenSpcItem[i].howmuch )
		return;
	
	GenSpcItem[i].now.Year = g_year;
	GenSpcItem[i].now.Mon  = g_mon+1;
	GenSpcItem[i].now.Day  = g_day;
	
	GenSpcItem[i].now.Day = GenSpcItem[i].now.Day + GenSpcItem[i].add_Day;
	if( EndDay[GenSpcItem[i].now.Mon]<GenSpcItem[i].now.Day)
	{
		GenSpcItem[i].now.Day = GenSpcItem[i].now.Day - EndDay[GenSpcItem[i].now.Mon];
		GenSpcItem[i].now.Mon++;
	}
	if(GenSpcItem[i].now.Mon>12)
	{
		GenSpcItem[i].now.Year++;
		GenSpcItem[i].now.Mon=1;
	}					
	GenSpcItem[i].Date  = GenSpcItem[i].now.Year*10000;	// = GenSpcItem[i].Date/10000;
	GenSpcItem[i].Date += GenSpcItem[i].now.Mon*100;	// = (GenSpcItem[i].Date%10000)/100;
	GenSpcItem[i].Date += GenSpcItem[i].now.Day;		// = (GenSpcItem[i].Date%10000)%100;
}

int CheckSpecialItem(int i,int NPCno,int MapNo,int dx,int dy)
{
	if( GenSpcItem[i].howmany==0)		//나누어 줄것이 없다.
		return -1;	
	if( GenSpcItem[i].now_store>=GenSpcItem[i].max_store && GenSpcItem[i].max_store!= -1)	//아무리 많이 나와도 최대값보다 나오지 않는다.
		return -2;
	if( NPCno != GenSpcItem[i].NpcNo ||										//해당하는 Npc가 있다
		( GenSpcItem[i].map_num != MapNo && GenSpcItem[i].map_num!=-1))  	//해당 맵이거나 맵에 상관 없으면
		return -3;
	if( GenSpcItem[i].now_store>GenSpcItem[i].max_store && GenSpcItem[i].max_store!=-1) //아직 나누어 줄 수 있는 아이템이면
		return -4;
	if( (GenSpcItem[i].time/100>g_hour || GenSpcItem[i].time%100<g_hour) && GenSpcItem[i].time!=0 ) //시간이 맞거나 상관없으면)
		return -5;
	if( (GenSpcItem[i].day/100 >g_day  && GenSpcItem[i].day%100 >g_day ) && GenSpcItem[i].day !=0 ) //날짜가 맞거나 상관없으면
		return -6;
	if ( GenSpcItem[i].now.Year > g_year )
		return -7;
	else if( GenSpcItem[i].now.Year == g_year )
	{
		if( GenSpcItem[i].now.Mon>g_mon+1 )
			return -8;
		else if( GenSpcItem[i].now.Mon == g_mon+1 )
		{
			if( GenSpcItem[i].now.Day>g_day )
				return -9;
		}
	}
	//	if(	((GenSpcItem[i].now.Year!=g_year || GenSpcItem[i].now.Mon>g_mon+1 || GenSpcItem[i].now.Day>g_day ) || 
	//		  GenSpcItem[i].now.Year!=0 && GenSpcItem[i].now.Year>=g_year) )
	//		return -7;
	if( ((dx>GenSpcItem[i].ex || dx<GenSpcItem[i].sx || dy>GenSpcItem[i].ey || dy<GenSpcItem[i].sy)
		&& GenSpcItem[i].ex != -1) )									//해당하는 좌표이거나 아니면 좌표에 제한이 없으면
		return -10;
	
	return 1;
}

int CheckSpecialItemRate(int i,int How)
{
	if( rand()%GenSpcItem[i].howmany != 0 )								//해당하는 확률이아니면
		return -1;
	if( How!=GenSpcItem[i].howget && GenSpcItem[i].howget!=0 )		//만약 얻는 방법이 있으면 또는 상관 없으면
		return -2;
	return 1;
}

int MakeSpecialItem(int i,int dx,int dy)
{
	int AddSuccess = 0;
	//	for(int add=0;add<GenSpcItem[i].howmuch;add++)
	//	{
	if(GenSpcItem[i].now_store>=GenSpcItem[i].max_store && GenSpcItem[i].max_store!= -1)
		return 0;
	ItemAttr item;
	item = GenerateItem( GenSpcItem[i].item_id);        //020509 lsw
	if(item.item_no!=0)
	{
		AddItemList( item.item_no, item.attr, 0, dx+(rand()%32)+16, dy+(rand()%32)+16, 0,0,0,0,0,0 );
		GenSpcItem[i].now_store++;
		AddSuccess++;
	}
	//	}
	return AddSuccess;
}
//1026 zhh
//g_year,g_mon,g_day,g_yday,g_hour,g_min,g_sec,
//1018 zhh		새로 만들어진 함수
//How : 0 죽이면  : 그외에 기술을 쓰면 얻을 수 있는 것들은 기술번호
int GiveSpecialItem(int NPCno,int dx,int dy,int MapNo,int How)
{
	int return_value=0;
	
	for(int i=0;i<MaxGenSpcItem;i++)
	{
		// YGI ACER
		if( !GenSpcItem[i].No ) continue;
		if( CheckSpecialItem(i,NPCno,MapNo,dx,dy) < 0)					//해당하는 아이템을 줄수 있는 상황인지 확인
			continue;
		if( CheckSpecialItemRate(i,How) < 0)				//해당하는 아이템을 줄 수 있는 확률계산
			continue;			
		int AddSuccess=MakeSpecialItem(i,dx,dy);				//아이템을 만든 갯수.
		if( AddSuccess ==0)
			continue;
		ChangeSpecialItemDate(i);						//날짜를 바꾸어 준다.
		UpdateSpecialItem( i );							//아이템을 생성했다구 디비에 쓴다.
		SaveGiveSpecialItem(NPCno,dx,dy,MapNo,How,i);	//화일로 로그를 남긴다.
		return_value++;
	}
	return(return_value);
}


// YGI acer
int LoadGenerateSpecialItem( void )
{			
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
	char		szQuerry[256];
	
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT * FROM Special_Item ORDER BY no" );
	
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{		
		int i = 0;
		retCode = SQLFetch(hStmt);
		while( retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
		{
			retCode = SQLGetData(hStmt, 1, SQL_C_LONG, &i, 0, &cbValue); GenSpcItem[i].No = i;
			retCode = SQLGetData(hStmt, 2, SQL_C_LONG, &GenSpcItem[i].item_id,	0, &cbValue); GenSpcItem[i].item_id /= 100;
			retCode = SQLGetData(hStmt, 3, SQL_C_LONG, &GenSpcItem[i].map_num,	0, &cbValue);
			retCode = SQLGetData(hStmt, 4, SQL_C_LONG, &GenSpcItem[i].sx,		0, &cbValue);
			retCode = SQLGetData(hStmt, 5, SQL_C_LONG, &GenSpcItem[i].sy,		0, &cbValue);
			retCode = SQLGetData(hStmt, 6, SQL_C_LONG, &GenSpcItem[i].ex,		0, &cbValue);
			retCode = SQLGetData(hStmt, 7, SQL_C_LONG, &GenSpcItem[i].ey,		0, &cbValue);
			retCode = SQLGetData(hStmt, 8, SQL_C_LONG, &GenSpcItem[i].NpcNo,	0, &cbValue);
			retCode = SQLGetData(hStmt, 9, SQL_C_LONG, &GenSpcItem[i].time,		0, &cbValue);
			retCode = SQLGetData(hStmt,10, SQL_C_LONG, &GenSpcItem[i].weather,	0, &cbValue);
			retCode = SQLGetData(hStmt,11, SQL_C_LONG, &GenSpcItem[i].day,		0, &cbValue);
			retCode = SQLGetData(hStmt,12, SQL_C_LONG, &GenSpcItem[i].howmuch,	0, &cbValue);
			retCode = SQLGetData(hStmt,13, SQL_C_LONG, &GenSpcItem[i].howmany,	0, &cbValue);
			retCode = SQLGetData(hStmt,14, SQL_C_LONG, &GenSpcItem[i].howget,	0, &cbValue);
			retCode = SQLGetData(hStmt,15, SQL_C_LONG, &GenSpcItem[i].max_store,0, &cbValue);
			//1026 zhh
			retCode = SQLGetData(hStmt,16, SQL_C_LONG, &GenSpcItem[i].now_store,0, &cbValue);
			retCode = SQLGetData(hStmt,17, SQL_C_LONG, &GenSpcItem[i].Date,		0, &cbValue);
			retCode = SQLGetData(hStmt,18, SQL_C_LONG, &GenSpcItem[i].add_Day,	0, &cbValue);
			//1026 zhh
			GenSpcItem[i].now.Year = GenSpcItem[i].Date/10000;
			GenSpcItem[i].now.Mon  = (GenSpcItem[i].Date%10000)/100;
			GenSpcItem[i].now.Day  = (GenSpcItem[i].Date%10000)%100;
			
			
			//1018 zhh
			//	GenSpcItem[i].now_store = 0;
			
			GenSpcItem[i].thowmany = 0;
			
			i++;
			
			retCode = SQLFetch(hStmt);
			if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
			{
			}
			else 
			{
				MaxGenSpcItem = i;
				SQLFreeStmt(hStmt, SQL_DROP);
				return(1);
			}
		}	
		
	}		
	
	//1027 zhh
	//	MessageBox(NULL,"Load Special Item Fail!","",NULL);
	SQLFreeStmt(hStmt, SQL_DROP);
	return(-1);
}			






//----------------------------------------------------------------------------------------

int InitMapInfo( t_MapInfo map[] )		// 1004 YGI
{//021113 lsw
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
	char		szQuerry[255];
	DWORD		value;
	
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT * FROM map_info ORDER BY number");
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
				
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{			
		int count = 0;
		retCode = SQLFetch(hStmt);
		while( retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
		{	
			int Col = 1;
			retCode = SQLGetData(hStmt, Col++, SQL_C_LONG, &value,	0, &cbValue); count = value;
			retCode = SQLGetData(hStmt, Col++, SQL_C_CHAR, map[count].mapfile,	30, &cbValue);
			retCode = SQLGetData(hStmt, Col++, SQL_C_CHAR, map[count].mapname,	30, &cbValue);
			retCode = SQLGetData(hStmt, Col++, SQL_C_LONG, &value,	0, &cbValue); map[count].cPosType = value; // CSD-030509
			retCode = SQLGetData(hStmt, Col++, SQL_C_LONG, &value,	0, &cbValue); map[count].nSaveX = value;   // CSD-030509
			retCode = SQLGetData(hStmt, Col++, SQL_C_LONG, &value,	0, &cbValue); map[count].nSaveY = value;   // CSD-030509
			retCode = SQLGetData(hStmt, Col++, SQL_C_LONG, &value,	0, &cbValue); map[count].daylightcontrol= value;
			retCode = SQLGetData(hStmt, Col++, SQL_C_LONG, &value,	0, &cbValue); map[count].rain= value;
			retCode = SQLGetData(hStmt, Col++, SQL_C_LONG, &value,	0, &cbValue); map[count].nation= value;
			retCode = SQLGetData(hStmt, Col++, SQL_C_LONG, &value,	0, &cbValue); map[count].forrookie = value;
			retCode = SQLGetData(hStmt, Col++, SQL_C_LONG, &value,	0, &cbValue); map[count].port      = value;
			retCode = SQLGetData(hStmt, Col++, SQL_C_LONG, &value,	0, &cbValue); map[count].ghostguide3x = value;
			retCode = SQLGetData(hStmt, Col++, SQL_C_LONG, &value,	0, &cbValue); map[count].ghostguide3y = value;
			retCode = SQLGetData(hStmt, Col++, SQL_C_LONG, &value,	0, &cbValue); map[count].ghostguide4x = value;
			retCode = SQLGetData(hStmt, Col++, SQL_C_LONG, &value,	0, &cbValue); map[count].ghostguide4y = value;
			retCode = SQLGetData(hStmt, Col++, SQL_C_LONG, &value,	0, &cbValue); map[count].group = value;
			retCode = SQLGetData(hStmt, Col++, SQL_C_LONG, &value,	0, &cbValue); map[count].Class = value;
			retCode = SQLGetData(hStmt, Col++, SQL_C_LONG, &value,	0, &cbValue); map[count].LvMin = value;//030116 lsw 레벨 제한 추가
			retCode = SQLGetData(hStmt, Col++, SQL_C_LONG, &value,	0, &cbValue); map[count].LvMax = value;
			retCode = SQLGetData(hStmt, Col++, SQL_C_LONG, &value,	0, &cbValue); map[count].nDisableMagicType = value; // CSD-030516
			
			EatRearWhiteChar(map[count].mapfile);
			_strupr_s( map[count].mapfile, sizeof(map[count].mapfile) );			// 1027 YGI
			EatRearWhiteChar(map[count].mapname);
			
			if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
			{	
				retCode = SQLFetch(hStmt);
			}	
			else
			{	
				SQLFreeStmt(hStmt, SQL_DROP);
				return 0;
			}
		}
		SQLFreeStmt(hStmt, SQL_DROP);
		
		//Set map number - Thralas
		for(int i = 0; i < count; i++)
		{
			if(map[i].port == g_pServerTable->GetOwnServerData()->wPort)
			{
				MapNumber = i;
				break;
			}
		}

		FILE *fp = fopen( "./output/map_info.bin", "wb" );
		if( fp ) 
		{
			fwrite( map, sizeof( t_MapInfo ), MAX_MAP_, fp );
			fclose( fp );
		}
		return 1;
	}
	else 
	{
		//SQLTransact(SQL_NULL_HENV, hDBC, SQL_ROLLBACK);
		SQLFreeStmt(hStmt, SQL_DROP);
		return 0;
	}
}



///////////////////////////////////////////////////////////////////
//		투표 쿼리문
//

// 특정 테이블에서 특정 필드값(int형) 하나를 가져온다.
int Get_INT_ValueBySQL( const char *table, const char *field, char *condition, int *value, int max )		
{
	SDWORD		cbValue;
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	char		szQuerry[255];
	DWORD		temp_value;
	
	if( !table || !field ) return - 100;
	if( condition )
		sprintf_s(szQuerry, sizeof(szQuerry), "SELECT %s FROM %s", field, table);
	else	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT %s FROM %s WHERE %s", field, table, condition );
	
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{
		int ct = 0;
		while( 1 )
		{
			retCode = SQLFetch(hStmt);
			if( retCode != SQL_SUCCESS ) break;
			
			SQLGetData(hStmt, 1, SQL_C_ULONG, &temp_value, 0, &cbValue); value[ct] = temp_value;
			ct++;
			if( ct >= max  ) break;		// 값이 좀더 있음
		}
		SQLFreeStmt(hStmt, SQL_DROP);
		return(-2);
	}
	SQLFreeStmt(hStmt, SQL_DROP);
	return(-3);
}



////////////////////////////////////////////////////////////
// GM_quest	값 가져오기

GM_QUEST	g_GmMain[MAX_GM_MAIN];
int InitGmQuest( void )
{
	HSTMT hStmt= NULL ;
	RETCODE ret ;
	char query_stmt[80];
	SDWORD cbValue ;
	
	
	SQLAllocStmt(hDBC, &hStmt);
	
	strcpy_s(query_stmt, sizeof(query_stmt), "select * from GMquest") ;
	ret= SQLExecDirect(hStmt, (UCHAR *)query_stmt, SQL_NTS) ;
	
	if( !SQLOK( ret ) ) 
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return -1 ;
	}
	ret= SQLFetch(hStmt);
	if( !SQLOK( ret) ) 
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return -1 ;
	}
	
	int c=0;
	int iPos = 0;
	while( c<MAX_GM_MAIN && ret == SQL_SUCCESS)
	{
		int iTemp = 0;
		ret = SQLGetData(hStmt,  1, SQL_C_LONG,	&iPos,			 0, &cbValue);
		
		GM_QUEST *pTemp = &g_GmMain[iPos];//위치 조심
		
		ret = SQLGetData(hStmt,  2, SQL_C_CHAR,	pTemp->name, 20, &cbValue);
		::EatRearWhiteChar( pTemp->name );
		
		ret = SQLGetData(hStmt,  3, SQL_C_LONG,	&pTemp->skill,			 0, &cbValue);
		ret = SQLGetData(hStmt,  4, SQL_C_LONG,	&pTemp->job_level,		 0, &cbValue);
		ret = SQLGetData(hStmt,  5, SQL_C_LONG,	&pTemp->need_money,	 0, &cbValue);
		
		ret = SQLGetData(hStmt,  6, SQL_C_LONG,	&iTemp,	 0, &cbValue);	pTemp->need_item[0] = iTemp/100;
		ret = SQLGetData(hStmt,  7, SQL_C_LONG,	&pTemp->need_quantity[0], 0, &cbValue);
		ret = SQLGetData(hStmt,  8, SQL_C_LONG,	&iTemp,	 0, &cbValue);	pTemp->need_item[1] = iTemp/100;
		ret = SQLGetData(hStmt,  9, SQL_C_LONG,	&pTemp->need_quantity[1], 0, &cbValue);
		ret = SQLGetData(hStmt, 10, SQL_C_LONG,	&iTemp,	 0, &cbValue);	pTemp->need_item[2] = iTemp/100;
		ret = SQLGetData(hStmt, 11, SQL_C_LONG,	&pTemp->need_quantity[2], 0, &cbValue);
		
		ret = SQLGetData(hStmt, 12, SQL_C_LONG,	&pTemp->need_questnum1, 0, &cbValue);
		ret = SQLGetData(hStmt, 13, SQL_C_LONG,	&pTemp->need_questnum2, 0, &cbValue);
		
		if( !SQLOK(ret) )
		{
			SQLFreeStmt(hStmt, SQL_DROP) ;
			return -1 ;
		}
		c++ ;
		ret= SQLFetch(hStmt);
	}
	
	SQLFreeStmt(hStmt, SQL_DROP);
	
	FILE *fp;
	int  i;
	char *tt = (char *)g_GmMain;
	char crc = 0;
	for( i = 0 ; i < MAX_GM_MAIN* sizeof(GM_QUEST) ; i ++ )
	{
		*tt += ( 200 + i );
		crc += ((*tt + *tt ) - i);
		tt++;
	}	
	
	fp = fopen( "./Output/gmquest.bin", "wb" );
	if( fp )
	{	
		fwrite( &crc, 1,1, fp );
		fwrite( g_GmMain, MAX_GM_MAIN, sizeof(GM_QUEST), fp );
		fclose(fp);
	}
	
	tt = (char *)g_GmMain;
	for( i = 0 ; i < MAX_GM_MAIN * sizeof(GM_QUEST) ; i ++ )
	{	
		*tt -= ( 200 + i );
		tt ++;
	}	
	
	return c ;			
}							
// 010502 YGI
int CAliveMapZoneAll::LoadAliveMapZone()
{
	HSTMT hStmt= NULL ;
	RETCODE ret ;
	char query_stmt[100];
	SDWORD cbValue ;
	
	SQLAllocStmt(hDBC, &hStmt);
	
	strcpy_s(query_stmt, sizeof(query_stmt), "SELECT map_number, x, y, xl, yl, is_all, alive_word_type FROM alive_zone") ;
	ret= SQLExecDirect(hStmt, (UCHAR *)query_stmt, SQL_NTS) ;
	if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) 
	{
		printf("\n alive_zone Init : ExecDirect Error ") ;
		SQLFreeStmt(hStmt, SQL_DROP);
		return -1 ;
	}
	ret= SQLFetch(hStmt) ;
	if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) 
	{
		printf("\n alive_zone init: Fetch Error ");
		return -1 ;
	}
	int ct = 0;
	while( ret == SQL_SUCCESS )
	{
		SQLGetData(hStmt,1, SQL_C_LONG,	&m_Map[ct].map_number,0, &cbValue);
		SQLGetData(hStmt,2, SQL_C_LONG,	&m_Map[ct].x, 0, &cbValue);
		SQLGetData(hStmt,3, SQL_C_LONG,	&m_Map[ct].y, 0, &cbValue);
		SQLGetData(hStmt,4, SQL_C_LONG,	&m_Map[ct].xl, 0, &cbValue);
		SQLGetData(hStmt,5, SQL_C_LONG,	&m_Map[ct].yl, 0, &cbValue);
		SQLGetData(hStmt,6, SQL_C_LONG,	&m_Map[ct].is_all, 0, &cbValue);
		SQLGetData(hStmt,7, SQL_C_LONG,	&m_Map[ct].alive_word_type, 0, &cbValue);
		
		ct++;
		if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) 
		{ 
			SQLFreeStmt(hStmt, SQL_DROP) ;
			printf("\nError!!! (%d)", ret) ; 
			return -1 ;
		}
		ret= SQLFetch(hStmt) ;
	}
	m_Max = ct;
	SQLFreeStmt(hStmt, SQL_DROP) ;
	return 1;
}

int LoadNationInfo( tagNationInfo *NationInfo )
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
	char		szQuerry[512];
	LONGLONG	temp_money = 0;
	
	memset( NationInfo, 0, sizeof( tagNationInfo ) * MAX_NATION );
	
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT nation_code, nation_money, main_map_port, visa_va, visa_zy, visa_ill, "
		" visa_pri_va, visa_pri_zy, visa_pri_ill, "
		" visa_mer_va, visa_mer_zy, visa_mer_ill, "
		" visa_mer_pri_va, visa_mer_pri_zy, visa_mer_pri_ill "
		" FROM nation_info" );
	
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	
	if( !SQLOK(retCode) ) 
	{
		//SQLTransact(SQL_NULL_HENV, hDBC, SQL_ROLLBACK);
		SQLFreeStmt(hStmt, SQL_DROP);
		return( retCode );
	}
	
	retCode = SQLFetch( hStmt );
	while( SQLOK( retCode ) )
	{
		int ct = 0;
		int count = 1;
		retCode = SQLGetData(hStmt, count, SQL_C_ULONG, &ct, 0, &cbValue);count++;
		if( ct > N_YLLSE ) 
		{
			retCode = SQLFetch( hStmt );
			continue;
		}
		
		retCode = SQLGetData(hStmt, count, SQL_C_UBIGINT, &temp_money, 0, &cbValue);count++;
		NationInfo[ct].nation_money = temp_money;
		
		retCode = SQLGetData(hStmt, count, SQL_C_ULONG, &NationInfo[ct].main_map_port	 , 0, &cbValue); count++;
		retCode = SQLGetData(hStmt, count, SQL_C_ULONG, &NationInfo[ct].visa_va			 , 0, &cbValue); count++;
		retCode = SQLGetData(hStmt, count, SQL_C_ULONG, &NationInfo[ct].visa_zy			 , 0, &cbValue); count++;
		retCode = SQLGetData(hStmt, count, SQL_C_ULONG, &NationInfo[ct].visa_ill		 , 0, &cbValue); count++;
		retCode = SQLGetData(hStmt, count, SQL_C_ULONG, &NationInfo[ct].visa_pri_va		 , 0, &cbValue); count++;
		retCode = SQLGetData(hStmt, count, SQL_C_ULONG, &NationInfo[ct].visa_pri_zy		 , 0, &cbValue); count++;
		retCode = SQLGetData(hStmt, count, SQL_C_ULONG, &NationInfo[ct].visa_pri_ill	 , 0, &cbValue); count++;
		retCode = SQLGetData(hStmt, count, SQL_C_ULONG, &NationInfo[ct].visa_mer_va		 , 0, &cbValue); count++;
		retCode = SQLGetData(hStmt, count, SQL_C_ULONG, &NationInfo[ct].visa_mer_zy	     , 0, &cbValue); count++;
		retCode = SQLGetData(hStmt, count, SQL_C_ULONG, &NationInfo[ct].visa_mer_ill	 , 0, &cbValue); count++;
		retCode = SQLGetData(hStmt, count, SQL_C_ULONG, &NationInfo[ct].visa_mer_pri_va  , 0, &cbValue); count++;
		retCode = SQLGetData(hStmt, count, SQL_C_ULONG, &NationInfo[ct].visa_mer_pri_zy  , 0, &cbValue); count++;
		retCode = SQLGetData(hStmt, count, SQL_C_ULONG, &NationInfo[ct].visa_mer_pri_ill , 0, &cbValue); count++;
		
		retCode = SQLFetch( hStmt );
	}
	
	//SQLTransact(SQL_NULL_HENV, hDBC, SQL_ROLLBACK);
	SQLFreeStmt(hStmt, SQL_DROP);
	
	CheckMainMap();
	
	return( 1 );
}

int LoadNationInfoOfMainMap( int nation )		// 010528 YGI
{
	if( nation < 0 || nation > N_YLLSE ) 
	{
		return 0;
	}
	
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
	
	char		szQuerry[1024];
	
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT king_support, friend_va, friend_zy, friend_ill, "
		" item_01, item_02, item_03, item_04, item_05, item_06, item_07, item_08, item_09, item_10, "
		" item_11, item_12, item_13, item_14, item_15, item_16, item_17, item_18, item_19, item_20 "
		" FROM nation_info WHERE nation_code = %d ", nation );
	
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	
	if( !SQLOK(retCode) ) 
	{
		//SQLTransact(SQL_NULL_HENV, hDBC, SQL_ROLLBACK);
		SQLFreeStmt(hStmt, SQL_DROP);
		return( retCode );
	}
	
	retCode = SQLFetch( hStmt );
	if( SQLOK( retCode ) )
	{
		int count = 1;
		retCode = SQLGetData(hStmt, count, SQL_C_ULONG, &NationInfo[nation].king_support, 0, &cbValue);count++;
		retCode = SQLGetData(hStmt, count, SQL_C_ULONG, &NationInfo[nation].friend_va, 0, &cbValue);count++;
		retCode = SQLGetData(hStmt, count, SQL_C_ULONG, &NationInfo[nation].friend_zy, 0, &cbValue);count++;
		retCode = SQLGetData(hStmt, count, SQL_C_ULONG, &NationInfo[nation].friend_ill, 0, &cbValue);count++;
		
		for( int i=0; i<20; i++ )
		{
			retCode = SQLGetData(hStmt, count, SQL_C_ULONG, &NationInfo[nation].nation_item[i], 0, &cbValue);count++;
		}
		
		retCode = SQLFetch( hStmt );
	}
	
	//SQLTransact(SQL_NULL_HENV, hDBC, SQL_ROLLBACK);
	SQLFreeStmt(hStmt, SQL_DROP);
	
	return 1;
}

int LoadGuildListForMapServer( )
{	//< CSD-030326
	char szQuerry[512];
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT code, active, first, mark_num, make, info_num, name, degree0, degree1, degree2, degree3, degree4, nation FROM guild_list ORDER BY code");

	HSTMT hStmt = NULL;
	SQLAllocStmt(hDBC, &hStmt);
	
	RETCODE	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	
	if (!SQLOK(retCode)) 
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return retCode;
	}
	// 최대가능 길드등록수 설정
	retCode = SQLFetch(hStmt);

	SDWORD cbValue;
	
	while (SQLOK(retCode))
	{
		int nGuildCode = 0, nTemp = 0;
		SQLGetData(hStmt, 1, SQL_C_ULONG, &nGuildCode, 0, &cbValue);
		
		CGuildInfo* pGuild = CGuildInfo::CreateGuildInfo();

		if (pGuild == NULL)
		{
			SQLFreeStmt(hStmt, SQL_DROP);
			return 0;
		}

		SQLGetData(hStmt, 2, SQL_C_SLONG, &nTemp, 0, &cbValue); 
		pGuild->active = nTemp;
		SQLGetData(hStmt, 3, SQL_C_SLONG, &nTemp, 0, &cbValue); 
		pGuild->first = nTemp;
		SQLGetData(hStmt, 4, SQL_C_SLONG, &nTemp, 0, &cbValue); 
		pGuild->mark_num = nTemp;
		SQLGetData(hStmt, 5, SQL_C_SLONG, &nTemp, 0, &cbValue); 
		pGuild->make = nTemp;
		SQLGetData(hStmt, 6, SQL_C_SLONG, &nTemp, 0, &cbValue); 
		pGuild->info_num = nTemp;
		SQLGetData(hStmt, 7, SQL_C_CHAR, pGuild->guild_name, 20, &cbValue); 
		EatRearWhiteChar(pGuild->guild_name);
		SQLGetData(hStmt, 8, SQL_C_CHAR, pGuild->guild_degree[0], 20, &cbValue); 
		EatRearWhiteChar(pGuild->guild_degree[0]);
		SQLGetData(hStmt, 9, SQL_C_CHAR, pGuild->guild_degree[1], 20, &cbValue); 
		EatRearWhiteChar(pGuild->guild_degree[1]);
		SQLGetData(hStmt, 10, SQL_C_CHAR, pGuild->guild_degree[2], 20, &cbValue); 
		EatRearWhiteChar(pGuild->guild_degree[2]);
		SQLGetData(hStmt, 11, SQL_C_CHAR, pGuild->guild_degree[3], 20, &cbValue); 
		EatRearWhiteChar(pGuild->guild_degree[3]);
		SQLGetData(hStmt, 12, SQL_C_CHAR, pGuild->guild_degree[4], 20, &cbValue); 
		EatRearWhiteChar(pGuild->guild_degree[4]);
		SQLGetData(hStmt, 13, SQL_C_SLONG, &nTemp, 0, &cbValue); pGuild->nation = nTemp;
		
		if (!g_pGuildManager->AddGuild(nGuildCode, pGuild))
		{
			SQLFreeStmt(hStmt, SQL_DROP);
			return 0;
		}

		retCode = SQLFetch(hStmt);
	}
	
	SQLFreeStmt(hStmt, SQL_DROP);
	return 1;
}	//> CSD-030326

int LoadAbilityLimit()
{
	char szQuerry[512];
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT * FROM ability_limit");
	HSTMT	hStmt = NULL;
	SQLAllocStmt(hDBC, &hStmt);
	RETCODE	retCode = SQLExecDirect(hStmt, (UCHAR*)szQuerry, SQL_NTS);
	
	if (retCode != SQL_SUCCESS_WITH_INFO && retCode != SQL_SUCCESS) 
	{
		MyLog(LOG_NORMAL, "ability_limit : ExecDirect Error");
		SQLFreeStmt(hStmt, SQL_DROP);
		return 0;
	}
	
	while ((retCode = SQLFetch(hStmt)) == SQL_SUCCESS)
	{
		SDWORD cbValue;
		int nClass = 0, nStep = 0;
		SQLGetData(hStmt, 1, SQL_C_ULONG, &nClass, 0, &cbValue);
		SQLGetData(hStmt, 2, SQL_C_ULONG, &nStep, 0, &cbValue);
		
		CAbilityLimit* pLimit = g_mgrLimit.GetCell(nClass, nStep);
		pLimit->SetClass(nClass);
		pLimit->SetStep(nStep);
		
		int nTemp = 0;  
		SQLGetData(hStmt,  3, SQL_C_ULONG, &nTemp, 0, &cbValue); pLimit->SetStr(nTemp);
		SQLGetData(hStmt,  4, SQL_C_ULONG, &nTemp, 0, &cbValue); pLimit->SetDex(nTemp);
		SQLGetData(hStmt,  5, SQL_C_ULONG, &nTemp, 0, &cbValue); pLimit->SetCon(nTemp);
		SQLGetData(hStmt,  6, SQL_C_ULONG, &nTemp, 0, &cbValue); pLimit->SetWis(nTemp);
		SQLGetData(hStmt,  7, SQL_C_ULONG, &nTemp, 0, &cbValue); pLimit->SetInt(nTemp);
		SQLGetData(hStmt,  8, SQL_C_ULONG, &nTemp, 0, &cbValue); pLimit->SetChar(nTemp);
		SQLGetData(hStmt,  9, SQL_C_ULONG, &nTemp, 0, &cbValue); pLimit->SetEndu(nTemp);
		SQLGetData(hStmt, 10, SQL_C_ULONG, &nTemp, 0, &cbValue); pLimit->SetMove(nTemp);
		SQLGetData(hStmt, 11, SQL_C_ULONG, &nTemp, 0, &cbValue); pLimit->SetMor(nTemp);
		SQLGetData(hStmt, 12, SQL_C_ULONG, &nTemp, 0, &cbValue); pLimit->SetLuck(nTemp);
		SQLGetData(hStmt, 13, SQL_C_ULONG, &nTemp, 0, &cbValue); pLimit->SetWs(nTemp);
		SQLGetData(hStmt, 14, SQL_C_ULONG, &nTemp, 0, &cbValue); pLimit->SetPs(nTemp);
		SQLGetData(hStmt, 15, SQL_C_ULONG, &nTemp, 0, &cbValue); pLimit->SetMaxTactics(nTemp);
	}
	
	retCode = SQLFreeStmt(hStmt, SQL_DROP);
	
	if (retCode == SQL_SUCCESS_WITH_INFO || retCode == SQL_SUCCESS)
	{
		g_mgrLimit.Save("./Output/AbilityLimit.bin");
		return 1;
	}
	
	return 0;
}

int LoadAbilityDivide()
{
	char szQuerry[512];
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT * FROM ability_divide");
	HSTMT	hStmt = NULL;
	SQLAllocStmt(hDBC, &hStmt);
	RETCODE	retCode = SQLExecDirect(hStmt, (UCHAR*)szQuerry, SQL_NTS);
	
	if (retCode != SQL_SUCCESS_WITH_INFO && retCode != SQL_SUCCESS) 
	{
		MyLog(LOG_NORMAL, "ability_divide : ExecDirect Error");
		SQLFreeStmt(hStmt, SQL_DROP);
		return -1;
	}
	
	while ((retCode = SQLFetch(hStmt)) == SQL_SUCCESS)
	{
		SDWORD cbValue;
		int nBefore = 0, nAfter = 0;
		SQLGetData(hStmt, 1, SQL_C_ULONG, &nBefore, 0, &cbValue);
		SQLGetData(hStmt, 2, SQL_C_ULONG, &nAfter, 0, &cbValue);
		
		CAbilityDivide* pDivide = g_mgrDivide.GetCell(nBefore, nAfter);
		pDivide->SetBeforeClass(nBefore);
		pDivide->SetAfterClass(nAfter);
		
		int nTemp = 0;
		SQLGetData(hStmt,  3, SQL_C_ULONG, &nTemp, 0, &cbValue); pDivide->SetStr(nTemp);
		SQLGetData(hStmt,  4, SQL_C_ULONG, &nTemp, 0, &cbValue); pDivide->SetDex(nTemp);
		SQLGetData(hStmt,  5, SQL_C_ULONG, &nTemp, 0, &cbValue); pDivide->SetCon(nTemp);
		SQLGetData(hStmt,  6, SQL_C_ULONG, &nTemp, 0, &cbValue); pDivide->SetWis(nTemp);
		SQLGetData(hStmt,  7, SQL_C_ULONG, &nTemp, 0, &cbValue); pDivide->SetInt(nTemp);
		SQLGetData(hStmt,  8, SQL_C_ULONG, &nTemp, 0, &cbValue); pDivide->SetChar(nTemp);
		SQLGetData(hStmt,  9, SQL_C_ULONG, &nTemp, 0, &cbValue); pDivide->SetEndu(nTemp);
		SQLGetData(hStmt, 10, SQL_C_ULONG, &nTemp, 0, &cbValue); pDivide->SetMove(nTemp);
		SQLGetData(hStmt, 11, SQL_C_ULONG, &nTemp, 0, &cbValue); pDivide->SetMor(nTemp);
		SQLGetData(hStmt, 12, SQL_C_ULONG, &nTemp, 0, &cbValue); pDivide->SetLuck(nTemp);
		SQLGetData(hStmt, 13, SQL_C_ULONG, &nTemp, 0, &cbValue); pDivide->SetWs(nTemp);
		SQLGetData(hStmt, 14, SQL_C_ULONG, &nTemp, 0, &cbValue); pDivide->SetPs(nTemp);
	}
	
	SQLFreeStmt(hStmt, SQL_DROP);
	return 1;
}

bool LoadDualInfo()
{	//< CSD-030522
	char szQuerry[512];
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT * FROM dual_info ORDER BY step");
	HSTMT	hStmt = NULL;
	SQLAllocStmt(hDBC, &hStmt);
	RETCODE	retCode = SQLExecDirect(hStmt, (UCHAR*)szQuerry, SQL_NTS);
	
	if (retCode != SQL_SUCCESS_WITH_INFO && retCode != SQL_SUCCESS) 
	{
		MyLog(LOG_NORMAL, "dual_info : ExecDirect Error");
		SQLFreeStmt(hStmt, SQL_DROP);
		return false;
	}
	
	SDWORD cbValue;
	SQLINTEGER nTemp = 0;
	
	while ((retCode = SQLFetch(hStmt)) == SQL_SUCCESS)
	{
		CDualDataInfo* pDualData = CDualDataInfo::CreateDaulDataInfo();

		if (pDualData == NULL)
		{
			SQLFreeStmt(hStmt, SQL_DROP);
			return false;
		}

		CDualClassInfo* pDualClass = CDualClassInfo::CreateDualClassInfo();

		if (pDualClass == NULL)
		{
			SQLFreeStmt(hStmt, SQL_DROP);
			return false;
		}

		SQLGetData(hStmt, 2, SQL_C_SLONG, &nTemp, 0, &cbValue);  
		pDualData->SetMaxLevel(nTemp);
		SQLGetData(hStmt, 3, SQL_C_SLONG, &nTemp, 0, &cbValue);  
		pDualData->SetNeedLadder(nTemp);
		SQLGetData(hStmt, 4, SQL_C_SLONG, &nTemp, 0, &cbValue);  
		pDualData->SetNeedFame(nTemp);
		SQLGetData(hStmt, 5, SQL_C_SLONG, &nTemp, 0, &cbValue);  
		pDualData->SetConsumeLadder(nTemp);
		SQLGetData(hStmt, 6, SQL_C_SLONG, &nTemp, 0, &cbValue);
		pDualData->SetConsumeFame(nTemp); // CSD-030603

		g_pDualManager->AddDualData(pDualData);
		g_pDualManager->AddDualClass(pDualClass);
	}
	
	SQLFreeStmt(hStmt, SQL_DROP);
	return true;
}	//> CSD-030522

// 011018 YGI
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	레코드 갯수를 가져온다.
//	첨자 : 테이블 이름( table ), key값이 걸린 필드명( field ) NULL일경우 * 으로 처리, 가지고갈 결과값( RowCount )
//	리턴값 : SQL문을 실행했을때 생긴 결과값( 성공, 실패 )을 리턴해준다.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int GetRowLineOfSQL( const char *table, const char *field, int *RowCount, const char *condition )		
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
	
	if( !table ) return -100;
	if( !field ) return -200;
	
	if( !condition )
	{
		//sprintf(szmailQuerry, "select count( %s ) FROM %s", field, table);								// 1016 YGI	
		/////////////////////////////////////////////////////////////////////////////
		sprintf_s(szmailQuerry, sizeof(szmailQuerry), "EXEC up_get_record_count1 '%s', '%s'", field, table);
		/////////////////////////////////////////////////////////////////////////////
	}
	else 
	{
		sprintf_s(szmailQuerry, sizeof(szmailQuerry), "select count( %s ) FROM %s WHERE %s", field, table, condition );
		/////////////////////////////////////////////////////////////////////////////
		//sprintf(szmailQuerry, "EXEC up_get_record_count2 '%s', '%s', '%s'", field, table, condition);
		/////////////////////////////////////////////////////////////////////////////
	}
	
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szmailQuerry, SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{
		retCode = SQLFetch( hStmt );
		retCode = SQLGetData(hStmt, 1, SQL_C_ULONG, RowCount, 0, &cbValue);
		return(SQLFreeStmt(hStmt, SQL_DROP));
	}
	else 
	{
		//SQLTransact(SQL_NULL_HENV, hDBC, SQL_ROLLBACK);
		SQLFreeStmt(hStmt, SQL_DROP);
		return( retCode );
	}
}

int InitGameItem()
{	//< CSD-030517
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
	char		szQuerry[512];
	
	int size;
	for( int i=0; i< MAX_GAMBLE_ITEM_KIND; i++ )
	{
		for( int j=0; j<MAX_GAMBLE_ITEM_TYPE; j++ )
		{
			sprintf_s(szQuerry, sizeof(szQuerry), "house = %d AND type = %d", i, j );
			size = 0;
			GetRowLineOfSQL( "gamble_item", "house", &size, szQuerry );
			g_GambleItem.SetSize( i, j, size );
		}
	}
	
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT house, type, item_no from gamble_item" );
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	
	if( !SQLOK(retCode) ) 
	{
		//SQLTransact(SQL_NULL_HENV, hDBC, SQL_ROLLBACK);
		SQLFreeStmt(hStmt, SQL_DROP);
		return( retCode );
	}
	
	retCode = SQLFetch( hStmt );
	int house, type, item_no, count;
	int pos[MAX_GAMBLE_ITEM_KIND][MAX_GAMBLE_ITEM_TYPE] = { 0, };
	while( SQLOK( retCode ) )
	{
		count = 1,house = 0,item_no= 0;
		retCode = SQLGetData(hStmt, count++, SQL_C_ULONG, &house, 0, &cbValue);
		retCode = SQLGetData(hStmt, count++, SQL_C_ULONG, &type, 0, &cbValue);
		retCode = SQLGetData(hStmt, count++, SQL_C_ULONG, &item_no, 0, &cbValue);
		g_GambleItem.SetItem( house, type, pos[house][type]++, item_no );
		
		retCode = SQLFetch( hStmt );
	}
	
	SQLFreeStmt(hStmt, SQL_DROP);
	return( 1 );
}	//> CSD-030517

int LoadNationItem( int week )
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
	char		szQuerry[512];
	
	memset( &g_nation_item, 0, sizeof( k_nation_item ) );
	
	int row = 0;
	sprintf_s(szQuerry, sizeof(szQuerry), "map = '%s' AND week = %d", MapName, week );
	GetRowLineOfSQL( "nation_item", "item_no", &row, szQuerry );
	if( row <= 0 ) return 0;
	
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT item_no, money, fame FROM nation_item WHERE map = '%s' AND week = %d", MapName, week );
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	
	if( !SQLOK(retCode) ) 
	{
		//SQLTransact(SQL_NULL_HENV, hDBC, SQL_ROLLBACK);
		SQLFreeStmt(hStmt, SQL_DROP);
		return( retCode );
	}
	
	int today_item = rand()%row;
	int count = 0;
	int item_no, money, fame;
	retCode = SQLFetch( hStmt );
	while( SQLOK( retCode ) )
	{
		item_no = 0; money=0; fame=0;
		retCode = SQLGetData(hStmt, 1, SQL_C_ULONG, &item_no, 0, &cbValue);
		retCode = SQLGetData(hStmt, 2, SQL_C_ULONG, &money, 0, &cbValue);
		retCode = SQLGetData(hStmt, 3, SQL_C_ULONG, &fame, 0, &cbValue);
		if( today_item == count )
		{
			g_nation_item.item_no = item_no;
			g_nation_item.money = money;
			g_nation_item.fame = fame;
		}
		count++;
		retCode = SQLFetch( hStmt );
	}
	
	//SQLTransact(SQL_NULL_HENV, hDBC, SQL_ROLLBACK);
	SQLFreeStmt(hStmt, SQL_DROP);
	return( 1 );
}

int LoadGuildHouse()
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
	char		szQuerry[512];
	TIMESTAMP_STRUCT	temp_time;
	
	g_GuildHouse.clear();
	
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT id, lv, nation, blue, red, map_name, x, y, guild_code, event_no, date "
		"FROM guild_house" );
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if( !SQLOK(retCode) ) 
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return( retCode );
	}
	retCode = SQLFetch( hStmt );
	int id = 0;
	while( SQLOK( retCode ) )
	{
		retCode = SQLGetData(hStmt, 1, SQL_C_ULONG, &id, 0, &cbValue);
		g_GuildHouse.SetMax( id );
		g_GuildHouse.m_aGuildHouse[id].id = id;
		
		retCode = SQLGetData(hStmt, 2, SQL_C_ULONG, &g_GuildHouse.m_aGuildHouse[id].level, 0, &cbValue);
		retCode = SQLGetData(hStmt, 3, SQL_C_ULONG, &g_GuildHouse.m_aGuildHouse[id].nation, 0, &cbValue);
		retCode = SQLGetData(hStmt, 4, SQL_C_ULONG, &g_GuildHouse.m_aGuildHouse[id].blue, 0, &cbValue);
		retCode = SQLGetData(hStmt, 5, SQL_C_ULONG, &g_GuildHouse.m_aGuildHouse[id].red, 0, &cbValue);
		retCode = SQLGetData(hStmt, 6, SQL_C_CHAR, g_GuildHouse.m_aGuildHouse[id].map_name, 20, &cbValue);
		EatRearWhiteChar( g_GuildHouse.m_aGuildHouse[id].map_name );
		CharUpper( g_GuildHouse.m_aGuildHouse[id].map_name );
		retCode = SQLGetData(hStmt, 7, SQL_C_ULONG, &g_GuildHouse.m_aGuildHouse[id].x, 0, &cbValue);
		retCode = SQLGetData(hStmt, 8, SQL_C_ULONG, &g_GuildHouse.m_aGuildHouse[id].y, 0, &cbValue);
		retCode = SQLGetData(hStmt, 9, SQL_C_ULONG, &g_GuildHouse.m_aGuildHouse[id].guild_code, 0, &cbValue);
		retCode = SQLGetData(hStmt, 10, SQL_C_ULONG, &g_GuildHouse.m_aGuildHouse[id].event_no, 0, &cbValue);
		retCode = SQLGetData(hStmt, 11, SQL_C_TIMESTAMP, &temp_time,  sizeof( TIMESTAMP_STRUCT ), &cbValue);
		g_GuildHouse.m_aGuildHouse[id].year = temp_time.year;
		g_GuildHouse.m_aGuildHouse[id].month = temp_time.month;
		g_GuildHouse.m_aGuildHouse[id].day = temp_time.day;
		
		retCode = SQLFetch( hStmt );
	}
	
	//SQLTransact(SQL_NULL_HENV, hDBC, SQL_ROLLBACK);
	SQLFreeStmt(hStmt, SQL_DROP);
	return( 1 );
}

/////////////////////////////////////////////////////////////////////////
// acer 3
extern int GetServerSetNum();
void InitItemLog( int port )
{
	g_item_log.Load( port );
	if( GetOwnPort() == port )
	{
		char qurry[MAX_PATH];
		sprintf_s( qurry, sizeof(qurry), "DELETE log_item_event WHERE date < getdate()-%d", g_item_log.GetDeleteDay() );
		Querry_SQL( qurry, hDBC_ChrLogDB );
	}
}
int SaveItemLog( k_item_log *pData )
{
	if( !hDBC_ChrLogDB ) return -100;		// 연결 되어 있지 않은 상태이다.
	
	//acer7
	static int count = 101;
	if( count > 100 )
	{
		count = 0;
		MyLog( 0, " ItemLog : %s->%s, item_no(%d), type(%d)",pData->name1, pData->name2, pData->item.item_no, pData->type );
	}
	count++;	// 020620 YGI
	
	char szQuerry[1024];
	sprintf_s(szQuerry, sizeof(szQuerry), "INSERT INTO log_item_event VALUES ( %d, %d, %d, %d, %d, %d, %d, "
		"%d, %d, '%s', '%s', %d, %d, getdate() ) ", 
		pData->item.item_no, pData->item.attr[0],
		pData->item.attr[1],pData->item.attr[2],
		pData->item.attr[3],pData->item.attr[4],
		pData->item.attr[5],		pData->port,
		pData->type, pData->name1, pData->name2, pData->lv,
		GetServerSetNum() );
	return SQLOK( Querry_SQL( szQuerry, hDBC_ChrLogDB ) );
}

int Init_ChrLogDB_SQL(LPSTR szDSN, const char* szUID, const char* szUPW )
{
	RETCODE		retCode;
	
	SQLAllocEnv(&hEnv_ChrLogDB);
	SQLAllocConnect(hEnv_ChrLogDB, &hDBC_ChrLogDB);
	
	retCode = SQLConnect(hDBC_ChrLogDB, (UCHAR *)szDSN, SQL_NTS, (UCHAR *)szUID, SQL_NTS, (UCHAR *)szUPW, SQL_NTS);
	
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{
		return(1);
	}
	return(0);
};

int LoadDungeonMapAll()		// 020701 YGI
{	//< CSD-030306

	if (!CheckMapPort(MN_SCENARIO_MAP)) 
	{
		return 0;
	}
	
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
	char		szQuerry[512];
	int			nRow = 0, nTemp = 0, nApplyDay = 0, nStartHour = 0, nEndHour = 0;
	DWORD idIndex = 0;
	
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT id FROM scenario_dungeon ORDER BY id DESC" );
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if (!SQLOK(retCode))	goto error__;
	retCode = SQLFetch(hStmt);
	if (!SQLOK(retCode))	goto error__;
	
	retCode = SQLGetData(hStmt, 1, SQL_C_ULONG, &nRow, 0, &cbValue);
	SQLFreeStmt(hStmt, SQL_DROP);
	
	SAFE_DELETE(g_pDungeonMgr);
	
	g_pDungeonMgr = new CDungeonMgr(nRow+1);
	
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT id, port1, port2 FROM scenario_dungeon");
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if( !SQLOK(retCode) ) 
	{
		SAFE_DELETE_ARRAY(g_pDungeonMgr);
		goto error__;
	}
	
	retCode = SQLFetch( hStmt );
	
	while( SQLOK( retCode ) )
	{
		CDungeonBasic *pData = new CDungeonBasic();
		SQLGetData(hStmt, 1, SQL_C_ULONG, &nTemp, 0, &cbValue);
		pData->SetID(nTemp);
		SQLGetData(hStmt, 2, SQL_C_ULONG, &nTemp, 0, &cbValue);
		pData->SetDungeonMapPort1(nTemp);
		SQLGetData(hStmt, 3, SQL_C_ULONG, &nTemp, 0, &cbValue);
		pData->SetDungeonMapPort2(nTemp);
		
		g_pDungeonMgr->SetData(pData->m_nId, pData);
		retCode = SQLFetch( hStmt );
	}
	
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT idDungeon, idStartUp, apply_day, start_hour, end_hour FROM scenario_dungeon_startup");
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if( !SQLOK(retCode) ) 
	{
		SAFE_DELETE_ARRAY(g_pDungeonMgr);
		goto error__;
	}
	
	retCode = SQLFetch( hStmt );
	
	while( SQLOK( retCode ) )
	{
		SQLGetData(hStmt, 1, SQL_C_ULONG, &nTemp, 0, &cbValue);
		// 시간 정보 설정
		CDungeonBasic *pData = g_pDungeonMgr->GetDungeon(nTemp);
		SQLGetData(hStmt, 2, SQL_C_ULONG, &idIndex, 0, &cbValue);
		SQLGetData(hStmt, 3, SQL_C_ULONG, &nApplyDay, 0, &cbValue);
		SQLGetData(hStmt, 4, SQL_C_ULONG, &nStartHour, 0, &cbValue);
		SQLGetData(hStmt, 5, SQL_C_ULONG, &nEndHour, 0, &cbValue);
		pData->AddStartUpInfo(idIndex, nApplyDay, nStartHour, nEndHour);
		
		retCode = SQLFetch( hStmt );
	}
	
	SQLFreeStmt(hStmt, SQL_DROP);
	g_pDungeonMgr->SendScenarioStatus();
	return( 1 );
	
error__ :
	SQLFreeStmt(hStmt, SQL_DROP);
	return -1;
}	//< CSD-030306


// 020701 YGI
int LoadDungeonMap()
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
	char		szQuerry[512];
	int nRow = 0;
	
	sprintf_s( szQuerry, sizeof(szQuerry), "map_name = '%s'", MapName );
	GetRowLineOfSQL( "scenario_dungeon_map", "*", &nRow, szQuerry );
	if( nRow <= 0 ) return 0;
	
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT layer, trap_count, type, out_map, x, y FROM scenario_dungeon_map WHERE map_name = '%s'", MapName );
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if( !SQLOK(retCode) ) 
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return( retCode );
	}
	
	retCode = SQLFetch( hStmt );
	if( !SQLOK( retCode ) ) return 0;
	
	
	SAFE_DELETE(g_pDungeonMap);
	g_pDungeonMap = new CDungeonMapMgr(nRow);
	
	int temp, ct;
	nRow = 0;
	while( SQLOK( retCode ) )
	{
		ct = 1;
		temp = 0;retCode = SQLGetData(hStmt, ct, SQL_C_ULONG, &temp, 0, &cbValue); ct++; g_pDungeonMap->m_pData[nRow].m_nLayer = temp;
		temp = 0;retCode = SQLGetData(hStmt, ct, SQL_C_ULONG, &temp, 0, &cbValue); ct++; g_pDungeonMap->m_pData[nRow].m_nTrapCount = temp;
		temp = 0;retCode = SQLGetData(hStmt, ct, SQL_C_ULONG, &temp, 0, &cbValue); ct++; g_pDungeonMap->m_pData[nRow].m_nDungeonType = temp;
		temp = 0;retCode = SQLGetData(hStmt, ct, SQL_C_CHAR, g_pDungeonMap->m_pData[nRow].m_szOutMap, 20, &cbValue); ct++;
		EatRearWhiteChar( g_pDungeonMap->m_pData[nRow].m_szOutMap );
		temp = 0;retCode = SQLGetData(hStmt, ct, SQL_C_ULONG, &temp, 0, &cbValue); ct++; g_pDungeonMap->m_pData[nRow].m_nOutX = temp;
		temp = 0;retCode = SQLGetData(hStmt, ct, SQL_C_ULONG, &temp, 0, &cbValue); ct++; g_pDungeonMap->m_pData[nRow].m_nOutY= temp;
		
		nRow++;
		
		retCode = SQLFetch( hStmt );
	}
	SQLFreeStmt(hStmt, SQL_DROP);
	g_pDungeonMap->LoadTrap();
	return( 1 );
}
// 020701 YGI
int LoadDungeonTrap( CDungeonObject *pData, int layer )
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
	char		szQuerry[512];
	
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT * FROM scenario_trap WHERE map_name = '%s' AND layer = %d ", MapName, layer );
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if( !SQLOK(retCode) ) 
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return( retCode );
	}
	
	retCode = SQLFetch( hStmt );
	int nTemp = 0;
	char map_name[20];
	while( SQLOK( retCode ) )
	{
		int ct = 1;
		nTemp = 0; SQLGetData(hStmt, ct++, SQL_C_ULONG, &nTemp, 0, &cbValue); pData->m_Index = nTemp;
		nTemp = 0; SQLGetData(hStmt, ct++, SQL_C_CHAR, map_name, 20, &cbValue);
		nTemp = 0; SQLGetData(hStmt, ct++, SQL_C_ULONG, &nTemp, 0, &cbValue); pData->m_nLayer = nTemp;
		nTemp = 0; SQLGetData(hStmt, ct++, SQL_C_ULONG, &nTemp, 0, &cbValue); pData->m_nCloseObjectImage = nTemp;
		nTemp = 0; SQLGetData(hStmt, ct++, SQL_C_ULONG, &nTemp, 0, &cbValue); pData->m_nActiveObjectImage = nTemp;
		nTemp = 0; SQLGetData(hStmt, ct++, SQL_C_ULONG, &nTemp, 0, &cbValue); pData->m_nActiveObjectType = nTemp;
		nTemp = 0; SQLGetData(hStmt, ct++, SQL_C_ULONG, &nTemp, 0, &cbValue); pData->m_nInputX= nTemp;
		nTemp = 0; SQLGetData(hStmt, ct++, SQL_C_ULONG, &nTemp, 0, &cbValue); pData->m_nInputY = nTemp;
		nTemp = 0; SQLGetData(hStmt, ct++, SQL_C_ULONG, &nTemp, 0, &cbValue); pData->m_nOutputX = nTemp;
		nTemp = 0; SQLGetData(hStmt, ct++, SQL_C_ULONG, &nTemp, 0, &cbValue); pData->m_nOutputY = nTemp;
		nTemp = 0; SQLGetData(hStmt, ct++, SQL_C_ULONG, &nTemp, 0, &cbValue); pData->m_nTime= nTemp*60;
		nTemp = 0; SQLGetData(hStmt, ct++, SQL_C_ULONG, &nTemp, 0, &cbValue); pData->m_nNeedType = nTemp;
		nTemp = 0; SQLGetData(hStmt, ct++, SQL_C_ULONG, &nTemp, 0, &cbValue); pData->m_nExecuteType = nTemp;
		nTemp = 0; SQLGetData(hStmt, ct++, SQL_C_ULONG, &nTemp, 0, &cbValue); pData->m_nActiveItemNo = nTemp;
		SQLGetData(hStmt, ct++, SQL_C_CHAR, map_name, 20, &cbValue);
		EatRearWhiteChar( map_name );
		CharUpper( map_name );
		strcpy_s( pData->m_szOutputMap, sizeof(pData->m_szOutputMap), map_name );
		nTemp = 0; SQLGetData(hStmt, ct++, SQL_C_ULONG, &nTemp, 0, &cbValue); pData->m_eTileType1 = nTemp;
		nTemp = 0; SQLGetData(hStmt, ct++, SQL_C_ULONG, &nTemp, 0, &cbValue); pData->m_nRange1 = nTemp;
		nTemp = 0; SQLGetData(hStmt, ct++, SQL_C_ULONG, &nTemp, 0, &cbValue); pData->m_nEventNo1  = nTemp;
		nTemp = 0; SQLGetData(hStmt, ct++, SQL_C_ULONG, &nTemp, 0, &cbValue); pData->m_eTileType2 = nTemp;
		nTemp = 0; SQLGetData(hStmt, ct++, SQL_C_ULONG, &nTemp, 0, &cbValue); pData->m_nRange2 = nTemp;
		nTemp = 0; SQLGetData(hStmt, ct++, SQL_C_ULONG, &nTemp, 0, &cbValue); pData->m_nEventNo2  = nTemp;
		nTemp = 0; SQLGetData(hStmt, ct++, SQL_C_ULONG, &nTemp, 0, &cbValue); pData->m_nBoss  = nTemp;
		nTemp = 0; SQLGetData(hStmt, ct++, SQL_C_ULONG, &nTemp, 0, &cbValue); pData->m_nBossX  = nTemp;
		nTemp = 0; SQLGetData(hStmt, ct++, SQL_C_ULONG, &nTemp, 0, &cbValue); pData->m_nBossY  = nTemp;
		nTemp = 0; SQLGetData(hStmt, ct++, SQL_C_ULONG, &nTemp, 0, &cbValue); pData->m_nBossItem  = nTemp;
		// 020707 YGI
		nTemp = 0; SQLGetData(hStmt, ct++, SQL_C_ULONG, &nTemp, 0, &cbValue); pData->m_nFollower  = nTemp;
		nTemp = 0; SQLGetData(hStmt, ct++, SQL_C_ULONG, &nTemp, 0, &cbValue); pData->m_nFollowerCount  = nTemp;
		
		pData->Start();
		
		pData++;
		
		retCode = SQLFetch( hStmt );
	}
	
	SQLFreeStmt(hStmt, SQL_DROP);
	return( 1 );
}



// -------------------------------------------------------------------------------------------
// 020701 YGI
int LoadDungeonMapEach( int port, int *temp )
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
	char		szQuerry[512];
	
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT id, continue_time, start_time, wait_time FROM scenario_dungeon WHERE port1 = %d", port );
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if( !SQLOK(retCode) ) 
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return -1;
	}
	
	retCode = SQLFetch( hStmt );
	
	if (SQLOK(retCode))
	{	//< CSD-030723
		memset(temp, 0, sizeof(int)*(MAX_CDUNGEONBASIC_INT)); 

		for(int ct = 0; ct < MAX_CDUNGEONBASIC_INT; ++ct)
		{
			retCode = SQLGetData(hStmt, ct + 1, SQL_C_ULONG, &temp[ct], 0, &cbValue);
		}
	}	//> CSD-030723

	SQLFreeStmt(hStmt, SQL_DROP);
	return 1;
}

int CEventMgr::LoadNpcItem()
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
	char		szQuerry[512];
	
	int row = 0;
	sprintf_s( szQuerry, sizeof(szQuerry), "map = '%s'", MapName );
	GetRowLineOfSQL( "Event_MonsterItem", "*", &row, szQuerry );
	if( row <= 0 ) return 0;
	
	m_eventNpcItem.Set( row );
	
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT event_no, event_start_date, event_end_date, npc_no, [percent], "
		" [max], item_no, item_mutant, item_grade, amount FROM event_monsteritem WHERE map = '%s'", MapName );
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if( !SQLOK(retCode) ) 
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return -1;
	}
	
	
	retCode = SQLFetch( hStmt );
	int index = 0;
	TIMESTAMP_STRUCT	start, end;
	
	CEventNpcItem npc_item;
	
	while( SQLOK( retCode ) )
	{
		int temp = 0;
		int ct = 1;
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); npc_item.m_nEventNo = temp; temp = 0;
		retCode = SQLGetData(hStmt, ct++, SQL_C_TIMESTAMP, &start, sizeof( TIMESTAMP_STRUCT ), &cbValue);
		retCode = SQLGetData(hStmt, ct++, SQL_C_TIMESTAMP, &end, sizeof( TIMESTAMP_STRUCT ), &cbValue);
		npc_item.SetTime( &start, &end );
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); npc_item.m_nNpcNo= temp; temp = 0;
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); npc_item.m_nPercent= temp; temp = 0;
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); npc_item.m_nMax = temp; temp = 0;
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); npc_item.m_nItemNo = temp; temp = 0;
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); npc_item.m_nItemMutant = temp; temp = 0;
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); npc_item.m_nItemGrade = temp; temp = 0;
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); npc_item.m_nAmount = temp; temp = 0;
		
		m_eventNpcItem.Init( index, &npc_item );
		retCode = SQLFetch( hStmt );
		index ++;
	}
	SQLFreeStmt(hStmt, SQL_DROP);
	
	// 이벤트 시간 체크
	m_eventNpcItem.CheckActiveEvent();
	return 1;
}


int CEventMgr::LoadNpcCreate()
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
	char		szQuerry[512];
	
	int row = 0;
	sprintf_s( szQuerry, sizeof(szQuerry), "map = '%s'", MapName );
	GetRowLineOfSQL( "Event_Npc", "*", &row, szQuerry );
	if( row <= 0 ) return 0;
	
	m_eventNpcCreate.Set( row );
	
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT event_no, event_start_date, event_end_date, map_x, map_y, npc_no, "
		" npc_script, npc_many, regen, regen_time FROM event_npc WHERE map = '%s'", MapName );
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if( !SQLOK(retCode) ) 
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return -1;
	}
	
	retCode = SQLFetch( hStmt );
	int index = 0;
	TIMESTAMP_STRUCT	start, end;
	
	CEventNpcCreate npc_create;
	
	while( SQLOK( retCode ) )
	{
		int temp = 0;
		int ct = 1;
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); npc_create.m_nEventNo = temp; temp = 0;
		retCode = SQLGetData(hStmt, ct++, SQL_C_TIMESTAMP, &start, sizeof( TIMESTAMP_STRUCT ), &cbValue);
		retCode = SQLGetData(hStmt, ct++, SQL_C_TIMESTAMP, &end, sizeof( TIMESTAMP_STRUCT ), &cbValue);
		npc_create.SetTime( &start, &end );
		
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); npc_create.m_nMapX= temp; temp = 0;
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); npc_create.m_nMapY= temp; temp = 0;
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); npc_create.m_nNpcNo= temp; temp = 0;
		
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); npc_create.m_nNpcScriptNo= temp; temp = 0;
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); npc_create.m_nNpcMany = temp; temp = 0;
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); npc_create.m_nRegen = temp; temp = 0;
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); npc_create.m_nRegenTime = temp; temp = 0;
		
		m_eventNpcCreate.Init( index, &npc_create );
		
		retCode = SQLFetch( hStmt );
		index ++;
	}
	SQLFreeStmt(hStmt, SQL_DROP);
	
	// 이벤트 시간 체크
	m_eventNpcCreate.CheckActiveEvent();
	return 1;
}

int CEventMgr::LoadEventMap()
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
	char		szQuerry[512];
	
	int row = 0;
	sprintf_s( szQuerry, sizeof(szQuerry), "map = '%s'", MapName );
	GetRowLineOfSQL( "Event_Object", "*", &row, szQuerry );
	if( row <= 0 ) return 0;
	
	m_eventObject.Set( row );
	
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT event_no, event_start_date, event_end_date, object_toi, object_b "
		" FROM event_object WHERE map = '%s'", MapName );
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if( !SQLOK(retCode) )
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return -1;
	}
	
	retCode = SQLFetch( hStmt );
	int index = 0;
	TIMESTAMP_STRUCT	start, end;
	
	
	
	while( SQLOK( retCode ) )
	{
		int temp = 0;
		int ct = 1;
		
		CEventObject *pData = m_eventObject.GetEventPoint( index );
		
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); pData->m_nEventNo = temp; temp = 0;
		retCode = SQLGetData(hStmt, ct++, SQL_C_TIMESTAMP, &start, sizeof( TIMESTAMP_STRUCT ), &cbValue);
		retCode = SQLGetData(hStmt, ct++, SQL_C_TIMESTAMP, &end, sizeof( TIMESTAMP_STRUCT ), &cbValue);
		pData->SetTime( &start, &end );
		
		retCode = SQLGetData(hStmt, ct++, SQL_C_CHAR, pData->m_szObjectTOI, 30, &cbValue);
		::EatRearWhiteChar( pData->m_szObjectTOI );
		retCode = SQLGetData(hStmt, ct++, SQL_C_CHAR, pData->m_szObjectB, 30, &cbValue);
		::EatRearWhiteChar( pData->m_szObjectB );
		
		retCode = SQLFetch( hStmt );
		index ++;
	}
	SQLFreeStmt(hStmt, SQL_DROP);
	
	// 이벤트 시간 체크
	m_eventObject.CheckActiveEvent();
	return 1;
}

int CEventMgr::LoadEventSound()
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
	char		szQuerry[512];
	
	int row = 0;
	sprintf_s( szQuerry, sizeof(szQuerry), "[all] <> 0 OR map = '%s'", MapName );
	GetRowLineOfSQL( "Event_Sound", "*", &row, szQuerry );
	if( row <= 0 ) return 0;
	
	m_eventSound.Set( row );
	
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT event_no, event_start_date, event_end_date, sound"
		" FROM event_sound WHERE [all] <> 0 OR map = '%s'", MapName );
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if( !SQLOK(retCode) )
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return -1;
	}
	
	retCode = SQLFetch( hStmt );
	int index = 0;
	TIMESTAMP_STRUCT	start, end;
	
	while( SQLOK( retCode ) )
	{
		int temp = 0;
		int ct = 1;
		
		CEventSound *pData = m_eventSound.GetEventPoint( index );
		
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); pData->m_nEventNo = temp; temp = 0;
		retCode = SQLGetData(hStmt, ct++, SQL_C_TIMESTAMP, &start, sizeof( TIMESTAMP_STRUCT ), &cbValue);
		retCode = SQLGetData(hStmt, ct++, SQL_C_TIMESTAMP, &end, sizeof( TIMESTAMP_STRUCT ), &cbValue);
		pData->SetTime( &start, &end );
		
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); pData->m_nSoundNo = temp; temp = 0;
		retCode = SQLFetch( hStmt );
		index ++;
	}
	SQLFreeStmt(hStmt, SQL_DROP);
	
	// 이벤트 시간 체크
	m_eventSound.CheckActiveEvent();
	return 1;
}


// 021107 YGI
int CEventMgr::LoadEventMessage()
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
	char		szQuerry[512];
	
	static int nation2nation[] = { 0, 0, 0, 1, 2, 0, 3 };
	int nation_num = nation2nation[MapInfo[MapNumber].nation];
	if( !nation_num ) nation_num = -1;
	
	int row = 0;
	sprintf_s( szQuerry, sizeof(szQuerry), "[all] <> 0 OR nation = %d OR map = '%s'", nation_num, MapName );
	GetRowLineOfSQL( "Event_Message", "*", &row, szQuerry );
	if( row <= 0 ) return 0;
	
	m_eventMessage.Set( row );
	
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT event_no, event_start_date, event_end_date, [all], nation, [time], "
		" DataLength(message), message FROM event_message WHERE [all] <> 0 OR nation = %d OR map = '%s'", 
		nation_num, MapName );
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if( !SQLOK(retCode) )
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return -1;
	}
	
	retCode = SQLFetch( hStmt );
	int index = 0;
	TIMESTAMP_STRUCT	start, end;
	
	while( SQLOK( retCode ) )
	{
		int temp = 0;
		int ct = 1;
		
		CEventMessage *pData = m_eventMessage.GetEventPoint( index );
		
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); pData->m_nEventNo = temp; temp = 0;
		retCode = SQLGetData(hStmt, ct++, SQL_C_TIMESTAMP, &start, sizeof( TIMESTAMP_STRUCT ), &cbValue);
		retCode = SQLGetData(hStmt, ct++, SQL_C_TIMESTAMP, &end, sizeof( TIMESTAMP_STRUCT ), &cbValue);
		pData->SetTime( &start, &end );
		
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); pData->m_bAll = temp; temp = 0;
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); pData->m_nNation = temp; temp = 0;
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); pData->m_nTime = temp; temp = 0;
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue);
		if( temp )
		{
			char *temp_msg = new char[temp+1];
			retCode = SQLGetData(hStmt, ct++, SQL_C_CHAR, temp_msg, temp+1, &cbValue);
			pData->SetMessage( temp_msg, temp );
			SAFE_DELETE_ARRAY(temp_msg);
		}
		
		retCode = SQLFetch( hStmt );
		index ++;
	}
	SQLFreeStmt(hStmt, SQL_DROP);
	
	// 이벤트 시간 체크
	m_eventMessage.CheckActiveEvent();
	return 1;
}

int CEventMgr::LoadEventNpcScript()
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
	char		szQuerry[512];
	
	int row = 0;
	sprintf_s( szQuerry, sizeof(szQuerry), "map = '%s'", MapName );
	GetRowLineOfSQL( "Event_NpcItem", "*", &row, szQuerry );
	if( row <= 0 ) return 0;
	
	m_eventNpcScript.Set( row );
	
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT event_no, event_start_date, event_end_date, script_no, number, event_npcitem_bag, "
		" DataLength(text), [text], DataLength( fail_text ), fail_text FROM event_NpcItem WHERE map = '%s'", MapName );
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if( !SQLOK(retCode) )
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return -1;
	}
	
	retCode = SQLFetch( hStmt );
	int index = 0;
	TIMESTAMP_STRUCT	start, end;
	
	while( SQLOK( retCode ) )
	{
		int temp = 0;
		int ct = 1;
		
		CEventNpcScript *pData = m_eventNpcScript.GetEventPoint( index );
		
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); pData->m_nEventNo = temp; temp = 0;
		retCode = SQLGetData(hStmt, ct++, SQL_C_TIMESTAMP, &start, sizeof( TIMESTAMP_STRUCT ), &cbValue);
		retCode = SQLGetData(hStmt, ct++, SQL_C_TIMESTAMP, &end, sizeof( TIMESTAMP_STRUCT ), &cbValue);
		pData->SetTime( &start, &end );
		
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); pData->m_nScriptNo = temp; temp = 0;
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); pData->m_nNumber = temp; temp = 0;
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); pData->m_nEventNpcItemBag = temp; temp = 0;
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue);
		if( temp )
		{
			char *temp_msg = new char[temp+1];
			retCode = SQLGetData(hStmt, ct++, SQL_C_CHAR, temp_msg, temp+1, &cbValue);
			pData->SetMessage( temp_msg, temp );
			SAFE_DELETE_ARRAY(temp_msg);
		}
		else ct++;
		temp = 0;
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue);
		if( temp )
		{
			char *temp_msg = new char[temp+1];
			retCode = SQLGetData(hStmt, ct++, SQL_C_CHAR, temp_msg, temp+1, &cbValue);
			pData->SetMessage2( temp_msg, temp );
			SAFE_DELETE_ARRAY(temp_msg);
		}
		
		retCode = SQLFetch( hStmt );
		index ++;
	}
	SQLFreeStmt(hStmt, SQL_DROP);
	
	// 이벤트 시간 체크
	m_eventNpcScript.CheckActiveEvent();
	return 1;
}

int CEventMgr::LoadItemBag()
{
	HSTMT		hStmt = NULL; 
	RETCODE		retCode;
	SDWORD		cbValue;
	char		szQuerry[512];
	
	int row = 0;
	SetItemBagCount( 0 );
	GetRowLineOfSQL( "Event_NpcItem_Bag", "*", &row, NULL );
	if( row <= 0 ) return 0;
	
	SetItemBagCount( row );
	
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT * FROM Event_NpcItem_Bag" );
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if( !SQLOK(retCode) )
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return -1;
	}
	
	retCode = SQLFetch( hStmt );
	int index = 0;
	while( SQLOK( retCode ) )
	{
		int temp = 0;
		int ct = 1;
		
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue);
		CEventScriptItemBag	*pData = GetBagPointByIndex( index );
		pData->m_nBagNumber = temp;
		temp = 0;
		for( int i=0; i<MAX_SCRIPT_ITEM-1; i++ )
		{
			retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); pData->m_aItemNo[i] = temp; 
			temp = 0;
		}
		pData->Count();
		
		retCode = SQLFetch( hStmt );
		index ++;
	}
	SQLFreeStmt(hStmt, SQL_DROP);
	return 1;
}

int CEventMgr::LoadEventMoveMap()
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
	char		szQuerry[512];
	
	int row = 0;
	GetRowLineOfSQL( "event_mapmove", "*", &row, NULL );
	if( row <= 0 ) return 0;
	
	m_eventMoveMap.Set( row );
	
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT event_no, event_start_date, event_end_date, [all], sex, class, nation, "
		" map, map_x, map_y, max FROM event_mapmove" );
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if( !SQLOK(retCode) )
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return -1;
	}
	
	retCode = SQLFetch( hStmt );
	int index = 0;
	TIMESTAMP_STRUCT	start, end;
	
	while( SQLOK( retCode ) )
	{
		int temp = 0;
		int ct = 1;
		
		CEventMoveMap *pData = m_eventMoveMap.GetEventPoint( index );
		
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); pData->m_nEventNo = temp; temp = 0;
		retCode = SQLGetData(hStmt, ct++, SQL_C_TIMESTAMP, &start, sizeof( TIMESTAMP_STRUCT ), &cbValue);
		retCode = SQLGetData(hStmt, ct++, SQL_C_TIMESTAMP, &end, sizeof( TIMESTAMP_STRUCT ), &cbValue);
		pData->SetTime( &start, &end );
		
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); pData->m_bAll = temp; temp = 0;
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); pData->m_nSex = temp; temp = 0;
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); pData->m_nClass = temp; temp = 0;
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); pData->m_nNation = temp; temp = 0;
		retCode = SQLGetData(hStmt, ct++, SQL_C_CHAR, pData->m_szMap, 20, &cbValue);
		EatRearWhiteChar( pData->m_szMap );
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); pData->m_nMapX = temp; temp = 0;
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); pData->m_nMapY = temp; temp = 0;
		retCode = SQLGetData(hStmt, ct++, SQL_C_LONG, &temp, 0, &cbValue); pData->m_nMax = temp; temp = 0;
		
		retCode = SQLFetch( hStmt );
		index ++;
	}
	SQLFreeStmt(hStmt, SQL_DROP);
	
	// 이벤트 시간 체크
	m_eventMoveMap.CheckActiveEvent();
	return 1;
}



//< soto-030324
int	LoadGuardianGuildList()
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
	char		szQuerry[512];

	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT guild_code FROM guardian_list");
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);

	if (!SQLOK(retCode))
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return 0;
	}	
	
	retCode = SQLFetch(hStmt);

	while (SQLOK(retCode))
	{		
		int temp = -1;
		retCode = SQLGetData(hStmt, 1, SQL_C_LONG, &temp, 0, &cbValue);

		if (!SQLOK(retCode))
		{
			SQLFreeStmt(hStmt, SQL_DROP);
			return 0;
		}
			
		g_CGuardianGuild.AddGuild(temp);
		retCode = SQLFetch(hStmt);
	}

	SQLFreeStmt(hStmt, SQL_DROP);
	return 1;
}
//> soto-030324

bool LoadArenaInfo()
{	//< CSD-030522
	char szQuerry[512];
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT * FROM arena_info ORDER BY map_port");
	HSTMT	hStmt = NULL;
	SQLAllocStmt(hDBC, &hStmt);

	RETCODE	retCode = SQLExecDirect(hStmt, (UCHAR*)szQuerry, SQL_NTS);
	
	if (retCode != SQL_SUCCESS_WITH_INFO && retCode != SQL_SUCCESS) 
	{
		MyLog(LOG_NORMAL, "arena_info : ExecDirect Error");
		SQLFreeStmt(hStmt, SQL_DROP);
		return false;
	}
	
	SDWORD cbValue;
	int nPort = 0, nTemp = 0, nX = 0, nY = 0;
	
	while ((retCode = SQLFetch(hStmt)) == SQL_SUCCESS)
	{
		CArenaInfo* pArena = CArenaInfo::CreateArenaInfo();

		if (pArena == NULL)
		{
			SQLFreeStmt(hStmt, SQL_DROP);
			return false;
		}
		
		SQLGetData(hStmt, 1, SQL_C_LONG, &nPort, 0, &cbValue);
		pArena->SetMapPort(nPort);
		SQLGetData(hStmt, 3, SQL_C_LONG, &nX, 0, &cbValue);
		SQLGetData(hStmt, 4, SQL_C_LONG, &nY, 0, &cbValue);
		pArena->SetWaitPosition(nX, nY);
//#ifdef LMS_MAPSERVER
//		if (nPort != g_MapPort)
//		{
//#endif
			if (!g_pArenaManager->AddArena(nPort, pArena))
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return false;
			}
//#ifdef LMS_MAPSERVER
//		}
//#endif
	}
	
	SQLFreeStmt(hStmt, SQL_DROP);
	return true;
}	//> CSD-030522

bool LoadArenaGameInfo()
{	//< CSD-030522
	if (!g_pArenaManager->IsArenaInfo(g_MapPort))
	{
		return true;
	}
	
	char szQuerry[512];
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT * FROM arena_game_info WHERE map_port = %d", g_MapPort);

	HSTMT	hStmt = NULL;
	SQLAllocStmt(hDBC, &hStmt);

	RETCODE	retCode = SQLExecDirect(hStmt, (UCHAR*)szQuerry, SQL_NTS);
	
	if (retCode != SQL_SUCCESS_WITH_INFO && retCode != SQL_SUCCESS) 
	{
		MyLog(LOG_NORMAL, "arena_game_info : ExecDirect Error");
		SQLFreeStmt(hStmt, SQL_DROP);
		return false;
	}

	SDWORD cbValue;
	int nIndex = 0, nTemp = 0, nMin = 0, nMax = 0;
	float fTemp = 0.0;
	
	while ((retCode = SQLFetch(hStmt)) == SQL_SUCCESS)
	{
		SQLGetData(hStmt, 1, SQL_C_LONG, &nIndex, 0, &cbValue);
		SQLGetData(hStmt, 2, SQL_C_LONG, &nTemp, 0, &cbValue);
		
		CBaseArena* pGame = CBaseArena::CreateArena(nIndex, nTemp);
		
		if (pGame == NULL)
		{
			SQLFreeStmt(hStmt, SQL_DROP);
			return false;
		}

		CArenaState* pState = new CArenaState;

		if (pState == NULL)
		{
			SQLFreeStmt(hStmt, SQL_DROP);
			return false;
		}

		SQLGetData(hStmt, 3, SQL_C_LONG, &nTemp, 0, &cbValue);
		pState->SetPlayTime(nTemp);
		pGame->SetArenaState(pState);

		CArenaObserve* pObserve = new CArenaObserve;

		if (pObserve == NULL)
		{
			SQLFreeStmt(hStmt, SQL_DROP);
			return false;
		}

		SQLGetData(hStmt, 4, SQL_C_LONG, &nTemp, 0, &cbValue);
		pObserve->SetObserveMoney(nTemp);
		pGame->SetArenaObserve(pObserve);

		CArenaGambling* pGambling = new CArenaGambling;

		if (pGambling == NULL)
		{
			SQLFreeStmt(hStmt, SQL_DROP);
			return false;
		}

		SQLGetData(hStmt, 5, SQL_C_LONG, &nMin, 0, &cbValue);
		SQLGetData(hStmt, 6, SQL_C_LONG, &nMax, 0, &cbValue);
		pGambling->SetBattingMoney(nMin, nMax);
		
		SQLGetData(hStmt, 7, SQL_C_FLOAT, &fTemp, 0, &cbValue);
		pGambling->SetDividendRate(fTemp);
		pGame->SetArenaGambling(pGambling);

		SQLGetData(hStmt, 8, SQL_C_FLOAT, &fTemp, 0, &cbValue);

		if (!g_pArenaManager->AddGame(nIndex, pGame))
		{
			SQLFreeStmt(hStmt, SQL_DROP);
			return false;
		}
	}
	
	SQLFreeStmt(hStmt, SQL_DROP);
	return true;
}	//> CSD-030522

bool LoadArenaTeamInfo()
{	//< CSD-030522
	CArenaManager::HASH_GAME mpGame = g_pArenaManager->GetGameSet();

	for (CArenaManager::ITOR_GAME i = mpGame.begin(); i != mpGame.end(); ++i)
	{
		const int nGame = i->first;

		CBaseArena* pGame = g_pArenaManager->GetGameInfo(nGame);

		if (pGame == NULL)
		{
			return false;
		}

		char szQuerry[512];
		sprintf_s(szQuerry, sizeof(szQuerry), "SELECT * FROM arena_team_info WHERE idGame = %d", nGame);
		HSTMT	hStmt = NULL;
		SQLAllocStmt(hDBC, &hStmt);

		RETCODE	retCode = SQLExecDirect(hStmt, (UCHAR*)szQuerry, SQL_NTS);
		
		if (retCode != SQL_SUCCESS_WITH_INFO && retCode != SQL_SUCCESS) 
		{
			MyLog(LOG_NORMAL, "arena_team_info : ExecDirect Error");
			SQLFreeStmt(hStmt, SQL_DROP);
			return false;
		}
		
		SDWORD cbValue;
		int nTeam = 0, nX = 0, nY = 0, nTemp = 0;
		
		while ((retCode = SQLFetch(hStmt)) == SQL_SUCCESS)
		{
			CArenaLobby* pLobby = CArenaLobby::CreateArenaLobby();

			if (pLobby == NULL)
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return false;
			}

			pGame->SetArenaLobby(pLobby);

			CArenaTeam* pTeam = CArenaTeam::CreateArenaTeam();

			if (pTeam == NULL)
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return false;
			}
			
			SQLGetData(hStmt, 2, SQL_C_LONG, &nX, 0, &cbValue);
			SQLGetData(hStmt, 3, SQL_C_LONG, &nY, 0, &cbValue);
			pTeam->SetBasePosition(nX, nY);
			SQLGetData(hStmt, 4, SQL_C_LONG, &nTemp, 0, &cbValue);
			pTeam->SetHuntNumber(nTemp);
			pTeam->SetTeamNumber(nTeam++);
			// 팀 추가
			pGame->AddTeam(pTeam);
		}
		
		SQLFreeStmt(hStmt, SQL_DROP);
	}

	return true;
}	//> CSD-030522

bool LoadArenaBonusInfo()
{	//< CSD-030522
	CArenaManager::HASH_GAME mpGame = g_pArenaManager->GetGameSet();

	for (CArenaManager::ITOR_GAME i = mpGame.begin(); i != mpGame.end(); ++i)
	{
		const int nGame = i->first;

		CBaseArena* pGame = g_pArenaManager->GetGameInfo(nGame);

		if (pGame == NULL)
		{
			return false;
		}

		char szQuerry[512];
		sprintf_s(szQuerry, sizeof(szQuerry), "SELECT * FROM arena_bonus_info WHERE idGame = %d", nGame);
		HSTMT	hStmt = NULL;
		SQLAllocStmt(hDBC, &hStmt);

		RETCODE	retCode = SQLExecDirect(hStmt, (UCHAR*)szQuerry, SQL_NTS);
		
		if (retCode != SQL_SUCCESS_WITH_INFO && retCode != SQL_SUCCESS) 
		{
			MyLog(LOG_NORMAL, "arena_bonus_info : ExecDirect Error");
			SQLFreeStmt(hStmt, SQL_DROP);
			return false;
		}
		
		SDWORD cbValue;
		int nMin = 0, nMax = 0, nTemp = 0;
		float fRate = 0.0;
		
		while ((retCode = SQLFetch(hStmt)) == SQL_SUCCESS)
		{
			CArenaBonus* pBonus = CArenaBonus::CreateArenaBonus();

			if (pBonus == NULL)
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return false;
			}
			
			SQLGetData(hStmt, 2, SQL_C_LONG, &nMin, 0, &cbValue);
			SQLGetData(hStmt, 3, SQL_C_LONG, &nMax, 0, &cbValue);
			pBonus->SetLevelRange(nMin, nMax);
			SQLGetData(hStmt, 4, SQL_C_LONG, &nTemp, 0, &cbValue);
			pBonus->SetItemHouse(nTemp);
			SQLGetData(hStmt, 5, SQL_C_LONG, &nTemp, 0, &cbValue);
			pBonus->SetItemKind(nTemp);
			SQLGetData(hStmt, 6, SQL_C_LONG, &nTemp, 0, &cbValue);
			pBonus->SetItemGrade(nTemp);
			SQLGetData(hStmt, 7, SQL_C_FLOAT, &fRate, 0, &cbValue);
			pBonus->SetItemGainRate(fRate);
			SQLGetData(hStmt, 8, SQL_C_FLOAT, &fRate, 0, &cbValue);
			pBonus->SetExpRate(fRate);
			// 팀 추가
			pGame->AddBonus(pBonus);
		}
		
		SQLFreeStmt(hStmt, SQL_DROP);
	}

	return true;
}	//> CSD-030517

bool LoadHuntInfo()
{   //< CSD-030522
	char szQuerry[512];
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT * FROM hunt_info WHERE map_port = %d ORDER BY idHunt", g_MapPort);

	HSTMT hStmt = NULL;
	SQLAllocStmt(hDBC, &hStmt);

	RETCODE	retCode = SQLExecDirect(hStmt, (UCHAR*)szQuerry, SQL_NTS);
	
	if (retCode != SQL_SUCCESS_WITH_INFO && retCode != SQL_SUCCESS) 
	{
		MyLog(LOG_NORMAL, "hunt_info : ExecDirect Error");
		SQLFreeStmt(hStmt, SQL_DROP);
		return false;
	}
	
	SDWORD cbValue;
	int nIndex = 0, nTemp = 0;
	
	while ((retCode = SQLFetch(hStmt)) == SQL_SUCCESS)
	{
		CGroupInfo* pGroup = CGroupInfo::CreateGroupInfo();

		if (pGroup == NULL)
		{
			SQLFreeStmt(hStmt, SQL_DROP);
			return false;
		}
		//< CSD-031027
		SQLGetData(hStmt, 1, SQL_C_SLONG, &nIndex, 0, &cbValue);
		SQLGetData(hStmt, 4, SQL_C_SLONG, &nTemp, 0, &cbValue);
		pGroup->SetGroupType(nTemp);
		SQLGetData(hStmt, 5, SQL_C_SLONG, &nTemp, 0, &cbValue);
		pGroup->SetEventType(nTemp);
		//> CSD-031027
		if (!g_pRegenManager->AddGroup(nIndex, pGroup))
		{
			SQLFreeStmt(hStmt, SQL_DROP);
			return false;
		}
	}
	
	SQLFreeStmt(hStmt, SQL_DROP);
	return true;
}	//> CSD-030522

bool LoadHuntGroupInfo()
{   //< CSD-030522

	CRegenManager::HASH_GROUP mpGroup = g_pRegenManager->GetGroupSet();

	for (CRegenManager::ITOR_GROUP i = mpGroup.begin(); i != mpGroup.end(); ++i)
	{
		CGroupInfo* pGroup = i->second;
		
		char szQuerry[512];
		sprintf_s(szQuerry, sizeof(szQuerry), "SELECT * FROM hunt_group_info WHERE idHunt = %d ORDER BY regen_order", i->first);

		HSTMT hStmt = NULL;
		SQLAllocStmt(hDBC, &hStmt);

		RETCODE	retCode = SQLExecDirect(hStmt, (UCHAR*)szQuerry, SQL_NTS);
		
		if (retCode != SQL_SUCCESS_WITH_INFO && retCode != SQL_SUCCESS) 
		{
			MyLog(LOG_NORMAL, "hunt_group_info : ExecDirect Error");
			SQLFreeStmt(hStmt, SQL_DROP);
			return false;
		}

		const int nGroupType = pGroup->GetGroupType();
		
		SDWORD cbValue;
		int nIndex = 0, nTemp = 0;
		
		while ((retCode = SQLFetch(hStmt)) == SQL_SUCCESS)
		{
			CRegenInfo* pRegen = CRegenInfo::CreateRegenInfo(nGroupType);

			if (pRegen == NULL)
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return false;
			}

			SQLGetData(hStmt, 1, SQL_C_SLONG, &nIndex, 0, &cbValue);
			pRegen->SetGroupNumber(nIndex);
			SQLGetData(hStmt, 3, SQL_C_SLONG, &nTemp, 0, &cbValue);
			pRegen->SetDelayTime(nTemp);
			pGroup->AddRegen(pRegen);
		}
		
		SQLFreeStmt(hStmt, SQL_DROP);
	}

	return true;
}	//> CSD-030522

bool LoadHuntMonsterInfo()
{   //< CSD-030522
	CRegenManager::HASH_GROUP mpGroup = g_pRegenManager->GetGroupSet();

	for (CRegenManager::ITOR_GROUP i = mpGroup.begin(); i != mpGroup.end(); ++i)
	{
		CGroupInfo* pGroup = i->second;
		CGroupInfo::LIST_REGEN ltRegen = pGroup->GetRegenSet();

		for (CGroupInfo::ITOR_REGEN j = ltRegen.begin(); j != ltRegen.end(); ++j)
		{
			CRegenInfo* pRegen = *j;

			char szQuerry[512];
			sprintf_s(szQuerry, sizeof(szQuerry), "SELECT * FROM hunt_monster_info WHERE idGroup = %d ORDER BY monster_type", pRegen->GetGroupNumber());

			HSTMT hStmt = NULL;
			SQLAllocStmt(hDBC, &hStmt);

			RETCODE	retCode = SQLExecDirect(hStmt, (UCHAR*)szQuerry, SQL_NTS);
			
			if (retCode != SQL_SUCCESS_WITH_INFO && retCode != SQL_SUCCESS) 
			{
				MyLog(LOG_NORMAL, "hunt_monster_info : ExecDirect Error");
				SQLFreeStmt(hStmt, SQL_DROP);
				return false;
			}

			SDWORD cbValue;
			int nTemp = 0;
			
			while ((retCode = SQLFetch(hStmt)) == SQL_SUCCESS)
			{
				REGEN_MONSTER_INFO infMonster;
				SQLGetData(hStmt, 3, SQL_C_SLONG, &nTemp, 0, &cbValue); 
				infMonster.nMonsterNo = nTemp;
				SQLGetData(hStmt, 4, SQL_C_SLONG, &nTemp, 0, &cbValue);
				infMonster.nMonsterType = nTemp;
				SQLGetData(hStmt, 5, SQL_C_SLONG, &nTemp, 0, &cbValue);
				infMonster.nMonsterCount = nTemp;
				pRegen->AddMonster(infMonster);
			}
			
			SQLFreeStmt(hStmt, SQL_DROP);
		}
	}

	return true;
}	//> CSD-030522

bool LoadHuntPlaceInfo()
{   //< CSD-030522
	CRegenManager::HASH_GROUP mpGroup = g_pRegenManager->GetGroupSet();

	for (CRegenManager::ITOR_GROUP i = mpGroup.begin(); i != mpGroup.end(); ++i)
	{
		CGroupInfo* pGroup = i->second;
		CGroupInfo::LIST_REGEN ltRegen = pGroup->GetRegenSet();

		for (CGroupInfo::ITOR_REGEN j = ltRegen.begin(); j != ltRegen.end(); ++j)
		{
			CRegenInfo* pRegen = *j;

			char szQuerry[512];
			sprintf_s(szQuerry, sizeof(szQuerry), "SELECT * FROM hunt_place_info WHERE idGroup = %d ORDER BY number", pRegen->GetGroupNumber());

			HSTMT hStmt = NULL;
			SQLAllocStmt(hDBC, &hStmt);

			RETCODE	retCode = SQLExecDirect(hStmt, (UCHAR*)szQuerry, SQL_NTS);
			
			if (retCode != SQL_SUCCESS_WITH_INFO && retCode != SQL_SUCCESS) 
			{
				MyLog(LOG_NORMAL, "hunt_place_info : ExecDirect Error");
				SQLFreeStmt(hStmt, SQL_DROP);
				return false;
			}

			SDWORD cbValue;
			int nTemp = 0;
			
			while ((retCode = SQLFetch(hStmt)) == SQL_SUCCESS)
			{
				REGEN_PLACE_INFO infPlace;
				SQLGetData(hStmt, 2, SQL_C_SLONG, &nTemp, 0, &cbValue); 
				infPlace.nNumber = nTemp;
				SQLGetData(hStmt, 3, SQL_C_SLONG, &nTemp, 0, &cbValue); 
				infPlace.nPosX = nTemp;
				SQLGetData(hStmt, 4, SQL_C_SLONG, &nTemp, 0, &cbValue); 
				infPlace.nPosY = nTemp;
				SQLGetData(hStmt, 5, SQL_C_SLONG, &nTemp, 0, &cbValue); 
				infPlace.nScope = nTemp;
				pRegen->AddPlace(infPlace);
			}
			
			SQLFreeStmt(hStmt, SQL_DROP);
		}
	}

	g_pRegenManager->Ready(); // CSD-HK-030829
	return true;
}	//> CSD-030522

bool LoadWarStartInfo()
{   //< CSD-030804
	if (g_pNation == NULL)
	{
		MyLog(0,"g_pNation was not Initiaized..!!");
		return false;
	}

	char szQuerry[512];
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT * FROM WarStartUpTBL WHERE Type = 2 ORDER BY DayofWeek");

	HSTMT hStmt = NULL;
	SQLAllocStmt(hDBC, &hStmt);

	RETCODE	retCode = SQLExecDirect(hStmt, (UCHAR*)szQuerry, SQL_NTS);
	
	if (retCode != SQL_SUCCESS_WITH_INFO && retCode !=  SQL_SUCCESS) 
	{
		MyLog(LOG_NORMAL, "WarStartUpTBL : ExecDirect Error");
		SQLFreeStmt(hStmt, SQL_DROP);
		return false;
	}
	
	SDWORD cbValue;
	int nIndex = 0, nTemp = 0;

	CWarStartInfo* pWarStartInfo = CWarStartInfo::CreateWarStartInfo();

	if (pWarStartInfo == NULL)
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return false;
	}

	MyLog(LOG_NORMAL, "<!-- LOCALWAR Startup Information -->");
	
	while ((retCode = SQLFetch(hStmt)) == SQL_SUCCESS)
	{
		START_UP_INFO* pTime = new START_UP_INFO;
		
		if (pTime == NULL)
		{
			SQLFreeStmt(hStmt, SQL_DROP);
			return false;
		}

		SQLGetData(hStmt, 1, SQL_C_SLONG, &nIndex, 0, &cbValue);
		pTime->idIndex = nIndex;
		SQLGetData(hStmt, 3, SQL_C_SLONG, &nTemp, 0, &cbValue);
		pTime->nApplyDay = nTemp;  
		SQLGetData(hStmt, 4, SQL_C_SLONG, &nTemp, 0, &cbValue);
		pTime->nStartHour = nTemp;
		pTime->nStartMinute = 0;
		pTime->nEndHour = (nTemp + 2)%24;
		pTime->nEndMinute = 0;

		MyLog(LOG_NORMAL, "[%d]%s(%02d:%02d - %02d:%02d)", 
			  nIndex,
			  g_pLogManager->ConvertToDay(pTime->nApplyDay),
			  pTime->nStartHour,
			  pTime->nStartMinute,
			  pTime->nEndHour,
			  pTime->nEndMinute);
		pWarStartInfo->Register(pTime);
	}

	g_pNation->SetWarStartInfo(pWarStartInfo);
	
	SQLFreeStmt(hStmt, SQL_DROP);
	return true;
}	//> CSD-030804

// 031009 CI YGI
int GetItemLimitCount_SQL( WORD port, DWORD *pLimit )
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
	char szQuerry[512];
	
	if( port < 4000 ) return 0;

	sprintf_s( szQuerry, sizeof(szQuerry), "SELECT limit_number FROM ItemLimitMgrTable WHERE port = %d", port );
	*pLimit = 0;

	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if( !SQLOK(retCode) )
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return 0;
	}
	
	retCode = ::SQLFetch( hStmt );
	if( SQLOK( retCode ) )
	{
		::SQLGetData(hStmt, 1, SQL_C_LONG, pLimit, 0, &cbValue);
		retCode = SQLFetch( hStmt );
		::SQLFreeStmt(hStmt, SQL_DROP);
		return 1;	
	}
	::SQLFreeStmt(hStmt, SQL_DROP);
	return 0;
}

// 040105 YGI 보물
bool LoadEventTreasureXY()
{
	int nRow = 0;
	char szQuerry[512];
	sprintf_s( szQuerry, sizeof(szQuerry), "mapname = '%s'", MapName );
	GetRowLineOfSQL( "Event_treasure_xy", "*", &nRow, szQuerry );

	g_EventFindTreasure.Clear();
	if( nRow <= 0 ) return true;
	g_EventFindTreasure.SetBuffer( nRow );
	
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT x, y FROM Event_treasure_xy WHERE mapname = '%s'", MapName );
	HSTMT hStmt = NULL;
	SQLAllocStmt(hDBC, &hStmt);
	RETCODE	retCode = SQLExecDirect(hStmt, (UCHAR*)szQuerry, SQL_NTS);	
	if (retCode != SQL_SUCCESS_WITH_INFO && retCode != SQL_SUCCESS) 
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return false;
	}
	
	SDWORD cbValue;
	int ct = 0;
	int x, y;
	while((retCode = SQLFetch(hStmt)) == SQL_SUCCESS)
	{
		SQLGetData(hStmt, 1, SQL_C_SLONG, &x, 0, &cbValue);
		SQLGetData(hStmt, 2, SQL_C_SLONG, &y, 0, &cbValue);
		g_EventFindTreasure.SetPoint( ct, x, y );
		ct++;
	}
	
	SQLFreeStmt(hStmt, SQL_DROP);

	MyLog( 0, "Read OK Event_Treasure_XY table(%d)!", ct );
	return true;
}

bool LoadEventTreasureGuard()
{
	for( int i=0; i<5; i++ )
	{
		g_TreasureGuardTBL[i].Reset();
		g_TreasureGuardTBL[i].SetCount( 5 );
	}

	char szQuerry[512];
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT lv, npc1, npc1_count, npc2, npc2_count, npc3, npc3_count, npc4, npc4_count, npc5, npc5_count FROM Event_treasure_Level" );
	HSTMT hStmt = NULL;
	SQLAllocStmt(hDBC, &hStmt);
	RETCODE	retCode = SQLExecDirect(hStmt, (UCHAR*)szQuerry, SQL_NTS);	
	if (retCode != SQL_SUCCESS_WITH_INFO && retCode != SQL_SUCCESS) 
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return false;
	}
	
	SDWORD cbValue;
	int lv, npc, npc_count;
	while((retCode = SQLFetch(hStmt)) == SQL_SUCCESS)
	{
		lv = 0;
		npc = npc_count = 0;
		SQLGetData(hStmt, 1, SQL_C_SLONG, &lv, 0, &cbValue);
		if( lv<=0 || lv>5 ) continue;

		SQLGetData(hStmt, 2, SQL_C_SLONG, &npc, 0, &cbValue);
		SQLGetData(hStmt, 3, SQL_C_SLONG, &npc_count, 0, &cbValue);
		g_TreasureGuardTBL[lv-1].Set( 0, npc, npc_count );

		npc = npc_count = 0;
		SQLGetData(hStmt, 4, SQL_C_SLONG, &npc, 0, &cbValue);
		SQLGetData(hStmt, 5, SQL_C_SLONG, &npc_count, 0, &cbValue);
		g_TreasureGuardTBL[lv-1].Set( 1, npc, npc_count );

		npc = npc_count = 0;
		SQLGetData(hStmt, 6, SQL_C_SLONG, &npc, 0, &cbValue);
		SQLGetData(hStmt, 7, SQL_C_SLONG, &npc_count, 0, &cbValue);
		g_TreasureGuardTBL[lv-1].Set( 2, npc, npc_count );

		npc = npc_count = 0;
		SQLGetData(hStmt, 8, SQL_C_SLONG, &npc, 0, &cbValue);
		SQLGetData(hStmt, 9, SQL_C_SLONG, &npc_count, 0, &cbValue);
		g_TreasureGuardTBL[lv-1].Set( 3, npc, npc_count );

		npc = npc_count = 0;
		SQLGetData(hStmt, 10, SQL_C_SLONG, &npc, 0, &cbValue);
		SQLGetData(hStmt, 11, SQL_C_SLONG, &npc_count, 0, &cbValue);
		g_TreasureGuardTBL[lv-1].Set( 4, npc, npc_count );
	}
	
	SQLFreeStmt(hStmt, SQL_DROP);
	MyLog( 0, "Read OK Event_treasure_Level table!" );
	return true;
}

bool LoadStoreTable() // finito 12/07/07
{
	// Clear list if already contains something
	if (g_Stores.size())
	{
		g_Stores.clear();
	}

	char szQuerry[80];
	sprintf_s(szQuerry, sizeof(szQuerry), "select store_id, event_no, map_no, map_x, map_y from store where map_no = %d", MapNumber);
	HSTMT hStmt = NULL;
	SQLAllocStmt(hDBC, &hStmt);
	RETCODE	retCode = SQLExecDirect(hStmt, (UCHAR*)szQuerry, SQL_NTS);	
	if (retCode != SQL_SUCCESS_WITH_INFO && retCode != SQL_SUCCESS) 
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return false;
	}
	
	SDWORD cbValue;
	int store_id, event_no, map_no, map_x, map_y;
	while((retCode = SQLFetch(hStmt)) == SQL_SUCCESS)
	{
		SQLGetData(hStmt, 1, SQL_C_SLONG, &store_id, 0, &cbValue);
		SQLGetData(hStmt, 2, SQL_C_SLONG, &event_no, 0, &cbValue);
		SQLGetData(hStmt, 3, SQL_C_SLONG, &map_no, 0, &cbValue);
		SQLGetData(hStmt, 4, SQL_C_SLONG, &map_x, 0, &cbValue);
		SQLGetData(hStmt, 5, SQL_C_SLONG, &map_y, 0, &cbValue);

		Store store;
		store.store_id = store_id;
		store.event_no = event_no;
		store.map_no = map_no;
		store.map_x = map_x;
		store.map_y = map_y;
		g_Stores.push_back(store);
	}
	
	SQLFreeStmt(hStmt, SQL_DROP);
	MyLog( 0, "Read OK store table!" );
	return true;
}

bool LoadItemMallTable() // finito 29/06/08
{
	// Clear list if already contains something
	if (g_MallItems.size())
	{
		g_MallItems.clear();
	}

	char szQuerry[80];
	sprintf_s(szQuerry, sizeof(szQuerry), "select item_no, price, time from ItemMall");
	HSTMT hStmt = NULL;
	SQLAllocStmt(hDBC, &hStmt);
	RETCODE	retCode = SQLExecDirect(hStmt, (UCHAR*)szQuerry, SQL_NTS);	
	if (retCode != SQL_SUCCESS_WITH_INFO && retCode != SQL_SUCCESS) 
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return false;
	}
	
	SDWORD cbValue;
	int item_no, price, time;
	while((retCode = SQLFetch(hStmt)) == SQL_SUCCESS)
	{		
		SQLGetData(hStmt, 1, SQL_C_SLONG, &item_no, 0, &cbValue);
		SQLGetData(hStmt, 2, SQL_C_SLONG, &price, 0, &cbValue);
		SQLGetData(hStmt, 3, SQL_C_SLONG, &time, 0, &cbValue);

		MallItem mallItem;
		mallItem.itemAttr.item_no = item_no;
		mallItem.price = price;
		mallItem.time = time;
		g_MallItems.push_back(mallItem);
	}
	
	SQLFreeStmt(hStmt, SQL_DROP);
	MyLog( 0, "Read OK mall table!" );
	return true;
}