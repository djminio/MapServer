#include "..\stdafx.h"
#include "..\LowerLayers\servertable.h"
#include "..\LowerLayers\recvmsg.h"
#include "..\LowerLayers\mylog.h"
#include "DefaultHeader.h"
#include "MenuDefine.h"
#include "ItemList.h"
#include "CItem.h"
#include "teambattle.h"
#include "scrp_exe.h"
#include "chatting.h"
#include "menuserver.h"		// 020620 YGI
#include "UserManager.h"
#include "ArenaManager.h"

//////////////////////////////////////////////////////////////////////////
// 콜로세스 경기 승패 저장
void CalcWinLoseScore( WINLOSEPOINT &win_lose, int wintype )
{
	switch( wintype )
	{
		case FST_WIN:		win_lose.win++;		break;	// 승
		case FST_DEFEAT	:	win_lose.defeat++;	break;	// 패
		case FST_DISCONNECT:win_lose.disconnect++;	break;	// disconnect
	}
}

void SendFightMapChatMyTeam( short int cn, t_packet *packet )
{	//< CSD-030509
	CHARLIST* pMember = CheckServerId(cn);

	if (pMember == NULL)
	{
		return;
	}

	CBaseArena* pGame = pMember->GetJoinArenaGame();

	if (pGame == NULL)
	{
		return;
	}

	CArenaTeam* pTeam = pMember->GetJoinArenaTeam();
	
	if (pTeam == NULL) 
	{
		return;
	}
	
	t_packet p;
	t_connection *c = connections;
	int range, type, length;
	
	range	= (int)packet->u.client_chat_data.range;
	type	= (int)packet->u.client_chat_data.type;
	length	= (int)packet->u.client_chat_data.length;
	if(length <= 0 || length >= TEXT_LENGTH) return;
		
	p.h.header.type					= CMD_CHAT_DATA;
	p.u.server_chat_data.server_id	= cn;
	p.u.server_chat_data.type		= CHATTYPE_COLOSSUS;
	p.u.server_chat_data.length		= (unsigned char)length;
		
	memcpy(p.u.server_chat_data.text, packet->u.client_chat_data.text, length);
	p.h.header.size = sizeof(t_server_chat_data)-TEXT_LENGTH+length;
	pTeam->SendPacket(&p);
}	//> CSD-030509

void SendFightMapChat( short int cn, t_packet *packet )		// 전체에게 보낸다.
{	//< CSD-030509
	CHARLIST* pMember = CheckServerId(cn);

	if (pMember == NULL)
	{
		return;
	}

	CBaseArena* pGame = pMember->GetJoinArenaGame();

	if (pGame == NULL)
	{
		return;
	}

	CArenaTeam* pTeam = pMember->GetJoinArenaTeam();
	
	if (pTeam == NULL) 
	{
		return;
	}

	t_packet p;
	t_connection *c = connections;
	int range, type, length;
	
	range	= (int)packet->u.client_chat_data.range;
	type	= (int)packet->u.client_chat_data.type;
	length	= (int)packet->u.client_chat_data.length;
	if(length <= 0 || length >= TEXT_LENGTH) return;
		
	p.h.header.type					= CMD_CHAT_DATA;
	p.u.server_chat_data.server_id	= cn;
	
	p.u.server_chat_data.length		= (unsigned char)length;
		
	memcpy(p.u.server_chat_data.text, packet->u.client_chat_data.text, length);
	p.h.header.size = sizeof(t_server_chat_data)-TEXT_LENGTH+length;

	CBaseArena::LIST_TEAM ltTeam = pGame->GetArenaTeamSet();

	for (CBaseArena::ITOR_TEAM i = ltTeam.begin(); i != ltTeam.end(); ++i)
	{
		if ((*i) == pTeam)
		{
			p.u.server_chat_data.type = CHATTYPE_COLOSSUS;
		}
		else
		{
			p.u.server_chat_data.type = CHATTYPE_COLOSSUS_GLOBAL;
		}

		(*i)->SendPacket(&p);
	}
}	//> CSD-030509

void SendGamblingResult(CHARLIST* pGambler, int nPayment, DWORD dwMoney)
{	//< CSD-030522
	t_packet packet;
	packet.h.header.type = CMD_ARENA_GAMBLING_RESULT;
	packet.h.header.size = sizeof(t_arena_gambling_result);
	packet.u.arena.arena_gambling_result.nPayment = nPayment;
	packet.u.arena.arena_gambling_result.dwMoney = dwMoney;
	::QueuePacket(connections, pGambler->GetServerID(), &packet, 1);
}	//> CSD-030522

