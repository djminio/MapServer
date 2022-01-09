
#include "../stdafx.h"
//#include "MySql.h"
#include "Dr_network.h"
#include "QuestFunction.h"
#include "Scrp_exe.h"
#include <assert.h>
#include "RegenManager.h" // 031028 kyo 
#include "SealStone.h"		// BBD 040318
#include "CItem.h"			// BBD 040329

CQuestInMap g_QuestInMap;
extern HDBC hDBC;
extern int  GetAliveNPCList(void);		//npc_list.h
extern void KillMonster(WORD wNumber); //op_magic.h
extern void CastMe2Other( int cn, t_packet *packet );	//Area.cpp
extern int SetRareItemInvPc( const int cn, const int iItemNo, const int iGradeMin, const int iGradeMax, const int iTryMulti, const int iDur, const int iHigh);
extern void FameUp2(const int cn, const int Type, const int Value);
extern SORT_DATA PC_TABLE[DRAGON_MAX_CONNECTIONS_+1];
extern void QuestTeleport( const int nCn, const int nX, const int nY );

//<! BBD 040401		// 국가방어력에 따른 결계석 아이템 보상용
int g_RequitalItemRate[8] =
{
	100, 70, 45, 25, 15, 5, 3, 1,

};

int DecideRequitalByNationDefensePoint(CHARLIST * ch);
unsigned short	g_DefensePoint[NW_NATION_COUNT];	// BY,ZY,YL LocalWar Point

#define SEALSTONE_ITEMGIVERATE	2

//> BBD 040401		// 국가방어력에 따른 결계석 아이템 보상용


struct IsIDHereInTimer : binary_function<CScriptTimer, int, bool>
{
public:
	bool operator()(const CScriptTimer& rTimer, const int nID) const //, const char* szName) const
	{
		return (rTimer.GetConnectionID() == nID) ? true:false;
	}
};

struct IsEndInTimer : binary_function<CScriptTimer, int, bool>
{
public:
	bool operator()(const CScriptTimer& rTimer, int z) const
	{
		return ( !rTimer.IsStart() );
	}
};

struct IsIDHereInCounter : binary_function<CScriptCounter, int, bool>
{
public:
	bool operator()(const CScriptCounter& rCounter, const int nID) const 
	{
		return (rCounter.GetConnectionID() == nID) ? true:false;
	}
};

struct IsEndInCounter : binary_function<CScriptCounter, int, bool>
{
public:
	bool operator()(const CScriptCounter& rCounter, int z) const //, int z) const
	{
		return ( !rCounter.IsStart() );
	}
};

struct IsNpcIndexHere : binary_function<table_requital_list, int, bool >
{
public:
	bool operator()( const table_requital_list& rRequital, int index) const
	{
		return ( rRequital.Npc_index == index )? true: false;
	}
};
//<! BBD 040329
struct IsItemIndex : binary_function<table_requital_Item, int, bool >
{
public:
	bool operator()( const table_requital_Item& rRequital, int index) const
	{
		return ( rRequital.index== index )? true: false;
	}
};
//> BBD 040329
/*
template <class T>
struct IsCnHere : binary_function<T, int,bool>
{
	bool operator()( const T& rT, int iId ) const
	{
		return ( rT.GetId( ) == iId )? true: false;
	}
};
*/
struct IsCnHere : binary_function<CSpellMove, int,bool>
{
	bool operator()(const CSpellMove &rT, int iId ) const
	{
		return ( rT.GetId() == iId )? true: false;
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CQuestInMap
CQuestInMap::CQuestInMap()
{
}
	
CQuestInMap::~CQuestInMap()
{	// 040305 kyo
	SAFE_DELETE(m_cRequital);
}	

int CQuestInMap::LoadQuestInMap()
{
	char szQuery[0xff];
	HSTMT hStmt= NULL ;
	RETCODE ret ;
	//SWORD	nCols ;
	SDWORD cbValue ;
	
    SQLAllocStmt(hDBC, &hStmt);
	
	strcpy_s( szQuery, sizeof(szQuery), "select * from quest_in_map order by quest_num" );

	ret= SQLExecDirect(hStmt, (UCHAR *)szQuery, SQL_NTS) ;
	
	if(ret == SQL_SUCCESS_WITH_INFO || ret == SQL_SUCCESS)
	{
		ret = SQLFetch( hStmt );
		while( ret == SQL_SUCCESS )
		{
			table_quest_in_map table;
			//quest_num, map_num
			ret = SQLGetData( hStmt, 1, SQL_C_LONG, &table.quest_num, 0, &cbValue );
			ret = SQLGetData( hStmt, 2, SQL_C_LONG, &table.map_num, 0, &cbValue );

			if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) 
			{
				MyLog( LOG_FATAL, "Table : quest_in_map: Error!!! (%d)", ret) ;
				return -1;
			}
			SetQuestInMap( table );

			ret = SQLFetch( hStmt );
		}
		return 1;
	}
	return 0;
}	

int CQuestInMap::LoadQuestInfoByStep()
{
	char szQuery[0xff];
	HSTMT hStmt= NULL ;
	RETCODE ret ;
	//SWORD	nCols ;
	SDWORD cbValue ;
	
    SQLAllocStmt(hDBC, &hStmt);
	
	strcpy_s( szQuery, sizeof(szQuery), "select * from quest_info_byStep order by quest_num" );

	ret= SQLExecDirect(hStmt, (UCHAR *)szQuery, SQL_NTS) ;
	
	if(ret == SQL_SUCCESS_WITH_INFO || ret == SQL_SUCCESS)
	{
		ret = SQLFetch( hStmt );
		while( ret == SQL_SUCCESS )
		{
			table_quest_info_bystep table;
			//quest_num, begin_step, end_step, quest_info
			ret = SQLGetData( hStmt, 1, SQL_C_LONG, &table.quest_num, 0, &cbValue );
			ret = SQLGetData( hStmt, 2, SQL_C_LONG, &table.begin_step, 0, &cbValue );
			ret = SQLGetData( hStmt, 3, SQL_C_LONG, &table.end_step, 0, &cbValue );
			ret = SQLGetData( hStmt, 4, SQL_C_LONG, &table.quest_info, 0, &cbValue );

			if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) 
			{
				MyLog( LOG_FATAL, "Table : quest_info_bystep: Error!!! (%d)", ret) ;
				return -1;
			}
			SetQuestInfoByMap( table );

			ret = SQLFetch( hStmt );
		}
		return 1;
	}
	return 0;
}

list<int> CQuestInMap::GetQuestInMap( int iMapnumber)
{
	list<int>	retQuest;
	list<table_quest_in_map>::iterator	it;

	for( it = m_tQuestInMap.begin(); it != m_tQuestInMap.end(); it++ )
		if( (*it).map_num == iMapnumber )
			retQuest.push_back( (*it).quest_num );

	return retQuest;
}

int CQuestInMap::GetQuestInfo( int iQuestNum, int iStep)
{//단 하나의 결과만이 있을 뿐이다.  결과 없으면 -1
	list<table_quest_info_bystep>::iterator it;
	for( it = m_tQuestInfo.begin(); it != m_tQuestInfo.end(); it++)
		if( (*it).quest_num == iQuestNum )
			if( (*it).begin_step < iStep && iStep <= (*it).end_step )
				return (*it).quest_info;
	return -1;
}

void CQuestInMap::SendQuestInfo( t_quest_in_map *tp, int cn)
{
	if( tp == NULL ) return;

	list<int> lQuestNum;
	lQuestNum = GetQuestInMap( tp->iMapnumber);
	if( lQuestNum.size() <=0 ) return;

	list<int>::iterator it;
	for( it = lQuestNum.begin(); it != lQuestNum.end(); it++)
	{
		//int z= GetQuestInfo( (*it), var[cn][(*it)]);
		if( 0 > (tp->iQuestInfo = GetQuestInfo( (*it), var[cn][(*it)])) ) continue;	// 021126 kyo
		else
			SendShowQuestInfo( tp, cn );
	}

}

void CQuestInMap::SendShowQuestInfo( t_quest_in_map *tp, int cn )
{
	if( tp==NULL ) return;
	t_packet packet;
	packet.h.header.type	= CMD_WHAT_QUEST_IN_MAP;
	packet.h.header.size = sizeof( t_quest_in_map);
	memset( (void**)&packet.u.quest_in_map, 0, sizeof( t_quest_in_map ) );
	memcpy( (void**)&packet.u.quest_in_map, tp, sizeof( t_quest_in_map ) );
	packet.u.quest_in_map.name[20] = '\0';
	
	QueuePacket( connections, cn, &packet, 1 );
}

