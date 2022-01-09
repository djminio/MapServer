#include "..\stdafx.h"
#include "DefaultHeader.h"

#define __RAJASYSTEM_H__
#include "RajaSystem.h"

#include <MMSystem.h>//010614 lsw
#include "Kein_Item.h"
#include "OP_Magic.h"
#include "SealStone.h" // 001209 KHS
#include "../LowerLayers/MyLog.h"
#include "UserManager.h"

int g_NoPkMode = false;  //021014 lsw


bool g_block_attack = false;//020821 lsw
bool g_block_magic	= false;//020821 lsw
bool g_block_move	= false;//020821 lsw
bool g_block_steal	= false;//020821 lsw
bool g_stop_please_give_life;

bool g_accelator_user_closeconnection;

char	GameServerDataPath[ FILENAME_MAX];

extern void SendViewTypeChange( int, int );
extern int initNPCGenerationTable(void);
extern CHARLIST* CheckServerId(WORD id);
extern CHARLIST* CheckServerName(const char* szName);// kyo 
extern CHARLIST* CheckNpcId(short int cn);


void SetGlobalBlockValues()
{
	g_NoPkMode				=	((::GetPrivateProfileInt( "Option", "NeverPK",		0, MAP_SERVER_INI_ ))?true:false);
	g_block_attack			=	((::GetPrivateProfileInt( "Option", "BlockAttack",	0, MAP_SERVER_INI_ ))?true:false);	
	g_block_magic			=	((::GetPrivateProfileInt( "Option", "BlockMagic",	0, MAP_SERVER_INI_ ))?true:false);
	g_block_move			=	((::GetPrivateProfileInt( "Option", "BlockMove",	0, MAP_SERVER_INI_ ))?true:false);
	g_block_steal			=	((::GetPrivateProfileInt( "Option", "BlockSkill",	0, MAP_SERVER_INI_ ))?true:false);
	g_stop_please_give_life =	((::GetPrivateProfileInt( "Option", "StopGiveLife",	0, MAP_SERVER_INI_ ))?true:false);
}

// ------------------------------------------------------------------------------------------------------------------------------
// 기  능 : 게임서버의 초기화를 위한 Init화일을 읽어들인다. 
//            이화일은 실행디렉토리에 있어야 한다. 
// 인  자 : 'dargon.ini'
// 결  과 : 성공 return 1;
//			싶패 reutrn < 0 :
// ------------------------------------------------------------------------------------------------------------------------------
int InitGameMakeModeSetting( char *filename )
{
	GetPrivateProfileString( "network", "path", "", GameServerDataPath, sizeof( GameServerDataPath ), filename );
	if( GameServerDataPath[0] == 0 )
	{
		return(-1);
	}

	GetPrivateProfileString( "network", "mapname", "", MapName, sizeof( MapName), filename );
	if( MapName[0] == 0 )
	{
		return(-1);
	}
	CharUpper( MapName );
	SetGlobalBlockValues();

	// 030919 HK YGI
	extern void LoadAIConfig();
	LoadAIConfig();

	return 1;
}	
	
void SaveGameServerStatus( void )
{	
	FILE *fp = fopen( "GameServer.Sav", "wb" );
	if( fp == NULL ) return; 
	
	fwrite( &g_curr_time, sizeof( g_curr_time ), 1, fp );
	fwrite( &CurrentNPCNo, sizeof( CurrentNPCNo ), 1, fp );
	
	fwrite( NPCList, sizeof( CHARLIST ), MAX_NPC_LIST, fp );
	fwrite( ItemList, sizeof( ITEMLIST ), MAX_ITEM_LIST, fp );
	
	fwrite( NPCgeneratePosition,  sizeof( sNPCgeneratePosition ), MAX_GENERATE_POSITION_, fp );
	fwrite( &NPCgeneratePositionMax, sizeof( int ), 1, fp );

	// 001209 KHS
	fwrite( NationRelation, sizeof( t_NationRelation ) , MAX_NATION * MAX_NATION, fp );
	
	fclose(fp);
}	
	
