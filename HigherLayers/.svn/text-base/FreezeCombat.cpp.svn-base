// FreezeCombat.cpp: implementation of the CFreezeCombat class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "FreezeCombat.h"

#include "..\HigherLayers\CItem.h"
#include "..\HigherLayers\Op_Battle.h"
#include "..\HigherLayers\OP_Magic.h"
///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CFreezeCombat::CFreezeCombat()
{

}

CFreezeCombat::~CFreezeCombat()
{

}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

bool CFreezeCombat::Bind() 
{ 
	Resist(ICING_BLAST, &CFreezeCombat::IcingBlast);
	Resist(CRITICAL_HIDING, &CFreezeCombat::CriticalHiding);
	Resist(SHARK_MISSILE, &CFreezeCombat::SharkMissile);
	Resist(HORN_OF_ICEBERG, &CFreezeCombat::HornOfIceBerg);
	Resist(ICE_SHIELD, &CFreezeCombat::IceShield);
	Resist(ICE_EXTREME, &CFreezeCombat::IceExtreme);
	return true; 
}

bool CFreezeCombat::Elapse(CHARLIST* pTarget)
{	//< CSD-TW-030623
	switch (pTarget->GetPassiveCombat())
	{
    case ICE_SHIELD:
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

bool CFreezeCombat::IcingBlast()
{
	// 결과값을 대상자에게 Packet으로 보내기
	t_server_combat_result packet;
	packet.nSkill = m_nIndex;
	packet.idTarget = m_idTarget;
	packet.nResult = HIT_AND_NOTDEAD;
	SendResult(CMD_COMBAT_RESULT, &packet);
	return true;
}

bool CFreezeCombat::CriticalHiding()
{
	WORD wRemain = m_pCaster->GetCombatValue(m_nIndex);
	// 투명체 출력을 위한 VIEWTYPE 설정
	m_pCaster->dwTransparency = m_dwNow + wRemain;
	m_pCaster->viewtype = VIEWTYPE_TRANSPARENCY_;
	::SendViewType(m_pCaster, VIEWTYPE_TRANSPARENCY_, m_pCaster->dwTransparency);
	// 결과값을 대상자에게 Packet으로 보내기
	t_server_combat_result packet;
	packet.nSkill = m_nIndex;
	packet.nResult = HIT_AND_NOTDEAD;
	packet.idTarget = m_idCaster;
	SendResult(CMD_COMBAT_RESULT, &packet);
	return true;
}

bool CFreezeCombat::SharkMissile()
{
	// 결과값을 대상자에게 Packet으로 보내기
	t_server_combat_result packet;
	packet.nSkill = m_nIndex;
	packet.nResult = HIT_AND_NOTDEAD;
	packet.idTarget = m_idTarget;
	SendResult(CMD_COMBAT_RESULT, &packet);
	return true;
}

bool CFreezeCombat::HornOfIceBerg()
{
	t_server_combat_result packet;
	packet.nSkill = m_nIndex;
	packet.nResult = HIT_AND_NOTDEAD;
	packet.idTarget = m_idTarget;
	SendResult(CMD_COMBAT_RESULT, &packet);
	return true;
}

bool CFreezeCombat::IceShield()
{
	m_pCaster->dwCombatState = m_dwNow + CalcMaintain();
	// CSD-021216 : 얼림 저주 마법 방어
	m_pCaster->SetCurseDefense(m_dwNow + CalcMaintain(), CMagicState::CURSE_FREEZE);
	// 일반 보호계열 마법 상태임을 설정
	m_pCaster->dwNormalProtect = m_dwNow + CalcMaintain();
	// 결과값을 대상자에게 Packet으로 보내기
	t_server_combat_result packet;
	packet.nSkill = m_nIndex;
	packet.idTarget = m_idTarget;
	packet.nResult = HIT_AND_NOTDEAD;
	SendResult(CMD_COMBAT_RESULT, &packet);
	return true;
}

bool CFreezeCombat::IceExtreme()
{
	// 결과값을 대상자에게 Packet으로 보내기
	t_server_combat_result packet;
	packet.nSkill = m_nIndex;
	packet.idTarget = m_idTarget;
	packet.nResult = HIT_AND_NOTDEAD;
	SendResult(CMD_COMBAT_RESULT, &packet);
	return true;
}