int CQuestInMap::CheckSpell( const int cn, const char* szSpell)
{	//사람이 엄꺼나 주문이 틀리면 0,성공하면 1
	//주문을 성공적으로 암기하면 기존 주문을 초기화한다.

	if( m_szClientSpell.size() <=0 ) return 0;
	string spell = GetSpellWord( cn );
	if( spell.size() == NULL ) return 0;

	if( strcmp( spell.c_str(), szSpell ) == 0 )
	{
		DeleteSpellUser( cn);
		return 1;
	}

	return 0;
}

/* Code is not unused, and no you can't return NULL instead of a list<T>. Bunch of monkies.
list<CSpellMove>::iterator CQuestInMap::FindSpellUser( const int cn )
{
	SPELLITER itFind = find_if(m_szClientSpell.begin(), m_szClientSpell.end(), bind2nd( IsCnHere(), cn ) );

	if( itFind != m_szClientSpell.end() )
		return itFind;

	return itFind;
}
*/

void CQuestInMap::DeleteSpellUser( const int cn )
{
	if( m_szClientSpell.size() <=0 ) return;

	SPELLITER itFind = find_if(m_szClientSpell.begin(), m_szClientSpell.end(), bind2nd( IsCnHere(), cn ) );

	if( itFind != m_szClientSpell.end() )
		m_szClientSpell.erase(itFind);
}

string CQuestInMap::GetSpellWord( const int cn )
{
	SPELLITER itFind = find_if(m_szClientSpell.begin(), m_szClientSpell.end(), bind2nd( IsCnHere(), cn ) );

	if( itFind != m_szClientSpell.end() )
		return (*itFind).m_szSpell;
  return ""; // 021114 kyo
}

void CQuestInMap::SendSpellMapMove( const int&cn, const char* szSpell, const char* szMap, const int&  x, const int& y)
{// 021128 kyo
	t_packet packet;
	packet.h.header.type	= CMD_SCRIPT_SPELL_MAPMOVE;
	packet.h.header.size	= sizeof( t_script_spellmapmove );
	packet.u.spell_mapmove.cn = cn;
	packet.u.spell_mapmove.iX = x;
	packet.u.spell_mapmove.iY = y;
	strcpy_s( packet.u.spell_mapmove.szMap, sizeof(packet.u.spell_mapmove.szMap), szMap );
	//주문은 유니코드가 될 가능성이 있으니깐 메모리 카피로 하자.
	//strcpy( packet.u.spell_mapmove.szSpell, szSpell );
	memset( (void**)&packet.u.spell_mapmove.szSpell, 0x00, sizeof( packet.u.spell_mapmove.szSpell ) );
	memcpy( (void**)&packet.u.spell_mapmove.szSpell, szSpell, sizeof( packet.u.spell_mapmove.szSpell ) );
	
	QueuePacket( connections, cn, &packet, 1 );	
}

void CQuestInMap::RecvSpellMapMove( const t_script_spellmapmove *spell, const int& cn )
{// 021128 kyo
	if( spell->cn != cn ) return;

	if( !spell->isSuc )
	{
		SendSpellMapMove_Fail(cn);
		return;
	}

	SendSpellMapMove_Suc(spell->szMap, cn);
	::MapMove( cn, spell->szMap, spell->iX, spell->iY );
}

void CQuestInMap::SendSpellMapMove_Suc(const char* szMap, const int& cn)
{// 021128 kyo
	t_packet packet;
	packet.h.header.type	= CMD_SCRIPT_SPELL_MAPMOVE_SUC;
	packet.h.header.size	= sizeof( t_script_spellmapmove);
	memset( packet.u.spell_mapmove.szMap, 0, sizeof( packet.u.spell_mapmove.szMap));
	strcpy_s( packet.u.spell_mapmove.szMap, sizeof(packet.u.spell_mapmove.szMap), szMap );//, strlen( packet.u.spell_mapmove.szMap));
	//memcpy( packet.u.spell_mapmove.szMap, szMap, strlen( packet.u.spell_mapmove.szMap));

	QueuePacket( connections, cn, &packet, 1 );
}

void CQuestInMap::SendSpellMapMove_Fail(const int& cn)
{// 021128 kyo
	t_packet packet;
	packet.h.header.type	= CMD_SCRIPT_SPELL_MAPMOVE_FAIL;
	packet.h.header.size	= 0;
	QueuePacket( connections, cn, &packet, 1 );
}

void CQuestInMap::ShowStateMsg( const int& cn, const char* szMsg, const int& R, const int& G, const int& B )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;
	t_packet packet;
	packet.h.header.type	= CMD_SCRIPT_SHOW_COLOR_MSG;
	packet.h.header.size	= sizeof( t_show_msg);
	packet.u.show_msg.R = R;
	packet.u.show_msg.G = G;
	packet.u.show_msg.B = B;
	//strcpy( packet.u.show_msg.szMsg , szMsg );
	memset( packet.u.show_msg.szMsg, 0, sizeof( packet.u.show_msg.szMsg ) );
	memcpy( packet.u.show_msg.szMsg, szMsg, strlen( szMsg ) );

	QueuePacket( connections, cn, &packet, 1 );	
}

void CQuestInMap::ShowStateMsg( const int& cn, const int& iMsgNum, const int& R, const int& G, const int& B )
{
	t_packet packet;
	packet.h.header.type	= CMD_SCRIPT_SHOW_COLOR_MSG;
	packet.h.header.size	= sizeof( t_show_msg);
	packet.u.show_msg.R = R;
	packet.u.show_msg.G = G;
	packet.u.show_msg.B = B;
	packet.u.show_msg.iMsgNum = iMsgNum;

	QueuePacket( connections, cn, &packet, 1 );
}

inline void RunNothing(){return;};

inline void SendSynchPacket1()
{
	t_packet packet;
	packet.h.header.type	= CMD_SCRIPT_SHOW_COLOR_MSG;
	packet.h.header.size	= sizeof( t_show_msg);
	char tmp[10];
	sprintf_s( tmp, sizeof(tmp), "%d", EventPC );
	strcpy_s( packet.u.show_msg.szMsg, sizeof(packet.u.show_msg.szMsg), tmp);
	packet.u.show_msg.R = 200;
	packet.u.show_msg.G= 200;
	packet.u.show_msg.B= 200;
	
	QueuePacket( connections, EventPC, &packet, 1 );
};

inline void SendSynchPacket2(DWORD dwTmp)
{
	t_packet packet;
	packet.h.header.type	= CMD_SCRIPT_SHOW_COLOR_MSG;
	packet.h.header.size	= sizeof( t_show_msg);
	char tmp[10];
	sprintf_s( tmp, sizeof(tmp), "%d", (dwTmp/1000) );
	strcpy_s( packet.u.show_msg.szMsg, sizeof(packet.u.show_msg.szMsg), tmp);
	packet.u.show_msg.R = 200;
	packet.u.show_msg.G= 0;
	packet.u.show_msg.B= 0;
	
	QueuePacket( connections, EventPC, &packet, 1 );
};


bool CQuestInMap::StartScriptTimer(const int iServerID, const char* szName, const DWORD &dwTimer, const DWORD &dwSync, const char* mapfile, const int& x, const int& y)
{
	//시간이 0이면 해당 cn과 name를 가진 타이머를 해제한다.
	//시간이 양수면 들어논 값으로 타이머를 push한다.

	_ASSERTE( szName != NULL );
	if( szName == NULL ) return false;
	
	if( dwTimer == 0 )	//0이면 해당 타이머 삭제
	{
		//해당번호로된 타이머를 해제한다.
		if( DeleteScriptTimer( iServerID, szName ) ) 
			return true;
		else
			return false;
		//만약 접속이 원활하지 않아 iServerID랑 szName가 일치하지 않아 타이머를 해제할수 없는 경우라도
		//RunScirtpTimer에서 일치하지 않는것을 very_long_term에 따라 확인하기때문에 상관없다.
	}
	else if( dwTimer < 0 )	//음수면 에러
		return false;

	//해당 유저가 타이머를 이미 실행하구 있으면 새로 타이머를 돌리지 않는다. 
	if( FindUserInScriptTimer( iServerID, szName ) != m_cTimer.end() ) 
		return false;
	
	//CScriptTimer CScriptTimer( iServerID, szName, dwTimer, dwSync );	
	CScriptTimer cTimer;//(iServerID, dwTimer, );
	cTimer.SetID( iServerID);
	cTimer.SetName( szName );
	cTimer.SetTimerTime( dwTimer );
	cTimer.SetSynchroneTime( DEFAULT_TIMER_SYNC_TIME ); //5초에 한번
	cTimer.SetFunction( RunNothing, SendSynchPacket1 );	//현재는 이 함수를 사용하지 않는다. 
	cTimer.SetAfterMap( mapfile, x, y);//원래 함수를 세팅해야하지만 

	//cTimer.SendSCRIPT_TIEMER( dwTimer, CMD_SCRIPT_TIMER_START );
	cTimer.StartTimer();
	m_cTimer.push_back( cTimer );
	return true;
}