void LoadGameServerStatus( void )
{	
	DWORD time; 
	int i;
	
	FILE *fp = fopen( "GameServer.sav","rb" );
	if( fp == NULL ) return;


	int usingsaveddata = GetPrivateProfileInt( "Option", "UsingSavedMapServerStatus",	0, MAP_SERVER_INI_ );

	if( usingsaveddata )
	{
		if( YesOrNo( "Load the last MAPSERVER status( Using 'GameServerData.sav' ) ?", "Load" ) == IDYES )
		{
		}
		else
		{	
			fclose(fp);
			return;
		}
	}
	else return;

	fread( &time, sizeof( g_curr_time ), 1, fp );
	fread( &CurrentNPCNo, sizeof( CurrentNPCNo ), 1, fp );
	fread( NPCList, sizeof( CHARLIST ), MAX_NPC_LIST, fp );
	for( i = 0 ; i < MAX_NPC_LIST ; i ++)
	{
	}
	
	fread( ItemList, sizeof( ITEMLIST ), MAX_ITEM_LIST, fp );
	for( i = 0 ; i < MAX_ITEM_LIST ; i ++)
	{	
	}		
	
	fread( NPCgeneratePosition,  sizeof( sNPCgeneratePosition ), MAX_GENERATE_POSITION_, fp );
	fread( &NPCgeneratePositionMax, sizeof( int ), 1, fp );

	// 001209 KHS
	fread( NationRelation, sizeof( t_NationRelation ) , MAX_NATION * MAX_NATION, fp );

	fclose(fp);
}			

/*----------------------------------------------------------------------------------------------------------
// 기  능 : 캐릭터의 자동 Update 자동 Upate ㅎ
			15분 마다 저장.. 
			레벨이 바뀌거나 Exp에 변화가 있으면 저장한다. 
// 인  자 : 없슴 
// 결과값 : 업슴 
----------------------------------------------------------------------------------------------------------*/
// 010301 khs    15MIN -> 20MIN...
extern void DeleteDoublePill(t_connection c[], int cn);
extern void UpdateCharacterData(t_connection c[], int cn, bool bDirect = false);

void CharacterAutoUpdate()
{	//< CSD-CN-031213
//	return; finito 20/07/07 enabling auto update

	static DWORD time;
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	char		szQuerry[512];
	int			valExpAdd;
	int			type_exp;
	SDWORD		cbValue;
	
	if (g_curr_time - time > 1800)
	{
		time = g_curr_time;
	}
	else
	{
		return;
	}
					
	CUserManager::HASH_USER mpUser = g_pUserManager->GetUserSet();

	for (CUserManager::ITOR_USER i = mpUser.begin(); i != mpUser.end(); ++i)
	{
		const int idUser = i->second;
		CHARLIST* pUser = CheckServerId(idUser);
		
		if (pUser != NULL)
		{
			if (g_curr_time - pUser->timetoupdate > 1800) // 20분. 
			{	
				pUser->timetoupdate = g_curr_time;
				DeleteDoublePill(connections, idUser);
				UpdateCharacterData(connections, idUser);
			}	
		}
	}		
}	//> CSD-CN-031213

void SendHowManyInMap(short how)
{				
	t_packet p;	
	p.h.header.type = CMD_HOW_MANY_IN_MAP;
	p.h.header.size = sizeof(t_how_many_in_map);
	strcpy_s(p.u.how_many_in_map.map, sizeof(p.u.how_many_in_map.map), MapName);
	p.u.how_many_in_map.how = how;
	QueuePacket(connections, DB_DEMON, &p, 1);
}				

extern CHARLIST* CheckServerId(WORD id);
extern CHARLIST* CheckServerName(const char* szName);// kyo 

