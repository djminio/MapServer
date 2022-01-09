// RecoveryMagic.cpp: implementation of the CRecoveryMagic class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "RecoveryMagic.h"


#include "..\HigherLayers\OP_Magic.h"


///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CRecoveryMagic::CRecoveryMagic()
{
	
}

CRecoveryMagic::~CRecoveryMagic()
{
	
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

bool CRecoveryMagic::Bind()
{
	Resist(LIFE_UP, &CRecoveryMagic::Healing);
	Resist(DECIPHER, &CRecoveryMagic::Decipher);
	Resist(HEAL_DISEASE, &CRecoveryMagic::HealDisease);
	Resist(GREAT_HEAL, &CRecoveryMagic::GreatHeal);
	Resist(REMOVE_CURSE, &CRecoveryMagic::RemoveCurse);
	Resist(RESTORE_AUTHORITY, &CRecoveryMagic::RestoreAuthority);
	Resist(MIRACLE_RECOVER, &CRecoveryMagic::MiracleRecover);
	Resist(RELIEF_AUTHORITY, &CRecoveryMagic::ReliefAuthority);
	Resist(RECOVER_POISON, &CRecoveryMagic::RecoverPoison);
	Resist(AUTHORITY_RECOVER_SIGHT, &CRecoveryMagic::AuthorityRecoverSight);
	Resist(CURE_DISEASE, &CRecoveryMagic::CureDisease);
	Resist(AUTHORITY_SIMUNIAN, &CRecoveryMagic::AuthoritySimunian);
	Resist(HOLY_CURE, &CRecoveryMagic::HolyCure); // 030415 kyo 
	return true;
}

bool CRecoveryMagic::Healing()
{ // [1]체력회복 : HP 증가
	if (m_pTarget->IsFullLife())
	{ // MP가 전부 차있다면
		return false;
	}
	const int nInt = m_pCaster->GetAbility(INT_);
	const int nWsPs = m_pCaster->GetAbility(WSPS);
	const int nPlus = ((nInt + nWsPs)>>1)*(m_pCaster->Skill[TACTICS_Magery]>>1) + 100;
	const int nReal = m_pTarget->IncLife(nPlus);
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex, nReal);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_H);
	m_packet.u.magic.server_magic_result_h.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_h.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_h.nLife = nReal;
	m_packet.u.magic.server_magic_result_h.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_h.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}

bool CRecoveryMagic::Decipher()
{ // [4]해독 마법 : 저주계열 마법인 중독, 독구름 상태를 해제
	m_pTarget->ClearCurse(3);
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT);
	m_packet.u.magic.server_magic_result.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}

bool CRecoveryMagic::HealDisease()
{ // [5]질병치료 : Health(배고픔 상태) 증가
	if (m_pTarget->IsFullHungry() == true)
	{ // 배고픔 수치가 전부 차있다면
		return false;
	}
	
	const int nPlus = m_pTarget->HungryMax/5;
	m_pTarget->IncHungry(nPlus);
	m_pTarget->SendCharInfoBasic(SP, m_pTarget->Hungry);//020704 lsw
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT);
	m_packet.u.magic.server_magic_result.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}

bool CRecoveryMagic::GreatHeal()
{ // [6]대체력회복 : HP를 최대까지 증가, 저주 1, 2계열의 마법 상태 회복
	if (m_pTarget->IsFullLife())
	{ // MP가 전부 차있다면
		return false;
	}
	
	const int nInt = m_pCaster->GetAbility(INT_);
	const int nWsPs = m_pCaster->GetAbility(WSPS);
	const int nPlus = ((nInt + nWsPs)>>1)*((m_pCaster->Skill[TACTICS_Magery]*3)>>1) + 600;
	const int nReal = m_pTarget->IncLife(nPlus);
	// 저주 1, 2계열 마법 상태 회복
	m_pTarget->ClearCurse(1);
	m_pTarget->ClearCurse(2);
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex, nReal);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_H);
	m_packet.u.magic.server_magic_result_h.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_h.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_h.nLife = nReal;
	m_packet.u.magic.server_magic_result_h.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_h.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}

bool CRecoveryMagic::RemoveCurse()
{ // [78]저주해제 : 저주 1, 2, 3, 4계열 마법 해제
	m_pTarget->ClearCurse(1);
	m_pTarget->ClearCurse(2);
	m_pTarget->ClearCurse(3);
	m_pTarget->ClearCurse(4);
	::SendMagicCommand(m_pTarget, SET_CUREBLIND, 0, 0);	//시력상실 회복
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT);
	m_packet.u.magic.server_magic_result.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}

