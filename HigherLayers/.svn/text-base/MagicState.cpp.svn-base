// MagicState.cpp: implementation of the CMagicState class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "MagicState.h"

#include "..\LowerLayers\MyLog.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
/* Declared inline in the header
CMagicState::CMagicState()
{
  Clear();
}
*/
///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CMagicState::Clear()
{	//< CSD-TW-030623
	ClearTransMap();          // 장소 기억 정보 초기화
	ClearMagic();             // 마법 번호 초기화

	ClearActiveState();
	ClearPassiveState();
	ClearRecoveryState();
	
	m_nAttack = 0;            // 물리적 공격 회수 
	m_nNature = NAT_NORMAL;   // 성질 상태 초기화
	m_nState = CON_NORMAL;    // 마법 상태 초기화
	m_bTamed = false;         // 테이밍 여부 초기화  
	m_bSummon = false;        // 소환 몬스터 여부 초기화
	m_wPoisonedPoint = 0;     // 중독되었을 때 HP가 감소되는 양 초기화
	m_wPoisonedCount = 0;     // 중독되었을 때 HP가 감소되는 초 초기화
	m_nLifeUp = 0;            // Life 증가량
	
	m_dwAttack = 0;           // 물리적 공격 시간 초기화
	m_dwCasting = 0;          // 마법 시전 시작 시간 초기화
	m_dwContinue = 0;         // 마법 시전 유지 시간 초기화
	m_bLock = true;           // 마법 시전 설정 초기화
	
	apShort.Clear();          // 단거리 물리적 공격력
	apMiddle.Clear();         // 중거리 물리적 공격력
	apLong.Clear();           // 장거리 물리적 공격력
	
	dpShort.Clear();          // 단거리 물리적 방어력
	dpMiddle.Clear();         // 중거리 물리적 방어력
	dpLong.Clear();           // 장거리 물리적 방어력
	
	dpFire.Clear();           // 불계열 공격마법  방어력
	dpIce.Clear();            // 얼음계열 공격마법 방어력
	dpElect.Clear();          // 전격계열 공격마법 방어력
	dpHoly.Clear();           // 신력계열 공격마법 방어력
	
	dpCurse1.Clear();         // 저주 1계열 마법 방어력
	dpCurse2.Clear();         // 저주 2계열 마법 방어력
	dpCurse3.Clear();         // 저주 3계열 마법 방어력
	dpCurse4.Clear();         // 저주 4계열 마법 방어력
	dpCurse5.Clear();         // 저주 5계열 마법 방어력
	dpFreeze.Clear();         // CSD-021216 : 얼림 저주 마법 방어력
    
	amplify.Clear();          // 공격 마법 데미지
	speedUp.Clear();          // 이동속도 증가   
	speedDown.Clear();        // 이동속도 감소  
	
	avoid.Clear();            // 회피율
	agility.Clear();          // 민첩성
	
	dwCurse1 = 0;             // 저주 1계열 마법 상태 
	dwCurse2 = 0;             // 저주 2계열 마법 상태 
	dwCurse3 = 0;             // 저주 3계열 마법 상태 
	dwCurse4 = 0;             // 저주 4계열 마법 상태 
	dwFreeze = 0;             // CSD-021024 : 얼림 저주 마법 상태
	
	dwNormalProtect = 0;      // 일반 보호계열 마법 상태
	dwSpecialProtect = 0;     // 특수 보호계열 마법 상태 
	dwExceptProtect = 0;      // 예외 보호계열 마법 상태 (모든 마법이 실패)
	
	dwAttackDelay = 0;	      // 공격 속도 감소(캐릭터가 공격 할수 있는 여부)
	dwDontMove = 0;           // 이동이 되지 않음
	dwDontAttack = 0;		      // 일반적인 공격이 되지 않음
	dwDontCasting = 0;		    // 마법 캐스팅이 되지 않음
	dwDontMiss = 0;           // 모든 물리적 공격과 마법이 성공
	dwBlocking = 0;           // 외부와 차단된 상태
	dwLight = 0;              // 주위에 빛을 밝히는 상태  
	dwMagicDamageToMana = 0;  // 상대의 공격 마법을 마나를 소모하여 막아냄
	dwReturnMagic = 0;		    // 마법의 효과를 시전자에게 되돌림
	dwTransparency = 0;		    // 캐릭터가 투명하게 됨 
	dwLifeUp = 0;             // 일정 시간동안 Life가 올라감
	dwStrikeReflect = 0;      // CSD-021015 : 물리공격 데미지 반사
	dwMagicReflect = 0;       // CSD-021015 : 마법공격 데미지 반사
	dwUpDivineMana = 0;		// 030415 kyo

	for (int i = 0; i < NAT_NORMAL; ++i)
	{
		dwNature[i] = 0;
	}
}	//> CSD-TW-030623

