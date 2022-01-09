#include "recvmsg.h"
#include "servertable.h"
//#include "playermanager.h"
#include "monitor.h"
#include "servertable.h"
#include "mylog.h"
#include "..\mapserver.h"
#include <sqltypes.h>
#include "..\HigherLayers\defaultheader.h"
#include "..\HigherLayers\dr_network.h"
#include "..\HigherLayers\MenuServer.h"
#include "..\HigherLayers\Map.h"
// 001206 KHS
#include "..\HigherLayers\SealStone.h"
#include "..\HigherLayers\SealStoneGameServer.h"
#include "DrServerManager.h"
#include "..\HigherLayers\eventmgr.h"		//021030 YGI
#include "..\HigherLayers\UserManager.h"
#include "../HigherLayers/KickOffAll.h"		// BBD 040110

extern int g_LocalWarBegin;		// 011215 LTS
extern CKickOffAll g_KickMgr;		// BBD 040110
// 011205 KBS
extern void RMProc(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
extern void UpdateCharacterData(t_connection c[], int cn, bool bDirect = false);
extern void CleanUpConnection(short idServer);

extern unsigned short	g_UsercountInServerSet;		// BBD 040311

DWORD GetServerIDByUniqID( DWORD dwUID, DWORD dwCI )
{	// 임시 리니어서치.
	for( int i = DRAGON_CONNECTIONS_START; i < DRAGON_MAX_CONNECTIONS; i++ )
	{
		if( (connections[i].dwIDForAgent == dwUID) && (connections[i].dwAgentConnectionIndex == dwCI) )
			return i;
	}
	return (DRAGON_MAX_CONNECTIONS);
}

DWORD GetNewCN( void )
{
	for (int i = DRAGON_CONNECTIONS_START; i < DRAGON_MAX_CONNECTIONS; ++i)
	{
		if (connections[i].dwAgentConnectionIndex == 0) 
		{	
			if (strlen(connections[i].name) == 0)
			{	//< CSD-031204
				CleanUpConnection(i);
				g_pServerTable->SetNumOfUsers( g_pServerTable->GetNumOfUsers()+1, 300, i,0, "GetNewCn() Called");
				return i;
			}	//> CSD-031204
		}
	}

	return 0;
}

void __stdcall ReceivedMsgFromServer(DWORD dwConnectionIndex, char* pMsg,DWORD dwLength)
{
	BYTE bID;
	bID = (BYTE)pMsg[0];
	
	if (dwConnectionIndex == 0)
	{
#ifdef __ON_DEBUG
		///		_asm int 3;
#endif
		return;
	}
	
	if( bID == (BYTE)PTCL_NOTIFY_SERVER_UP )
	{
		if( g_pServerTable->OnRecvServerUpMsg(dwConnectionIndex, *(WORD*)(pMsg+1)) )
		{
#ifdef __ON_DEBUG
			//			_asm int 3;
#endif
		}
		return;
	}
	
	LP_SERVER_DATA pSender = g_pServerTable->GetConnectedServerData( dwConnectionIndex );
	
	if( !pSender ) return;

	if (dwLength > MAX_PACKET_SIZE) // finito 22/06/08 
	{
		return; 
	}
	
	switch (bID)
	{
		// -------------
		// 기본패킷 
		// -------------
		// 전용
	case PTCL_ORDER_SET_SERVER_LIST:
	case PTCL_ORDER_CONNECT_TO_SERVERS:
	case PTCL_ORDER_SET_DB_DEMON:
	case PTCL_ORDER_TO_REPORT_SERVER_DATAS:
	case PTCL_NOTIFY_YOU_ARE_CERTIFIED:
		
		//공통
	case PTCL_ORDER_DESTROY_SERVER:
	case PTCL_NOTIFY_SERVER_STATUS:
	case PTCL_SERVER_TRY_TO_CHECK_CONNECTION:
	case PTCL_SERVER_CONNECTION_OK:
	case PTCL_ORDER_TO_REPORT_SERVER_STATUS:
		{
			if( !g_pServerTable->OnRecvNegotiationMsgs( pSender, bID, pMsg+1, dwLength-1 ) )
			{
				MyLog( LOG_FATAL, "OnRecvNegotiationMsg() Failed :: (pSender(%d), bId(%d), MsgLength(%d))", pSender->wPort, bID, dwLength );
#ifdef __ON_DEBUG
				//				_asm int 3;
#endif
			}
		}
		break;
		// -------------
		// 맵서버용 패킷
		// -------------
		// Added by chan78 at 2001/02/20
	case PTCL_AGENT_TO_MAP_REQUEST_REMOVE_GHOST_USER:
		{	//< CSD-030509
			LP_REQUEST_REMOVE_GHOST_USER_PACKET pPacket = (LP_REQUEST_REMOVE_GHOST_USER_PACKET)(pMsg+1);
			
			if (pPacket->dwID <= 0)
			{
				MyLog( LOG_FATAL, "PTCL_AGENT_TO_MAP_REQUEST_REMOVE_GHOST_USER :: dwID is 0 !!!(From %d/%d)", dwConnectionIndex, pSender->wPort );
				break;
			}
			
			DWORD dwTarget = GetServerIDByUniqID( pPacket->dwID, dwConnectionIndex);
			
			if (dwTarget > 0)
			{
				MyLog( LOG_NORMAL, "PTCL_AGENT_TO_MAP_REQUEST_REMOVE_GHOST_USER :: Ghost User(%d) is Wiped(From %d/%d)", pPacket->dwID, dwConnectionIndex, pSender->wPort );
				closeconnection( connections, (int)dwTarget, -220 );
			}
			else
			{
				MyLog( LOG_IMPORTANT, "PTCL_AGENT_TO_MAP_REQUEST_REMOVE_GHOST_USER :: User(%d) is Already Wiped(From %d/%d)", pPacket->dwID, dwConnectionIndex, pSender->wPort );
				break;
			}

			break;
		}	//> CSD-030509
	case PTCL_AGENT_TO_MAP:
		{	
			//<! BBD 040401 유저 방출중엔 에이전트에서 대기중인 놈이 맵서버에 들어옴 않됨
//			if(g_KickMgr.IsKicking())
//			{
//				break;
//			}
			//> BBD 040401 유저 방출중엔 에이전트에서 대기중인 놈이 맵서버에 들어옴 않됨
			//< CSD-HK-030829
			// 기본 사이즈 검사
			if (dwLength < 1 + 4 + sizeof(t_header)) 
			{
				MyLog(LOG_NORMAL, "PTCL_USER_CMD_TO_MAP_SERVER :: Too short packet(%d)", dwLength);
			}
			
			DWORD dwUniqID = 0;
			memcpy(&dwUniqID, pMsg+1, 4);

			t_packet raja_packet;
			memset(&raja_packet, 0, sizeof(t_packet));
			memcpy(raja_packet.h.data, pMsg+1+4, sizeof(t_header));
			memcpy(raja_packet.u.data, pMsg+1+4+sizeof(t_header), raja_packet.h.header.size);
			// chan78's comments 2000/11/01
			// ----------------------------
			// dwUniqID로 connections[]에서의 index를 search ...
			// 기존 라자는 server_id(== dwUniqID)가 connection_index와 같다.
			// 이 방식으로는 다른 MapServer에 접속할 때 마다 server_id를 재 부여받아야 한다.
			// 2byte씩 나누어 상위 2바이트는 MapServer 번호, 하위 2바이트는 server_id로 하면 좋을 것 같은데...
			// 클라이언트도 고쳐야 할지도 모르므로 일단 차분히 생각해보자.
			
			// 임시 리니어서치.
			DWORD ServerID = GetServerIDByUniqID(dwUniqID, dwConnectionIndex);

			if (ServerID == DRAGON_MAX_CONNECTIONS && !g_KickMgr.IsKicking())	// BBD 040401
			{
				// 없으면 새로운 connection
				ServerID = GetNewCN();

				if ((ServerID >= DRAGON_CONNECTIONS_START) && (ServerID < DRAGON_MAX_CONNECTIONS))
				{
					connections[ServerID].dwIDForAgent = dwUniqID;
					connections[ServerID].dwAgentConnectionIndex = dwConnectionIndex;
				}
				else
				{
					MyLog(LOG_FATAL, "New User(dwUniqID:%d)(Agent:%d) has tried to connect but connections[] is full!", dwUniqID, dwConnectionIndex);
					break;
				}
			}

			if (connections[ServerID].state == CONNECT_CHANGING_MAP)
			{
				if (raja_packet.h.header.type != CMD_CHANGE_MAP_DETECTED)
				{
					break;
				}
			}
			// Raja의 Command Handler로 Packet을 넘겨 준다.
			HandleCommand(ServerID, &raja_packet);
			break;
		}	//> CSD-HK-030829
	case PTCL_AGENT_TO_MAP_REQUEST_ADD_USER:
		{
			// 현재 안씀.
		}
		break;
	case PTCL_AGENT_TO_MAP_REQUEST_REMOVE_USER:
		{	//< CSD-HK-030829
			DWORD* dwUniqID = (DWORD *)(pMsg+1);
			DWORD dwServerID = GetServerIDByUniqID(*dwUniqID, dwConnectionIndex);
			
			if (dwServerID == DRAGON_MAX_CONNECTIONS)
			{
				return;
			}
			// 접속종료처리
			closeconnection(connections, dwServerID, 1);
			connections[dwServerID].dwIDForAgent = 0;
			connections[dwServerID].dwAgentConnectionIndex = 0;
			// MyLog( LOG_IGNORE, "User(dwUniqID:%d/dwServerID:%d) has lost connection", *dwUniqID, dwServerID);
			break;	
		}	//> CSD-HK-030829
	case PTCL_DB_ANSWER:
		{
			t_packet raja_packet;
			memcpy( &raja_packet, pMsg+1+4, dwLength-1-4);
			*((char *)&raja_packet + dwLength-1-4 )= 0;
			
			//			MyLog( LOG_DEBUG, "From DB Demon(type:%d, Size:%d)", raja_packet.h.header.type, raja_packet.h.header.size );
			HandleCommand( DB_DEMON, &raja_packet );
		}
		break;
		// -------------------------------------------------
		// Map Change Protocol (From MapServer To MapServer)
		// -------------------------------------------------
	case PTCL_MAP_TO_MAP:
		{
			DWORD dwNewCN = 0;
			LP_SERVER_DATA *pServerData = NULL;
			char szMsg[MM_MAX_PACKET_SIZE];
			t_packet *packet = (t_packet *)(pMsg+1);
			t_packet *spacket =(t_packet *)(szMsg+1);
			
			// Minimum Size Check
			if( (dwLength-1) < sizeof(t_header) ) {
				MyLog( LOG_FATAL, "PTCL_MAP_TO_MAP :: Too Short Packet(%d<sizeof(t_header))", dwLength-1);
				break;
			}
			// Packet Size Check
			if( (dwLength-1) != sizeof(t_header)+packet->h.header.size ) {
				MyLog( LOG_FATAL, "PTCL_MAP_TO_MAP :: Invalid Packet Size(%d != %d)", dwLength-1, sizeof(t_header)+packet->h.header.size );
				break;
			}
			
			// Packet Analysys
			switch( packet->h.header.type )
			{
			case CMD_MAP_CHANGE_REQUEST:
				{
					DWORD dwMyAgentConnectionIndex;
					
					// Packet size Check
					if( sizeof(t_change_map_protocol) != (dwLength-1-sizeof(t_header)) )
					{
						MyLog( LOG_FATAL, "PTCL_MAP_TO_MAP :: Invalid Packet Length(Expected:%d)(Received:%d(+1))", sizeof(t_change_map_protocol), (dwLength-1));
						break;
					}
					
					// 내 Active Agent Server List에 있는지 확인.
					if( !g_pServerTable->GetServerData(packet->u.change_map_protocol.wAgentServerID) )
					{
						MyLog( LOG_FATAL, "Map Changer's Agent Server(%d) is not available in Own ACTIVE SERVERLIST", packet->u.change_map_protocol.wAgentServerID );
						dwMyAgentConnectionIndex = 0;
					} else {
						dwMyAgentConnectionIndex = g_pServerTable->GetServerData(packet->u.change_map_protocol.wAgentServerID)->dwConnectionIndex;
					}
					
					// 새로운 유저를 받을수 있는지 Check
					dwNewCN = GetNewCN();
					
					if( !dwNewCN || !dwMyAgentConnectionIndex || g_KickMgr.IsKicking())	// BBD 040329
					{	// 실패
						
						// Modified by chan78 at 2000/11/29
						// dwNewCN은 성공했지만 다른 이유로 실패한경우 접속된 사용자수 줄임.
						if( dwNewCN ) 
						{
						//	g_pServerTable->SetNumOfUsers( g_pServerTable->GetNumOfUsers()-1 );
							g_pServerTable->SetNumOfUsers( g_pServerTable->GetNumOfUsers()-1, 400, dwNewCN,0, "ReceivedMsgFromServer()");
						} 
						else 
						{
							MyLog( LOG_IMPORTANT, "PTCL_MAP_TO_MAP :: Can't Accept MORE Character!" );
						}
						
						// 더 이상 받을수 없음을 sender에게 알린다
						szMsg[0] = (BYTE)PTCL_MAP_TO_MAP;
						spacket->h.header.type = CMD_MAP_CHANGE_DENIED;
						spacket->h.header.size = sizeof( t_map_change_result );
						spacket->h.header.crc = 0;
						
						spacket->u.map_change_result.wAgentServerID = packet->u.change_map_protocol.wAgentServerID;
						spacket->u.map_change_result.dwIDForAgent = packet->u.change_map_protocol.dwIDForAgent;
						spacket->u.map_change_result.TargetServerCn = 0;
						spacket->u.map_change_result.FromServerCn = packet->u.change_map_protocol.FromServerCn;
						
						if( !g_pServerTable->Send(dwConnectionIndex, szMsg, 1+sizeof(t_header)+spacket->h.header.size) )
						{
							MyLog( LOG_FATAL, "PTCL_MAP_TO_MAP :: Can't Answer to MapServer(ConnectionIndex:%d)", dwConnectionIndex );
							break;
						}
					} 
					else 
					{
						t_packet tp;
						memset( &tp, 0, sizeof(t_packet) );
						
						// 성공 
						connections[dwNewCN].dwIDForAgent = packet->u.change_map_protocol.dwIDForAgent;
						connections[dwNewCN].dwAgentConnectionIndex = dwMyAgentConnectionIndex;
						connections[dwNewCN].state = CONNECT_LOGIN_KEIN; // nMap에서의 상태..
						
						memcpy( connections[dwNewCN].id, packet->u.change_map_protocol.id, ID_LENGTH );
						memcpy( connections[dwNewCN].name, packet->u.change_map_protocol.name, NM_LENGTH );
						memcpy( connections[dwNewCN].chrlst.Name, packet->u.change_map_protocol.name, NM_LENGTH );
						memcpy( connections[dwNewCN].pw, packet->u.change_map_protocol.password, PW_LENGTH );
						memcpy( connections[dwNewCN].mapname, MapName, NM_LENGTH );
						strcpy_s( connections[dwNewCN].chrlst.MapName, sizeof(connections[dwNewCN].chrlst.MapName), MapName );
						strcpy_s( connections[dwNewCN].ip_address, sizeof(connections[dwNewCN].ip_address), packet->u.change_map_protocol.ip );
						
						// Build Packet For DB Demon
						// DB의 'LoginTable'의 MapName을 바꾼다.  
						tp.h.header.type = CMD_ACCESS_JOIN_KEIN;
						tp.u.gs_req_insert_userid.server_id = dwNewCN;
						memcpy( tp.u.gs_req_insert_userid.id,		packet->u.change_map_protocol.id, ID_LENGTH );
						memcpy( tp.u.gs_req_insert_userid.pw,		packet->u.change_map_protocol.password, PW_LENGTH );
						memcpy( tp.u.gs_req_insert_userid.name,		packet->u.change_map_protocol.name, NM_LENGTH );
						memcpy( tp.u.gs_req_insert_userid.mapname,	packet->u.change_map_protocol.mapname, NM_LENGTH );
						tp.h.header.size = sizeof( t_gs_req_insert_userid );
						QueuePacket( connections, DB_DEMON, &tp, 1 );			// LogIn Server로 보낸다. 
						
						// Now Reading...
						NewRecvChangeMapData1( &packet->u.change_map_protocol.mapdata1, dwNewCN );
						NewRecvChangeMapData2( &packet->u.change_map_protocol.mapdata2, dwNewCN );
						NewRecvChangeMapData3( &packet->u.change_map_protocol.mapdata3, dwNewCN );
						NewRecvChangeMapData4( &packet->u.change_map_protocol.mapdata4, dwNewCN );
						NewRecvChangeMapData5( &packet->u.change_map_protocol.mapdata5, dwNewCN );
						NewRecvChangeMapData6( &packet->u.change_map_protocol.mapdata6, dwNewCN );
						NewRecvChangeMapData7( &packet->u.change_map_protocol.mapdata7, dwNewCN );
						NewRecvChangeMapData8( &packet->u.change_map_protocol.mapdata8, dwNewCN );
						NewRecvChangeMapData9( &packet->u.change_map_protocol.mapdata9, dwNewCN );
						
						//----------------- 서버로 부터 받지 않는 Data를 정리.  001218 KHS
						LPCHARLIST tch = &connections[dwNewCN].chrlst;
						tch->SetServerID(dwNewCN); // CSD-HK-030829
						tch->startposition	= 0;	
						tch->updatable		= 1; // CSD-TW-030620
						tch->timetoupdate   = g_curr_time;
						tch->Last_MapChangeTime = g_curr_time;
						tch->Race			= HUMAN;
						
						tch->cur_skillno	= 0;
						
						int i;
						for( i = 0 ; i < 1000 ; i ++)	if( tch->Item[i] == 0) break;
						tch->ItemMax = i;
						//----------------------------------------------------
						
						// Answer(success)
						szMsg[0] = (BYTE)PTCL_MAP_TO_MAP;
						spacket->h.header.type = CMD_MAP_CHANGE_ACCEPTED;
						spacket->h.header.size = sizeof( t_map_change_result );
						spacket->h.header.crc = 0;
						
						spacket->u.map_change_result.wAgentServerID = packet->u.change_map_protocol.wAgentServerID;
						spacket->u.map_change_result.dwIDForAgent = connections[dwNewCN].dwIDForAgent ;
						spacket->u.map_change_result.TargetServerCn = dwNewCN;
						spacket->u.map_change_result.FromServerCn = packet->u.change_map_protocol.FromServerCn;
						
						//021030 YGI---------------------------------------
						spacket->u.kein.new_map_change_result.u.data2.active = 0;
						if( g_EventMgr.IsActive( g_EventMgr.ET_EVENT_OBJECT ) )
						{
							CEventObject *pData = (CEventObject*)g_EventMgr.GetActiveEvent( g_EventMgr.ET_EVENT_OBJECT );
							spacket->u.kein.new_map_change_result.u.data2.active = pData->ACTIVE_NUMBER_;
							strcpy_s( spacket->u.kein.new_map_change_result.u.data2.object_toi, sizeof(spacket->u.kein.new_map_change_result.u.data2.object_toi), pData->m_szObjectTOI );
							strcpy_s( spacket->u.kein.new_map_change_result.u.data2.object_b, sizeof(spacket->u.kein.new_map_change_result.u.data2.object_b), pData->m_szObjectB );
							spacket->h.header.size += sizeof( k_event_object );
						}
						
						if( g_EventMgr.IsActive( g_EventMgr.ET_EVENT_SOUND ) )
						{
							CEventSound *pData = (CEventSound *)g_EventMgr.GetActiveEvent( g_EventMgr.ET_EVENT_SOUND );
							if( spacket->u.kein.new_map_change_result.u.data3.active )
							{
								spacket->h.header.size = sizeof( k_new_server_change_map );
								spacket->u.kein.new_map_change_result.u.data2.active |= pData->ACTIVE_NUMBER_;
								spacket->u.kein.new_map_change_result.u.data2.sound_no = pData->m_nSoundNo;
							}
							else
							{
								spacket->u.kein.new_map_change_result.u.data3.active = pData->ACTIVE_NUMBER_;
								spacket->u.kein.new_map_change_result.u.data3.sound_no = pData->m_nSoundNo;
								spacket->h.header.size += sizeof( k_event_sound );
							}
						}
						//----------------------------------------------------
						
						//						tch->server_id      = dwNewCN;
						if( !g_pServerTable->Send(dwConnectionIndex, szMsg, 1+sizeof(t_header)+spacket->h.header.size) )
						{
							MyLog( LOG_FATAL, "PTCL_MAP_TO_MAP :: Can't Answer to MapServer(ConnectionIndex:%d)", dwConnectionIndex );
							break;
						}
					}
				}
				break;
				//021030 YGI
			case CMD_MAP_CHANGE_ACCEPTED:
				{	//< CSD-HK-030829
					t_packet *rpacket = (t_packet*)(pMsg+1);
					int size = dwLength-1-sizeof(t_header);
					
					if( size > sizeof(k_new_server_change_map) )
					{
						MyLog( LOG_FATAL, "CMD_MAP_CHANGE_ACCEPTED :: Invalid Packet Size(%d != %d)", size, sizeof(k_new_server_change_map) );
						//break;
					}
					rpacket->u.data[size] = 0;
					//------------------
					
					if( connections[rpacket->u.map_change_result.FromServerCn].server_change_map.port == 0 ) 
					{
						MyLog( LOG_FATAL, "CMD_MAP_CHANGE_ACCEPTED :: connection[%d] is not found!", rpacket->u.map_change_result.FromServerCn );
						break;
					}
					// Make Packet.
					spacket->h.header.type = CMD_CHANGE_MAP;
					spacket->h.header.size = sizeof(t_server_change_map);
					connections[rpacket->u.map_change_result.FromServerCn].server_change_map.server_id = (short int)rpacket->u.map_change_result.TargetServerCn;
					memcpy( &spacket->u.server_change_map, &connections[rpacket->u.map_change_result.FromServerCn].server_change_map, sizeof(server_change_map) );
					if( rpacket->u.kein.new_map_change_result.u.data2.active & CEventObject::ACTIVE_NUMBER_ )
					{
						spacket->u.kein.new_server_change_map.u.data2 = rpacket->u.kein.new_map_change_result.u.data2;
						spacket->h.header.size += sizeof(k_event_object);
						if( rpacket->u.kein.new_map_change_result.u.data2.active & CEventSound::ACTIVE_NUMBER_ )
						{
							spacket->h.header.size = sizeof(k_new_server_change_map);
						}
					}
					else if( rpacket->u.kein.new_map_change_result.u.data2.active & CEventSound::ACTIVE_NUMBER_ )
					{
						spacket->u.kein.new_server_change_map.u.data3 = rpacket->u.kein.new_map_change_result.u.data3;
						spacket->h.header.size += sizeof(k_event_sound);
					}	
					// Send It.
					QueuePacket(connections, rpacket->u.map_change_result.FromServerCn, spacket, 1);
					break;
				}	//> CSD-HK-030829
			case CMD_MAP_CHANGE_DENIED:
				{
					t_packet *rpacket = (t_packet*)(pMsg+1);
					
					if( dwLength-1 != (sizeof(t_header)+sizeof(t_map_change_result)) )
					{
						MyLog( LOG_FATAL, "CMD_MAP_CHANGE_DENIED :: Invalid Packet Size(%d != %d)", dwLength-1, sizeof(t_header)+sizeof(t_map_change_result) );
						break;
					}
					
					if( connections[rpacket->u.map_change_result.FromServerCn].server_change_map.port == 0 )
					{
						MyLog( LOG_FATAL, "CMD_MAP_CHANGE_DENIED :: connection[%d] is not found!", rpacket->u.map_change_result.FromServerCn );
						break;
					}
					
					// 혹시 MapChange 패킷이 오가는중 나가버릴수도 있다...
					// 순간이므로 가능성은 매우 낮지만.
					if( connections[rpacket->u.map_change_result.FromServerCn].dwAgentConnectionIndex == 0 )
						break;
					
					// Added by chan78 at 2000/11/27
					LPCHARLIST ch;
					ch = &connections[rpacket->u.map_change_result.FromServerCn].chrlst;
					
					// 커넥션 값 복구
					connections[rpacket->u.map_change_result.FromServerCn].state = CONNECT_JOIN;
					memcpy( connections[rpacket->u.map_change_result.FromServerCn].mapname, MapName, NM_LENGTH);
					
					// 백업해놓았던 값으로 되돌리기.
					ch->MoveGox = ch->MapChange_MoveGox;
					ch->MoveGoy = ch->MapChange_MoveGoy;
					ch->X = ch->MapChange_X;
					ch->Y = ch->MapChange_Y;
					ch->MoveSx = ch->MapChange_MoveSx;
					ch->MoveSy = ch->MapChange_MoveSy;
					ch->OldMoveSx = ch->MapChange_MoveSx;
					ch->OldMoveSy = ch->MapChange_MoveSy;
					ch->MoveLength = ch->MapChange_MoveLength;
					ch->MovePathCount = ch->MapChange_MovePathCount;
					
					// MAP 변경 캔슬
					CallClient( rpacket->u.map_change_result.FromServerCn, CMD_CHANGE_MAP_FAIL);
				}
				break;
				
				// 001206 KHS 국가전관련 
			case CMD_SEALSTONE_STATUS :
				{
					t_packet *rpacket = (t_packet*)(pMsg+1);
					RecvSealStoneStatus( rpacket );
					break;	
				}
			case CMD_STEALSTONE_BREAKABLE:			
				{	//< CSD-CN-031213
					t_packet *rpacket = (t_packet*)(pMsg+1);
					g_pUserManager->SendPacket(rpacket);
					SealStoneBreakable = CMD_STEALSTONE_BREAKABLE;	
					g_LocalWarBegin=rpacket->u.LocalWar.CommonDataC.Data;	// 011215 LTS
					// MyLog(0,"Local War Status : %d",g_LocalWarBegin);
					break;										
				}	//> CSD-CN-031213
			case CMD_STEALSTONE_NON_BREAKABLE_PEACE:	
				{	//< CSD-CN-031213
					t_packet *rpacket = (t_packet*)(pMsg+1);
					g_pUserManager->SendPacket(rpacket);
					SealStoneBreakable = CMD_STEALSTONE_NON_BREAKABLE_PEACE;	
					g_LocalWarBegin=rpacket->u.LocalWar.CommonDataC.Data;	// 011215 LTS
					// MyLog(0,"Local War Status : %d",g_LocalWarBegin);
					break;
				}	//> CSD-CN-031213
			case CMD_STEALSTONE_NON_BREAKABLE:			
				{	//< CSD-CN-031213
					t_packet *rpacket = (t_packet*)(pMsg+1);
					g_pUserManager->SendPacket(rpacket);
					SealStoneBreakable = CMD_STEALSTONE_NON_BREAKABLE;	
					g_LocalWarBegin=rpacket->u.LocalWar.CommonDataC.Data;	// 011215 LTS
					// MyLog(0,"Local War Status : %d",g_LocalWarBegin);
					break;									
				}	//> CSD-CN-031213
			default:
				{
					t_packet *rpacket = (t_packet*)(pMsg+1);
					HandleCommand( CN_FROM_OTHER_MAPSERVER_MSG, rpacket );
					//				MyLog( LOG_FATAL, "PTCL_MAP_TO_MAP :: Invalid Packet(%d) has received!", packet->h.header.type );
					break;
				}
			}
		}
		break;
	case PTCL_BROADCAST_TO_SERVERS:
		{
			t_packet packet;
			memcpy( &packet, pMsg+1, dwLength-1 );
			*(((char *)&packet)+dwLength) = '\0';
			
			// RajaPacket검증
			if( ((dwLength-1) < sizeof(t_header)) || ((dwLength-1) != (sizeof(t_header)+packet.h.header.size)) )
			{
				MyLog( LOG_FATAL, "PTCL_BROADCAST_TO_SERVERS :: Invalid Packet Size(Expected:%d)(Length:%d)", (sizeof(t_header)+packet.h.header.size), dwLength-1 );
				break;
			}
			// HandleBroadCastedPackets( &packet );
			// Modifed by chan78 2000/12/01
			HandleCommand( CN_FROM_OTHER_MAPSERVER_MSG, &packet );
		}
		break;
		// Added by chan78 at 2001/03/16
		// -----------------------------
	case PTCL_MANAGER_QUERY:
		{
			if( !OnRecvMsgFromManager( (LP_MANAGER_PACKET)pMsg, dwLength ) )
			{
				MyLog( LOG_FATAL, "PTCL_MANAGER_ANSWER :: OnRecvMsgFromManager() has return false(%d)", pSender->wPort );
				break;
			}
		}
		break;
		// Added by chan78 at 2001/03/16
	case PTCL_MANAGER_ANSWER:
		{
			// 있을 수 없다.
			MyLog( LOG_FATAL, "PTCL_MANAGER_ANSWER :: has received!!!(%d)", pSender->wPort );
			g_pServerTable->DestroyServer( FINISH_TYPE_UNKNOWN_ERROR);
		}
		break;
		// -----------------------------
		
		// 011012 KBS
	case PTCL_PROXY_TO_MAP:
		{
			t_packet packet;
			memcpy( &packet, pMsg+1, dwLength-1 );
			*(((char *)&packet)+dwLength) = '\0';
			
			HandleCommand( CN_FROM_OTHER_MAPSERVER_MSG, &packet );
		}
		break;
		
	case PTCL_RM_FROM_PROXY:
		{
			RMProc(dwConnectionIndex, pMsg, dwLength);
		}
		break;
		//
//<! BBD 040311	프록시로부터의 동접수 응답
	case PTCL_SERVERSET_USERNUM_REQUEST:
		{
			memcpy(&g_UsercountInServerSet, &(pMsg[1]), sizeof(g_UsercountInServerSet));
			unsigned short count2 = g_pServerTable->GetNumOfUsersInServerSet();
			break;
		}
//> BBD 040311	프록시로부터의 동접수 응답
	default:
		{
			MyLog( LOG_FATAL, "Unknown bID(%d) Received. From %s(wPort:%d) Size(%d) ConnectionID(%d)", bID, GetTypedServerText(pSender->dwServerType), pSender->wPort, dwLength, dwConnectionIndex);
#ifdef __ON_DEBUG
			//			_asm int 3;
#endif
		}
		break;
	}
}
void __stdcall ReceivedMsgFromUser(DWORD dwConnectionIndex, char* pMsg,DWORD dwLength)
{
	return;
}

// 020701 YGI
WORD GetServerPortById( DWORD dwConnectionIndex )
{
	if( !g_pServerTable ) return 0;
	LP_SERVER_DATA pServerData;
	pServerData = g_pServerTable->GetConnectedServerData( dwConnectionIndex );
	if ( !pServerData || !pServerData->pHashedServerData )
	{
		return 0;
	}
	return pServerData->wPort;
}

extern void DisconnectServer_kein( WORD port );	// 020627 YGI
extern void ConnectServer_kein( WORD port );	// 020627 YGI

void __stdcall OnAcceptServer( DWORD dwConnectionIndex )
{
	ConnectServer_kein( GetServerPortById( dwConnectionIndex ) );		// 020627 YGI
}

void __stdcall OnAcceptUser( DWORD dwConnectionIndex )
{
}

void __stdcall OnDisconnectServer( DWORD dwConnectionIndex )
{
	g_pServerTable->RemoveConnectedServerDataFromHashTable( dwConnectionIndex );
	DisconnectServer_kein( GetServerPortById( dwConnectionIndex ) );		// 020627 YGI
}
