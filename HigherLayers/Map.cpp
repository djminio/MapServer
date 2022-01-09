/*****************************************************************************\
* Copyright (c), Future Entertainment World / Seoul, Republic of Korea        *
* All Rights Reserved.                                                        *
*                                                                             *
* This document contains proprietary and confidential information.  No        *
* parts of this document or the computer program it embodies may be in        *
* any way copied, duplicated, reproduced, translated into a different         *
* programming language, or distributed to any person, company, or             *
* corporation without the prior written consent of Future Entertainment World *
\*****************************************************************************/
#include "..\stdafx.h"
#include "DefaultHeader.h"

#include "io.h"
#include "RajaSystem.h"
#include "..\LowerLayers\mylog.h"
#include "scrp_exe.h"		// 020620 YGI
		
//#include "map.h"
		
		
MAP					g_Map;
TILE				**TileMap; // TileMap[ VILLAGE_SIZE ] [ VILLAGE_SIZE ]
EVENTLISTHEADER		g_EventListHeaderMap;
EVENTLISTHEADER		g_EventListHeaderRoom;
EVENTLISTHEADER		g_EventListHeaderMusic;
		
LPMAPFILE           g_lpMapFile;
		
		
///////////////////////////////////////////////////////////////////////////////
//		
		
BOOL	MapBuild( LPMAP lpMap, char *filename );
void	MapDestroy( LPMAP lpMap );
BOOL	MapRestoreSurface( LPMAP lpMap );
BOOL	MapReadFile( LPMAPFILE lpMapFile, char* pszFileName );
void	MapDisplay( int nType );
void	TileDisplay( LPTILE lpTile, int x, int y, int nType );
		
		
BOOL		BuildEventList( char* lpszFileName );
void		DestroyEventList();
LPEVENTLIST	AddEventList( LPEVENTLISTHEADER lpEventListHeader, WORD x, WORD y, WORD wIndex );
void		DeleteEventList( LPEVENTLISTHEADER lpEventListHeader, LPEVENTLIST lpEventList );
LPEVENTLIST	FindEventList( LPEVENTLISTHEADER lpEventListHeader, WORD x, WORD y );
BOOL		ReadEventList( char* lpszFileName );
		
char MapName[ NM_LENGTH] = {0,};
char MAPNAME[ NM_LENGTH] = {0,};
int  MapNumber = 0;
int	 g_MapPort = 0;		// 010522 YGI

///////////////////////////////////////////////////////////////////////////////
//		
		
