// ProtectionMagic.cpp: implementation of the CProtectionMagic class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "ProtectionMagic.h"

#include "..\HigherLayers\OP_Magic.h"
///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CProtectionMagic::CProtectionMagic()
{
  
}

CProtectionMagic::~CProtectionMagic()
{

}

///////////////////////////////////////////////////////////////////////////////
// Protected Method
///////////////////////////////////////////////////////////////////////////////

bool CProtectionMagic::Bind()
{ 
	Resist(PROTECTION, &CProtectionMagic::Protection);
	Resist(SHIELD, &CProtectionMagic::Shield);
	Resist(FIRE_RESIST, &CProtectionMagic::FireResist);
	Resist(ICE_RESIST , &CProtectionMagic::IceResist);
	Resist(ELECT_RESIST, &CProtectionMagic::ElectResist);
	Resist(TWINKLE_STAR, &CProtectionMagic::TwinkleStar);
	Resist(PROTECT_RANGE_WEAPON, &CProtectionMagic::ProtectRangeWeapon); // 장거리 무기로부터 보호
	Resist(DRAGON_SCALE, &CProtectionMagic::DragonScale);
	Resist(BLESS, &CProtectionMagic::Bless);
	Resist(PROTECT_DANGER, &CProtectionMagic::ProtectDanger);
	Resist(GREAT_SHIELD, &CProtectionMagic::GreatShield);
	Resist(AUTHORITY_LETHY, &CProtectionMagic::AuthorityLethy); // CSD-021015
	Resist(NULLITY_MAGIC, &CProtectionMagic::NullityMagic);
	Resist(CANCEL_DIVINE, &CProtectionMagic::CancelDivine);
	Resist(RECURRENCE_DIVINE, &CProtectionMagic::RecurrenceDivine);
	Resist(PERFECT_PROTECT, &CProtectionMagic::PerfectProtect);
	Resist(AUTHORITY_OF_ATHYAS, &CProtectionMagic::AuthorityOfAthyas); // 030415 kyo 
	return true;
} 

bool CProtectionMagic::DragonScale()
{ // [86]드래곤의 비늘 : 물리적 공격 데미지 60% 방어
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	// 물리적 방어력 60% 증가
	m_pTarget->SetPhysicalDefense(m_dwNow + wPeriod, 60, 60, 60);
	// 일반 보호계열 마법 상태임을 설정
	m_pTarget->dwNormalProtect = m_dwNow + wPeriod;
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
	SendMagicResult();
	return true;
}

bool CProtectionMagic::ProtectRangeWeapon()
{ // [79]장거리 무기로부터 보호 : 장거리 공격 데미지 90% 방어
	// 유지시간 계산
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	// 장거리 공격 데미지 90% 방어
	m_pTarget->dpLong.SetState(m_dwNow + wPeriod, 90);
	// 일반 보호계열 마법 상태임을 설정
	m_pTarget->dwNormalProtect = m_dwNow + wPeriod;
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
	SendMagicResult();
	return true;
}

bool CProtectionMagic::FireResist()
{ // [28]불에 대한 저항 : 불계열 공격마법에 대한 방어력 50% 증가
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	// 불계열 공격마법에 대한 방어력 50% 증가
	m_pTarget->SetMagicalDefense(m_dwNow + wPeriod, 50, 0, 0, 0);
	// 일반 보호계열 마법 상태임을 설정
	m_pTarget->dwNormalProtect = m_dwNow + wPeriod;
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
	SendMagicResult();
	return true;
}

bool CProtectionMagic::IceResist()
{ // [37]빙계 저항 : 얼음계열 공격마법에 대한 방어력 50% 증가
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	// 얼음계열 공격마법에 대한 방어력 50% 증가
	m_pTarget->SetMagicalDefense(m_dwNow + wPeriod, 0, 50, 0, 0);
	// CSD-021216 : 얼림 저주 마법 방어
	m_pTarget->SetCurseDefense(m_dwNow + wPeriod, CMagicState::CURSE_FREEZE);
	// 일반 보호계열 마법 상태임을 설정
	m_pTarget->dwNormalProtect = m_dwNow + wPeriod;
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	m_packet.u.magic.server_magic_result_t.wDuration= wPeriod;
	SendMagicResult();
	return true;
}

