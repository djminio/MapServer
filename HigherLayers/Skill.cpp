#include "..\stdafx.h"
#include "DefaultHeader.h"
#include "Citem.h"
#include "Kein_item.h"
#include "Sealstone.h"
#include "..\HigherLayers\WeatherControl.h"//020610 lsw
#include "Scrp_exe.h"
#include "..\LowerLayers\servertable.h"	// BBD 040311

extern void SaveBallancingSkill( LPCHARLIST a, int skillno );

extern int CanLocalWarAttack(CHARLIST* pAttacker,CHARLIST* pDefencer);		// 020115 LTS 최성동 ,임상우공동 //020205 lsw
extern int CheckEventAttack();
extern bool isLevelUp(CHARLIST*);

t_ItemRare ItemRare[MAX_ITEM_RARE];
t_ItemGrade ItemGrade[MAX_ITEM_GRADE];
t_ItemMutantKind ItemMutantKind[MAX_ITEM_MUTANT_KIND];

static lpMAPSKILLTABLE	g_Header[8][8];
CItem_Join	item_join[ ITEM_JOIN_MAX];
t_earthpower EarthPower;
int TotalItemJoinTableCount;
int Npc_Item_Table[NPC_ITEM_TABLE_X][NPC_ITEM_TABLE_Y];	//npc가 죽을때 나올 아이템들이 모여있는 테이블
int SkillLevelExp[ SKILL_LEVEL_EXP_TABLE_MAX_ + 1 ][ SKILL_LEVEL_EXP_TABLE_MAX_ +1 ];
int SkillExp[ SKILL_EXP_TABLE_MAX_ +1 ][SKILL_EXP_TABLE_MAX_ +1 ];
int MAX_JOB_LEVEL = 100; // CSD-040223

//<! BBD 040311
unsigned short	g_UsercountInServerSet = 1501;		// 최초 1501동접이라고 보고 시작한다.
float resource_rate[6] =
{
	// 동접에 따른 자원 배율
	1.0f, 1.2f, 1.4f, 1.6f, 1.8f, 2.0f
};

//<! BBD 040322 에이서용
float resource_rate2[6] =
{
	// 동접에 따른 자원 배율
	1.4f, 1.55f, 1.7f, 1.85f, 2.0f, 2.15f
};
//> BBD 040322

float GetResourceRate(int nCount);
//> BBD 040311

extern HDBC hDBC;
extern void SendOtherInv( int type, int other_id, t_connection c[], int cn );
extern void SendNK( int id, int nk );
extern inline int	IsHeNK(  LPCHARLIST ch, int nation  );
extern inline int	OutNK( LPCHARLIST ch, int nation );
extern void			InNK( LPCHARLIST ch, int nation, short int add_nk );
extern inline bool	IsWarfare( int anation, int bnation );
extern int			GetCaseOfNK( int cur_nation, int a_nation, int d_nation, int a_nk, int d_nk ) ;
extern void			CheckNK( int a_id, int d_id, int type  );
extern bool ExtraExp(void);
extern int	addEXP(CHARLIST *a, CHARLIST *d, int criteria, int damage );

extern void RequestUsercountToProxy();		// BBD 040311

// 030307 soto추가 중립지역인지 판단한다.
bool IsNeutralZone(int nMapNumber)
{	//< CSD-030314   
	return (MapInfo[nMapNumber].nation == 0) ? true:false; //중립지역에서만 도둑질이 가능하다.
}	//> CSD-030314
// soto End. 

int skillProb_k1( const int skillno, CItem_Join *join )
{	
	int k1 = 65;
	
	if( join ) // ItemJoinTable에서 k1을 가져온다.
	{
		k1 = join->k1;
	}
	else
	{
		k1 = SkillTbl[skillno].k1;
	}
	if( k1 < 0 ) k1 = 0;
	if( k1 > 100 ) k1 = 100;
	return k1;
}	
//-----------------------------------------------------
// 기  능 : 기술 성공륭의 최소값을 리턴한다. 
// 인  자 : 기술번호.
// 리턴값 : 0 < n < 100 
//-----------------------------------------------------
int skillProb_k2( const int skillno, CItem_Join *join )
{	
	int k2 = 65;
	if( join ) // ItemJoinTable에서 k2을 가져온다.
	{
		k2 = join->k2;
	}
	else
	{
		k2 = SkillTbl[skillno].k2;
	}
	if( k2 < 0 ) k2 = 0;
	if( k2 > 100 ) k2 = 100;
	return k2;
}	

int skillProb_needmax( const int skillno, const int target_id, CItem_Join *join )
{	
	int ret = 0;

	switch( SkillTbl[skillno].MotherSkillExp )
	{
		// 무조건 100%
	case 0 : ret = 100; 
		break;

		// 그냥 1 - 100
	case 1 : ret = 100;							
		break;	

		// 해당 아치템의 결합테이블참조..
	case 2 : if( join == NULL ) ret = 0;
		else 
		{
			ret = join->skill1_maximum;	
			return ret;
		}
		break;

		// 해당 스킬의 테이블참조.
	case 3 : ret = SkillTbl[ skillno].Ability_max;
		break;

		// 스킬의 태릭터 데이터 참조..( 테이밍 )
	case 4 : if( target_id >= 10000 )
			 {
				ret = NPC_Gen_Ref[NPCList[ target_id - 10000].SprNo].Taming_max;
			 }
			 else
			 {
				ret = 100;
			 }break;
		
		// 해당 스킬의 캐릭터 데이터 참조..
	case 5 : if( target_id >= 10000 )
			 {
				 ret = NPC_Gen_Ref[NPCList[ target_id-10000].SprNo].Talkanimal_max;
			 }
			 else
			 {
				ret = 100;
			 }break;
		
		// 아이템의 특성 참조..경작...
	case 6 :  ret = 100;							
		break;

		// 물고기 테이블 참조.. 낚시.
	case 7 :  ret = 100;

		// 해당 스킬 캐릭터 데이터 참조 ( 음악스킬 ) 
	case 8 :
	case 9 :
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	default :	ret = 100;
		break;
	}
	if( ret < 0 ) ret = 0;
	if( ret > 100 ) ret = 100;
	return ret;
}

int skillProb_needmin( const int skillno, const int target_id, CItem_Join *join )
{	
	int ret = 25;
	switch( SkillTbl[skillno].MotherSkillExp )
	{
		// 무조건 100%
	case 0 : ret = 1; 
		break;

		// 그냥 1 - 100
	case 1 : ret = 1;							
		break;	

		// 해당 아치템의 결합테이블참조..
	case 2 : if( join == NULL ) ret = 0;
			 else ret = join->skill1_minimum;		
		break;

		// 해당 스킬의 테이블참조.
	case 3 : ret = SkillTbl[ skillno].Ability_min;
		break;

		// 스킬의 태릭터 데이터 참조..( 테이밍 )
	case 4 :	if( target_id >= 10000 )
				{
					ret = NPC_Gen_Ref[NPCList[ target_id - 10000].SprNo].Taming_min;
				}
				else
				{
					ret = 1;
				}
		break;
		
		// 해당 스킬의 캐릭터 데이터 참조..
	case 5 : if( target_id >= 10000 )
			 {
				ret = NPC_Gen_Ref[NPCList[ target_id-10000].SprNo].Talkanimal_min;
			 }
			 else
			 {
				 ret = 1;
			 }
		 
		break;
		
		// 아이템의 특성 참조..경작...
	case 6 :  ret = 1;							
		break;

		// 물고기 테이블 참조.. 낚시.
	case 7 :  ret = 1;

		// 해당 스킬 캐릭터 데이터 참조 ( 음악스킬 ) 
	case 8 :
	case 9 :
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	default :	ret = 1;
		break;
	}

	if( ret < 0 ) ret = 0;
	if( ret > 100 ) ret = 100;
	return ret;
}

int ReturnSkillProbablity(	const int cn, const int skillno, const int x, const int y, 
							const int targettype, const int target_id,CItem_Join *joinitem = NULL )
{	
	if( cn >= 10000 ) return 0;
	
	const int needmin = skillProb_needmin( skillno, target_id, joinitem );//니드미니멈 받아오는곳! //this2 lsw
	const int needmax = skillProb_needmax( skillno, target_id, joinitem );
	int base = needmax - needmin;
	
	if( base < 0 ) 
	{
		return 0; 
	}
	if( base <= 0 ) 
	{
		base = 1;
	}

	int mybase = connections[ cn].chrlst.skillexp[ SkillTbl[skillno].MotherSkillType].skillexp / 10000 -  needmin;

	double k = (double)mybase / (double)base;

	if(k>1) k=1;
	
	const int k1 =  skillProb_k1( skillno, joinitem );
	const int k2 =  skillProb_k2( skillno, joinitem );	
	
	int hitrate =  (int)(k * k1 + k2);
	
	if( hitrate < 0 )	hitrate = 0;
	if( hitrate > 100 ) hitrate = 100;

	AddSkillExp( cn, skillno, hitrate );

	return hitrate;
}	

void SendSkillResultFail( const int cn, const int iSkillNo, const int iWhy = 0 )
{	
	t_packet p;
	p.h.header.type = CMD_SKILL_RESULT_FAIL;
	p.u.server_skill_result_fail.skillno	= iSkillNo;
	p.u.server_skill_result_fail.why		= iWhy;
	p.h.header.size = sizeof( t_server_skill_result_fail );
	QueuePacket( connections, cn, &p, 1);
}	

void SendViewCharInfo(const int cn, int target_id, int type, DWORD data, char *killer)
{
	t_packet p;
	int len;

	p.h.header.type = CMD_VIEW_CHAR_INFO;
	p.u.view_char_info.server_id= target_id;
	p.u.view_char_info.type		= type;
	p.u.view_char_info.data		= data;
	if( killer )
	{
		if( killer[0] )	EatRearWhiteChar( killer );
		len = strlen( killer );
	}
	else len = 0;

	if( killer != NULL ) strcpy_s( p.u.view_char_info.killer, sizeof(p.u.view_char_info.killer), killer );
	else	p.u.view_char_info.killer[0] = 0;

	p.h.header.size = sizeof( t_view_char_info ) - ( 31 - len );

	QueuePacket( connections, cn, &p, 1);
}

//1013 zhh 통나무와 목재에만 불이 붙는 것을 바꾼다
int skill_Makefire_preCheck	(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id )
{	
	
	int ret = ReturnSkillProbablity( cn, skillno, x, y, targettype, target_id );
	if( ret == 0 ) { SendSkillResultFail(cn, skillno); return 0; }

	switch( ItemList[target_id].item_no  )
	{
	case 1011	:	//	<Log>	<통나무>
	case 1012	:	//	<Timber>	<목재>	
	case 1023	:	//	<Nutmeg Log>	<비자나무>
	case 1024	:	//	<나무>	<리기다나무>
	case 1025	:	//	<Fir Log>	<구상나무>
	case 1026	:	//	<Oak Log>	<상수리나무>
	case 1027	:	//	<나무>	<거제수나무>
	case 1028	:	//	<Beech Log>	<너도밤나무>
	case 1029	:	//	<Whitebirch Log>	<자작나무 >
	case 1030	:	//	<Nutmeg Timber>	<비자나무 목재>
	case 1031	:	//	<나무>	<리기다나무 목재>
	case 1032	:	//	<Fir Timber>	<구상나무 목재>
	case 1033	:	//	<Oak Timber>	<상수리나무 목재>
	case 1034	:	//	<나무>	<거제수나무 목재>
	case 1035	:	//	<Beech Timber>	<너도밤나무 목재>
	case 1036	:	//	<Whitebirch Timber>	<자작나무 목재>
		break;
		
	default :
				SendSkillResultFail(cn, skillno);
				return 0;		
	} 
	
	if( ( ItemList[ target_id].attr[ IATTR_ATTR] & IA2_FIRE ) == 0 )
	{
		ItemList[ target_id].attr[ IATTR_ATTR] |= IA2_NOTMOVE;
		SetArea( CHANGE_IA2_ATTR_ITEM_AREA, target_id );
	}
				
	return 1; 
}	

void PrintSkillItemJoin( void  )
{			
	FILE *fp;
	CItem_Join *s;
	int i;
	int no;
			
	fp = fopen( "./OutPut/WhatNeededToMakeItem.txt", "wt");
	if( fp == NULL ) return;
			
	for( i = 0 ; i < TotalItemJoinTableCount ; i ++)
	{		
		s = &item_join[i];
			
		no = s->item_id;
		CItem *ii = ItemUnit( no/1000, no%1000 );
			
		if( ii == NULL ) 
		{	
			fprintf( fp, "\n %d에 있는 Item은 사용되지 않는 Item이가요? \n\n", no ); 
			continue;
		}	
			
		fprintf( fp, "'%s(%d)'을 얻을려면 '%s(%d)기술이 필요.\n", ii->GetHanName(), no, 
						SkillTbl[s->need_skill1].Skill_Minute, s->need_skill1 );
		
			
		fprintf( fp, "        필요한 기술은 ");
		if( s->need_skill1 )	fprintf( fp,"'%30s(%7d)'( %d - %d )\n", SkillTbl[s->need_skill1].Skill_Minute, s->need_skill1, s->skill1_minimum, s->skill1_maximum );
		if( s->need_skill2 )	fprintf( fp, "                        '%30s(%7d)'( %d - %d )\n", SkillTbl[s->need_skill2].Skill_Minute, s->need_skill2,  s->skill2_minimum, s->skill2_maximum );

		if( s->need_skill1 == 0 && s->need_skill2 == 0 ) 
		{
		    fprintf( fp, "없습니다. \n" );
		}
				
		if( s->resource1 == 0 && s->resource2 == 0 && s->resource3 == 0 && s->resource4 == 0)
		{		
			fprintf( fp, " 이기술은 아이템이 필요가 없습니다. \n" );
		}		
		else	
		{		
//			fprintf( fp, "        필요한 Item 은  " );

			if( s->resource1 )
				if( s->resource1_amount ) 
				{
					ii = ItemUnit( s->resource1/1000, s->resource1%1000 );
					if( ii ) fprintf( fp, "        '%15s(%7d)'  %d개 필요\n", ii->GetHanName(), s->resource1, s->resource1_amount );
				}
			if( s->resource2 )
				if( s->resource2_amount ) 
				{
					ii = ItemUnit( s->resource2/1000, s->resource2%1000 );
					if( ii ) fprintf( fp, "        '%15s(%7d)'  %d개 필요\n", ii->GetHanName(), s->resource2, s->resource2_amount );
				}
			if( s->resource3 )
				if( s->resource3_amount ) 
				{
					ii = ItemUnit( s->resource3/1000, s->resource3%1000 );
					if( ii ) fprintf( fp, "        '%15s(%7d)'  %d개 필요\n", ii->GetHanName(), s->resource3, s->resource3_amount );
				}
			if( s->resource4 )
				if( s->resource4_amount ) 
				{
					ii = ItemUnit( s->resource4/1000, s->resource4%1000 );
					if( ii ) fprintf( fp, "        '%15s(%7d)'  %d개 필요\n", ii->GetHanName(), s->resource4, s->resource4_amount );
				}
		}		
		
		switch( s->Tool_kind )
		{
			case 48	:		fprintf( fp, "        필요로하는 하는 Tool 종류는 : 도구 개간용(48)\n" );		break;
			case 49	:		fprintf( fp, "        필요로하는 하는 Tool 종류는 : 도구 추수용(49)\n");		break;
			case 50	:		fprintf( fp, "        필요로하는 하는 Tool 종류는 : 도구 채광용(50)\n");		break;
			case 51	:		fprintf( fp, "        필요로하는 하는 Tool 종류는 : 도구 제련용(51)\n");		break;
			case 52	:		fprintf( fp, "        필요로하는 하는 Tool 종류는 : 도구 낚시용(52)\n");		break;
			case 53	:		fprintf( fp, "        필요로하는 하는 Tool 종류는 : 도구 벌목용(53)\n");		break;
			case 54	:		fprintf( fp, "        필요로하는 하는 Tool 종류는 : 도구 목재가공용(54)\n");	break;
			case 55	:		fprintf( fp, "        필요로하는 하는 Tool 종류는 : 도구 도축용(55)\n");		break;
			case 56	:		fprintf( fp, "        필요로하는 하는 Tool 종류는 : 도구 약초채집용(56)\n");	break;
			case 57	:		fprintf( fp, "        필요로하는 하는 Tool 종류는 : 도구 약재만들기용(57)\n");	break;
			case 58	:		fprintf( fp, "        필요로하는 하는 Tool 종류는 : 도구 가위류(58)\n");		break;
			case 59	:		fprintf( fp, "        필요로하는 하는 Tool 종류는 : 도구 목공용(59)\n"); break;
			case 60	:		fprintf( fp, "        필요로하는 하는 Tool 종류는 : 도구 철공용(60)\n"); break;
			case 61	:		fprintf( fp, "        필요로하는 하는 Tool 종류는 : 도구 대우는요리용(쿠커)(61)\n"); break;
			case 62	:		fprintf( fp, "        필요로하는 하는 Tool 종류는 : 도구 써는요리용(62)\n"); break;
			case 63	:		fprintf( fp, "        필요로하는 하는 Tool 종류는 : 도구 바늘류(63)\n"); break;
			case 64	:		fprintf( fp, "        필요로하는 하는 Tool 종류는 : 도구 활제작용(64)\n"); break;
			case 65	:		fprintf( fp, "        필요로하는 하는 Tool 종류는 : 도구 불대(65)\n"); break;
			case 66	:		fprintf( fp, "        필요로하는 하는 Tool 종류는 : 도구 양초제작용(66)\n"); break;
			case 67	:		fprintf( fp, "        필요로하는 하는 Tool 종류는 : 도구 밀납가공용(67)\n"); break;
			case 68	:		fprintf( fp, "        필요로하는 하는 Tool 종류는 : 도구 테이밍용(68)\n"); break;
			case 69	:		fprintf( fp, "        필요로하는 하는 Tool 종류는 : 도구 그물류(69)\n"); break;
			case 70	:		fprintf( fp, "        필요로하는 하는 Tool 종류는 : 도구 부싯돌류(70)\n"); break;

			default : 		fprintf( fp, "        필요로하는 하는 Tool 종류는 : 없음..\n" ); break;
		}

		fprintf( fp, "        필요한 장소는  " );
		switch( s->Flatform_kind )
		{
			case 0 :  fprintf( fp, "'아무데나' 이다. \n");			break;

			case 71	:		fprintf( fp, " 작업대 용광로(71)\n"); break;
			case 72	:		fprintf( fp, " 작업대 배틀(72)\n"); break;
			case 73	:		fprintf( fp, " 작업대 선반(73)\n"); break;
			case 74	:		fprintf( fp, " 작업대 모루(74)\n"); break;
			case 75	:		fprintf( fp, " 작업대 화덕(75)\n"); break;
			case 76	:		fprintf( fp, " 작업대 시험관류(76)\n"); break;
			case 77	:		fprintf( fp, " 작업대 물레(77)\n"); break;
			case 78	:		fprintf( fp, " 작업대 불대 (78)\n"); break;

			default : fprintf( fp, "'테이블값이 잘못된것같다.\n" );	break;
		}

		fprintf( fp, "---------------------------------------------------------------------------------------------\n" );
	}		
	fclose(fp);
}

void CItem_Join::ReadItemJoin( char *buf )
{	
	char *token;
	int *target;
	token = strtok(buf,"\t\n\r");
	if( token == NULL ) return;
	no = atoi(token);
	
	token = strtok(NULL,"\t\n\r");
	if( token == NULL ) return;
	item_id = atoi(token);
	
	token= strtok(NULL, "\t\n");
	if( token == NULL ) return;
	strcpy_s(item_name, sizeof(item_name), token);

	token = strtok(NULL,"\t\n\r");
	if( token == NULL ) return;
	item_amount = atoi(token);
	
	token= strtok(NULL, "\t\n");
	if( token == NULL ) return;
	strcpy_s(han_name, sizeof(han_name), token);
	
	target = &need_know;
	int ct=0;
	while( ( token= strtok(NULL, "\t\n\r")) != NULL )
	{
		*(target+ct) = atoi(token);
		ct++;
	}
}

