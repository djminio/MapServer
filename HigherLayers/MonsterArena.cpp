// MonsterArena.cpp: implementation of the CMonsterArena class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "MonsterArena.h"

#include "RegenManager.h"
//#include "Kein_item.h"

///////////////////////////////////////////////////////////////////////////////
// Global Function
///////////////////////////////////////////////////////////////////////////////

extern int DeleteINVItem(CHARLIST *pChar,int nItemNumber,int nDelCount);

void SaveMonsterArena(int idGame, int nPoint, CArenaTeam::LIST_MEMBER& rMember)
{	//< CSD-030603
	char szFile[FILENAME_MAX];											
	sprintf_s(szFile, sizeof(szFile), "Event/MonsterArena.txt");
	
	FILE* fp = fopen(szFile, "at+");
	if (fp == NULL)  return;
	
	time_t nowTime;
	time(&nowTime);
	tm* pNow = localtime(&nowTime);

	const int nYear = pNow->tm_year + 1900;
	const int nMon  = pNow->tm_mon + 1;
	const int nDay  = pNow->tm_mday;
#ifdef BUFF_BUG_CHECKS // Finito 24/08/07 buff bug checks to take away 12 hours when time changes
	int nHour;
	if (pNow->tm_hour - g_hour >= 12 && g_hour != -1)
	{
		nHour = pNow->tm_hour - 12;
	}
	else
	{
		nHour = pNow->tm_hour;
	}
#else
	const int nHour = pNow->tm_hour;
#endif
	const int nMin  = pNow->tm_min;
	const int nSec  = pNow->tm_sec;

	fprintf(fp, "%04d-%02d-%02d ", nYear, nMon, nDay);
	fprintf(fp, "%02d:%02d ", nHour, nMin);
	fprintf(fp, "%2d %6d ", idGame, nPoint);

	for (CArenaTeam::ITOR_MEMBER i = rMember.begin(); i != rMember.end(); ++i)
	{
		fprintf(fp, "%s ", (*i)->Name);
	}

	fprintf(fp, "\n");
	fclose(fp);
}	//> CSD-030603

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CMonsterArena::CMonsterArena(int nIndex, int nType)
{
	m_nIndex = nIndex;
	m_nArenaType = nType;
#ifdef LMS_MAPSERVER
	m_lvl_min = 1;
	m_lvl_max = 150;
#endif
}

CMonsterArena::~CMonsterArena()
{

}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CMonsterArena::StartGame(DWORD dwTime)
{
	CBaseArena::StartGame(dwTime);

	for (ITOR_TEAM i = m_ltTeam.begin(); i != m_ltTeam.end(); ++i)
	{
		const int nTeam = (*i)->GetTeamNumber();
		SendKillCount(nTeam, 0);

		const int nHunt = (*i)->GetHuntNumber();
		g_pRegenManager->Ready(nHunt);
	}
}

void CMonsterArena::EndGame(CArenaInfo* pArena)
{
	//몬스터 아레나 최강자전 이벤트 soto-030605
	for (ITOR_TEAM t = m_ltTeam.begin(); t != m_ltTeam.end(); ++t)
	{
		CArenaTeam* pTeam = *t;
		if (pTeam == NULL) continue;

		CArenaTeam::LIST_MEMBER ltMember = pTeam->GetMemberSet();
		for (CArenaTeam::ITOR_MEMBER i = ltMember.begin(); i != ltMember.end(); ++i)
		{
			CHARLIST* pMember = *i;
			if (pMember == NULL) continue;
			
			//DeleteINVItem(pMember,10247,1); // 030617 kyo
			DeleteINVItem(pMember,10256,1); // 030617 kyo
		}
	}
	
	CBaseArena::EndGame(pArena);

	for (ITOR_TEAM i = m_ltTeam.begin(); i != m_ltTeam.end(); ++i)
	{
		const int nHunt = (*i)->GetHuntNumber();
		g_pRegenManager->Remove(nHunt);

		(*i)->ResetKillCount();
	}
}
	
bool CMonsterArena::PlayGame(DWORD dwTime)
{
	DWORD dwRemainTime = m_pState->GetRemainTime(dwTime);

	for (ITOR_TEAM i = m_ltTeam.begin(); i != m_ltTeam.end(); ++i)
	{
		const int nHunt = (*i)->GetHuntNumber();
		g_pRegenManager->Regenerate(nHunt);
	}

	if (dwRemainTime <= 0)
	{
		for (ITOR_TEAM j = m_ltTeam.begin(); j != m_ltTeam.end(); ++j)
		{
			DoResult(*j);
		}

		return false;
	}
	
	return CBaseArena::PlayGame(dwTime);
}

///////////////////////////////////////////////////////////////////////////////
// Protected Method
///////////////////////////////////////////////////////////////////////////////

void CMonsterArena::DoResult(CArenaTeam* pTeam)
{
	for (ITOR_TEAM i = m_ltTeam.begin(); i != m_ltTeam.end(); ++i)
	{
		CArenaTeam* pTemp = *i;

		if (pTemp == pTeam)
		{
			continue;
		}
			
		if (pTemp->GetKillCount() < pTeam->GetKillCount())
		{
			pTeam->SetGameResult(FST_WIN);
		}
		else
		{
			pTeam->SetGameResult(FST_DEFEAT);
		}
	}

	CBaseArena::DoResult(pTeam);
}

void CMonsterArena::DoVictory(CArenaTeam* pTeam)
{
	CBaseArena::DoVictory(pTeam);

	CArenaTeam::LIST_MEMBER ltMember = pTeam->GetMemberSet();

	for (CArenaTeam::ITOR_MEMBER i = ltMember.begin(); i != ltMember.end(); ++i)
	{
		CHARLIST* pMember = *i;
		if (pMember == NULL) continue;

		for (ITOR_BONUS j = m_ltBonus.begin(); j != m_ltBonus.end(); ++j)
		{
			if ((*j)->GiveBonus(pMember))
			{
				break;
			}
		}
	}

	::SaveMonsterArena(GetGameIndex(), pTeam->GetKillCount(), ltMember); // CSD-030603
}
	
void CMonsterArena::DoDefeat(CArenaTeam* pTeam)
{
	CBaseArena::DoDefeat(pTeam);
}