bool CQuestInMap::DeleteScriptTimer( const int iServerID , const char* szName )	
{//해당 cn과 이름이 있는것은 지운다.
//패킷을 받거나 스크립트일때다..
	TIMERITER itFind = find_if( m_cTimer.begin(), m_cTimer.end(), bind2nd( IsIDHereInTimer(), iServerID) );

	if (itFind != m_cTimer.end())
		if( strcmp( szName, (*itFind).GetName() ) == 0 )
		{		
			(*itFind).SendSCRIPT_TIEMER( 0, CMD_SCRIPT_TIMER_END_FAL);
			DeleteScriptCounter( iServerID, szName );	//Timmer가 지워질때 Counter도 지워진다. 
			m_cTimer.erase(itFind);
			return true;
		}
	return false;
}

void CQuestInMap::DeleteScriptTimer()	
{//타이머가 끝난것지운다. 

	int tmp = 0;	//난중에 지웁시다.
	TIMERITER itFind = find_if(m_cTimer.begin(), m_cTimer.end(), bind2nd( IsEndInTimer(),  tmp) );

	if (itFind != m_cTimer.end())
	{
		(*itFind).SendSCRIPT_TIEMER( 0, CMD_SCRIPT_TIMER_END_SUC);
		DeleteScriptCounter( (*itFind).GetConnectionID(), (*itFind).GetName());//iServerID, szName );	//Timmer가 지워질때 Counter도 지워진다. 
		m_cTimer.erase(itFind);
	}
}


void CQuestInMap::RunScriptTimer()
{
	if( m_cTimer.size() <= 0 ) return;	//하나두 없쓰면 기냥 리턴한다.

	list<CScriptTimer>::iterator it;
	//for_each( m_cTimer.begin(), m_cTimer.end(), m_cTimer.RunTimer() );
	//난중에 성동씨에게 물어볼껏... for_each에서it에 들어가는 함수 써도 되는가...
	for( it = m_cTimer.begin(); it != m_cTimer.end(); it++)
	{
		//int z = (*it).GetConnectionID();
		(*it).RunTimer();

		//이름이랑 cn이 틀리는 경우는 타이머를 멈춘다.(싶
		if( !CheckID2Name( (*it).GetConnectionID(), (*it).GetName() ) )
			(*it).EndTimer( SCRIPT_END_FAL );
	}


	//이게 [ if(!ch) return; ] 여기있는 이유 .. 유저가 접속해서 퀘스트를 진행하다 로그아웃했다. 모든 이들이 동시에 말야..
	// m_cTimer.size()는 여러개지만 cn이 없기 때문에 이게 입구에 있다면 바로 리턴해버러셔
	// cn,name쌍이 틀리는 놈을 지울수가 없다.
	// 그러다가 아까 퀘스트 진행하던 넘이 들어왔는데. 재수로 전의 cn을 받을 가능성이 있꺼덩..  그라면 
	// 아까 카운터가 계속 돌고( 싱크관련은 넣지 안아쓰므로 ) 이유없이 맵이동을 할수가 있따.. 그럼 그거 어찌 차져? 졸나 어려운 버그가 될지두 멀른단말야.
	CHARLIST *ch = CheckServerId( EventPC );
	if( !ch ) return;

	//타이머가 끝난것이 있으면 종료시킨다.
	DeleteScriptTimer();
}

void CQuestInMap::EndScriptTimer( const int iID )
{
	CHARLIST *ch = CheckServerId( iID );
	if( !ch ) return;

	DeleteScriptTimer( iID , ch->Name );
}

bool CQuestInMap::CheckID2Name( const int& iID, const char* szName )
{//아뒤랑 이름이 현 리스트랑 일치하지 않는다.
	if( !szName ) return false;
	CHARLIST *ch = CheckServerId( iID );
	if( !ch) return false;//유저가 이미 나갔다.

	//유저가 바꼈다.
	if( strcmp( ch->Name, szName ) != 0  )
		return false;

	return true;
}

void CQuestInMap::RevcScriptTimerPacket( t_script_timer *timer, int iType, int cn )
{
	//해당 cn의 캐릭터가 타이머를 사용하고 있을때
	CHARLIST *ch = CheckServerId( cn);
	if( !ch) return;

	TIMERITER it;
	it= FindUserInScriptTimer(cn, ch->Name);
	if( it == m_cTimer.end() ) return;

	switch( iType )
	{
	case CMD_SCRIPT_TIMER_START:
		{}break;
	case CMD_SCRIPT_TIMER_SYNC:
		{
			//싱크가 안 맞으면 맵타이머랑 클라이 타이머 종료한다..
			if( !(*it).ConfirmSyncTimer( timer->dwTime ) )
			{
				(*it).SendSCRIPT_TIEMER( 0, CMD_SCRIPT_TIMER_END_FAL);
				m_cTimer.erase( it );
			}

		}break;
	case CMD_SCRIPT_TIMER_END_SUC:
		{
			(*it).EndTimer( SCRIPT_END_SUC );	

			DeleteScriptCounter( cn, ch->Name );		//타이머가 종료할때 카운터도 같이 종료시킨다. 
			
			//(*it).SendSCRIPT_TIEMER( 0, CMD_SCRIPT_TIMER_END_SUC);
			//m_cTimer.erase( it );
		}break;
	case CMD_SCRIPT_TIMER_END_FAL:
		{
			(*it).EndTimer( SCRIPT_END_FAL ); //m_cTimer.erase( it );
			DeleteScriptCounter( cn, ch->Name );		//타이머가 종료할때 카운터도 같이 종료시킨다. 
			
		}break;
	default:
		break;
	}
	
}

list<CScriptTimer>::iterator CQuestInMap::FindUserInScriptTimer( const int& iId, const char* name)
{
	TIMERITER itFind = find_if(m_cTimer.begin(), m_cTimer.end(), bind2nd( IsIDHereInTimer(), iId ) );
	if( itFind != m_cTimer.end() ) 
		if( strcmp( (*itFind).GetName(), name) == 0)
			return itFind;

	return itFind;
}

bool CQuestInMap::StartScriptCounter( const int iServerID, const char* szName, const int iNum, const int iType, const int iMuch, const int iFlag)
{	//종류: 0=ncp,1=아이템,2=미정. 플러그: 0=멈춤,1=시작,2=갯수랑 일치

	_ASSERTE( szName != NULL );
	if( szName == NULL ) return false;

	switch( iFlag) 
	{
		case SCRIPT_COUNT_STOP :
		{	//카운터를 멈춰라(삭제해라)
			return DeleteScriptCounter( iServerID, szName );			
		}break;

		case SCRIPT_COUNT_CHECK :
		{	//카운터 값을 확인해라
			return CheckScriptCounter( iServerID, szName, iNum, iType, iMuch );			
		}break;

		case SCRIPT_COUNT_START:
		{
			if( FindUserInScriptCounter( iServerID, szName, iType ) != m_cCounter.end() ) return false;//!= m_cCounter.end() ) return false;

			CScriptCounter cCounter;
			cCounter.SetConnectionID( iServerID);
			cCounter.SetName( szName );
			cCounter.SetSpeciesNum( iNum );
			cCounter.SetSpeciesType( iType );
			cCounter.SetSpeciesMuch( iMuch );
			cCounter.StartCounter();
			m_cCounter.push_back( cCounter );

		}break;
	}

	return true;
	
}

bool CQuestInMap::DeleteScriptCounter( const int iServerId, const char* szName)
{	
	COUNTERITER itFind = find_if(m_cCounter.begin(), m_cCounter.end(), bind2nd( IsIDHereInCounter(), iServerId ) );

	if( itFind != m_cCounter.end() ) 
		if( strcmp( (*itFind).GetName(), szName) == 0 )
		{
			(*itFind).EndCounter();
			m_cCounter.erase( itFind);
			return true;
		}
	return false;
}

