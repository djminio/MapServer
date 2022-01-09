#if !defined(AFX_NPC_PATTERN_H__562FB52A_67CE_44B8_82E6_8CCA090DFDA2__INCLUDED_)
#define AFX_NPC_PATTERN_H__562FB52A_67CE_44B8_82E6_8CCA090DFDA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define NPC_PATTERN_NO_MOVE_			 0		// 가만히 있는다.
#define NPC_PATTERN_HOSTILE_			10		// 적대적으로 바뀐다. 
#define NPC_PATTERN_PEACE_				20		// 평화적으로 바뀐다.
#define NPC_PATTERN_FIND_CLOSE_PC_		30		// 가까운 pc를 찾는다.(그쪽을 바라본다. )
#define NPC_PATTERN_FIND_WEAK_PC_		40		// 약한  pc를 찾는다.(그쪽을 바라본다. )
#define NPC_PATTERN_FIND_STRONG_PC_		50		// 강한  pc를 찾는다.(그쪽을 바라본다. )
#define NPC_PATTERN_ACCESS_PC_			60		// 찾은 PC에 적근한다. ( Hostile일경우 공격범위에 있으면 공격한다. peace일경우 접근해도 가만히 접근하는놈을 바라본다.  
#define NPC_PATTERN_ACCESS_PC_ATTACKED_	70		// 나를 공격한 Pc에 접근한다. 
#define NPC_PATTERN_ATTACK_PC_			71
#define NPC_PATTERN_WANDER_RANGE_1_		80		// 배회한다. ( 1번 배회범위가 1이다. )
#define NPC_PATTERN_WANDER_RANGE_2_		90		// 배화한다. ( 1번 배회범위가 2이다. )
#define NPC_PATTERN_WANDER_RANGE_3_		100  	// 배화한다. ( 1번 배회범위가 3이다. )
#define NPC_PATTERN_ESCAPE_PC_			110		// 찾은 PC로 부터 도망간다. 
#define NPC_PATTERN_ESCAPE_BATTLE_PC_	120		// 근처에 공격 PC가 있으면 멀리 도망간다. 
#define NPC_PATTERN_RETURN_				130		// patterntype을 0으로 Setting한다.
#define NPC_PATTERN_BACKDRAW_			140		// 공격하려는 놈뒤로 3타일 빠진다.
#define NPC_PATTERN_IAM_BOSS_8_			150
#define NPC_PATTERN_TOBOSS_8_			160
#define NPC_PATTERN_WANDER_8_			170
#define NPC_PATTERN_MURI_8_				180
#define NPC_PATTERN_ACCESS_PC_8_		190
#define NPC_PATTERN_ATTACK_PC_8_		200
#define NPC_PATTERN_BACKDRAW_8_			210
#define NPC_PATTERN_IAM_BOSS_28_		220
#define NPC_PATTERN_TOBOSS_28_			230
#define NPC_PATTERN_WANDER_28_			240
#define NPC_PATTERN_MURI_28_			250
#define NPC_PATTERN_BACKDRAW_PC_28_		260
#define NPC_PATTERN_ACCESS_PC_38_		270
#define NPC_PATTERN_ATTACK_PC_38_		280
#define NPC_PATTERN_WANDER_38_			290
#define NPC_PATTERN_MURI_38_			300
#define NPC_PATTERN_BACKDRAW_PC_38_		310
#define NPC_PATTERN_IAM_BOSS_18_		320
#define NPC_PATTERN_TOBOSS_18_			330
#define NPC_PATTERN_WANDER_18_			340
#define NPC_PATTERN_MURI_18_			350
#define NPC_PATTERN_ACCESS_PC_18_		360
#define NPC_PATTERN_ATTACK_PC_18_		370
#define NPC_PATTERN_BACKDRAW_18_		380
#define	NPC_PATTERN_TAME_				390
#define NPC_PATTERN_IAM_BOSS_GUARD_		400
#define NPC_PATTERN_TOBOSS_GUARD_		410
#define NPC_PATTERN_WANDER_GUARD_		420
#define NPC_PATTERN_MURI_GUARD_			430
#define NPC_PATTERN_ACCESS_PC_GUARD_	440
#define NPC_PATTERN_ATTACK_PC_GUARD_	450
#define NPC_PATTERN_BACKDRAW_PC_GUARD_	460
#define NPC_PATTERN_EVENTING_MAUL_		470
#define NPC_PATTERN_WANDER_MAUL_		480
#define NPC_PATTERN_ATTACK_MAUL_		490
#define NPC_PATTERN_BACKDRAW_MAUL_		500
#define NPC_PATTERN_ACCESS_PC_MAUL_		510
#define NPC_PATTERN_SEALSTONE_NO_OPERATE_	520
#define NPC_PATTERN_SEALSTONE_ATTACK_		530
#define NPC_PATTERN_ACCESS_PC_48_		540			// LTS AI
#define NPC_PATTERN_ATTACK_PC_48_		550			// LTS AI
#define NPC_PATTERN_WANDER_48_			560			// LTS AI
#define NPC_PATTERN_MURI_48_			570			// LTS AI
#define NPC_PATTERN_BACKDRAW_PC_48_		580			// LTS AI

#define	NPC_PATTERN_WANDER				601
#define	NPC_PATTERN_ACCESS				602
#define	NPC_PATTERN_ATTACK				603
#define	NPC_PATTERN_ATTACKED			604
#define	NPC_PATTERN_MOVING				605			// LTS AI2

