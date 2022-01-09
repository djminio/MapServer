// MagicState.h: interface for the CMagicState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAGICSTATE_H__1253A149_5DFB_439E_9CEE_DA046405F173__INCLUDED_)
#define AFX_MAGICSTATE_H__1253A149_5DFB_439E_9CEE_DA046405F173__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////
// 마법 상태 기능을 위한 클래스
class CCharState
{
public:
	CCharState() { Clear(); }
	
public:
	void Clear()
	{	// 변수 초기화 
		m_dwTime = 0, m_nRatio = 0;
	}
	
	BYTE GetRatio() const { return m_nRatio; }
	
	void SetState(DWORD dwTime, BYTE nRatio)
	{	// 상태 재설정 
		m_dwTime = dwTime, m_nRatio = nRatio;
	}
	
	int Convert(int nValue) const 
	{	// 입력값을 비율로 환산
		return nValue*m_nRatio/100; 
	}
	
	bool IsMax() const 
	{
		return (m_dwTime > 0 && m_nRatio == 100) ? true:false;
	}
	
	bool IsContinue() const
	{	// 상태 지속 여부 검사
		return (m_dwTime > 0) ? true:false; 
	}
	
	bool IsContinue(DWORD dwTime) const 
	{	// 상태 지속 여부 검사
		return (m_dwTime > dwTime) ? true:false; 
	}
	
	bool IsBound() const 
	{ // 범위(0 ~ 100%) 검사
		return (m_nRatio >= 0 && m_nRatio <= 100) ? true:false; 
	}
	
private:
	DWORD  m_dwTime; // 지속시간
	BYTE   m_nRatio; // 비율(0 ~ 100%)
};

//
///////////////////////////////////////////////////////////////////////////////
#define NAT_SPEED_UP  0
#define NAT_STATUS_UP 1
#define NAT_LIFE_UP   2
#define NAT_NORMAL    3
#define DELAY         4
///////////////////////////////////////////////////////////////////////////////
// 마법을 지속할 수 있는 시간량
struct MAGIC_DURATION
{	//< CSD-TW-030606
	CCharState apShort;     // 단거리 물리적 공격력
	CCharState apMiddle;    // 중거리 물리적 공격력
	CCharState apLong;      // 장거리 물리적 공격력
	
	CCharState dpShort;     // 단거리 물리적 방어력
	CCharState dpMiddle;    // 중거리 물리적 방어력
	CCharState dpLong;      // 장거리 물리적 방어력
	
	CCharState dpFire;      // 불계열 공격마법  방어력
	CCharState dpIce;       // 얼음계열 공격마법 방어력
	CCharState dpElect;     // 전격계열 공격마법 방어력
	CCharState dpHoly;      // 신력계열 공격마법 방어력
	
	CCharState dpCurse1;    // 저주 1계열 마법 방어력
	CCharState dpCurse2;    // 저주 2계열 마법 방어력
	CCharState dpCurse3;    // 저주 3계열 마법 방어력
	CCharState dpCurse4;    // 저주 4계열 마법 방어력
	CCharState dpCurse5;    // 저주 5계열 마법 방어력
	CCharState dpFreeze;    // CSD-021216 : 얼림 저주 마법 방어력
    
	CCharState amplify;     // 공격 마법 데미지
	CCharState speedUp;     // 이동속도 증가   
	CCharState speedDown;   // 이동속도 감소  
	
	CCharState avoid;       // 회피율
	CCharState agility;     // 민첩성
	
	DWORD dwCurse1;             // 저주 1계열 마법 상태 
	DWORD dwCurse2;             // 저주 2계열 마법 상태 
	DWORD dwCurse3;             // 저주 3계열 마법 상태 
	DWORD dwCurse4;             // 저주 4계열 마법 상태 
	DWORD dwFreeze;             // CSD-021024 : 얼림 저주 마법 상태
	