void CQuestInMap::DeleteScriptCounter()
{
	int z=0;	//난중에 bind1st를 잘 알게되면 고치자.
	COUNTERITER itFind = find_if( m_cCounter.begin(), m_cCounter.end(), bind2nd( IsEndInCounter(), z ) );
	if( itFind != m_cCounter.end() ) 
	{
		(*itFind).EndCounter();
		m_cCounter.erase( itFind);
	}

}

void CQuestInMap::RunScriptCounter( const int cn, const char* szName, int iNum, int iType)//CHARLIST *ch )//, const char* szName)
{

	if( m_cCounter.size() <=0 ) return;

	COUNTERITER	it = FindUserInScriptCounter( cn, szName, iType );	//해당 유저가 해당타입의 카운터를 실행하면
	if( it == m_cCounter.end() ) return;

	{
		(*it).RunCounter( iNum );
	}
	
	DeleteScriptCounter();	//카운터가 끝이 나면 카운터를 지울까?( 서버타이머만 죽는다.) 클라이언트는 RunCounter에 있다.
}

list<CScriptCounter>::iterator CQuestInMap::FindUserInScriptCounter( const int& iId, const char* name, const int iType)
{
	COUNTERITER itFind = find_if(m_cCounter.begin(), m_cCounter.end(), bind2nd( IsIDHereInCounter(), iId ) );
	if( itFind != m_cCounter.end() ) 
		if( strcmp( (*itFind).GetName(), name) == 0)
			if( (*itFind).GetSpeciesType() == iType )
				return itFind;

	return m_cCounter.end();
}

bool CQuestInMap::CheckScriptCounter( const int& iServerId, const char* szName, const int iNum, const int iType, const int iCounter )
{
	COUNTERITER itFind = find_if(m_cCounter.begin(), m_cCounter.end(), bind2nd( IsIDHereInCounter(), iServerId ) );
	if( itFind == m_cCounter.end() ) 
	{
		MyLog(0,"CheckScriptCounter Returned 1");
		return false;
	}

	if( ::strcmp( (*itFind).GetName(), szName)) 
	{
		MyLog(0,"CheckScriptCounter Returned 2");
		return false;
	}

	if( (*itFind).GetSpeciesType() != iType )
	{
		MyLog(0,"CheckScriptCounter Returned 3");
		return false;
	}

	if( (*itFind).GetSpeciesNum() != iNum )					
	{
		MyLog(0,"CheckScriptCounter Returned 4 user's %d zs file %d",(*itFind).GetSpeciesNum(),iNum);
		return false;
	}

	if( (*itFind).GetCounter() < iCounter) 				
	{
		MyLog(0,"CheckScriptCounter Returned 5 user's user's %d, %d",(*itFind).GetCounter(),iCounter);
		return false;
	}
	return true;
}

void CQuestInMap::RevcScriptCounterSync( t_script_counter *counter,  int cn )
{// 030110 
	CHARLIST *ch = CheckServerId( cn);
	if( ch == NULL ) return;

	COUNTERITER itFind = find_if(m_cCounter.begin(), m_cCounter.end(), bind2nd( IsIDHereInCounter(), cn ) );	
	if( itFind != m_cCounter.end() ) 
		if( strcmp( (*itFind).GetName(), ch->Name) == 0)
		{
			(*itFind).SetCounter( counter->iCounter );
		}
	
}

void CQuestInMap::RunQuestTimer()
{
	RunScriptTimer();	//스크립트 타이머
	RunBossTimer();		//boss 타이머
}

DWORD CQuestInMap::StartBossTimer(const int& iCn, const int& iBoss, const int& iX, const int& iY, const DWORD& dwTimer, const int iMaxEntree, const int nFlag) 
{	//boss타이머를 생성한다. 보스도 생성한다.
	CHARLIST *ch = ::CheckServerId( iCn );
	if( !ch ) return 0;

	//제약인원에 걸린다. 반환 0
	//타이머가 돌고있다  현재시간 반환
	//타이머가 없다.     설정타이머 시간 반환하고 보스생성
	int nKey = iX*1000 + iY; 
	if( m_mBossTimer.find( nKey ) != m_mBossTimer.end() )	//이 타이머가 있다.
	{
		if( iMaxEntree<= m_mBossTimer[nKey]->GetUserSize() )
		{
			return 0;
		}
		else
		{
			m_mBossTimer[nKey]->AddUser( iCn );
			return (m_mBossTimer[nKey]->m_cBossTimer.GetCurretTimerTime() - GetCurrentBossTimer( nKey));
		}

	}
	else
	{//해당 타이머가 없으르모 생성한다. 
		
		CBossTimer *cBTimer;
		cBTimer= new CBossTimer;
		cBTimer->m_cBossTimer.SetTimerTime( dwTimer );		
		cBTimer->m_cBossTimer.SetSynchroneTime( DEFAULT_TIMER_SYNC_TIME ); //5초에 한번
		cBTimer->m_cBossTimer.StartTimer( true );	//서버만 도는 타이머
		cBTimer->m_nFlag	=nFlag; 
		
		DeleteBossNpc( nKey );
		if( CBossTimer::TYPE_BOSS_ONLY == nFlag )
		{			
			cBTimer->m_nBossID = GetAliveNPCList(); //생성할수 있는 listid를 부여받는다.
			cBTimer->m_iBossSprNo = iBoss;
			if( ::NPC_Create( cBTimer->m_nBossID , iBoss, iX, iY,-1, 0, GT_SCRIPT ) )//021230 lsw 마법을 안쓰기 때문에 GT_SCRIPT로 바꿔서 테스트
				
			{	// npc를 생성한로그를 남긴다.
				MyLog( 1, "BossTimerStart :StartBossTimer:BossListID=%d,BossSprNo=%d,OpenCN=%s,Timer=%d",
							cBTimer->m_nBossID, iBoss, ch->Name, dwTimer);
			}
			
		}
		else if( CBossTimer::TYPE_BOSS_GROUP == nFlag )
		{
			cBTimer->m_nBossID = iBoss;
			g_pRegenManager->Ready(iBoss);
		}

		cBTimer->AddUser( iCn);
		m_mBossTimer.insert( PAIR_BOSS(nKey, cBTimer) );
	}

	return dwTimer;
}

void CQuestInMap::RunBossTimer()
{
	if( m_mBossTimer.empty() ) return;

BEGIN_LOOP:
	for( map<int, CBossTimer*>::iterator it = m_mBossTimer.begin() ; it != m_mBossTimer.end(); it++)
	{
		(*it).second->m_cBossTimer.RunTimer();
		if( !(*it).second->m_cBossTimer.IsStart() )
		{
			if( DeleteBossTimer( (*it).first ) )
			{
				goto BEGIN_LOOP;
			}
		}
	}
}

void CQuestInMap::DecreaseBossTimerUser(const int nKey, const int nCn)
{
	//return;
	ITOR_BOSS it = m_mBossTimer.find( nKey );
	if( it == m_mBossTimer.end() )
	{
		return;
	}
	m_mBossTimer[nKey]->DeleteUser(nCn);
}

void CQuestInMap::CheckBossTimerBoss( const int nKey)
{
	ITOR_BOSS it = m_mBossTimer.find( nKey );
	if( it == m_mBossTimer.end() )
	{
		return;
	}
	if( 0 >= m_mBossTimer[nKey]->GetUserSize() )
	{
		DeleteBossTimer( nKey );
	}
}

bool CQuestInMap::DeleteBossTimer(int nKey)
{
	ITOR_BOSS it = m_mBossTimer.find( nKey );
	if( it == m_mBossTimer.end() )
	{
		return false;
	}

	DeleteBossNpc(nKey);
	SAFE_DELETE( (*it).second );
	m_mBossTimer.erase( it );	

	return true;
}


void CQuestInMap::DeleteBossNpc(int nKey)
{
	ITOR_BOSS it = m_mBossTimer.find( nKey );
	if( it == m_mBossTimer.end() )
	{
		return;
	}

	(*it).second->RemoveBossNpc();
}

DWORD CQuestInMap::GetCurrentBossTimer(int nKey)
{
	if( m_mBossTimer.find(nKey) == m_mBossTimer.end() )
	{
		return 0;
	}
	return m_mBossTimer[nKey]->m_cBossTimer.GetSpendTime();
}

int CQuestInMap::GetBossTimerEntree(int nKey)
{
	if( m_mBossTimer.find(nKey) == m_mBossTimer.end() )
	{
		return 0;
	}
	return m_mBossTimer[nKey]->GetUserSize();
}

