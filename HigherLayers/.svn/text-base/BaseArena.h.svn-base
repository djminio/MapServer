// BaseArena.h: interface for the CBaseArena class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEARENA_H__A82D2E3F_C5D3_4956_B3CF_C3F4B57DB266__INCLUDED_)
#define AFX_BASEARENA_H__A82D2E3F_C5D3_4956_B3CF_C3F4B57DB266__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ArenaDef.h"

///////////////////////////////////////////////////////////////////////////////
//
class CBaseArena  
{
	enum { MAX_TEAM_NUMBER = 7, MAX_LMS_TEAM_NUMBER = 100 }; // CSD-TW-030618

public:
	typedef list<CArenaTeam*>    LIST_TEAM;
	typedef LIST_TEAM::iterator  ITOR_TEAM;

	typedef list<CArenaBonus*>   LIST_BONUS;
	typedef LIST_BONUS::iterator ITOR_BONUS;

public:
	static CBaseArena* CreateArena(int nIndex, int nType);

protected:
	CBaseArena();
	
public:	
	virtual ~CBaseArena();

public:
	void Clear();
	
	void AddTeam(CArenaTeam* pTeam);
	void DelTeam(CArenaTeam* pTeam);
	void ClearTeam();

	void AddBonus(CArenaBonus* pBonus);
	void DelBonus(CArenaBonus* pBonus);
	void ClearBonus();

	bool AddLobby(int nGame, CHARLIST* pMember);
	bool DelLobby(CHARLIST* pMember);

	bool AddTeam(int nTeam, CHARLIST* pMember);
	bool DelTeam(CHARLIST* pMember);

	bool AddObserve(int nTeam, CHARLIST* pMember);
	bool DelObserve(CHARLIST* pMember);

	bool AddGambler(int nTeam, CHARLIST* pMember, DWORD dwMoney);
	bool DelGambler(CHARLIST* pMember);

	void SendAddLobby(CHARLIST* pMember);
	void SendDeleteLobby(CHARLIST* pMember);
	void SendAddTeam(int nTeam, CHARLIST* pMember);
	void SendDeleteTeam(int nTeam, CHARLIST* pMember);
	void SendAddGambling(int nTeam, CHARLIST* pGambler);
	void SendDeleteGambling(int nTeam, CHARLIST* pGambler);
	void SendAddObserve(int nTeam, CHARLIST* pObserve);
	void SendLeaderInfo(CHARLIST* pMember);
	void SendGameInfo(CHARLIST* pTarget, WORD wPort); // CSD-TW-030701
	void SendDeleteObserve(int nTeam, CHARLIST* pObserve);
	void SendRemainCount(int nTeam, int nCount);
	void SendRemainTime(CHARLIST* pTarget, DWORD dwTime); // CSD-TW-030627
	void SendKillCount(int nTeam, int nCount);
	void SendOccupyStone(int nTeam, int nCount);
	void SendStartGame(CHARLIST* pLeader);
	void SendGameMessage(int nTeam, int nType, int nData);
	void SendGameMessage(CHARLIST* pCaster, CHARLIST* pTarget);
	void SendGameMessage(CHARLIST* pTarget, int nType, int nData);
	void SendMemberDefeat(CHARLIST* pMember);

	void RequestArenaMember(CHARLIST* pMember);
	void RequestRegisterLeader(CHARLIST* pMember);
	void RequestChangeLeader(CHARLIST* pMember);
	void RequestGambling(CHARLIST* pGambler);
	void RequestProbability();

public:
	virtual void StartReady(DWORD dwTime);
	virtual void EndReady();
	virtual bool ReadyGame(DWORD dwTime);
	virtual bool DelayGame(DWORD dwTime);
	virtual void StartGame(DWORD dwTime);
	virtual void EndGame(CArenaInfo* pArena);
	virtual bool PlayGame(DWORD dwTime);

public:
	void SetArenaState(CArenaState* pState)
	{
		m_pState = pState;
	}

	void SetArenaGambling(CArenaGambling* pGambling)
	{
		m_pGambling	= pGambling;
	}

	void SetArenaObserve(CArenaObserve* pObserve)
	{
		m_pObserve	= pObserve;
	}

	void SetArenaLobby(CArenaLobby* pLobby)
	{
		m_pLobby = pLobby;
	}
#ifdef LMS_MAPSERVER
	void SetLvlRange(int min, int max)//Eleval 18/07/09 - To set lvl range
	{
		m_lvl_min = min;
		m_lvl_max = max;
	}
	int GetLvlMin() const { return m_lvl_min; }
	int GetLvlMax() const { return m_lvl_max; }
#endif

	CArenaLobby* GetArenaLobby() const
	{
		return m_pLobby;
	}

	int GetGameIndex() const { return m_nIndex; }
	int GetArenaType() const { return m_nArenaType; }

	CHARLIST* GetLeader() const { return m_pLeader; }
	void SetLeader(CHARLIST* pLeader) { m_pLeader = pLeader; }

	LIST_TEAM& GetArenaTeamSet()
	{
		return m_ltTeam;
	}

	CArenaTeam* GetArenaTeam(int nTeam)
	{
		CArenaTeam* pTeam = NULL;

		for (ITOR_TEAM i = m_ltTeam.begin(); i != m_ltTeam.end(); ++i)
		{
			pTeam = *i;

			if (pTeam->GetTeamNumber() == nTeam)
			{
				return pTeam;
			}
		}

		return NULL;
	}

	bool IsReadyGame() const
	{
		return m_pState->IsReadyGame();
	}

	bool IsPlayGame() const
	{
		return m_pState->IsPlayGame();
	}

protected:
	virtual void DoResult(CArenaTeam* pTeam);
	virtual void DoVictory(CArenaTeam* pTeam);
	virtual void DoDefeat(CArenaTeam* pTeam);
	void DoMemberVictory(CHARLIST* pMember);

protected:
	void SendPacketAll(t_packet* pPacket);
	void SendPacketAll(int nType, void* pMsg, int nSize);
	void SendPacketTeam(t_packet* pPacket);
	void SendPacketTeam(int nType, void* pMsg, int nSize);
	void SendPacketWait(t_packet* pPacket);
	void SendPacketWait(int nType, void* pMsg, int nSize);

	int GetGameMemberCount(); // CSD-TW-030701

private:
	bool IsEnableReady(); // CSD-TW-030618

protected:
	CArenaState*    m_pState;
	CArenaGambling* m_pGambling;
	CArenaObserve*  m_pObserve;
	CArenaLobby*    m_pLobby;
	LIST_TEAM       m_ltTeam;
	LIST_BONUS      m_ltBonus;
	int             m_nIndex;
	int             m_nArenaType;
#ifdef LMS_MAPSERVER
	int				m_lvl_min; //Eleval 18/07/09 - LVL Range
	int				m_lvl_max; //Eleval 18/07/09 - LVL Range
#endif

private:
	CHARLIST*       m_pLeader;
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_BASEARENA_H__A82D2E3F_C5D3_4956_B3CF_C3F4B57DB266__INCLUDED_)

