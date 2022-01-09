
#ifndef		_QUEST_IN_MAP_
#define		_QUEST_IN_MAP_

#include <list>
#include <vector>
using namespace std;

enum SCRIPT
{
	SCRIPT_COUNTER_TYPE__NPCKILL = 0,
	SCRIPT_COUNTER_TYPE__ITEMCOUNT = 1,

	SCRIPT_COUNT_STOP = 0,
	SCRIPT_COUNT_START = 1,
	SCRIPT_COUNT_CHECK = 2,

	SCRIPT_END_SUC		=10,
	SCRIPT_END_FAL		=20,
};

class CRequital;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CScriptTimer	타이머를 사용한다 .

class CScriptTimer
{
	/*
	enum
	{
		MAX_TIMER_TIME	= 5 * 3600 * 1000,		//5시간이 제한이다.
		MIN_SYNCH_TIME	= 2 * 1000,				//2초이하다.
	};

  //도대체 동기화가 필요도 없는 함수지만 나중울 생각해서 동기화를 위한 2가지 함수를 세팅할수 있게 했다.
*/
	
public:
	CScriptTimer(int id, char * name, DWORD dwTime, DWORD dwSynch):m_ID(id), m_dwTimerTime(dwTime), m_dwSynchTime(dwSynch)
	{	//< CSD-030509
		strcpy_s( m_szName, sizeof(m_szName), name); 
		m_bStart = false;
		m_dwOld= 0;
		m_dwSynch=0;
		memset(m_szMapFile, 0, sizeof(m_szMapFile));
		m_nPosX = 0;
		m_nPosY = 0;
	}	//> CSD-030509

	CScriptTimer()
	{	//< CSD-030509
		m_ID=0; 
		m_dwTimerTime=0; 
		m_dwSynchTime=0; 
		m_dwSpendTime=0; 
		m_bStart=false; 
		m_dwOld=0; 
		m_dwSynch=0;
		memset(m_szMapFile, 0, sizeof(m_szMapFile));
		m_nPosX = 0;
		m_nPosY = 0;
	}	//> CSD-030509

	~CScriptTimer(){}

private:
	DWORD	m_dwTimerTime;		//할당된 타임
	DWORD	m_dwSpendTime;		//사용된 시간. 동기화를 위해
	bool	m_bStart;			//타이머가 시동되었는가?
	DWORD	m_dwSynchTime;		//얼마만의 시간마다 동기화 하는가? 아니면 이 시기마다 어떤 함수를 실행할것인가?

	void	(*m_func)();		//타이머 돌면서 호출하는 함수
	void	(*m_syncFunc)();	//타이머 도중에 호출하는 함수
	void	(*m_afterFunc)();	//타이머 끝나고 호출하는 함수	<-설정 안함 나중에 티어머 끈나면 이 함수를 호출하게 한다.

	void	AfterTimer();//char* szMap, int x, int y);	//타이머가 끝이 나면 바리 실행한다?

	int		m_ID;				//cn값을 저장한다.
	char	m_szName[31];			//만약을 위해 이름도 저장한다.
	char    m_szMapFile[30]; // CSD-030509
	int     m_nPosX;		 // CSD-030509
	int     m_nPosY;		 // CSD-030509

public:
	DWORD	m_dwOld;
	DWORD	m_dwSynch;

	void	SetID( const int iID){m_ID = iID;};						//타이머 id할당
	void	SetName( const char* name );//{ strcpy(m_szName, name);};
	void	SetTimerTime( DWORD dwTime);				//타이머의 시간을 설정한다. 
	void	SetSynchroneTime( DWORD dwSyncTime );		//동기화 하는 시간을 설정
	void	SetFunction( void(*func)(), void (*syncFunc)());
	void	SetAfterMap( const char* mapfile, const int& x, const int& y);	//타이머 끈나구 실핼될 함수
	void	ClearTimer();								//타이머 값들을 전부 0으로

	DWORD	GetCurretTimerTime() const { return m_dwTimerTime;};			//현재 남은 시간을 알아본다.
	DWORD	GetCurretSynchTime() const { return m_dwSynchTime;};		//설정한 동기화 시간을 알아본다.
	DWORD	GetSpendTime() const { return m_dwSpendTime;};
	int		GetConnectionID() const { return m_ID;};						//EventPc값을 넘긴다.
	char*	GetName() { return m_szName;};

	bool	IsRunning();				//진행중인가?
	bool	IsStart() const { return m_bStart;};

	void	SendSCRIPT_TIEMER( DWORD dwSpendTime , int iType);		//싱크륾 맞추기 위한 패킷을 보낸다.
	bool	ConfirmSyncTimer( DWORD dwTimer);						//싱크 맞출때 타이머가 적절하게 증가했는가?

