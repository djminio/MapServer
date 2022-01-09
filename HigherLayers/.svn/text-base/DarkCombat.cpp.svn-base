// DarkCombat.cpp: implementation of the CDarkCombat class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "DarkCombat.h"

#include "..\HigherLayers\Op_Battle.h"
///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CDarkCombat::CDarkCombat()
{

}

CDarkCombat::~CDarkCombat()
{

}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

bool CDarkCombat::Bind() 
{ 
	Resist(POISONING_NOVA, &CDarkCombat::PoisoningNova);
	Resist(CHERROY_SHADE, &CDarkCombat::CherroyShade);
	Resist(DARK_BLADE, &CDarkCombat::DarkBlade);
	Resist(TYBERN_GIFT, &CDarkCombat::TybernGift);
	Resist(ACUQUIRE_DARK, &CDarkCombat::AcuquireDark);
	Resist(DARK_EXTREME, &CDarkCombat::DarkExtreme);
	return true; 
}

bool CDarkCombat::Elapse(CHARLIST* pTarget)
{	//< CSD-TW-030624
	if (pTarget == NULL)
	{
		return false;
	}
	
	switch (pTarget->GetPassiveCombat())
	{
    case TYBERN_GIFT:
		{
			if (!pTarget->IsPassiveCombatState())
			{
				pTarget->ResetAbility(0);
				pTarget->InitPassiveCombat();
				SendInit(PASSIVE_COMBAT, pTarget->GetServerID());
			}

			break;
		}
	}
	
	switch (pTarget->GetRecoveryCombat())
	{
	case ACUQUIRE_DARK:
		{
			if (!pTarget->IsRecoveryCombatState())
			{
				pTarget->InitRecoveryCombat();
				SendInit(RECOVERY_COMBAT, pTarget->GetServerID());
			}
			
			break;    
		}
	}
	
	return true;
}	//> CSD-TW-030624

bool CDarkCombat::PoisoningNova()
{
	const int nCaster = m_pCaster->GetActiveCombat(); // 시전자의 전투속성
	const int nTarget = m_pTarget->GetActiveCombat(); // 대상자의 전투속성
	const int nResult = Relation(nCaster, nTarget);   // 승패, 대립 관계
	const int nPercent = m_pCaster->CorrectCombatAttack(nCaster, nResult);
	const int nDamage = m_pTarget->HpMax*nPercent/100;
	WORD wRemain = CalcMaintain();
	m_pTarget->SetPoisonedPoint(nDamage);
	m_pTarget->ApplyCurse(3, CON_POISON, m_dwNow + wRemain);
	// NK에 계산
	::CheckNK(m_idCaster, m_idTarget, NK_TYPE_NO_KILL_);
	// 결과값을 대상자에게 Packet으로 보내기
	t_server_combat_result_t packet;
	packet.nSkill = m_nIndex;
	packet.nResult = HIT_AND_NOTDEAD;
	packet.idTarget = m_idTarget;
	packet.nState = m_pTarget->GetState();
	packet.wMaintain = wRemain;
	SendResult(CMD_COMBAT_RESULT_T, &packet);
	return true;
}

bool CDarkCombat::CherroyShade()
{
	// 결과값을 대상자에게 Packet으로 보내기
	t_server_combat_result packet;
	packet.nSkill = m_nIndex;
	packet.nResult = HIT_AND_NOTDEAD;
	packet.idTarget = m_idTarget;
	SendResult(CMD_COMBAT_RESULT, &packet);
	return true;
}

bool CDarkCombat::DarkBlade()
{
	// 결과값을 대상자에게 Packet으로 보내기
	t_server_combat_result packet;
	packet.nSkill = m_nIndex;
	packet.nResult = HIT_AND_NOTDEAD;
	packet.idTarget = m_idTarget;
	SendResult(CMD_COMBAT_RESULT, &packet);
	return true;
}

bool CDarkCombat::TybernGift()
{
	m_pCaster->dwCombatState = m_dwNow + CalcMaintain();
	m_pCaster->ResetAbility(m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기
	t_server_combat_result packet;
	packet.nSkill = m_nIndex;
	packet.idTarget = m_idTarget;
	packet.nResult = HIT_AND_NOTDEAD;
	SendResult(CMD_COMBAT_RESULT, &packet);
	return true;
}

bool CDarkCombat::AcuquireDark()
{	//< CSD-TW-030606
	const WORD wPeriod = CalcMaintain(); // 유지시간 계산
	m_pCaster->dwCombatRecovery = m_dwNow + wPeriod;
	// 결과값을 대상자에게 Packet으로 보내기
	t_server_combat_result_t packet;
	packet.nSkill = m_nIndex;
	packet.idTarget = m_idTarget;
	packet.nState = m_pTarget->GetState();
	packet.nResult = HIT_AND_NOTDEAD;
	SendResult(CMD_COMBAT_RESULT, &packet);
	return true;
}	//> CSD-TW-030606

bool CDarkCombat::DarkExtreme()
{
	const int nCaster = m_pCaster->GetActiveCombat(); // 시전자의 전투속성
	const int nTarget = m_pTarget->GetActiveCombat(); // 대상자의 전투속성
	const int nResult = Relation(nCaster, nTarget);   // 승패, 대립 관계
	const int nPercent = m_pCaster->CorrectCombatAttack(nCaster, nResult);
	const int nDamage = m_pTarget->HpMax*nPercent/100;
	WORD wRemain = CalcMaintain();
	m_pTarget->SetPoisonedPoint(nDamage);
	m_pTarget->ApplyCurse(3, CON_POISON, m_dwNow + wRemain);
	// NK에 계산
	::CheckNK(m_idCaster, m_idTarget, NK_TYPE_NO_KILL_);
	// 결과값을 대상자에게 Packet으로 보내기
	t_server_combat_result_t packet;
	packet.nSkill = m_nIndex;
	packet.nResult = HIT_AND_NOTDEAD;
	packet.idTarget = m_idTarget;
	packet.nState = m_pTarget->GetState();
	packet.wMaintain = wRemain;
	SendResult(CMD_COMBAT_RESULT_T, &packet);
	return true;
}
