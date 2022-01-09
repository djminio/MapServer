// WarStartInfo.h: interface for the CWarStartInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WARSTARTINFO_H__669C6DC3_152C_4B2E_BCDC_B79B64D90ADE__INCLUDED_)
#define AFX_WARSTARTINFO_H__669C6DC3_152C_4B2E_BCDC_B79B64D90ADE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////
// 立加磊 包府 努贰胶
class CWarStartInfo  
{
	typedef vector<START_UP_INFO*> VECTOR_TIME;
	typedef VECTOR_TIME::iterator  ITOR_TIME;

public:
	static CWarStartInfo* CreateWarStartInfo()
	{
		return new CWarStartInfo;
	}

private:
	CWarStartInfo();

public:
	virtual ~CWarStartInfo();

public:
	void Register(START_UP_INFO* pTime);
	void Execute();

protected:
	bool IsCheckIn(int nIndex) const;
	bool IsCheckOut(int nIndex) const;
	
public:
	DWORD GetRemainTime() const
	{
		return 2*60*60;
	}

	bool IsActive() const 
	{ 
		return m_bActive; 
	}

private:
	VECTOR_TIME m_vtTime;
	DWORD       m_dwRemainTime;
	int         m_nIndex;
	bool        m_bActive;
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_WARSTARTINFO_H__669C6DC3_152C_4B2E_BCDC_B79B64D90ADE__INCLUDED_)
