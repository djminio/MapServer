// LMSArena.cpp: implementation of the CLMSArena class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "LMSArena.h"
#include "Chatting.h" // For counselor lms chat

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CLMSArena::CLMSArena(int nIndex, int nType)
{
	m_nIndex = nIndex;
	m_nArenaType = nType;
	m_dwDelaySecond = 10;
}

CLMSArena::~CLMSArena()
{

}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CLMSArena::StartGame(DWORD dwTime)
{
	g_block_attack = true;
	g_block_magic = true;
	m_dwDelaySecond = 10;
#ifdef LMS_MAPSERVER
	m_lvl_min = 1;
	m_lvl_max = 150;
#endif

	CBaseArena::StartGame(dwTime);

	for (ITOR_TEAM i = m_ltTeam.begin(); i != m_ltTeam.end(); ++i)
	{
		const int nTeam = (*i)->GetTeamNumber();
		const int nCount = (*i)->GetAliveCount();
		SendRemainCount(nTeam, nCount);
	}
}

void CLMSArena::EndGame(CArenaInfo* pArena)
{
	CBaseArena::EndGame(pArena);
}
	
bool CLMSArena::PlayGame(DWORD dwTime)
{
	DWORD dwRemainTime = m_pState->GetRemainTime(dwTime);

	for (ITOR_TEAM i = m_ltTeam.begin(); i != m_ltTeam.end(); ++i)
	{
		CArenaTeam* pTeam = *i;
		if (pTeam == NULL) continue;

		const int nTeam = pTeam->GetTeamNumber();
		const int nCount = pTeam->GetAliveCount();
		SendRemainCount(nTeam, nCount);

		if (nCount <= 1)
		{
			// Find who winner of LMS is
			CArenaTeam::LIST_MEMBER ltMember = pTeam->GetMemberSet();
			for (CArenaTeam::ITOR_MEMBER i = ltMember.begin(); i != ltMember.end(); ++i)
			{
				if ((*i)->bAlive == ALIVE_)
				{
					char temp[TEXT_LENGTH];
					sprintf_s(temp, sizeof(temp), "LMS: %s is the winner!", (*i)->Name);
					string chatData(temp);
					SendCounselorChat(chatData, CHATTYPE_LMS);
					DoMemberVictory(*i);
				}
			}
			return false; // Ends the game!
		}
	}

	return CBaseArena::PlayGame(dwTime);
}

bool CLMSArena::DelayGame(DWORD dwTime)
{
	if (m_dwDelaySecond <= 0)
	{
		return true;
	}
	else
	{
		--m_dwDelaySecond;
		if (m_dwDelaySecond == 0)
		{
			t_packet packet;
			packet.h.header.type = CMD_FIGHT_MAP_START;
			packet.h.header.size = 0;
			SendPacketAll(&packet);

			g_block_attack = false;
			g_block_magic = false;
			return true;
		}
	}

	return false;
}