	DWORD dwNormalProtect;      // 일반 보호계열 마법 상태
	DWORD dwSpecialProtect;     // 특수 보호계열 마법 상태 
	DWORD dwExceptProtect;      // 예외 보호계열(모든 마법이 실패) 마법 상태
	DWORD dwAttackDelay;	      // 공격 속도 감소(캐릭터가 공격 할수 있는 여부)
	DWORD dwDontMove;           // 이동이 되지 않음
	DWORD dwDontAttack;		      // 일반적인 공격이 되지 않음
	DWORD dwDontCasting;		    // 마법 캐스팅이 되지 않음
	DWORD dwDontMiss;           // 모든 물리적 공격과 마법이 성공
	DWORD dwBlocking;           // 외부와 차단되는 상태
	DWORD dwLight;              // 주위에 빛을 밝히는 상태
	DWORD dwMagicDamageToMana;  // 상대의 공격 마법을 마나를 소모하여 막아냄
	DWORD dwReturnMagic;		    // 마법의 효과를 시전자에게 되돌림
	DWORD dwTransparency;		    // 캐릭터가 투명하게 됨 
	DWORD dwLifeUp;             // 일정 시간동안 Life가 올라감
	DWORD dwStrikeReflect;      // CSD-021015 : 물리공격 데미지 반사
	DWORD dwMagicReflect;       // CSD-021015 : 마법공격 데미지 반사
	DWORD dwNature[NAT_NORMAL]; // 성질 변화 상태  
	DWORD dwCombatState;        // 전투스킬 속성 상태
	DWORD dwUpDivineMana;		// 마나를 올린다.	// 030415 kyo 
	DWORD dwCombatRecovery;		//cp를 올리고 hp를 줄인다.	// 030415 kyo 
};	//> CSD-TW-030606
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// 마법 상태에 대한 클래스
class CMagicState : public MAGIC_DURATION
{ 
public:
	enum 
	{	//< CSD-021216
		CURSE_NONE = 0x00,   // 000000
		CURSE_1 = 0x01,      // 000001
		CURSE_2 = 0x02,      // 000010
		CURSE_3 = 0x04,      // 000100
		CURSE_4 = 0x08,      // 001000
		CURSE_5 = 0x10,      // 010000
		CURSE_FREEZE  = 0x20 // 100000
	};	//> CSD-021216
	
public:
	CMagicState()
	{
		m_nExecuteMagic = 0; // Last executed spell
		m_nCastMagic = 0;
		m_dwExecuteTime = 0; // Time last spell was executed
		m_cResultMismatch = 0;
		m_cMagicResult = 0;
	}
	
public:
	void Clear();
	void Recovery(DWORD dwNow);
	
	bool CheckAttack(BYTE nAttack, DWORD dwNow);
	bool CheckStart(BYTE nMagic, DWORD dwNow);
	bool CheckDoing(BYTE nMagic, DWORD dwNow);
	bool CheckEnd(BYTE nMagic, DWORD dwNow);
	bool CheckDelay(BYTE nMagic, DWORD dwNow);
	bool CheckCombat(BYTE nCombat, DWORD dwNow);
	bool CheckMultiEffect(BYTE nMagic); //Antispellhack - Thralas

	void InitCurse();
	void ClearCurse(BYTE nType);
	void ApplyCurse(BYTE nType, BYTE nState, DWORD dwTime = 0);
	
	int GetAvailRange(BYTE nMagic) const;
	int GetCastReady(BYTE nMagic) const;
	int GetCastContinue(BYTE nMagic) const;
	int GetDualStep(BYTE nMagic) const; // 030415 kyo 
	
	void ClearTransMap();
	void GetTransMap(int& rPort, int& rX, int& rY) const;
	void SetTransMap(int nPort, int nX, int nY);
	
	void ClearCombat(BYTE nSelect = 0);
	void SetActiveCombat(BYTE nCombat, int nCount);
	void SetPassiveCombat(BYTE nCombat, DWORD dwPeriod);
	void SetRecoveryCombat(BYTE nCombat, DWORD dwPeriod); // 030415 kyo 
	
	void SetPhysicalAttack(DWORD dwTime, BYTE nShort, BYTE nMiddle, BYTE nLong);
	void SetPhysicalDefense(DWORD dwTime, BYTE nShort, BYTE nMiddle, BYTE nLong);
	void SetMagicalDefense(DWORD dwTime, BYTE nFire, BYTE nIce, BYTE nElect, BYTE nHoly);
	void SetCurseDefense(DWORD dwTime, BYTE nSet);
	
	void ClearNpcRun();
	void ClearNpcStatusUp();
	void ClearNpcRecovery();
	void SetNpcRun(DWORD dwTime, int nSpeed);
	void SetNpcStatusUp(DWORD dwTime, int nAttack, int nDefense, int nSpeed);
	void SetNpcRecovery(DWORD dwTime, int nLife);
	
	void CorrectShortDamage(int& rDamage, int nPlus = 0, int nMinus = 0) const;
	void CorrectMiddleDamage(int& rDamage, int nPlus = 0, int nMinus = 0) const;
	void CorrectLongDamage(int& rDamage, int nPlus = 0, int nMinus = 0) const;
	
