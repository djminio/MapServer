#include "..\stdafx.h"
#include "..\LowerLayers\servertable.h"
#include "..\LowerLayers\recvmsg.h"
#include "..\LowerLayers\mylog.h"
#include "DefaultHeader.h"
#include "MenuDefine.h"
#include "ItemList.h"
#include "CItem.h"
#include "Scrp_exe.h"
#include "SealStone.h"
#include "SealStonegameserver.h"
#include "op_magic.h"
#include "map.h"
#include "teambattle.h"
#include "LTSSupport.h"
#include "dungeon.h"
#include "eventmgr.h" 
#include "LogManager.h"
#include "ArenaManager.h"
#include "UserManager.h"

void SendSaveEventItem( CHARLIST *ch, int item_no, int count )
{
	t_packet packet;
	packet.h.header.type = CMD_ADD_EVENT_ITEM;
	packet.u.kein.save_event_item.item_no = item_no;
	packet.u.kein.save_event_item.count = count;
	strcpy_s( packet.u.kein.save_event_item.name, sizeof(packet.u.kein.save_event_item.name), ch->Name );
	packet.h.header.size = sizeof( k_save_event_item ) - 20 + strlen( ch->Name )+1 ;
	QueuePacket( connections, DB_DEMON, &packet, 1 );
}

void RecvOXSelect( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	SetOXSelect( ch,p->u.kein.default_char ); 
	return;
}

void SetOXSelect( CHARLIST *ch, int type )
{
	ch->ox_select = type;
}

void RecvOXAnser( t_packet *p, short int cn )
{	//< CSD-CN-031213
	const int type = p->u.kein.default_char;

	CUserManager::HASH_USER mpUser = g_pUserManager->GetUserSet();

	for (CUserManager::ITOR_USER i = mpUser.begin(); i != mpUser.end(); ++i)
	{
		const int idUser = i->second;

		if (idUser == cn)
		{
			continue;
		}

		CHARLIST* pUser = CheckServerId(idUser);
		
		if (pUser == NULL)
		{
			continue;
		}

		if (pUser->IsCounselor() || pUser->IsReporter())
		{
			continue;
		}

		if (pUser->ox_select == type) 
		{
			continue;
		}

		MapMove(idUser, "G_BATTLE", 140, 100);	// 030929 kyo
	}
}	//> CSD-CN-031213

// 이 맵에 있는 모든 유저를 지정한 곳으로 맵이동 시킨다.
// type == 1 이면 기자나, 운영자는 남긴다.
int UserAllMapMove( int type, char *map_name, int x, int y )
{	//< CSD-CN-031213
	if (strcmp(map_name, MapName) == 0)
	{
		return 0;
	}

	if (GetMapMoveType(map_name) == MMT_FAIL)
	{	// 없는 맵이다.
		return 0;
	}

	CUserManager::HASH_USER mpUser = g_pUserManager->GetUserSet();

	for (CUserManager::ITOR_USER i = mpUser.begin(); i != mpUser.end(); ++i)
	{
		const int idUser = i->second;
		CHARLIST* pUser = CheckServerId(idUser);
		
		if (pUser != NULL)
		{
			if (type == 1)
			{
				if (pUser->IsCounselor() || pUser->IsReporter()) 
				{
					continue;
				}
			}

			MapMove(idUser, map_name, x, y);
		}
	}

	return 1;
}	//> CSD-CN-031213

void RecvCheckSkill( short int cn )
{
	CHARLIST *ch= CheckServerId( cn );
	if( !ch ) return;

	// 염색 기술을 배웠으면 돈을 준다.
	if( ch->Skill[46] )
	{
		ch->IncBankMoney(50000, BMCT_SKILL_RESET); // CSD-030723
		ch->Skill[46] = 0;
		SendPutMenuString( KM_BBS, 121, cn ); // 염색하기 스킬을 삭제했습니다.
	}	
}


void DeleteMenuData()
{

	// 종료시 프리시키는 곳..

}

void RecvPutGuildItem( t_packet *p, short int cn )
{	//< CSD-030324
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	POS pos = p->u.kein.default_pos;
	ItemAttr *pItem = GetItemByPOS( ch, pos );
	if( !pItem ) return;
	if( !pItem->item_no ) return;
	if( !ch->GetGuildCode() ) return;		// 길드가 없다.
	//if( ch->name_status.guild_master ) return;		// 길드내 등급 제약을 하려면 여기서

	CItem *ii = ItemUnit( pItem->item_no );
	if( NULL == ii ) return;

	// finito 040707 check for noexchange item kind
	if (IsNoExchange(pItem))
	{
		SendPutMenuString( KM_FAIL, 246, cn );
		return;
	}
	// end of check

	t_packet packet;
	packet.h.header.type = CMD_PUT_GUILD_ITEM;
		packet.u.kein.put_guild_item.item = *pItem;
		packet.u.kein.put_guild_item.guild_code = ch->GetGuildCode();
		strcpy_s( packet.u.kein.put_guild_item.name, sizeof(packet.u.kein.put_guild_item.name), ch->Name );
	packet.h.header.size = sizeof( k_put_guild_item );
	QueuePacket( connections, DB_DEMON, &packet, 1 );	// DB에 저장한다.

	//021030 YGI
	SendItemEventLog( pItem, cn, SN_NOT_USER, SILT_PUT_GUILD_BOX, 1 ); // 021018 YGI

	// 아이템을 지우고
	memset( pItem, 0, sizeof( ItemAttr ) );
	// 저장한 사실을 클라이언트에 보내준다.
	SendPutMenuString( KM_OK, 132, cn );		// 보관되었습니다.
}	//> CSD-030324

void RecvGetGuildItemClient( t_packet *p, short int cn )
{	//< CSD-030324
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

//	if( !ch->GetGuildCode() ) return;
//	int degree = ch->name_status.guild_master;
//	if( !degree || degree > 2 ) return;		// 1과 2만 사용 가능
	if( !CheckGuildPower( GDP_GUILD_BOX, ch ) ) return;

	DWORD index = p->u.kein.default_dword;
	t_packet packet;
	packet.h.header.type = CMD_GET_GUILD_ITEM_MAP;

		packet.u.kein.get_guild_item_map.server_id = cn;
		packet.u.kein.get_guild_item_map.guild_code = ch->GetGuildCode();
		packet.u.kein.get_guild_item_map.index = index;

	packet.h.header.size = sizeof( k_get_guild_item_map );
	QueuePacket( connections, DB_DEMON, &packet, 1 );	// DB에서 해결
}	//> CSD-030324

// 디비 데몬에서 아이템을 꺼내 왔다.
void RecvGetGuildItemMap( t_packet *p )
{
	int server_id = p->u.kein.get_guild_item_db.server_id;
	ItemAttr *pItem = &p->u.kein.get_guild_item_db.item;

	CHARLIST *ch = CheckServerId( server_id );
	if( !ch ) return;

	//021030 YGI
	SendItemEventLog( pItem, server_id, SN_NOT_USER, SILT_GET_GUILD_BOX, 3 ); // 021018 YGI
	MoveEmptyInv( pItem, ch );
}

void RecvChangeGuildMaster(short int cn)
{	//< CSD-030326
	CHARLIST* ch = CheckServerId(cn);
	if (!ch)  return;

	int guild_code = ch->GetGuildCode();
	const int guild_master = ch->name_status.guild_master;
	
	if (guild_code <= 0 || guild_master != 1) 
	{
		return;		// 길드 마스터가 아니다.
	}

	SendPacketDefault(CMD_CHANGE_GUILD_MASTER, &guild_code, sizeof(short int), DB_DEMON);
	ch->name_status.guild_master = 0;		// 길드 마스터
	SendChangeGuildDegree(cn, guild_code, 0, cn);
	SendPutMenuString(KM_OK, 178, cn);
}	//> CSD-030326

// 길드원 정보를 변경
void SendChangeGuildDegree( int server_id, int guild_code, int guild_degree, short int cn )
{
	t_packet packet;

	packet.h.header.type = CMD_CHANGE_GUILD_MASTER;
		packet.u.kein.change_guild_degree.server_id = server_id;
		packet.u.kein.change_guild_degree.guild_code = guild_code;
		packet.u.kein.change_guild_degree.degree = guild_degree;
	packet.h.header.size = sizeof( k_change_guild_degree );
	QueuePacket( connections, cn, &packet, 1 );
	CastMe2Other( cn, &packet );
}

// 길마가 바꼈다.
void RecvChangeGuildMasterAllMap( t_packet *p )
{	//< CSD-030324
	char *name = p->u.kein.default_name;
	int cn = ExistHe( name );
	if( cn <= 0 ) return;

	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	ch->name_status.guild_master = 1;
	SendChangeGuildDegree( cn, ch->GetGuildCode(), ch->name_status.guild_master, cn );
}	//> CSD-030324

int GetDegree2Param( int degree )		// degree 값을 파라메터로 변경한다.
{
	static int degree2param[] = { 4, 0, 1, 2, 3 };
	return degree2param[degree];
}

