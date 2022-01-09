// BattleManager.h: interface for the CBattleManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATTLEMANAGER_H__D73AA91E_7319_4B3E_B315_CAFEB42E9086__INCLUDED_)
#define AFX_BATTLEMANAGER_H__D73AA91E_7319_4B3E_B315_CAFEB42E9086__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////
// 전투에 관한 클래스
class CBattle;
class CBattleManager : public TSingleton<CBattleManager>
{
	typedef vector<CBattle*> VECTOR;
	
public:
	CBattleManager();
	virtual ~CBattleManager();
	
public:
	void Elapse(DWORD dwSecond = 1);
	void ClearCombat(BYTE nCombat, WORD idMaster);
	void ChangeCombat(BYTE nCombat, WORD idMaster);
	void ResetCombat(BYTE nCombat, WORD idMaster);
	
	void SendNpcAttack(WORD idNpc);
	void SendNpcThrow(WORD idNpc);
	void SendNpcMagic(WORD idNpc, BYTE nMagic);
	void SendNpcMagic(WORD idNpc, BYTE nMagic, int nX, int nY, bool bTimeDisable);
	void SendNpcRecall(WORD idNpc, BYTE nMagic, BYTE nMonster, int nX, int nY);
	void SendNpcTeleport(WORD idNpc, int nX, int nY); // CSD_030314
	
	void RecvNpcAttack(t_npc_attack* pPacket);
	void RecvStrikeAttack(WORD idCaster, t_player_attack* pPacket);
	
	void RecvNpcThrow(t_npc_throw* pPacket);
	void RecvThrowAttack(WORD idCaster, t_client_throw_attack* pPacket);
	void RecvThrowResult(WORD idCaster, t_client_throw_result* pPacket);

	void RecvEffectResult(WORD idCaster, t_client_effect_result* pPacket); // CSD-031007
	
	void RecvMagicLearn(WORD idMaster, k_client_learn_magic* pPacket);
	void RecvNpcMagic(t_client_npc_magic* pPacket);
	void RecvNpcRecall(t_client_npc_magic* pPacket); // CSD-021119
	void RecvNpcTeleport(t_client_npc_magic* pPacket); // CSD-030306
	
	void RecvMagicSelect(WORD idCaster, t_client_magic_select* pPacket); // CSD-TW-030606
	void RecvMagicCasting(WORD idCaster, t_client_magic_casting* pPacket); // CSD-TW-030606
	void RecvMagicExecute(WORD idCaster, t_client_magic_execute* pPacket); // CSD-TW-030606
	void RecvMagicResult(WORD idCaster, t_client_magic_result* pPacket);
	void RecvMagicResult(t_magic_result_d* pPacket);
	
	void RecvCombatReset(WORD idMaster);
	void RecvCombatObtain(WORD idMaster, t_client_combat_obtain* pPacket);
	void RecvCombatRequest(WORD idMaster, t_client_combat_request* pPacket);
	void RecvCombatLearn(WORD idMaster, t_client_combat_learn* pPacket);
	void RecvCombatClear(WORD idMaster, t_client_combat_clear* pPacket); // CSD-TW-030606
	void RecvCombatSelect(WORD idCaster, t_client_combat_select* pPacket);
	void RecvCombatAttack(WORD idCaster, t_client_combat_attack* pPacket);
	void RecvCombatResult(WORD idCaster, t_client_combat_result* pPacket);
	
protected:
	void AutoCurse(WORD idCaster, WORD idTarget);
	
