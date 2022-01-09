// BattleManager.cpp: implementation of the CBattleManager class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "BattleManager.h"

#include "PhysicalAttack.h"
#include "AttackMagic.h"
#include "AssistanceMagic.h"
#include "ProtectionMagic.h"
#include "CursingMagic.h"
#include "RecoveryMagic.h"
#include "ExtraMagic.h"

#include "ThunderCombat.h" 
#include "EarthCombat.h"
#include "FlameCombat.h"
#include "DarkCombat.h"
#include "FreezeCombat.h"
#include "WindCombat.h"

#include "Op_Magic.h"
#include "Op_Battle.h"
#include "CItem.h"
#include "SealStoneGameServer.h"
#include "NationSys.h"
#include "scrp_exe.h"
#include "LogManager.h"
#include "UserManager.h"
#include "ArenaManager.h"
#include "SymbolItemMgr.h" // 031110 kyo
#include "eventmgr.h" //Eleval

//<< 031111 kyo
#include "SymbolItemMgr.h"
extern CSymbolItemMgr g_CSymbolMgr;
//>> 031111 kyo


extern int g_isLocalWarServer;
extern int g_LocalWarBegin;
extern bool IsWar();
extern bool isAttacker(CHARLIST* pCaster);
extern bool isNationWarfieldServer();
extern int CanLocalWarAttack(CHARLIST* pAttacker,CHARLIST* pDefencer); // 020115 LTS
extern int CheckLocalWarAttacker(CHARLIST* pAttacker);						     // 020115 LTS
extern int CheckEventAttack();	 // 020115 LTS
extern int CheckEventWarDoing(); // 020115 LTS
extern void SaveObtainCombat(CHARLIST* ch, int nOld, int nNew);
extern void SaveChangeCombat(CHARLIST* ch, int nCombat, int nOld, int nNew);
extern bool isNewWarfieldServer();
extern bool isMyTeam(LPCHARLIST pCaster,LPCHARLIST pTarget);
extern CSymbolItemMgr g_CSymbolMgr; //031110 kyo

EventPKRestriction g_PKRestriction; //Eleval 24/07/09

///////////////////////////////////////////////////////////////////////////////
// Global Member
///////////////////////////////////////////////////////////////////////////////

DWORD CBattleManager::s_dwDelay = 0;

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CBattleManager::CBattleManager()
{
	m_vtBuffer.clear();
	m_vtBuffer.reserve(13);
	// magic_type에 따라서 순서적으로 생성
	m_vtBuffer.push_back(new CPhysicalAttack);  // [ 0] 물리적 공격
	m_vtBuffer.push_back(new CRecoveryMagic);   // [ 1] 10 ~ 19 : 회복계열 마법
	m_vtBuffer.push_back(new CProtectionMagic); // [ 2] 20 ~ 29 : 보호계열 마법
	m_vtBuffer.push_back(new CAssistanceMagic); // [ 3] 30 ~ 39 : 보조계열 마법
	m_vtBuffer.push_back(new CCursingMagic);    // [ 4] 40 ~ 49 : 저주계열 마법
	m_vtBuffer.push_back(new CAttackMagic);     // [ 5] 50 ~ 59 : 공격계열 마법
	m_vtBuffer.push_back(new CExtraMagic);      // [ 6] 60 ~ 69 : 시공, 소환, 기타계열 마법
	// order_type에 따라서 순서적으로 생성
	m_vtBuffer.push_back(new CThunderCombat);   // [ 7] 61 : 번개속성 전투스킬
	m_vtBuffer.push_back(new CEarthCombat);     // [ 8] 62 : 대지속성 전투스킬
	m_vtBuffer.push_back(new CFlameCombat);     // [ 9] 63 : 화염속성 전투스킬
	m_vtBuffer.push_back(new CDarkCombat);      // [10] 64 : 어둠속성 전투스킬
	m_vtBuffer.push_back(new CFreezeCombat);    // [11] 65 : 얼음속성 전투스킬
	m_vtBuffer.push_back(new CWindCombat);      // [12] 66 : 바람속성 전투스킬
	// Method 바인딩
	for_each(m_vtBuffer.begin(), m_vtBuffer.end(), mem_fun(&CBattle::Bind));

	m_cThrowResult = 0;
	m_cThrowResultAvg = 0;
	m_dwLastThrowAttack = 0;

	m_nLastCombatSelect = 0;
	m_cCombatResult = 0;
	m_cCombatResultAvg = 0;
	m_dwLastCombatResult = 0;
}