void CMagicState::Recovery(DWORD dwNow)
{	//< CSD-TW-030623 : 유지시간이 지난 마법에 대하여 상태를 회복
	// 마법 상태 검사
	if (apShort.IsContinue(dwNow) == false)  apShort.Clear();
	if (apMiddle.IsContinue(dwNow) == false)  apMiddle.Clear();
	if (apLong.IsContinue(dwNow) == false)  apLong.Clear();
	if (dpShort.IsContinue(dwNow) == false)  dpShort.Clear();
	if (dpMiddle.IsContinue(dwNow) == false)  dpMiddle.Clear();
	if (dpLong.IsContinue(dwNow) == false)  dpLong.Clear();
	
	if (dpFire.IsContinue(dwNow) == false)  dpFire.Clear();
	if (dpIce.IsContinue(dwNow) == false)  dpIce.Clear();
	if (dpElect.IsContinue(dwNow) == false)  dpElect.Clear();
	if (dpHoly.IsContinue(dwNow) == false)  dpHoly.Clear();
	
	if (dpCurse1.IsContinue(dwNow) == false)  dpCurse1.Clear();
	if (dpCurse2.IsContinue(dwNow) == false)  dpCurse2.Clear();
	if (dpCurse3.IsContinue(dwNow) == false)  dpCurse3.Clear();
	if (dpCurse4.IsContinue(dwNow) == false)  dpCurse4.Clear();
	if (dpCurse5.IsContinue(dwNow) == false)  dpCurse5.Clear();
	if (dpFreeze.IsContinue(dwNow) == false)  dpFreeze.Clear();
	
	if (amplify.IsContinue(dwNow) == false)  amplify.Clear();  
	if (speedUp.IsContinue(dwNow) == false)  speedUp.Clear();
	if (speedDown.IsContinue(dwNow) == false)  speedDown.Clear();  
	if (avoid.IsContinue(dwNow) == false)  avoid.Clear();
	if (agility.IsContinue(dwNow) == false)  agility.Clear();
	
	if (dwNormalProtect < dwNow)  dwNormalProtect = 0;
	if (dwSpecialProtect < dwNow)  dwSpecialProtect = 0;
	if (dwExceptProtect < dwNow)  dwExceptProtect = 0;
	
	if (dwAttackDelay < dwNow)  dwAttackDelay = 0;
	if (dwDontMove < dwNow)  dwDontMove = 0;
	if (dwDontAttack < dwNow)  dwDontAttack = 0;
	if (dwDontCasting < dwNow)  dwDontCasting = 0;
	if (dwDontMiss < dwNow)  dwDontMiss = 0;
	if (dwBlocking < dwNow)  dwBlocking = 0;
	if (dwLight < dwNow) dwLight = 0;
	if (dwTransparency < dwNow)  dwTransparency = 0;
	
	if (dwMagicDamageToMana < dwNow)  dwMagicDamageToMana = 0;
	if (dwReturnMagic < dwNow)  dwReturnMagic = 0;
	if (dwLifeUp < dwNow)  dwLifeUp = 0;
	if (dwStrikeReflect < dwNow)  dwStrikeReflect = 0;
	if (dwMagicReflect < dwNow)  dwMagicReflect = 0;
	if (dwUpDivineMana < dwNow) dwUpDivineMana = 0;
	
	for (int i = 0; i < NAT_NORMAL; ++i)
	{
		if (dwNature[i] < dwNow)
		{
			dwNature[i] = 0;
			m_nNature = NAT_NORMAL;
		}
	}
	
	if (dwCombatState < dwNow)  dwCombatState = 0;
	if (dwCombatRecovery < dwNow)  dwCombatRecovery = 0; 
}	//> CSD-TW-030623