void RecvBlockAttack( short int cn, bool flag )
{
	CHARLIST *pUser = CheckServerId( cn );
	if( !pUser ) return;
	if( !pUser->IsCounselor() ) return;
	
	CrackMgr.SendHackingUser( cn, USE_GM_TOOL, "block attack on/off" );
	g_block_attack = flag;
}
void RecvBlockMove( short int cn, bool flag )
{
	CHARLIST *pUser = CheckServerId( cn );
	if( !pUser ) return;
	if( !pUser->IsCounselor() ) return;
	CrackMgr.SendHackingUser( cn, USE_GM_TOOL, "block move on/off" );
	g_block_move = flag;
}
void RecvBlockMagic( short int cn, bool flag )
{
	CHARLIST *pUser = CheckServerId( cn );
	if( !pUser ) return;
	if( !pUser->IsCounselor() ) return;
	CrackMgr.SendHackingUser( cn, USE_GM_TOOL, "block magic on/off" );
	g_block_magic = flag;
}
void RecvBlockGiveLife( short int cn, bool flag )
{
	CHARLIST *pUser = CheckServerId( cn );
	if( !pUser ) return;
	if( !pUser->IsCounselor() ) return;
	CrackMgr.SendHackingUser( cn, USE_GM_TOOL, "block give life on/off" );
	g_stop_please_give_life = flag;
}

void RecvSummonMonster(short int cn, short id, short amount, short x, short y, bool spread, bool summon)
{
	CHARLIST *pMaster = CheckServerId(cn);
	if (!pMaster) return;
	if(!pMaster->IsCounselor()) return;
	if(spread)
	{
		int rowsize = ceil(sqrt((double)amount));
		int spawned = 0;
		for(int i = y; i < (y + rowsize); i++)
		{
			for(int j = x; j < (x + rowsize); j++)
			{
				if(spawned >= amount) break;
				if(summon)
				{
					SummonMonster(pMaster, id, (j - (rowsize / 2)), (i - (rowsize / 2)));
				}
				else
				{
					NPC_Create(GetAliveNPCList(), id, (j - (rowsize / 2)), (i - (rowsize / 2)), -1, 0, 9); //9 is unused
				}
				spawned++;
			}
		}
	}
	else
	{
		for(int i = 0; i < amount; i++)
		{
			if(summon)
			{
				SummonMonster(pMaster, id, x, y);
			}
			else
			{
				NPC_Create(GetAliveNPCList(), id, x, y, -1, 0, 9); //9 is unused
			}
		}
	}
}

void RecvSummonKillall(short int cn)
{
	CHARLIST *pMaster = CheckServerId(cn);
	if (!pMaster) return;
	if(!pMaster->IsCounselor()) return;
	pMaster->m_xSummon.Remove(KillMonster);
}

void RecvRemoveNK(short int cn, unsigned short id)
{
	CHARLIST *pMaster = CheckServerId(cn);
	if (!pMaster) return;
	if(!pMaster->IsCounselor()) return;

	CHARLIST *pTarget = CheckServerId(id);
	if (pTarget == NULL) return; // finito null check
	pTarget->nk[N_ZYPERN] = 0;
	pTarget->nk[N_YLLSE] = 0;
	pTarget->nk[N_VYSEUS] = 0;
}

void RecvAbsoluteLogOut( short int cn, short id )
{
	CHARLIST *pUser = CheckServerId( cn );
	if( !pUser ) return;
	if( !pUser->IsCounselor() ) return;
	if( !CheckServerId( id ) ) return;

	CrackMgr.SendHackingUser( cn, USE_GM_TOOL, "absolute LogOut" );
	closeconnection( connections, id, -33 ); // 운영자에 의한 강제종료..
}

void RecvBlockSkill( short int cn, bool flag )//020821 lsw
{
	CHARLIST *pUser = CheckServerId( cn );
	if( !pUser ) return;
	if( !pUser->IsCounselor() ) return;
	CrackMgr.SendHackingUser( cn, USE_GM_TOOL, "block Skill on/off" );
	g_block_steal = flag;//020926 lsw
}

