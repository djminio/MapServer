// CursingMagic.cpp: implementation of the CCursingMagic class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "CursingMagic.h"

#include "..\HigherLayers\Op_Magic.h"
#include "..\HigherLayers\Op_Battle.h"
#include "..\HigherLayers\Op_chr_status.h"
#include "..\HigherLayers\NationSys.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CCursingMagic::CCursingMagic()
{

}

CCursingMagic::~CCursingMagic()
{

}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

bool CCursingMagic::Bind()
{ 
	Resist(CONFUSION, &CCursingMagic::Confusion);
	Resist(ABSOLUTE_PARALYSIS, &CCursingMagic::AbsoluteParalysis);
	Resist(STONELY, &CCursingMagic::Stonely);
	Resist(CURSE, &CCursingMagic::Curse);
	Resist(AREA_PARALYSIS, &CCursingMagic::AreaParalysis);
	Resist(AREA_CONFUSION, &CCursingMagic::AreaConfusion);
	Resist(SLOW, &CCursingMagic::Slow);
	Resist(SPIDER_WEB, &CCursingMagic::SpiderWeb);
	Resist(AREA_SLOW, &CCursingMagic::AreaSlow);
	Resist(FEATHER_OF_CURSE, &CCursingMagic::FeatherOfCurse);
	Resist(INTOXICATION, &CCursingMagic::Intoxication);
	Resist(TOXIC_CLOUD, &CCursingMagic::ToxicCloud);
	Resist(VAMPIRE_POISON, &CCursingMagic::VampirePoison);
	Resist(CURSE_OF_HELL, &CCursingMagic::CurseOfHell);
	Resist(AREA_POISON, &CCursingMagic::AreaPoison);
	Resist(LOST_SIGHT, &CCursingMagic::LostSight);
	Resist(AUTHORITY_LOST_SIGHT, &CCursingMagic::AuthorityLostSight);
	Resist(DISEASE_INDUCE, &CCursingMagic::DiseaseInduce);
	Resist(VAMPIRE_TOUCH, &CCursingMagic::VampireTouch);
	Resist(LIFE_DRAIN, &CCursingMagic::LifeDrain);
	Resist(GAZER, &CCursingMagic::Gazer); // 030509 kyo
	return true;
} 

bool CCursingMagic::Elapse(CHARLIST* pTarget)
{
	const int nState = pTarget->GetState();
	
	switch (nState)
	{ 
    case CON_NORMAL:
    case CON_DEATH:
		{
			break;
		}
    default:
		{ // 각 저주상태 검사
			CheckStun(pTarget);
			CheckSlow(pTarget);
			CheckPoison(pTarget);
			CheckBlind(pTarget);
			CheckFreeze(pTarget);
			// 상태변경
			if (nState != pTarget->GetState())
			{
				::SendCharacterCondition(pTarget, 0);
			}
			// 10초에 한번씩 HP감소
			if (pTarget->GetPoisonedCount() >= 10)
			{
				const int nPoison = pTarget->GetPoisonedPoint();
				const int nLife = pTarget->DecLife(nPoison, 10);
				
				if (nLife != 0)
				{
					::SendCharacterCondition(pTarget, -nLife);
				}
				
				pTarget->SetPoisonedCount(0);
			}
			
			break;    
		}
	} 
	
	return true;
}

bool CCursingMagic::Confusion()
{	// [11]혼란 : 상대방이 마우스 클릭한 곳과 다른 곳으로 이동, 마법 사용 불가
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	
	if (IsReturn(m_pTarget))
	{ // 시전자가 회귀의 권능 마법 상태라면
		m_pCaster->dwDontCasting = m_dwNow + wPeriod;
		// 대상자의 상태 설정
		m_pCaster->ApplyCurse(1, CON_DAZE, m_dwNow + wPeriod);
		// 결과값을 대상자에게 Packet으로 보내기
		InitMagicResult(CMD_MAGIC_RESULT_T);
		m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
		m_packet.u.magic.server_magic_result_t.idTarget = m_idCaster;
		m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
		m_packet.u.magic.server_magic_result_t.nState = m_pCaster->GetState();
		m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
		SendMagicResult();
		return true;
	}
	
	m_pTarget->dwDontCasting = m_dwNow + wPeriod;
	// 대상자의 상태 설정
	m_pTarget->ApplyCurse(1, CON_DAZE, m_dwNow + wPeriod);
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// NK에 계산
	::CheckNK(m_idCaster, m_idTarget, NK_TYPE_NO_KILL_);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.wDuration= wPeriod;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}

