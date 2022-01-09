/*******************************************************************

		Map.H

********************************************************************/


#define	VILLAGE_SIZE				720
#define TILE_SIZE					32


#ifndef	__MAP_H__
#define	__MAP_H__


#pragma	pack	(push,1)

#define TILE_MAX_   3600

#include "MapInfo.h"

typedef struct	tagTILE
{
	DWORD	back			: 12;
	DWORD	front			: 12;
	DWORD	empty__1	    :  1;
	DWORD	rooftype		:  1;
	DWORD	attr_dont		:  1;
	DWORD	attr_map		:  1;
	DWORD	attr_room		:  2;
	DWORD	empty__2		:  1;
	DWORD	attr_skill		:  1;

	DWORD	roof			: 12;
	DWORD	show_roof		:  1;
	DWORD	frontoffset		: 10;

	DWORD	occupied		:  1;
	DWORD	canmatch		:  1;
	DWORD	empty__4		:  1;
	DWORD	attr_light		:  1;
	DWORD	attr_inside		:  1;
	DWORD attr_river		:  1;
	DWORD	attr_magic		:  1;
	DWORD	attr_no_battle  :  1;
	DWORD	empty__6		:  1;

} TILE, *LPTILE;

#pragma pack (pop)
#pragma	pack (push, 8)

typedef struct	tagMAPFILE
{
	WORD		wType;
	WORD		wWidth, wHeight;
	char		cOdd, cEven;
} MAPFILE, *LPMAPFILE;

typedef struct	tagMAP
{
	int				x, y;
	WORD			wScreenWidth, wScreenHeight;
	MAPFILE			file;
} MAP, *LPMAP;


// event
typedef struct	tagEVENTLIST
{
	WORD				x, y;
	WORD				index;
	struct tagEVENTLIST	*lpPrev, *lpNext;
} EVENTLIST, *LPEVENTLIST;

typedef struct	tagEVENTLISTHEADER
{
	WORD			wCount;
	LPEVENTLIST		lpFirst, lpLast;
} EVENTLISTHEADER, *LPEVENTLISTHEADER;

#pragma pack (pop)

extern MAP					g_Map;
extern TILE					**TileMap;
extern EVENTLISTHEADER		g_EventListHeaderMap;
extern EVENTLISTHEADER		g_EventListHeaderRoom;
extern EVENTLISTHEADER		g_EventListHeaderMusic;


extern char					MapName[ NM_LENGTH];
extern char					MAPNAME[ NM_LENGTH];
extern int					MapNumber;
extern LPMAPFILE			g_lpMapFile;

#ifdef __cplusplus
extern "C"
{
#endif

extern BOOL		MapBuild( LPMAP lpMap, char *mapname );
extern void		MapDestroy( LPMAP lpMap );
extern BOOL		MapReadFile( LPMAPFILE lpMapFile, char* pszFileName );

extern BOOL			BuildEventList( char* lpszFileName );
extern void			DestroyEventList( void );
extern LPEVENTLIST	AddEventList( LPEVENTLISTHEADER lpEventListHeader, WORD x, WORD y, WORD wIndex );
extern void			DeleteEventList( LPEVENTLISTHEADER lpEventListHeader, LPEVENTLIST lpEventList );
extern LPEVENTLIST	FindEventList( LPEVENTLISTHEADER lpEventListHeader, WORD x, WORD y );
extern BOOL			ReadEventList( char* lpszFileName );



extern int ReturnEventNo( int mx, int my );
extern int ChangeEventNo( int mx, int my, int eventno  );
extern void RecvRoofOpen( int cn, int roofno );
extern void RecvRoofClose( int cn, int roofno );
extern int MapPort( char *mapname );
extern int GetChangeMapPort(t_connection c[], int cn, t_packet *packet);
#ifdef __cplusplus
}
#endif

std::string GetMapName(int nPort);
extern const char *GetMapName2(int nPort);
extern int GetMapMoveType( const char *map_name );

extern int GotoUser( char *map_name, int x, int y, short int cn );
extern int GetMapPortByName( char *map_name );
#endif	// __MAP_H__
