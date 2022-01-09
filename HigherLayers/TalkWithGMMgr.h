// TalkWithGMMgr.h: interface for the CTalkWithGMMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TALKWITHGMMGR_H__5FD02332_DC58_4CBA_AF9B_91EBA90C757E__INCLUDED_)
#define AFX_TALKWITHGMMGR_H__5FD02332_DC58_4CBA_AF9B_91EBA90C757E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "TalkWithGM.h"
class CTalkWithGMMgr  
{
public:
	CTalkWithGMMgr();
	virtual ~CTalkWithGMMgr();
	void Clear();
	//신청 받음
	//리스트 요청 받음
	//

private:
	typedef list<CTalkWithGM*> TWG_LIST;
	typedef TWG_LIST::iterator twgIter;
	TWG_LIST	m_listTWG;

};

#endif // !defined(AFX_TALKWITHGMMGR_H__5FD02332_DC58_4CBA_AF9B_91EBA90C757E__INCLUDED_)