bool CMagicState::CheckAttack(BYTE nCount, DWORD dwNow)
{
	if (m_dwAttack < dwNow)
	{
		m_dwAttack = dwNow;
		m_nAttack = 0;
	}
	else
	{
		if (m_nAttack >= nCount)
		{
			return false;
		}
		
		++m_nAttack;
	}
	
	return true;
}

bool CMagicState::CheckStart(BYTE nMagic, DWORD dwNow)
{	//< CSD-TW-030606
	switch (Magic_Ref[nMagic].magic_Type)
	{
	case 0:
	case 9: return false;
	}
	
	if (m_dwCasting > dwNow)
	{
		return false;
	}
	// 화염벽인 경우 유지 시간 설정
	if (nMagic == FIRE_WALL)  
	{
		m_dwFireWall = dwNow + Magic_Ref[nMagic].continue_Time;
	}
	// 얼음벽인 경우 유지 시간 설정
	if (nMagic == ICE_WALL)  
	{
		m_dwIceWall = dwNow + Magic_Ref[nMagic].continue_Time;
	}
	
	m_nMagic = nMagic;                              // 시전할 마법 번호 설정
	m_dwCasting = dwNow + DELAY;                    // 마법 시전 시작 시간
	m_dwContinue = dwNow + GetCastContinue(nMagic); // 캐스팅 유지 시간 설정
	m_bLock = true;                                 // 마법 시전 설정
	return true;
}	//> CSD-TW-030606

bool CMagicState::CheckDoing(BYTE nMagic, DWORD dwNow)
{ //
	switch (Magic_Ref[nMagic].Excute_Type)
	{
    case 0: // 한번 시전에 캐스팅 유지 시간동안 마법 사용
		{
			if (m_dwContinue < dwNow)  
			{ // 유효한 마법 시전 가능 시간인지 검사
				return false;
			}
			
			break;
		}
    case 1: // 한번 시전에 한번 마법 사용
		{
			if (m_bLock == false)  
			{ // 시전을 했는지 검사
				return false;
			}
			
			break;
		}
    default: return false;
	}
	
	m_nMagic = nMagic; // 사용할 마법 번호 설정
	return true;
}

bool CMagicState::CheckEnd(BYTE nMagic, DWORD dwNow)
{ //
	// 화염벽 유지 시간 검사
	if (nMagic == FIRE_WALL && m_dwFireWall < dwNow)  
	{ 
		return false;
	}
	// 얼음벽 유지 시간 검사
	if (nMagic == ICE_WALL && m_dwIceWall < dwNow)  
	{ 
		return false;
	}
	
	m_nMagic = nMagic; // 사용할 마법 번호 설정
	return true;
}

bool CMagicState::CheckDelay(BYTE nMagic, DWORD dwNow)
{ // 유효한 마법 시전 가능 시간인지 검사
	if (m_dwCasting > dwNow)  
	{ // 유효한 마법 시전 가능 시간인지 검사
		return false;
	}
	
	m_nMagic = nMagic;                             // 사용할 마법 번호 설정
	m_dwCasting = dwNow + GetCastContinue(nMagic); // 캐스팅 유지 시간 설정 
	return true;
}

bool CMagicState::CheckCombat(BYTE nCombat, DWORD dwNow)
{
	if (dwCombatState > dwNow)  
	{ // 유효한 마법 시전 가능 시간인지 검사
		return false;
	}
	
	return true;
}