// 020620 YGI
bool CheckGuildPower( int type, CHARLIST *ch )
{	//< CSD-030324
	if( !ch ) return 0;
	if( !ch->GetGuildCode() ) return 0;
	if( type < 0 || type > GDP_MAX ) return 0;
	
	static int power_table[][5] = 
	{
		{ 1, 1, 0, 0, 0 },		// 길드 가입				// GDP_JOINING
		{ 1, 1, 0, 0, 0 },		// 길드 전체 메시지			// GDP_BBS
		{ 1, 0, 0, 0, 0 },		// 길드원 직책변경			// GDP_CHANGE_DEGREE  // 020620 YGI
		{ 1, 1, 0, 0, 0 },		// 길드 창고 이용			// GDP_GUILD_BOX
		{ 1, 1, 0, 0, 0 },		// 강제 길드 탈퇴 시키기	// GDP_EXIT_GUILD_HIM
		{ 1, 0, 0, 0, 0 },		// 길드 마크 변경			// GDP_CHANGE_MARK
		{ 1, 0, 0, 0, 0 },		// 길드 직책 명 변경		// GDP_CHANGE_DEGREE_INFO
		{ 1, 0, 0, 0, 0 },		// 길드 마스터 변경			// GDP_GUILD_CHANGE_MASTER
		{ 1, 0, 0, 0, 0 },		// 길드 제거				// GDP_GUILD_BREAK
		{ 1, 0, 0, 0, 0 },		// 부길마 등록				// GDP_REGIST_SUB_MASTER
		{ 0, 1, 1, 1, 1 },		// 스스로 길드 탈퇴			// GDP_EXIT_GUILD_SELF
		{ 1, 0, 0, 0, 0 },		// 엠포리아 구입			// GDP_BUY_GUILD_HOUSE
	};

	int para = GetDegree2Param( ch->name_status.guild_master );
	return ((power_table[type][para])?true:false);
}	//> CSD-030324

void RecvGetNationItem( short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	t_packet packet;
	packet.h.header.type = CMD_GET_NATION_ITEM;
		packet.u.kein.nation_item.item_no = g_nation_item.item_no;
		if( g_nation_item.money > 0 )
		{
			packet.u.kein.nation_item.give_type = 0;		// money_type
			packet.u.kein.nation_item.give_much = g_nation_item.money;
		}
		else
		{
			packet.u.kein.nation_item.give_type = 1;		// money_type
			packet.u.kein.nation_item.give_much = g_nation_item.fame;
		}
	packet.h.header.size = sizeof( k_nation_item );
	QueuePacket( connections, cn, &packet, 1 );
}

extern void SaveChangeNationItem(CHARLIST* pTarget, int type, ItemAttr* item); // CSD-040224

void RecvBuyNationItem( short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	int ret = 0;
	if( g_nation_item.money > 0 )
	{
		if( ch->Money< g_nation_item.money )
		{
			// 돈이 부족하다. 
			ret = 2;
		}
		else
		{
			// 021209 YGI
			if( SubtractMoney( g_nation_item.money, ch ) == 1 )
			{
				ItemAttr item = GenerateItem( g_nation_item.item_no);
				SendItemEventLog( &item, cn, SN_NOT_USER, SILT_BUY_NATION_ITEM_MONEY, 100 );
				SaveChangeNationItem(ch, 0, &item);	// CSD-040224
				MoveEmptyInv( &item, ch );
				SendNationMoney( NM_ADD, ch->name_status.nation, g_nation_item.money); // 국고를 변화시킨다.
				ret = 1;
			}
			else
			{
				ret = 2;
			}
		}
	}
	else
	{
		if( ch->fame - g_nation_item.fame < 1000 )
		{
			// 명성치가 부족하다.
			ret = 3;
		}
		else
		{
			// 021209 YGI
			ret = 1;
			SaveChangeFame(ch,ch->fame,ch->fame-g_nation_item.fame,LF_ITEMBUY);
			ch->fame -= g_nation_item.fame;
			ItemAttr item = GenerateItem( g_nation_item.item_no);
			SendItemEventLog( &item, cn, SN_NOT_USER, SILT_BUY_NATION_ITEM_FAME, 100 );
			SaveChangeNationItem(ch, 1, &item);	// CSD-040224
			MoveEmptyInv( &item, ch );
		}
	}

	t_packet packet;
	packet.h.header.type = CMD_BUY_NATION_ITEM_OK;
		packet.u.kein.default_char = ret;
	packet.h.header.size = sizeof( char );
	QueuePacket( connections, cn, &packet, 1 );	
}

void RecvCheckColossusMap(t_packet *p, short int cn)
{	//< CSD-TW-030701
	CHARLIST* pTarget = CheckServerId(cn);
		
	if (pTarget == NULL)
	{
		return;
	}

	CArenaInfo* pArenaInfo = g_pArenaManager->GetArenaInfo(g_MapPort);

	if (pArenaInfo == NULL)
	{
		return;
	}

	CArenaManager::HASH_GAME mpArena = g_pArenaManager->GetGameSet();

	for (CArenaManager::ITOR_GAME i = mpArena.begin(); i != mpArena.end(); ++i)
	{
		CBaseArena* pGame = i->second;
		pGame->SendGameInfo(pTarget, g_MapPort);
	}
}	//> CSD-TW-030701

// 021107 YGI
void SendPacket2Map( t_packet *p, int port )
{
	if( port != ST_SEND_MAP_ALL	)
	{
		if( port == GetOwnPort() )	// 같은 맵으로 보낼 경우
		{
			HandleCommand( CN_FROM_OTHER_MAPSERVER_MSG, p );
		}
		else
		{
			g_pServerTable->SendRajaPacketToOtherMapServer( port, (char *)p, p->h.header.size+sizeof(t_header) );
		}
	}
	else // all
		g_pServerTable->BroadCastToEveryServer((char *)p, p->h.header.size+sizeof(t_header),SERVER_TYPE_MAP);		
}

void RecvInviteColossus( t_packet *p, short int cn )
{	//< CSD-030509
	CHARLIST* ch = CheckServerId(cn);
	
	if (ch == NULL)
	{
		return;
	}

	CBaseArena* pGame = ch->GetJoinArenaGame();

	if (pGame == NULL)
	{
		return;
	}
	// 방장만 사용할 수 있는 명령어
	if (pGame->GetLeader() != ch) 
	{
		return;		
	}
	
	t_packet packet;
	packet.h.header.type = CMD_INVITE_COLOSSUS_REPLY;
	packet.h.header.size = sizeof(k_invite_colossus_map) - 20 + strlen(ch->Name) + 1;
	packet.u.kein.invite_colossus_map.port = g_MapPort;
	strcpy_s(packet.u.kein.invite_colossus_map.name, sizeof(packet.u.kein.invite_colossus_map.name), ch->Name);
	SendPacket2Map(&packet, ST_SEND_MAP_ALL);
	RecvInviteColossusReply(&packet);
}	//> CSD-030509

void RecvInviteColossusReply( t_packet *p )
{	//< CSD-CN-031213
	g_pUserManager->SendPacket(p);
}	//> CSD-CN-031213

void RecvJoinColossusMap( t_packet *p, short int cn )
{	//< CSD-030519
	CHARLIST* ch = CheckServerId(cn);
	
	if (ch == NULL)
	{
		return;
	}

	const int nPort = p->u.kein.default_short_int;

	if (nPort == g_MapPort)
	{
		return;
	}

	CArenaInfo* pArena = g_pArenaManager->GetArenaInfo(nPort);

	if (pArena == NULL)
	{
		return;
	}
	
	t_MapInfo* pMapInfo = GetMapInfoByMapPort(nPort);

	if (pMapInfo == NULL)
	{
		return;
	}

	int nX = 0, nY = 0;
	pArena->GetWaitPosition(nX, nY);
	MapMove(cn, pMapInfo->mapfile, nX, nY);
}	//> CSD-030519

void RecvAttackColossusItem( t_packet *p, short int cn )
{	//< CSD-030509
	CHARLIST* ch = CheckServerId(cn);
	
	if (ch == NULL) 
	{
		return;
	}

	CBaseArena* pArena = ch->GetJoinArenaGame();

	if (pArena == NULL)
	{
		return;
	}

	if (!pArena->IsPlayGame())
	{
		return;
	}

	CArenaTeam* pTeam = ch->GetJoinArenaTeam();

	if (pTeam == NULL)
	{
		return;
	}

	const int nItem = p->u.kein.default_short_int;
	
	if (nItem < 0 || nItem >= MAX_ITEM_LIST) 
	{
		return;
	}

	SetStoneByTeam(&ItemList[nItem], pTeam->GetTeamNumber());

	t_packet packet;
	packet.h.header.type = CMD_ATTACK_COLOSSUS_STONE_RESULT;
	packet.h.header.size = sizeof(k_attack_colossus_stone_result);
	packet.u.kein.attack_colossus_stone_result.server_id = cn;
	packet.u.kein.attack_colossus_stone_result.item_id = nItem;
	packet.u.kein.attack_colossus_stone_result.attr = ItemList[nItem].attr[IATTR_ATTR];
	QueuePacket(connections, cn, &packet, 1);
	CastMe2Other(cn, &packet);
	// 현재 돌의 수를 카운트 해서 보내준다.
	int team_stone[4] = {0,};
	g_ColossusStone.GetTeamCount( team_stone );
	
	int i;
	for (i = 0; i < 2; ++i)
	{	//< CSD-030520
		CBaseArena* pGame = g_pArenaManager->GetGameInfo(i);

		if (pGame == NULL)
		{
			continue;
		}

		pGame->SendGameMessage(i, 2, team_stone[i]);
	}	//> CSD-030520
	// 전체에게 누가 스톤을 차지 했는지 알려준다.
	CBaseArena* pGame = ch->GetJoinArenaGame();

	if (pGame != NULL)
	{	//< CSD-030520
		pGame->SendGameMessage(ch, 5, ItemList[nItem].dumno+1);
	}	//> CSD-030520

	// 전부 차지 했는지 알아본다.
	int time = 0;
	int got_stone_team = 0;		// 돌을 독점한 팀
	for( i=0; i<2; i++ )
	{
		if( team_stone[i] == g_ColossusStone.GetMaxStone() )
		{
			got_stone_team = i;
			time = g_curr_time;
			break;
		}
	}
	g_ColossusStone.SetWinCondition( time, got_stone_team );
}	//> CSD-030509

void CallMenu( short int menu, short int cn )
{
	SendPacketDefault( CMD_CALL_MAIN_MENU, &menu, sizeof( short int ), cn );
}

