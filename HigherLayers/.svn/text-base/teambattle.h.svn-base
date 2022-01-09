#include "defaultheader.h"
#include "Citem.h"
#ifndef __teambattle_h__
#define __teambattle_h__

extern void Script( int num );
extern void CalcWinLoseScore( WINLOSEPOINT &win_lose, int wintype );
extern void SendFightMapChat( short int cn, t_packet *packet );		// 전체에게 보낸다.
extern void SendFightMapChatMyTeam( short int cn, t_packet *packet );
extern void SendGamblingResult(CHARLIST* pGambler, int nPayment, DWORD dwMoney);

//////////////////////////////////////////////////////////////////
// 겜블 아이템
struct k_game_item
{
	int count;
	int *lpItem;

	k_game_item()
	{
		count = 0;
		lpItem = 0;
	}
	~k_game_item() { SAFE_DELETE_ARRAY(lpItem); }
};

class CGambleItem
{
private:
	k_game_item m_GambleItem[MAX_GAMBLE_ITEM_KIND][MAX_GAMBLE_ITEM_TYPE]; // CSD-030520

public :
	CGambleItem()
	{
	}

	~CGambleItem()
	{	//< CSD-030517
		for (int i = 0; i < MAX_GAMBLE_ITEM_KIND; ++i)
		{	
			for (int j = 0; j < MAX_GAMBLE_ITEM_TYPE; ++j)
			{
				Release(i, j);
			}
		}		
	}	//> CSD-030517

public:
	int SetSize( int house, int type, int count );
	int Release( int houes, int type );

public:
	void SetItem( int house, int type, int pos, int item_no )
	{
		m_GambleItem[house][type].lpItem[pos] = item_no;
	}

	int GetItem(int house, int type, int pos)
	{	//< CSD-040223
		return m_GambleItem[house][type].lpItem[pos];
	}	//> CSD-040223
	
	int Rand(int house, int type)
	{	//< CSD-040223
		if (m_GambleItem[house][type].count)
		{
			return rand()%m_GambleItem[house][type].count;
		}

		return -1;
	}	//> CSD-040223

	int GetRandItem(int house, int type)
	{	//< CSD-040223
		const int nRandom = Rand(house, type);
		return (nRandom == -1) ? -1:GetItem(house, type, nRandom);
	}	//> CSD-040223
};
extern CGambleItem g_GambleItem;
//////////////////////////////////////////////////////////////////
// >> 011019 ygi
class CTodayGambleItem
{
public :
	ItemAttr m_TodayItem;
public :
	CTodayGambleItem()
	{
		DeleteTodyaItem();
	}
	int DeleteTodyaItem()
	{
		memset( &m_TodayItem, 0, sizeof( ItemAttr ) );
		return 1;
	}
	ItemAttr GetTodayItem()
	{
		return m_TodayItem;
	}
	int SetTodayItem( ItemAttr *item )
	{
		m_TodayItem = *item;
		return 1;
	}
	int CreateTodayItem();
};

extern CTodayGambleItem g_TodayGambleItem;
extern int RefreshTodayItem();
extern k_nation_item_data g_nation_item;	// 국가 고급 아이템

// 그 스톤을 어느팀이 차지 했는가?

const int  STONE_SCORE_FIRST=	IA2_NOTMOVE;		// 1: 초기 일때 
const int  STONE_SCORE_2	=	IA2_OPENED;		//	2 자리
const int  STONE_SCORE_1	=	IA2_BROKEN;		// 첫째자리

class CColossusStone
{
public :
	ITEMLIST * stone;
	CColossusStone *next;
public : 
	CColossusStone() { next = 0; }
	~CColossusStone() { SAFE_DELETE(next); }
};

class CColossusStoneMgr
{
public :
	CColossusStone	m_StoneList;
	DWORD m_GotStoneTime;		// 돌을 전부 차지한 시간
	int m_GotStoneTeam;			// 돌을 전부 차지한 팀
	int m_MaxStone;


public:
	CColossusStoneMgr()
	{
		m_MaxStone = 0;
		SetWinCondition();
	}
	int SetWinCondition( DWORD time = 0, int team = 0 )
	{
		if( !time || !m_GotStoneTime || team != m_GotStoneTime )
		{
			m_GotStoneTime = time;
			m_GotStoneTeam = team;
		}
		return 1;
	}
	bool IsEnd()
	{
		if( !m_GotStoneTime ) return false;
		if( g_curr_time - m_GotStoneTime > 30 ) return true;
		else return false;
	}
	~CColossusStoneMgr()
	{
	}

	int AddStone( int id );
	ITEMLIST *GetList( int count );
	void GetTeamCount( int team[4] );
	void SetStoneByTeam( int id, int team );		// 한팀이 스톤을 건드렸을때;
	void SetStoneFirst();		// 초기 세팅	
	int GetMaxStone();
};

extern void SetStoneByTeam( ITEMLIST *item, int team );
extern int GetTeamByStone( DWORD attr );
extern CColossusStoneMgr g_ColossusStone;
extern int GetFightTeamNumber( CHARLIST *ch );		// 0, 1, 2, 3