	void CorrectFireDamage(int& rDamage, int nPlus = 0, int nMinus = 0) const;
	void CorrectIceDamage(int& rDamage, int nPlus = 0, int nMinus = 0) const;
	void CorrectElectDamage(int& rDamage, int nPlus = 0, int nMinus = 0) const;
	void CorrectHolyDamage(int& rDamage, int nPlus = 0, int nMinus = 0) const;
	void CorrectMovePlus(int& rMove, int nPlus) const;
	void CorrectMoveMinus(int& rMove, int nMinus) const;
	
public:
	BYTE GetMagic() const { return m_nMagic; }
	BYTE GetActiveCombat() const  { return m_nActive; }
	BYTE GetPassiveCombat() const { return m_nPassive; }
	BYTE GetCombatCount() const { return m_nCount; }
	BYTE GetRecoveryCombat() const { return m_nRecovery; }// 030415 kyo 
	DWORD GetRecoveryCombatTime() const { return dwCombatRecovery;}// 030415 kyo 
	int	 GetRecoveryCombatDecHPRate(int nLevel); // 030514 kyo 5:5%,7:10%,10:15%
	
	int IncCombatCount() { return ++m_nCount; }
	int DecCombatCount() { return --m_nCount; }
	
	void ClearMagic() { m_nMagic = 0; }
	void ClearMagic(bool bLock) { m_bLock = bLock; }

	void InitActiveCombat() { m_nActive = 0; } // CSD-TW-030623
	void InitPassiveCombat() { m_nPassive = 0; } // CSD-TW-030623
	void InitRecoveryCombat() { m_nRecovery = 0; } // CSD-TW-030623

	void ClearActiveState() { m_nCount = 0; } // CSD-TW-030623
	void ClearPassiveState() { dwCombatState = 0; } // CSD-TW-030623
	void ClearRecoveryState() { dwCombatRecovery = 0, m_nRecoveryPoint = 0; } // CSD-TW-030623
	
	int GetShortAttack() const { return apShort.GetRatio(); }
	int GetMiddleAttack() const { return apMiddle.GetRatio(); }
	int GetLongAttack() const { return apLong.GetRatio(); }
	int GetShortDefense() const { return dpShort.GetRatio(); }
	int GetMiddleDefense() const { return dpMiddle.GetRatio(); }
	int GetLongDefense() const { return dpLong.GetRatio(); }
	int GetMagicAmplify() const { return amplify.GetRatio(); }
	int GetFireDefense() const { return dpFire.GetRatio(); }
	int GetIceDefense() const { return dpIce.GetRatio(); }
	int GetElectDefense() const { return dpElect.GetRatio(); }
	int GetHolyDefense() const { return dpHoly.GetRatio(); }
	
	BYTE GetNature() const { return m_nNature; }
	BYTE GetState() const { return m_nState; }
	void SetState(BYTE nState) { m_nState = nState; }
	void AddState(BYTE nState) { m_nState |= nState; }
	void DelState(BYTE nState) { m_nState &= ~nState; }
	// Life 증가량
	int GetLifeUp() const { return m_nLifeUp; }
	// 중독되었을 때 HP가 감소되는 양 얻기
	WORD GetPoisonedPoint() const { return m_wPoisonedPoint; }
	WORD GetPoisonedCount() const { return m_wPoisonedCount; }
	// 중독되었을 때 HP가 감소되는 양 설정
	void SetPoisonedPoint(WORD wPoint = 0) { m_wPoisonedPoint = wPoint; }
	void SetPoisonedCount(WORD wCount = 0) { m_wPoisonedCount = wCount; }
	void IncPoisonedCount() { ++m_wPoisonedCount; }
	void DecPoisonedCount() { --m_wPoisonedCount; }
	
	// 자동 hp다운 
	int	 GetRecoveryCPCount() const { return m_nRecoveryPoint;} // 030415 kyo 
	void SetRecoveryCPCount(int nCount = 0) { m_nRecoveryPoint = nCount; } // 030415 kyo 
	void IncRecoveryCPCount() { ++m_nRecoveryPoint; } // 030415 kyo 
	void DecRecoveryCPCount() { --m_nRecoveryPoint; } // 030415 kyo 