bool CCursingMagic::AbsoluteParalysis()
{	// [14]절대마비 : 마법 사용 불가, 물리적 공격 불가, 위치 이동 불가
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	
	if (IsReturn(m_pTarget))
	{
		m_pCaster->dwDontCasting = m_dwNow + wPeriod;
		m_pCaster->dwDontAttack = m_dwNow + wPeriod;
		m_pCaster->dwDontMove = m_dwNow + wPeriod;
		m_pCaster->ApplyCurse(1, CON_STONE, m_dwNow + wPeriod); 
		// 결과값을 대상자에게 Packet으로 보내기
		InitMagicResult(CMD_MAGIC_RESULT_T);
		m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
		m_packet.u.magic.server_magic_result_t.idTarget = m_idCaster;
		m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
		m_packet.u.magic.server_magic_result_t.nState = m_pCaster->GetState();
		m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
		SendMagicResult();
		return true;
	}
	
	m_pTarget->dwDontCasting = m_dwNow + wPeriod;
	m_pTarget->dwDontAttack = m_dwNow + wPeriod;
	m_pTarget->dwDontMove = m_dwNow + wPeriod;
	m_pTarget->ApplyCurse(1, CON_STONE, m_dwNow + wPeriod);
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// NK에 계산
	::CheckNK(m_idCaster, m_idTarget, NK_TYPE_NO_KILL_);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}

bool CCursingMagic::Stonely()
{	// [13]석화 : 몸이 굳어서 움직일 수 없는 상태(물리적 공격이나 마법 사용이 불가,저주해제를 제외한 모든 마법의 적용이 안됨, 어떠한 데미지도 받지 못함)
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	
	if (IsReturn(m_pTarget))
	{
		m_pCaster->dwDontCasting = m_dwNow + wPeriod;
		m_pCaster->dwDontAttack = m_dwNow + wPeriod;
		m_pCaster->dwDontMove = m_dwNow + wPeriod;
		m_pCaster->dwBlocking = m_dwNow + wPeriod;
		m_pCaster->ApplyCurse(1, CON_STONE, m_dwNow + wPeriod);
		// 결과값을 대상자에게 Packet으로 보내기
		InitMagicResult(CMD_MAGIC_RESULT_T);
		m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
		m_packet.u.magic.server_magic_result_t.idTarget = m_idCaster;
		m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
		m_packet.u.magic.server_magic_result_t.nState = m_pCaster->GetState();
		m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
		SendMagicResult();
		return true;
	}
	
	m_pTarget->dwDontCasting = m_dwNow + wPeriod;
	m_pTarget->dwDontAttack = m_dwNow + wPeriod;
	m_pTarget->dwDontMove = m_dwNow + wPeriod;
	m_pTarget->dwBlocking = m_dwNow + wPeriod;
	m_pTarget->ApplyCurse(1, CON_STONE, m_dwNow + wPeriod);
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// NK에 계산
	::CheckNK(m_idCaster, m_idTarget, NK_TYPE_NO_KILL_);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.wDuration= wPeriod;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}

bool CCursingMagic::Curse()
{	// [172]신의 저주 : 마법 사용 불가
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	
	if (IsReturn(m_pTarget))
	{
		m_pCaster->dwDontCasting = m_dwNow + wPeriod;
		m_pCaster->ApplyCurse(1, CON_CURSE, m_dwNow + wPeriod);
		// 결과값을 대상자에게 Packet으로 보내기
		InitMagicResult(CMD_MAGIC_RESULT_T);
		m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
		m_packet.u.magic.server_magic_result_t.idTarget = m_idCaster;
		m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
		m_packet.u.magic.server_magic_result_t.nState = m_pCaster->GetState();
		m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
		SendMagicResult();
		return true;
	}
	
	m_pTarget->dwDontCasting = m_dwNow + wPeriod;
	m_pTarget->ApplyCurse(1, CON_CURSE, m_dwNow + wPeriod);
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// NK에 계산
	::CheckNK(m_idCaster, m_idTarget, NK_TYPE_NO_KILL_);
	// 결과값 설정
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.wDuration= wPeriod;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}

