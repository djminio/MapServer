// ExtraMagic.cpp: implementation of the CExtraMagic class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "ExtraMagic.h"

#include "..\HigherLayers\OP_Magic.h"
#include "..\HigherLayers\OP_Battle.h"

int	PRAY_RECOVER_DIVINE_TIME			=	0	; // 030520 kyo
///////////////////////////////////////////////////////////////////////////////
// Static Variable
///////////////////////////////////////////////////////////////////////////////

static POINT s_aPlace[8][5] =
{
	{{ 0, -5}, {-3, -5}, { 3, -5}, {-5, -5}, { 5, -5}},
	{{-3, -3}, {-4,  0}, { 0, -4}, {-6,  3}, { 3, -6}},
	{{-5,  0}, {-5,  3}, {-5, -3}, {-5,  5}, {-5, -5}},
	{{-2,  2}, { 0,  4}, {-4,  0}, { 2,  6}, {-6, -2}},
	{{ 0,  5}, {-3,  5}, { 3,  5}, {-5,  5}, { 5,  5}},
	{{ 3,  3}, { 4,  0}, { 0,  4}, { 6, -3}, {-3,  6}},
	{{ 5,  0}, { 5,  3}, { 5, -3}, { 5,  5}, { 5, -5}},
	{{ 3, -3}, { 0, -4}, { 4,  0}, {-3, -6}, { 6,  3}}
};

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CExtraMagic::CExtraMagic()
{
    
}

CExtraMagic::~CExtraMagic()
{
	
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

bool CExtraMagic::Bind()
{ 
	Resist(PHASING, &CExtraMagic::Phasing);
	Resist(TELEPORT, &CExtraMagic::Teleport);
	Resist(MEMORIZING_LOCATION, &CExtraMagic::MemorizingLocation);
	Resist(TOWN_PORTAL, &CExtraMagic::TownPortal);
	Resist(MULTI_PORT, &CExtraMagic::MultiPort);
	Resist(VAMPIRE_DODGE, &CExtraMagic::VampireDodge);
	Resist(LOW_MONSTER_RECALL, &CExtraMagic::LowMonsterRecall);
	Resist(MIDDLE_MONSTER_RECALL, &CExtraMagic::MiddleMonsterRecall);
	Resist(SUMMONING_SKELETON, &CExtraMagic::SummoningSkeleton);
	Resist(SUMMONING_UNDEAD, &CExtraMagic::SummoningUndead);
	Resist(RECALL_FOLLOW, &CExtraMagic::RecallFollow);
	Resist(MAGICAL_TRAP, &CExtraMagic::MagicalTrap);
	Resist(MAGIC_DETECT, &CExtraMagic::MagicDetect);
	Resist(ANTHEM, &CExtraMagic::Anthem);
	Resist(INFLUENCE_DIVINE, &CExtraMagic::InfluenceDivine);
	Resist(VOICE_SOUL, &CExtraMagic::VoiceSoul);
	Resist(APPRECIATE, &CExtraMagic::Appreciate);
	Resist(PRAY, &CExtraMagic::Pray);	// 030415 kyo
	return true;
} 

bool CExtraMagic::Phasing()
{	// [64]장소이동 : 임의적으로 위치가 순간적으로 이동
	int nCount = 0;
	
	do	
	{
		m_nX = (m_pCaster->X>>5);
		m_nY = (m_pCaster->Y>>5);
		// 위치 얻기
		GetArea(m_nX, m_nY);
		//< CSD-030930
		if (m_nX < 0)
		{
			return false;
		}

		if (m_nY < 0)
		{
			return false;
		}
		//> CSD-030930
		if (++nCount > MAX_COUNT)
		{	// 10회 이상 시도시 실패했다면 마법이 실패한 것으로 간주
			return false;
		}
	} 
	while (!::IsMovable(m_nX, m_nY));
	
	::TransArea(PT_PHASING, m_pCaster, m_nX, m_nY);
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex); // 시전자의 경험치 계산
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT);
	m_packet.u.magic.server_magic_result.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}

