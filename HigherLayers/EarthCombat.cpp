// EarthCombat.cpp: implementation of the CEarthCombat class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "EarthCombat.h"

#include "..\HigherLayers\Op_Magic.h"
///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CEarthCombat::CEarthCombat()
{

}

CEarthCombat::~CEarthCombat()
{

}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

bool CEarthCombat::Bind()
{
	Resist(BLOOD_EARTH, &CEarthCombat::BloodEarth);
	Resist(GROUND_ATTACK, &CEarthCombat::GroundAttack);
	Resist(BLOOD_WILL, &CEarthCombat::BloodWill);
	Resist(STONE_ARMOR, &CEarthCombat::StoneArmor);
	Resist(SIMUNIAN_BOSOM, &CEarthCombat::SimunianBosom);
	Resist(EARTH_EXTREME, &CEarthCombat::EarthExtreme);
	return true;
}

bool CEarthCombat::Elapse(CHARLIST* pTarget)
{	//< CSD-TW-030623
	switch (pTarget->GetPassiveCombat())
	{
    case SIMUNIAN_BOSOM:
		{
			if (!pTarget->IsPassiveCombatState())
			{
				pTarget->InitPassiveCombat();
				SendInit(PASSIVE_COMBAT, pTarget->GetServerID()); 
			}

			break;
		}
	}
	
	return true;
}	//> CSD-TW-030623

bool CEarthCombat::BloodEarth()
{	//< CSD-030723
	switch (GetKind(m_pCaster))
	{
    case WR_SHORT:  return AttackShort();
    case WR_MIDDLE: return AttackMiddle();
    case WR_LONG1:  return AttackLong(WR_LONG1);
    case WR_LONG2:  return AttackLong(WR_LONG2);
	}
	
	return false;
}	//> CSD-030723

bool CEarthCombat::GroundAttack()
{	//< CSD-030723
	switch (GetKind(m_pCaster))
	{
    case WR_SHORT:  return AttackShort();
    case WR_MIDDLE: return AttackMiddle();
    case WR_LONG1:  return AttackLong(WR_LONG1);
    case WR_LONG2:  return AttackLong(WR_LONG2);
	}
	
	return false;
}	//> CSD-030723

bool CEarthCombat::BloodWill()
{ // [223]블러드 윌
	// 결과값을 대상자에게 Packet으로 보내기
	t_server_combat_result packet;
	packet.nSkill = m_nIndex;
	packet.nResult = HIT_AND_NOTDEAD;
	packet.idTarget = m_idTarget;
	SendResult(CMD_COMBAT_RESULT, &packet);
	return true;
}

bool CEarthCombat::StoneArmor()
{
	// 결과값을 대상자에게 Packet으로 보내기
#ifndef LMS_MAPSERVER //Eleval 19/07/08 - To disable Stone Armor from the LMS
	t_server_combat_result packet;
	packet.nSkill = m_nIndex;
	packet.idTarget = m_idTarget;
	packet.nResult = HIT_AND_NOTDEAD;
	SendResult(CMD_COMBAT_RESULT, &packet);
	return true;
#else
	return false;
#endif
}

bool CEarthCombat::SimunianBosom()
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

bool CEarthCombat::EarthExtreme()
{	//< CSD-030723
	switch (GetKind(m_pCaster))
	{
    case WR_SHORT:  return AttackShort();
    case WR_MIDDLE: return AttackMiddle();
    case WR_LONG1:  return AttackLong(WR_LONG1);
    case WR_LONG2:  return AttackLong(WR_LONG2);
	}
	
	return false;
}	//> CSD-030723