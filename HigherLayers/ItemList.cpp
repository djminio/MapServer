#include "..\stdafx.h"
#include "DefaultHeader.h"
#include "CItem.h"


#include "../LowerLayers/MyLog.h"




extern HDBC hDBC;
extern void rollDice(int, int &) ;



ITEMLIST ItemList[MAX_ITEM_LIST];
ItemsInBox Itemsinbox[ MAX_TABLE_ITEMS_IN_BOX];
int		   ItemsinboxMax;

typedef struct t_StoreItemList
{	int no;
	ItemAttr item[ 100];
	ItemAttr unique[10];
}STOREITEMLIST, *LPSTOREITEMLIST;

#define MAX_STORE 10
STOREITEMLIST  StoreItemList[ MAX_STORE];



//--------------------------------------------------------
void InitItemList(void)
{
	int i;

	  for(i = 0; i < MAX_ITEM_LIST; i ++)
			ItemList[i].bAlive = REMOVE_;
};

//--------------------------------------------------------
int  GetAliveItemList(void)
{
	int i;

	  for(i = 0; i < MAX_ITEM_LIST; i ++)
		{
			if(ItemList[i].bAlive == FALSE)
				return(i);
		}

		return(-1);
};











//--------------------------------------------------------
int  AddItemList(int item_no, unsigned int attr[6], int dumno, int x, int y, int offx, int offy,  int dsx, int dsy, int ddx, int ddy )
{	
	int Num;
	
	if( item_no == 0 ) return -1;
	
	Num = GetAliveItemList();
	if(Num < 0)	return(-1);
	
	if( ( item_no == IN_COINS ||  item_no == IN_COIN ) && attr[ IATTR_MUCH] ==0 ) return -1; 
	if( ( item_no == IN_NEW_COINS ||  item_no == IN_NEW_COIN ) && attr[ IATTR_MUCH] ==0 ) return -1;		// 010210 YGI
	
	ITEMLIST *i = &ItemList[Num];
		
	i->item_no		= item_no;
	i->attr[0]		= attr[0];
	i->attr[1]		= attr[1];
	i->attr[2]		= attr[2];
	i->attr[3]		= attr[3];
	i->attr[4]		= attr[4];
	i->attr[5]		= attr[5];
	i->x			= x;							
	i->y			= y;						
	i->offx			= offx;						
	i->offy			= offy;								
	i->bAlive		= ALIVE_;								
	i->dumno		= dumno;							
	
	i->dsx			= dsx;								
	i->dsy			= dsy;						
	i->ddx			= ddx;											
	i->ddy			= ddy;						

	i->birthday		= g_curr_time;
	i->bObject		= 0;	//021030 YGI
	
	// Box일경우 박스를 부실때, 생성될 Item과 돈을 넣어둔다.
	if( i->attr[ IATTR_ATTR] & IA2_BOX )									
	{
		int n = i->dumno;
											
		int dum		= Itemsinbox[n].item[rand()%40];
		int m		= Itemsinbox[n].money[rand()%10];
		int q		= Itemsinbox[n].quantity;
		int	attrbroadcast = 0;
		
		i->dum[0] = 0;
		i->dum[1] = 0;
		for( int j = 0 ; j < q ; j ++)	i->dum[j] = dum;							
		 i->money = m;								
	}
	
	SetArea( ADD_ITEM_AREA, Num);

	return(Num);
};		
		
//--------------------------------------------------------
void RemoveItemList(int Num)
{		
		if( Num < 0 ) return;
		if( Num >= MAX_ITEM_LIST ) return;
	
		ItemList[Num].bAlive = FALSE;
		SetArea(REMOVE_ITEM_AREA, Num);
};

//--------------------------------------------------------


