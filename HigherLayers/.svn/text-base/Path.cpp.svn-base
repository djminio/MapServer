/*****************************************************************************\
* Copyright (c), Future Entertainment World / Seoul, Republic of Korea        *
* All Rights Reserved.                                                        *
\*****************************************************************************/
#include "..\stdafx.h"
#include "DefaultHeader.h"
#include "path.h"
//#include "map.h"
//#include "NpcList.h"
//#include "Hong_Sub.h"
//#include "ItemList.h"
#include "Citem.h"
//#include "Object.h"
			
			
#define MAX_NODE_POINTER		1000
#define MAX_STACK_POINTER		1000
			
			
			
LPSP_NODE	*NodeP[ MAX_NODE_POINTER];
LPSP_STACK	*StackP[MAX_STACK_POINTER];
			
			
///////////////////////////////////////////////////////////////////////////////
//			
			
int			Pathcount;
int			SHORTPATH[ MAX_SHORTPATH ][ 2 ];
LPSP_NODE	GLPSP_OPEN, GLPSP_CLOSED;
LPSP_STACK	GLPSP_STACK;
LPMAPFILE   MapFile;
			
///////////////////////////////////////////////////////////////////////////////
//			
			
BOOL		PathBuild( LPVOID lpCharacter, int dx, int dy );
void		PathDestroy( void );
LPSP_NODE	PathFind( CHARLIST *c, int sx, int sy, int dx, int dy );
void		GenerateSucc( LPSP_NODE lpspNode, int x, int y, int dx, int dy );
void		PropagateDown( LPSP_NODE lpspNode );
LPSP_NODE	CheckOPEN( int tilenum );
LPSP_NODE	CheckCLOSED( int tilenum );
void		Insert( LPSP_NODE lpspNode );
void		Push( LPSP_NODE lpspNode );
LPSP_NODE	Pop( void );
			
///////////////////////////////////////////////////////////////////////////////
//			
			

void InitPathBuild( void )
{		

}		


// 길찾기 할때...
// wis ==   0  똑똑한것. 
//		  100	
//		  200
//		  300
//		  399  명청한것. 

BOOL PathBuildWis( CHARLIST *n, int dx, int dy, int wis )
{
	return 0;
}


// 이동목표점까지를 직선으로 그어 충돌생기는것이 있나 알아본다. 
//  TileMap[][]을 체크하여 충돌되면 0을 리턴.


int				TempPath[ MAX_SHORTPATH][2];		// PathBuild()를 위한 변수..
int				TempPathCount;

int CheckLineCollision( LPCHARLIST ch, int sx, int sy, int ex, int ey )
{
	int t,distance;
	int xerr = 0, yerr = 0, delta_x, delta_y;
	int incx, incy;
	
	if( sx < 1 ) return 0; //Eleval 30/05/09 (modified 0 to 1)
	if( sy < 1 ) return 0; //Eleval 30/05/09 (modified 0 to 1)
	if( sx >= g_Map.file.wWidth - 2 ) return 0; //Eleval 30/05/09 (added - 2)
	if( sy >= g_Map.file.wHeight - 2 ) return 0; //Eleval 30/05/09 (added - 2)
	if( ex >= g_Map.file.wWidth - 2 ) return 0; //Eleval 30/05/09 (added - 2)
	if( ey >= g_Map.file.wHeight - 2 ) return 0; //Eleval 30/05/09 (added - 2)

	delta_x = ex - sx;
	delta_y = ey - sy;
	
	incx = (delta_x > 0) ?  1 : ((delta_x == 0) ? 0 : -1);
	incy = (delta_y > 0) ?  1 : ((delta_y == 0) ? 0 : -1);
	
	delta_x = abs(delta_x);
	delta_y = abs(delta_y);
	
	distance = (delta_x > delta_y) ? delta_x : delta_y;

	if( !FreeTile( ch, sx, sy , sx, sy ) )    return 0;

	TempPath[ 0 ][ 0 ] = sx;
	TempPath[ 0 ][ 1 ] = sy;

	TempPathCount = 1;

	for( t = 0 ; t <= distance ; t ++ )
	{
		xerr += delta_x;
		yerr += delta_y;
		if(xerr >= distance)    {        xerr -= distance;       sx += incx;      }
		if(yerr >= distance)    {        yerr -= distance;       sy += incy;      }
		if( !FreeTile( ch, sx, sy , sx, sy ) ) return 0;

		if ( TempPathCount >= MAX_SHORTPATH )
		{
			break;
		}
	
		TempPath[ TempPathCount ][ 0 ] = sx;
		TempPath[ TempPathCount ][ 1 ] = sy;
		TempPathCount++;
	}


	return 1;
}	


