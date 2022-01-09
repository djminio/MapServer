// ArenaDef.h: interface for the CArenaDef class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARENADEF_H__44CA5FE7_F9FF_4391_8EF1_49C44E9A5EA0__INCLUDED_)
#define AFX_ARENADEF_H__44CA5FE7_F9FF_4391_8EF1_49C44E9A5EA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////
//
static int s_aCheckTime[] = 
{ 
	600, 300, 60, 30, 0 
};
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
class CArenaInfo
{
public:
	static CArenaInfo* CreateArenaInfo()
	{
		return new CArenaInfo;
	}

private:
	CArenaInfo();

public:
	~CArenaInfo();
	
public:
	void SetMapPort(int nPort) 
	{ 
		m_nMapPort = nPort; 
	}

	void SetWaitPosition(int nX, int nY) 
	{
		m_ptWait.x = nX; 
		m_ptWait.y = nY; 
	}

	void GetWaitPosition(int& rX, int& rY)
	{ 
		rX = m_ptWait.x;
		rY = m_ptWait.y; 
	}

private:
	int   m_nMapPort;
	POINT m_ptWait;
};
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
class CArenaLobby
{
public:
	typedef list<CHARLIST*>       LIST_MEMBER;
	typedef LIST_MEMBER::iterator ITOR_MEMBER;

public:
	static CArenaLobby* CreateArenaLobby()
	{
		return new CArenaLobby; 
	}

private:
	CArenaLobby();

public:
	~CArenaLobby();

public:
	void Clear();
	void AddMember(int nGame, CHARLIST* pMember);
	void DelMember(CHARLIST* pMember);
	void ClearMember();

	void SendPacket(t_packet* pPacket);

public:
	LIST_MEMBER& GetMemberSet()
	{
		return m_ltMember;
	}

	int GetMemberCount() const 
	{	//< CSD-TW-030701
		return m_ltMember.size(); 
	}	//> CSD-TW-030701

	bool IsEmptyMember() const
	{
		return (m_ltMember.empty()) ? true:false;
	}

	bool IsExistMember(CHARLIST* pMember)
	{
		ITOR_MEMBER itor = find(m_ltMember.begin(), m_ltMember.end(), pMember);
		return (itor != m_ltMember.end()) ? true:false;	
	}

private:
	LIST_MEMBER m_ltMember;
};
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
class CArenaTeam
{
public:
	typedef list<CHARLIST*>          LIST_MEMBER;
	typedef LIST_MEMBER::iterator    ITOR_MEMBER;
	typedef map<CHARLIST*, DWORD>    HASH_GAMBLER;
	typedef HASH_GAMBLER::iterator   ITOR_GAMBLER;
	typedef HASH_GAMBLER::value_type PAIR_GAMBLER;

public:
	static CArenaTeam* CreateArenaTeam()
	{
		return new CArenaTeam;
	}

private:
	CArenaTeam();

public:
	~CArenaTeam();

public:
	void Clear();
	void AddMember(CHARLIST* pMember);
	void DelMember(CHARLIST* pMember);
	void ClearMember();

	void AddObserve(CHARLIST* pObserve);
	void DelObserve(CHARLIST* pObserve);
	void ClearObserve();

	void AddGambler(CHARLIST* pGambler, DWORD dwMoney);
	void DelGambler(CHARLIST* pGambler);
	void ClearGambler();

	int GetAliveCount();
	int GetDeadCount();
	int GetTeamLevel();
	DWORD GetBattingMoney();

	bool ChanageLeader(CHARLIST*& rLeader);
	void GiveBonus();

	void StartGame();
	void EndGame(int nX, int nY);

	void SendPacket(t_packet* pPacket);
	void SendPacket(int idTarget, int nType);
	
public:
	void GetBasePosition(int& rX, int& rY)
	{
		rX = m_ptBase.x;
		rY = m_ptBase.y;
	}

	void SetBasePosition(int nX, int nY)
	{
		m_ptBase.x = nX;
		m_ptBase.y = nY;
	}

	int GetHuntNumber() const { return m_nHuntNumber; }
	void SetHuntNumber(int nNumber) { m_nHuntNumber = nNumber; }

	int GetTeamNumber() const { return m_nTeamNumber; }
	void SetTeamNumber(int nNumber) { m_nTeamNumber = nNumber; }

	int GetGameResult() const { return m_nGameResult; }
	void SetGameResult(int nResult) { m_nGameResult = nResult; }

	int GetMemberCount() const { return m_ltMember.size(); }
	int GetGamblerCount() const { return m_mpGambler.size(); }
	
	DWORD GetBattingMoney(CHARLIST* pGambler)
	{
		ITOR_GAMBLER itor = m_mpGambler.find(pGambler);
		
		if (itor != m_mpGambler.end())
		{
			return itor->second;
		}

		return 0;
	}

	int GetKillCount() const { return m_nKillCount; }
	void IncKillCount() { ++m_nKillCount; }
	void DecKillCount() { --m_nKillCount; }
	void ResetKillCount() { m_nKillCount = 0; }

	LIST_MEMBER& GetMemberSet()
	{
		return m_ltMember;
	}

	LIST_MEMBER& GetObserveSet()
	{
		return m_ltObserve;
	}

	HASH_GAMBLER& GetGamblerSet()
	{
		return m_mpGambler;
	}

	bool IsEmptyMember() const
	{
		return m_ltMember.empty();
	}

	bool IsExistMember(CHARLIST* pMember)
	{
		ITOR_MEMBER itor = find(m_ltMember.begin(), m_ltMember.end(), pMember);
		return (itor != m_ltMember.end()) ? true:false;	
	}