bool CMagicState::CheckMultiEffect(BYTE nMagic)
{
	switch(nMagic)
	{
	//Can have effect more than once per execute
	case TOXIC_CLOUD:
	case FIRE_ARROW:
	case SCORCHER:
	case CIRCLE_FLAME:
	case FIRE_WALL:
	case FLAME_WAVE:
	case APOCALYPSE:
	case ICE_ARROW:
	case FREEZE_5TILE_RADIUS:
	case ICE_WALL:
	case CIRCLE_ICE:
	case ICE_STORM:
	case FREEZE_9TILE_RADIUS:
	case LIGHTNING_BOLT:
	case CHAIN_LIGHTNING:
	case CIRCLE_LIGHTNING:
	case LIGHTNING_5TILE_RADIUS:
	case SNOWSTORM:
	case ICE_BREATH2:
	case BLOOD_LIGHTNING:
	case PHEONIX:
	case METEOR_STRIKE:
	case GREAT_THUNDER_BOLT:
	case GREAT_METEOR:
	case MIRACLE_RECOVER:
	case HOLY_LIGHT:
	case GREAT_PRAY:
	case DIVINE_POWER:
	case HOLY_CURE:
	case AUTHORITY_SIMUNIAN:
	case AUTHORITY_CHARISNUMEN:
	case AUTHORITY_OF_WHOARENCHA:
	case GREAT_SHIELD:
		return true;
		break;
	//Cannot have an effect multiple times
	case PROTECTION:
	case LIFE_UP:
	case SHIELD:
	case HEAL_DISEASE:
	case GREAT_HEAL:
	case SLOW:
	case CONFUSION:
	case INTOXICATION:
	case STONELY:
	case ABSOLUTE_PARALYSIS:
	case VAMPIRE_TOUCH:
	case SPIDER_WEB:
	case TRANSPARENCY:
	case ENCHANT_WEAPON:
	case MINOR_AMPLIFY:
	case MAGIC_AMPLIFY:
	case FIRE_RESIST:
	case ICE_RESIST:
	case LIGHT:
	case ELECT_RESIST:
	case LIGHTNING:
	case ICE_BALL:
	case FIRE_BALL:
	case TWINKLE_STAR:
	case LOW_MONSTER_RECALL:
	case MIDDLE_MONSTER_RECALL:
	case PHASING:
	case MEMORIZING_LOCATION:
	case TOWN_PORTAL:
	case TELEPORT:
	case MAGIC_DETECT:
	case REMOVE_CURSE:
	case PROTECT_RANGE_WEAPON:
	case SPEED_UP:
	case LOST_SIGHT:
	case DRAGON_SCALE:
	case SUMMONING_SKELETON:
	case SUMMONING_UNDEAD:
	case RESTORE_AUTHORITY:
	case PROTECT_DANGER:
	case BLESS:
	case RECOVER_POISON:
	case CURE_DISEASE:
	case PERFECT_PROTECT:
	case CANCEL_DIVINE:
	case RECURRENCE_DIVINE:
	case PRAY_POWER:
	case ANTHEM:
	case HOLY_FIRE:
	case APPRECIATE:
	case VOICE_SOUL:
	case TURN_UNDEAD:
	case CURSE:
	case AUTHORITY_RECOVER_SIGHT:
	case AUTHORITY_LUCK:
	case AUTHORITY_LOST_SIGHT:
	case PRAY:
	case AUTHORITY_LETHY:
	case AUTHORITY_OF_ATHYAS:
	case RAGE_STONE:
	case LIFE_DRAIN:
	case RAGE_BLAST:
	case RAGE_LIGHT_BALL:
		return false;
		break;
	default:
		/*
		PHYSICAL
		DECIPHER
		MAGICAL_TRAP
		NULLITY_MAGIC
		DISEASE_INDUCE
		MULTI_PORT
		DEATH_FIRE
		FLYING_SPARK
		VAMPIRE_POISON
		VAMPIRE_DODGE
		FEATHER_OF_CURSE
		CURSE_OF_HELL
		RECALL_FOLLOW
		FIRE_EXPLOSION
		FIRE_STORM
		SHADOW_OF_EVIL = 98,           // 악의 그림자
		FREEZING = 99,                 // 주위 얼림
		BLITZ_SPEAR = 100,             // 번개창
		LIGHTNING_CLOUD = 101,         // 전격 구름
		FIRE_BREATH = 102,             // 화염 브레쓰
		ICE_BREATH = 103,              // 얼음 브레쓰
		LIGHTNING_BREATH = 104,        // 전격 브레쓰
		FLAME_PILLAR = 105,            // 화염 기둥
		FLAME_POUR = 106,              // 화염 폭풍 
		OCTA_LIGHTNING = 107,          // 방사 전격
		BLOOD_SPRINKLE = 108,          // 피뿌리기
		GAZER	= 109,				   // 게이저
		FLAME_ARROW = 110,             // 화염화살 
		AREA_BAT_ATTACK = 111,         // 지역 박쥐공격
		AREA_PARALYSIS = 112,          // 지역 마비
		AREA_POISON = 113,             // 지역 중독
		AREA_CONFUSION = 114,          // 지역 혼란
		AREA_SLOW = 115,               // 지역 속도저하
		RELIEF_AUTHORITY = 164,        // 구원의 빛 
		INFLUENCE_DIVINE = 168,        // 신에의 감화
		HOLY_RAGE = 170,               // 성스런 분노

  		THROW_ARROW = 194,             // 화살
		THROW_KNIFE = 195,             // 비도

  		// 번개 속성 전투스킬
		LIGHTNING_BOOM = 215,          
		THUNDER_BLOW = 216,
		LIGHTNING_SHOCK = 217,
		THUNDER_STRIKE = 218,
		LIGHTNING_SHIELD = 219,
		LIGHTNING_EXTREME = 220,
		// 대지 속성 전투스킬
		BLOOD_EARTH = 221,             
		GROUND_ATTACK = 222, 
		BLOOD_WILL = 223,
		STONE_ARMOR = 224,
		SIMUNIAN_BOSOM = 225,
		EARTH_EXTREME = 226,
		// 화염 속성 전투스킬 
		SWORD_N_ROSES = 227,
		MULTIPLE_FIRE = 228, 
		FLEET_MISSILE = 229,
		CONVERTING_ARMOR = 230, 
		BURNING_BLOODY= 231,
		FIRE_EXTREME = 232,
		// 어둠 속성 전투스킬
		POISONING_NOVA = 233,
		CHERROY_SHADE = 234, 
		DARK_BLADE = 235, 
		TYBERN_GIFT = 236,
		ACUQUIRE_DARK = 237,
		DARK_EXTREME = 238,
		// 얼음 속성 전투스킬
		ICING_BLAST = 239,
		CRITICAL_HIDING = 240, 
		SHARK_MISSILE = 241, 
		HORN_OF_ICEBERG = 242, 
		ICE_SHIELD = 243,
		ICE_EXTREME = 244,
		// 바람 속성 전투스킬
		CHARGING = 245,
		DOUBLE_ATTACK = 246, 
		WHILWIND = 247,
		TWISTER = 248, 
		GUST = 249, 
		WIND_EXTREME = 250,
		// 이펙트 아이템
		ITEM_FIRE_EFFECT = 610, // CSD-030916
		ITEM_ICE_EFFECT = 611, // CSD-030916
		ITEM_LIGHTNING_EFFECT = 612, // CSD-030916	
		// 이벤트
		THROW_SNOW = 193,
		THROW_WATER_BALL = 661,
		THROW_BIG_GULP = 663,
		// 아이템 효과
		ITEM_EFFECT_FIRE = 605,
		ITEM_EFFECT_ICE = 606,
		ITEM_EFFECT_LIGHTNING = 607

		*/
		return true;
		break;
	};
};

