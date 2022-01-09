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
#include "dungeon.h"
#include "eventmgr.h"
#include "ItemMgr.h"



CEventMgr	g_EventMgr;

// 030624 YGI
CEventFlagMgr g_EventFlagMgr;
EventExp g_EventExp;

//////////////////////////////////////////////////////////////////////////////////
// CEventTime
int CEventTime::SetTime( TIMESTAMP_STRUCT *pStart, TIMESTAMP_STRUCT *pEnd )
{
	Remove();
	CTime temp_start( pStart->year, pStart->month, pStart->day,
		pStart->hour, pStart->minute, pStart->second );

	CTime temp_end( pEnd->year, pEnd->month, pEnd->day,
		pEnd->hour, pEnd->minute, pEnd->second );

	m_timeStart = temp_start;
	m_timeEnd = temp_end;

	//MyLog( 0, "%s", m_pTimeStart->Format( "%Y/%m/%d %H:%M:%S" ) );
	//MyLog( 0, "%s", m_pTimeEnd->Format( "%Y/%m/%d %H:%M:%S" ) );

	return 1;
	return CheckActive();
}

int CEventTime::CheckActive()
{
	m_nEventActive = 0;

	CTime curr_time;
	curr_time = CTime::GetCurrentTime();

	if( m_timeStart > curr_time || m_timeEnd < curr_time ) return 0;

	m_nEventActive = 1;
	return 1;
}

