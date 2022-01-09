// AssistanceMagic.cpp: implementation of the CAssistanceMagic class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "AssistanceMagic.h"

#include "..\HigherLayers\OP_Magic.h"
#include "..\HigherLayers\WeatherControl.h"

extern CHARLIST* CheckServerName( const char* szName );// kyo 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAssistanceMagic::CAssistanceMagic()
{
	
}

CAssistanceMagic::~CAssistanceMagic()
{
	
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

bool CAssistanceMagic::Bind()
{
	Resist(SPEED_UP, &CAssistanceMagic::SpeedUp);
	Resist(ENCHANT_WEAPON, &CAssistanceMagic::EnchantWeapon);
	Resist(PRAY_POWER, &CAssistanceMagic::PrayPower);
	Resist(GREAT_PRAY, &CAssistanceMagic::GreatPray);
	Resist(MINOR_AMPLIFY, &CAssistanceMagic::MinorAmplify);
	Resist(MAGIC_AMPLIFY, &CAssistanceMagic::MagicAmplify);
	Resist(TRANSPARENCY, &CAssistanceMagic::Transparency);
	Resist(LIGHT, &CAssistanceMagic::Light);
	Resist(AUTHORITY_LUCK, &CAssistanceMagic::AuthorityLuck);
	return true;
}

bool CAssistanceMagic::SpeedUp()
{	// [83]속도증가 : 이동속도를 20 ~ 50% 증가
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	// 증가률 계산
	const int nInt = m_pCaster->GetAbility(INT_);
	const int nTactics = m_pCaster->Skill[TACTICS_Magery];
	int nBasic = (nInt + nTactics)/20;
	if (nBasic < 0)   nBasic = 0;
	if (nBasic > 30)  nBasic = 30;
	// 이동속도 20 ~ 50% 증가
	m_pTarget->speedUp.SetState(m_dwNow + wPeriod, nBasic + 20);
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

bool CAssistanceMagic::EnchantWeapon()
{	// [21]인첸트웨폰 : 물리적 공격 데미지을 50% 증가
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	// 물리적 공격력 50% 증가
	m_pTarget->SetPhysicalAttack(m_dwNow + wPeriod, 50, 50, 50);
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

bool CAssistanceMagic::PrayPower()
{	// [161]기도의 역사 : 물리적 공격 데미지를 50% 증가
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	// 물리적 공격력 50% 증가
	m_pTarget->SetPhysicalAttack(m_dwNow + wPeriod, 50, 50, 50);
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

bool CAssistanceMagic::GreatPray()
{	// [166]위대한 기도
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	// 물리적 공격력 50% 증가
	m_pTarget->SetPhysicalAttack(m_dwNow + wPeriod, 50, 50, 50);
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

bool CAssistanceMagic::MinorAmplify()
{	// [25]소증폭 : 공격 마법의 데미지를 20% 증가
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	// 공격 마법의 데미지를 20% 증가
	m_pTarget->amplify.SetState(m_dwNow + wPeriod, 20);
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

bool CAssistanceMagic::MagicAmplify()
{	// [26]마법증폭 : 공격 마법의 데미지를 50% 증가
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	// 공격 마법의 데미지를 50% 증가
	m_pTarget->amplify.SetState(m_dwNow + wPeriod, 50);
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

bool CAssistanceMagic::Transparency()
{	// [22]투명주문 : 자신의 몸을 투명하게 만들어 줌
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	m_pTarget->dwTransparency = m_dwNow + wPeriod; 
	// 투명체 출력을 위한 VIEWTYPE 설정(transparency)
	m_pTarget->viewtype = VIEWTYPE_TRANSPARENCY_;
	::SendViewType(m_pTarget, VIEWTYPE_TRANSPARENCY_, m_pTarget->dwTransparency);
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	// 일반 성공으로 보내는 경우 클라이언트에서 다시 정상상태로 셋팅하므로 실패로 보냄
	m_packet.u.magic.server_magic_result_t.nResult = HIT_FAILED;
	m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
	SendMagicResult();
	return true;
}

bool CAssistanceMagic::Light()
{	// [46]빛 : 주위를 환하게 해줌(밤에 시야가 어두워지는 것을 위해)
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	m_pTarget->dwLight = m_dwNow + wPeriod;
	// 주위를 환하게 함
	WeatherControl.SetChLight(m_pCaster, nMaximumLight, wPeriod);
	
	for (int i = 0; i < MAX_PARTY_MEMBER; ++i)
	{
		if (m_pCaster->party[i].On)
		{ 
			//CHARLIST *Target = CheckServerId( m_pCaster->party[i].Server_id );
			CHARLIST *Target = CheckServerName( &(m_pCaster->party[i].Name[0]) );
			if(!Target){continue;}
			WeatherControl.SetChLight(Target,nMaximumLight/2,5*60);
		}
	}
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

bool CAssistanceMagic::AuthorityLuck()
{	// [175]절대 행운의 권능 : 모든 물리적 공격과 마법이 성공
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	m_pTarget->dwDontMiss = m_dwNow + wPeriod;
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