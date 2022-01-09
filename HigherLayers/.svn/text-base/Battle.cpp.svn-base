// Battle.cpp: implementation of the CBattle class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "Battle.h"

#include "CItem.h"
#include "OP_Magic.h"
#include "Op_Battle.h"
#include "MenuServer.h"
#include "Npc_Pattern.h"
#include "AIManager.h"
#include "LogManager.h"

extern int  MAX_DUMMY_LEVEL;
extern bool IsWar();
extern bool CanDestroyTarget(CHARLIST* pCaster,CHARLIST* pTarget);
extern void Send_CMD_SEALSTONE_STATUS(LPCHARLIST ch, int status);

///////////////////////////////////////////////////////////////////////////////
// Global Member
///////////////////////////////////////////////////////////////////////////////

static int RATE_BY_CLASS[5][4] = 
{ // 클래스의 무기별 보정값
	{100, 100,  90,  90}, // 전  사 : 단거리, 중거리, 장거리(활), 장거리(비도)
	{ 90,  90, 100, 100}, // 도  적 : 단거리, 중거리, 장거리(활), 장거리(비도)
	{ 90,  90, 100, 100}, // 궁  수 : 단거리, 중거리, 장거리(활), 장거리(비도)
	{ 90, 100,  90, 100}, // 마법사 : 단거리, 중거리, 장거리(활), 장거리(비도)
	{ 90, 100,  90, 100}  // 성직자 : 단거리, 중거리, 장거리(활), 장거리(비도)
};

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CBattle::CBattle()
{
	m_nIndex = 0;
	m_idCaster = 0; 
	m_idTarget = 0;
	m_pCaster = NULL;
	m_pTarget = NULL;
	m_dwNow = 0; 
	m_nContinue = 0;
	m_nX = 0; 
	m_nY = 0;
}

