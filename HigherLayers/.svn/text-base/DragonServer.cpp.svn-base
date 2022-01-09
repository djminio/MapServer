#include "..\stdafx.h"
#include <mmsystem.h>
#include "DefaultHeader.h"
#include "Scrp_int.h"
#include "Scrp_Exe.h"
#include "NPC_Pattern.h"
#include "map.h"

#include "OP_Magic.h"
#include "CItem.h"
#include "WeatherControl.h"
#include "Map.h"
#include "Chatting.h"

#include "RajaSystem.h"
#include "SealStoneGameServer.h"
#include "SealStone.h"
#include "..\LowerLayers\mylog.h"
#include "..\LowerLayers\packed.h"
#include "..\LowerLayers\servertable.h"
#include "dr_network.h"

#include "ChrLog.h"
#include "teambattle.h"		// 010406 YGI
#include "skill_lsw.h"
#include "RareEffectMgr.h"				//020214 lsw
#include "op_battle.h"
#include "LTSSupport.h"					// LTS DRAGONLORDWAR
#include "dungeon.h"		// 020701 YGI
#include "eventmgr.h"
#include "UserManager.h"
#include "BattleManager.h"
#include "DualManager.h"
#include "SymbolItemMgr.h" // soto-1.02
#include "BASEARENA.h"
#include "EventTreasureBoxMgr.h"//soto-030711
#include "NationSys.h" //eleval
// 030329 kyo << //just using in thailand
#include "..\HigherLayers\QuestFunction.h"	
extern void CheckLimitedAge();
//extern int localWarMsgDebug;
int g_nLimitedStartTime=0;
int g_nLimitedEndTime=0;
int g_nLimitedAge	=0;
char g_szLimitedUser_Message[MAX_PATH];
// 030929 kyo >> 

extern void ResetAbility(CHARLIST *ch);	//020214 lsw
extern void RecvCMD_SKILL_RARE_UPGRADE(int cn, t_packet *p);
extern int HandleCommand2( t_packet *p, t_connection c[], int cn );

extern int g_LocalWarBegin;			// 011215 LTS
#define MELEE			0	// 단거리.
#define RANGE_BOW		1	// 장거리..
#define RANGE_HURL		2	// 중거리.
#define RANGE_DEFAULT	3	// 단거리..
#define MAGIC			4	// 마법.

t_connection connections[ DRAGON_MAX_CONNECTIONS_+1];

extern bool isLevelUp(CHARLIST *);
extern int  updateCharacterStatus(t_connection [], int) ;
extern int  CheckHandleByKein( t_packet *p, t_connection c[], int cn );
extern int	CheckHandleByNationWar( t_packet *p, t_connection c[], const int cn );  // 010904 LTS//020903 lsw
extern void RecvSkillGauge( int cn, t_client_skill_gauge *p );
extern void RecvDebugReqCharInfo( int cn, int target ); // OP_Battle.cpp

extern void RecvCMD_MAGIC_RESULT( int caster_id, int target_id, int magic_no, int x, int y, int hunter_target );
extern void RecvCMD_CLIENT_CHARACTER_DEMAND( int cn, t_client_character_demand *p ); 
extern void Recv_CMD_PLEASE_GIVE_LIFE( int cn, int alive_word_type = 0);		// 010502 YGI
extern void Recv_CMD_PLEASE_GIVE_LIFE_GM(int cn, int id);
extern void RecvBBSLogin( char *msg );
extern void RecvAbsLogOut2GameServer( char *id );// main.cpp
extern int initItem(void);
extern void Recv_fight_stone(int cn, t_packet packet);

//this msg1 lsw
extern bool CheckFailedMagic(CHARLIST *caster, CHARLIST *target, int magic_no, int cn);//010604 lsw
extern void Recv_CMD_SERVER_NPC_ACCESS_TABLE_RELOAD( int cn, t_packet *p );
extern void BroadCastBBS_Sub( char *msg, int len );			// main.cpp

extern void SendNK( int id, int nk );
extern void SendNK( int id, int nk3, int nk4, int nk6 );

// 1010 CSB
extern int SearchItemByInv( int item_no, CHARLIST *ch );
extern int HandleCommand3( t_packet *p, t_connection c[], const int cn );		// 021028 kyo

extern cWarfield*	g_pWarfield; //Eleval 07/09/09 - To get the current warfield

void CleanUpConnection(short idServer)
{	//< CSD-HK-030829
	memset(&connections[idServer], 0, sizeof(t_connection));
	memset(var[idServer], 0, SIZE_OF_SCRIPT_VAR);
}	//> CSD-HK-030829

void CleanUpConnectionList()
{
	for (DWORD i = 0; i <= DRAGON_MAX_CONNECTIONS; ++i) 
	{
		connections[i].dwAgentConnectionIndex = 0;
		connections[i].dwIDForAgent = 0;
	}
}

// 2000/11/06, chan78 :: New QueuePacket()
static char szMsg[MAX_PACKET_SIZE+5];
//int QueuePacket( t_connection c[], int cn, t_packet *packet, int type, char *file = __FILE__, int line = __LINE__ )
int QueuePacket_( t_connection c[], int cn, t_packet *packet, int type, char *file, int line )
{
	CPackedMsg packedMsg;

	if(cn < 0 || cn >= DRAGON_MAX_CONNECTIONS) 
	{
		MyLog(LOG_IGNORE, "ERROR : cn:%d > DRAGON_MAX_CONNECTIONS:%d  cmd:%d  type:%d ", cn, DRAGON_MAX_CONNECTIONS, packet->h.header.type, type);

		FILE *fp = fopen( "./OutPut/InvalidPacketLog.txt", "at+" );
		if(fp )
		{
			if( file )
				fprintf( fp, "%02d.%02d:%02d  cn:%5d header:%5d type:%d, %s:%d \n", g_day, g_hour, g_min, cn, packet->h.header.type, type, file, line );
 			fclose(fp);
		}
		return 0;
	}

	if( (cn == DB_DEMON) || (cn == 0) ) 
	{	// To DB Demon
		DWORD dummy = 0;
		szMsg[0] = (BYTE)PTCL_MAP_TO_DB;
		memcpy( szMsg+1, (char*)&dummy, 4);
		memcpy( szMsg+1+4, (char*)packet, sizeof(t_header)+packet->h.header.size );

		if( g_pServerTable 
		&& !g_pServerTable->SendToDBDemon( szMsg, (sizeof(t_header)+packet->h.header.size)+5 ) )
		{
			if( g_pServerTable->GetOwnDBDemonData() )	// 031021 kyo
			{
				MyLog( LOG_FATAL, "Can't Write to DB Demon(%d) :: ServerTable()", g_pServerTable->GetOwnDBDemonData()->dwConnectionIndex );
			}
			else
				MyLog( LOG_FATAL, "Can't Write to DB Demon :: ServerTable");
		}
	} 
	else 
	{	// To Agent
		if( connections[cn].dwAgentConnectionIndex == NULL) 
		{
			// MyLog(LOG_FATAL, "c[%d].dwAgentConnectionIndex is NULL", cn);
			return 0;
		}

		// Message Packing
		packedMsg.SetMsg( (char *)packet, (sizeof(t_header)+packet->h.header.size) );
		packedMsg.AddUser( connections[cn].dwIDForAgent );
		packedMsg.SetAgentServerIndex( connections[cn].dwAgentConnectionIndex );

		// Send it.
		if( g_pServerTable 
		&&	!g_pServerTable->Send( connections[cn].dwAgentConnectionIndex, (char *)&packedMsg, packedMsg.GetPacketSize() ) )
		{
			MyLog( LOG_IMPORTANT, "QueuePacket() Failed To Send Packet to Agent Server(%d)", connections[cn].dwAgentConnectionIndex );
		}
	}
	return true;
}

int DequeuePacket(t_connection c[], int cn, t_packet *packet, int type)
{
	return 1;
}

void SendAliveCheckRoutine(t_connection c[])
{	//< CSD-CN-31213
	if ((global_time - g_alive_time) > 6000)
	{
		t_packet packet;
		packet.h.header.type = CMD_NONE;
		packet.h.header.size = 0;
		g_pUserManager->SendPacket(&packet);
		g_alive_time = global_time;
	}
}	//> CSD-CN-31213

extern short int getWeatherCorrection( void );

void updateCharacterStatusToLoginServer( t_connection c[], int char_id);
void updateBinaryDataToLoaginServer( t_connection c[], int id );

void DeleteDoublePill(t_connection c[], int cn)
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	char		szQuerry[512];
	int FoundPill;
	CHARLIST *ch = &connections[cn].chrlst;
	
	/*Verifica se o cara tem double xp na bag*/
	
	FoundPill = SearchItemByInv(10301,ch);
	/*if(FoundPill != 1) FoundPill = SearchItemByInv(10311,ch); //Eleval 19/08/09 - Added other pills
	if(FoundPill != 1) FoundPill = SearchItemByInv(10312,ch); //Eleval 19/08/09 - Added other pills
	if(FoundPill != 1) FoundPill = SearchItemByInv(10313,ch); //Eleval 19/08/09 - Added other pills*/
	//MyLog(LOG_NORMAL,"Procura a pill na bag");
	if( FoundPill==1 ){	
		/*Verifico se o cara ainda tem double xp no banco*/		
		//MyLog(LOG_NORMAL,"Checa a double pill");
		sprintf(szQuerry, "select top 1 exp_value, type_exp from increase_exp where name = '%s' and datestart <= getdate() and dateend >= getdate() and type_exp = 2 order by id",ch->Name);
		MyLog(LOG_NORMAL,"Query %s",szQuerry);
		SQLAllocStmt(hDBC, &hStmt);		
		retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);	
		if (!SQLOK(retCode))
		{
			MyLog(LOG_NORMAL,"increase_exp Table ....Loading Error!!!!");
			SQLFreeStmt(hStmt, SQL_DROP);
			return;
		}
		retCode = SQLFetch(hStmt);
		if (!SQLOK(retCode))
		{
			//deleta o item da exp
			MyLog(LOG_NORMAL,"Delete a double pill");
			if(SearchItemByInv(10301,ch) == 1) ::SendDeleteItemQuantity(ch,10301,1);
			/*else if(SearchItemByInv(10311,ch) == 1) ::SendDeleteItemQuantity(ch,10311,1); //Eleval 19/08/09 - Added other pills
			else if(SearchItemByInv(10312,ch) == 1) ::SendDeleteItemQuantity(ch,10312,1); //Eleval 19/08/09 - Added other pills
			else if(SearchItemByInv(10313,ch) == 1) ::SendDeleteItemQuantity(ch,10313,1); //Eleval 19/08/09 - Added other pills*/
			SQLFreeStmt(hStmt, SQL_DROP);
			return;
		}
		SQLFreeStmt(hStmt, SQL_DROP);
	}
	FoundPill = 0;
	FoundPill = SearchItemByInv(10292,ch);
	if( FoundPill==1 ){	
		/*Verifico se o cara ainda tem double xp no banco*/		
		//MyLog(LOG_NORMAL,"Checa a double pill");
		sprintf(szQuerry, "select top 1 exp_value, type_exp from increase_exp where name = '%s' and datestart <= getdate() and dateend >= getdate() and type_exp = 3 order by id ",ch->Name);
		MyLog(LOG_NORMAL,"Query %s",szQuerry);
		SQLAllocStmt(hDBC, &hStmt);		
		retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);	
		if (!SQLOK(retCode))
		{
			MyLog(LOG_NORMAL,"increase_exp Table ....Loading Error!!!!");
			SQLFreeStmt(hStmt, SQL_DROP);
			return;
		}
		retCode = SQLFetch(hStmt);
		if (!SQLOK(retCode))
		{
			//deleta o item da exp
			MyLog(LOG_NORMAL,"Deleta a smp pill");
			::SendDeleteItemQuantity(ch,10292,1);
			SQLFreeStmt(hStmt, SQL_DROP);
			return;
		}
		SQLFreeStmt(hStmt, SQL_DROP);
	}
}

