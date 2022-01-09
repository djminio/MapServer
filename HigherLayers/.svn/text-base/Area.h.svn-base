#ifndef _DRAGON_AREA_
#define _DRAGON_AREA_

#define ADD_AREA_X (18*32)
#define ADD_AREA_Y (14*32)

#define REM_AREA_X (21*32)
#define REM_AREA_Y (17*32)

//----------------------------------------------------------------------------------------------
#define MAX_CMD_AREA			1000	// 하나의 Area에 1000개까지 들어갈수 있다. 

#define	ADD_PC_AREA				1
#define	MOVE_PC_AREA			2									
#define	REMOVE_PC_AREA			3
#define	ADD_NPC_AREA			4
#define	MOVE_NPC_AREA			5
#define	REMOVE_NPC_AREA			6
#define	ADD_ITEM_AREA			7
#define	REMOVE_ITEM_AREA		8

#define CHANGE_IA2_DURATION_ITEM_AREA		 9
#define CHANGE_IA2_LIMIT_ITEM_AREA			10
#define CHANGE_IA2_ATTR_ITEM_AREA			11
#define CHANGE_IA2_ID_ITEM_AREA				12
#define CHANGE_IA2_RESERVED0_ITEM_AREA		13
#define CHANGE_IA2_RESERVED1_ITEM_AREA		14
#define ITEM_EXPLOTION_AREA					17

//----------------------------------------------------------------------------------------------
typedef struct defSortData
{
	int ID;
	int X;
	int Y;
} SORT_DATA;

//----------------------------------------------------------------------------------------------
typedef struct defCmdArea
{
	int	 Cmd;
	int  ID;
} CMD_AREA;

//----------------------------------------------------------------------------------------------
extern void CastMe2Other( int cn, t_packet *packet );
extern void CastNPC2Other( int npc, t_packet *packet );
extern int  SetArea(int Cmd, int Id);
extern void RunArea(t_connection c[]);
extern void SendModifyPositionNPC( t_connection c[], int npc );
extern void RecvNPCModifyPos( int cn, int id, int mx, int my );
	   
extern void SendMoveNPCArea(t_connection c[], int cn);
extern void SendNPCChatArea( int npc, char *String, ...  );
extern void SendChatArea(const int cn, t_packet *packet);//021126 lsw
extern void SendAttackNPCArea( t_connection c[], int npc );
extern void SendMagicNPCArea(t_connection c[], int npc, BYTE nMagic, bool bPattern = false);
extern void JustMessage( char *txt, ... );

extern int Zip(BYTE * pbIn, int iSize,BYTE *pbOut,int iMax);

extern int PC_COUNT;
extern int NPC_COUNT;
extern int ITEM_COUNT;
extern int CMD_AREA_COUNT;

#endif