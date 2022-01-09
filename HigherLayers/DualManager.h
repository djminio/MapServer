// DualManager.h: interface for the CDualManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DUALMANAGER_H__21B717A0_78B5_4ABC_A426_45F53B443D93__INCLUDED_)
#define AFX_DUALMANAGER_H__21B717A0_78B5_4ABC_A426_45F53B443D93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////
// 듀얼 데이타에 관한 클래스
class CDualDataInfo
{
public:
	static CDualDataInfo* CreateDaulDataInfo()
	{
		return new CDualDataInfo;
	}

private:
	CDualDataInfo();

public:
	~CDualDataInfo();

public:
	int GetMaxLevel() const { return m_nMaxLevel; }
	int GetNeedLadder() const { return m_nNeedLadder; }
	int GetNeedFame() const { return m_nNeedFame; }
	int GetConsumeLadder() const { return m_nConsumeLadder; }
	int GetConsumeFame() const { return m_nConsumeFame; }
	
	void SetMaxLevel(int nLevel) { m_nMaxLevel = nLevel; }
	void SetNeedLadder(int nLadder) { m_nNeedLadder = nLadder; }
	void SetNeedFame(int nFame) { m_nNeedFame = nFame; }
	void SetConsumeLadder(int nLadder) { m_nConsumeLadder = nLadder; }
	void SetConsumeFame(int nFame) { m_nConsumeFame = nFame; }

private:
	int   m_nMaxLevel;
	int   m_nNeedLadder;
	int   m_nNeedFame;
	int   m_nConsumeLadder;
	int   m_nConsumeFame;
};
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// 듀얼 클래스에 관한 클래스
class CDualClassInfo
{
public:
	typedef list<WORD>            LIST_MEMBER;
	typedef LIST_MEMBER::iterator ITOR_MEMBER;

public:
	static CDualClassInfo* CreateDualClassInfo()
	{
		return new CDualClassInfo;
	}

private:
	CDualClassInfo();

public:
	~CDualClassInfo();

public:
	void AddNew(WORD idMemeber);
	void Delete(WORD idMemeber);

	void SendPacket(t_packet* pPacket);

	bool IsExist(WORD idMemeber) const;

public:
	LIST_MEMBER& GetDualClassList()
	{
		return m_ltMember;
	}

private:
	LIST_MEMBER m_ltMember;
};
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// 듀얼에 관한 클래스
class CDualManager : public TSingleton<CDualManager>
{
	typedef vector<CDualClassInfo*> VECTOR_DUAL_CLASS;
	typedef vector<CDualDataInfo*>  VECTOR_DUAL_DATA;

	enum 
	{
		DC_FAIL = 0, 
		DC_LEVLE = 1, 
		DC_LADDER = 2, 
		DC_FAME = 3,
		MAX_DUAL_STEP = 5
	};
	
public:
	CDualManager();
	virtual ~CDualManager();

public:
	void AddDualData(CDualDataInfo* pDualData);
	void AddDualClass(CDualClassInfo* pDualClass);

	bool AddMember(WORD idMember);
	bool DelMember(WORD idMember);

	void SendDualEnable(WORD idMaster, BYTE nPara, BYTE nX, BYTE nY);
	void SendDualChange(WORD idMaster);
	void SendDualDivide(WORD idMaster);
	void RecvDualEnable(WORD idMaster, t_client_dual_enable* pPacket);
	void RecvDualChange(WORD idMaster, t_client_dual_change* pPacket);
	void RecvDualDivide(WORD idMaster, t_client_dual_divide* pPacket);
	void RecvResetAbility(WORD idMaster);
	
public:
	CDualDataInfo* GetDualDataInfo(int nStep)
	{
		if (nStep >= 1 && nStep <= MAX_DUAL_STEP)
		{
			return m_vtDualData[nStep];
		}

		return NULL;
	}

	CDualClassInfo* GetDualClassInfo(int nStep)
	{
		if (nStep >= 1 && nStep <= MAX_DUAL_STEP)
		{
			return m_vtDualClass[nStep];
		}

		return NULL;
	}

	bool IsExist(int nStep) const
	{
		return (nStep >= 1 && nStep <= MAX_DUAL_STEP) ? true:false;
	}

protected:
	void Change(BYTE nStep, CHARLIST* pMaster);
	
private:
	bool IsLevelUp(BYTE nStep, CHARLIST* pMaster);
	
private:
	VECTOR_DUAL_DATA  m_vtDualData;
	VECTOR_DUAL_CLASS m_vtDualClass;
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_DUALMANAGER_H__21B717A0_78B5_4ABC_A426_45F53B443D93__INCLUDED_)
