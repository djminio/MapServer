#include "stdafx.h"
#include "LowerLayers\\inetwork.h"
#include "LowerLayers\\ServerTable.h"
#include "RMDefine.h"
#include "HigherLayers\\KickOffAll.h"		// BBD 040110

extern void __stdcall ReLoadGameServerDataByKeyInput(DWORD dwValue); //020511 lsw

CKickOffAll g_KickMgr;		// BBD 040110

void RMProc(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	BYTE header;
	memcpy(&header, pMsg + 1, 1);

	switch (header)
	{
	case MSG_SHUT_DOWN:
		{
			g_pServerTable->DestroyServer(FINISH_TYPE_NORMAL);
			break;
		}
	case MSG_RELOAD_GAMESERVER_DATA:
		{
			ReLoadGameServerDataByKeyInput(0);
			break;
		}
	case MSG_CHANGE_WEATHER:	//클라이언트들에게 메세지 뿌려라~
		{
			PACKET_CHANGE_WEATHER *rcvPacket = (PACKET_CHANGE_WEATHER*)pMsg;

			t_packet packet;
			packet.h.header.type = CMD_RM_CHANGE_WEATHER;
			packet.h.header.size = sizeof(t_rm_change_weather);
			packet.u.RmChangeWeather.bWeather = rcvPacket->bWeather;
			packet.u.RmChangeWeather.dwAmount = rcvPacket->dwAmount;
			packet.u.RmChangeWeather.bStopWeather = rcvPacket->bStopWeather;
			
			for (int cn = DRAGON_CONNECTIONS_START ; cn < DRAGON_MAX_CONNECTIONS ; cn ++)
			{
				if(connections[cn].dwAgentConnectionIndex && connections[cn].state >= CONNECT_JOIN)
				{
					QueuePacket(connections, cn, &packet, 1);
				}
			}

			break;
		}
	case MSG_ECHO:
		{	// Echo
			if (g_pINet)
			{
				g_pINet->SendToServer(dwConnectionIndex, pMsg, dwLength, FLAG_SEND_NOT_ENCRYPTION);
			}
			
			break;
		}
	//<! BBD 040110	RM으로 전제접속을 종료시작
	case MSG_RM_KICKOFF_USER_ALL:	// 전체 유저를 맵서버가 차례대로 잘라준다
		{
			if(!g_KickMgr.IsKicking())				// 진행중 다시 들어오면 무시한다
			{
				//<! BBD 040401
				if(g_KickMgr.InitKickOffUserAll())
				{
					MyLog( LOG_NORMAL, "Start Blocking Agent -> Map Join" );
					MyLog( LOG_NORMAL, "Start Blocking Map -> Map Move" );	// BBD 040329
				}
				//> BBD 040401
			}
			break;
		}
	//> BBD 040110	RM으로 전제접속을 종료시작

	//<! BBD 040110 RM으로 일정수의 접속유저를 자른다
	case MSG_RM_KICKOFF_USER_SEVERAL:
		{
			g_KickMgr.KickOffSeverlUser();
			break;
		}
	//> BBD 040110 RM으로 일정수의 접속유저를 자른다

	//<! BBD 040110 Agent의 맵서버 조인 허용
	case MSG_RM_KICKOFF_AGENTCANJOIN:
		{
			//<! BBD 040401
			if(g_KickMgr.IsKicking())	// 킥중일때만 처리
			{
				g_KickMgr.StopKicking();
				MyLog( LOG_NORMAL, "End Blocking Agent -> Map Join" );
				MyLog( LOG_NORMAL, "End Blocking Map -> Map Move" );			// BBD 040329
			}
			//> BBD 040401
			break;
		}
	//> BBD 040110 Agent의 맵서버 조인 허용
	}
}