int MakeLinePath( LPCHARLIST ch, int dx, int dy )
{
	return 0;
}



BOOL		
PathBuild( CHARLIST *n, int dx, int dy )
{		
	LPSP_NODE		lpspNode;
	int				sx, sy;
	static bool     startpathbuildflag = false;

	while( startpathbuildflag == true );
	startpathbuildflag = true;
		
	sx = n->MoveSx;
	sy = n->MoveSy;
		
	if( CheckLineCollision( n, dx, dy, sx, sy ) )
	{	
		n->pathcount = TempPathCount;
		memcpy( n->path, TempPath, sizeof( int ) * MAX_SHORTPATH * 2 );
		startpathbuildflag = false;
		return TRUE;
	}	
		
	lpspNode = PathFind( n, sx, sy, dx, dy );
	if ( lpspNode == NULL )
	{
		PathDestroy( );
		startpathbuildflag = false;
		return	FALSE;
	}

	n->path[ 0 ][ 0 ] = dx;
	n->path[ 0 ][ 1 ] = dy;
	n->pathcount = 1;
	
	while ( lpspNode->lpParent != NULL )
	{
		if ( n->pathcount >= MAX_SHORTPATH )
		{
			break;
		}
	
		lpspNode = lpspNode->lpParent;
		n->path[ n->pathcount ][ 0 ] = lpspNode->x;
		n->path[ n->pathcount ][ 1 ] = lpspNode->y;
		n->pathcount++;
	}
	
	PathDestroy( );
	
	if ( n->pathcount > 1 )
	{
		startpathbuildflag = false;
		return	TRUE;
	}
	
	startpathbuildflag = false;
	return	FALSE;
}	
		
void
PathDestroy( void )
{	
	LPSP_NODE	lpspNode, lpspTemp;
	
	if ( GLPSP_OPEN != NULL && GLPSP_CLOSED != NULL && GLPSP_STACK != NULL )
	{
		lpspNode = GLPSP_OPEN->lpNext;
		while( lpspNode != NULL )
		{
			lpspTemp = lpspNode->lpNext;
			GlobalFree( lpspNode );
			lpspNode = lpspTemp;
		}
	
		lpspNode = GLPSP_CLOSED->lpNext;
		while( lpspNode != NULL )
		{
			lpspTemp = lpspNode->lpNext;
			GlobalFree( lpspNode );
			lpspNode = lpspTemp;
		}
	
		GlobalFree( GLPSP_OPEN );
		GlobalFree( GLPSP_CLOSED );
		GlobalFree( GLPSP_STACK );
		GLPSP_OPEN = NULL;
		GLPSP_CLOSED = NULL;
		GLPSP_STACK = NULL;
	}
}	
	
	
	
