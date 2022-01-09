// CharInfo.cpp: implementation of the CCharInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "CharInfo.h"

#include "CItem.h"
#include "Ability.h"
#include "LTSSupport.h"
#include "ItemMgr.h"//020110 LSW
#include "RareEffectMgr.h"
#include "LocalizingMgr.h"
#include "DualManager.h"

extern int NPC_MaleName_Count;
extern cLocalWarfield* g_pLocalWarfield;
extern NPCName_by_Gender NPC_Name_Ref[Num_Of_NPC_Name];

int MAX_EXP_LEVEL = 150;  // CSD-040204
int MAX_TAC_LEVEL = 199;  // CSD-040204
int MAX_DUMMY_LEVEL = 60; // Finito

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

int CStatePoint::Increase(int nPoint, int nPercent = 100) 
{	// 증가
	const int nOldPoint = m_nCurPoint;
	m_nCurPoint += nPoint; 

	const int nMaxPoint = m_nMaxPoint*nPercent/100;

	if (m_nCurPoint > nMaxPoint)
	{
		m_nCurPoint = nMaxPoint;
	}

	return m_nCurPoint - nOldPoint;
}

int CStatePoint::Decrease(int nPoint, int nPercent = 0) 
{	// 감소
	const int nOldPoint = m_nCurPoint;
	m_nCurPoint -= nPoint; 

	const int nMaxPoint = m_nMaxPoint*nPercent/100;

	if (m_nCurPoint < nMaxPoint)
	{
		m_nCurPoint = nMaxPoint;
	}

	return nOldPoint - m_nCurPoint;
}

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CCharInfo::CCharInfo()
{
	m_idServer = 0; // CSD-HK-030829
	m_nLevel = 0; // CSD-030806
	m_nGuildCode = 0; // CSD-030806
	m_nCriticalCount = 0; // CSD-031007
	m_nItemEffectCount = 0; // CSD-031007
	m_nDualCls = 0;
	m_nClsStep = 0;
	m_nExpStep = 0;
	m_bEventRecv = false;	// BBD 040308
	
	for (int i = 0; i < NO_TAC; ++i)
	{
		m_aTacStep[i] = 0;
	}
	
	m_gnAbility.Clear(); // 올릴 수 있는 어빌러티 포인터 초기화
	m_gnCombat.Clear();  // 올릴 수 있는 전투스킬 포인터 초기화
	m_pArenaGame = NULL; // CSD-030509
	m_pArenaTeam = NULL; // CSD-030509
	m_dwLastMoveTime = 0;
	m_bIsDuelAllow = true;

	s_quest_floor = 0; //Eleval 10/08/09 - To bring ppl back in quest
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CCharInfo::InitCombat()
{
	SetCombatPoint(0);
	
	for (int i = LIGHTNING_BOOM; i <= WIND_EXTREME; ++i)
	{
		Skill[i - 100] = 0;
	}
}

void CCharInfo::IncAbility(BYTE nType)
{
	CAbilityLimit* pLimit = g_mgrLimit.GetCell(Class, m_nClsStep);
	
	switch (nType)
	{
	case STR:	if (pLimit->GetStr() <= Str)     return; ++Str;   break;
	case CON:	if (pLimit->GetCon() <= Con)     return; ++Con;   break;
    case DEX:	if (pLimit->GetDex() <= Dex)     return; ++Dex;   break;
    case WIS:	if (pLimit->GetWis() <= Wis)     return; ++Wis;   break;
	case INT_: if (pLimit->GetInt() <= Int)     return; ++Int;   break;
	case MOVP: if (pLimit->GetMove() <= MoveP)  return; ++MoveP; break;
	case CHA:	 if (pLimit->GetChar() <= Char)    return; ++Char;  break;
	case ENDU: if (pLimit->GetEndu() <= Endu)   return; ++Endu;  break;
    case MOR:  if (pLimit->GetMor() <= Moral)   return; ++Moral; break;
	case LUCK: if (pLimit->GetLuck() <= Luck)   return; ++Luck;  break;
	case WSPS: 
		{
			switch (Spell)
			{
			case WIZARD_SPELL: if (pLimit->GetWs() <= wsps)  return; ++wsps;  break;
			case PRIEST_SPELL: if (pLimit->GetPs() <= wsps)  return; ++wsps;  break;
			}
		}
	}
	
	m_gnAbility.Decrease();
}

void CCharInfo::DecAbility(BYTE nType)
{
	switch (nType)
	{
	case STR:	 if (Str > 10)    return; --Str;   break;
	case CON:	 if (Con > 10)    return; --Con;   break;
    case DEX:	 if (Dex > 10)    return; --Dex;   break;
    case WIS:	 if (Wis > 10)    return; --Wis;   break;
	case INT_: if (Int > 10)    return; --Int;   break;
	case MOVP: if (MoveP > 10)  return; --MoveP; break;
	case CHA:	 if (Char > 10)   return; --Char;  break;
	case ENDU: if (Endu > 10)   return; --Endu;  break;
    case MOR:  if (Moral > 10)  return; --Moral; break;
	case LUCK: if (Luck > 10)   return; --Luck;  break;
	case WSPS: 
		{
			switch (Spell)
			{
			case WIZARD_SPELL: if (wsps > 10)  return; --wsps;  break;
			case PRIEST_SPELL: if (wsps > 10)  return; --wsps;  break;
			}
		}
	}
	
	m_gnAbility.Increase();
}

void CCharInfo::ResetAbility(BYTE nCombat)
{
	// 최대 Life량 계산
	const int nMaxHp = CalcNewLife(nCombat);
	
	if (HpMax != nMaxHp)
	{
		HpMax = nMaxHp;
		// Player인 경우 값이 변경되면 Client에 전송
		if (IsPlayer())  SendCharInfoBasic(MAX_HP, HpMax);
	}
	// 최대 Mana량 계산
	const int nMaxMp = CalcNewMana(nCombat);
	
	if (ManaMax != nMaxMp)
	{
		ManaMax = nMaxMp;
		// Player인 경우 값이 변경되면 Client에 전송
		if (IsPlayer())  SendCharInfoBasic(MAX_MP, ManaMax);
	}
	// 최대 Hungry량 계산
	const int nMaxSp = CalcNewHungry(nCombat);
	
	if (HungryMax != nMaxSp)
	{
		HungryMax = nMaxSp;
		// Player인 경우 값이 변경되면 Client에 전송
		if (IsPlayer())  SendCharInfoBasic(MAX_SP, HungryMax);
	}
	// 최대 전투스킬 사용가능량 계산
	const int nMaxCp = CalcNewCombat();
	
	if (CpMax != nMaxCp)
	{
		CpMax = nMaxCp;
		// Player인 경우 값이 변경되면 Client에 전송
		if (IsPlayer())  SendCharInfoBasic(MAX_CP, CpMax);
	}
	// 변화된 최대 Life에 따른 Life 재설정
	if (Hp > HpMax)  
	{
		Hp = HpMax;
		// Player인 경우 값이 변경되면 Client에 전송
		if (IsPlayer())  SendCharInfoBasic(HP, Hp);
	}
	// 변화된 최대 Mana에 따른 Mana 재설정
	if (Mana > ManaMax)  
	{
		Mana = ManaMax;
		// Player인 경우 값이 변경되면 Client에 전송
		if (IsPlayer())  SendCharInfoBasic(MP, Mana);
	}
	// 변화된 최대 Hungry에 따른 Hungry 재설정
	if (Hungry > HungryMax)	 
	{
		Hungry = HungryMax;
		// Player인 경우 값이 변경되면 Client에 전송
		if (IsPlayer())  SendCharInfoBasic(SP, Hungry); 
	}
	// 변화된 최대 전투스킬 사용가능량에 따른 Cp 재설정
	if (Cp > CpMax)
	{
		Cp = CpMax;
		// Player인 경우 값이 변경되면 Client에 전송
		if (IsPlayer())  SendCharInfoBasic(CP, Cp);
	}
	// AC 계산
	Ac = CalcNewAc();
}

void CCharInfo::DivideAbility(BYTE nNext)
{	// 듀얼 어빌리티 재분배
	CAbilityDivide* pDivide = g_mgrDivide.GetCell(Class, nNext);
	// 듀얼 어빌리티 분배
	Str += pDivide->GetStr();
	Con += pDivide->GetCon();
	Dex += pDivide->GetDex();
	Wis += pDivide->GetWis();
	Int += pDivide->GetInt();
	MoveP += pDivide->GetMove();
	Char += pDivide->GetChar();
	Endu += pDivide->GetEndu();
	Moral += pDivide->GetMor();
	Luck += pDivide->GetLuck();
	
	switch (Spell)
	{
    case WIZARD_SPELL: wsps += pDivide->GetWs(); break; 
    case PRIEST_SPELL: wsps += pDivide->GetPs(); break;
	}
	// 듀얼 클래스 단계 상승
	++m_nClsStep; 
	// Reserved Point 계산
	int nPoint = GetRemainStr();
	nPoint += GetRemainCon();
	nPoint += GetRemainDex();
	nPoint += GetRemainWis();
	nPoint += GetRemainInt();
	nPoint += GetRemainMove();
	nPoint += GetRemainChar();
	nPoint += GetRemainEndu();
	nPoint += GetRemainMor();
	nPoint += GetRemainLuck();
	
	switch (Spell)
	{
    case WIZARD_SPELL: nPoint += GetRemainWs(); break; 
    case PRIEST_SPELL: nPoint += GetRemainPs(); break;
	}
	// Reserved Point 추가
	IncReservedPoint(nPoint);
	// 어빌리티 재설정
	Str -= GetRemainStr();
	Con -= GetRemainCon();
	Dex -= GetRemainDex();
	Wis -= GetRemainWis();
	Int -= GetRemainInt();
	MoveP -= GetRemainMove();
	Char -= GetRemainChar();
	Endu -= GetRemainEndu();
	Moral -= GetRemainMor();
	Luck -= GetRemainLuck();
	
	switch (Spell)
	{
    case WIZARD_SPELL: wsps -= GetRemainWs(); break; 
    case PRIEST_SPELL: wsps -= GetRemainPs(); break;
	}
}

void CCharInfo::ConvertTacticsLevel(BYTE nKind)
{	//< CSD-030314
	for (int i = 1 ; i <= MAX_TAC_LEVEL; ++i)
	{
		if (m_aTacStep[nKind] == NPC_Lev_Ref[i].nStep && tac_skillEXP[nKind] <= NPC_Lev_Ref[i].nMaxExp)
		{
			Skill[TACTICS_Crapple + nKind] = i;
			return;
		}
	}
	
	Skill[TACTICS_Crapple + nKind] = MAX_TAC_LEVEL;
}	//> CSD-030314

void CCharInfo::Message(BYTE nType, BYTE nKind, WORD nNumber)
{
	t_packet packet;
	packet.h.header.type = CMD_BATTLE_MESSAMGE;
	packet.h.header.size = sizeof(t_battle_message);
	packet.u.battle.battle_message.nType = nType;
	packet.u.battle.battle_message.nKind = nKind;
	packet.u.battle.battle_message.nNumber = nNumber;
	::QueuePacket(connections, GetServerID(), &packet, 1);
}

bool CCharInfo::IncExperience(int nExp)
{	//< CSD-030930
	const int nLevel = GetLevel();

	if (nExp <= 0)
	{
		return false;
	}

	Exp += nExp;

	if (Exp > NPC_Lev_Ref[nLevel].nMaxExp)
	{
		Exp = NPC_Lev_Ref[nLevel].nMaxExp + 1;
	}

	SendCharInfoBasic(EXP, Exp);
	return true;
}	//> CSD-030930
	
bool CCharInfo::DecExperience(bool bNK)
{	//< CSD-030930
	const int nLevel = GetLevel();
	const float fReduce = (bNK) ? NPC_Lev_Ref[nLevel].fNKReduction:
	                              NPC_Lev_Ref[nLevel].fExpReduction;
	const int nMinus = (NPC_Lev_Ref[nLevel].nNeedExp/100)*fReduce;
		
	if (Exp < nMinus)
	{
		Exp = 0;
		SendCharInfoBasic(EXP, Exp);
		return false;
	}

	Exp -= nMinus;
	SendCharInfoBasic(EXP, Exp);
	return true;
}	//> CSD-030930

bool CCharInfo::IncTacticExp(BYTE nKind, int nExp)
{   //< CSD-030930
	const int nLevel = Skill[TACTICS_Crapple + nKind];
	const int nPlus = nExp*NPC_Lev_Ref[nLevel].nTacRate/100;

	if (nPlus <=0)
	{
		return false;
	}

	tac_skillEXP[nKind] += nPlus;

	if (tac_skillEXP[nKind] > NPC_Lev_Ref[nLevel].nMaxExp)
	{
		tac_skillEXP[nKind] = NPC_Lev_Ref[nLevel].nMaxExp + 1;
	}
	
	SendCharInfoTactic(nKind);
	return true;
}   //> CSD-030930

bool CCharInfo::DecTacticExp(BYTE nKind, bool bNK)
{   //< CSD-030930
	int nLevel = Skill[TACTICS_Crapple + nKind];
	const float fReduce = (bNK) ? NPC_Lev_Ref[nLevel].fNKReduction:
	                              NPC_Lev_Ref[nLevel].fExpReduction;
	const int nMinus = (NPC_Lev_Ref[nLevel].nNeedExp/100)*fReduce;
		
	if (tac_skillEXP[nKind] < nMinus)
	{
		Skill[TACTICS_Crapple + nKind] = --nLevel;
		tac_skillEXP[nKind] = NPC_Lev_Ref[nLevel].nMaxExp + tac_skillEXP[nKind] - nMinus;
		SetTacticsStep(nKind, NPC_Lev_Ref[nLevel].nStep);
		SendCharInfoTactic(nKind);
		//tac_skillEXP[nKind] = 0;
		return false;
	}

	tac_skillEXP[nKind] -= nMinus;
	SendCharInfoTactic(nKind);
	return true;
}   //> CSD-030930

int CCharInfo::GetAbility(BYTE nType) const
{
	CAbilityLimit* pLimit = g_mgrLimit.GetCell(Class, m_nClsStep);
	
	switch (nType)
	{
	case STR:	 return __min(pLimit->GetStr(), GetExtStr());
	case CON:	 return __min(pLimit->GetCon(), GetExtCon());
	case DEX:	 return __min(pLimit->GetDex(), GetExtDex());
	case WIS:	 return __min(pLimit->GetWis(), GetExtWis());
	case INT_: return __min(pLimit->GetInt(), GetExtInt());
	case MOVP: return __min(pLimit->GetMove(), GetExtMove());
	case CHA:	 return __min(pLimit->GetChar(), GetExtChar());
	case ENDU: return __min(pLimit->GetEndu(), GetExtEndu());
	case MOR:	 return __min(pLimit->GetMor(), GetExtMor());
	case LUCK: return __min(pLimit->GetLuck(), GetExtLuck());
	case WSPS: 
		{
			switch (Spell)
			{
			case WIZARD_SPELL: return __min(pLimit->GetWs(), GetExtWs());
			case PRIEST_SPELL: return __min(pLimit->GetPs(), GetExtPs());
			}
		}
	}
	
	return 0;
}

int CCharInfo::GetWeaponSkill()
{	// 기술숙련도 구하기
	const int nIndex = GetTacticsKind();
	
	if (nIndex >= TACTICS_Crapple && nIndex <= TACTICS_Orison)
	{
		cur_tac_skill = nIndex - TACTICS_Crapple;
	}
	else
	{
		cur_tac_skill = 0;
	}
	
	return Skill[nIndex];
}

int CCharInfo::GetNpcAttack() const
{	//< CSD-031007 : NPC 물리적 공격력
	int nRandom = 0;
	
	if (NPC_Gen_Ref[npc_index].nStrikeRandom != 0)
	{
		if (++m_nCriticalCount > 5 && NPC_Gen_Ref[npc_index].nCriticalRate < rand()%101)
		{
			nRandom += NPC_Gen_Ref[npc_index].nStrikeDamage*NPC_Gen_Ref[npc_index].nCriticalDamage/100;
			m_nCriticalCount = 0;
		}
		
		nRandom += rand()%NPC_Gen_Ref[npc_index].nStrikeRandom;
	}
	
	return NPC_Gen_Ref[npc_index].nStrikeDamage + nRandom;
}	//> CSD-031007

int CCharInfo::GetNpcDefence() const
{	//< CSD-021019 : NPC 물리적 방어력
	return NPC_Gen_Ref[npc_index].nPhysical;
}	//> CSD-021019

int CCharInfo::GetNpcMagic() const
{	//< CSD-020822 : NPC 마법 공격력
	int nRandom = 0;
	
	if (NPC_Gen_Ref[npc_index].nMagicRandom != 0)
	{
		nRandom = rand()%NPC_Gen_Ref[npc_index].nMagicRandom;
	}
	
	return (NPC_Gen_Ref[npc_index].nMagicDamage) + nRandom;
}	//> CSD-020822

int CCharInfo::GetNpcDefence(BYTE nType) const
{	//< CSD-021019 : NPC 마법 방어력
	int nResist = 0; // 몬스터의 기본 마법저항력
	
	switch (nType)
	{    
    case RESIST_FIRE: 
		{ // 불계열 마법에 대한 저항력
			nResist += NPC_Gen_Ref[npc_index].nFire;  
			break;
		}
    case RESIST_ICE: 
		{ // 얼음계열 마법에 대한 저항력
			nResist += NPC_Gen_Ref[npc_index].nIce;   
			break;
		}
    case RESIST_ELECT: 
		{ // 전격계열 마법에 대한 저항력
			nResist += NPC_Gen_Ref[npc_index].nElect; 
			break;
		}
    case RESIST_HOLY: 
		{ // 신력계열 마법에 대한 저항력
			nResist += NPC_Gen_Ref[npc_index].nHoly;  
			break;
		}
	}
	
	return nResist;
}	//> CSD-021019

int CCharInfo::GetNpcStrikePiercing() const
{	//< CSD-021024
	return NPC_Gen_Ref[npc_index].nPiercingStrike;
}	//> CSD-021024

int CCharInfo::GetNpcMagicPiercing() const
{	//< CSD-021024
	return NPC_Gen_Ref[npc_index].nPiercingMagic;
}	//> CSD-021024

int CCharInfo::GetNpcStunAvoid() const
{	//< CSD-021024
	return NPC_Gen_Ref[npc_index].nStun;
}	//> CSD-021024

int CCharInfo::GetNpcSlowAvoid() const
{	//< CSD-021024
	return NPC_Gen_Ref[npc_index].nSlow;
}	//> CSD-021024

int CCharInfo::GetNpcPoisonAvoid() const
{	//< CSD-021024
	return NPC_Gen_Ref[npc_index].nPoison;
}	//> CSD-021024

int CCharInfo::GetNpcBlindAvoid() const
{	//< CSD-021024
	return NPC_Gen_Ref[npc_index].nBlind;
}	//> CSD-021024

int CCharInfo::GetNpcDrainAvoid() const
{	//< CSD-021024
	return NPC_Gen_Ref[npc_index].nDrain;
}	//> CSD-021024

int CCharInfo::GetNpcFreezeAvoid() const
{	//< CSD-021024
	return NPC_Gen_Ref[npc_index].nFreeze;
}	//> CSD-021024

int CCharInfo::GetMovePower(BYTE nType) const
{	// 이동력 구하기
	const int nMax = __min(MoveP, 310);
	if (IsNpc())  return nMax;
	
	int nBase = 0, nMove = 50;
	// 아이템의 이동력 가지고 오기
	CItem* pItem = ::ItemUnit(equip[WT_SHOES]);
	
	if (pItem != NULL)
	{ 
		switch (nType)
		{
		case HORSE:
			{
				nBase = __max(8*((nMax - 10)/30), 0);
				nMove += (pItem->GetDefense_power() + nBase)*0.3;
				break;
			}
		case BULL:
			{
				nBase = __max(9*((nMax - 10)/30), 0);
				nMove += (pItem->GetDefense_power() + nBase)*0.3;
				break;
			}
		case PHANTOM:
			{
				nBase = __max(10*((nMax - 10)/30), 0);
				nMove += (pItem->GetDefense_power() + nBase)*0.3;
				break;
			}
		default:
			{
				nMove += pItem->GetDefense_power()*0.3;
				break;
			}
		}
	}
    
	return nMove;
}

int CCharInfo::GetDistance(int nX, int nY) const
{
	const int nDistance = int(sqrt(pow((float)X - nX, (float)2) + pow((float)Y - nY, (float)2)));
	return nDistance>>5;
}

int CCharInfo::GetRiseFall(int nX, int nY) const
{	// 0 : 실패,  1 : 고 -> 저, 2 : 저 -> 고, 3 : 같은 높이
	if (MoveSx < 0 || nX < 0)  return 0;
	if (MoveSy < 0 || nY < 0)  return 0; 
	if (MoveSx >= g_lpMapFile->wWidth || nX >= g_lpMapFile->wWidth)   return 0;
	if (MoveSy >= g_lpMapFile->wHeight || nY >= g_lpMapFile->wHeight)  return 0;

	if (TileMap[MoveSx][MoveSy].empty__4 && !TileMap[nX][nY].empty__4)  return 1;
	if (!TileMap[MoveSx][MoveSy].empty__4 && TileMap[nX][nY].empty__4)  return 2;
	if (TileMap[MoveSx][MoveSy].empty__4 && TileMap[nX][nY].empty__4)  return 3;
	if (!TileMap[MoveSx][MoveSy].empty__4 && !TileMap[nX][nY].empty__4)  return 3;
	return 0;
}

int CCharInfo::GetWeaponKind() const
{
	const int nItem = equip[WT_WEAPON].item_no;
	CItem* pItem = ::ItemUnit(nItem/1000, nItem%1000);	
	return (pItem != NULL) ? pItem->GetItemKind():IK_NONE;
}

int CCharInfo::GetTacticsKind() const
{
	const int nItem = equip[WT_WEAPON].item_no;
	CItem* pItem = ::ItemUnit(nItem/1000, nItem%1000);	
	
	if (pItem == NULL)
	{	// 030116 kyo 오른손에 있는 무기가 없다. 
		return SKILL_UNKNOWN;	// 030117 맨손일때는 tatic이 오르지 않는다. 
	}
	
	const int nTactic = pItem->GetSkill_Ability();
	
	if (nTactic < TACTICS_Crapple || nTactic > TACTICS_Orison)
	{
		return TACTICS_Crapple;
	}
	
	return nTactic;
}

int CCharInfo::GetCombatValue(int nIndex) const 
{
	const int nLevel = Skill[nIndex - 100];
	return Magic_Ref[nIndex].aLevel[nLevel]; 
}

int CCharInfo::GetPureLife() const
{	//< CSD-030806
	const int nLevel = GetLevel();
	int nBase = 0;
	
	switch (Class)
	{	
    case WARRIOR: nBase = NPC_Lev_Ref[nLevel].nWarriorLife; break;
    case THIEF:   nBase = NPC_Lev_Ref[nLevel].nThiefLife;   break;
    case ARCHER:  nBase = NPC_Lev_Ref[nLevel].nArcherLife;  break;
    case WIZARD:  nBase = NPC_Lev_Ref[nLevel].nWizardLife;  break;
    case PRIEST:  nBase = NPC_Lev_Ref[nLevel].nPriestLife;  break;
	}
	
	const int nCon = GetAbility(CON)/5;
	return (nBase + 111 + (nCon - 1)*(15 + nCon))<<1;
}	//> CSD-030806

int CCharInfo::GetExtendAc() const
{
	return CalcNewAc() + 
		   RareEM.GetStaticRareHardest(StaticRare) +
	       RareEM.GetStaticRareHighShield(StaticRare) +
		   RareEM.GetDynamicRareValue(FITEM_HEAVY_ARMOR, DynamicRare);
}

int CCharInfo::GetItemEffectNumber() const
{	//< CSD-031101 : 물리적 공격 성공 여부 판단
	CItem_Weapon* pWeapon = static_cast<CItem_Weapon*>(::ItemUnit(equip[WT_WEAPON]));
	
	if (pWeapon == NULL)
	{
		return 0;
	}

	if (pWeapon->SkillEffect <= 0)
	{
		return 0;
	}

	const int nRate = pWeapon->kind_of_magic +  rand()%pWeapon->SkillEffect;
	
	if (++m_nItemEffectCount <= nRate)
	{ 
		return 0;
	}

	m_nItemEffectCount = 0;
	return pWeapon->nWeaponSpeed;
}	//> CSD-031101

void CCharInfo::GetStepInfo(char* pData, int nSize)
{	// 단계 데이타 구하기
	if (nSize < MAX_STEP)  return;
	
	pData[CLS_STEP] = char(m_nClsStep);
	pData[DUAL_CLS] = char(m_nDualCls);
	pData[EXP_STEP] = char(m_nExpStep);
	
	for (int i = 0; i < NO_TAC; ++i)
	{
		pData[i + T01_STEP] = char(m_aTacStep[i]);
	}
	
	pData[CSP_STEP] = char(m_gnCombat.GetPoint());
	pData[MAX_STEP] = '\0';
}

void CCharInfo::SetStepInfo(char* pData, int nSize)
{	//< CSD-030806 : 단계 데이타 재설정
	if (nSize < MAX_STEP)  return;
	
	m_nClsStep = __int8(pData[CLS_STEP]);
	m_nDualCls = __int8(pData[DUAL_CLS]);
	m_nExpStep = __int8(pData[EXP_STEP]);

	const int nLevel = GetLevel();

	if (m_nExpStep != NPC_Lev_Ref[nLevel].nStep)
	{
		m_nExpStep = NPC_Lev_Ref[nLevel].nStep;
	}
	
	for (int i = 0; i < NO_TAC; ++i)
	{
		m_aTacStep[i] = __int8(pData[i + T01_STEP]);
	}

	int nCombatPoint = __int8(pData[CSP_STEP]);

	if (nCombatPoint > NPC_Lev_Ref[nLevel].nCspMax)
	{
		nCombatPoint = NPC_Lev_Ref[nLevel].nCspMax;
	}
	
	m_gnCombat.SetPoint(nCombatPoint);
}	//> CSD-030806

int CCharInfo::CorrectCombatAttack(int nCombat, int nResult) const
{	// 공격 전투스킬 속성 보정
	const int nLevel = GetCombatLevel(nCombat);
	
	switch (nResult)
	{
    case CSR_NORMAL: 
		{
			return Magic_Ref[nCombat].aLevel[nLevel] + Magic_Ref[nCombat].aRandom[nLevel]/2;
		}
    case CSR_PRIOR: 
		{
			return Magic_Ref[nCombat].aLevel[nLevel] + Magic_Ref[nCombat].aRandom[nLevel];
		}
    case CSR_POSTERIOR: 
		{
			return Magic_Ref[nCombat].aLevel[nLevel];
		}
    case CSR_CONFLICT: 
		{
			return Magic_Ref[nCombat].aLevel[nLevel] + Magic_Ref[nCombat].aRandom[nLevel];
		}
    case CSR_EQUAL:
		{
			return Magic_Ref[nCombat].aLevel[nLevel] + Magic_Ref[nCombat].aRandom[nLevel]/2;
		}
	}
	
	return 0;
}

int CCharInfo::CorrectCombatDefense(int nCombat, int nResult) const
{	// 방어 전투스킬 속성 보정
	const int nLevel = GetCombatLevel(nCombat);
	
	switch (nResult)
	{
    case CSR_NORMAL: 
		{
			return Magic_Ref[nCombat].aLevel[nLevel] + Magic_Ref[nCombat].aRandom[nLevel]/2;
		}
    case CSR_PRIOR: 
		{
			return Magic_Ref[nCombat].aLevel[nLevel];
		}
    case CSR_POSTERIOR: 
		{
			return Magic_Ref[nCombat].aLevel[nLevel] + Magic_Ref[nCombat].aRandom[nLevel];
		}
    case CSR_CONFLICT: 
		{
			return Magic_Ref[nCombat].aLevel[nLevel] + Magic_Ref[nCombat].aRandom[nLevel];
		}
    case CSR_EQUAL:
		{
			return Magic_Ref[nCombat].aLevel[nLevel] + Magic_Ref[nCombat].aRandom[nLevel]/2;
		}
	}
	
	return 0;
}

int CCharInfo::CorrectItemEffect() const
{	//< CSD-031007
	CItem_Weapon* pWeapon = static_cast<CItem_Weapon*>(::ItemUnit(equip[WT_WEAPON]));
	
	if (pWeapon == NULL)
	{
		return 0;
	}

	return pWeapon->Incr_damage;
}	//> CSD-031007

bool CCharInfo::LevelUpAbility()
{	//< CSD-030806 : 경험치 레벨업
	if (!IsPlayer())
	{
		return false;
	}

	if (!IsLevelUp())
	{
		return false;
	}

	if (Exp <= NPC_Lev_Ref[m_nLevel].nMaxExp)
	{
		return false;
	}
	
	++m_nLevel;

	Exp = NPC_Lev_Ref[m_nLevel].nMinExp;
	SetExperienceStep(NPC_Lev_Ref[m_nLevel].nStep);
	Hp = HpMax;
	SendCharInfoBasic(HP, Hp);
	Mana = ManaMax;
	SendCharInfoBasic(MP, Mana);
	m_gnAbility.Increase(10);
	return true;
}	//> CSD-030806

extern void SaveTacticUp(int nKind, int nLevel, int EventPC);

bool CCharInfo::LevelUpTactics(int nKind)
{	//< CSD-030314 : 택틱 경험치 레벨업
	int nLevel = Skill[TACTICS_Crapple + nKind];
	if (!IsPlayer())                                         return false;
	if (!IsLevelUp(nLevel))                                  return false;
	if (tac_skillEXP[nKind] <= NPC_Lev_Ref[nLevel].nMaxExp)  return false;
	
	++nLevel;
	tac_skillEXP[nKind] = NPC_Lev_Ref[nLevel].nMinExp;
	SetTacticsStep(nKind, NPC_Lev_Ref[nLevel].nStep);
	Skill[TACTICS_Crapple + nKind] = nLevel;
	if(nLevel >= 100 ) SaveTacticUp(nKind, nLevel, GetServerID()); //Eleval 15/08/09 - Saves Tactic Lvl Up in LogOutput
	return true;	
}	//> CSD-030314

bool CCharInfo::LevelUpCombat(int nKind, int nPoint)
{	// 전투스킬 레벨업
	if (!m_gnCombat.IsEnough(nPoint))  return false;
	
	int nLevel = GetCombatLevel(nKind);
	
	if (nLevel < MAX_COMBAT_LEVEL)
	{
		nLevel += nPoint;
		SetCombatLevel(nKind, nLevel);
		m_gnCombat.Decrease(nPoint);
		return true;
	}
	
	return false;
}

bool CCharInfo::ChangeUpAddExp(int nExp)
{	//< CSD-030806
	if (IsNpc())
	{
		return false;
	}

	const int nLevel = GetLevel();

	if (nLevel > MAX_EXP_LEVEL)
	{
		return false;
	}

	float fPlusRate = 1.0;

	switch (Class)
	{
	case WARRIOR: fPlusRate = NPC_Lev_Ref[nLevel].nWarriorExpRate/100.0; break;
	case THIEF:   fPlusRate = NPC_Lev_Ref[nLevel].nThiefExpRate/100.0;   break;
	case ARCHER:  fPlusRate = NPC_Lev_Ref[nLevel].nArcherExpRate/100.0;  break;
	case WIZARD:  fPlusRate = NPC_Lev_Ref[nLevel].nWizardExpRate/100.0;  break;
	case PRIEST:  fPlusRate = NPC_Lev_Ref[nLevel].nPriestExpRate/100.0;  break;
	}

	IncExperience(nExp*fPlusRate);
	return true;
}	//> CSD-030806

bool CCharInfo::ChangeUpAddExpBonus(int nExp)
{	//< CSD-030806
	if (IsNpc())
	{
		return false;
	}

	if (GetLevel() > MAX_EXP_LEVEL)
	{
		return false;
	}
	
	IncExperience(nExp);
	return true;
}	//> CSD-030806


bool CCharInfo::ChangeUpTacExp(int nType, int nExp)
{	//< CSD-TW-030624
	if (nExp <= 0)                                   return false;
	if (nType < 0 && nType > 13)                     return false;
	if (!IsPlayer())                                 return false;
	if (!IsLevelUp(Skill[TACTICS_Crapple + nType]))  return false;
	
	switch (nType)
	{
	case 0: // 일반 Tactics
		{	
			const int nKind = GetTacticsKind();
			
			if (SKILL_UNKNOWN == nKind) break; // 030116 kyo 맨손일땐 태틱이 올르지 안는다. 
			AddPhysicalTacExp1(nKind - TACTICS_Crapple, nExp);
			
			if (IsShield())
			{
				const int nParrying = TACTICS_Parrying - TACTICS_Crapple;
				AddPhysicalTacExp1(nParrying, nExp);
				// 경험치 변화를 위한 Packet 전송
				t_packet packet;
				packet.h.header.type = CMD_TACTICS_PARRYING_EXP;
				packet.h.header.size = sizeof(t_tactics_parrying_exp);
				packet.u.tactics_parrying_exp.exp = tac_skillEXP[nParrying];
				packet.u.tactics_parrying_exp.step = GetTacticsStep(nParrying);
				::QueuePacket(connections, GetServerID(), &packet, 1);
			}
			
			break;
		}
	case 1:	// 택틱수련에서 Tactics
		{
			const int nKind = GetTacticsKind();
			if( SKILL_UNKNOWN == nKind ) break; // 030116 kyo 맨손일땐 태틱이 올르지 안는다. 				
			AddPhysicalTacExp2(nKind - TACTICS_Crapple, nExp);
			
			if (IsShield())
			{
				const int nParrying = TACTICS_Parrying - TACTICS_Crapple;
				AddPhysicalTacExp2(nParrying , nExp);
				// 경험치 변화를 위한 Packet 전송
				t_packet packet;
				packet.h.header.type = CMD_TACTICS_PARRYING_EXP;
				packet.h.header.size = sizeof(t_tactics_parrying_exp);
				packet.u.tactics_parrying_exp.exp = tac_skillEXP[nParrying];
				packet.u.tactics_parrying_exp.step = GetTacticsStep(nParrying);
				::QueuePacket(connections, GetServerID(), &packet, 1);
			}
			
			break;
		}
    case 2: // 일반 Tactics
		{
			const int nKind = (Spell == 0) ? TACTICS_Magery - TACTICS_Crapple:
			TACTICS_Orison - TACTICS_Crapple;
			AddMagicTacExp1(nKind, nExp);
			break;
		}
    case 3:	// 택틱수련에서 Tactics
		{
			const int nKind = (Spell == 0) ? TACTICS_Magery - TACTICS_Crapple:
			TACTICS_Orison - TACTICS_Crapple;
			AddMagicTacExp2(nKind, nExp);
			break;
		}
    case 4: // 맞았을 경우 방패 택틱
		{
			if (IsShield() && Peacests)
			{
				const int nParrying = TACTICS_Parrying - TACTICS_Crapple;
				AddPhysicalTacExp1(nParrying, nExp);
				// 경험치 변화를 위한 Packet 전송
				t_packet packet;
				packet.h.header.type = CMD_TACTICS_PARRYING_EXP;
				packet.h.header.size = sizeof(t_tactics_parrying_exp);
				packet.u.tactics_parrying_exp.exp = tac_skillEXP[nParrying];
				packet.u.tactics_parrying_exp.step = GetTacticsStep(nParrying);
				::QueuePacket(connections, GetServerID(), &packet, 1);
			}
			
			break;
		}
    case 5: // 파티시 물리적 공격 택틱
		{
			const int nKind = GetTacticsKind();
			if( SKILL_UNKNOWN == nKind ) break; // 030116 kyo 맨손일땐 태틱이 올르지 안는다. 
			AddPhysicalTacExp3(nKind - TACTICS_Crapple, nExp);
			break; 
		}
	}
	
	return true;
}	//> CSD-TW-030624

void CCharInfo::GenerateNpcName()
{	//< CSD-030408
	const int nName = (NPC_MaleName_Count == 0) ? 0:(rand()%NPC_MaleName_Count);
	strcpy_s(Name, sizeof(Name), NPC_Name_Ref[nName].Male);
	::EatRearWhiteChar(Name);

	memset(HostName, 0, NM_LENGTH);
	namenumber = nName;
}	//> CSD-030408

void CCharInfo::AutoAttribute()
{	//< CSD-021024 : 자동 속성 설정
	const int nBasicResist = RareEM.GetStaticRareBasicResist(StaticRare) +
							 RareEM.GetDynamicRareValue(FITEM_INVALID_RESIST, DynamicRare);
	if (nBasicResist > 0)
	{
		if (rand()%101 < nBasicResist)
		{
			m_bsAttr.set(IGNORE_MAGIC_RESIST);
		}
		else
		{
			m_bsAttr.reset(IGNORE_MAGIC_RESIST);
		}
	}
	else
	{
		m_bsAttr.reset(IGNORE_MAGIC_RESIST);
	}
}	//> CSD-021024

void CCharInfo::SetAttribute(int nRace)
{	//< CSD-030408 : NPC 속성 설정
	switch (nRace)
	{	
	case GUARD:
		{	// 경비원인 경우
			m_bsAttr.set(IGNORE_STRIKE_ATTACK);
			m_bsAttr.set(IGNORE_MAGIC_AMPLIFY);
			
			if (eventno >= 0)  
			{
				eventno = -1;
			}

			break;
		}
	case NORM_MON:
		{	// 몬스터들은 집안에 들어갈수 없다. 
			notcomeinside = true;
			break;
		}
	}
}	//> CSD-030408

bool CCharInfo::IsShield() const
{
	ItemAttr shield = equip[WT_SHIELD];
	if (shield.item_no == 0)  return false;
	
	CItem* pShield = ItemUnit(shield);
	if (pShield == NULL)  return false;
	//  방어구가 방패이면
	return (pShield->GetItemKind() == IK_SHIELD || pShield->GetItemKind() == IK_IRON_SHIELD)	? true:false;
}

bool CCharInfo::IsArrow() const
{
	ItemAttr arrow = equip[WT_SHIELD];
	if (arrow.item_no == 0)  return false;
	
	CItem* pArrow = ItemUnit(arrow);
	if (pArrow == NULL)  return false;
	//  화살이 있다면
	return (pArrow->GetItemKind() == IK_TWO_ARROW || pArrow->GetItemKind() == IK_DUAL_ARROW) ? true:false;//030102 lsw Arrow
}

bool CCharInfo::IsLimit(int nPoint) const
{	//< CSD-030806
	int nCount = GetCombatPoint() + nPoint;
	
	for (int i = LIGHTNING_BOOM; i <= WIND_EXTREME; ++i)
	{
		nCount += GetCombatLevel(i);
	}
	
	const int nLevel = GetLevel();
	return (nCount > NPC_Lev_Ref[nLevel].nCspMax) ? true:false;
}	//> CSD-030806

bool CCharInfo::IsEnableCombat(BYTE nCombat) const 
{
	return (Magic_Ref[nCombat].exhaust_MP <= Cp) ? true:false;
}

///////////////////////////////////////////////////////////////////////////////
// Protected Method
///////////////////////////////////////////////////////////////////////////////

int CCharInfo::GetExtStr() const
{
	return Str+ Strw + RareEM.GetStaticRareStrength(StaticRare);
}

int CCharInfo::GetExtCon() const
{
	return Con + Conw + RareEM.GetStaticRareConstitution(StaticRare);  
}

int CCharInfo::GetExtDex() const
{
	return Dex + Dexw + RareEM.GetStaticRareDexterity(StaticRare);  
}

int CCharInfo::GetExtWis() const
{
	return Wis + Wisw + RareEM.GetStaticRareWisdom(StaticRare);  
}

int CCharInfo::GetExtInt() const
{
	return Int + Intw + RareEM.GetStaticRareIntelligence(StaticRare);  
}

int CCharInfo::GetExtMove() const
{
	return MoveP + MovePw + RareEM.GetStaticRareMovement(StaticRare);  
}

int CCharInfo::GetExtChar() const
{
	return Char + Charw + RareEM.GetStaticRareCharisma(StaticRare);  
}

int CCharInfo::GetExtEndu() const
{
	return Endu + Enduw + RareEM.GetStaticRareEndurance(StaticRare);  
}

int CCharInfo::GetExtMor() const
{
	return Moral + Moralw + RareEM.GetStaticRareMoral(StaticRare);  
}

int CCharInfo::GetExtLuck() const
{
	return Luck + Luckw + RareEM.GetStaticRareLucky(StaticRare);  
}

int CCharInfo::GetExtWs() const
{
	return wsps + wspsw + RareEM.GetStaticRareWizard(StaticRare);
}

int CCharInfo::GetExtPs() const
{
	return wsps + wspsw + RareEM.GetStaticRarePriest(StaticRare);
}

int CCharInfo::GetRemainStr() const
{
	CAbilityLimit* pLimit = g_mgrLimit.GetCell(Class, m_nClsStep);
	
	const int nDiff = Str - pLimit->GetStr();
	return (nDiff < 0) ? 0:nDiff;
}

int CCharInfo::GetRemainCon() const
{
	CAbilityLimit* pLimit = g_mgrLimit.GetCell(Class, m_nClsStep);
	
	const int nDiff = Con - pLimit->GetCon();
	return (nDiff < 0) ? 0:nDiff;
}

int CCharInfo::GetRemainDex() const
{
	CAbilityLimit* pLimit = g_mgrLimit.GetCell(Class, m_nClsStep);
	
	const int nDiff = Dex - pLimit->GetDex();
	return (nDiff < 0) ? 0:nDiff;
}

int CCharInfo::GetRemainWis() const
{
	CAbilityLimit* pLimit = g_mgrLimit.GetCell(Class, m_nClsStep);
	
	const int nDiff = Wis - pLimit->GetWis();
	return (nDiff < 0) ? 0:nDiff;
}

int CCharInfo::GetRemainInt() const
{
	CAbilityLimit* pLimit = g_mgrLimit.GetCell(Class, m_nClsStep);
	
	const int nDiff = Int - pLimit->GetInt();
	return (nDiff < 0) ? 0:nDiff;
}

int CCharInfo::GetRemainMove() const
{
	CAbilityLimit* pLimit = g_mgrLimit.GetCell(Class, m_nClsStep);
	
	const int nDiff = MoveP - pLimit->GetMove();
	return (nDiff < 0) ? 0:nDiff;
}

int CCharInfo::GetRemainChar() const
{
	CAbilityLimit* pLimit = g_mgrLimit.GetCell(Class, m_nClsStep);
	
	const int nDiff = Char - pLimit->GetChar();
	return (nDiff < 0) ? 0:nDiff;
}

int CCharInfo::GetRemainEndu() const
{
	CAbilityLimit* pLimit = g_mgrLimit.GetCell(Class, m_nClsStep);
	
	const int nDiff = Endu - pLimit->GetEndu();
	return (nDiff < 0) ? 0:nDiff;
}

int CCharInfo::GetRemainMor() const
{
	CAbilityLimit* pLimit = g_mgrLimit.GetCell(Class, m_nClsStep);
	
	const int nDiff = Moral - pLimit->GetMor();
	return (nDiff < 0) ? 0:nDiff;
}

int CCharInfo::GetRemainLuck() const
{
	CAbilityLimit* pLimit = g_mgrLimit.GetCell(Class, m_nClsStep);
	
	const int nDiff = Luck - pLimit->GetLuck();
	return (nDiff < 0) ? 0:nDiff;
}

int CCharInfo::GetRemainWs() const
{
	CAbilityLimit* pLimit = g_mgrLimit.GetCell(Class, m_nClsStep);
	
	const int nDiff = wsps - pLimit->GetWs();
	return (nDiff < 0) ? 0:nDiff;
}

int CCharInfo::GetRemainPs() const
{
	CAbilityLimit* pLimit = g_mgrLimit.GetCell(Class, m_nClsStep);
	
	const int nDiff = wsps - pLimit->GetPs();
	return (nDiff < 0) ? 0:nDiff;
}

int CCharInfo::CalcNewLife(BYTE nCombat) const
{	// 최대 Life량 계산
	if (IsNpc())
	{
		return NPC_Gen_Ref[npc_index].nHpMax;
	}
	
	int nNew = GetPureLife(), nPercent = 0, nAdd = 0;
	
	switch (nCombat)
	{
	case TYBERN_GIFT:
		{
			nPercent = __max(RareEM.GetStaticRareBrood(StaticRare), GetCombatValue(nCombat));
			nAdd = RareEM.GetStaticRareLife(StaticRare) + RareEM.GetStaticRareHighBlood(StaticRare);//020730 lsw
			
			break;
		}
    default:
		{
			nPercent = RareEM.GetStaticRareBrood(StaticRare);
			nAdd = RareEM.GetStaticRareLife(StaticRare)+RareEM.GetStaticRareHighBlood(StaticRare);//020730 lsw
			break;
		}
	}
	
	nNew += nNew*nPercent/100;
	nNew += nAdd;
	
	for (int i = 0 ; i < 8 ; ++i)	
	{
		const int nItem = equip[i].item_no;
		if (nItem) 
		{
			CItem* pItem = ::ItemUnit(nItem/1000, nItem%1000);
			if (pItem != NULL)
			{
				nNew += pItem->GetPlusLife();
			}
		}
	}
	
	if (nNew <= 0 )  nNew = 1;
	if (nNew >= 150000)  nNew = 150000; // CSD-030306
	return nNew;
}

int CCharInfo::CalcNewMana(BYTE nCombat) const
{	// 최대 Mana량 계산 
	if (IsNpc() == true)  return 0;
	
	int nNew = GetAbility(WSPS)*5.3;
	
	if (Spell == PRIEST_SPELL)//성직자
	{
		nNew += nNew * RareEM.GetStaticRareSpirit(StaticRare)/100; //020214 lsw
		nNew += RareEM.GetStaticRareDivine(StaticRare);
	}
	else
	{
		nNew += nNew * RareEM.GetStaticRareMind(StaticRare)/100;	 //020214 lsw
		nNew += RareEM.GetStaticRareMana(StaticRare);
	}
	// 장착한 장비의 Mana 속성값을 더함
	for (int i = 0 ; i < 8 ; ++i)	
	{
		const int nItem = equip[i].item_no;
		if (nItem) 
		{
			CItem* pItem = ::ItemUnit(nItem/1000, nItem%1000);
			if (pItem != NULL)
			{
				nNew += (Spell) ? pItem->GetPlusDivine():pItem->GetPlusMana();
			}
		}
	}
	
	if (nNew <= 0)  nNew = 1;
	if (nNew >= 150000)	nNew = 150000; // CSD-030306
	return nNew;
}

int CCharInfo::CalcNewHungry(BYTE nCombat) const
{	// 최대 Hungry량 계산
	const int nCon = GetAbility(CON)*20;
	int nNew = (nCon + GetLevel()*100 + 3000)/100; // CSD-030806
	// 장착한 장비의 Hungry 속성값을 더함
	for (int i = 0 ; i < 8 ; ++i)	
	{
		const int nItem = equip[i].item_no;
		
		if (nItem) 
		{
			CItem* pItem = ::ItemUnit(nItem/1000, nItem%1000);
			
			if (pItem != NULL)
			{
				nNew += pItem->GetPlusHungry();
			}
		}
	}
	
	nNew += nNew*RareEM.GetStaticRareYouth(StaticRare)/100; //020214 lsw
	nNew += RareEM.GetStaticRareHealth(StaticRare);
	return nNew;
}

int CCharInfo::CalcNewCombat() const
{	//< CSD-040223
	const int nLevel = GetLevel();
	return NPC_Lev_Ref[nLevel].nBaseCP + GetExtChar();
}	//> CSD-040223

int CCharInfo::CalcNewAc() const
{
	int nAc = 1;
	
	for (int i = 0 ; i < 8 ; i ++)	// 장착한 장비의 방어력 가치를 더한다. 
	{
		const int nItem = equip[i].item_no;
		
		if (nItem) 
		{
			CItem* pItem = ::ItemUnit(nItem/1000, nItem%1000);
			
			if (pItem != NULL)  
			{
				nAc += pItem->GetAR_or_DH();
			}
		}
	}
	
	return nAc;
}

int CCharInfo::CalcWeaponHit() const
{
	int nWeapon	=	iPhysicalTotalDamage;
	
	if (iPhysicalRandomDamage!= 0)
	{
		const int nRandom	=	rand()%abs(iPhysicalRandomDamage);
		nWeapon = (iPhysicalRandomDamage > 0) ? nWeapon + nRandom:nWeapon - nRandom;
	}
	
	return nWeapon;
}

int CCharInfo::CalcCriticalHit(BYTE nActive, BYTE nPassive) const
{	//< CSD-031007 : 물리적 공격에서 크리티컬 히트 보정
	const int nItem = RareEM.GetStaticRareCritical(StaticRare);
	int nRate = __max(50 - (nItem*50/100 + 20) + rand()%10, 5);
	
	if (RareEM.GetDynamicRareValue(FITEM_CRITICAL, DynamicRare))
	{
		nRate = 5;
	}
	
	switch (nActive)
	{
    case CRITICAL_HIDING:
		{
			nRate = 0;
			break;
		}
	}
	
	if (++m_nCriticalCount > nRate)
	{ 
		const int nRandom = Hp*100/HpMax;
		m_nCriticalCount = 0; // 크리티컬 히트 초기화
		return (nRandom <= 0) ? 50:50 + rand()%nRandom;
	}
	
	return 0;
}	//> CSD-031007

int CCharInfo::CalcTacticExp(BYTE nKind, DWORD dwExp) const
{   //< CSD-020821
	const int nLevel = Skill[TACTICS_Crapple + nKind];
	//MyLog(LOG_NORMAL, "Tactics Rate : %d", NPC_Lev_Ref[nLevel].nTacRate);
	return dwExp*NPC_Lev_Ref[nLevel].nTacRate/100;
}   //> CSD-020821

void CCharInfo::SendCharInfoBasic(BYTE nKind, DWORD dwData) const
{	//< CSD-TW-030624 : 플레이어가 아니면 안보낸다
	if (IsPlayer())
	{
		t_packet packet;
		packet.h.header.type = CMD_CHAR_INFO_BASIC;
		packet.h.header.size = sizeof(t_char_info_basic) - 1;
		packet.u.char_info_basic.kind = nKind;
		packet.u.char_info_basic.data = dwData;
		::QueuePacket(connections, GetServerID(), &packet, 1);
	}
}	//> CSD-TW-030624

void CCharInfo::SendCharInfoTactic(BYTE nKind) const
{	//< CSD-TW-030624
	if (IsPlayer())
	{
		t_packet packet;
		packet.h.header.type = CMD_CHAR_INFO_TACTIC;
		packet.h.header.size = sizeof(t_char_info_tactic);
		packet.u.char_info_tactic.wKind = nKind;
		packet.u.char_info_tactic.dwExperience = tac_skillEXP[nKind];
		packet.u.char_info_tactic.nStep = GetTacticsStep(nKind);
		::QueuePacket(connections, GetServerID(), &packet, 1);
	}
}	//> CSD-TW-030624

void CCharInfo::AddPhysicalTacExp1(int nKind, int nExp)
{	//< CSD-TW-030624 : 사냥시 택틱 경험치
	if (!IsLimitTac(nKind))  return;
	
	if (Tactics_para == nKind)  
	{
		nExp = nExp*1.2;
	}
	
	cur_tac_skill = nKind;

	IncTacticExp(nKind, nExp);
	LevelUpTactics(nKind);
}	//> CSD-TW-030624

void CCharInfo::AddPhysicalTacExp2(int nKind, int nExp)
{	//< CSD-030806 : 택틱 수련시 택틱 경험치
	if (!IsLimitTac(nKind))
	{
		return;
	}

	const int nLevel = GetLevel();
	
	if (NPC_Lev_Ref[nLevel].nMaxTactic > Skill[TACTICS_Crapple + nKind])
	{	// 택틱 수련에서 올릴 수 있는 경험치 제한
		if (Tactics_para == nKind)
		{
			nExp = (NPC_Lev_Ref[nLevel].nTrainingExp*nExp/HpMax)/7;
		}
		else
		{
			nExp = (NPC_Lev_Ref[nLevel].nTrainingExp*nExp/HpMax)/12;
		}
		
		cur_tac_skill = nKind;
		IncTacticExp(nKind, nExp);
		LevelUpTactics(nKind);
	}	
}	//> CSD-030806

void CCharInfo::AddPhysicalTacExp3(int nKind, int nExp)
{	//< CSD-030806
	if (!IsLimitTac(nKind))
	{
		return;
	}

	const int nLevel = GetLevel();
	
	if (NPC_Lev_Ref[nLevel].nMaxTactic > Skill[TACTICS_Crapple + nKind])
	{	// 택틱 수련에서 올릴 수 있는 경험치 제한
		cur_tac_skill = nKind;
		IncTacticExp(nKind, nExp);
		LevelUpTactics(nKind);
	}	
}	//> CSD-030806

void CCharInfo::AddMagicTacExp1(int nKind, int nExp)
{	//< CSD-TW-030624 : 사냥시 택틱 경험치
	if (!IsLimitTac(nKind))  return;
	
	cur_tac_skill = nKind;
	IncTacticExp(nKind, nExp);
	LevelUpTactics(nKind);
}	//> CSD-TW-030624

void CCharInfo::AddMagicTacExp2(int nKind, int nExp)
{	//< CSD-030806 : 택틱 수련시 택틱 경험치
	if (!IsLimitTac(nKind))
	{
		return;
	}

	const int nLevel = GetLevel();
	
	if (NPC_Lev_Ref[nLevel].nMaxTactic > Skill[TACTICS_Crapple + nKind])
	{	// 택틱 수련에서 올릴 수 있는 경험치 제한
		cur_tac_skill = nKind;
		IncTacticExp(nKind, nExp);
		LevelUpTactics(nKind);
	}
}	//> CSD-030806

///////////////////////////////////////////////////////////////////////////////
// Private Method
///////////////////////////////////////////////////////////////////////////////

bool CCharInfo::IsLimitExp(int nLevel)
{	//< CSD-030415
	const int nStep = GetClassStep();  
	// 클래스 단계에 따른 제한
	CDualDataInfo* pDualData = g_pDualManager->GetDualDataInfo(nStep);

	if (pDualData == NULL)
	{
		return false;
	}

	if (nLevel >= pDualData->GetMaxLevel())
	{
		return false;
	}
	
	return true;
}	//> CSD-030415

bool CCharInfo::IsLimitTac(int nKind)
{	//< CSD-030806
	const int nTactic = Skill[TACTICS_Crapple + nKind];
	
	if (!IsLevelUp(nTactic))
	{
		return false;
	}
	
	if (GetLevel() <= 99)
	{ 
		if (nTactic >= 99)
		{
			m_aTacStep[nKind] = NPC_Lev_Ref[nTactic].nStep;
			tac_skillEXP[nKind] = NPC_Lev_Ref[nTactic].nMinExp;
			return false;
		}
	}
	
	return true;
}	//> CSD-030806