bool CRecoveryMagic::RestoreAuthority()
{ // [151]회복의권능 : 최대의 HP로 채워줌
	if (m_pTarget->IsFullLife())
	{ // MP가 전부 차있다면
		return false;
	}
	
	const int nWis = m_pCaster->GetAbility(WIS);
	const int nWsPs = m_pCaster->GetAbility(WSPS);
	const int nPlus = ((nWis + nWsPs)>>1)*(m_pCaster->Skill[TACTICS_Orison]) + 400;
	const int nReal = m_pTarget->IncLife(nPlus);
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex, nReal);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_H);
	m_packet.u.magic.server_magic_result_h.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_h.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_h.nLife = nReal;
	m_packet.u.magic.server_magic_result_h.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_h.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}

bool CRecoveryMagic::MiracleRecover()
{ // [156]완치의 기적 : 시전자와 주변의 일정 거리 내에 있는 캐릭터들의 HP을 50000까지 올려줌
	if (m_pTarget->IsFullLife())
	{ // HP가 전부 차있다면
		return false;
	}
	// Life 증가
	const int nReal = m_pTarget->IncLife(50000);
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex, nReal);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_H);
	m_packet.u.magic.server_magic_result_h.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_h.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_h.nLife = nReal;
	m_packet.u.magic.server_magic_result_h.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_h.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}

bool CRecoveryMagic::ReliefAuthority()
{ // [164]구원의 권능 : 대상자를 유령 상태에서 정상 상태로 만들고 HP와 마나를 최대까지 배고픔 수치를 절반 올려줌
	//020501 lsw 살리기쪽 고침
	if(!m_pCaster->bCanViewGhost)//유령을 볼 수 없는 상태라면
	{
		return false;
	}
	if(m_pTarget->viewtype != VIEWTYPE_GHOST_) 
	{ // 유령 상태가 아니라면
		return false;
	}
	if(m_pTarget->IsReporter()) return false; //Finito added to that reporter cant be res.

	SkillMgr.CharacterToAlive(m_pTarget, 1); //배고픔 절반인데 풀로 채웁니다.
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_M);
	m_packet.u.magic.server_magic_result_m.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_m.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_m.nLife = m_pTarget->HpMax;
	m_packet.u.magic.server_magic_result_m.nMana = m_pTarget->ManaMax;
	m_packet.u.magic.server_magic_result_m.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_m.nResult = HIT_AND_ALIVE;
	SendMagicResult();
	return true;
}

bool CRecoveryMagic::RecoverPoison()
{ // [154]해독의 권능 : 저주계열 마법인 중독, 독구름 상태를 해제
	m_pTarget->ClearCurse(3);
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT);
	m_packet.u.magic.server_magic_result.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}

bool CRecoveryMagic::AuthorityRecoverSight()
{ // [173]시력회복의 권능 : 저주계열 마법인 시력상실을 해제
	m_pTarget->ClearCurse(4);
	::SendMagicCommand(m_pTarget, SET_CUREBLIND, 0, 0);	//시력상실 회복
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT);
	m_packet.u.magic.server_magic_result.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}

bool CRecoveryMagic::CureDisease()
{ // [155]질병치료의 권능 : Health(배고픔 상태) 증가
	if (m_pTarget->IsFullHungry()) 
	{ // 배고픔 수치가 전부 차있다면
		return false;
	}
	
	const int nPlus = m_pTarget->HungryMax/4;
	m_pTarget->IncHungry(nPlus);
	m_pTarget->SendCharInfoBasic( SP, m_pTarget->Hungry);//020704 lsw
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}

bool CRecoveryMagic::AuthoritySimunian()
{ // [181]시무니안의 권능
	if (m_pTarget->IsFullHungry()) 
	{ // 배고픔 수치가 전부 차있다면
		return false;
	}
	
	const int nPlus = m_pTarget->HungryMax/4;
	m_pTarget->IncHungry(nPlus);
	m_pTarget->SendCharInfoBasic(SP, m_pTarget->Hungry);//020704 lsw
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}

bool CRecoveryMagic::HolyCure()
{ // [179]신성한 치유 = 저주해제+완치의기적+프리즌 해제 // 030415 kyo
	
	m_pTarget->ClearCurse(1);
	m_pTarget->ClearCurse(2);
	m_pTarget->ClearCurse(3);
	m_pTarget->ClearCurse(4);
	m_pTarget->ClearCurse(6);//Freezn 해제
	
	const int nPlus = m_pTarget->HungryMax; // 030516 kyo
	m_pTarget->IncHungry(nPlus);
	m_pTarget->SendCharInfoBasic(SP, m_pTarget->Hungry);//020704 lsw
	
	::SendMagicCommand(m_pTarget, SET_CUREBLIND, 0, 0);	//시력상실 회복
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기	
	if (m_pTarget->IsFullLife())
	{ // HP가 전부 차있다면 무조건 뺀다. 
		m_pTarget->DecLife(1);
		//return false;
	}
	const int nReal = m_pTarget->IncLife(50000);
	
	InitMagicResult(CMD_MAGIC_RESULT_H);
	m_packet.u.magic.server_magic_result_h.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_h.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_h.nLife = nReal;
	m_packet.u.magic.server_magic_result_h.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_h.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}