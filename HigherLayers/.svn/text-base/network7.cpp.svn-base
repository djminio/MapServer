/*
// 021008 추가하는 함수들
//extern 은 모두 network7.h에 들어있다. 
*/

#include "..\stdafx.h"
//#include "MAIN.H"
//#include "servertable.h"
#include "QuestFunction.h"
#include "Dr_network.h"

extern CAliveMapZoneAll g_AliveMap;		//m_Map테이블을 사용하기 위해

void RecvWhatQuestInMap( t_packet *p, int cn);			//맵번호가 오면 관련 퀘스트에 따른 스텝값을 참조하여 정보번호를 보낸다.
void CheckSpellMapMoveArea( char *szMsg, int cn);		//그 지역이 주문으로 맵이동 하는 지역이면 문자열을 g_QuestInMap.m_szClientSpell에 넣는다.


void RecvWhatQuestInMap( t_packet *p, int cn)
{

	t_quest_in_map *tp = &(p->u.quest_in_map);

	CHARLIST *ch = CheckServerId( cn );
	if( !ch) return;

	//이름확인
	if( 0 != strcmp( ch->Name, p->u.quest_in_map.name ) ) return;

	g_QuestInMap.SendQuestInfo( tp, cn );

}

void CheckSpellMapMoveArea( char *szMsg, int cn)
{//성공 1, 실패 0
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	if( !szMsg ) return;

	if( ch->viewtype == VIEWTYPE_GHOST_ ) return; //유령타입이면 안된다. 

	for( int i =0;i < g_AliveMap.m_Max; i++)
	{
		if( g_AliveMap.m_Map[i].map_number != MapNumber ) continue;
		//if( ! TileMap[ch->MoveSx][ch->MoveSy].attr_inside ) // 내부 속성이 있는 곳이 아니라면
		//	continue;

		if( BoxAndDotCrash( g_AliveMap.m_Map[i].x,g_AliveMap.m_Map[i].y, g_AliveMap.m_Map[i].xl, g_AliveMap.m_Map[i].yl, ch->MoveSx, ch->MoveSy	) )
		{	//그 상자안에 있을때
			if( g_AliveMap.m_Map[i].alive_word_type == SPELL_MAP_MOVE_TYPE ) //10번 속성일때..
			{
				//기존 cn값이 이쓰면 지우고 새로 쓴다. 
				g_QuestInMap.DeleteSpellUser( cn );	

				CSpellMove cSpell(cn, szMsg);				
				g_QuestInMap.m_szClientSpell.push_back( cSpell );
				break;
			}
		}
	}
	return;
}

int HandleCommand3( t_packet *packet, t_connection c[], const int cn )
{
	switch( packet->h.header.type)
	{
	case CMD_WHAT_QUEST_IN_MAP:		// 021008 kyo 퀘스트 지도명 쳤을때
		{
			RecvWhatQuestInMap( packet, cn);			
		}break;
	case CMD_SCRIPT_TIMER_START:	// 021022 kyo 스크립트 타이머에 관련된것
	case CMD_SCRIPT_TIMER_SYNC:
	case CMD_SCRIPT_TIMER_END_SUC:
	case CMD_SCRIPT_TIMER_END_FAL:
		{
			g_QuestInMap.RevcScriptTimerPacket( (&packet->u.script_timer), packet->h.header.type, cn);
		}break;
	case CMD_SCRIPT_COUNTER_SYNC:	// 030108
		{
			g_QuestInMap.RevcScriptCounterSync( (&packet->u.script_counter),  cn);
		}break;

	case CMD_SCRIPT_SPELL_MAPMOVE:
		{
			g_QuestInMap.RecvSpellMapMove( &packet->u.spell_mapmove, cn );
		}break;
	default:	
		{
			return 0;
		}break;
	}
	return 1;
}