BOOL	
MapBuild( LPMAP lpMap, char *mapname )
{		
	lpMap->wScreenWidth = 20;
	lpMap->wScreenHeight = 11;
	char szFileName[ FILENAME_MAX];
		
	
	wsprintf( szFileName, "%s/map/%s.map", GameServerDataPath, MapName );
	if ( !MapReadFile( &lpMap->file, szFileName ) )
	{	
		//Error( "[%s] LoadError!", szFileName );
		return	FALSE;
	}	
		
	g_lpMapFile = &g_Map.file;
	return	TRUE;
}		
		
		
		
		
BOOL MapReadFile( LPMAPFILE lpMapFile, char* pszFileName )
{		
	FILE *fp;
	int		t = 0, t1;
	DWORD	ver;	
		
	fp = fopen( pszFileName, "rb" );
	if( fp == NULL ) return 0;
		
	{	
		fread( &ver, sizeof( ver ), 1, fp );
		if( ver == 0xffff )
		{	
			fread( &lpMapFile->wType, sizeof( lpMapFile->wType ), 1, fp );
			fread( &lpMapFile->wWidth, sizeof( lpMapFile->wWidth ), 1, fp );
			fread( &lpMapFile->wHeight, sizeof( lpMapFile->wHeight ), 1, fp );

			TileMap = new TILE*[ lpMapFile->wWidth ];
			for(int s = 0; s < lpMapFile->wWidth; s++)
			{
				TileMap[s] = new TILE[lpMapFile->wHeight];
			}
			
			for ( int y = 0; y < lpMapFile->wHeight; y++ )
			{
				for ( int x = 0; x < lpMapFile->wWidth; x++ )
				{
					fread( &TileMap[ x ][ y ], sizeof( TILE ), 1, fp );
			
//					cc = (double*)&TileMap[ x][ y];
//					*cc -= (x+y);
//					cc = ((double*)&TileMap[ x][ y]) + 1;
//					*cc -= (y);
					t += *(int*)&TileMap[ x][ y];
				}
			}
			
			fread( &t1, sizeof( t ), 1, fp );
			if( t1 - t ) Error( "CheckSumError " );
			fread( &lpMapFile->cOdd, sizeof( lpMapFile->cOdd ), 1, fp );
			fread( &lpMapFile->cEven, sizeof( lpMapFile->cEven ), 1, fp );
			
			fclose( fp );
		}	
		else
		{	
			fseek( fp, 0, SEEK_SET );
			
			fread( &lpMapFile->wType, sizeof( lpMapFile->wType ), 1, fp );
			fread( &lpMapFile->wWidth, sizeof( lpMapFile->wWidth ), 1, fp );
			fread( &lpMapFile->wHeight, sizeof( lpMapFile->wHeight ), 1, fp );

			TileMap = new TILE*[ lpMapFile->wWidth ];
			for(int s = 0; s < lpMapFile->wWidth; s++)
			{
				TileMap[s] = new TILE[lpMapFile->wHeight];
			}
			
			for ( int y = 0; y < lpMapFile->wHeight ; y++ )
			{
				for ( int x = 0; x < lpMapFile->wWidth ; x++ )
				{
					fread( &TileMap[ x ][ y ], sizeof( TILE ), 1, fp );
				}
			}
			fread( (char *)&t, sizeof( t ), 1, fp );
			fread( &lpMapFile->cOdd, sizeof( lpMapFile->cOdd ), 1, fp );
			fread( &lpMapFile->cEven, sizeof( lpMapFile->cEven ), 1, fp );
			fclose( fp );
		}
	}	
		
	return	TRUE;
}		
		
		
		
///////////////////////////////////////////////////////////////////////////////
// event
		
BOOL	
BuildEventList( char* mapname )
{		
	char szFileName[ FILENAME_MAX];
		
	wsprintf( szFileName, "./event/%s.ent", MapName );
	g_EventListHeaderMap.lpFirst = g_EventListHeaderMap.lpLast = NULL;
	g_EventListHeaderMap.wCount = 0;
		
	g_EventListHeaderRoom.lpFirst = g_EventListHeaderRoom.lpLast = NULL;
	g_EventListHeaderRoom.wCount = 0;
		
	g_EventListHeaderMusic.lpFirst = g_EventListHeaderMusic.lpLast = NULL;
	g_EventListHeaderMusic.wCount = 0;
		
		
	if ( !ReadEventList( szFileName ) )
	{	
		//JustMsg("[%s] \n LoadError", szFileName );
		return	FALSE;
	}	
		
	return	TRUE;
}		
		
void	
DestroyEventListSub( LPEVENTLISTHEADER lpEventListHeader )
{
	LPEVENTLIST		lpEventList, lpEventListNext;
	
	lpEventList = lpEventListNext = lpEventListHeader->lpFirst;
	while ( lpEventList )
	{
		lpEventListNext = lpEventList->lpNext;
		DeleteEventList( lpEventListHeader, lpEventList );
		lpEventList = lpEventListNext;
	}	
}		
		
		
		
