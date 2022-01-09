// ThunderCombat.h: interface for the CThunderCombat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THUNDERCOMBAT_H__05DD80F9_E301_4FC5_9A67_E7EAB59EFB90__INCLUDED_)
#define AFX_THUNDERCOMBAT_H__05DD80F9_E301_4FC5_9A67_E7EAB59EFB90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Battle.h"
///////////////////////////////////////////////////////////////////////////////
// 번개속성 전투스킬
class CThunderCombat : public TBinder<CCombat, CThunderCombat>
{
public:
	CThunderCombat();
	virtual ~CThunderCombat();

public:
  virtual bool Bind();
  virtual bool Elapse(CHARLIST* pTarget);
  
public:
  virtual bool Execute() 
  { 
    return (!IsExist(m_nIndex)) ? false:(*this.*m_tblHash[m_nIndex])(); 
  }

public:
  bool LightningBoom();
  bool ThunderBlow();
  bool LightningShock();
  bool ThunderStrike();
  bool LightningShield();
  bool LightningExtreme();

};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_THUNDERCOMBAT_H__05DD80F9_E301_4FC5_9A67_E7EAB59EFB90__INCLUDED_)
