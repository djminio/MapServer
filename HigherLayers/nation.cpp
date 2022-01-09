#include "..\stdafx.h"
#include "..\LowerLayers\servertable.h"
#include "..\LowerLayers\recvmsg.h"
#include "..\LowerLayers\mylog.h"
#include "DefaultHeader.h"
#include "MenuDefine.h"
#include "ItemList.h"
#include "CItem.h"
#include "map.h"

tagNationInfo	NationInfo[MAX_NATION];
bool			g_MainNationMap[MAX_NATION];			// 담당하는 나라 번호

void CheckMainMap()
{	//< CSD-030506
	for (int i = 0; i < MAX_NATION; ++i)
	{
		if (NationInfo[i].main_map_port == g_MapPort)
		{
			g_MainNationMap[i] = true;
			LoadNationInfoOfMainMap(i);	// 메인 메뉴에서 담당할 내용들을 로드해온다.
		}
	}
}	//> CSD-030506

void SendNationMoney( int type, int nation, double money )
{
	t_packet packet;

	packet.h.header.type = CMD_NATION_INFO_MAP_TO_MAP;

		packet.u.kein.nation_info_money.type = type;
		packet.u.kein.nation_info_money.nation = nation;
		packet.u.kein.nation_info_money.money = money;

	packet.h.header.size = sizeof( k_nation_info_money );

	if( type != N_NATION_ALL && g_MainNationMap[nation] )		// 이곳에서 담당하는 곳이라면
	{
		RecvNationInfo( &packet );		// 직접 처리하고
	}
	else								// 아니면
	{									// 담당 맵으로 보낸다.
		g_pServerTable->SendRajaPacketToOtherMapServer( (WORD)NationInfo[nation].main_map_port, (char *)&packet, packet.h.header.size+sizeof(t_header) );
	}
}

void SaveNationInfo( int nation )		// 디비 데몬에 저장한다.
{
	/*static save_time = g_curr_time;
	if( (g_curr_time - save_time) < 600 )	return;	// 10분에 한번씩
	save_time = g_curr_time; finito removed not needed*/

	if( !g_MainNationMap[nation] ) // 메인 맵이 아니다.	
		return;

	t_packet packet;
	packet.h.header.type = CMD_SAVE_NATION_INFO;

		packet.u.kein.save_nation_info.nation		=	nation;
		packet.u.kein.save_nation_info.nation_money	=	NationInfo[nation].nation_money;
		packet.u.kein.save_nation_info.king_support	=	NationInfo[nation].king_support;
		packet.u.kein.save_nation_info.friend_va	=	NationInfo[nation].friend_va	;
		packet.u.kein.save_nation_info.friend_zy	=	NationInfo[nation].friend_zy;
		packet.u.kein.save_nation_info.friend_ill	=	NationInfo[nation].friend_ill;
		memcpy( packet.u.kein.save_nation_info.nation_item, NationInfo[nation].nation_item, sizeof( int ) * 20 );

	packet.h.header.size = sizeof( k_save_nation_info );
	QueuePacket( connections, DB_DEMON, &packet,1 );


	// 모든 맵에 뿌려 준다.
	packet.h.header.type = CMD_NATION_INFO_MAP_TO_MAP;

		packet.u.kein.nation_info_money.type = N_NATION_ALL;
		packet.u.kein.nation_info_money.nation = nation;
		packet.u.kein.nation_info_money.money = NationInfo[nation].nation_money;

	packet.h.header.size = sizeof( k_nation_info_money );
	g_pServerTable->BroadCastToEveryServer( (char*)&packet, (sizeof(t_header)+packet.h.header.size), SERVER_TYPE_MAP );
}

void RecvNationInfo( t_packet *p )
{
	int type = p->u.kein.nation_type;
	switch( type )
	{
		case NM_ADD	:		NationInfo[p->u.kein.nation_info_money.nation].nation_money += p->u.kein.nation_info_money.money;
			SaveNationInfo(p->u.kein.nation_info_money.nation);
			break;
		case NM_SUBTRACT :	
			{
				int nation = p->u.kein.nation_info_money.nation;
				int money = p->u.kein.nation_info_money.money;
				if( NationInfo[nation].nation_money > money )
				{
					NationInfo[p->u.kein.nation_info_money.nation].nation_money -= p->u.kein.nation_info_money.money;
				}
				else NationInfo[nation].nation_money = 0;
				SaveNationInfo(p->u.kein.nation_info_money.nation);
				break;
			}
		case N_NATION_ALL : 
			{
				int nation = p->u.kein.nation_info_money.nation;
				NationInfo[nation].nation_money = p->u.kein.nation_info_money.money;
				break;
			}
		default : return;
	}
}