int LoadItemsInBoxTableSQL( void )
{
	HSTMT hStmt= NULL ;
	RETCODE ret ;
	SWORD nCols ;
	SDWORD cbValue ;
	int c= 1, i ;
	int n;

	SQLAllocStmt(hDBC, &hStmt);
	ret= SQLExecDirect(hStmt, (UCHAR *)"select * from ItemsBox_new order by NO", SQL_NTS) ;

	if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) 
	{
		MyLog( LOG_NORMAL, "'ItemsBox_new': ExecDirect Error " );
		return -1 ;
	}

	SQLNumResultCols(hStmt, &nCols) ;
	
	ret= SQLFetch(hStmt) ;
	if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) 
	{
		MyLog( LOG_NORMAL, "'ItemsBox_new': Fetch Error ") ;
		return -1 ;
	}
		
	int temp;
	int j;
	for( n = 0, i = 0 ; i < MAX_TABLE_ITEMS_IN_BOX ; i ++)
	{
		// Accquire the DATA
		c = 1;
		ret= SQLGetData(hStmt, c++, SQL_C_SLONG, &temp, sizeof(int), &cbValue) ;
		if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) 	goto ERROR_;

		for( j = 0 ; j < 40 ; j ++)
		{
			ret= SQLGetData(hStmt, c++, SQL_C_SLONG, &temp, sizeof(int), &cbValue) ;
			if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) 	goto ERROR_;
			if( temp >= 100 ) temp /= 100;
			Itemsinbox[n].item[j] = temp;
		}

		for( j = 0 ; j < 10 ; j ++) // money;
		{
			ret= SQLGetData(hStmt, c++, SQL_C_SLONG, &temp, sizeof(int), &cbValue) ;
			if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) 	goto ERROR_;
			Itemsinbox[n].money[j] = temp;
		}

		ret= SQLGetData(hStmt, c++, SQL_C_SLONG, &temp, sizeof(int), &cbValue) ;
		if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) 	goto ERROR_;
		Itemsinbox[n].quantity = temp;

		ret= SQLGetData(hStmt, c++, SQL_C_SLONG, &temp, sizeof(int), &cbValue) ;
		if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) 	goto ERROR_;
		Itemsinbox[n].lev = temp;

		ret= SQLGetData(hStmt, c++, SQL_C_SLONG, &temp, sizeof(int), &cbValue) ;
		if(ret == SQL_SUCCESS_WITH_INFO || ret == SQL_SUCCESS) 	Itemsinbox[n].Village = temp;
		ret= SQLGetData(hStmt, c++, SQL_C_SLONG, &temp, sizeof(int), &cbValue) ;
		if(ret == SQL_SUCCESS_WITH_INFO || ret == SQL_SUCCESS) 	Itemsinbox[n].Out_village1 = temp;
		ret= SQLGetData(hStmt, c++, SQL_C_SLONG, &temp, sizeof(int), &cbValue) ;
		if(ret == SQL_SUCCESS_WITH_INFO || ret == SQL_SUCCESS) 	Itemsinbox[n].Out_village2 = temp;
		ret= SQLGetData(hStmt, c++, SQL_C_SLONG, &temp, sizeof(int), &cbValue) ;
		if(ret == SQL_SUCCESS_WITH_INFO || ret == SQL_SUCCESS) 	Itemsinbox[n].Out_village3 = temp;
		ret= SQLGetData(hStmt, c++, SQL_C_SLONG, &temp, sizeof(int), &cbValue) ;
		if(ret == SQL_SUCCESS_WITH_INFO || ret == SQL_SUCCESS) 	Itemsinbox[n].Castle_out   = temp;
		ret= SQLGetData(hStmt, c++, SQL_C_SLONG, &temp, sizeof(int), &cbValue) ;
		if(ret == SQL_SUCCESS_WITH_INFO || ret == SQL_SUCCESS) 	Itemsinbox[n].Castle_in	   = temp;
		ret= SQLGetData(hStmt, c++, SQL_C_SLONG, &temp, sizeof(int), &cbValue) ;
		if(ret == SQL_SUCCESS_WITH_INFO || ret == SQL_SUCCESS) 	Itemsinbox[n].Fire_Dungeon = temp;
		ret= SQLGetData(hStmt, c++, SQL_C_SLONG, &temp, sizeof(int), &cbValue) ;
		if(ret == SQL_SUCCESS_WITH_INFO || ret == SQL_SUCCESS) 	Itemsinbox[n].Ice_Dungeon  = temp;
		ret= SQLGetData(hStmt, c++, SQL_C_SLONG, &temp, sizeof(int), &cbValue) ;
		if(ret == SQL_SUCCESS_WITH_INFO || ret == SQL_SUCCESS) 	Itemsinbox[n].Human_Dungeon= temp;
		ret= SQLFetch(hStmt) ;			


		switch( MapNumber )
		{
			//MA-IN
			case 0 :		if( Itemsinbox[n].Village )			n++; break;
			//K_SUNG2
			case 1 :		if( Itemsinbox[n].Castle_out )		n++; break;
			//FIREDUN1
			case 2 :		if( Itemsinbox[n].Fire_Dungeon )	n++; break;
			//ICE-W01	
			case 3 :		if( Itemsinbox[n].Ice_Dungeon )		n++; break;
			//SOURCE		
			case 4 :		if( Itemsinbox[n].Out_village1 )	n++; break;
			//MANDUN
			case 5 :		if( Itemsinbox[n].Human_Dungeon )	n++; break;
			//SUNG_TILE_00		
			case 6 :		if( Itemsinbox[n].Castle_in )		n++; break;
		}
	}

	SQLFreeStmt(hStmt, SQL_DROP) ;
	ItemsinboxMax = n;
	return n ;