bool LoadItemJoinTable( void )
{			
	TotalItemJoinTableCount=0;

	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
	char		szQuerry[256];
			
	sprintf_s(szQuerry, sizeof(szQuerry), "Select * From itemjoin order by no#" );
			
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{		
		int i = 0;
		retCode = SQLFetch(hStmt);
		while( retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
		{	
			retCode = SQLGetData(hStmt, 1, SQL_C_LONG, &item_join[i].no,		0, &cbValue);
			retCode = SQLGetData(hStmt, 2, SQL_C_LONG, &item_join[i].item_id,	0, &cbValue);
			item_join[i].item_id/=100;
			retCode = SQLGetData(hStmt, 3, SQL_C_CHAR, &item_join[i].item_name	,31, &cbValue);
			retCode = SQLGetData(hStmt, 4, SQL_C_LONG, &item_join[i].item_amount	,0, &cbValue);
			retCode = SQLGetData(hStmt, 5, SQL_C_CHAR, &item_join[i].han_name	,31, &cbValue);
			retCode = SQLGetData(hStmt, 6, SQL_C_LONG, &item_join[i].need_know	,0, &cbValue);
			retCode = SQLGetData(hStmt, 7, SQL_C_LONG, &item_join[i].teach_place	,0, &cbValue);
			retCode = SQLGetData(hStmt, 8, SQL_C_LONG, &item_join[i].need_skill1	,0, &cbValue);
			retCode = SQLGetData(hStmt, 9, SQL_C_LONG, &item_join[i].skill1_minimum	,0, &cbValue);
			retCode = SQLGetData(hStmt,10, SQL_C_LONG, &item_join[i].skill1_maximum	,0, &cbValue);
			retCode = SQLGetData(hStmt,11, SQL_C_LONG, &item_join[i].need_skill2	,0, &cbValue);
			retCode = SQLGetData(hStmt,12, SQL_C_LONG, &item_join[i].skill2_minimum	,0, &cbValue);
			retCode = SQLGetData(hStmt,13, SQL_C_LONG, &item_join[i].skill2_maximum	,0, &cbValue);
			retCode = SQLGetData(hStmt,14, SQL_C_LONG, &item_join[i].tech_price	,0, &cbValue);
			retCode = SQLGetData(hStmt,15, SQL_C_LONG, &item_join[i].Tool_kind	,0, &cbValue);
			retCode = SQLGetData(hStmt,16, SQL_C_LONG, &item_join[i].Flatform_kind	,0, &cbValue);
			retCode = SQLGetData(hStmt,17, SQL_C_LONG, &item_join[i].resource1	,0, &cbValue);item_join[i].resource1/=100;
			retCode = SQLGetData(hStmt,18, SQL_C_LONG, &item_join[i].resource1_amount	,0, &cbValue);
			retCode = SQLGetData(hStmt,19, SQL_C_LONG, &item_join[i].resource2	,0, &cbValue);item_join[i].resource2/=100;
			retCode = SQLGetData(hStmt,20, SQL_C_LONG, &item_join[i].resource2_amount	,0, &cbValue);
			retCode = SQLGetData(hStmt,21, SQL_C_LONG, &item_join[i].resource3	,0, &cbValue);item_join[i].resource3/=100;
			retCode = SQLGetData(hStmt,22, SQL_C_LONG, &item_join[i].resource3_amount	,0, &cbValue);
			retCode = SQLGetData(hStmt,23, SQL_C_LONG, &item_join[i].resource4	,0, &cbValue);item_join[i].resource4/=100;
			retCode = SQLGetData(hStmt,24, SQL_C_LONG, &item_join[i].resource4_amount	,0, &cbValue);
			retCode = SQLGetData(hStmt,25, SQL_C_LONG, &item_join[i].Result_resource1	,0, &cbValue);
			retCode = SQLGetData(hStmt,26, SQL_C_LONG, &item_join[i].Result_resource1_amount	,0, &cbValue);
			retCode = SQLGetData(hStmt,27, SQL_C_LONG, &item_join[i].Result_resource2	,0, &cbValue);
			retCode = SQLGetData(hStmt,28, SQL_C_LONG, &item_join[i].Result_resource2_amount	,0, &cbValue);
			retCode = SQLGetData(hStmt,29, SQL_C_LONG, &item_join[i].reutilizerble	,0, &cbValue);
			retCode = SQLGetData(hStmt,30, SQL_C_LONG, &item_join[i].k1	,0, &cbValue);
			retCode = SQLGetData(hStmt,31, SQL_C_LONG, &item_join[i].k2	,0, &cbValue);
			retCode = SQLGetData(hStmt,32, SQL_C_CHAR, &item_join[i].txt	,220, &cbValue);
			TotalItemJoinTableCount= i;
			i++;
			
		retCode = SQLFetch(hStmt);
		if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
			{
			}
			
		else if( retCode == SQL_NO_DATA ) 
			{
				break;
			}
		
		else
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return 0;
			}

		}		
	TotalItemJoinTableCount= i; //010618 lsw 위에서 이동
	}			
				
	SQLFreeStmt(hStmt, SQL_DROP);
			
	PrintSkillItemJoin();
				
	return 1;	
}

bool LoadItemMutantKind()
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
	char		szQuerry[256];
			
	sprintf_s(szQuerry, sizeof(szQuerry), "Select * From Item_mutant_kind order by item_kind" );
			
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{		
		int i = 0;
		retCode = SQLFetch(hStmt);
		while( retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
		{
			if(i >= MAX_ITEM_MUTANT_KIND) return false;
			int column = 0;
			retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&ItemMutantKind[i].item_kind,	0, &cbValue);
			//< CSD-030324
			//retCode = SQLGetData(hStmt, ++column, SQL_C_CHAR,	&ItemMutantKind[i].exp_kind,	100, &cbValue);
			++column;
			//> CSD-030324
			retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&ItemMutantKind[i].upgrade_type,	0, &cbValue);
			int count = 0;
			for(count = 0; count <4; count ++)
			{
				int iDummy = 0;
				retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&iDummy,	0, &cbValue);
				ItemMutantKind[i].AddItem[count]= iDummy;
			}
			retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&ItemMutantKind[i].iBuffer1,	0, &cbValue);
			retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&ItemMutantKind[i].iBuffer2,	0, &cbValue);
			retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&ItemMutantKind[i].iBuffer3,	0, &cbValue);
			retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&ItemMutantKind[i].iBuffer4,	0, &cbValue);

			i++;
			retCode = SQLFetch(hStmt);
			if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
			{
			}
			else if( retCode == SQL_NO_DATA ) 
			{
				break;
			}
			else
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return false;
			}
		}	
	}		
	else 
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return 0;
	}
	SQLFreeStmt(hStmt, SQL_DROP);
	return true;	
}

bool LoadItemGrade()
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
			
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)"Select * From Item_grade order by grade", SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{		
		int i = 0;
		retCode = SQLFetch(hStmt);
		while( retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
		{
			if(i >= MAX_ITEM_GRADE) return false;
/*
		typedef struct ITEMGRADE
		{
			int iGrade;
			int iAble;
			int ResourceAmount[4];
			int iBuffer1;
			int iBuffer2;
			int iBuffer3;
			int iBuffer4;
		}t_ItemGrade,*LPITEM_GRADE;
*/
			int column = 0;
			retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&ItemGrade[i].iGrade,	0, &cbValue);
			retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&ItemGrade[i].iAble,	0, &cbValue);
			int count = 0;
			for(count = 0; count <4; count ++)
			{
				int iDummy = 0;
				retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&iDummy,	0, &cbValue);
				ItemGrade[i].ResourceAmount[count]= iDummy;
			}
			retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&ItemGrade[i].iBuffer1,	0, &cbValue);
			retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&ItemGrade[i].iBuffer2,	0, &cbValue);
			retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&ItemGrade[i].iBuffer3,	0, &cbValue);
			retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&ItemGrade[i].iBuffer4,	0, &cbValue);

			i++;
			retCode = SQLFetch(hStmt);
			if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
			{
			}
			else if( retCode == SQL_NO_DATA ) 
			{
				break;
			}
			else
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return false;
			}
		}	
	}
	else 
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return 0;
	}
	SQLFreeStmt(hStmt, SQL_DROP);
	return true;	
}

bool LoadItemRare( void )
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
			
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)"Select * From Item_rare order by [NO]", SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{		
		int i = 0;
		retCode = SQLFetch(hStmt);
		while( retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
		{
			if(i >= MAX_ITEM_RARE) return false;
/*
			typedef struct ITEM_MUTANT
			{
				int		iNo		;
				int		iType	;
				char	EngName[100];
				char	LocalName[100];
				char	Explain[100];
				char	ExplainAdd[100];
				int		iExpMark;// + - 기호 표시
				int		iMakeAble;//만들어 지는가?
				int		RaisePro[8];// 0번은 DefaultRaisePro
				int		iUpgradeAble;
				int		iAbleNation;//
				int		GRADE[16];//0번은 MAX 나머지는 그레이드별 값
				int		Buffer1;
				int		Buffer2;
				int		Buffer3;
				int		Buffer4;
				int		Buffer5;
				int		Buffer6;
			}t_Item_Mutant,*LPITEM_MUTANT;
*/
			int column = 0;
			retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&ItemRare[i].iNo,	0, &cbValue);
			retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&ItemRare[i].iType,	0, &cbValue);
			retCode = SQLGetData(hStmt, ++column, SQL_C_CHAR,	&ItemRare[i].EngName,	100, &cbValue);
			retCode = SQLGetData(hStmt, ++column, SQL_C_CHAR,	&ItemRare[i].LocalName,	100, &cbValue);
			retCode = SQLGetData(hStmt, ++column, SQL_C_CHAR,	&ItemRare[i].Explain,	100, &cbValue);
			retCode = SQLGetData(hStmt, ++column, SQL_C_CHAR,	&ItemRare[i].ExplainAdd,	100, &cbValue);
			retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&ItemRare[i].iExpMark,	0, &cbValue);
			retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&ItemRare[i].iMakeAble,	0, &cbValue);
			int count = 0;
			for(count = 0; count <8; count ++)
			{
				int iDummy = 0;
				retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&iDummy,	0, &cbValue);
				ItemRare[i].RaisePro[count]= iDummy;
			}
			retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&ItemRare[i].iUpgradeAble,	0, &cbValue);
			retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&ItemRare[i].iAbleNation,	0, &cbValue);
							
			for(count= 0; count <16; count ++)
			{
				int iDummy = 0;
				retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&iDummy,	0, &cbValue);
				ItemRare[i].GRADE[count]= iDummy;
			}

			i++;
			retCode = SQLFetch(hStmt);
			if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
			{
			}
			else if( retCode == SQL_NO_DATA ) 
			{
				break;
			}
			else
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return false;
			}
		}	
	}		
	else 
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return 0;
	}
	return true;	
}

bool LoadItemTable( void )//020815 lsw
{	//< CSD-038006
	if( !LoadItemRare() )			{return false;}
	if( !LoadItemMutantKind()	)	{return false;}
	if( !LoadItemGrade()		)	{return false;}
	if( !LoadRaregroup()		)	{return false;}
	if( !LoadItemFunctionGroup())	{return false;}
	if( !LoadRareProbability()	)	{return false;}
	if( !LoadItemFunction()		)	{return false;}
	if( !LoadLearnItemConvetrer())	{return false;}
	if( !LoadRareItemBag())			{return false;}//020505 lsw
	if( !LoadItemControlPercent())	{return false;}
	if( !LoadItemFallPercent())		{return false;}
	if( !LoadBasicItemBag())		{return false;}
	if( !LoadItemControl())			{return false;}
	if( !RareEM.LoadItem_Multi_Rare_Table()){return false;}//021111 lsw 
	if( !RareEM.LoadItem_Legend_Table()){return false;}//021111 lsw 
	if( !RareEM.LoadItem_Legend_Bonus_Item()){return false;}//021111 lsw 
	
	return true;
}	//> CSD-038006

bool youvegotitInventory(const int cn, ItemAttr *item )
{
	CHARLIST *ch = GetCharListPtr(cn);
	if (!ch) return false;
	int  i,j,m;
	if( item == NULL ) return false;
	CItem *ii = ItemUnit( item->item_no/1000, item->item_no % 1000 );	// 그런 Item이 Table에 있는가?
	if( ii == NULL ) return false;

	ItemAttr *tempitem;

	for( i = 0 ; i < 3 ; i ++)
		for( j = 0 ; j < 3 ; j ++)
			for( m = 0 ; m < 8 ; m ++ )
			{
				tempitem = &ch->inv[i][j][m];
				if( tempitem->item_no  == item->item_no	&& 
					tempitem->attr[ 0] == item->attr[0] &&
					tempitem->attr[ 1] == item->attr[1] &&
					tempitem->attr[ 2] == item->attr[2] &&
					tempitem->attr[ 3] == item->attr[3] &&
					tempitem->attr[ 4] == item->attr[4] &&
					tempitem->attr[ 5] == item->attr[5]  )
				{
					// amount ..
					return true;
				}
			}

	for( i = 0 ; i < 8 ; i ++)
	{
		tempitem = &ch->equip[i];
		if( tempitem->item_no  == item->item_no	&& 
			tempitem->attr[ 0] == item->attr[0] &&
			tempitem->attr[ 1] == item->attr[1] &&
			tempitem->attr[ 2] == item->attr[2] &&
			tempitem->attr[ 3] == item->attr[3] &&
			tempitem->attr[ 4] == item->attr[4] &&
			tempitem->attr[ 5] == item->attr[5]  )
		{
					// amount ..
				return true;
		}
	}

	for( i = 0 ;  i < 7 ; i ++)
	{
		tempitem = &ch->quick[i];
		if( tempitem->item_no  == item->item_no	&& 
			tempitem->attr[ 0] == item->attr[0] &&
			tempitem->attr[ 1] == item->attr[1] &&
			tempitem->attr[ 2] == item->attr[2] &&
			tempitem->attr[ 3] == item->attr[3] &&
			tempitem->attr[ 4] == item->attr[4] &&
			tempitem->attr[ 5] == item->attr[5]  )
		{
					// amount ..
				return true;
		}
	}

	tempitem = &ch->handheld;
	if( tempitem->item_no  == item->item_no	&& 
		tempitem->attr[ 0] == item->attr[0] &&
		tempitem->attr[ 1] == item->attr[1] &&
		tempitem->attr[ 2] == item->attr[2] &&
		tempitem->attr[ 3] == item->attr[3] &&
		tempitem->attr[ 4] == item->attr[4] &&
		tempitem->attr[ 5] == item->attr[5]  )
	{
				// amount ..
			return true;
	}
	
	return false;
}

bool youvegotitInventory(const int cn, int target_id )
{
	CHARLIST *ch = GetCharListPtr(cn);
	if( !ch ) return false;	// 020620 YGI
	int  i,j,m;

	for( i = 0 ; i < 3 ; i ++)
		for( j = 0 ; j < 3 ; j ++)
			for( m = 0 ; m < 8 ; m ++ )
				if( ch->inv[i][j][m].item_no == target_id ) 
				{
					return true;
				}

	for( i = 0 ; i < 8 ; i ++)
		if( ch->equip[i].item_no == target_id )	
				{
					return true;
				}
	for( i = 0 ;  i < 7 ; i ++)
		if( ch->quick[i].item_no == target_id ) 
				{
					return true;
				}

	if( ch->handheld.item_no == target_id ) 
				{
					return true;
				}
	
	return false;
}
			
CItem_Join *ReturnItemJoinTable(const int cn, int skillno, int target_id )
{			
	int i;	
	for( i = 0 ; i < TotalItemJoinTableCount ; i ++)
	{		
		if( item_join[i].item_id == target_id && item_join[i].need_skill1 == skillno )
		{		
			return 	&item_join[i];
		}	
	}		
			
	return NULL;
}			
	
int AddSkillExp2Exp(const int cn, int skillno, int hitrate )
{										
	int fix;					
	int inc;						
	int lev;					
	CHARLIST *ch;				
								
	ch = &connections[ cn].chrlst;
	if (!ch) return 0;
								
	int kind = SkillTbl[skillno].MotherSkillType;		// 0527 YGI
	lev = ch->skillexp[kind].skillexp / 10000;
	if( lev < 1 )	lev = 1;	
	if( lev > 100 ) lev = 100;	
								
	//1010 zhh	경험치 테이블에	서 읽어 들여서 처리한다.이미 테이블을 읽어 들이고 있지만 일단 이걸로 고쳐둔다. 새로운 테이블이라고
	//생각된다.					
	inc = SkillExp[hitrate][lev];

								
	fix = SkillTbl[ skillno].Ability_Fix;  // 0 <= fix <= SKILL_ABILITY_FIX_
	inc = inc * fix / SKILL_EXP_FIX_;
  
	ch->ChangeUpAddExp(inc); // CSD-TW-030624
	isLevelUp(ch);
	return 1;					
}								
								
//-------------------------------------------------------------------------------------------------------------------------
// 기능 :  
// 인  자 : 0 <= hitrate <= 100
// 리턴값 : 
//-------------------------------------------------------------------------------------------------------------------------
int AddSkillExp(const int cn, int skillno, int hitrate )
{	
	int expadd;
	int fix;
	bool myjob =false;

	switch( connections[ cn].chrlst.Job )
	{
	case J_FAMER		:	 if( SkillTbl[skillno].Select_Farmer		== 1 )		 myjob = true;		break;
	case J_MINNER		:	 if( SkillTbl[skillno].Select_miner			== 1 )		 myjob = true;		break;
	case J_FISHER		:	 if( SkillTbl[skillno].Select_fisher		== 1 )		 myjob = true;		break;
	case J_LUMBERJACK	:	 if( SkillTbl[skillno].Select_Lumberjack	== 1 )		 myjob = true;		break;
	case J_BUTCHER		:	 if( SkillTbl[skillno].Select_Butcher		== 1 )		 myjob = true;		break;
	case J_HERB_DIGGER	:	 if( SkillTbl[skillno].Select_Herbdigger	== 1 )		 myjob = true;		break;
	case J_HERDSMAN		:	 if( SkillTbl[skillno].Select_herdman		== 1 )		 myjob = true;		break;
	case J_CARPENTER	:	 if( SkillTbl[skillno].Select_Carpenter		== 1 )		 myjob = true;		break;
	case J_BLACKSMITH	:	 if( SkillTbl[skillno].Select_Blacksmith	== 1 )		 myjob = true;		break;
	case J_COOKER		:	 if( SkillTbl[skillno].Select_Cooking		== 1 )		 myjob = true;		break;
	case J_TAILOR		:	 if( SkillTbl[skillno].Select_Tailoring		== 1 )		 myjob = true;		break;
	case J_BOWCRAFT		:	 if( SkillTbl[skillno].Select_Bowcraft_Fletcher == 1 )	 myjob = true;		break;
	case J_ALCHEMIST	:	 if( SkillTbl[skillno].Select_Alchemy		== 1 )		 myjob = true;		break;
	case J_CANDLEMAKER	:	 if( SkillTbl[skillno].Select_Candlemaker	== 1 )		 myjob = true;		break;
	case J_MERCHANT		:	 if( SkillTbl[skillno].Select_Merchant		== 1 )		 myjob = true;		break;
	}

	if( myjob == true ) // 지금 사용하는 기술이...
	{}
	
	fix = SkillTbl[ skillno].Ability_Fix;  // 0 <= fix <= SKILL_ABILITY_FIX_
	
	//1010 zhh		스킬레벨과 확률에 따라서 얻어지는 경험치가 다르다.
	int skilllevel=connections[ cn].chrlst.skillexp[SkillTbl[skillno].MotherSkillType].skillexp/10000;
	if(skilllevel<0)	skilllevel=0;		//안전장치들
	if(skilllevel>100)	skilllevel=100;		//안전장치들
	if(hitrate<0)		hitrate=0;			//안전장치들
	if(hitrate>100)		hitrate=100;		//안전장치들
	expadd = fix * SkillLevelExp[hitrate][skilllevel];	
//	expadd = fix * ( 1 +(100 - hitrate)*(100 - hitrate) / 30 );  // 0607 
	//1010 zhh		스킬레벨과 확률에 따라서 얻어지는 경험치가 다르다.여기까지

	expadd /= SKILL_ABILITY_FIX_;
	
	int inc = SkillTbl[skillno].MotherSkillType;										// 0527 YGI
	//001214 zhh	//아래에 있던 경험치 주는 루틴을 위로 끌어 올렸다.스킬 경험치는 안올라도 경험치는 오른도록 수정.
	AddSkillExp2Exp( cn, skillno, hitrate );

	if (connections[cn].chrlst.skillexp[inc].skillexp >= MAX_JOB_LEVEL*10000)
	{	//< CSD-040223
		return 1;
	}	//> CSD-040223

	//001214 zhh	//gm일 경우는 경험치를 깍으면 안된다.
	//011012 lsw >
	if( connections[ cn].chrlst.skillexp[ inc].skillexp > 999999 ) 
	{
	}
	else
	{
		CHARLIST *ch = CheckServerId(cn);
		int FoundPill = 0;
		if(!ch)FoundPill = SearchItemByInv(10292,ch);
		if( FoundPill==1 ) connections[ cn].chrlst.skillexp[ inc].skillexp += expadd * 3; //150% job exp - Thralas + Eleval 19/08/09 - Added dbl exp for jobs if have smp pill
		else connections[ cn].chrlst.skillexp[ inc].skillexp += expadd * 1.5; //Eleval 19/08/09 - Added dbl exp for jobs if have smp pill
		if( connections[ cn].chrlst.skillexp[ inc].skillexp > 999999 ) 
		{
			connections[ cn].chrlst.skillexp[ inc].skillexp = 999999;
			return 1;
		}
	}

	//011022 lsw >
	SkillMgr.SendSkillExp( inc, connections[ cn].chrlst.skillexp[inc], cn);
	//011022 lsw <
	
	return 1;
}	
/**************************************************************************************************
	
  SkillTbl[skillno].MotherSkillExp : 성공률 계산 의 형태 
	
(2000.6.3 16:25)
	
			(나의 능력(mybase/10000) - 최소치(needmin))
 성공률 = ----------------------------------------------   *  k1  +  K2
			 (최대치(needmax)    -    최소치(needmin))
	
	mybase = connections[ cn].chrlst.skillexp[ 45 ];   ( 0 =< n< 1,000,000 ) 
			-->경험치로 계산하고 Level을 출력할때,  / 10000으로 한다.


***************************************************************************************************/

int ReturnTamingPoint(const int cn , int skillno, int target_id, CItem_Join *joinitem = NULL )
{	
	int tame_add;
	int base, mybase;
	int needmin, needmax;
	int k1,k2;
	
	if( cn >= 10000 ) return 0;
	
	needmin = skillProb_needmin( skillno, target_id, joinitem );
	needmax = skillProb_needmax( skillno, target_id, joinitem );
	base = needmax - needmin;
	
	if( base < 0 ) return 0;	// 에러검출루틴..
	if( base <= 0 ) base = 1;	// min == max인경우 ?

	mybase = connections[ cn].chrlst.skillexp[ SkillTbl[skillno].MotherSkillType].skillexp / 10000 -  needmin;

	double k = (double)mybase / (double)base;
	
	k1 =  skillProb_k1( skillno, joinitem );
	k2 =  skillProb_k2( skillno, joinitem );	
	
	int t = k * k1 + k2;
	if( t > 100 ) t = 100;

	tame_add=  SkillTbl[ skillno].Produce_Fix * t / 10000;
	return tame_add;
}	

int ReturnSkillProbablityNoExpAdd(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id, CItem_Join *joinitem = NULL )
{	
	int hitrate;
	int base, mybase;
	int needmin, needmax;
	int k1,k2;
	
	if( cn >= 10000 ) return 0;
	
	needmin = skillProb_needmin( skillno, target_id, joinitem );
	needmax = skillProb_needmax( skillno, target_id, joinitem );
	base = needmax - needmin;
	
	if( base < 0 ) return 0; 
	if( base <= 0 ) base = 1;

	mybase = connections[ cn].chrlst.skillexp[ SkillTbl[skillno].MotherSkillType].skillexp / 10000 -  needmin;

	double k = (double)mybase / (double)base;

	if(k>1)	k=1;
	
	k1 =  skillProb_k1( skillno, joinitem );
	k2 =  skillProb_k2( skillno, joinitem );


	hitrate =  (int)(k * k1 + k2);
	
	if( hitrate < 0 )	hitrate = 0;
	if( hitrate > 100 ) hitrate = 100;
	
	return hitrate;
}	
	