////////////////// 0730 yjs 수정 ////////////////////
////////////////// 축구공인 경우 나자신은 체크 안하기 위해 디폴트 인자값을 넣음....
int FreeTile( LPCHARLIST c, int sx, int sy, int x, int y, BOOL	notCheck_My, LPCHARLIST attacker )
///////////////////////////////////////////////////////////////////////
{	
	static bool startfreetileflag = false;
	int i;
	int xx,yy;

	startfreetileflag = true;

	if( x < 0 )						goto FAIL_;
	if( x >= g_Map.file.wWidth )	goto FAIL_;
	if( y < 0 )						goto FAIL_;
	if( y >= g_Map.file.wHeight)	goto FAIL_;
	if( sx < 0 )					goto FAIL_;
	if( sx >= g_Map.file.wWidth )	goto FAIL_;
	if( sy < 0 )					goto FAIL_;
	if( sy >= g_Map.file.wHeight)	goto FAIL_;

	if( TileMap[ x][ y].attr_dont )	goto FAIL_;
	
///////////////// 0730 yjs 수정 /////////////////////////////////////
///////////////// 여기서 나자신을 체크 안하기 위해 //////////////////
	if(notCheck_My && attacker)
		if( !(attacker->X/32 == x && attacker->Y/32 == y) )
			if( TileMap[ x][ y].occupied  )	goto FAIL_;
	else
		if( TileMap[ x][ y].occupied  )	goto FAIL_;
/////////////////////////////////////////////////////////////////////
		
	if( x > sx + 20 ) goto FAIL_;
	if( x < sx - 20 ) goto FAIL_;
	if( y < sy - 20 ) goto FAIL_;
	if( y > sy + 20 ) goto FAIL_; // 
	
	xx = x * TILE_SIZE;
	yy = y * TILE_SIZE;

	return 1;

	if( c )
	{
		for( i  = NPC_LIST_START ; i < MAX_NPC_LIST ; i ++ )
		{
			if( NPCList[i].bAlive == ALIVE_ && c->GetServerID() != i )
			{
				if( x == NPCList[i].MoveGox/ TILE_SIZE && y ==  NPCList[i].MoveGoy/ TILE_SIZE )
				{
					goto FAIL_; 
				}
				
				switch( NPCList[i].SprNo )
				{
				case 	30 :	if( BoxAndDotCrash( NPCList[i].MoveGox - 80, NPCList[i].MoveGox - 80, 160,160, xx, yy ) )goto FAIL_;
				case	 9 :
				case 	10 :
				case 	11 :
				case 	13 :
				case 	14 :
				case 	15 :
				case 	16 :
				case 	20 :
				case 	25 :
				case 	26 :
				case 	27 :
				
				case 	37 :
				case 	38 :// 돌골램..
				case 	40 :
				case 	41 :
				case 	42 :
				case 	45 :
				case 	49 :
				case 	59 :
				case 	61 :	if( BoxAndDotCrash( NPCList[i].MoveGox - 40, NPCList[i].MoveGox - 40, 80,80, xx, yy ) )goto FAIL_;
				}
			}
		}
		
		// 집안에 들어갈수 없다. 
		if( c->notcomeinside ) if( TileMap[ x][ y].attr_inside ) goto FAIL_;
	}		
			
	startfreetileflag = false; 
	return 1;
			
	int id;	
	for( i = 0 ; i < DoorNum ; i ++)
	{		
		id = Doors[i];
			
		if( ItemList[id].attr[IATTR_ATTR] & IA2_OPENED == 0 ) // 닽혔다. 
		{	
			ITEMLIST *ii = &ItemList[id];
			if( CheckIntersect( ii->dsx, ii->dsy, ii->ddx, ii->ddy, (sx<<5) + 16, (sy<<5) + 16, (x<<5) + 16, (y<<5) + 16 ) > 0)
			{
				goto FAIL_;
			}
		}	
	}		

	startfreetileflag = false;
	return 1;

FAIL_:

	startfreetileflag = false;
	return 0;
}			
	
	
	