	bool	StartTimer( bool bOnlyServer = false);				//시간이 설정되어 있으면 m_bStart = true한다. 매개변수가 true이면 서버만 돈다.
	void	RunTimer();					//계쏙해서 실행
	void	EndTimer( int iFlag);		//iflag ==0 성공, ==1 실패

	void	Log_UserInfo(const int nCn);	// 030617 kyo
};
//CScriptTimer
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CScriptCounter
class CScriptCounter
{
public:
	CScriptCounter(){ClearCounter();};
	~CScriptCounter(){};

private:
	int		m_iSpeciesNum;		//카운터할꺼 번호
	int		m_iSpeciesType;		//어떤걸 카운터할까?
	int		m_iSpeciesMuch;		//몇개를 카운터할까?
	bool	m_bStart;
	int		m_iCounter;			//현재 몇개인나?
	int		m_ID;
	char	m_szName[31];

public:
	void	SetSpeciesNum	( const int& iNum )		{ m_iSpeciesNum = iNum; };
	void	SetSpeciesType	( const int& iType )	{ m_iSpeciesType= iType; };
	void	SetSpeciesMuch	( const int& iMuch )	{ m_iSpeciesMuch= iMuch; };
	void	SetConnectionID ( const int& iID )		{ m_ID = iID;};
	void	SetName			( const char* szName)	{ strcpy_s( m_szName, sizeof(m_szName), szName); };
	void	SetCounter		( const int& iCounter)	{ m_iCounter = iCounter;};	

	int		GetSpeciesNum() const { return m_iSpeciesNum; };
	int		GetSpeciesType() const { return m_iSpeciesType; };
	int		GetSpeciesMuch() const { return m_iSpeciesMuch; };
	int		GetCounter() const { return m_iCounter; };
	int		GetConnectionID() const { return m_ID; };
	char*	GetName() { return m_szName; };

	bool	IsStart() const { return m_bStart; };
	void	ClearCounter();

	void	SendSCRIPT_COUNTER( int iType);
	void	RecvSCRIPT_COUNTER();

	bool	StartCounter();
	void	RunCounter( int iNum );		//설정치까지 
	void	EndCounter();		// 끈나따고 패킷만 보냄.. 카운터는 타이머가 죽을때, 스크립트로 죽일때 의 경우에만 멈춘다
	void	AddCounter();		//npc지기기에서 사용되는 거. 증가시마나 패킷보냄
};

//CScriptCounter
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CSpellMove
{
public:
	CSpellMove(){};
	CSpellMove(int id, string spells){ m_ID = id; m_szSpell = spells;};
	~CSpellMove(){};
	string		m_szSpell;
	int			m_ID;
	
	int			GetId() const {return m_ID;};
};

class CBossTimer
{

	typedef list<int>::iterator	ITOR_INT;
public:
	enum{
		TYPE_BOSS_ONLY = 0,
		TYPE_BOSS_GROUP = 1,
		TYPE_BOSS_OUT	=2,
	};

	CBossTimer();
	~CBossTimer();
	CScriptTimer m_cBossTimer;
	list<int>	m_lUserCn;
	int			m_nBossID,m_iBossSprNo;
	DWORD		m_dwBossTimer;
	int			m_nFlag;	// 보스를 한마리만 할껀지 단체로할껀지. 031029 kyo 
	
	bool AddUser(const int nCn);
	bool DeleteUser(const int nCn);
	int GetUserSize() const {return m_lUserCn.size();}
	void RemoveBossNpc();		//보스 죽임
	
};

class CQuestInMap
{
	typedef list<CScriptTimer>           TIMERLIST;
	typedef list<CScriptTimer>::iterator TIMERITER;

	typedef list<CScriptCounter>			COUNTERLIST;
	typedef list<CScriptCounter>::iterator	COUNTERITER;

	typedef list<table_quest_in_map>		TB_QUESTINMAP;
	typedef list<table_quest_info_bystep>	TB_QUESTINFO;

	typedef list<table_requital_list >	TB_REQUITALLIST;
	typedef list<table_requital_list >::iterator REQUITALLISTITER;
//<! BBD 040329
	typedef list<table_requital_Item >	TB_REQUITALITEM;
	typedef list<table_requital_Item >::iterator REQUITALITEMITER;
//> BBD 040329

	typedef list<CSpellMove>::iterator	SPELLITER;

	//bosstimer관련
	typedef map<int, CBossTimer*>				MAP_BOSSTIMER;
	typedef map<int, CBossTimer*>::iterator		ITOR_BOSS;
	typedef map<int, CBossTimer*>::value_type	PAIR_BOSS;

friend class CScriptTimer;
public:
	CQuestInMap();
	~CQuestInMap();

protected:
	//table
	TB_QUESTINMAP			m_tQuestInMap;
	TB_QUESTINFO			m_tQuestInfo;

public:
	