/////////////////////////////////////////////////////////////////////////////////////
// CEventNpcItem
int CEventNpcItem::DropNpcItem( CHARLIST *pNpc )
{
	if( !m_nItemNo ) return 0;
	if( m_nAmount >= m_nMax ) return 0;		// 더이상 아이템이 나오면 안된다.
	if( !m_nPercent ) return 0;
	if( m_nPercent != -1 )
	{
		if( rand()%m_nPercent != m_nPercent/2 ) return 0;
	}
	// 아이템을 만들자..
	ItemAttr item;
	if( ItemMgr.MakeDefaultRareItem( &item, m_nItemNo, m_nItemMutant, m_nItemGrade ) )
	{
		// 성공, 저장및 떨궈준다.
		DropItem( pNpc->X + (rand()%100)-50, pNpc->Y +(rand()%100)-50, &item);
		m_nAmount++;
		
		// 디비데몬으로 사실을 보내준다.
		SendSaveAmountEventNpcItem( m_nEventNo, m_nAmount );
		return 1;
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////
// CEventNpcCreate
bool CEventNpcCreate::IsCreateAble()
{
	if( !IsEventActive() ) return false;
	if( IsAlive() ) return false;
	if(	!m_nFirst && !m_nRegen ) return false;

	int after_dead = g_curr_time - m_nDeadTime;
	if( after_dead < m_nRegenTime ) return false;
	
	m_nFirst = 0;	// 이젠 처음이 아니다.

	return true;
}

int CEventNpcCreate::CreateNpc()
{
	if( !IsCreateAble() ) return 0;

	const int nId = ::GetDeleteAbleNPC();
	if(0 <= nId)
	{//일반 몹이 있다면
		::DeleteNpc( nId );
	}
	else
	{//지울 수 있는 몬스터가 없다면
		::MyLog(0,"Critical Error Can't Get Normal Monster(CEventNpcCreate::CreateNpc())");
		return 0;
	}

	::NPC_Create( nId, m_nNpcNo, m_nMapX, m_nMapY, m_nNpcScriptNo, 0, GT_EVENT_NPC );
	m_bAlive = true;
	m_nNpcIndex = nId;

	return 1;
}

int CEventNpcCreate::CheckKillMonster( CHARLIST *pNpc )
{
	if( !IsAlive() ) return 0;
	if (m_nNpcIndex != pNpc->GetServerID()) return 0;
	//if( pNpc->generationtype != GT_EVENT_NPC ) return 0;
	m_bAlive = false;
	m_nDeadTime = g_curr_time;
	m_nNpcIndex = 0;
	return 1;
}

// 이벤트가 끝났는데 남아 있는 npc는 없엔다.
int CEventNpcCreate::CheckClearCreatedNpc()
{
	if( IsEventActive() ) return 0;	// 이벤트가 활성화 되어 있으면 안됨
	if( !IsAlive() ) return 0;		// 살아 있는 놈이 있으면...
	if( m_nNpcIndex && NPCList[m_nNpcIndex].generationtype == GT_EVENT_NPC )
	{
		DeleteEventNpc();
	}
	return 1;
}

int CEventNpcCreate::DeleteEventNpc()
{
	// 걍 죽인다. 아이템은 떨구지 않는다.
	if( !m_nNpcIndex ) return 0;
	::DeleteNpc( m_nNpcIndex );

	m_bAlive = false;
	m_nDeadTime = 0;
	m_nNpcIndex = 0;
	return 1;
}
/////////////////////////////////////////////////////////////////////////////////////
// CEventObject
extern int ReloadTOI( char *filename1, char *filename2 );
void CEventObject::RunEvent()
{
	::ReloadTOI( m_szObjectTOI, m_szObjectB );
}

/////////////////////////////////////////////////////////////////////////////////////
// CEventMessage
extern void BroadCastBBS_Sub( char *msg, int len );
int CEventMessage::CheckEvent()
{
	if( !IsEventActive() ) return 0;
	if( !m_nTime ) return 0;
	if( !m_lpszMessage ) return 0;
	int gab = g_curr_time - m_nLastTime;
	if( gab < m_nTime ) return 0;
	if( !m_bAll )
	{
		switch( m_nNation )
		{
			case 1 :	// 바이서스
				if( MapInfo[MapNumber].nation != N_VYSEUS ) 
				{
					m_nTime = 0;		// 비교를 간단하게 끝내기 위해
					return 0;
				}
				break;
			case 2 :	// 자이펀
				if( MapInfo[MapNumber].nation != N_ZYPERN ) 
				{
					m_nTime = 0;
					return 0;
				}
				break;
			case 3 :	// 일스
				if( MapInfo[MapNumber].nation != N_YLLSE ) 
				{
					m_nTime = 0;
					return 0;
				}
				break;
		}
	}
	BroadCastBBS_Sub( m_lpszMessage, m_nMessageSize );
	m_nLastTime = g_curr_time;
	return 1;
}

/////////////////////////////////////////////////////////////////////////////////////
// CEventNpcScript
int CEventNpcScript::CheckEvent( int npc_index, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return 0;

	if( !m_nScriptNo ) return 0;
	if( NPCList[npc_index].eventno != m_nScriptNo ) return 0;
	if( !m_nEventNpcItemBag ) return 0;

	if( m_nNumber )
	{
		// 디비 데몬으로 보냈다가 확인후 처리
		//
		k_event_script_item	data;
		data.event_no = m_nEventNo;
		data.server_id = cn;
		data.npc_index = npc_index;
		strcpy_s( data.name, sizeof(data.name), ch->Name );
		strcpy_s( data.id, sizeof(data.id), connections[cn].id );
		data.bag_num = m_nEventNpcItemBag;
		data.script_no = m_nScriptNo;
		data.result = 0;
		int size = sizeof( k_event_script_item );
		::SendPacketDefault( CMD_CHECK_EVENT_SCRIPT_ITEM, &data, size, DB_DEMON );		// 디비 데몬으로 보내 보자

		return 1;
	}
	else
	{
		// 아이템을 만들어 주고 메세지를 보낸다.
		::SendEventNpcScriptItem( cn, npc_index, m_nEventNpcItemBag, m_lpszMessage, m_nMessageSize );
	}
	return 1;
}

int SendEventNpcScriptItem( short int cn, int npc_index, int bag_num, char *message, int size )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return 0;
	CEventScriptItemBag *pItemBag = g_EventMgr.GetBagPoint( bag_num );
	if( !pItemBag ) return 0;
	int item_no = pItemBag->GetItemNumber();
	ItemAttr item = GenerateItem( item_no );
	if( !item.item_no ) return 0;

	int a, b, c;
	int ret = SearchInv( ch->inv, a, b, c );
	if( !ret ) return 0;		// 빈자리가 없다.

	// 아이템을 넣어 준다.
	POS pos;
	SetItemPos( INV, a, b, c, &pos );
	ch->inv[a][b][c] = item;
	SendServerEachItem( &pos, &item, ch->GetServerID());

	if( message && size )		// npc의 대화를 보내준다.
		SendDial( cn, npc_index, message, size );

	return 1;
}

/////////////////////////////////////////////////////////////////////////////////////
// CEventMgr
int CEventMgr::CheckActiveEvent( int type )
{
	if( type & ET_EVENT_NPC_ITEM )
	{
		m_eventNpcItem.CheckActiveEvent();
	}
	if( type & ET_EVENT_NPC_CREATE )
	{
		m_eventNpcCreate.CheckActiveEvent();
	}
	if( type & ET_EVENT_OBJECT )
	{
		m_eventObject.CheckActiveEvent();
	}
	if( type & ET_EVENT_SOUND )
	{
		m_eventSound.CheckActiveEvent();
	}
	if( type & ET_EVENT_MESSAGE )
	{
		m_eventMessage.CheckActiveEvent();
	}


	return 1;
}

int CEventMgr::CheckNpcItem( CHARLIST *pNpc )
{
	m_eventNpcItem.m_listActive.SetFind();
	while(  m_eventNpcItem.m_listActive.FindNext() )
	{
		int index = m_eventNpcItem.m_listActive.GetFind();
		if( m_eventNpcItem.GetNpcNo(index) == pNpc->npc_index )
		{
			// 아이템 관련
			m_eventNpcItem.m_pEvent[index].DropNpcItem( pNpc );
		}
	}

	// 액티브 되어 있는 이벤트를 찾아서 해당 npc가 죽었음을 세팅한다.
	if( pNpc->generationtype == GT_EVENT_NPC )
	{
		m_eventNpcCreate.m_listActive.SetFind();
		while( m_eventNpcCreate.m_listActive.FindNext() )
		{
			int index = m_eventNpcCreate.m_listActive.GetFind();
			m_eventNpcCreate.GetEventPoint( index )->CheckKillMonster( pNpc );
		}
	}

	return 1;
}

int CEventMgr::IsActive( int type )
{
	if( type & ET_EVENT_NPC_ITEM )
	{
		return m_eventNpcItem.IsActive();
	}
	if( type & ET_EVENT_NPC_CREATE )
	{
		return m_eventNpcCreate.IsActive();
	}
	if( type & ET_EVENT_OBJECT )
	{
		return m_eventObject.IsActive();
	}
	if( type & ET_EVENT_SOUND )
	{
		return m_eventSound.IsActive();
	}
	if( type & ET_EVENT_MESSAGE )
	{
		return m_eventMessage.IsActive();
	}



	return 0;
}

// 이벤트가 끝난 경우 처리
int CEventMgr::CheckCloseEvent( int type )
{
	if( type & ET_EVENT_NPC_CREATE )
	{
		for( int i=0; i<m_eventNpcCreate.m_nEventCount; i++ )
		{
			m_eventNpcCreate.GetEventPoint( i )->CheckClearCreatedNpc();
		}
	}
	return 1;
}

void *CEventMgr::GetActiveEvent( int type )
{
	if( type & ET_EVENT_NPC_ITEM )
	{
		return m_eventNpcItem.GetActiveEvent();
	}
	if( type & ET_EVENT_NPC_CREATE )
	{
		return m_eventNpcCreate.GetActiveEvent();
	}
	if( type & ET_EVENT_OBJECT )
	{
		return m_eventObject.GetActiveEvent();
	}
	if( type & ET_EVENT_SOUND )
	{
		return m_eventSound.GetActiveEvent();
	}
	if( type & ET_EVENT_MESSAGE )
	{
		return m_eventMessage.GetActiveEvent();
	}

	return 0;
}

int CEventMgr::Proc()
{
	// 몬스터 생성확인
	m_eventNpcCreate.m_listActive.SetFind();
	while( m_eventNpcCreate.m_listActive.FindNext() )
	{
		int index = m_eventNpcCreate.m_listActive.GetFind();
		m_eventNpcCreate.GetEventPoint( index )->CreateNpc();
	}

	// 이벤트 메시지 확인
	m_eventMessage.m_listActive.SetFind();
	while( m_eventMessage.m_listActive.FindNext() )
	{
		int index = m_eventMessage.m_listActive.GetFind();
		m_eventMessage.GetEventPoint( index )->CheckEvent();
	}

	// 021128 YGI
	static DWORD dwCheckTime = g_curr_time;
	if( g_curr_time - dwCheckTime > 600 )	// 10분마다 체크한다.
	{
		CheckActiveEvent( ET_EVENT_NPC_CREATE );
		CheckCloseEvent( ET_EVENT_NPC_CREATE );
		dwCheckTime = g_curr_time;
	}

	return 1;
}

// 해당 이벤트를 수행했으면 1을 리턴한다.
int CEventMgr::CheckScriptNo( int npc_index, short int cn )
{
	if( !NPCList[npc_index].eventno ) return 0;
	int ret = 0;

	m_eventNpcScript.m_listActive.SetFind();
	while( m_eventNpcScript.m_listActive.FindNext() )
	{
		int index = m_eventNpcScript.m_listActive.GetFind();
		ret += m_eventNpcScript.GetEventPoint( index )->CheckEvent( npc_index, cn );
	}

	return ret;
}

CEventNpcScript *CEventMgr::GetEventNpcScript( int script_no )
{
	for( int i=0; i<m_eventNpcScript.m_nEventCount; i++ )
	{
		if( m_eventNpcScript.m_pEvent[i].m_nScriptNo == script_no ) 
			return &m_eventNpcScript.m_pEvent[i];
	}
	return NULL;
}
CEventNpcScript *CEventMgr::GetEventNpcScriptByEventNo( int event_no )
{
	for( int i=0; i<m_eventNpcScript.m_nEventCount; i++ )
	{
		if( m_eventNpcScript.m_pEvent[i].m_nEventNo == event_no ) 
			return &m_eventNpcScript.m_pEvent[i];
	}
	return NULL;
}


CEventMoveMap *CEventMgr::GetEventMoveMapPoint( int event_no )
{
	for( int i=0; i<m_eventMoveMap.m_nEventCount; i++ )
	{
		if( m_eventMoveMap.m_pEvent[i].m_nEventNo == event_no ) 
			return &m_eventMoveMap.m_pEvent[i];
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////

void CheckEventWhenKillMonster( CHARLIST *pKiller, CHARLIST *pNpc )
{
	if( pNpc->generationtype == GT_SCENARIO_MONSTER )
	{
		KilledScenarioBoss( pKiller, pNpc );
	}
	g_EventMgr.CheckNpcItem( pNpc );
	// 040105 YGI 보물
	if( pNpc->generationtype == GT_TREASUER_GUARD )
	{
		g_TreasureGuardMgr.KilledTreasureGuard( pKiller, pNpc );
	}
}

void SendSaveAmountEventNpcItem( int event_no, int amount )
{
	t_packet packet;

	packet.h.header.type = CMD_SAVE_AMOUNT_EVENT_NPC_ITEM;
	int *data = (int *)packet.u.data;

	data[0] = event_no;
	data[1] = amount;

	packet.h.header.size = sizeof( int ) *2;
	QueuePacket( connections, DB_DEMON, &packet, 1 );
}



///////////////////////////////////////////////////////////////////////////////////
// light_version
CLightVersion g_LightVersion;
int CLightVersion::LoadLightVersionMap()
{
	char filename[300]; //Eleval 21/08/09 - Put 300 to prevent any buffer error
	sprintf_s( filename, sizeof(filename), "%s/data/light_map.txt", GameServerDataPath );
	
	FILE *fp;
	fp = fopen( filename, "rt" );
	if( !fp ) return 0;

	Clear();		// 변수 클리어

	char temp[255];
	while( fgets( temp, 255, fp ) )
	{
		if( CheckContinueLine( temp[0] ) ) continue;
		m_nCount++;
	}

	fseek( fp, 0, SEEK_SET );
	m_szlpMap = new string[m_nCount];

	int count = 0;
	while( fgets( temp, 255, fp ) )
	{
		if( CheckContinueLine( temp[0] ) ) continue;
		_strupr_s( temp, sizeof(temp) );
		m_szlpMap[count++] = temp;
	}

	fclose( fp );
	return 1;
}

bool CLightVersion::IsAbleMapMove( CHARLIST *ch, const char *mapname )		
{
	if( !IsLightVersion( ch ) ) return true;
	for( int i=0; i<m_nCount; i++ )
	{
		if( _stricmp( mapname, m_szlpMap[i].c_str() ) == 0 ) return true;
	}
	return false;
}

bool CLightVersion::IsLightVersion( CHARLIST *ch )
{
	return (ch->installType)?true:false;
}


bool CheckContinueLine( char check )
{
	if( check == 0 || check == ' ' || check == ';' || check == '#' || check == '\n' ) return true;
	return false;
}
/////////////////////////////////////////////////////////////////////


// 030627 YGI
bool InitEventFlag()
{
	int nRow = 0;
	char query[256];
	sprintf_s( query, sizeof(query), "mapname = '%s'", MapName );
	GetRowLineOfSQL( "Event_MoveWhenDie", "*", &nRow, query );
	g_EventFlagMgr.SetMoveMapWhenDie( (nRow<=0)?0:1 );

	g_EventFlagMgr.InitSaveLogFlag();	// 030919 HK YGI
	// 040105 YGI 보물
	// 보물 찾기 이벤트용
	LoadEventTreasureXY();
	LoadEventTreasureGuard();
	return true;
}

// 030919 HK YGI
/////////////////////////////////////////////////////////////////////
//CEventFlagMgr member functions
void CEventFlagMgr ::InitSaveLogFlag()
{
	m_bSaveLogAboutSaveUserData = (int)GetPrivateProfileInt("option", "savelogflag_when_save_user_data" ,0,MapServerConfigFileName);
}

/////////////////////////////////////////////////////////////
// 040105 YGI 보물
int CTreasureGuardMgr::CreateGuard( int index, int treasure_class, int x, int y, int cn )
{// 보물 지킴이 출동
	if( treasure_class<0 || treasure_class>=5 ) return 0;
	int count = 0;		// 총 출현 개수
	for( int i=0; i<g_TreasureGuardTBL[treasure_class].m_nCount; i++ )
	{
		int spr_no = g_TreasureGuardTBL[treasure_class].m_pNpcNo[i];
		int max = g_TreasureGuardTBL[treasure_class].m_pNpcCount[i];
		if( !spr_no || !max ) continue;
		for( int j=0; j<max; j++ )
		{
			const int id = ::GetDeleteAbleNPC();//030211 lsw
			if( id >= 0 )
			{
				::DeleteNpc( id );
			}
			else
			{//지울 수 있는 몬스터가 없다면
				::MyLog(0,"Critical Error Can't Get Normal Monster(CTreasureGuardMgr::CreateGuard())");
				break;
			}
			int xx = x+(5-rand()%11);
			int yy = y+(5-rand()%11);
			NPC_Create( id, spr_no, xx, yy, 0, index, GT_TREASUER_GUARD);

			count++;
		}
	}
	if( count <= 0 ) return 0;		// 생성 실패
	POINTS data;
	data.x = treasure_class+1;	// 보물 상자에 넣어줄값. 1부터 시작
	data.y = count;
	AddNew( index, data );
	return 1;						// 생성 성공
}
/////////////////////////////////////////////////////////////
CEventFindTreasure g_EventFindTreasure;
CTreasureGuardTBL g_TreasureGuardTBL[5];
CTreasureGuardMgr g_TreasureGuardMgr;
#include "LogManager.h"
int CTreasureGuardMgr::KilledTreasureGuard( CHARLIST *user, CHARLIST *boss )
{
	int index = boss->generationpos;		// 튀어 나온 그룹 번호
	ITOR_MAP_GUARD itor = m_Data.find(index);
	if (itor == m_Data.end()) return 0;

	itor->second.y--;
	if( itor->second.y == 0 )		// 모두 다 죽였다.
	{
		// 보물 발견
		ItemAttr item = ::GenerateItem( TREASURE_BOX );
		item.attr[IATTR_TREASURE_MAP] = itor->second.x;
		::DropItem( boss->X, boss->Y, &item );
		string strPath;
		char temp[256];
		sprintf_s( temp, sizeof(temp), "<%02d:%02d:%02d>", g_mon+1, g_day,g_hour, g_min, g_sec );
		sprintf_s( temp, sizeof(temp), "%s drop treasure box ( %s(%d, %d) : %d(%d) )", temp, MapName, boss->X/TILE_SIZE, boss->Y/TILE_SIZE, item.item_no, item.attr[IATTR_LIMIT] );
		if( g_pLogManager->GetLogPath(strPath) )
		{
			char temp_path[80];			
			sprintf_s( temp_path, sizeof(temp_path), "%s/Event/event_treasure_box.txt", strPath.c_str());
			::SaveLogDefault( temp_path, temp, 0 );
		}
		else
		{
			MyLog( 0, "SaveTreasureLogFile fail");
		}
		MyLog( 0, temp );
		m_Data.erase(itor);	// 리스트(맵)에서 삭제
	}	
	return 1;
}

// 보물 지도 아이템인가?
int IsTreasureMapItem( int item_no )
{
	int level = 0;
	switch( item_no )
	{
		case LOW_LEVEL_MAP : level = 1; break;
		case MIDDLE_LEVEL_MAP : level = 2; break;
		case HIGH_LEVEL_MAP : level = 3; break;
		case UNKNOWN_MAP : level = 4; break;
	}
	return level;
}

CTreasureMapItem GetTreasureAttr( ItemAttr *item )
{
	CTreasureMapItem attr;
	memcpy( &attr, &item->attr[IATTR_TREASURE_MAP], sizeof( DWORD ) );
	return attr;
}