void SkillTargetTypeItemMake(const int cn, t_client_skill_info *sp)
{	
	t_packet p;
	ItemAttr item = {0,};
	CItem_Join join;

	CHARLIST *ch= GetCharListPtr(cn);
	if( !ch ) return;	// 020620 YGI

	unsigned int iAttr3		= 0;
	int RareType		= 0;
	int SuccessMuch		= 0;//성공 수량
	int iTryRare		= sp->s.p1;
	int iRareGroup		= sp->s.p2;
	int iTryMakeCount	= sp->s.p3;

	if( iTryMakeCount > 10 )
	{
		return;
	}

	const int  skillno		= sp->skillno;
	const int  x			= sp->x;
	const int  y			= sp->y;
	const int  targettype	= sp->targettype;
	const int  target_id	= sp->target_id;

	const int check = SkillMgr.CheckYouCanSkillIt( cn, join, skillno, x, y, target_id );//순서 주의.. //여기서 Join을 변경 시켜준다
	const int hitrate = ReturnSkillProbablityNoExpAdd( cn, skillno, x, y, targettype, target_id, &join );

	switch(iTryRare)
	{
	case 1:
		{
			iTryMakeCount = 1;//레어 시도를 하면 시도갯수는 1이 된다
		}break;
	case 21:
	case 31:
	case 41:
		{
		}break;
	default://레어 시도가 아니면 그룹에 값들어와도 지워 버린다
		{
			iRareGroup =0;
		}break;
	}

	switch( check )// 기술을 쓸수 있는 상황인지를 체크한다. 
	{
	case ITEMMAKE_YOUCAN:{break;}//나는 이 아이템을 만들 수 있다!
	default://만들 수 없다 
		{
			SendSkillResultFail( cn, skillno, check );
			return;
		}
		break;
	}

//	DecreaseEquipDuration(ch,(rand()%6+1)*iTryMakeCount,WT_WEAPON,0,false);//스킬툴 내구력 감소 //시도횟수 만큼

	//시간 체크 1개당 1.5 초 딜레이를 먹는다
	if( global_time < ch->dwSkillMakeitemTime )	{return;}
	else{	ch->dwSkillMakeitemTime = global_time+iTryMakeCount*1500;}

	for(int count = 0  ; count < iTryMakeCount ; count++)
	{
		int r = (rand()%100)+1;	// 성공확률을 계산한다. 
		if( r < hitrate ) 	{ 	SuccessMuch++;	}//성공한 갯수 구한다
	}
	
	int	amount[4]	={	join.resource1_amount,join.resource2_amount,join.resource3_amount,join.resource4_amount	};
	int	resource[4]	={	join.resource1,join.resource2,join.resource3,join.resource4	};
	int	resultresource[2]	={	join.Result_resource1,	join.Result_resource2	};

	// ItemJoin테이블에 있는 결과값대로 처리
	for ( int cou = 0 ; cou	< iTryMakeCount; cou++ ) //리소스 지워주고 병 같은거 떨궈 주기
	{//리소스는 시도한 횟수만큼 지운다
		HungryMuchAdd( ch , HT_SKILL );//020214 lsw
		int roll = 0; 
		for(; roll < 4; roll++)
		{
			for( int i = 0 ; i < amount[roll] ; i ++)
			{
				if (!DeleteMakeItemResource(ch, resource[roll]))
				{
					SendSkillResultFail( cn, skillno, ITEMMAKE_NOT_RESOURCE);
					return;
				}
				else//1 2 만 있는데
				{
					if( 0 < roll && 2 > roll ) //조인 1 2 만 결과 리소스가 존재 
					{
						if( resultresource[roll])
						{
						item = GenerateItem( resultresource[roll] );
						DropItem(	ch->X + (rand()%40) - 40, ch->Y + (rand()%40) - 40, &item);
						}
					}
				}
			}
		}
		AddSkillExp( cn, skillno, hitrate );//경험치 여러번 주기
	}
	
	//맥스 때문에 못만들면 아이템 안 사라지고,
	RareType = TryRareItem(cn,skillno,join,iTryRare,iRareGroup,iTryMakeCount,SuccessMuch);//레어 타입 받아오기

	if( !SuccessMuch )//실패 메세지
	{	
		RareType = 0;
		SendSkillResultFail( cn, skillno ); 
		if(iTryRare)
		{
			OutMessage(cn,2,259);//아이템이 실패하여 만들수 없음
			return;
		}
		else
		{
			OutMessage(cn,2,260);//아이템이 실패하여 만들수 없음
		}
	}

	if(iTryRare && RareType && SuccessMuch)
	{	
		if(iTryRare/10)
		{
			ItemMgr.MakeRareAttr(iAttr3,1,RareType,0,0,0,true);	
		}
		else
		{
			ItemMgr.MakeRareAttr(iAttr3,1,RareType,0,0,0,false);	
		}
	}

	if(sp->s.p1)
	{
		if(!RareType)
		{
			SendSkillResultFail( cn, skillno );
	 		return;
		}
	}

	for ( int iResultCount = 0 ; iResultCount< SuccessMuch; iResultCount++ ) 			
	{	// 기술의 결과물을 생성시킨다. 

		CItem *ii = ItemUnit( target_id );
		if( !ii ){return;}
		int iAmount = 0;
		if( ii->GetRbutton() == DIVIDE_ITEM )
		{	iAmount  = 1;	}
		else 
		{	iAmount = join.item_amount;	}

		for(int c = 0; c< iAmount;c++)
		{
			item = GenerateItem( target_id );//010618 lsw

			//011013 lsw <
			if(iAttr3)
			{
				ItemAttr *inv = SearchInv(ch);
				item.attr[IATTR_RARE_MAIN] = iAttr3;
				if(ii->GetRbutton() == DIVIDE_ITEM )
				{//아이템 로그 남기기 
					Send_RareItemMakeLog( cn, join.item_id, 
					((t_skillexp2*)&ch->skillexp[SkillTbl[skillno].MotherSkillType-14])->makecount
					,1, RareType,0,0,0,0,0,0,item.attr[IATTR_LIMIT],
					item.attr[IATTR_DURATION],0,100);
				}
				else
				{
					Send_RareItemMakeLog( cn, join.item_id, 
					((t_skillexp2*)&ch->skillexp[SkillTbl[skillno].MotherSkillType-14])->makecount
					,1, RareType,0,0,0,0,0,0,item.attr[IATTR_LIMIT],
					LOWORD(item.attr[IATTR_DURATION]),HIWORD(item.attr[IATTR_DURATION]),100);
				}
				
				if(inv)//인벤토리가 비었으면 
				{
					*inv = item;
					SendItemEventLog( inv, cn, SN_NOT_USER, SILT_MAKE_BY_SKILL, 1 ); //YGI acer
					SendServerEachItem( inv,ch);
					OutMessage(cn,2,524);
					continue;//아이템을 넣어 줬으니 바닥에는 아이템을 떨어뜨리지 않는다 
				}
			}
			SendItemEventLog( &item, cn, SN_NOT_USER, SILT_MAKE_TO_GROUND_BY_SKILL, 1 ); //YGI acer	// 따로 로그 타입을 정의한다.
			DropItem(	ch->X + (rand()%100) - 50, ch->Y + (rand()%100) - 50, &item);
		}
	}

	if(SuccessMuch)//성공 갯수 없으면 안보냄//020603 lsw
	{
		p.h.header.type = CMD_SKILL_ITEMMAKE_RESULT;
		{
			p.u.server_skill_itemmake_result.skillno = skillno;
			p.u.server_skill_itemmake_result.item = item;
			p.u.server_skill_itemmake_result.MakeItemCount  = SuccessMuch;
			p.u.server_skill_itemmake_result.TryItemCount	= iTryMakeCount;
		}							
		p.h.header.size = sizeof( t_server_skill_itemmake_result );
		QueuePacket( connections, cn, &p, 1);	
	}	
}
lpMAPSKILLTABLE	skill_ReturnProbablityFromSkillTable( int *probablity,  int skillno, int x, int y )
{			
			
	lpMAPSKILLTABLE	 s;
	int range = 0;	
	int pro;		
	int dx, dy;		
			
	x = x / TILE_SIZE;
	y = y / TILE_SIZE;
			
	dx = (g_Map.file.wWidth +7 ) / 8;
	dy = (g_Map.file.wHeight+7 ) / 8;
			
	dx = x / dx;	
	dy = y / dy;	
			
//	for( i = 0 ; i < 8 ; i ++ )
	{				
//		for( j = 0 ; j < 8 ; j ++ )
		{			
			s = g_Header[dx][dy];
					
			while( s != NULL )
			{		
				if( s->skillno == skillno )
				{	
					range = s->tile_Range +1;
					if( s->x == x && s->y == y ) 
					{
						*probablity = s->probability;
						return s;
					}
					else if( BoxAndDotCrash( s->x - range, s->y - range, range*2, range*2, x, y ) )
					{
						pro =  abs(range - abs(s->x-x));
						pro = s->probability * pro / range;
						*probablity = pro;
						return s;
					}
				}	
				s = s->next;
			}		
		}			
	}				
	return NULL;	
}					
void SendSkillGauge(const int cn, int skillno, int x, int y )
{
	t_packet p;
	lpMAPSKILLTABLE	 ms; 
	int probablity;
	t_earthpower *ep = &EarthPower;

	switch(skillno )
	{
			// Reclain..
		case 21 : 
			// mining
		case 26 : 
			// cutting
		case 32 : 
			// digging..
		case 36 : 
			// Tamming..
		case 75 :	
		case 28:	//fish
		case 30:	//netting

			break;

		default : return;
	}

	p.h.header.type = CMD_SKILL_GAUGE;
	p.h.header.size = sizeof( t_server_skill_gauge );
				
	p.u.server_skill_gauge.skillno =  skillno;
	p.u.server_skill_gauge.x		= x;
	p.u.server_skill_gauge.y		= y;

	
	//	mining/cutting에 10으로 나눈이유는 값이 보통 0 - 1000의 값이고 게이지의 표현범위는 1/100이기 때문..
	switch( skillno )
	{				
		// 개간...	
	case 21 :	ms = skill_ReturnProbablityFromSkillTable( &probablity,  TOOL_FARMING, x, y );
				if( ms )
				{	

					p.u.server_skill_gauge.min = ep->reclaim[ ms->type_Num ]/100;
					QueuePacket( connections, cn, &p, 1 );
				}
		break;
		// mining
	case 26 :	ms = skill_ReturnProbablityFromSkillTable( &probablity,  TOOL_MINING, x, y );
				if( ms )
				{
					
					p.u.server_skill_gauge.min = ep->mining[ ms->type_Num ]/10;
					QueuePacket( connections, cn, &p, 1 );
				}
				//1018 zhh
				break;

		// cutting
	case 32 : ms = skill_ReturnProbablityFromSkillTable( &probablity,  TOOL_CHOPPING, x, y );
				if( ms )
				{
					p.u.server_skill_gauge.min = ep->cutting[ ms->type_Num ]/10;
					QueuePacket( connections, cn, &p, 1 );
				}
				//1018 zhh
				break;

		// digging..
	case 36 : ms = skill_ReturnProbablityFromSkillTable( &probablity,  TOOL_HUB, x, y );
				if( ms )
				{
					p.u.server_skill_gauge.min = ep->digging[ ms->type_Num ]/10;
					QueuePacket( connections, cn, &p, 1 );		
				}
				//1018 zhh
				break;

	//1018 zhh
	case 28 :
	case 30 : ms = skill_ReturnProbablityFromSkillTable( &probablity,  TOOL_FISHING, x, y );
				if( ms )
				{
					p.u.server_skill_gauge.min = ep->fishing[ ms->type_Num ]/10;
					QueuePacket( connections, cn, &p, 1 );
				}				
				break;

	}
}
void RecvSkillInfo(const int cn, t_client_skill_info *sp)//020531 lsw 
{
	const int	skillno			= sp->skillno;
	const int	x				= sp->x;
	const int	y				= sp->y;
	const int	targettype		= sp->targettype;
	const int	target_id		= sp->target_id;
	
	char TargetName[31]={0,};
	memcpy(TargetName,sp->TargetName,30);

	int temp	= 0;
	int hitrate = 0;

	const int iTryRare		= sp->s.p1;
	const int iRareGroup	= sp->s.p2;
	int iTryMakeCount	= sp->s.p3;

	// 020428 YGI acer
	if( iTryMakeCount > 10 ) iTryMakeCount = 10;
	
	// acer5		// 몬스터 아이디가 넘어 오면 어떻게 되라고
	CHARLIST *ch= CheckServerId(cn);
	if( !ch )					{return;}
	if( ch->cur_skillno )		{return;} // 현재 시전중인 기술이 있다면 받지 않는다. 
	if( ch->bAlive != ALIVE_ )	{return;}

	SaveBallancingSkill( ch, skillno );
		
	if( !ch->Hungry ) 
	{
		SendSkillResultFail( cn, skillno, ITEMMAKE_TIRED ); 
		return;
	}

	if(  timeGetTime() > ch->dwSkillTime)
	{
		ch->dwSkillTime = timeGetTime()+2000;
	}
	else
	{
		return;
	}

	//	내가 가진 기술이 맞다면 넘어가고 아니면 리턴한다.
	if(!SkillMgr.CheckIsYourSkill( ch, skillno )) 
	{
//		MyLog(LOG_NORMAL,"RecvSkillInfo() Recved Error SKillNo\n");
//		MyLog(LOG_NORMAL,"Name : %s Skill NO : %d\n",ch->Name, skillno);
		return;
	}
	HungryMuchAdd( ch , HT_SKILL );
	
	switch( sp->targettype )// 실패가 아니라 잘못된 Data를	걸러낸다. 
	{		
	case SKILLTARGETTYPE_MAP_ :	
		{
			if( x < 0 ) goto FAIL_;
			if( y < 0 ) goto FAIL_;
			if( x / TILE_SIZE >= g_Map.file.wWidth )	goto FAIL_;
			if( y / TILE_SIZE >= g_Map.file.wHeight )	goto FAIL_;
		//	DecreaseEquipDuration(ch,rand()%6+1,WT_WEAPON,0,true);//020720 lsw
		}
		break;										
	case SKILLTARGETTYPE_TABLE_ :	//양초 만들기나 병만들기를 위한 부분
		{
			if( target_id/1000 < 0 || target_id/1000 >ETC ) goto FAIL_;
		}
		break;
														
	case SKILLTARGETTYPE_ITEM_ :	//타겟이 아이템인것
		{
			if( target_id < 0 || target_id >= MAX_ITEM_LIST ) goto FAIL_;
			if( ItemList[ target_id].bAlive == REMOVE_ ) goto FAIL_;

			temp = ItemList[ target_id].item_no;
			// 혹시 기술을 사용할수 있는 Item외의 번호가 오는것을 막는다.
			if( temp / 1000 < 0 || temp /1000 > 10 ) goto FAIL_;
		}
		break;
			     
	case SKILLTARGETTYPE_CHAR_ :	
		{
			if( target_id >= 10000 )	
			{
				if( target_id -10000 < NPC_LIST_START || target_id -10000 >= MAX_NPC_LIST ) goto FAIL_;
			}
			else
			{
				if( target_id < DRAGON_CONNECTIONS_START || target_id >= DRAGON_MAX_CONNECTIONS ) goto FAIL_;
			}
			
			CHARLIST *targetch = CheckServerId(target_id);
			if(targetch)
			{
				if(strcmp(TargetName,targetch->Name))//이름이 다르다면//020531 lsw
				{
					goto FAIL_;
				}
			}
		}
		break;
	}	
	
	hitrate = ReturnSkillProbablityNoExpAdd( cn, skillno, x, y, targettype, target_id );

	switch( targettype )
	{												
		case SKILLTARGETTYPE_TABLE_ : //SkillTargetTypeItemMake
			{
				SkillTargetTypeItemMake( cn, sp);
			}
			break;
			
		default : 
		{	
			t_packet p;
			p.h.header.type = CMD_SKILL_INFO;
			{
				p.u.server_skill_info.server_id = cn;
				p.u.server_skill_info.skillno	= skillno;
				p.u.server_skill_info.x			= x;
				p.u.server_skill_info.y			= y;
				p.u.server_skill_info.targettype= targettype;
				p.u.server_skill_info.target_id = target_id;
			}
						
			p.h.header.size = sizeof( t_server_skill_info );
			CastMe2Other( cn, &p );	
			QueuePacket( connections, cn, &p, 1);
		}			
				
		// 해당하는 기술일경우 클라이언트에서 게이지를 출력하게 한다. 
		SendSkillGauge( cn, skillno, x, y );
		// Pre Check....//왜 먼저 체크 하는데??
		switch( skillno )
		{	// MakeFire
			case  SKILL_MAKEFIRE :	
				{
					if( skill_Makefire_preCheck	( cn, skillno, x, y, targettype, target_id ) == 0 )	return;
				}break;
		}	
					
		ch->cur_skillno				= sp->skillno;				
		ch->cur_skillx				= sp->x;					
		ch->cur_skilly				= sp->y;							
		ch->cur_skilltargettype		= sp->targettype;				
		ch->cur_skilltarget_id		= sp->target_id;
		ch->cur_skillKey			= sp->iKey;//030102 lsw
		ch->cur_skillPOS			= sp->s;
		//020531 lsw		>
		memcpy(ch->cur_skillTargetName	,sp->TargetName,30);
		ch->cur_skillTargetName[30] = 0;//strcmp할때 NtDll에서 뻑나는거 예방
		//020531 lsw		<
		ch->exetime_skill			= g_curr_time + SkillTbl[ skillno].Time_judge / 10;
		break;						
	}
	return;
FAIL_:	
	SendSkillResultFail( cn, skillno );		
	return ;
}		

void ReduceTame()		//일정 시간이 흐르면 tame을 깍는다.void CheckNPCTammingTimeElapsed( CHARLIST *ch )을 복사해서 사용.
{	
	static int min;				
	
	if( g_curr_time - min > 900 )
	{		
		min = g_curr_time;
		for(int i = NPC_LIST_START ; i < MAX_NPC_LIST; i ++)
		{	
			if( NPCList[i].bAlive == ALIVE_ )
			if( NPCList[i].tame > 0 )
			{					
				CHARLIST *n = &NPCList[i];
				if (!n) return;

				if (n->IsTamedNpc() == true)
				{	
					n->tame -= NPC_Gen_Ref[n->SprNo].ValueSP * 15;
					if(n->tame<0)	n->tame=0;
				}	
				else
				{	
					n->tame -= (NPC_Gen_Ref[n->SprNo].ValueHP * 15);
					if(n->tame<0)	n->tame=0;
				}				
								
				if (n->IsTamedNpc() && n->tame <= NPC_Gen_Ref[n->SprNo].Taming_min)
				{				
					n->SetTamed(false);		// LTS TAME
					n->HostName[0] = 0;
					n->tame_cmd = LC_TAME_STOP;
					n->patterntype = n->oldpatterntype;
								
					t_packet p;	
					p.h.header.type= CMD_TAME_END;
					p.u.tame_end.id = n->GetServerID() + 10000;
					p.h.header.size = sizeof( t_tame_host_name );
					CastNPC2Other( n->GetServerID(), &p );
								
					SendNPC_Parameter( n->GetServerID(), NPC_PARAMETER_TAME_CMD,	n->tame_cmd );
					SendNPC_Parameter( n->GetServerID(), NPC_PARAMETER_PATTERNTYPE,	n->patterntype );
				}				
			}					
		}						
	}							
}								

void RefreshSkillMapTable( void )
{	
//<! BBD 040311
/*	static	int		ttime;

	int g_y, g_mo, g_d, g_h, g_mi, g_se;		// 게임시간
	CurrentGameDate( g_curr_time*6, &g_y, &g_mo, &g_d, &g_h, &g_mi, &g_se );

	int sub = g_h-ttime;
	if( sub < 0 ) sub=sub+24;
	if( sub < 6 ) return;	//1시간 마다 refresh

	ttime = g_h;

	t_earthpower *ep;				
	ep = &EarthPower;
	int		i;

	static  int iRefreshCount=0;	
	if(iRefreshCount>3)	iRefreshCount=0;
	for( i = iRefreshCount ; i < ep->miningcount   ; i=i+4)	ep->mining[i]  =ep->mining_old[i];
	for( i = iRefreshCount ; i < ep->fishingcount  ; i=i+4)	ep->fishing[i] =ep->fishing_old[i];
	for( i = iRefreshCount ; i < ep->cuttingcount  ; i=i+4)	ep->cutting[i] =ep->cutting_old[i];
	for( i = iRefreshCount ; i < ep->diggingcount  ; i=i+4)	ep->digging[i] =ep->digging_old[i];
	iRefreshCount++;*/

	// 이전의 타이머를 신뢰할수 없었다. 그래서 함수내부를 앗사리 다시 구성함

	static int old_hour = g_hour;
	static int old_min = g_min;
	static bool bReq = false;	// 동접요청을 1번만 하기위한 플레그

	int sub = g_hour-old_hour;
	if( sub < 0 ) sub=sub+24;

	if( sub != 4 || old_min != g_min)	// 4 시간 흘러야지 밑쪽으로 진입 4시간마다 refresh	// BBD 040322
	{
		int min = old_min - 1;
		if(min < 0) min += 60;

		if(!bReq && sub == 4 && min == g_min)	// 이전시간으로부터 4시간 흐른 시간의 1분전 동접을 요청한다	// BBD 040322
		{
			RequestUsercountToProxy();		// 동접요청
			bReq = true;
		}
		return;	// 6시간이 않됬으면 리턴
	}

	bReq = false;
	old_hour = g_hour;
	old_min = g_min;

	t_earthpower *ep;				
	ep = &EarthPower;

	float Rate = GetResourceRate(g_UsercountInServerSet);	// 자원배율을 구해온다

	MyLog(0, "Resetting ResourceRate = %.1f, cause UsercountInServerSet = %d", Rate, g_UsercountInServerSet);
	MyLog(0, "mine[0] orign:%d\tbefore:%d\tcurrent:%d", ep->mining_old[0], ep->mining[0], (int)(ep->mining_old[0] * Rate));
	MyLog(0, "fishing[0] orign:%d\tbefore:%d\tcurrent:%d", ep->fishing_old[0], ep->fishing[0], (int)(ep->fishing_old[0] * Rate));
	MyLog(0, "cutting[0] orign:%d\tbefore:%d\tcurrent:%d", ep->cutting_old[0], ep->cutting[0], (int)(ep->cutting_old[0] * Rate));
	MyLog(0, "digging[0] orign:%d\tbefore:%d\tcurrent:%d", ep->digging_old[0], ep->digging[0], (int)(ep->digging_old[0] * Rate));

	int		i;
	static  int iRefreshCount=0;	
	if(iRefreshCount>3)	iRefreshCount=0;
	for( i = iRefreshCount ; i < ep->miningcount   ; i=i+4)
	{
		ep->mining[i]  = (int)(ep->mining_old[i] * Rate);
	}
	for( i = iRefreshCount ; i < ep->fishingcount  ; i=i+4)
	{
		ep->fishing[i] = (int)(ep->fishing_old[i] * Rate);
	}
	for( i = iRefreshCount ; i < ep->cuttingcount  ; i=i+4)
	{
		ep->cutting[i] = (int)(ep->cutting_old[i] * Rate);
	}
	for( i = iRefreshCount ; i < ep->diggingcount  ; i=i+4)
	{
		ep->digging[i] = (int)(ep->digging_old[i] * Rate);
	}

	iRefreshCount++;
//> BBD 040311
}