// 넌 그 팀에 등록 됐다.
void SendTeamBattleMessageAddCharOk( char team_num, short int cn )
{
	t_packet p;
	p.h.header.type = CMD_FIGHT_MAP_ADD_OK;
		p.u.kein.default_char = team_num;
	p.h.header.size = sizeof( char );
	QueuePacket(connections, cn, &p, 1);
}

inline int GetNationByMapName( char *mapname )
{
	EatRearWhiteChar( mapname );
	_strupr_s(mapname, 20 );
	for( int i=0; i<MAX_MAP_;i++ )
	{
		if( MapInfo[i].mapfile[0] ) 
		{
			if( strcmp( MapInfo[i].mapfile, mapname) ==  0 )
			{
				return MapInfo[i].nation;
			}
		}
	}
	return -1;
}

// recent시 사용하는 실제 db 에 저장하는 mapname과 x, y를 변환 시킬수 있다.
struct k_impossible_map
{
	int map_index;
	POINTS xy[3];
};

// recent 시 xy 값을 바꿔준다.	// 저장할때 바꿔서 저장한다.
void CheckFightMap( char *mapname, short int &x, short int &y, CHARLIST *ch )
{	//< CSD-030509 : recent시 위치값을 변경
	LPMAPINFO pMapInfo = ::GetMapInfoByMapName(mapname);

	if (pMapInfo == NULL)
	{
		return;
	}

	switch (pMapInfo->cPosType)
	{
	case 1:
		{
			x = pMapInfo->nSaveX*TILE_SIZE;
			y = pMapInfo->nSaveY*TILE_SIZE;
			break;
		}
	default:
		{
			CArenaInfo* pArena = g_pArenaManager->GetArenaInfo(pMapInfo->port);

			if (pArena == NULL)
			{
				return;
			}
	
			int nX = 0, nY = 0;
			pArena->GetWaitPosition(nX, nY);
			x = nX*TILE_SIZE;
			y = nY*TILE_SIZE;
			break;
		}
	}
}	//> CSD-030509

////////////////////////////////////////////////////////////
// 겜블 아이템 CGambleItem // << 011019 ygi
int CGambleItem::SetSize( int house, int type, int count )
{
	Release( house, type );
	if(0 >= count){ return 0; }
	m_GambleItem[house][type].lpItem = new int[count];
	m_GambleItem[house][type].count = count;
	return 1;
}

int CGambleItem::Release( int house, int type )
{
	if( !m_GambleItem[house][type].lpItem )  return 1;

	SAFE_DELETE_ARRAY(m_GambleItem[house][type].lpItem);
	m_GambleItem[house][type].count = 0;
	return 1;
}

CGambleItem g_GambleItem;
// >> 011019 ygi
////////////////////////////////////////////////////////////
CTodayGambleItem g_TodayGambleItem;
int CheckMapPort( int port )
{
	if( port == g_MapPort ) return true;
	else return false;
}
int CTodayGambleItem::CreateTodayItem()
{
	int house = 0;
	int room = 0;

	int ran = rand()%100;
	int type = ChoiceParam( g_game_rate_data.create_today_item, 8, ran );
	switch( type )
	{
		case 0 : house = 2; room = 0; break;
		case 1 : house = 2; room = 1; break;
		case 2 : house = 2; room = 2; break;
		case 3 : house = 4; room = 0; break;
		case 4 : house = 4; room = 1; break;
		case 5 : house = 4; room = 2; break;
		case 6 : house = 4; room = 3; break;
		case 7 : house = 4; room = 4; break;
		default: return 0;
	}

	int item_no = g_GambleItem.GetRandItem( house, room );
	if( house == 2 )
	{
		m_TodayItem = MakeGambleItem( house, room, item_no );
	}
	else
	{
		m_TodayItem = MakeGambleItem( house%2+3, rand()%4+1, item_no );
	}
	return 1;
}