#define NPC_PATTERN_BOSS_WANDER			606			// LTS NEW AI
#define NPC_PATTERN_BOSS_ACCESS			607			// LTS NEW AI
#define NPC_PATTERN_BOSS_ATTACK			608			// LTS NEW AI
#define NPC_PATTERN_BOSS_ATTACKED		609			// LTS NEW AI
#define NPC_PATTERN_BOSS_MOVING			610			// LTS NEW AI
#define NPC_PATTERN_BOSS_BACKDRAW		611			// LTS NEW AI
#define NPC_PATTERN_BOSS_WAIT			612			// LTS NEW AI
#define NPC_PATTERN_BOSS_AVOID			613			// LTS NEW AI
// Client에서 변화된 NPC 파라메터를 Server로 보내준다. 
#define NPC_PARAMETER_TARGETID			0
#define NPC_PARAMETER_TARGETNAME		1
#define NPC_PARAMETER_BOSSID			2
#define NPC_PARAMETER_HOSTID			3
#define NPC_PARAMETER_TAME_CMD			4
#define NPC_PARAMETER_PATTERNTYPE		5
#define NPC_PARAMETER_OLDPATTERNTYPE	6

#define MAX_GENERATE_POSITION_			1000

// 010129 KHS
#define GT_SKB_FILE			0
#define GT_SCRIPT			1
#define GT_SKILL_SUMMON		2
#define	GT_EVENT_MONSTER	4 // LTS AI2
#define	GT_SCENARIO_MONSTER	5 // 020701 YGI
#define GT_EVENT_NPC		6 // 기간 이벤트를 통해 등장하는 NPC들	//021030 YGI
#define GT_HUNT_MONSTER		7 // CSD-030408 : hunt monster
#define GT_TREASUER_GUARD	8	// 보물 지킴이	// 040105 YGI 보물

// 원천적인 속성을 의미한다
#define	ELEMENT_FIRE					1
#define ELEMENT_WATER					2
#define ELEMENT_AIR						3
#define ELEMENT_ICE						4
#define ELEMENT_LIGHT					5
#define ELEMENT_EARTH					6

typedef struct tNPCgeneratePosition
{
	short int curnpc;
	short int npcindex;
	short int eventno;
	short int x, y;
	short int maxno;
}sNPCgeneratePosition;

extern int NpcPatternTable[20];
extern sNPCgeneratePosition NPCgeneratePosition[ MAX_GENERATE_POSITION_];
extern int NPCgeneratePositionMax;

extern bool CharMovingThreadStart0;
extern bool CharMovingThreadStart1;
extern bool CharMovingThreadStart2;

extern void InitMoveDelayTable( void );
extern DWORD Get_MoveDelayTime( CHARLIST *ch );
extern int InDistance( CHARLIST *s, CHARLIST *d, int dis );
extern int NPC_NearCh( CHARLIST *n, CHARLIST *ch, int *x, int *y );
extern int NPC_NearBackCh( CHARLIST *n, CHARLIST *ch, int *x, int *y, int dist );
extern int NPC_GetDir( int sx, int sy, int ex, int ey );
extern int NPC_MakePath( CHARLIST *n, int tx, int ty, int how );
extern int NPC_MakePathBumn( CHARLIST *n, int tx, int ty, int how );
extern int NPC_IsMoving( CHARLIST *n );
extern int NPC_IsWhoNearPC( CHARLIST *npc, int tilerange );
extern CHARLIST* NPC_ReturnCharListPoint( const int id );
extern CHARLIST* ReturnCharListPoint( char *name );
extern void RecvTameCommand( int cn, t_tame_command *p );
extern void SendTameResult( int cn, int result  );
extern void SendHostName( int cn, CHARLIST *n );
extern void SendToAllHostName( int cn, CHARLIST *n );
extern void CheckNPCTammingTimeElapsed( CHARLIST *n );

extern DWORD ReturnDestTime( LPCHARLIST ch );
extern void SaveMoveDelayTable( void );
extern void InitMoveDelayTable( void );

extern void NPCAddBasicData(  int i, t_server_npc_add *p );

extern void NPCGeneratePosition( int npcindex, int x, int y, int eventno, int maxno );
extern int  GetDeleteAbleNPC();//030211 lsw
extern int  NPC_Create( int Num, int npcindex, int x, int y, int eventno, int gen_pos, int gen_type = 0,int GroupNo=0,int isBoss=0);	// LTS AI2
extern int  NPC_AdjustPosition( int sprno, int *x, int *y );
extern void NPC_AutoCreationProc( void );
extern void NPC_AutoCreationProc2( void );				// LTS AI2
extern void NPC_CheckRemove( void );
extern void NPC_AutoCreate();							// LTS AI2
extern void RecvCMD_MODIFY_POSITION(  int cn,  t_client_modify_position *p );
extern void SendReadyMoveDate( int cn );
extern void RecvSmileFace( int cn, int smileno );

extern void SendNPCControlData( int cn, int npc );
extern int ManageNPC( int );

extern void SendNPC_Parameter( int id, int type, int data );
extern void SendNPC_StringParameter( int id, int type, char *data );

extern void RecvNPC_Parameter( int id, int type, int data );
extern void RecvNPC_StringParameter( int id, int type, char *data );

extern void Recv_JustAni( t_packet *p );
extern int initNPCGenerationTable(void);
extern void MovingCharacter();
extern void MovingNPC();
extern bool SetTileOccupied(short int &iX, short int &iY, const bool bOccupiedState);
extern void RecvCMD_MOVE(const int cn, t_packet *packet);
#endif //AFX_NPC_PATTERN_H__562FB52A_67CE_44B8_82E6_8CCA090DFDA2__INCLUDED_