bool CProtectionMagic::ElectResist()
{ // [47]전격에 대한 저항 : 전격계열 공격 마법에 대한 방어력 50% 증가
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	// 전격계열 공격 마법에 대한 방어력 50% 증가
	m_pTarget->SetMagicalDefense(m_dwNow + wPeriod, 0, 0, 50, 0);
	// 일반 보호계열 마법 상태임을 설정
	m_pTarget->dwNormalProtect = m_dwNow + wPeriod;
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
	SendMagicResult();
	return true;
}

bool CProtectionMagic::TwinkleStar()
{ // [53]수호성 : 물리적 공격 데미지 40% 방어, 저주 1계열의 마법 100% 방어
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	// 물리적 방어력 30% 증가
	m_pTarget->SetPhysicalDefense(m_dwNow + wPeriod, 40, 40, 40);
	// 저주 1계열 마법 방어
	m_pTarget->SetCurseDefense(m_dwNow + wPeriod, CMagicState::CURSE_1);
	// 일반 보호계열 마법 상태임을 설정
	m_pTarget->dwNormalProtect = m_dwNow + wPeriod;
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
	SendMagicResult();
	return true;
}

bool CProtectionMagic::Protection()
{ // [2]보호 : 물리적 공격 데미지 30% 방어, 저주 1, 3, 5계열의 마법 100% 방어
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	// 물리적 방어력 30% 증가
	m_pTarget->SetPhysicalDefense(m_dwNow + wPeriod, 30, 30, 30);
	// 저주 3, 4, 5계열 마법 방어
	m_pTarget->SetCurseDefense(m_dwNow + wPeriod, 
		CMagicState::CURSE_1|CMagicState::CURSE_3|CMagicState::CURSE_4);
	// 일반 보호계열 마법 상태임을 설정
	m_pTarget->dwNormalProtect = m_dwNow + wPeriod;
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
	SendMagicResult();
	return true;
}

bool CProtectionMagic::Shield()
{ // [3]보호막 : 물리적 공격 데미지, 불, 얼음, 전격계열의 공격마법 40% 방어
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	// 물리적 방어력 40% 증가
	m_pTarget->SetPhysicalDefense(m_dwNow + wPeriod, 40, 40, 40);
	// 불, 얼음, 전격계열 공격마법 방어력 40 증가
	m_pTarget->SetMagicalDefense(m_dwNow + wPeriod, 40, 40, 40, 0);
	// 일반 보호계열 마법 상태임을 설정
	m_pTarget->dwNormalProtect = m_dwNow + wPeriod;
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
	SendMagicResult();
	return true;
}

bool CProtectionMagic::Bless()
{ // [153]신의 축복 : 저주계열(1, 2, 3, 4) 마법을 100% 방어
	// 유지시간 계산
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	// 저주 1, 2, 3, 4계열 마법 방어
	m_pTarget->SetCurseDefense(m_dwNow + wPeriod, 
		CMagicState::CURSE_1|CMagicState::CURSE_2|CMagicState::CURSE_3|CMagicState::CURSE_4);
	// 일반 보호계열 마법 상태임을 설정
	m_pTarget->dwNormalProtect = m_dwNow + wPeriod;
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
	SendMagicResult();
	return true;
}