// Marked by chan78
void UpdateCharacterData(t_connection c[], int cn, bool bDirect)
{	//< CSD-HK-030829
	if (g_pServerTable == NULL)
	{
		goto SAVE_MAP;
	} 

	if (!g_pServerTable->GetOwnDBDemonData() || !g_pServerTable->GetOwnDBDemonData()->dwConnectionIndex)
	{
		goto SAVE_MAP;
	}
	
	if (bDirect)
	{
		goto SAVE_MAP;
	}
	
	updateCharacterStatusToLoginServer(connections, cn);
	updateBinaryDataToLoaginServer(c, cn);
	SendLoginServerForCharUpdate(cn); //  추가 캐릭터 정보 저장
	UpdateLadderScore(&c[cn].chrlst); // 래더 랭킹에 저장
	return;

SAVE_MAP:
	updateCharacterStatus(connections, cn);
	UpdateCharStatusByKein(c, cn); // 추가 캐릭터 정보 저장
}	//> CSD-HK-030829

extern void DeleteSquadMember(const int cn); // 010904 LTS		// 국가전 부대정보를 삭제한다.//020903 lsw
extern void DecLocalWarfieldMemberCount(const int cn);		// LTS LOCALWAR//020903 lsw
extern cDragonLordWar		g_pDragonLordWar;					// LTS DRAGONLORD
//---------------------------------------------------------------------------------
// Re-Wrote by chan78 at 2000/11/09 :: New closeconnection()
// 변경사항: socket을 직접 다루지 않는다. Server간 커넥션은 이 함수를 사용하지 않는다.
//<! BBD 040110	인자 추가
//void closeconnection(t_connection c[], int cn, int errnum)
void closeconnection_msg(t_connection c[], int cn, int errnum, char * msg)
{
	//SendMsg
	t_packet p;
	p.h.header.size = sizeof( t_server_bbs ) - MAX_PATH + strlen(msg);
	p.h.header.type = CMD_BBS;
	p.h.header.size = sizeof(t_server_bbs) - MAX_PATH + p.h.header.size;
	strcpy_s(p.u.server_bbs.bbs, sizeof(p.u.server_bbs.bbs), msg);
	//g_pUserManager->SendPacket(&p);
	QueuePacket(connections, cn, &p, 1);

	closeconnection(c, cn, errnum);
}

void closeconnection(t_connection c[], int cn, int errnum, bool bUseDemon)
//> BBD 040110 인자 추가
{	
	if (cn < DRAGON_CONNECTIONS_START || cn >= DRAGON_MAX_CONNECTIONS)	
	{	//< CSD-030322	
		return;
	}	//> CSD-030322

	CHARLIST *ch = &connections[cn].chrlst;

	if (cn != ch->GetServerID())
	{	//< CSD-HK-030829
		g_pServerTable->SetNumOfUsers(g_pServerTable->GetNumOfUsers() - 1, 100, cn, errnum, connections[cn].name);
		CleanUpConnection(cn);
		MyLog(LOG_FATAL, "closeconnection : cn(%d), server_id(%d)", cn, ch->GetServerID());
		return;
	}	//> CSD-HK-030829

	t_packet packet;
	QTickData[cn].Func = 0;

	if (ch->IsPlayer())
	{	// 소환된 몬스터가 있다면 전부 제거
		ch->m_xSummon.Remove(KillMonster);

		if (ch->DragonLordWarTeam>0)					// LTS DRAGONLORDWAR
		{
			g_pDragonLordWar.FindAndDeleteMember(cn);
		}

		g_pWarfield->RemoveWonHonor(ch->GetServerID()); //eleval 07/09/09 - To remove the honor won if the char dc during NW
	}

	SendExitGameWho(cn);		// 파티원에게 나 나간다고 알린다. // 0116 YGI
	SetArea(REMOVE_PC_AREA, cn);
	ResetManToManItemWhenCloseConnect(ch);		// 0911 YGI

	if( ch->reporter_mode )		// 기자단 모드이면
	{
		ch->Hp		= ch->HpMax		* 20 / 100;
		ch->Hungry  = ch->HungryMax * 30 / 100;
		ch->Mana	= 0;//020214 lsw
		aliveCharacter( ch );
	}
	if( ch->handheld.item_no )
	{
		MoveEmptyInv( &ch->handheld, ch );
		POS pos;
		SetItemPos( HAND, &pos );
		SendServerEachItem( &pos, &ch->handheld, cn );
	}

	if( ch->fight_flag )
	{
		CHARLIST *win = CheckServerId( ch->fight_id );
		if( win )
		{
			EndManToManFight( win, ch );
		}
	}

	if( g_pDungeonMap ) 
	{
		g_pDungeonMap->CloseConnect( cn );		// 020701 YGI
	}

	::SetTileOccupied(ch->MoveSx,ch->MoveSy,false);
	
	for( int i = 0 ; i < MAX_PC_CONTROL_NPC ; i ++)	// LTS AI BUG
	{
		if( ch->ctrlnpc[i] != -1 )
		{
			NPCList[ ch->ctrlnpc[i]].ctrlpc = -1;
			SetArea(ADD_NPC_AREA, ch->ctrlnpc[i]);	
		}
	}
	
	if( connections[cn].state >= CONNECT_JOIN ) // 연결하자마자 끝어졌다고 읽지도 않는 Data를 갱신할수는 없다.
	{
		packet.h.header.type = CMD_USER_REMOVE;
		packet.u.server_user_remove.server_id = cn;
		packet.h.header.size = sizeof(t_server_user_remove);
		QueuePacket(connections, cn, &packet, 1);
	}

	DeleteSquadMember(cn);  // Remove From Warfield Squad Hash;   // 010904 LTS
	DecLocalWarfieldMemberCount(cn);									// LTS LOCALWAR

	// 011210 YGI
	// DB에게 나 나간다고 알린다.
	SendPacketDefault( CMD_SERVER_EXIT_GAME , ch->Name, strlen( ch->Name), DB_DEMON );
	//<! BBD 040110 플래그를 이용
	//	UpdateCharacterData(c, cn, false);
	DeleteDoublePill(c, cn); // Add by rogerio
	UpdateCharacterData(c, cn, bUseDemon);
	//> BBD 040110
	SendLog2ChrLogDB( c, cn, CHR_LOG_LOGOUT_ );
	// Modified by chan78 at 2000/12/17
	// 접속종료 요구 메시지를 보낸다.
	char szMsg[32];
	// Connected User Counter
	// Added by chan78 at 2000/12/17
	if( connections[cn].dwAgentConnectionIndex && connections[cn].dwIDForAgent )
	{
		szMsg[0] = (BYTE)PTCL_MAP_TO_AGENT_REQUEST_DISCONNECT_USER;
		memcpy(szMsg+1,&connections[cn].dwIDForAgent,4);

		if( !g_pServerTable->Send(connections[cn].dwAgentConnectionIndex, szMsg, 1+4) )
		{
			MyLog( LOG_FATAL, "closeconnection_core() :: Failed to Send PTCL_USER_LOST_CONNECTION(cn:%d/dwCI%d/ID:%d)", cn, connections[cn].dwAgentConnectionIndex, connections[cn].dwIDForAgent );
		}
	}

	switch( errnum )
	{
	case -200: //Yaddaa skill speedlimit/missing attack packets
		{
			MyLog(LOG_NORMAL, "Forced Disconnect! (Skillhack): %s:%s( %s )", connections[cn].id,  connections[cn].name, connections[cn].ip_address);
			break;
		}
	case -208 : // Accelator User...
		{
			MyLog( LOG_NORMAL, "Forced Disconnect ! (Accelator): %s:%s( %s )", connections[cn].id,  connections[cn].name, connections[cn].ip_address );
			break;
		}
	case -206 : // Filter User...
		{
			MyLog( LOG_NORMAL, "Forced Disconnect ! (Filter): %s:%s( %s )", connections[cn].id,  connections[cn].name, connections[cn].ip_address );
			break;
		}	
	case -210 :// Finito - Teleport Hack used...
		{
			MyLog( LOG_NORMAL, "Forced Disconnect ! (Teleport Hack): %s:%s( %s )", connections[cn].id,  connections[cn].name, connections[cn].ip_address );
			break;
		}
	case -203:  // CMD_CLOSE_LOGIN_ID
	case -220:  // Death Connections
	case -205:  // 클라이언트에서 온 정상종료.
	case -33 :  // 운영자에 의한 강제종료.
	case -204:  // 크래킹시도에 의한 종료 (클라이언트에서 날아온)
		{
			MyLog( LOG_NORMAL, "Forced Disconnect ! (%d): %s:%s( %s )", errnum, connections[cn].id,  connections[cn].name, connections[cn].ip_address );
			break;
		}
	}

	g_pServerTable->SetNumOfUsers(g_pServerTable->GetNumOfUsers()-1,100, cn,errnum, connections[cn].name);	
	//< CSD-HK-030829
	CleanUpConnection(cn);
}	//> CSD-HK-030829



//-------------------------------------------------------------------------

int SettingMoveData_( int j, CHARLIST *c, t_packet *p )
{
	t_server_move *t	= &(p->u.server_move);

	t->length			= (unsigned char)( c->MoveLength - c->MovePathCount );

	if( j >= 10000 && t->length <= 0 ) return 0;
	if( t->length < 0 ) return 0;

	t->targetid			= c->targetid;
	t->server_id		= j;
	t->length			= (unsigned char)( c->MoveLength - c->MovePathCount );

	if( c->MovePathCount % 2 )  t->length += 1000; // 만약 1000보다 큰수가 가면 먼저 1바이트의 하위4Bit를 이용한다.
	
	t->sx				= (short int)c->MoveSx;
	t->sy				= (short int)c->MoveSy;
	t->gox				= (short int)c->MoveGox;
	t->goy				= (short int)c->MoveGoy;

	memcpy(t->dir_tbl, c->MoveDirTbl + c->MovePathCount / 2, ( c->MoveLength - c->MovePathCount+1  )/2 );

	t->desttime = ReturnDestTime( c );
	t->movetype = c->MoveType;

    p->h.header.size = sizeof(t_server_move) - MAX_DIR_TABLE + (( c->MoveLength - c->MovePathCount+1 )/2 );

	return 1;
}









int move_count = 0;


//-----------------------------------------------------------------
// Send Environment Info
//-----------------------------------------------------------------
void SendEnvironment( t_connection c[], int cn)
{
	t_packet packet;
	t_server_envir_info *tp = &packet.u.server_envir_info;

	packet.h.header.type = CMD_ENVIR_INFO;
	{
		tp->weather_count	= global_time;
		tp->currtime_count	= g_curr_time;
		tp->rainstart		= today_rainstart[ today_weathercount];
		tp->rainend			= today_rainend[ today_weathercount];
		tp->amount			= today_rainamount[ today_weathercount];
		tp->temperature		= getWeatherCorrection();		// 현재 월, 일, 시간을 보내준다. 
		memcpy(tp->mapname, MapName, NM_LENGTH);
	}
	packet.h.header.size = sizeof(t_server_envir_info);
	QueuePacket(connections, cn, &packet, 1);
}