ERROR_:

	MyLog( LOG_NORMAL, "'ItemsBox_new': Error( ItemsInBox ) : %d  \n", c );

	return 0;
}




// 현재 사용 안함.
void SendItemExplotion( int cn, int id ) 
{	
	t_packet p;
	int eff;
	int tx = ItemList[id].x;
	int ty = ItemList[id].y;
	
	if( ItemList[id].item_no % 2 ) eff = 208;
	else  eff = 257;
		
	p.h.header.type = CMD_INSERT_MAGIC;
	{	
		p.u.server_insert_magic.item_id		= id;
		p.u.server_insert_magic.effectno	= eff;
		p.u.server_insert_magic.tx			= tx;
		p.u.server_insert_magic.ty			= ty;
	}	
	p.h.header.size = sizeof( t_server_insert_magic );
	
	QueuePacket( connections, cn, &p, 1 );
}	
	
	

	
// Box을 부수고 난뒤 Item을 발생시키거나 폭파시키는 처리를 한다. 
/*void ProcessItemsInBox( int boxitemid )
{		
	if( boxitemid < 0  || boxitemid >= MAX_ITEM_LIST ) return;
		
	ITEMLIST *i = &ItemList[ boxitemid];
	if( i->bAlive != ALIVE_ ) return;
	int n = i->dum;
	if( n < 0 || n >= MAX_TABLE_ITEMS_IN_BOX ) return;
		
	if( !(i->attr[IATTR_ATTR] & IA2_BOX ) ) return; // Box가 아니면....
		
	ItemAttr	item;
	int itemno	= Itemsinbox[n].item[rand()%40];
	int m		= Itemsinbox[n].money[rand()%10];
	int q		= Itemsinbox[n].quantity;
	int	attrbroadcast = 0;
		
	//	아이템을 발생시킨다. 
	if( itemno != -1 )
	{	
		for( int j  = 0 ; j < q ; j ++)
		{
			item = GenerateItem( itemno/ 1000, itemno%1000, 0, 100 );
			//int  AddItemList(int item_no, unsigned int attr[6], int dum, int x, int y, int offx, int offy )
			AddItemList( item.item_no, item.attr, 0, i->x + 30 + (rand()%32),  i->x + 30 + (rand()%32), 0,0 );
		
			i->attr[ IATTR_ATTR] |= IA2_OPENED;
			attrbroadcast = 1;
		}
	}	
	else
	{	
		SetArea(	ITEM_EXPLOTION_AREA,	boxitemid );
		SetArea(	REMOVE_ITEM_AREA,		boxitemid );
		// 폭파...
		return;
	}	
		
	//	돈을 발생시킨다. 
	if( m > 0 ) 
	{	
		item = GenerateItem( itemno/ 1000, itemno%1000, m, 0, 100 );
		AddItemList( item.item_no, item.attr, 0, i->x + 30 + (rand()%32),  i->x + 30 + (rand()%32), 0,0 );
		i->attr[ IATTR_ATTR] |= IA2_OPENED;
		attrbroadcast = 1;
	}	
		
	if( attrbroadcast)
	{	
		SetArea( CHANGE_IA2_ATTR_ITEM_AREA,	boxitemid );
	}	
}		*/