LPSP_NODE
PathFind( CHARLIST *n, int sx, int sy, int dx, int dy )
{		
	LPMAPFILE	lpMapFile = &g_Map.file;
	LPSP_NODE	lpspNode, lpspTemp;
	int			nDestTile = dy * g_Map.file.wWidth + dx;
	int			x, y;
		
	GLPSP_OPEN = ( LPSP_NODE )GlobalAlloc( GPTR, sizeof( SP_NODE ) );
	GLPSP_CLOSED = ( LPSP_NODE )GlobalAlloc( GPTR, sizeof( SP_NODE ) );
	GLPSP_STACK = ( LPSP_STACK )GlobalAlloc( GPTR, sizeof( SP_STACK ) );
	lpspNode = ( LPSP_NODE )GlobalAlloc( GPTR, sizeof( SP_NODE ) );
	if ( GLPSP_OPEN == NULL || GLPSP_CLOSED == NULL || GLPSP_STACK == NULL || lpspNode == NULL )
	{	
		return	NULL;
	}	
		
	lpspNode->g = 0;
	lpspNode->h = ( sx - dx ) * ( sx - dx ) + ( sy - dy ) * ( sy - dy );
	lpspNode->f = lpspNode->g + lpspNode->h;
	lpspNode->number = sy * g_Map.file.wWidth + sx;
	lpspNode->x = sx;
	lpspNode->y = sy;
		
	int c = 0;
	GLPSP_OPEN->lpNext = lpspNode;
	while ( 1 )
	{	
		if ( GLPSP_OPEN->lpNext == NULL )
		{
			return	NULL;
		}
		c++;
		if( c > 100 ) return NULL;		// LTS AI OLD 100
		
		lpspTemp = GLPSP_OPEN->lpNext;
		GLPSP_OPEN->lpNext = lpspTemp->lpNext;
		lpspTemp->lpNext = GLPSP_CLOSED->lpNext;
		GLPSP_CLOSED->lpNext = lpspTemp;
		
		if ( lpspTemp->number == nDestTile )
		{
			break;
		}
		
		// upper
		x = lpspTemp->x, y = lpspTemp->y - 1;
		if ( FreeTile( n, sx, sy, x, y ))
		{
			GenerateSucc( lpspTemp, x, y, dx, dy );
		}
		
		// upper-right
		x = lpspTemp->x + 1, y = lpspTemp->y - 1;
		if ( FreeTile( n, sx, sy, x, y ))
		{
			GenerateSucc( lpspTemp, x, y, dx, dy );
		}

		// right
		x = lpspTemp->x + 1, y = lpspTemp->y;
		if ( FreeTile( n, sx, sy, x, y ))
		{
			GenerateSucc( lpspTemp, x, y, dx, dy );
		}

		// right-down
		x = lpspTemp->x + 1, y = lpspTemp->y + 1;
		if ( FreeTile( n, sx, sy, x, y ))
		{
			GenerateSucc( lpspTemp, x, y, dx, dy );
		}

		// down
		x = lpspTemp->x, y = lpspTemp->y + 1;
		if ( FreeTile( n, sx, sy, x, y ))
		{
			GenerateSucc( lpspTemp, x, y, dx, dy );
		}
	
		// left-down
		x = lpspTemp->x - 1, y = lpspTemp->y + 1;
		if ( FreeTile( n, sx, sy, x, y ))
		{
			GenerateSucc( lpspTemp, x, y, dx, dy );
		}
	
		// left
		x = lpspTemp->x - 1, y = lpspTemp->y;
		if ( FreeTile( n, sx, sy, x, y ))
		{
			GenerateSucc( lpspTemp, x, y, dx, dy );
		}
	
		// left-up
		x = lpspTemp->x - 1, y = lpspTemp->y - 1;
		if ( FreeTile( n, sx, sy, x, y ))
		{
			GenerateSucc( lpspTemp, x, y, dx, dy );
		}
	}

	return	lpspTemp;
}

void
GenerateSucc( LPSP_NODE lpspNode, int x, int y, int dx, int dy )
{
	int			g, nTile;
	int			i;
	LPSP_NODE	lpspOld, lpspSucc = 0;

	g = lpspNode->g + 1;
	nTile = y * g_Map.file.wWidth + x;

	if ( ( lpspOld = CheckOPEN( nTile ) ) != NULL )
	{
		for ( i = 0; i < 8; i++ )
		{
			if ( lpspNode->lpChild[ i ] == NULL )
			{
				break;
			}
		}

		lpspNode->lpChild[ i ] = lpspOld;
		if ( g < lpspOld->g )
		{
			lpspOld->lpParent = lpspNode;
			lpspOld->g = g;
			lpspOld->f = g + lpspOld->h;
		}
	}
	else if ( ( lpspOld = CheckCLOSED( nTile ) ) != NULL )
	{
		for ( i = 0; i < 8; i++ )
		{
			if ( lpspNode->lpChild[ i ] == NULL )
			{
				break;
			}
		}

		lpspNode->lpChild[ i ] = lpspOld;
		if ( g < lpspOld->g )
		{
			lpspOld->lpParent = lpspNode;
			lpspOld->g = g;
			lpspOld->f = g + lpspOld->h;

			PropagateDown( lpspOld );
		}
	}
	else
	{
		lpspSucc = ( LPSP_NODE )GlobalAlloc( GPTR, sizeof( SP_NODE ) );
		lpspSucc->lpParent = lpspNode;
		lpspSucc->g = g;
		lpspSucc->h = ( x - dx ) * ( x - dx ) + ( y - dy ) * ( y - dy );
		lpspSucc->f = g + lpspSucc->h;
		lpspSucc->x = x;
		lpspSucc->y = y;
		lpspSucc->number = nTile;

		Insert( lpspSucc );

		for( i = 0; i < 8; i++ )
		{
			if ( lpspNode->lpChild[ i ] == NULL )
			{
				break;
			}
		}

		lpspNode->lpChild[ i ] = lpspSucc;
	}
}