extern void RecvNewDay( t_packet *packet );
int HandleCommand( int cn, t_packet *packet)
{
	if ((cn < 0 || cn > DRAGON_MAX_CONNECTIONS) && cn != CN_FROM_OTHER_MAPSERVER_MSG)
	{
		return -80;
	}		
	
	switch (packet->h.header.type)
	{
	case CMD_NONE:
		{
			break;
		}
	case CMD_PROXY_TO_MAP_CHANGE_DATE:	
		{
			RecvNewDay(packet);
			break;
		}
	case CMD_CHANGE_MAP_DETECTED:
		{
			closeconnection_kein( connections, cn, 1 );
			break;
		}
	case CMD_HEXAEDITING:
		{
			RecvHexaEditing(cn, &packet->u.angra.client_editing_table);
			break;
		}
	case CMD_PING:
		{
			packet->h.header.type = CMD_PING;
			packet->h.header.size = 0;
			QueuePacket(connections, cn, packet, 1);
			break;
		}
	case CMD_LOST_CONNECTION:
		{
			closeconnection(connections, cn, -51);
			break;	
		}	
	case CMD_ACCESS_JOIN:
		{	//< CSD-HK-030829
			if (!CheckServerIndex(cn))
			{
				break;
			}
			
			ACCESSJOIN *pAJ = &packet->u.AccessJoin;
			
			if (g_pUserManager->IsExist(pAJ->name))
			{	
				break;
			}
			// initializing user	// THAI kein
			const DWORD idUniq = connections[cn].dwIDForAgent;
			const DWORD dwConnectionIndex = connections[cn].dwAgentConnectionIndex;
			CleanUpConnection(cn);
			
			connections[cn].dwIDForAgent = idUniq;
			connections[cn].dwAgentConnectionIndex = dwConnectionIndex;
			connections[cn].state = CONNECT_LOGIN;
			strcpy_s(connections[cn].id, sizeof(connections[cn].id), pAJ->id);
			strcpy_s(connections[cn].pw, sizeof(connections[cn].pw), pAJ->pw);
			strcpy_s(connections[cn].name, sizeof(connections[cn].name), pAJ->name);
			strcpy_s( connections[cn].ip_address, sizeof(connections[cn].ip_address), pAJ->ip);
			connections[cn].chrlst.SetServerID(cn);
			connections[cn].chrlst.startposition = pAJ->startposition;

			if(LocalMgr.IsAbleNation(KOREA))
			{
				connections[cn].chrlst.SetBillingType(pAJ->nBillingType);
			}
			else
			{
				connections[cn].chrlst.SetBillingType(BT_COMMERCIAL_IP);
			}
			//DB에게 받아도 좋은지 요청한다.  또는 Map이동해도 좋은지 요청한다. 
			// Mapserver->DBDemon
			t_packet tp;
			tp.h.header.type = CMD_REQ_INSERT_USERID;
			tp.h.header.size = sizeof(t_gs_req_insert_userid);
			tp.u.gs_req_insert_userid.server_id = cn;
			strcpy_s(tp.u.gs_req_insert_userid.id, sizeof(tp.u.gs_req_insert_userid.id), pAJ->id);
			strcpy_s(tp.u.gs_req_insert_userid.pw, sizeof(tp.u.gs_req_insert_userid.pw), pAJ->pw);
			strcpy_s(tp.u.gs_req_insert_userid.name, sizeof(tp.u.gs_req_insert_userid.name), pAJ->name);
			strcpy_s(tp.u.gs_req_insert_userid.mapname, sizeof(tp.u.gs_req_insert_userid.mapname), MapName);
			QueuePacket(connections, DB_DEMON, &tp, 1);
			break;
		}	//> CSD-HK-030829
	case CMD_JOINABLE: // DB_DEMON 에서 받아도 좋다는 허락을 받았다. 
		{	//< CSD-HK-030829
			const int server_id = packet->u.ls_joinable.server_id;

			if (connections[server_id].dwAgentConnectionIndex == 0)
			{
				break;
			}
			
			if (connections[server_id].state != CONNECT_LOGIN && connections[server_id].state != CONNECT_LOGIN_KEIN)
			{
				break;
			}
			//> CSD-HK-030829
			packet->h.header.type = CMD_ASSIGN_SERVER_ID;
			packet->h.header.size = sizeof(t_server_assign_server_id );
			packet->u.server_assign_server_id.server_id	= server_id;
			connections[server_id].chrlst.SetServerID(server_id);
			QueuePacket(connections, server_id, packet, 1);
			// DB쪽에 허락받은 캐릭에 대한 Data를 요청한다. 
			// 최초에 맵에 들어올경우, 맵이동이 아닐경우...
			if( connections[server_id].state == CONNECT_LOGIN )			
			{	
				packet->h.header.type = CMD_ACCESS_CHAR_DB;
				packet->u.server_access_char_db.server_id	= server_id;
				memcpy(packet->u.server_access_char_db.id,	connections[server_id].id,	ID_LENGTH);
				memcpy(packet->u.server_access_char_db.name, connections[server_id].name,	NM_LENGTH);
				packet->h.header.size = sizeof(t_server_access_char_db);
				::QueuePacket(connections, DB_DEMON, packet, 1);
				
				t_packet	GuildPacket;//지키미 길드리스트 보냄.
				GuildPacket.h.header.type = CMD_GUARDIANLIST;
				GuildPacket.h.header.size = sizeof(t_GUARDIANLIST);
				const int count = GuildPacket.u.GuardianList.nCount = g_CGuardianGuild.GetCount();
				for(int i = 0;i < count;++i)
				{
					GuildPacket.u.GuardianList.anGuildCodes[i] = g_CGuardianGuild.GetGuildCode(i);	
				}
				::QueuePacket( connections, server_id, &GuildPacket, 1 );
				
				::SendReqGetCharInfoEtc( server_id, connections[server_id].name );	// 0410 YGI	// 캐릭터 추가 정보를 받아 온다.
				connections[server_id].chrlst.updatable = 0;			// 030919 HK YGI
			}

			break;
		}
	case CMD_NOT_JOINABLE:
		{	//< CSD-HK-030828
			const int server_id = packet->u.ls_not_joinable.server_id;
			
			if (connections[server_id].dwAgentConnectionIndex == 0)
			{
				break;
			}
			
			if (connections[server_id].state != CONNECT_LOGIN)
			{
				break;
			}
			
			packet->h.header.type = CMD_INVALID_ID;
			packet->h.header.size = 0;
			QueuePacket( connections, server_id, packet, 1 );
			break;
		}	//> CSD-HK-030828
	case CMD_ACCEPT_CHAR_DB:
		{
			t_server_accept_char_db  *tt = &packet->u.server_accept_char_db;
			const int server_id = tt->server_id;
			if( !CheckServerIndex( server_id ) ) 
			{
				MyLog( 0, "CMD_ACCEPT_CHAR_DB, wrong server_id : [%d]", server_id );	// 030923 HK YGI
				break;
			}
			LPCHARLIST ch = &connections[server_id].chrlst;
			// 030923 HK YGI
			if( strcmp( tt->name, connections[server_id].name ) != 0 ) 
			{
				MyLog( 0, "CMD_ACCEPT_CHAR_DB, wrong character name : [%s], [%s]", tt->name, connections[server_id].name );
				break; // 030921 HK YGI
			}
			
			strcpy_s( ch->Name, sizeof(ch->Name), connections[server_id].name );		// KHS
			EatRearWhiteChar( ch->Name );
			
			ch->SetLevel(tt->Level); // CSD-030806
			ch->Exp			= tt->Exp;
			
			ch->Gender		= tt->Gender;
			ch->Face		= tt->Face;
			ch->SetGuildCode(tt->nGuildCode); // CSD-030806
			ch->Class		= tt->Class;
			ch->Job			= tt->Job;
			ch->Spell		= tt->Spell;
			
			ch->Str  		= tt->Str;
			ch->Con  		= tt->Con;
			ch->Dex  		= tt->Dex;
			ch->Wis  		= tt->Wis;
			ch->Int  		= tt->Int;
			ch->MoveP		= tt->MoveP;
			ch->Char 		= tt->Char;
			ch->Endu 		= tt->Endu;
			ch->Moral 		= tt->Moral;
			
			ch->Money		= tt->Money;
			ch->Hp = tt->nLife;
			ch->HpMax = tt->nMaxHp;
			
			ch->Mana = tt->nMana;//020214 lsw
			ch->ManaMax	= tt->nMaxMp;
			ch->Hungry = tt->nHungry;
			ch->HungryMax = tt->nMaxHungry;
			
			ch->SetState(tt->Condition);
			ch->SprNo		= tt->SprNo;
			ch->X    		= tt->X;
			ch->Y    		= tt->Y;
			
			memcpy( ch->MapName, MapName, NM_LENGTH);
			
			ch->Peacests	= tt->Peacests;
			ch->Sight		= tt->Sight;
			
			ch->BodyR		= tt->BodyR;
			ch->BodyG		= tt->BodyG;
			ch->BodyB		= tt->BodyB;
			
			ch->ClothR		= tt->ClothR;
			ch->ClothG		= tt->ClothG;
			ch->ClothB		= tt->ClothB;
			
			ch->Age			= tt->Age;
			ch->Luck		= tt->Luck;		// 0208 YGI
			ch->wsps		= tt->wsps;
			memcpy( &ch->name_status, &tt->nation, sizeof(DWORD ) );		// 1004 YGI
			
			ch->accessory[0] = tt->accessory[0];
			ch->accessory[1] = tt->accessory[1];
			ch->accessory[2] = tt->accessory[2];
			ch->accessory[3] = tt->accessory[3];
			ch->mantle		 = tt->mantle;// 011018 KHS 망또 
			
			ch->openhouse	= tt->openhouse;
			ch->total_id	= tt->total_id;
			ch->viewtype	 = tt->viewtype;
			
			ch->SetBasicResist(RT_POISON,tt->nPoison); // 저주계열 마법에 대한 저항력//030227 lsw
			ch->SetBasicResist(RT_CURSE,tt->nCurse); //	저주계열 마법에 대한 저항력
			ch->SetBasicResist(RT_HOLY,tt->nHoly); // 신력계열 공격 마법에 대한 저항력
			ch->SetBasicResist(RT_FIRE,tt->nFire); //	불계열 공격 마법에 대한 저항력
			ch->SetBasicResist(RT_ICE,tt->nIce); //	얼음계열 공격 마법에 대한 저항력
			ch->SetBasicResist(RT_ELECT,tt->nElect); //	전격계열 공격 마법에 대한 저항력
			
			ch->social_status	= tt->social_status;
			ch->fame			= tt->fame;
			ch->fame_pk			= tt->fame_pk;	// 010915 LTS	//Fame_PK -> NWCharacter로 교체 DB에는 실제로 NWCharacter의 값이 들어갑니다.		
			ch->NWCharacter		= tt->NWCharacter;	// 010915 LTS
			// 자동 Update하기 위한 기본 Data Setting.
			ch->timetoupdate	= g_curr_time;
			ch->SetStepInfo(tt->aStepInfo, sizeof(tt->aStepInfo));

			ch->m_nUserAge		= tt->nUserAge;			// 030929 kyo
			ch->EventJoin		= tt->EventJoin;		// 020115 LTS
			// 030923 HK YGI
			extern void SaveLoginLogoutLogByKein( CHARLIST *ch, int type, int );
			SaveLoginLogoutLogByKein( ch, 1, 0 );

			break;
		}
	case CMD_ACCEPT_CHAR_GAME_DB:
		{
			int px, py;
			t_server_accept_char_game_db *tt = &packet->u.server_accept_char_game_db;
			const int server_id	= tt->server_id;
			// 030921 HK YGI
			if( !CheckServerIndex( server_id ) )break;
			if( strcmp( tt->name, connections[server_id].name ) != 0 ) break; 
			
			LPCHARLIST ch = &connections[server_id].chrlst;
			ch->SetBankMoney(tt->BankMoney, BMCT_CHAR_LOGIN); // CSD-030723
			
			memcpy( &ch->WinLoseScore, &tt->win_defeat, sizeof( DWORD ) );
			ch->LadderScore = tt->LadderScore;
			ch->LastLoan 	= tt->LastLoan;
			
			ch->nk[ N_VYSEUS]	= tt->nk3;
			ch->nk[ N_ZYPERN]	= tt->nk4;
			ch->nk[ N_YLLSE] 	= tt->nk6;
			
			ch->killmon		= tt->killmon;
			ch->killanimal	= tt->killanimal;
			ch->killpc 		= tt->killpc;
			ch->SetReservedPoint(tt->reserved_point);
			ch->Tactics		= tt->Tactics;
			// 게임에 들어와 서 기본적으로  Setting해야 할것...
			ch->itemweight		= GetTotalItemWeight( ch );
			
			if( ch->Peacests < 0 || ch->Peacests > 1 ) ch->Peacests = 0;
			
			if( ch->startposition == 99 )
			{	
				px = ch->X>>5;
				py = ch->Y>>5;
				NPC_AdjustPosition( ch->SprNo, &px, &py );
				
				ch->X = px * TILE_SIZE + 16;
				ch->Y = py * TILE_SIZE + 16;
			}	
			else 
			{	
				px = ch->X>>5;
				py = ch->Y>>5;
				// 010414 KHS
				// 실제적인 캐릭터의 좌표가 바뀌는곳.  --> 만약, 맵을 선택하여 
				// DB Demon에서 넘어오는 캐릭터의 좌표는 DB의 내용이고
				// 이곳에서는 Client에서 선택한 Map의 시작좌표로 Setting한다. 
				// Recent를 선택했을 경우에는 DB에서 보내온 좌표를 보내준다.
				// ( 또는 DB_Demon:CheckStartMap()에 의해 MapName이 바뀔수 있다.. )
				
				if( strcmp( MAPNAME, "MA-IN" ) == 0 )
				{
					switch( rand()%3 )
					{
					case 0 :  px = 247,py = 107; break;
					case 1 :  px = 232,py =  97; break;
					default : px = 266,py = 122; break;
					}
				}
				else if( strcmp( MAPNAME, "RENES_C" ) == 0 ) 
				{
					px = 210, py = 175;
				}
				else if( strcmp( MAPNAME, "COLOR" ) == 0 ) 
				{
					px = 196, py = 156;
				}
				else if( strcmp( MAPNAME, "KAL_OS" ) == 0 ) 
				{
					px = 122, py = 164;
				}
				else if( strcmp( MAPNAME, "SCHOLIUM" ) == 0 ) 
				{
					switch( rand()%2 )
					{
					case 0 :	px = 4;	py = 23; break;
					default :	px = 4;	py = 24; break;
					}
				}
				else if( strcmp( MAPNAME, "BIS_I" ) == 0 )
				{
					switch( rand()%3 )
					{
					case 0 :	px = 253;	py = 449; break;
					case 1 :	px = 292;	py = 456; break;
					default :	px = 274;	py = 472; break;
					}
					
				}
				else if( strcmp( MAPNAME, "IRAMUS" ) == 0 )
				{
					switch( rand()%2 )
					{
					case 0 :	px = 92;	py = 67; break;
					default :	px = 135;	py = 74; break;
					}
				}
				else if( strcmp( MAPNAME, "DEP" ) == 0 )
				{
					switch( rand()%3 )
					{
					case 0 :	px = 276;	py = 360; break;
					case 1 :	px = 289;	py = 369; break;
					default :	px = 302;	py = 381; break;
					}
				}
				else if( strcmp( MAPNAME, "TYNEN" ) == 0 )
				{
					switch( rand()%2 )
					{
					case 0 :	px = 445;	py = 260; break;
					default :	px = 494;	py = 305; break;
					}
				}
				else if( strcmp( MAPNAME, "BARANTAN" ) == 0 )
				{
					switch( rand()%2 )
					{
					case 0 :	px = 329;	py =  95; break;
					default :	px = 360;	py = 150; break;
					}
				}
				else if( strcmp( MAPNAME, "ICE-W01" ) == 0 )
				{
					px = 112, py = 147;
				}
				else if( strcmp( MAPNAME, "K_SUNG2" ) == 0 )
				{
					px = 322, py = 202;
				}
				else if( strcmp( MAPNAME, "SUNG_TILE_00" ) == 0 ) 
				{
					px = 138, py = 215;
				}
				else if( strcmp( MAPNAME, "SOURCE" ) == 0 ) 
				{
					px = 274+ rand()%10, py = 250 + rand()%6;
				}
				else if( strcmp( MAPNAME, "MANDUN1" ) == 0 ) 
				{
					switch( rand()%2 )
					{
					case 0 :	px = 108;	py = 236; break;
					default :	px = 131;	py = 229; break;
					}
				}
				else if( strcmp( MAPNAME, "FIREDUN1" ) == 0 ) 
				{
					px = 128, py = 81;
				}
				else if( strcmp( MAPNAME, "GRAY" ) == 0 ) 
				{
					px = 320, py = 325;
				}
				else if( strcmp( MAPNAME, "SCHOLIUM2" ) == 0 ) //020822 lsw
				{
					switch( rand()%2 )
					{
					case 0 :	px = 124;	py = 74; break;
					default :	px = 132;	py = 62; break;
					}
				}
				
				NPC_AdjustPosition( ch->SprNo, &px, &py );
				
				ch->X = px * TILE_SIZE + 16;
				ch->Y = py * TILE_SIZE + 16;
			}
			
			ch->OldMoveSx = ch->MoveSx = px;
			ch->OldMoveSy = ch->MoveSy = py;
			
			::SetTileOccupied(ch->MoveSx,ch->MoveSy,true);
			
			sRECALL_SCRP_NO = 0; // 이벤트가 발생하면 이값은 0이 아닌값으로 된다.  들어오면 다시 Setting..
			break;
		}	
		case CMD_ACCEPT_BINARY_DATA_ERROR:
			{	//< CSD-030415
				const int idUser = g_pUserManager->GetServerID(packet->u.server_accept_data_error.name);
				
				if (idUser != -1)
				{
					packet->h.header.type = CMD_INVALID_DB;
					packet->h.header.size = sizeof(t_server_accept_data_error);
					QueuePacket(connections, idUser, packet, 1);
				}
				
				break;
			}	//> CSD-030415
		case CMD_ACCEPT_BINARY_DATA:
			{
				const int server_id	= packet->u.server_accept_binary_data.server_id;
				if (!CheckServerIndex(server_id)) break;
				if( strcmp( packet->u.server_accept_binary_data.name, connections[server_id].name ) != 0 ) break; // 030921 HK YGI
				
				memcpy((UCHAR *)connections[server_id].chrlst.Ws,				(UCHAR *)packet->u.server_accept_binary_data.Ws,			SIZE_OF_WS);
				memcpy((UCHAR *)connections[server_id].chrlst.Ps,				(UCHAR *)packet->u.server_accept_binary_data.Ps,			SIZE_OF_PS);
				memcpy((UCHAR *)connections[server_id].chrlst.Skill,			(UCHAR *)packet->u.server_accept_binary_data.Skill,			SIZE_OF_SKILL);
				memcpy((UCHAR *)connections[server_id].chrlst.skillexp,			(UCHAR *)packet->u.server_accept_binary_data.skillexp,		SIZE_OF_SKILL_EXP);
				memcpy((UCHAR *)connections[server_id].chrlst.tac_skillEXP,		(UCHAR *)packet->u.server_accept_binary_data.tac_skillEXP,	SIZE_OF_TAC_SKILL_EXP);
				memcpy((UCHAR *)connections[server_id].chrlst.equip,			(UCHAR *)packet->u.server_accept_binary_data.equip,			SIZE_OF_EQUIP);
				memcpy((UCHAR *)connections[server_id].chrlst.quick,			(UCHAR *)packet->u.server_accept_binary_data.quick,			SIZE_OF_QUICK);
				memcpy((UCHAR *)connections[server_id].chrlst.party_str,		(UCHAR *)packet->u.server_accept_binary_data.party,			SIZE_OF_PARTY);				// 0218 YGI
				memcpy((UCHAR *)connections[server_id].chrlst.relation_str,		(UCHAR *)packet->u.server_accept_binary_data.relation,		SIZE_OF_RELATION);
				memcpy((UCHAR *)connections[server_id].chrlst.employment_str,	(UCHAR *)packet->u.server_accept_binary_data.employment,	SIZE_OF_EMPLOYMENT);
				
				SetTacSkillLevel( &connections[server_id].chrlst );		// 0711 YGI
				SetMySkill( &connections[server_id].chrlst );		// 일단 기본적으로 알아야하는 스킬 셋
				break;
			}
		case CMD_ACCEPT_SCRIPT_DATA:
			{
				const int server_id	= packet->u.server_accept_script_data.server_id;
				if (!CheckServerIndex(server_id)) break;
				if( strcmp( packet->u.server_accept_script_data.name, connections[server_id].name ) != 0 ) break; // 030921 HK YGI

				memcpy((UCHAR *)var[server_id], (UCHAR *)packet->u.server_accept_script_data.script_var, SIZE_OF_SCRIPT_VAR);
				break;
			}
		case CMD_ACCEPT_INV_DATA:
			{
				const int server_id = packet->u.server_accept_inv_data.server_id;
				if (!CheckServerIndex(server_id)) break;
				if( strcmp( packet->u.server_accept_inv_data.name, connections[server_id].name ) != 0 ) break; // 030921 HK YGI
				
				memcpy((UCHAR *)connections[server_id].chrlst.inv, (UCHAR *)packet->u.server_accept_inv_data.inv, SIZE_OF_INV);
				
				if( packet->u.server_accept_inv_data.refresh_inventory ) 
				{	// 클라이언트로 다시 값을 보내주자...
					RecvResendItem( server_id );
				}

				break;
			}
		case CMD_ACCEPT_ITEM_DATA:
			{
				const int server_id = packet->u.server_accept_item_data.server_id;
				if (!CheckServerIndex(server_id)) break;
				if( strcmp( packet->u.server_accept_item_data.name, connections[server_id].name ) != 0 ) break; // 030921 HK YGI

				memcpy((UCHAR *)connections[server_id].chrlst.Item, (UCHAR *)packet->u.server_accept_item_data.Item, SIZE_OF_ITEMINDEX);
				break;
			}
		case CMD_ACCEPT_BANKITEM_DATA :
			{
				const int server_id	= packet->u.server_accept_bankitem_data.server_id;
				if (!CheckServerIndex(server_id)) break;
				if( strcmp( packet->u.server_accept_bankitem_data.name, connections[server_id].name ) != 0 ) break; // 030921 HK YGI

				memcpy((UCHAR *)connections[server_id].chrlst.bank, (UCHAR *)packet->u.server_accept_bankitem_data.bankitem, SIZE_OF_BANKITEM);
				CheckItem(server_id);		// 0218 YGI//020228 lsw
				CheckMallItemTime(server_id); // Sets the time left on timed mall items
				
				int i;
				for( i = 0 ; i < 1000 ; i ++)
				{
					if (connections[server_id].chrlst.Item[i] == 0)
					{
						break;
					}
				}

				connections[server_id].chrlst.ItemMax = i;
				// 맵에 들어오는 좌표를 보내준다..
				packet->h.header.type = CMD_ACCEPT_JOIN;
				packet->u.server_accept_join.x = connections[server_id].chrlst.X;
				packet->u.server_accept_join.y = connections[server_id].chrlst.Y;
				packet->h.header.size = sizeof( t_server_accept_join );
				QueuePacket(connections, server_id, packet, 1);
				CheckCharacterWhenJoinGame( &connections[server_id].chrlst );		// 캐릭터에 처음 조인 했을때의 체크할것들
				break;
			}
		case CMD_ALL_READY:
			{	//< CSD-HK-030829 : 이곳은 CMD_CHANGE_MAP 다시 들어 오는 부분.
				if (connections[cn].state == CONNECT_JOIN)
				{
					break;
				}
				
				LPCHARLIST ch = &connections[cn].chrlst;
				CheckCurrStats(ch);
				ResetAbility(ch);
				
				ch->MoveGox = ch->X	= packet->u.all_ready.x;
				ch->MoveGoy = ch->Y	= packet->u.all_ready.y;
				ch->MoveSx = ch->X/TILE_SIZE;
				ch->MoveSy = ch->Y/TILE_SIZE;
				ch->OldMoveSx = ch->X/TILE_SIZE;
				ch->OldMoveSy = ch->Y/TILE_SIZE;
				ch->attackedblock = 9;
				
				for (int i = 0 ; i < MAX_PC_CONTROL_NPC; ++i)
				{
					ch->ctrlnpc[i] = -1;
				}

				switch (connections[cn].state)
				{
				case CONNECT_LOGIN:
				case CONNECT_LOGIN_KEIN:
					{
						connections[cn].state= CONNECT_JOIN;
						InitJoinGameWho(ch);
						ch->updatable = 1;
						break;
					}
				default: 
					{	//< CSD-031129
						MyLog(0, "CMD_ALL_READY error [%s]", connections[cn].name);
						closeconnection(connections, cn, -303);
						return 1;
					}	//> CSD-031129
				}

				t_packet timepacket;
				timepacket.h.header.type = CMD_SERVER_TIME;
				timepacket.h.header.size = sizeof(t_ServerTime);
				DWORD servertime =
					+ (g_day* 86400) 
					+ (g_hour* 3600) 
					+ (g_min * 60) 
					+ g_sec;
				timepacket.u.ServerTime.nTime = servertime;				
				QueuePacket(connections, cn, &timepacket, 1);

				//< CSD-031107
				SendEnvironment(connections, cn);
				SendReadyMoveDate(cn);
				SetArea(ADD_PC_AREA, cn);
				RunArea(connections);
				//> CSD-031107
				packet->h.header.type = SealStoneBreakable;
				packet->h.header.size = 0;					
				QueuePacket(connections, cn, packet, 1);

				SendLog2ChrLogDB( connections, cn, CHR_LOG_LOGIN_ );
				CheckAndSendChangeEquip(ch, 1);	// 020620 YGI

				RandomWeatherSystem.SendWeather(cn); //Eleval 12/08/09 - Send the weather statut to the client when he logs on the map
				
				//All stuff below added by Eleval 07/09/09 so then if a Reporter char moves map, he stays in reporter mode on
					if( ch->IsReporter() || ch->IsCounselor())		// 기자가 아니다. //Eleval added GM in that so can check on normal client if needed
					if( ch->reporter_mode && ch->viewtype != VIEWTYPE_GHOST_ )		// 유령상태에서 유령상태로 가려고 할경우
					{
						ch->reporter_mode = 1 - ch->reporter_mode;

						if( ch->reporter_mode )		// 기자단 모드이면
						{
							// 유령으로 만들고.. 
							ch->Hp = 1;
							ch->bAlive = DEAD_;
							ch->viewtype = VIEWTYPE_GHOST_;
							ch->InitCurse(); 
							SendViewTypeChange( cn, VIEWTYPE_GHOST_ );
							ch->SendCharInfoBasic( HP, 0 );////020704 lsw
						}
						else
						{
							// 유령에서 정상으로 돌려 준다.
							SkillMgr.CharacterToAlive(ch,1);// 살려준다.//020501 lsw
						}
						SendPacketDefault( CMD_REPORTER_MODE, &ch->reporter_mode, sizeof( char ), cn );
					}
					//End of it

				// finito 09/08/07 
				t_packet friendpacket;
				friendpacket.h.header.type = CMD_FRIEND_STATUS;
				friendpacket.h.header.size = 0;
				QueuePacket(connections, DB_DEMON, &friendpacket, 1);

			#ifdef LMS_MAPSERVER
				RecvPartyAllFree(cn);
			#endif
				break;
			}	//> CSD-HK-030829
		case CMD_USER_REMOVE:
			{
				SetArea(REMOVE_PC_AREA, cn);
				connections[cn].state = CONNECT_LOGIN;
				break;
			}
		case CMD_MOVE:
			{
				RecvCMD_MOVE(cn,packet);
				break;
			}
		case CMD_ITEM_DROP:
			{		
				CHARLIST *ch = CheckServerId( cn );
				if( !ch ) break;
				// Item이 정말로 가지고 있는 Item인지 확인해야 한다. 
				t_client_item_drop *pi = &packet->u.client_item_drop;
				ItemAttr *item = &(pi->item);
				*item = ch->handheld;
				CItem *ii = ItemUnit( item->item_no );	// 그런 Item이 Table에 있는가?
				if( ii == NULL ) break;//021030 lsw
				// finito 040707 check for noexchange item kind
				if (IsNoExchange(item))
				{
					MoveEmptyInv( &ch->handheld, ch );
					POS pos;
					SetItemPos(HAND, &pos);
					SendServerEachItem(&pos, &ch->handheld, cn);					
					SendPutMenuString( KM_FAIL, 243, ch->GetServerID() );
					break;
				}
				// end of check				
				const int item_id = AddItemList( item->item_no, ch->handheld.attr, 0, pi->x, pi->y, 0, 0 );
				if(item_id < 0) break;
				//	손에 들은 아이템을 지워버린다.
				memset( &ch->handheld, 0, sizeof( ItemAttr ) );
				ch->Money = GetMoneyByItem( ch );		// 0430 YGI
				//CheckMoveItem( ch->Name, HAND, GROUND, pi->item, ch->Money);
				SendItemEventLog( item, cn, SN_NOT_USER, SILT_PUT_GROUND, 1 ); //YGI acer
				break;
			}
		case CMD_ITEM_TAKE:
			{
				CHARLIST *ch = ::CheckServerId( cn );
				if( !ch ) break;
				
				if(VIEWTYPE_OBSERVE_  == ch->viewtype)//관전모드 이면 아이템 안 집어짐
				{
					break;
				}
				
				const int item_id = packet->u.client_item_take.item_id;
				
				if( item_id < 0 || item_id >= MAX_ITEM_LIST ) break;
				if( ItemList[item_id].bAlive != ALIVE_ )	break;
				if( ItemList[item_id].attr[IATTR_ATTR] & IA2_NOTMOVE ) break;
				//<soto-030711 보물상자 추가 코드.(왼쪽 버튼을 아이템에 대고 다운시에 발생.)
				//단순히 보물 상자인가를 체크.
				CItem *pCItem = ItemUnit(ItemList[item_id].item_no);
				if(pCItem == NULL) break;
				if(TreasureBoxMgr()->IsTreasureBox(pCItem->GetItemKind()))//보물 상자 맞구요~
				{
					TreasureBoxMgr()->CreatTreasure(item_id, cn);		
					break; // 보불상자 부분을 처리 했으므로.다른 것은 처리 하지 않아도 된다.
				}
				//보물상자 아니면 진행.
				//>soto-030711
				//acer5
				int gab_x = abs( ch->X-ItemList[item_id].x );
				int gab_y = abs( ch->Y-ItemList[item_id].y );
				int total_gab = gab_x/16+1 + gab_y/16+1;
				if( total_gab > 8 ) break;		// 함이 9타일 이상이면 너무 멀리 떨어져 있다.
				// 추가된 부분	//1214
				if( ch->handheld.item_no ) MoveEmptyInv( &ch->handheld, ch );
				// 아아템을 손으로 셋팅	
				ch->handheld.item_no = ItemList[item_id].item_no ;
				memcpy(ch->handheld.attr, ItemList[item_id].attr, 6 * sizeof( DWORD )) ;
				ch->Money = GetMoneyByItem( ch );		// 0430 YGI
				//CheckMoveItem( ch->Name, GROUND, HAND, ch->handheld, ch->Money);
				SendItemEventLog( &ch->handheld, cn, SN_NOT_USER, SILT_GET_GROUND, 1 ); //YGI acer
				
				packet->h.header.type = CMD_YOU_GOT_ITEM;
				packet->u.server_you_got_item.item_id = item_id;
				packet->h.header.size = sizeof(t_server_you_got_item);
				QueuePacket(connections, cn, packet, 1);
				
				ItemList[item_id].bAlive = REMOVE_;
				SetArea(REMOVE_ITEM_AREA, item_id);
				
				break;
			}
		case CMD_REQ_LEARN_MAGIC:
			{
				g_pBattleManager->RecvMagicLearn(cn, &packet->u.kein.client_learn_magic);
				break;
			}
		case CMD_NPC_MAGIC:
			{
				g_pBattleManager->RecvNpcMagic(&packet->u.magic.client_npc_magic);
				break;
			}
		case CMD_MAGIC_SELECT:
			{	//< CSD-TW-030606
				//MyLog(LOG_NORMAL, "CMD_MAGIC_SELECT %d", packet->u.magic.client_magic_select.nMagic);
				g_pBattleManager->RecvMagicSelect(cn, &packet->u.magic.client_magic_select);
				break;
			}	//> CSD-TW-030606
		case CMD_MAGIC_CASTING:
			{	//< CSD-TW-030606
				//MyLog(LOG_NORMAL, "CMD_MAGIC_CASTING %d", packet->u.magic.client_magic_casting.nMagic);
				g_pBattleManager->RecvMagicCasting(cn, &packet->u.magic.client_magic_casting);
				break;
			}	//> CSD-TW-030606
		case CMD_MAGIC_EXECUTE:		
			{	//< CSD-TW-030606
				//MyLog(LOG_NORMAL, "CMD_MAGIC_EXECUTE %d", packet->u.magic.client_magic_casting.nMagic);
				g_pBattleManager->RecvMagicExecute(cn, &packet->u.magic.client_magic_execute);
				break;
			}	//> CSD-TW-030606
		case CMD_MAGIC_RESULT:
			{		
				//MyLog(LOG_NORMAL, "CMD_MAGIC_RESULT %d", packet->u.magic.client_magic_casting.nMagic);
				g_pBattleManager->RecvMagicResult(cn, &packet->u.magic.client_magic_result);
				break;
			}
		case CMD_MAGIC_RESULT_D:
			{ 			
				//MyLog(LOG_NORMAL, "CMD_MAGIC_RESULT_D %d", packet->u.magic.client_magic_casting.nMagic);
				g_pBattleManager->RecvMagicResult(&packet->u.magic.magic_result_d);
				break;
			}
		case CMD_COMBAT_CLEAR:
			{	//< CSD-TW-030606
				//MyLog(LOG_NORMAL, "CMD_COMBAT_CLEAR %d", packet->u.combat.client_combat_clear.nType);
				g_pBattleManager->RecvCombatClear(cn, &packet->u.combat.client_combat_clear);
				break;
			}	//> CSD-TW-030606
		case CMD_COMBAT_OBTAIN:
			{
				//MyLog(LOG_NORMAL, "CMD_COMBAT_OBTAIN %d", packet->u.combat.client_combat_obtain.nPoint);
				g_pBattleManager->RecvCombatObtain(cn, &packet->u.combat.client_combat_obtain);
				break;
			}
		case CMD_COMBAT_RESET:
			{
				//MyLog(LOG_NORMAL, "CMD_COMBAT_RESET");
				CHARLIST* ch = CheckServerId( cn );
				if( !ch ) break;
				if( !ch->IsCounselor() ) break;

				g_pBattleManager->RecvCombatReset(cn);
				break;
			}
		case CMD_COMBAT_REQUEST:
			{
				//MyLog(LOG_NORMAL, "CMD_COMBAT_REQUEST %d", packet->u.combat.client_combat_request.idMaster);
				g_pBattleManager->RecvCombatRequest(cn, &packet->u.combat.client_combat_request);
				break;
			}
		case CMD_COMBAT_LEARN:
			{
				//MyLog(LOG_NORMAL, "CMD_COMBAT_LEARN %d %d", packet->u.combat.client_combat_learn.nSkill, packet->u.combat.client_combat_learn.nPoint);
				g_pBattleManager->RecvCombatLearn(cn, &packet->u.combat.client_combat_learn);
				break;
			}
		case CMD_COMBAT_SELECT:
			{
				//MyLog(LOG_NORMAL, "CMD_COMBAT_SELECT %d", packet->u.combat.client_combat_select.nCombat);
				g_pBattleManager->RecvCombatSelect(cn, &packet->u.combat.client_combat_select);
				break;
			}
		case CMD_COMBAT_ATTACK:
			{
				//MyLog(LOG_NORMAL, "CMD_COMBAT_ATTACK %d", packet->u.combat.client_combat_attack.nSkill);
				g_pBattleManager->RecvCombatAttack(cn, &packet->u.combat.client_combat_attack);
				break;
			}
		case CMD_COMBAT_RESULT:
			{
				//MyLog(LOG_NORMAL, "CMD_COMBAT_RESULT %d", packet->u.combat.client_combat_result.nSkill);
				g_pBattleManager->RecvCombatResult(cn, &packet->u.combat.client_combat_result);
				break;
			}
		case CMD_CLIENT_CHARACTER_DEMAND :	RecvCMD_CLIENT_CHARACTER_DEMAND( cn, &packet->u.angra.client_character_demand);
			break;
			
		case CMD_CHANGE_EQUIP:
			{
				
				break; // 0713 YKI
				
				unsigned char equip[4] = {0, } ;							// 0623 YGI
				SetShape( equip, cn );										// 0623 YGI		// 서버에서 클라이언트 장착화면을 보내준다.
				
				//					memcpy(equip, packet->u.client_change_equip.equip, 4);
				memcpy(connections[cn].chrlst.accessory, equip, 4);
				
				packet->h.header.type = CMD_CHANGE_EQUIP;
				packet->h.header.size = sizeof(t_server_change_equip);
				packet->u.server_change_equip.server_id= cn;
				memcpy(packet->u.server_change_equip.equip, equip, 4);
				CastMe2Other( cn, packet);
				break ;
			}
		case CMD_ATTACK:
			{
				g_pBattleManager->RecvStrikeAttack(cn, &packet->u.strike.player_attack);
				break;
			}        
		case CMD_NPC_ATTACK:	
			{
				g_pBattleManager->RecvNpcAttack(&packet->u.strike.npc_attack);
				break;
			}
		case CMD_THROW_ATTACK:
			{
				//MyLog(LOG_NORMAL, "CMD_THROW_ATTACK %s", connections[cn].name);
				g_pBattleManager->RecvThrowAttack(cn, &packet->u.strike.client_throw_attack);
				break;
			}
		case CMD_THROW_RESULT:
			{
				//MyLog(LOG_NORMAL, "CMD_THROW_RESULT %s", connections[cn].name);
				g_pBattleManager->RecvThrowResult(cn, &packet->u.strike.client_throw_result);
				break;
			}
		case CMD_EFFECT_RESULT:
			{	//< CSD-031007
				//MyLog(LOG_NORMAL, "CMD_EFFECT_RESULT %d", packet->u.strike.client_effect_result.wdEffect);
				g_pBattleManager->RecvEffectResult(cn, &packet->u.strike.client_effect_result);
				break;
			}	//> CSD-031007
		case CMD_CHAT_DATA :
			{//021126 lsw
				CHARLIST *ch = CheckServerId(cn);
				
				if (!ch)
				{
					break;
				}
				
				::CheckChatingData( ch->Name, packet->u.client_chat_data.text, packet->u.client_chat_data.type);
				::ConvString( packet->u.client_chat_data.text );
				ch->attackedblock = 0;
				
				switch (packet->u.client_chat_data.type)
				{
				case CHATTYPE_WHISPER:
				case CHATTYPE_PARTY:		
					{
						::SendChatParty(connections, cn, packet);
						break;
					}
				case CHATTYPE_GUILD:
					{
						::SendWhisper(connections,cn, packet);
						break;
					}
				case CHATTYPE_NORMAL:
				case CHATTYPE_MERCHANT_BBS:
					{
						::SendChatArea(cn , packet);
						break;
					}
				case CHATTYPE_COLOSSUS_GLOBAL:
					{
						::SendFightMapChat(cn, packet);
						break;
					}
				case CHATTYPE_COLOSSUS:
					{
						::SendFightMapChatMyTeam( cn, packet );
						break;
					}
				}

				break;	
			}
		case CMD_EVENT:
			{
				RecvEvent(&packet->u.client_event, cn);
				break;
			}				
		case CMD_ITEM_EXCHANGE:		
			{
				CHARLIST *ch = CheckServerId( cn );
				if( !ch ) break;
				if( connections[cn].state < CONNECT_JOIN ) break;
				
				if( exist(packet->u.item_ex_info.itemno, cn, packet->u.item_ex_info.pos_s) ) 
				{ // if item exists
					if(exchangePOS_of_Item(cn, &packet->u.chr_status_info, packet->u.item_ex_info.pos_s, packet->u.item_ex_info.pos_t) ) 
					{
						ch->Money = GetMoneyByItem( ch );		// 0111
						CheckAndSendChangeEquip( ch );// 0713 YGI
						break;
					} 
				}		
				// 실패
				packet->h.header.type= CMD_ITEM_EXCHANGE_REFUSE ;
				packet->h.header.size = sizeof(t_header) ;
				QueuePacket(connections, cn, packet, 1) ;
				break;
			}
		case CMD_INTERFACE_BATTLE:
			{
				connections[ cn].chrlst.Peacests= 1 ;
				packet->h.header.type = CMD_INTERFACE_BATTLE;
				packet->u.server_interface_battle.client_id = cn; 
				packet->h.header.size = sizeof( t_server_interface_battle);
				CastMe2Other( cn, packet );
				break ;
			}
		case CMD_INTERFACE_PEACE:
			{
				connections[ cn].chrlst.Peacests = 0;
				packet->h.header.type = CMD_INTERFACE_PEACE;
				packet->u.server_interface_peace.client_id = cn; 
				packet->h.header.size = sizeof( t_server_interface_peace);
				CastMe2Other( cn, packet );
				break ;
			}
		case CMD_LEVELUP_POINT:
			{
				RecvLevelUpPoint(cn, &packet->u.client_levelup_point);
				break;
			}
		case CMD_CALL_SMALLMENU:
			{
				CallSmallMenu(cn, packet->u.server_call_smallmenu.storelistindex, packet->u.server_call_smallmenu.menu);
				break;
			}
		case CMD_DUAL_MAGIC_LEARN_BOOK: 
			{	//<soto-030421
				SendCallDualMagicLearnMenu(cn);
				break;
			}	//>soto-030421
		case CMD_OPEN_LUCKY_MENU:
			{
				RecvCallLuckyMenuOpen(cn); //soto-030429
				break;
			}
		case CMD_OPEN_WINNER_MENU:
			{
				if (LottoSystem()) 
				{
					LottoSystem()->RecvOpenWinnerMenu(cn);//soto-030502
				}

				break;
			}
		case CMD_CHECK_WINNER_MENU:
			{
				if (LottoSystem())
				{
					LottoSystem()->RecvCheckOpenWinnerMenu(&packet->u.Lotto_Winner_Menu);
				}

				break;
			}
			//<soto-030504
		case CMD_NEW_EVENT:
			{
				if (LottoSystem())
				{
					LottoSystem()->RecvDBNewEvent(&packet->u.Lotto_Info);
				}

				break;
			}
		case CMD_LOTTO_BUY:
			{
				if(LottoSystem())
				{
					LottoSystem()->RecvBuyLotto(&packet->u.Lotto_Buy);//C
				}

				break;
			}
		case CMD_CAN_BUY:
			{
				if(LottoSystem())
				{
					LottoSystem()->RecvCanBuyLotto(&packet->u.Lotto_Buy);//DB
				}

				break;
			}
		case CMD_WINNER_CHECK:
			{
				if(LottoSystem())
				{
					LottoSystem()->RecvCheckWinner(cn);
				}

				break;
			}
		case CMD_CHECK_OK:
			{
				if(LottoSystem())
				{
					LottoSystem()->RecvCheckOKWinner(&packet->u.Check_Winner);
				}

				break;
			}
		//<! BBD 040127		로또확인 절차의 마지막 프로세스	
		case CMD_DEL_LOTTO_USER_OK:
			{
				if(LottoSystem())
				{
					LottoSystem()->CheckIsDelOk(&packet->u.Lotto_Del_Ok.bIsDelOK);
				}

				break;
			}
		//> BBD 040127		로또확인 절차의 마지막 프로세스	
		case CMD_LOTTERY_INFO:
			{
				if(LottoSystem())
				{
					LottoSystem()->RecvLottoInfo(&packet->u.Lotto_Info);
				}

				break;		
			}
			//>soto-030504
		case CMD_OPEN_LOTTOMENU://soto-030520
			{
				if(LottoSystem())
				{
					LottoSystem()->RecvOpenLottoMenu(cn);
				}
			}
			break;
		case CMD_ITEM_BUY_REQ:
			{
				RecvItemBuy(cn, &packet->u.client_item_buy);
				break;
			}
		case CMD_ITEM_SELL_REQ:
			{
				RecvItemSell(cn, &packet->u.client_item_sell);
				break;
			}
		case CMD_SKILL_INFO:
			{
				RecvSkillInfo( cn, &packet->u.client_skill_info );
				break;
			}
		case CMD_SKILL_CURSOR_VIEW:
			{
				RecvSkillCursorView( cn, &packet->u.client_skill_cursor_view );
				break;
			}
		case CMD_SKILL_GAUGE:
			{
				RecvSkillGauge( cn, &packet->u.client_skill_gauge );
				break;
			}
		case CMD_FOOT_ON_TRAP_0_ITEM:
			{
				RecvCMD_FOOT_ON_TRAP_0_ITEM( cn, &packet->u.client_foot_on_trap0 );
				break;
			}
		case CMD_FOOT_ON_TRAP_1_ITEM:
			{
				RecvCMD_FOOT_ON_TRAP_1_ITEM(  cn, &packet->u.client_foot_on_trap1 );
				break;
			}
		case CMD_MODIFY_POSITION:
			{
				if( cn < DRAGON_CONNECTIONS_START || cn >= DRAGON_MAX_CONNECTIONS  ) 
				{
				}
				else
				{
					if( connections[cn].state < CONNECT_JOIN ) break;
					RecvCMD_MODIFY_POSITION( cn, &packet->u.client_modify_position );
				}

				break;
			}
		case CMD_ITEM_DOOR_OPEN_CLOSE:
			{
				RecvItemDoorOpenClose(cn, &packet->u.item_door_open_close);
				break;
			}
		case CMD_ITEM_BOX_BREAK:
			{
				RecvCMD_ITEM_BOX_BREAK( cn, &packet->u.client_item_box_break );
				break;
			}
		case CMD_JUST_ATTACK_ANIMATION:
			{
				RecvCMD_JUST_ATTACK_ANIMATION( cn, &packet->u.client_just_attack_animation );
				break;
			}
		case CMD_ITEM_BOX_BREAK_RESULT:
			{
				RecvCMD_ITEM_BOX_BREAK_RESULT( cn, &packet->u.client_item_box_break_result );
				break;
			}
		case CMD_ITEM_BOX_MAGIC_BREAK:
			{
				RecvCMD_ITEM_BOX_MAGIC_BREAK(cn, &packet->u.client_item_box_magic_break);
				break;
			}
		case CMD_ROOF_OPEN:
			{
				RecvRoofOpen( cn, packet->u.roof_open.roofno );
				break;
			}
		case CMD_ROOF_CLOSE:
			{
				RecvRoofClose(cn, packet->u.roof_close.roofno);
				break;
			}
		case CMD_CHAT_SET:
			{
				RecvChatSet(cn, &packet->u.chat_set);
				break;
			}
		case CMD_TOTAL_MAP_CONNECTIONS:
			{
				RecvTotalMapConnections( cn );
				break;
			}
		case CMD_TOTAL_CONNECTIONS:
			{
				RecvTotalConnections( cn );
				break;
			}
		case CMD_ISTHERE_CHARNAME:
			{
				SendCheckName(cn, packet->u.client_isthere_charname.name);
				break;		// 1004 YGI
			}
		case CMD_TAME_COMMAND:
			{
				RecvTameCommand(cn, &packet->u.tame_command);
				break;
			}
		case CMD_DEBUG_REQ_CHAR_INFO:
			{
				// RecvDebugReqCharInfo( cn, packet->u.debug_client_req_char_info.id );
				break;
			}
		case CMD_PLEASE_GIVE_LIFE:
			{
				Recv_CMD_PLEASE_GIVE_LIFE(cn, packet->u.kein.default_char);
				break;		// 010502 YGI
			}
		case CMD_PLEASE_GIVE_LIFE1:
			{
				Recv_CMD_PLEASE_GIVE_LIFE_GM(cn, cn);
				break;	// 010502 YGI
			}
		case CMD_QUEST_JOB_FINISH:	
			{	// lhs 0810 퀘스트 잡 피니쉬 직업인의 서약
				Recv_CMD_QUEST_JOB_FINISH( cn,1 );
				break;
			}	
		case CMD_SMILE_FACE:
			{
				RecvSmileFace( cn, packet->u.client_smile_face.smileno );
				break;
			}
		// -----------------운영자용 명령 packet-> //acer7
		case CMD_BLOCK_ATTACK_START:
			{
				RecvBlockAttack( cn, 1 );
				break;
			}
		case CMD_BLOCK_ATTACK_END:
			{
				RecvBlockAttack( cn, 0 );
				break;
			}
		case CMD_BLOCK_MOVE_START:
			{
				RecvBlockMove( cn, 1 );
				break;
			}
		case CMD_BLOCK_MOVE_END:
			{
				RecvBlockMove( cn, 0 );
				break;
			}
		case CMD_BLOCK_MAGIC_START:
			{
				RecvBlockMagic ( cn, 1 );
				break;
			}
		case CMD_BLOCK_MAGIC_END:
			{
				RecvBlockMagic ( cn, 0 );
				break;
			}
		case CMD_BLOCK_GIVE_LIFE_START:
			{
				RecvBlockGiveLife( cn, 1 );
				break;
			}
		case CMD_BLOCK_GIVE_LIFE_END:
			{
				RecvBlockGiveLife( cn, 0 ); 
				break;
			}
		case CMD_BLOCK_SKILL_START:
			{
				RecvBlockSkill ( cn, 1 );
				break;//020821 lsw
			}
		case CMD_BLOCK_SKILL_END:
			{
				RecvBlockSkill ( cn, 0 );
				break;//020821 lsw
			}
		case CMD_SUMMON_MONSTER:
			{
				RecvSummonMonster(cn,
								  packet->u.summon_monster.id,
								  packet->u.summon_monster.amount,
								  packet->u.summon_monster.x,
								  packet->u.summon_monster.y,
								  packet->u.summon_monster.spread,
								  packet->u.summon_monster.summon);
				break;
			}
		case CMD_SUMMON_KILLALL:
			{
				RecvSummonKillall(cn);
				break;
			}
		case CMD_REMOVE_NK:
			{
				RecvRemoveNK(cn, packet->u.remove_nk.id);
				break;
			}
		case CMD_ABSOLUTE_LOGOUT:
			{
				RecvAbsoluteLogOut( cn, packet->u.absolute_logout.id );
				break;
			}
		case CMD_MAKE_GHOST:
			{
				RecvMakeGhost( cn, packet->u.make_ghost.id );
				break;
			}
		case CMD_GIVE_LIFE:
			{
				Recv_CMD_PLEASE_GIVE_LIFE_GM(cn, packet->u.give_life.id);
				break;
			}
		case CMD_ABSOLUTE_MOVE_DUNGEON_KEEPER:
			{
				Recv_CMD_ABSOLUTE_MOVE_DUNGEON_KEEPER( packet );
				break;
			}
		case CMD_BBS:
			{
				CHARLIST* ch = CheckServerId( cn );
				if( !ch ) break;
				if( !ch->IsCounselor() ) break;
				BroadCastBBS_Sub( packet->u.server_bbs.bbs, packet->h.header.size );
				break;
			}
		case CMD_BBS_RMTOOL: //Eleval 21/07/09 - To Re-Allow RMTool to broadcast messages
			{
				CHARLIST* ch = CheckServerId( cn );
				if( ch ) break;
				BroadCastBBS_Sub( packet->u.server_bbs.bbs, packet->h.header.size );
				break;
			}
		case CMD_NPC_CHAT_DATA:
			{
				SendNPCChatArea(packet->u.npc_chat_data.id -10000, packet->u.npc_chat_data.data);
				break;
			}
		case CMD_NPC_MODIFY_POS:
			{
				RecvNPCModifyPos( cn, packet->u.npc_modify_pos.id, packet->u.npc_modify_pos.mx,packet->u.npc_modify_pos.my );
				break;
			}
		case CMD_SEND_NPC_PARAMETER:
			{
				RecvNPC_Parameter(packet->u.send_npc_parameter.id, 
								  packet->u.send_npc_parameter.type, 
								  packet->u.send_npc_parameter.data);
				break;
			}								
		case CMD_SEND_NPC_STRING_PARAMETER:
			{
				RecvNPC_StringParameter(packet->u.send_npc_string_parameter.id, 
										packet->u.send_npc_string_parameter.type, 
										packet->u.send_npc_string_parameter.data );
				break;
			}
			// Agent->MapServer
		case CMD_LOGIN_BBS:
			{
				BroadCastBBS_Sub( packet->u.login_bbs.bbs, packet->h.header.size);
				break;
			}
		case CMD_CLOSE_LOGIN_ID:
			{
				RecvAbsLogOut2GameServer( packet->u.login_close_login_id.id );
				break;
			}
		case   CMD_SERVER_ITEMTABLE_RELOAD:
			{
				initItem();
				LoadItemJoinTable();
				break;
			}
		case CMD_DEBUG_CHANGE_SKILL_EXP:
			{
				RecvDebugChangeSkillExp( &packet->u.debug_change_skill_exp );
				break;
			}
		case CMD_SERVER_NPC_ACCESS_TABLE_RELOAD:
			{
				Recv_CMD_SERVER_NPC_ACCESS_TABLE_RELOAD( cn, packet );
				break;							
			}
		case CMD_SERVER_FIGHT_STONE:
			{
				Recv_fight_stone(cn, *packet);
				break;
			}
		case CMD_SEALSTONE_STATUS:
			{
				Recv_CMD_SEALSTONE_STATUS(&packet->u.sealstone_status);
				break;
			}
		case CMD_SEALSTONE_WAR_START:
			{
				Recv_CMD_SEALSTONE_WAR_START(packet->u.sealstone_war_start.nation);
				break;
			}
		case CMD_SEALSTONE_RESULT:
			{
				Recv_CMD_SEALSTONE_RESULT(&packet->u.sealstone_result);
				break;
			}
		case CMD_SEALSTONE_WAR_LEFTTIME:
			{
				Recv_CMD_SEALSTONE_WAR_LEFTTIME(packet->u.sealstone_warlefttime.nation,
					packet->u.sealstone_warlefttime.lefttime);
				break;
			}								
		case CMD_STEALSTONE_BREAKABLE:
			{	//< CSD-CN-031213
				g_pUserManager->SendPacket(packet);
				SealStoneBreakable = CMD_STEALSTONE_BREAKABLE;
				g_LocalWarBegin=packet->u.LocalWar.CommonDataC.Data; // 011215 LTS
				break;
			}	//> CSD-CN-031213
		case CMD_STEALSTONE_NON_BREAKABLE_PEACE:
			{	//< CSD-CN-031213
				g_pUserManager->SendPacket(packet);
				SealStoneBreakable = CMD_STEALSTONE_NON_BREAKABLE_PEACE;
				g_LocalWarBegin=packet->u.LocalWar.CommonDataC.Data;  // 011215 LTS
				break;
			}	//> CSD-CN-031213
			
		case CMD_STEALSTONE_NON_BREAKABLE:
			{	//< CSD-CN-031213
				g_pUserManager->SendPacket(packet);
				SealStoneBreakable = CMD_STEALSTONE_NON_BREAKABLE;	
				g_LocalWarBegin=packet->u.LocalWar.CommonDataC.Data;	// 011215 LTS													
				break;									
			}	//> CSD-CN-031213
		case CMD_NATION2NATION_RELATION_RESULT:
			{
				RecvNation2NationRelationResult(packet);
				break;
			}
		case CMD_NATION2NATION_RELATION:
			{
				RecvNation2NationRelation(cn, &(packet->u.nation2nation_relation));
				break;
			}
		case CMD_JUST_ANI:
			{
				Recv_JustAni(packet);
				break;
			}
		case CMD_FACE_DIRECTIONS:
			{
				CastMe2Other(cn, packet);
				break;
			}
		case CMD_INSTALL_TYPE:
			{
				connections[ cn].chrlst.installType = packet->u.client_install_type.type;
				break;
			}
		case CMD_CHECK_ACCEL:
			{
				CrackMgr.ComformAccelator(cn, packet);
				break;
			}
		case CMD_SEND_ACCEL_TYPE:
			{	//< CSD-030808
				CrackMgr.RecvAccelType(cn,packet);
				break;
			}	//> CSD-030808
		case CMD_SKILL_RARE_UPGRADE:
			{
				RecvCMD_SKILL_RARE_UPGRADE(cn,packet);
				break;
			}
		case CMD_DUAL_ENABLE:
			{ //< CSD-021003
				g_pDualManager->RecvDualEnable(cn, &packet->u.dual.client_dual_enable);
				break;
			} //> CSD-021003
		case CMD_DUAL_CHANGE:
			{
				g_pDualManager->RecvDualChange(cn, &packet->u.dual.client_dual_change);
				break;
			}
		case CMD_DUAL_DIVIDE:
			{
				g_pDualManager->RecvDualDivide(cn, &packet->u.dual.client_dual_divide);
				break;
			}
		case CMD_RESET_ABILITY:
			{
				g_pDualManager->RecvResetAbility(cn);
				break;
			}
		case CMD_RARE_UPGRADE_BBS_MAP_TO_MAP:
			{	//< CSD-CN-031213
				//packet->h.header.type = CMD_RARE_UPGRADE_BBS;  finito 29/08/07 removed!
				//g_pUserManager->SendPacket(packet);
				break;
			}	//> CSD-CN-031213
		case CMD_LOTTERY_BROADCAST:
			{	//< CSD-CN-031213	
				g_pUserManager->SendPacket(packet);
				break;
			}	//> CSD-CN-031213
//<soto-Lotto추가.
		case CMD_LOTTO_SEEK:
			{
				if(cn == DB_DEMON)
				{
					int nCn = ::ExistHe(packet->u.Lotto_Seek.strChrName);
					
					QueuePacket(connections,nCn,packet,1);
				}
				else
				{
					char*	pName = connections[cn].name;
					if(pName)
					{
						strcpy_s(packet->u.Lotto_Seek.strChrName, sizeof(packet->u.Lotto_Seek.strChrName),pName);
						packet->u.Lotto_Seek.nLottoID = LottoSystem()->GetLottoID();

						QueuePacket(connections,DB_DEMON,packet,1);
					}
				}
			}
			break;
		case CMD_FRIEND_STATUS:
			{
				if(cn == -1)
				{
					RecvFriendStatus(packet);
				}
			}
			break;
		case CMD_DUEL_ALLOWDENY:
			{
				CHARLIST *ch = GetCharListPtr(cn);
				if (ch)
				{
					ch->m_bIsDuelAllow = packet->u.DuelAllowDeny.bIsDuelAllow;
				}
			}
			break;
		case CMD_MALLITEM_BUY:
			{
				RecvMallItemBuy(cn, &packet->u.MallItemBuy);
			}
//>soto-Lotto추가.
		default:
			{
				if (1 == CheckHandleByKein(packet, connections, cn))
				{
					break;
				}

				if (HandleCommand2(packet, connections, cn))
				{
					break;
				}

				if (HandleCommand3(packet, connections, cn))
				{
					break;
				}

				if (1 == CheckHandleByNationWar(packet,connections,cn))
				{
					break;
				}
				else
				{
					::MyLog( LOG_NORMAL, "Illegal Packet (type:%d)(size:%d)(cn:%d)", packet->h.header.type, packet->h.header.size, cn );
					return 0;
				}

				break;
			}
	}

	return 1;
}