CBattleManager::~CBattleManager()
{
	for_each(m_vtBuffer.begin(), m_vtBuffer.end(), TDelete());
	m_vtBuffer.clear();
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CBattleManager::Elapse(DWORD dwSecond)
{	//< CSD-CHAOS : 시간관련 상태를 조정
	if (g_curr_time < s_dwDelay)
	{
		return;
	}

	mem_fun1_t<bool, CBattle, CHARLIST*> mf(&CBattle::Elapse);
	CHARLIST* pTemp = NULL;
	
	for (int i = DRAGON_CONNECTIONS_START; i < DRAGON_MAX_CONNECTIONS; ++i)
	{	
		pTemp = ::CheckServerId(i);

		if (pTemp != NULL)
		{
			pTemp->IncPoisonedCount();

			::CheckCharacterCondition(pTemp);
			::CheckTransparency(pTemp);
			// 마법이나 전투스킬 상태 검사
			for_each(m_vtBuffer.begin(), m_vtBuffer.end(), bind2nd(mf, pTemp));
		}
	}	
	
	for (int j = NPC_LIST_START; j < MAX_NPC_LIST; ++j)
	{
		pTemp = ::CheckNpcId(j);

		if (pTemp != NULL && pTemp->bAlive != REMOVE_)
		{
			pTemp->IncPoisonedCount();
		
			::CheckCharacterCondition(pTemp);
			// 마법이나 전투스킬 상태 검사
			for_each(m_vtBuffer.begin(), m_vtBuffer.end(), bind2nd(mf, pTemp));
		}
	}
	// 지연 시간 설정
	s_dwDelay = g_curr_time + dwSecond;
}	//> CSD-CHAOS

void CBattleManager::ClearCombat(BYTE nCombat, WORD idMaster)
{	//< CSD-TW-030623
	CHARLIST* pMaster = ::GetCharListPtr(idMaster);
	if (pMaster == NULL)  return;
	
	if (nCombat != 0 && !IsEnableWeapon(nCombat, pMaster))
	{	// 무기를 바꾼 경우에 처리
		t_client_combat_clear packet;
		packet.nType = 0;
		RecvCombatClear(idMaster, &packet);
		return;
	}
	
	switch (nCombat)
	{
    case LIGHTNING_BOOM:
    case THUNDER_BLOW:
    case LIGHTNING_SHOCK:
    case THUNDER_STRIKE:
    case BLOOD_WILL:
    case GROUND_ATTACK:
    case SWORD_N_ROSES:
    case FLEET_MISSILE:
    case CONVERTING_ARMOR:
    case CHERROY_SHADE:
    case DARK_BLADE:
    case SHARK_MISSILE:
    case HORN_OF_ICEBERG:
    case DOUBLE_ATTACK:
    case CHARGING:
    case MULTIPLE_FIRE:
    case ICING_BLAST:
    case LIGHTNING_EXTREME:
    case EARTH_EXTREME:
    case FIRE_EXTREME:
    case DARK_EXTREME:
    case ICE_EXTREME:
    case WIND_EXTREME:
		{
			if (pMaster->IsCount())
			{
				const int nCount = pMaster->DecCombatCount();
				pMaster->SendCharInfoBasic( BP, nCount);//020704 lsw
				break;
			}
			// 전투스킬 초기화
			pMaster->InitActiveCombat();
			pMaster->ClearActiveState();
			// 관련 Packet 전송  
			memset(&m_packet, 0, sizeof(t_packet));
			m_packet.h.header.type = CMD_COMBAT_INIT;
			m_packet.h.header.size = sizeof(t_combat_init);
			m_packet.u.combat.server_combat_clear.idMaster = idMaster;
			m_packet.u.combat.server_combat_clear.nType = ACTIVE_COMBAT;
			::QueuePacket(connections, idMaster, &m_packet, 1);
			::CastMe2Other(idMaster, &m_packet);
			break;
		}
    case CRITICAL_HIDING:
		{
			if (pMaster->IsCount())
			{
				const int nCount = pMaster->DecCombatCount();
				pMaster->SendCharInfoBasic( BP, nCount);//020704 lsw
				
				if (nCount <= 0)
				{
					::CheckTransparency(pMaster, true);
				}
				
				break;
			}
			// 전투스킬 초기화
			pMaster->InitActiveCombat();
			pMaster->ClearActiveState();
			// 관련 Packet 전송  
			memset(&m_packet, 0, sizeof(t_packet));
			m_packet.h.header.type = CMD_COMBAT_INIT;
			m_packet.h.header.size = sizeof(t_combat_init);
			m_packet.u.combat.server_combat_clear.idMaster = idMaster;
			m_packet.u.combat.server_combat_clear.nType = ACTIVE_COMBAT;
			::QueuePacket(connections, idMaster, &m_packet, 1);
			::CastMe2Other(idMaster, &m_packet);
			break;
		}
	}
}	//> CSD-TW-030623

void CBattleManager::ChangeCombat(BYTE nCombat, WORD idMaster)
{
	CHARLIST* pMaster = ::GetCharListPtr(idMaster);
	if (pMaster == NULL)  return;
	
	switch (nCombat)
	{
    case BLOOD_EARTH:
    case TWISTER:
    case POISONING_NOVA:
    case WHILWIND:
		{
			const int nCount = pMaster->DecCombatCount();
			pMaster->SendCharInfoBasic(BP, nCount);//020704 lsw
			break;
		}
	}
}

void CBattleManager::ResetCombat(BYTE nCombat, WORD idMaster)
{	// 전투스킬 사용가능수 재설정
	CHARLIST* pMaster = ::GetCharListPtr(idMaster);
	if (pMaster == NULL)  return;
	// CP량 재설정  
	pMaster->DecCombat(Magic_Ref[nCombat].exhaust_MP);
	// 액티브 속성의 전투스킬인 경우
	if (IsActiveCombat(nCombat))
	{
		int nMax = 0, nNow = 0;
		
		switch (nCombat)
		{
		case BLOOD_EARTH:
		case POISONING_NOVA:
		case WHILWIND:
		case TWISTER:
			{ // 사용뒤 감소
				nMax = Magic_Ref[nCombat].nCombatCount;
				pMaster->SetActiveCombat(nCombat, nMax);
				pMaster->SendCharInfoBasic(MAX_BP, nMax);//020704 lsw
				nNow = pMaster->DecCombatCount();
				pMaster->SendCharInfoBasic(BP, nNow);//020704 lsw
				break;
			}
		case STONE_ARMOR:
			{
				nMax = pMaster->GetCombatValue(nCombat);
				pMaster->SetActiveCombat(nCombat, nMax);
				pMaster->SendCharInfoBasic(MAX_BP, nMax);//020704 lsw
				nNow = pMaster->GetCombatCount();
				pMaster->SendCharInfoBasic(BP, nNow);//020704 lsw
				break;
			}
		default:
			{ // 공격뒤 감소
				nMax = Magic_Ref[nCombat].nCombatCount;
				pMaster->SetActiveCombat(nCombat, nMax);
				pMaster->SendCharInfoBasic(MAX_BP, nMax);//020704 lsw
				nNow = pMaster->GetCombatCount();
				pMaster->SendCharInfoBasic(BP, nNow);//020704 lsw
				break;
			}
		}
		
		return;
	}
	// 패시브 속성의 전투스킬인 경우
	if (IsPassiveCombat(nCombat))
	{
		DWORD dwPeriod = g_curr_time + Magic_Ref[nCombat].continue_Time;
		pMaster->SetPassiveCombat(nCombat, dwPeriod);
		return;
	}
	// 회복 속성의 전투스킬인 경우
	if (IsRecoveryCombat(nCombat))
	{	// 030415 kyo
		DWORD dwPeriod = g_curr_time + Magic_Ref[nCombat].continue_Time;
		pMaster->SetRecoveryCombat(nCombat, dwPeriod);
		return;
	}
}

void CBattleManager::SendNpcAttack(WORD idNpc)
{
	// 공격자
	WORD idCaster = idNpc + 10000;
	CHARLIST* pCaster = ::GetCharListPtr(idCaster);
	if (pCaster == NULL)  return;
	// 방어자
	WORD idTarget = pCaster->targetid;
	CHARLIST* pTarget = ::GetCharListPtr(idTarget);
	if (pTarget == NULL)  return;
	// 공격 수행
	t_npc_attack packet;
	packet.idCaster = idCaster;
	packet.idTarget = idTarget;
	packet.nX = pTarget->Tox;
	packet.nY = pTarget->Toy;
	RecvNpcAttack(&packet);
}

void CBattleManager::SendNpcThrow(WORD idNpc)
{
	// 공격자
	WORD idCaster = idNpc + 10000;
	CHARLIST* pCaster = ::GetCharListPtr(idCaster);
	if (pCaster == NULL)  return;
	// 방어자
	WORD idTarget = pCaster->targetid;
	CHARLIST* pTarget = ::GetCharListPtr(idTarget);
	if (pTarget == NULL)  return;
	// 공격 수행
	t_npc_throw packet;
	packet.idCaster = idCaster;
	packet.idTarget = idTarget;
	packet.nX = pTarget->Tox;
	packet.nY = pTarget->Toy;
	RecvNpcThrow(&packet);
}

void CBattleManager::SendNpcMagic(WORD idNpc, BYTE nMagic)
{ // 대상 지정 마법
	// 시전자
	WORD idCaster = idNpc + 10000;
	CHARLIST* pCaster = ::GetCharListPtr(idCaster);
	if (pCaster == NULL)  return;
	// 대상자
	WORD idTarget = pCaster->targetid;
	CHARLIST* pTarget = ::GetCharListPtr(idTarget);
	if (pTarget == NULL)  return;
	// 마법 수행
	t_client_npc_magic packet;
	packet.nMagic = nMagic;
	packet.idCaster = idCaster;
	packet.idTarget = idTarget;
	packet.nX = pTarget->X;
	packet.nY = pTarget->Y;
	packet.bTimeDisable = false; // LTS DRAGON MODIFY
	RecvNpcMagic(&packet);
}

void CBattleManager::SendNpcMagic(WORD idNpc, BYTE nMagic, int nX, int nY, bool bTimeDisable)
{ // 지역 마법
	WORD idCaster = idNpc + 10000;
	CHARLIST* pCaster = ::GetCharListPtr(idCaster);
	if (pCaster == NULL)  return;
	// 마법 수행
	t_client_npc_magic packet;
	packet.nMagic = nMagic;
	packet.idCaster = idCaster;
	packet.idTarget = idCaster;
	packet.nX = nX;
	packet.nY = nY;
	packet.bTimeDisable = bTimeDisable;	// LTS DRAGON MODIFY
	RecvNpcMagic(&packet);
}

void CBattleManager::SendNpcRecall(WORD idNpc, BYTE nMagic, BYTE nMonster, int nX, int nY)
{ // 소환 마법
	WORD idMaster = idNpc + 10000;
	CHARLIST* pMaster = ::GetCharListPtr(idMaster);
	if (pMaster == NULL)  return;
	// 소환할 몬스터 설정
	pMaster->SetFollow(nMonster);
	// 마법 수행
	t_client_npc_magic packet;
	packet.nMagic = nMagic;
	packet.idCaster = idMaster;
	packet.idTarget = idMaster;
	packet.nX = nX << 5;
	packet.nY = nY << 5;
	packet.bTimeDisable = true;			// LTS DRAGON MODIFY		
	RecvNpcRecall(&packet); // CSD-021119
}

void CBattleManager::SendNpcTeleport(WORD idNpc, int nX, int nY)
{	//< CSD-030314
	WORD idMaster = idNpc + 10000;
	CHARLIST* pMaster = ::GetCharListPtr(idMaster);
	if (pMaster == NULL)  return;
	// 마법 수행
	t_client_npc_magic packet;
	packet.nMagic = TELEPORT;
	packet.idCaster = idMaster;
	packet.idTarget = idMaster;
	packet.nX = nX;
	packet.nY = nY;
	packet.bTimeDisable = false;
	RecvNpcTeleport(&packet);
}	//> CSD-030314

void CBattleManager::RecvNpcAttack(t_npc_attack* pPacket)
{	//< CSD-030723
	const WORD idCaster = pPacket->idCaster;
	const WORD idTarget = pPacket->idTarget;
	CHARLIST* pCaster = ::GetCharListPtr(idCaster);
	CHARLIST* pTarget = ::GetCharListPtr(idTarget);
	if (pCaster == NULL || pTarget == NULL)  return;
	
	if (FilterNpcAttack(pCaster, pTarget) == false)
	{
		return;
	}
	
	const int nBattle = 0; // 현재 NPC는 단거리 공격만 가능하도록 설정
	GetBattle(nBattle)->SetIndex(WR_SHORT);
	GetBattle(nBattle)->SetBothID(idCaster, idTarget);
	GetBattle(nBattle)->SetBothPtr(pCaster, pTarget);
	GetBattle(nBattle)->Execute();
}	//> CSD-030723

void CBattleManager::RecvStrikeAttack(WORD idCaster, t_player_attack* pPacket)
{	//< CSD-030723
	const WORD idTarget = pPacket->idTarget;
	CHARLIST* pCaster = ::GetCharListPtr(idCaster);
	CHARLIST* pTarget = ::GetCharListPtr(idTarget);
	if (pCaster == NULL || pTarget == NULL)     return;
	// 공격 가능여부 검사
	if (!IsBattle())                            return;
	if (!IsBattle(pCaster))                     return;
	if (!IsBattle(pCaster, pTarget))            return;
	if (!pCaster->CheckAttack(2, g_curr_time))  return;
	if (!FilterAttack(pCaster, pTarget))        return;
	// 현상범인지 여부 설정
	pTarget->SetWanted(pPacket->bWanted);
	// 저주속성 설정
	AutoCurse(idCaster, idTarget);
	// 전투스킬 사용 가능수 계산
	const int nBattle = 0;
	const int nCombat = pCaster->GetActiveCombat();
	ClearCombat(nCombat, idCaster);
	// CSD-030806
	g_pLogManager->SaveStrikeAttack(pCaster, pTarget);

	switch (pCaster->GetTacticsKind())
	{	
	case TACTICS_Archery: // 장거리 무기
	case TACTICS_Hurl:
		{ 
			return;
		}
    case TACTICS_Whirl:   // 중거리 무기
    case TACTICS_Pierce:
    case TACTICS_Magery:
    case TACTICS_Orison:    
		{ 
			GetBattle(nBattle)->SetIndex(WR_MIDDLE);
			break;
		}
    default:              //	단거리 무기  
		{ 
			GetBattle(nBattle)->SetIndex(WR_SHORT);
			break;
		}
	}
	
	GetBattle(nBattle)->SetCurrentTime(g_curr_time);
	GetBattle(nBattle)->SetBothID(idCaster, idTarget);
	GetBattle(nBattle)->SetBothPtr(pCaster, pTarget);
	// 공격 횟수 설정
	int nCount = 1;
	
	switch (nCombat)
	{
    case SWORD_N_ROSES: nCount = pCaster->GetCombatLevel(nCombat); break;
    case DOUBLE_ATTACK: nCount = 2;                                break;
	}
	
	for (int i = 0; i < nCount; ++i)
	{
		GetBattle(nBattle)->Execute();
	}
}	//> CSD-030723

void CBattleManager::RecvNpcThrow(t_npc_throw* pPacket)
{
	const WORD idCaster = pPacket->idCaster;
	const WORD idTarget = pPacket->idTarget;
	CHARLIST* pCaster = ::GetCharListPtr(idCaster);
	CHARLIST* pTarget = ::GetCharListPtr(idTarget);
	if (pCaster == NULL || pTarget == NULL)  return;
	if (!FilterNpcAttack(pCaster, pTarget))  return;
	// Packet 전송
	memset(&m_packet, 0, sizeof(t_packet));
	m_packet.h.header.type = CMD_THROW_ATTACK;
	m_packet.h.header.size = sizeof(t_server_throw_attack);
	m_packet.u.strike.server_throw_attack.idCaster = idCaster;
	m_packet.u.strike.server_throw_attack.idTarget = idTarget;
	m_packet.u.strike.server_throw_attack.nX = pTarget->Tox;
	m_packet.u.strike.server_throw_attack.nY = pTarget->Toy;
	::CastNPC2Other(idCaster - 10000, &m_packet);
}

void CBattleManager::RecvThrowAttack(WORD idCaster, t_client_throw_attack* pPacket)
{	//< CSD-TW-030606
	const WORD idTarget = pPacket->idTarget;
	CHARLIST* pCaster = ::GetCharListPtr(idCaster);
	CHARLIST* pTarget = ::GetCharListPtr(idTarget);
	if (pCaster == NULL || pTarget == NULL)     return;

	// 공격 가능여부 검사
	if (g_block_attack)                         return;
	if (!IsBattle(pCaster))                     return;
	if (!IsBattle(pCaster, pTarget))            return;
	if (!pCaster->CheckAttack(2, g_curr_time))  return;
	
	const int nCombat = pCaster->GetActiveCombat();
	ClearCombat(nCombat, idCaster);
	if (!FilterAttack(pCaster, pTarget))        return;
	// 현상범인지 여부 설정
	pTarget->SetWanted(pPacket->bWanted);
	// CSD-030806
	g_pLogManager->SaveThrowAttack(pCaster, pTarget);

	switch (pCaster->GetTacticsKind())
	{	
	case TACTICS_Archery: // 활쏘기
		{
			if (!pCaster->IsArrow())
			{
				pCaster->Message(MK_SHORTAGE, 0, 31);
				return;
			}
		}
	case TACTICS_Hurl: // 비검 던지기
		{ 
			memset(&m_packet, 0, sizeof(t_packet));
			m_packet.h.header.type = CMD_THROW_ATTACK;
			m_packet.h.header.size = sizeof(t_server_throw_attack);
			m_packet.u.strike.server_throw_attack.idCaster = idCaster;
			m_packet.u.strike.server_throw_attack.idTarget = idTarget;
			m_packet.u.strike.server_throw_attack.nX = pTarget->Tox;
			m_packet.u.strike.server_throw_attack.nY = pTarget->Toy;
			m_packet.u.strike.server_throw_attack.nCombat = pCaster->GetActiveCombat();
			::QueuePacket(connections, idCaster, &m_packet, 1);
			::CastMe2Other(idCaster, &m_packet);
			break;
		}
	}
	m_cThrowResult = 0;
}	//> CSD-TW-030606

void CBattleManager::RecvThrowResult(WORD idCaster, t_client_throw_result* pPacket)
{	//< CSD-030723
	const WORD idTarget = pPacket->idTarget; // 마법을 사용한 대상자
	CHARLIST* pCaster = ::GetCharListPtr(idCaster);
	CHARLIST* pTarget = ::GetCharListPtr(idTarget);
	if (pCaster == NULL || pTarget == NULL)  return;  
	if (!FilterThrow(pCaster, pTarget))      return;

	if(g_curr_time != m_dwLastThrowAttack) { m_cThrowResultAvg = 0; m_dwLastThrowAttack = g_curr_time; }
	
	if(m_cThrowResult > 20 || (g_curr_time - m_dwLastThrowAttack >= 1 && m_cThrowResultAvg > 25))
	{
		//MyLog(LOG_NORMAL, "Hacker! %d %d", m_cThrowResult, m_cThrowResultAvg);
		char info[256];
		sprintf_s(info, sizeof(info), "m_cThrowResult %d m_cThrowResultAvg %d", m_cThrowResult, m_cThrowResultAvg);
		g_pLogManager->SaveEtc(pCaster, info);

		//closeconnection_msg(connections, idCaster, -200, "How about, no?");
		return;
	}
	
	// 현상범인지 여부 설정
	pTarget->SetWanted(pPacket->bWanted);
	// 저주속성 설정
	AutoCurse(idCaster, idTarget);

	const int nBattle = 0; 
	
	switch (pCaster->GetTacticsKind())
	{	
	case TACTICS_Archery: 
		{ 
			GetBattle(nBattle)->SetIndex(WR_LONG1);
			break; 
		}
	case TACTICS_Hurl:    
		{ 
			if (pCaster->GetWeaponKind() == IK_THROW_EVENT)
			{	// 눈싸움
				GetBattle(nBattle)->SetIndex(WR_LONG3);
			}
			else
			{	// 비검 던지기
				GetBattle(nBattle)->SetIndex(WR_LONG2);
			}
			
			break;
		}
	default:
		{
			return;
		}
	}  
	GetBattle(nBattle)->SetCurrentTime(g_curr_time);
	GetBattle(nBattle)->SetBothID(idCaster, idTarget);
	GetBattle(nBattle)->SetBothPtr(pCaster, pTarget);
	GetBattle(nBattle)->Execute();
	
	m_cThrowResult++;
	m_cThrowResultAvg++;
}	//> CSD-030723

void CBattleManager::RecvEffectResult(WORD idCaster, t_client_effect_result* pPacket)
{	//< CSD-031007
	const WORD wdEffect = pPacket->wdEffect;
	const WORD idTarget = pPacket->idTarget;
	CHARLIST* pCaster = ::GetCharListPtr(idCaster);
	CHARLIST* pTarget = ::GetCharListPtr(idTarget);
	if (pCaster == NULL || pTarget == NULL)  return;
	// 서로 공격 가능여부 검사
	if (!IsBattle())
	{
		return;
	}

	if (!IsBattle(pCaster))
	{
		return;
	}

	if (!IsBattle(pCaster, pTarget))
	{
		return;
	}

	if (!FilterAttack(pCaster, pTarget))
	{
		return;
	}
	
	pTarget->SetWanted(pPacket->bWanted); // 현상범인지 여부 설정
	
	const int nBattle = 0;
	const int nX = pPacket->nX;           // 전투스킬이 발생된 위치의 X좌표
	const int nY = pPacket->nY;           // 전투스킬이 발생된 위치의 Y좌표
	GetBattle(nBattle)->SetIndex(WR_EFFECT);
	GetBattle(nBattle)->SetBothID(idCaster, idTarget);
	GetBattle(nBattle)->SetBothPtr(pCaster, pTarget);
	GetBattle(nBattle)->SetCurrentTime(g_curr_time);
	GetBattle(nBattle)->SetContinueTime(0);
	GetBattle(nBattle)->SetPosition(nX>>5, nY>>5);
	GetBattle(nBattle)->Execute();
}	//< CSD-031007

void CBattleManager::RecvMagicLearn(WORD idMaster, k_client_learn_magic* pPacket)
{
	CHARLIST* pMaster = ::GetCharListPtr(idMaster);
	if (pMaster == NULL)  return;
	
	const int nMagic = pPacket->num;
	
	switch (pMaster->Spell)
	{
    case WIZARD_SPELL:
		{
			if (!IsEnableClass(nMagic, pMaster))  
			{
				return;
			}
			
			if (pMaster->Ws[nMagic])
			{
				::SendServerResult(CM_LEARN_MAGIC, 2, idMaster);
				return;
			}
			
			break;
		}
    case PRIEST_SPELL:
		{
			if (!IsEnableClass(nMagic + 150, pMaster))  
			{
				return;
			}
			
			if (pMaster->Ps[nMagic] != 0)	
			{
				::SendServerResult(CM_LEARN_MAGIC, 2, idMaster);
				return;
			}
			
			break;
		}
	}
	
	switch (::CanLearnMagic(nMagic, pMaster))
	{
	case 1:	 
		{
			::SendLearnMagicOk(nMagic, idMaster);	
			break;
		}
    case 3:	 
		{
			::SendServerResult(CM_LEARN_MAGIC, 3, idMaster);	
			return;
		}
    case 4:	 
		{
			::SendServerResult(CM_LEARN_MAGIC, 4, idMaster);	
			return;
		}
	default:	
		{
			return;
		}
	}
	
	switch (pMaster->Spell)
	{
    case WIZARD_SPELL:
		{
			::SubtractMoney(Magic_Ref[nMagic].Price, pMaster);
			pMaster->Ws[nMagic] = true;
			break;
		}
    case PRIEST_SPELL:
		{
			pMaster->Ps[nMagic] = true;
			break;
		}
	}
}

void CBattleManager::RecvNpcMagic(t_client_npc_magic* pPacket)
{
	const WORD idCaster = pPacket->idCaster; // 마법을 사용한 시전자
	const WORD idTarget = pPacket->idTarget; // 마법을 사용한 대상자
	const int nMagic = pPacket->nMagic;
	const bool bTimeDisable = pPacket->bTimeDisable;	// LTS DRAGON MODIFY
	
	CHARLIST* pCaster = ::GetCharListPtr(idCaster);
	CHARLIST* pTarget = ::GetCharListPtr(idTarget);
	if (pCaster == NULL || pTarget == NULL)  return;  
	
	switch (nMagic)
	{
    case FIRE_ARROW:
    case ICE_ARROW: 
		{ // 화염화살이나 얼음화살인 경우는 물리적 공격으로 처리
			return;
		}
    case SUMMONING_SKELETON: // 스켈레톤킹 소환
    case SUMMONING_UNDEAD:   // 언데드 소환
		{
			if (pCaster->m_xSummon.Count() >= Magic_Ref[nMagic].nCombatCount)
			{ // 소환된 몬스터수가 최대라면 마법을 실행하지 않음
				return;
			}
			
			break;
		}
	}
	
	if (!bTimeDisable && !pCaster->CheckDelay(nMagic, g_curr_time))		// LTS DRAGON MODIFY
	{
		pCaster->ClearMagic();
		return;
	}
	
	bool bFail = false;
	// 석화에 걸려있으면 어떠한 마법도 적용을 받지 못함. 단 저주해제는 제외
	if (pTarget->IsStone() && pPacket->nMagic != REMOVE_CURSE)
	{		
		bFail = true;
		goto MAGIC_FAIL;
	}
	// 마법 사용 산출 여부 가능 검사
	if (!FilterNpcMagic(nMagic, pCaster, pTarget, pPacket->nX, pPacket->nY))
	{ 
		bFail = true;
		goto MAGIC_FAIL;
	}
	
MAGIC_FAIL:
	memset(&m_packet, 0, sizeof(t_packet));
	m_packet.h.header.type = CMD_NPC_MAGIC;
	m_packet.h.header.size = sizeof(t_server_npc_magic);
	m_packet.u.magic.server_npc_magic.nMagic = pPacket->nMagic;
	m_packet.u.magic.server_npc_magic.idCaster = idCaster;
	m_packet.u.magic.server_npc_magic.idTarget = idTarget;
	m_packet.u.magic.server_npc_magic.nX = pPacket->nX;
	m_packet.u.magic.server_npc_magic.nY = pPacket->nY;
	m_packet.u.magic.server_npc_magic.bFailed = false;
	::CastNPC2Other(idCaster - 10000, &m_packet);
}

void CBattleManager::RecvNpcRecall(t_client_npc_magic* pPacket)
{ //< CSD-021119
	const WORD idCaster = pPacket->idCaster; // 마법을 사용한 시전자
	const WORD idTarget = pPacket->idTarget; // 마법을 사용한 대상자
	const int nMagic = pPacket->nMagic;
	const int nX = pPacket->nX;
	const int nY = pPacket->nY;
	CHARLIST* pCaster = ::GetCharListPtr(idCaster);
	CHARLIST* pTarget = ::GetCharListPtr(idTarget);
	if (pCaster == NULL || pTarget == NULL)  return;  
	// 마법 사용 산출 여부 가능 검사
	if (!FilterNpcMagic(nMagic, pCaster, pTarget, nX, nY))
	{ 
		return;
	}
	
	const int nBattle = Magic_Ref[nMagic].magic_Type/10;
	GetBattle(nBattle)->SetIndex(nMagic);
	GetBattle(nBattle)->SetBothID(idCaster, idTarget);
	GetBattle(nBattle)->SetBothPtr(pCaster, pTarget);
	GetBattle(nBattle)->SetCurrentTime(g_curr_time);
	GetBattle(nBattle)->SetContinueTime(0);
	GetBattle(nBattle)->SetPosition(nX>>5, nY>>5);
	GetBattle(nBattle)->Execute();
} //> CSD-021119

void CBattleManager::RecvNpcTeleport(t_client_npc_magic* pPacket)
{	//< CSD-030306
	const WORD idCaster = pPacket->idCaster; // 마법을 사용한 시전자
	const WORD idTarget = pPacket->idTarget; // 마법을 사용한 대상자
	const int nMagic = pPacket->nMagic;
	const int nX = pPacket->nX;
	const int nY = pPacket->nY;
	CHARLIST* pCaster = ::GetCharListPtr(idCaster);
	CHARLIST* pTarget = ::GetCharListPtr(idTarget);
	if (pCaster == NULL || pTarget == NULL)  return;  
	// 마법 사용 산출 여부 가능 검사
	if (!FilterNpcMagic(nMagic, pCaster, pTarget, nX, nY))
	{ 
		return;
	}
	
	const int nBattle = Magic_Ref[nMagic].magic_Type/10;
	GetBattle(nBattle)->SetIndex(nMagic);
	GetBattle(nBattle)->SetBothID(idCaster, idTarget);
	GetBattle(nBattle)->SetBothPtr(pCaster, pTarget);
	GetBattle(nBattle)->SetCurrentTime(g_curr_time);
	GetBattle(nBattle)->SetContinueTime(0);
	GetBattle(nBattle)->SetPosition(nX>>5, nY>>5);
	GetBattle(nBattle)->Execute();
} //> CSD-030306

void CBattleManager::RecvMagicSelect(WORD idCaster, t_client_magic_select* pPacket)
{	//< CSD-TW-030623
	CHARLIST* pCaster = ::GetCharListPtr(idCaster);
	
	if (pCaster == NULL)
	{
		return;
	}

	if (pCaster->IsRecoveryCombatState())
	{
		pCaster->Message(MK_NORMAL, 1, 42);
		return;
	}

	pCaster->InitActiveCombat();
	pCaster->ClearActiveState();
	pCaster->ClearPassiveState();
	pCaster->ClearRecoveryState();
	
	memset(&m_packet, 0, sizeof(t_packet));
	m_packet.h.header.type = CMD_MAGIC_SELECT;
	m_packet.h.header.size = sizeof(t_server_magic_select);
	m_packet.u.magic.server_magic_select.idCaster = idCaster;
	m_packet.u.magic.server_magic_select.nMagic = pPacket->nMagic;
	::QueuePacket(connections, idCaster, &m_packet, 1);
	::CastMe2Other(idCaster, &m_packet);
}	//> CSD-TW-030623

void CBattleManager::RecvMagicCasting(WORD idCaster, t_client_magic_casting* pPacket)
{	//< CSD-TW-030606
	CHARLIST* pCaster = GetCharListPtr(idCaster);
	if (pCaster == NULL)  return;  
	// 마법 시전 검사 설정
	const BYTE nMagic = pPacket->nMagic;

#ifdef LMS_MAPSERVER 
	switch (nMagic)
	{
	case PERFECT_PROTECT:
	case PROTECT_RANGE_WEAPON:
		return;
	}
#endif

	const DWORD dwNow = g_curr_time + RareEM.GetStaticRareBear(pCaster->StaticRare);
	
	if (!pCaster->CheckStart(nMagic, g_curr_time)) 
	{ 
		pCaster->ClearMagic();
		return;
	}
	// 서로 공격 가능여부 검사
	if (!IsBattle(pCaster))  
	{
		pCaster->ClearMagic();
		return;
	}
	// 마법 시전 산출 여부 가능 검사
	if (!FilterStart(pCaster))
	{ 
		pCaster->ClearMagic();
		return;
	}
	
	pCaster->m_nCastMagic = nMagic;
	
	memset(&m_packet, 0, sizeof(t_packet));
	m_packet.h.header.type = CMD_MAGIC_CASTING;
	m_packet.h.header.size = sizeof(t_server_magic_casting);
	m_packet.u.magic.server_magic_casting.nMagic = nMagic;
	m_packet.u.magic.server_magic_casting.wReady = WORD(pCaster->CalcMagicCastReady(nMagic));
	m_packet.u.magic.server_magic_casting.idCaster = idCaster;
	::QueuePacket(connections, idCaster, &m_packet, 1);
	::CastMe2Other(idCaster, &m_packet);
}	//> CSD-TW-030606

void CBattleManager::RecvMagicExecute(WORD idCaster, t_client_magic_execute* pPacket)
{	//< CSD-TW-030606
	const int nMagic = pPacket->nMagic;
	// 화염화살이나 얼음화살인 경우는 물리적 공격으로 처리
	
	const WORD idTarget = pPacket->idTarget; // 마법 대상자 
	CHARLIST* pCaster = ::GetCharListPtr(idCaster);
	CHARLIST* pTarget = ::GetCharListPtr(idTarget);
	if (pCaster == NULL || pTarget == NULL)  return;  

	// finito 16/08/07 client has a delay of atleast 800 for executing magic so if an execute is sent earlier then it is not valid.
/*	if (timeGetTime() - pCaster->m_dwExecuteTime < 800 && pCaster->m_dwExecuteTime != 0) 
	{
		char info[256];
		sprintf_s(info, sizeof(info), "RecvMagicExeucte ExecuteTime: %d Magic: %d", timeGetTime() - pCaster->m_dwExecuteTime, nMagic);
		g_pLogManager->SaveTimeChecks(pCaster, info);
		return; 
	}
*/
	switch (nMagic)
	{
    case FIRE_ARROW:
    case ICE_ARROW: 
		// Set last execute info and reset resultcounter
		pCaster->m_nExecuteMagic = nMagic;
		pCaster->m_dwExecuteTime = timeGetTime();
		pCaster->m_cMagicResult = 0;
		return;
#ifdef LMS_MAPSERVER 
	case PERFECT_PROTECT:
	case PROTECT_RANGE_WEAPON:
		return;
	case PROTECTION:
	case SHIELD:
	case FIRE_RESIST:
	case ICE_RESIST:
	case ELECT_RESIST:
	case TWINKLE_STAR:
	case DRAGON_SCALE:
	case BLESS:
	case PROTECT_DANGER:
	case GREAT_SHIELD:
	case AUTHORITY_LETHY:
	case NULLITY_MAGIC:
	case CANCEL_DIVINE:
	case RECURRENCE_DIVINE:
	case AUTHORITY_OF_ATHYAS:
	case LIFE_UP:
	case DECIPHER:
	case HEAL_DISEASE:
	case GREAT_HEAL:
	case REMOVE_CURSE:
	case RESTORE_AUTHORITY:
	case MIRACLE_RECOVER:
	case RELIEF_AUTHORITY:
	case RECOVER_POISON:
	case AUTHORITY_RECOVER_SIGHT:
	case CURE_DISEASE:
	case AUTHORITY_SIMUNIAN:
	case HOLY_CURE:
	case SPEED_UP:
	case ENCHANT_WEAPON:
	case PRAY_POWER:
	case GREAT_PRAY:
	case MINOR_AMPLIFY:
	case MAGIC_AMPLIFY:
	case TRANSPARENCY:
	case LIGHT:
	case AUTHORITY_LUCK:
	case CURSE:
		{
			if (pCaster != pTarget)
			{
				return;
			}
		}
#endif
	}

	// 효과벽에 대한 처리
	const int nX = pPacket->nX;
	const int nY = pPacket->nY;
	
	if(nMagic != pCaster->m_nCastMagic)
	{
		//MyLog(LOG_NORMAL, "Last cast of %s (%d) NOT match execute result (%d)!", pCaster->Name, pCaster->m_nCastMagic, nMagic);
		char info[256];
		sprintf_s(info, sizeof(info), "Cast (%d) did NOT match execute result (%d)!", pCaster->m_nCastMagic, nMagic);
		g_pLogManager->SaveEtc(pCaster, info);
		pCaster->ClearMagic();
		//closeconnection_msg(connections, idCaster, -201, "How about, no?");
		return;
	}

	if (!IsValid(nMagic, nX>>5, nY>>5))
	{
		pCaster->Message(MK_WARNING, 1, 4);
		return;
	}
	
	// Set last execute info and reset resultcounter
	pCaster->m_nExecuteMagic = nMagic;
	pCaster->m_dwExecuteTime = timeGetTime();
	pCaster->m_cResultMismatch = 0;
	pCaster->m_cMagicResult = 0;

	bool bFail = false;
	// 석화에 걸려있으면 어떠한 마법도 적용을 받지 못함. 단 저주해제는 제외
	if (pTarget->IsStone() && pPacket->nMagic != REMOVE_CURSE)
	{		
		pCaster->Message(MK_NORMAL, 1, 18);
		pCaster->ClearMagic();
		bFail = true;
		goto MAGIC_FAIL;
	}
	// 서로 공격 가능여부 검사
	if (!IsBattle(pCaster, pTarget))
	{
		pCaster->ClearMagic();
		bFail = true;
		goto MAGIC_FAIL;
	}
	
	// 마법 사용이 가능한지 검사
	if (!pCaster->CheckDoing(nMagic, g_curr_time))
	{
		pCaster->ClearMagic();
		bFail = true;
		goto MAGIC_FAIL;
	}
	
	if (!FilterMagic(pCaster, pTarget, nX, nY))
	{
		pCaster->ClearMagic();
		bFail = true;
		goto MAGIC_FAIL;
	}
	
	pCaster->ClearMagic(false); // 마법 사용이 되었음을 설정
	// CSD-030806
	g_pLogManager->SaveMagicExecute(nMagic, pCaster, pTarget, nX, nY);
	
MAGIC_FAIL:
	memset(&m_packet, 0, sizeof(t_packet));
	m_packet.h.header.type = CMD_MAGIC_EXECUTE;
	m_packet.h.header.size = sizeof(t_server_magic_execute);
	m_packet.u.magic.server_magic_execute.nMagic = nMagic;
	m_packet.u.magic.server_magic_execute.idCaster = idCaster;
	m_packet.u.magic.server_magic_execute.idTarget = idTarget;
	m_packet.u.magic.server_magic_execute.nX = nX;
	m_packet.u.magic.server_magic_execute.nY = nY;
	m_packet.u.magic.server_magic_execute.bFailed = bFail;
	::QueuePacket(connections, idCaster, &m_packet, 1);
	::CastMe2Other(idCaster, &m_packet);
	//> CSD-TW-030606

	/* t_client_magic_result fakeresult;
	fakeresult.bWanted = false;
	fakeresult.nX = nX;
	fakeresult.nY = nY;
	fakeresult.nMagic = nMagic;
	fakeresult.idTarget = idTarget;

	RecvMagicResult(idCaster, &fakeresult); finito debug stuff */
}
void CBattleManager::RecvMagicResult(WORD idCaster, t_client_magic_result* pPacket)
{
	const WORD idTarget = pPacket->idTarget; // 마법을 사용한 대상자
	CHARLIST* pCaster = ::GetCharListPtr(idCaster);
	CHARLIST* pTarget = ::GetCharListPtr(idTarget);
	if (pCaster == NULL || pTarget == NULL)  return;  
	
	const int nMagic = pPacket->nMagic;
	if (nMagic == 0)  return;

#ifdef LMS_MAPSERVER 
	switch (nMagic)
	{
	case PERFECT_PROTECT:
	case PROTECT_RANGE_WEAPON:
		return;
	case PROTECTION:
	case SHIELD:
	case FIRE_RESIST:
	case ICE_RESIST:
	case ELECT_RESIST:
	case TWINKLE_STAR:
	case DRAGON_SCALE:
	case BLESS:
	case PROTECT_DANGER:
	case GREAT_SHIELD:
	case AUTHORITY_LETHY:
	case NULLITY_MAGIC:
	case CANCEL_DIVINE:
	case RECURRENCE_DIVINE:
	case AUTHORITY_OF_ATHYAS:
	case LIFE_UP:
	case DECIPHER:
	case HEAL_DISEASE:
	case GREAT_HEAL:
	case REMOVE_CURSE:
	case RESTORE_AUTHORITY:
	case MIRACLE_RECOVER:
	case RELIEF_AUTHORITY:
	case RECOVER_POISON:
	case AUTHORITY_RECOVER_SIGHT:
	case CURE_DISEASE:
	case AUTHORITY_SIMUNIAN:
	case HOLY_CURE:
	case SPEED_UP:
	case ENCHANT_WEAPON:
	case PRAY_POWER:
	case GREAT_PRAY:
	case MINOR_AMPLIFY:
	case MAGIC_AMPLIFY:
	case TRANSPARENCY:
	case LIGHT:
	case AUTHORITY_LUCK:
	case CURSE:
		{
			if (pCaster != pTarget)
			{
				return;
			}
		}
	}
#endif

	// 효과벽에 대한 처리
	const int nX = pPacket->nX;
	const int nY = pPacket->nY;
	
	/*
	char *msg = "You got owned newb!";
	t_packet p;
	p.h.header.size = sizeof( t_server_bbs ) - MAX_PATH + strlen(msg);
	p.h.header.type = CMD_BBS;
	p.h.header.size = sizeof(t_server_bbs) - MAX_PATH + p.h.header.size;
	strcpy(p.u.server_bbs.bbs, msg);
	//g_pUserManager->SendPacket(&p);
	QueuePacket(connections, idCaster, &p, 1);
	*/
	if(nMagic != pCaster->m_nExecuteMagic  && !( (nMagic == FIRE_WALL || nMagic == ICE_WALL) && global_time - pCaster->m_dwExecuteTime < 15000))
	{
		/*
		MyLog(LOG_NORMAL, "Last execute of %s (%d) NOT match received result (%d)!", pCaster->Name, pCaster->m_nExecuteMagic, nMagic);
		char info[256];
		sprintf(info, "Execute (%d) did NOT match received result (%d)!", pCaster->m_nExecuteMagic, nMagic);
		g_pLogManager->SaveEtc(pCaster, info);

		if(pCaster->m_cResultMismatch > 1)
		{
			closeconnection_msg(connections, idCaster, -201, "How about, no?");
		}
		pCaster->m_cResultMismatch++;
		*/
		return;
	}

	if(!pCaster->CheckMultiEffect(nMagic) && pCaster->m_cMagicResult >= 1 && !((nMagic == HEAL_DISEASE || nMagic == CURE_DISEASE) && pCaster->m_cMagicResult < 4))
	{
		//char info[256];
		//sprintf(info, "CheckMultiEffect failed! (%d)", nMagic);
		//g_pLogManager->SaveEtc(pCaster, info);
		//MyLog(LOG_NORMAL, "Spell (%d) ignored due to SpellMultiEffect returning false.", nMagic);
		return;
	}

	if(timeGetTime() - pCaster->m_dwExecuteTime > 15000 && !( (nMagic == FIRE_ARROW || nMagic == ICE_ARROW || nMagic == LIGHTNING_BOLT || nMagic == DECIPHER) && timeGetTime() - pCaster->m_dwExecuteTime < 20000))
	{
		//MyLog(LOG_NORMAL, "Last execute of %s was too long ago! (%dx)", pCaster->Name, pCaster->m_cMagicResult);
		char info[256];
		sprintf_s(info, sizeof(info), "Last execute of (%d) was too long ago (%d)!", nMagic, (timeGetTime() - pCaster->m_dwExecuteTime));
		g_pLogManager->SaveEtc(pCaster, info);

		//closeconnection_msg(connections, idCaster, -201, "How about, no?");
		return;
	}

	if (!IsValid(nMagic, nX>>5, nY>>5))
	{
		pCaster->Message(MK_WARNING, 1, 4);
		return;
	}
	// 석화에 걸려있으면 어떠한 마법도 적용을 받지 못함. 단 저주해제는 제외
	if (pTarget->IsStone() && nMagic != REMOVE_CURSE)
	{	
		pCaster->ClearMagic();
		return;
	}
	// 마법 적용이 가능한지 검사
	if (!pCaster->CheckEnd(nMagic, g_curr_time))
	{ 
		pCaster->ClearMagic();
		return;
	}  
	// 서로 공격 가능여부 검사
	if (!IsBattle(pCaster, pTarget))
	{
		pCaster->ClearMagic();
		return;
	}
	
	if (!FilterResult(pCaster, pTarget))
	{
		pCaster->ClearMagic();
		return;
	}
	
	pTarget->SetWanted(pPacket->bWanted);    // 현상범인지 여부 설정
	
	const int nBattle = Magic_Ref[nMagic].magic_Type/10;
	
	GetBattle(nBattle)->SetIndex(nMagic);
	GetBattle(nBattle)->SetBothID(idCaster, idTarget);
	GetBattle(nBattle)->SetBothPtr(pCaster, pTarget);
	GetBattle(nBattle)->SetCurrentTime(g_curr_time);
	GetBattle(nBattle)->SetContinueTime(0);
	GetBattle(nBattle)->SetPosition(nX>>5, nY>>5);
	GetBattle(nBattle)->Execute();

	//Increment counter
	pCaster->m_cMagicResult++;
}

void CBattleManager::RecvMagicResult(t_magic_result_d* pPacket)
{
	const WORD idCaster = pPacket->idCaster;
	const WORD idTarget = pPacket->idTarget; // 마법을 사용한 대상자
	CHARLIST* pCaster = ::GetCharListPtr(idCaster);
	CHARLIST* pTarget = ::GetCharListPtr(idTarget);
	if (pCaster == NULL || pTarget == NULL)  return;
	// 마법 결과 산출 여부 가능 검사
	if (!FilterResult(pPacket->nMagic, pCaster, pTarget))
	{ 
		return;
	}
	
	const BYTE nMagic = pPacket->nMagic; // 마법의 번호
	const int nX = pPacket->nX;          // 마법이 발생된 위치의 X좌표
	const int nY = pPacket->nY;          // 마법이 발생된 위치의 Y좌표  
	const int nBattle = Magic_Ref[nMagic].magic_Type/10;
	
	if (nBattle >= 1 && nBattle <= 6)
	{
		GetBattle(nBattle)->SetIndex(nMagic);
		GetBattle(nBattle)->SetBothID(idCaster, idTarget);
		GetBattle(nBattle)->SetBothPtr(pCaster, pTarget);
		GetBattle(nBattle)->SetCurrentTime(g_curr_time);
		GetBattle(nBattle)->SetContinueTime(0);
		GetBattle(nBattle)->SetPosition(nX>>5, nY>>5);
		GetBattle(nBattle)->Execute();
	}
}

void CBattleManager::RecvCombatReset(WORD idMaster)
{ // 전투스킬 포인터 재분배
	CHARLIST* pMaster = ::GetCharListPtr(idMaster);
	if (pMaster == NULL)  return;
	
	int nCount = pMaster->GetCombatPoint();
	
	for (int i = LIGHTNING_BOOM; i <= WIND_EXTREME; ++i)
	{
		nCount += pMaster->GetCombatLevel(i);
		pMaster->SetCombatLevel(i, 0);
	}
	
	pMaster->SetCombatPoint(nCount);
}

void CBattleManager::RecvCombatObtain(WORD idMaster, t_client_combat_obtain* pPacket)
{
	CHARLIST* pMaster = ::GetCharListPtr(idMaster);
	if (pMaster == NULL)  return;
	
	const int nPoint = pPacket->nPoint;
	const int nOld = pMaster->GetCombatPoint();
	// 제한치 검사
	if (pMaster->IsLimit(nPoint))  
	{
		pMaster->Message(MK_INFORMATION, 1, 58);
		return;
	}
	
	const int nNew = pMaster->GetCombatPoint();
	::SaveObtainCombat(pMaster, nOld, nNew);
	// Packet 전송
	memset(&m_packet, 0, sizeof(t_packet));
	m_packet.h.header.type = CMD_COMBAT_OBTAIN;
	m_packet.h.header.size = sizeof(t_server_combat_obtain);
	m_packet.u.combat.server_combat_obtain.idMaster = idMaster;
	m_packet.u.combat.server_combat_obtain.nPara = pPacket->nPara;
	m_packet.u.combat.server_combat_obtain.nPosX = pPacket->nPosX;
	m_packet.u.combat.server_combat_obtain.nPosY = pPacket->nPosY;
	m_packet.u.combat.server_combat_obtain.nTotal = pMaster->GetCombatPoint();
	::QueuePacket(connections, idMaster, &m_packet, 1);
}

void CBattleManager::RecvCombatRequest(WORD idMaster, t_client_combat_request* pPacket)
{  
	CHARLIST* pMaster = ::GetCharListPtr(idMaster);
	if (pMaster == NULL)  return;
	// 제한치를 넘었다면 초기화
	if (pMaster->IsLimit())  pMaster->InitCombat();
	// Packet 전송
	memset(&m_packet, 0, sizeof(t_packet));
	m_packet.h.header.type = CMD_COMBAT_REQUEST;
	m_packet.h.header.size = sizeof(t_server_combat_request);
	m_packet.u.combat.server_combat_request.idMaster = idMaster;
	m_packet.u.combat.server_combat_request.nRemain = pMaster->GetCombatPoint();
	
	for (int i = 0; i < 36; ++i)
	{
		m_packet.u.combat.server_combat_request.aLevel[i] = pMaster->GetCombatLevel(i + LIGHTNING_BOOM);
	}
	
	::QueuePacket(connections, idMaster, &m_packet, 1);
}

void CBattleManager::RecvCombatLearn(WORD idMaster, t_client_combat_learn* pPacket)
{  
	CHARLIST* pMaster = ::GetCharListPtr(idMaster);
	if (pMaster == NULL)  return;
	
	const int nCombat = pPacket->nSkill;
	
	if (!IsEnableClass(nCombat, pMaster))  return;
	// 제한치를 넘었다면 초기화
	if (pMaster->IsLimit())  
	{
		pMaster->InitCombat();
		return;
	}
	
	switch (nCombat)
	{
    case LIGHTNING_EXTREME:
    case EARTH_EXTREME:
    case FIRE_EXTREME:
    case DARK_EXTREME:
    case ICE_EXTREME:
    case WIND_EXTREME:
		{
			if (!pMaster->IsDual())
			{
				pMaster->Message(MK_SHORTAGE, 1, 54);
				return;
			}
			
			break;
		}
	}
	// 전투스킬 습득
	const int nLevel = pMaster->GetCombatLevel(nCombat) + 1;
	
	if (!IsValid(nCombat, nLevel, pMaster))
	{
		pMaster->Message(MK_SHORTAGE, 1, 54);
		return;
	}
	//< CSD-020909
	const int nPoint = pPacket->nPoint;
	const int nOld = pMaster->GetCombatLevel(nCombat);
	
	if (!pMaster->LevelUpCombat(nCombat, nPoint))
	{
		pMaster->Message(MK_SHORTAGE, 1, 54);
		return;
	}
	
	const int nNew = pMaster->GetCombatLevel(nCombat);
	::SaveChangeCombat(pMaster, nCombat, nOld, nNew);
	//> CSD-020909
	
	// Packet 전송
	memset(&m_packet, 0, sizeof(t_packet));
	m_packet.h.header.type = CMD_COMBAT_LEARN;
	m_packet.h.header.size = sizeof(t_server_combat_learn);
	m_packet.u.combat.server_combat_learn.idMaster = idMaster;
	m_packet.u.combat.server_combat_learn.nSkill = nCombat;
	m_packet.u.combat.server_combat_learn.nLevel = pMaster->GetCombatLevel(nCombat);
	m_packet.u.combat.server_combat_learn.nRemain = pMaster->GetCombatPoint();
	::QueuePacket(connections, idMaster, &m_packet, 1);
}

void CBattleManager::RecvCombatClear(WORD idMaster, t_client_combat_clear* pPacket)
{	//< CSD-TW-030606
	CHARLIST* pMaster = ::GetCharListPtr(idMaster);
	
	if (pMaster == NULL)
	{
		return;
	}

	pMaster->InitActiveCombat();
	pMaster->ClearActiveState();
	pMaster->ClearPassiveState();
	pMaster->ClearRecoveryState();
	
	memset(&m_packet, 0, sizeof(t_packet));
	m_packet.h.header.type = CMD_COMBAT_CLEAR;
	m_packet.h.header.size = sizeof(t_server_combat_clear);
	m_packet.u.combat.server_combat_clear.idMaster = idMaster;
	m_packet.u.combat.server_combat_clear.nType = 0;
	::QueuePacket(connections, idMaster, &m_packet, 1);
	::CastMe2Other(idMaster, &m_packet);
}	//> CSD-TW-030606

void CBattleManager::RecvCombatSelect(WORD idCaster, t_client_combat_select* pPacket)
{   //< CSD-TW-030623
	CHARLIST* pCaster = ::GetCharListPtr(idCaster);
	if (pCaster == NULL)  return;
	// 서로 공격 가능여부 검사
	if (!IsBattle())                 return;
	if (!IsBattle(pCaster))          return;
	// 사용가능한 전투스킬인지 검사
	const int nCombat = pPacket->nCombat;
	if (!IsValid(nCombat, pCaster))  return;
	// cp회복계열 마법이 발동중일때는 아무것도 할 수 없음 // 030430 kyo
	if (pCaster->IsRecoveryCombatState())
	{	
		pCaster->Message(MK_INFORMATION, 1, 43);
		return;
	}

	switch (Magic_Ref[nCombat].Spell_Type)
	{
	case ACTIVE_COMBAT:
		{	// Passive 전투스킬 상태에서 다른 계열의 전투스킬을 선택한 경우
			const int nPassive = pCaster->GetPassiveCombat();
			
			if (nPassive > 0)
			{				
				if (Magic_Ref[nCombat].order_Type%10 != Magic_Ref[nPassive].order_Type%10)
				{ 
					pCaster->ClearPassiveState();
					pCaster->Message(MK_INFORMATION, 1, 90);
				}
			}
			
			break;
		}
	case PASSIVE_COMBAT:
		{	// Active 속성 전투스킬 상태에서 다른 계열의 전투스킬을 선택한 경우
			const int nActive = pCaster->GetActiveCombat();
			
			if (nActive > 0)
			{
				if (Magic_Ref[nCombat].order_Type%10 != Magic_Ref[nActive].order_Type%10)
				{
					pCaster->Message(MK_INFORMATION, 1, 90);
					return;
				}
			}

			break;
		}
	case RECOVERY_COMBAT: // 030415 kyo
		{	// RECOVERY_COMBAT타입 
			const int nRecovery = pCaster->GetRecoveryCombat();

			if (nRecovery > 0)
			{	// cp회복 타입은 active속성을 초기화 시킨다.
				pCaster->InitActiveCombat();
				pCaster->ClearActiveState();

				if (Magic_Ref[nCombat].order_Type%10 != Magic_Ref[nRecovery].order_Type%10)
				{
					pCaster->Message(MK_INFORMATION, 1, 90);
					return;
				}
			}

			break;
		}
	}
	
	//Var
	m_nLastCombatSelect = nCombat;
	pCaster->ClearCombat(nCombat);

	// Packet 전송
	memset(&m_packet, 0, sizeof(t_packet));
	m_packet.h.header.type = CMD_COMBAT_SELECT;
	m_packet.h.header.size = sizeof(t_server_combat_select);
	m_packet.u.combat.server_combat_select.nCombat = nCombat;
	m_packet.u.combat.server_combat_select.idMaster = idCaster;
	::QueuePacket(connections, idCaster, &m_packet, 1);
	::CastMe2Other(idCaster, &m_packet);
}   //> CSD-TW-030623

void CBattleManager::RecvCombatAttack(WORD idCaster, t_client_combat_attack* pPacket)
{	//< CSD-TW-030606
	const int nCombat = pPacket->nSkill;
	const int nX = pPacket->nX;
	const int nY = pPacket->nY;
	const WORD idTarget = pPacket->idTarget; // 마법 대상자
	CHARLIST* pCaster = ::GetCharListPtr(idCaster);
	CHARLIST* pTarget = ::GetCharListPtr(idTarget);
	if (pCaster == NULL || pTarget == NULL)  return;
	
	bool bFail = false;
	// 석화에 걸려있으면 어떠한 전투스킬도 적용을 받지 못함
	if (pTarget->IsStone())
	{		
		pCaster->Message(MK_NORMAL, 1, 18);
		bFail = true;
		goto COMBAT_FAIL;
	}
	// 서로 공격 가능여부 검사
	if (!IsBattle())                        return;
	if (!IsBattle(pCaster))                 return;
	if (!IsBattle(pCaster, pTarget))        return;
	// 사용가능한 전투스킬인지 검사
	if (!IsValid(nCombat, pCaster))         return;
	if (!IsEnableClass(nCombat, pCaster))   return;
	if (!IsEnableWeapon(nCombat, pCaster))  return;
	// 전투스킬 사용 여부 가능 검사
	if (!FilterCombat(nCombat, pCaster, pTarget, nX, nY))
	{ 
		bFail = true;
		goto COMBAT_FAIL;
	}

	if (!pCaster->IsEnableCombat(nCombat))
	{
		pCaster->Message(MK_SHORTAGE, 1, 57);
		return;
	}
	// 스킬 사용가능수 재설정
	switch (nCombat)
	{
    case BLOOD_EARTH:
    case TWISTER:
    case POISONING_NOVA:
    case WHILWIND:
		{
			if (pCaster->IsCount())
			{
				ChangeCombat(nCombat, idCaster);
			}
			else
			{
				ResetCombat(nCombat, idCaster);
			}
			
			break;
		}
    default:
		{
			ResetCombat(nCombat, idCaster);
			break;
		}
	}
	// CSD-030806
	g_pLogManager->SaveCombatExecute(nCombat, pCaster, pTarget, nX, nY);
	
	//MyLog(LOG_NORMAL, "Reset %d", m_cCombatResult);
	m_cCombatResult = 0;

COMBAT_FAIL:
	memset(&m_packet, 0, sizeof(t_packet));
	m_packet.h.header.type = CMD_COMBAT_ATTACK;
	m_packet.h.header.size = sizeof(t_server_combat_attack);
	m_packet.u.combat.server_combat_attack.nSkill = nCombat;
	m_packet.u.combat.server_combat_attack.nLevel = pCaster->GetCombatLevel(nCombat);
	m_packet.u.combat.server_combat_attack.idCaster = idCaster;
	m_packet.u.combat.server_combat_attack.idTarget = idTarget;
	m_packet.u.combat.server_combat_attack.wAmount = pCaster->Cp;
	m_packet.u.combat.server_combat_attack.nX = nX;
	m_packet.u.combat.server_combat_attack.nY = nY;
	m_packet.u.combat.server_combat_attack.bFailed = bFail;
	::QueuePacket(connections, idCaster, &m_packet, 1);
	::CastMe2Other(idCaster, &m_packet);
}	//> CSD-TW-030606

void CBattleManager::RecvCombatResult(WORD idCaster, t_client_combat_result* pPacket)
{ //
	const BYTE nCombat = pPacket->nSkill; // 전투스킬 번호
	const WORD idTarget = pPacket->idTarget;
	CHARLIST* pCaster = ::GetCharListPtr(idCaster);
	CHARLIST* pTarget = ::GetCharListPtr(idTarget);
	if (pCaster == NULL || pTarget == NULL)  return;

	if(g_curr_time - m_dwLastCombatResult >= 4 || g_curr_time - m_dwLastCombatResult < 0) { m_cCombatResultAvg = 0; m_dwLastCombatResult = g_curr_time; }

	if((pPacket->nSkill != LIGHTNING_EXTREME && (m_cCombatResult > 120 || m_cCombatResultAvg > 80)))
	{
		//MyLog(LOG_NORMAL, "Hacker! %d %d", m_cThrowResult, m_cThrowResultAvg);
		char info[256];
		sprintf_s(info, sizeof(info), "RecvCombatResult: m_cCombatResult (%d) m_cCombatResultAvg (%d) nSkill (%d)!", m_cCombatResult, m_cCombatResultAvg, pPacket->nSkill);
		g_pLogManager->SaveEtc(pCaster, info);
		
		//closeconnection_msg(connections, idCaster, -200, "How about, no?");
		return;
	}
	else if(m_cCombatResult > 40) { m_cCombatResult++; return; }

	// 서로 공격 가능여부 검사
	if (!IsBattle())                               return;
	if (!IsBattle(pCaster))                        return;
	if (!IsBattle(pCaster, pTarget))               return;
	if (!FilterCombat(nCombat, pCaster, pTarget))  return;
	pTarget->SetWanted(pPacket->bWanted); // 현상범인지 여부 설정
	
	const int nType = Magic_Ref[nCombat].order_Type;
	const int nBattle = (nType/10) + (nType%10);
	const int nX = pPacket->nX;           // 전투스킬이 발생된 위치의 X좌표
	const int nY = pPacket->nY;           // 전투스킬이 발생된 위치의 Y좌표  
	
	GetBattle(nBattle)->SetIndex(nCombat);
	GetBattle(nBattle)->SetBothID(idCaster, idTarget);
	GetBattle(nBattle)->SetBothPtr(pCaster, pTarget);
	GetBattle(nBattle)->SetCurrentTime(g_curr_time);
	GetBattle(nBattle)->SetContinueTime(0);
	GetBattle(nBattle)->SetPosition(nX>>5, nY>>5);
	GetBattle(nBattle)->Execute();

	m_cCombatResult++;
	m_cCombatResultAvg++;
}

///////////////////////////////////////////////////////////////////////////////
// Protected Method
///////////////////////////////////////////////////////////////////////////////

void CBattleManager::AutoCurse(WORD idCaster, WORD idTarget)
{ //
	CHARLIST* pCaster = ::GetCharListPtr(idCaster);
	CHARLIST* pTarget = ::GetCharListPtr(idTarget);
	if (pCaster == NULL || pTarget == NULL)  return;
	if (rand()%101 >= 5)                     return;
	
	int nMagic = 0, nContinue = 0;
	// 저주계열의 레어 아이템
	const int nStiff = RareEM.GetStaticRareStiff(pCaster->StaticRare);       // 석화  
	const int nSnag = RareEM.GetStaticRareSnag(pCaster->StaticRare);         // 절대마비 
	const int nVertigly = RareEM.GetStaticRareVertigly(pCaster->StaticRare); // 혼란
	const int nPoison = RareEM.GetStaticRarePoison(pCaster->StaticRare);     // 중독
	const int nSlack = RareEM.GetStaticRareSlack(pCaster->StaticRare);       // 속도저하
	
	if (nStiff > 0)
	{
		nMagic = STONELY;            
		nContinue = nStiff;
	}
	else if (nSnag > 0)
	{
		nMagic = ABSOLUTE_PARALYSIS; 
		nContinue = nSnag;
	}
	else if (nVertigly > 0)
	{
		nMagic = CONFUSION;          
		nContinue = nVertigly;
	}
	else if (nPoison > 0)
	{
		nMagic = INTOXICATION;       
		nContinue = nPoison;
	}
	else if (nSlack > 0)
	{
		nMagic = SLOW;               
		nContinue = nStiff;
	}
	
	if (nMagic > 0)
	{
		const BYTE nType = Magic_Ref[nMagic].magic_Type;
		// 마법을 적용할 수 없는 경우
		if (!IsApply(nType, pCaster, pTarget))
		{ 
			return;
		}
		// 마법이 실패한 경우라면
		if (IsMissMagic(nType, pCaster, pTarget))
		{
			return;
		}
		
		const int nBattle = Magic_Ref[nMagic].magic_Type/10;
		GetBattle(nBattle)->SetIndex(nMagic);
		GetBattle(nBattle)->SetBothID(idCaster, idTarget);
		GetBattle(nBattle)->SetBothPtr(pCaster, pTarget);
		GetBattle(nBattle)->SetCurrentTime(g_curr_time);
		GetBattle(nBattle)->SetContinueTime(nContinue);
		GetBattle(nBattle)->Execute();
	}
}

bool CBattleManager::FilterNpcAttack(CHARLIST* pCaster, CHARLIST* pTarget)
{	//< CSD-030723
	// 만약 이동에 공격한거라면 제자리에 멈춤(NPC에게만 적용)
	pCaster->MoveLength = pCaster->MovePathCount = 0;
	// 버그몹에 대한 임시 방편
	if (!IsBugMon(pCaster, pTarget))  return false;
	// 공격이 불가한 경우라면
	if (!::CanBattleArea(pCaster, pTarget))  return false;
	// 소환몬은 인간 NPC를 죽일 수 없음
	if (pCaster->IsTamedNpc() && pTarget->IsNpc())
	{	
		switch (pTarget->Race)
		{
		case HUMAN:  
			{
				return false;
			}
		case SEALSTONE:
			{
				return IsBreak(pCaster, pTarget);
			}
		}
	}	
	
	return (IsMissAttack(pCaster, pTarget)) ? false:true;
}	//> CSD-030723

bool CBattleManager::FilterAttack(CHARLIST* pCaster, CHARLIST* pTarget)
{ 
	// 버그몹에 대한 임시 방편
	if (!IsBugMon(pCaster, pTarget))  return false;
	// 공격이 불가한 경우라면
	if (!::CanBattleArea(pCaster, pTarget))  
	{
		pCaster->Message(MK_WARNING, 1, 4);
		return false;
	}
	// 극악 NK라면 물리적 공격 불가
	if (pCaster->IsUltraNK(MapInfo[MapNumber].nation))
	{ 
		::OutMessage(pCaster, 1, 18);
		return false;
	}
	// NPC인 경우라면
	if (pTarget->IsNpc())
	{ 
		switch (pTarget->Race)
		{
		case HUMAN:
			{ //< CSD-021019
				t_client_event data;
				data.type = EVENT_TYPE_NPC;
				data.event_no = pTarget->GetServerID() + 10000;
				RecvEvent( &data, pCaster->GetServerID());
				return false;
			} //> CSD-021019
		case SEALSTONE: 
			{
				return IsBreak(pCaster, pTarget);
			}
		}
	}  
	
	if (IsColleague(pCaster, pTarget))
	{ 
		return false;
	}
	// 1 : 고 -> 저, 2 : 저 -> 고, 3 : 같은 높이
	const int nX = pTarget->MoveSx; 
	const int nY = pTarget->MoveSy;
	const BYTE nRiseFall = pCaster->GetRiseFall(nX, nY);
	
	switch (pCaster->GetTacticsKind())
	{
    case TACTICS_Archery: // 활쏘기
	case TACTICS_Hurl:    // 비검 던지기
		{ // 장거리 무기
			return true;
		}
    case TACTICS_Whirl:
    case TACTICS_Pierce:
    case TACTICS_Magery:
    case TACTICS_Orison:
		{ // 중거리 무기
			return (nRiseFall == 3) ? true:false; // 높이가 다르다면 실패
		}
    default: 
		{ // 단거리 무기
			return (nRiseFall == 3) ? true:false; // 높이가 다르다면 실패
		}
	}
	
	return true;
}

bool CBattleManager::FilterThrow(CHARLIST* pCaster, CHARLIST* pTarget)
{
	if (pTarget->IsNpc())
	{ // NPC인 경우라면
		switch (pTarget->Race)
		{
		case HUMAN:     return false;
		case SEALSTONE: return IsBreak(pCaster, pTarget);
		}
	}

	// 카운셀러인 경우라면
	if (!pCaster->IsCounselor())
	{
		if (pTarget->IsCounselor() && pTarget->name_status.santa_power)  return false;
		if (pTarget->IsReporter())  return false; //Finito
	}
	if (pTarget->accessory[0] == 114)  return false;
	// 버그몬에 대한 임시 방편
	if (!IsBugMon(pCaster, pTarget))  return false;
	// 동료인 경우라면
	if (IsColleague(pCaster, pTarget))
	{ 
		return false;
	}
	
	return true;
}

bool CBattleManager::FilterNpcMagic(BYTE nMagic, CHARLIST* pCaster, CHARLIST* pTarget, int nX, int nY)
{	//< CSD-030723
	if (pTarget->IsNpc())
	{	// NPC인 경우라면
		switch (pTarget->Race)
		{
		case HUMAN:
			{
				return false;
			}
		case SEALSTONE: 
			{
				return IsBreak(pCaster, pTarget);
			}
		}
	}
	// 마법 시전이 불가한 상태나 능력이나 마나가 부족한 상태인 경우
	if (!IsEnable(nMagic, pCaster))
	{
		return false;
	}
	
	const BYTE nType = Magic_Ref[nMagic].magic_Type;
	// 마법을 적용할 수 없는 경우
	if (!IsApply(nType, pCaster, pTarget))
	{ 
		return false;
	}
	// 마법이 실패한 경우라면
	if (IsMissMagic(nType, pCaster, pTarget))
	{ 
		return false;
	}
	
	return true;
}	//> CSD-030723

bool CBattleManager::FilterStart(CHARLIST* pCaster)
{ //
	// 살리기 마법 사용 불가 검사
	const BYTE nMagic = pCaster->GetMagic();
	if (IsValid(nMagic, pCaster) == false)                   return false;
	if (nMagic == RELIEF_AUTHORITY && CheckEventWarDoing())  return false;
	
	const int nX = pCaster->MoveSx;
	const int nY = pCaster->MoveSy;
	// 마법 사용 불가 지역인 경우  															 
	if (!IsPlaceMagic(nMagic, nX, nY))
	{
		pCaster->Message(MK_WARNING, 1, 4);
		return false;
	}
		
	const BYTE nType = Magic_Ref[nMagic].magic_Type;

	if (nType == MapInfo[MapNumber].nDisableMagicType)
	{	//< CSD-030516
		pCaster->Message(MK_WARNING, 1, 44);
		return false;
	}	//> CSD-030516
	// 마법을 사용할수 없게 해도 Counselor는 마법 사용 가능
	if (g_block_magic && !pCaster->IsCounselor())
	{
		pCaster->Message(MK_WARNING, 1, 4);
		return false;
	}  
	// 극악 NK라면 마법 사용이 불가
	if (pCaster->IsUltraNK(MapInfo[MapNumber].nation)) 
	{ 
		pCaster->Message(MK_WARNING, 1, 5);
		return false;
	}
	// 마법 시전이 불가한 상태나 능력이나 마나가 부족한 상태인 경우
	if (!IsAbility(nMagic, pCaster) || !IsEnable(nMagic, pCaster))
	{
		return false;
	}
	return true;
}

bool CBattleManager::FilterMagic(CHARLIST* pCaster, CHARLIST* pTarget, int nX, int nY)
{	// 마법 사용이 가능한지 검사 
	const BYTE nMagic = pCaster->GetMagic(); // 마법의 번호
	if (!IsValid(nMagic, pCaster))    return false;
	if (!IsAbility(nMagic, pCaster))  return false;
	// NPC인 경우라면 이로운 마법을 사용한 경우 마법 적용 불가
	if (pTarget->IsNpc())
	{ 
		if (!IsHarmfulMagic(nMagic))
		{
			pCaster->Message(MK_NORMAL, 1, 17);
			return false;
		}
	}
	// 마법 사용 유효거리 검사
	if (Magic_Ref[nMagic].avail_Range != 0)
	{
		if (pCaster->GetDistance(nX, nY) > Magic_Ref[nMagic].avail_Range)
		{
			pCaster->Message(MK_NORMAL, 1, 19);
			return false;
		}
	}
	// 1 : 고 -> 저, 2 : 저 -> 고, 3 : 같은 높이
	const BYTE nRiseFall = pCaster->GetRiseFall(nX>>5, nY>>5);
	
	switch (Magic_Ref[nMagic].nRiseFall)
	{
    case 1: 
		{ // 같은 높이만 공격이 가능한 경우
			if (nRiseFall == 1 || nRiseFall == 2)  
			{ 
				pCaster->Message(MK_NORMAL, 1, 20);
				return false;
			}
			
			break;
		}
    case 2: 
		{ // 고->저와 같은 높이만 공격이 가능한 경우
			if (nRiseFall == 2)  
			{
				pCaster->Message(MK_NORMAL, 1, 20);
				return false;
			}
			
			break;
		}
    case 3: 
		{ // 저->고와 같은 높이만 공격이 가능한 경우
			if (nRiseFall == 1)  
			{
				pCaster->Message(MK_NORMAL, 1, 20);
				return false;
			}
			
			break;
		}
	}
	
	const BYTE nType = Magic_Ref[nMagic].magic_Type;
	// 마법을 적용할 수 없는 경우
	if (!IsApply(nType, pCaster, pTarget))
	{ 
		return false;
	}
	// 마법이 실패한 경우라면
	if (IsMissMagic(nType, pCaster, pTarget))
	{ 
		return false;
	}
	
	pCaster->DecMana(Magic_Ref[nMagic].exhaust_MP);
	return true;
}

bool CBattleManager::FilterCombat(BYTE nCombat, CHARLIST* pCaster, CHARLIST* pTarget, int nX, int nY)
{	//< CSD-TW-030606
	if (pCaster->IsRecoveryCombatState())
	{	// cp회복계열 마법이 발동중일때는 아무것도 할 수 없음 // 030430 kyo			
		pCaster->Message(MK_INFORMATION, 1, 43);
		return false;
	}
	// 공격이 불가한 경우라면
	if (!::CanBattleArea(pCaster, pTarget))
	{
		pCaster->Message(MK_WARNING, 1, 4);
		return false;
	}
	// 극악 NK라면 마법 사용이 불가
	if (pCaster->IsUltraNK(MapInfo[MapNumber].nation)) 
	{ 
		pCaster->Message(MK_WARNING, 1, 5);
		return false;
	}
	// 전투스킬 사용 유효거리 검사
	if (Magic_Ref[nCombat].avail_Range != 0)
	{
		switch (nCombat)
		{
		case CHARGING:
			{
				const int nTemp = pCaster->GetDistance(nX, nY);
				
				if (pCaster->GetDistance(nX, nY) > pCaster->GetCombatValue(nCombat))
				{
					pCaster->Message(MK_NORMAL, 1, 72);
					return false;
				} 
				
				break;
			}
		default:
			{
				if (pCaster->GetDistance(nX, nY) > Magic_Ref[nCombat].avail_Range)
				{
					pCaster->Message(MK_NORMAL, 1, 72);
					return false;
				} 
				
				break;
			}
		}
	}
	// 1 : 고 -> 저, 2 : 저 -> 고, 3 : 같은 높이
	const BYTE nRiseFall = pCaster->GetRiseFall(nX>>5, nY>>5);
	
	switch (Magic_Ref[nCombat].nRiseFall)
	{
    case 1: 
		{ // 같은 높이만 공격이 가능한 경우
			if (nRiseFall == 1 || nRiseFall == 2)  
			{ 
				//pCaster->Message(MK_NORMAL, 1, 20);
				return false;
			}
			
			break;
		}
    case 2: 
		{ // 고->저와 같은 높이만 공격이 가능한 경우
			if (nRiseFall == 2)  
			{
				//pCaster->Message(MK_NORMAL, 1, 20);
				return false;
			}
			
			break;
		}
    case 3: 
		{ // 저->고와 같은 높이만 공격이 가능한 경우
			if (nRiseFall == 1)  
			{
				//pCaster->Message(MK_NORMAL, 1, 20);
				return false;
			}
			
			break;
		}
	}  
	
	const BYTE nType = Magic_Ref[nCombat].magic_Type;
	// 마법을 적용할 수 없는 경우
	if (!IsApply(nType, pCaster, pTarget))
	{ 
		return false;
	}
	// 마법이 실패한 경우라면
	if (IsMissMagic(nType, pCaster, pTarget))
	{ 
		return false;
	}
	
	return true;
}	//> CSD-TW-030606

bool CBattleManager::FilterCombat(BYTE nCombat, CHARLIST* pCaster, CHARLIST* pTarget)
{	//< CSD-TW-030606
	// 공격이 불가한 경우라면
	if (!::CanBattleArea(pCaster, pTarget))
	{
		pCaster->Message(MK_WARNING, 1, 4);
		return false;
	}
	// 전투스킬 상태 검사
	switch (Magic_Ref[nCombat].Spell_Type)
	{
    case ACTIVE_COMBAT: 
		{
			if (pCaster->GetActiveCombat() != nCombat)  
			{
				return false;
			}
			
			break;
		}
    case PASSIVE_COMBAT:
		{
			if (pCaster->GetPassiveCombat() != nCombat)  
			{
				return false;
			}
			
			break;
		}
	case RECOVERY_COMBAT: // 030415 kyo
		{
			if (pCaster->GetRecoveryCombat() != nCombat)  
			{
				return false;
			}
			break;
		}
    default:  
		{
			return false;
		}
	}
    
	const BYTE nType = Magic_Ref[nCombat].magic_Type;
	// 마법을 적용할 수 없는 경우
	if (!IsApply(nType, pCaster, pTarget))
	{ 
		return false;
	}
	// 마법이 실패한 경우라면
	if (IsMissMagic(nType, pCaster, pTarget))
	{ 
		return false;
	}
	// NPC인 경우라면
	if (pTarget->IsNpc())
	{ 
		switch (pTarget->Race)
		{
		case HUMAN:      return false;
		case SEALSTONE:  return IsBreak(pCaster, pTarget);
		}
		// 위의 경우를 제외하고는 NPC는 이로운 마법 적용이 불가능  
		if (!IsHarmfulMagic(nCombat))  return false;
	} 
	// 카운셀러인 경우라면
	if (!pCaster->IsCounselor())
	{
		if (pTarget->IsCounselor() && pTarget->name_status.santa_power)     return false;
		if (pTarget->IsReporter())  return false; //Finito
	}
	if (pTarget->accessory[0] == 114)       return false;
	// 버그몬에 대한 임시 방편
	if (!IsBugMon(pCaster, pTarget))        return false;
	
	return (IsHarmfulMagic(nCombat) && IsColleague(pCaster, pTarget)) ? false:true;
}	//> CSD-TW-030606

bool CBattleManager::FilterResult(CHARLIST* pCaster, CHARLIST* pTarget)
{	//< CSD-TW-030627
	const BYTE nMagic = pCaster->GetMagic();         // 마법의 번호
	if (!IsValid(nMagic, pCaster))          return false;
	const BYTE nType = Magic_Ref[nMagic].magic_Type; // 마법의 타입
	if (!IsApply(nType, pCaster, pTarget))  return false;
	// 마법이 실패한 경우라면
	if (IsHarmfulMagic(nMagic) && IsMissMagic(nType, pCaster, pTarget))
	{ 
		return false;
	}
	// 투명상태에서 마법 적용시 투명 상태가 해제
	if (pTarget->dwTransparency > 0)  
	{
		::CheckTransparency(pTarget, true);
		pTarget->Message(MK_NORMAL, 1, 28);
	}
	// NPC인 경우라면
	if (pTarget->IsNpc())
	{ 
		switch (pTarget->Race)
		{
		case HUMAN:      return false;
		case GUARDSTONE: return true; // 완치의 기적이 가능하도록 함
		}
		// 위의 경우를 제외하고는 NPC는 이로운 마법 적용이 불가능  
		if (!IsHarmfulMagic(nMagic))  
		{
			return false;
		}
		
		switch (pTarget->Race)
		{
		case SEALSTONE:  return IsBreak(pCaster, pTarget);
		}
	} 
	// 카운셀러인 경우라면
	if (!pCaster->IsCounselor())
	{
		if (pTarget->IsCounselor() && pTarget->name_status.santa_power)  return false;
		if (pTarget->IsReporter())  return false; //Finito
	}

	if (pTarget->accessory[0] == 114)  return false;
	// 버그몬에 대한 임시 방편
	if (!IsBugMon(pCaster, pTarget) && nMagic != RELIEF_AUTHORITY)  return false;
	
	return (IsHarmfulMagic(nMagic) && IsColleague(pCaster, pTarget)) ? false:true;
}	//> CSD-TW-030627

bool CBattleManager::FilterResult(BYTE nMagic, CHARLIST* pCaster, CHARLIST* pTarget)
{ //
	//if (IsValid(nMagic, pCaster) == false)           return false;
	if (pTarget->IsNpc() && IsHarmfulMagic(nMagic))  return false;
	// 버그몬에 대한 임시 방편
	return (IsBugMon(pCaster, pTarget) || nMagic == RELIEF_AUTHORITY) ? true:false;
}

///////////////////////////////////////////////////////////////////////////////
// Private Method
///////////////////////////////////////////////////////////////////////////////

bool CBattleManager::IsBattle() const
{ // 현재 상태에서 전투가 가능한지 여부 검사
	if (g_block_magic)   return false;
	if (g_block_attack)  return false;
	return true;
}

bool CBattleManager::IsBattle(CHARLIST* pCaster) const
{ // 현재 상태에서 전투가 가능한지 여부 검사
	if (::CheckEventAttack())  
	{ // 백병전인 경우
		pCaster->Message(MK_NORMAL, 0, 346);
		return false;
	}
	// 020428 YGI acer
	if( pCaster->Hungry <= 0 ) 
	{
		pCaster->Message(MK_NORMAL, 2, 5);
		return false;
	}
	// 석화상태이면 어떠한 전투스킬도 사용하지 못함
	if (pCaster->IsStone())
	{	//< CSD-021011	
		pCaster->Message(MK_NORMAL, 1, 91);
		return false;
	} //> CSD-021011
	
	return true;
}

bool CBattleManager::IsBattle(CHARLIST* pCaster, CHARLIST* pTarget) const
{ // 서로 전투가 가능한지 여부 검사

	if (pTarget->ChairNum)// LTS DRAGON MODIFY
	{
		if (pTarget->patterntype==NPC_PATTERN_BOSS_WAIT)
		{
			pTarget->patterntype=NPC_PATTERN_BOSS_ACCESS;
		}
	}
	
	if (pCaster->IsPlayer() && pTarget->IsPlayer())
	{ 
		if (pCaster != pTarget) 
		{  
			if (g_LocalWarBegin && g_isLocalWarServer)
			{ // 국지전은 경우
				if (pCaster->JoinLocalWar && pTarget->JoinLocalWar)
				{ // 둘다 참가자인 경우
					return true;
				}
				
				if (!pCaster->JoinLocalWar && !pTarget->JoinLocalWar)
				{ // 둘다 비참가자인 경우
					return true;
				}

				return false;
			}
			/*
				Check if the 2 chars are duelling
			*/
			
			if(pCaster->fight_flag)
			{
				if(CheckServerId(pCaster->fight_id) == pTarget && CheckServerId(pTarget->fight_id) == pCaster)
					return true;
			}
			
		}
	}
	
	return true;
}

bool CBattleManager::IsValid(BYTE nMagic, int nX, int nY) const
{ //< 020522 : 효과벽인 경우 처리
	if (nX < 0)  return false;
	if (nY < 0)  return false; 
	if (nX >= g_lpMapFile->wWidth)   return false;
	if (nY >= g_lpMapFile->wHeight)  return false; 
	
	switch (Magic_Ref[nMagic].magic_Type/10)
	{
    case 4: // 저주계열의 마법인 경우
    case 5: // 공격계열의 마법인 경우
		{
			if (TileMap[nX][nY].attr_light || TileMap[nX][nY].attr_no_battle) 
			{
				return false;
			}
			
			break;
		}
	}
	
	return true;
} //> 020522

bool CBattleManager::IsValid(BYTE nKind, CHARLIST* pCaster) const
{ //
	if (!IsEnableClass(nKind, pCaster))
	{
		return false;
	}
	// 마법인 경우
	if (IsMagic(nKind))
	{
		switch (pCaster->Spell)
		{
		case WIZARD_SPELL: // 마법사라면  
			{
				if (pCaster->Ws[nKind])  return true;
				break;
			}
		case PRIEST_SPELL: // 성직자라면
			{
				if (pCaster->Ps[nKind - 150])  return true;
				break;
			}
		}
		
		return false;
	}
	// 전투스킬인 경우
	if (IsCombat(nKind))
	{
		const int nLevel = pCaster->GetLevel(); // CSD-030806
		
		switch (pCaster->GetCombatLevel(nKind))
		{
		case 1: return (nLevel >= Magic_Ref[nKind].basic_Level) ? true:false;
		case 2: return (nLevel >= Magic_Ref[nKind].before_Magic) ? true:false;
		case 3: return (nLevel >= Magic_Ref[nKind].basic_magery) ? true:false;
		}
		
		return false;
	}
	
	return false;
}

bool CBattleManager::IsValid(BYTE nKind, BYTE nStep, CHARLIST* pCaster) const
{ // 전투스킬인 경우
	if (IsCombat(nKind))
	{
		const int nLevel = pCaster->GetLevel(); // CSD-030806
		
		switch (nStep)
		{
		case 1: return (nLevel >= Magic_Ref[nKind].basic_Level) ? true:false;
		case 2: return (nLevel >= Magic_Ref[nKind].before_Magic) ? true:false;
		case 3: return (nLevel >= Magic_Ref[nKind].basic_magery) ? true:false;
		}
		return false;
	}
	
	return false;
}

bool CBattleManager::IsAbility(BYTE nMagic, CHARLIST* pCaster) const
{
	if (pCaster->GetAbility(INT_) < Magic_Ref[nMagic].require_IT)	
	{
		pCaster->Message(MK_SHORTAGE, 1, 6);
		return false;
	}
	
	if (pCaster->GetAbility(WIS) < Magic_Ref[nMagic].require_WP)
	{
		pCaster->Message(MK_SHORTAGE, 1, 6);
		return false;
	}
	
	if (pCaster->GetAbility(WSPS) < Magic_Ref[nMagic].point_WS)
	{
		pCaster->Message(MK_SHORTAGE, 1, 6);
		return false;
	}
	
	switch (pCaster->Spell)
	{
    case WIZARD_SPELL: // 마법사라면
		{
			if (pCaster->Skill[TACTICS_Magery] + 1 < Magic_Ref[nMagic].basic_magery)
			{
				pCaster->Message(MK_SHORTAGE, 1, 7);
				return false;
			}
			
			if (pCaster->Mana < Magic_Ref[nMagic].exhaust_MP)	
			{
				pCaster->Message(MK_SHORTAGE, 1, 9);
				return false;
			}
			
			break;
		}
    case PRIEST_SPELL: // 성직자라면
		{
			if (pCaster->Skill[TACTICS_Orison] + 1 < Magic_Ref[nMagic].basic_magery)
			{
				pCaster->Message(MK_SHORTAGE, 1, 8);
				return false;
			}
			
			if (pCaster->Mana < Magic_Ref[nMagic].exhaust_MP)	
			{
				pCaster->Message(MK_SHORTAGE, 1, 10);
				return false;
			}
			
			break;
		}
	}
	
	return true;
}

bool CBattleManager::IsEnable(BYTE nMagic, CHARLIST* pCaster) const
{	//< CSD-TW-030606 : 시전자 마법 사용 가능 여부 판단
	const BYTE nType = Magic_Ref[nMagic].magic_Type;
	// 보호계열 마법 중 완전한 결계인 경우
	if (pCaster->IsPerfect() || pCaster->IsDivineUp())
	{
		switch (nType/10)
		{
		case 4: 
			{ // 저주계열의 마법
				pCaster->Message(MK_WARNING, 1, 11);
				return false; 
			}
		case 5: 
			{ // 공격계열의 마법
				pCaster->Message(MK_WARNING, 1, 12);
				return false; 
			}		
		}
	}
	/*
	if (nType == 64)
	{	// 기도일때 안됨
		pCaster->Message(MK_WARNING, 1, 42);
		return false; 
	}
	*/
	// 저주 1계열 마법 상태나 캐스팅 불가한 경우
	if (pCaster->dwCurse1 > 0 || !pCaster->IsCasting())
	{
		pCaster->Message(MK_WARNING, 1, 13);
		return false;
	}

	// 듀얼 아이템레벨에 맞지 않는 마법 사용불가 // 031110 kyo
	if( Magic_Ref[nMagic].nDualStep > g_CSymbolMgr.GetSymbolGrade(pCaster) )	
	{
		pCaster->Message(MK_WARNING, 1, 45);
		return false;
	}
	
	return true;
}	//> CSD-TW-030606

bool CBattleManager::IsHarmfulMagic(BYTE nMagic) const
{
	switch (Magic_Ref[nMagic].magic_Type/10)
	{
    case 4:              // 저주계열의 마법인 경우
    case 5: return true; // 공격계열의 마법인 경우
	}
	
	return false;
}

bool CBattleManager::IsPlaceMagic(BYTE nMagic, int nX, int nY) const
{	// 이로운 마법을 사용할 수 있는 지역인지 검사
	if (nX < 0)  return false;
	if (nY < 0)  return false; 
	if (nX >= g_lpMapFile->wWidth)   return false;
	if (nY >= g_lpMapFile->wHeight)  return false;
	if (IsHarmfulMagic(nMagic) && TileMap[nX][nY].attr_no_battle)
	{
		return false;
	}
	
	return true;
}

bool CBattleManager::IsApply(BYTE nType, CHARLIST* pCaster, CHARLIST* pTarget) const
{	// 대상자 마법 적용 가능 여부 판단
	// 특별한 NPC는 저주 계열 마법 적용 불가
	if (pTarget->IsNpc() && nType/10 == 4)
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
	
	switch (nType)
	{ 
		// 보호계열 마법인 경우
    case 21: 
		{ // 일반보호계열 마법
			if (pTarget->dwNormalProtect > 0)
			{
				pCaster->Message(MK_WARNING, 1, 21);
				return false;
			}
			
			break;  
		}
    case 22: 
		{ // 특수보호계열 마법
			if (pTarget->dwSpecialProtect > 0)
			{
				pCaster->Message(MK_WARNING, 1, 21);
				return false;
			}
			
			break;
		}
    case 23: 
		{	// 예외보호계열 마법
			if (pTarget->dwSpecialProtect > 0 ||
				pTarget->dwExceptProtect > 0) // 내가 기도나 완결일땐 완결 안됨
			{
				pCaster->Message(MK_WARNING, 1, 21);
				return false;
			}
			
			break;
		}
		// 보조계열 마법인 경우
    case 31: 
		{ // 이동 속도
			if (pTarget->speedUp.IsContinue() == true)
			{
				pCaster->Message(MK_WARNING, 1, 22);
				return false;
			}
			
			break;
		}
    case 32: 
		{ // 물리적 공격 데미지
			if (pTarget->apShort.IsContinue() && 
				pTarget->apMiddle.IsContinue() && 
				pTarget->apLong.IsContinue())
			{
				pCaster->Message(MK_WARNING, 1, 22);
				return false;
			}
			
			break;
		}
    case 33: 
		{ // 공격 마법 데미지
			if (pTarget->amplify.IsContinue() == true)
			{
				pCaster->Message(MK_WARNING, 1, 22);
				return false;
			}
			
			break;
		}
    case 34: 
		{ // 투명
			if (pTarget->dwTransparency > 0)
			{
				pCaster->Message(MK_WARNING, 1, 22);
				return false;
			}
			
			break;
		}
    case 35: 
		{ // 빛
			if (pTarget->dwLight > 0)
			{
				pCaster->Message(MK_WARNING, 1, 22);
				return false;
			}
			
			break;
		}
    case 36: 
		{ // 행운
			if (pTarget->dwDontMiss > 0)
			{
				pCaster->Message(MK_WARNING, 1, 22);
				return false;
			}
			
			break;
		}
		// 저주계열 마법인 경우
    case 41: 
		{ // 저주 1계열 마법
			if (pTarget->dwCurse1 > 0)
			{
				pCaster->Message(MK_WARNING, 1, 23);
				return false;
			}
			
			break;
		}
    case 42:
		{ //< CSD-021024 : 저주 2계열 마법 
			if (pTarget->dwCurse2 > 0)
			{
				pCaster->Message(MK_WARNING, 1, 23);
				return false;
			}
			
			break;
		} //> CSD-021024
    case 43: 
		{ // 저주 3계열 마법
			if (pTarget->dwCurse3 > 0)
			{
				pCaster->Message(MK_WARNING, 1, 23);
				return false;
			}
			
			break;
		}
    case 44: 
		{ // 저주 4계열 마법
			if (pTarget->dwCurse4 > 0)
			{
				pCaster->Message(MK_WARNING, 1, 23);
				return false;
			}
			
			break;
		} 
    case 46:
		{ //< CSD-021024 : 얼림 저주 마법
			if (pTarget->IsFreeze())
			{
				pCaster->Message(MK_WARNING, 1, 23);
				return false;
			}
			
			break;
		} //> CSD-021024
  }
  
  return true;
}

bool CBattleManager::IsMissAttack(CHARLIST* pCaster, CHARLIST* pTarget) const
{ // 물리적 공격 성공 여부 판단
	const int nDefence = RareEM.GetStaticRareSmart(pTarget->StaticRare);
	// 20을 100으로 환산하여 계산
	const int nRate = pCaster->CalcAttackSuccessRate(nDefence); 
	return (nRate < rand()%101) ? true:false;
}

bool CBattleManager::IsMissMagic(BYTE nType, CHARLIST* pCaster, CHARLIST* pTarget) const
{	//< CSD-021024 : 마법 실패 여부 판단
	if (pTarget->IsPerfect())
	{ 
		pCaster->Message(MK_WARNING, 1, 24);
		return true;
	}
	// 해당 저주 계열의 마법 방어력이 있는지 검사
	if (pTarget->IsCurseDefense(nType))
	{
		pCaster->Message(MK_WARNING, 1, 25);
		return true;
	}
	// 저주계열 마법 회피 여부 검사
	if (pTarget->IsCurseAvoid(nType))
	{ 
		pCaster->Message(MK_WARNING, 1, 37);
		return true;
	} 
	
	switch (nType)
	{
    case 41:
    case 42:
    case 43:
    case 44:
    case 45:
		{
			if (RareEM.GetDynamicRareValue(FITEM_INVALID_CURSE, pTarget->DynamicRare))
			{ 
				return true;
			}			
			
			break;
		}
	}
	
	return false;
} //> CSD-021024

bool CBattleManager::IsBugMon(CHARLIST* pCaster, CHARLIST* pTarget) const
{
	if (pCaster->Hp <= 0)  return false;
	if (pTarget->Hp <= 0)  return false;
	return (pTarget->Hp > pTarget->HpMax) ? false:true;
}

bool CBattleManager::IsBreak(CHARLIST* pCaster, CHARLIST* pTarget) const
{
	if (::ReturnSealStoneBreakable() != CMD_STEALSTONE_BREAKABLE)
	{ // 깰 수 있는 상태가 아니라면 
		return false;
	}
	
	if (::CheckLocalWarAttacker(pCaster))  
	{ // 국지전 참여자가 아니라면
		return false;
	}
    
	switch (pTarget->SprNo)
	{
    case 98: return (pCaster->name_status.nation == 3) ? false:true; // 바이서스 
    case 99: return (pCaster->name_status.nation == 4) ? false:true; // 자이펀
    case 91: return (pCaster->name_status.nation == 6) ? false:true; // 일스
	}
	
	return false;
}

extern int g_NoPkMode;//acer7
bool CBattleManager::IsColleague(CHARLIST* pCaster, CHARLIST* pTarget) const
{	//< CSD-031013
	if(pCaster->IsPlayer() && pTarget->IsNpc() && !g_PKRestriction.CanBattle(pCaster)) //Eleval
		return true;

	if (!pCaster->IsPlayer() || !(pTarget->IsPlayer() || pTarget->IsTamedNpc()))
	{
		return false;
	}
	// 자기자신인 경우
	if (pCaster->GetServerID() == pTarget->GetServerID())
	{
		return true;
	}
	// 아레나 경기 중인 경우
	if (g_pArenaManager->IsColossusArena())
	{
		if (pCaster->IsJoinArenaGame() && pTarget->IsJoinArenaGame())
		{
#ifdef LMS_MAPSERVER
			CBaseArena* pGame = pCaster->GetJoinArenaGame();
			if (pGame != NULL)
			{
				if (pGame->GetArenaType() != AT_LMS)
				{
#endif
					if (pCaster->IsJoinArenaTeam() && pTarget->IsJoinArenaTeam())
					{
						if (pCaster->GetJoinArenaTeam() == pTarget->GetJoinArenaTeam())
						{
							return true;
						}
					}
#ifdef LMS_MAPSERVER
				}
			}
#endif
		}
	}
	// 국지전인 경우
	if (g_LocalWarBegin && g_isLocalWarServer)
	{
		if (pCaster->JoinLocalWar && pTarget->JoinLocalWar)
		{	// 모두 국지전에 참여한 경우라면
			if (pCaster->name_status.nation == pTarget->name_status.nation)
			{
				return true;
			}
			
			return false;
		}
	}
	// 국가전인 경우
	if (::isNationWarfieldServer() && ::IsWar())
	{
		if (::isAttacker(pCaster) && ::isAttacker(pTarget))
		{ // 둘다 공격자인 경우
			return true;
		}
		
		if (!::isAttacker(pCaster) && !::isAttacker(pTarget))
		{ // 둘다 방어자인 경우
			return true;
		}
		
		return false;
	}
	
	if (isNewWarfieldServer() && isMyTeam(pCaster,pTarget))
	{
		return true;
	}
	
	if (g_NoPkMode == 1)
	{
		return true;
	}
	// if (MapInfo[MapNumber].forrookie)
	/*
		Check if the 2 chars are duelling
	*/
	
	if(pCaster->fight_flag)
	{
		if(CheckServerId(pCaster->fight_id) == pTarget && CheckServerId(pTarget->fight_id) == pCaster)
			return false;
	}
	{
		// Scholium에서는 무조건 이 사람을 칠수 없음
		if (MapNumber == 30 || MapNumber == 85 || (!g_PKRestriction.CanPK(pCaster)))//020905 lsw //Eleval 22/07/09 - Added command to disable/enable pk
		{ 
			return true;
		}
	}
	// 대상자가 파티원인 경우
#ifndef LMS_MAPSERVER
	for (int i = 0; i < 6; ++i)
	{
		if (pCaster->party[i].On && !strcmp(pCaster->party[i].Name, pTarget->Name))
		{ 
			return true;
		}
	}
#endif
	
	return (::IsColleagueWhenColossus(pCaster, pTarget)) ? true:false;
}	//> CSD-031013

bool CBattleManager::IsEnableClass(BYTE nCombat, CHARLIST* pCaster) const
{	// 클래스에 따른 전투스킬 습득 여부 검사
	bitset<MAX_CLASS> bsClass(Magic_Ref[nCombat].nClass);
	// 클래스 검사  
	const int nClass = pCaster->Class;
	
	if (nClass < WARRIOR || nClass > PRIEST)
	{
		pCaster->Message(MK_INFORMATION, 1, 73);
		return false;
	}
	// 습득 여부 검사
	if (!bsClass[nClass])
	{
		switch (nClass)
		{
		case WARRIOR: pCaster->Message(MK_INFORMATION, 1, 74); break;
		case ARCHER:  pCaster->Message(MK_INFORMATION, 1, 75); break;
		case THIEF:   pCaster->Message(MK_INFORMATION, 1, 76); break;
		case WIZARD:  pCaster->Message(MK_INFORMATION, 1, 77); break;
		case PRIEST:  pCaster->Message(MK_INFORMATION, 1, 78); break;
		}
		
		return false;
	}
	
	return true;
}

bool CBattleManager::IsEnableWeapon(BYTE nCombat, CHARLIST* pCaster) const
{ // 무기택틱에 따른 전투스킬 습득 여부 검사
	bitset<ALL_WEAPON> bsWeapon(Magic_Ref[nCombat].nTactics);
	// 무기택틱 검사
	const int nTactics = pCaster->GetTacticsKind() - TACTICS_Crapple;
	
	if (nTactics < CRAPPLE || nTactics > DOUBLE_MACEFIGHTING)
	{
		pCaster->Message(MK_INFORMATION, 1, 79);
		return false;
	}
	// 습득 여부 검사
	if (!bsWeapon[nTactics])
	{
		switch (nTactics)
		{
		case CRAPPLE:             pCaster->Message(MK_INFORMATION, 1, 80); break;
		case SWORDMANSHIP:        pCaster->Message(MK_INFORMATION, 1, 81); break;
		case ARCHERY:             pCaster->Message(MK_INFORMATION, 1, 82); break;
		case FENCING:             pCaster->Message(MK_INFORMATION, 1, 83); break;
		case MACE_FIGHTING:       pCaster->Message(MK_INFORMATION, 1, 84); break;
		case PIERCE:              pCaster->Message(MK_INFORMATION, 1, 85); break;
		case HURL:                pCaster->Message(MK_INFORMATION, 1, 86); break;
		case DOUBLE_SWORDMANSHIP: pCaster->Message(MK_INFORMATION, 1, 87); break;
		case DOUBLE_MACEFIGHTING: pCaster->Message(MK_INFORMATION, 1, 88); break;
		}
		
		return false;
	}
	
	return true;
}

bool CBattleManager::IsActiveCombat(BYTE nCombat) const
{
	return (Magic_Ref[nCombat].Spell_Type == ACTIVE_COMBAT) ? true:false;
}

bool CBattleManager::IsPassiveCombat(BYTE nCombat) const
{
	return (Magic_Ref[nCombat].Spell_Type == PASSIVE_COMBAT) ? true:false;
}

bool CBattleManager::IsRecoveryCombat(BYTE nCombat) const // 030415 kyo
{
	return (Magic_Ref[nCombat].Spell_Type == RECOVERY_COMBAT) ? true:false;
}