bool CProtectionMagic::ProtectDanger()
{	// [152]위험으로부터 보호 : 물리적 공격 데미지 40 ~ 50% 방어, 저주계열(1, 2) 방어
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	// 방어률 계산
	const int nWis = m_pCaster->GetAbility(WIS) - GetRequireWidom(m_nIndex);
	const int nTactics = m_pCaster->Skill[TACTICS_Orison] - GetRequireTactics(m_nIndex);
	int nBasic = (nWis + nTactics)/10 + 40;
	if (nBasic < 0)   nBasic = 0;
	if (nBasic > 50)  nBasic = 50;
	m_pTarget->SetPhysicalDefense(m_dwNow + wPeriod, nBasic, nBasic, nBasic);
	// 저주 1, 2계열 마법 방어
	m_pTarget->SetCurseDefense(m_dwNow + wPeriod, 
		CMagicState::CURSE_1|CMagicState::CURSE_2);
	// 일반 보호계열 마법 상태임을 설정
	m_pTarget->dwNormalProtect = m_dwNow + wPeriod;
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
	SendMagicResult();
	return true;
}

bool CProtectionMagic::GreatShield()
{ // [158]위대한 기도
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	// 방어률 계산
	const int nWis = m_pCaster->GetAbility(WIS) - GetRequireWidom(m_nIndex);
	const int nTactics = m_pCaster->Skill[TACTICS_Orison] - GetRequireTactics(m_nIndex);
	int nBasic = (nWis + nTactics)/10 + 40;
	if (nBasic < 0)   nBasic = 0;
	if (nBasic > 50)  nBasic = 50;
	m_pTarget->SetPhysicalDefense(m_dwNow + wPeriod, nBasic, nBasic, nBasic);
	// 저주 1, 2계열 마법 방어
	m_pTarget->SetCurseDefense(m_dwNow + wPeriod, 
		CMagicState::CURSE_1|CMagicState::CURSE_2);
	// 일반 보호계열 마법 상태임을 설정
	m_pTarget->dwNormalProtect = m_dwNow + wPeriod;
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
	SendMagicResult();
	return true;
}

bool CProtectionMagic::AuthorityLethy()
{ //< CSD-021019 : [184]레티의 권능
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	// 물리적 공격 방어력 계산
	const int nWis = m_pCaster->GetAbility(WIS) - GetRequireWidom(m_nIndex);
	const int nTactics = m_pCaster->Skill[TACTICS_Orison] - GetRequireTactics(m_nIndex);
	int nBasic = (nWis + nTactics)/10;
	if (nBasic < 0)   nBasic = 0;
	if (nBasic > 10)  nBasic = 10;
	// 물리 공격 방어력 40 - 50% 증가
	const int nStrike = 40 + nBasic;
	m_pTarget->SetPhysicalDefense(m_dwNow + wPeriod, nStrike, nStrike, nStrike);
	// 공격 마법 방어력 20 - 30% 증가
	const int nMagic = 20 + nBasic;
	m_pTarget->SetMagicalDefense(m_dwNow + wPeriod, nMagic, nMagic, nMagic, nMagic);
	// 일반 보호계열 마법 상태임을 설정
	m_pTarget->dwNormalProtect = m_dwNow + wPeriod;
	// 물리, 마법 공격 데미지 반사 상태임을 설정
	m_pTarget->dwStrikeReflect = m_dwNow + wPeriod;
	m_pTarget->dwMagicReflect = m_dwNow + wPeriod;
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
	SendMagicResult();
	return true;
} //> CSD-021019

bool CProtectionMagic::NullityMagic()
{ // [24]마법무효화 : 일정 시간동안 공격계열 마법을 100% 방어(데미지만큼 마나 소비)
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	// 공격계열 마법을 100% 방어(데미지만큼 마나 소비)
	m_pTarget->dwMagicDamageToMana = m_dwNow + wPeriod;
	// 특수 보호계열 마법 상태임을 설정
	m_pTarget->dwSpecialProtect = m_dwNow + wPeriod;
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
	SendMagicResult();
	return true;
}