class CGuildHouseUnit
{
public :
	int id;
	int level;				// 등급
	int nation;				// 존재하는 위치 국가
	int blue;				// 구매에 필요한 파란 시그넷 수
	int red;				// 구매에 필요한 빨간 시그넷 수
	char map_name[20];		// 위치 맵
	int x;					// 위치 좌표 ( 입구 좌표 )
	int y;
	int guild_code;			// 소유 길드
	int year;
	int month;
	int day;
	int	event_no;		// 해당 맵에 찍어 놓은 발판 번호

public : 
	CGuildHouseUnit() { clear(); }
	int GoToHouse( short int cn );		// 다른 맵이면 맵이동을 시작하고 같은 맵일 경우엔 맵이동 루틴을 실행시킨다.
	void clear() { id = 0; }
	
};

//#define MAX_GUILD_HOUSE	100
class CGuildHouse
{
public :
	int m_Max;
	CGuildHouseUnit	m_aGuildHouse[MAX_GUILD_HOUSE];
public :
	CGuildHouse() { m_Max = 0; }
	CGuildHouseUnit *GetPointByEventNo( char *map_name, int event_no );	// 해당 맵과 이벤트 번호로 엡포리아 클레스에 접근
	int GoToHouse( CHARLIST *ch );	// 해당 엠포리아로 이동
	CGuildHouseUnit *GetPointByGuildCode( int guild_code );		// 길드 코드로 엠포리아 클레스에 접근
	int IsMyGuildHouse( CHARLIST *ch, int event_no );
	int SetMax( int max ) { if( max >= m_Max ) m_Max = max+1; return 1; }
	int clear();
	CGuildHouseUnit	*GetHouseByGuildCode( int guild_code );
	CGuildHouseUnit	*GetHouseByIndex( int index )
	{ 
		return &m_aGuildHouse[index]; 
	}
};

extern CGuildHouse	g_GuildHouse;



//< soto-030324
#define		MAX_GUARDIAN_GUILD	3

// 지키미 길드. // 복장이 바뀌는 길드는 1위길드만 바뀐다.
class CGuardianGuild
{
private:
	int m_nGuildCount; // 현재 지키미 길드의 수를 나타낸다.
	int	m_n1stGuildCode; // 1위인 길드 코드를 알아낸다.
	vector<int>m_vtGuildList;
	
	// 안의코드를 검사하지 않고 길드 코드를 등록한다.(중복의 위험이 있다)
	int	ForcingAddGuild(int nGuildCode)
	{		
		m_vtGuildList.push_back(nGuildCode);
		return ++m_nGuildCount;
	}
public:
	CGuardianGuild() //생성자
	{ 
		m_vtGuildList.reserve(MAX_GUARDIAN_GUILD);
		m_nGuildCount = 0;
		m_n1stGuildCode = -1; 

	} // 생성자.
	
	bool IsGuardianGuild(int nGuildCode)// 이 길드코드가 지키미 길드의 코드이냐?
	{
		for(int i = 0;i < m_nGuildCount;i++)
		{
			if(m_vtGuildList[i] == nGuildCode) return true;
		}
		return false;
	}	
	
	int AddGuild(int nGuildCode)//중복되지 않게 추가 시킨다.
	{
		if(!IsGuardianGuild(nGuildCode))
		{
			m_nGuildCount = ForcingAddGuild(nGuildCode);
		}

		return m_nGuildCount;
	}
	
	int FirstGuild()//1등 길드코드를 알아낸다.// 없을때는 -1을 리턴.
	{
		if(!m_nGuildCount)return -1;

		return m_n1stGuildCode;
	}

	int GetCount()
	{
		if(m_vtGuildList.empty()){return 0;}
		const int nCount = m_vtGuildList.size();
		return __min(nCount,MAX_GUARDIAN);
	}

	int	GetGuildCode(int nIndex)
	{
		if(nIndex < m_nGuildCount)
			return m_vtGuildList[nIndex];
		return -1;
	}

	// 일등인 길드는 하나가 아니라.. 각국가별로 하나로 바뀌었다. 당근 3개지.
	bool	Is1StGuild(int nGuildCode){ return IsGuardianGuild(nGuildCode); }// soto-030324

	void Set1stGuild(int nGuildCode)//1등인 길드를 등록한다.
	{
		if(!m_nGuildCount)//현재 길드가 아무도 등록 되어 있지 않을때.
		{
			ForcingAddGuild(nGuildCode);
			m_n1stGuildCode = nGuildCode;
		}
		else // 하나 이상의 길드가 등록되어 있을때.
		{
			if(IsGuardianGuild(nGuildCode)) // 리스트에 등록된 코드인가?
			{
				m_n1stGuildCode = nGuildCode;
			}
			else
			{
				ForcingAddGuild(nGuildCode);
				m_n1stGuildCode = nGuildCode;
			}
		}
	}

	void ClearGuild()
	{ 
		m_vtGuildList.clear();
		m_n1stGuildCode = -1;
		m_nGuildCount = 0;
	}
};

extern CGuardianGuild	g_CGuardianGuild; // 지키미 길드 관리 전역 클래스. // soto
//> soto-030324
#endif