int	CQuestInMap::LoadRequitalTable()
{
	if( m_cRequital != NULL )
		DeleteRequeitalTable();
	m_cRequital =  new CRequital;

	return m_cRequital->LoadRequitalTable();
}
//<! BBD 040329
int	CQuestInMap::LoadRequitalItemTable()
{
	if( m_cRequital == NULL )
		m_cRequital =  new CRequital;

	return m_cRequital->LoadRequitalItemTable();
}
//> BBD 040329
void CQuestInMap::DeleteRequeitalTable()
{
	SAFE_DELETE(m_cRequital);
}


void CQuestInMap::GiveRequitalAfterKillNpc( const CHARLIST *a, const CHARLIST *d)
{
	if( !d ) return;

	//d가 몬스터가 아니면 리턴
	if( d->IsPlayer() ) return;

	//npc먼저 찾고 지도가 맞는지 찾는다. 
	if( !m_cRequital ) return;
	REQUITALLISTITER it = FindRequitalNpc( d->npc_index );
	if( it == m_cRequital->m_tRequital.end() ) return;

//<! BBD 040318	// 애시당초 맵넘버에 해당하는 레코드만 얻어왔으므로 비교가 무의미
	//맵번호 확인
//	if( (*it).Map_num != MapNumber ) return;
//> BBD 040318

	if( (*it).Tile_area > 0 )
	{
		RequitalRoundUser( d->GetServerID(), (*it) );
	}
	else if( (*it).Tile_area  == 0 ) 
	{
		RequitalKillUser( a->GetServerID(), (*it) );
	}
}

list<table_requital_list >::iterator CQuestInMap::FindRequitalNpc( const int iNpcIndex)
{
	return m_cRequital->GetRequital_KillNpc( iNpcIndex );
}

void CQuestInMap::RequitalRoundUser( const int &cn, table_requital_list& table)
{
	list<int> ListCn;
	ListCn = GetRoundUser( cn );
	if( ListCn.size() <= 0 ) return;
	list<int>::iterator it;

	int i = 0;		// BBD 040401
	int rate = rand()%SEALSTONE_ITEMGIVERATE;	// BBD 040401

	for( it = ListCn.begin(); it != ListCn.end(); it++ )
	{
		//<! BBD 040401		결계석은 전체의 1/2만 지급
		if(table.WarStatus)
		{
			if((i%SEALSTONE_ITEMGIVERATE) == rate)
			{
				GiveRequitalByCn( (*it), table );
			}
			i++;
		}
		else
		{
			GiveRequitalByCn( (*it), table );	
		}
		//> BBD 040401		결계석은 전체의 1/2만 지급
	}
}

void CQuestInMap::RequitalKillUser( int cn, table_requital_list& table)
{
	GiveRequitalByCn( cn, table );
}

void CQuestInMap::GiveRequitalByCn( const int cn, table_requital_list& table)
{
	CHARLIST *ch = CheckServerId( cn);
	if( !ch) return;
//<! BBD 040318	 국지전에 참가중인지의 체크
	if(table.WarStatus)	// 국지전 참가일때만 보상을 줄것임
	{
		//<!BBD 040401	 국가 방어력에 따라 아이템 지급을 결정
		int nResult = DecideRequitalByNationDefensePoint(ch);

		if( nResult == -1)	// 에러인경우
		{
			MyLog( LOG_NORMAL, "Character have wrong nation code name : %s", ch->Name );			
		}
		else if(nResult == 0)	// 보상확률내로 못들어왔다.
		{
			return;
		}
		//>BBD 040401	 국가 방어력에 따라 아이템 지급을 결정
		
		// Attack한 놈이 국지전에 참가중인지 확인할 필요 있음
		if(!ch->JoinLocalWar)
		{
			return;
		}
		// 방어국 플레이어면 주지 말자
		switch(table.Npc_index)
		{
		case SEALSTONE_YLLSE_NO:
			{
				if(ch->name_status.nation == N_YLLSE)
				{
					return;
				}
				break;
			}
		case SEALSTONE_VYSEUS_NO:
			{
				if(ch->name_status.nation == N_VYSEUS)
				{
					return;
				}
				break;
			}
		case SEALSTONE_ZYPERN_NO:
			{
				if(ch->name_status.nation == N_ZYPERN)
				{
					return;
				}
				break;
			}
		default:
			break;
		}
		//<! BBD 040329 국지전 전용의 아이템을 준후 리턴하자
		if(!Requital_About_SealStone(cn, table.Sardonyx, table.LeafOfBlessed, table.ref_index))
		{
			::OutMessage(ch,2,13);//"인벤토리 공간이 충분치 않습니다."
		}
		else
		{
			::OutMessage(ch,4,120);//"아이템이 인벤토리로 지급되었습니다.
			return;
		}
		//> BBD 040329 국지전 전용의 아이템을 준후 리턴하자
	}
//> BBD 040318	 국지전에 참가중인지의 체크

	if(table.Item_no != 0)	//item
	{
		//<! BBD 040322
		if(SetRareItemInvPc( cn, table.Item_no, table.Item_Min, table.Item_Max, table.Item_rare_count, 0, table.Item_rare_type ))
		{
			// 성공
			::OutMessage(ch,4,120);//"아이템이 인벤토리로 지급되었습니다.
		}
		else
		{
			// 실패
			::OutMessage(ch,2,13);//"인벤토리 공간이 충분치 않습니다."
		}
		//> BBD 040322
	}

	if( table.Quest_no !=0)	//qust_step
	{
		var[ cn ][ table.Quest_no ] = table.Quest_step;
	}

	if( table.Real_Fame !=0 )	//real_fame
	{
		FameUp2( cn, 1, table.Real_Fame );
	}

	{//dual fame값은 quest값의 10번값이다.
		const int iOldDF = var[ch->GetServerID()][DUAL_FAME_FIELD];
		const int iNewDF = iOldDF+table.Dual_Fame;
		var[ch->GetServerID()][DUAL_FAME_FIELD] = iNewDF;
		
		::SaveChangeDualFame( ch, iOldDF, iNewDF, LDF_QUEST);
		::MyLog(0,"Increase User Spy Game Point, User : %s, OldPoint %d, NewPoint %d",ch->Name,iOldDF,iNewDF);
	}
}

list<int> CQuestInMap::GetRoundUser( const int cn)
{
	list<int> RtList;
	for( int i = 0; i < PC_COUNT; i++ )
		for(int j = 0; j < MAX_AREA_BUFFER; j ++)
		if( connections[PC_TABLE[i].ID	].area_tbl[j].bAlive == TRUE )
		if( connections[PC_TABLE[i].ID	].area_tbl[j].Type == 2 )
		if( connections[	PC_TABLE[i].ID	].area_tbl[j].ID == cn)
		if( connections[	PC_TABLE[i].ID	].dwAgentConnectionIndex) 
		{ 
			RtList.push_back(  PC_TABLE[i].ID );
		}
	return RtList;
}

///CQuestInMap
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///CBossTimer

CBossTimer::CBossTimer()
{

}

CBossTimer::~CBossTimer()
{
	m_lUserCn.clear();
	int zz = m_lUserCn.size();
	RemoveBossNpc();
}

bool CBossTimer::AddUser(const int nCn)
{
	ITOR_INT it =find( m_lUserCn.begin(), m_lUserCn.end(), nCn );
	if( it == m_lUserCn.end() )
	{
		m_lUserCn.push_back(nCn);
		return true;
	}
	return false;
}

bool CBossTimer::DeleteUser(const int nCn)
{
	ITOR_INT it =find( m_lUserCn.begin(), m_lUserCn.end(), nCn );
	if( it != m_lUserCn.end() )
	{
		m_lUserCn.erase( it);
		return true;
	}
	return false;
	
}

void CBossTimer::RemoveBossNpc()
{
	if( TYPE_BOSS_ONLY == m_nFlag)
	{
		CHARLIST* pNpc = &NPCList[m_nBossID];
		if( !pNpc )	return; //보스가 죽었다는 야기. 
		
		if( pNpc->SprNo == m_iBossSprNo ) 
		{
			::KillMonster( m_nBossID);
		}
		else
		{
			MyLog( 1, "BossTimerStart :DeleteBossNpc: Boss is alive. but sprNo is not accorded" );	
		}
	}
	else if( TYPE_BOSS_GROUP == m_nFlag)
	{
		if( g_pRegenManager->IsExistHunt(m_nBossID) )
		{
			g_pRegenManager->Remove(m_nBossID);
		}
	}
}

