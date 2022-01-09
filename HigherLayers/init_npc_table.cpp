#include "..\stdafx.h"
#include "DefaultHeader.h"
#include "..\LowerLayers\MyLog.h"

NPCName_by_Gender	NPC_Name_Ref[Num_Of_NPC_Name];
NPCLev_to_Exp		NPC_Lev_Ref[Num_Of_NPC_Lev];
NPC_Generation		NPC_Gen_Ref[Num_Of_NPC_Generation];
BYTE				EnemyEnemy[100][100];

int NPC_FemaleName_Count;
int NPC_MaleName_Count;
int NPC_LastName_Count;

short int nNPC_Type, nNPC_Name, nNPC_Lev ;
int initNPCNameTable(void) ;
int initNPCLevTable(void) ;
int initNPCGenerationTable(void) ;
void MakeEnemyEnemyTable( void );

extern HDBC hDBC ;

// DB에 있는 3개의 NPC 테이블을 메모리로 읽어온다.
int initNPCTable(void)
{
	if(initNPCNameTable() < 0) return -1 ;
	if(initNPCLevTable() < 0) return -1 ;
	if(initNPCGenerationTable() < 0) return -1 ;
	
	MakeEnemyEnemyTable();
	
	return 1 ;
}

void MakeEnemyEnemyTable( void )
{
	int i,j;
	int d;
	
	for( i = 0 ; i < 98 ; i ++)
	{
		for( j = 0 ; j < 10 ; j ++)
		{
			d = NPC_Gen_Ref[i].offset[j];
			if( d ) 
			{
				EnemyEnemy[i+1][d] = 1;
			}
		}
		for( j = 10 ; j < 20 ; j ++)
		{
			d = NPC_Gen_Ref[i].offset[j];
			if( d ) 
			{
				EnemyEnemy[i+1][d] = 2;
			}
		}
	}
}

int GetEnemyEnemy( CHARLIST *attacker, CHARLIST *defender )
{
	if( defender == NULL ) return 100;
	
	int d = EnemyEnemy[attacker->SprNo][ defender->SprNo];
	
	switch( d )
	{
	case 0 : return 100;
	case 1 : return 75;
	}
	
	return 125;
}	

