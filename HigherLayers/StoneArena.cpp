// StoneArena.cpp: implementation of the CStoneArena class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "StoneArena.h"
#include "TeamBattle.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CStoneArena::CStoneArena(int nIndex, int nType)
{
	m_nIndex = nIndex;
	m_nArenaType = nType;
#ifdef LMS_MAPSERVER
	m_lvl_min = 1;
	m_lvl_max = 150;
#endif
}

CStoneArena::~CStoneArena()
{

}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CStoneArena::StartGame(DWORD dwTime)
{
	CBaseArena::StartGame(dwTime);

	g_ColossusStone.SetStoneFirst();
	// 현재 돌의 수를 카운트 해서 보내준다.
	g_ColossusStone.GetTeamCount(m_aStone);

	for (int i = 0; i < 2; ++i)
	{
		SendOccupyStone(i, m_aStone[i]);
	}
}

void CStoneArena::EndGame(CArenaInfo* pArena)
{
	CBaseArena::EndGame(pArena);
}
	
bool CStoneArena::PlayGame(DWORD dwTime)
{
	DWORD dwRemainTime = m_pState->GetRemainTime(dwTime);
	
	g_ColossusStone.GetTeamCount(m_aStone);

	for (int i = 0; i < 2; ++i)
	{
		SendOccupyStone(i, m_aStone[i]);
	}
	
	if (dwRemainTime <= 0 || g_ColossusStone.IsEnd())
	{
		int bottom_level = 1000000;		// 가장 낮은 레벨
		int top = -1, nIndex = 0;
		int the_end = 0;

		for (ITOR_TEAM i = m_ltTeam.begin(); i != m_ltTeam.end(); ++i)
		{
			const int nTotal = (*i)->GetTeamLevel();

			if (top < m_aStone[nIndex])
			{
				top = m_aStone[nIndex];
				the_end = 1;
			}
			else if (top == m_aStone[nIndex])		// 동점	 // 팀이 2개 일때만 적용
			{
				if (bottom_level > nTotal) 
				{
					bottom_level = nTotal;
				}
			}

			++nIndex;
		}
		
		nIndex = 0;

		for (ITOR_TEAM j = m_ltTeam.begin(); j != m_ltTeam.end(); ++j)
		{
			const int nTotal = (*j)->GetTeamLevel();

			if (top < m_aStone[nIndex])
			{
				top = m_aStone[nIndex];
				the_end = 1;
			}
			else if (top == m_aStone[nIndex])	// 동점	 // 팀이 2개 일때만 적용
			{
				if (bottom_level > nTotal) 
				{
					bottom_level = nTotal;
				}
			}

			++nIndex;
		}

		if (the_end)		// 승패가 갈렸다.
		{
			nIndex = 0;

			for (ITOR_TEAM k = m_ltTeam.begin(); k != m_ltTeam.end(); ++k)
			{
				CArenaTeam* pTeam = *k;

				if (top == m_aStone[nIndex++]) // 이긴팀
				{
					pTeam->SetGameResult(FST_WIN);
				}
				else
				{
					pTeam->SetGameResult(FST_DEFEAT);
				}

				DoResult(pTeam);
			}
		}

		return false;
	}

	return CBaseArena::PlayGame(dwTime);
}

///////////////////////////////////////////////////////////////////////////////
// Protected Method
///////////////////////////////////////////////////////////////////////////////

void CStoneArena::DoResult(CArenaTeam* pTeam)
{
	CBaseArena::DoResult(pTeam);
}

void CStoneArena::DoVictory(CArenaTeam* pTeam)
{
	CBaseArena::DoVictory(pTeam);
}

void CStoneArena::DoDefeat(CArenaTeam* pTeam)
{
	CBaseArena::DoDefeat(pTeam);
}