bool CProtectionMagic::CancelDivine()
{ // [159]반신력 무효화 : 저주계열(1, 2, 3, 4, 5)  마법을 100% 방어
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	// 저주 1, 2, 3, 4, 5계열 마법 방어
	m_pTarget->SetCurseDefense(m_dwNow + wPeriod, 
		CMagicState::CURSE_1|CMagicState::CURSE_2|CMagicState::CURSE_3|CMagicState::CURSE_4|CMagicState::CURSE_5);
	// 특수 보호계열 마법 상태임을 설정
	m_pTarget->dwNormalProtect = m_dwNow + wPeriod;
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
	SendMagicResult();
	return true;
}

bool CProtectionMagic::RecurrenceDivine()
{ // [160]회귀의 권능 : 저주 1, 2, 3, 4계열 마법을 시전자에게 되돌림
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	// 저주계열(1, 2, 3, 4) 마법을 시전자에게 되돌림
	m_pCaster->dwReturnMagic = m_dwNow + wPeriod;
	// 특수 보호계열 마법 상태임을 설정
	m_pCaster->dwSpecialProtect = m_dwNow + wPeriod;
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
	SendMagicResult();
	return true;
}

bool CProtectionMagic::PerfectProtect()
{	// [157]완전한 결계 : 모든 마법 방어와 물리적 공격(Tactic이 40부터는 100%)에 대한 
	//                    방어시전자는 회복계열, 보호계열, 보조계열 마법들은 사용할 수 없음
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	// 물리적 방어율 계산
	const int nTactics = m_pCaster->Skill[TACTICS_Orison];
	int nBasic = 80 + (nTactics>>1);
	if (nBasic > 100)  nBasic = 100;
	// 물리적 방어력
	m_pTarget->SetPhysicalDefense(m_dwNow + wPeriod, nBasic, nBasic, nBasic);
	m_pTarget->SetMagicalDefense(m_dwNow + wPeriod, nBasic, nBasic, nBasic, nBasic);
	m_pTarget->SetCurseDefense(m_dwNow + wPeriod, 
		CMagicState::CURSE_1|CMagicState::CURSE_2|
		CMagicState::CURSE_3|CMagicState::CURSE_4|
		CMagicState::CURSE_5);
	//  예외 보호 계열 마법은 일반이면서 특수 보호계열 마법 상태로 설정
	m_pTarget->dwExceptProtect = m_dwNow + wPeriod;
	m_pTarget->dwNormalProtect = m_dwNow + wPeriod;
	m_pTarget->dwSpecialProtect = m_dwNow + wPeriod;
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
	SendMagicResult();
	return true;
}
bool CProtectionMagic::AuthorityOfAthyas() // 030415 kyo 
{ // [185] 아샤스의 권능 물리방어20, 공격마법방어50, 일반 물리,이동속도 30%증가
	// 레티의 권능 + 보호 + 속도증가
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	// 방어률 계산
	const int nWis = m_pCaster->GetAbility(WIS) - GetRequireWidom(m_nIndex);
	const int nTactics = m_pCaster->Skill[TACTICS_Orison] - GetRequireTactics(m_nIndex);
	int nBasic = (nWis + nTactics)/10;
	if (nBasic < 0)   nBasic = 0;
	if (nBasic > 10)  nBasic = 10;
	
	//물리방어 10 - 20
	const int nStrike = nBasic + 10;
	m_pTarget->SetPhysicalDefense(m_dwNow + wPeriod, nStrike, nStrike, nBasic);
	// 저주 1, 2계열 마법 방어
	//m_pTarget->SetCurseDefense(m_dwNow + wPeriod, CMagicState::CURSE_1|CMagicState::CURSE_2);
	// 공격 마법 방어력 40 - 50% 증가
	const int nMagic = 40 + nBasic;
	m_pTarget->SetMagicalDefense(m_dwNow + wPeriod, nMagic, nMagic, nMagic, nMagic);
	// 일반 보호계열 마법 상태임을 설정
	m_pTarget->dwNormalProtect = m_dwNow + wPeriod;
	// 이동속도 20-30%증가
	const int nSpeed = 20 + nBasic;
	m_pTarget->speedUp.SetState(m_dwNow + wPeriod, nSpeed);
	
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
	SendMagicResult();
	return true;
}