void CMagicState::InitCurse()
{ //< CSD-021024 : 저주계열 마법 상태에 대한 초기화
	dwCurse1 = 0;
	dwCurse2 = 0;
	dwCurse3 = 0;
	dwCurse4 = 0;
	dwFreeze = 0;
} //> CSD-021024

void CMagicState::ClearCurse(BYTE nType)
{
	switch (nType)
	{
    case 1: 
		{ 
			dwCurse1 = 0;
			dwDontCasting = 0;
			dwDontAttack = 0;
			dwBlocking = 0;
			DelState(CON_CURSE|CON_DAZE|CON_STONE);
			break;
		} 
    case 2: 
		{ //< CSD-021024
			dwCurse2 = 0;
			speedDown.Clear();
			DelState(CON_SLOW);
			break;
		} //> CSD-021024
    case 3: 
		{
			dwCurse3 = 0; 
			dwCurse3 = 0;
			m_wPoisonedPoint = 0;
			DelState(CON_POISON);
			break;
		}
    case 4: 
		{
			dwCurse4 = 0;
			DelState(CON_DARKNESS);
			break;
		}
    case 6:
		{ //< CSD-021024
			dwFreeze = 0;
			dwAttackDelay = 0;
			DelState(CON_FROZEN);
			break;
		} //> CSD-021024
	}
}

