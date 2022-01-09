#include "..\stdafx.h"
#include "..\LowerLayers\servertable.h"
#include "..\LowerLayers\recvmsg.h"
#include "..\LowerLayers\mylog.h"
#include "DefaultHeader.h"
#include "MenuDefine.h"
#include "ItemList.h"
#include "CItem.h"
#include "teambattle.h"
#include "eventmgr.h"
#include "LogManager.h"
bool CheckContinueLint( char check )
{
	if(check == 0 || check == ' '||  check == ';' || check == '#' || check == '\n' ) return true;
	return false;
}

CMoneyToFame g_Money_to_Fame;
////////////////////////////////////////////////////////////
int CMoneyToFame::LoadData()
{
	char file[MAX_PATH];
	FILE *fp;
	sprintf_s( file, sizeof(file),"%s/data/money_fame.txt", GameServerDataPath );
	fp = fopen( file, "rt" );
	if( !fp ) return 0;

	char temp[256];
	char *token;
	m_nCount = 0;
	while( fgets( temp, 256, fp) )
	{
		if( ::CheckContinueLint( temp[0] ) ) continue;
		token = strtok( temp, "\t\n" );
		if( !token ) continue;
		data[m_nCount].money = (DWORD)atol( token );
		token = strtok( NULL, "\t\n" );
		data[m_nCount].fame = atoi( token );
		m_nCount ++;
	}
	fclose( fp );
	return 1;
}

int CMoneyToFame::GetFame( DWORD money )
{
	int fame = 0;
	for( int i=0; i<m_nCount; i++ )
	{
		if( money < data[i].money ) break;
		fame = data[i].fame;
	}
	return fame;
}

extern WORD FindMannagementMap( WORD port );
int InitDataFromNewDay( int day, int month )
{
	// 하루에 한번 기부 횟수를 갱신해 준다.
	int port = FindMannagementMap( MM_TODAY_GAMBLE_ITEM_MAIN_MAP_PORT );
	if( g_MapPort == port )
	{
		//CallClient( DB_DEMON, CMD_REFRESH_SALVATION_NAME ); - Commented out so that salvation log is always kept.
		MyLog( 0, "REMOVED! **Send to DB_DEMON for 'Delete Salvation**'" );
	}
	else
	{
		MyLog( 0, "No Send to DB_DEMON for 'Delete Salvation [%d != %d]'", GetOwnPort(), port );
	}

	if( CheckMapPort(MM_TODAY_GAMBLE_ITEM_MAIN_MAP_PORT ) )
	{
		if( day == 15 || day == 1 )
			CallClient( DB_DEMON, CMD_ADD_GUILD_BOX_MONEY );
	}

	// 매일 이벤트 활성화 되는게 있는지 확인한다.
	g_EventMgr.CheckActiveEvent( CEventMgr::ET_EVENT_ALL );
	g_EventMgr.CheckCloseEvent( CEventMgr::ET_EVENT_ALL );
	return 1;
}


/////////////////////////////////////////////
// class CMapPotalNation
int CMapPotalNation::InsertMap( char *map_name, int map_number, int money, int x, int y, int level )
{
	strcpy_s( m_MapNation[m_Count].m_MapFile, sizeof(m_MapNation[m_Count].m_MapFile), map_name );
	m_MapNation[m_Count].m_MapNumber=map_number;
	m_MapNation[m_Count].m_Money = money;
	m_MapNation[m_Count].x = x;
	m_MapNation[m_Count].y = y;
	m_MapNation[m_Count].m_Level = level;
	m_Count++;
	return 1;
}

CMapPotalNation g_MapPotal[MAX_NATION];

int LoadMapPotal()
{
	char filename[MAX_PATH];
	sprintf_s( filename, sizeof(filename), "%s/data/map_potal.txt", GameServerDataPath );
	int nation;
	int map_number;
	int money, x, y, level;

	char map_name[50] = {0,};

	FILE *fp = fopen( filename,"rt" );
	if( !fp ) return 0;

	for( int i=0; i<MAX_NATION; i++ )
	{
		g_MapPotal[i].clear();
	}

	char temp[1024];
	while( fgets( temp, 1024, fp) )
	{
		if( ::CheckContinueLint( temp[0] ) ) continue;
		char *token = strtok( temp, "\t\n" );
		if( !token ) continue;
		map_number = atoi( token );

		token = strtok( NULL, "\t\n" );
		strcpy_s( map_name, sizeof(map_name), token );

		token = strtok( NULL, "\t\n" );
		nation = atoi( token );

		token = strtok( NULL, "\t\n" );
		money = atoi( token );

		token = strtok( NULL, "\t\n" );
		x = atoi( token );

		token = strtok( NULL, "\t\n" );
		y = atoi( token );

		token = strtok( NULL, "\t\n" );		// 분류
		token = strtok( NULL, "\t\n" );		// 이미지 분류

		token = strtok( NULL, "\t\n" );		// 레벨
		level = atoi( token );

		g_MapPotal[nation].InsertMap( map_name, map_number, money, x, y, level );
		fgets( temp, 1024, fp);		// 설명 뛰어 넘기
	}

	fclose( fp );
	return 1;
}

int SaveLogDefault( char *file_name, char *str, int is_first )
{
	FILE *fp;
	if( is_first == 1 )
		fp = fopen( file_name, "wt" );
	else fp = fopen( file_name, "at+" );

	if( !fp ) return 0;
	fprintf( fp, "%s\n", str ); 
	fclose( fp );
	return 1;
}

// 030919 HK YGI
int SaveLogDefault2( char *file_name, char *str )
{
	if( !g_EventFlagMgr.IsSaveLogWhenSaveUserData() ) return 0;

	FILE *fp;
	fp = fopen( file_name, "at+" );

	if( !fp ) return 0;

	fprintf( fp, "<%02d-%02d-%02d %02d:%02d:%02d> %s\n", 
			g_year-2000, g_mon+1, g_day, 
			g_hour, g_min, g_sec, str );			

	fclose( fp );

	return 1;
}