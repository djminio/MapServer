// ImportantListMgr.h: interface for the CImportantListMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMPORTANTLISTMGR_H__DF60A76B_DFE5_4D5C_B43A_90F7F1AAF4DE__INCLUDED_)
#define AFX_IMPORTANTLISTMGR_H__DF60A76B_DFE5_4D5C_B43A_90F7F1AAF4DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CImportantListMgr  
{
public:
	enum eInitType
	{
		IL_INIT_JUST_ITEM_NO = 0,
		IL_INIT_ITEM_NO_WITH_LIMIT = 1,
	};

public:
	CImportantListMgr();
	virtual ~CImportantListMgr();
public:
	bool IsExist(const int iItemNo)
	{
		const HashItor iter = m_mapItemList.find(iItemNo); 
		if (m_mapItemList.end() == iter) 
		{ 
			return false;
		} 
		return true;
	}

	int GetChangeLimitNo(const int iItemNo, const bool bIsInc)
	{
		const HashItor iter = m_mapItemList.find(iItemNo); 
		if (iter == m_mapItemList.end()) { return 0;} 

		const int iNowNo = iter->second;
		if(bIsInc){iter->second++;}
		return iNowNo;
	}

	void Clear();
	bool Init(const eInitType InitType, const char *filename);
	int GetCount()const{return m_mapItemList.size();}//Return Elements Count

private:
	typedef map<__int32, __int32> HASH;
	typedef HASH::iterator HashItor;
	HASH m_mapItemList;
};
extern CImportantListMgr *g_pILMgr;
#endif // !defined(AFX_IMPORTANTLISTMGR_H__DF60A76B_DFE5_4D5C_B43A_90F7F1AAF4DE__INCLUDED_)