extern bool DungeonEventTileProc( int event_no, short int cn );

void RecvEvent( t_client_event *pEvent, short int cn )
{	//< CSD-030506
	if (!CheckServerId(cn))
	{
		return;
	}
	
	if (sRECALL_SCRP_NO) 
	{
		SendPutMenuString( KM_FAIL, 216, cn );	// 031110 YGI // 용오와 상의
		return; // 현재 스크립트가 진행중이다. 
	}
	
	EventPC = cn;
	
	switch (pEvent->type)
	{
	case EVENT_TYPE_NPC:
		{
			int ttt = pEvent->event_no - 10000;
			if (ttt < NPC_LIST_START || ttt >= MAX_NPC_LIST) break;
			if (NPCList[ ttt].Race == SEALSTONE)  break;
			if (NPCList[ ttt].IsTamedNpc())  break;
			if (g_EventMgr.CheckScriptNo(ttt, cn)) break;		// 이벤트에서 먼저 적용됨
			
			EventNPC[EventPC] = ttt;
			const int eventno = NPCList[EventNPC[EventPC]].eventno;
			
			if (0 <= eventno && eventno < EVENT_NPC_FUNCTION_TOTAL)
			{
				Script(eventno + EVENT_NPC_FUCTION_START);
			}
			
			break;
		}
	case EVENT_TYPE_MAPTILE:
		{
			const int eventno = pEvent->event_no;
			
			if (0 > eventno) 
			{
				break;
			}
			
			if (DungeonEventTileProc(eventno, cn))
			{
				break;
			}
			
			Script(eventno + EVENT_MAPTILE_FUCTION_START);
			break;
		}
	default : break;
	}
}	//> CSD-030506