short nSadTime = -1;
void UpdateSadonixCount()
{
	if( MapInfo[MapNumber].port == NATION_MANAGE_SERVER )
	{
		if( nSadTime == -1 )
		{
			nSadTime = g_wday*100 + g_hour;
		}
	
		const short tempnSadTme = nSadTime;
		nSadTime = g_wday*100 + g_hour;
		if( tempnSadTme != nSadTime)//변했다면
		{
			t_packet p;
			p.h.header.type = CMD_SADONIX_COUNT_UPDATE;
			p.h.header.size = sizeof(t_SadonixCountUpdate);
			p.u.SadonixCountUpdate.nSadTime = nSadTime;
			QueuePacket( connections, DB_DEMON, &p, 1 );
		}
	}
}
// 건 기술에 대해 시전시간이 지나면 실행시킨다. 
void CheckSkillTimeElapsed( void )
{	//< CSD-HK-030829
	static DWORD time;

	RefreshSkillMapTable();
	ReduceTame();

	if (g_curr_time != time) 
	{ 
		time = g_curr_time; 
	}
	else
	{
		return;
	}

	CheckWeatherSystem();
	WeatherControl.CheckDayLight();
	
	for (int i = DRAGON_CONNECTIONS_START; i < DRAGON_MAX_CONNECTIONS ; ++i)
	{
		if (connections[i].dwAgentConnectionIndex && connections[i].state >= CONNECT_JOIN)
		{	
			CHARLIST* pTarget = &connections[i].chrlst;
			if (!pTarget) return;

			if (pTarget->GetServerID() != i)//020530 lsw
			{ 
				MyLog(LOG_FATAL,"Wrong ServerID User Have [%d] But [%d]", pTarget->GetServerID(), i);
				pTarget->SetServerID(i);
			}

			CrackMgr.AutoCheckAccelator(pTarget);
			WeatherControl.AutoSetDayLight(pTarget);

			if (pTarget->bAlive == ALIVE_)
			{
				SkillMgr.AutoRecoverHp(pTarget);
				SkillMgr.AutoRecoverMp(pTarget);
				SkillMgr.AutoChangeHungry(pTarget);
				SkillMgr.AutoRecoverCp(pTarget);
				SkillMgr.AutoRefreshRareMakeCount(pTarget);
				SkillMgr.AutoSetCanView(pTarget); //유령 , 폭탄 , 마법트랩 등을 볼 수 있는지 체크
				RareEM.AutoCheckDynamicRare(pTarget->DynamicRare);

				if (pTarget->cur_skillno)
				{
					if (pTarget->exetime_skill < g_curr_time)
					{
						SkillMapper(i, pTarget->cur_skillno, pTarget->cur_skillx, pTarget->cur_skilly, pTarget->cur_skilltargettype, pTarget->cur_skilltarget_id, pTarget->cur_skillPOS);
						pTarget->cur_skillno = 0;
					}
				}
			}
		}
	}
	// 몬스터 자동 체력 회복
	for (int j = NPC_LIST_START; j < MAX_NPC_LIST; ++j)
	{
		CHARLIST* ch = CheckNpcId(j);
		if (ch == NULL)  continue;

		if (ch->bAlive == ALIVE_ && ch->IsLifeUp() && global_time >= ch->dwHpRecoverTime)
		{
			ch->dwHpRecoverTime = global_time + DEFAULT_RECOVER_HP_TIME;

			const int nLife = ch->GetLifeUp();
			const int nReal = ch->IncLife(nLife);

			if (nReal > 0)
			{	// 체력 회복처럼 사용 				
				t_packet packet;
				packet.h.header.type = CMD_MAGIC_RESULT_H;
				packet.h.header.size = sizeof(t_server_magic_result_h);
				packet.u.magic.server_magic_result_h.nMagic = LIFE_UP;
				packet.u.magic.server_magic_result_h.idTarget = j + 10000;
				packet.u.magic.server_magic_result_h.nLife = nReal;
				packet.u.magic.server_magic_result_h.nState = ch->GetState();
				packet.u.magic.server_magic_result_h.nResult = HIT_AND_NOTDEAD;
				::CastNPC2Other(j, &packet);
			}
		}
	}

	UpdateSadonixCount();
}	//> CSD-HK-030829

//지워야 할 것 같은 함수 입니다.
void RecvSkillCursorView(const int cn, t_client_skill_cursor_view  *p)
{	
	t_packet packet;
	
	packet.h.header.type = CMD_SKILL_CURSOR_VIEW;
	{
		packet.u.client_skill_cursor_view.skillno = p->skillno;
		packet.u.client_skill_cursor_view.x = p->x;
		packet.u.client_skill_cursor_view.y = p->y;
	}
	packet.h.header.size = sizeof( t_client_skill_cursor_view );

	//	QueuePacket( connection, cn, &packet );
	CastMe2Other( cn, &packet );
}	
	
//	기술 결과를 다른이에게 알린다.
//  특히 도둑기술에 실패하면 필히 알려야 하지... 암....
//	cn : 기술을 쓴사람.
//	target_id : 기술을 당한 사람..
void SendSkillResultToOther(const int cn, const int skillno, const int target_id, const int result )
{	
	t_packet p;
	p.h.header.type = CMD_SKILL_RESULT_TOOTHER;
	
		p.u.server_skill_result_toother.id		= cn; // 기술을 쓴사람..
		p.u.server_skill_result_toother.skillno = skillno;
		p.u.server_skill_result_toother.result  = result;
	
	p.h.header.size = sizeof( t_server_skill_result_fail ); // 
	
	QueuePacket( connections, target_id, &p, 1);
}	
		
void SendSkillResult(const int cn, const int skillno, const int target_id, const int x, const int y, const int result )
	{	
	t_packet p;
	p.h.header.type = CMD_SKILL_RESULT;
	p.u.server_skill_result.skillno = skillno;
	p.u.server_skill_result.x		= x;
	p.u.server_skill_result.y		= y;
	p.u.server_skill_result.result	= result;
	p.h.header.size = sizeof( t_server_skill_result );
	QueuePacket( connections, cn, &p, 1);
}	
	
void SendViewTypeChange(const int cn, int viewtype )
{	
	t_packet p;
	
	p.h.header.type		= CMD_VIEWTYPE_CHANGE;
	p.h.header.size		= sizeof( t_viewtype_change );
	p.u.viewtype_change.server_id = cn;
	p.u.viewtype_change.viewtype = viewtype;
	if( cn < 10000 )		//PC인 경우만 보냄
	{
		QueuePacket( connections, cn, &p, 1);
		CastMe2Other( cn, &p );
	}
	else
	{
		CastNPC2Other( cn-10000, &p);
	}
}
		
int skill_Detect_Ghost	( const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id )
{	
	CHARLIST *ch = CheckServerId(cn);//020531 lsw
	if( !ch ) return false;	// 020620 YGI

	const int iHitrate	= ReturnSkillProbablity( cn, skillno, x, y, targettype, target_id );
	const int r = rand()%100;
	const int iTime = (SkillTbl[skillno].Produce_Fix*iHitrate)/1000000;//볼 수 있는 초 단위
	
	if(SkillMgr.CanViewGhost(ch)){goto FAIL_; }
	if( r > iHitrate )	{ goto FAIL_; }
	if(!iTime)			{ goto FAIL_; }
	
	SkillMgr.SetCanViewGhost(ch,iTime);
	SendSkillResult( cn, skillno, target_id, x, y, iTime );
	return 1; 

FAIL_:
	SendSkillResultFail(cn, skillno); 
	return 0;
}	

int skill_Poisoning			(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id )
{
	SendSkillResult( cn, skillno, target_id, x, y, -999 ); 
	return 1; 
}

int skill_Detect_Poison		(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id )
{ 
	SendSkillResult( cn, skillno, target_id, x, y, -999 ); 
	return 1; 
}

int skill_Find_Traps		(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id )
{	
	CHARLIST *ch = GetCharListPtr(cn);
	if(!ch) return 0;
	if( ch->bCanViewTrap ){ return 1;}
	
	const int iHitrate	= ReturnSkillProbablity( cn, skillno, x, y, targettype, target_id );
	const int r = rand()%100;
	if( r > iHitrate ) {  SendSkillResultFail(cn, skillno); return 0; }

	const int lv = ch->skillexp[ SkillTbl[skillno].MotherSkillType].skillexp / 10000;
	int iTime = 30 + (lv*3);//	기본 30 초 + 1 당 3		
	const int MAX = 300;
	
	if( iTime > MAX ) {iTime = MAX;}
	SkillMgr.SetCanViewTrap(ch,iTime);//020510 lsw
	SendSkillResult( cn, skillno, target_id, x, y, iTime );
	return 1; 
}

int skill_GhostResurrection(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id )
{
#ifdef LMS_MAPSERVER //Eleval 19/07/09 - To disable noob ress in LMS Arena
	goto FAIL_;
#endif

	const int r = rand()%100;;
	int hitrate;
	LPCHARLIST Caster=	CheckServerId(cn);
	LPCHARLIST Target=	CheckServerId(target_id);

	if( ! Caster ) {goto FAIL_;}
	if( ! Target ) {goto FAIL_;}
	
	if(!SkillMgr.CanViewGhost(Caster))			{	goto FAIL_;	}//유령을 볼 수 없다//020701 lsw
	if( Target->viewtype != VIEWTYPE_GHOST_ )	{	goto FAIL_;	}
	if(Target->IsReporter())						{	goto FAIL_; } //Finito added so reporter cant be res.

	hitrate = ReturnSkillProbablity( cn, skillno, x, y, targettype, target_id );
	
	if( r >= hitrate ) {goto FAIL_; }

	// 이기술은 클래스가 성직자만 사용할수 있다. 
	switch(Caster->Class)
	{
		case WARRIOR : 
		case THIEF	 : 
		case ARCHER	 : 
		case WIZARD	 : goto FAIL_;
		case PRIEST	 : break;
	}

//< soto-030326 중립지역이 아닐때는...다른 국가 사람을 살릴수가 없다.  1.02버젼
	if(!IsNeutralZone(MapNumber))
	{
		if(Caster->name_status.nation != Target->name_status.nation)
			goto FAIL_;
	}
//> soto-030326

	


	// 기술이 들어갔으나 누가 먼저 살렸을 때...
//	if(CON_NORMAL == Target->GetState() )	{goto FAIL_;}
	if( CheckEventAttack())					{goto FAIL_;}//020205 lsw
	if( CanLocalWarAttack(Caster,Target) )	{goto FAIL_;}//020205 lsw

	SkillMgr.CharacterToAlive(Target);//사람 살리기
	
	addEXP( Caster, Target, HIT_GHOSTRESURRECTION, Target->HpMax / 10 );

	SendSkillResult( cn, skillno, target_id, x, y, target_id );
	return 1; 

FAIL_:

	SendSkillResultFail(cn, skillno); 
	return 0;
}
//01214 lsw
int skill_Remove_Traps		(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id )
{
	CItem_Etc *t;
	int UserSkillLevel = 0;
	const int ret = ReturnSkillProbablity( cn, skillno, x, y, targettype, target_id );
	if( ret == 0 ) goto FAIL_;

	t  =(CItem_Etc *)ItemUnit( ItemList[ target_id].item_no);

	if(!t){goto FAIL_;}

	if(t->GetItemKind() != IK_TRAP){goto FAIL_;}
	
	UserSkillLevel = connections[ cn].chrlst.skillexp[ SkillTbl[skillno].MotherSkillType].skillexp / 10000;
	if( t->Repair_max > UserSkillLevel ) {goto FAIL_;}
	
	if( ItemList[ target_id].attr[ IATTR_ATTR] & IA2_TRAPED1 )
	{
		ItemList[ target_id].attr[ IATTR_ATTR] &= ( ~(IA2_NOTMOVE) );
		ItemList[ target_id].attr[ IATTR_ATTR] &= ( ~(IA2_TRAPED1) );

		//1024 zhh
		ItemList[ target_id].attr[IATTR_DURATION] = 10;

		SetArea( CHANGE_IA2_ATTR_ITEM_AREA, target_id );

		//1020 zhh
		RemoveItemList(target_id);
		AddItemList( ItemList[target_id].item_no  , ItemList[target_id].attr  , 0, ItemList[target_id].x, ItemList[target_id].y, 0, 0);
	//	AddItemList( c[cn].chrlst.handheld.item_no, c[cn].chrlst.handheld.attr, 0, pi->x, pi->y, 0, 0 );

		SendSkillResult( cn, skillno, target_id, x, y, 1 );
		return 1; 
	}
	
FAIL_ :
	 SendSkillResultFail(cn, skillno); return 0; 
}
//011215 lsw 
int skill_Entrapment		(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id )
{
	const int ret = ReturnSkillProbablity( cn, skillno, x, y, targettype, target_id );
	CItem *t;
//	if( g_block_steal ) {	goto FAIL_;	}//020821 lsw
	if( ret == 0 ) goto FAIL_;

	// 폭탄의 공격.. 마법 무시 하고 간다.
	t  = ItemUnit( ItemList[ target_id].item_no);

	if(!t){goto FAIL_;}

	if(t->GetItemKind() != IK_TRAP){goto FAIL_;}//if( ItemList[ target_id].item_no == 10033 ) 대체
	
	//repairmin 데미지 ,...
	//repairmax 가 능력치 제한
	if( ( ItemList[ target_id].attr[ IATTR_ATTR] & IA2_TRAPED1 ) == 0 )
	{
		//1009 zhh	//데미2지를 미리 계산한다
		CItem_Etc *t = (CItem_Etc *)ItemUnit(ItemList[ target_id].item_no/1000, ItemList[ target_id].item_no%1000);
			
		if(!t){goto FAIL_;}

		int UserSkillLevel = connections[ cn].chrlst.skillexp[ SkillTbl[skillno].MotherSkillType].skillexp / 10000;
		if( t->Repair_max > UserSkillLevel ) 
		{
			OutMessage(cn,2,522);
			goto FAIL_;
		}
		
		if( !SkillTbl[skillno].Produce_Fix){goto FAIL_;}
		if(	!abs(ret) ){goto FAIL_;}
		if(!t->Repair_min){goto FAIL_;}

		DWORD dwDmg = ((SkillTbl[skillno].Produce_Fix/10000) * (abs(ret)/10) * ((t->Repair_min)/10));//020205 lsw

		if(!dwDmg) {goto FAIL_;}

		ItemList[ target_id].attr[ IATTR_ATTR] |= IA2_NOTMOVE;
		ItemList[ target_id].attr[ IATTR_ATTR] |= IA2_TRAPED1;
		
		ItemList[ target_id].birthday = g_curr_time;//020215 lsw
		
		ItemList[ target_id].attr[IATTR_DURATION] = dwDmg + (dwDmg*(rand()%20))/100;
				
		SetArea( CHANGE_IA2_ATTR_ITEM_AREA, target_id );
		SetArea( CHANGE_IA2_DURATION_ITEM_AREA, target_id );//021107 lsw
		
		SendSkillResult( cn, skillno, target_id, x, y, 1 );
		return 1; 
	}
	
FAIL_ :
	 SendSkillResultFail(cn, skillno); return 0; 
	return 0; 
}






int skill_Detect_Bomb		(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id )//011215 lsw
{	
	CHARLIST *ch = GetCharListPtr(cn);
	if(!ch) return 0;
	
	const int iHitrate	= ReturnSkillProbablity( cn, skillno, x, y, targettype, target_id );
	const int r = rand()%100;
	if( r > iHitrate ) {  SendSkillResultFail(cn, skillno); return 0; }

	const int lv = ch->skillexp[ SkillTbl[skillno].MotherSkillType].skillexp / 10000;
	int iTime = 30 + (lv*3);//	기본 30 초 + 1 당 3		
	const int MAX = 300;
	
	if( ch->bCanViewBomb ){ return 1;}
	
	if( iTime > MAX ) {iTime = MAX;}
	SkillMgr.SetCanViewBomb(ch,iTime);//020510 lsw
	SendSkillResult( cn, skillno, target_id, x, y, iTime );
	return 1; 
}
//011215 lsw
int skill_Bomb_in			(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id )
{
	int ret = ReturnSkillProbablity( cn, skillno, x, y, targettype, target_id );
	CItem *t;
//	if( g_block_steal ) {	goto FAIL_;	}//020821 lsw
	if( ret == 0 ) goto FAIL_;

	// 폭탄의 공격.. 마법 무시 하고 간다.
	t  = ItemUnit( ItemList[ target_id].item_no);

	if(!t){goto FAIL_;}

	if(t->GetItemKind() != IK_BOMB){goto FAIL_;}//if( ItemList[ target_id].item_no == 10033 ) 대체
	
	//repairmin 데미지 ,...
	//repairmax 가 능력치 제한
	if( ( ItemList[ target_id].attr[ IATTR_ATTR] & IA2_TRAPED0 ) == 0 )
	{
		//1009 zhh	//데미2지를 미리 계산한다
		CItem_Etc *t = (CItem_Etc *)ItemUnit(ItemList[ target_id].item_no/1000, ItemList[ target_id].item_no%1000);
			
		if(!t){goto FAIL_;}

		int UserSkillLevel = connections[ cn].chrlst.skillexp[ SkillTbl[skillno].MotherSkillType].skillexp / 10000;
		if( t->Repair_max > UserSkillLevel ) 
		{	OutMessage(cn,2,522);
			goto FAIL_;
		}

		if( !SkillTbl[skillno].Produce_Fix){goto FAIL_;}
		if(	!abs(ret) ){goto FAIL_;}
		if(!t->Repair_min){goto FAIL_;}

		DWORD dwDmg = ((SkillTbl[skillno].Produce_Fix/10000) * (abs(ret)/10) * ((t->Repair_min)/10));//020205 lsw

		if(!dwDmg) {goto FAIL_;}

		ItemList[ target_id].attr[ IATTR_ATTR] |= IA2_NOTMOVE;
		ItemList[ target_id].attr[ IATTR_ATTR] |= IA2_TRAPED0;

		ItemList[ target_id].attr[IATTR_DURATION] = dwDmg+ (dwDmg*(rand()%20))/100;
		ItemList[ target_id].birthday = g_curr_time;//020215 lsw

		SetArea( CHANGE_IA2_ATTR_ITEM_AREA, target_id );
		SetArea( CHANGE_IA2_DURATION_ITEM_AREA, target_id );//021107 lsw
		
		SendSkillResult( cn, skillno, target_id, x, y, 1 );
		return 1; 
	}
	
FAIL_ :
	 SendSkillResultFail(cn, skillno); 
	 return 0; 
}
//1027 zhh
int BombExplosionCheck(const int cn,int id)
{
	
	
	if( id < 0 ) return 0;
	if( id >= MAX_ITEM_LIST ) return 0;
	
	if( connections[cn].chrlst.bAlive == ALIVE_ )
	if( ItemList[ id].bAlive == ALIVE_ )
	{
		if( ItemList[ id].attr[ IATTR_ATTR] & IA2_TRAPED0 )
		{
			if(rand()%10) return 0;
			
			t_packet packet;

      connections[cn].chrlst.DecLife(ItemList[ id ].attr[IATTR_DURATION]);

			if (connections[cn].chrlst.IsDead() == true)
			{
				killCharacter( NULL, GetCharListPtr(cn) );
			}

      packet.h.header.type = CMD_FOOT_ON_TRAP_0_ITEM;
      packet.h.header.size = sizeof(t_server_foot_on_trap0);
			packet.u.server_foot_on_trap0.idServer = WORD(cn);
			packet.u.server_foot_on_trap0.idItem = WORD(id);
			packet.u.server_foot_on_trap0.nLife = connections[cn].chrlst.Hp;

			QueuePacket(connections, cn, &packet, 1);
			CastMe2Other( cn, &packet );
			
			RemoveItemList( id );
			return 1;
		}
	}
	return 0;
}

//011214 lsw
int skill_Remove_Bomb		(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id )
{	
	CItem_Etc *t;
	int UserSkillLevel = 0;
	int ret = ReturnSkillProbablity( cn, skillno, x, y, targettype, target_id );
	if( ret == 0 ) goto FAIL_;

	t  = (CItem_Etc *)ItemUnit( ItemList[ target_id].item_no);

	if(!t){goto FAIL_;}

	if(t->GetItemKind() != IK_BOMB){goto FAIL_;}//if( ItemList[ target_id].item_no == 10033 ) 대체

	UserSkillLevel = connections[ cn].chrlst.skillexp[ SkillTbl[skillno].MotherSkillType].skillexp / 10000;
	if( t->Repair_max > UserSkillLevel ) {goto FAIL_;}
	
	if( ItemList[ target_id].attr[ IATTR_ATTR] & IA2_TRAPED0 ) // Bomb
	{
		//1019 zhh
	//	ItemList[ target_id].birthday=g_curr_time;

//		if(BombExplosionCheck(const int cn,int id)==1)		goto fail_;
		//1024 zhh
		ItemList[ target_id].attr[IATTR_DURATION] = 10;

		ItemList[ target_id].attr[ IATTR_ATTR] &= ( ~(IA2_NOTMOVE) );
		ItemList[ target_id].attr[ IATTR_ATTR] &= ( ~(IA2_TRAPED0) );
		SetArea( CHANGE_IA2_ATTR_ITEM_AREA, target_id );
		//1020 zhh
		RemoveItemList(target_id);
		AddItemList( ItemList[target_id].item_no  , ItemList[target_id].attr  , 0, ItemList[target_id].x, ItemList[target_id].y, 0, 0);
	//	AddItemList( c[cn].chrlst.handheld.item_no, c[cn].chrlst.handheld.attr, 0, pi->x, pi->y, 0, 0 );
		
		SendSkillResult( cn, skillno, target_id, x, y, 1 );
		return 1; 
	}
FAIL_:
	SendSkillResultFail(cn, skillno); 
	return 0; 
}

