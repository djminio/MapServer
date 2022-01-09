#ifndef	_LTS_SUPPORT_H_
#define	_LTS_SUPPORT_H_


#define	LOCALWAR_NATION_MAX			3			// 0 : BY, 1 : ZY, 3 : YL
#define	ACTION_SITDOWN				17			// 011214 LTS
#define LOCALWAR_SEALSTONE_MAX		4			// LTS NEW LOCALWAR

enum eLF_TYPE//enum Log Fame
{
	LF_DUAL						=0,			// 듀얼소비
	LF_LOCALWAR					=1,			// 국지전
	LF_ITEMBUY					=2,			// 국가고급아이템
	LF_DONATION					=3,			// 국가기부
	LF_NATIONWAR				=4,			// 국가전
	LF_SCRIPT					=5			// 스크립트
};

#include <list>						// LTS DRAGONLORD
using namespace std;

struct LocalWarfield_Info			// LTS NEW LOCALWAR	
{
	int				Index;			// 배열에 들어갈 위치
	int				Phase;			// 맵에서 맵으로의 위치 
	char			MapName[NM_LENGTH];
	int				Port;
	int				SealNo[3][4];		
};

struct SealStoneStatus
{
	char		SprNo;
	short int	ID;
	char		MapNumber;
	short int	X, Y;
	char		Status;
};

class cLocalWarfield
{
	LocalWarfield_Info	m_tLocalWarfieldInfo;

	short				m_ManCount[LOCALWAR_NATION_MAX];
//	short				m_LocalWarPoint[LOCALWAR_NATION_MAX];
	int					m_bisLocalWarActive;

	int					m_SealStoneNo[NW_NATION_COUNT][LOCALWAR_SEALSTONE_MAX];			// LTS NEW LOCALWAR
	CHARLIST*			m_SealStonePtr[NW_NATION_COUNT][LOCALWAR_SEALSTONE_MAX];		// LTS NEW LOCALWAR


public :

	cLocalWarfield();
	~cLocalWarfield() {}

	LocalWarfield_Info	GetLocalWarfieldInfo() { return m_tLocalWarfieldInfo; }
	void				SetLocalWarfieldInfo(LocalWarfield_Info LF);
	int					GetLocalWarfieldPhase() { return m_tLocalWarfieldInfo.Phase; }
	int					GetLocalWarfieldIndex()	{ return m_tLocalWarfieldInfo.Index; }

	int				ConvertNation2Index(int Nation);
	short			GetManCount(int NationIndex) { return m_ManCount[NationIndex]; }
	void			IncManCount(int NationIndex) { m_ManCount[NationIndex]++; }
	void			DecManCount(int NationIndex) { m_ManCount[NationIndex]--; }
	void			ClearManCount();

/*	short			GetPoint(int NationIndex) { return m_LocalWarPoint[NationIndex]; }
	void			IncPoint(int NationIndex,int Value) { m_LocalWarPoint[NationIndex]+=Value; }
	void			ClearPoint();*/

	int				GetLocalWarStatus() { return m_bisLocalWarActive; }
	void			SetLocalWarStatus(int Status) { m_bisLocalWarActive=Status; }

	int				CheckAllSealStonebroked(int NationIndex);						// LTS NEW LOCALWAR
	void			CheckSealStonePtr(CHARLIST* ch);								// LTS NEW LOCALWAR
	void			ClearSealStonePtr();											// LTS NEW LOCALWAR
	CHARLIST*		GetSealStonePtr( const int nNation, const int nStoneNum );		// 030506 kyo //해당 결계석의 포인터를 리턴한다.
	void			RecoveryAllSealStone(); // 030520 kyo
};


struct EventMapData					// 020115 LTS				// LTS BUG
{
	char			EventActive;			// 이벤트 루프의 진행 해제
	char			EventStartTime;			// 이벤트 루프의 진행 시작시간(매일)
	char			EventRegTime;
	char			EventMapMoveTime;
	char			EventTime;
	char			EventDelayTime;			// 이벤트 사이의 휴식시간
	unsigned short	EventUserLevelMin[3];	// 이벤트 1,2,3의 조인레벨 MIN
	unsigned short	EventUserLevelMax[3];	// 이벤트 1,2,3의 조인레벨 MAX
	POINTS			StartPoint[6];		// 나라별 2개씩								// LTS BUG
	unsigned short	EventRegMax[3][3];		// 나라별, 이벤트 번호별 참여인원 제한
	POINTS			OutPoint[3];			// 나라별 하나씩
	char			EventMessageTime;		//이벤트 시작시 메세지 전송 주기
	POINTS			EventJoinLocation[3];	// 마을별 이벤트 접수대 x,y
};