void RecvNewDay( t_packet *packet )
{
	WORD wDay =	packet->u.send_map_change_date.wDay;
	WORD wMonth = packet->u.send_map_change_date.wMonth;
	//날(Day)가 변경되었으면 1, 달(Month)가 변경되었으면 2, 둘다 변경되었으면 3
	BYTE bChanged = packet->u.send_map_change_date.bChanged;

	static int save_day = 0;
	static int save_month = 0;

	if( wDay == save_day && wMonth == save_month ) return;

	save_day = wDay;
	save_month = wMonth;
	
	InitDataFromNewDay( wDay, wMonth );
	//SendAllProtocal( packet );
	
	RefreshTodayItem();
	LoadNationItem( g_wday );

	//localWarMsgDebug++; //Eleval - To ++ the thing that will remove the lwm after 3 days
	
	MyLog(0, "Start New Day ^^ : by kein " );

	int w_type = RandomWeatherSystem.SetRandomWeatherTypeOfTheDay(); //Define what kind of day it gonna be

	switch(w_type)
	{
	case WK_GOOD:
		MyLog(LOG_NORMAL, "Today is going to be a Good day !");
		break;
	case WK_RAIN:
		MyLog(LOG_NORMAL, "Today is going to be a Rainy day !");
		break;
	case WK_SNOW:
		MyLog(LOG_NORMAL, "Today is going to be a Snowy day !");
		break;
	case WK_THUNDER:
		MyLog(LOG_NORMAL, "Today is going to be a Thunder day !");
		break;
	}
}

