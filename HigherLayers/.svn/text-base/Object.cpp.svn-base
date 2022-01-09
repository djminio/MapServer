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
#include "object.h"
#include "map.h"
#include "Citem.h"
#include "RajaSystem.h"



///////////////////////////////////////////////////////////////////////////////
//
MAPOBJECT			Mo[ MAX_MAPOBJECT_];
WORD				TotalMapObjectID;
WORD				TotalMapObject;
int					MapObjectLevel;
short int			Doors[ 500];
int					DoorNum;


///////////////////////////////////////////////////////////////////////////////
//


/////////////////////////////////////////////////////////////////////////////
// user functions
	
	
	
/* Origin을 잡을때, 

  기본의 Saf는 BMP에서 offsetx, offsety를 왼쪽위에서 읽은 image를 toi의 
  
	를 찍을때,,,,
*/


//---------------------------------------------------------------------------------
				
/////////////////////////////////////////////////////////////////////////////////////////////


//021030 YGI
void LoadItemDoorLine( MAPOBJECT *mo, int *dsx, int *dsy, int *ddx, int *ddy, char *filename )
{
	char temp[ FILENAME_MAX];
	int j;
	int obcount;
	int no;
	FILE *fp = 0;

	if( filename )
	{
		sprintf_s( temp, sizeof(temp), "%s/map/%s", GameServerDataPath, filename );
		fp = fopen( temp,"rb" );
	}
	if( !fp )
	{
		sprintf_s( temp, sizeof(temp), "%s/map/%s_toi2.b", GameServerDataPath, filename );
		fp = fopen( temp,"rb" );
	}
	
	if( fp != NULL )
	{									
		fread( &obcount, sizeof( int), 1, fp );
			
		for( j = 0 ; j < obcount ; j ++)
		{		
			fseek (fp, j * (sizeof( int ) * 20 + sizeof( short int ) * 4 * ITEM_FRAME_MAX_ ) + sizeof( int ), SEEK_SET );
			fread( &no, sizeof( int ), 1, fp );
			if( no == mo->id + 13000 )
			{	
				fseek( fp, 19 * sizeof( int ), SEEK_CUR );

				*dsx = 0;
				*dsy = 0;
				*ddx = 0;
				*ddy = 0;

				short int sx, sy, dx, dy;

				fread( &sx, sizeof( short),  1, fp );	fseek( fp, sizeof( short ) * (ITEM_FRAME_MAX_-1), SEEK_CUR );
				*dsx = sx;
				fread( &sy, sizeof( short),  1, fp );	fseek( fp, sizeof( short ) * (ITEM_FRAME_MAX_-1), SEEK_CUR );
				*dsy = sy;
				fread( &dx, sizeof( short),  1, fp );	fseek( fp, sizeof( short ) * (ITEM_FRAME_MAX_-1), SEEK_CUR );
				*ddx = dx;
				fread( &dy, sizeof( short),  1, fp );	fseek( fp, sizeof( short ) * (ITEM_FRAME_MAX_-1), SEEK_CUR );
				*ddy = dy;

				*dsx += mo->x ;
				*dsy += mo->y ;
				*ddx += mo->x ;
				*ddy += mo->y ;
				fclose(fp ); 
				return;
			}
		}	
				
		fclose(fp );
	}		
}			
			
// Table의 값을 검사한다. 			
void CheckItemsBox( void )
{
	int j,i;
	for( i = 0 ; i < MAX_TABLE_ITEMS_IN_BOX ; i ++)
	{
		for( j = 0 ; j < 40 ; j ++)
		{
			if( Itemsinbox[i].item[j] )
			{
				ItemAttr item= GenerateItem( Itemsinbox[i].item[j] );//020509 lsw
				AddItemList( item.item_no, item.attr, 0, 1000 + TILE_SIZE  * j, 1000 + TILE_SIZE * i, 0,0,0,0,0,0 );
			}
		}
	}
}
			
int InputItemInBox( void )
{			
	int r = rand()%100;
	int i;	
			
	for( i  = 1 ; i < ItemsinboxMax ; i ++)
	{		
		if( Itemsinbox[i].lev > r ) 
		{	
			return i;
		}	
	}		
	return 0;
}
	

/*
	ItemsInBox Itemsinbox[ MAX_TABLE_ITEMS_IN_BOX];
		
		int dum		= Itemsinbox[n].item[rand()%40];
		int m		= Itemsinbox[n].money[rand()%10];
		int q		= Itemsinbox[n].quantity;
		int	attrbroadcast = 0;
		
		i->dum[0] = 0;
		i->dum[1] = 0;
		for( int j = 0 ; j < q ; j ++)	i->dum[j] = dum;
		 i->money = m;



	int n = i->dumno;
		
	int dum		= Itemsinbox[n].item[rand()%40];
	int m		= Itemsinbox[n].money[rand()%10];
	int q		= Itemsinbox[n].quantity;
	int	attrbroadcast = 0;
		
*/


			
//021030 YGI
extern int LoadTOI( int type, char *filename1, char *filename2 );
#include "teambattle.h"
int ReloadTOI( char *filename1, char *filename2 )
{
	for( int i = 0; i < MAX_ITEM_LIST; i ++)
	{
		if( ItemList[i].bAlive == ALIVE_ && ItemList[i].bObject )
		{
			RemoveItemList( i );
		}
	}
	return LoadTOI( 1, filename1, filename2 );
}

