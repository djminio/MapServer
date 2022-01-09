// FlameCombat.h: interface for the CFlameCombat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLAMECOMBAT_H__A3949B64_07C3_42BF_9F11_F9C51DCC4405__INCLUDED_)
#define AFX_FLAMECOMBAT_H__A3949B64_07C3_42BF_9F11_F9C51DCC4405__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Battle.h"
///////////////////////////////////////////////////////////////////////////////
// 화염속성 전투스킬
class CFlameCombat : public TBinder<CCombat, CFlameCombat>  
{
public:
	CFlameCombat();
	virtual ~CFlameCombat();

public:
  virtual bool Bind();
  virtual bool Elapse(CHARLIST* pTarget);
  
public:
  virtual bool Execute() 
  { 
    return (!IsExist(m_nIndex)) ? false:(*this.*m_tblHash[m_nIndex])(); 
  }

public:
  bool SwordNRoses();
  bool MultipleFire();
  bool FleetMissile();
  bool ConvertingArmor();
  bool CriticalFire();
  bool FireExtreme();
  bool BurningBloody(); // 030415 kyo
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_FLAMECOMBAT_H__A3949B64_07C3_42BF_9F11_F9C51DCC4405__INCLUDED_)
