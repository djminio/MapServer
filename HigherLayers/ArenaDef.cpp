// ArenaDef.cpp: implementation of the CArenaDef class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "ArenaDef.h"

#include "TeamBattle.h"
#include "BaseArena.h"

extern CGambleItem g_GambleItem;
extern void CalcWinLoseScore(WINLOSEPOINT& win_lose, int wintype);
extern ItemAttr GenerateItem(const int iItemFullNo);
extern void SendViewType(CHARLIST *target, int view_Type, int continue_Time);
extern int SubtractMoney( DWORD money, CHARLIST *ch );
extern bool isLevelUp(CHARLIST*);
extern void SaveChangeBattingMoney(CHARLIST* pTarget, DWORD dwOldMoney, DWORD dwNewMoney);
extern void SaveChangeDividendMoney(CHARLIST* pTarget, DWORD dwOldMoney, DWORD dwNewMoney, bool bBank);

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CArenaInfo::CArenaInfo()
{
	m_nMapPort = 0;
	m_ptWait.x = 0;
	m_ptWait.y = 0;
}
	
CArenaInfo::~CArenaInfo()
{
	
}

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CArenaLobby::CArenaLobby()
{
	Clear();
}

CArenaLobby::~CArenaLobby()
{

}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CArenaLobby::Clear()
{
	m_ltMember.clear();
}

void CArenaLobby::AddMember(int nGame, CHARLIST* pMember)
{
	m_ltMember.push_back(pMember);
}

void CArenaLobby::DelMember(CHARLIST* pMember)
{
	m_ltMember.remove(pMember);
}

void CArenaLobby::ClearMember()
{
	m_ltMember.clear();
}

