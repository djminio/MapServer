// DualManager.cpp: implementation of the CDualManager class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "DualManager.h"
#include "Scrp_exe.h"
#include "CItem.h"
#include "LTSSupport.h"

extern inline int GetTotalAbility(LPCHARLIST ch);
extern void AddAbility(CHARLIST *ch, int type, int add);
extern int SendCharInfo(short int cn);
extern inline void	CallClient( short int id, short int type );

///////////////////////////////////////////////////////////////////////////////
// Global Memeber
///////////////////////////////////////////////////////////////////////////////

static int ADD_ABILITY[5][2] = 
{ 
	{STR,  CON},
	{DEX,  MOVP},
	{DEX,  CON},
	{INT_, WSPS},
	{WIS,  WSPS}
};

static int BASIC_ABILITY[5][11] = 
{
	{50, 10, 30, 10, 10, 10, 10, 10, 10, 10, 10},
	{10, 40, 20, 10, 10, 10, 20, 10, 10, 10, 20},
	{10, 50, 20, 10, 10, 10, 20, 10, 10, 10, 10},
	{10, 10, 15, 10, 40, 10, 10, 20, 10, 25, 10},
	{10, 10, 20, 35, 10, 10, 10, 20, 10, 25, 10},
};

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CDualDataInfo::CDualDataInfo()
{
	m_nMaxLevel = 0;
	m_nNeedLadder = 0;
	m_nNeedFame = 0;
	m_nConsumeLadder = 0;
	m_nConsumeFame = 0;
}

CDualDataInfo::~CDualDataInfo()
{

}

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CDualClassInfo::CDualClassInfo()
{ 
	m_ltMember.clear();
}