void DestroyEventList( void )
{		
	DestroyEventListSub( &g_EventListHeaderMap );
	DestroyEventListSub( &g_EventListHeaderRoom );
	DestroyEventListSub( &g_EventListHeaderMusic);
}		
		
		
LPEVENTLIST
AddEventList( LPEVENTLISTHEADER lpEventListHeader, WORD x, WORD y, WORD wIndex )
{		
	LPEVENTLIST		lpEventList;
		
	if ( ( lpEventList = ( LPEVENTLIST )GlobalAlloc( GPTR, sizeof( EVENTLIST ) ) ) == ( LPEVENTLIST )NULL )
	{	
		return	NULL;
	}	
		
	if ( lpEventListHeader->lpLast == NULL )
	{	
		lpEventList->lpPrev = lpEventList->lpNext = NULL;
		
		lpEventListHeader->lpFirst = lpEventListHeader->lpLast = lpEventList;
	}
	else
	{
		lpEventList->lpPrev = lpEventListHeader->lpLast;
		lpEventList->lpNext = NULL;

		lpEventListHeader->lpLast->lpNext = lpEventList;
		lpEventListHeader->lpLast = lpEventList;
	}
	lpEventListHeader->wCount++;

	lpEventList->x = x;
	lpEventList->y = y;
	lpEventList->index = wIndex;

	return	lpEventList;
}

void
DeleteEventList( LPEVENTLISTHEADER lpEventListHeader, LPEVENTLIST lpEventList )
{
	if ( lpEventList != NULL )
	{
		if ( lpEventList->lpPrev != NULL )
		{
			lpEventList->lpPrev->lpNext = lpEventList->lpNext;
		}
		if ( lpEventList->lpNext != NULL )
		{
			lpEventList->lpNext->lpPrev = lpEventList->lpPrev;
		}

		if ( lpEventListHeader->lpFirst == lpEventList )
		{
			lpEventListHeader->lpFirst = lpEventList->lpNext;
		}
		if ( lpEventListHeader->lpLast == lpEventList )
		{
			lpEventListHeader->lpLast = lpEventList->lpPrev;
		}

		lpEventListHeader->wCount--;

		GlobalFree( lpEventList );
		lpEventList = NULL;
	}
}

LPEVENTLIST
FindEventList( LPEVENTLISTHEADER lpEventListHeader, WORD x, WORD y )
{
	LPEVENTLIST		lpEventList = lpEventListHeader->lpFirst;

	while ( lpEventList )
	{
		if ( lpEventList->x == x && lpEventList->y == y )
		{
			return	lpEventList;
		}

		lpEventList = lpEventList->lpNext;
	}

	return	NULL;
}

BOOL
ReadEventList( char* lpszFileName )
{		
	int		fh;
	WORD	count;
	WORD	x, y;
	WORD	index;
	int		i;
		
	if ( ( fh = _lopen( lpszFileName, OF_READ ) ) != -1 )
	{	
		_lread( fh, &count, sizeof( WORD ) );
		for ( i = 0; i < count; i++ )
		{
			_lread( fh, &x, sizeof( WORD ) );
			_lread( fh, &y, sizeof( WORD ) );
			_lread( fh, &index, sizeof( WORD ) );
		
			AddEventList( &g_EventListHeaderMap, x, y, index );
		}
		
		_lread( fh, &count, sizeof( WORD ) );
		for ( i = 0; i < count; i++ )
		{
			_lread( fh, &x, sizeof( WORD ) );
			_lread( fh, &y, sizeof( WORD ) );
			_lread( fh, &index, sizeof( WORD ) );
		
			AddEventList( &g_EventListHeaderRoom, x, y, index );
		}
		
		_lread( fh, &count, sizeof( WORD ) );
		for ( i = 0; i < count; i++ )
		{
			_lread( fh, &x, sizeof( WORD ) );
			_lread( fh, &y, sizeof( WORD ) );
			_lread( fh, &index, sizeof( WORD ) );
		
			AddEventList( &g_EventListHeaderMusic, x, y, index );
		}
		
		_lclose( fh );
		return	TRUE;
	}		
		
	return	FALSE;
}			