WORD g_wCurDay;			//현재 날짜를 저장 하는 전역변수 
WORD g_wCurMonth;		//현재 달(month)을  저장 하는 전역변수 

void SetCurrentDate()
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	g_wCurMonth = time.wMonth;
	g_wCurDay = time.wDay;
}
void CheckDay()
{
	static int first;
	if( !first ) 
	{
		SetCurrentDate();
		first = 1;
	}
	
	static int curr_time = g_curr_time;
	if( g_curr_time - curr_time < 300 ) return;		// 5분

	curr_time = g_curr_time;

	SYSTEMTIME time;
	GetLocalTime(&time);
	BYTE	bChanged = 1;		//날(day)만 변경되었으면 1, 날과 달(month) 둘다 변경 되었으면 2

	//달이 바뀌었을때
	if(time.wMonth != g_wCurMonth)
	{
		g_wCurMonth = time.wMonth;		//달(month) 갱신 
		bChanged++;
	}

	// 030929 kyo <<
	static bool IsKickoffLimitedUser = false;
	if( LocalMgr.IsAbleNation(THAI) )
	{
		if( g_nLimitedStartTime == time.wHour && false == IsKickoffLimitedUser)
		{
			CheckLimitedAge();	
			IsKickoffLimitedUser = true;
		}
		else
			IsKickoffLimitedUser = false;
	}
	// 030929 kyo >>

	//날짜가 바뀌었을때 
	if(time.wDay != g_wCurDay)
	{
		t_packet packet;
		g_wCurDay = time.wDay;

		packet.u.send_map_change_date.wDay = time.wDay;
		packet.u.send_map_change_date.wMonth = time.wMonth;
		RecvNewDay( &packet );		// 날자 리플레시
	}
}
void KickOffLimitedUser( const int cn)	// 030929 kyo
{//메시지를 남기고 유저 접속을 종료한다.
	//leave log // add plz......
	g_QuestInMap.ShowStateMsg( cn, g_szLimitedUser_Message, 255, 255, 0 ); //yellow
	::closeconnection( connections, cn, 100 );
}