bool CCursingMagic::AreaParalysis()
{	// [112]지역 마비
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	m_pTarget->dwDontCasting = m_dwNow + wPeriod;
	m_pTarget->dwDontAttack = m_dwNow + wPeriod;
	m_pTarget->dwDontMove = m_dwNow + wPeriod;
	m_pTarget->ApplyCurse(1, CON_STONE, m_dwNow + wPeriod);
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// NK에 계산
	::CheckNK(m_idCaster, m_idTarget, NK_TYPE_NO_KILL_);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}

bool CCursingMagic::AreaConfusion()
{	// [114]지역 혼란
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	m_pTarget->dwDontCasting = m_dwNow + wPeriod;
	// 대상자의 상태 설정
	m_pTarget->ApplyCurse(1, CON_DAZE, m_dwNow + wPeriod);
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// NK에 계산
	::CheckNK(m_idCaster, m_idTarget, NK_TYPE_NO_KILL_);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.wDuration= wPeriod;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}

bool CCursingMagic::Slow()
{	// [10]속도저하 : 상대방의 이동속도(MovP)를 1/2로 감소
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	
	if (IsReturn(m_pTarget))
	{
		m_pCaster->speedDown.SetState(m_dwNow + wPeriod, 50);
		m_pCaster->ApplyCurse(2, CON_SLOW, m_dwNow + wPeriod);
		// 결과값을 대상자에게 Packet으로 보내기
		InitMagicResult(CMD_MAGIC_RESULT_T);
		m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
		m_packet.u.magic.server_magic_result_t.idTarget = m_idCaster;
		m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
		m_packet.u.magic.server_magic_result_t.nState = m_pCaster->GetState();
		m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
		SendMagicResult();
		return true;
	}
	
	m_pTarget->speedDown.SetState(m_dwNow + wPeriod, 50);
	m_pTarget->ApplyCurse(2, CON_SLOW, m_dwNow + wPeriod);
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// NK에 계산
	::CheckNK(m_idCaster, m_idTarget, NK_TYPE_NO_KILL_);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.wDuration= wPeriod;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}

bool CCursingMagic::SpiderWeb()
{	// [19]거미줄	: 상대방의 이동속도(MovP) 1/3 줄이고, 공격 성공률 감소                                
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	
	if (IsReturn(m_pTarget))
	{ 
		m_pCaster->speedDown.SetState(m_dwNow + wPeriod, 30);
		m_pCaster->ApplyCurse(2, CON_SLOW, m_dwNow + wPeriod);
		// 결과값을 대상자에게 Packet으로 보내기
		InitMagicResult(CMD_MAGIC_RESULT_T);
		m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
		m_packet.u.magic.server_magic_result_t.idTarget = m_idCaster;
		m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
		m_packet.u.magic.server_magic_result_t.nState = m_pCaster->GetState();
		m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
		SendMagicResult();
		return true;
	} 
	
	m_pTarget->speedDown.SetState(m_dwNow + wPeriod, 30);
	m_pTarget->ApplyCurse(2, CON_SLOW, m_dwNow + wPeriod);
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// NK에 계산
	::CheckNK(m_idCaster, m_idTarget, NK_TYPE_NO_KILL_);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.wDuration= wPeriod;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}

bool CCursingMagic::AreaSlow()
{	// [115]지역 속도저하
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	m_pTarget->speedDown.SetState(m_dwNow + wPeriod, 30);
	m_pTarget->ApplyCurse(2, CON_SLOW, m_dwNow + wPeriod);
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// NK에 계산
	::CheckNK(m_idCaster, m_idTarget, NK_TYPE_NO_KILL_);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.wDuration= wPeriod;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}

bool CCursingMagic::FeatherOfCurse()
{	// [93]마비의 깃털
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	
	if (IsReturn(m_pTarget))
	{
		m_pCaster->speedDown.SetState(m_dwNow + wPeriod, 50);
		m_pCaster->ApplyCurse(2, CON_SLOW, m_dwNow + wPeriod);
		// 결과값을 대상자에게 Packet으로 보내기
		InitMagicResult(CMD_MAGIC_RESULT_T);
		m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
		m_packet.u.magic.server_magic_result_t.idTarget = m_idCaster;
		m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
		m_packet.u.magic.server_magic_result_t.nState = m_pCaster->GetState();
		m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
		SendMagicResult();
		return true;
	}
	
	m_pTarget->speedDown.SetState(m_dwNow + wPeriod, 50);
	m_pTarget->ApplyCurse(2, CON_SLOW, m_dwNow + wPeriod);
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// NK에 계산
	::CheckNK(m_idCaster, m_idTarget, NK_TYPE_NO_KILL_);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.wDuration= wPeriod;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}

