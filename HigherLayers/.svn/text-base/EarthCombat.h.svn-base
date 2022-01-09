// EarthCombat.h: interface for the CEarthCombat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EARTHCOMBAT_H__B46D22F7_C0B2_44BC_B02D_E24B30A3C8F8__INCLUDED_)
#define AFX_EARTHCOMBAT_H__B46D22F7_C0B2_44BC_B02D_E24B30A3C8F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Battle.h"
///////////////////////////////////////////////////////////////////////////////
// 대지속성 전투스킬
class CEarthCombat : public TBinder<CCombat, CEarthCombat>  
{
public:
	CEarthCombat();
	virtual ~CEarthCombat();

public:
  virtual bool Bind();
  virtual bool Elapse(CHARLIST* pTarget);
  
public:
  virtual bool Execute() 
  { 
    return (!IsExist(m_nIndex)) ? false:(*this.*m_tblHash[m_nIndex])(); 
  }

public:
  bool BloodEarth();
  bool GroundAttack();
  bool BloodWill();
  bool StoneArmor();
  bool SimunianBosom();
  bool EarthExtreme();
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_EARTHCOMBAT_H__B46D22F7_C0B2_44BC_B02D_E24B30A3C8F8__INCLUDED_)