//CBossTimer
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CScriptTimer

bool CScriptTimer::StartTimer( bool bOnlyServer )
{
	//시간 절정 채크함수 넣을것
	//시간이 남아있는가 하는거?

	if( m_bStart == true) return false;

	//server only가 아니면 클라이언트도 시작시킨다. 
	if( bOnlyServer == false )	
		SendSCRIPT_TIEMER( m_dwTimerTime, CMD_SCRIPT_TIMER_START);

	m_bStart = true;
	return m_bStart;
}

void CScriptTimer::EndTimer( int iFlag)
{
	m_bStart = false; 
	if( iFlag == SCRIPT_END_SUC )
	{
		SendSCRIPT_TIEMER( 0, CMD_SCRIPT_TIMER_END_SUC);
		AfterTimer();
	}
	else if( iFlag == SCRIPT_END_FAL )
	{
		SendSCRIPT_TIEMER( 0, CMD_SCRIPT_TIMER_END_FAL);
	}
}

void CScriptTimer::ClearTimer()
{
	m_dwTimerTime = 0;
	m_dwSynchTime = 0;
	m_dwSpendTime = 0;
	m_ID		  = 0;
	//m_szName	  = NULL;
	m_dwOld		  = 0;
	m_dwSynch     = 0;
	m_bStart	  = false;
}

bool CScriptTimer::IsRunning()
{
	if( m_bStart== true && m_dwTimerTime > 0 && m_dwSpendTime > 0 )
		return true;
	return false;
}
void CScriptTimer::SetName( const char* name )
{
	if( !name  ) return;
	strcpy_s( m_szName, sizeof(m_szName), name );
	m_szName[30] = '\0';
}

void CScriptTimer::SetTimerTime( DWORD dwTime )
{
	//_ASSERTE( dwTime < MAX_TIMER_TIME );
	//_ASSERTE( dwTime > 0 );
	if( dwTime > MAX_TIMER_TIME || dwTime < 0 )
		m_dwTimerTime = MAX_TIMER_TIME;
	else
		m_dwTimerTime = dwTime;
}

void CScriptTimer::SetSynchroneTime( DWORD dwSynchTime )
{
	//_ASSERTE( dwSynchTime < MAX_TIMER_TIME );
	//_ASSERTE( dwSynchTime > MIN_SYNCH_TIME );

	if( dwSynchTime < MIN_SYNCH_TIME || dwSynchTime > MAX_TIMER_TIME )
		m_dwSynchTime = MIN_SYNCH_TIME;
	else
		m_dwSynchTime = dwSynchTime;
}


void CScriptTimer::RunTimer( )	//이 함수를 very long이니깐 .75초마다 실행된다는걸 유의하도록
{
	//if( !IsRunning() ) return;
	//static DWORD dwOld= 0;		//처음 시작시간
	//static DWORD dwSynch = 0;	//얼마마다 보낼까?
	if( m_dwOld == 0 ) 
		m_dwOld = GetTickCount();

	DWORD dwCurrent = GetTickCount();
	m_dwSpendTime = dwCurrent - m_dwOld;		//얼마나 시간이 지난는가?

	if( m_dwSpendTime > m_dwTimerTime )		//시간이 다 끈나면 끈내라는 명령을 보낸다. 
	{		
		m_dwOld = 0;
		m_dwSynch = 0;
		EndTimer( SCRIPT_END_SUC );	//시간 다 되서 끝나면 성공적인 종료
		return;
	}

	//설정한 시간마다 (*sync)함수 실행
	if( (m_dwSpendTime - m_dwSynch) > m_dwSynchTime )
	{
		m_dwSynch = m_dwSpendTime;

		//for test
		SendSCRIPT_TIEMER( m_dwSpendTime, CMD_SCRIPT_TIMER_SYNC);
		//(*m_syncFunc)();
	}
	//else
	//항상 (*func)함수 실행
	//(*m_func)();
}

void CScriptTimer::SetFunction( void (*func)(), void (*syncFunc)() )
{
	m_func = func;
	m_syncFunc = syncFunc;
}

void CScriptTimer::SendSCRIPT_TIEMER( DWORD dwSpendTime , int iType)
{
	if( m_ID == 0) return;

	t_packet packet;
	packet.h.header.type	= iType;
	packet.h.header.size	= sizeof( t_script_timer);
	packet.u.script_timer.dwTime = dwSpendTime;	
	
	QueuePacket( connections, m_ID, &packet, 1 );
}

void CScriptTimer::SetAfterMap( const char* mapfile, const int& x, const int& y)
{	//< CSD-030509
	strcpy_s(m_szMapFile, sizeof(m_szMapFile), mapfile); 
	m_nPosX	= x;
	m_nPosY = y;
}	//> CSD-030509

void CScriptTimer::AfterTimer()
{	//< CSD-030509
	//타이머가 끝이 난 후에 맵이동 할수도 있고 다른 일을 할수도 있따. 
	if( m_ID == 0 )	return;// id가 설정되지 않았을때는 이동하지 않는다. bosstimer에서 사용된다.
	
	//Log_UserInfo( m_ID); // 030617 kyo
	::CharUpper(m_szMapFile);
	::CharUpper(MapName);
	if( strcmp( m_szMapFile, MapName ) == 0 )
	{//같은 맵이면 퀘스트 텔레포트  로 이동
		::QuestTeleport( m_ID, m_nPosX, m_nPosY);
	}
	else
	{
		::MapMove( m_ID, m_szMapFile, m_nPosX, m_nPosY);
	}
}	//> CSD-030509

bool CScriptTimer::ConfirmSyncTimer( DWORD dwTimer)
{
	//dwRecvTime이랑 현재시간이랑 +-X초 이상 차이나면 동기화 실패다..
	return true;
}

extern void LogInFile( string szFilePath,  char *szFormat, ... ); //kyo
void CScriptTimer::Log_UserInfo( const int nCn)//kyo
{	
	CHARLIST *ch = CheckServerId( nCn );
	if( !ch ) return;
	char szFilename[64]; //Eleval 21/08/09 - Put 64 to prevent having a too small buffer
	string szItem;
	sprintf_s( szFilename, sizeof(szFilename), "./QuestLog/%s.txt", ch->Name );
	char buff[8];
	for( int a=0; a<3; a++ )
	{
		for( int b=0; b<3; b++ )
		{
			for( int c=0; c<8; c++ )
			{
				_itoa_s( ch->inv[a][b][c].item_no, buff, sizeof(buff), 8);
				szItem += buff;
				szItem += " ";
			}
			
		}
	}
	::LogInFile( szFilename, "CScriptTime-Mapmove :: Name:%s, InvItem: %s", ch->Name, szItem.c_str() );
}
//CScriptTimer
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CScriptCounter
void CScriptCounter::ClearCounter()
{
	m_iSpeciesNum = 0;
	m_iSpeciesType =0;
	m_iSpeciesMuch =0;
	m_iCounter	= 0;
	m_bStart	= false;
	m_ID =0;
	m_szName[0] ='\0';
}

void CScriptCounter::SendSCRIPT_COUNTER( int iType)
{
	t_packet packet;
	packet.h.header.type	= iType;
	packet.h.header.size	= sizeof( t_script_counter);
	packet.u.script_counter.iNum = m_iSpeciesNum;	
	packet.u.script_counter.iType = m_iSpeciesType;	
	packet.u.script_counter.iMuch = m_iSpeciesMuch;	
	packet.u.script_counter.iCounter = m_iCounter;	

	QueuePacket( connections, m_ID, &packet, 1 );
}

void CScriptCounter::RecvSCRIPT_COUNTER()
{
}

bool CScriptCounter::StartCounter()
{
	if( m_bStart == true ) return false;
	
	SendSCRIPT_COUNTER( CMD_SCRIPT_COUNTER_START);
	m_bStart = true;
	m_iCounter =0;
	return true;
}

void CScriptCounter::RunCounter( int iNum )
{
	if( iNum != GetSpeciesNum() ) return;

	AddCounter();
	if( GetCounter() == GetSpeciesMuch() )
	{
		//카운터를 돌다 값이 지나면 멈추는데 지금은 멈추지 안케 만듬
		//EndCounter();	//에서 // 021129

	}
	
}

void CScriptCounter::AddCounter()
{
	m_iCounter++;
	SendSCRIPT_COUNTER( CMD_SCRIPT_COUNTER_SYNC );
}

void CScriptCounter::EndCounter()
{
	SendSCRIPT_COUNTER( CMD_SCRIPT_COUNTER_END );
	//ClearCounter();	//안멈추게함.
}