void CMagicState::ApplyCurse(BYTE nType, BYTE nState, DWORD dwTime)
{ //< CSD-021024 : 저주계열 마법 상태로 적용
	switch (nType)
	{
    case 1: dwCurse1 = dwTime; AddState(nState); break;
    case 2: dwCurse2 = dwTime; AddState(nState); break;
    case 3: dwCurse3 = dwTime; AddState(nState); break;
    case 4: dwCurse4 = dwTime; AddState(nState); break;
    case 6: dwFreeze = dwTime; AddState(nState); break;
    default: return;
	}
} //> CSD-021024

int CMagicState::GetAvailRange(BYTE nMagic) const
{
	return Magic_Ref[nMagic].avail_Range;
}

int CMagicState::GetCastReady(BYTE nMagic) const 
{ 
	return Magic_Ref[nMagic].cast_Time*70; 
}

int CMagicState::GetCastContinue(BYTE nMagic) const
{ 
	return Magic_Ref[nMagic].cast_Continue/6;
}

int CMagicState::GetDualStep(BYTE nMagic) const // 030415 kyo
{
	return Magic_Ref[nMagic].nDualStep;
}

void CMagicState::ClearTransMap()
{ // 기억된 이동 장소 정보 초기화
	m_nTransPort = m_nTransX = m_nTransY = 0;
}

void CMagicState::GetTransMap(int& rPort, int& rX, int& rY) const
{ // 기억된 이동 장소 정보 구하기
	rPort = m_nTransPort;
	rX = m_nTransX;
	rY = m_nTransY;
}

void CMagicState::SetTransMap(int nPort, int nX, int nY)
{ // 기억된 이동 장소 정보 설정
	m_nTransPort = nPort;
	m_nTransX = nX;
	m_nTransY = nY;
}

void CMagicState::ClearCombat(BYTE nSelect)
{	//< CSD-TW-030623
	switch (Magic_Ref[nSelect].Spell_Type)
	{
    case ACTIVE_COMBAT:
		{
			InitActiveCombat();
			ClearActiveState();
			break;
		}
    case PASSIVE_COMBAT:
		{
			InitPassiveCombat();
			ClearPassiveState();
			break;
		}
	case RECOVERY_COMBAT:
		{
			InitRecoveryCombat();
			ClearRecoveryState();
			break;
		}
	}
}	//> CSD-TW-030623

void CMagicState::SetActiveCombat(BYTE nCombat, int nCount)
{ 
	m_nActive = nCombat; 
	m_nCount = nCount;
}

void CMagicState::SetPassiveCombat(BYTE nCombat, DWORD dwPeriod) 
{ 
	m_nPassive = nCombat; 
	dwCombatState = dwPeriod;
}

void CMagicState::SetRecoveryCombat(BYTE nCombat, DWORD dwPeriod) // 030415 kyo
{
	m_nRecovery = nCombat;
	dwCombatRecovery = dwPeriod;
}

void CMagicState::SetPhysicalAttack(DWORD dwTime, BYTE nShort, BYTE nMiddle, BYTE nLong)
{ // 물리적 공격력 증가 설정
	if (nShort > 0)   apShort.SetState(dwTime, nShort);   // 단거리 물리적 공격력
	if (nMiddle > 0)  apMiddle.SetState(dwTime, nMiddle); // 중거리 물리적 공격력
	if (nLong > 0)    apLong.SetState(dwTime, nLong);     // 장거리 물리적 공격력
}