	// 021014 //주문입력으로 맵이동시 사용
	list<CSpellMove>	m_szClientSpell;				//유저가 친 주문은 SendChatData로 넘어와 ::CheckSpellMapMoveArea에서 저장된다.
	int			CheckSpell( const int cn, const char* szSpell);	//유저가 친 주문과 스크립트 주문과 비교한다.
	//SPELLITER	FindSpellUser( const int cn );				//Unused and bugged
	void		DeleteSpellUser( const int cn );			//유저 죽이기..
	string		GetSpellWord( const int cn );		//유저의 주문 찾기

	void	IniteAllTable(){ m_tQuestInMap.clear();m_tQuestInfo.clear();};		//모든 테이블 지우기

	int		LoadQuestInfoByStep();	//quest_info_byStep 테이블 load
	int		LoadQuestInMap();		//quest_in_map 테이블 load

	//SetTable
	void	SetQuestInMap(	table_quest_in_map &tp ){			m_tQuestInMap.push_back( tp );};
	void	SetQuestInfoByMap( table_quest_info_bystep &tp){	m_tQuestInfo.push_back( tp );};

	//정보를 가져온다. 
	list<int> GetQuestInMap( int iMapnumber);		//해당 맵에 있는 퀘스트를 넘겨준다.
	list<int> GetRoundUser( const int cn );			//해당 cn주위의 사람들을 리스트를 가져온다.ㄴ
	int		GetQuestInfo( int iQuestNum, int iStep );	//퀘스트번호랑 스텝값으로 정보를 알아낸다.
		
	//패킷을 보낸다
	void	SendQuestInfo(	t_quest_in_map *tp, int cn);		
	void	SendShowQuestInfo( t_quest_in_map *tp, int cn );

//script함수
	void	ShowStateMsg( const int& cn, const char* szMsg, const int& R, const int& G, const int& B );	//메시지를 지정색으로 출력한다.
	void	ShowStateMsg( const int& cn, const int& iMsgNum, const int& R, const int& G, const int& B );	//미사용

//BossTimerCheck
	MAP_BOSSTIMER m_mBossTimer;
	int		m_iBossEntree;	//누가 들어왔는지 알고 싶다면 나중에 list로 바꿔서 cn을 넣는다.
	DWORD	StartBossTimer( const int& iCn, const int& iBoss, const int& iX, const int& iY, const DWORD& dwTimer, const int iEntree, const int nFlag);	//boss타이머를 생성한다. 보스도 생성한다. 
	bool	DeleteBossTimer(int nKey);		//boss타이머를 죽인다. 
	void	DeleteBossNpc(int nKey);		//Boss를 강제로 죽인다.
	void	RunBossTimer();		//boss타이머를 돌린다. 
	