//CScriptCounter
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CRequital

int CRequital::LoadRequitalTable()
{
	char szQuery[0xff];
	HSTMT hStmt= NULL ;
	RETCODE ret ;
	SDWORD cbValue ;	
    SQLAllocStmt(hDBC, &hStmt);
	
//	strcpy( szQuery, "select * from requital_list order by npc_index" );	// BBD 040318
	sprintf_s( szQuery, sizeof(szQuery), "select * from requital_list where map_num = %d  order by npc_index ", MapNumber );	// BBD 040318

	ret= SQLExecDirect(hStmt, (UCHAR *)szQuery, SQL_NTS) ;
	
	if(ret == SQL_SUCCESS_WITH_INFO || ret == SQL_SUCCESS)
	{
		ret = SQLFetch( hStmt );
		while( ret == SQL_SUCCESS )
		{
			table_requital_list table;
			//Map_num,Npc_index,Quest_no, Quest_step,Item_no, Item_rare_type, ,Item_rare_count, Real_Fame,Dual_Fame,	Tile_area;
			ret = SQLGetData( hStmt, 1, SQL_C_LONG, &table.Map_num, 0, &cbValue );
			ret = SQLGetData( hStmt, 2, SQL_C_LONG, &table.Npc_index, 0, &cbValue );
			ret = SQLGetData( hStmt, 3, SQL_C_LONG, &table.Quest_no, 0, &cbValue );
			ret = SQLGetData( hStmt, 4, SQL_C_LONG, &table.Quest_step, 0, &cbValue );
			ret = SQLGetData( hStmt, 5, SQL_C_LONG, &table.Item_no, 0, &cbValue );
			ret = SQLGetData( hStmt, 6, SQL_C_LONG, &table.Item_rare_type, 0, &cbValue );
			ret = SQLGetData( hStmt, 7, SQL_C_LONG, &table.Item_Max, 0, &cbValue );
			ret = SQLGetData( hStmt, 8, SQL_C_LONG, &table.Item_Min, 0, &cbValue );
			ret = SQLGetData( hStmt, 9, SQL_C_LONG, &table.Item_rare_count, 0, &cbValue );
			ret = SQLGetData( hStmt, 10,SQL_C_LONG, &table.Real_Fame, 0, &cbValue );
			ret = SQLGetData( hStmt, 11,SQL_C_LONG, &table.Dual_Fame, 0, &cbValue );
			ret = SQLGetData( hStmt, 12,SQL_C_LONG, &table.Tile_area, 0, &cbValue );
			ret = SQLGetData( hStmt, 13,SQL_C_LONG, &table.WarStatus, 0, &cbValue );	// BBD 040318
			ret = SQLGetData( hStmt, 14,SQL_C_LONG, &table.Sardonyx, 0, &cbValue );			// BBD 040329
			ret = SQLGetData( hStmt, 15,SQL_C_LONG, &table.LeafOfBlessed, 0, &cbValue );	// BBD 040329
			ret = SQLGetData( hStmt, 16,SQL_C_LONG, &table.ref_index, 0, &cbValue );		// BBD 040329
			if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) 
			{
				MyLog( LOG_FATAL, "Table : requital_list : Error!!! (%d)", ret) ;
				SQLFreeStmt(hStmt, SQL_DROP);	// BBD 040329
				return -1;
			}
			SetRequitalTable( table );

			ret = SQLFetch( hStmt );
		}
		SQLFreeStmt(hStmt, SQL_DROP);	// BBD 040329
		return 1;
	}
	SQLFreeStmt(hStmt, SQL_DROP);	// BBD 040329
	return 0;
}

list<table_requital_list >::iterator CRequital::GetRequital_KillNpc( const int index )			//npc를 죽였을때 주는 해택의 종류
{
	REQUITALLISTITER itFind = find_if(m_tRequital.begin(), m_tRequital.end(), bind2nd( IsNpcIndexHere(), index ) );
	if( itFind != m_tRequital.end() )
	{
		return itFind;	
	}

	return m_tRequital.end();
}

//<! BBD 040329
list<table_requital_Item >::iterator CRequital::GetRequital_ItemIttr( const int index)
{
	REQUITALITEMITER itFind = find_if(m_tRequitalItem.begin(), m_tRequitalItem.end(), bind2nd( IsItemIndex(), index ) );
	if( itFind != m_tRequitalItem.end() )
	{
		return itFind;	
	}

	return m_tRequitalItem.end();
}
//> BBD 040329
//CRequital
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int InitLoadQuestTable()		//퀘스트에 관련된 테이블의 로드
{
	g_QuestInMap.IniteAllTable();
	if( 1 != g_QuestInMap.LoadQuestInMap() ) return -1;
	if( 1 != g_QuestInMap.LoadQuestInfoByStep() ) return -2;
	//보스죽여 해택주는 테이블 로드
	if( 1 != g_QuestInMap.LoadRequitalTable() ) return -3;	// 021105 kyo
	if( 1 != g_QuestInMap.LoadRequitalItemTable() ) return -4;	// BBD 040329

	return 1;
}

//<! BBD 040329
int CRequital::LoadRequitalItemTable()
{
	char szQuery[0xff];
	HSTMT hStmt= NULL ;
	RETCODE ret ;
	SDWORD cbValue ;	
    SQLAllocStmt(hDBC, &hStmt);
	
	strcpy_s( szQuery, sizeof(szQuery), "select * from requital_item order by [index]" );


	ret= SQLExecDirect(hStmt, (UCHAR *)szQuery, SQL_NTS) ;
	
	if(ret == SQL_SUCCESS_WITH_INFO || ret == SQL_SUCCESS)
	{
		ret = SQLFetch( hStmt );
		while( ret == SQL_SUCCESS )
		{
			table_requital_Item table;
			//Map_num,Npc_index,Quest_no, Quest_step,Item_no, Item_rare_type, ,Item_rare_count, Real_Fame,Dual_Fame,	Tile_area;
			ret = SQLGetData( hStmt, 1, SQL_C_LONG, &table.index, 0, &cbValue );
			ret = SQLGetData( hStmt, 2, SQL_C_LONG, &table.count, 0, &cbValue );
			ret = SQLGetData( hStmt, 3, SQL_C_LONG, &table.rate, 0, &cbValue );
			ret = SQLGetData( hStmt, 4, SQL_C_LONG, &table.item_no[0], 0, &cbValue );
			ret = SQLGetData( hStmt, 5, SQL_C_LONG, &table.item_no[1], 0, &cbValue );
			ret = SQLGetData( hStmt, 6, SQL_C_LONG, &table.item_no[2], 0, &cbValue );
			ret = SQLGetData( hStmt, 7, SQL_C_LONG, &table.item_no[3], 0, &cbValue );
			ret = SQLGetData( hStmt, 8, SQL_C_LONG, &table.item_no[4], 0, &cbValue );
			ret = SQLGetData( hStmt, 9, SQL_C_LONG, &table.grade, 0, &cbValue );
			ret = SQLGetData( hStmt, 10,SQL_C_LONG, &table.kind1, 0, &cbValue );
			ret = SQLGetData( hStmt, 11,SQL_C_LONG, &table.kind2, 0, &cbValue );
			ret = SQLGetData( hStmt, 12,SQL_C_LONG, &table.kind3, 0, &cbValue );
			ret = SQLGetData( hStmt, 13,SQL_C_LONG, &table.IsDynamic, 0, &cbValue );
			ret = SQLGetData( hStmt, 14,SQL_C_LONG, &table.HighRare, 0, &cbValue );
			if(ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS) 
			{
				MyLog( LOG_FATAL, "Table : requital_list : Error!!! (%d)", ret) ;
				SQLFreeStmt(hStmt, SQL_DROP);
				return -1;
			}
			SetRequitalItemTable( table );

			ret = SQLFetch( hStmt );
		}
		SQLFreeStmt(hStmt, SQL_DROP);
		return 1;
	}
	SQLFreeStmt(hStmt, SQL_DROP);
	return 0;
}
//> BBD 040329