void CMagicState::SetPhysicalDefense(DWORD dwTime, BYTE nShort, BYTE nMiddle, BYTE nLong)
{ // 물리적 방어력 증가 설정   
	if (nShort > 0)   dpShort.SetState(dwTime, nShort);   // 단거리 물리적 방어력
	if (nMiddle > 0)  dpMiddle.SetState(dwTime, nMiddle); // 중거리 물리적 방어력
	if (nLong > 0)    dpLong.SetState(dwTime, nLong);     // 장거리 물리적 방어력
}

void CMagicState::SetMagicalDefense(DWORD dwTime, BYTE nFire, BYTE nIce, BYTE nElect, BYTE nHoly)
{ // 공격계열 마법에 대한 방어력 증가 설정
	if (nFire > 0)   dpFire.SetState(dwTime, nFire);   // 불계열 공격마법 방어력
	if (nIce > 0)    dpIce.SetState(dwTime, nIce);     // 얼음계열 공격마법 방어력
	if (nElect > 0)  dpElect.SetState(dwTime, nElect); // 전격계열 공격마법 방어력
	if (nHoly > 0)   dpHoly.SetState(dwTime, nHoly);   // 홀리계열 공격마법 방어력
}

void CMagicState::SetCurseDefense(DWORD dwTime, BYTE nSet)
{ // 저주계열 마법에 대한 방어 설정
	if (nSet&CURSE_1)  dpCurse1.SetState(dwTime, 100);
	if (nSet&CURSE_2)  dpCurse2.SetState(dwTime, 100);
	if (nSet&CURSE_3)  dpCurse3.SetState(dwTime, 100);
	if (nSet&CURSE_4)  dpCurse4.SetState(dwTime, 100);
	if (nSet&CURSE_5)  dpCurse5.SetState(dwTime, 100);
	if (nSet&CURSE_FREEZE)  dpFreeze.SetState(dwTime, 100); // CSD-021216
}

void CMagicState::ClearNpcRun()
{ //
	m_nNature = NAT_NORMAL;
	dwNature[NAT_SPEED_UP] = 0;
	speedUp.Clear();
}

void CMagicState::ClearNpcStatusUp()
{ //
	m_nNature = NAT_NORMAL;
	dwNature[NAT_STATUS_UP] = 0;
	apShort.Clear();  // 물리적 공격 초기화  
	apMiddle.Clear();
	apLong.Clear();
	dpShort.Clear();  // 물리적 방어력 초기화
	dpMiddle.Clear();
	dpLong.Clear();
	amplify.Clear();  // 마법증폭 초기화
	dpFire.Clear();   // 마법 방어력 초기화
	dpIce.Clear();
	dpElect.Clear();
	dpHoly.Clear();
	speedUp.Clear();  // 속도증가 초기화
}

void CMagicState::ClearNpcRecovery()
{ //
	m_nNature = NAT_NORMAL;
	dwNature[NAT_LIFE_UP] = 0; 
	dwLifeUp = 0;
	m_nLifeUp = 0;
}

void CMagicState::SetNpcRun(DWORD dwTime, int nSpeed)
{ //
	m_nNature = NAT_SPEED_UP;
	dwNature[NAT_SPEED_UP] = dwTime;
	speedUp.SetState(dwTime, nSpeed);
}

void CMagicState::SetNpcStatusUp(DWORD dwTime, int nAttack, int nDefense, int nSpeed)
{ //
	m_nNature = NAT_STATUS_UP;
	dwNature[NAT_STATUS_UP] = dwTime;
	// 물리적 방어 및 공격
	SetPhysicalAttack(dwTime, nAttack, nAttack, nAttack);
	SetPhysicalDefense(dwTime, nDefense, nDefense, nDefense);
	// 마법 방어 및 공격
	amplify.SetState(dwTime, nAttack);
	SetMagicalDefense(dwTime, nDefense, nDefense, nDefense, nDefense);
	// 속도 증가
	speedUp.SetState(dwTime, nSpeed);
}

void CMagicState::SetNpcRecovery(DWORD dwTime, int nLife)
{ //
	m_nNature = NAT_LIFE_UP;
	dwNature[NAT_LIFE_UP] = dwTime; 
	dwLifeUp = dwTime;
	m_nLifeUp = nLife;
}