bool CCursingMagic::Intoxication()
{	// [12]중독 : 마법의 기본 데미지값만큼 HP 감소 후 일정량의 HP가 서서히 감소
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	const int nBase = 100 + m_pTarget->HpMax*0.05 + (m_pCaster->Skill[TACTICS_Magery]>>2);
	const int nPoison = __min(nBase, 2000);
	
	if (IsReturn(m_pTarget))
	{
		m_pCaster->SetPoisonedPoint(nPoison);
		m_pCaster->ApplyCurse(3, CON_POISON, m_dwNow + wPeriod);
		// 결과값을 대상자에게 Packet으로 보내기
		InitMagicResult(CMD_MAGIC_RESULT_T);
		m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
		m_packet.u.magic.server_magic_result_t.idTarget = m_idCaster;
		m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
		m_packet.u.magic.server_magic_result_t.nState = m_pCaster->GetState();
		m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
		SendMagicResult();
		return true;
	}
	
	m_pTarget->SetPoisonedPoint(nPoison);
	m_pTarget->ApplyCurse(3, CON_POISON, m_dwNow + wPeriod);
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// NK에 계산
	::CheckNK(m_idCaster, m_idTarget, NK_TYPE_NO_KILL_);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}

bool CCursingMagic::ToxicCloud()
{	// [15]독구름 : 일정지역 내에 중독이 발생
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	const int nBase = 100 + m_pTarget->HpMax*0.04 + (m_pCaster->Skill[TACTICS_Magery]>>1);
	const int nPoison = __min(nBase, 2000);
	// 기본 마법 데미지
	if (IsReturn(m_pTarget))
	{
		m_pCaster->SetPoisonedPoint(nPoison);
		m_pCaster->ApplyCurse(3, CON_POISON, m_dwNow + wPeriod);
		// 결과값을 대상자에게 Packet으로 보내기
		InitMagicResult(CMD_MAGIC_RESULT_T);
		m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
		m_packet.u.magic.server_magic_result_t.idTarget = m_idCaster;
		m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
		m_packet.u.magic.server_magic_result_t.nState = m_pCaster->GetState();
		m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
		SendMagicResult();
		return true;
	}
	
	m_pTarget->SetPoisonedPoint(nPoison);
	m_pTarget->ApplyCurse(3, CON_POISON, m_dwNow + wPeriod);
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// NK에 계산
	::CheckNK(m_idCaster, m_idTarget, NK_TYPE_NO_KILL_);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}

bool CCursingMagic::VampirePoison()
{	//< CSD-TW-030627 : [90]뱀파이어의 독
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	const int nPoison = m_pTarget->HpMax*0.1;
	m_pTarget->SetPoisonedPoint(nPoison);
	m_pTarget->ApplyCurse(3, CON_POISON, m_dwNow + wPeriod);
	// 시전자의 경험치 계산 : 공격마법으로 간주해서 죽지 않은 경우
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// NK에 계산
	::CheckNK(m_idCaster, m_idTarget, NK_TYPE_NO_KILL_);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}	//> CSD-TW-030627

bool CCursingMagic::CurseOfHell()
{	//< CSD-TW-030627 : [94]헬의 저주
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	const int nPoison = m_pTarget->HpMax*0.1;
	m_pTarget->SetPoisonedPoint(nPoison);
	m_pTarget->ApplyCurse(3, CON_POISON, m_dwNow + wPeriod);
	// 시전자의 경험치 계산 : 공격마법으로 간주해서 죽지 않은 경우
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// NK에 계산
	::CheckNK(m_idCaster, m_idTarget, NK_TYPE_NO_KILL_);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}	//> CSD-TW-030627