//<soto-steal추가
int		SerchItemNPCINV(CHARLIST* ch,int nItemNumber)
{
	int count = 0;
	for(int a=0;a<3;a++)
		for(int  b=0;b<3;b++)
			for(int c=0;c<8;c++)
				if(ch->inv[a][b][c].item_no == nItemNumber)
				{
					++count;
				}

	return count;
}

int		TotalItemCountNPCINV(CHARLIST* ch)
{
	int count = 0;
	for(int a=0;a<3;a++)
		for(int  b=0;b<3;b++)
			for(int c=0;c<8;c++)
				if(ch->inv[a][b][c].item_no)
				{
					++count;
				}

	return count;
}
//>soto-steal추가


int skill_Stealing	(const int cn, const int skillno, int x, int y, const int targettype, const int target_id )//020815 lsw
{	
	t_packet p;	
	
	int count =0;
	int i=0,j=0,k=0;
	int ex=0, ey=0;
	int sx=0, sy=0;
	int iTrapdamage=0;
	int iTrapResultdamage=0;
	int a1=0,a2=0,a3=0;		
	int why = 0;
	int ret = 0,r = 0;
	int iTraped = 0;
	int iSteal  = 0;
	int warning = 40;
	int iForceItemNo = 0;

	ItemAttr StealItem;
	ItemAttr ResultItem;
	POS pos;
	CItem *ii = NULL;
	
	ItemAttr *pStealItem = NULL;

	LPCHARLIST target = ::CheckNpcId(target_id - 10000);
//	LPCHARLIST target = ::CheckServerId(target_id);
	LPCHARLIST caster = ::CheckServerId(cn);

	int nItemCount = SerchItemNPCINV(target,10123);
	int nTotalCount = TotalItemCountNPCINV(target);
	
	POS ItemPos[72] = {0,};

	if( !target ) {goto FAIL__;}
	if( !caster ) {goto FAIL__;}
	if( caster == target ) {goto FAIL__;}
	
	if( g_block_steal ) {goto FAIL__;}//020821 lsw

	if(	targettype != SKILLTARGETTYPE_CHAR_ ){	goto FAIL__;	}
	if( MapInfo[MapNumber].forrookie == 1 
	||	MapInfo[MapNumber].forrookie == 2)
	{
		goto FAIL__;
	} //PK가 안되는 지역에서는 훔칠 수 없다.
	

	if(::_stricmp(target->Name,caster->cur_skillTargetName))//020531 lsw 
	{
		goto FAIL__;
	}
	if(caster->GetLevel() < 31) {goto FAIL__;} // CSD-030806
	if(target->IsCounselor()){goto FAIL__;}//운영자는 못훔쳐//020730 lsw
	if(target->IsReporter()){goto FAIL__;}//Finito Added so that reporter cant be stole from.
	if(CanLocalWarAttack(caster,target)){goto FAIL__;}
	if( target->viewtype == VIEWTYPE_GHOST_ ) {goto FAIL__;}
	if(	target->Hp <=0 ) {goto FAIL__;}

	x = target->MoveSx;
	y = target->MoveSy;

	if (x < 0)  goto FAIL__;
	if (y < 0)  goto FAIL__; 
	if (x >= g_lpMapFile->wWidth)   goto FAIL__;
	if (y >= g_lpMapFile->wHeight)	goto FAIL__;

	if(LocalMgr.IsAbleNation(KOREA))
	{
		if( TileMap[x][y].attr_no_battle || !IsNeutralZone(MapNumber) ) // soto 030307 " || !IsEnableSkill_Steal(MapNumber) " 부분 추가.
		{
			OutMessage(caster->GetServerID(),2,525);//011218 lsw
			::SendSkillResultFail(cn, skillno); 
			return 0;
		}

		if(!LocalMgr.IsFreeBeta())
		{
			if (!caster->IsPayedUser())
			{
				::SendSkillResultFail(cn, skillno); 
				return 0;
			}
		}
	}
	else
	{
		if( TileMap[x][y].attr_no_battle )
		{
			OutMessage(caster->GetServerID(),2,525);//011218 lsw
			SendSkillResultFail(cn, skillno); 
			return 0;
		}
	}

	ex = target->MoveSx;	
	ey = target->MoveSy;
	sx = caster->MoveSx;	
	sy = caster->MoveSy;

	if(BoxAndDotCrash(sx-3,sy-3,7,7,ex,ey))//	도둑질은 약 반경 3타일안에 있어야 4-8타일밖에 있으면 20%로 뚝떨어진다. 
	{

	}	
	else if( BoxAndDotCrash( sx - 6, sy-6, 13, 13, ex, ey ) ) 
	{
		if( (rand()%100) < 80 )
		{
			goto FAIL__;
		}
	}
	else 
	{
		goto FAIL__;
	}

	ret = ReturnSkillProbablity( cn, skillno, x, y, targettype, target_id );
	r  = rand()%100;
	
	if( r > ret )
	{
		goto FAIL__;
	}

	count= 0;
	for(i = 0;	i<3	;	i++)
	{
		for(j= 0;	j<3	;j++)
		{
			for(k= 0;	k<8	;k++)
			{
				StealItem = target->inv[i][j][k];
				if(StealItem.item_no)
				{		
					if(StealItem.item_no == IN_STEAL_RESULT)//증거 아템이면 백장미이다.
					{
						continue;
					}
					ii = ItemUnit( StealItem.item_no );
					if( ii == NULL ) {continue;}
					
					if(ii->GetItemKind() == IK_LOCK_STEAL)
					{
						iForceItemNo = StealItem.item_no;
						iTraped =1;
					}

					//if( ii->GetLevel() > caster->skillexp[SkillTbl[skillno].MotherSkillType].skillexp / 10000 )
					if( ii->GetLevel() > 98)//98까지만 훔칠수 있다.
					{
						continue;
					}
					SetItemPos(INV, i,j,k, &ItemPos[count++]);
				}
			}
		}
	}
	if(!count)
	{
		goto FAIL__;
	}

	iSteal= rand()%count;

	if(!iTraped )//트랩이 아님 
	{
		pStealItem = GetItemByPOS( target, ItemPos[iSteal]);
		i =ItemPos[iSteal].p1;
		j =ItemPos[iSteal].p2;
		k =ItemPos[iSteal].p3;
	}
	else
	{
		if(SearchItemByInv(iForceItemNo,target,i,j,k))//트랩이면 트랩번호로 찾아낸다
		{
			POS pos;
			pos.type = INV;
			pos.p1 = i; pos.p2 = j; pos.p3 = k;
			pStealItem = GetItemByPOS( target, pos);
		}
		else
		{
			goto FAIL__;
		}
	}
	
	if( !pStealItem ) goto FAIL__;
	StealItem = *pStealItem;//아이템을 훔치니까 값을 임시공간에 저장 해 두고
	ii = ItemUnit( StealItem.item_no );

	if(!StealItem.item_no)
	{
		goto FAIL__;
	}
	///////////////////////////////
	// 훔치기 당한자의 처리(뭐가 됐든 타겟되는 자리의 아이템은 장미가 되던지 없어진다)
	if(!iTraped )//트랩이 아님 
	{
		ResultItem = ::GenerateItem( IN_STEAL_RESULT );//백장미
	}
	else//트랩임
	{
		ZeroMemory(&ResultItem	,sizeof(ItemAttr));//아이템 없어짐
		ZeroMemory(&StealItem   ,sizeof(ItemAttr));//훔쳐논 아이템 없어짐//트랩을 훔칠 수는 없잖아.
////////덫 처리
// soto 030307 덫에 걸리면...바로 죽어야 된다.		
		if(LocalMgr.IsAbleNation(KOREA))
		{
			caster->Hp = 0;
		}
		else
		{
			iTrapdamage  = ii->Getbody();
			iTrapResultdamage = (iTrapdamage / 10000) ;
			iTrapResultdamage += rand()%((iTrapdamage%10000)+1);
			caster->Hp -= iTrapResultdamage;
			if( 0 > caster->Hp){caster->Hp=0;}			
		}
// soto End.
		p.h.header.type = CMD_STEAL_ON_TRAP;
		p.h.header.size = sizeof(t_server_steal_on_trap);
		p.u.server_steal_on_trap.idServer = WORD(cn);
		p.u.server_steal_on_trap.nLife = caster->Hp;//시전자의 Hp를 그대로 보내줌

		QueuePacket(connections, cn, &p, 1);
		CastMe2Other( cn, &p );

		if (caster->IsDead() == true)
		{
			killCharacter( NULL, caster );
			SendCharacterCondition(caster, 0); // soto 030307 추가. 상태를 보내 주어야 죽더라. ㅡㅡ;
		}
		
		OutMessage(target->GetServerID(),2,523);
		OutMessage(caster->GetServerID(),2,523);

// soto 030307 추가.
////////트렙을 없앤다.
		SetItemPos( INV, i,j,k, &pos );
//		SendDeleteItem(&target->inv[i][j][k],&pos,target,false);
		DeleteItem(&target->inv[i][j][k]);//몬스터 이므로.
		return 0; 		
// soto End.
	}
	SendItemEventLog( pStealItem, target_id, cn, SILT_PUT_STOLEN, 1 );
	*pStealItem = ResultItem;//당한 물건을 넣어주자	//훔친 아이템 인벤토리 포인터에 값을 넣어준다

// soto 030307 lsw 님의 요청으로 수정.
//	p.h.header.type = CMD_GIVE_ITEM; 
//	{
//		p.u.server_give_item.p1 = i;
//		p.u.server_give_item.p2 = j;
//		p.u.server_give_item.p3 = k;
//		p.u.server_give_item.item = ResultItem;
//	}
//	p.h.header.size = sizeof( t_server_give_item );
//	QueuePacket( connections, target_id, &p, 1);//당한사람 아이템 기록 남김

//	SetItemPos( INV, i,j,k, &pos );
//	SendServerEachItem( &pos, &target->inv[i][j][k], target_id);
// soto End.

	memset(&p, 0, sizeof(t_packet));
	// 여기까지 오면 훔치기 성공이기 때문에 
	if( SearchInv( caster->inv, a1, a2, a3 ) )  //빈칸이 있다면
	{		
		caster->inv[a1][a2][a3] = StealItem;	// 
		SetItemPos( INV, a1,a2,a3, &pos );
		SendServerEachItem( &pos, &caster->inv[a1][a2][a3], cn );
	}				
	else//땅에 덜어뜨림....
	{				
		AddItemList( StealItem.item_no,StealItem.attr, 0, target->X+rand()%40, target->Y+rand()%60, 0, 0,  0,0,0,0 );
	}
	
	if( caster->IsUltraNK(MapInfo[MapNumber].nation) )	
	{	
		warning = 100;
	}


//	if( rand()%100 < warning ) // 5%의 확률로 당한 얘한데 결과를 알려준다.  ( 들켰다 )
//	{
//		SendSkillResult( cn, skillno, target_id, x, y, 2 ); // 들켰다. 
//		SendSkillResultToOther( cn, skillno, target_id, 1 );
//		CheckNK( cn, target_id, NK_TYPE_STEAL_ );
//	}
//	else 
	{
		SendSkillResult( cn, skillno, target_id, x, y, 1 );
	}


	return 0;
FAIL__:
	{
		CheckNK( cn, target_id, NK_TYPE_NO_KILL_);//NK 추가
		SendSkillResultFail(cn, skillno); 
		return 0;
	}
	return 0;
}
		

int skill_Reclaim			(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id )
{	
	CHARLIST *ch = CheckServerId(cn);//020720 lsw
	if(!ch){return 0;}

	int probablity;
	int why = 0; 
	int how;
	lpMAPSKILLTABLE	mt;
	int ret;
	t_earthpower *ep = &EarthPower;
	DecreaseEquipDuration(ch,rand()%8+1,WT_WEAPON,0,true);//020720 lsw

	mt = skill_ReturnProbablityFromSkillTable( &probablity,  1, x, y );
	if( mt == NULL ) { why = SKILLFAIL_NOT_PROPER_PLACE; goto FAIL_; }

	if( mt->type_Num && mt->type_Num < MAX_EARTHSKILL ) 
	{
 		if( ep->reclaim[ mt->type_Num ] >= 10000 ) // 다 개간이 되었으면..
		{
			why = SKILLFAIL_NO_MORE_RACLAIM;
			goto FAIL_;
		}

		ret = ReturnSkillProbablity( cn, skillno, x, y, targettype, target_id );
		how = ret * SkillTbl[skillno].Produce_Fix;
		how /= 10000;

		ep->reclaim[ mt->type_Num ] += how;
		if( ep->reclaim[ mt->type_Num ] > 10000 ) ep->reclaim[ mt->type_Num ] = 10000;
		SendSkillResult( cn, skillno, target_id, x, y, ep->reclaim[ mt->type_Num ]/ 100 );
		return 1; 
	}
	
FAIL_:
	
	SendSkillResultFail( cn, skillno, why );
	return 0;
}	
/*int skill_Sow				(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id )
{	
	DWORD attr = ItemList[target_id].attr[2];
	
	if( attr & IA2_FARM0 ) { SendSkillResultFail(cn, skillno); return 0; }
	if( attr & IA2_FARM1 ) { SendSkillResultFail(cn, skillno); return 0; }
	if( attr & IA2_FARM2 ) { SendSkillResultFail(cn, skillno); return 0; }
	if( attr & IA2_FARM3 ) { SendSkillResultFail(cn, skillno); return 0; }
	if( attr & IA2_FARM4 ) { SendSkillResultFail(cn, skillno); return 0; }
	
	int ret = ReturnSkillProbablity( cn, skillno, x, y, targettype, target_id );
	if( ret == 0 ) { SendSkillResultFail(cn, skillno); return 0; }
	
	int probablity;
	lpMAPSKILLTABLE	mt = skill_ReturnProbablityFromSkillTable( &probablity,  1, x, y );
	
	ItemList[target_id].attr[2] |= IA2_FARM0;
	ItemList[target_id].attr[2] |= IA2_NOTMOVE;
	ItemList[target_id].attr[IATTR_DURATION] = g_curr_time + 6;
	
	SetArea( CHANGE_IA2_ATTR_ITEM_AREA, target_id );
	SetArea( CHANGE_IA2_DURATION_ITEM_AREA, target_id );
	
	SendSkillResult( cn, skillno, target_id, x, y, mt != NULL );
	
	return 1; 
}*/	
	

int skill_Sow				(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id, POS s )
{
	ItemAttr *item, ditem;
	CItem *ii;	
	lpMAPSKILLTABLE	mt;
	int probablity;
	int why = 0;
	DWORD har;	
	t_earthpower *ep;
	int hitrate;
	int tep;
	int iSendValue = 0;		// acer
	
	CHARLIST *pCh = CheckServerId(cn);//030102 lsw
	if(!pCh){return 0;}

	switch(s.type) 
	{			
		case INV: 	item = &pCh->inv[s.p1][s.p2][s.p3];	break;
		case HAND:	item = &pCh->handheld;				break;
		case EQUIP:	item = &pCh->equip[s.p3];			break;
		case QUICK:	item = &pCh->quick[s.p3];			break;
		default : goto ERROR_;
	}			
	if( !item->attr[IATTR_MUCH])//수량이 모자라다면 기술이 안된다
	{
		goto ERROR_;
	}
	if( item->item_no != target_id )
	{
		goto ERROR_;
	}
	
	ii = ItemUnit( item->item_no );
	if( ii == NULL ) 
	{
		goto ERROR_;
	}
	
	if( ii->GetItemKind() != IK_SEED ) 
	{
		goto ERROR_;
	}

	mt = skill_ReturnProbablityFromSkillTable( &probablity,  1, x, y );

	if( NULL == mt)
	{
		why = SKILLFAIL_NOT_PROPER_PLACE; 
		goto FAIL_; 
	}

	hitrate = ReturnSkillProbablity( cn, skillno, x, y, targettype, target_id );
	if( 0 == hitrate ) 
	{
		goto ERROR_; 
	}
	
	memcpy( &ditem, item, sizeof( ItemAttr ) );
	iSendValue = ItemMgr.DecItemDur(*item,1);
	
	SendServerEachItem(&s,item,cn);		// acer

	ditem.attr[ IATTR_ATTR]		|=	IA2_FARM0;
	ditem.attr[ IATTR_ATTR]		|=	IA2_NOTMOVE;
	ditem.attr[IATTR_DURATION]	 =  g_curr_time + ITEM_FARM_GROW_TIME;
			
//			
//          개간도/100 * Produce_Fix/100 * 기본성공률 * ( 101 - 작물의 Lv ) 
//            100 *   100      *     100%   *       100       = 100,000,000
//			
//          하나의 구역에서 얻을수 있는 수확물은 위의 계산에서 나온값 / 1,000,000 이다. 
// 인 자 :	
//	

	ep = &EarthPower;

	tep = ep->reclaim[ mt->type_Num ] / 3;
	if( tep < 0 ) tep = 0;
	if( tep > 10000 ) tep = 10000;

	har = tep;
	har /= 100;
	har *= ( SkillTbl[ skillno].Produce_Fix / 100 );
	har *= hitrate;
	har *= ( 101 - ii->GetLevel() );
	har /= 1000000;

	ep->reclaim[ mt->type_Num ] -= tep;

	ditem.attr[ IATTR_RESERVED0] = har;
			
	AddItemList( ditem.item_no, ditem.attr, 0, x, y, 0,0 );
			
	SendSkillResult( cn, skillno, target_id, x, y, mt != NULL );	
			
	return 1; 
FAIL_ :
ERROR_:

	SendSkillResultFail( cn, skillno, why );
	return 0;
}	
	
int skill_Havest			(  int cn, const int skillno, const int x, const int y, const int targettype, const int target_id )
{		
	ItemAttr item = {0,};
	int why = 0;
	CItem *ii;
	int ret;
	LPITEMLIST target;
	int getitemno;
	int amount;
	int hitrate;
	int i;
	CHARLIST *ch = GetCharListPtr(cn);
	if (!ch) return 0;
	int sx = ch->X;
	int sy = ch->Y;

	
	if( target_id < 0 || target_id >= MAX_ITEM_LIST ) goto FAIL_;

	target = &ItemList[ target_id];
	ii = ItemUnit( target->item_no );
	if( ii == NULL ) { why = SKILLFAIL_UNKNOWN_ITEM; goto FAIL_; }

	if( ii->GetItemKind() != IK_SEED ){	why = SKILLFAIL_NO_SEED;	goto FAIL_; }

	ret = SkillMgr.FindLearnItem( ch, target->item_no );
	if( ret == false )  { why = SKILLFAIL_UNKNOWN_ITEM; goto FAIL_; }

	if( target->attr[ IATTR_ATTR ] & IA2_FARM7 ) 
	{	
		hitrate = ReturnSkillProbablity(cn, skillno, x, y, targettype, target_id );

		getitemno = ii->Getmouse_MRD();
		getitemno /= 100;
		if( getitemno == 0 ) { why = SKILLFAIL_NO_SEED; goto FAIL_; }
		
		item = GenerateItem( getitemno );//020509 lsw
		if( item.item_no == 0 ) { why = SKILLFAIL_NO_SEED; goto FAIL_; }

		// 한번 낮질에 수확할수 있는 량을 정한다. 
		amount = 5;
		if( target->attr[ IATTR_RESERVED0] < amount ) amount = target->attr[ IATTR_RESERVED0];
		target->attr[ IATTR_RESERVED0] -= amount;
		
		for( i = 0 ; i < amount ; i ++)
		{
			DropItem( sx-50+rand()%100, sy-50+rand()%100, &item);
		}
		
		//1002 zhh
		if( i == 0 ) { why = 0; goto FAIL_; }
		
		// 모두 수확했으면..
		if( target->attr[ IATTR_RESERVED0] == 0 ) RemoveItemList( target_id);

		//스킬 능력치 80 이상이고 //확률은 1/500
/*
		if((connections[cn].chrlst.skillexp[ SkillTbl[skillno].MotherSkillType].skillexp / 10000)  > 80)
		{
			const int r = rand()%10000;
			if(TAKE_SADONIX_PERCENT > r)
			{
				GiveSadonixToGround(cn, SADONIX_NO,skillno,MapInfo[MapNumber].port,sx, sy);//021010 lsw
			}
		}
*/
		SkillMgr.IncSkillCount(ch,BLESSLEAF_NO,skillno,ItemMgr.GetSkillPointByItem(item));//스킬 카운트 증가
		SendSkillResult( cn, skillno, target_id, x, y, 1 );
	}	
	else {  why = SKILLFAIL_NOT_YET_HARVEST; goto FAIL_; }
		
	return 1; 
		
FAIL_:	
		
	SendSkillResultFail( cn, skillno, why );
	return 0;	
		
}		

