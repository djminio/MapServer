// ArenaManager.h: interface for the CArenaManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARENAMANAGER_H__446DAC05_9DA9_465B_AA88_7C859F256EEE__INCLUDED_)
#define AFX_ARENAMANAGER_H__446DAC05_9DA9_465B_AA88_7C859F256EEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseArena.h"

///////////////////////////////////////////////////////////////////////////////
//
class CArenaManager : public TSingleton<CArenaManager>  
{
public:
	typedef map<int, CArenaInfo*>  HASH_ARENA;
	typedef HASH_ARENA::iterator   ITOR_ARENA;
	typedef HASH_ARENA::value_type PAIR_ARENA;

	typedef map<int, CBaseArena*>  HASH_GAME;
	typedef HASH_GAME::iterator    ITOR_GAME;
	typedef HASH_GAME::value_type  PAIR_GAME;

protected:
	static DWORD s_dwReady;
	static DWORD s_dwDelay;

public:
	CArenaManager();
	virtual ~CArenaManager();

public:
	bool AddArena(int nPort, CArenaInfo* pArena);
	void ClearArena();

	bool AddGame(int nIndex, CBaseArena* pArena);
	void ClearGame();

	void Ready(DWORD dwSecond = 1);
	void Execute(DWORD dwSecond = 1);
	void Remove(WORD idMember);

	void RecvEnterLobby(WORD idWaiter, t_arena_enter_lobby* pPacket);
	void RecvLeaveLobby(WORD idWaiter);

	void RecvEnterTeam(WORD idMember, t_arena_enter_team* pPacket);
	void RecvLeaveTeam(WORD idMember, t_arena_leave_team* pPacket);

	void RecvReadyGame(WORD idLeader, t_arena_ready_game* pPacket);
	void RecvStartGame(WORD idLeader);

	void RecvEnterObserve(WORD idMember, t_arena_enter_observe* pPacket);

	void RecvEnterGambling(WORD idMember, t_arena_enter_gambling* pPacket);

public:
	HASH_ARENA& GetArenaSet() { return m_mpArena; }
	HASH_GAME& GetGameSet()   { return m_mpGame; }

	CArenaInfo* GetArenaInfo(int nPort)
	{
		if (m_mpArena.find(nPort) != m_mpArena.end())
		{
			return m_mpArena[nPort];
		}

		return NULL;
	}

	CBaseArena* GetGameInfo(int nIndex)
	{
		if (m_mpGame.find(nIndex) != m_mpGame.end())
		{
			return m_mpGame[nIndex];
		}

		return NULL;
	}

	bool IsArenaInfo(int nPort) const
	{
		return (m_mpArena.find(nPort) != m_mpArena.end()) ? true:false;
	}

	bool IsGameInfo(int nIndex)
	{
		return (m_mpGame.find(nIndex) != m_mpGame.end()) ? true:false;
	}

	bool IsColossusArena() const
	{
		return (!m_mpGame.empty()) ? true:false;
	}

private:
	HASH_ARENA m_mpArena;
	HASH_GAME  m_mpGame;
	DWORD      m_dwContinueTime;
};
//
///////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_ARENAMANAGER_H__446DAC05_9DA9_465B_AA88_7C859F256EEE__INCLUDED_)