// 020708 YGI
void RecvMakeGhost(short int cn, short id)
{	//< CSD-030408
	CHARLIST* pTarget = NULL;
	CHARLIST *pUser = CheckServerId( cn );
	if( !pUser ) return;
	if( !pUser->IsCounselor() ) return;
	
	if (id <= 1000)
	{
		pTarget = CheckServerId(id);
		
		if (pTarget == NULL)
		{
			return;
		}

		pTarget->Hp				= 0;
		pTarget->bAlive			= DEAD_;
		pTarget->viewtype		= VIEWTYPE_GHOST_;
		pTarget->InitCurse(); 
		SendViewTypeChange(id, VIEWTYPE_GHOST_);
		pTarget->SendCharInfoBasic(HP, pTarget->Hp);
	}
	else
	{	
		pTarget = &NPCList[id - 10000];
		
		if (pTarget == NULL)
		{
			return;
		}

		pTarget->m_xSummon.Remove(KillMonster);
		//KillMonster(id - 10000);

		if (pTarget != NULL)
		{
			killCharacter(NULL, pTarget);
			SendCharacterCondition(pTarget, 0);
		}
	}	

	CrackMgr.SendHackingUser( cn, USE_GM_TOOL, "make ghost" );
}	//> CSD-030408

void Recv_CMD_ABSOLUTE_MOVE_DUNGEON_KEEPER( t_packet *p )
{
	t_server_magic_position	smp;
	int target_id  = p->u.angra.server_magic_position.idTarget;
	LPCHARLIST ch;
	int area_cmd;
	int id;

	//클라이언트로 보낼 패킷 셋팅
  smp.nType =	PT_ABSOL_MOVE_BY_COUNSELLER;
  smp.idTarget = WORD(target_id);
	smp.nX =	p->u.angra.server_magic_position.nX;
	smp.nY =	p->u.angra.server_magic_position.nY;

	if( target_id >= 10000 )
	{	
		id = target_id - 10000;
		ch = CheckNpcId( id );
		if( !ch ) return;
		area_cmd = MOVE_NPC_AREA;
	}	
	else
	{	
		id = target_id;
		ch = CheckServerId( id );
		if( !ch ) return;
		area_cmd = MOVE_PC_AREA;
	}
	
	ch->MoveLength = ch->MovePathCount = 0;
  ch->MoveGox = ch->X = smp.nX*TILE_SIZE;
	ch->MoveGoy = ch->Y = smp.nY*TILE_SIZE;
	ch->MoveSx = smp.nX;
	ch->MoveSy = smp.nY;

  SetArea( area_cmd, id );	
	SendCMD_MAGIC_POSITION(&smp);
}	