void CheckLimitedAge()	// 030929 kyo
{	//< CSD-CN-031213 : 모든 유저의 접속을 확인해서 제한나이 미만이면 접속을 종료시킨다.
	CUserManager::HASH_USER mpUser = g_pUserManager->GetUserSet();

	for (CUserManager::ITOR_USER i = mpUser.begin(); i != mpUser.end(); ++i)
	{
		CHARLIST* pUser = CheckServerId(i->second);

		if (pUser != NULL && pUser->IsLimitedAge(g_nLimitedAge))
		{
			KickOffLimitedUser(pUser->GetServerID());
		}
	}
}	//> CSD-CN-031213

void InitLimitedTimeAndAge() // 030929 kyo
{// 제한나이와 시간, 출력시킬 메시지를 가져온다.
	char	NetworkDir[MAX_PATH];
	char	MapServerConfigFileName[MAX_PATH];

	if (GetPrivateProfileString("Network","path","",NetworkDir,MAX_PATH,MAP_SERVER_INI_)<=0)
	{
		MyLog(0,"Error!! Check Mapserver.ini's Path Set.");
		return ;
	}

	sprintf_s(MapServerConfigFileName, sizeof(MapServerConfigFileName),"%s/data/MapServerConfig.ini",NetworkDir);

	g_nLimitedStartTime	=::GetPrivateProfileInt("Thai Limited","StartTime",22,MapServerConfigFileName);	
	g_nLimitedEndTime	=::GetPrivateProfileInt("Thai Limited","EndTime",6,MapServerConfigFileName);	
	g_nLimitedAge		=::GetPrivateProfileInt("Thai Limited","Age",18,MapServerConfigFileName);			
	::GetPrivateProfileString( "Thai Limited", "Message", "", g_szLimitedUser_Message, sizeof( g_szLimitedUser_Message ), MapServerConfigFileName );
}