void CArenaLobby::SendPacket(t_packet* pPacket)
{
	for (ITOR_MEMBER i = m_ltMember.begin(); i != m_ltMember.end(); ++i)
	{
		::QueuePacket(connections, (*i)->GetServerID(), pPacket, 1);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CArenaTeam::CArenaTeam()
{
	Clear();
}
	
CArenaTeam::~CArenaTeam()
{
	ClearMember();
	ClearObserve();
	ClearGambler();
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CArenaTeam::Clear()
{
	ClearMember();
	ClearObserve();
	ClearGambler();

	m_ptBase.x = 0;
	m_ptBase.y = 0;
	m_nHuntNumber = 0;
	m_nKillCount = 0;
	m_nGameResult = FST_DEFEAT;
}

void CArenaTeam::AddMember(CHARLIST* pMember)
{
	m_ltMember.push_back(pMember);
}
	
void CArenaTeam::DelMember(CHARLIST* pMember)
{
	m_ltMember.remove(pMember);
}

void CArenaTeam::ClearMember()
{
	for (ITOR_MEMBER i = m_ltMember.begin(); i != m_ltMember.end(); ++i)
	{
		(*i)->SetJoinArenaGame(NULL);
		(*i)->SetJoinArenaTeam(NULL);
	}

	m_ltMember.clear();
}

void CArenaTeam::AddObserve(CHARLIST* pObserve)
{
	m_ltObserve.push_back(pObserve);
}

void CArenaTeam::DelObserve(CHARLIST* pObserve)
{
	m_ltObserve.remove(pObserve);
}

void CArenaTeam::ClearObserve()
{
	for (ITOR_MEMBER i = m_ltObserve.begin(); i != m_ltObserve.end(); ++i)
	{
		(*i)->SetJoinArenaGame(NULL);
		(*i)->SetJoinArenaTeam(NULL);
	}

	m_ltObserve.clear();
}

void CArenaTeam::AddGambler(CHARLIST* pGambler, DWORD dwMoney)
{
	ITOR_GAMBLER itor = m_mpGambler.find(pGambler);

	if (itor != m_mpGambler.end())
	{
		MyLog(LOG_NORMAL, "CArenaTeam::AddGambler : Duplicate %s", pGambler->Name);
		return;
	}

	m_mpGambler.insert(PAIR_GAMBLER(pGambler, dwMoney));
}

void CArenaTeam::DelGambler(CHARLIST* pGambler)
{
	ITOR_GAMBLER itor = m_mpGambler.find(pGambler);

	if (itor != m_mpGambler.end())
	{
		m_mpGambler.erase(itor);	
	}
	else
	{
		MyLog(LOG_NORMAL, "CArenaTeam::DelGambler : Don't Exist %s", pGambler->Name);
		return;
	}
}

void CArenaTeam::ClearGambler()
{
	for (ITOR_GAMBLER i = m_mpGambler.begin(); i != m_mpGambler.end(); ++i)
	{
		i->first->SetJoinArenaGame(NULL);
		i->first->SetJoinArenaTeam(NULL);
	}

	m_mpGambler.clear();
}

int CArenaTeam::GetAliveCount()
{
	int nCount = 0;

	for (ITOR_MEMBER i = m_ltMember.begin(); i != m_ltMember.end(); ++i)
	{
		if ((*i)->bAlive == ALIVE_)
		{
			++nCount;
		}
	}

	return nCount;
}
	
int CArenaTeam::GetDeadCount()
{
	int nCount = 0;

	for (ITOR_MEMBER i = m_ltMember.begin(); i != m_ltMember.end(); ++i)
	{
		if ((*i)->bAlive != ALIVE_)
		{
			++nCount;
		}
	}

	return nCount;
}

bool CArenaTeam::ChanageLeader(CHARLIST*& rLeader)
{
	if (m_ltMember.empty())
	{
		return false;
	}

	rLeader = (*m_ltMember.begin());
	return true;
}

void CArenaTeam::GiveBonus()
{
	const int nSize = m_ltMember.size();

	if (nSize <= 1 || nSize > 15) 
	{
		return;
	}

	const int nParam = ::ChoiceParam(g_game_rate_data.give_gamble_item_rate[nSize], 
		                             2, 
									 rand()%100 + 1);

	for (ITOR_MEMBER i = m_ltMember.begin(); i != m_ltMember.end(); ++i)
	{
		const int nItemNo = g_GambleItem.GetRandItem(GT_RESOURCE, nParam);
		
		if (nItemNo == 0) 
		{
			continue;
		}

		ItemAttr item = ::GenerateItem(nItemNo);
		
		if (item.item_no == 0) 
		{
			continue;
		}

		CHARLIST* pMember = *i;
		if (pMember == NULL) continue;
		::SendItemEventLog(&item, pMember->GetServerID(), SN_NOT_USER, SILT_GET_GUILD_WAR, 2);
		::MoveEmptyInv(&item, pMember);
	}
}

void CArenaTeam::StartGame()
{
	for (ITOR_MEMBER i = m_ltMember.begin(); i != m_ltMember.end(); ++i)
	{
		CHARLIST* pMember = *i;
		if (pMember == NULL) continue;
		pMember->Cp = pMember->CpMax;
		::MovePc(pMember->GetServerID(), m_ptBase.x, m_ptBase.y);
	}

	for (ITOR_MEMBER j = m_ltObserve.begin(); j != m_ltObserve.end(); ++j)
	{
		CHARLIST* pMember = *j;
		if (pMember == NULL) continue;
		::MovePc(pMember->GetServerID(), m_ptBase.x, m_ptBase.y);
	}

	m_nKillCount = 0;
}

void CArenaTeam::EndGame(int nX, int nY)
{
	for (ITOR_MEMBER i = m_ltMember.begin(); i != m_ltMember.end(); ++i)
	{
		CHARLIST* pMember = *i;
		if (pMember == NULL) continue;
		pMember->Cp = 0;
		SkillMgr.CharacterToAlive(pMember, 1);
		::MovePc(pMember->GetServerID(), nX, nY);
		::CheckAndSendChangeEquip(pMember, 1);
	}

	for (ITOR_MEMBER j = m_ltObserve.begin(); j != m_ltObserve.end(); ++j)
	{
		CHARLIST* pObserve = *j;
		if (pObserve == NULL) continue;
		::CallClient(pObserve->GetServerID(), CMD_FIGHT_MAP_END);

		pObserve->viewtype = VIEWTYPE_NORMAL_;
		::SendViewType(pObserve, VIEWTYPE_NORMAL_, 0);
		::MovePc(pObserve->GetServerID(), nX, nY);
	}
	
	ClearMember();
	ClearObserve();
	ClearGambler();

	m_nKillCount = 0;
}

void CArenaTeam::SendPacket(t_packet* pPacket)
{
	for (ITOR_MEMBER i = m_ltMember.begin(); i != m_ltMember.end(); ++i)
	{
		::QueuePacket(connections, (*i)->GetServerID(), pPacket, 1);
	}
}

void CArenaTeam::SendPacket(int idTarget, int nType)
{
	t_packet packet;
	packet.h.header.type = nType;
	packet.h.header.size = sizeof(k_team_add_message);
		
	for (ITOR_MEMBER i = m_ltMember.begin(); i != m_ltMember.end(); ++i)
	{
		packet.u.kein.team_add_message.total_id = (*i)->total_id;
		packet.u.kein.team_add_message.team_num = m_nTeamNumber;
		packet.u.kein.team_add_message.id = (*i)->GetServerID();
		strcpy_s(packet.u.kein.team_add_message.name, sizeof(packet.u.kein.team_add_message.name), (*i)->Name);
		::QueuePacket(connections, idTarget, &packet, 1);
	}
}	

///////////////////////////////////////////////////////////////////////////////
// Private Method
///////////////////////////////////////////////////////////////////////////////

int CArenaTeam::GetTeamLevel()
{
	int nTeamLevel = 0;

	for (ITOR_MEMBER i = m_ltMember.begin(); i != m_ltMember.end(); ++i)
	{
		nTeamLevel += (*i)->GetLevel(); // CSD-030806
	}

	return nTeamLevel;
}

DWORD CArenaTeam::GetBattingMoney()
{
	DWORD dwBattingMoney = 0;

	for (ITOR_GAMBLER i = m_mpGambler.begin(); i != m_mpGambler.end(); ++i)
	{
		dwBattingMoney += i->second;
	}

	return dwBattingMoney;
}

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CArenaState::CArenaState()
{
	m_nState = CS_NOW_PEACE;
	m_nCheck = 0;
	m_dwReadySecond = 0;
	m_dwPlayMinute = 0;
	m_dwPrepareTime = 0;
	m_dwContinueTime = 0;
	m_nTotalLevel = 0;
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CArenaState::InitTeamLevel()
{
	m_nTotalLevel = 0;
}

void CArenaState::SumTeamLevel(CArenaTeam* pTeam)
{
	const int nTeamLevel = pTeam->GetTeamLevel();
	m_nTotalLevel += nTeamLevel;
}

void CArenaState::ReadyState(DWORD dwTime)
{
	m_dwPrepareTime = dwTime + m_dwReadySecond;
}

void CArenaState::StartState(DWORD dwTime)
{
	m_nState = CS_NOW_FIGHT;
	m_dwContinueTime = dwTime + m_dwPlayMinute*60; 
	
	DWORD dwRemainTime = GetRemainTime(dwTime);

	for (int i = 0; i < 4; ++i)
	{
		if (dwRemainTime > s_aCheckTime[i])
		{
			m_nCheck = i;
			return;
		}
	}
}

void CArenaState::EndState()
{
	m_nState = CS_NOW_PEACE;
	m_nCheck = 0;
	m_dwContinueTime = 0;
}

int CArenaState::CheckState(DWORD dwTime)
{
	DWORD dwRemainTime = GetRemainTime(dwTime);

	if (dwRemainTime >= 20 && dwRemainTime < s_aCheckTime[m_nCheck])
	{
		return ++m_nCheck;
	}

	return -1;
}

DWORD CArenaState::GetReadyTime(DWORD dwTime) const
{
	return (m_dwPrepareTime <= dwTime) ? 0:m_dwPrepareTime - dwTime;
}

DWORD CArenaState::GetRemainTime(DWORD dwTime) const
{
	return (m_dwContinueTime <= dwTime) ? 0:m_dwContinueTime - dwTime;
}

DWORD CArenaState::GetFlowTime(DWORD dwTime) const
{
	return m_dwPlayMinute*60 - (m_dwContinueTime - dwTime);
}

void CArenaState::CalcWinTeamLaderScore(CArenaTeam* pTeam)
{
	const int nPoint = GetWinTeamLaderScore(pTeam);

	CArenaTeam::LIST_MEMBER ltMember = pTeam->GetMemberSet();

	for (CArenaTeam::ITOR_MEMBER i = ltMember.begin(); i != ltMember.end(); ++i)
	{
		CHARLIST* pMember = *i;
		if (pMember == NULL) continue;
		::SaveChangeLadder(pMember, pMember->LadderScore, pMember->LadderScore + nPoint, CLT_WIN_COLOSSUS);
		pMember->LadderScore += nPoint;
		pMember->refresh_ladder = true;
		::CalcWinLoseScore(pMember->WinLoseScore, FST_WIN);
	}
}

void CArenaState::CalcLoseTeamLaderScore(CArenaTeam* pTeam)
{
	int nPoint = GetLoseTeamLaderScore(pTeam);

	CArenaTeam::LIST_MEMBER ltMember = pTeam->GetMemberSet();

	for (CArenaTeam::ITOR_MEMBER i = ltMember.begin(); i != ltMember.end(); ++i)
	{
		CHARLIST* pMember = *i;
		if (pMember == NULL) continue;
	
		if (pMember->LadderScore < nPoint)
		{
			nPoint = pMember->LadderScore;
		}
		// 020830 YGI
		::SaveChangeLadder(pMember, pMember->LadderScore, pMember->LadderScore - nPoint, CLT_LOSE_COLOSSUS);

		pMember->LadderScore -= nPoint;
		pMember->refresh_ladder = true;
		::CalcWinLoseScore(pMember->WinLoseScore, FST_DEFEAT);
	}
}

float CArenaState::GetTeamProbability(CArenaTeam* pTeam)
{
	if (m_nTotalLevel == 0)
	{
		return 0.0;
	}

	return pTeam->GetTeamLevel()*100.0/m_nTotalLevel;
}

bool CArenaState::IsReadyGame() const
{ 
	return (m_dwReadySecond != 0) ? true:false;
}

bool CArenaState::IsPlayGame() const
{ 
	return (m_nState == CS_NOW_FIGHT) ? true:false;
}

///////////////////////////////////////////////////////////////////////////////
// Private Method
///////////////////////////////////////////////////////////////////////////////

int CArenaState::GetWinTeamLaderScore(CArenaTeam* pTeam)
{
	const int nTeamLevel = pTeam->GetTeamLevel();
	float fTeamRate = (float)(m_nTotalLevel - nTeamLevel)/nTeamLevel;

	if (m_nTotalLevel == 0)
	{
		fTeamRate = 1.0;
	}
	
	bool bHiLevel = (nTeamLevel >= m_nTotalLevel - nTeamLevel) ? true:false;

	int nCorrection = 10;

	if (fTeamRate > 2.0)
	{
		nCorrection = 20;
		fTeamRate = 1.0;
	}
	else if (fTeamRate < 0.5)
	{
		nCorrection = 5;
		fTeamRate = 1.0;
	}
	else 
	{
		if (bHiLevel)
		{
			fTeamRate = pow(fTeamRate, (float)1.2);
		}
		else
		{
			fTeamRate = pow(fTeamRate, (float)1.5);
		}
	}

	return fTeamRate*5*nCorrection;
}

int CArenaState::GetLoseTeamLaderScore(CArenaTeam* pTeam)
{
	const int nTeamLevel = pTeam->GetTeamLevel();
	float fTeamRate = (float)(m_nTotalLevel - nTeamLevel)/nTeamLevel;

	if (m_nTotalLevel == 0)
	{
		fTeamRate = 1.0;
	}
	
	bool bHiLevel = (nTeamLevel >= m_nTotalLevel - nTeamLevel) ? true:false;

	int nCorrection = 10;

	if (fTeamRate > 2.0)
	{
		nCorrection = 20;
		fTeamRate = 1.0;
	}
	else if (fTeamRate < 0.5)
	{
		nCorrection = 5;
		fTeamRate = 1.0;
	}
	else 
	{
		if (bHiLevel)
		{
			fTeamRate = pow(fTeamRate, (float)1.5);
		}
		else
		{
			fTeamRate = pow(fTeamRate, (float)1.2);
		}
	}

	return (fTeamRate*5 - 1)*nCorrection;
}

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
	
CArenaGambling::CArenaGambling()
{
	m_dwMaxBattingMoney = 0;
	m_dwMinBattingMoney = 0;
	m_dwBattingMoney = 0;
	m_fDividendRate = 0.0;
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CArenaGambling::InitBattingMoney()
{
	m_dwBattingMoney = 0;
}

void CArenaGambling::SumBattingMoney(CArenaTeam* pTeam)
{
	const DWORD dwTeamMoney = pTeam->GetBattingMoney();
	m_dwBattingMoney += dwTeamMoney;
}

void CArenaGambling::CalcBattingMoney(CArenaTeam* pTeam)
{	//< CSD-030723
	CArenaTeam::HASH_GAMBLER mpGambler = pTeam->GetGamblerSet();

	for (CArenaTeam::ITOR_GAMBLER i = mpGambler.begin(); i != mpGambler.end(); ++i)
	{	
		CHARLIST* pGambler = i->first;
		if (pGambler == NULL) continue;

		const DWORD dwOldMoney = pGambler->GetBankMoney();
		pGambler->DecBankMoney(i->second, BMCT_ARENA_GAMBLE_BETTING);

		const DWORD dwNewMoney = pGambler->GetBankMoney();
		::SaveChangeBattingMoney(pGambler, dwOldMoney, dwNewMoney);
	}
}	//> CSD-030723

void CArenaGambling::CalcDividendMoney(CArenaTeam* pTeam)
{	//< CSD-030723
	CArenaTeam::HASH_GAMBLER mpGambler = pTeam->GetGamblerSet();

	for (CArenaTeam::ITOR_GAMBLER i = mpGambler.begin(); i != mpGambler.end(); ++i)
	{
		CHARLIST* pGambler = i->first;
		if (pGambler == NULL) continue;

		const DWORD dwDividendMoney = GetDividendRate(pTeam) + i->second;

		if (pGambler->GetBankMoney() + dwDividendMoney < MAX_BANK_MONEY)
		{
			const DWORD dwOldMoney = pGambler->GetBankMoney();
			pGambler->IncBankMoney(dwDividendMoney, BMCT_ARENA_GAMBLE_DIVIDEND);

			const DWORD dwNewMoney = pGambler->GetBankMoney();
			::SaveChangeDividendMoney(pGambler, dwOldMoney, dwNewMoney, true);
			::SendGamblingResult(pGambler, 0, dwDividendMoney);
		}
		else
		{
			const DWORD dwOldMoney = ::GetMoneyByItem(pGambler);
			::GiveMoney(pGambler, dwDividendMoney);
			
			const DWORD dwNewMoney = ::GetMoneyByItem(pGambler);
			::SaveChangeDividendMoney(pGambler, dwOldMoney, dwNewMoney, false);
			::SendGamblingResult(pGambler, 1, dwDividendMoney);
		}
	}
}	//> CSD-030723

DWORD CArenaGambling::GetDividendRate(CArenaTeam* pTeam)
{
	const int nTeamCount = pTeam->GetGamblerCount();
	
	if (nTeamCount == 0)
	{
		return 0;
	}
	
	const DWORD dwTeamMoney = pTeam->GetBattingMoney();
	return ((m_dwBattingMoney - dwTeamMoney)*m_fDividendRate)/nTeamCount;
}

bool CArenaGambling::IsEnableGamblerCount(CArenaTeam* pTeam) const
{
	if (pTeam->GetGamblerCount() > MAX_MEMBER_COUNT)
	{
		return false;
	}

	return true;
}

bool CArenaGambling::IsEnableBattingMoney(CArenaTeam* pTeam) const
{	//< CSD-031119
	CArenaTeam::HASH_GAMBLER mpGambler = pTeam->GetGamblerSet();

	for (CArenaTeam::ITOR_GAMBLER i = mpGambler.begin(); i != mpGambler.end(); ++i)
	{	
		CHARLIST* pGambler = i->first;
		if (pGambler == NULL) continue;
		
		if (pGambler->GetBankMoney() < i->second)
		{
			return false;
		}
	}

	return true;
}	//> CSD-031119
	
bool CArenaGambling::IsEnableBattingMoney(DWORD dwMoney) const
{
	if (m_dwMinBattingMoney > dwMoney)
	{
		return false;
	}

	if (m_dwMaxBattingMoney < dwMoney)
	{
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CArenaObserve::CArenaObserve()
{
	m_dwObserveMoney = 0;
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CArenaObserve::CalcObserveMoney(CArenaTeam* pTeam)
{
	CArenaTeam::LIST_MEMBER ltObserve = pTeam->GetObserveSet();

	for (CArenaTeam::ITOR_MEMBER i = ltObserve.begin(); i != ltObserve.end(); ++i)
	{
		CHARLIST* pObserve = *i;
		if (pObserve == NULL) continue;
		pObserve->viewtype = VIEWTYPE_OBSERVE_;
		::SendViewType(pObserve, VIEWTYPE_OBSERVE_, 0);
		::SubtractMoney(m_dwObserveMoney, pObserve);
	}
}

bool CArenaObserve::IsEnoughMoney(CHARLIST* pObserve) const
{
	if (::GetMoneyByItem(pObserve) < m_dwObserveMoney)
	{
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CArenaBonus::CArenaBonus()
{
	Clear();
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CArenaBonus::Clear()
{
	m_nMinLevel = 0;
	m_nMaxLevel = 0;
	m_nItemHouse = 0;
	m_nItemKind = 0;
	m_nItemGrade = 0;
	m_fItemGainRate = 0.0;
	m_fExpRate = 0.0;
}

bool CArenaBonus::GiveBonus(CHARLIST* pMember)
{
	const int nLevel = pMember->GetLevel(); // CSD-030806

	if (m_nMinLevel > nLevel)
	{
		return false;
	}

	if (m_nMaxLevel < nLevel)
	{
		return false;
	}

	AddExperience(pMember);
	AddGambleItem(pMember);
	return true;
}

///////////////////////////////////////////////////////////////////////////////
// Private Method
///////////////////////////////////////////////////////////////////////////////

void CArenaBonus::AddExperience(CHARLIST* pMember)
{	//< CSD-030806
	const int nLevel = pMember->GetLevel();
	pMember->ChangeUpAddExpBonus((NPC_Lev_Ref[nLevel].nNeedExp/100)*m_fExpRate);
	::isLevelUp(pMember);
}	//> CSD-030806

void CArenaBonus::AddGambleItem(CHARLIST* pMember)
{	
	ItemAttr item;
	
	if (rand()%1000 > int(m_fItemGainRate)*10)
	{
		const int nParam = ::ChoiceParam(g_game_rate_data.give_gamble_item_rate[15],
										 2, 
										 rand()%100 + 1);
		const int nItemNo = g_GambleItem.GetRandItem(GT_RESOURCE, nParam);
		
		if (nItemNo == 0) 
		{
			return;
		}

		item = ::GenerateItem(nItemNo);
	}
	else
	{
		const int nItem = g_GambleItem.GetRandItem(m_nItemHouse, 0);
			
		if (nItem == 0) 
		{
			return;
		}

		RareMain r;

		switch (m_nItemKind)
		{
		case H_LV_NO_HIGH_ITEM:
			{
				item = ItemMgr.GiveRareItem(nItem, 
											0, 
											RARE_ABLE, 
											1,
											1, m_nItemGrade, 
											r, 
											H_LV_NO_HIGH_ITEM);
				break;
			}
		case H_LV_HIGH_ITEM:
			{
				item = ItemMgr.GiveRareItem(nItem, 
											0,
											1,
											RARE_ABLE, 
											1, m_nItemGrade,
											r, 
											H_LV_HIGH_ITEM);
				break;
			}
		default:
			{
				return;
			}
		}
	}

	if (item.item_no == 0) 
	{
		return;
	}

	::SendItemEventLog(&item, pMember->GetServerID(), SN_NOT_USER, SILT_GET_GUILD_WAR, 2);
	::MoveEmptyInv(&item, pMember);
}