extern NPC_Generation		NPC_Gen_Ref[Num_Of_NPC_Generation] ;
// 자이펀의 분노 이벤트( 0720 )
void GuardEvent( void )
{	
	static DWORD ttime;
	int i;

	return;
			
	if( g_curr_time - ttime  < 900 )  return;
	ttime = g_curr_time;
			
	if( g_mon == 6 && g_day == 19 )
	{		
		for( i = NPC_LIST_START ; i < MAX_NPC_LIST ; i ++)
		{	
			if( NPCList[i].bAlive == ALIVE_ )
			switch( NPCList[i].SprNo )
			{
			case 18 :
			case 19 :
			case 20 :	NPCList[i].MoveP = NPC_Gen_Ref[NPCList[i].SprNo].Movp / 2;
						switch( rand()%7 )
						{
						case 0 : SendNPCChatArea( i, "크~~억.. 속이 이상해.. 미식거려.. 머리도 어지럽고..이런때 <자이펀>놈들 나타나면 안되는데..." );
							break;
						case 1 : SendNPCChatArea( i, "아우래도 무슨 큰병이라도 걸린것 같은데.. 자이펀 간첩들이 침투한다던데.. 몸까지 이러니.." );
							break;
						case 2 : SendNPCChatArea( i, "아~ 아~~ 며칠전부터 자꾸 눈앞이 어지럽고 머리속이 빙글 도는것이 이상한걸..." );
							break;
						case 3 : SendNPCChatArea( i, "<자이펀>놈들 때문에 비상인테 몸상태가 안 좋아" );
							break;
						case 4 : SendNPCChatArea( i, "자꾸 멀리서 헛게 보이고 어지러운게 늙었나?" );
							break;
						case 5 : SendNPCChatArea( i, "자이펀간첩은 '<자이펀>'이란 마크를 하고 있어.. 찾으면 신고해줘.." );
							break;
						}
					break;
			}
		}
	}
	else if( g_mon == 6 && g_day == 20 )
	{		
		for( i = NPC_LIST_START ; i < MAX_NPC_LIST ; i ++)
		{	
			if( NPCList[i].bAlive == ALIVE_ )
			switch( NPCList[i].SprNo )
			{
			case 18 :
			case 19 :
			case 20 :	NPCList[i].MoveP = NPC_Gen_Ref[NPCList[i].SprNo].Movp / 4;
						switch( rand()%7 )
						{
						case 0 : SendNPCChatArea( i, "크~~억.. 속이 이상해.. 미식거려.. 머리도 어지럽고..어제보다 상태가 더 안좋은것 같아.." );
							break;
						case 1 : SendNPCChatArea( i, "아우래도 무슨 큰병이라도 걸린것 같은데.. <자이펀> 간첩들이 침투한다던데.. 몸까지 이러니.." );
							break;
						case 2 : SendNPCChatArea( i, "아~ 아~~ 며칠전부터 자꾸 눈앞이 어지럽고 머리속이 빙글빙글...." );
							break;
						case 3 : SendNPCChatArea( i, "<자이펀>놈들 때문에 비상인테 몸상태가 안 좋아" );
							break;
						case 4 : SendNPCChatArea( i, "자꾸 멀리서 헛게 보이고 어지러운게 늙었나?  이럴때 <자이펀> 놈들이 나타나면 안되는데.." );
							break;
						}
					break;
			}
		}	
	}			
	else	
	{		
		for( i = NPC_LIST_START ; i < MAX_NPC_LIST ; i ++)
		{	
			if( NPCList[i].bAlive == ALIVE_ )
			switch( NPCList[i].SprNo )
			{
			case 18 :
			case 19 :
			case 20 :	NPCList[i].MoveP = NPC_Gen_Ref[NPCList[i].SprNo].Movp;
					break;
			}
		}	
	}		
}	