// 020428 YGI acer
int RefreshTodayItem()
{
	if( !CheckMapPort( FindMannagementMap( MM_TODAY_GAMBLE_ITEM_MAIN_MAP_PORT ) ) ) return 0;
	if( !g_TodayGambleItem.CreateTodayItem() ) return 0;

	t_packet packet;
	packet.h.header.type = CMD_SET_TODAY_GAMBLE_ITEM;
		packet.u.kein.default_item = g_TodayGambleItem.GetTodayItem();
	packet.h.header.size = sizeof( ItemAttr );
	SendPacket2Maps( &packet );
	return 1;
}

// 020707 YGI
WORD FindMannagementMap( WORD port )
{
	if( GetOwnPort() == port ) return port;
	LP_SERVER_DATA pServerData = g_pServerTable->GetServerData( (WORD)port );
	if(  !pServerData || !pServerData->dwConnectionIndex )
	{
		pServerData = g_pServerTable->GetServerData( (WORD)5190 );
		if( !pServerData || !pServerData->dwConnectionIndex )
		{
			return GetOwnPort();
		}
		return 5190;
	}
	return port;
}



//acer5
// 020620 YGI
int g_GetManagementMapPortFlag;
WORD GetManagementMapPort( WORD port )
{
	if( !g_GetManagementMapPortFlag )
	{
		g_GetManagementMapPortFlag = (int)GetPrivateProfileInt("kein", "GetPortFlag", 0, MAP_SERVER_INI_ );
		if( !g_GetManagementMapPortFlag) g_GetManagementMapPortFlag = 100;
	}

	if(LocalMgr.IsFreeBeta())//030102 lsw
	{
		if( g_GetManagementMapPortFlag != 100 )
		{
			FindMannagementMap( port );
		}
	}
	return port;
}

k_nation_item_data g_nation_item;	// 국가 고급 아이템

extern int IsStartNationWar();		// 이 맵이 국가전 맵이고 현재 전투 중인가?
extern int g_LocalWarBegin;			// 011215 LTS
extern int g_isLocalWarServer;      // LTS 020725
// 길드전 중, 같은 팀인지 검사

// 020620 YGI
bool IsColleagueWhenColossus( CHARLIST *a, CHARLIST *d )
{	//< CSD-031013
	// isPKONOFF
	if( a->pk_on_off == 0 )		// 공격 불가 설정을 해둔 상태에서
	{
		if (d->IsTamedNpc())
		{
			return false; //Eleval 22/05/09 - Re-Enable attack tamed monsters in Battle Mode
		}

		if(LocalMgr.IsAbleNation(KOREA))//030102 lsw
		{
			if( ! // 밑의 상황중 어느것도 아닐때 
				( IsStartNationWar()  // 국가전일때 // 모두 참여자
				|| ( a->fight_flag && a->fight_id == d->GetServerID()) // 결투중인데 대상자를 때렸을 때 
				|| (IsArenaFight(a)) // 길드전시 참여 중일때 
				|| ( g_LocalWarBegin && a->JoinLocalWar&&g_isLocalWarServer ) // 국지전시 참여 중일때     // LTS 020725
				|| ( d->GetNK( a->name_status.nation ) )	// 상대방이 nk 일때
				))
			{ 
				// 메시지 출력
				CallClient( a->GetServerID(), CMD_FAIL_ATTACK_BY_PK_MODE );
				return true;		// 공격이 불가능 하다
			}
		}
		else
		{
			CallClient( a->GetServerID(), CMD_FAIL_ATTACK_BY_PK_MODE );
			return true;		// 공격이 불가능 하다
		}
	}
	else if(d->IsTamedNpc() && d->GetMaster() == a->GetServerID()) //Eleval 22/05/09 - then the master won't be able to attack his tamed creatures in PKMode
	{
		return true;
	}

	return false;
}	//> CSD-031013

//////////////////////////////////////////////////////////////////////////
// CColossusStone
CColossusStoneMgr g_ColossusStone;
int CColossusStoneMgr::AddStone( int id )
{
	if( id == -1 ) return 0;
	CColossusStone *temp = &m_StoneList;
	for( ; temp->next; temp=temp->next );
	CColossusStone *data = new CColossusStone;
	data->stone = &ItemList[id];
	temp->next = data;
	return 1;
}
ITEMLIST *CColossusStoneMgr::GetList( int count )
{
	CColossusStone *target = m_StoneList.next;
	for( ;target; target = target->next )
	{
		if( target->stone->dumno == count )
		{
			return target->stone;
		}
	}
	return NULL;
}
void CColossusStoneMgr::GetTeamCount( int team[4] )
{
	memset( team, 0, sizeof( int )*4 );
	CColossusStone *target = m_StoneList.next;
	for( ;target; target = target->next )
	{
		DWORD attr = target->stone->attr[IATTR_ATTR];
		int t= GetTeamByStone( attr );
		if( t>= 0 )
		{
			team[t]++;
		}
	}
}
void CColossusStoneMgr::SetStoneByTeam( int id, int team )		// 어떤 팀이 스톤을 건드렸을때
{
	ITEMLIST *target = GetList( id );
	if( target )
	{
		::SetStoneByTeam( target, team );
	}
}