void RecvRevivalColossus(t_packet *p, short int cn)
{	//< CSD-030509
	CHARLIST* ch = CheckServerId(cn);
	
	if (ch == NULL)
	{
		return;
	}

	CBaseArena* pGame = ch->GetJoinArenaGame();

	if (pGame == NULL)
	{
		return;
	}

	if (!pGame->IsPlayGame())
	{
		return;
	}

	CArenaTeam* pTeam = ch->GetJoinArenaTeam();

	if (pTeam == NULL)
	{
		return;
	}

	switch (pGame->GetArenaType())
	{
	case AT_STONE:
	case AT_MONSTER:
		{
			if (ch->viewtype != VIEWTYPE_GHOST_ && ch->Hp > 0) 
			{	// 유령이 아니다.
				return;
			}

			int nX = 0, nY = 0;
			pTeam->GetBasePosition(nX, nY);
			MovePc(cn, nX, nY);
			SkillMgr.CharacterToAlive(ch);
			// 전체 공지 메시지
			pGame->SendGameMessage(ch, 4, 0); // CSD-030520
			break;
		}
	}
}	//> CSD-030509

void RecvGuildHouseInfo( t_packet *p, short int cn )
{	//< CSD-030324
	CHARLIST *ch = CheckServerId( cn ); 
	if( !ch ) return;

	//if( !ch->GetGuildCode() ) return;
	int nation = ch->name_status.nation;

	t_packet packet;
	packet.h.header.type = CMD_GUILD_HOUSE_INFO;
	k_guild_house_info *t = (k_guild_house_info *)packet.u.data;
	CGuildHouseUnit *pData;
	t->count=0;
	for( int i=0; i<g_GuildHouse.m_Max; i++ )
	{
		pData = &g_GuildHouse.m_aGuildHouse[i];
		if( !pData->id ) continue;
		if( pData->nation != nation ) continue;
		t->data[t->count].id = pData->id;
		t->data[t->count].guild_code = pData->guild_code;
		t->data[t->count].blue = pData->blue;
		t->data[t->count].red = pData->red;
		t->data[t->count].lv = pData->level;

		t->data[t->count].time = 0;
		// 031009 CI YGI
		int time = 0;
		if( pData->year >= 1900 )
		{
			time += (pData->year-g_year )*365;
			time += (pData->month-g_mon-1)*30;
			time += pData->day-g_day;
			if( time < 0 ) t->data[t->count].time = 0;
			else if( time > 125 ) t->data[t->count].time = 60;
			else
			{
				t->data[t->count].time = time;
			}
		}
		
		strcpy_s( t->data[t->count].map_name, sizeof(t->data[t->count].map_name), pData->map_name );
		t->count ++;
	}
	packet.h.header.size = sizeof( k_guild_house_info_unit )*t->count+sizeof( char );
	QueuePacket( connections, cn, &packet, 1 );
	static int check_day = g_day;
	//if( check_day != g_day )
	{
		check_day = g_day;
		CallClient( DB_DEMON, CMD_CHECK_GUILD_HOUSE );		// 없어져야 할 길드가 있는지 체크한다. // 변경 사항을 전 맵에 뿌려준다.
	}
}	//> CSD-030324

void RecvBuyGuildHouse( t_packet *p, short int cn )
{	//< CSD-030324
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	int house_id = p->u.kein.default_char;
	int guild_code = ch->GetGuildCode();

	if( !CheckGuildPower( GDP_BUY_GUILD_HOUSE, ch ) ) return;		// 길마만 사용할 수 있다.

	CGuildHouseUnit	*pHouse = g_GuildHouse.GetHouseByIndex( house_id );		// 기존 길드가 있다.
	if( !pHouse  || !pHouse->id ) return;
	if( pHouse->nation != ch->name_status.nation ) return;


	if( GetItemMuchByMuchItem( ch, IN_BLUE_SIGNET ) < pHouse->blue ) return;		// 시그넷이 부족하다.
	if( GetItemMuchByMuchItem( ch, IN_RED_SIGNET ) < pHouse->red ) return;		// 시그넷이 부족하다.

	k_change_guild_house_info data;
	data.guild_code = guild_code;
	data.id = pHouse->id;		// 엠포리아 인덱스
	if( pHouse->guild_code )
	{
		if( pHouse->guild_code != guild_code ) return;

		int time = 0;
		if( pHouse->year >= 1900 )
		{
			time += (pHouse->year-g_year )*365;
			time += (pHouse->month-g_mon-1)*30;
			time += pHouse->day-g_day;
		}
		if( time < 0 || time > 5 )
		{
			// 한번만 연장 할 수 있다.
			SendPutMenuString( KM_FAIL, 174, cn );
			return;
		}
		// 이경우엔 디비 데몬이 모든서버로 보내주게 하자..
		data.day_type = 2;		// 연장
	}
	else
	{
		CGuildHouseUnit	*pData = g_GuildHouse.GetHouseByGuildCode( guild_code );
		if( pData ) return;		// 이미 가지고 있는 엠포리아가 있다.
		// 새로 신청
		data.day_type = 1;		// 신규 신청
	}

	// 삭제 // 취소 됐을 경우 삭제 되므로 DB-demon에서 취소 되면 안된다.
	SendDeleteMuchItemQuantity( ch, IN_BLUE_SIGNET, pHouse->blue );
	SendDeleteMuchItemQuantity( ch, IN_RED_SIGNET, pHouse->red );

	SendPacketDefault( CMD_CHANGE_GUILDHOUSE_INFO, &data, sizeof( k_change_guild_house_info ), DB_DEMON );
}	//> CSD-030324

void RecvChangeGuildHouse( t_packet *p, short int cn )
{
	// 엠포리아 정보 갱신
	int house_id = p->u.kein.change_guild_house_info_db2map.house_id;
	CGuildHouseUnit	*pHouse = g_GuildHouse.GetHouseByIndex( house_id );
	if( !pHouse  || !pHouse->id ) return;

	pHouse->guild_code		=	p->u.kein.change_guild_house_info_db2map.guild_code;
	pHouse->year			=	p->u.kein.change_guild_house_info_db2map.year;
	pHouse->month			=	p->u.kein.change_guild_house_info_db2map.month;
	pHouse->day				=	p->u.kein.change_guild_house_info_db2map.day;
}

void RecvCheckGuildHouse( t_packet *p, short int cn )
{
	int count = p->u.kein.check_guild_house.count;
	char *pHouseId = p->u.kein.check_guild_house.pHouseId;
	if( !count ) return;

	for( int i=0; i<count; i++ )
	{
		CGuildHouseUnit	*pHouse = g_GuildHouse.GetHouseByIndex( pHouseId[i] );
		if( !pHouse ) continue;
		pHouse->guild_code = 0 ;
	}
}

/////////////////////////////////////////////////////////////////////////////////////
// CPotionBox member fuctions
int CPotionBox::PutItem( ItemAttr *item )
{
	//if( !box_type ) return 0;
	CItem *t= ::ItemUnit( item->item_no );
	if( !t ) return 0;

	switch( t->GetItemKind())
	{
	case IK_POTION_BAG:		// BBD 040213	포션백도 포션박스에 들어가믄 않된다
	case IK_POTION_BOX:
	case IK_CANDLE:
	case IK_GOOD_CANDLE:
	case IK_COMBATSKILL_BOOK:
		{
			return 0;
		}break;
	}
	if( t->GetRbutton() != USE_ITEM ) {return 0;}

	if(item->attr[IATTR_RARE_MAIN])//기능 아이템은 못먹어
	{
		return 0;
	}

	if( much )
	{
		if( item_no != item->item_no ) {return 0;}
		if( 50<= much){return 0;}//020520 lsw 50개로 제한
	}
	else
	{
		item_no = item->item_no ;
	}
 
	much++;
	SaveData();
	::DeleteItem(item);
	
	return 1;
}

int CPotionBox::DeleteItem()
{
	if( !much ) return 0;		// 뺄거 없음
	much--;
	SaveData();
	return 1;
}

int CPotionBox::UseItem()
{
	if( !DeleteItem() ) return 0;		// 없다.
	CItem *t = ::ItemUnit( item_no );
	if( !t ) 
	{
		return 0;
	}
	
	switch( t->GetItemKind())//양초나 포션 상자일 경우 지워 버린다
	{
	case IK_POTION_BAG:		// BBD 040213	포션백도 포션박스에 들어가믄 않된다, 당근 사용도 않됨
	case IK_POTION_BOX:
	case IK_CANDLE:
	case IK_GOOD_CANDLE:
	case IK_COMBATSKILL_BOOK:
		{
			item_no = 0;
			much = 0;
			return 0;
		}break;
	}
	const int add_hp = t->EatItem( hero );
	return add_hp;
}

void CPotionBox::SaveData( )		// 아이템을 사용한후  저장한다.
{
	DWORD temp;
	temp = MAKELONG( item_no, much );
	hero->Sight = temp;
	hero->SendCharInfoBasic( CIB_POTION_BOX, GetData() );		// 변경 사항을 알려준다.////020704 lsw
}

void CPotionBox::Empty()
{
	int a, b, c;
	while(much > 0 && SearchInv(hero->inv, a, b, c))
	{
		much--;
		ItemAttr newItem = GenerateItem(item_no);
		hero->inv[a][b][c] = newItem;
		POS pos;
		SetItemPos(INV, a, b, c, &pos);
		SendServerEachItem( &pos, &newItem, hero->GetServerID());
	}
	CPotionBox::SaveData();
}

void RecvPkOnOff( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	int pk_on_off= p->u.kein.default_char;
	if( ch->pk_on_off != pk_on_off )
	{
		ch->pk_on_off = pk_on_off;
		QueuePacket( connections, cn, p, 1 );
	}
}

void RecvCMD_GET_PK_ON_OFF( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	SendPacketDefault( CMD_GET_PK_ON_OFF, &ch->pk_on_off, sizeof( char ), cn );
}