	bool FilterNpcAttack(CHARLIST* pCaster, CHARLIST* pTarget);
	bool FilterAttack(CHARLIST* pCaster, CHARLIST* pTarget);
	bool FilterThrow(CHARLIST* pCaster, CHARLIST* pTarget);
	bool FilterNpcMagic(BYTE nMagic, CHARLIST* pCaster, CHARLIST* pTarget, int nX, int nY);
	bool FilterStart(CHARLIST* pCaster);
	bool FilterMagic(CHARLIST* pCaster, CHARLIST* pTarget, int nX, int nY);
	bool FilterCombat(BYTE nCombat, CHARLIST* pCaster, CHARLIST* pTarget, int nX, int nY);
	bool FilterCombat(BYTE nCombat, CHARLIST* pCaster, CHARLIST* pTarget);
	bool FilterResult(CHARLIST* pCaster, CHARLIST* pTarget);
	bool FilterResult(BYTE nMagic, CHARLIST* pCaster, CHARLIST* pTarget);
	
protected:
	CBattle* GetBattle(int nIndex) const 
	{ 
		return m_vtBuffer[nIndex]; 
	}
	
	bool IsMagic(int nIndex) const
	{
		return (nIndex >= LIFE_UP && nIndex <= RAGE_LIGHT_BALL) ? true:false;
	}
	
	bool IsCombat(int nIndex) const
	{
		return (nIndex >= LIGHTNING_BOOM && nIndex <= WIND_EXTREME) ? true:false;
	}
	
private:  
	bool IsBattle() const;
	bool IsBattle(CHARLIST* pCaster) const;
	bool IsBattle(CHARLIST* pCaster, CHARLIST* pTarget) const;
	bool IsValid(BYTE nMagic, int nX, int nY) const;
	bool IsValid(BYTE nKind, CHARLIST* pCaster) const;
	bool IsValid(BYTE nKind, BYTE nStep, CHARLIST* pCaster) const;
	bool IsAbility(BYTE nMagic, CHARLIST* pCaster) const;
	bool IsEnable(BYTE nMagic, CHARLIST* pCaster) const;
	bool IsHarmfulMagic(BYTE nMagic) const;
	bool IsPlaceMagic(BYTE nMagic, int nX, int nY) const; // CSD-021127
	bool IsApply(BYTE nType, CHARLIST* pCaster, CHARLIST* pTarget) const;
	bool IsMissAttack(CHARLIST* pCaster, CHARLIST* pTarget) const;
	bool IsMissMagic(BYTE nType, CHARLIST* pCaster, CHARLIST* pTarget) const;
	bool IsBugMon(CHARLIST* pCaster, CHARLIST* pTarget) const;
	bool IsBreak(CHARLIST* pCaster, CHARLIST* pTarget) const;
	bool IsColleague(CHARLIST* pCaster, CHARLIST* pTarget) const;
	bool IsEnableClass(BYTE nCombat, CHARLIST* pCaster) const;
	bool IsEnableWeapon(BYTE nCombat, CHARLIST* pCaster) const;
	bool IsActiveCombat(BYTE nCombat) const;
	bool IsPassiveCombat(BYTE nCombat) const;
	bool IsRecoveryCombat(BYTE nCombat) const; // 030415 kyo

public:
	int m_cThrowResult;
	int m_cThrowResultAvg;
	DWORD m_dwLastThrowAttack;

	int m_nLastCombatSelect;
	int m_cCombatResult;
	int m_cCombatResultAvg;
	DWORD m_dwLastCombatResult;

protected:
	static DWORD s_dwDelay;
	
private:
	enum 
	{
		CRAPPLE = 0,              // 주먹
		SWORDMANSHIP = 1,         // 한손검
		ARCHERY = 2,              // 활
		FENCING = 3,              // 팬싱검
		MACE_FIGHTING = 4,        // 한손도끼, 메이스
		PIERCE = 5,               // 창
		WHIRL = 6,                // 봉
		HURL = 7,                 // 비도
		PARRYING = 8,             // 방패 
		DOUBLE_SWORDMANSHIP = 9,  // 양손검
		DOUBLE_MACEFIGHTING = 10, // 양손도끼, 메이스
		ALL_WEAPON = 11           // 모든 무기
	};
	
	VECTOR   m_vtBuffer;
	t_packet m_packet;
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_BATTLEMANAGER_H__D73AA91E_7319_4B3E_B315_CAFEB42E9086__INCLUDED_)