bool CExtraMagic::Teleport()
{	// [66]순간이동 : 선택한 장소로 순간적으로 이동
	if (::IsMovable(m_nX, m_nY) == false)  
	{ // 이동을 할 수 없는 곳이라면
		return false;
	}
	
	::TransArea(PT_TELEPORT, m_pCaster, m_nX, m_nY);
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

bool CExtraMagic::MemorizingLocation()
{ // [65]이동장소기억 : 차원이동이나 복수차원으로 이동될 장소(맵이름, 좌표값)를 기억시킴
	if (::IsMovable(m_nX, m_nY) == false)  
	{ // 이동을 할 수 없는 곳이라면
		return false;
	}
	// 이동할 장소에 대한 정보(맵이름, 위치) 설정
	const int nPort = ::MapPort(m_pCaster->MapName);
	m_pCaster->SetTransMap(nPort, m_nX, m_nY);
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

bool CExtraMagic::TownPortal()
{ // [68]차원이동 : 자기자신을 기억된 장소(맵이름, 좌표값)로 이동
	if (!m_pCaster->IsTransMap())
	{ // 기억된 이동 장소 정보가 없다면
		return false;
	}
	
	::TransMap(m_pCaster);
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

bool CExtraMagic::MultiPort()
{ // [67]복수차원이동 : 일정 영역에 있는 사람들을 기억된 장소(맵이름, 좌표값)로 이동
	if (m_pCaster->IsTransMap() == false)  
	{ // 기억된 이동 장소 정보가 없다면 실패
		return false;
	}
	
	::TransMap(m_pCaster);
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

bool CExtraMagic::VampireDodge()        
{ // [91]뱀파이어 회피
	int nCount = 0;
	
	do	
	{
		m_nX = (m_pCaster->X>>5);
		m_nY = (m_pCaster->Y>>5);
		// 위치 얻기
		GetArea(m_nX, m_nY);    
		// 10회 이상 시도시 실패했다면 마법이 실패한 것으로 간주
		if (++nCount > MAX_COUNT)	 
		{
			return false;
		}
	} 
	while (!::IsMovable(m_nX, m_nY));
	
	::TransArea(PT_DODGE, m_pCaster, m_nX, m_nY);
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex); // 시전자의 경험치 계산
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT);
	m_packet.u.magic.server_magic_result.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}

bool CExtraMagic::LowMonsterRecall()
{	// [55]저급 몬스터 소환
	if (m_pCaster->m_xSummon.IsExist() == true)
	{ // 소환된 몬스터가 있다면
		m_pCaster->m_xSummon.Remove(KillMonster);
	}
	
	int nCreate = 0;
	
	switch (rand()%3)
	{
    case 0:  nCreate = 14; break; // 라이칸스롭
    case 1:  nCreate = 47; break; // 도마뱀인간
    case 2:  nCreate = 42; break; // 무지나
    default: nCreate = -1; break;
	}
	
	if (::SummonMonster(m_pCaster, nCreate, m_nX, m_nY) == false)  
	{ // 소환에 실패했다면
		return false;
	}
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

bool CExtraMagic::MiddleMonsterRecall()
{ // [58]중급 몬스터 소환
	if (m_pCaster->m_xSummon.IsExist() == true)
	{ // 소환된 몬스터가 있다면
		m_pCaster->m_xSummon.Remove(KillMonster);
	}
	
	int nCreate = 0;
	
	switch (rand()%2)
	{
    case 0:  nCreate = 34; break; // 브루드
    case 1:  nCreate = 38; break; // 스톤골렘
    default: nCreate = -1; break;
	}
	
	if (::SummonMonster(m_pCaster, nCreate, m_nX, m_nY) == false)  
	{ // 소환에 실패했다면
		return false;
	}
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

bool CExtraMagic::SummoningSkeleton()
{ // [88]스켈레톤킹 소환
	if (m_pCaster->m_xSummon.Count() >= GetSummonMax(m_nIndex))
	{ // 소환된 몬스터가 있다면
		return false;
	}
	
	if (::SummonMonster(m_pCaster, 49, m_nX, m_nY) == false)  
	{ // 소환에 실패했다면
		return false;
	}
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT);
	m_packet.u.magic.server_magic_result.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result.nResult = HIT_SUMMONING;
	SendMagicResult();
	return true;
}

bool CExtraMagic::SummoningUndead()
{ // [92]언데드 소환
	if (m_pCaster->m_xSummon.Count() >= GetSummonMax(m_nIndex))
	{ // 소환된 몬스터가 있다면
		return false;
	}
	
	if (!::IsMovable(m_nX, m_nY))  return false;
	if (!::SummonMonster(m_pCaster, 81, m_nX, m_nY))  return false;
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT);
	m_packet.u.magic.server_magic_result.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result.nResult = HIT_SUMMONING;
	SendMagicResult();
	return true;
}

bool CExtraMagic::RecallFollow()
{	// [95]부하 소환
	/*
	if (m_pCaster->m_xSummon.Count() >= GetSummonMax(m_nIndex))
	{ // 소환된 몬스터가 있다면
		return false;
	}
	*/
	//< CSD-030324
	int nX = 0, nY = 0;
	// 소환할 위치 구하기
	if (!GetRecallPlace(m_nX, m_nY, nX, nY))
	{
		return false;
	}
	
	const int nMonster = m_pCaster->GetFollow();

	if (!::SummonMonster(m_pCaster, nMonster, nX, nY))  
	{
		return false;
	}
	//> CSD-030324
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT);
	m_packet.u.magic.server_magic_result.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result.nResult = HIT_SUMMONING;
	SendMagicResult();
	return true;
}

bool CExtraMagic::MagicalTrap()
{	// [20]마법트랩 : 지정된 위치에 화염, 얼음, 복합 마법 효과를 가진 덫 설치
	// 30초(1 레벨) ~ 150초(99 레벨)
	const WORD	wDuration = TRAP_TIME + ((m_pCaster->GetLevel()/5 - 5)*TRAP_TIME/10); // CSD-030806
	const int nX = m_nX;
	const int nY = m_nY;
	// 덫의 종류 정의(0->X/1->+/2->복합)
	switch (rand()%3)
	{
	case 0:	// ㅁ형 트랩	(화염) - 데미지:경
		{
			::InsertTrap(nX,     nY,     0, g_curr_time + wDuration);
			::InsertTrap(nX + 1, nY,     0, g_curr_time + wDuration);
			::InsertTrap(nX,     nY + 1, 0, g_curr_time + wDuration);
			::InsertTrap(nX + 1, nY + 1, 0, g_curr_time + wDuration);
			break;
		}
	case 1:	// +형 트랩	(얼음) - 데미지:중
		{
			::InsertTrap(nX,     nY - 1, 1, g_curr_time + wDuration);
			::InsertTrap(nX,     nY,     1, g_curr_time + wDuration);
			::InsertTrap(nX,     nY + 1, 1, g_curr_time + wDuration);
			::InsertTrap(nX - 1, nY,     1, g_curr_time + wDuration);
			::InsertTrap(nX + 1, nY,     1, g_curr_time + wDuration);
			break;
		}
	case 2:	// ㅁ형 트랩 (복합체) - 데미지:강
		{
			::InsertTrap(nX,     nY,     2, g_curr_time + wDuration);
			::InsertTrap(nX + 1, nY,     2, g_curr_time + wDuration);
			::InsertTrap(nX,     nY + 1, 2, g_curr_time + wDuration);
			::InsertTrap(nX + 1, nY + 1, 2, g_curr_time + wDuration);
			break;
		}
	}
	// 시전자의 경험치 계산
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex);
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.wDuration = wDuration;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}