// 020430 YGI acer 3
//////////////////////////////////////////////////////////////////////
// CItemLog member functions
CItemLog	g_item_log;
//acer4
void CItemLog::Load( int port )
{
	int temp = 0;
	char MapServerConfigFileName[MAX_PATH];
	sprintf_s(MapServerConfigFileName, sizeof(MapServerConfigFileName),"%s/data/MapServerConfig.ini", GameServerDataPath);
	//if( GetOwnPort() == GetManagementMapPort( port ) )
	{
		temp = 0; temp = (int)GetPrivateProfileInt("ItemLog", "DeleteDay",0,MapServerConfigFileName);
		if( !temp ) return;
		m_nDeleteDay = temp;

		temp = 0; temp = (int)GetPrivateProfileInt("ItemLog", "SaveLevel",0,MapServerConfigFileName);
		if( temp ) m_nSaveLv = temp;

		temp = 0; temp = (int)GetPrivateProfileInt("ItemLog", "ItemLevel",0,MapServerConfigFileName);
		if( temp ) m_nItemLv = temp;

		MyLog( LOG_NORMAL, "Item Log config, DeleteDay = %d, SaveLevel = %d, ItemLevel = %d", m_nDeleteDay, m_nSaveLv, m_nItemLv );

		temp = 0; temp = (int)GetPrivateProfileInt("ItemLog", "trade",0,MapServerConfigFileName);
		if( temp ) 
		{
			m_aType2Lv[SILT_PUT_OTHER_CH] = temp;
			m_aType2Lv[SILT_GET_OTHER_CH] = temp;
		}
		temp = 0; temp = (int)GetPrivateProfileInt("ItemLog", "store",0,MapServerConfigFileName);
		if( temp ) 
		{
			m_aType2Lv[SILT_GET_STORE] = temp;
			m_aType2Lv[SILT_PUT_STORE] = temp;
		}
		temp = 0; temp = (int)GetPrivateProfileInt("ItemLog", "ground",0,MapServerConfigFileName);
		if( temp ) 
		{
			m_aType2Lv[SILT_PUT_GROUND] = temp;
			m_aType2Lv[SILT_GET_GROUND] = temp;
			m_aType2Lv[SILT_PUT_GROUND_MAX_INVEN] = temp;
			m_aType2Lv[SILT_DROP_BY_PK] = temp;
		}
		temp = 0; temp = (int)GetPrivateProfileInt("ItemLog", "duration",0,MapServerConfigFileName);
		if( temp ) 
		{
			m_aType2Lv[SILT_DELETE_BY_DURATION] = temp;
		}
		temp = 0; temp = (int)GetPrivateProfileInt("ItemLog", "stolen",0,MapServerConfigFileName);
		if( temp ) 
		{
			m_aType2Lv[SILT_GET_OTHER_CH_INVEN] = temp;
			m_aType2Lv[SILT_GET_STOLEN] = temp;
			m_aType2Lv[SILT_PUT_STOLEN] = temp;
		}
		temp = 0; temp = (int)GetPrivateProfileInt("ItemLog", "make_by_skill",0,MapServerConfigFileName);
		if( temp ) 
		{
			m_aType2Lv[SILT_MAKE_BY_SKILL] = temp;
		}
		temp = 0; temp = (int)GetPrivateProfileInt("ItemLog", "make_by_event",0,MapServerConfigFileName);
		if( temp ) 
		{
			m_aType2Lv[SILT_MAKE_BY_DB] = temp;
			m_aType2Lv[SILT_MAKE_BY_SCRIPT] = temp;
			m_aType2Lv[SILT_GET_GAMBLE_ITEM] = temp;
			m_aType2Lv[SILT_PUT_GAMBLE_ITEM] = temp;
			m_aType2Lv[SILT_DELETE_BY_SCRIPT] = temp;
		}
		temp = 0; temp = (int)GetPrivateProfileInt("ItemLog", "resource",0,MapServerConfigFileName);
		if( temp ) 
		{
			m_aType2Lv[SILT_RESOURCE_BY_SKILL] = temp;
			m_aType2Lv[SILT_NEED_ITEM] = temp;
		}
		temp = 0; temp = (int)GetPrivateProfileInt("ItemLog", "man2man",0,MapServerConfigFileName);
		if( temp ) 
		{
			m_aType2Lv[SILT_GET_MEN_TO_MEN] = temp;
			m_aType2Lv[SILT_PUT_MEN_TO_MEN] = temp;
		}
	}
	// 020620 YGI
	sprintf_s(MapServerConfigFileName, sizeof(MapServerConfigFileName),"%s/data/NoItemLog.txt", GameServerDataPath);
	m_NoItemList.Load( MapServerConfigFileName );
}

bool CItemLog::IsSave( ItemAttr *item, int event_type, int lv )
{
	if( lv == 100 ) return true;		// 무조건 저장 // 030506 YGI

	if( !item ) return false;
	if( item->attr[IATTR_RARE_MAIN] ) return true;			// 레어 저장
	
	if( m_NoItemList.Search( item->item_no ) ) return false;		// 저장하지 말아라 목록에 있으면 저장하지 않는다.
	if( item->item_no/1000 == ACCESSORY ) return true;		// 유니크면 저장
	
	CItem *t = ::ItemUnit( item->item_no );
	if( !t ) return false;

	if( t->GetLevel() < m_nItemLv ) return false;			// 아이템 레벨이 낮으면 저장하지 않는다.
	if( !m_aType2Lv[event_type] ) return false;				// 0 일경우 저장하지 않는다.
	if( m_nSaveLv && m_aType2Lv[event_type] > m_nSaveLv ) return false;	// 이벤트 타입이 낮으면 저장하지 않는다.
	
	return true;		// 아이템 레벨과 타입이 높아야지만 저장한다.
}

// 020620 YGI
////////////////////////////////////////////////////////////////////////////
// class C2JinSearch

int C2JinSearch::Load( char *filename )
{
	Remove();

	FILE *fp = fopen( filename, "rt" );
	if( !fp ) return 0;	

	char temp[256];
	int count = 0;
	while( fgets( temp, 256, fp ) )
	{
		if( temp[0] == ' ' || temp[0] == ';' || temp[0] == '\n' ) continue;
		if( !atoi( temp ) ) continue;
		count++;
	}
	fseek( fp, 0, SEEK_SET );
	max = count;
	item = new int[max];
	count = 0;
	while( fgets( temp, 256, fp ) )
	{
		if( temp[0] == ' ' || temp[0] == ';' || temp[0] == '\n' ) continue;
		int value = atoi( temp );
		if( !value ) continue;
		item[count] = value;
		count++;
	}
	Sort();
	fclose( fp );
	return 1;
}

int C2JinSearch::Sort()
{
	if( !max ) return 0;
	for( int i=0; i<max-1; i++ )
	{
		for( int j=i+1; j<max; j++ )
		{
			if( item[i] > item[j] )
			{
				int temp = item[i];
				item[i] = item[j];
				item[j] = temp;
			}
		}
	}
	return 1;
}

int C2JinSearch::Search( int no )
{
	if( !max ) return 0;
	int low=0, high = max-1;
	int mid = 0;
	while( low <= high )
	{
		mid = (low+high)/2;
		if( item[mid] == no ) return 1;
		if( item[mid] < no ) low = mid+1;
		else high = mid-1;
	}

	return 0;
}

int RecvSaveItemLogByEvent( t_packet *p )
{	//< CSD-030806
	if( g_MapPort == GetManagementMapPort( MM_SAVE_LOG_FILE_MAP ) ) 
	{
		k_item_log *pData = &p->u.kein.item_log;
		if( g_item_log.GetSaveLv() )
		{
			//받는데에서는 다 저장하자.
			//if( pData->lv > g_item_log.GetSaveLv()  ) return 0;
		}
		return SaveItemLog( pData );
	}
	return 0;
}	//> CSD-030806

