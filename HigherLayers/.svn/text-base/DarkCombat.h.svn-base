// DarkCombat.h: interface for the CDarkCombat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DARKCOMBAT_H__5C737E9A_36C8_4985_ACCC_F2AC5FD58076__INCLUDED_)
#define AFX_DARKCOMBAT_H__5C737E9A_36C8_4985_ACCC_F2AC5FD58076__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Battle.h"
///////////////////////////////////////////////////////////////////////////////
// 어둠속성 전투스킬
class CDarkCombat : public TBinder<CCombat, CDarkCombat>  
{
public:
	CDarkCombat();
	virtual ~CDarkCombat();

public:
  virtual bool Bind();
  virtual bool Elapse(CHARLIST* pTarget);

public:  
  virtual bool Execute() 
  { 
    return (!IsExist(m_nIndex)) ? false:(*this.*m_tblHash[m_nIndex])(); 
  }
  
  virtual bool Exist()
  { 
    return IsExist(m_nIndex); 
  }

public:
  bool PoisoningNova();
  bool CherroyShade();
  bool DarkBlade();
  bool TybernGift();
  bool DarkArmor();
  bool DarkExtreme();
  bool AcuquireDark(); // 030415 kyo
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_DARKCOMBAT_H__5C737E9A_36C8_4985_ACCC_F2AC5FD58076__INCLUDED_)