#define				EVENT_NOTHING		0
#define				EVENT_JOIN			1
#define				EVENT_MOVE			2
#define				EVENT_DOING			3
#define				EVENT_REST			4

#define				EVENT_1				0
#define				EVENT_2				1
#define				EVENT_3				2

class cEventLocalWarfield			// 020115 LTS
{
	int				m_EventStep;			// 1번이벤트, 2번이벤트, 3번이벤트 
	int				m_EventStatus;				// 0 : 사용안함 // 1 :참여 2 : 이동 // 3 : 시작 //4 :휴식
	__int64			m_EventEndTime;			// TimeUpdate 시간 
	EventMapData	m_tData;					// 이벤트 맵데이타
	int				m_CheckLoadData;			// 데이터 로딩 이상없나 체크
	WORD			m_ManCount[3];				
public :
	cEventLocalWarfield();
	~cEventLocalWarfield();

	void			InitEventLocalWarfield();
	void			LoadEventLocalWarfieldData();
	inline int		CheckLoadData()				{ return m_CheckLoadData; }
	void			InitEvent();

	void			ClearUserCloth();
	void			SetEventLocalWarStatus(int Status);
	void			UpdateEventLocalWarTime();

	void			EventLocalWarMessage();
	void			SendEvent1Message(int Status);
	void			SendEvent2Message(int Status);
	void			SendEvent3Message(int Status);
	void			SendEventDoingMessage();

	void			ClearUserALL();
	void			LogJoinUser();
	void			LogVictoryUser();
	int				GetNationIndex(int Nation);
	inline int		GetEventStatus()			{ return m_EventStatus; }		

	inline bool		CheckLevel(int Level);
	inline bool		IncManCount(int Nation);
	inline void		ClearManCount()				{ m_ManCount[0]=0;m_ManCount[1]=0;m_ManCount[2]=0; }
};

typedef list<int>					MemberList;
typedef MemberList::iterator		MemberListItor;	

#define BLUETEAM				0
#define REDTEAM					1

#define NONE_ARMOR				0
#define	BLUE_ARMOR				1
#define RED_ARMOR				2

#define	WAR_NOT_END				-2
#define WAR_RESULT_LOOSE_ALL	-1
#define	WAR_RESULT_BLUE_WIN		0
#define WAR_RESULT_RED_WIN		1

class cDragonLordWar													// LTS DRAGONLORD
{
	__int64			m_64Timer;
	DWORD			MessageTime;

	bool			m_bisActive;
	char			m_cResult;

	int				TeamCount[2];

	MemberList		TeamMemberList[2];

	POINT			KickPoint[3];

	void			(*DeActiveFunc)();
	void			(*MessageFunc)();
	
	void			SetTimer(DWORD	WarTime);
	void			SetMessageTime(DWORD MsgTime);
	void			PushMember(int Type,int ID);
	void			ClearMemberList();
	void			ClearMemberCount();
	void			SetReturnFunc(void(*pFunc)());
	void			SetMessageFunc(void(*pMsgFunc)());
	void			CheckMemberCount();
	void			SetKickPoint(POINT* kPoint);
	void			SetWarResult(char Result);
	int				GetTeamMemberCount(int Type);
	void			PutArmor();
	void			RemoveArmor();
	void			KickTeamAll();
	void			KickTeam(int Type);
	void			KickUser(int ID);
	void			SendTeamChange(int ID,int TeamNo);
	void			CheckRemainTime();

public :

	cDragonLordWar();
	~cDragonLordWar();

	DWORD			GetRemainTime();
	void			ActiveWar(DWORD WarTime,void(*pFunc)(),DWORD MessageTime,void(*pMsgFunc)(),POINT* kPoint,int RedTeamCount,int* RedTeam,int BlueTeamCount,int* BlueTeam);
	void			DeActiveWar();
	void			SetTeamList(int Type,int Count,int* Member);	// Type RED:0,BLUE:1
	void			FindAndDeleteMember(int ID,bool Kick=true);
	void			UpdateWarTimer();
	bool			GetWarStatus();
	int				ReturnWinMemberCount();
	int				GetWarResult();
	void			ReturnWinMemerList(int* Member);
};

extern cDragonLordWar		g_pDragonLordWar;					// LTS DRAGONLORD
extern void SaveChangeClassStep(CHARLIST* pTarget, int nPrevStep, int nNextStep); // CSD-TW-030606
extern void SaveChangeClassQuest(CHARLIST* pTarget, const char* pMsg, int nQuestNo, int nQcount, int nStart); // CSD-TW-030618
extern void SaveChangeFame( CHARLIST *ch, int old_fame, int new_fame, eLF_TYPE type );
extern void SaveScriptLog( CHARLIST *ch, const int nQuestNo, const int nQuestStep, const char* msg); // 030826 kyo
#endif 