	DWORD	GetCurrentBossTimer(int nKey);	//현재 남아있는 시간이 리턴된다.
	int		GetBossTimerEntree(int nKey);	//현재 방에 들어가 있는 유저수가 리턴된다. 
	void	DecreaseBossTimerUser(const int nKey, const int nCn);	
	void	CheckBossTimerBoss( const int nKey);		//현재 보스방에 남은사람이 0이면 보스룰 죽이고 방을 닫는다. 

//Timer에 관련된 내용
	list<CScriptTimer> m_cTimer;
	void	RunQuestTimer();		//퀘스트,시나리오에 관련된 모든 타이머를 돌린다. 	
	bool	StartScriptTimer(const int iServerID, const char* szName, const DWORD &dwTimer, const DWORD &dwSync, const char* mapfile, const int& x, const int& y);		//스크립트 함수 초기화
	void	RunScriptTimer();												//very_long_term에서 맨날 도는 넘
	void	EndScriptTimer( const int iID );
	bool	DeleteScriptTimer( const int iServerID, const char *szName );	//해당 이름과 id인 타이머를 종료
	void	DeleteScriptTimer();											//타이머가 끝난 것을 종료 시킨다.
	bool	CheckID2Name( const int& iID, const char* szName );				//아뒤랑 이름이 현 리스트랑 일치하지 않는다.
	void	RevcScriptTimerPacket( t_script_timer *timer, int iType, int cn );				//script timer에 관련된 모든 패킷을 받는다.
	list<CScriptTimer>::iterator FindUserInScriptTimer( const int& iId, const char* name);	//해당 유저가 스크립트 타이머를 사용하고 있는가?

//Counter에 관련된 내용.
	list<CScriptCounter> m_cCounter;
	bool	StartScriptCounter( const int iServerID, const char* szName, const int iNum, const int iType, const int iMuch, const int iFlag);
	void	RunScriptCounter( const int cn, const char* szName, int iNum, int iType);
	void	EndScriptCounter( const int iServerId, const char* szName);
	bool	CheckScriptCounter( const int& iServerId, const char* szName, const int iNum, const int iType, const int iCounter );
	void	DeleteScriptCounter();											//카운터가 끝이 난것은...지울까??
	bool	DeleteScriptCounter( const int iServerId, const char* szName);
	void	AddScriptCounter(const int iServerId, const char* szName);		//해당 유저의 카운터를 증가하고 그 유저에게 패킷 보낸다.
	void	RevcScriptCounterSync( t_script_counter *counter,  int cn );	//030108 클라이언트 카운터의 숫자를 받는다.
	list<CScriptCounter>::iterator FindUserInScriptCounter( const int& iId, const char* name, const int iType);	//해당 유저가 해당타입의 카운터를 실행하고 있는가?
	
//Npc를 죽이며 받게 되는 보상 requital_list에 저장되어 있는 값을 준다. 
	CRequital *m_cRequital;	
	int		LoadRequitalTable();												//테이블 로드해서 값들 넣기
	void	DeleteRequeitalTable();												//삭제
	void	GiveRequitalAfterKillNpc( const CHARLIST *a, const CHARLIST *d);	//죽였다. 
	void	RequitalRoundUser( const int& cn, table_requital_list& table);		//죽인것 근처에들에게 해택준다.
	void	RequitalKillUser( int cn, table_requital_list &table );				//죽인넘만 해택준다.
	void	GiveRequitalByCn( const int cn , table_requital_list& table);		//cn에게 해택을 준다. 
	list<table_requital_list>::iterator FindRequitalNpc( const int iNpcIndex);	//해당 npc가 테이블에 존제하는가?

//spellmapmove 개정판
	void SendSpellMapMove( const int&cn, const char* szSpell, const char* szMap, const int&  x, const int& y);
	void SendSpellMapMove_Suc(const char* szMap, const int& cn);
	void SendSpellMapMove_Fail(const int& cn);
	void RecvSpellMapMove( const t_script_spellmapmove *spell, const int& cn );

//<! BBD 040329
	int		LoadRequitalItemTable();
	bool Requital_About_SealStone( const int cn, int nSardAmount, int nLeafAmount, int nRef_index);
//> BBD 040329
};


template <class T>
class CTable
{
private:
	list<T>		m_tTable;
	char		m_szTableName[32];
	char		m_szOrderType[32];
	//int			m_iFildeSize;
	vector<t_field_type>	m_tFieldType;
/*
#define	SQL_UNKNOWN_TYPE	0
#define SQL_CHAR            1
#define SQL_NUMERIC         2
#define SQL_DECIMAL         3
#define SQL_INTEGER         4
#define SQL_SMALLINT        5
#define SQL_FLOAT           6
#define SQL_REAL            7
#define SQL_DOUBLE          8
*/


public:
	CTable(){};
	~CTable(){};
	void	InitTable(const char* szTableName, const char* szOrder, const int iFieldNum);
	void	InsertFieldType(t_field_type type){	m_FieldType.push_back(type);	};
	//int		GetFieldType(){	m_FieldType.pop_front();};

	int	LoadTable();
	void	SetTable( const T t){ m_tTable.push_back(t); };
	T		GetTable() { return m_tTable.pop_back();};
};


class CRequital	//해택주기 클래스
{	
	typedef list<table_requital_list >	TB_REQUITALLIST;
	typedef list<table_requital_list >::iterator REQUITALLISTITER;
//<! BBD 040329
	typedef list<table_requital_Item >	TB_REQUITALITEM;
	typedef list<table_requital_Item >::iterator REQUITALITEMITER;
//> BBD 040329

public:
	CRequital(){};
	~CRequital(){};

	void	InitTable(){};
	int		LoadRequitalTable();
	void	SetRequitalTable( table_requital_list &table){ m_tRequital.push_back( table); };

	void	GiveRequital2User( const int cn, table_requital_list t);		//cn에게 해택을 준다. 

	//<! BBD 040329
	int		LoadRequitalItemTable();
	void	SetRequitalItemTable( table_requital_Item &table){ m_tRequitalItem.push_back( table); };
	REQUITALITEMITER GetRequital_ItemIttr( const int index);
	//> BBD 040329
	
	REQUITALLISTITER	GetRequital_KillNpc( const int index);			//npc를 죽였을때 주는 해택의 종류	

//protected:
	TB_REQUITALLIST			m_tRequital;
	TB_REQUITALITEM			m_tRequitalItem;	// BBD 040329
};

extern CQuestInMap g_QuestInMap;

#endif //_QUEST_IN_MAP_