void CColossusStoneMgr::SetStoneFirst()		// 초기 세팅
{
	DWORD attr = 0;
	SetAttr2( attr, IA2_COLOSSUS_STONE | STONE_SCORE_FIRST );

	CColossusStone *target = m_StoneList.next;
	for( ;target; target = target->next )
	{
		target->stone->attr[IATTR_ATTR] = attr;
	}
	SetWinCondition();
}

int CColossusStoneMgr::GetMaxStone()
{
	if( !m_MaxStone )
	{
		CColossusStone *target = m_StoneList.next;
		for( ;target; target = target->next )
		{
			m_MaxStone++;
		}
	}
	return m_MaxStone;	
}



void SetStoneByTeam( ITEMLIST *item, int team )		// 어떤 팀이 스톤을 건드렸을때
{
	DWORD attr = 0;
	SetAttr2( attr, IA2_COLOSSUS_STONE );		// or 연산 
	switch( team )
	{
		case 0 : break;
		case 1 : 
			SetAttr2( attr, STONE_SCORE_1 );
			break;
		case 2 : 
			SetAttr2( attr, STONE_SCORE_2 );
			break;
		case 3 : 
			SetAttr2( attr, STONE_SCORE_2 );
			SetAttr2( attr, STONE_SCORE_1 );
			break;
	}
	item->attr[IATTR_ATTR] = attr;
}

int GetTeamByStone( DWORD attr )
{
	if( GetAttr2( attr, STONE_SCORE_FIRST )  ) return -1;		// 아직 팀이 없다.
	return ( GetAttr2( attr, STONE_SCORE_2 )?2:0 + GetAttr2( attr, STONE_SCORE_1 )?1:0 );
}
//
//////////////////////////////////////////////////////////////////////////

// if( !ch->curr_fight_map ) 라는 조건문을 미리 체크 해두고 호출해야 한다.
int GetFightTeamNumber( CHARLIST *ch )
{
	return ( ch->curr_fight_map - 100 );
}

/////////////////////////////////////////////////////////////////////////
// CGuildHouse member functions
CGuildHouse	g_GuildHouse;

CGuildHouseUnit *CGuildHouse::GetPointByEventNo(char *map_name, int event_no)
{	// 해당 맵과 이벤트 번호로 엡포리아 클레스에 접근
	for (int i = 0; i < m_Max; ++i)
	{
		// 대문자이어야 한다.
		if (strcmp( m_aGuildHouse[i].map_name, map_name) == 0)
		{
			if (m_aGuildHouse[i].event_no == event_no)
			{
				return &m_aGuildHouse[i];
			}
		}
	}

	return NULL;
}

CGuildHouseUnit *CGuildHouse::GetPointByGuildCode(int guild_code)
{	// 길드 코드로 엠포리아 클레스에 접근
	for (int i = 0; i< m_Max; ++i)
	{
		if (m_aGuildHouse[i].guild_code == guild_code)
		{
			return &m_aGuildHouse[i];
		}
	}

	return NULL;
}

// 020620 YGI
int CGuildHouseUnit::GoToHouse(short int cn)
{
	return GotoUser(map_name, x, y, cn);
}

int CGuildHouse::GoToHouse( CHARLIST *ch ) // 해당 엠포리아로 이동
{	//< CSD-030324
	int guild_code = ch->GetGuildCode();
	if( !guild_code ) return 0;

	CGuildHouseUnit *house = GetPointByGuildCode( guild_code );
	if( !house ) return 0;

	return house->GoToHouse( ch->GetServerID());
}	//> CSD-030324

