
#define IT_NORMAL_VERSION	0
#define IT_LIGHT_VERSION	1

extern bool g_block_attack;
extern bool g_block_move;
extern bool g_block_magic;
extern bool g_stop_please_give_life;

extern bool g_accelator_user_closeconnection;

extern char	GameServerDataPath[ FILENAME_MAX];

// 030919 HK YGI
extern char NetworkDir[MAX_PATH];
extern char MapServerConfigFileName[MAX_PATH];


extern int InitGameMakeModeSetting( char *filename );

extern void SaveGameServerStatus( void );
extern void LoadGameServerStatus( void );
extern void CharacterAutoUpdate( void );
extern void SendHowManyInMap( short how );

//acer7-------
extern void RecvBlockAttack( short int cn, bool flag );
extern void RecvBlockMove( short int cn,bool flag );
extern void RecvBlockMagic( short int cn,bool flag );
extern void RecvBlockSkill( short int cn, bool flag );//020821 lsw
extern void RecvBlockGiveLife( short int cn,bool flag );
extern void RecvAbsoluteLogOut(short,  short id );
extern void RecvMakeGhost( short, short id );
extern void RecvSummonMonster(short int cn, short id, short amount, short x, short y, bool spread, bool summon);
extern void RecvSummonKillall(short int cn);
extern void RecvRemoveNK(short int cn, unsigned short id);
//---------------
extern void RecvMakeGhost( short id );
extern void Recv_CMD_ABSOLUTE_MOVE_DUNGEON_KEEPER( t_packet *p );
//extern void CheckHungryUp( void );//020214 lsw
extern void GuardEvent( void );
extern void SetGlobalBlockValues();//021026 lsw