bool CCursingMagic::AreaPoison()
{	// [113]지역 중독
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	const int nPoison = m_pTarget->HpMax*0.05;
	m_pTarget->SetPoisonedPoint(nPoison);
	m_pTarget->ApplyCurse(3, CON_POISON, m_dwNow + wPeriod);
	// 시전자의 경험치 계산 : 공격마법으로 간주해서 죽지 않은 경우
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// NK에 계산
	::CheckNK(m_idCaster, m_idTarget, NK_TYPE_NO_KILL_);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}

bool CCursingMagic::LostSight()
{	// [82] 시력상실 : 시야를 2타일로 좁힘
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	
	if (IsReturn(m_pTarget))
	{
		m_pCaster->ApplyCurse(4, CON_DARKNESS, m_dwNow + wPeriod);
		// 시야상실 상태를 클라이언트로 전송
		::SendMagicCommand(m_pCaster, SET_LOSTSIGHT, 0, wPeriod);
		// 결과값을 대상자에게 Packet으로 보내기
		InitMagicResult(CMD_MAGIC_RESULT_T);
		m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
		m_packet.u.magic.server_magic_result_t.idTarget = m_idCaster;
		m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
		m_packet.u.magic.server_magic_result_t.nState = m_pCaster->GetState();
		m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
		SendMagicResult();
		return true;
	}
	
	m_pTarget->ApplyCurse(4, CON_DARKNESS, m_dwNow + wPeriod);
	
	if (m_pTarget->IsNpc())
	{	// NPC 및 몬스터의 시야상실 효과 연출함수 호출
		::NPCLostSight(m_pTarget, m_dwNow + wPeriod);
	}
	else
	{	// 시야상실 상태를 클라이언트로 전송
		::SendMagicCommand(m_pTarget, SET_LOSTSIGHT, 0, wPeriod);
	}
	// 시전자의 경험치 계산 : 공격마법으로 간주해서 죽지 않은 경우
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// NK에 계산
	::CheckNK(m_idCaster, m_idTarget, NK_TYPE_NO_KILL_);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.wDuration= wPeriod;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}

bool CCursingMagic::AuthorityLostSight()
{	// [177] 시력상실의 권능 : 시야를 2타일로 좁힘
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	
	if (IsReturn(m_pTarget))
	{
		m_pCaster->ApplyCurse(4, CON_DARKNESS, m_dwNow + wPeriod);
		// 시야상실 상태를 클라이언트로 전송
		::SendMagicCommand(m_pCaster, SET_LOSTSIGHT, 0, wPeriod);		
		// 결과값을 대상자에게 Packet으로 보내기
		InitMagicResult(CMD_MAGIC_RESULT_T);
		m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
		m_packet.u.magic.server_magic_result_t.idTarget = m_idCaster;
		m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
		m_packet.u.magic.server_magic_result_t.nState = m_pCaster->GetState();
		m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
		SendMagicResult();
		return true;
	}
	
	m_pTarget->ApplyCurse(4, CON_DARKNESS, m_dwNow + wPeriod);
	
	if (m_pTarget->IsNpc())
	{ // NPC 및 몬스터의 시야상실 효과 연출함수 호출
		::NPCLostSight(m_pTarget, m_dwNow  + wPeriod);
	}
	else
	{ // 시야상실 상태를 클라이언트로 전송
		::SendMagicCommand(m_pTarget, SET_LOSTSIGHT, 0, wPeriod);
	}
	// 시전자의 경험치 계산 : 공격마법으로 간주해서 죽지 않은 경우
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// NK에 계산
	::CheckNK(m_idCaster, m_idTarget, NK_TYPE_NO_KILL_);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}

bool CCursingMagic::DiseaseInduce()
{	//< CSD-031007 : [16]질병유발 : Health(배고픔 정도) 감소
	if (m_pTarget->IsNpc())
	{
		return true;
	}

	if (IsReturn(m_pTarget))
	{
		// 결과값을 대상자에게 Packet으로 보내기
		InitMagicResult(CMD_MAGIC_RESULT);
		m_packet.u.magic.server_magic_result.nMagic = m_nIndex;
		m_packet.u.magic.server_magic_result.idTarget = m_idCaster;
		m_packet.u.magic.server_magic_result.nState = m_pCaster->GetState();
		m_packet.u.magic.server_magic_result.nResult = HIT_AND_NOTDEAD;
		SendMagicResult();
		return true;
	}
	
	if (m_pTarget->DecHungry(m_pTarget->HungryMax/2) > 0)
	{
		m_pTarget->SendCharInfoBasic(SP, m_pTarget->Hungry);
		// 시전자의 경험치 계산 : 공격마법으로 간주해서 죽지 않은 경우
		::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	}
	// NK에 계산
	::CheckNK(m_idCaster, m_idTarget, NK_TYPE_NO_KILL_);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT);
	m_packet.u.magic.server_magic_result.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}	//> CSD-031007