//010604 lsw
int skill_Mining	(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id )
{
	CHARLIST *ch = CheckServerId(cn);//020720 lsw
	if(!ch){return 0;}

	ItemAttr item = {0,};
	int probablity;
	lpMAPSKILLTABLE mt;
	short int subType;
	int why = 0;
	int i;
	int r;
	int hitrate;
	int sx, sy;
	int itemno;
	int amount;
	t_earthpower *ep;
	
	int checkitem  = 0;//011014 lsw
	
	DecreaseEquipDuration(ch,rand()%8+1,WT_WEAPON,0,true);//020720 lsw

	mt = skill_ReturnProbablityFromSkillTable( &probablity,  TOOL_MINING , x, y );
	if( mt == NULL ) { why = SKILLFAIL_NOT_PROPER_PLACE; goto FAIL_; }

	r = rand()%100;

//	hitrate = ReturnSkillProbablityNoExpAdd( cn, skillno, x, y, targettype, target_id );
	hitrate = ReturnSkillProbablity( cn, skillno, x, y, targettype, target_id );//010612 lsw 예전으로
	if( r >= hitrate ) { goto FAIL_; }

	ep = &EarthPower;
	
	if( ep->mining[ mt->type_Num ] > 0 )//	매장량이 있으면 
	{	
//		AddSkillExp( cn, skillno, hitrate );//010607 lsw
		subType = mt->subType;
		itemno = ReturnFromSkillMapTable(cn,TOOL_MINING,subType,hitrate - r,RETURN_SMT_ITEMNO);//020214 lsw
		//011014 lsw >
		checkitem = SkillMgr.FindLearnItem(GetCharListPtr(cn),itemno);
		if ( checkitem != -1 ) {goto FAIL_;}
		//011014 lsw <

		if( itemno == 0 )  { goto FAIL_; }
		
		//1013 zhh
		int demand=ReturnFromSkillMapTable(cn,TOOL_MINING,subType,hitrate - r,RETURN_SMT_DEMAND);//020214 lsw
		if(demand!=-9999)
			amount = SkillTbl[ skillno].Produce_Fix * ( abs(hitrate - demand)/15 ) / 10000 +1;
		else
		{
			amount=1;
		}
		
		ep->mining[ mt->type_Num ] -= amount;
		if( ep->mining[ mt->type_Num ] < 0 ) ep->mining[ mt->type_Num ]  = 0;
	}	
	else
	{	
		why = SKILLFAIL_NO_MINING;
		goto FAIL_; 
	}	
		
	sx = ch->X;
	sy = ch->Y;
	item = GenerateItem( itemno);//020509 lsw 
	for( i = 0 ; i < amount;  i ++)
	{
		DropItem( sx-50+rand()%100, sy-50+rand()%100, &item);
	}
	SkillMgr.IncSkillCount(ch,SADONIX_NO,skillno,ItemMgr.GetSkillPointByItem(item));//스킬 카운트 증가
	SendSkillResult( cn, skillno, target_id, x, y, 1 );
	return 1; 
		
FAIL_ :		

	//1013 zhh
	if(why==0 || why==SKILLFAIL_NO_MINING)
		GiveBasicItem(cn,TOOL_MINING,1);

	SendSkillResultFail( cn, skillno, why );
	return 0;
}		
		
		
int skill_Refine			(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id )
{
	SendSkillResult( cn, skillno, target_id, x, y, -999 );
		
	return 1; 
}
//010604 lsw
int skill_Fish				(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id )
{ 
	CHARLIST *ch = CheckServerId(cn);//020720 lsw
	if(!ch){return 0;}

	ItemAttr item = {0,};
	int probablity;
	lpMAPSKILLTABLE mt;
	int why = 0; 
	int r;
	int sx, sy;
	int hitrate;
	int checkitem  = 0;//011014 lsw
	
	// 낚시.....
	mt = skill_ReturnProbablityFromSkillTable( &probablity,  TOOL_FISHING, x, y );
	if( mt == NULL ) { why = SKILLFAIL_NOT_PROPER_PLACE; goto FAIL_; }

	DecreaseEquipDuration(ch,rand()%8+1,WT_WEAPON,0,true);//020720 lsw

	r = rand()%100;
//	hitrate = ReturnSkillProbablityNoExpAdd( cn, skillno, x, y, targettype, target_id );
	hitrate = ReturnSkillProbablity( cn, skillno, x, y, targettype, target_id );//010612 lsw 예전으로
	if( r >= hitrate ) { goto FAIL_; }

	//1009 zhh
	int i;
	int itemno;
	int amount;
	t_earthpower *ep;
	short int subType;
	ep = &EarthPower;
	//	매장량이 있으면 
	if( ep->fishing[ mt->type_Num ] > 0 )
	{	
//		AddSkillExp( cn, skillno, hitrate );//010607 lsw
		subType = mt->subType;
		itemno = ReturnFromSkillMapTable(cn,TOOL_FISHING,subType,hitrate - r,RETURN_SMT_ITEMNO);//020214 lsw
		if( itemno == 0 )  { goto FAIL_; }
	
		//1013 zhh
		int demand=ReturnFromSkillMapTable(cn,TOOL_FISHING,subType,hitrate - r,RETURN_SMT_DEMAND);//020214 lsw
		if(demand!=-9999)
			amount = SkillTbl[ skillno].Produce_Fix * ( abs(hitrate - demand)/15 ) / 10000 +1;
		else
		{
			amount=1;
		}
		
		ep->fishing[ mt->type_Num ] -= amount;
		if( ep->fishing[ mt->type_Num ] < 0 ) ep->fishing[ mt->type_Num ]  = 0;
	}	
	else
	{	
		//1017 zhh
		why = SKILLFAIL_NO_FISHING;
		goto FAIL_; 
	}	
		
	sx = ch->X;
	sy = ch->Y;
	item = GenerateItem( itemno);//020509 lsw 

	//011014 lsw >
	checkitem = SkillMgr.FindLearnItem(GetCharListPtr(cn),itemno);
	if ( checkitem != -1 ) {goto FAIL_;}
	//011014 lsw <

	for( i = 0 ; i < amount;  i ++)
	{
		DropItem( sx-50+rand()%100, sy-50+rand()%100, &item);
	}

	SkillMgr.IncSkillCount(ch,BLESSLEAF_NO,skillno,ItemMgr.GetSkillPointByItem(item));//스킬 카운트 증가
	SendSkillResult( cn, skillno, target_id, x, y, 1 );
	return 1;

FAIL_:

	//1013 zhh //1017 zhh
	if(why==0 || why==SKILLFAIL_NO_FISHING)
		GiveBasicItem(cn,TOOL_FISHING,1);

	SendSkillResultFail( cn, skillno, why );
	return 0; 
}



//1016 zhh 모두 새로 구현된 함수
int skill_Netting			(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id )
{ 
	CHARLIST *ch = CheckServerId(cn);//020720 lsw
	if(!ch){return 0;}

	ItemAttr item = {0,};
	int probablity;
	lpMAPSKILLTABLE mt;
	int why = 0; 
	int r;
	int sx, sy;
	int hitrate;
	int checkitem =0;//011014 lsw 
	
	DecreaseEquipDuration(ch,rand()%8+1,WT_WEAPON,0,true);//020720 lsw

	// 낚시.....
	mt = skill_ReturnProbablityFromSkillTable( &probablity,  TOOL_FISHING, x, y );
	if( mt == NULL ) { why = SKILLFAIL_NOT_PROPER_PLACE; goto FAIL_; }

	hitrate = ReturnSkillProbablity( cn, skillno, x, y, targettype, target_id );
	r = rand()%100;
	//1011 zhh
	if( r >= hitrate ) { goto FAIL_; }

	//1009 zhh
	int i;
	int itemno;
	int amount;
	t_earthpower *ep;
	short int subType;
	ep = &EarthPower;
	//	매장량이 있으면 
	if( ep->fishing[ mt->type_Num ] > 0 )
	{	
		subType = mt->subType;
		itemno = ReturnFromSkillMapTable(cn,TOOL_FISHING,subType,hitrate - r,RETURN_SMT_ITEMNO);//020214 lsw
		if( itemno == 0 )  { goto FAIL_; }
	
		//1013 zhh
		int demand=ReturnFromSkillMapTable(cn,TOOL_FISHING,subType,hitrate - r,RETURN_SMT_DEMAND);//020214 lsw
		if(demand!=-9999)
			amount = SkillTbl[ skillno].Produce_Fix * ( abs(hitrate - demand)/15 ) / 10000 +1;
		else
			amount=1;
	//	amount = SkillTbl[ skillno].Produce_Fix * (( hitrate ) + 2 ) / 10000;
		
		ep->fishing[ mt->type_Num ] -= amount;
		if( ep->fishing[ mt->type_Num ] < 0 ) ep->fishing[ mt->type_Num ]  = 0;
	}	
	else
	{	
		//1017 zhh
		why = SKILLFAIL_NO_FISHING;
		goto FAIL_; 
	}	
		
	sx = ch->X;
	sy = ch->Y;
	item = GenerateItem( itemno);//020509 lsw 
	//011014 lsw >
	
	checkitem = SkillMgr.FindLearnItem(GetCharListPtr(cn),itemno);
	if ( checkitem != -1 ) {goto FAIL_;}
	//011014 lsw <

	for( i = 0 ; i < amount;  i ++)
	{
		DropItem( sx-50+rand()%100, sy-50+rand()%100, &item);
	}
	SkillMgr.IncSkillCount(ch,BLESSLEAF_NO,skillno,ItemMgr.GetSkillPointByItem(item));//스킬 카운트 증가
	SendSkillResult( cn, skillno, target_id, x, y, 1 );
	return 1;

FAIL_:

	//1013 zhh	//1017 zhh
	if(why==0 || why==SKILLFAIL_NO_FISHING)
		GiveBasicItem(cn,TOOL_FISHING,1);

	SendSkillResultFail( cn, skillno, why );
	return 0; 
}




//010604 lsw
int skill_Cutting			(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id )
{	
	ItemAttr item = {0,};
	int probablity;
	lpMAPSKILLTABLE mt;
	int why = 0; 
	int r;
	int sx, sy;
	int hitrate;
	int amount;
	int subType;
	int itemno;
	int i;
	CHARLIST *ch = CheckServerId(cn);//020720 lsw
	if(!ch){return 0;}
	t_earthpower *ep;	

	//	벌목......
	mt = skill_ReturnProbablityFromSkillTable( &probablity,  TOOL_CHOPPING, x, y );
	if( mt == NULL ) { why = SKILLFAIL_NOT_PROPER_PLACE; goto FAIL_; }

	r = rand()%100;
//	hitrate = ReturnSkillProbablityNoExpAdd( cn, skillno, x, y, targettype, target_id );
	hitrate = ReturnSkillProbablity( cn, skillno, x, y, targettype, target_id );//010612 lsw 예전으로
	DecreaseEquipDuration(ch,rand()%8+1,WT_WEAPON,0,true);//020803 lsw
	if( r >= hitrate ) { goto FAIL_; }

	//1011 zhh

	ep = &EarthPower;
	//	벨 나무가 있으면 
	if( ep->cutting[ mt->type_Num ] > 0 )
	{	
//		AddSkillExp( cn, skillno, hitrate );//010607 lsw
		subType = mt->subType;
		itemno = ReturnFromSkillMapTable(cn,TOOL_CHOPPING,subType,hitrate - r,RETURN_SMT_ITEMNO);//020214 lsw
		if( itemno == 0 )  { goto FAIL_; }
	
		//1013 zhh
		int demand=ReturnFromSkillMapTable(cn,TOOL_CHOPPING,subType,hitrate - r,RETURN_SMT_DEMAND);//020214 lsw
		if(demand!=-9999)
			amount = SkillTbl[ skillno].Produce_Fix * ( abs(hitrate - demand)/15 ) / 10000 +1;
		else
		{
			amount=1;
		}
	//	amount = 1 + rand() % 5;

		ep->cutting[ mt->type_Num ] -= amount;
		if( ep->cutting[ mt->type_Num ] < 0 ) ep->cutting[ mt->type_Num ]  = 0;
		SendSkillGauge( cn, skillno, x, y );
	}	
	else
	{	
		why = SKILLFAIL_NO_CUTTING;
		goto FAIL_; 
	}	
		
	sx = ch->X;
	sy = ch->Y;

	item = GenerateItem( itemno );
	for( i = 0 ; i < amount ; i ++)
	{
		DropItem( sx-50+rand()%100, sy-50+rand()%100, &item);
	}
	SkillMgr.IncSkillCount(ch,SADONIX_NO,skillno,ItemMgr.GetSkillPointByItem(item));//스킬 카운트 증가
	SendSkillResult( cn, skillno, target_id, x, y, 1 );
	return 1; 
		
FAIL_:	

	//1013 zhh
	if(why==0 || why==SKILLFAIL_NO_CUTTING)
		GiveBasicItem(cn,TOOL_CHOPPING,1);

	SendSkillResultFail( cn, skillno, why );
	return 0; 
}		

// 1005 zhh
int skill_Lumbering			(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id )
{
	ItemAttr item = {0,};
	int why = 0, r = 0, hitrate = 0;
	
	CItem_Join *join = NULL;
	CItem *ii = NULL;
	int iResultItemNo = 0;
	int iGroundItemNo = 0;
	CHARLIST* pCh = CheckServerId(cn);
	if (pCh == NULL) return 0; // finito null check
	const int iDestItemNo = pCh->cur_skillKey;//Clicked Item
	
	const ITEMLIST *pItemList = &ItemList[target_id];
	iGroundItemNo = pItemList->item_no;
	if( FALSE == pItemList->bAlive
	||	iDestItemNo != iGroundItemNo
	||  !iGroundItemNo) 
	{
		why = ITEMMAKE_NO;
		goto _ERROR;
	}
	
	ii = ItemUnit(iGroundItemNo);
	if( !ii )
	{
		goto _FAIL;
	}

	iResultItemNo = ii->Getmouse_MRD()/100;	// 목적 ItemNo를 구한다. 
	if( !iResultItemNo )	
	{  
		why = SKILLFAIL_NOT_RESOURCE; 
		//goto _FAIL; 
		goto _ERROR;
	}

	join = ReturnItemJoinTable( cn, skillno, iResultItemNo );
	if(!join){goto _ERROR;}

	hitrate = ReturnSkillProbablity( cn, skillno, x, y, targettype, target_id , join );
	r = rand()%100;
	if( r >= hitrate ) 
	{
		goto _FAIL; 
	}
 
	RemoveItemList( target_id );
	item = GenerateItem(iResultItemNo);
	AddItemList( item.item_no, item.attr,0,  x, y, 0,0 );
	
	SendSkillResult( cn, skillno, target_id, x, y, 1 );
	return 1; 

_ERROR:	//Data Error Dest Item No Delete
	{
		SendSkillResultFail( cn, skillno, why );
	}
	return 0; 
_FAIL: //Skill Failed
	{
		RemoveItemList( target_id );
		SendSkillResultFail( cn, skillno, why );
	}
	return 0; 
}	
	
// 010129 KHS
int skill_Butchering		(const int cn, const int skillno, const int x, const int y, const int targettype, int target_id )
{	
	ItemAttr item = {0,};
	CHARLIST *n;
	CHARLIST *butcher = GetCharListPtr(cn);//010302 lsw// 도축자의 캐릭터 받아옴 
	if (!butcher) return 0;
	int itemno;
	int sx, sy;
	int why = 0;
	int ret,r;
	int checkitem;
	CHARLIST *ch = CheckServerId(cn);//020720 lsw
	if(!ch){return 0;}

	if( target_id < 10000 )
	{	
		//010204 lsw			// 010210 YGI
		why = -84;//010302 lsw
		goto FAIL_;
	}
		
	target_id -= 10000;
	if( target_id < NPC_LIST_START || target_id >= MAX_NPC_LIST )
	{
		//010204 lsw			// 010210 YGI
		why = -84;//010302 lsw
		goto FAIL_;
	}
		
	n = &NPCList[ target_id];
	if (!n) goto FAIL_;

	if( n->bAlive != DEAD_ )  
	{
		why = -82;	
		goto FAIL_;
	}
		
	//1101 zhh
	switch( n->SprNo )
	{	
	case 18: case 19 : case 20 : case 21: case 22: case 23: case 24: case 56: case 57: case 63: case 64: case 65: case 66: case 67: case 69: case 71: case 72: case 73: case 74: case 75: case 76: case 77: case 78: case 79: case 80:
		goto FAIL_;
	}	

	ret = ReturnSkillProbablity( cn, skillno, x, y, targettype, target_id );
	r = rand()%100;
	//1011 zhh
	if( r > ret ) goto FAIL_;
				
	// Meat..
	itemno = n->Get_meat;

	checkitem = SkillMgr.FindLearnItem(butcher,itemno);
	if(checkitem== -1)//010302  lsw
	{
	if( n->Meat_amount > 0 )
		{
			n->Meat_amount --;
			sx = connections[cn].chrlst.X - 30 + rand()%60;
			sy = connections[cn].chrlst.Y - 30 + rand()%60;
			item = GenerateItem(itemno);	
			DropItem( sx-50+rand()%100, sy-50+rand()%100, &item);
			//1002 zhh
			SendSkillResult( cn, skillno, target_id, x, y, 1 );
		}
	}	
	else
	{
		why = -10;
		goto FAIL_;
	}

	// Leather..
	
	itemno = n->Get_skin;
	checkitem = SkillMgr.FindLearnItem(butcher,itemno);
	if(checkitem== -1)
	{
		if( n->Skin_amount > 0 )
		{
			n->Skin_amount --;
			sx = connections[cn].chrlst.X - 30 + rand()%60;
			sy = connections[cn].chrlst.Y - 30 + rand()%60;
			item = GenerateItem( itemno);//020509 lsw 
			DropItem( sx-50+rand()%100, sy-50+rand()%100, &item);
			//1002 zhh
			SendSkillResult( cn, skillno, target_id, x, y, 1 );
/*
			//011014 lsw >
			if( n->Level >= 36 )
			{
				const int r = rand()%10000;
				if(TAKE_SADONIX_PERCENT > r)
				{
					GiveSadonixToGround(cn,SADONIX_NO,skillno,MapInfo[MapNumber].port,sx, sy);
				}
			}
			//011014 lsw <
*/
		}
	}
	else
	{
		why = -10;
		goto FAIL_;
	}

	if( n->Skin_amount == 0 &&  n->Meat_amount == 0 )
	{
		why = SKILLFAIL_NO_MEET;
		goto FAIL_;
	}
	SkillMgr.IncSkillCount(ch,SADONIX_NO,skillno,ItemMgr.GetSkillPointByItem(item));//스킬 카운트 증가
	SendSkillResult( cn, skillno, target_id, x, y, 1 );

	return 1; 
		
FAIL_:	
		
		
	SendSkillResultFail(cn, skillno, why ); 
	
		
	return 0;
}		
	

//010604 lsw
int skill_Digging			(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id )
{
	CHARLIST *ch = CheckServerId(cn);//020720 lsw
	if(!ch){return 0;}

	ItemAttr item = {0,};
	int probablity;
	lpMAPSKILLTABLE mt;
	short int subType;
	int why = 0; 
	int r;
	int hitrate;
	int amount;
	int itemno;
	int i;
	t_earthpower *ep;
	int checkitem = 0;
	
	mt = skill_ReturnProbablityFromSkillTable( &probablity,  TOOL_HUB, x, y );
	if( mt == NULL ) { why = SKILLFAIL_NOT_PROPER_PLACE; goto FAIL_; }
	
	DecreaseEquipDuration(ch,rand()%8+1,WT_WEAPON,0,true);//020720 lsw

	r = rand()%100;
//	hitrate = ReturnSkillProbablityNoExpAdd( cn, skillno, x, y, targettype, target_id );
	hitrate = ReturnSkillProbablity( cn, skillno, x, y, targettype, target_id );//010612 lsw 예전으로
	if( r >= hitrate ) { goto FAIL_; }

	ep = &EarthPower;
	//	케낼 약초가 있으면 
	if( ep->digging[ mt->type_Num ] > 0 )
	{	
//		AddSkillExp( cn, skillno, hitrate );//010607 lsw
		subType = mt->subType;
		itemno = ReturnFromSkillMapTable(cn,TOOL_HUB,subType,hitrate - r,RETURN_SMT_ITEMNO);//020214 lsw
		if( itemno == 0 )  { goto FAIL_; }
		
		//1013 zhh
		int demand=ReturnFromSkillMapTable(cn,TOOL_HUB,subType,hitrate - r,RETURN_SMT_DEMAND);//020214 lsw
		if(demand!=-9999)
		{
			amount = SkillTbl[ skillno].Produce_Fix * ( abs(hitrate - demand)/15 ) / 10000 +1;
		}
		else
		{
			amount=1;//나올 수량이로고..
		}
		ep->digging[ mt->type_Num ] -= amount;
		if( ep->digging[ mt->type_Num ] < 0 ) ep->digging[ mt->type_Num ]  = 0;
		SendSkillGauge( cn, skillno, x, y );
	}	
	else
	{	
		why = SKILLFAIL_NO_DIGGING;
		goto FAIL_; 
	}	
		
	int sx, sy;
	sx = connections[cn].chrlst.X;
	sy = connections[cn].chrlst.Y;
		
	item = GenerateItem( itemno);//020509 lsw 
	//011014 lsw >
	checkitem = SkillMgr.FindLearnItem(GetCharListPtr(cn),itemno);
	if ( checkitem != -1 ) {goto FAIL_;}
	//011014 lsw <

	for( i = 0 ; i < amount ; i ++)
	{
		DropItem( sx-50+rand()%100, sy-50+rand()%100, &item);
	}

	SkillMgr.IncSkillCount(ch,BLESSLEAF_NO,skillno,ItemMgr.GetSkillPointByItem(item));//스킬 카운트 증가
	SendSkillResult( cn, skillno, target_id, x, y, item.item_no );
	
	return 1; 
FAIL_ :	

	//1013 zhh
	if(why==0 || why==SKILLFAIL_NO_DIGGING)
		GiveBasicItem(cn,TOOL_HUB,1);
		
	SendSkillResultFail( cn, skillno, why );
	return 0;
}
int skill_Makepowder		(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id ){ SendSkillResult( cn, skillno, target_id, x, y, -999 );return 1; }



//010604 lsw 
int skill_Shear				(const int cn, const int skillno, const int x, const int y, const int targettype, int target_id )
{ 
	ItemAttr item = {0,};
	CHARLIST *n;
	int itemno;
	int sx, sy;
	int hitrate;
	int why = 0;
	int r;
//사람 한테 썼는데 메세지 이상하고, 경험치 동시에 안올라감?

	if( target_id < 10000 ) 
	{
		//010204 lsw			// 010210 YGI
		why = -83;	//010302 lsw
		goto FAIL_;	// PC에게 기술을 썻으면 에러.
	}

	target_id -= 10000;
	n = &NPCList[ target_id];
	if (!n) goto FAIL_;
	
	if( n->bAlive != ALIVE_ ) 
	{	
		//010204 lsw			// 010210 YGI
		why = -85; //this2 lsw lsw
		goto FAIL_;
	}
	// Hair..
	itemno = n->Get_Wool;
	if( n->Wool_amount > 0 )
	{
		hitrate = ReturnSkillProbablity( cn, skillno, x, y, targettype, target_id );//010604 lsw
		r = rand()%100;
		if( r > hitrate )  goto FAIL_;

		n->Wool_amount--;
		sx = connections[cn].chrlst.X + 30 + rand()%60;
		sy = connections[cn].chrlst.Y + 30 + rand()%60;
		item = GenerateItem( itemno);//020509 lsw 
		
		DropItem( sx-50+rand()%100, sy-50+rand()%100, &item);

		//1002 zhh
		SendSkillResult( cn, skillno, target_id, x, y, 1 );
		return 1; 
	}
	else why = SKILLFAIL_NO_SHEAR;
	
FAIL_:
	SendSkillResultFail(cn, skillno, why ); 
	return 0;
}


int skill_Manufacturing(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id )
{ 
	return 1; 
}

int skill_Cooking			(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id ){ SendSkillResult( cn, skillno, target_id, x, y, -999 );return 1; }
int skill_Clothing			(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id ){ SendSkillResult( cn, skillno, target_id, x, y, -999 );return 1; }