int LoadTOI( char *filename )
{
	return LoadTOI( 0, filename, filename );
}
int LoadTOI( int type, char *filename1, char *filename2 )
{
	FILE *fp = 0;
	char szFilePath[ FILENAME_MAX];
	WORD i;		
	int  count = 0;
	MAPOBJECT mo;
	MAPOBJECT_old moold;
	int dsx,dsy, ddx, ddy;
	int id;
	bool newtoi;
		
	
	DoorNum = 0;

	if( type )
	{
		sprintf_s( szFilePath, sizeof(szFilePath), "%s/Map/%s", GameServerDataPath, filename1 );
		fp = fopen( szFilePath, "rb" );
	}
	if( !fp )
	{
		sprintf_s( szFilePath, sizeof(szFilePath), "%s/Map/%s.toi2", GameServerDataPath, filename1 );
		fp = fopen( szFilePath, "rb" );
	}
	if( fp )	
	{	
		fread ( &TotalMapObject, 2,1, fp );
		
		if( TotalMapObject == 0xffff ) { newtoi = true;	fread ( &TotalMapObject, 2,1, fp ); }
		else newtoi = false;
		
		fread ( &TotalMapObjectID, 2,1, fp );
			
		fseek( fp, TotalMapObject * sizeof( int ), SEEK_CUR );
		//fread ( TempMoImageID, TotalMapObject, sizeof( int ), fp );
									
		for( i = 0 ; i < TotalMapObject ; i ++)
		{			
			if( newtoi == true )
			{
				fread( &mo, sizeof( MAPOBJECT ), 1, fp );
			}
			else
			{
				fread( &moold, sizeof( MAPOBJECT_old ), 1, fp );
				memcpy( &mo, &moold, sizeof( MAPOBJECT_old ) );
				mo.soundno		= 0;
				mo.soundframe	= 0;
				mo.sounddelay   = 1;
			}
					
			// ITEMTYPE_DIRECTBOARD, ITEMTYPE_BOARD..
			switch( mo.objectoritem )
			{					
											
				// 이정표 간판.					
			case 5 :						
			case 6 :							
												
				//	기술용 작업대..				
			case 71 :						
			case 72 :				
			case 73 :						
			case 74 :							
			case 75 :						
			case 76 :						
			case 77 :						
			case 78 :							
											
			case 81	:					
			case 82	:								
			case 83	:						
			case 84	:							
			case 85	:					
			case 86	:							
				continue;							
												
			case 79 :						
			case 80 :						
				break;								
			}								
										
			if( mo.objectoritem )
			{	
				ItemAttr I;
				
				dsx = dsy = ddx = ddy = 0;
				
				I.item_no = mo.id + 13000;
				I.attr[0] =  1;
				I.attr[1] =  1;
				I.attr[IATTR_ATTR] =0;
				I.attr[3] =  1;
				I.attr[4] =  1;
				I.attr[5] =  1;
							
				switch( mo.objectoritem )
				{			
				case ITEMTYPE_NORMAL	:
				case ITEMTYPE_CHAIR		:	
				case ITEMTYPE_TABLE		:	I.attr[IATTR_ATTR] =   0;	break;
							
				case ITEMTYPE_DOOR		:	I.attr[IATTR_ATTR] =  IA2_NOTMOVE | IA2_DOOR;	
											LoadItemDoorLine( &mo, &dsx, &dsy, &ddx, &ddy, filename2 );break;
						
				case ITEMTYPE_BOX		:	I.attr[IATTR_ATTR] =  IA2_NOTMOVE | IA2_BOX	;	
											mo.dum = InputItemInBox();
											break;
				case ITEMTYPE_COLOSSUS_STONE	:
					{
						I.attr[IATTR_ATTR] = IA2_COLOSSUS_STONE;
						static int count = 0;
						mo.dum = count;
						count++;
						break;
					}

				case ITEMTYPE_SEAL_SPC  :
				case ITEMTYPE_SEAL_NOR	:	I.attr[IATTR_ATTR]				=  IA2_SEAL_NOR | IA2_NOTMOVE;	
											I.attr[ IATTR_SEAL_STONE_NO]	=  mo.dum;						
											I.attr[IATTR_DURATION]			=  800000;						
											break;															
				}			
							
				id = AddItemList( I.item_no, I.attr, mo.dum, mo.x, mo.y, mo.offx, mo.offy, dsx, dsy, ddx, ddy  );
				if( id > 0) 
					ItemList[id].bObject = 1;

				count ++;
				
				if( mo.objectoritem == ITEMTYPE_DOOR )
				{
					Doors[ DoorNum] = id;
					DoorNum++;
				}
				else if( mo.objectoritem == ITEMTYPE_COLOSSUS_STONE )
				{
					g_ColossusStone.AddStone( id );
				}

				

///////////////////////////////////////////////////


///////////////////////////////////////////////////



			}
		}
		fclose(fp);

		return 1;
	}			
				
	return 1;	
}				