	bool IsState(BYTE nState) const { return (m_nState == nState) ? true:false; }
	bool IsPhysicalDamage() const
	{	// 물리적 데미지를 100% 방어하는지 여부
		return (dpShort.IsMax() && dpMiddle.IsMax() && dpLong.IsMax()) ? false:true;
	}  
	// 완전한 결계 상태인지 여부 검사
	bool IsPerfect() const { return (dwExceptProtect > 0) ? true:false; }
	// 회귀의 권능 상태인지 여부 검사
	bool IsReturn() const { return (dwReturnMagic > 0) ? true:false; }
	// 기도 상태인지 여부 검사
	bool IsDivineUp() const { return (dwUpDivineMana > 0) ? true:false; } // CSD-TW-030606
	// 석화 상태인지 여부 검사
	bool IsStone() const { return (dwBlocking > 0) ? true:false; }
	// 얼림 저주 상태 여부 검사
	bool IsFreeze() const { return (dwAttackDelay > 0) ? true:false; }
	// 시전불가 상태인지 여부 검사
	bool IsCasting() const { return (dwDontCasting > 0) ? false:true; }
	// NPC인 경우 마법 사용이 가능한지 여부 검사
	bool IsCasting(DWORD dwNow) const { return (m_dwCasting > dwNow) ? false:true; }
	// CSD-021015 : 일정 시간동안 Life가 올라감
	bool IsLifeUp() const { return (dwLifeUp > 0) ? true:false; }
	// CSD-021015 : 물리공격 반사 상태 검사
	bool IsStrikeReflect() const { return (dwStrikeReflect > 0) ? true:false; }
	// CSD-021015 : 마법공격 반사 상태 검사
	bool IsMagicReflect() const { return (dwMagicReflect > 0) ? true:false; }
	// 전투스킬 속성 유지 상태
	bool IsPassiveCombatState() const { return (dwCombatState <= 0) ? false:true; } // CSD-TW-030606
	bool IsRecoveryCombatState() const { return (dwCombatRecovery <= 0) ? false:true; } // CSD-TW-030606
	// 마법 방어력이 있는지 여부 검사
	bool IsCurseDefense(BYTE nType) const
	{
		switch (nType)
		{ //< CSD-021216
		case 41: return (dpCurse1.IsContinue()) ? true:false;
		case 42: return (dpCurse2.IsContinue()) ? true:false;
		case 43: return (dpCurse3.IsContinue()) ? true:false;
		case 44: return (dpCurse4.IsContinue()) ? true:false;
		case 45: return (dpCurse5.IsContinue()) ? true:false;
		case 46: return (dpFreeze.IsContinue()) ? true:false; 
		} //> CSD-021216
		
		return false;
	}
	// 저주계열 마법 상태인지 여부 검사
	bool IsCurse(BYTE nType) const
	{	//< CSD-021024 
		switch (nType)
		{
		case 1: return (dwCurse1 > 0) ? true:false;
		case 2: return (dwCurse2 > 0) ? true:false;
		case 3: return (dwCurse3 > 0) ? true:false;
		case 4: return (dwCurse4 > 0) ? true:false;
		case 6: return (dwFreeze > 0) ? true:false;
		}
		
		return false;
	}	//> CSD-021024
	
	bool IsCurse(BYTE nType, DWORD dwNow) const
	{	//> CSD-021024
		switch (nType)
		{
		case 1: return (dwCurse1 < dwNow) ? true:false;
		case 2: return (dwCurse2 < dwNow) ? true:false;
		case 3: return (dwCurse3 < dwNow) ? true:false;
		case 4: return (dwCurse4 < dwNow) ? true:false;
		case 6: return (dwFreeze < dwNow) ? true:false;
		}
		
		return false;
	}	//> CSD-021024
	// 일반 보호계열 마법인지 여부 검사
	bool IsNormalProtect() const { return (dwNormalProtect > 0) ? true:false; }
	// 특수 보호계열 마법인지 여부 검사
	bool IsSpecialProtect() const { return (dwSpecialProtect > 0) ? true:false; }
	// 이동 장소 기억이 설정되었는지 여부 검사
	bool IsTransMap() const { return (m_nTransPort > 0) ? true:false; }
	// 전투스킬 사용 가능 회수 검사
	bool IsCount() const { return (m_nCount > 0) ? true:false; }
	// 길들이기 여부 검사
	bool IsTamedNpc() const { return m_bTamed; }
	void SetTamed(bool bTamed) { m_bTamed = bTamed; }
	// 소환 몬스터 여부 검사
	bool IsSummonMonster() const { return m_bSummon; }
	void SetSummonMonster(bool bSummon) { m_bSummon = bSummon; }
	
public:
	/* Testing
	BYTE  m_nAttack;		   // 020425 YGI from acer
	BYTE  m_nMagic;        // 마법 번호   
	BYTE  m_nActive;       // 단발성의 전투스킬 번호
	BYTE  m_nPassive;      // 지속적인 전투스킬 번호
	BYTE  m_nRecovery;		// 회복 전투스킬  // 030415 kyo 
	BYTE  m_nCount;        // 전투스킬 사용 회수 
	DWORD m_dwAttack;		   // 020425 YGI from acer
	DWORD m_dwCasting;     // 마법 시전 시작 시간
	DWORD m_dwContinue;		// 마법 시전이 가능한 시간
	*/

