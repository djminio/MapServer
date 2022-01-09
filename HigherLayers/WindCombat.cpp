// WindCombat.cpp: implementation of the CWindCombat class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "WindCombat.h"

#include "..\HigherLayers\OP_Magic.h"
///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CWindCombat::CWindCombat()
{

}

CWindCombat::~CWindCombat()
{

}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

bool CWindCombat::Bind()
{ 
	Resist(CHARGING, &CWindCombat::Charging);
	Resist(DOUBLE_ATTACK, &CWindCombat::DoubleAttack);
	Resist(WHILWIND, &CWindCombat::Whilwind);
	Resist(TWISTER, &CWindCombat::Twister);
	Resist(GUST, &CWindCombat::Gust);
	Resist(WIND_EXTREME, &CWindCombat::WindExtreme);
	return true; 
}

bool CWindCombat::Elapse(CHARLIST* pTarget)
{	//< CSD-TW-030623
	switch (pTarget->GetPassiveCombat())
	{
    case GUST:
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

bool CWindCombat::Charging()
{	// [239] Charging
	if (m_idCaster == m_idTarget)  
	{
		return false;
	}
	// 결과값을 대상자에게 Packet으로 보내기
	t_server_combat_result packet;
	packet.nSkill = m_nIndex;
	packet.idTarget = m_idTarget;
	packet.nResult = HIT_AND_NOTDEAD;
	SendResult(CMD_COMBAT_RESULT, &packet);
	return true;
}

bool CWindCombat::DoubleAttack()
{
	// 결과값을 대상자에게 Packet으로 보내기
	t_server_combat_result packet;
	packet.nSkill = m_nIndex;
	packet.idTarget = m_idTarget;
	packet.nResult = HIT_AND_NOTDEAD;
	SendResult(CMD_COMBAT_RESULT, &packet);
	return true;
}

bool CWindCombat::Whilwind()
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

bool CWindCombat::Twister()
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

bool CWindCombat::Gust()
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

bool CWindCombat::WindExtreme()
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