// NPC이름 생성을 위한 테이블을 DB에서 읽어오는 함수
int initNPCNameTable(void)
{
	if(Num_Of_NPC_Name <= 0) {
		//fprintf(stdout, "\n	No item's available ") ;
		return -4 ; // No item's available
	}
	
	HSTMT hStmt= NULL ;
	RETCODE ret ;
	char query_stmt[80]= {0, } ;
	SDWORD cbValue ;
	int c= 0 ;
	SQLAllocStmt(hDBC, &hStmt);
	
	strcpy_s(query_stmt, sizeof(query_stmt), "select * from NPC_NamebyGender order by No") ;
	ret= SQLExecDirect(hStmt, (UCHAR *)query_stmt, SQL_NTS) ;
	
	if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) {
		extern void displaySQLError(SQLHSTMT hstmt);
		displaySQLError( hStmt );
		MyLog( LOG_FATAL, "NPC_NamebyGender : ExecDirect Error" );
		return -1 ;
	}	
	
	//SQLNumResultCols(hStmt, &nCols) ;
	
	ret= SQLFetch(hStmt) ;
	ret= SQLFetch(hStmt) ;
	if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) 
	{
		MyLog( LOG_FATAL, "NPC_NamebyGender : Fetch ERROR" );
		return -1 ;
	}	
	
	NPC_FemaleName_Count = 0;
	NPC_MaleName_Count   = 0;
	NPC_LastName_Count   = 0;
	
	while(c < Num_Of_NPC_Name && ret == SQL_SUCCESS)
	{	
		ret= SQLGetData(hStmt, 2, SQL_C_CHAR, (UCHAR *)NPC_Name_Ref[NPC_FemaleName_Count].Female, 17, &cbValue) ;
		if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) {
			MyLog( LOG_FATAL, "NPC_NamebyGender :String GET Error!!! (%d)", ret);
			return -1 ;
		}
		if( NPC_Name_Ref[NPC_FemaleName_Count].Female[0] )	
		{
			NPC_FemaleName_Count++;
		}
		ret= SQLGetData(hStmt, 3, SQL_C_CHAR, (UCHAR *)NPC_Name_Ref[NPC_MaleName_Count].Male, 17, &cbValue) ;
		if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) {
			MyLog( LOG_FATAL, "NPC_NamebyGender :String GET Error!!! (%d)", ret);
			return -1 ;
		}
		if( NPC_Name_Ref[NPC_MaleName_Count].Male[0] )	
		{
			NPC_MaleName_Count++;
		}
		ret= SQLGetData(hStmt, 4, SQL_C_CHAR, (UCHAR *)NPC_Name_Ref[NPC_LastName_Count].Last, 17, &cbValue) ;
		if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) {
			MyLog( LOG_FATAL, "NPC_NamebyGender :String GET Error!!! (%d)", ret);
			return -1 ;
		}
		if( NPC_Name_Ref[NPC_LastName_Count].Last[0] )	
		{
			NPC_LastName_Count++;
		}
		
		c++ ;
		ret= SQLFetch(hStmt) ;
	}	
	
	nNPC_Name = c ;
	SQLFreeStmt(hStmt, SQL_DROP) ;
	MyLog( LOG_NORMAL, "	.NPC_NamebyGender		%4d data Loaded", c) ;
	
	FILE * fp;
	int i;
	// 현재 남/여 구분이 없는상태이다.
	fp = fopen( "./OutPut/Mon_Name.bin", "wb" );
	if( fp )
	{
		for( i = 0 ; i < NPC_FemaleName_Count ; i ++)
		{
			char temp[17];
			strcpy(temp, NPC_Name_Ref[i].Male);
			char *p = strchr(temp, ' ' );
			if( p ) *p = '-';
			fwrite( temp, 17, 1, fp );
		}
		
		fclose( fp );
	}
	fp = fopen( "./OutPut/Mon_Name.txt", "wt" );
	if( fp )
	{
		for( i = 0 ; i < NPC_FemaleName_Count ; i ++)
		{
			fprintf( fp, "%s\n", NPC_Name_Ref[i].Male );
		}
		fclose( fp );
	}
	
	return c ;
}		

