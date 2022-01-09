// WindCombat.h: interface for the CWindCombat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINDCOMBAT_H__BBAA3D62_FB7C_4B4B_9B9F_832DBFD9315E__INCLUDED_)
#define AFX_WINDCOMBAT_H__BBAA3D62_FB7C_4B4B_9B9F_832DBFD9315E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Battle.h"
///////////////////////////////////////////////////////////////////////////////
// 바람속성 전투스킬
class CWindCombat : public TBinder<CCombat, CWindCombat>  
{
public:
	CWindCombat();
	virtual ~CWindCombat();

public:
  virtual bool Bind();
  virtual bool Elapse(CHARLIST* pTarget);
  
public:
  virtual bool Execute() 
  { 
    return (!IsExist(m_nIndex)) ? false:(*this.*m_tblHash[m_nIndex])(); 
  }

public:
  bool Charging();
  bool DoubleAttack();
  bool Whilwind();
  bool Twister();
  bool Gust();
  bool WindExtreme();
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_WINDCOMBAT_H__BBAA3D62_FB7C_4B4B_9B9F_832DBFD9315E__INCLUDED_)
