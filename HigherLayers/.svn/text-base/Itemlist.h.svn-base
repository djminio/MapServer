#ifndef _DRAGON_ITEMLIST_
#define _DRAGON_ITEMLIST_

#define MAX_ITEM_LIST 10000

#define ITEM_JOIN_MAX 1000


#define ITEM_FARM_GROW_TIME		   5
#define ITEM_FIRE_GROW_TIME		 300
#define ITEM_BOX_REFRESH_TIME   (3600*2)

typedef struct tagITEMLIST
{ 
	BOOL bAlive;

	short			item_no;
	unsigned int	attr[6];
	short int		x, y;
	short int		offx, offy;
	short int		dumno;

	short int		money;
	short int		dum[2];

	short int		dsx, dsy, ddx,ddy;

	DWORD			birthday; // Item이 땅에 떨어진 시기...
	char			bObject;		// toi 로 생성된 아이템		//021030 YGI

} ITEMLIST, *LPITEMLIST;


#define MAX_TABLE_ITEMS_IN_BOX					35



typedef struct tagItemsInBox{
		short int	item[ 40];
		short int	money[ 10];
		BYTE		quantity;
		BYTE		lev;
		int			Village;
		int			Out_village1;
		int			Out_village2; 
		int			Out_village3; 
		int			Castle_out;   
		int			Castle_in;	   
		int			Fire_Dungeon; 
		int			Ice_Dungeon;  
		int			Human_Dungeon;
}ItemsInBox;







extern ITEMLIST ItemList[ MAX_ITEM_LIST];
extern ItemsInBox Itemsinbox[ MAX_TABLE_ITEMS_IN_BOX];
extern int		  ItemsinboxMax;

extern void InitItemList(void);
extern int	GetAliveItemList(void);
extern int  AddItemList(int itemno, unsigned int attr[6], int dum, int x, int y, int offx, int offy, int dsx = 0, int dsy = 0, int ddx = 0, int ddy = 0);
extern void RemoveItemList(int Num);

extern int LoadItemsInBoxTableSQL( void );
extern void ProcessItemsInBox( int boxitemid );
extern void SendItemExplotion( int cn, int id );
extern void ProcessItemsInBox( int boxitemid );


#define GetAttr2( attr2, a )		((attr2) & (a))
#define SetAttr2( attr2, b )		{(attr2) |=(b);}

// 020620 YGI
extern int CreateMapObject( int object_no, DWORD attr, int x, int y );





#endif
