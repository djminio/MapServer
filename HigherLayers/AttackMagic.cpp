// AttackMagic.cpp: implementation of the CAttackMagic class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "AttackMagic.h"

#include <math.h>
#include "OP_Magic.h"
#include "Op_Battle.h"
#include "CItem.h"
#include "AIManager.h"
#include "SymbolItemMgr.h"	// 030506 kyo
#include "LogManager.h"

extern int  MAX_DUMMY_LEVEL;
extern bool IsWar();
extern bool CanDestroyTarget(CHARLIST* pCaster,CHARLIST* pTarget);
extern void Send_CMD_SEALSTONE_STATUS(LPCHARLIST ch, int status);
extern void SaveLogHackHits(CHARLIST* pTarget);
extern void SendBBS2( char *text, short int cn );
///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CAttackMagic::CAttackMagic()
{
	
}

CAttackMagic::~CAttackMagic()
{
	
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

bool CAttackMagic::Bind()
{
	Resist(FIRE_ARROW, &CAttackMagic::FireArrow);
	Resist(FIRE_WALL, &CAttackMagic::FireWall);
	Resist(FIRE_BALL, &CAttackMagic::FireBall);
	Resist(SCORCHER, &CAttackMagic::Scorcher);
	Resist(CIRCLE_FLAME, &CAttackMagic::CircleFlame);
	Resist(FLAME_WAVE, &CAttackMagic::FlameWave);
	Resist(APOCALYPSE, &CAttackMagic::Apocalypse);
	Resist(PHEONIX, &CAttackMagic::Pheonix);
	Resist(METEOR_STRIKE, &CAttackMagic::MeteorStrike);
	Resist(GREAT_METEOR, &CAttackMagic::GreatMeteor);
	Resist(FLYING_SPARK, &CAttackMagic::FlyingSpark);
	Resist(FIRE_EXPLOSION, &CAttackMagic::FireExplosion);
	Resist(FIRE_STORM, &CAttackMagic::FireStorm);
	Resist(FIRE_BREATH, &CAttackMagic::FireBreath);
	Resist(FLAME_PILLAR, &CAttackMagic::FlamePillar);
	Resist(FLAME_POUR, &CAttackMagic::FlamePour);
	Resist(ICE_ARROW, &CAttackMagic::IceArrow);
	Resist(ICE_WALL, &CAttackMagic::IceWall);
	Resist(ICE_BALL, &CAttackMagic::IceBall);
	Resist(FREEZE_5TILE_RADIUS, &CAttackMagic::Freeze5TileRadius);
	Resist(CIRCLE_ICE, &CAttackMagic::CircleIce);
	Resist(ICE_STORM, &CAttackMagic::IceStorm);
	Resist(FREEZE_9TILE_RADIUS, &CAttackMagic::Freeze9TileRadius);
	Resist(FREEZING, &CAttackMagic::Freezing);
	Resist(ICE_BREATH, &CAttackMagic::IceBreath);
	Resist(LIGHTNING_BOLT, &CAttackMagic::LightningBolt);
	Resist(CHAIN_LIGHTNING, &CAttackMagic::ChainLightning);
	Resist(CIRCLE_LIGHTNING, &CAttackMagic::CircleLightning);
	Resist(LIGHTNING, &CAttackMagic::Lightning);
	Resist(GREAT_THUNDER_BOLT, &CAttackMagic::GreatThunderBolt);
	Resist(LIGHTNING_5TILE_RADIUS, &CAttackMagic::Lightning5TileRadius);
	Resist(BLITZ_SPEAR, &CAttackMagic::BlitzSpear);
	Resist(LIGHTNING_CLOUD, &CAttackMagic::LightningCloud);
	Resist(LIGHTNING_BREATH, &CAttackMagic::LightningBreath);
	Resist(OCTA_LIGHTNING, &CAttackMagic::OctaLightning);
	Resist(DEATH_FIRE, &CAttackMagic::DeathFire);
	Resist(HOLY_FIRE, &CAttackMagic::HolyFire);
	Resist(HOLY_LIGHT, &CAttackMagic::HolyLight);
	Resist(HOLY_RAGE, &CAttackMagic::HolyRage);
	Resist(TURN_UNDEAD, &CAttackMagic::TurnUndead);
	Resist(DIVINE_POWER, &CAttackMagic::DivinePower);
	Resist(AUTHORITY_CHARISNUMEN, &CAttackMagic::AuthorityCharisNumen); // CSD-021015
	Resist(RAGE_STONE, &CAttackMagic::RageStone);
	Resist(RAGE_BLAST, &CAttackMagic::RageBlast);
	Resist(RAGE_LIGHT_BALL, &CAttackMagic::RageLightBall);
	Resist(SHADOW_OF_EVIL, &CAttackMagic::ShadowOfEvil);
	Resist(BLOOD_SPRINKLE, &CAttackMagic::BooldSprinkle);
	Resist(FLAME_ARROW, &CAttackMagic::FlameArrow); // CSD-031020
	Resist(AREA_BAT_ATTACK, &CAttackMagic::AreaBatAttack);

	Resist(SNOWSTORM, &CAttackMagic::SnowStorm);// 030415 kyo
	Resist(ICE_BREATH2, &CAttackMagic::IceBreath2);// 030415 kyo
	Resist(BLOOD_LIGHTNING, &CAttackMagic::BloodLightning);// 030415 kyo
	Resist(AUTHORITY_OF_WHOARENCHA ,&CAttackMagic::AuthorityOfWhoarencha);// 030415 kyo
	return true;
} 

bool CAttackMagic::FireArrow()
{	// [29]불화살
	if (m_pCaster->equip[WT_WEAPON].item_no == 0)  
	{
		return false;
	}
	
	const int nHD = GetMagicDamage(m_nIndex);
	return AttackMagic(nHD);
}

bool CAttackMagic::FireWall()
{	// [34]화염벽
	const int nHD = GetMagicDamage(m_nIndex);
	return AttackMagic(nHD);
}

bool CAttackMagic::FireBall()
{	// [30]화염구
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::Scorcher()
{	// [31]화염군
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::CircleFlame()
{	// [33]방사화염
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::FlameWave()
{	// [35]움직이는 화염벽
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::Apocalypse()
{	// [36]지옥의 불
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::Pheonix()
{	// [109]불새
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::MeteorStrike()
{	// [110]운석
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::GreatMeteor()
{	// [85]대운석
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::FlyingSpark()
{	// [89]파이어블래스트
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::FireExplosion()
{	// [96]화염폭발
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::FireStorm()
{	// [97]파이어스톰
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::FireBreath()
{	// [102]화염브레쓰
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::FlamePillar()
{	// [106]화염폭풍 
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::FlamePour()
{
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::IceArrow()
{	// [38]얼음화살
	if (m_pCaster->equip[WT_WEAPON].item_no == 0)  
	{
		return false;
	}
	
	const int nHD = GetMagicDamage(m_nIndex);
	return AttackMagic(nHD);
}

bool CAttackMagic::IceWall()
{	// [42]얼음벽
	const int nHD = GetMagicDamage(m_nIndex);
	return AttackMagic(nHD);
}

bool CAttackMagic::IceBall()
{	// [39]얼음구
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::Freeze5TileRadius()
{	// [41]반경 5타일 얼림
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::CircleIce()
{	// [43]원형얼음 공격 
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}	

bool CAttackMagic::IceStorm()
{	// [44]얼음폭풍
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::Freeze9TileRadius()
{	// [45]반경 9타일 얼리기
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::Freezing()
{	// [99]주위얼림
	const int nHD = CalcHitDice();
	SetFrozen(1);
	return AttackMagic(nHD);
}

bool CAttackMagic::IceBreath()
{	// [103]얼음브레쓰
	const int nHD = CalcHitDice();
	SetFrozen(1);
	return AttackMagic(nHD);
}

bool CAttackMagic::LightningBolt()
{	// [48]벼락
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::ChainLightning()
{	// [50]유도 전격
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::CircleLightning()
{	// [51]원형 전격
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::Lightning()
{	// [52]직선 전격
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::GreatThunderBolt()
{	// [84]대전격
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::Lightning5TileRadius()
{	// [54]반경 5타일 전격
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::BlitzSpear()
{	// [100]번개창
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::LightningCloud()
{	// [101]전격구름
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::LightningBreath()
{	// [103]전격브레쓰
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::OctaLightning()
{	// [1078]방사 전격
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::DeathFire()
{	// [87]데쓰파이어
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::HolyFire()
{	// [165]성스러운 불
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::HolyLight()
{	// [162]성스러운 빛
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::HolyRage()
{	// [170]성스러운 분노
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::TurnUndead()
{	// [171]턴 언데드
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::DivinePower()
{	// [174]디바인 파워
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::AuthorityCharisNumen()
{	// [182]카리스 누멘의 권능
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::RageStone()
{	// [189]대지의 분노
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::RageBlast()
{	// [191]화신의 분노
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::RageLightBall()
{	// [192]성령의 분노
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::ShadowOfEvil()
{	// [98]악의 그림자
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::BooldSprinkle()
{	// [108]피뿌리기
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::FlameArrow()
{	// [110] 화염화살
	//< CSD-031020
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}	//> CSD-031020

bool CAttackMagic::AreaBatAttack()
{	// [114]지역 박쥐공격
	const int nHD = CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::SnowStorm() // 030415 kyo
{	// [59]눈보라의 폭풍
	const int nHD =  CalcHitDice();
	
	if (rand()%100 < g_CSymbolMgr.GetSymbolGrade(m_pCaster))
	{	//< CSD-030519
		SetFrozen(5);
	}	//> CSD-030519

	return AttackMagic(nHD);
}

bool CAttackMagic::IceBreath2()// 030415 kyo
{	// [60]아이스브레스2
	const int nHD =  CalcHitDice();

	if (rand()%100 < g_CSymbolMgr.GetSymbolGrade(m_pCaster))
	{
		SetFrozen(5);
	}

	return AttackMagic(nHD);
}

bool CAttackMagic::BloodLightning()// 030415 kyo
{	// [60]블러드라이트닝
	const int nHD =  CalcHitDice();
	return AttackMagic(nHD);
}

bool CAttackMagic::AuthorityOfWhoarencha() // 030415 kyo
{	// [187]화랜타의 권능
	const int nHD =  CalcHitDice();
	return AttackMagic(nHD);
}

///////////////////////////////////////////////////////////////////////////////
// Protected Method
///////////////////////////////////////////////////////////////////////////////

void CAttackMagic::SetFrozen(int nPeriod)
{
	if (m_pTarget->IsNpc())
	{ 
		switch (m_pTarget->Race)
		{
		case DUMMY:
		case HUMAN:
		case GUARD:
		case GUARDSTONE:
		case SEALSTONE:  return;
		}
	}
	
	m_pTarget->speedDown.SetState(m_dwNow + nPeriod, 70);
	m_pTarget->ApplyCurse(6, CON_FROZEN, m_dwNow + nPeriod);
}

bool CAttackMagic::AttackMagic(int nHD)
{
	const int nAttack = CalcAttack(nHD);
	const int nDefense = CalcDefense(nAttack);
	int nDamage = nAttack - nDefense;
	
	switch (m_nIndex)
	{
    case FIRE_ARROW:
    case ICE_ARROW:
		{
			const int nStrike = ((m_pTarget->IsIgnorePhysicalAttack()) ? 0:m_pCaster->GetLongAttack());
			const int nAmplify = m_pCaster->GetMagicAmplify(); 
			const int nAttack = m_pCaster->CalcPhysicalStrikingPower(nStrike + nAmplify);
			const int nDefence = m_pTarget->CalcPhysicalDefensivePower(nAttack);
			// 총 데미지 계산(수정요망 - 1.4배 제거)
			int nPhysical = (nAttack - nDefence)*1.4;
			// 마법에 의한 장거리 무기에 의한 방어력 보정
			if (!m_pCaster->IsIgnorePhysicalDefence())
			{
				m_pTarget->CorrectLongDamage(nPhysical);
			}
			// 마법에 의한 데미지 + 활의 물리적 데미지
			nDamage = (nDamage>>1) + nPhysical;
			break;
		}
	}
	// 공격 마법에 의한 데미지 계산
	const int nType = GetResistType(m_nIndex);
	nDamage = CalcDamage(nType, nDamage);
	// 각종보정
	Correct(nDamage); 
	// 방어구의 내구도 감소
	::DecreaseEquipDuration(m_pTarget, nDamage, ::getEquipRandomPos(), m_pTarget->SprNo, false);
	
	if (m_pTarget->IsNpc())				// LTS AI
	{
		if (m_pTarget->ChairNum)		// 서버 AI이면 	// LTS AI2	
		{
			if (m_pTarget->patterntype>=NPC_PATTERN_WANDER&&m_pTarget->patterntype<NPC_PATTERN_BOSS_WANDER)  // LTS DRAGON MODIFY
			{
				if (rand()%10 > 5)							// LTS AI2
				{
					g_pAIManager->SetAI(m_pTarget,NPC_PATTERN_ATTACKED);
					g_pAIManager->SetTarget(m_pTarget,m_pCaster->GetServerID());
				}
			}
		}
	}
	
	return Result(nDamage);
}

///////////////////////////////////////////////////////////////////////////////
// Private Method
///////////////////////////////////////////////////////////////////////////////

void CAttackMagic::Correct(int& rDamage)
{
	// 아이템 속성에 의한 보정
	if (RareEM.GetDynamicRareValue(FITEM_INVALID_OFFENCE, m_pTarget->DynamicRare))
	{
		rDamage = 0;
		return;
	}
	// 속성 설정
	if (m_pCaster->IsPlayer())
	{
		m_pCaster->AutoAttribute();
	}
	// 높낮이에 따른 보정
	const BYTE nRiseFall = GetRiseFall(m_nIndex);
	const int nX = m_pTarget->MoveSx; 
	const int nY = m_pTarget->MoveSy;
	m_pCaster->CorrectRiseFall(nRiseFall, nX, nY, rDamage);
	// 거리에 따른 보정
	const int nDistance = ::getDistance(m_pCaster, m_pTarget);
	m_pCaster->CorrectMagicRange(nDistance, rDamage);
	// 왕이나 운영자에 대한 보정
	if(m_pCaster->IsCounselor() && m_pCaster->fame >= 10000 && m_pCaster->name_status.santa_power) //EasyKill mode
	{
		rDamage *= 30;
	}

	/*
	if (m_pCaster->name_status.nation == m_pTarget->name_status.nation )
	{	
		if (m_pCaster->name_status.king || m_pTarget->IsCounselor())	
		{
			rDamage /= 30;
		}
	}	
	else 
	{	
		if (m_pCaster->IsCounselor())	 
		{
			rDamage /= 30;
		}
	}
	*/
}

bool CAttackMagic::Result(int nDamage)
{
	BYTE nResult = HIT_FAILED, nState = CON_NORMAL;
	int nMana = 0;
	
	//Finito Added check for reporter so they cant do dmg.
	if (nDamage <= 0 || IsMiss() || m_pCaster->IsReporter())
	{
		nDamage = 0;
		nState = m_pTarget->GetState();
		nResult = HIT_FAILED;
		goto SEND;
	}
	// 석화에 걸려있으면 어떠한 공격 마법의 데미지를 받지 못함
	if (m_pTarget->IsStone())
	{		
		nDamage = 0;
		nState = m_pTarget->GetState();
		nResult = HIT_FAILED;
		goto SEND;
	}
	// 마법 무효화에 대한 보정
	if (!m_pCaster->IsMagicPiercing() && m_pTarget->dwMagicDamageToMana > 0)
	{
		const int	nMinus = GetExhaustMana(m_nIndex);
		
		if (m_pTarget->Mana - nMinus >= 0)
		{	
			m_pTarget->DecMana(nMinus);
			nDamage	= 0;
			nMana	= nMinus;
			nState = m_pTarget->GetState();
			nResult = HIT_AND_NOTDEAD;
			goto SEND;
		}							
		else
		{	
			m_pTarget->dwMagicDamageToMana = 0;
			nMana = 0;
		}
	}
	// NPC 종류에 따른 보정
	if (m_pTarget->IsNpc())
	{
		nDamage /= 1.4;
		
		switch (m_pTarget->Race)
		{
		case DUMMY:
			{
				break;
			}
		case SEALSTONE:
			{
				m_pTarget->DecLife(nDamage);
				g_pLogManager->SaveSealStoneInfo(m_pCaster, m_pTarget, nDamage); // CSD-040316
				break;
			}
		case GUARDSTONE:
			//case CASTLEGATEWAY:
			//case WARARMS:
			{
				if (IsWar() && ::CanDestroyTarget(m_pCaster, m_pTarget))
				{ // 전쟁인 경우 보조 수호석이 다 파괴되었다면          
					m_pTarget->DecLife(nDamage);
				}
				else
				{ // 전쟁이 아닌 경우나 전쟁 중 보조 수호석이 다 파괴되지 않았다면
					nDamage = 0;
					nState = m_pTarget->GetState();
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
					::AddCasterEXP(m_pCaster, m_pTarget, 2, 1, m_nIndex, abs(nDamage));
					nState = m_pTarget->GetState();
				}
				nResult = HIT_AND_NOTDEAD;
				goto SEND;
			}
		case LIFELESS:
			{
				nDamage = 0;
				nState = m_pTarget->GetState();
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
	// 죽었는지 검사
	if (m_pTarget->IsDead())
	{
		::AddCasterEXP(m_pCaster, m_pTarget, 1, 1, m_nIndex, abs(nDamage));
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
		{
			m_pTarget->m_xSummon.Remove(KillMonster);
		}
		// 020813 YGI 
		::killCharacter(m_pCaster, m_pTarget);
		nState = CON_DEATH;
		nResult = HIT_AND_DEAD;
	}
	else
	{		
		if (m_pTarget->Race == SEALSTONE)
		{
			if (++m_pTarget->sealstone%50 == 0)
			{
				::Send_CMD_SEALSTONE_STATUS(m_pTarget, 2);
			}
		}
		
		if (!m_pCaster->IsTamedNpc())
		{
			::AddCasterEXP(m_pCaster, m_pTarget, 2, 1, m_nIndex, abs(nDamage));
			::CheckNK(m_idCaster, m_idTarget, NK_TYPE_NO_KILL_);
		}
		
		nState = m_pTarget->GetState();
		nResult = HIT_AND_NOTDEAD;
	}
	
SEND:  
	// 결과값을 대상자에게 Packet으로 보내기
	InitMagicResult(CMD_MAGIC_RESULT_M);
	m_packet.u.magic.server_magic_result_m.nMagic = m_nIndex;
	m_packet.u.magic.server_magic_result_m.idTarget = m_idTarget;
	m_packet.u.magic.server_magic_result_m.nLife = -nDamage;
	m_packet.u.magic.server_magic_result_m.nMana = -nMana;
	m_packet.u.magic.server_magic_result_m.nState = nState;
	m_packet.u.magic.server_magic_result_m.nResult = nResult;
	SendMagicResult();
	
	if (!m_pCaster->IsNpc() && m_pTarget->IsMagicReflect())
	{
		if (rand()%101 <= 20)
		{
			const int nReal = m_pCaster->DecLife(nDamage*0.3, 10);
			
			if (nReal > 0)
			{
				SendBattleDamage(m_idCaster, 0, -nReal);
			}
		}
	}
	
	return true;
}