// 0811 NPC KHS
// NPC레벨과 경험치 관련 정보를 담고 있는 테이블을 DB에서 읽어오는 함수
int initNPCLevTable(void)
{
	if(Num_Of_NPC_Lev <= 0) 
	{
		//fprintf(stdout, "\n	No item's available ") ;
		return -4 ; // No item's available
	}
	
	HSTMT hStmt= NULL ;
	RETCODE ret ;
	char query_stmt[80]= {0, } ;
	SDWORD cbValue ;
	int c= 1 ;
	SQLAllocStmt(hDBC, &hStmt);
	
	strcpy_s(query_stmt, sizeof(query_stmt), "select * from NPC_Lv2Exp order by Lv") ;
	ret= SQLExecDirect(hStmt, (UCHAR *)query_stmt, SQL_NTS) ;
	
	if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) {
		MyLog( LOG_FATAL, "NPC_Lv2Exp: ExecDirect Error " );
		return -1 ;
	}
	
	//SQLNumResultCols(hStmt, &nCols) ;
	
	ret= SQLFetch(hStmt) ;
	if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) 
	{
		MyLog( LOG_FATAL, "NPC_Lv2Exp:: Fetch Error" ) ;
		return -1 ;
	}
	
	while(c < Num_Of_NPC_Lev && ret == SQL_SUCCESS)
	{	
		ret = SQLGetData(hStmt,  2, SQL_C_SLONG, &NPC_Lev_Ref[c].nMinExp, 0, &cbValue);
		ret = SQLGetData(hStmt,  3, SQL_C_SLONG, &NPC_Lev_Ref[c].nMaxExp, 0, &cbValue);
		ret = SQLGetData(hStmt,  4, SQL_C_SLONG, &NPC_Lev_Ref[c].nNeedExp, 0, &cbValue);
		ret = SQLGetData(hStmt,  5, SQL_C_SLONG, &NPC_Lev_Ref[c].nTrainingExp, 0, &cbValue);
		ret = SQLGetData(hStmt,  6, SQL_C_FLOAT, &NPC_Lev_Ref[c].fExpReduction, 0, &cbValue); // CSD-030430
		ret = SQLGetData(hStmt,  7, SQL_C_FLOAT, &NPC_Lev_Ref[c].fNKReduction, 0, &cbValue);  // CSD-030430
		ret = SQLGetData(hStmt,  8, SQL_C_SLONG, &NPC_Lev_Ref[c].nTacRate, 0, &cbValue);
		ret = SQLGetData(hStmt,  9, SQL_C_SLONG, &NPC_Lev_Ref[c].nMaxTactic, 0, &cbValue);
		ret = SQLGetData(hStmt, 10, SQL_C_SLONG, &NPC_Lev_Ref[c].nDyeingPrice, 0, &cbValue);
		ret = SQLGetData(hStmt, 11, SQL_C_SLONG, &NPC_Lev_Ref[c].nLessRate, 0, &cbValue);
		ret = SQLGetData(hStmt, 12, SQL_C_SLONG, &NPC_Lev_Ref[c].nGreatRate, 0, &cbValue);
		ret = SQLGetData(hStmt, 13, SQL_C_SLONG, &NPC_Lev_Ref[c].nCspMax, 0, &cbValue);
		ret = SQLGetData(hStmt, 14, SQL_C_SLONG, &NPC_Lev_Ref[c].nBaseCP, 0, &cbValue);
		ret = SQLGetData(hStmt, 15, SQL_C_SLONG, &NPC_Lev_Ref[c].bank_loan, 0, &cbValue);
		ret = SQLGetData(hStmt, 16, SQL_C_SLONG, &NPC_Lev_Ref[c].nStep, 0, &cbValue);
		ret = SQLGetData(hStmt, 17, SQL_C_SLONG, &NPC_Lev_Ref[c].nWarriorLife, 0, &cbValue);
		ret = SQLGetData(hStmt, 18, SQL_C_SLONG, &NPC_Lev_Ref[c].nThiefLife, 0, &cbValue);
		ret = SQLGetData(hStmt, 19, SQL_C_SLONG, &NPC_Lev_Ref[c].nArcherLife, 0, &cbValue); 
		ret = SQLGetData(hStmt, 20, SQL_C_SLONG, &NPC_Lev_Ref[c].nWizardLife, 0, &cbValue); 
		ret = SQLGetData(hStmt, 21, SQL_C_SLONG, &NPC_Lev_Ref[c].nPriestLife, 0, &cbValue); 
		ret = SQLGetData(hStmt, 22, SQL_C_SLONG, &NPC_Lev_Ref[c].nWarriorExpRate, 0, &cbValue);
		ret = SQLGetData(hStmt, 23, SQL_C_SLONG, &NPC_Lev_Ref[c].nThiefExpRate, 0, &cbValue);
		ret = SQLGetData(hStmt, 24, SQL_C_SLONG, &NPC_Lev_Ref[c].nArcherExpRate, 0, &cbValue); 
		ret = SQLGetData(hStmt, 25, SQL_C_SLONG, &NPC_Lev_Ref[c].nWizardExpRate, 0, &cbValue); 
		ret = SQLGetData(hStmt, 26, SQL_C_SLONG, &NPC_Lev_Ref[c].nPriestExpRate, 0, &cbValue); 
		
		if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) 
		{
			MyLog( LOG_FATAL, "NPC_Lv2Exp: Error!!! (%d)", ret) ;
			return -1 ;
		}
		c++ ;
		ret= SQLFetch(hStmt) ;
	}	
	
	nNPC_Lev= c ;
	SQLFreeStmt(hStmt, SQL_DROP) ;
	MyLog( LOG_NORMAL, "	.NPC_Lv2Exp		%4d data Loaded", c );
	
	int i;
	char * tt = (char *)NPC_Lev_Ref;
	char crc = 0;
	for( i = 0 ; i < sizeof( NPCLev_to_Exp ) * Num_Of_NPC_Lev ; i  ++)
	{
		*tt += ( 100 + i );
		crc  += *tt;
		tt ++;
	}
	
	FILE *fp;
	fp = fopen( "./Output/levelexp.bin", "wb" ); // CSD-030306
	if( fp) 
	{
		fwrite( NPC_Lev_Ref, sizeof( NPCLev_to_Exp ), Num_Of_NPC_Lev, fp );
		fwrite( &crc,1,1, fp);
		fwrite( NPC_Lev_Ref, sizeof( NPCLev_to_Exp ), Num_Of_NPC_Lev, fp );
		fclose(fp);
	}
	
	tt = (char *)NPC_Lev_Ref;
	for( i = 0 ; i < sizeof( NPCLev_to_Exp ) * Num_Of_NPC_Lev ; i  ++)
	{
		*tt -= ( 100 + i );
		tt ++;
	}
	
	return c ;
}