// 화살로 BoxItem을 쏘는 부분때문에 나온 Packet... 0209 KHS
void RecvCMD_JUST_ATTACK_ANIMATION( int cn, t_client_just_attack_animation *p )
{
	t_packet packet;
	int boxitemid = p->item_id;
	
	if( boxitemid < 0 || boxitemid >= MAX_ITEM_LIST ) return;
	if( ItemList[boxitemid].attr[ IATTR_ATTR] & IA2_OPENED ) return; // 열려 있으면 리턴.


	packet.h.header.type = CMD_ITEM_BOX_BREAK;
	{
		packet.u.server_item_box_break.server_id = cn;
		packet.u.server_item_box_break.item_id = boxitemid;
		packet.u.server_item_box_break.type = 0;
	}
	packet.h.header.size = sizeof( t_server_item_box_break );

	QueuePacket( connections, cn, &packet, 1 );
	CastMe2Other( cn, &packet );
}





void RecvCMD_ITEM_BOX_BREAK( int cn, t_client_item_box_break *p )
{
	t_packet packet;
	int type;
	int boxitemid = p->item_id;

	
	if( boxitemid < 0 || boxitemid >= MAX_ITEM_LIST ) return;
	if( ItemList[boxitemid].attr[ IATTR_ATTR] & IA2_OPENED ) return; // 열려 있으면 리턴.

	if( ItemList[boxitemid].dum[0] == -1 || 
		ItemList[boxitemid].dum[1] == -1 ||
		ItemList[boxitemid].money  == -1  ) type = 0;
	else type = 1;



	ItemList[boxitemid].attr[ IATTR_ATTR] |= IA2_OPENED;
	ItemList[boxitemid].attr[ IATTR_LIMIT] = g_curr_time + ITEM_BOX_REFRESH_TIME + ( rand() % ITEM_BOX_REFRESH_TIME ); //ran( 100 );

	packet.h.header.type = CMD_ITEM_BOX_BREAK;
	{
		packet.u.server_item_box_break.server_id = cn;
		packet.u.server_item_box_break.item_id = boxitemid;
		packet.u.server_item_box_break.type = type;
	}
	packet.h.header.size = sizeof( t_server_item_box_break );

	QueuePacket( connections, cn, &packet, 1 );
	CastMe2Other( cn, &packet );
}