void SendItemEventLog( ItemAttr *item, short int hero_cn, short int target, int event_type, int lv )
{
	if( !g_item_log.IsSave( item, event_type, lv ) ) return;		// 저장 등급이 안되면 리턴하자.. // 030506 YGI

	CHARLIST *hero = CheckServerId( hero_cn );
	if( !hero ) return;

	t_packet packet;
	k_item_log *pData = &packet.u.kein.item_log;
	packet.h.header.type = CMD_SAVE_ITEM_LOG;
	packet.h.header.size = sizeof( k_item_log );

	if( lv == 0 ) lv = 1;
	pData->item = *item;
	pData->lv = g_item_log.GetTypeLv(event_type)?g_item_log.GetTypeLv(event_type):1;	// 030506 YGI
	pData->port = GetOwnPort();	
	pData->type = event_type;
	strcpy_s( pData->name1, sizeof(pData->name1), hero->Name );
	if( target > 0 )
	{
		CHARLIST *target_ch;
		if( target > 10000 )	target_ch = CheckNpcId( target-10000 );
		else					target_ch = CheckServerId( target );

		if( target_ch )	
		{
			if( target_ch->IsNpc() )
			{
				strcpy_s( pData->name2, sizeof(pData->name2), "<_NPC_>");
			}
			else
			{
				memcpy( pData->name2, target_ch->Name, 20 );//020510 lsw //혹시 모를 Garbage Value
			}
		}
		else
		{
			strcpy_s( pData->name2, sizeof(pData->name2), "<_NULL_>" );
		}
	}
	else
	{
		switch( event_type )
		{
			default :	//acer7
			case SILT_GET_STOLEN					:
				strcpy_s( pData->name2, sizeof(pData->name2), "<_GETSTOLEN_>" );
				break;
			case SILT_PUT_STOLEN					:
				strcpy_s( pData->name2, sizeof(pData->name2), "<_PUTSTOLEN_>" );
				break;
			case SILT_GET_OTHER_CH_INVEN	:	
				strcpy_s( pData->name2, sizeof(pData->name2), "<_GETOTHCHINV_>" );
				break;
			case SILT_PUT_OTHER_CH				:
				strcpy_s( pData->name2, sizeof(pData->name2), "<_PUTOTHCHINV_>" );
				break;
			case SILT_GET_OTHER_CH				:
				strcpy_s( pData->name2, sizeof(pData->name2), "<_GETOTHCH_>" );
				break;
			case SILT_PUT_GROUND					:
				strcpy_s( pData->name2, sizeof(pData->name2), "<_PUTGROUND_>" );
				break;
			case SILT_GET_GROUND					:
				strcpy_s( pData->name2, sizeof(pData->name2), "<_GETGROUND_>" );
				break;
			case SILT_PUT_BANK						:
				strcpy_s( pData->name2, sizeof(pData->name2), "<_PUBANK_>" );
				break;
			case SILT_GET_BANK						:
				strcpy_s( pData->name2, sizeof(pData->name2), "<_GETBANK_>" );
				break;

			case SILT_MAKE_BY_DB					:	
				strcpy_s( pData->name2, sizeof(pData->name2), "<_MAKEDB_>" );
				break;
			case SILT_MAKE_BY_SCRIPT			:	
				strcpy_s( pData->name2, sizeof(pData->name2), "<_MAKESPT_>" );
				break;
			case SILT_MAKE_BY_SKILL				:
				strcpy_s( pData->name2, sizeof(pData->name2), "<_MAKESKL_>" );
				break;
			case SILT_RESOURCE_BY_SKILL		:
				strcpy_s( pData->name2, sizeof(pData->name2), "<_RESMAKE_>" );
				break;
			case SILT_PUT_STORE						:
				strcpy_s( pData->name2, sizeof(pData->name2), "<_GETSTORE_>" );
				break;
			case SILT_GET_STORE						:
				strcpy_s( pData->name2, sizeof(pData->name2), "<_PUTSTORE_>" );
				break;

			case SILT_GET_GAMBLE_ITEM			:
				strcpy_s( pData->name2, sizeof(pData->name2), "<_GAMBLE_>" ); //From this one to RAREATTR, put new "reason" for those logs by Eleval, to check which one makes items disapear. 12/07/09
				break;
			case SILT_GET_TODAY_ITEM			:	
				strcpy_s( pData->name2, sizeof(pData->name2), "<_TODAYITEM_>" );
				break;
			case SILT_GET_GUILD_WAR				:
				strcpy_s( pData->name2, sizeof(pData->name2), "<_GUILDWAR_>" );
				break;
			case SILT_USE									:
				strcpy_s( pData->name2, sizeof(pData->name2), "<_USE_>" );
				break;
			case SILT_DELETE_BY_DURATION	:
				strcpy_s( pData->name2, sizeof(pData->name2), "<_DURATION_>" );
				break;
			case SILT_DELETE_BY_ATTR			:
				strcpy_s( pData->name2, sizeof(pData->name2), "<_ATTRIBUTE_>" );
				break;
			case SILT_DELETE_BY_RARE_ATTR	:
				//strcpy_s( pData->name2, sizeof(pData->name2), "<_SERVER_>" );
				strcpy_s( pData->name2, sizeof(pData->name2), "<_RAREATTR_>" );
				break;
			//<! BBD 040226	옥션 거래시 로그라는걸 알수 있도록 추가
			case SILT_REGISTER_AUCTION_ITEM		:	// 옥션 등록시
				strcpy_s( pData->name2, sizeof(pData->name2), "<_REGAUCTION_>" );
				break;
			case SILT_CANCEL_AUCTION_SELL_ITEM	:	// 등록 취소시
				strcpy_s( pData->name2, sizeof(pData->name2), "<_CANAUCTION_>" );
				break;
			case SILT_TAKE_AUCTION_RESULT_ITEM	:	// 구입한 물건 찾을시
				strcpy_s( pData->name2, sizeof(pData->name2), "<_TAKEAUCTION_>" );
				break;
			//> BBD 040226	옥션 거래시 로그라는걸 알수 있도록 추가

		}
	}
	SendDirectMap( &packet, GetManagementMapPort( MM_SAVE_LOG_FILE_MAP ) );
}

int GetServerSetNum()
{
	return g_pServerTable->GetServerSetNum();
}

// GM 기능, 맵 강제 이동
//acer7
void RecvDirectMapMove( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;
	if( !ch->IsCounselor() && !ch->IsReporter()) return; //Eleval - Added Reporter for new command for them

	int index = p->u.kein.direct_map_move.map_number;
	int x = p->u.kein.direct_map_move.x;
	int y = p->u.kein.direct_map_move.y;
	char *name = p->u.kein.direct_map_move.name;
	int target_id = cn;

	if( name[0] )
	{
		int temp = ExistHe( name );		// cn 값이 있다면...
		if( temp == -1 ) 
		{
			SendPutMenuString( KM_FAIL, 181, cn );		// 같은 맵에 없는 캐릭터 입니다.
			return;		// 상대편 이름이 잘 못됐다.
		}
		target_id = temp;
	}
	CrackMgr.SendHackingUser( cn, USE_GM_TOOL, " Map Move " );
	GotoUser( MapInfo[index].mapfile, x, y, target_id );			// 020705 YGI
}


// acer4
void RecvSearchChar( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;
	if( !ch->IsCounselor() ) return;

	t_packet packet;
	char *name = p->u.kein.default_name;
	int he = ExistHe( name );
	if( he > 0 )
	{
		CHARLIST *him = CheckServerId( he );
		if( !him ) return;

		PutHimInfo( him, &packet.u.kein.char_info );
		packet.h.header.type = CMD_SEARCH_OTHER_CHAR;
		packet.h.header.size = sizeof( k_char_info );
		QueuePacket( connections, cn, &packet, 1 );
		return;
	}
	else		// 다른 맵으로 요청한다.
	{
		packet.h.header.type = CMD_SEARCH_OTHER_CHAR_MAP;
		packet.u.kein.char_info_map.port = GetOwnPort(); 
		packet.u.kein.char_info_map.cn = cn;
		strcpy_s( packet.u.kein.char_info_map.name, sizeof(packet.u.kein.char_info_map.name), name );
		packet.h.header.size = sizeof( k_char_info_map );
		SendPacket2Maps( &packet );
	}
}

void RecvTeleportToChar( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;
	if( !ch->IsCounselor() && !ch->IsReporter()) return;

	t_packet packet;
	char *name = p->u.kein.default_name;
	int he = ExistHe( name );
	if( he > 0 )
	{
		CHARLIST *him = CheckServerId( he );
		if( !him ) return;

		PutHimInfo( him, &packet.u.kein.char_info );
		packet.h.header.type = CMD_TELEPORT_OTHER_CHAR;
		packet.h.header.size = sizeof( k_char_info );
		QueuePacket( connections, cn, &packet, 1 );
		return;
	}
	else		// 다른 맵으로 요청한다.
	{
		packet.h.header.type = CMD_TELEPORT_OTHER_CHAR_MAP;
		packet.u.kein.char_info_map.port = GetOwnPort(); 
		packet.u.kein.char_info_map.cn = cn;
		strcpy_s( packet.u.kein.char_info_map.name, sizeof(packet.u.kein.char_info_map.name), name );
		packet.h.header.size = sizeof( k_char_info_map );
		SendPacket2Maps( &packet );
	}
}

// acer4
int PutHimInfo( CHARLIST *him, k_char_info *him_info )
{	//< CSD-030324
	him_info->map_number = MapNumber;
	him_info->curr_tactic_kind = him->GetTacticsKind();
	him_info->curr_tactic_lv = him->GetWeaponSkill();
	memcpy( &him_info->name_status, &him->name_status, sizeof( DWORD ) );
	him_info->btLevel = him->GetLevel(); // CSD-030806
	him_info->nk = him->GetNK( him->name_status.nation );
	him_info->x = him->MoveSx;
	him_info->y = him->MoveSy;
	him_info->guild_code = him->GetGuildCode();
	return 1;
}	//> CSD-030324

void RecvSearchCharMap( t_packet *p )
{
	int server_id = p->u.kein.char_info_map.cn;
	int port = p->u.kein.char_info_map.port;
	char *name = p->u.kein.char_info_map.name;

	int he = ExistHe( name );
	if( he > 0 )
	{
		CHARLIST *him = CheckServerId( he );
		if( !him ) return;

		k_char_info data;
		PutHimInfo( him, &data );
		SendDirectClientOtherMap( CMD_SEARCH_OTHER_CHAR, server_id, port, &data, sizeof( k_char_info ) );
	}
}

void RecvTeleportToCharMap( t_packet *p )
{
	int server_id = p->u.kein.char_info_map.cn;
	int port = p->u.kein.char_info_map.port;
	char *name = p->u.kein.char_info_map.name;

	int he = ExistHe( name );
	if( he > 0 )
	{
		CHARLIST *him = CheckServerId( he );
		if( !him ) return;

		k_char_info data;
		PutHimInfo( him, &data );
		SendDirectClientOtherMap( CMD_TELEPORT_OTHER_CHAR, server_id, port, &data, sizeof( k_char_info ) );
	}
}

//acer7
void RecvUseGmTool_SaveLog( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	char *type = p->u.kein.default_msg;
	CrackMgr.SendHackingUser( cn, USE_GM_TOOL, type );
}

void RecvEventNumber( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	int event_no = p->u.kein.default_int;
}

void MapServerFree_kein()
{
	SAFE_DELETE(g_pDungeonMap);
	SAFE_DELETE(g_pDungeonMgr);
	
}