//<! BBD 040329	// 결계석에 관한 보상함수 
bool CQuestInMap::Requital_About_SealStone( const int cn, int nSardAmount, int nLeafAmount, int nRef_index)
{
	CHARLIST *ch = CheckServerId( cn);
	if( !ch )		return false;

	int a,b,c;
//	RareMain RareAttr;
	ItemAttr  Item;
	POS pos;
	//<! BBD 040401
	int rate = rand()%10;
	switch(rate)
	{
	// 사드 주기////////////////////////////////////////////////////////////////////////
	case 0:
	case 1:
	case 2:
		{
			for(int i = 0; i < nSardAmount; i++)		// 사드를주자
			{
				Item = ::GenerateItem(SARD_ID);	// BBD 040331
				if( SearchInv( ch->inv, a, b, c ))
				{
					ch->inv[a][b][c] = Item;
					SetItemPos( INV, a, b, c, &pos );
					::SendItemEventLog( &Item, ch->GetServerID(), 0, SILT_MAKE_BY_DB, 100 );
					SendServerEachItem(  &pos, &Item, cn);	// 이넘이 DB데몬으로 아템로그 남기라고 한다
/*					Send_RareItemMakeLog(	cn,	Item.item_no,
							0,0,0,0,0,
							0,0,0,0,
							Item.attr[IATTR_LIMIT],
							Item.attr[IATTR_MUCH],
							0,3000,MapInfo[MapNumber].port,
							0,0,0,0,
							3000,0
							);				*/
				}
				else	// 인벤에 자리없음
				{
					return false;
				}
			}
			break;
		}
	// 축잎 주기////////////////////////////////////////////////////////////////////////
	case 3:
	case 4:
	case 5:
		{
			for(int i = 0; i < nLeafAmount; i++)		// 축복의 잎사귀를주자
			{
				Item = ::GenerateItem(LEAFBLESS_ID);	// BBD 040331
				if( SearchInv( ch->inv, a, b, c ))
				{
					ch->inv[a][b][c] = Item;
					SetItemPos( INV, a, b, c, &pos );
					::SendItemEventLog( &Item, ch->GetServerID(), 0, SILT_MAKE_BY_DB, 100 );
					SendServerEachItem(  &pos, &Item, cn);	// 이넘이 DB데몬으로 아템로그 남기라고 한다
/*					Send_RareItemMakeLog(	cn,	Item.item_no,
							0,0,0,0,0,
							0,0,0,0,
							Item.attr[IATTR_LIMIT],
							Item.attr[IATTR_MUCH],
							0,3000,MapInfo[MapNumber].port,
							0,0,0,0,
							3000,0
							);				*/
				}
				else	// 인벤에 자리없음
				{
					return false;
				}
			}
			break;
		}
	// 아이템 주기////////////////////////////////////////////////////////////////////////
	case 6:
	case 7:
	case 8:
	case 9:
		{
			if(nRef_index)	// 참조인덱스가 있으면 아템을 만들어주자
			{
				if(m_cRequital)
				{
					REQUITALITEMITER it = m_cRequital->GetRequital_ItemIttr(nRef_index);
					if(it != m_cRequital->m_tRequitalItem.end())	// 해당 아이템의 인덱스를 찾았냐?
					{
						int rate = rand()%100;
						if(rate < (*it).rate) // 확률내로 들어왔냐?
						{
							if(!SearchInv( ch->inv, a, b, c ))	// 인벤에 자리없음					{
							{
								return false;
							}
							// 해당된 놈이 몇개의 아이템을 보유했는지 확인
							int amount = 0;
							for(int i = 0; i < 5; i++)
							{
								if((*it).item_no[i])
								{
									amount++;
								}
							}

							if(!amount)	// 헉~ 한개도 없다
							{
								return true;
							}

							int rate2 = rand()%amount;	// 있는갯수 중(item_no1 ~ 5) 한놈을 골라준다
							// 아이템을 만든다
							ItemAttr item = ::GenerateItem((*it).item_no[rate2]);	// BBD 040331
							ItemMgr.MakeRareAttr(item.attr[IATTR_RARE_MAIN], (*it).grade, (*it).kind1, (*it).kind2, (*it).kind3,
								(*it).IsDynamic, (*it).HighRare);

							ch->inv[a][b][c] = item;	// 아이템을 넣어준다.
							SetItemPos( INV, a, b, c, &pos );
							::SendItemEventLog( &item, ch->GetServerID(), 0, SILT_MAKE_BY_DB, 100 );
							SendServerEachItem(  &pos, &item, cn);	// 이넘이 DB데몬으로 아템로그 남기라고 한다
						}
					}
				}

			}
			break;
		}
	default:
		{
			break;
		}
	}
	//> BBD 040401
	return true;
}
//> BBD 040329	// 결계석에 관한 보상함수

//<! BBD 040401	 캐릭터의 국가에 따라 아이템 보상여부를 판단
int DecideRequitalByNationDefensePoint(CHARLIST *ch)
{
	int NationIndex = 0;

	switch(ch->name_status.nation)	// 국가 확인
	{
	case N_VYSEUS:
		{
			NationIndex = 0;
			break;
		}
	case N_ZYPERN:
		{
			NationIndex = 1;
			break;
		}
	case N_YLLSE:
		{
			NationIndex = 2;
			break;
		}

	default:
		{
			return -1;		// 헉 넌 어느나라 놈이냐?
		}
	}

	int DefensePoint = g_DefensePoint[NationIndex];
	int DefIndex = 0;
	// 배열의 해당 인덱스를 찾음
	if(DefensePoint >= 100)
	{
		DefIndex = 0;
	}
	else if(DefensePoint >= 80 && DefensePoint < 100)
	{
		DefIndex = 1;
	}
	else if(DefensePoint >= 70 && DefensePoint < 80)
	{
		DefIndex = 2;
	}
	else if(DefensePoint >= 60 && DefensePoint < 70)
	{
		DefIndex = 3;
	}
	else if(DefensePoint >= 50 && DefensePoint < 60)
	{
		DefIndex = 4;
	}
	else if(DefensePoint >= 40 && DefensePoint < 50)
	{
		DefIndex = 5;
	}
	else if(DefensePoint >= 30 && DefensePoint < 40)
	{
		DefIndex = 6;
	}
	else
	{
		DefIndex = 7;
	}

	int RequitalRate = g_RequitalItemRate[DefIndex];

	int rate = rand()%100;

	if(rate < RequitalRate)
	{
		return 1;	// 확률내로 들어왔다.
	}
	else
	{
		return 0;	// 확률내로 않들어왔다.
	}
}
//> BBD 040401	 캐릭터의 국가에 따라 아이템 보상여부를 판단


void GiveQuestItemToPlayer( CHARLIST *pCaster, int k) //Eleval - Give the Quest Item to the player
{
	if(NpcQuestItem.empty()) return;
	int item_no = NpcQuestItem[k].item_id;
	for( int a=0; a<3; a++ )
		for( int b=0; b<3; b++ )
			for( int c=0; c<8; c++ )
				if( pCaster->inv[a][b][c].item_no == 0 ) 
				{
					SendItemEventLog( &pCaster->inv[a][b][c], pCaster->GetServerID(), SN_NOT_USER, SILT_DELETE_BY_SCRIPT, 2 );//넣기전.

					ItemAttr target = GenerateItem( item_no, IATTR_MUCH,1);//020509 lsw
					
					pCaster->inv[a][b][c] = target;
					if(target.item_no)
					{
						SendItemEventLog( &pCaster->inv[a][b][c], EventPC, SN_NOT_USER, SILT_MAKE_BY_SCRIPT, 2 );//넣은후
					}
					
					POS pos;
					SetItemPos( INV, a, b, c, &pos );
					SendServerEachItem(  &pos, &target, pCaster->GetServerID() );		// 001028 YGI

					NpcQuestItem[k].amount--;
					if(NpcQuestItem[k].amount <= 0)
					{
						NpcQuestItem[k].amount = 0;
						NpcQuestItem[k].bound = 0;
						NpcQuestItem[k].item_id = 0;
						NpcQuestItem[k].npc_id = 0;
						NpcQuestItem[k].player_name[0] = '\0';

					}
					
					return;
				}

}

void CheckIfQuestItem( CHARLIST *pCaster, CHARLIST *pTarget) //Eleval - Quest Item Check
{
	int i = 0;
	int k = -1;

	if(pCaster == NULL) return;
	if(pTarget == NULL) return;

	if(!NpcQuestItem.empty())
	{
		for(i = 0 ; i < NpcQuestItem.size() ; i++)
		{
			if(NpcQuestItem[i].item_id > 0)
			{
				if(!stricmp(NpcQuestItem[i].player_name, pCaster->Name) && NpcQuestItem[i].npc_id == pTarget->npc_index)
				{
					k = i;
					break;
				}
			}
		}
	}

	if(k >= 0 && pCaster != NULL)
	{
		GiveQuestItemToPlayer(pCaster, k);
	}
}