int CGuildHouse::IsMyGuildHouse(CHARLIST *ch, int event_no)
{	//< CSD-030806 : 해당 엠포리아로 이동
	CGuildHouseUnit* house = GetPointByEventNo(MapName, event_no);
	
	if (house == NULL) 
	{
		return 0;
	}

	if (house->guild_code == 0)
	{
		return 0;
	}

	if (house->guild_code != ch->GetGuildCode())
	{	// 팅겨 버리게 한다.
		house->GoToHouse(ch->GetServerID());
		return 0;
	}
	
	return 1;
}	//> CSD-030806

int CGuildHouse::clear()
{
	for( int i=0; i<MAX_GUILD_HOUSE; i++ )
	{
		m_aGuildHouse[i].clear();
	}

	m_Max = 0;
	return 1;
}

CGuildHouseUnit	*CGuildHouse::GetHouseByGuildCode( int guild_code )
{
	for( int i=0; i<m_Max; i++ )
	{
		if( m_aGuildHouse[i].guild_code == guild_code ) return &m_aGuildHouse[i];
	}

	return NULL;
}

// 020620 YGI
void GetTimeForFilename( char *filename1 )
{
	time_t nowTime;
	struct tm *now;
	time( &nowTime );
	now = localtime(&nowTime);

	int year = now->tm_year + 1900;
	int mon  = now->tm_mon + 1;
	int day  = now->tm_mday;
	sprintf_s(filename1, sizeof(filename1), "%s_%d-%02d-%02d.log", filename1, year, mon, day);
}

////////////////////////////////////////////////////////////////////
// man to man fight

void EndManToManFight( CHARLIST *win, CHARLIST *lose )
{
	Fight_Stone_End( lose->GetServerID(), 1 );		// 정상적으로 졌다.
}

extern void SendMagicCommand(CHARLIST* lpChar, short int slot1 = 0, short int slot2 = 0, short int slot3 = 0);

void Fight_Stone_End(int cn, int flag)
{
	LPCHARLIST lpChar, lpTarget;

	lpChar = &connections[cn].chrlst;
	lpTarget = &connections[lpChar->fight_id].chrlst;

	if(lpChar)
	{
		switch(flag)
		{
		case 1:	// 진경우
			{
				if(lpChar->fight_id)
				{
					SendMagicCommand( lpChar, 5, lpChar->fight_id, 2);
				}

				break;
			}
		case 2:	// 이긴경우
			{
				if(lpChar->fight_id)
				{
					SendMagicCommand( lpChar, 5, lpChar->fight_id, 1);
				}
				
				break;
			}
		case 3:	// 비긴경우
			{
				if(lpChar->fight_id)
				{
					SendMagicCommand(  lpChar, 5, lpChar->fight_id, 3);
				}

				break;
			}
		case 4:
			{
				SendMagicCommand(  lpChar, 5, 0, 4);
				break;
			}
		}

		ResultManToMan(cn, lpChar->fight_id, flag);	// 0910_2 YGI
		lpChar->fight_flag = 0;
		lpChar->fight_id = 0;
	}
	
	if(lpTarget)
	{
		switch(flag)
		{
		case 1:
			{
				if(lpChar->GetServerID())
				{
					SendMagicCommand(lpTarget, 5, lpChar->GetServerID(), 1);
				}

				break;
			}
		case 2:
			{
				if(lpChar->GetServerID())
				{
					SendMagicCommand(lpTarget, 5, lpChar->GetServerID(), 2);
				}
			
				break;
			}
		case 3:
			{
				if(lpChar->GetServerID())
				{
					SendMagicCommand(lpTarget, 5, lpChar->GetServerID(), 3);
				}

				break;
			}
		case 4:
			{
				break;
			}
		}

		lpTarget->fight_flag = 0;
		lpTarget->fight_id = 0;
	}
}

bool IsManToManFight( CHARLIST *attack, CHARLIST *defence )
{
	if( !attack || !defence ) return false;
	if( attack->IsNpc() || defence->IsNpc() ) return false;
	if( !attack->fight_flag || !defence->fight_flag ) return false;
	if( attack->fight_id == defence->GetServerID()) return true;
	return false;
}

bool IsArenaFight(CHARLIST* pMember)
{	//< CSD-030509
	CBaseArena* pGame = pMember->GetJoinArenaGame();

	if (pGame == NULL)
	{
		return false;
	}

	if (!pGame->IsPlayGame())
	{
		return false;
	}

	CArenaTeam* pTeam = pMember->GetJoinArenaTeam();

	if (pTeam == NULL)
	{
		return false;
	}

	return true;
}	//> CSD-030509