CDualClassInfo::~CDualClassInfo()
{
	m_ltMember.clear();
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CDualClassInfo::AddNew(WORD idMember)
{
	m_ltMember.push_back(idMember);
}
	
void CDualClassInfo::Delete(WORD idMember)
{
	m_ltMember.remove(idMember);
}

void CDualClassInfo::SendPacket(t_packet* pPacket)
{
	for (ITOR_MEMBER i = m_ltMember.begin(); i != m_ltMember.end(); ++i)
	{
		const WORD idMember = *i;
		CHARLIST* pMember = ::CheckServerId(idMember);

		if (pMember != NULL)
		{
			::QueuePacket(connections, idMember, pPacket, 1);
		}
	}
}

bool CDualClassInfo::IsExist(WORD idMember) const
{
	if (find(m_ltMember.begin(), m_ltMember.end(), idMember) != m_ltMember.end())
	{
		return true;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CDualManager::CDualManager()
{
	m_vtDualData.clear();
	m_vtDualData.reserve(MAX_DUAL_STEP);

	m_vtDualClass.clear();
	m_vtDualClass.reserve(MAX_DUAL_STEP);
}

CDualManager::~CDualManager()
{
	for_each(m_vtDualData.begin(), m_vtDualData.end(), TDelete());
	m_vtDualData.clear();

	for_each(m_vtDualClass.begin(), m_vtDualClass.end(), TDelete());
	m_vtDualClass.clear();
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CDualManager::AddDualData(CDualDataInfo* pDualData)
{
	m_vtDualData.push_back(pDualData);
}
	
void CDualManager::AddDualClass(CDualClassInfo* pDualClass)
{
	m_vtDualClass.push_back(pDualClass);
}

bool CDualManager::AddMember(WORD idMember)
{
	CHARLIST* pMember = ::CheckServerId(idMember);

	if (pMember == NULL)
	{
		MyLog(LOG_NORMAL, "CDualManager::AddMember : pMember == NULL");
		return false;
	}

	const int nStep = pMember->GetClassStep();

	if (!IsExist(nStep))
	{
		if (nStep != 0)
		{
			MyLog(LOG_NORMAL, "CDualManager::AddMember : don't exist dual step");
			MyLog(LOG_NORMAL, ">>> dual step : %d", nStep);
		}

		return false;
	}
		
	if (m_vtDualClass[nStep]->IsExist(idMember))
	{
		MyLog(LOG_NORMAL, "CGuildManager::AddMember : exist user");
		MyLog(LOG_NORMAL, ">>> dual step : %d, user name : %s", nStep, pMember->Name);
		return false;
	}

	m_vtDualClass[nStep]->AddNew(idMember);
	return true;
}
	
bool CDualManager::DelMember(WORD idMember)
{
	CHARLIST* pMember = ::CheckServerId(idMember);

	if (pMember == NULL)
	{
		MyLog(LOG_NORMAL, "CDualManager::DelMember : pMember == NULL");
		return false;
	}

	const int nStep = pMember->GetClassStep();

	if (!IsExist(nStep))
	{
		if (nStep != 0)
		{	//< CSD-030804
			MyLog(LOG_NORMAL, "CDualManager::DelMember : don't exist dual step");
			MyLog(LOG_NORMAL, ">>> dual step : %d", nStep);
		}	//< CSD-030804

		return false;
	}
		
	if (!m_vtDualClass[nStep]->IsExist(idMember))
	{
		return false;
	}

	m_vtDualClass[nStep]->Delete(idMember);
	return true;
}

void CDualManager::SendDualEnable(WORD idMaster, BYTE nPara, BYTE nX, BYTE nY)
{
	t_packet packet;
	packet.h.header.type = CMD_DUAL_ENABLE;
	packet.h.header.size = sizeof(t_server_dual_enable);
	packet.u.dual.server_dual_enable.idMaster = idMaster;
	packet.u.dual.server_dual_enable.nPara = nPara;
	packet.u.dual.server_dual_enable.nPosX = nX;
	packet.u.dual.server_dual_enable.nPosY = nY;
	::QueuePacket(connections, idMaster, &packet, 1);
}

void CDualManager::SendDualChange(WORD idMaster)
{
	CHARLIST* pMaster = ::CheckServerId(idMaster);
	if (pMaster == NULL)  return;
	
	t_packet packet;
	packet.h.header.type = CMD_DUAL_CHANGE;
	packet.h.header.size = sizeof(t_server_dual_change);
	packet.u.dual.server_dual_change.idMaster = idMaster;
	packet.u.dual.server_dual_change.nDual = pMaster->GetDualClass();
	packet.u.dual.server_dual_change.dwFame = pMaster->fame;
	packet.u.dual.server_dual_change.wStr = pMaster->Str;  
	packet.u.dual.server_dual_change.wCon = pMaster->Con;  
	packet.u.dual.server_dual_change.wDex = pMaster->Dex;  
	packet.u.dual.server_dual_change.wWis = pMaster->Wis;  
	packet.u.dual.server_dual_change.wInt = pMaster->Int;  
	packet.u.dual.server_dual_change.wMovep = pMaster->MoveP;
	packet.u.dual.server_dual_change.wCha = pMaster->Char; 
	packet.u.dual.server_dual_change.wEndu = pMaster->Endu;
	packet.u.dual.server_dual_change.wMor = pMaster->Moral; 
	packet.u.dual.server_dual_change.wLuck = pMaster->Luck;
	packet.u.dual.server_dual_change.wWsPs = pMaster->wsps;
	packet.u.dual.server_dual_change.wPoint = pMaster->GetReservedPoint();
	::QueuePacket(connections, idMaster, &packet, 1);
}

void CDualManager::SendDualDivide(WORD idMaster)
{
	CHARLIST* pMaster = ::CheckServerId(idMaster);
	if (pMaster == NULL)  return;
	
	t_packet packet;
	packet.h.header.type = CMD_DUAL_DIVIDE;
	packet.h.header.size = sizeof(t_server_dual_divide);
	packet.u.dual.server_dual_divide.idMaster = idMaster;
	packet.u.dual.server_dual_divide.nDual = pMaster->GetDualClass();
	packet.u.dual.server_dual_divide.wStr = pMaster->Str;  
	packet.u.dual.server_dual_divide.wCon = pMaster->Con;  
	packet.u.dual.server_dual_divide.wDex = pMaster->Dex;  
	packet.u.dual.server_dual_divide.wWis = pMaster->Wis;  
	packet.u.dual.server_dual_divide.wInt = pMaster->Int;  
	packet.u.dual.server_dual_divide.wMovep = pMaster->MoveP;
	packet.u.dual.server_dual_divide.wCha = pMaster->Char; 
	packet.u.dual.server_dual_divide.wEndu = pMaster->Endu;
	packet.u.dual.server_dual_divide.wMor = pMaster->Moral; 
	packet.u.dual.server_dual_divide.wLuck = pMaster->Luck;
	packet.u.dual.server_dual_divide.wWsPs = pMaster->wsps;
	packet.u.dual.server_dual_divide.wPoint = pMaster->GetReservedPoint();
	::QueuePacket(connections, idMaster, &packet, 1);
}

void CDualManager::RecvDualEnable(WORD idMaster, t_client_dual_enable* pPacket)
{	// 듀얼이 가능한지 검사
	CHARLIST* pMaster = ::CheckServerId(idMaster);
	if (pMaster == NULL)  return;
	// 듀얼 여부 검사
	if (pMaster->IsDual())
	{
		pMaster->Message(MK_WARNING, 0, 383);
		return;
	}
	
	const BYTE nPara = pPacket->nPara;
	const BYTE nX = pPacket->nPosX;
	const BYTE nY = pPacket->nPosY;
	const int nStep = 1;
	
	POS pos;
	::SetItemPos(INV, nPara, nY, nX, &pos);
	ItemAttr* pAttr = ::GetItemByPOS(idMaster, pos);
	if (pAttr == NULL)  return;
	CItem* pItem = ::ItemUnit(*pAttr);
	if (pItem == NULL)  return;
	
	const int ni = pItem->GetRbutton();
	
	if (pItem->GetRbutton() == DUAL_CHANGE_ITEM)
	{ 
		if (IsLevelUp(nStep, pMaster))
		{
			SendDualEnable(idMaster, nPara, nX, nY);
		}
		else
		{
			pMaster->Message(MK_WARNING, 0, 381);
		}
	}
}

void CDualManager::RecvDualChange(WORD idMaster, t_client_dual_change* pPacket)
{	// 듀얼 클래스로 변환 되었다는 Packet를 받음
	CHARLIST* pMaster = ::CheckServerId(idMaster);
	if (pMaster == NULL)  return;
	// 듀얼 여부 검사
	if (pMaster->IsDual())
	{
		pMaster->Message(MK_WARNING, 0, 383);
		return;
	}
	// 일반에서 듀얼로 전환하는 경우
	const BYTE nPara = pPacket->nPara;
	const BYTE nX = pPacket->nPosX;
	const BYTE nY = pPacket->nPosY;
	const int nStep = 1;
	
	POS pos;
	::SetItemPos(INV, nPara, nY, nX, &pos);
	ItemAttr* pAttr = ::GetItemByPOS(idMaster, pos);
	if (pAttr == NULL)  return;
	CItem* pItem = ::ItemUnit(*pAttr);
	if (pItem == NULL)  return;
	
	const int ni = pItem->GetRbutton();
	
	if (pItem->GetRbutton() == DUAL_CHANGE_ITEM)
	{ // 020428 YGI acer
		if (IsLevelUp(nStep, pMaster))
		{
			::SendItemEventLog(pAttr, idMaster, 0, SILT_USE, 3); //020829 lsw
			::SendDeleteItem(pAttr, &pos, pMaster, false);
			pMaster->SetDualClass(pPacket->nNext); // 듀얼 클래스 설정
			pMaster->DivideAbility(pPacket->nNext);
			Change(nStep, pMaster);
			SendDualChange(idMaster);
		}
		else
		{
			pMaster->Message(MK_WARNING, 0, 381);
		}
	}
}

void CDualManager::RecvDualDivide(WORD idMaster, t_client_dual_divide* pPacket)
{	// 듀얼 클래스 어빌리티 분배를 하겠다는 Packet를 받음
	CHARLIST* pMaster = ::CheckServerId(idMaster);
	if (pMaster == NULL)  return;
	// 듀얼의 단계가 올라가는 경우
	const int nStep = pMaster->GetClassStep() + 1;
	
	if (pMaster->IsDual() && IsLevelUp(nStep, pMaster))
	{	//< CSD-030326
		DelMember(idMaster);
		pMaster->DivideAbility(pPacket->nNext);
		Change(nStep, pMaster);
		SendDualDivide(idMaster);
		AddMember(idMaster);
	}	//> CSD-030326
	else
	{
		pMaster->Message(MK_WARNING, 0, 382);
	}
}

void CDualManager::RecvResetAbility(WORD idMaster)
{
	CHARLIST* pMaster = ::CheckServerId(idMaster);
	if (pMaster == NULL)  return;
	
	if (pMaster->GetLevel() == CROSSING_CLASS_LEVEL)
	{	//< CSD-030806
		const int nTotal = ::GetTotalAbility(pMaster);
		const int nClass = pMaster->Class;
		// 어빌리티 초기화
		pMaster->Str = BASIC_ABILITY[nClass][STR];
		pMaster->Dex = BASIC_ABILITY[nClass][DEX];
		pMaster->Con = BASIC_ABILITY[nClass][CON];
		pMaster->Wis = BASIC_ABILITY[nClass][WIS];
		pMaster->Int = BASIC_ABILITY[nClass][INT_];
		pMaster->Char = BASIC_ABILITY[nClass][CHA];
		pMaster->MoveP = BASIC_ABILITY[nClass][MOVP];
		pMaster->Endu = BASIC_ABILITY[nClass][ENDU];
		pMaster->Moral = BASIC_ABILITY[nClass][MOR];
		pMaster->wsps = BASIC_ABILITY[nClass][WSPS];
		pMaster->Luck = BASIC_ABILITY[nClass][LUCK];
		const int nFirst = 3;
		::AddAbility(pMaster, ADD_ABILITY[nClass][0], nFirst);
		const int nSecond = 2;
		::AddAbility(pMaster, ADD_ABILITY[nClass][1], nSecond);
		
		int nMinus = 0;
		
		for (int i = STR; i <= LUCK; ++i)
		{
			nMinus += BASIC_ABILITY[nClass][i];
		}
		// 어빌리티 포인터 재설정
		pMaster->SetReservedPoint(nTotal - nMinus - nFirst - nSecond);
		::SendCharInfo(idMaster);
	}	//> CSD-030806
}

///////////////////////////////////////////////////////////////////////////////
// Protected Method
///////////////////////////////////////////////////////////////////////////////

void CDualManager::Change(BYTE nStep, CHARLIST* pMaster)
{	//< CSD-TW-030606 : 듀얼 클래스 단계 상승에 따른 소모 Ladder나 Fame
	CDualDataInfo* pDualData = GetDualDataInfo(nStep);

	if (pDualData == NULL)
	{
		return;
	}

	switch (nStep)
	{
    case 1:
    case 2:
    case 3:
    case 4:
		{
			if (pMaster->LadderScore >= pDualData->GetNeedLadder())
			{
				::SaveChangeClassStep(pMaster, nStep - 1, nStep);
				const int change = pDualData->GetConsumeLadder();
				::SaveChangeLadder(pMaster, pMaster->LadderScore, pMaster->LadderScore - change, CLT_CHANGE_DUAL);
				pMaster->LadderScore -= pDualData->GetConsumeLadder();
				return;
			}
			
			if (pMaster->fame >= pDualData->GetNeedFame())
			{
				::SaveChangeClassStep(pMaster, nStep - 1, nStep);
				const int DecFame = pDualData->GetConsumeFame();
				::SaveChangeFame(pMaster,pMaster->fame,pMaster->fame-DecFame,LF_DUAL);
				pMaster->fame -= DecFame;
				return;
			}
			
			break;
		}
    case 5:
		{
			::SaveChangeClassStep(pMaster, nStep - 1, nStep);
			const int change = pDualData->GetConsumeLadder();
			::SaveChangeLadder( pMaster, pMaster->LadderScore, pMaster->LadderScore - change, CLT_CHANGE_DUAL );
			pMaster->LadderScore -= pDualData->GetConsumeLadder();
			const int DecFame = pDualData->GetConsumeFame();
			::SaveChangeFame(pMaster,pMaster->fame,pMaster->fame-DecFame,LF_DUAL);
			pMaster->fame -= DecFame;
			break;
		}
	}
}	//> CSD-TW-030606

///////////////////////////////////////////////////////////////////////////////
// Private Method
///////////////////////////////////////////////////////////////////////////////

bool CDualManager::IsLevelUp(BYTE nStep, CHARLIST* pMaster)
{	// 듀얼 클래스 단계 상승이 가능한지 여부
	CDualDataInfo* pDualData = GetDualDataInfo(nStep);

	if (pDualData == NULL)
	{
		return false;
	}
	
	switch (nStep)
	{	//< CSD-030806
    case 1:
    case 2:
    case 3:
    case 4:
		{
			if (pMaster->GetLevel() >= pDualData->GetMaxLevel())
			{
				if (pMaster->LadderScore >= pDualData->GetNeedLadder())  
				{
					return true;
				}

				if (pMaster->fame >= pDualData->GetNeedFame())
				{
					return true;
				}
			}

			break;
		}
    case 5:
		{
			if (pMaster->GetLevel() >= pDualData->GetMaxLevel())
			{
				if (pMaster->LadderScore < pDualData->GetNeedLadder())
				{
					break;
				}
				
				if (pMaster->fame < pDualData->GetNeedFame())
				{
					break;
				}

				return true;
			}
			
			break;
		}
	}	//> CSD-030806
	
	return false;
}