CBattle::~CBattle() 
{
	
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

int CBattle::Relation(int nCaster, int nTarget) const
{ // 전투스킬에 의한 상성 관계
	const int nSrcHigh = Magic_Ref[nCaster].order_Type/10;
	const int nSrcLow = Magic_Ref[nCaster].order_Type%10;
	const int nDstHigh = Magic_Ref[nTarget].order_Type/10;
	const int nDstLow =  Magic_Ref[nTarget].order_Type%10;
	if (nSrcHigh == 0 || nDstHigh == 0)  return CSR_NORMAL;
	if (nSrcHigh == nDstHigh)  return (nSrcLow == nDstLow) ? CSR_EQUAL:CSR_CONFLICT;
	
	const int nDiff = abs(nSrcHigh < nDstHigh);
	
	if (nDiff < 2)
	{
		return (nSrcHigh < nDstHigh) ? CSR_PRIOR:CSR_POSTERIOR;
	}
	else
	{
		return (nSrcHigh < nDstHigh) ? CSR_POSTERIOR:CSR_PRIOR;
	}
	
	return CSR_NORMAL;
}

///////////////////////////////////////////////////////////////////////////////
// Protected Method
///////////////////////////////////////////////////////////////////////////////

void CBattle::SendBattleDamage(WORD idTarget, BYTE nKind, int nDamage)
{ //< CSD-021019
	memset(&m_packet, 0, sizeof(t_packet));
	m_packet.h.header.type = CMD_BATTLE_DAMAGE;
	m_packet.h.header.size = sizeof(t_battle_damage);
	m_packet.u.battle.battle_damage.idTarget = idTarget;
	m_packet.u.battle.battle_damage.nKind = nKind;
	m_packet.u.battle.battle_damage.nDamage = nDamage;
	SendPacket(idTarget);
} //> CSD-021019

void CBattle::SendPacket(WORD idTarget)
{ // Packet 전송
	if (idTarget < 10000)
	{ // 대상자가 PC라면
		::QueuePacket(connections, idTarget, &m_packet, 1);
		::CastMe2Other(idTarget, &m_packet);
	}
	else
	{ // 대상자가 NPC라면
		::CastNPC2Other(idTarget - 10000, &m_packet);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

int CStrike::CalcAttack(BYTE nType) const
{
	const int nCaster = m_pCaster->GetActiveCombat();
	const int nTarget = m_pTarget->GetActiveCombat();
	const int nResult = Relation(nCaster, nTarget);
	
	int nPercent = 0;
	
	if (!m_pTarget->IsIgnorePhysicalAttack())
	{
		// 전투스킬에 의한 공격력 보정
		switch (nCaster)
		{ //< CSD-021127
		case THUNDER_BLOW:
		case THUNDER_STRIKE:
		case BLOOD_WILL:
		case FLEET_MISSILE:
			{ 
				nPercent += m_pCaster->CorrectCombatAttack(nCaster, nResult);
				break;
			}
		case SWORD_N_ROSES:
		case MULTIPLE_FIRE:
		case ICING_BLAST:
		case DOUBLE_ATTACK:
			{
				nPercent -= (100 - m_pCaster->CorrectCombatAttack(nCaster, nResult));
				break;
			}
		case CHERROY_SHADE:
			{ // 타격 대상자는 질병의 효과
				if (!IsCurseEnable(m_pTarget))
				{
					break;
				}
				
				if (m_pTarget->IsCurseAvoid(Magic_Ref[nCaster].magic_Type))
				{
					break;
				}
				
				if (m_pTarget->IsCurseDefense(Magic_Ref[nCaster].magic_Type))
				{
					break;
				}
				
				const int nTemp = m_pTarget->HungryMax*m_pCaster->GetCombatValue(CHERROY_SHADE)/100;
				m_pTarget->DecHungry(nTemp);
				m_pTarget->SendCharInfoBasic(SP, m_pTarget->Hungry);
				break;
			}
		case DARK_BLADE:
			{ // 타격 대상자는 속도저하의 효과
				if (!IsCurseEnable(m_pTarget))
				{
					break;
				}
				
				if (m_pTarget->IsCurseAvoid(Magic_Ref[nCaster].magic_Type))
				{
					break;
				}
				
				if (m_pTarget->IsCurseDefense(Magic_Ref[nCaster].magic_Type))
				{
					break;
				}
				
				const int nTemp = m_pCaster->CorrectCombatAttack(nCaster, nResult);
				WORD wRemain = GetContinueTime(nCaster);
				m_pTarget->speedDown.SetState(m_dwNow + wRemain, nTemp);
				m_pTarget->ApplyCurse(2, CON_SLOW, m_dwNow + wRemain);
				::SendCharacterCondition(m_pTarget, 0);
				break;
			}
		case HORN_OF_ICEBERG:
			{ // 타격 대상자는 얼림의 효과
				if (!IsCurseEnable(m_pTarget))
				{
					break;
				}
				
				if (m_pTarget->IsCurseAvoid(Magic_Ref[nCaster].magic_Type))
				{
					break;
				}
				
				if (m_pTarget->IsCurseDefense(Magic_Ref[nCaster].magic_Type))
				{
					break;
				}
				
				WORD wRemain = m_pCaster->CorrectCombatAttack(nCaster, nResult);
				m_pTarget->dwAttackDelay = m_dwNow + wRemain;
				m_pTarget->ApplyCurse(6, CON_FROZEN, m_dwNow + wRemain);
				::SendCharacterCondition(m_pTarget, 0);
				break;
			}
		} //> CSD-021127
		// 마법에 의한 공격력 보정
		switch (nType)
		{	//< CSD-030723
		case WR_SHORT:  nPercent += m_pCaster->GetShortAttack();  break;
		case WR_MIDDLE: nPercent += m_pCaster->GetMiddleAttack(); break;
		case WR_LONG1:
		case WR_LONG2:  nPercent += m_pCaster->GetLongAttack();   break;
		}	//> CSD-030723
	}
	
	return m_pCaster->CalcPhysicalStrikingPower(nPercent, 0);
}

int CStrike::CalcDefense(int nAttack) const
{
	int nPercent = 0;
	// 방어력 계산
	return m_pTarget->CalcPhysicalDefensivePower(nAttack, nPercent);
}

int CStrike::CalcDamage(BYTE nType, int nDamage) const
{
	const int nCaster = m_pCaster->GetActiveCombat();
	const int nTarget = m_pTarget->GetPassiveCombat();
	const int nResult = Relation(nCaster, nTarget);
	
	int nPlus = 0, nMinus = 0;
	
	if (!m_pCaster->IsStrikePiercing())
	{	// 전투스킬에 의한 데미지 보정
		switch (nCaster)
		{
		case LIGHTNING_SHOCK:
			{
				if (m_pCaster->CorrectCombatAttack(nCaster, nResult) > rand()%101)
				{
					return nDamage;
				}
				
				break;
			}
		case CONVERTING_ARMOR:
			{
				nMinus += m_pCaster->CorrectCombatAttack(nCaster, nResult);
				break;
			}
		}
		
		switch (nTarget)
		{
		case LIGHTNING_SHIELD:
		case ICE_SHIELD:
			{
				nPlus += m_pTarget->CorrectCombatDefense(nTarget, nResult);
				break;
			}
		}
		
		switch (m_pTarget->GetActiveCombat())
		{
		case STONE_ARMOR:
			{				
				if (m_pTarget->IsCount())
				{
					const int nCount = m_pTarget->DecCombatCount();
					m_pTarget->SendCharInfoBasic(BP, nCount);//020704 lsw
					return 0;
				}
				else
				{	//< CSD-TW-030623 : 전투스킬 초기화
					m_pTarget->InitActiveCombat();
					m_pTarget->ClearActiveState();
					// 관련 Packet 전송  
					t_packet packet;
					packet.h.header.type = CMD_COMBAT_INIT;
					packet.h.header.size = sizeof(t_combat_init);
					packet.u.combat.combat_init.idMaster = m_idTarget;
					packet.u.combat.combat_init.nType = ACTIVE_COMBAT;
					::QueuePacket(connections, m_idTarget, &packet, 1);
				}	//> CSD-TW-030623
				
				break;
			}
		}
		// 마법에 의한 데미지 보정
		switch (nType)
		{	//< CSD-030723
		case WR_SHORT:  m_pTarget->CorrectShortDamage(nDamage, nPlus, nMinus);  break;
		case WR_MIDDLE: m_pTarget->CorrectMiddleDamage(nDamage, nPlus, nMinus); break;
		case WR_LONG1:
		case WR_LONG2:  m_pTarget->CorrectLongDamage(nDamage, nPlus, nMinus);   break;
		}	//> CSD-030723
	}
	
	return nDamage;
}

bool CStrike::IsCurseEnable(CHARLIST* pTarget) const
{	//< CSD-021216
	if (pTarget->IsNpc())
	{
		switch (pTarget->Race)
		{
		case DUMMY:
		case HUMAN:
		case GUARD:
		case ORC_DUMMY:
		case SEALSTONE:
		case GUARDSTONE: return false;
			//case CASTLEGATEWAY: 미결정
			//case WARARMS: 미결정5
		}
	}
	
	return (pTarget->IsPerfect()) ? false:true;
}	//> CSD-021216

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CMagic::InitMagicResult(int nType)
{	//< CSD-TW-030606 : Packet 초기화
	memset(&m_packet, 0, sizeof(t_packet));
	m_packet.h.header.type = nType;
	
	switch (nType)
	{
    case CMD_MAGIC_RESULT: 
		{
			m_packet.h.header.size = sizeof(t_server_magic_result);
			break;
		}
    case CMD_MAGIC_RESULT_T: 
		{
			m_packet.h.header.size = sizeof(t_server_magic_result_t); 
			break;
		}
    case CMD_MAGIC_RESULT_H: 
		{
			m_packet.h.header.size = sizeof(t_server_magic_result_h); 
			break;
		}
    case CMD_MAGIC_RESULT_M:
		{
			m_packet.h.header.size = sizeof(t_server_magic_result_m); 
			break;
		}
	}
	
	m_nType = nType;
}	//> CSD-TW-030606

void CMagic::SendMagicResult()
{	//< CSD-TW-030606 : 대상자에게 Packet 보내기
	const int nMagic = m_pCaster->GetMagic();
	WORD idTarget = 0;
	
	switch (m_nType)
	{
    case CMD_MAGIC_RESULT:
		{
			idTarget = m_packet.u.magic.server_magic_result.idTarget;
			::SaveBallancing(m_pCaster, m_pTarget, nMagic, 0, 0, 0, 0, 0);
			break;
		}
    case CMD_MAGIC_RESULT_T: 
		{
			idTarget = m_packet.u.magic.server_magic_result_t.idTarget;
			::SaveBallancing(m_pCaster, m_pTarget, nMagic, 0, 0, 0, 0, 0);
			break;
		}
    case CMD_MAGIC_RESULT_H: 
		{
			idTarget = m_packet.u.magic.server_magic_result_h.idTarget;
			const int nLife = m_packet.u.magic.server_magic_result_h.nLife;
			::SaveBallancing(m_pCaster, m_pTarget, nMagic, nLife, 0, 0, 0, 0);
			break;
		}
    case CMD_MAGIC_RESULT_M:
		{
			idTarget = m_packet.u.magic.server_magic_result_m.idTarget;
			const int nLife = m_packet.u.magic.server_magic_result_m.nLife;
			const int nMana = m_packet.u.magic.server_magic_result_m.nMana;
			::SaveBallancing(m_pCaster, m_pTarget, nMagic, nLife, nMana, 0, 0, 0);
			break;
		}  
	}
	
	CBattle::SendPacket(idTarget);
}	//> CSD-TW-030606

///////////////////////////////////////////////////////////////////////////////
// Protected Method
///////////////////////////////////////////////////////////////////////////////

int CMagic::CalcPeriod() const
{
	const int nContinue = GetContinueTime(m_nIndex);
	const int nType = GetMagicType(m_nIndex);
	const int nResist = GetResistType(m_nIndex);
	// 고유 유지 시간 계산
	int nPlus = 0, nMinus = 0;
	
	if ( m_nContinue > 0)
	{
		nPlus = m_nContinue;
		nMinus = m_pTarget->CalcResistPeriod(nResist, nPlus);
	}
	else
	{  
		nPlus = m_pCaster->CalcMagicPeriod(nType, nContinue);
		nMinus = m_pTarget->CalcResistPeriod(nResist, nPlus);
	}
	
	return nPlus - nMinus;
}
int CMagic::CalcMaintain() const
{// 030521 kyo
	return GetContinueTime(m_nIndex);
}


int CMagic::CalcHitDice() const
{
	const int nHD = GetMagicDamage(m_nIndex);
	const int nGap = m_pCaster->GetLevel() - m_pTarget->GetLevel(); // CSD-030806
	return nHD + nGap/10*nHD/20;
}

int CMagic::CalcAttack(int nBase) const
{
	const int nCaster = m_pCaster->GetActiveCombat();
	const int nTarget = m_pTarget->GetActiveCombat();
	const int nResult = Relation(nCaster, nTarget);
	
	int nPercent = 0, nAdd = 0;
	
	if (!m_pTarget->IsIgnoreMagicAmplify())
	{ // 마법에 의한 공격력 보정
		nPercent += m_pCaster->GetMagicAmplify();
	}
	
	return m_pCaster->CalcMagicStrikingPower(nBase, nPercent, nAdd);
}

int CMagic::CalcDefense(int nAttack) const
{
	int nResist = RESIST_IGNORE, nPercent = 0;
	
	if (!m_pCaster->IsIgnoreMagicResist() || m_pTarget->IsNpc()) 
	{ //< CSD-021128
		nResist = Magic_Ref[m_nIndex].nResist;
	} //> CSD-021128
	// 방어력 계산
	return m_pTarget->CalcMagicDefensivePower(nResist, nAttack, nPercent);
}

int CMagic::CalcDamage(BYTE nType, int nDamage) const
{ 
	const int nCaster = m_pCaster->GetActiveCombat();
	const int nTarget = m_pTarget->GetPassiveCombat();
	const int nResult = Relation(nCaster, nTarget);
	
	int nPlus = 0, nMinus = 0;
	
	if (!m_pCaster->IsMagicPiercing())
	{ //< CSD-021024
		switch (nTarget)
		{
		case LIGHTNING_SHIELD:
			{
				if (nType == RESIST_ELECT)
				{
					nPlus += m_pTarget->CorrectCombatDefense(nTarget, nResult);
				}
				
				break;
			}
		case ICE_SHIELD:
			{
				if (nType == RESIST_ICE)
				{
					nPlus += m_pTarget->CorrectCombatDefense(nTarget, nResult);
				}
				
				break;
			}
		}
		
		switch (nType)
		{
		case RESIST_FIRE:  m_pTarget->CorrectFireDamage(nDamage, nPlus, nMinus);  break;
		case RESIST_ICE:   m_pTarget->CorrectIceDamage(nDamage, nPlus, nMinus);   break;
		case RESIST_ELECT: m_pTarget->CorrectElectDamage(nDamage, nPlus, nMinus); break;
		case RESIST_HOLY:  m_pTarget->CorrectHolyDamage(nDamage, nPlus, nMinus);  break;
		}
	} //> CSD-021024
	
	return nDamage;
}

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

bool CCombat::AttackShort()
{	//< CSD-030723
	switch (m_pTarget->SprNo)
	{
    case 94:
	case 95: return false;
	}
	
	const int nAttack = CalcAttack(WR_SHORT); 
	const int nDefense = CalcDefense(nAttack);
	// 단거리 무기에 의한 데미지 계산
	int nDamage = CalcDamage(WR_SHORT, nAttack - nDefense);
	Correct(WR_SHORT, nDamage); 
	// 사용 무기의 내구도 감소
	::DecreaseEquipDuration(m_pCaster, nDamage, WT_WEAPON, m_pTarget->SprNo);
	return Result(nDamage);
}	//> CSD-030723

bool CCombat::AttackMiddle()
{	//< CSD-030723
	switch (m_pTarget->SprNo)
	{					
	case 92:
	case 93:
	case 94: return false;
	}
	
	const int nAttack = CalcAttack(WR_MIDDLE); 
	const int nDefense = CalcDefense(nAttack);
	// 중거리 무기에 의한 데미지 계산
	int nDamage = CalcDamage(WR_MIDDLE, nAttack - nDefense);
	Correct(WR_MIDDLE, nDamage); 
	// 사용 무기의 내구도 감소
	::DecreaseEquipDuration(m_pCaster, nDamage, WT_WEAPON, m_pTarget->SprNo);
	return Result(nDamage);
}	//> CSD-030723

bool CCombat::AttackLong(BYTE nType)
{	//< CSD-030723
	switch (m_pTarget->SprNo)
	{
	case 92:
	case 93:
	case 95: return false;
	}
	
	const int nAttack = CalcAttack(nType);
	const int nDefense = CalcDefense(nAttack);
	// 장거리 무기에 의한 데미지 계산
	int nDamage = CalcDamage(m_nIndex, nAttack - nDefense);
	Correct(nType, nDamage); 
	// 사용 무기의 내구도 감소
	switch (nType)
	{
    case WR_LONG1: // 활
		{
			::DecreaseEquipDuration(m_pCaster, nDamage, WT_WEAPON, m_pTarget->SprNo);
			::DecreaseEquipDuration(m_pCaster, 1, WT_SHIELD, m_pTarget->SprNo);
			break;
		}
    case WR_LONG2: // 비도
		{
			::DecreaseEquipDuration(m_pCaster, 1, WT_WEAPON, m_pTarget->SprNo);
			break;
		}
	}
	
	if (m_pTarget->IsNpc() == true)				// LTS AI
	{
		if (m_pTarget->ChairNum)		// 서버 AI이면 	// LTS AI2	
		{
			if (rand()%10 > 5)							// LTS AI2
			{
				g_pAIManager->SetAI(m_pTarget,NPC_PATTERN_ATTACKED);
				g_pAIManager->SetTarget(m_pTarget,m_pCaster->GetServerID());
			}
		}
	}
	
	return Result(nDamage);
}	//> CSD-030723

void CCombat::SendInit(BYTE nType, WORD idMaster)
{	//< CSD-TW-030606
	memset(&m_packet, 0, sizeof(t_packet));
	m_packet.h.header.size = sizeof(t_combat_init);
	m_packet.h.header.type = CMD_COMBAT_INIT;
	m_packet.u.combat.combat_init.idMaster = idMaster;
	m_packet.u.combat.combat_init.nType = nType;
	CBattle::SendPacket(idMaster);
}	//> CSD-TW-030606

void CCombat::SendResult(int nCmd, t_server_combat_result* pPacket)
{
	memset(&m_packet, 0, sizeof(t_packet));
	m_packet.h.header.type = nCmd;
	
	switch (nCmd)
	{
    case CMD_COMBAT_RESULT:
		{
			m_packet.h.header.size = sizeof(t_server_combat_result);
			memcpy(&m_packet.u.combat.server_combat_result, 
				(t_server_combat_result*)pPacket, 
				sizeof(t_server_combat_result)); 
			break;
		}
    case CMD_COMBAT_RESULT_H:
		{
			m_packet.h.header.size = sizeof(t_server_combat_result_h);
			memcpy(&m_packet.u.combat.server_combat_result_h, 
				(t_server_combat_result_h*)pPacket, 
				sizeof(t_server_combat_result_h)); 
			break;
		}
    case CMD_COMBAT_RESULT_M:
		{
			m_packet.h.header.size = sizeof(t_server_combat_result_m);
			memcpy(&m_packet.u.combat.server_combat_result_m, 
				(t_server_combat_result_m*)pPacket, 
				sizeof(t_server_combat_result_m)); 
			break;
		}
    case CMD_COMBAT_RESULT_S:
		{
			m_packet.h.header.size = sizeof(t_server_combat_result_s);
			memcpy(&m_packet.u.combat.server_combat_result_s, 
				(t_server_combat_result_s*)pPacket, 
				sizeof(t_server_combat_result_s)); 
			break;
		}
    case CMD_COMBAT_RESULT_T:
		{
			m_packet.h.header.size = sizeof(t_server_combat_result_t);
			memcpy(&m_packet.u.combat.server_combat_result_t, 
				(t_server_combat_result_t*)pPacket, 
				sizeof(t_server_combat_result_t)); 
			break;
		}
	}
	
	CBattle::SendPacket(pPacket->idTarget);
}

///////////////////////////////////////////////////////////////////////////////
// Protected Method
///////////////////////////////////////////////////////////////////////////////

int CCombat::GetKind(CHARLIST* pSomeone) const
{	//< CSD-030723
	switch (pSomeone->GetTacticsKind())
	{	
    case TACTICS_Whirl:   
    case TACTICS_Pierce:
    case TACTICS_Magery:
    case TACTICS_Orison:  return WR_MIDDLE;
	case TACTICS_Archery: return WR_LONG1;
	case TACTICS_Hurl:    return WR_LONG2;
	}
	
	return WR_SHORT;
}	//> CSD-030723

int CCombat::CalcMaintain() const
{
	return GetContinueTime(m_nIndex);
}

int CCombat::CalcAttack(BYTE nType) const
{
	const int nCaster = m_pCaster->GetActiveCombat();
	const int nTarget = m_pTarget->GetActiveCombat();
	const int nResult = Relation(nCaster, nTarget);
	
	int nPercent = 0, nAdd = 0;
	
	if (!m_pTarget->IsIgnorePhysicalAttack())
	{	// 마법에 의한 공격력 보정
		switch (nType)
		{	//< CSD-030723	
		case WR_SHORT:  nPercent += m_pCaster->GetShortAttack();  break;
		case WR_MIDDLE: nPercent += m_pCaster->GetMiddleAttack(); break;
		case WR_LONG1:
		case WR_LONG2:  nPercent += m_pCaster->GetLongAttack();   break;
		}	//> CSD-030723
	}
	
	int nAttack = m_pCaster->CalcPhysicalStrikingPower(nPercent, nAdd);
	
	if (!m_pTarget->IsIgnorePhysicalAttack())
	{ // 전투스킬에 의한 공격력 보정
		switch (nCaster)
		{
		case LIGHTNING_BOOM:
		case BLOOD_EARTH:
		case GROUND_ATTACK:
		case WHILWIND:
		case TWISTER:
		case LIGHTNING_EXTREME:
		case EARTH_EXTREME:
		case WIND_EXTREME:
			{
				nAttack = nAttack*m_pCaster->CorrectCombatAttack(nCaster, nResult)/100;
				break;
			}
		}
	}
	
	return nAttack;
}

int CCombat::CalcDefense(int nAttack) const
{
	int nPercent = 0;
	// 방어력 계산
	return m_pTarget->CalcPhysicalDefensivePower(nAttack, nPercent);
}

int CCombat::CalcDamage(BYTE nType, int nDamage) const
{ 
	const int nCaster = m_pCaster->GetActiveCombat();
	const int nTarget = m_pTarget->GetPassiveCombat();
	const int nResult = Relation(nCaster, nTarget);
	
	int nPercent = 0;
	
	if (!m_pCaster->IsStrikePiercing())
	{
		switch (nTarget)
		{
		case STONE_ARMOR:
			{
				if (m_pTarget->IsCount())
				{
					const int nCount = m_pTarget->DecCombatCount();
					m_pTarget->SendCharInfoBasic(BP, nCount);//020704 lsw
					return 0;
				}
				else
				{	//< CSD-TW-030623 : 전투스킬 초기화
					m_pTarget->InitActiveCombat();
					m_pTarget->ClearActiveState();
					// 관련 Packet 전송  
					t_packet packet;
					packet.h.header.type = CMD_COMBAT_INIT;
					packet.h.header.size = sizeof(t_combat_init);
					packet.u.combat.combat_init.idMaster = m_idTarget;
					packet.u.combat.combat_init.nType = 0;
					::QueuePacket(connections, m_idTarget, &packet, 1);
				}	//>	 CSD-TW-030623
				
				break;
			}
		}
		// 마법에 의한 데미지 보정
		switch (nType)
		{	//< CSD-030723
		case WR_SHORT:  m_pTarget->CorrectShortDamage(nDamage, nPercent);  break;
		case WR_MIDDLE: m_pTarget->CorrectMiddleDamage(nDamage, nPercent); break;
		case WR_LONG1:
		case WR_LONG2:  m_pTarget->CorrectLongDamage(nDamage, nPercent);   break;
		}	//> CSD-030723
	}
	
	return nDamage;
}

void CCombat::Correct(BYTE nType, int& rDamage)
{ 
	// 아이템 속성에 의한 보정
	RareEM.CorrectByStaticRare(m_pCaster, m_pTarget,rDamage); //020303 lsw
	// 클래스에 대한 보정
	if (m_pCaster->IsPlayer())
	{ //< CSD-021024
		rDamage = rDamage*RATE_BY_CLASS[m_pCaster->Class][nType]/100;
	} //> CSD-021024
	// 거리에 대한 보정
	const int nDistance = ::getDistance(m_pCaster, m_pTarget);
	m_pCaster->CorrectStrikeRange(nDistance, rDamage);
	// 높낮이에 따른 보정
	const int nX = m_pTarget->MoveSx; 
	const int nY = m_pTarget->MoveSy;
	m_pCaster->CorrectRiseFall(0, nX, nY, rDamage);
	// 왕이나 카운셀러에 대한 보정
	if(m_pCaster->IsCounselor() && m_pCaster->fame >= 10000 && m_pCaster->name_status.santa_power) //EasyKill mode
	{
		rDamage *= 30;
	}
	/*
	if (m_pCaster->name_status.nation == m_pTarget->name_status.nation)
	{	
		if (m_pTarget->name_status.king || m_pTarget->IsCounselor())  
		{
			rDamage /= 30;
		}
	}
	else 
	{
		if (m_pTarget->IsCounselor())
		{
			rDamage /= 30;
		}
	}
	*/
}

bool CCombat::Result(int nDamage, bool bAni)
{
	BYTE nResult = HIT_FAILED;
	
	//Finito Added check for reporter so they cant do dmg.
	if (nDamage <= 0 || m_pCaster->IsReporter())
	{ 
		nDamage = 0;
		nResult = HIT_FAILED;
		goto SEND;
	}
	// 석화에 걸려있으면 어떠한 물리적 데이지도 받지 못함
	if (m_pTarget->IsStone())
	{		
		nDamage = 0;
		nResult = HIT_FAILED;
		goto SEND;
	}
	// 만약 공격을 받으면 자리에 멈추게 함
	if (m_pTarget->IsNpc())
	{ 
		m_pTarget->MoveLength = m_pTarget->MovePathCount = 0;
	}
	// 만약 ROOKIE_LEVEL 이하이면 돈이 나오게 함
	if (m_pCaster->IsPlayer() && m_pTarget->IsNpc())
	{
		if (m_pCaster->GetLevel() <= ROOKIE_LEVEL && m_pTarget->Race == DUMMY)
		{	//< CSD-030806
			if (!(rand()%10)) 
			{
				ItemAttr item = {0,};
				if(LocalMgr.IsChangeMoney())//030102 lsw
				{
					item = ::GenerateItem(IN_NEW_COINS, IATTR_MUCH, rand()%6 +10);
				}
				else
				{
					item = ::GenerateItem(IN_COINS, IATTR_MUCH , rand()%6 + 10);
				}
				::AddItemList(item.item_no, item.attr, 0, 
					m_pCaster->X + rand()%TILE_SIZE, m_pCaster->Y + rand()%TILE_SIZE, 0, 0);
			}
		}	//> CSD-030806
	}
	// 방어구의 내구도 감소
	::DecreaseEquipDuration(m_pTarget, nDamage, ::getEquipRandomPos(), m_pTarget->SprNo,false);//020401 lsw
	::HungryMuchAdd(m_pCaster, HT_ATTACK);
	
	if (m_pTarget->IsNpc())
	{
		switch (m_pTarget->Race)
		{
		case DUMMY:
			{	//< CSD-030806
				if (m_pCaster->GetLevel() <= ENABLE_DUMMY_LEVEL)
				{	// 경험치가 10 이하면서 더미를 공격한 경우만 경험치 상승  
					const int nExp = ::addEXP(m_pCaster, m_pTarget, HIT_AND_NOTDEAD, nDamage);
					const int nTemp = (rand()%10 < 3) ? 3:4; // 일반 경험치에서 택틱 경험치로 변환상수
					m_pCaster->ChangeUpTacExp(0, nExp*nTemp);
				}	
				
				nResult = HIT_AND_NOTDEAD;
				goto SEND;
			}	//> CSD-030806
		case SEALSTONE:
			{
				m_pTarget->DecLife(nDamage);
				g_pLogManager->SaveSealStoneInfo(m_pCaster, m_pTarget, nDamage); // CSD-040316
				break;
			}
		case GUARDSTONE:
			{
				if (::IsWar() && ::CanDestroyTarget(m_pCaster, m_pTarget))
				{ // 전쟁인 경우 보조 수호석이 다 파괴되었다면
					m_pTarget->DecLife(nDamage);
				}
				else
				{ // 전쟁이 아닌 경우나 전쟁 중 보조 수호석이 다 파괴되지 않았다면
					nDamage = 0;
					nResult = HIT_AND_NOTDEAD;
					goto SEND;
				}
				
				break;
			}
		case ORC_DUMMY:
			{
				//Finito Added max level for Tactic Dummy.
				if (m_pCaster->GetLevel() <= MAX_DUMMY_LEVEL)
				{
					m_pCaster->ChangeUpTacExp(1, nDamage*3);
				}
				nResult = HIT_AND_NOTDEAD;
				goto SEND;
			}
		case LIFELESS:
			{
				nDamage = 0;
				nResult = HIT_AND_NOTDEAD;
				goto SEND;
			}
		default:
			{
				m_pTarget->DecLife(nDamage);
				break;
			}
		}
	}
	else
	{
		m_pTarget->DecLife(nDamage);
	}
	
	if (m_pTarget->IsDead())
	{					
		const int nExp = ::addEXP(m_pCaster, m_pTarget, HIT_AND_DEAD, nDamage);
		m_pCaster->ChangeUpTacExp(0, nExp*3);
		
		::KillWho(m_pTarget, m_pCaster);
		// NK에 계산
		if(LocalMgr.IsAbleNation(TAIWAN))//030102 lsw Local
		{
			if (m_pCaster->IsTamedNpc())
			{
				::CheckNK(m_pCaster->GetMaster(), m_idTarget, NK_TYPE_KILL_);
			}
			else
			{
				::CheckNK(m_idCaster, m_idTarget, NK_TYPE_KILL_);		
			}
		}
		else
		{
			::CheckNK(m_idCaster, m_idTarget, NK_TYPE_KILL_);
		}
		
		if (m_pTarget->IsNpc())
		{	//< CSD-030306
			m_pTarget->m_xSummon.Remove(KillMonster);
		}	//> CSD-030306
		// 020813 YGI 
		::killCharacter(m_pCaster, m_pTarget);
		nResult = HIT_AND_DEAD;				
		
		goto SEND;
	}				
	else // 죽지 않은 경우
	{		
		const int nExp = ::addEXP(m_pCaster, m_pTarget, HIT_AND_NOTDEAD, nDamage);
		m_pCaster->ChangeUpTacExp(0, nExp*3);
		
		if (m_pTarget->Race == SEALSTONE)
		{
			if (++m_pTarget->sealstone%50 == 0)
			{
				::Send_CMD_SEALSTONE_STATUS(m_pTarget, 2);
			}
		}
		
		if (!m_pCaster->IsTamedNpc())
		{
			m_pTarget->ChangeUpTacExp(4, m_pCaster->GetLevel()*2); // CSD-030806

			if( m_pTarget != m_pCaster ) // 030430 kyo //자기자신에 관한 일일때는 pk를 안올린다.
			{
				::CheckNK(m_idCaster, m_idTarget, NK_TYPE_NO_KILL_);
			}
		}
		
		nResult = HIT_AND_NOTDEAD;
		goto SEND;
	}
	
SEND:
	if (m_pCaster->IsPlayer())  
	{
		::SaveBallancing(m_pCaster, m_pTarget, 0, nDamage, 0, 0, 0, 0);
	}
	
	if (m_pCaster->IsPlayer())
	{
		memset(&m_packet, 0, sizeof(t_packet));
		m_packet.h.header.type = CMD_COMBAT_RESULT_H;
		m_packet.h.header.size = sizeof(t_server_combat_result_h);
		m_packet.u.combat.server_combat_result_h.nSkill = m_nIndex;
		m_packet.u.combat.server_combat_result_h.idTarget = m_idTarget;
		m_packet.u.combat.server_combat_result_h.nResult= nResult;
		m_packet.u.combat.server_combat_result_h.nState = m_pTarget->GetState();
		m_packet.u.combat.server_combat_result_h.nLife = -nDamage;
		m_packet.u.combat.server_combat_result_h.bChangeAni = bAni; // 030415 kyo
		::QueuePacket(connections, m_idCaster, &m_packet, 1);
		::CastMe2Other(m_idCaster, &m_packet);
		return true;
	}
	else
	{
		memset(&m_packet, 0, sizeof(t_packet));
		m_packet.h.header.type = CMD_COMBAT_RESULT_H;
		m_packet.h.header.size = sizeof(t_server_combat_result_h);
		m_packet.u.combat.server_combat_result_h.nSkill = m_nIndex;
		m_packet.u.combat.server_combat_result_h.idTarget = m_idTarget;
		m_packet.u.combat.server_combat_result_h.nResult= nResult;
		m_packet.u.combat.server_combat_result_h.nState = m_pTarget->GetState();
		m_packet.u.combat.server_combat_result_h.nLife = -nDamage;
		m_packet.u.combat.server_combat_result_h.bChangeAni = bAni; // 030415 kyo
		::CastNPC2Other(m_idCaster - 10000, &m_packet);
		return true;
	}
	
	return false;
}