void CheckMallItemTimeElapsed()
{
	DWORD time = g_curr_time;

	ItemAttr *item;
	POS pos;
	CUserManager::HASH_USER mpUser = g_pUserManager->GetUserSet();

	for (CUserManager::ITOR_USER i = mpUser.begin(); i != mpUser.end(); ++i)
	{
		const int idUser = i->second;
		CHARLIST* pUser = CheckServerId(idUser);
		
		if (pUser != NULL)
		{
			for(int i = 0;i < 3; i++ ) // Inv
			{
				for(int j = 0; j < 3; j++ ) 
				{
					for(int k = 0; k < 8; k++ ) 
					{
						item = &pUser->inv[i][j][k];
						if (item->attr[IATTR_MALL_TIMED] > 0)
						{
							CItem* t = ItemUnit(item->item_no);
							if (t && t->GetItemKind() != IK_TREASURE_MAP)
							{
								if (time >= item->attr[IATTR_ATTR])
								{
									SendDeleteItemQuantity(pUser, item->item_no, 1);
									//MyLog(LOG_NORMAL, "Item deleted due to end of time..."); //Eleval 19/08/09 - TEST EXP PILLS
								}
								else
								{
									if (item->attr[IATTR_MALL_TIMED] > 1)
									{
										--item->attr[IATTR_MALL_TIMED];
									}
								}
							}
						}
					}
				}
			}

			for(int a=0; a<ITEM_EQUIP_MAX; a++ )
			{
				item = &pUser->equip[a];
				if (item->attr[IATTR_MALL_TIMED] > 0)
				{
					CItem* t = ItemUnit(item->item_no);
					if (t && t->GetItemKind() != IK_TREASURE_MAP)
					{
						if (time >= item->attr[IATTR_ATTR])
						{
							POS pos;
							SetItemPos( EQUIP, a, &pos );
							SendDeleteItem(&pUser->equip[a], &pos, pUser);
							//MyLog(LOG_NORMAL, "Item deleted due to end of time..."); //Eleval 19/08/09 - TEST EXP PILLS
							CheckAndSendChangeEquip(pUser, 1);
						}
						else
						{
							if (item->attr[IATTR_MALL_TIMED] > 1)
							{
								--item->attr[IATTR_MALL_TIMED];
							}
						}
					}
				}
			}
			
			for(int a=0; a<ITEM_QUICK_MAX ; a++ )
			{
				item =  &pUser->quick[a];
				if (item->attr[IATTR_MALL_TIMED] > 0)
				{
					CItem* t = ItemUnit(item->item_no);
					if (t && t->GetItemKind() != IK_TREASURE_MAP)
					{
						if (time >= item->attr[IATTR_ATTR])
						{
							POS pos;
							SetItemPos( QUICK, a, &pos );
							SendDeleteItem(&pUser->quick[a], &pos, pUser);
							//MyLog(LOG_NORMAL, "Item deleted due to end of time..."); //Eleval 19/08/09 - TEST EXP PILLS
						}
						else
						{
							if (item->attr[IATTR_MALL_TIMED] > 1)
							{
								--item->attr[IATTR_MALL_TIMED];
							}
						}
					}
				}
			}

			for(int a=0; a<5; a++ )
			for(int b=0; b<3; b++ )
			for(int c=0; c<6; c++ )
			{
				item = &pUser->bank[a][b][c];
				if (item->attr[IATTR_MALL_TIMED] > 0)
				{
					CItem* t = ItemUnit(item->item_no);
					if (t && t->GetItemKind() != IK_TREASURE_MAP)
					{
						if (time >= item->attr[IATTR_ATTR])
						{
							POS pos;
							SetItemPos( BANK, a, b, c, &pos );
							SendDeleteItem(&pUser->bank[a][b][c], &pos, pUser);
							//MyLog(LOG_NORMAL, "Item deleted due to end of time..."); //Eleval 19/08/09 - TEST EXP PILLS
						}
						else
						{
							if (item->attr[IATTR_MALL_TIMED] > 1)
							{
								--item->attr[IATTR_MALL_TIMED];
							}
						}
					}
				}
			}

			item = &pUser->handheld;
			if (item->attr[IATTR_MALL_TIMED] > 0)
			{
				CItem* t = ItemUnit(item->item_no);
				if (t && t->GetItemKind() != IK_TREASURE_MAP)
				{
					if (time >= item->attr[IATTR_ATTR])
					{
						POS pos;
						SetItemPos( HAND, &pos );
						SendDeleteItem(&pUser->handheld, &pos, pUser);
						//MyLog(LOG_NORMAL, "Item deleted due to end of time..."); //Eleval 19/08/09 - TEST EXP PILLS
					}
					else
					{
						if (item->attr[IATTR_MALL_TIMED] > 1)
						{
							--item->attr[IATTR_MALL_TIMED];
						}
					}
				}
			}
		}
	}	
}