void SaveChangeLadder(CHARLIST* pTarget, int old_ladder, int new_ladder, int type)
{	//< CSD-040224
	string strPath;

	if (!g_pLogManager->GetLogPath(LT_CHANGE, strPath))
	{
		return;
	}
	
	FILE* fp = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");

	if(fp == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, pTarget->Name);
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (fp == NULL)
	{
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pTarget->GetServerID()].id), "at+");

		if (fp == NULL)
		{
			return;
		}
	}
	
	char why[MAX_PATH];

	switch (type)
	{
		case CLT_CHANGE_DUAL: 
			{
				strcpy_s(why, sizeof(why), "upgrade dual");
				break;
			}
		case CLT_WIN_COLOSSUS:
			{
				strcpy_s(why, sizeof(why), "win colossus");
				break;
			}
		case CLT_LOSE_COLOSSUS:
			{
				strcpy_s(why, sizeof(why), "loss colossus");
				break;
			}
		default:
			{
				strcpy_s(why, sizeof(why), "default");
				break;
			}
	}

	fprintf(fp,
		    "@ChangeLadderScore	%02d/%02d/%02d/%02d/%02d/%02d	(old ladder)%d(==>)	(new ladder)%d	(why)%s",
			g_year - 2000, g_mon + 1, g_day, g_hour, g_min, g_sec,
			old_ladder, new_ladder, why);
	fprintf(fp, "\n");
	fclose(fp);
}	//> CSD-040224

void SaveObtainCombat(CHARLIST* pTarget, int nOld, int nNew)
{	//< CSD-040224
	string strPath;

	if (!g_pLogManager->GetLogPath(LT_CHANGE, strPath))
	{
		return;
	}
	
	FILE* fp = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");

	if(fp == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, pTarget->Name);
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (fp == NULL)
	{
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pTarget->GetServerID()].id), "at+");

		if (fp == NULL)
		{
			return;
		}
	}
	
	fprintf(fp ,
		    "@ObtainCombatPoint	%02d/%02d/%02d/%02d/%02d/%02d	(old point)%d(==>)	(new point)%d\n",
		    g_year - 2000, g_mon + 1, g_day, g_hour, g_min, g_sec, nOld, nNew);
	fprintf(fp, ";Lightning Boom    %3d  Thunder Blow      %3d\n", pTarget->GetCombatLevel(LIGHTNING_BOOM), pTarget->GetCombatLevel(THUNDER_BLOW));
	fprintf(fp, ";Lightning Shock   %3d  Thunder Strike    %3d\n", pTarget->GetCombatLevel(LIGHTNING_SHOCK), pTarget->GetCombatLevel(THUNDER_STRIKE));
	fprintf(fp, ";Lightning Shield  %3d  Lightning Extreme %3d\n", pTarget->GetCombatLevel(LIGHTNING_SHIELD), pTarget->GetCombatLevel(LIGHTNING_EXTREME));
	fprintf(fp, ";Blood Earth       %3d  Ground Attack     %3d\n", pTarget->GetCombatLevel(BLOOD_EARTH), pTarget->GetCombatLevel(GROUND_ATTACK));
	fprintf(fp, ";Blood Will        %3d  Stone Armor       %3d\n", pTarget->GetCombatLevel(BLOOD_WILL), pTarget->GetCombatLevel(STONE_ARMOR));
	fprintf(fp, ";Simunian Bosom    %3d  Earth Extreme     %3d\n", pTarget->GetCombatLevel(SIMUNIAN_BOSOM), pTarget->GetCombatLevel(EARTH_EXTREME));
	fprintf(fp, ";Sword N'Roses     %3d  Multiple Fire     %3d\n", pTarget->GetCombatLevel(SWORD_N_ROSES), pTarget->GetCombatLevel(MULTIPLE_FIRE));
	fprintf(fp, ";Fleet Missile     %3d  Converting Armor  %3d\n", pTarget->GetCombatLevel(FLEET_MISSILE), pTarget->GetCombatLevel(CONVERTING_ARMOR));
	fprintf(fp, ";Burning Bloody	%3d  Fire Extreme      %3d\n", pTarget->GetCombatLevel(BURNING_BLOODY), pTarget->GetCombatLevel(FIRE_EXTREME)); // 030415 kyo 
	fprintf(fp, ";Poisoning Nova    %3d  Cherroy Shade     %3d\n", pTarget->GetCombatLevel(POISONING_NOVA), pTarget->GetCombatLevel(CHERROY_SHADE));
	fprintf(fp, ";Dark Blade        %3d  Tybern Gift       %3d\n", pTarget->GetCombatLevel(DARK_BLADE), pTarget->GetCombatLevel(TYBERN_GIFT));
	fprintf(fp, ";AcuquireDark      %3d  Dark Extreme      %3d\n", pTarget->GetCombatLevel(ACUQUIRE_DARK), pTarget->GetCombatLevel(DARK_EXTREME));// 030415 kyo 
	fprintf(fp, ";Icing Blast       %3d  Critical Hiding   %3d\n", pTarget->GetCombatLevel(ICING_BLAST), pTarget->GetCombatLevel(CRITICAL_HIDING));
	fprintf(fp, ";Shark Missile     %3d  Horn Of Iceberg   %3d\n", pTarget->GetCombatLevel(SHARK_MISSILE), pTarget->GetCombatLevel(HORN_OF_ICEBERG));
	fprintf(fp, ";Ice Shield        %3d  Ice Extreme       %3d\n", pTarget->GetCombatLevel(ICE_SHIELD), pTarget->GetCombatLevel(ICE_EXTREME));
	fprintf(fp, ";Charging          %3d  Double Attack     %3d\n", pTarget->GetCombatLevel(CHARGING), pTarget->GetCombatLevel(DOUBLE_ATTACK));
	fprintf(fp, ";Whilwind          %3d  Twister           %3d\n", pTarget->GetCombatLevel(WHILWIND), pTarget->GetCombatLevel(TWISTER));
	fprintf(fp, ";Gust              %3d  Wind Extreme      %3d\n", pTarget->GetCombatLevel(GUST), pTarget->GetCombatLevel(WIND_EXTREME));
	fprintf(fp, ";Reserved Combat Skill Point %3d \n", pTarget->GetCombatPoint());
	
	fclose(fp);
}	//> CSD-040224

void SaveChangeCombat(CHARLIST* pTarget, int nCombat, int nOld, int nNew)
{	//< CSD-040224
	string strPath;
	
	if (!g_pLogManager->GetLogPath(LT_CHANGE, strPath))
	{
		return;
	}
	
	FILE* fp = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");

	if(fp == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, pTarget->Name);
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (fp == NULL)
	{
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pTarget->GetServerID()].id), "at+");

		if (fp == NULL)
		{
			return;
		}
	}
	
	fprintf(fp,
		    "@ChangeCombatPoint	%02d/%02d/%02d/%02d/%02d/%02d	(old level)%d(==>)	(new level)%d	(combat skill)%d\n",
		    g_year - 2000, g_mon + 1, g_day, g_hour, g_min, g_sec, nOld, nNew, nCombat);
	fprintf(fp, ";Lightning Boom    %3d  Thunder Blow      %3d\n", pTarget->GetCombatLevel(LIGHTNING_BOOM), pTarget->GetCombatLevel(THUNDER_BLOW));
	fprintf(fp, ";Lightning Shock   %3d  Thunder Strike    %3d\n", pTarget->GetCombatLevel(LIGHTNING_SHOCK), pTarget->GetCombatLevel(THUNDER_STRIKE));
	fprintf(fp, ";Lightning Shield  %3d  Lightning Extreme %3d\n", pTarget->GetCombatLevel(LIGHTNING_SHIELD), pTarget->GetCombatLevel(LIGHTNING_EXTREME));
	fprintf(fp, ";Blood Earth       %3d  Ground Attack     %3d\n", pTarget->GetCombatLevel(BLOOD_EARTH), pTarget->GetCombatLevel(GROUND_ATTACK));
	fprintf(fp, ";Blood Will        %3d  Stone Armor       %3d\n", pTarget->GetCombatLevel(BLOOD_WILL), pTarget->GetCombatLevel(STONE_ARMOR));
	fprintf(fp, ";Simunian Bosom    %3d  Earth Extreme     %3d\n", pTarget->GetCombatLevel(SIMUNIAN_BOSOM), pTarget->GetCombatLevel(EARTH_EXTREME));
	fprintf(fp, ";Sword N'Roses     %3d  Multiple Fire     %3d\n", pTarget->GetCombatLevel(SWORD_N_ROSES), pTarget->GetCombatLevel(MULTIPLE_FIRE));
	fprintf(fp, ";Fleet Missile     %3d  Converting Armor  %3d\n", pTarget->GetCombatLevel(FLEET_MISSILE), pTarget->GetCombatLevel(CONVERTING_ARMOR));
	fprintf(fp, ";Burning Bloody	  %3d  Fire Extreme      %3d\n", pTarget->GetCombatLevel(BURNING_BLOODY), pTarget->GetCombatLevel(FIRE_EXTREME)); // 030415 kyo 
	fprintf(fp, ";Poisoning Nova    %3d  Cherroy Shade     %3d\n", pTarget->GetCombatLevel(POISONING_NOVA), pTarget->GetCombatLevel(CHERROY_SHADE));
	fprintf(fp, ";Dark Blade        %3d  Tybern Gift       %3d\n", pTarget->GetCombatLevel(DARK_BLADE), pTarget->GetCombatLevel(TYBERN_GIFT));
	fprintf(fp, ";AcuquireDark      %3d  Dark Extreme      %3d\n", pTarget->GetCombatLevel(ACUQUIRE_DARK), pTarget->GetCombatLevel(DARK_EXTREME)); // 030415 kyo 
	fprintf(fp, ";Icing Blast       %3d  Critical Hiding   %3d\n", pTarget->GetCombatLevel(ICING_BLAST), pTarget->GetCombatLevel(CRITICAL_HIDING));
	fprintf(fp, ";Shark Missile     %3d  Horn Of Iceberg   %3d\n", pTarget->GetCombatLevel(SHARK_MISSILE), pTarget->GetCombatLevel(HORN_OF_ICEBERG));
	fprintf(fp, ";Ice Shield        %3d  Ice Extreme       %3d\n", pTarget->GetCombatLevel(ICE_SHIELD), pTarget->GetCombatLevel(ICE_EXTREME));
	fprintf(fp, ";Charging          %3d  Double Attack     %3d\n", pTarget->GetCombatLevel(CHARGING), pTarget->GetCombatLevel(DOUBLE_ATTACK));
	fprintf(fp, ";Whilwind          %3d  Twister           %3d\n", pTarget->GetCombatLevel(WHILWIND), pTarget->GetCombatLevel(TWISTER));
	fprintf(fp, ";Gust              %3d  Wind Extreme      %3d\n", pTarget->GetCombatLevel(GUST), pTarget->GetCombatLevel(WIND_EXTREME));
	fprintf(fp, ";Reserved Combat Skill Point %3d \n", pTarget->GetCombatPoint());
	
	fclose(fp);
}	//> CSD-040224

