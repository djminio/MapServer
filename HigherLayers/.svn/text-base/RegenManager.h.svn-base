// RegenManager.h: interface for the CRegenManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGENMANAGER_H__8FB67005_1FDA_4191_9CC2_4F4B35A724EF__INCLUDED_)
#define AFX_REGENMANAGER_H__8FB67005_1FDA_4191_9CC2_4F4B35A724EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////
// 몬스터 출현에 관련된 클래스
class CRegenInfo
{
	typedef vector<REGEN_MONSTER_INFO> VECTOR_MONSTER;
	typedef vector<REGEN_PLACE_INFO>   VECTOR_PLACE;
	typedef list<CHARLIST*>            LIST_MEMBER;
	typedef LIST_MEMBER::iterator      ITOR_MEMBER;

public:
	static CRegenInfo* CreateRegenInfo(int nType)
	{
		return new CRegenInfo(nType);
	}

private:
	CRegenInfo(int nType);

public:
	~CRegenInfo();

public:
	void CalcPlaceOrder(int nCount);
	void CalcRegenTime();

	void AddMaster(CHARLIST* pMonster);
	void DelMaster(CHARLIST* pMonster);

	void AddMember(CHARLIST* pMonster);
	void DelMember(CHARLIST* pMonster);
	void ClearMember();

	void AddMonster(REGEN_MONSTER_INFO& rMonster);
	void AddPlace(REGEN_PLACE_INFO& rPlace);

	bool IsPassTime() const;

public:
	void SetGroupNumber(int nNo) { m_nGroupNo = nNo; }
	void SetDelayTime(DWORD dwTime) { m_dwDelayTime = dwTime; }

	int GetGroupNumber() const { return m_nGroupNo; }
	int GetPlaceOrder() const { return m_nOrder; }
	int GetMonsterInfoSize() const { return m_vtMonster.size(); }
	int GetPlaceInfoSize() const { return m_vtPlace.size(); }

	REGEN_MONSTER_INFO& GetMonsterInfo(int nIndex)
	{
		return m_vtMonster[nIndex];
	}

	REGEN_PLACE_INFO& GetPlaceInfo(int nIndex)
	{
		return m_vtPlace[nIndex];
	}

	bool IsEmptyMember() const
	{
		return m_ltMember.empty();
	}

	bool IsExistMaster() const
	{
		return (m_pMaster != NULL) ? true:false;
	}
	
protected:
	CHARLIST*      m_pMaster;
	LIST_MEMBER    m_ltMember;
	VECTOR_MONSTER m_vtMonster;
	VECTOR_PLACE   m_vtPlace;

private:
	int   m_nOrder;
	int   m_nGroupNo;
	int   m_nCurrentCount;
	DWORD m_dwDelayTime;
	DWORD m_dwRegenTime;
};
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// 몬스터 출현 관리 위한 관리 클래스
class CGroupInfo
{
public:
	typedef list<CRegenInfo*>    LIST_REGEN;
	typedef LIST_REGEN::iterator ITOR_REGEN;

public:
	static CGroupInfo* CreateGroupInfo()
	{
		return new CGroupInfo;
	}

private:
	CGroupInfo();

public:
	~CGroupInfo();

public:
	void Clear();
	void AddRegen(CRegenInfo* pRegen);
	void DelRegen(CRegenInfo* pRegen);
	void ClearRegen();

	CRegenInfo* GetRegenInfo(int nGroup);

	void Ready();
	void Regenerate();
	void Remove();

public:
	LIST_REGEN& GetRegenSet()
	{
		return m_ltRegen;
	}

	int GetGroupType() const
	{ 
		return m_nGroupType; 
	}
	
	void SetGroupType(int nType) 
	{ 
		m_nGroupType = nType; 
	}

	int GetEventType() const 
	{	//< CSD-031027
		return m_nEventType; 
	}	//> CSD-031027

	void SetEventType(int nType)
	{	//< CSD-031027
		m_nEventType = nType; 
	}	//> CSD-031027
	
protected:
	void RegenerateAll();
	void RegenerateCycle();

private:
	void CreateMonster(CRegenInfo* pRegen, int nPlace, int nMonster);

private:
	LIST_REGEN m_ltRegen;
	ITOR_REGEN m_itCurrent;
	int        m_nGroupType;
	int        m_nEventType;
};
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// 몬스터 출현 관리 위한 관리 클래스
class CRegenManager : public TSingleton<CRegenManager> 
{
public:
	typedef map<int, CGroupInfo*>  HASH_GROUP;
	typedef HASH_GROUP::iterator   ITOR_GROUP;	
	typedef HASH_GROUP::value_type PAIR_GROUP;

	CRegenManager();
	~CRegenManager();

public:
	bool AddGroup(int nHunt, CGroupInfo* pGroup);
	bool DelGroup(int nHunt);
	void ClearGroup();

	CRegenInfo* GetRegenInfo(int nGroup);

	void Ready();
	void Ready(int nHunt);

	void Regenerate();
	void Regenerate(int nHunt);

	void Remove();
	void Remove(int nHunt);

public:
	HASH_GROUP& GetGroupSet()
	{
		return m_mpGroup;
	}

	bool IsExistHunt(int nHunt) const
	{
		return (m_mpGroup.find(nHunt) != m_mpGroup.end()) ? true:false;
	}

	bool IsReady() const
	{	//< CSD-040319
		return m_bReady;
	}	//> CSD-040319

private:
	HASH_GROUP m_mpGroup;
	bool m_bReady;
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_REGENMANAGER_H__8FB67005_1FDA_4191_9CC2_4F4B35A724EF__INCLUDED_)