bool CExtraMagic::MagicDetect()
{ // [75]마법탐지 : 마법덫 탐지
	if (::SearchTrap(m_pCaster, m_nX, m_nY) == false)
	{ // 마법덫를 찾기에 실패했다면
		return false;
	}
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

bool CExtraMagic::Anthem()
{	// [163]성가 : 대상의 NK수치를 감소시키고 대신 경험치를 줄임
	if (::IsHeNK(m_pTarget, MapInfo[MapNumber].nation) <= 0)  
	{ // 현재있는 맵의 NK 수치 검사
		return false;
	}
	//< CSD-030314 : 경험치 감소
	const bool bNK = (IsHeNK(m_pTarget, MapInfo[MapNumber].nation) > 0) ? true: false; 
	m_pTarget->DecExperience(bNK);
	//> CSD-030314 : NK 수치 10 감소
	::InNK(m_pTarget, MapInfo[MapNumber].nation, -10); 
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

bool CExtraMagic::InfluenceDivine()
{	// [168]신에의 감화 : 미결정
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

bool CExtraMagic::VoiceSoul()
{ // [169]영혼의 소리 : 1분에서 1분30초간 유령을 보고 들을 수 있음
	const WORD wPeriod = CalcPeriod(); // 유지시간 계산
	SkillMgr.SetCanViewGhost(m_pCaster, wPeriod);
	::SendSkillResult(m_pCaster->GetServerID(), 1, m_pTarget->GetServerID(), 0, 0, wPeriod);
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex); // 시전자의 경험치 계산
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

bool CExtraMagic::Appreciate()
{	// [167]바른 깨달음 : 상대방의 wis를 알 수 있음
	// 상대의 모든 정보(상태/능력)를 확인
	::SendMagicCommand(m_pCaster, SET_INFORMATION, m_pTarget->GetServerID(), 0);
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

bool CExtraMagic::Pray()
	{	//< CSD-TW-030606 : [178]기도 : 마나를 PRAY_RECOVER_DIVINE_TIME(5)초 더 빨리 채운다. // 030415 kyo
	const WORD wPeriod = CalcMaintain(); // 030521 kyo
	m_pCaster->dwUpDivineMana = m_dwNow + wPeriod;
	::AddCasterEXP(m_pCaster, m_pTarget, 2, 0, m_nIndex); // 시전자의 경험치 계산
	// 결과값을 대상자에게 Packet으로 보내기	
	InitMagicResult(CMD_MAGIC_RESULT_T);
	m_packet.u.magic.server_magic_result_t.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_t.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_t.wDuration = wPeriod;
	m_packet.u.magic.server_magic_result_t.nState = m_pTarget->GetState();
	m_packet.u.magic.server_magic_result_t.nResult = HIT_AND_NOTDEAD;
	SendMagicResult();
	return true;
}	//> CSD-TW-030606

///////////////////////////////////////////////////////////////////////////////
// Protected Method
///////////////////////////////////////////////////////////////////////////////

void CExtraMagic::GetArea(int& rX, int& rY)
{  
	const int nLeft = rX - 3;
	const int nTop = rY - 2;
	const int nRight = rX + 3;
	const int nBottom = rY + 2;
	
	RECT aArea[4] =
	{
		{nLeft - 5, nTop - 7, nLeft, nTop},
		{nRight, nTop - 7, nRight + 5, nTop},
		{nLeft - 5, nBottom, nLeft, nBottom + 7},
		{nRight, nBottom, nRight + 5, nBottom + 7}
	};
	
	RECT rtRand = aArea[rand()%4];
	rX = rtRand.left + rand()%5;
	rY = rtRand.top + rand()%7;
}

bool CExtraMagic::GetRecallPlace(int nSrcX, int nSrcY, int& rDstX, int& rDstY)
{	//< CSD-030324 : 소환할 곳의 위치 구하기
	const int nDir = m_pCaster->Direction;
	int nX = nSrcX + s_aPlace[nDir][rand()%5].x;
	int nY = nSrcY - s_aPlace[nDir][rand()%5].y;

	if (nX <= g_lpMapFile->wWidth && nX >= 0 && nY <= g_lpMapFile->wHeight  && nY >= 0 ) // finito 05/09/07
	{
		if (::IsMovable(nX, nY))
		{
			rDstX = nX;
			rDstY = nY;
			return true;
		}
	}

	// 소환하는 자의 주변을 검색
	for (int i = DIRECTION_DOWN; i <= DIRECTION_RIGHTDOWN; ++i)
	{
		nX = nSrcX + s_aPlace[i][rand()%5].x;
		nY = nSrcY - s_aPlace[i][rand()%5].y;

		if (nX < 0 || nX > g_lpMapFile->wWidth || nY < 0 || nY > g_lpMapFile->wHeight) // finito 05/09/07
		{
			continue;
		}

		if (::IsMovable(nX, nY))
		{
			rDstX = nX;
			rDstY = nY;
			return true;
		}
	}

	return false;
}	//> CSD-030324