// PhysicalAttack.cpp: implementation of the CPhysicalAttack class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "PhysicalAttack.h"

#include "Op_Battle.h"
#include "Op_Magic.h"
#include "CItem.h"
#include "NationSys.h"
#include "SealStoneGameServer.h"
#include "Npc_Pattern.h"
#include "AIManager.h"
#include "LogManager.h"

extern int  MAX_DUMMY_LEVEL;
extern bool IsWar();
extern bool CanDestroyTarget(CHARLIST* pCaster,CHARLIST* pTarget);
extern void Send_CMD_SEALSTONE_STATUS(LPCHARLIST ch, int status);
extern void SendBBS2( char *text, short int cn );
extern void SaveLogHackHits(CHARLIST* pTarget);

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

CPhysicalAttack::CPhysicalAttack()
{
	
}

CPhysicalAttack::~CPhysicalAttack()
{
	
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

bool CPhysicalAttack::Bind() 
{	//< CSD-030723
	Resist(WR_SHORT,  &CPhysicalAttack::AttackShort);  // 단거리
	Resist(WR_MIDDLE, &CPhysicalAttack::AttackMiddle); // 중거리
	Resist(WR_LONG1,  &CPhysicalAttack::AttackLong);   // 장거리(활)
	Resist(WR_LONG2,  &CPhysicalAttack::AttackLong);   // 장거리(비도)
	Resist(WR_LONG3,  &CPhysicalAttack::AttackEvent);  // 장거리(이벤트)
	Resist(WR_EFFECT, &CPhysicalAttack::AttackEffect); // 아이템 효과
	return true; 
}	//> CSD-030723

bool CPhysicalAttack::AttackShort()
{
	switch (m_pTarget->SprNo)
	{
    case 94 :	case 95 :	return false;
	}
	
	const int nAttack = CalcAttack(m_nIndex);
	const int nDefense = CalcDefense(nAttack);
	// 단거리 무기에 의한 데미지 계산
	int nDamage = CalcDamage(m_nIndex, nAttack - nDefense);
	Correct(nDamage); 
	// 거리 검사
	switch (m_pCaster->GetActiveCombat())
	{
    case CHARGING:
		{
			break;
		}
    default:
		{
			LogIsRange(); // finito 27/08/07 temp
			if (!IsRange())  nDamage = 0;
			break;
		}
	}
	
	// 사용 무기의 내구도 감소
	::DecreaseEquipDuration(m_pCaster, nDamage, WT_WEAPON, m_pTarget->SprNo);
	return Result(nDamage);
}

bool CPhysicalAttack::AttackMiddle()
{
	switch (m_pTarget->SprNo)
	{					
	case 92:	case 93:	case 94:	return false;
	}
	
	const int nAttack = CalcAttack(m_nIndex);
	const int nDefense = CalcDefense(nAttack);
	// 중거리 무기에 의한 데미지 계산
	int nDamage = CalcDamage(m_nIndex, nAttack - nDefense);
	Correct(nDamage); 
	// 거리 검사
	LogIsRange(); // finito 27/08/07 temp
	if (!IsRange())  nDamage = 0;
	
	// 사용 무기의 내구도 감소
	::DecreaseEquipDuration(m_pCaster, nDamage, WT_WEAPON, m_pTarget->SprNo);
	return Result(nDamage);
}

bool CPhysicalAttack::AttackLong()
{
	switch (m_pTarget->SprNo)
	{
	case 92 :	case 93 :	case 95 :	return false;
	}
	
	const int nAttack = CalcAttack(m_nIndex);
	const int nDefense = CalcDefense(nAttack);
	// 장거리 무기에 의한 데미지 계산
	int nDamage = CalcDamage(m_nIndex, nAttack - nDefense);
	Correct(nDamage); 
//	if (!IsRange())  nDamage = 0; // finito 21/08/07 range checking for throwing weapons
	LogIsRange(); // finito 27/08/07 temp
	// 사용 무기의 내구도 감소
	switch (m_nIndex)
	{	//< CSD-030723
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
	}	//> CSD-030723
	
	if (m_pTarget->IsNpc()) // LTS AI
	{
		if (m_pTarget->ChairNum)		// 서버 AI이면 	// LTS AI2	
		{
			if (m_pTarget->patterntype>=NPC_PATTERN_WANDER&&m_pTarget->patterntype<NPC_PATTERN_BOSS_WANDER)  // LTS DRAGON MODIFY
			{
				if (rand()%10>5)							// LTS AI2
				{	//< CSD-030417
					g_pAIManager->SetAI(m_pTarget,NPC_PATTERN_ATTACKED);
					g_pAIManager->SetTarget(m_pTarget,m_pCaster->GetServerID());
				}	//> CSD-030417
			}
		}
	}
	
	return Result(nDamage);
}

bool CPhysicalAttack::AttackEvent()
{	//< CSD-030723	
	switch (m_pTarget->SprNo)
	{
	case 92 :	case 93 :	case 95 :	return false;
	}

	switch (m_nIndex)
	{	
    case WR_LONG3: // 눈싸움
		{
			::DecreaseEquipDuration(m_pCaster, 1, WT_WEAPON, m_pTarget->SprNo);
			break;
		}
	}
	
	BYTE nResult = HIT_AND_NOTDEAD;
	int nDamage = 1;

	
	//Finito Added check for reporter so they cant do dmg.
	if (m_pCaster->IsReporter())
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
	// 방어구의 내구도 감소
	::DecreaseEquipDuration(m_pTarget, nDamage, ::getEquipRandomPos(), m_pTarget->SprNo, false);
	::HungryMuchAdd(m_pCaster, HT_ATTACK);
	
	switch (m_pTarget->Race)
	{
    case SEALSTONE:  // 결계석인 경우
		{
			m_pTarget->DecLife(nDamage);
			g_pLogManager->SaveSealStoneInfo(m_pCaster, m_pTarget, nDamage); // CSD-040316
			break;
		}
    case GUARDSTONE: // 수호석인 경우
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
		//case CASTLEGATEWAY: 미결정
		//case WARARMS: 미결정
	}
	
	switch (m_pTarget->SprNo)
	{
	case 92:	// 기둥..
	case 93:	// 기둥..
	case 94:	// 과녁..
	case 95:	// 허수아비.
    case 96:	// 태틱수련용.
		{ 
			nResult = HIT_AND_NOTDEAD;
			goto SEND;
		}	
	case 97:	// 축구공
		{
			nDamage = 0;
			nResult = HIT_AND_NOTDEAD;
			goto SEND;
		}
	default :	
		{
			m_pTarget->DecLife(nDamage);
			break;
		}
	} 
	
	if (m_pTarget->IsDead() == true)
	{					
		m_pTarget->Hp = 1;
	}
    
	if (m_pTarget->Race == SEALSTONE)
	{
		if (++m_pTarget->sealstone%50 == 0)
		{
			::Send_CMD_SEALSTONE_STATUS(m_pTarget, 2);
		}
	}
	
	if (m_pCaster->IsPlayer() == true)
	{
		::SaveBallancing(m_pCaster, m_pTarget, 0, nDamage, 0, 0, 0, 0);
	}
	
SEND:
	const int nTactics = m_pCaster->cur_tac_skill;
	
	switch (m_nIndex)
	{
    case WR_LONG3:
		{
			if (m_pCaster->IsPlayer())
			{
				memset(&m_packet, 0, sizeof(t_packet));
				m_packet.h.header.type = CMD_THROW_RESULT;
				m_packet.h.header.size = sizeof(t_server_throw_result); 
				m_packet.u.strike.server_throw_result.idTarget = m_idTarget;
				m_packet.u.strike.server_throw_result.nX = m_pTarget->Tox;
				m_packet.u.strike.server_throw_result.nY = m_pTarget->Toy;
				m_packet.u.strike.server_throw_result.nDamage = -nDamage;
				m_packet.u.strike.server_throw_result.nResult = nResult;
				::QueuePacket(connections, m_idCaster, &m_packet, 1);
				
				memset(&m_packet, 0, sizeof(t_packet));
				m_packet.h.header.type = CMD_THROW_RESULT_D;
				m_packet.h.header.size = sizeof(t_throw_result_d);
				m_packet.u.strike.throw_result_d.idCaster = m_idCaster;
				m_packet.u.strike.throw_result_d.idTarget = m_idTarget;
				m_packet.u.strike.throw_result_d.nDamage = -nDamage;
				m_packet.u.strike.throw_result_d.nX = m_pTarget->Tox;
				m_packet.u.strike.throw_result_d.nY = m_pTarget->Toy;
				m_packet.u.strike.throw_result_d.nResult = nResult;
				::CastMe2Other(m_idCaster, &m_packet);
				return true;
			}
			
			break;
		}
	}
	
	return false;
}	//> CSD-030723

bool CPhysicalAttack::AttackEffect()
{	//< CSD-031007
	const int nAttack = CalcAttack(m_nIndex);
	const int nDefense = CalcDefense(nAttack);
	// 아이템 효과에 의한 데미지 계산
	int nDamage = CalcDamage(m_nIndex, nAttack - nDefense)*m_pCaster->CorrectItemEffect()/100;
	Correct(nDamage);

	LogIsRange(); // finito 27/08/07 temp
	
	if (!IsRange())
	{	// 거리 검사
		nDamage = 0;
	}

	return Result(nDamage);
}	//> CSD-031007

///////////////////////////////////////////////////////////////////////////////
// Protected Method
///////////////////////////////////////////////////////////////////////////////

bool CPhysicalAttack::IsRange() const
{	// 적절한 범위인지 검사
	if (m_pCaster->IsPlayer() && m_pTarget->IsNpc())
	{
		const int nDistX = pow(m_pCaster->X - m_pTarget->X, 2.0);
		const int nDistY = pow(m_pCaster->Y - m_pTarget->Y, 2.0);
		
		if (nDistX + nDistY > 50000)
		{
			return false;
		}
	}

	return true;
}

void CPhysicalAttack::LogIsRange() // finito 27/08/07 temp function to log
{
	if (m_pCaster->IsPlayer()) // finito 21/08/07 allow range checking for if target is player
	{
		const int nDistX = pow(((m_pCaster->MoveSx*32)+16) - ((m_pTarget->MoveSx*32)+16), 2.0);
		const int nDistY = pow(((m_pCaster->MoveSy*32)+16) - ((m_pTarget->MoveSy*32)+16), 2.0);
		
		int maxrange = 12769;

		// finito 21/08/07 set maxrange for different weapons

		CItem *tt = ItemUnit(m_pCaster->equip[0]);
		if(tt)
		{
			if (m_pCaster->Gender == 1) // Male
			{
				switch (tt->GetItemShow(m_pCaster->Gender))
				{
				case 78: // 80 ~ 320 Low level knive
				case 120:
				case 127:
				case 128:
				case 167:
				case 177:
					maxrange = 181476;
					break;
				case 79:	// 80 ~ 360 high level knive
					maxrange = 230400;
					break;
				case 80:	// 80 ~ 480 bow
					maxrange = 409600;
					break;
				case 81:	// 80 ~ 530 high level bow
				case 162:
				case 172:
					maxrange = 498436;
					break;
				default:	// 60 ~ 85 other weapons
					maxrange = 12769;
					break;
				}
			}
			else if (m_pCaster->Gender == 0) // Female
			{
				switch (tt->GetItemShow(m_pCaster->Gender))
				{
				case 76: // 80 ~ 320 Low level knive
				case 120:
				case 127:
				case 128:
				case 167:
				case 177:
					maxrange = 181476;
					break;
				case 77:	// 80 ~ 360 high level knive
					maxrange = 230400;
					break;
				case 79:	// 80 ~ 480 bow
					maxrange = 409600;
					break;
				case 78:	// 80 ~ 530 high level bow
				case 162:
				case 172:
					maxrange = 498436;
					break;
				default:	// 60 ~ 85 other weapons
					maxrange = 12769;
					break;
				}
			}		
		}

		if (nDistX + nDistY > maxrange)
		{
			char info[400];
			sprintf(info, "MaxRange: %d Range: %d Target X: %d Y: %d X2: %d Y2: %d X3: %d Y3: %d Caster: X: %d Y: %d X2: %d Y2: %d X3: %d Y3: %d ", maxrange, nDistX + nDistY, (m_pTarget->MoveSx*32)+16, (m_pTarget->MoveSy*32)+16, m_pTarget->MoveSx, m_pTarget->MoveSy, m_pTarget->X , m_pTarget->Y, (m_pCaster->MoveSx*32)+16, (m_pCaster->MoveSy*32)+16, m_pCaster->MoveSx, m_pCaster->MoveSy, m_pCaster->X , m_pCaster->Y);
			g_pLogManager->SaveAttackRange(m_pCaster, info);
			// return false;
		}
	}
}

bool CPhysicalAttack::IsMiss() const
{	// 물리적 공격 성공 여부 판단
	if( m_pTarget && m_pTarget->IsDead() ) return true;
	const int nStatic = RareEM.GetStaticRareSmart(m_pTarget->StaticRare);
	const int nDynamic = RareEM.GetDynamicRareValue(FITEM_AGILITY, m_pTarget->DynamicRare);
	// 회피율 계산
	const int nRate = m_pCaster->CalcAttackSuccessRate(nStatic, nDynamic);
	return (nRate < rand()%101) ? true:false;
}

///////////////////////////////////////////////////////////////////////////////
// Private Method
///////////////////////////////////////////////////////////////////////////////

void CPhysicalAttack::Correct(int& rDamage)
{ 	// 아이템 속성에 의한 보정
	RareEM.CorrectByStaticRare(m_pCaster, m_pTarget, rDamage);
	// 클래스에 대한 보정
	if (m_pCaster->IsPlayer())
	{ 
		rDamage = rDamage*RATE_BY_CLASS[m_pCaster->Class][m_nIndex]/100;
	} 
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
	
}

bool CPhysicalAttack::Result(int nDamage)
{	//< CSD-031007
	BYTE nResult = HIT_FAILED;

	//Finito Added check for reporter so they cant do dmg.
	if (nDamage <= 0 || IsMiss() || m_pCaster->IsReporter())
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
		{
			if(!(rand()%10))
			{
				ItemAttr item = {0,};

				if (LocalMgr.IsChangeMoney())
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
		}
	}
	// 방어구의 내구도 감소
	::DecreaseEquipDuration(m_pTarget, nDamage, ::getEquipRandomPos(), m_pTarget->SprNo, false);
	::HungryMuchAdd(m_pCaster, HT_ATTACK);
	// NPC 종류에 따른 보정  
	if (m_pTarget->IsNpc())
	{
		switch (m_pTarget->Race)
		{
		case DUMMY:
			{
				if (m_pCaster->GetLevel() <= ENABLE_DUMMY_LEVEL)
				{	// 경험치가 10 이하면서 더미를 공격한 경우만 경험치 상승  
					const int nExp = ::addEXP(m_pCaster, m_pTarget, HIT_AND_NOTDEAD, nDamage);
					const int nTemp = (rand()%10 < 3) ? 3:4; // 일반 경험치에서 택틱 경험치로 변환상수
					m_pCaster->ChangeUpTacExp(0, nExp*nTemp);
				}
				
				nResult = HIT_AND_NOTDEAD;
				goto SEND;
			}
		case SEALSTONE:
			{
				m_pTarget->DecLife(nDamage);
				g_pLogManager->SaveSealStoneInfo(m_pCaster, m_pTarget, nDamage); // CSD-040316
				break;
			}
		case GUARDSTONE:
		//case CASTLEGATEWAY: 미결정
		//case WARARMS: 미결정
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
		if (LocalMgr.IsAbleNation(TAIWAN))
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
			m_pTarget->ChangeUpTacExp(4, m_pCaster->GetLevel()*2);
			::CheckNK(m_idCaster, m_idTarget, NK_TYPE_NO_KILL_);
		}
		
		nResult = HIT_AND_NOTDEAD;
		goto SEND;
	}
	
SEND:
	if (m_pCaster->IsPlayer() == true)  
	{
		::SaveBallancing(m_pCaster, m_pTarget, 0, nDamage, 0, 0, 0, 0);
	}
	
	const int nTactics = m_pCaster->cur_tac_skill;
	const WORD wdEffect = (m_pCaster->GetActiveCombat() > 0) ? 0:m_pCaster->GetItemEffectNumber();
	
	switch (m_nIndex)
	{
    case WR_SHORT:
    case WR_MIDDLE:
		{
			if (m_pCaster->IsPlayer())
			{
				memset(&m_packet, 0, sizeof(t_packet));
				m_packet.h.header.type = CMD_ATTACK_RESULT;
				m_packet.h.header.size = sizeof(t_attack_result);
				m_packet.u.strike.attack_result.idTarget = m_idTarget;
				m_packet.u.strike.attack_result.nDamage = -nDamage;
				m_packet.u.strike.attack_result.nX = m_pTarget->Tox;
				m_packet.u.strike.attack_result.nY = m_pTarget->Toy;
				m_packet.u.strike.attack_result.wdEffect = wdEffect; // CSD-031001
				m_packet.u.strike.attack_result.nCombat =  m_pCaster->GetActiveCombat();
				m_packet.u.strike.attack_result.nResult	= nResult;
				::QueuePacket(connections, m_idCaster, &m_packet, 1);
				
				memset(&m_packet, 0, sizeof(t_packet));
				m_packet.h.header.type = CMD_ATTACK_RESULT_D;
				m_packet.h.header.size = sizeof(t_attack_result_d);
				m_packet.u.strike.attack_result_d.idCaster = m_idCaster;
				m_packet.u.strike.attack_result_d.idTarget = m_idTarget;
				m_packet.u.strike.attack_result_d.nDamage = -nDamage;
				m_packet.u.strike.attack_result_d.nX = m_pTarget->Tox;
				m_packet.u.strike.attack_result_d.nY = m_pTarget->Toy;
				m_packet.u.strike.attack_result_d.wdEffect = wdEffect; // CSD-031001
				m_packet.u.strike.attack_result_d.nCombat = m_pCaster->GetActiveCombat();
				m_packet.u.strike.attack_result_d.nResult = nResult;
				::CastMe2Other(m_idCaster, &m_packet);
			}
			else
			{
				memset(&m_packet, 0, sizeof(t_packet));
				m_packet.h.header.type = CMD_ATTACK_RESULT_D;
				m_packet.h.header.size = sizeof(t_attack_result_d);
				m_packet.u.strike.attack_result_d.idCaster = m_idCaster;
				m_packet.u.strike.attack_result_d.idTarget = m_idTarget;
				m_packet.u.strike.attack_result_d.nDamage = -nDamage;
				m_packet.u.strike.attack_result_d.nX = m_pTarget->Tox;
				m_packet.u.strike.attack_result_d.nY = m_pTarget->Toy;
				m_packet.u.strike.attack_result_d.wdEffect = wdEffect; // CSD-031001
				m_packet.u.strike.attack_result_d.nCombat = m_pCaster->GetActiveCombat();
				m_packet.u.strike.attack_result_d.nResult = nResult;
				::CastNPC2Other(m_idCaster - 10000, &m_packet);
			}
			
			break;
		}
    case WR_LONG1:
    case WR_LONG2:
		{
			if (m_pCaster->IsPlayer())
			{
				memset(&m_packet, 0, sizeof(t_packet)); 
				m_packet.h.header.type = CMD_THROW_RESULT;
				m_packet.h.header.size = sizeof(t_server_throw_result); 
				m_packet.u.strike.server_throw_result.idTarget = m_idTarget;
				m_packet.u.strike.server_throw_result.nX = m_pTarget->Tox;
				m_packet.u.strike.server_throw_result.nY = m_pTarget->Toy;
				m_packet.u.strike.server_throw_result.nDamage = -nDamage;
				m_packet.u.strike.server_throw_result.wdEffect = wdEffect; // CSD-031001
				m_packet.u.strike.server_throw_result.nCombat = m_pCaster->GetActiveCombat();
				m_packet.u.strike.server_throw_result.nResult = nResult;
				::QueuePacket(connections, m_idCaster, &m_packet, 1);
				
				memset(&m_packet, 0, sizeof(t_packet));
				m_packet.h.header.type = CMD_THROW_RESULT_D;
				m_packet.h.header.size = sizeof(t_throw_result_d);
				m_packet.u.strike.throw_result_d.idCaster = m_idCaster;
				m_packet.u.strike.throw_result_d.idTarget = m_idTarget;
				m_packet.u.strike.throw_result_d.nDamage = -nDamage;
				m_packet.u.strike.throw_result_d.nX = m_pTarget->Tox;
				m_packet.u.strike.throw_result_d.nY = m_pTarget->Toy;
				m_packet.u.strike.throw_result_d.wdEffect = wdEffect; // CSD-031001
				m_packet.u.strike.throw_result_d.nCombat = m_pCaster->GetActiveCombat();
				m_packet.u.strike.throw_result_d.nResult = nResult;
				::CastMe2Other(m_idCaster, &m_packet);
			}
			
			break;
		}
	case WR_EFFECT:
		{
			memset(&m_packet, 0, sizeof(t_packet)); 
			m_packet.h.header.type = CMD_EFFECT_RESULT;
			m_packet.h.header.size = sizeof(t_server_effect_result); 
			m_packet.u.strike.server_effect_result.idTarget = m_idTarget;
			m_packet.u.strike.server_effect_result.nDamage = -nDamage;
			m_packet.u.strike.server_effect_result.nResult = nResult;
			CBattle::SendPacket(m_idCaster);
			break;
		}
	}
	
	if (!m_pCaster->IsNpc() && m_pTarget->IsStrikeReflect())
	{
		if (rand()%101 <= 20)
		{
			const int nReal = m_pCaster->DecLife(nDamage, 10);
			
			if (nReal > 0)
			{
				SendBattleDamage(m_idCaster, 0, -nReal);
			}
		}
	}
	
	return true;
}	//> CSD-031007