int skill_Tailoring			(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id ){ SendSkillResult( cn, skillno, target_id, x, y, -999 );return 1; }
int skill_Bowcrafting		(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id ){ SendSkillResult( cn, skillno, target_id, x, y, -999 );return 1; }
int skill_Fletching			(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id ){ SendSkillResult( cn, skillno, target_id, x, y, -999 );return 1; }
int skill_MakePotion		(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id ){ SendSkillResult( cn, skillno, target_id, x, y, -999 );return 1; }
int skill_MakeMetal			(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id ){ SendSkillResult( cn, skillno, target_id, x, y, -999 ); return 1; }
int skill_MakeBottle		(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id ){ SendSkillResult( cn, skillno, target_id, x, y, -999 );return 1; }
int skill_Candlemaking		(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id ){ SendSkillResult( cn, skillno, target_id, x, y, -999 );return 1; }
int skill_Oiling			(const int cn, const int skillno, const int x, const int y, const int targettype, int target_id )
{ 
	ItemAttr item = {0,};
	CHARLIST *n;
	int itemno;
	int sx, sy;

	int ret = ReturnSkillProbablity( cn, skillno, x, y, targettype, target_id );
	if( ret == 0 ) { SendSkillResultFail(cn, skillno); return 0; }

	if( target_id < 10000 )
	{
		// 사라믕ㄹ Butchering할순  없지...
		SendSkillResultFail( cn, skillno );
		return 0;
	}

	target_id -= 10000;
	n = &NPCList[ target_id];
	if (!n) return 0;
	if( n->bAlive != DEAD_ )  { SendSkillResultFail(cn, skillno); return 0; }

	// Wax..
	itemno = n->Get_beewax;
	if( n->BeeWax_amount > 0 )
	{
		n->BeeWax_amount--;
		sx = connections[cn].chrlst.X - 30 + rand()%60;
		sy = connections[cn].chrlst.Y - 30 + rand()%60;

		item = GenerateItem( itemno);//020509 lsw 
		AddItemList( item.item_no, item.attr,0,  sx, sy, 0,0 );
		//1002 zhh
		SendSkillResult( cn, skillno, target_id, x, y, 1 );
	}

	return 1; 
}

int skill_Snooping			(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id )
{
	// Finito - Always make snooping fail
	// Stops snooping from being successful.


/*
	int ret,r;

	LPCHARLIST target;//020530 lsw 타겟
	LPCHARLIST caster;

	if( cn == target_id )	goto FAIL_;
	if( target_id >= 10000 )	goto FAIL_;

	if( MapNumber == 30 ) goto FAIL_; // Scholium에서는 훔치기 할 수 없다.

	target = CheckServerId(target_id);
	caster = CheckServerId(cn);
	
	if( !target ) {goto FAIL_;}
	if( !caster ) {goto FAIL_;}
	
	if(strcmp(target->Name,caster->cur_skillTargetName))//020531 lsw 
	{
		goto FAIL_;
	}
	if(target->IsCounselor()){goto FAIL_;}//운영자는 못훔쳐//020730 lsw
	if(target->IsReporter()){goto FAIL_;}//Finito Added to Reporter cant be snooped

	ret = ReturnSkillProbablity( cn, skillno, x, y, targettype, target_id );

	r = rand()%100;
	if( ret > r )
	{
		if( target->bAlive == DEAD_ && 	target->viewtype != VIEWTYPE_GHOST_ )
		{
			SendOtherInv( CMD_OTHER_CH_INV, target_id, connections, cn );
			SendSkillResult( cn, skillno, target_id, x, y, 1 );

			CheckNK( cn, target_id,NK_TYPE_STEAL_ );
		}
		else goto FAIL_;
	}

	return 1; 

FAIL_:*/

	SendSkillResultFail(cn, skillno); return 0;
}

int skill_Makefire			(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id )
{	
//	int ret = ReturnSkillProbablity( cn, skillno, x, y, targettype, target_id );
//	if( ret == 0 ) { SendSkillResultFail(cn, skillno); return 0; }
	int ret = 1;
	switch( ItemList[target_id].item_no  )
	{
	case 1011	: 
	case 1012	:	
	case 1023	:	//	<Nutmeg Log>	<비자나무>
	case 1024	:	//	<나무>	<리기다나무>
	case 1025	:	//	<Fir Log>	<구상나무>
	case 1026	:	//	<Oak Log>	<상수리나무>
	case 1027	:	//	<나무>	<거제수나무>
	case 1028	:	//	<Beech Log>	<너도밤나무>
	case 1029	:	//	<Whitebirch Log>	<자작나무 >
	case 1030	:	//	<Nutmeg Timber>	<비자나무 목재>
	case 1031	:	//	<나무>	<리기다나무 목재>
	case 1032	:	//	<Fir Timber>	<구상나무 목재>
	case 1033	:	//	<Oak Timber>	<상수리나무 목재>
	case 1034	:	//	<나무>	<거제수나무 목재>
	case 1035	:	//	<Beech Timber>	<너도밤나무 목재>
	case 1036	:	//	<Whitebirch Timber>	<자작나무 목재>
		break;
	default :		SendSkillResultFail(cn, skillno);		return 0;
	}
	
	if( ( ItemList[ target_id].attr[ IATTR_ATTR] & IA2_FIRE ) == 0 )
	{
		ItemList[ target_id].attr[ IATTR_ATTR] |= IA2_FIRE;
		ItemList[ target_id].attr[ IATTR_ATTR] |= IA2_NOTMOVE;
	
		// 원래는 만들어질때 시간을 가기고 만들어진다. 
		ItemList[ target_id].attr[IATTR_DURATION ] = g_curr_time + ITEM_FIRE_GROW_TIME;
			
		SetArea( CHANGE_IA2_ATTR_ITEM_AREA, target_id );
		SendSkillResult( cn, skillno, target_id, x, y, 1 );
	}		
					
	return 1; 
}			

// 1. 캐릭터의 n->tame
// 2. 시전자의 taming skil (SkillMotherExp 타입) Level이   
//  	NPC_Gen_Ref[n->SprNo].Taming_min보다 높아야 Taming할수 있다.
// 3. 
//1018 zhh 모두 바뀐 함수
//참고 자료
//#define SKILLFAIL_LOW_LEVEL_TO_TAMING		-110	//taming할 수 없는 능력치를 유저가 가지고 있다
//#define SKILLFAIL_NO_TAMING_ALLOWED		-111	//taming할 수 없는 대상
//#define SKILLFAIL_TARGET_IS_NOT_NPC		-112	//taming할 스 있는 NPC가 아니다
int skill_Taming			(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id )
{		
	int why=0, hitrate=0, r=0;
	int needed_max_tame;

	if( target_id - 10000 < NPC_LIST_START || target_id - 10000 >= MAX_NPC_LIST ) 
	{	
		//1018 zhh
		why=SKILLFAIL_TARGET_IS_NOT_NPC;
		SendSkillResultFail(cn, skillno,why); return 0;
	}
	//여기까지 오면 NPC가 아니고 최대 NPC번호 보다 적다
	
	if( target_id < 10000 ) { why=SKILLFAIL_TARGET_IS_NOT_NPC;	SendSkillResultFail(cn, skillno,why); return 0; }
	//여기까지 오면 일단 NPC다

	CHARLIST *n = &NPCList[ target_id - 10000];	
	CHARLIST *ch = CheckServerId( cn );
	if (!n) return 0;
	if( n->tamingable == 0 )
	{				
		why=SKILLFAIL_NO_TAMING_ALLOWED;
		SendSkillResultFail(cn, skillno,why); return 0;
	}			
	//여기까지 오면 테이밍 할 수 있는 NPC라는 이야기
	if( NPC_Gen_Ref[n->SprNo].Taming_min >= 100 )	
	{		
		why=SKILLFAIL_NO_TAMING_ALLOWED;
		SendSkillResultFail(cn, skillno,why); return 0;
	}
	// 여기까지 오면 테이밍 할 수 있는 NPC라는 이야기
	if (n->IsTamedNpc() || n->IsSummonMonster())
	{
		SendSkillResultFail(cn, skillno ); return 0;
	}

	int MyLevel = connections[ cn].chrlst.skillexp[ SkillTbl[skillno].MotherSkillType].skillexp / 10000;
	int MonLevel = n->GetLevel(); // CSD-030806

	if(		MonLevel>MyLevel)	hitrate = 70 - (MonLevel-MyLevel)*1.2;	
	else if(MonLevel==MyLevel)	hitrate = 70;	
	else if(MonLevel<MyLevel)	hitrate = 70 + ((MyLevel-MonLevel)*5)/8;	

	if(hitrate>95)	hitrate=95;
	if(hitrate<0)	hitrate=0;

	needed_max_tame = NPC_Gen_Ref[n->SprNo].Taming_max;

	AddSkillExp( cn, skillno, hitrate );

	// 이제부터 주인이 없고, 이놈을 태밍하는 사람도 주위에 없다. 

	r=rand()%100;
	if( hitrate > r )
	{
		int add=ReturnTamingPoint( cn ,skillno, target_id );
		
		n->tame += add;

		if(n->tame < 0)		n->tame=0;	// 에러검출루틴.. 
		if( n->tame > needed_max_tame ) n->tame = needed_max_tame;
	}

	// 태밍에 성공하였다. 
	if( n->tame >= needed_max_tame )
	{	//< CSD-031106
		n->SetTamed(true);
		strcpy_s(n->HostName, sizeof(n->HostName), connections[cn].chrlst.Name);
		n->SetMaster(cn);
		
		if (n->patterntype != NPC_PATTERN_TAME_)
		{
			n->oldpatterntype	= n->patterntype;
		}

		n->patterntype = NPC_PATTERN_TAME_;
		n->tame_cmd	= LC_TAME_STOP;

		SendNPC_Parameter(n->GetServerID(), NPC_PARAMETER_TAME_CMD,	n->tame_cmd);
		SendNPC_Parameter(n->GetServerID(), NPC_PARAMETER_OLDPATTERNTYPE, n->oldpatterntype);
		SendNPC_Parameter(n->GetServerID(), NPC_PARAMETER_PATTERNTYPE, n->patterntype);
		SendNPC_Parameter(n->GetServerID(), NPC_PARAMETER_ATTRIBUTE, AT_TAMING);

		n->tamingcountdown	=  60;
		SendSkillResult(cn, skillno, target_id, x, y, 200);
		SendToAllHostName(cn, n);
		if(ch != NULL)
			ch->TamedMonster.push_back(target_id - 10000);
		return 1; 
	}	//> CSD-031106

	SendSkillResult( cn, skillno, target_id, x, y, n->tame );
	return 1; 
}


int skill_Evaluate_Inteligence	(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id )
{
	LPCHARLIST ch;
	int ret = ReturnSkillProbablity( cn, skillno, x, y, targettype, target_id );
	if( ret == 0 ) { goto FAIL_; }

	if( target_id >= 10000 )	
	{
		if( target_id -10000 < NPC_LIST_START || target_id -10000 >= MAX_NPC_LIST ) goto FAIL_;
		ch = &NPCList[ target_id - 10000];

	}
	else
	{
		if( target_id < DRAGON_CONNECTIONS_START || target_id >= DRAGON_MAX_CONNECTIONS ) goto FAIL_;
		ch = &connections[ target_id].chrlst;
	}
	if( ch->bAlive == DEAD_ ) goto FAIL_;
	SendViewCharInfo( cn,  target_id, INT_, ch->Int, NULL );

	
	return 1; 

FAIL_:

	SendSkillResultFail(cn, skillno); 
	return 0; 
}

int skill_Evaluate_Wisdom		(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id )
{
	LPCHARLIST ch;
	int ret = ReturnSkillProbablity( cn, skillno, x, y, targettype, target_id );
	if( ret == 0 ) { goto FAIL_; }

	if( target_id >= 10000 )	
	{
		if( target_id -10000 < NPC_LIST_START || target_id -10000 >= MAX_NPC_LIST ) goto FAIL_;
		ch = &NPCList[ target_id - 10000];
	}
	else
	{
		if( target_id < DRAGON_CONNECTIONS_START || target_id >= DRAGON_MAX_CONNECTIONS ) goto FAIL_;
		ch = &connections[ target_id].chrlst;
	}

	if( ch->bAlive == DEAD_ ) goto FAIL_;
	SendViewCharInfo( cn,  target_id, WIS, ch->Wis, NULL );
	
	return 1; 

FAIL_:

	SendSkillResultFail(cn, skillno); 
	return 0; 
}

int skill_Anatomy_Judgement		(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id )
{
	LPCHARLIST ch;
	int ret = ReturnSkillProbablity( cn, skillno, x, y, targettype, target_id );
	if( ret == 0 ) { goto FAIL_; }

	if( target_id >= 10000 )	
	{
		if( target_id - 10000 < NPC_LIST_START || target_id - 10000 >=MAX_NPC_LIST ) goto FAIL_;
		ch = &NPCList[ target_id - 10000];
	}
	else
	{
		if( target_id < DRAGON_CONNECTIONS_START || target_id >= DRAGON_MAX_CONNECTIONS ) goto FAIL_;
		ch = &connections[ target_id].chrlst;
	}

	if( ch->bAlive != DEAD_ ) goto FAIL_;

	SendViewCharInfo( cn, target_id,  9999, 0, ch->KillerName );
	
	return 1; 

FAIL_:

	SendSkillResultFail(cn, skillno); 
	return 0; 
}

int skill_Item_Identification	(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id ){ SendSkillResult( cn, skillno, target_id, x, y, -999 );return 1; }
int skill_Crapple				(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id ){ SendSkillResult( cn, skillno, target_id, x, y, -999 );return 1; }
int skill_Swordmanship			(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id ){ SendSkillResult( cn, skillno, target_id, x, y, -999 );return 1; }
int skill_Archery				(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id ){ SendSkillResult( cn, skillno, target_id, x, y, -999 ); return 1; }
int skill_Fencing				(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id ){ SendSkillResult( cn, skillno, target_id, x, y, -999 );return 1; }
int skill_Mace_Fighting			(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id ){ SendSkillResult( cn, skillno, target_id, x, y, -999 );return 1; }
int skill_Pierce				(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id ){ SendSkillResult( cn, skillno, target_id, x, y, -999 );return 1; }
int skill_Whirl					(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id ){ SendSkillResult( cn, skillno, target_id, x, y, -999 );return 1; }
int skill_Hurl					(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id ){ SendSkillResult( cn, skillno, target_id, x, y, -999 );return 1; }
int skill_Parrying				(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id ){ SendSkillResult( cn, skillno, target_id, x, y, -999 );return 1; }
int skill_Double_Swordmanship	(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id ){ SendSkillResult( cn, skillno, target_id, x, y, -999 );return 1; }
int skill_Double_Macefighting	(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id ){ SendSkillResult( cn, skillno, target_id, x, y, -999 );return 1; }
int skill_Magery				(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id ){ SendSkillResult( cn, skillno, target_id, x, y, -999 );return 1; }
int skill_Orison				(const int cn, const int skillno, const int x, const int y, const int targettype, const int target_id ){ SendSkillResult( cn, skillno, target_id, x, y, -999 );return 1; }
	
void AddSkill( lpMAPSKILLTABLE *Header, lpMAPSKILLTABLE	lpST )		//링크드 리스트의 머리에 붙여 넣기
{		
	lpMAPSKILLTABLE t, temp; 
		
	if( *Header == NULL )		//링크드 리스트 최초 생성
	{	
		MemAlloc( *Header, sizeof( MAPSKILLTABLE ));

		(*Header)->type			= lpST->type;
		(*Header)->x			= lpST->x; 
		(*Header)->y			= lpST->y; 
		(*Header)->skillno		= lpST->skillno;
		(*Header)->tile_Range	= lpST->tile_Range;
		(*Header)->subType		= lpST->subType;
		(*Header)->probability	= lpST->probability;
		(*Header)->type_Num		= lpST->type_Num;

		(*Header)->prev			= NULL; 
		(*Header)->next			= NULL; 
	}	
	else						//이미 생성된 리스트에 첨가 연결
	{	
		t = NULL;
		MemAlloc( t, sizeof( MAPSKILLTABLE ));

		t->type			= lpST->type;
		t->x			= lpST->x; 
		t->y			= lpST->y; 
		t->skillno		= lpST->skillno;
		t->tile_Range	= lpST->tile_Range;
	 	t->subType		= lpST->subType;
		t->probability	= lpST->probability;
		t->type_Num		= lpST->type_Num;
		
		temp = *Header ;
		temp->prev = t;
		t->next = temp;
		t->prev = NULL;
		*Header = t;
	}	
}

void DeleteSkillLink( lpMAPSKILLTABLE lpHeader )
{	
	if( !lpHeader ) return;
	if( lpHeader->next ) 
	{
		DeleteSkillLink( lpHeader->next );	
	}
	MemFree( lpHeader );
}

void DeleteSkillAll()
{
	for( int i=0; i<8; i++ )
	{
		for( int j=0; j<8; j++ )
		{
			DeleteSkillLink( g_Header[i][j] );
		}
	}
}

void DeleteSkill( lpMAPSKILLTABLE *Header, lpMAPSKILLTABLE f)
{
	lpMAPSKILLTABLE  t = *Header;//g_lpMapSkillTable;

	while( t != NULL )
	{
		if( t == f )
		{
			if( f == *Header )		//header가 제거되는 경우
			{
				t = (*Header)->next;
		
				if(*Header!=NULL)
					MemFree( *Header );
		
				if(t!=NULL)
				{
					*Header = t;
					(*Header)->prev = NULL;
				}
				return;
			}
			else 
			{
				if ( t->prev != NULL )
				{
					t->prev->next = t->next;
				}
				if( t->next != NULL )
				{
					t->next->prev = t->prev;
				}
				MemFree( t );
				return;
			}
		}
		t= t->next;
	}	
}
		
//	맵은 절대좌표...
lpMAPSKILLTABLE FindSkill( lpMAPSKILLTABLE *H, int x, int y )
{	   	
	lpMAPSKILLTABLE t;
	t = *H;
	while( t != NULL )
	{	
		if( t->x == x && t->y == y )
		{
			return t;
		}
		t = t->next;
	}			
	return NULL;
}			
	

//1017 zhh	//상당 부분 수정 되었습니다
//--------------------------------------------------------------------------------------------
//	기  능 : 1시간(실제 시간. 라자에서는 6h)마다 자연에서 얻어낼수 있는 량이  초기화된다. 
//	인  자 : 
//	결  과 : 
//--------------------------------------------------------------------------------------------
/*------------------------------------------------------------------------------------------------------------
  기 능 : SkillMapTable에서 그지역에서 발생할수 있는 적당한 아이템번호를 리턴해준다. 
  인 자 : type  :   TOOL_FARMING			1
				 	 TOOL_MINING			2
					 TOOL_FISHING			3
					 TOOL_CHOPPING			4
					 TOOL_HUB				5
					 TOOL_NPC_GENER			6
					 TOOL_DONTSKILL			7
	    tableno :  0 - 10 
	    hitrate :  0 - 100
  결 과 : 아이템번호를 리턴한다. 
------------------------------------------------------------------------------------------------------*/
int ReturnFromSkillMapTable(const int cn, const int iSkillToolType, const int iTableno, const int iHitrate ,int iReturnType)
{				
	int i = 0;
	int iReturnValue = 0;
	t_SkillMapTable *smt;
	int maxdemand = -9999;
	t_SkillMapTable smtsad = SkillMapTable[0];
	
	if( 10 < iTableno || 0 > iTableno) {return 0;}
	CHARLIST *ch = GetCharListPtr(cn);
	if(!ch){ return 0;}

	for( i = 0 ; i < SkillMapTableCount ; i ++)
	{	
		smt = &SkillMapTable[ i];

		if( smt->Skill_type == iSkillToolType && smt->table[iTableno] )
		{
			if( smt->demand <= iHitrate )
			{
				if( maxdemand < smt->demand )
				{
					switch(iReturnType)
					{
					case RETURN_SMT_DEMAND:
						iReturnValue  = smt->demand;
						break;
					case RETURN_SMT_ITEMNO:
						iReturnValue = smt->Item_id;//020507 lsw
						smtsad = *smt;//020401 lsw
						break;
					}
				}
			}
		}
	}	
	if(iReturnType	== RETURN_SMT_ITEMNO)
	{
		if(smtsad.new_item[0] && smtsad.new_item_pro[0] )
		{
			if(iReturnValue)
			{
				const int r = rand()%10000;

				if( smtsad.new_item_pro[0] > r )
				{
					if( -1 == SkillMgr.FindLearnItem(ch,iReturnValue))
					{
//						GiveSadonixToGround(cn, smtsad.new_item[0],iSkillToolType,MapInfo[MapNumber].port,ch->X, ch->Y);//020110 lsw
					}
				}
			}
		}

		if(smtsad.new_item[1] && smtsad.new_item_pro[1] )
		{
			if(iReturnValue)
			{
				const int r = rand()%10000;

				if( smtsad.new_item_pro[1] > r )
				{
					if( -1 == SkillMgr.FindLearnItem(ch,iReturnValue))
					{
//						GiveSadonixToGround(cn, smtsad.new_item[1],iSkillToolType,MapInfo[MapNumber].port,ch->X, ch->Y);//020110 lsw
					}
				}
			}
		}
	}

	return iReturnValue;
}		