int initNPCGenerationTable(void)
{	//< CSD-030806
	char query_stmt[80]= {0, };
	strcpy_s(query_stmt, sizeof(query_stmt), "select * from NPC_Generation_SP order by NO") ;

	HSTMT hStmt= NULL;
	SQLAllocStmt(hDBC, &hStmt);
	RETCODE retCode = SQLExecDirect(hStmt, (UCHAR *)query_stmt, SQL_NTS) ;
	
	if (retCode != SQL_SUCCESS_WITH_INFO && retCode != SQL_SUCCESS) 
	{
		MyLog( LOG_FATAL, "NPC_Generation_SP: ExecDirect Error ");
		SQLFreeStmt(hStmt, SQL_DROP);
		return -1 ;
	}
	
	int nNo = 0;

	while ((retCode = SQLFetch(hStmt)) == SQL_SUCCESS)
	{
		SDWORD cbValue;		
		SQLGetData(hStmt, 1, SQL_C_ULONG, &nNo, 0, &cbValue);   

		if (nNo > Num_Of_NPC_Generation)
		{
			SQLFreeStmt(hStmt, SQL_DROP);
			return -1;
		}

		SQLGetData(hStmt, 2, SQL_C_ULONG, &NPC_Gen_Ref[nNo].SprNO,0, &cbValue);
		//ret = SQLGetData(hStmt,   3, SQL_C_CHAR,		NPC_Gen_Ref[nNo].Han_name, 20, &cbValue); NPC_Gen_Ref[c].Han_name[19] = 0;
		SQLGetData(hStmt, 4, SQL_C_CHAR, NPC_Gen_Ref[nNo].Name, 30, &cbValue); NPC_Gen_Ref[nNo].Name[31] = 0;
		//SQLGetData(hStmt, 5, SQL_C_ULONG, &NPC_Gen_Ref[nNo].Selectable,0, &cbValue);
		SQLGetData(hStmt, 6, SQL_C_ULONG, &NPC_Gen_Ref[nNo].Sel_gender,0, &cbValue);
		//ret = SQLGetData(hStmt,   7, SQL_C_ULONG,		&NPC_Gen_Ref[nNo].Sel_FirstName,0, &cbValue);
		//ret = SQLGetDatahStmt,   8, SQL_C_ULONG,		&NPC_Gen_Ref[nNo].Sel_LastName,0, &cbValue);
		SQLGetData(hStmt, 9, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nNpcRace, 0, &cbValue);
		SQLGetData(hStmt, 10, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nNpcNK, 0, &cbValue);
		SQLGetData(hStmt, 15, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nStep, 0, &cbValue);
		SQLGetData(hStmt, 16, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nAttr, 0, &cbValue);
		SQLGetData(hStmt, 17, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nHpMax, 0, &cbValue);
		SQLGetData(hStmt, 18, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nEpMax, 0, &cbValue);
		SQLGetData(hStmt, 19, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nStrikeDamage, 0, &cbValue);
		SQLGetData(hStmt, 20, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nStrikeRandom, 0, &cbValue);
		SQLGetData(hStmt, 21, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nMagicDamage, 0, &cbValue);
		SQLGetData(hStmt, 22, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nMagicRandom, 0, &cbValue);
		SQLGetData(hStmt, 23, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nCriticalRate, 0, &cbValue);
		SQLGetData(hStmt, 24, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nCriticalDamage, 0, &cbValue);
		SQLGetData(hStmt, 25, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nPiercingStrike, 0, &cbValue);
		SQLGetData(hStmt, 26, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nPiercingMagic, 0, &cbValue);
		SQLGetData(hStmt, 27, SQL_C_ULONG, &NPC_Gen_Ref[nNo].Sel_Job_Alchemist, 0, &cbValue);
		SQLGetData(hStmt, 28, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nAIBase, 0, &cbValue); // CSD-021114
		SQLGetData(hStmt, 29, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nAppearType, 0, &cbValue);
		SQLGetData(hStmt, 30, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nAIType, 0, &cbValue);
		SQLGetData(hStmt, 31, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nAINo, 0, &cbValue);
		SQLGetData(hStmt, 32, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nLvMin, 0, &cbValue);
		SQLGetData(hStmt, 33, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nLvMax, 0, &cbValue);
		SQLGetData(hStmt, 36, SQL_C_ULONG, &NPC_Gen_Ref[nNo].Money_min, 0, &cbValue);
		SQLGetData(hStmt, 37, SQL_C_ULONG, &NPC_Gen_Ref[nNo].Money_Max, 0, &cbValue);
		SQLGetData(hStmt, 38, SQL_C_ULONG, &NPC_Gen_Ref[nNo].AttackDelay, 0, &cbValue);
		SQLGetData(hStmt, 39, SQL_C_ULONG, &NPC_Gen_Ref[nNo].AccessDelay, 0, &cbValue);
		SQLGetData(hStmt, 40, SQL_C_ULONG, &NPC_Gen_Ref[nNo].BackDelay, 0, &cbValue);
		SQLGetData(hStmt, 41, SQL_C_ULONG, &NPC_Gen_Ref[nNo].PCfindDelay, 0, &cbValue);
		SQLGetData(hStmt, 42, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nRvAp, 0, &cbValue);
		SQLGetData(hStmt, 43, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nRvLife,0, &cbValue);
		SQLGetData(hStmt, 44, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nRvExp, 0, &cbValue);
		SQLGetData(hStmt, 45, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nAc, 0, &cbValue);
		SQLGetData(hStmt, 46, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nDpRate, 0, &cbValue); 
		SQLGetData(hStmt, 47, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nApRate, 0, &cbValue); 
		SQLGetData(hStmt, 48, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nPhysical, 0, &cbValue); 
		SQLGetData(hStmt, 49, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nFire, 0, &cbValue); 
		SQLGetData(hStmt, 50, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nIce, 0, &cbValue); 
		SQLGetData(hStmt, 51, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nElect, 0, &cbValue); 
		SQLGetData(hStmt, 52, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nHoly, 0, &cbValue); 
		SQLGetData(hStmt, 53, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nStun, 0, &cbValue);
		SQLGetData(hStmt, 54, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nSlow, 0, &cbValue);
		SQLGetData(hStmt, 55, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nPoison, 0, &cbValue);
		SQLGetData(hStmt, 56, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nBlind, 0, &cbValue);
		SQLGetData(hStmt, 57, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nDrain, 0, &cbValue);
		SQLGetData(hStmt, 58, SQL_C_ULONG, &NPC_Gen_Ref[nNo].nFreeze, 0, &cbValue);
		//SQLGetData(hStmt, 59, SQL_C_ULONG, &NPC_Gen_Ref[nNo].SpiritValue, 0, &cbValue);
		//SQLGetData(hStmt, 60, SQL_C_ULONG, &NPC_Gen_Ref[nNo].SpiritValue_Range, 0, &cbValue);
		//SQLGetData(hStmt, 76, SQL_C_ULONG, &NPC_Gen_Ref[nNo].Sel_Dice, 0, &cbValue);
		//SQLGetData(hStmt, 77, SQL_C_ULONG, &NPC_Gen_Ref[nNo].QuantityperLv, 0, &cbValue);
		//SQLGetData(hStmt, 93, SQL_C_ULONG, &NPC_Gen_Ref[nNo].Sel_Dice1, 0, &cbValue);
		//SQLGetData(hStmt, 94, SQL_C_ULONG, &NPC_Gen_Ref[nNo].QuantityperLv1, 0, &cbValue);
		//SQLGetData(hStmt, 95, SQL_C_ULONG, &NPC_Gen_Ref[nNo].Leathal_weapon1, 0, &cbValue);
		//SQLGetData(hStmt, 96, SQL_C_ULONG, &NPC_Gen_Ref[nNo].Leathal_weapon2, 0, &cbValue);
		//SQLGetData(hStmt, 97, SQL_C_ULONG, &NPC_Gen_Ref[nNo].Damage_weapon, 0, &cbValue);
		//SQLGetData(hStmt, 107, SQL_C_ULONG, &NPC_Gen_Ref[nNo].Damage_WP, 0, &cbValue);
		//SQLGetData(hStmt, 108, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Leathal_WS, 0, &cbValue);
		//SQLGetData(hStmt, 109, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Success, 0, &cbValue);
		//SQLGetData(hStmt, 110, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Leathal_ps, 0, &cbValue);
		//SQLGetData(hStmt, 111, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].DamagePercent_ps, 0, &cbValue);
		SQLGetData(hStmt, 112, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Inventory[0], 0, &cbValue);
		SQLGetData(hStmt, 113, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Inventory[1], 0, &cbValue);
		SQLGetData(hStmt, 114, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Inventory[2], 0, &cbValue);
		SQLGetData(hStmt, 115, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Inventory[3], 0, &cbValue);
		SQLGetData(hStmt, 116, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Inventory[4], 0, &cbValue);
		SQLGetData(hStmt, 117, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Inventory[5], 0, &cbValue);
		SQLGetData(hStmt, 118, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Inventory[6], 0, &cbValue);
		SQLGetData(hStmt, 119, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Inventory[7], 0, &cbValue);
		SQLGetData(hStmt, 120, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Inventory[8], 0, &cbValue);
		SQLGetData(hStmt, 121, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Inventory[9], 0, &cbValue);
		SQLGetData(hStmt, 122, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Inventory[10], 0, &cbValue);
		SQLGetData(hStmt, 123, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Inventory[11], 0, &cbValue);
		SQLGetData(hStmt, 124, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Inventory[12], 0, &cbValue);
		SQLGetData(hStmt, 125, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Inventory[13], 0, &cbValue);
		SQLGetData(hStmt, 126, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Inventory[14], 0, &cbValue);
		//SQLGetData(hStmt, 127, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Amount_Dice, 0, &cbValue);
		//SQLGetData(hStmt, 128, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Righthand, 0, &cbValue);
		//SQLGetData(hStmt, 129, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Lefthand, 0, &cbValue);
		//SQLGetData(hStmt, 130, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Body, 0, &cbValue);
		//SQLGetData(hStmt, 131, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Helmet, 0, &cbValue);
		//SQLGetData(hStmt, 132, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].leg, 0, &cbValue);
		//SQLGetData(hStmt, 133, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Leg_Probability, 0, &cbValue);
		//SQLGetData(hStmt, 134, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].neck, 0, &cbValue);
		//SQLGetData(hStmt, 135, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].neck_Probability, 0, &cbValue);
		//SQLGetData(hStmt, 136, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].unique1, 0, &cbValue);
		//SQLGetData(hStmt, 137, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Unique1_probalbility, 0, &cbValue);
		//SQLGetData(hStmt, 138, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Unique2,0, &cbValue);
		//SQLGetData(hStmt, 139, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].unique2_probability, 0, &cbValue);
		//SQLGetData(hStmt, 140, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Crapple, 0, &cbValue);
		//SQLGetData(hStmt, 141, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].swordmanship, 0, &cbValue);
		//SQLGetData(hStmt, 142, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Archery, 0, &cbValue);
		//SQLGetData(hStmt, 143, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Fencing, 0, &cbValue);
		//SQLGetData(hStmt, 144, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Mace_fighting, 0, &cbValue);
		//SQLGetData(hStmt, 145, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Pierce, 0, &cbValue);
		//SQLGetData(hStmt, 146, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Whirl, 0, &cbValue);
		//SQLGetData(hStmt, 147, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Hurl, 0, &cbValue);
		//SQLGetData(hStmt, 148, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Parrying,0, &cbValue);
		//SQLGetData(hStmt, 149, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Double_Swordmanship, 0, &cbValue);
		//SQLGetData(hStmt, 150, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Double_MaceFighting, 0, &cbValue);
		//SQLGetData(hStmt, 151, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Magery, 0, &cbValue);
		//SQLGetData(hStmt, 152, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Orison, 0, &cbValue);
		//SQLGetData(hStmt, 153, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].LV_Str, 0, &cbValue);
		//SQLGetData(hStmt, 154, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Con, 0, &cbValue);
		//SQLGetData(hStmt, 155, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Dex, 0, &cbValue);
		//SQLGetData(hStmt, 156, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Wis, 0, &cbValue);
		//SQLGetData(hStmt, 157, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Int, 0, &cbValue);
		SQLGetData(hStmt, 158, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Movp, 0, &cbValue);
		//SQLGetData(hStmt, 159, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Char, 0, &cbValue);
		//SQLGetData(hStmt, 160, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Endu, 0, &cbValue);
		//SQLGetData(hStmt, 161, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Mor, 0, &cbValue);
		//SQLGetData(hStmt, 162, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Luck, 0, &cbValue);
		//SQLGetData(hStmt, 163, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].WS_PS, 0, &cbValue);
		//SQLGetData(hStmt, 164, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Fullpoint, 0, &cbValue);
		//SQLGetData(hStmt, 165, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Suplus_Resist_Death, 0, &cbValue);
		//SQLGetData(hStmt, 166, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Suplus_Resist_Ston, 0, &cbValue);
		//SQLGetData(hStmt, 167, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Suplus_Resist_Magic, 0, &cbValue);
		//SQLGetData(hStmt, 168, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Suplus_Resist_Fire, 0, &cbValue);
		//SQLGetData(hStmt, 169, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Suplus_Resist_Ice, 0, &cbValue);
		//SQLGetData(hStmt, 170, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Suplus_Resist_Light, 0, &cbValue);
		//SQLGetData(hStmt, 171, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].DivideLv,0, &cbValue);
		//SQLGetData(hStmt, 172, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Resist_Range,0, &cbValue);
		SQLGetData(hStmt, 173, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Tamingable,0, &cbValue);
		SQLGetData(hStmt, 174, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Taming_min,0, &cbValue);
		SQLGetData(hStmt, 175, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Taming_max,0, &cbValue);
		SQLGetData(hStmt, 176, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Talkanimal_min,0, &cbValue);
		SQLGetData(hStmt, 177, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Talkanimal_max,0, &cbValue);
		//SQLGetData(hStmt, 178, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Resist,0, &cbValue);
		//SQLGetData(hStmt, 179, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Resist_range1,0, &cbValue);
		SQLGetData(hStmt, 180, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].ValueHP,0, &cbValue);
		SQLGetData(hStmt, 181, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].ValueSP,0, &cbValue);
		//SQLGetData(hStmt, 182, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Rely_value,0, &cbValue);
		//SQLGetData(hStmt, 183, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].nor_range,0, &cbValue);
		//SQLGetData(hStmt, 184, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].att_range,0, &cbValue);
		SQLGetData(hStmt, 185, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].offset[0],0, &cbValue);
		SQLGetData(hStmt, 186, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].offset[1],0, &cbValue);
		SQLGetData(hStmt, 187, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].offset[2],0, &cbValue);
		SQLGetData(hStmt, 188, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].offset[3],0, &cbValue);
		SQLGetData(hStmt, 189, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].offset[4],0, &cbValue);
		SQLGetData(hStmt, 190, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].offset[5],0, &cbValue);
		SQLGetData(hStmt, 191, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].offset[6],0, &cbValue);
		SQLGetData(hStmt, 192, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].offset[7],0, &cbValue);
		SQLGetData(hStmt, 193, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].offset[8],0, &cbValue);
		SQLGetData(hStmt, 194, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].offset[9],0, &cbValue);
		SQLGetData(hStmt, 195, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].offset[10],0, &cbValue);
		SQLGetData(hStmt, 196, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].offset[11],0, &cbValue);
		SQLGetData(hStmt, 197, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].offset[12],0, &cbValue);
		SQLGetData(hStmt, 198, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].offset[13],0, &cbValue);
		SQLGetData(hStmt, 199, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].offset[14],0, &cbValue);
		SQLGetData(hStmt, 200, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].offset[15],0, &cbValue);
		SQLGetData(hStmt, 201, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].offset[16],0, &cbValue);
		SQLGetData(hStmt, 202, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].offset[17],0, &cbValue);
		SQLGetData(hStmt, 203, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].offset[18],0, &cbValue);
		SQLGetData(hStmt, 204, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].offset[19],0, &cbValue);
		//SQLGetData(hStmt, 205, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].neglect_attack, 0, &cbValue);
		//SQLGetData(hStmt, 206, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].neglect_avoid, 0, &cbValue);
		//SQLGetData(hStmt, 207, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].check_pass, 0, &cbValue);
		//SQLGetData(hStmt, 208, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].party, 0, &cbValue);
		//SQLGetData(hStmt, 209, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].enemy_no, 0, &cbValue);
		//SQLGetData(hStmt, 210, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].levelgap, 0, &cbValue);
		//SQLGetData(hStmt, 211, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].apgap, 0, &cbValue);
		//SQLGetData(hStmt, 212, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].vpgap, 0, &cbValue);
		//SQLGetData(hStmt, 213, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].opendoor, 0, &cbValue);
		//SQLGetData(hStmt, 214, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].lockdoor, 0, &cbValue);
		//SQLGetData(hStmt, 215, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].closedoor, 0, &cbValue);
		//SQLGetData(hStmt, 216, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].breakdoor, 0, &cbValue);
		//SQLGetData(hStmt, 217, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].openobj, 0, &cbValue);
		//SQLGetData(hStmt, 218, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].lockbj, 0, &cbValue);
		//SQLGetData(hStmt, 219, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].closeobj, 0, &cbValue);
		//SQLGetData(hStmt, 220, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].breakobj, 0, &cbValue);
		//SQLGetData(hStmt, 221, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].act_month, 0, &cbValue);
		//SQLGetData(hStmt, 222, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].act_time, 0, &cbValue);
		//SQLGetData(hStmt, 223, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].act_bi, 0, &cbValue);
		//SQLGetData(hStmt, 224, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].act_chun, 0, &cbValue);
		//SQLGetData(hStmt, 225, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].act_nun, 0, &cbValue);
		SQLGetData(hStmt, 226, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Get_meat, 0, &cbValue);
		SQLGetData(hStmt, 227, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Meat_amount, 0, &cbValue);
		SQLGetData(hStmt, 228, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Get_skin, 0, &cbValue);
		SQLGetData(hStmt, 229, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Skin_amount, 0, &cbValue);
		SQLGetData(hStmt, 230, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Get_Wool, 0, &cbValue);
		SQLGetData(hStmt, 231, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Wool_amount, 0, &cbValue);
		SQLGetData(hStmt, 232, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].Get_beewax, 0, &cbValue);
		SQLGetData(hStmt, 233, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].BeeWax_amount, 0, &cbValue);
		SQLGetData(hStmt, 234, SQL_C_ULONG,	&NPC_Gen_Ref[nNo].mutant, 0, &cbValue);
	}

	SQLFreeStmt(hStmt, SQL_DROP);	
	MyLog(LOG_NORMAL, "	.NPC Generation Info	%4d data Loaded", nNo);
	return nNo;
}	//> CSD-030806