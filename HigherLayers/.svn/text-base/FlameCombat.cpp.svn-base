// FlameCombat.cpp: implementation of the CFlameCombat class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "FlameCombat.h"

#include "..\HigherLayers\CItem.h"
#include "..\HigherLayers\OP_Magic.h"
///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CFlameCombat::CFlameCombat()
{

}

CFlameCombat::~CFlameCombat()
{

}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

bool CFlameCombat::Bind() 
{	//< CSD-TW-030606
	Resist(SWORD_N_ROSES, &CFlameCombat::SwordNRoses);
	Resist(MULTIPLE_FIRE, &CFlameCombat::MultipleFire);
	Resist(FLEET_MISSILE, &CFlameCombat::FleetMissile);
	Resist(CONVERTING_ARMOR, &CFlameCombat::ConvertingArmor);
	Resist(BURNING_BLOODY, &CFlameCombat::BurningBloody); // 030415 kyo
	Resist(FIRE_EXTREME, &CFlameCombat::FireExtreme);
	return true; 
}	//> CSD-TW-030606

bool CFlameCombat::Elapse(CHARLIST* pTarget)
{	//< CSD-TW-030624
	switch( pTarget->GetRecoveryCombat())
	{
	case BURNING_BLOODY:
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

bool CFlameCombat::SwordNRoses()
{
	// 결과값을 대상자에게 Packet으로 보내기
	t_server_combat_result packet;
	packet.nSkill = m_nIndex;
	packet.idTarget = m_idTarget;
	packet.nResult = HIT_AND_NOTDEAD;
	SendResult(CMD_COMBAT_RESULT, &packet);
	return true;
}

bool CFlameCombat::MultipleFire()
{
	// 결과값을 대상자에게 Packet으로 보내기
	t_server_combat_result packet;
	packet.nSkill = m_nIndex;
	packet.idTarget = m_idTarget;
	packet.nResult = HIT_AND_NOTDEAD;
	SendResult(CMD_COMBAT_RESULT, &packet);
	return true;
}

bool CFlameCombat::FleetMissile()
{
	// 결과값을 대상자에게 Packet으로 보내기
	t_server_combat_result packet;
	packet.nSkill = m_nIndex;
	packet.idTarget = m_idTarget;
	packet.nResult = HIT_AND_NOTDEAD;
	SendResult(CMD_COMBAT_RESULT, &packet);
	return true;
}

bool CFlameCombat::ConvertingArmor()
{
	// 결과값을 대상자에게 Packet으로 보내기
	t_server_combat_result packet;
	packet.nSkill = m_nIndex;
	packet.idTarget = m_idTarget;
	packet.nResult = HIT_AND_NOTDEAD;
	SendResult(CMD_COMBAT_RESULT, &packet);
	return true;
}

bool CFlameCombat::CriticalFire()
{
	m_pCaster->dwCombatState = m_dwNow + CalcMaintain();
	// 결과값을 대상자에게 Packet으로 보내기
	t_server_combat_result packet;
	packet.nSkill = m_nIndex;
	packet.idTarget = m_idTarget;
	packet.nResult = HIT_AND_NOTDEAD;
	SendResult(CMD_COMBAT_RESULT, &packet);
	return true;
}

bool CFlameCombat::BurningBloody() // 030415 kyo
{	//< CSD-TW-030606
	const WORD wPeriod = CalcMaintain(); // 유지시간 계산
	m_pCaster->dwCombatRecovery = m_dwNow + wPeriod;
	
	t_server_combat_result_t packet;
	packet.nSkill = m_nIndex;
	packet.idTarget = m_idTarget;
	packet.nState = m_pTarget->GetState();
	packet.nResult = HIT_AND_NOTDEAD;
	SendResult(CMD_COMBAT_RESULT, &packet);
	return true;
}	//> CSD-TW-030606

bool CFlameCombat::FireExtreme()
{
	// 결과값을 대상자에게 Packet으로 보내기
	t_server_combat_result packet;
	packet.nSkill = m_nIndex;
	packet.idTarget = m_idTarget;
	packet.nResult = HIT_AND_NOTDEAD;
	SendResult(CMD_COMBAT_RESULT, &packet);
	return true;
}