void CMagicState::CorrectShortDamage(int& rDamage, int nPlus, int nMinus) const
{ // 단거리 무기에 대한 방어력 보정
	int nRatio = dpShort.GetRatio();
	
	if (nPlus > 0)
	{
		nRatio += nPlus;
	}
	
	if (nMinus > 0)
	{
		nRatio -= nMinus;
		
	}
	
	rDamage -= rDamage*nRatio/100;
}

void CMagicState::CorrectMiddleDamage(int& rDamage, int nPlus, int nMinus) const
{ // 중거리 무기에 대한 방어력 보정
	int nRatio = dpMiddle.GetRatio();
	
	if (nPlus > 0)
	{
		nRatio += nPlus;
	}
	
	if (nMinus > 0)
	{
		nRatio -= nMinus;
		
	}
	
	rDamage -= rDamage*nRatio/100;
}

void CMagicState::CorrectLongDamage(int& rDamage, int nPlus, int nMinus) const
{ // 장거리 무기에 대한 방어력 보정
	int nRatio = dpLong.GetRatio();
	
	if (nPlus > 0)
	{
		nRatio += nPlus;
	}
	
	if (nMinus > 0)
	{
		nRatio -= nMinus;
		
	}
	
	rDamage -= rDamage*nRatio/100;
}

void CMagicState::CorrectFireDamage(int& rDamage, int nPlus, int nMinus) const
{ // 불계열 마법에 대한 방어력 보정
	int nRatio = dpFire.GetRatio();
	
	if (nPlus > 0)
	{
		nRatio += nPlus;
	}
	
	if (nMinus > 0)
	{
		nRatio -= nMinus;
		
	}
	
	rDamage -= rDamage*nRatio/100;
}

void CMagicState::CorrectIceDamage(int& rDamage, int nPlus, int nMinus) const
{ // 얼음계열 마법에 대한 방어력 보정
	int nRatio = dpIce.GetRatio();
	
	if (nPlus > 0)
	{
		nRatio += nPlus;
	}
	
	if (nMinus > 0)
	{
		nRatio -= nMinus;
		
	}
	
	rDamage -= rDamage*nRatio/100;
}

void CMagicState::CorrectElectDamage(int& rDamage, int nPlus, int nMinus) const
{ // 전격계열 마법에 대한 방어력 보정
	int nRatio = dpElect.GetRatio();
	
	if (nPlus > 0)
	{
		nRatio += nPlus;
	}
	
	if (nMinus > 0)
	{
		nRatio -= nMinus;
		
	}
	
	rDamage -= rDamage*nRatio/100;
}

void CMagicState::CorrectHolyDamage(int& rDamage, int nPlus, int nMinus) const
{ // 신력계열 마법에 대한 방어력 보정
	int nRatio = dpHoly.GetRatio();
	
	if (nPlus > 0)
	{
		nRatio += nPlus;
	}
	
	if (nMinus > 0)
	{
		nRatio -= nMinus;
		
	}
	
	rDamage -= rDamage*nRatio/100;
}

void CMagicState::CorrectMovePlus(int& rMove, int nPlus) const
{ //< CSD-020710
	const int nCorrent = rMove;
	
	if (speedUp.GetRatio() > nPlus)
	{
		rMove += speedUp.Convert(nCorrent);
	}
	else
	{
		rMove += (nCorrent*nPlus/100);
	}
} //> CSD-020710

void CMagicState::CorrectMoveMinus(int& rMove, int nMinus) const
{ //< CSD-021024
	const int nCorrent = rMove;
	
	if (speedDown.GetRatio() > nMinus)
	{ // 얼림 상태인 경우라면
		rMove -= speedDown.Convert(nCorrent);
	}
	else
	{
		rMove -= (nCorrent*nMinus/100);
	}
} //> CSD-021024

int CMagicState::GetRecoveryCombatDecHPRate(int nLevel) 
{// 030514 kyo // cp회복 전투스킬에 대한 HP떨어지는 %
	if( nLevel <= 0 )
	{
		return 0;
	}
	else if( nLevel == 1 )
	{
		return 5;
	}
	else if( nLevel == 2 )
	{
		return 10;
	}
	else
		return 15;
	return 0;
}