// ---------------------------------------마우스에 의한 Map Scroll...

int ReturnEventNo( int mx, int my )
{
	if (mx < 0)  return 0;
	if (my < 0)  return 0; 
	if (mx >= g_lpMapFile->wWidth)   return 0;
	if (my >= g_lpMapFile->wHeight)  return 0;

	LPEVENTLIST	lpEventList;

	if( TileMap[ mx][ my ].attr_map )
	{
		lpEventList = FindEventList( &g_EventListHeaderMap, ( WORD )mx, ( WORD )my );
		return lpEventList->index;
	}

	return 0;
}


int ChangeEventNo( int mx, int my, int eventno  )
{	
	if (mx < 0)  return FALSE;
	if (my < 0)  return FALSE; 
	if (mx >= g_lpMapFile->wWidth)   return FALSE;
	if (my >= g_lpMapFile->wHeight)  return FALSE;

	LPEVENTLIST	lpEventList;
				
	if( TileMap[ mx][ my ].attr_map )
	{			
		lpEventList = FindEventList( &g_EventListHeaderRoom, ( WORD )mx, ( WORD )my );
		lpEventList->index = eventno;
				
		return TRUE;
	}			
				
	return FALSE;
}				
// 010508 KHS
void RecvRoofOpen( int cn, int roofno )
{
	connections[cn].chrlst.openhouse = (MapNumber << 8 ) + roofno;
}

void RecvRoofClose( int cn, int roofno )
{
	connections[cn].chrlst.openhouse = -1;
}

int MapPort( char *mapname )
{
	int i;

	_strupr_s( mapname, 20 );

	for( i = 0 ;  i< MAX_MAP_ ; i ++ )
	{
		if( strcmp( MapInfo[i].mapfile, mapname ) == 0 ) return MapInfo[i].port;
	}

	return 0;
}

string GetMapName(int nPort)
{
  if (nPort <= 0)  return "";

	for (int i = 0; i <  MAX_MAP_ ; ++i)
	{
		if (nPort == MapInfo[i].port) return MapInfo[i].mapfile;
	}

  return "";
}

const char *GetMapName2(int nPort)
{
  if (nPort <= 0)  return NULL;

	for (int i = 0; i <  MAX_MAP_ ; ++i)
	{
		if (nPort == MapInfo[i].port) return MapInfo[i].mapfile;
	}

  return NULL;
}


int GetChangeMapPort(t_connection c[], int cn, t_packet *packet)
{
	int port = MapPort( c[cn].mapname );
	if( port == 0 ) 
	{
		MyLog(  LOG_FATAL, "Map Port Missing.   T T(%s)", c[cn].mapname );
		return 0;
	}
	packet->u.server_change_map.port = port;
	return(1);
};

int GetMapMoveType(const char* map_name)
{	//< CSD-030515
	for (int i = 0 ; i < MAX_MAP_ ; ++i)
	{
		if (0 == _stricmp(MapInfo[i].mapfile, map_name))
		{
			return MapInfo[i].m_MoveType;
		}
	}

	return MMT_FAIL;
}	//> CSD-030515


// 020620 YGI -------------------
int GetMapPortByName( char *map_name )
{
	_strupr_s( map_name, 20 );
	for( int i = 0 ;  i< MAX_MAP_ ; i ++ )
	{
		if( strcmp( MapInfo[i].mapfile, map_name ) == 0 ) 
		{
			return MapInfo[i].port;
		}
	}

	return 0;
}

int GotoUser( char *map_name, int x, int y, short int cn )
{
	if( strcmp( map_name, MapName ) == 0 )		// 같은 곳에 있다.
	{
		MovePc( cn, x, y );
		return 1;
	}
	else		// 다른 맵에 있다. 맵이동을 시켜준다.
	{
		 return MapMove( cn, map_name, x, y );
	}
}


















