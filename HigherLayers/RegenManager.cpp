// RegenManager.cpp: implementation of the CRegenManager class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "RegenManager.h"

#include "..\HigherLayers\OP_Magic.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CRegenInfo::CRegenInfo(int nType) 
{
	m_nOrder = 0;
	m_nGroupNo = -1;
	m_nCurrentCount = nType;
	m_dwDelayTime = 0;
	m_dwRegenTime = 0;

	m_pMaster = NULL;
	m_ltMember.clear();
	m_vtMonster.clear();
	m_vtPlace.clear();
}

CRegenInfo::~CRegenInfo()
{
	m_ltMember.clear();
	m_vtMonster.clear();
	m_vtPlace.clear();
}

///////////////////////////////////////////////////////////////////////////////
// Public Mehtod
///////////////////////////////////////////////////////////////////////////////

void CRegenInfo::CalcPlaceOrder(int nCount)
{
	if (--m_nCurrentCount > 0)
	{
		return;
	}

	if (++m_nOrder > m_vtPlace.size() - 1)
	{
		m_nOrder = 0;
	}

	m_nCurrentCount = nCount;
}

void CRegenInfo::CalcRegenTime()
{
	m_dwRegenTime = global_time + m_dwDelayTime*1000;
}

void CRegenInfo::AddMaster(CHARLIST* pMonster)
{
	m_ltMember.push_back(pMonster);

	if (!IsExistMaster())
	{
		m_pMaster = pMonster;
	}
}

void CRegenInfo::DelMaster(CHARLIST* pMonster)
{
	if (IsExistMaster())
	{
		m_ltMember.remove(pMonster);
		m_pMaster = NULL;
	}
}

void CRegenInfo::AddMember(CHARLIST* pMonster)
{
	m_ltMember.push_back(pMonster);

	if (IsExistMaster())
	{	
		pMonster->SetMaster(m_pMaster->GetServerID());          // 리더을 설정
		m_pMaster->m_xSummon.Increase(pMonster->GetServerID()); // 리더가 부하로 추가
	}
}

void CRegenInfo::DelMember(CHARLIST* pMonster)
{
	m_ltMember.remove(pMonster);
}

void CRegenInfo::ClearMember()
{
	for (ITOR_MEMBER i = m_ltMember.begin(); i != m_ltMember.end(); ++i)
	{
		CHARLIST* pMonster = *i;
		if (!pMonster) continue;
		::DeleteNpc(pMonster->GetServerID());
	}

	m_pMaster = NULL;
	m_ltMember.clear();
}

void CRegenInfo::AddMonster(REGEN_MONSTER_INFO& rMonster)
{
	m_vtMonster.push_back(rMonster);
}

void CRegenInfo::AddPlace(REGEN_PLACE_INFO& rPlace)
{
	m_vtPlace.push_back(rPlace);
}