void
PropagateDown( LPSP_NODE lpspNode )
{
	int			i;
	int			g = lpspNode->g;
	LPSP_NODE	lpspChild, lpspParent;

	for ( i = 0; i < 8; i++ )
	{
		if ( ( lpspChild = lpspNode->lpChild[ i ] ) == NULL )
		{
			break;
		}

		if ( g + 1 < lpspChild->g )
		{
			lpspChild->g = g + 1;
			lpspChild->f = lpspChild->g + lpspChild->h;
			lpspChild->lpParent = lpspNode;
			Push( lpspChild );
		}
	}

	while ( GLPSP_STACK->lpNext != NULL )
	{
		lpspParent = Pop( );
		for ( i = 0; i < 8; i++ )
		{
			if ( ( lpspChild = lpspParent->lpChild[ i ] ) == NULL )
			{
				break;
			}

			if ( lpspParent->g + 1 < lpspChild->g )
			{
				lpspChild->g = lpspParent->g + 1;
				lpspChild->f = lpspChild->g + lpspChild->h;
				lpspChild->lpParent = lpspParent;
				Push( lpspChild );
			}
		}
	}
}

LPSP_NODE
CheckOPEN( int tilenum )
{
	LPSP_NODE	lpspNode;

	lpspNode = GLPSP_OPEN->lpNext;
	while ( lpspNode != NULL )
	{
		if ( lpspNode->number == tilenum )
		{
			return	lpspNode;
		}
		else
		{
			lpspNode = lpspNode->lpNext;
		}
	}

	return	NULL;
}

LPSP_NODE
CheckCLOSED( int tilenum )
{
	LPSP_NODE	lpspNode;

	lpspNode = GLPSP_CLOSED->lpNext;
	while ( lpspNode != NULL )
	{
		if ( lpspNode->number == tilenum )
		{
			return	lpspNode;
		}
		else
		{
			lpspNode = lpspNode->lpNext;
		}
	}

	return	NULL;
}

void
Insert( LPSP_NODE lpspNode )
{
	LPSP_NODE	lpspTmp1, lpspTmp2;
	int			f;

	if ( GLPSP_OPEN->lpNext == NULL )
	{
		GLPSP_OPEN->lpNext = lpspNode;
		return;
	}

	f = lpspNode->f;
	lpspTmp1 = GLPSP_OPEN;
	lpspTmp2 = GLPSP_OPEN->lpNext;
	while ( ( lpspTmp2 != NULL ) && ( lpspTmp2->f < f ) )
	{
		lpspTmp1 = lpspTmp2;
		lpspTmp2 = lpspTmp2->lpNext;
	}

	lpspNode->lpNext = lpspTmp2;
	lpspTmp1->lpNext = lpspNode;
}

void
Push( LPSP_NODE lpspNode )
{
	LPSP_STACK	lpspStack = 0;

	lpspStack = ( LPSP_STACK )GlobalAlloc( GPTR, sizeof( SP_STACK ) );
	lpspStack->lpNode = lpspNode;
	lpspStack->lpNext = GLPSP_STACK->lpNext;
	GLPSP_STACK->lpNext = lpspStack;
}

LPSP_NODE
Pop( void )
{
	LPSP_NODE	lpspNode;
	LPSP_STACK	lpspStack;

	lpspStack = GLPSP_STACK->lpNext;
	lpspNode = lpspStack->lpNode;
	GLPSP_STACK->lpNext = lpspStack->lpNext;
	GlobalFree( lpspStack );

	return	lpspNode;
}