bool CCursingMagic::VampireTouch()
{	// [17]뱀파이어의 손길 : 대상자의 HP를 빼앗아 시전자의 HP에 더함(대상자가 죽지는 않음)
	const int nDiff = m_pCaster->GetLevel() - m_pTarget->GetLevel(); // CSD-030806
	// HP를 빼앗을 % 계산
	int nPercent = (35 + nDiff) + m_pCaster->Skill[TACTICS_Magery]/4;
	if (nPercent > 80)  nPercent = 80;
	if (nPercent < 10)  nPercent = 10;
	
	if (IsReturn(m_pTarget))
	{
		const int nTemp = m_pTarget->GetPureLife()*nPercent/100; // Hp를 빼았는 량
		const int nMinus = m_pCaster->DecLife(nTemp, 1);    // 실제 HP를 빼았는 량
		const int nPlus = m_pTarget->IncLife(nMinus);       // 실제 HP를 더하는 량
		::SendCharacterCondition(m_pTarget, nPlus);
		// 결과값을 대상자에게 Packet으로 보내기
		InitMagicResult(CMD_MAGIC_RESULT_H);
		m_packet.u.magic.server_magic_result_h.nMagic = m_nIndex;
		m_packet.u.magic.server_magic_result_h.idTarget = m_idCaster;
		m_packet.u.magic.server_magic_result_h.nLife = -nMinus;
		m_packet.u.magic.server_magic_result_h.nState = m_pCaster->GetState();
		m_packet.u.magic.server_magic_result_h.nResult = HIT_AND_NOTDEAD;
		SendMagicResult();
		return true;
	}
	
	const int nTemp = m_pCaster->GetPureLife()*nPercent/100; // Hp를 빼았는 량
	const int nMinus = m_pTarget->DecLife(nTemp, 1);    // 실제 HP를 빼았는 량
	const int nPlus = m_pCaster->IncLife(nMinus);       // 실제 HP를 더하는 량
	::SendCharacterCondition(m_pCaster, nPlus);
	// 시전자의 경험치 계산 : 공격마법으로 간주해서 죽지 않은 경우
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 1, abs(nMinus));
	// NK에 계산
	::CheckNK(m_idCaster, m_idTarget, NK_TYPE_NO_KILL_);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_H);
	m_packet.u.magic.server_magic_result_h.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_h.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_h.nLife = -nMinus;
	m_packet.u.magic.server_magic_result_h.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_h.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}

bool CCursingMagic::LifeDrain()
{	// [190]생명력 탈취 : 대상자의 HP를 빼앗아 시전자의 HP에 더함(레벨에 따른 체력치 흡수량 차등 적용
	const int nDiff = m_pCaster->GetLevel() - m_pTarget->GetLevel(); // CSD-030806
	// HP를 빼앗을 % 계산
	int nPercent = (50 + nDiff) + m_pCaster->Skill[TACTICS_Orison]/3;
	if (nPercent > 90)  nPercent = 90;
	if (nPercent < 10)  nPercent = 10;
	
	if (IsReturn(m_pTarget))
	{
		const int nTemp = m_pTarget->GetPureLife()*nPercent/100; // Hp를 빼았는 량
		const int nMinus = m_pCaster->DecLife(nTemp, 1);    // 실제 HP를 빼았는 량
		const int nPlus = m_pTarget->IncLife(nMinus);       // 실제 HP를 더하는 량
		::SendCharacterCondition(m_pTarget, nPlus);
		// 결과값을 대상자에게 Packet으로 보내기
		InitMagicResult(CMD_MAGIC_RESULT_H);
		m_packet.u.magic.server_magic_result_h.nMagic = m_nIndex;
		m_packet.u.magic.server_magic_result_h.idTarget = m_idCaster;
		m_packet.u.magic.server_magic_result_h.nLife = -nMinus;
		m_packet.u.magic.server_magic_result_h.nState = m_pCaster->GetState();
		m_packet.u.magic.server_magic_result_h.nResult = HIT_AND_NOTDEAD;
		SendMagicResult();
		return true;
	}
	
	const int nTemp = m_pCaster->GetPureLife()*nPercent/100; // Hp를 빼았는 량
	const int nMinus = m_pTarget->DecLife(nTemp, 1);    // 실제 HP를 빼았는 량
	const int nPlus = m_pCaster->IncLife(nMinus);       // 실제 HP를 더하는 량
	::SendCharacterCondition(m_pCaster, nPlus);
	// 시전자의 경험치 계산 : 공격마법으로 간주해서 죽지 않은 경우
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 1, abs(nMinus));
	// NK에 계산
	::CheckNK(m_idCaster, m_idTarget, NK_TYPE_NO_KILL_);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_H);
	m_packet.u.magic.server_magic_result_h.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_h.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_h.nLife = -nMinus;
	m_packet.u.magic.server_magic_result_h.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_h.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}