	bool IsExistObserve(CHARLIST* pObserve)
	{
		ITOR_MEMBER itor = find(m_ltObserve.begin(), m_ltObserve.end(), pObserve);
		return (itor != m_ltObserve.end()) ? true:false;	
	}

	bool IsExistGambler(CHARLIST* pGambler)
	{
		ITOR_GAMBLER itor = m_mpGambler.find(pGambler);
		return (itor != m_mpGambler.end()) ? true:false;
	}

private:
	LIST_MEMBER  m_ltMember;
	LIST_MEMBER  m_ltObserve;
	HASH_GAMBLER m_mpGambler;
	POINT        m_ptBase;
	int          m_nHuntNumber;
	int          m_nTeamNumber;
	int          m_nKillCount;
	int          m_nGameResult;
};
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
class CArenaState
{
public:
	CArenaState();

public:
	void InitTeamLevel();
	void SumTeamLevel(CArenaTeam* pTeam);

	void ReadyState(DWORD dwTime);
	void StartState(DWORD dwTime);
	void EndState();
	int CheckState(DWORD dwTime);

	DWORD GetRemainTime(DWORD dwTime) const;
	DWORD GetFlowTime(DWORD dwTime) const;
	DWORD GetReadyTime(DWORD dwTime) const;

	void CalcWinTeamLaderScore(CArenaTeam* pTeam);
	void CalcLoseTeamLaderScore(CArenaTeam* pTeam);

	float GetTeamProbability(CArenaTeam* pTeam);

	bool IsReadyGame() const;
	bool IsPlayGame() const;

public:
	void SetPlayTime(DWORD dwMinute) 
	{ 
		m_dwPlayMinute = dwMinute; 
	}

	void SetReadyTime(DWORD dwSecond)
	{
		m_dwReadySecond = dwSecond;
	}

	void SetContinueTime(DWORD dwTime)
	{ 
		m_dwContinueTime = dwTime; 
	}
	
private:
	int GetWinTeamLaderScore(CArenaTeam* pTeam);
	int GetLoseTeamLaderScore(CArenaTeam* pTeam);

private:
	int	  m_nState;
	int   m_nCheck;
	DWORD m_dwReadySecond;
	DWORD m_dwPlayMinute;
	DWORD m_dwPrepareTime;
	DWORD m_dwContinueTime;
	int   m_nTotalLevel;
};
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
class CArenaGambling
{
	enum {MAX_MEMBER_COUNT = 30};

public:
	CArenaGambling();

public:
	void InitBattingMoney();
	void SumBattingMoney(CArenaTeam* pTeam);

	void CalcBattingMoney(CArenaTeam* pTeam);
	void CalcDividendMoney(CArenaTeam* pTeam);

	DWORD GetDividendRate(CArenaTeam* pTeam);

	bool IsEnableGamblerCount(CArenaTeam* pTeam) const;
	bool IsEnableBattingMoney(CArenaTeam* pTeam) const; // CSD-031119
	bool IsEnableBattingMoney(DWORD dwMoney) const;

public:
	void SetBattingMoney(DWORD dwMin, DWORD dwMax)
	{
		m_dwMinBattingMoney = dwMin;
		m_dwMaxBattingMoney = dwMax;
	}

	void SetBattingMoney(DWORD dwMoney)
	{
		m_dwBattingMoney = dwMoney;
	}

	void SetDividendRate(float fRate)
	{
		m_fDividendRate = fRate;
	}

	void SetTeamCount(int nCount)
	{
		m_nTeamCount = nCount;
	}

private:
	DWORD m_dwMaxBattingMoney;
	DWORD m_dwMinBattingMoney;
	DWORD m_dwWinBattingMoney;
	DWORD m_dwLoseBattingMoney;
	DWORD m_dwBattingMoney;
	float m_fDividendRate;
	int   m_nTeamCount;
};
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
class CArenaObserve
{
public:
	CArenaObserve();

public:
	void CalcObserveMoney(CArenaTeam* pTeam);
	bool IsEnoughMoney(CHARLIST* pObserve) const;

public:
	void SetObserveMoney(DWORD dwMoney)
	{
		m_dwObserveMoney = dwMoney;
	}

private:
	DWORD m_dwObserveMoney;
};
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
class CArenaBonus
{
public:
	static CArenaBonus* CreateArenaBonus()
	{
		return new CArenaBonus;
	}

private:
	CArenaBonus();

public:
	void Clear();
	bool GiveBonus(CHARLIST* pMember);

public:
	void SetLevelRange(int nMin, int nMax)
	{
		m_nMinLevel = nMin, m_nMaxLevel = nMax;
	}

	void SetItemHouse(int nHouse)
	{ 
		m_nItemHouse = nHouse;
	}

	void SetItemKind(int nKind)
	{ 
		m_nItemKind = nKind;
	}

	void SetItemGrade(int nGrade)
	{ 
		m_nItemGrade = nGrade;
	}

	void SetItemGainRate(float fRate) 
	{ 
		m_fItemGainRate = fRate; 
	}

	void SetExpRate(float fRate) 
	{ 
		m_fExpRate = fRate; 
	}

private:
	void AddExperience(CHARLIST* pMember);
	void AddGambleItem(CHARLIST* pMember);

private:
	int m_nMinLevel;
	int m_nMaxLevel;
	int m_nItemHouse;
	int m_nItemKind;
	int m_nItemGrade;
	float m_fItemGainRate;
	float m_fExpRate;
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_ARENADEF_H__44CA5FE7_F9FF_4391_8EF1_49C44E9A5EA0__INCLUDED_)