void RecvCMD_ITEM_BOX_BREAK_RESULT( int cn, t_client_item_box_break_result *p )
{
	int boxitemid = p->item_id;
	ITEMLIST *i;

	if( boxitemid < 0 || boxitemid >= MAX_ITEM_LIST ) return;
	
	i = &ItemList[boxitemid];

	if( !(i->attr[ IATTR_ATTR] & IA2_OPENED) ) return; // 열려 있으면 리턴.
		
	int attrbroadcast = 0;
	ItemAttr item;
	if( i->dum[0] > 0 )
	{	
		item = GenerateItem( i->dum[0] );//020509 lsw
		AddItemList( item.item_no, item.attr, 0, i->x + 30 + (rand()%32),  i->y + 30 + (rand()%32), 0,0 );
	}	
	if( i->dum[1] > 0 )
	{	
		item = GenerateItem( i->dum[1] );//020509 lsw
		AddItemList( item.item_no, item.attr, 0, i->x + 30 + (rand()%32),  i->y + 30 + (rand()%32), 0,0 );
	}	
		
	//	돈을 발생시킨다. 
	if( i->money > 0 ) 
	{	
		if(LocalMgr.IsChangeMoney())//030102 lsw
		{
			if( i->money <= 5 ) 
				item = GenerateItem( IN_NEW_COIN, IATTR_MUCH, i->money);//020509 lsw
			else 
				item = GenerateItem( IN_NEW_COINS, IATTR_MUCH,	i->money);//020509 lsw
		}
		else
		{
			if( i->money <= 5 ) 
				item = GenerateItem( IN_COIN, IATTR_MUCH, i->money);//020509 lsw
			else 
				item = GenerateItem( IN_COINS, IATTR_MUCH, i->money);//020509 lsw
		}

		AddItemList( item.item_no, item.attr, 0, i->x + 30 + (rand()%32),  i->y + 30 + (rand()%32), 0,0 );
	}	
		
	SetArea( CHANGE_IA2_ATTR_ITEM_AREA,	boxitemid );
}

void RecvCMD_ITEM_BOX_MAGIC_BREAK( int cn, t_client_item_box_magic_break	*p )
{
	t_packet packet;
	int type;
	int boxitemid = p->item_id;
	ITEMLIST *i;
	
	if( boxitemid < 0 || boxitemid >= MAX_ITEM_LIST ) return;
	if( ItemList[boxitemid].attr[ IATTR_ATTR] & IA2_OPENED ) return; // 열려 있으면 리턴.
	i = &ItemList[boxitemid];

	if( i->dum[0] == -1 || i->dum[1] == -1 ||i->money  == -1  ) type = 0;	// 폭파시킨다. 
	else type = 1;	// 돈이나 아이템을 준다. 

	ItemList[boxitemid].attr[ IATTR_ATTR] |= IA2_OPENED;
	ItemList[boxitemid].attr[ IATTR_LIMIT] = g_curr_time + 300 + (rand()%100);

	if( type == 0 )
	{
		packet.h.header.type = CMD_ITEM_BOX_MAGIC_BREAK_RESULT;
		{
			packet.u.server_item_box_magic_break_result.item_id = boxitemid;
			packet.u.server_item_box_magic_break_result.type	= type;
		}
		packet.h.header.size = sizeof( t_server_item_box_magic_break_result );
		QueuePacket( connections, cn, &packet, 1 );
		CastMe2Other( cn, &packet );
	}
		
	int attrbroadcast = 0;

	ItemAttr item;
	if( i->dum[0] > 0 )
	{	
		item = GenerateItem( i->dum[0]);//020509 lsw
		AddItemList( item.item_no, item.attr, 0, i->x + 30 + (rand()%32),  i->y + 30 + (rand()%32), 0,0 );
	}	
	if( i->dum[1] > 0 )
	{	
		item = GenerateItem( i->dum[1]);//020509 lsw
		AddItemList( item.item_no, item.attr, 0, i->x + 30 + (rand()%32),  i->y + 30 + (rand()%32), 0,0 );
	}	
		
	//	돈을 발생시킨다. 
	if( i->money > 0 ) 
	{	
		if(LocalMgr.IsChangeMoney())//030102 lsw
		{
			if( i->money > 5 )	item = GenerateItem( IN_NEW_COINS, IATTR_MUCH, i->money);//020509 lsw
			else				item = GenerateItem( IN_NEW_COIN, IATTR_MUCH, i->money);//020509 lsw
		}
		else 
		{
			if( i->money > 5 )	item = GenerateItem( IN_COINS, IATTR_MUCH, i->money);//020509 lsw
			else				item = GenerateItem( IN_COIN, IATTR_MUCH, i->money);//020509 lsw
		}

		AddItemList( item.item_no, item.attr, 0, i->x + 30 + (rand()%32),  i->y + 30 + (rand()%32), 0,0 );
	}	
		
	SetArea( CHANGE_IA2_ATTR_ITEM_AREA,	boxitemid );
}
