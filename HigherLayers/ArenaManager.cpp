// ArenaManager.cpp: implementation of the CArenaManager class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "ArenaManager.h"

#include "teambattle.h"

extern CColossusStoneMgr g_ColossusStone;

///////////////////////////////////////////////////////////////////////////////
// Static Member
///////////////////////////////////////////////////////////////////////////////

DWORD CArenaManager::s_dwReady = 0;
DWORD CArenaManager::s_dwDelay = 0;

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CArenaManager::CArenaManager()
{
	m_mpArena.clear();
	m_mpGame.clear();
	m_dwContinueTime = 0;
}

CArenaManager::~CArenaManager()
{
	ClearArena();
	ClearGame();
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

bool CArenaManager::AddArena(int nPort, CArenaInfo* pArena)
{
	ITOR_ARENA itor = m_mpArena.find(nPort);

	if (itor != m_mpArena.end())
	{
		MyLog(LOG_NORMAL, "CArenaManager::AddArena : Duplicate %d", nPort);
		return false;
	}

	m_mpArena.insert(PAIR_ARENA(nPort, pArena));
	return true;
}

void CArenaManager::ClearArena()
{
	for (ITOR_ARENA i = m_mpArena.begin(); i != m_mpArena.end(); ++i)
	{
		delete i->second;	
	}

	m_mpArena.clear();
}

bool CArenaManager::AddGame(int nIndex, CBaseArena* pArena)
{
	ITOR_GAME itor = m_mpGame.find(nIndex);

	if (itor != m_mpGame.end())
	{
		MyLog(LOG_NORMAL, "CArenaManager::AddGame : Duplicate %d", nIndex);
		return false;
	}

	m_mpGame.insert(PAIR_GAME(nIndex, pArena));
	return true;
}

void CArenaManager::ClearGame()
{
	for (ITOR_GAME i = m_mpGame.begin(); i != m_mpGame.end(); ++i)
	{
		delete i->second;	
	}

	m_mpGame.clear();
}

void CArenaManager::Ready(DWORD dwSecond)
{
	if (g_curr_time < s_dwReady)
	{
		return;
	}

	CArenaInfo* pArena = GetArenaInfo(g_MapPort);
	
	if (pArena == NULL)
	{
		return;
	}

	for (ITOR_GAME i = m_mpGame.begin(); i != m_mpGame.end(); ++i)
	{
		CBaseArena* pGame = i->second;

		if (!pGame->IsReadyGame())
		{
			continue;
		}

		if (!pGame->ReadyGame(g_curr_time))
		{
			pGame->EndReady();
		}
	}

	s_dwReady = g_curr_time + dwSecond;
}

void CArenaManager::Execute(DWORD dwSecond)
{
	if (g_curr_time < s_dwDelay)
	{
		return;
	}

	CArenaInfo* pArena = GetArenaInfo(g_MapPort);
	
	if (pArena == NULL)
	{
		return;
	}

	for (ITOR_GAME i = m_mpGame.begin(); i != m_mpGame.end(); ++i)
	{
		CBaseArena* pGame = i->second;

		if (!pGame->IsPlayGame())
		{
			continue;
		}

	#ifdef LMS_MAPSERVER
		if (!pGame->DelayGame(g_curr_time))
		{
			continue;
		}
	#endif

		if (!pGame->PlayGame(g_curr_time))
		{
			pGame->EndGame(pArena);
		}
	}

	s_dwDelay = g_curr_time + dwSecond;
}

void CArenaManager::Remove(WORD idMember)
{
	CHARLIST* pMember = ::CheckServerId(idMember);

	if (pMember == NULL)
	{
		return;
	}

	CBaseArena* pGame = pMember->GetJoinArenaGame();
	
	if (pGame == NULL)
	{
		return;
	}
	
	if (pGame->DelLobby(pMember))
	{
	
	}

	if (pGame->DelTeam(pMember))
	{
		pGame->RequestProbability();
		pGame->RequestChangeLeader(pMember);
	}

	if (pGame->DelGambler(pMember))
	{
		pGame->RequestGambling(pMember);
	}

	if (pGame->DelObserve(pMember))
	{
	
	}

	pMember->SetJoinArenaGame(NULL);
	pMember->SetJoinArenaTeam(NULL);
}

void CArenaManager::RecvEnterLobby(WORD idWaiter, t_arena_enter_lobby* pPacket)
{
	CHARLIST* pMember = ::CheckServerId(idWaiter);

	if (pMember == NULL)
	{
		return;
	}

	const int nGame = pPacket->nArenaGame;
	CBaseArena* pGame = GetGameInfo(nGame);
	
	if (pGame == NULL)
	{
		pMember->Message(MK_INFORMATION, 1, 102);
		return;
	}
	
	if (pGame->IsPlayGame())
	{
		pMember->Message(MK_INFORMATION, 1, 103);
		return;
	}

	if (pGame->AddLobby(nGame, pMember))
	{
		pGame->RequestArenaMember(pMember);
		pGame->RequestGambling(pMember);
		pGame->RequestProbability();
	}
}

void CArenaManager::RecvLeaveLobby(WORD idWaiter)
{
	CHARLIST* pMember = ::CheckServerId(idWaiter);

	if (pMember == NULL)
	{
		return;
	}

	CBaseArena* pGame = pMember->GetJoinArenaGame();

	if (pGame == NULL)
	{
		return;
	}

	if (pGame->DelLobby(pMember))
	{

	}

	if (pGame->DelTeam(pMember))
	{
		pGame->RequestProbability();
		pGame->RequestChangeLeader(pMember);
	}

	if (pGame->DelGambler(pMember))
	{
		pGame->RequestGambling(pMember);
	}

	if (pGame->DelObserve(pMember))
	{
	
	}

	pMember->SetJoinArenaGame(NULL);
	pMember->SetJoinArenaTeam(NULL);
}

void CArenaManager::RecvEnterTeam(WORD idMember, t_arena_enter_team* pPacket)
{
	CHARLIST* pMember = ::CheckServerId(idMember);
	
	if (pMember == NULL)
	{
		return;
	}

	const int nGame = pPacket->nArenaGame;
	CBaseArena* pGame = GetGameInfo(nGame);

	if (pGame == NULL)
	{
		pMember->Message(MK_INFORMATION, 1, 102);
		return;
	}

	if (pGame->IsPlayGame())
	{
		pMember->Message(MK_INFORMATION, 1, 103);
		return;
	}
	
	const int nTeam = pPacket->nArenaTeam;
	
#ifdef LMS_MAPSERVER
	if((pMember->GetLevel() < pGame->GetLvlMin() || pMember->GetLevel() > pGame->GetLvlMax()) && !pMember->IsCounselor()) //Eleval 18/07/09 - To check if the player match the lvl range
	{
		return;
	}
#endif
	
	if (pGame->AddTeam(nTeam, pMember))
	{	// 팀에서 등록된 사람이 있음을 알린다.
		SendTeamBattleMessageAddCharOk((char)nTeam, idMember);
		pGame->RequestRegisterLeader(pMember);
		pGame->RequestProbability();
	}
}

void CArenaManager::RecvLeaveTeam(WORD idMember, t_arena_leave_team* pPacket)
{
	CHARLIST* pMember = ::CheckServerId(idMember);
	
	if (pMember == NULL)
	{
		return;
	}

	const int nGame = pPacket->nArenaGame;
	CBaseArena* pGame = g_pArenaManager->GetGameInfo(nGame);

	if (pGame == NULL)
	{
		return;
	}

	if (pGame->DelTeam(pMember))
	{
		pGame->RequestChangeLeader(pMember);
		pMember->SetJoinArenaTeam(NULL);

		if (pGame->AddLobby(nGame, pMember))
		{
			pGame->RequestProbability();
		}
	}
}

void CArenaManager::RecvReadyGame(WORD idLeader, t_arena_ready_game* pPacket)
{	//< CSD-TW0-030606
	CHARLIST* pLeader = ::CheckServerId(idLeader);
	
	if (pLeader == NULL)
	{
		return;
	}

	//Finito added reporter check.
#ifndef LMS_MAPSERVER
	if ((pLeader->IsCounselor() || pLeader->IsReporter()) && pLeader->name_status.santa_power) //Eleval 22/06/09 - Santa Power
	{
		return;
	}
#endif

	CBaseArena* pGame = pLeader->GetJoinArenaGame();

	if (pGame == NULL)
	{
		pLeader->Message(MK_INFORMATION, 1, 102);
		return;
	}

	if (pGame->IsPlayGame())
	{
		pLeader->Message(MK_INFORMATION, 1, 103);
		return;
	}

	if (pGame->GetLeader() != pLeader)
	{
		pLeader->Message(MK_INFORMATION, 1, 117);
		return;
	}

	pGame->StartReady(pPacket->dwDelay);
}	//> CSD-TW0-030606

void CArenaManager::RecvStartGame(WORD idLeader)
{
	CHARLIST* pLeader = ::CheckServerId(idLeader);
	
	if (pLeader == NULL)
	{
		return;
	}

	CBaseArena* pGame = pLeader->GetJoinArenaGame();

	if (pGame == NULL)
	{
		pLeader->Message(MK_INFORMATION, 1, 102);
		return;
	}

	if (pGame->IsPlayGame())
	{
		pLeader->Message(MK_INFORMATION, 1, 103);
		return;
	}

	pGame->StartGame(g_curr_time);
}

void CArenaManager::RecvEnterObserve(WORD idMember, t_arena_enter_observe* pPacket)
{
	CHARLIST* pMember = ::CheckServerId(idMember);
	
	if (pMember == NULL)
	{
		return;
	}

	const int nGame = pPacket->nArenaGame;
	CBaseArena* pGame = GetGameInfo(nGame);

	if (pGame == NULL)
	{
		pMember->Message(MK_INFORMATION, 1, 102);
		return;
	}

	if (pGame->GetArenaType() == AT_LMS)
	{
		pMember->Message(MK_INFORMATION, 1, 102);
		return;
	}

	if (pGame->IsPlayGame())
	{
		pMember->Message(MK_INFORMATION, 1, 103);
		return;
	}

	const int nTeam = pPacket->nArenaTeam;
	pGame->AddObserve(nTeam, pMember);
}

void CArenaManager::RecvEnterGambling(WORD idMember, t_arena_enter_gambling* pPacket)
{
	CHARLIST* pMember = ::CheckServerId(idMember);
	
	if (pMember == NULL)
	{
		return;
	}

	const int nGame = pPacket->nArenaGame;
	CBaseArena* pGame = GetGameInfo(nGame);

	if (pGame == NULL)
	{
		pMember->Message(MK_INFORMATION, 1, 102);
		return;
	}

	if (pGame->GetArenaType() == AT_LMS)
	{
		pMember->Message(MK_INFORMATION, 1, 102);
		return;
	}

	if (pGame->IsPlayGame())
	{
		pMember->Message(MK_INFORMATION, 1, 103);
		return;
	}
	
	const int nTeam = pPacket->nArenaTeam;
	const DWORD dwBattingMoney = pPacket->dwBattingMoney;
	
	if (pGame->AddGambler(nTeam, pMember, dwBattingMoney))
	{
		pGame->RequestGambling(pMember);
	}
}