// CoreArena.cpp: implementation of the CCoreArena class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "CoreArena.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CCoreArena::CCoreArena(int nIndex, int nType)
{
	m_nIndex = nIndex;
	m_nArenaType = nType;
	m_bGameOver = false;
#ifdef LMS_MAPSERVER
	m_lvl_min = 1; //Eleval 18/07/09 - LVL Range
	m_lvl_max = 150; //Eleval 18/07/09 - LVL Range
#endif
}

CCoreArena::~CCoreArena()
{

}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CCoreArena::StartGame(DWORD dwTime)
{
	CBaseArena::StartGame(dwTime);

	for (ITOR_TEAM i = m_ltTeam.begin(); i != m_ltTeam.end(); ++i)
	{
		const int nTeam = (*i)->GetTeamNumber();
		const int nCount = (*i)->GetAliveCount();
		SendRemainCount(nTeam, nCount);
	}

	m_bGameOver = false;
}

void CCoreArena::EndGame(CArenaInfo* pArena)
{
	CBaseArena::EndGame(pArena);
}
	
bool CCoreArena::PlayGame(DWORD dwTime)
{
	DWORD dwRemainTime = m_pState->GetRemainTime(dwTime);

	for (ITOR_TEAM i = m_ltTeam.begin(); i != m_ltTeam.end(); ++i)
	{
		const int nTeam = (*i)->GetTeamNumber();
		const int nCount = (*i)->GetAliveCount();
		SendRemainCount(nTeam, nCount);

		if (nCount <= 0 && !m_bGameOver)
		{
			m_pState->SetContinueTime(dwTime + 5);
			m_bGameOver = true;
			break;
		}
	}
	
	if (dwRemainTime <= 0)
	{
		/////////////////////////////////////////////////////////////////////////////
		//	승패 결정은 가장 많이 살아 있는 팀이 우승하고 혹시 
		// 살아 있는 수가 같을 경우 가장 낮은 레벨 팀이 우승한다.
		// 가장 많이 살아 있는 팀의 살아 남은 수와 레벨을 저장해두고 나중에 처리한다.
		/////////////////////////////////////////////////////////////////////////////
		int top = -1;		// 가장 많이 살아 있는 팀의 수 
		int bottom_level = 1000000;		// 가장 낮은 레벨
		bool is_over = false;	// 가장 많이 살아 있는 팀이 2팀 이상인가?

		for (ITOR_TEAM j = m_ltTeam.begin(); j != m_ltTeam.end(); ++j)
		{
			const int nCount = (*j)->GetAliveCount();
			const int nTotal = (*j)->GetTeamLevel();

			if (top < nCount)
			{
				top = nCount;
				is_over = true;
				bottom_level = nTotal;
			}
			else if (top == nCount)
			{
				if (bottom_level > nTotal) 
				{
					bottom_level = nTotal;
				}
			}			
		}

		if (is_over) // 남은 수가 같은 경우 레벨까지 체크한다.
		{
			for (ITOR_TEAM k = m_ltTeam.begin(); k != m_ltTeam.end(); ++k)
			{
				CArenaTeam* pTeam = *k;

				const int nCount = pTeam->GetAliveCount();
				const int nTotal = pTeam->GetTeamLevel();
				
				if (nCount == top && nTotal == bottom_level)
				{
					pTeam->SetGameResult(FST_WIN);
					pTeam->GiveBonus(); // 겜블 아이템을 준다.

					const DWORD dwFlowTime = m_pState->GetFlowTime(dwTime);
					
					if (dwFlowTime < 300)		// 5분 이내라면
					{
						MyLog(LOG_NORMAL, "Team Battle End!!! time = %d", dwFlowTime);
					}
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

void CCoreArena::DoResult(CArenaTeam* pTeam)
{
	CBaseArena::DoResult(pTeam);
}

void CCoreArena::DoVictory(CArenaTeam* pTeam)
{
	CBaseArena::DoVictory(pTeam);
}

void CCoreArena::DoDefeat(CArenaTeam* pTeam)
{
	CBaseArena::DoDefeat(pTeam);
}