// 021117 YGI
void RecvSetInstallVersion ( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;
	ch->installType = IT_LIGHT_VERSION;
}

//021030 YGI
void RecvResendItem( short int cn )
{
	if( !CheckServerIndex( cn ) ) return;
	
	SendItemInventory( 0, connections, cn );
	SendItemInventory( 1, connections, cn );
	SendItemInventory( 2, connections, cn );
	SendItemInventory( 3, connections, cn );
	SendItemInventory( 4, connections, cn );
	SendItemInventory( 5, connections, cn );
	SendItemInventory( 6, connections, cn );
	SendItemInventory( 7, connections, cn );
	SendItemInventory( 8, connections, cn );
}

void RecvScriptItem( t_packet *p, short int cn )
{
	k_event_script_item	*pData = (k_event_script_item *)p->u.data;

	CEventNpcScript *pEvent = g_EventMgr.GetEventNpcScriptByEventNo( pData->event_no);
	if( !pEvent ) return;

	CHARLIST *ch = CheckServerId( pData->server_id );
	if( !ch ) return;

	if( strcmp( ch->Name, pData->name ) != 0 ) return ;
	
	if( pData->result )
	{
		SendEventNpcScriptItem( pData->server_id, pData->npc_index, pEvent->m_nEventNpcItemBag, 
			pEvent->m_lpszMessage, pEvent->m_nMessageSize );
	}
	else		// 아이템을 만들지 말고 실패한 이야기를 들려주자
	{
		if( pEvent->m_lpszMessage2 && pEvent->m_nMessageSize2 )		// npc의 대화를 보내준다.
			SendDial( pData->server_id, pData->npc_index, pEvent->m_lpszMessage2, pEvent->m_nMessageSize2 );

	}
}

void RecvEventMoveMap( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	int event_no = p->u.kein.default_int;
	CEventMoveMap *pEvent = g_EventMgr.GetEventMoveMapPoint( event_no );
	if( !pEvent ) return;

	// 이벤트 참여 조건 확인 --------------------
	if( !pEvent->m_bAll )
	{
		static int sex2sex[] = { 2, 1 };
		static int class2class[] = { 1, 3, 2, 5, 4 };
		static int nation2nation[] = { 0, 0, 0, 1, 2, 0, 3 };

		if( pEvent->m_nSex )
		{
			if( pEvent->m_nSex != sex2sex[ch->SprNo] ) return;
		}
		if( pEvent->m_nClass )
		{
			if( pEvent->m_nClass != class2class[ch->Class] ) return;
		}
		if( pEvent->m_nNation )
		{
			if( pEvent->m_nNation != nation2nation[ch->name_status.nation] ) return;
		}
	}
	//----------------------------

	t_packet packet;
	packet.h.header.type = CMD_EVENT_MOVE_MAP_CHECK_MAP;
		packet.u.kein.event_move_map_check_map.server_id = cn;
		packet.u.kein.event_move_map_check_map.server_port = GetOwnPort();
		packet.u.kein.event_move_map_check_map.event_no = event_no;
	packet.h.header.size =  sizeof( k_event_move_map_check_map );

	int port = GetMapPortByName( pEvent->m_szMap );
	if( port == GetOwnPort() ) return;
	if( !port )	return;
	SendPacket2Map( &packet, port );
}

void RecvEventMoveMapCheckMap( t_packet *p, short int cn )		// 다른 맵에서 올경우
{
	k_event_move_map_check_map *pData = (k_event_move_map_check_map *)p->u.data;
	CEventMoveMap *pEvent = g_EventMgr.GetEventMoveMapPoint( pData->event_no );
	if( !pEvent ) return;
	if( _stricmp( pEvent->m_szMap, MapName ) != 0 ) return;		// 이벤트 번호가 꼬였다. 

	int user_count = g_pServerTable->GetNumOfUsers();
	if( user_count >= pEvent->m_nMax )
	{
		// 더이상 참가할 수 없다.
		k_put_menu_string msg;
		msg.type = KM_FAIL;
		msg.str_num = 212;

		SendDirectClientOtherMap( CMD_PUT_MENU_STRING, pData->server_id, pData->server_port, &msg, sizeof( k_put_menu_string ) );
	}
	else
	{
		t_packet packet;
		packet.h.header.type = CMD_EVENT_MOVE_MAP_OK;
			strcpy_s( packet.u.kein.event_move_map_ok.map_name, sizeof(packet.u.kein.event_move_map_ok.map_name), MapName );
			packet.u.kein.event_move_map_ok.server_id = pData->server_id;
			packet.u.kein.event_move_map_ok.map_x = pEvent->m_nMapX;
			packet.u.kein.event_move_map_ok.map_y = pEvent->m_nMapY;

		packet.h.header.size = sizeof( k_event_move_map_ok );

		SendPacket2Map( &packet, pData->server_port );

		// 서버에 저장한다.
		k_save_event_move_map save_data;
		save_data.event_no = pData->event_no;
		save_data.max = pEvent->m_nMax;	// 021128 YGI
		save_data.now = user_count;
		SendPacketDefault( CMD_SAVE_EVENT_MAPMOVE_NOW, &save_data, sizeof( k_save_event_move_map ), DB_DEMON );
	}
}

void RecvEventMoveMapOk( t_packet *p, short int cn )
{
	k_event_move_map_ok *pData = (k_event_move_map_ok *)p->u.data;
	MapMove( pData->server_id, pData->map_name, pData->map_x, pData->map_y );
}

void CheckDoubleName( CHARLIST *ch, short int cn )
{
	char name[20]={0,};
	::strcpy_s( name, sizeof(name), ch->Name );
	int target_id = ExistHe( ch->Name );
	if( target_id > 0 )		// 누군가 같은 이름을 가지고 있다.
	{
		MyLog(1,"CheckDoubleName() name = %s", ch->Name );
		closeconnection( connections, target_id, -101 );		// 일단 다른 놈을 끊고 --> 저장
		ch->updatable = 0; // CSD-TW-030620
		closeconnection( connections, cn, -102 );				// 동시 접속 한 놈은 저장하지 말면서 끊자...
	}
	else
	{
		target_id = ExistConnectingHe( ch->Name );
		if( target_id > 0 )
		{
			connections[target_id].chrlst.updatable = 1; // CSD-TW-030620
			closeconnection( connections, target_id, -103 );		// 일단 다른 놈을 끊고 --> 저장
			ch->updatable = 0; // CSD-TW-030620
			closeconnection( connections, cn, -104 );				// 동시 접속 한 놈은 저장하지 말면서 끊자...
		}
	}

	// 다른 곳에 있는 놈도 끊자..
	t_packet packet;
	packet.h.header.type = CMD_CHECK_DOUBLE_NAME;
		strcpy_s( packet.u.kein.invite_colossus_map.name, sizeof(packet.u.kein.invite_colossus_map.name), name );
		packet.u.kein.invite_colossus_map.port = GetOwnPort();
	packet.h.header.size = sizeof( k_invite_colossus_map );
	SendPacket2Map( &packet, ST_SEND_MAP_ALL );
}

// 해당 이름이 있는지 확인한다.
void RecvCheckDoubleName( t_packet *p, short int cn )
{
	char *name = p->u.kein.invite_colossus_map.name;
	int port = p->u.kein.invite_colossus_map.port;

	int target_id = ExistHe( name );
	if( target_id > 0 )
	{
		closeconnection( connections, target_id, -105 );		// 일단 다른 놈을 끊고 --> 저장
		// 그 사실을 알려준다.
		t_packet packet;
		packet.h.header.type = CMD_CHECK_DOUBLE_NAME_OK;
			strcpy_s( packet.u.kein.default_name, sizeof(packet.u.kein.default_name), name );
		packet.h.header.size = strlen(name);
		SendPacket2Map( &packet, port );
	}
	else
	{
		target_id = ExistConnectingHe( name );
		if( target_id > 0 )
		{
			connections[target_id].chrlst.updatable = 1; // CSD-TW-030620
			closeconnection( connections, target_id, -106 );		// 일단 다른 놈을 끊고 --> 저장

			// 그 사실을 알려준다.
			t_packet packet;
			packet.h.header.type = CMD_CHECK_DOUBLE_NAME_OK;
				strcpy_s( packet.u.kein.default_name, sizeof(packet.u.kein.default_name), name );
			packet.h.header.size = strlen(name);
			SendPacket2Map( &packet, port );
		}
	}
}

void RecvCheckDoubleNameOk( t_packet *p, short int cn )
{
	char *name = p->u.kein.default_name;
	int target_id = ExistHe(name );
	if( target_id > 0 )
	{	// 다른 맵에 같은 놈이 있다는 소식이 왔기 때문에 저장하지 말고 삭제하자
		connections[target_id].chrlst.updatable = 0; // CSD-TW-030620
		closeconnection( connections, target_id, -102 );
	}
	else
	{
		target_id = ExistConnectingHe( name );

		if( target_id > 0 )
		{
			connections[target_id].chrlst.updatable = 0; // CSD-TW-030620
			closeconnection( connections, target_id, -102 );		// 일단 다른 놈을 끊고 --> 저장
		}
	}
}