bool CCursingMagic::Gazer()
{	// [109]30%의 확률로 신의 저주(마법사용불가)를 실행한다
	if( rand()%2 == 0 ) // 030514 kyo 50%확률
	{
		return false;
	}

	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	m_pTarget->dwDontCasting = m_dwNow + wPeriod;
	m_pTarget->ApplyCurse(1, CON_CURSE, m_dwNow + wPeriod);
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.wDuration= wPeriod;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}


///////////////////////////////////////////////////////////////////////////////
// Protected Method
///////////////////////////////////////////////////////////////////////////////

bool CCursingMagic::CheckDeath(int nDamage)
{	//< CSD-TW-030627
	if (IsReturn(m_pCaster))    return false;
	// Life 감소
	const int nLife = m_pTarget->DecLife(nDamage); 

	if (nLife > 0)
	{
		::SendCharacterCondition(m_pTarget, -nLife);
	}	
	
	if (m_pTarget->IsDead())
	{
		if (m_pCaster->IsPlayer() && m_pTarget->IsPlayer())
		{
			::KillWho(m_pTarget, m_pCaster);
		}

		if (m_pTarget->IsNpc())
		{
			m_pTarget->m_xSummon.Remove(KillMonster);
		}
		
		::killCharacter(m_pCaster, m_pTarget);
		// 시전자의 경험치 계산 : 공격마법으로 간주해서 죽은 경우
		::AddCasterEXP(m_pCaster, m_pTarget, 1, 1, abs(nDamage));
		// NK에 계산
		::CheckNK(m_idCaster, m_idTarget, NK_TYPE_NO_KILL_);
		// 결과값을 대상자에게 Packet으로 보내기
		InitMagicResult(CMD_MAGIC_RESULT);
		m_packet.u.magic.server_magic_result.nMagic = m_nIndex;
		m_packet.u.magic.server_magic_result.idTarget = m_idTarget;
		m_packet.u.magic.server_magic_result.nState = CON_DEATH;
		m_packet.u.magic.server_magic_result.nResult = HIT_AND_DEAD;
		SendMagicResult();
		return true;
	}
	
	return false;
}	//> CSD-TW-030627

void CCursingMagic::CheckStun(CHARLIST* pTarget)
{
	if (pTarget->IsCurse(1, g_curr_time))
	{
		pTarget->ClearCurse(1);
	}
}

void CCursingMagic::CheckSlow(CHARLIST* pTarget)
{
	if (pTarget->IsCurse(2, g_curr_time))
	{
		pTarget->ClearCurse(2);
	}
}

void CCursingMagic::CheckPoison(CHARLIST* pTarget)
{
	if (pTarget->IsCurse(3, g_curr_time))
	{
		pTarget->ClearCurse(3);
	}
}

void CCursingMagic::CheckBlind(CHARLIST* pTarget)
{
	if (pTarget->IsCurse(4, g_curr_time))
	{
		pTarget->ClearCurse(4);
	}
}

void CCursingMagic::CheckFreeze(CHARLIST* pTarget)
{
	if (pTarget->IsCurse(6, g_curr_time))
	{
		pTarget->ClearCurse(6);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Private Method
///////////////////////////////////////////////////////////////////////////////

bool CCursingMagic::IsReturn(CHARLIST* pTarget) const
{
	return (pTarget->IsPlayer() && pTarget->IsReturn()) ? true:false;
}