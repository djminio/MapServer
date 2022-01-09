// FreezeCombat.h: interface for the CFreezeCombat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FREEZECOMBAT_H__4A0284B3_A810_488E_84FF_EE36B1899B46__INCLUDED_)
#define AFX_FREEZECOMBAT_H__4A0284B3_A810_488E_84FF_EE36B1899B46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Battle.h"
///////////////////////////////////////////////////////////////////////////////
// 얼음속성 전투스킬
class CFreezeCombat : public TBinder<CCombat, CFreezeCombat>  
{
public:
	CFreezeCombat();
	virtual ~CFreezeCombat();

public:
  virtual bool Bind();
  virtual bool Elapse(CHARLIST* pTarget);
  
public:
  virtual bool Execute() 
  { 
    return (!IsExist(m_nIndex)) ? false:(*this.*m_tblHash[m_nIndex])(); 
  }

public:
  bool IcingBlast();
  bool CriticalHiding();
  bool SharkMissile();
  bool HornOfIceBerg();
  bool IceShield();
  bool IceExtreme();
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_FREEZECOMBAT_H__4A0284B3_A810_488E_84FF_EE36B1899B46__INCLUDED_)