void RecvCheckDoubleNameDB2Map( t_packet *p, short int cn )
{
	return;
	int target_id;
	int bClose;
	char *pName;  
	for( int i=0; i<4; i++ )
	{
		pName = p->u.kein.login_check_double_name.name[i];
		if( pName[0] )
		{
			EatRearWhiteChar( pName );
			bClose = 0;
			target_id = ExistHe( pName );
			if( target_id > 0 )
			{
				bClose = 1;
			}
			else
			{
				target_id = ExistConnectingHe( pName );
				if( target_id > 0 )
				{
					bClose = 1;
				}
			}

			if( bClose )
			{
				connections[target_id].chrlst.updatable = 1; // CSD-TW-030620
				closeconnection( connections, target_id, -101 );
			}
		}
	}
}

void RecvCheckDoubleNameWhenFirst( t_packet *p, short int cn )
{
	char *pName = p->u.kein.check_double_when_first.name;
	int server_id = p->u.kein.check_double_when_first.server_id;
	int port = p->u.kein.check_double_when_first.port;

	int bClose = 0;
	int target_id = ExistHe( pName );
	if( target_id > 0 )
	{
		bClose = 1;
	}
	else
	{
		target_id = ExistConnectingHe( pName );
		if( target_id > 0 )
		{
			bClose = 1;
		}
	}

	if( bClose )
	{
		connections[target_id].chrlst.updatable = 1; // CSD-TW-030620
		closeconnection( connections, target_id, -101 );

		t_packet packet;
		packet.h.header.type = CMD_CHECK_DOUBLE_NAME_WHERE_OK;
			packet.u.kein.default_short_int = server_id;
		packet.h.header.size = sizeof( short int );
		SendPacket2Map( &packet, port );
	}
}

void RecvCheckDoubleNameWhenFirstOk( t_packet *p, short int cn )
{
	int server_id = p->u.kein.default_short_int;
	if( CheckServerIndex( server_id ) )
	{
		connections[server_id].chrlst.updatable = 0; // CSD-TW-030620
		closeconnection( connections, server_id, -102 );
	}
}

void CheckDoubleNameWhenFirst( char *pName, short int server_id )
{
	int bClose = 0;
	int target_id = ExistHe( pName );
	if( target_id > 0 )
	{
		bClose = 1;
	}
	else
	{
		target_id = ExistConnectingHe( pName );
		if( target_id > 0 )
		{
			bClose = 1;
		}
	}
	if( bClose )
	{
		connections[target_id].chrlst.updatable = 1; // CSD-TW-030620
		closeconnection( connections, target_id, -101 );

		connections[server_id].chrlst.updatable = 0; // CSD-TW-030620	
		closeconnection( connections, server_id, -102 );
	}

	t_packet packet;
	packet.h.header.type = CMD_CHECK_DOUBLE_NAME_WHERE_FIRST;
		strcpy_s( packet.u.kein.check_double_when_first.name, sizeof(packet.u.kein.check_double_when_first.name), connections[server_id].name );
		packet.u.kein.check_double_when_first.server_id = server_id;
		packet.u.kein.check_double_when_first.port = GetOwnPort();
	packet.h.header.size = sizeof( k_check_double_when_first );
	SendPacket2Map( &packet, ST_SEND_MAP_ALL );
}

// 030923 HK YGI
void SaveLoginLogoutLogByKein(CHARLIST* pTarget, int type, int call_function_id )
{
	if (!g_EventFlagMgr.IsSaveLogWhenSaveUserData())
	{
		return;
	}

	char type2str[2][20] = {"LOGOUT", "LOGIN"};
	
	string strPath;

	if (g_pLogManager->GetLogPath(LT_CHANGE, strPath))
	{
		char temp[FILENAME_MAX] = {0,};
		char log[FILENAME_MAX] = {0,};
		::sprintf_s(temp, sizeof(temp), "%s/savelog_%s.txt", strPath.c_str(), pTarget->Name);
		::sprintf_s(log, sizeof(log), "%s(%d): [%s][%s] lv=%d, exp=(%d)%d, class=%d",
			      type2str[type], call_function_id, MapName,
				  pTarget->Name, pTarget->GetLevel(), pTarget->GetExperienceStep(), pTarget->Exp, pTarget->Class);
		SaveLogDefault2(temp, log);
	}
}

// 031009 CI YGI
int InitItemLimitCountFromDB( )
{
	char query[512];
	WORD port = GetOwnPort();
	if( !port )
	{
		return 0;
	}
	DWORD limit_number;
	int ret = GetItemLimitCount_SQL( port, &limit_number );
	if( ret < 1 )
	{
		limit_number = 1;
		sprintf_s( query, sizeof(query), "INSERT INTO ItemLimitMgrTable ( port, limit_number ) VALUES ( %d, %d )", port, limit_number );
		Querry_SQL( query );
	}

	ItemMgr.SetLimitCount( limit_number );

	ret = ItemMgr.LoadHaveLimitItem();
	if( !ret )		// 실패
	{
		JustMsg( "Error!!! check please ItemList_HaveLimit table" );
		return 0;
	}
	return 1;
}
// 040105 YGI 보물 ----------------
int GetTreasureMining( int lv )
{
	switch( lv )
	{
		case 1: return 30;
		case 2: return 50;
		case 3: return 70;
		case 4: return 99;
	}
	return 101;
}
void RecvCheckTreasureXY( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;
	int event_index = -1;

	k_check_treasure_xy *pData = (k_check_treasure_xy *)p->u.data;
	int tile_x = pData->x/TILE_SIZE;
	int tile_y = pData->y/TILE_SIZE;
	int lv = 0;
	bool mining = false;		// 기술이 부족하다

	for( int a=0; a<3; a++ )
	{
		for( int b=0; b<3; b++ )
		{
			for( int c=0; c<8; c++ )
			{
				ItemAttr *pItem = &ch->inv[a][b][c];
				lv = IsTreasureMapItem( pItem->item_no );
				if( !lv ) continue;		// 보물 지도가 아니다.
				CTreasureMapItem *pAttr = (CTreasureMapItem *)&pItem->attr[IATTR_TREASURE_MAP];
				if( !pAttr->isChecked || pAttr->mapnumber != MapNumber ) continue;
				if( ch->skillexp[MINING].skillexp/10000 < GetTreasureMining( lv ) )
				{
					mining = false;
					continue;
				}
				if( pAttr->x != tile_x || pAttr->y != tile_y ) continue;
				event_index = g_EventFindTreasure.CheckXY( tile_x, tile_y );
				if( event_index == -1 ) continue;
				// 마이너 레벨
				mining = true;
				
				// 사용한것은 다시 사용하지 못하게 삭제 한다.
				//pAttr->use = 1;
				POS pos;
				SetItemPos( INV, a, b, c, &pos );
				SendDeleteItem( pItem, &pos, ch );
				//SendServerEachItem( &pos, pItem, cn );
				goto find_stop;
			}
		}
	}

find_stop : 
	MyLog(LOG_NORMAL, "RecvCheckTreasureXY - FindStop");
	if( event_index == -1 ) return;
	if( lv == 0 ) return;
	if( !mining ) 
	{
		SendPutMenuString( KM_FAIL, 215, cn );
		return;
	}

	if( g_TreasureGuardMgr.IsEvent( event_index ) ) return;	// 이미 등장했었는지 확인
	int treasure_class = lv-1;		// 튀어 나와야 할 몹과 나중에 보물 상자의 등급
	int ret = g_TreasureGuardMgr.CreateGuard( event_index, treasure_class, tile_x, tile_y, cn );
	if( ret )
	{
		MyLog( 0, "Treasure Guard Event Start <event_index:%d>, <treasure_class:%d>, (%d,%d) <%s>",
			event_index, treasure_class, tile_x, tile_y, ch->Name );
	}
} //////////////////////////

void RecvGetTreasureMapDetail( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	POS *pPos = &p->u.kein.default_pos;
	ItemAttr *item = GetItemByPOS( ch, *pPos );
	if( !item ) return;

	int lv = IsTreasureMapItem( item->item_no );
	if( !lv ) return;		// 보물 지도가 아니다.

	CTreasureMapItem *pAttr = (CTreasureMapItem *)&item->attr[IATTR_TREASURE_MAP];
	if( pAttr->isChecked) return;

	t_packet packet;
	packet.h.header.type = CMD_GET_TREASURE_MAP_DETAIL_DB;
		packet.u.kein.get_treasure_map_detail_to_db.cn = cn;
		packet.u.kein.get_treasure_map_detail_to_db.pos = *pPos;
	packet.h.header.size = sizeof( k_get_treasure_map_detail_to_db );
	QueuePacket( connections, DB_DEMON, &packet, 1 );
}

void RecvGetTreasureMapDetailDB( t_packet *p, short int cn )
{
	k_get_treasure_map_detail_to_map *pData = &p->u.kein.get_treasure_map_detail_to_map;
	CHARLIST *ch = CheckServerId( pData->cn );
	if( !ch ) return;

	ItemAttr *item = GetItemByPOS( ch, pData->pos );
	if( !item ) return;
	int lv = IsTreasureMapItem( item->item_no );
	if( !lv ) return;		// 보물 지도가 아니다.
	CTreasureMapItem *pAttr = (CTreasureMapItem *)&item->attr[IATTR_TREASURE_MAP];
	if( pAttr->isChecked ) return;

	pAttr->isChecked = 1;
	pAttr->mapnumber = pData->map_number;
	//pAttr->use = 0;
	// lv에 따라 랜덤하게...
	pAttr->x = pData->x;
	pAttr->y = pData->y;
	switch( lv )
	{
		case 1 : pAttr->offset2 = 0; pAttr->offset = 0; break;
		case 2 : pAttr->offset2 = rand()%4; pAttr->offset = rand()%2; break;
		case 3 : pAttr->offset2 = rand()%4; pAttr->offset = rand()%3; break;
		case 4 : pAttr->offset2 = rand()%4; pAttr->offset = rand()%4; break;
	}

	SendServerEachItem( &pData->pos, item, pData->cn );
}