	BYTE  m_nExecuteMagic; // Last executed spell
	BYTE  m_nCastMagic;
	DWORD m_dwExecuteTime; // Time last spell was executed
	int m_cResultMismatch;
	int m_cMagicResult;

	class XSummonMgr
	{	// 소환한 몬스터 관리 클래스
		typedef vector<WORD> VECTOR;
		
	public:
		XSummonMgr() : m_nFollow(0), m_idMaster(0) {}
		
	public:
		BYTE GetFollow() const { return m_nFollow; }
		void SetFollow(BYTE nFollow) { m_nFollow = nFollow; }
		WORD GetMaster() const { return m_idMaster; }
		void SetMaster(WORD idMaster) { m_idMaster = idMaster; }
		// 소환한 몬스터의 수 증가
		void Increase(WORD wID) 
		{ 
			m_vtBuffer.push_back(wID); 
		}
		// 소환한 몬스터의 수 감소
		void Decrease(WORD wID)  
		{ 
			m_vtBuffer.erase(remove(m_vtBuffer.begin(), m_vtBuffer.end(), wID)); 
		}
		// 소환 몬스터에게 명령
		void Command(bool (*pFunc)(WORD, WORD))
		{
			pointer_to_binary_function<WORD, WORD, bool> pf(pFunc);
			for_each(m_vtBuffer.begin(), m_vtBuffer.end(), bind2nd(pf, m_idMaster));
		}
		// 소환 몬스터 전부 제거
		void Remove(void (*pFunc)(WORD))
		{
			for_each(m_vtBuffer.begin(), m_vtBuffer.end(), pFunc);
			m_vtBuffer.clear();
		}

		// 소환한 몬스터의 수 얻기
		int Count() const { return m_vtBuffer.size(); }
		// 소환한 몬스터가 존재하는지 여부 검사
		bool IsExist() const { return !m_vtBuffer.empty(); }
		
	private:
		VECTOR m_vtBuffer;
		BYTE   m_nFollow;
		WORD   m_idMaster;
	}	m_xSummon;
	
	WORD GetFollow() const { return m_xSummon.GetFollow(); }
	void SetFollow(BYTE nFollow) { m_xSummon.SetFollow(nFollow); }
	WORD GetMaster() const { return m_xSummon.GetMaster(); }
	void SetMaster(WORD idMaster) { m_xSummon.SetMaster(idMaster); }
	
protected:
	BYTE  m_nNature;        // 성질 상태
	BYTE  m_nState;         // 마법 상태
	WORD  m_wPoisonedPoint; // 중독되었을 때 HP가 감소되는 양
	WORD  m_wPoisonedCount; // 중독되었을 때 HP가 감소 초
	int	  m_nRecoveryPoint; // 자동cp회복에서 몇초당 hp가 빠지는가? // 030415 kyo 
	int   m_nTransPort;     // 이동할 맵에 대한 정보
	int   m_nTransX;        // 이동 좌표
	int   m_nTransY;
	int   m_nLifeUp;        // Life 증가량
	bool  m_bTamed;         // 테이밍 여부  
	bool  m_bSummon;        // 소환 몬스터 여부
		
private:
	bool  m_bLock;
	BYTE  m_nAttack;		   // 020425 YGI from acer
	BYTE  m_nMagic;        // 마법 번호
	BYTE  m_nActive;       // 단발성의 전투스킬 번호
	BYTE  m_nPassive;      // 지속적인 전투스킬 번호
	BYTE  m_nRecovery;		// 회복 전투스킬  // 030415 kyo 
	BYTE  m_nCount;        // 전투스킬 사용 회수 
	DWORD m_dwAttack;		   // 020425 YGI from acer
	DWORD m_dwCasting;     // 마법 시전 시작 시간
	DWORD m_dwContinue;		// 마법 시전이 가능한 시간
	DWORD m_dwFireWall;    // 화염벽이 유지되는 시간
	DWORD m_dwIceWall;     // 얼음벽이 유지되는 시간
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_MAGICSTATE_H__1253A149_5DFB_439E_9CEE_DA046405F173__INCLUDED_)