bool CRegenInfo::IsPassTime() const
{
	if (m_dwRegenTime > global_time)
	{
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CGroupInfo::CGroupInfo()
{
	Clear();
}

CGroupInfo::~CGroupInfo()
{
	ClearRegen();
}

///////////////////////////////////////////////////////////////////////////////
// Public Mehtod
///////////////////////////////////////////////////////////////////////////////

void CGroupInfo::Clear()
{
	m_ltRegen.clear();
	m_itCurrent = m_ltRegen.begin();
	m_nGroupType = 0;
	m_nEventType = 0;
}

void CGroupInfo::AddRegen(CRegenInfo* pRegen)
{
	m_ltRegen.push_back(pRegen);
}

void CGroupInfo::DelRegen(CRegenInfo* pRegen)
{
	m_ltRegen.remove(pRegen);
}
	
void CGroupInfo::ClearRegen()
{
	for_each(m_ltRegen.begin(), m_ltRegen.end(), TDelete());
	m_ltRegen.clear();
}

CRegenInfo* CGroupInfo::GetRegenInfo(int nGroup)
{
	for (ITOR_REGEN i = m_ltRegen.begin(); i != m_ltRegen.end(); ++i)
	{
		CRegenInfo* pRegen = *i;

		if (pRegen->GetGroupNumber() == nGroup)
		{
			return pRegen;
		}
	}

	return NULL;
}

void CGroupInfo::Ready()
{
	if (m_ltRegen.empty())
	{
		return;
	}

	m_itCurrent = m_ltRegen.begin();

	CRegenInfo* pRegen = (*m_itCurrent);

	for (int i = 0; i < pRegen->GetMonsterInfoSize(); ++i)
	{
		CreateMonster(pRegen, 0, i);
	}
}

void CGroupInfo::Regenerate()
{	// 몬스터 발생
	if (m_nGroupType == 0)
	{
		RegenerateAll();
	}
	else
	{
		RegenerateCycle();
	}
}

void CGroupInfo::Remove()
{
	for (ITOR_REGEN i = m_ltRegen.begin(); i != m_ltRegen.end(); ++i)
	{
		CRegenInfo* pRegen = *i;
		pRegen->ClearMember();
	}
}

///////////////////////////////////////////////////////////////////////////////
// Protected Mehtod
///////////////////////////////////////////////////////////////////////////////

void CGroupInfo::RegenerateAll()
{	// 위치에 전부 몬스터가 발생
	if (m_ltRegen.empty())
	{
		return;
	}

	if (!(*m_itCurrent)->IsEmptyMember())
	{
		return;
	}

	if (!(*m_itCurrent)->IsPassTime())
	{
		return;
	}

	if (++m_itCurrent != m_ltRegen.end())
	{
	
	}
	else
	{
		m_itCurrent = m_ltRegen.begin();
	}

	CRegenInfo* pRegen = *m_itCurrent;

	for (int i = 0; i < pRegen->GetMonsterInfoSize(); ++i)
	{
		CreateMonster(pRegen, 0, i);
	}
}

void CGroupInfo::RegenerateCycle()
{	// 위치에 순환하며 몬스터가 발생
	for (ITOR_REGEN i = m_ltRegen.begin(); i != m_ltRegen.end(); ++i)
	{
		CRegenInfo* pRegen = *i;

		if (!pRegen->IsEmptyMember())
		{
			continue;
		}

		if (!pRegen->IsPassTime())
		{
			continue;
		}

		const int nPlace = pRegen->GetPlaceOrder();

		for (int j = 0; j < pRegen->GetMonsterInfoSize(); ++j)
		{
			CreateMonster(pRegen, nPlace, j);
		}

		pRegen->CalcPlaceOrder(m_nGroupType);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Private Mehtod
///////////////////////////////////////////////////////////////////////////////

void CGroupInfo::CreateMonster(CRegenInfo* pRegen, int nPlace, int nMonster)
{
	REGEN_PLACE_INFO infPlace = pRegen->GetPlaceInfo(nPlace);
	REGEN_MONSTER_INFO infMonster = pRegen->GetMonsterInfo(nMonster);
	
	const int nGroup = pRegen->GetGroupNumber();
	const int nCount = infMonster.nMonsterCount;
	const int nNo = infMonster.nMonsterNo;
	const int nType = infMonster.nMonsterType;
	
	for (int i = 0; i < nCount; ++i)
	{
		const int nIndex = ::GetAliveNPCList();

		if (nIndex < 0)
		{
			// CSD-040211 : MyLog(LOG_NORMAL,"No more create npc!!!");
			continue;
		}

		const int nRandomX = rand()%infPlace.nScope;
		const int nRandomY = rand()%infPlace.nScope;

		int nX = 0, nY = 0;
		
		if (nType == 1)
		{	// 보스 몬스터 생성 좌표설정
			nX = infPlace.nPosX;
			nY = infPlace.nPosY;
		}
		else
		{
			nX = infPlace.nPosX + ((rand()%2) ? -nRandomX:nRandomX);
			nY = infPlace.nPosY + ((rand()%2) ? -nRandomY:nRandomY);
		}

		if (::NPC_AdjustPosition(nNo, &nX, &nY) == 0)
		{	
			MyLog(LOG_NORMAL,"No more create place(%d, %d)!!!", nX, nY);
			continue;
		}

		if (::NPC_Create(nIndex, nNo, nX, nY, 0, 0, GT_HUNT_MONSTER, nGroup, nType) > 0)
		{
			CHARLIST* pMonster = &NPCList[nIndex];
			if (!pMonster) continue;

			switch (nType)
			{
			case 0:
			case 2: pRegen->AddMember(pMonster); break;
			case 1: pRegen->AddMaster(pMonster); break;
			}

			MyLog(LOG_NORMAL,"[%d]Regen Group Monster - No : %d, nX : %d, nY : %d", nGroup, nNo, nX, nY);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CRegenManager::CRegenManager() : m_bReady(false) // CSD-040319
{
	m_mpGroup.clear();
}

CRegenManager::~CRegenManager()
{
	ClearGroup();
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

bool CRegenManager::AddGroup(int nHunt, CGroupInfo* pGroup)
{
	ITOR_GROUP itor = m_mpGroup.find(nHunt);

	if (itor != m_mpGroup.end())
	{
		MyLog(LOG_NORMAL, "CRegenManager::AddGroup : exist Hunt");
		MyLog(LOG_NORMAL, ">>> Hunt : %d", nHunt);
		return false;
	}

	m_mpGroup.insert(PAIR_GROUP(nHunt, pGroup));
	return true;
}
	
bool CRegenManager::DelGroup(int nHunt)
{
	ITOR_GROUP itor = m_mpGroup.find(nHunt);

	if (itor != m_mpGroup.end())
	{
		CGroupInfo* pTemp = itor->second;
		// 
		m_mpGroup.erase(itor);

		if (pTemp != NULL)
		{
			delete pTemp;
		}

		return true;
	}

	MyLog(LOG_NORMAL, "CRegenManager::DelGroup : don't exist Hunt");
	MyLog(LOG_NORMAL, ">>> Hunt : %d", nHunt);
	return false;
}

void CRegenManager::ClearGroup()
{
	for (ITOR_GROUP i = m_mpGroup.begin(); i != m_mpGroup.end(); ++i)
	{
		delete i->second;	
	}

	m_mpGroup.clear();
}

CRegenInfo* CRegenManager::GetRegenInfo(int nGroup)
{
	for (ITOR_GROUP i = m_mpGroup.begin(); i != m_mpGroup.end(); ++i)
	{
		CGroupInfo* pGroup = i->second;
		CRegenInfo* pRegen = pGroup->GetRegenInfo(nGroup);
		
		if (pRegen != NULL)
		{
			return pRegen;
		}
	}

	return NULL;
}

void CRegenManager::Ready()
{	//> CSD-040319
	for (ITOR_GROUP i = m_mpGroup.begin(); i != m_mpGroup.end(); ++i)
	{
		CGroupInfo* pGroup = i->second;
		
		if (pGroup->GetEventType() == 0)
		{
			pGroup->Ready();
		}
	}

	if (!m_mpGroup.empty())
	{
		m_bReady = true;
	}
}	//> CSD-040319

void CRegenManager::Ready(int nHunt)
{
	if (!IsExistHunt(nHunt))
	{
		return;
	}

	m_mpGroup[nHunt]->Ready();
}

void CRegenManager::Regenerate()
{	//< CSD-040319 : 몬스터 발생
	if (!IsReady())
	{
		return;
	}

	for (ITOR_GROUP i = m_mpGroup.begin(); i != m_mpGroup.end(); ++i)
	{
		CGroupInfo* pGroup = i->second;

		if (pGroup->GetEventType() == 0)
		{
			pGroup->Regenerate();
		}
	}
}	//> CSD-040319

void CRegenManager::Regenerate(int nHunt)
{	// 몬스터 발생
	if (!IsExistHunt(nHunt))
	{
		return;
	}

	m_mpGroup[nHunt]->Regenerate();
}

void CRegenManager::Remove()
{
	for (ITOR_GROUP i = m_mpGroup.begin(); i != m_mpGroup.end(); ++i)
	{
		CGroupInfo* pGroup = i->second;
		pGroup->Remove();
	}
}

void CRegenManager::Remove(int nHunt)
{
	if (!IsExistHunt(nHunt))
	{
		return;
	}

	m_mpGroup[nHunt]->Remove();
}