/*

*/

#ifndef	__OBJECT_H__
#define	__OBJECT_H__



///////////////////////////////////////////////////////////////////////////////
//
//

#define MAX_MAPOBJECTIMAGE			1000
#define	MAX_OBJECTIMAGE				1000
#define ITEM_FRAME_MAX_				10

#define	OBJECTATTR_BACK_ABOVE		1
#define	OBJECTATTR_BACK_BELOW		2
#define	OBJECTATTR_FRONT_ABOVE		3
#define	OBJECTATTR_FRONT_BELOW		4
#define	OBJECTATTR_ROOF_ABOVE		5
#define	OBJECTATTR_ROOF_BELOW		6

#define	OBJECTDIRECTION_RIGHTUP		1
#define OBJECTDIRECTION_RIGHTDOWN	2
#define	OBJECTDIRECTION_LEFTDOWN	3
#define	OBJECTDIRECTION_LEFTUP		4

#define	OBJECTANIMATION_STOP		1
#define	OBJECTANIMATION_LOOP		2
#define	OBJECTANIMATION_PLAY		3


#define OBJECTANI_NO				1
#define OBJECTANI_RETURNLOOP		2	
#define OBJECTANI_LOOP				3
#define OBJECTANI_1_RETURNLOOP		4
#define OBJECTANI_1_LOOP			5
#define OBJECTANI_TOGGLE_ANI		6


#define OBJECTSPRITE_NORMAL			0
#define OBJECTSPRITE_TRANS			1
#define OBJECTSPRITE_ALPHA     		2


#define MAX_MAPOBJECT_		5000






		
typedef struct {	
	WORD  id;
	BYTE  status;
	WORD  objectoritem;
	char  curframe;
	WORD  dum;
	BYTE  animationType;
	int   x, y;
	BYTE  attr;
	BYTE  attr1;
	BYTE  attr2;
	WORD  dir;
	int   offx, offy;
	short dx0[ITEM_FRAME_MAX_], dy0[ITEM_FRAME_MAX_];
	short dx1[ITEM_FRAME_MAX_], dy1[ITEM_FRAME_MAX_];
	BYTE  light;
	short lightx, lighty;
	char  delay;
	char  anidir;
}MAPOBJECT_old, *LPMAPOBJECT_old;

typedef struct {	
	WORD  id;
	BYTE  status;
	WORD  objectoritem;
	char  curframe;
	WORD  dum;
	BYTE  animationType;
	int   x, y;
	BYTE  attr;
	BYTE  attr1;
	BYTE  attr2;
	WORD  dir;
	int   offx, offy;
	short dx0[ITEM_FRAME_MAX_], dy0[ITEM_FRAME_MAX_];
	short dx1[ITEM_FRAME_MAX_], dy1[ITEM_FRAME_MAX_];
	BYTE  light;
	short lightx, lighty;
	char  delay;
	char  anidir;

	short soundno;
	short soundframe;
	short sounddelay;

}MAPOBJECT, *LPMAPOBJECT;

		
		
extern WORD			TotalMapObject;
extern MAPOBJECT	Mo[ MAX_MAPOBJECT_];
extern int			MapObjectLevel;
extern short int	Doors[ 500];
extern int			DoorNum;
		
///////////////////////////////////////////////////////////////////////////////
//		
//		
		
#ifdef __cplusplus
extern "C"
{		
#endif	
		
extern int  SaveTOI( char *filename );
extern int  SaveTOI2( char *filename );
extern int	LoadTOI( char *filename );
extern void FreeTOI( void );


#ifdef __cplusplus
}	
#endif
	
#endif	// __OBJECT_H__
	