void Recv_CMD_SERVER_NPC_ACCESS_TABLE_RELOAD( int cn, t_packet *p )
{	
	int i;						
	
	initNPCGenerationTable(); 
	
	memset( p->u.server_npc_access_table_reload.data, 0, 800 );
	p->h.header.type = CMD_SERVER_NPC_ACCESS_TABLE_RELOAD;
	for( i = 0 ; i < 100 ; i ++)			
	{
		memcpy( p->u.server_npc_access_table_reload.data + i * 8, 
			&NPC_Gen_Ref[i].AttackDelay, 2 );			
		memcpy( p->u.server_npc_access_table_reload.data + i * 8 + 2,		
			&NPC_Gen_Ref[i].PCfindDelay, 2 );
	}							
	p->h.header.size = sizeof( t_server_npc_access_table_reload );		
	QueuePacket( connections, cn, p, 1 ); //--------------------------
	memset( p->u.server_npc_access_table_reload.data, 0, 800 );
	
	p->h.header.type = CMD_SERVER_NPC_ACCESS_TABLE_RELOAD2;				
	for( i = 0 ; i < 100 ; i ++)						
	{
		memcpy( p->u.server_npc_access_table_reload.data + i * 8,		
			&NPC_Gen_Ref[i+100].AttackDelay, 2 );
		memcpy( p->u.server_npc_access_table_reload.data + i * 8 + 2,		
			&NPC_Gen_Ref[i+100].PCfindDelay, 2 );
	}
	p->h.header.size = sizeof( t_server_npc_access_table_reload );		
	QueuePacket( connections, cn, p, 1 );//---------------------------	
	memset( p->u.server_npc_access_table_reload.data, 0, 800 );																		
	
	p->h.header.type = CMD_SERVER_NPC_ACCESS_TABLE_RELOAD3;				
	for( i = 0 ; i < 100 ; i ++)										
	{
		memcpy( p->u.server_npc_access_table_reload.data + i * 8,		
			&NPC_Gen_Ref[i+200].AttackDelay, 2 );						
		memcpy( p->u.server_npc_access_table_reload.data + i * 8 + 2,		
			&NPC_Gen_Ref[i+200].PCfindDelay, 2 );
	}
	p->h.header.size = sizeof( t_server_npc_access_table_reload );		
	QueuePacket( connections, cn, p, 1 );//---------------------------	
	memset( p->u.server_npc_access_table_reload.data, 0, 800 );																		
	
	p->h.header.type = CMD_SERVER_NPC_ACCESS_TABLE_RELOAD4;				
	for( i = 0 ; i <  97 ; i ++)										
	{
		memcpy( p->u.server_npc_access_table_reload.data + i * 8,		
			&NPC_Gen_Ref[i+300].AttackDelay, 2 );						
		memcpy( p->u.server_npc_access_table_reload.data + i * 8 + 2,		
			&NPC_Gen_Ref[i+300].PCfindDelay, 2 );
	}
	p->h.header.size = sizeof( t_server_npc_access_table_reload );		
	QueuePacket( connections, cn, p, 1 );//---------------------------	
}	
	
void RecvHexaEditing(int cn, t_client_editing_table *p)
{
	char	fn[FILENAME_MAX];
	FILE*	fp;

	wsprintf( fn, "HackingUsers_%04d%02d%02d.txt", g_year, g_mon+1, g_day );
	fp = fopen( fn, "at+" );
	if(fp)
	{
		char					User_ID[ID_LENGTH];
		char					User_Name[NM_LENGTH];
		char					User_IP[IP_LENGTH];
		char					filename[FILENAME_MAX];
		
		wsprintf(User_ID, connections[p->id].id);
		wsprintf(User_Name, connections[p->id].name);
		wsprintf(User_IP, connections[p->id].ip_address);
		
		switch(p->type)
		{
		case HACKING_EFFECTTABLE_:	
			wsprintf(filename, "effect_table.bin");
			break;
		case HACKING_011106_:
			wsprintf(filename, "011106.spr");// 0822 YJS 수정
			break;
		case HACKING_CONDITIONTABLE_:
			wsprintf(filename, "condition_table.bin");
			break;
		case HACKING_MOVEPATTERN_:
			wsprintf(filename, "move_pattern.bin");
			break;
		case HACKING_ANITABLE_:
			wsprintf(filename, "animationtable");
			break;
		case HACKING_ATTACKRANGE_:
			wsprintf(filename, "attackrange");
			break;
		case HACKING_ACCELATOR_:
			wsprintf(filename, "Filter  User");
			break;
		default:
			if(p->type<0)		wsprintf(filename, "Captain Lee Treated Data.");
			else				wsprintf(filename, "PD.Kim  Treadted Data...");
			break;
		}


		CrackMgr.SendHackingUser( cn, p->type, filename );

		fprintf(fp, "%02d:%02d:%02d  ID[%20s] Name[%20s] IP[%20s] %s\n", 
			g_hour, g_min, g_sec, User_IP, User_ID, User_Name, filename);
	
		fclose(fp);
	}

	//if( g_accelator_user_closeconnection ) closeconnection( connections, cn, -204);
}