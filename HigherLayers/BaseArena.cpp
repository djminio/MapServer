// BaseArena.cpp: implementation of the CBaseArena class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "BaseArena.h"

#include "CoreArena.h"
#include "StoneArena.h"
#include "MonsterArena.h"
#include "LMSArena.h"

///////////////////////////////////////////////////////////////////////////////
// Static Method
///////////////////////////////////////////////////////////////////////////////

CBaseArena* CBaseArena::CreateArena(int nIndex, int nType)
{
	CBaseArena* pArena = NULL;

	switch (nType)
	{
	case AT_CORE:
		{
			pArena = new CCoreArena(nIndex, nType);
			break;
		}
	case AT_STONE:
		{
			pArena = new CStoneArena(nIndex, nType);
			break;
		}
	case AT_MONSTER:
		{
			pArena = new CMonsterArena(nIndex, nType);
			break;
		}
	case AT_LMS:
		{
			pArena = new CLMSArena(nIndex, nType);
			break;
		}
	}

	return pArena;
}

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CBaseArena::CBaseArena()
{
	Clear();
}

CBaseArena::~CBaseArena()
{
	ClearTeam();
	ClearBonus();
	m_pLeader = NULL;

	SAFE_DELETE(m_pState);
	SAFE_DELETE(m_pGambling);
	SAFE_DELETE(m_pObserve);
	SAFE_DELETE(m_pLobby);
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CBaseArena::Clear()
{
	m_pState = NULL;
	m_pGambling = NULL;
	m_pObserve = NULL;
	m_pLobby = NULL;
	m_ltTeam.clear();
	m_ltBonus.clear();
	m_nIndex = 0;
	m_nArenaType = AT_UNKNOWN;
	m_pLeader = NULL;
}

void CBaseArena::AddTeam(CArenaTeam* pTeam)
{
	m_ltTeam.push_back(pTeam);
}
	
void CBaseArena::DelTeam(CArenaTeam* pTeam)
{
	m_ltTeam.remove(pTeam);
}

void CBaseArena::ClearTeam()
{
	for_each(m_ltTeam.begin(), m_ltTeam.end(), TDelete());
	m_ltTeam.clear();
}

void CBaseArena::AddBonus(CArenaBonus* pBonus)
{
	m_ltBonus.push_back(pBonus);
}
	
void CBaseArena::DelBonus(CArenaBonus* pBonus)
{
	m_ltBonus.remove(pBonus);
}

void CBaseArena::ClearBonus()
{
	for_each(m_ltBonus.begin(), m_ltBonus.end(), TDelete());
	m_ltBonus.clear();
}

bool CBaseArena::AddLobby(int nGame, CHARLIST* pMember)
{
	if (m_pLobby->IsExistMember(pMember))
	{
		pMember->Message(MK_INFORMATION, 1, 104);
		return false;
	}

	pMember->SetJoinArenaGame(this);
	m_pLobby->AddMember(nGame, pMember);
	SendAddLobby(pMember);
	return true;
}

bool CBaseArena::DelLobby(CHARLIST* pMember)
{
	if (!m_pLobby->IsExistMember(pMember))
	{
		return false;
	}

	SendDeleteLobby(pMember);
	m_pLobby->DelMember(pMember);
	return true;
}

bool CBaseArena::AddTeam(int nTeam, CHARLIST* pMember)
{	//< CSD-030930
	for (ITOR_TEAM i = m_ltTeam.begin(); i != m_ltTeam.end(); ++i)
	{
		if ((*i)->IsExistMember(pMember))
		{
			pMember->Message(MK_INFORMATION, 1, 105);
			return false;
		}

		if ((*i)->IsExistObserve(pMember))
		{
			pMember->Message(MK_INFORMATION, 1, 106);
			return false;
		}

		if ((*i)->IsExistGambler(pMember))
		{
			pMember->Message(MK_INFORMATION, 1, 123);
			return false;
		}
	}

	CArenaTeam* pTeam = GetArenaTeam(nTeam);

	if (pTeam == NULL)
	{
		pMember->Message(MK_INFORMATION, 1, 107);
		return false;
	}
#ifdef LMS_MAPSERVER
	if (GetArenaType() == AT_LMS)
	{
		if (pTeam->GetMemberCount() >= MAX_LMS_TEAM_NUMBER)
		{
			pMember->Message(MK_INFORMATION, 1, 119);
			return false;
		}
	}
	else
#endif
	{
		if (pTeam->GetMemberCount() >= MAX_TEAM_NUMBER)
		{
			pMember->Message(MK_INFORMATION, 1, 119);
			return false;
		}
	}

	if (m_pLobby->IsExistMember(pMember))
	{
		SendDeleteLobby(pMember);
		m_pLobby->DelMember(pMember);
	}
		
	pMember->SetJoinArenaTeam(pTeam);
	pTeam->AddMember(pMember);
	SendAddTeam(nTeam, pMember);
	return true;
}	//> CSD-030930

bool CBaseArena::DelTeam(CHARLIST* pMember)
{
	CArenaTeam* pTeam = pMember->GetJoinArenaTeam();

	if (pTeam == NULL)
	{
		return false;
	}
		
	if (!pTeam->IsExistMember(pMember))
	{
		return false;
	}
	
	const int nTeam = pTeam->GetTeamNumber();
	SendDeleteTeam(nTeam, pMember);

	pTeam->DelMember(pMember);
	return true;
}

bool CBaseArena::AddObserve(int nTeam, CHARLIST* pMember)
{
	CArenaTeam* pTeam = GetArenaTeam(nTeam);

	if (pTeam == NULL)
	{
		pMember->Message(MK_INFORMATION, 1, 107);
		return false;
	}

	if (pTeam->IsEmptyMember())
	{
		pMember->Message(MK_INFORMATION, 1, 108);
		return false;
	}

	if (pTeam->IsExistMember(pMember))
	{
		pMember->Message(MK_INFORMATION, 1, 101);
		return false;
	}

	if (pTeam->IsExistObserve(pMember))
	{
		pTeam->DelObserve(pMember);
	}
	// 관전료 계산
	if (!m_pObserve->IsEnoughMoney(pMember))
	{
		pMember->Message(MK_INFORMATION, 1, 109);
		return false;
	}

	pMember->SetJoinArenaTeam(pTeam);
	pTeam->AddObserve(pMember);
	SendAddObserve(nTeam, pMember);
	return true;
}

bool CBaseArena::DelObserve(CHARLIST* pMember)
{
	CArenaTeam* pTeam = pMember->GetJoinArenaTeam();

	if (pTeam == NULL)
	{
		return false;
	}
		
	if (!pTeam->IsExistObserve(pMember))
	{
		return false;
	}

	const int nTeam = pTeam->GetTeamNumber();
	SendDeleteObserve(nTeam, pMember);
	pTeam->DelObserve(pMember);
	return true;
}

bool CBaseArena::AddGambler(int nTeam, CHARLIST* pMember, DWORD dwMoney)
{
	for (ITOR_TEAM i = m_ltTeam.begin(); i != m_ltTeam.end(); ++i)
	{
		CArenaTeam* pTeam = *i;

		if (!m_pGambling->IsEnableGamblerCount(pTeam))
		{
			pMember->Message(MK_SHORTAGE, 1, 110);
			return false;
		}
	}

	if (!m_pGambling->IsEnableBattingMoney(dwMoney))
	{
		pMember->Message(MK_SHORTAGE, 1, 111);
		return false;
	}

	if (pMember->GetBankMoney() < dwMoney)
	{
		pMember->Message(MK_SHORTAGE, 1, 112);
		return false;
	}

	CArenaTeam* pTeam = GetArenaTeam(nTeam);

	if (pTeam == NULL)
	{
		pMember->Message(MK_INFORMATION, 1, 107);
		return false;
	}

	if (pTeam->IsEmptyMember())
	{
		pMember->Message(MK_INFORMATION, 1, 113);
		return false;
	}

	if (pTeam->IsExistGambler(pMember))
	{
		pTeam->DelGambler(pMember);
	}

	pMember->SetJoinArenaTeam(pTeam);
	pTeam->AddGambler(pMember, dwMoney);
	SendAddGambling(nTeam, pMember);
	return true;
}

bool CBaseArena::DelGambler(CHARLIST* pMember)
{
	CArenaTeam* pTeam = pMember->GetJoinArenaTeam();

	if (pTeam == NULL)
	{
		return false;
	}
		
	if (!pTeam->IsExistGambler(pMember))
	{
		return false;
	}

	const int nTeam = pTeam->GetTeamNumber();
	SendDeleteGambling(nTeam, pMember);
	pTeam->DelGambler(pMember);
	return true;
}

void CBaseArena::SendAddLobby(CHARLIST* pMember)
{
	t_packet packet;
	packet.h.header.type = CMD_ARENA_ADD_LOBBY;
	packet.h.header.size = sizeof(t_arena_add_lobby);
	packet.u.arena.arena_add_lobby.idMember = pMember->GetServerID();
	SendPacketAll(&packet);
}
	
void CBaseArena::SendDeleteLobby(CHARLIST* pMember)
{
	t_packet packet;
	packet.h.header.type = CMD_ARENA_DELETE_LOBBY;
	packet.h.header.size = sizeof(t_arena_delete_lobby);
	packet.u.arena.arena_delete_lobby.idMember = pMember->GetServerID();
	SendPacketAll(&packet);
}

void CBaseArena::SendAddTeam(int nTeam, CHARLIST* pMember)
{
	t_packet packet;
	packet.h.header.type = CMD_ARENA_ADD_TEAM;
	packet.h.header.size = sizeof(t_arena_add_team);
	packet.u.arena.arena_add_team.idMember = pMember->GetServerID();
	packet.u.arena.arena_add_team.nTeam = nTeam;
	SendPacketAll(&packet);
}

void CBaseArena::SendDeleteTeam(int nTeam, CHARLIST* pMember)
{
	t_packet packet;
	packet.h.header.type = CMD_ARENA_DELETE_TEAM;
	packet.h.header.size = sizeof(t_arena_delete_team);
	packet.u.arena.arena_delete_team.idMember = pMember->GetServerID();
	packet.u.arena.arena_delete_team.nTeam = nTeam;
	SendPacketAll(&packet);
}

void CBaseArena::SendAddGambling(int nTeam, CHARLIST* pGambler)
{
	t_packet packet;
	packet.h.header.type = CMD_ARENA_ADD_GAMBLING;
	packet.h.header.size = sizeof(t_arena_add_gambling);
	packet.u.arena.arena_add_gambling.idGambler = pGambler->GetServerID();
	packet.u.arena.arena_add_gambling.nTeam = nTeam;
	SendPacketAll(&packet);
}

void CBaseArena::SendDeleteGambling(int nTeam, CHARLIST* pGambler)
{
	t_packet packet;
	packet.h.header.type = CMD_ARENA_DELETE_GAMBLING;
	packet.h.header.size = sizeof(t_arena_delete_gambling);
	packet.u.arena.arena_delete_gambling.idGambler = pGambler->GetServerID();
	packet.u.arena.arena_delete_gambling.nTeam = nTeam;
	SendPacketAll(&packet);
}

void CBaseArena::SendAddObserve(int nTeam, CHARLIST* pObserve)
{
	t_packet packet;
	packet.h.header.type = CMD_ARENA_ADD_OBSERVE;
	packet.h.header.size = sizeof(t_arena_add_observe);
	packet.u.arena.arena_add_observe.idObserve = pObserve->GetServerID();
	packet.u.arena.arena_add_observe.nTeam = nTeam;
	SendPacketAll(&packet);
}

void CBaseArena::SendDeleteObserve(int nTeam, CHARLIST* pObserve)
{
	t_packet packet;
	packet.h.header.type = CMD_ARENA_DELETE_OBSERVE;
	packet.h.header.size = sizeof(t_arena_delete_observe);
	packet.u.arena.arena_delete_observe.idObserve = pObserve->GetServerID();
	packet.u.arena.arena_delete_observe.nTeam = nTeam;
	SendPacketAll(&packet);
}

void CBaseArena::SendLeaderInfo(CHARLIST* pMember)
{
	t_packet packet;
	packet.h.header.type = CMD_ARENA_LEADER_INFO;
	packet.h.header.size = sizeof(t_arena_leader_info);
	packet.u.arena.arena_leader_info.idLeader = pMember->GetServerID();
	SendPacketAll(&packet);
	// 리더로 설정
	m_pLeader = pMember;
}

void CBaseArena::SendGameInfo(CHARLIST* pTarget, WORD wPort)
{	//< CSD-TW-030701
	t_packet packet;
	packet.h.header.size = sizeof(t_arena_game_info);
	packet.h.header.type = CMD_ARENA_GAME_INFO;
	packet.u.arena.arena_game_info.wPort = wPort;
	packet.u.arena.arena_game_info.wMemberCount = GetGameMemberCount();

	if (m_pLeader != NULL)
	{
		strcpy_s(packet.u.arena.arena_game_info.szLeader, sizeof(packet.u.arena.arena_game_info.szLeader), m_pLeader->Name);
	}
	else
	{
		strcpy_s(packet.u.arena.arena_game_info.szLeader, sizeof(packet.u.arena.arena_game_info.szLeader), " ");
	}

	::QueuePacket(connections, pTarget->GetServerID(), &packet, 1);
}	//> CSD-TW-030701

void CBaseArena::SendStartGame(CHARLIST* pLeader)
{
	t_packet packet;
	packet.h.header.type = CMD_ARENA_START_GAME;
	packet.h.header.size = sizeof(t_arena_start_game);
	packet.u.arena.arena_start_game.idLeader = pLeader->GetServerID();
	::QueuePacket(connections, pLeader->GetServerID(), &packet, 1);
}

void CBaseArena::SendRemainCount(int nTeam, int nCount)
{	//< CSD-031106
	t_packet packet;
	packet.h.header.size = sizeof(t_arena_game_message1);
	packet.h.header.type = CMD_ARENA_GAME_MESSAGE1;
	packet.u.arena.arena_game_message1.nTeamNumber = nTeam;
	packet.u.arena.arena_game_message1.nMsgType = 6;
	packet.u.arena.arena_game_message1.nGameData = nCount;
	SendPacketTeam(&packet);
}	//> CSD-031106

void CBaseArena::SendRemainTime(CHARLIST* pTarget, DWORD dwTime)
{	//< CSD-TW-030627
	t_packet packet;
	packet.h.header.size = sizeof(t_arena_game_message4);
	packet.h.header.type = CMD_ARENA_GAME_MESSAGE4;
	packet.u.arena.arena_game_message4.dwRemainMinute = m_pState->GetRemainTime(dwTime)/60;
	::QueuePacket(connections, pTarget->GetServerID(), &packet, 1);
}	//> CSD-TW-030627

void CBaseArena::SendKillCount(int nTeam, int nCount)
{	//< CSD-031106
	t_packet packet;
	packet.h.header.size = sizeof(t_arena_game_message1);
	packet.h.header.type = CMD_ARENA_GAME_MESSAGE1;
	packet.u.arena.arena_game_message1.nTeamNumber = nTeam;
	packet.u.arena.arena_game_message1.nMsgType = 6;
	packet.u.arena.arena_game_message1.nGameData = nCount;
	SendPacketTeam(&packet);
}	//> CSD-031106

void CBaseArena::SendOccupyStone(int nTeam, int nCount)
{	//< CSD-031106
	t_packet packet;
	packet.h.header.size = sizeof(t_arena_game_message1);
	packet.h.header.type = CMD_ARENA_GAME_MESSAGE1;
	packet.u.arena.arena_game_message1.nTeamNumber = nTeam;
	packet.u.arena.arena_game_message1.nMsgType = 6;
	packet.u.arena.arena_game_message1.nGameData = nCount;
	SendPacketTeam(&packet);
}	//> CSD-031106

void CBaseArena::SendGameMessage(int nTeam, int nType, int nData)
{	//< CSD-031106
	t_packet packet;
	packet.h.header.size = sizeof(t_arena_game_message1);
	packet.h.header.type = CMD_ARENA_GAME_MESSAGE1;
	packet.u.arena.arena_game_message1.nTeamNumber = nTeam;
	packet.u.arena.arena_game_message1.nMsgType = nType;
	packet.u.arena.arena_game_message1.nGameData = nData;
	SendPacketTeam(&packet);
}	//> CSD-031106

void CBaseArena::SendGameMessage(CHARLIST* pCaster, CHARLIST* pTarget)
{
	if (pCaster->GetJoinArenaGame() != this)
	{
		return;
	}

	if (pTarget->GetJoinArenaGame() != this)
	{
		return;
	}

	CArenaTeam* pTeam1 = pCaster->GetJoinArenaTeam();
	
	if (pTeam1 == NULL) 
	{
		return;
	}

	CArenaTeam* pTeam2 = pTarget->GetJoinArenaTeam();
	
	if (pTeam2 == NULL) 
	{
		return;
	}

	t_packet packet;
	packet.h.header.size = sizeof(t_arena_game_message2);
	packet.h.header.type = CMD_ARENA_GAME_MESSAGE2;
	packet.u.arena.arena_game_message2.nTeamNumber1 = pTeam1->GetTeamNumber();
	packet.u.arena.arena_game_message2.nTeamNumber2 = pTeam2->GetTeamNumber();
	strcpy_s(packet.u.arena.arena_game_message2.szTeamMember1, sizeof(packet.u.arena.arena_game_message2.szTeamMember1), pCaster->Name);
	strcpy_s(packet.u.arena.arena_game_message2.szTeamMember2, sizeof(packet.u.arena.arena_game_message2.szTeamMember2), pTarget->Name);
	pTeam1->SendPacket(&packet);
	pTeam2->SendPacket(&packet);
}

void CBaseArena::SendGameMessage(CHARLIST* pTarget, int nType, int nData)
{
	CArenaTeam* pTeam = pTarget->GetJoinArenaTeam();
	
	if (pTeam == NULL) 
	{
		return;
	}

	t_packet packet;
	packet.h.header.size = sizeof(t_arena_game_message3);
	packet.h.header.type = CMD_ARENA_GAME_MESSAGE3;
	packet.u.arena.arena_game_message3.nTeamNumber = pTeam->GetTeamNumber();
	packet.u.arena.arena_game_message3.nMsgType = nType;
	packet.u.arena.arena_game_message3.nStoneNumber = nData;
	strcpy_s(packet.u.arena.arena_game_message3.szTeamMember, sizeof(packet.u.arena.arena_game_message3.szTeamMember), pTarget->Name);
	pTeam->SendPacket(&packet);
}

void CBaseArena::SendMemberDefeat(CHARLIST* pMember)
{
	t_packet packet;
	packet.h.header.type = CMD_FIGHT_MAP_YOU_DEFEAT;
	packet.h.header.size = 0;
	QueuePacket(connections, pMember->GetServerID(), &packet, 1);
}

void CBaseArena::SendPacketAll(t_packet* pPacket)
{
	for (ITOR_TEAM i = m_ltTeam.begin(); i != m_ltTeam.end(); ++i)
	{
		(*i)->SendPacket(pPacket);
	}

	m_pLobby->SendPacket(pPacket);
}

void CBaseArena::RequestArenaMember(CHARLIST* pMember)
{	//< CSD-030522
	CArenaLobby::LIST_MEMBER ltLobby = m_pLobby->GetMemberSet();

	for (CArenaLobby::ITOR_MEMBER i = ltLobby.begin(); i != ltLobby.end(); ++i)
	{
		CHARLIST* pTemp = *i;
		if (!pTemp) continue;

		if (pMember == pTemp)
		{
			continue;
		}

		t_packet packet;
		packet.h.header.type = CMD_ARENA_ADD_LOBBY;
		packet.h.header.size = sizeof(t_arena_add_lobby);
		packet.u.arena.arena_add_lobby.idMember = pTemp->GetServerID();
		::QueuePacket(connections, pMember->GetServerID(), &packet, 1);
	}

	for (ITOR_TEAM j = m_ltTeam.begin(); j != m_ltTeam.end(); ++j)
	{
		const int nTeam = (*j)->GetTeamNumber();

		CArenaTeam::LIST_MEMBER ltMember = (*j)->GetMemberSet();

		for (CArenaTeam::ITOR_MEMBER k = ltMember.begin(); k != ltMember.end(); ++k)
		{
			CHARLIST* pTemp = *k;
			if (!pTemp) continue;

			if (pMember == pTemp)
			{
				continue;
			}

			if (pTemp == m_pLeader)
			{	// 리더임을 알림
				t_packet packet;
				packet.h.header.type = CMD_ARENA_LEADER_INFO;
				packet.h.header.size = sizeof(t_arena_leader_info);
				packet.u.arena.arena_leader_info.idLeader = pTemp->GetServerID();
				::QueuePacket(connections, pMember->GetServerID(), &packet, 1);
			}

			t_packet packet;
			packet.h.header.type = CMD_ARENA_ADD_TEAM;
			packet.h.header.size = sizeof(t_arena_add_team);
			packet.u.arena.arena_add_team.idMember = pTemp->GetServerID();
			packet.u.arena.arena_add_team.nTeam = nTeam;
			::QueuePacket(connections, pMember->GetServerID(), &packet, 1);
		}

		CArenaTeam::LIST_MEMBER ltObserve = (*j)->GetObserveSet();

		for (CArenaTeam::ITOR_MEMBER l = ltObserve.begin(); l != ltObserve.end(); ++l)
		{
			CHARLIST* pTemp = *l;
			if (!pTemp) continue;

			if (pMember == pTemp)
			{
				continue;
			}

			t_packet packet;
			packet.h.header.type = CMD_ARENA_ADD_OBSERVE;
			packet.h.header.size = sizeof(t_arena_add_team);
			packet.u.arena.arena_add_observe.idObserve = pTemp->GetServerID();
			packet.u.arena.arena_add_observe.nTeam = nTeam;
			::QueuePacket(connections, pMember->GetServerID(), &packet, 1);
		}

		CArenaTeam::HASH_GAMBLER mpGambler = (*j)->GetGamblerSet();

		for (CArenaTeam::ITOR_GAMBLER m = mpGambler.begin(); m != mpGambler.end(); ++m)
		{
			CHARLIST* pTemp = m->first;
			if (!pTemp) continue;

			if (pMember == pTemp)
			{
				continue;
			}

			t_packet packet;
			packet.h.header.type = CMD_ARENA_ADD_GAMBLING;
			packet.h.header.size = sizeof(t_arena_add_gambling);
			packet.u.arena.arena_add_gambling.idGambler = pTemp->GetServerID();
			packet.u.arena.arena_add_gambling.nTeam = nTeam;
			::QueuePacket(connections, pMember->GetServerID(), &packet, 1);
		}
	}

	::CallClient(pMember->GetServerID(), CMD_TEAMBATTLE_INFO);
}	//> CSD-030522

void CBaseArena::RequestRegisterLeader(CHARLIST* pMember)
{
	int nTotal = 0;

	for (ITOR_TEAM i = m_ltTeam.begin(); i != m_ltTeam.end(); ++i)
	{
		nTotal += (*i)->GetMemberCount();
	}

	if (nTotal == 1)
	{
		SendLeaderInfo(pMember);
	}
}

void CBaseArena::RequestChangeLeader(CHARLIST* pMember)
{
	if (m_pLeader != pMember)
	{
		return;
	}

	CArenaTeam* pTeam = pMember->GetJoinArenaTeam();

	if (pTeam == NULL)
	{
		return;
	}

	if (!pTeam->ChanageLeader(pMember))
	{
		m_pLeader = NULL;
		return;
	}

	SendLeaderInfo(pMember);
}

void CBaseArena::RequestGambling(CHARLIST* pGambler)
{
	t_packet packet;
	packet.h.header.type = CMD_ARENA_GAMBLING_INFO;
	packet.h.header.size = sizeof(t_arena_gambling_info);

	CArenaTeam* pTeam = NULL;
	
	for (ITOR_TEAM i = m_ltTeam.begin(); i != m_ltTeam.end(); ++i)
	{
		if ((*i)->IsExistGambler(pGambler))
		{
			pTeam = *i;
		}

		packet.u.arena.arena_gambling_info.nArenaTeam = (*i)->GetTeamNumber();
		packet.u.arena.arena_gambling_info.dwBattingMoney = (*i)->GetBattingMoney();
		SendPacketAll(&packet);
	}

	if (pTeam == NULL)
	{
		const DWORD dwBankMoney = pGambler->GetBankMoney();
		pGambler->SendCharInfoBasic(BANKMONEY, dwBankMoney);

		memset(&packet, 0, sizeof(t_arena_dividend_info));
		packet.h.header.type = CMD_ARENA_DIVIDEND_INFO;
		packet.h.header.size = sizeof(t_arena_dividend_info);
		packet.u.arena.arena_dividend_info.dwBattingMoney = 0;
		packet.u.arena.arena_dividend_info.dwDividendMoney = 0;
		::QueuePacket(connections, pGambler->GetServerID(), &packet, 1);
		return;
	}

	m_pGambling->InitBattingMoney();

	for (ITOR_TEAM j = m_ltTeam.begin(); j != m_ltTeam.end(); ++j)
	{
		m_pGambling->SumBattingMoney(*j);
	}

	for (ITOR_TEAM k = m_ltTeam.begin(); k != m_ltTeam.end(); ++k)
	{
		CArenaTeam::HASH_GAMBLER mpGambler = (*k)->GetGamblerSet();

		for (CArenaTeam::ITOR_GAMBLER l = mpGambler.begin(); l != mpGambler.end(); ++l)
		{
			CHARLIST* pTemp = l->first;
			if (!pTemp) continue;
			const DWORD dwBattingMoney = l->second;
			const DWORD dwBankMoney = pTemp->GetBankMoney();
			pGambler->SendCharInfoBasic(BANKMONEY, dwBankMoney - dwBattingMoney);

			const DWORD dwDividendMoney = m_pGambling->GetDividendRate(*k) + dwBattingMoney;
			
			memset(&packet, 0, sizeof(t_arena_dividend_info));
			packet.h.header.type = CMD_ARENA_DIVIDEND_INFO;
			packet.h.header.size = sizeof(t_arena_dividend_info);
			packet.u.arena.arena_dividend_info.dwBattingMoney = dwBattingMoney;
			packet.u.arena.arena_dividend_info.dwDividendMoney = dwDividendMoney;
			::QueuePacket(connections, pTemp->GetServerID(), &packet, 1);
		}
	}
}

void CBaseArena::RequestProbability()
{
	m_pState->InitTeamLevel();

	for (ITOR_TEAM i = m_ltTeam.begin(); i != m_ltTeam.end(); ++i)
	{
		m_pState->SumTeamLevel(*i);
	}

	t_packet packet;
	packet.h.header.type = CMD_ARENA_PROBABILITY_INFO;
	packet.h.header.size = sizeof(t_arena_probability_info);

	for (ITOR_TEAM j = m_ltTeam.begin(); j != m_ltTeam.end(); ++j)
	{
		packet.u.arena.arena_probability_info.nArenaTeam = (*j)->GetTeamNumber();
		packet.u.arena.arena_probability_info.fProbability = m_pState->GetTeamProbability(*j);
		SendPacketAll(&packet);
	}
}

void CBaseArena::StartReady(DWORD dwTime)
{	//< CSD-TW-030618
	if (!IsEnableReady())
	{
		m_pLeader->Message(MK_INFORMATION, 1, 122);
		return;
	}

	m_pState->SetReadyTime(dwTime);
	m_pState->ReadyState(g_curr_time);
}	//> CSD-TW-030618

void CBaseArena::EndReady()
{
	m_pState->SetReadyTime(0);

	if (m_pLeader != NULL)
	{
		SendStartGame(m_pLeader);
	}
}

bool CBaseArena::ReadyGame(DWORD dwTime)
{
	DWORD dwReadyTime = m_pState->GetReadyTime(dwTime);

	if (dwReadyTime <= 0)
	{
		return false;
	}

	t_packet packet;
	packet.h.header.type = CMD_ARENA_READY_INFO;
	packet.h.header.size = sizeof(t_arena_ready_info);
	packet.u.arena.arena_ready_info.dwReadyTime = dwReadyTime;
	SendPacketAll(&packet);
	return true;
}

void CBaseArena::StartGame(DWORD dwTime)
{
#ifdef LMS_MAPSERVER
	t_packet packet;
	packet.h.header.type = CMD_CALL_SMALLMENU;
	packet.u.server_call_smallmenu.menu = 299; // Must be the last menu number + 1
	packet.u.server_call_smallmenu.storelistindex = 0;
	packet.h.header.size = sizeof( t_server_call_smallmenu );
	SendPacketAll(&packet);
#else
	t_packet packet;
	packet.h.header.type = CMD_FIGHT_MAP_START;
	packet.h.header.size = 0;
	SendPacketAll(&packet);
#endif
	m_pState->InitTeamLevel();
	m_pGambling->InitBattingMoney();

	for (ITOR_TEAM i = m_ltTeam.begin(); i != m_ltTeam.end(); ++i)
	{
		CArenaTeam* pTeam = *i;
		
		if (!m_pGambling->IsEnableBattingMoney(pTeam))
		{	//< CSD-031119
			//pMember->Message(MK_SHORTAGE, 1, 112);
			return;
		}	//> CSD-031119

		pTeam->StartGame();

		m_pState->SumTeamLevel(pTeam);
		m_pGambling->SumBattingMoney(pTeam);
		m_pGambling->CalcBattingMoney(pTeam);
		m_pObserve->CalcObserveMoney(pTeam);
	}

	m_pState->StartState(dwTime);
}

void CBaseArena::EndGame(CArenaInfo* pArena)
{
	int nX = 0, nY = 0;
	pArena->GetWaitPosition(nX, nY);

	m_pLobby->Clear();

	for (ITOR_TEAM i = m_ltTeam.begin(); i != m_ltTeam.end(); ++i)
	{
		CArenaTeam::LIST_MEMBER ltMember = (*i)->GetMemberSet();

		for (CArenaTeam::ITOR_MEMBER j = ltMember.begin(); j != ltMember.end(); ++j)
		{
			::CallClient((*j)->GetServerID(), CMD_FIGHT_MAP_END);
		}
	}

	for (ITOR_TEAM k = m_ltTeam.begin(); k != m_ltTeam.end(); ++k)
	{
		(*k)->EndGame(nX, nY);
	}

	m_pState->EndState();
	m_pGambling->InitBattingMoney();
}

bool CBaseArena::PlayGame(DWORD dwTime)
{
	int nCheck = m_pState->CheckState(dwTime);
	
	if (nCheck >= 0)
	{
		SendPacketTeam(CMD_FIGHT_MAP_REMAIN_TIME, &nCheck, 1);
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////
// Protected Method
///////////////////////////////////////////////////////////////////////////////

void CBaseArena::DoResult(CArenaTeam* pTeam)
{
	switch (pTeam->GetGameResult())
	{
	case FST_WIN:
		{
			DoVictory(pTeam);
			break;
		}
	case FST_DEFEAT:
		{
			DoDefeat(pTeam);
			break;
		}
	}
}

void CBaseArena::DoVictory(CArenaTeam* pTeam)
{
	t_packet packet;
	packet.h.header.type = CMD_FIGHT_MAP_YOU_WIN;
	packet.h.header.size = 0;
	pTeam->SendPacket(&packet);

	m_pState->CalcWinTeamLaderScore(pTeam);
	m_pGambling->CalcDividendMoney(pTeam);
}

void CBaseArena::DoDefeat(CArenaTeam* pTeam)
{
	t_packet packet;
	packet.h.header.type = CMD_FIGHT_MAP_YOU_DEFEAT;
	packet.h.header.size = 0;
	pTeam->SendPacket(&packet);

	m_pState->CalcLoseTeamLaderScore(pTeam);
}

void CBaseArena::DoMemberVictory(CHARLIST* pMember)
{
	t_packet packet;
	packet.h.header.type = CMD_FIGHT_MAP_YOU_WIN;
	packet.h.header.size = 0;
	QueuePacket(connections, pMember->GetServerID(), &packet, 1);
}

void CBaseArena::SendPacketAll(int nType, void* pMsg, int nSize)
{
	t_packet packet;
	packet.h.header.type = nType;
	packet.h.header.size = nSize;
	
	if (pMsg != NULL)
	{
		memcpy(packet.u.data, pMsg, nSize);
	}

	for (ITOR_TEAM i = m_ltTeam.begin(); i != m_ltTeam.end(); ++i)
	{
		(*i)->SendPacket(&packet);
	}

	m_pLobby->SendPacket(&packet);
}

void CBaseArena::SendPacketTeam(t_packet* pPacket)
{
	for (ITOR_TEAM i = m_ltTeam.begin(); i != m_ltTeam.end(); ++i)
	{
		(*i)->SendPacket(pPacket);
	}
}

void CBaseArena::SendPacketTeam(int nType, void* pMsg, int nSize)
{
	t_packet packet;
	packet.h.header.type = nType;
	packet.h.header.size = nSize;
	
	if (pMsg != NULL)
	{
		memcpy(packet.u.data, pMsg, nSize);
	}

	for (ITOR_TEAM i = m_ltTeam.begin(); i != m_ltTeam.end(); ++i)
	{
		(*i)->SendPacket(&packet);
	}
}

void CBaseArena::SendPacketWait(t_packet* pPacket)
{
	m_pLobby->SendPacket(pPacket);
}

void CBaseArena::SendPacketWait(int nType, void* pMsg, int nSize)
{
	t_packet packet;
	packet.h.header.type = nType;
	packet.h.header.size = nSize;
	
	if (pMsg != NULL)
	{
		memcpy(packet.u.data, pMsg, nSize);
	}

	m_pLobby->SendPacket(&packet);
}

int CBaseArena::GetGameMemberCount()
{	//< CSD-TW-030701
	int nCount = m_pLobby->GetMemberCount();

	for (ITOR_TEAM i = m_ltTeam.begin(); i != m_ltTeam.end(); ++i)
	{
		nCount += (*i)->GetMemberCount();
	}

	return nCount;
}	//> CSD-TW-030618

///////////////////////////////////////////////////////////////////////////////
// Private Method
///////////////////////////////////////////////////////////////////////////////

bool CBaseArena::IsEnableReady()
{	//< CSD-TW-030618
	int nCount = 0;

	for (ITOR_TEAM i = m_ltTeam.begin(); i != m_ltTeam.end(); ++i)
	{
		if ((*i)->IsEmptyMember())
		{
			return false;
		}
		
		nCount = __max((*i)->GetMemberCount(), nCount);
	}
#ifdef LMS_MAPSERVER
	if (GetArenaType() == AT_LMS)
	{
		if (nCount < 2 || nCount > MAX_LMS_TEAM_NUMBER)
		{
			return false;
		}
	}
	else
#endif
	{
		if (nCount > MAX_TEAM_NUMBER)
		{
			return false;
		}
	}
/*
	Removed by Tya : Allow an uneven team (still don't allow empty teams ! :p)
	for (ITOR_TEAM j = m_ltTeam.begin(); j != m_ltTeam.end(); ++j)
	{
		if (nCount != (*j)->GetMemberCount())
		{
		 	return false;
		}
	}*/
	return true;
}	//> CSD-TW-030618

bool CBaseArena::DelayGame(DWORD dwTime)
{
	return true;
}