//-------------------------------------------------------------------------------------------------------		
//	기 능 : 기술을 읽어들이면서  NPC 생성위치를 잡는다. 	
//	인 자 :		
//	결 과 :		
//-------------------------------------------------------------------------------------------------------
BOOL LoadSkillMapTable(void)
{				
	int		i=0;
	char	temp[FILENAME_MAX];
	FILE *fp ;	
	MAPSKILLTABLE st;
	int t;
				
	t_earthpower *ep;
				
	sprintf_s( temp, sizeof(temp), "%s/map/%s.skb", GameServerDataPath, MapName );
	fp = fopen( temp, "rb" );
	if(fp == NULL) 		return FALSE;
				
	int dx = (g_Map.file.wWidth+ 7)/8;
	int dy = (g_Map.file.wHeight+7)/8;
				
	ep = &EarthPower;
				
	ep->reclaimcount = 0;
	ep->miningcount  = 0;
	ep->fishingcount = 0;
	ep->cuttingcount = 0;
	ep->diggingcount = 0;

	//<! BBD 040311	
	float Rate = GetResourceRate(g_UsercountInServerSet);	// 자원배율을 구해온다
	MyLog(0, "Starting ResourceRate = %.1f, cause UsercountInServerSet = %d", Rate, g_UsercountInServerSet);
	//> BBD 040311	
	
	while( !feof( fp ) )
	{			
    memset(&st,0,sizeof( MAPSKILLTABLE));				// LTS DRAGON MODIFY
		fread( &st, sizeof( MAPSKILLTABLE), 1, fp );
				
		int x = (int)(st.x / dx);
		int y = (int)(st.y / dy);
		TileMap[ st.x ][ st.y ].attr_skill = 1;
							
		// 경작..		
		// EarthPower[]	는 0 - 10000의 수치를 가진다. 최초 약 0-2000의 수치를 가지고 시작한다. 
		t = st.probability * 10;
		//1018 zhh
		if( t > 1000 ) t = 1000;
		if( t < 0    ) t = 0;

		t = (int)(t * Rate);		// BBD 040311	

		switch( st.skillno )
		{				
			// 경작..	
		case 1 :	if( ep->reclaimcount < MAX_EARTHSKILL )	
					{												
						ep->reclaim[ ep->reclaimcount] = 700 + (rand()%1300);
						st.type_Num = ep->reclaimcount ++;	
					}	
			break;		
			// 채광.	
		case 2 :	if( ep->miningcount < MAX_EARTHSKILL )	
					{												
						ep->mining_old[ ep->miningcount] = ep->mining[ ep->miningcount] = t;
						st.type_Num = ep->miningcount ++;	
					}	
			break;		
			// 낚시.	
		case 3 :	if( ep->fishingcount < MAX_EARTHSKILL )	
					{												
						ep->fishing_old[ ep->fishingcount] = ep->fishing[ ep->fishingcount] = t;
						st.type_Num = ep->fishingcount ++;	
					}			
			break;		
			// 벌목.	
		case 4 :	if( ep->cuttingcount < MAX_EARTHSKILL )	
					{												
						ep->cutting_old[ ep->cuttingcount] = ep->cutting[ ep->cuttingcount] = t;
						st.type_Num = ep->cuttingcount ++;	
					}				
			break;		
			// 약초.	
		case 5 :	if( ep->diggingcount < MAX_EARTHSKILL )	
					{												
						ep->digging_old[ ep->diggingcount] = ep->digging[ ep->diggingcount] = t;
						st.type_Num = ep->diggingcount ++;	
					}			
			break;		
			// NPC생성. 
		case 6 : 	//	tile_Range	-> 최대 발생수.
					//	probability	-> 스크립트번호( 0 - 100 ).
					//	type_Num	-> NPC 고유번호.
					NPCGeneratePosition( st.type_Num, st.x, st.y,  st.probability, st.tile_Range );
			break;
		}
		
		if (st.skillno>0) AddSkill( &g_Header[ x][y], &st );	// LTS DRAGON MODIFY
	}	
	fclose(fp);	
		
	return TRUE;	
}		
	
DWORD ReturnHowLongTimeGrow( LPITEMLIST i )
{	
	CItem *ii = ItemUnit( i->item_no/1000, i->item_no % 1000 );

	if( ii )	
	{
//		return ii->Getmouse_MR_Effect();
		return ii->Getmouse_Mr_Wave();
	}

	return 0;
}			
			

	
void RecvCMD_FOOT_ON_TRAP_0_ITEM(const int cn, t_client_foot_on_trap0 *p )
{
  if( !CheckServerId( cn ) ) return;
	int id = p->idServer;
	if( id < 0 ) return ;
	if( id >= MAX_ITEM_LIST ) return;
	
	if( connections[cn].chrlst.bAlive == ALIVE_ )
	if( ItemList[ id].bAlive == ALIVE_ )
	{
		if( ItemList[ id].attr[ IATTR_ATTR] & IA2_TRAPED0 )
		{
//			SetArea( CHANGE_FOOT_ON_TRAP0_AREA, id );
			SendItemFoot_On_Trap0( connections, cn, id ); // ####			
			RemoveItemList( id );
		}
	}
}	
	
void RecvCMD_FOOT_ON_TRAP_1_ITEM(const int cn, t_client_foot_on_trap1 *p )
{
  if( !CheckServerId( cn ) ) return;
	int id = p->idServer;
	if( id < 0 ) return ;
	if( id >= MAX_ITEM_LIST ) return;

	if( connections[cn].chrlst.bAlive == ALIVE_ )
	if( ItemList[ id].bAlive == ALIVE_ )
	{
		if( ItemList[ id].attr[ IATTR_ATTR] & IA2_TRAPED1 )
		{
//			SetArea( CHANGE_FOOT_ON_TRAP1_AREA, id );
			SendItemFoot_On_Trap1( connections, cn, id ); 
			RemoveItemList( id );
		}
	}
}

void RecvSkillGauge(const int cn, t_client_skill_gauge *p )
{					
	t_packet packet;	
	int probablity;	
	lpMAPSKILLTABLE	 ms;
	t_earthpower *ep;
					
	packet.h.header.type = CMD_SKILL_GAUGE;
	packet.h.header.size = sizeof( t_server_skill_gauge );
					
	packet.u.server_skill_gauge.skillno = p->skillno;
	packet.u.server_skill_gauge.x		= p->x;
	packet.u.server_skill_gauge.y		= p->y;
					
	ep = &EarthPower;

	switch( p->skillno )
	{				
		// 개간...	
	case 21 :	ms = skill_ReturnProbablityFromSkillTable( &probablity,  1, p->x, p->y );
				if( ms )
				{	
					packet.u.server_skill_gauge.min = ep->reclaim[ ms->type_Num ]/100;
					QueuePacket( connections, cn, &packet, 1 );
					return;
				}	
		break;		
	}				
}					

////////////////////////////////////////////////////////////////////////////////////////
//                                    경  제
////////////////////////////////////////////////////////////////////////////////////////
// Item가격 조정 
const char itemtypename[ 11][20] = {	"Plant",		
								"Mineral",		
								"Herb",		
								"Cook",		
								"Potion",		
								"Tool",		
								"Weapon",		
								"Disposable",	
								"Armor",		
								"Accessory",	
								"Etc"};

void RecvDebugChangeSkillExp( t_debug_change_skill_exp *tp )
{
	LPCHARLIST ch;
	int i;

	if( connections[tp->id].dwAgentConnectionIndex && connections[tp->id].state >= CONNECT_JOIN)
	{
		ch = &connections[tp->id].chrlst;
	}
	else return;

	if (!ch->IsCounselor()) return;

	for( i = 0 ; i < 45 ; i ++)
	{
		ch->skillexp[i].skillexp = tp->skillexp[i];
	}
}

int SkillLevelExpLoad()
{
	HSTMT hStmt= NULL ;
	RETCODE ret ;
	SWORD nCols ;
	int c=0;
	
	SQLAllocStmt(hDBC, &hStmt);
	ret= SQLExecDirect(hStmt, (UCHAR *)"select * from skill_lvexp order by probability", SQL_NTS) ;
	if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) 
	{
		MyLog(LOG_NORMAL, "skill_lvexpl: ExecDirect Error ") ;
		return -1 ;
	}

	SQLNumResultCols(hStmt, &nCols) ;
	
	ret= SQLFetch(hStmt) ;
	if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) 
	{
		MyLog(LOG_NORMAL, "skill_lvexpl: Fetch Error ") ;
		return -1 ;
	}
	
	while( ret == SQL_SUCCESS)
	{
		// Accquire the DATA		
		SQLINTEGER  sCustID, cbCustID , cbexp , exp;
		ret= SQLGetData(hStmt, 1, SQL_C_SLONG, &sCustID, 0, &cbCustID);	
		if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) 
		{
			MyLog(LOG_NORMAL, "skill_lvexpl: Error at 1 %d ", ret) ;
			return -1 ;
		}

		if( sCustID < 0 || sCustID > SKILL_LEVEL_EXP_TABLE_MAX_ ) 
		{
			MyLog(LOG_NORMAL, "skill_lvexpl:  Data is Invalid...( sCusID = %d )", sCustID ) ;
			return -1;
		}

		for(int i=2;i<103;i++)
		{
			ret= SQLGetData(hStmt, i, SQL_C_SLONG, &exp, 0, &cbexp) ;
			if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) 
			{
				MyLog(LOG_NORMAL, "skill_lvexpl:  Error at %d ", c ) ;
				return -1 ;
			}
			SkillLevelExp[sCustID][i-2]=exp;
		}
		c++;
		ret= SQLFetch(hStmt) ;
	}
	SQLFreeStmt(hStmt, SQL_DROP) ;

	return c ;
}

int SkillExpLoad()
{
	HSTMT hStmt= NULL ;
	RETCODE ret ;
	SWORD nCols ;
	char query_stmt[80]= {0, } ;
	int c=0;
	
	SQLAllocStmt(hDBC, &hStmt);
	
	strcpy_s(query_stmt, sizeof(query_stmt), "select * from skill_exp order by probability") ;
	ret= SQLExecDirect(hStmt, (UCHAR *)query_stmt, SQL_NTS) ;
	if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) 
	{
		MyLog(LOG_NORMAL,"skill_exp: ExecDirect Error ") ;
		return -1 ;
	}

	SQLNumResultCols(hStmt, &nCols) ;
	
	ret= SQLFetch(hStmt) ;
	if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) 
	{
		MyLog(LOG_NORMAL, "skill_exp: Fetch Error ") ;
		return -1 ;
	}
	
	while( ret == SQL_SUCCESS)
	{
		SQLINTEGER  sCustID, cbCustID , cbexp , exp;
		ret= SQLGetData(hStmt, 1, SQL_C_SLONG, &sCustID, 0, &cbCustID);	
		if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) {
			MyLog(LOG_NORMAL, "skill_exp: Error at 1" ) ;
			return -1 ;
		}

		if( sCustID < 0 || sCustID > SKILL_EXP_TABLE_MAX_ ) 
		{
			MyLog(LOG_NORMAL, "skill_exp: Data is Invalid...( sCusID = %d )", sCustID ) ;
			return -1;
		}

		for(int i=2;i<103;i++)
		{
			ret= SQLGetData(hStmt, i, SQL_C_SLONG, &exp, 0, &cbexp) ;
			if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) 
			{
				MyLog(LOG_NORMAL, "skill_exp: Error at %d", c ) ;
				return -1 ;
			}
			SkillExp[sCustID][i-2]=exp;
		}
		c++;
		ret= SQLFetch(hStmt) ;
	}
	SQLFreeStmt(hStmt, SQL_DROP) ;
	return c ;
}

//63(BETWEEN_POS) = 60(NPC_ITEM_TABLE_TOTAL_ITEM) + 2(NPC_ITEM_TABLE_START_ITEM_POS) + 1(다음이니까)
int Load_NPC_ItemGenerateTable()									//DB에서 테이블을 읽어 온다
{
	HSTMT hStmt= NULL ;
	RETCODE ret ;
	SWORD nCols ;
	int c=0;
	
	SQLAllocStmt(hDBC, &hStmt);
	ret= SQLExecDirect(hStmt, (UCHAR *)"select * from mon_bag order by bagno", SQL_NTS) ;
	if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) 
	{
		MyLog(LOG_NORMAL, "mon_bag : ExecDirect Error ") ;
		return -1 ;
	}

	SQLNumResultCols(hStmt, &nCols) ;
	
	ret= SQLFetch(hStmt) ;
	if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) 
	{
		MyLog(LOG_NORMAL, "mon_bag: Fetch Error ") ;
		return -1 ;
	}
	
	for(int x=0;x<NPC_ITEM_TABLE_X;x++)
	{
		SQLINTEGER  cbitemno , itemno;
		for(int i=1;i<NPC_ITEM_TABLE_Y+1;i++)
		{
			ret= SQLGetData(hStmt, i, SQL_C_SLONG, &itemno, 0, &cbitemno) ;
			if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) 
			{
				MyLog(LOG_NORMAL, "mon_bag: Error at %d", i ) ;
				return -1 ;
			}
			Npc_Item_Table[x][i-1]=itemno;
		}
		c++;
		ret= SQLFetch(hStmt) ;
	}
	SQLFreeStmt(hStmt, SQL_DROP) ;
	return c ;
}

int LoadSkillLevelTable(void)
{
	if(SkillLevelExpLoad()<0)			return -1;
	if(SkillExpLoad()<0)				return -2;
	if(Load_NPC_ItemGenerateTable()<0)	return -3;
	return 1;
}

/*------------------------------------------------------------------------------------------------------------
  기 능 : SkillMapTable에서 그지역에서 발생할수 있는 적당한 demand를 리턴해준다. 
  인 자 : type  :   TOOL_FARMING			1
			 		TOOL_MINING				2
					TOOL_FISHING			3
					TOOL_CHOPPING			4
					TOOL_HUB				5
					TOOL_NPC_GENER			6
					TOOL_DONTSKILL			7
	    tableno :  0 - 10 
	    hitrate :  0 - 100
  결 과 : demand를 리턴한다. 
------------------------------------------------------------------------------------------------------*/
int GiveBasicItem(const int cn,const int iSkillTooltype,const int subType)//020214 스킬 시전시 실패해도 툴에 따른 기본 아이템 제공 1/15
{
	CHARLIST *ch = GetCharListPtr(cn);
	if(!ch){return 0;}
	if(!(rand()%15))
	{
		const int sx = ch->X;
		const int sy = ch->Y;
		const int itemno =ReturnFromSkillMapTable(cn,iSkillTooltype,subType,14,RETURN_SMT_ITEMNO);//020214 lsw
		ItemAttr item = GenerateItem(itemno);
		DropItem( sx-50+rand()%100, sy-50+rand()%100, &item);
		return 1;
	}
	return 0;
}

void DecreaseEquipDuration( CHARLIST *ch, const DWORD dwDecValue, const int iItemPos, const int iTargetSprNo, const bool bIsAttacker,bool bIsNoCount)
{	
	POS p;
	ItemAttr *item		=	&ch->equip[iItemPos];
	CItem *t			=	ItemUnit( item->item_no );	
	int iSendValue		= 0;
	const int iValue	= abs((long)dwDecValue);
	const int cn		= ch->GetServerID();
	
	if( NULL == t){return;}
	if( SPRITETYPE_NPC 	==  ch->SprType){return;}
	if( 96 	==  iTargetSprNo){return;}

	if( DIVIDE_ITEM == t->GetRbutton() )//내구도용 아이템
	{	// DIVIDE_ITEM일경우 attack_pow만큼 뺀다. 
		if(bIsAttacker)
		{
			iSendValue = ItemMgr.DecItemDur(*item,iValue);
		}
		else
		{
			iSendValue = ItemMgr.DecItemDur(*item,1);
		}
	}
	else	//내구도가 아닌 아이템
	{	
		int t = 0;
		if(!bIsNoCount)
		{
			ch->dec_duration++ ;
			if( ch->dec_duration > 30 ) ch->dec_duration = 0;
			else return;
			t = (int) log((float)abs(iValue)+1);
			if( t > 14 ) t = 14;
			if( t < 1 ) t = 1;
		}
		else//카운트 및 log 없이 바로 원하는 만큼 감소
		{	
			t = iValue;
		}
		iSendValue = ItemMgr.DecItemDur(*item,t);
	}
	
	switch(iSendValue)
	{
	case 1:
		{	
			::SetItemPos( EQUIP, iItemPos, &p );
			::BreakItemByDuration( item, ch, p );
		}
		break;
	case 2:
		{
			::SendItemAttrDurationChange( connections, cn, iItemPos );
		}break;
	default:
		break;
	}
}				

int OutMessage(const int cn, const DWORD para1, const DWORD para2)
{
	return OutMessage( ::CheckServerId(cn), para1, para2);
}

int OutMessage( LPCHARLIST ch, const DWORD para1, const DWORD para2)
{
	if(!ch){return 0;}

	if( !ch->IsPlayer() ) {return 0;}
	
	t_packet p;	
	p.h.header.type = CMD_OUTPUT_MESSAGE;
	{
		p.u.server_output_message.para1 = para1;
		p.u.server_output_message.para2 = para2;
	}
	p.h.header.size = sizeof( t_server_output_message );
	QueuePacket( connections, ch->GetServerID(), &p, 1);
	return 1;
}

int OutSound(LPCHARLIST ch, const int iFileType, const int iNo)
{
	if(!ch){return 0;}
	if(!ch->IsPlayer() ){return 0;}
	
	t_packet p;	
	p.h.header.type = CMD_SOUND_OUT;
	{
		p.u.server_output_message.para1 = iFileType;
		p.u.server_output_message.para2 = iNo;
	}
	p.h.header.size = sizeof( t_server_output_message );
	::QueuePacket( connections, ch->GetServerID(), &p, 1);
	return 1;
}

void SkillMapper( const int cn, const int iSkillNo, const int x, const int y, const int targettype, const int target_id, const POS s )
{	//< CSD-030821
	CHARLIST *ch = CheckServerId(cn);
	
	if (ch == NULL)
	{
		return;
	}

	if (target_id < 0)
	{
		return;
	}

	switch (iSkillNo)
	{
	case SKILL_DETECT_GHOST:
		{
			skill_Detect_Ghost(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_POISONING:
		{
			skill_Poisoning(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_DETECT_POISON:
		{
			skill_Detect_Poison(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_FIND_TRAPS:
		{
			skill_Find_Traps(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_REMOVE_TRAPS:
		{
			skill_Remove_Traps(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_ENTRAPMENT:
		{
			skill_Entrapment(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}

	case SKILL_DETECT_BOMB:
		{
			skill_Detect_Bomb(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_BOMB_IN_BOX:
		{
			skill_Bomb_in(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_REMOVE_BOMB:
		{
			skill_Remove_Bomb(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_STEALING:
		{
			skill_Stealing(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_RECLAIM:
		{
			skill_Reclaim(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_SOW:
		{
			skill_Sow(cn, iSkillNo, x, y, targettype, target_id, s);
			break;
		}
	case SKILL_HAVEST:
		{
			skill_Havest(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_MINING:
		{
			skill_Mining(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_REFINE:
		{
			skill_Refine(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_FISH_HOOKING					:
		{
			skill_Fish(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_NETTING:
		{
			skill_Netting(cn, iSkillNo, x, y, targettype, target_id);			
			break;
		}
	case SKILL_CUTTING:
		{
			skill_Cutting(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_LUMBERING:
		{
			skill_Lumbering(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_BUTCHERING:
		{
			skill_Butchering(cn, iSkillNo, x, y, targettype, target_id);			
			break;
		}
	case SKILL_DIGGING:
		{
			skill_Digging(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_MAKEPOWDER:
		{
			skill_Makepowder(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_SHEAR:
		{
			skill_Shear(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_CARPENTRY:
		{
			skill_Manufacturing(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_BLACKSMITHY:
		{
			skill_Manufacturing(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_COOKING:
		{
			skill_Cooking(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_CLOTHING:
		{
			skill_Clothing(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_TAILORING:
		{
			skill_Tailoring(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_BOWCRAFTING:
		{
			skill_Bowcrafting(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_FLETCHING:
		{
			skill_Fletching(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_MAKEPOTION:
		{
			skill_MakePotion(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_MAKEMETAL:
		{
			skill_MakeMetal(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_MAKEBOTTLE:
		{
			skill_MakeBottle(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_CANDLEMAKING:
		{
			skill_Candlemaking(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_SNOOPING:
		{
			skill_Snooping(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_MAKEFIRE:
		{
			skill_Makefire(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_TAMING:
		{
			skill_Taming(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_EVALUATE_INTELIGENCE:
		{
			skill_Evaluate_Inteligence(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_EVALUATE_WISDOM:
		{
			skill_Evaluate_Wisdom(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_ANATOMY_JUDGEMENT:
		{
			skill_Anatomy_Judgement(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_ITEM_IDENTIFICATION:
		{
			skill_Item_Identification(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case TACTICS_Crapple:
		{
			skill_Crapple(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case TACTICS_swordmanship:
		{
			skill_Swordmanship(cn, iSkillNo, x, y, targettype, target_id);			
			break;
		}
	case TACTICS_Archery:
		{
			skill_Archery(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case TACTICS_Fencing:
		{
			skill_Fencing(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case TACTICS_Mace_fighting:
		{
			skill_Mace_Fighting(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case TACTICS_Pierce:
		{
			skill_Pierce(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case TACTICS_Whirl:
		{
			skill_Whirl(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case TACTICS_Hurl:
		{
			skill_Hurl(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case TACTICS_Parrying:
		{
			skill_Parrying(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case TACTICS_Double_Swordmanship:
		{
			skill_Double_Swordmanship(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case TACTICS_Double_MaceFighting:
		{
			skill_Double_Macefighting(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case TACTICS_Magery:
		{
			skill_Magery(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case TACTICS_Orison:
		{
			skill_Orison(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_GHOST_RESURRECTION:
		{
			skill_GhostResurrection(cn, iSkillNo, x, y, targettype, target_id);
			break;
		}
	case SKILL_GM_CARPENTER:
	case SKILL_GM_BLACKSMITH:
	case SKILL_GM_COOK:
	case SKILL_GM_TAILOR:
	case SKILL_GM_BOWCRAFT:
	case SKILL_GM_ALCHEMIST:
	case SKILL_GM_CANDLEMAKER:
		{
			break;
		}
	}
}	//> CSD-030821

//<! BBD 040311	프록시로 동접수 요청하는 함수
void RequestUsercountToProxy()
{
	char Msg = 0;
	Msg = BYTE(PTCL_SERVERSET_USERNUM_REQUEST);

	g_pServerTable->SendToProxyServer(&Msg, sizeof(Msg));

}
//> BBD 040311	프록시로 동접수 요청하는 함수

//<! BBD 040311	동접수로 적용될 자원배율을 얻어오는 함수
float GetResourceRate(int nCount)
{
	int nIndex = 0;
	if( nCount <= 1000)		// 1000 이하
	{
		nIndex =  0;
	}
	else if( nCount > 1000 && nCount <= 1500)	// 1000 ~ 1500
	{
		nIndex = 1;
	}
	else if( nCount > 1500 && nCount <= 2000)	// 1500 ~ 2000
	{
		nIndex = 2;
	}
	else if( nCount > 2000 && nCount <= 2500)	// 2000 ~ 2500
	{
		nIndex = 3;
	}
	else if( nCount > 2500 && nCount < 3000)	// 2500 ~ 3000
	{
		nIndex = 4;
	}
	else		// 3000 이상일때
	{
		nIndex = 5;
	}

	//<! BBD 040322	에이서에 관하여 따로처리
	if(LocalMgr.IsAbleNation( CHINA | TAIWAN | HONGKONG ))
	{
		return resource_rate2[nIndex];
	}
	else
	{
		return resource_rate[nIndex];
	}
	//> BBD 040322	에이서에 관하여 따로처리
}
//> BBD 040311	동접수로 적용될 자원배율을 얻어오는 함수
