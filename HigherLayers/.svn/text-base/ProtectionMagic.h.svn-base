// ProtectionMagic.h: interface for the CProtectionMagic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROTECTIONMAGIC_H__B3EDBBDD_81DB_480A_9922_D200B57AA76F__INCLUDED_)
#define AFX_PROTECTIONMAGIC_H__B3EDBBDD_81DB_480A_9922_D200B57AA76F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Battle.h"
///////////////////////////////////////////////////////////////////////////////
// 보호계열 마법
class CProtectionMagic : public TBinder<CMagic, CProtectionMagic>
{
public:
	CProtectionMagic();
	virtual ~CProtectionMagic();

public:
  virtual bool Bind();

public:
  virtual bool Execute() 
  { 
    return (!IsExist(m_nIndex)) ? false:(*this.*m_tblHash[m_nIndex])(); 
  }

  virtual bool Elapse(CHARLIST* pTarget) 
  { 
    return true; 
  }

public:
  bool DragonScale();        // 일반 보호 계열 마법
  bool ProtectRangeWeapon();
  bool FireResist();
  bool IceResist();
  bool ElectResist();
  bool TwinkleStar();
  bool Protection();
  bool Shield();
  bool Bless();
  bool ProtectDanger();
  bool GreatShield();
  bool AuthorityLethy();     // CSD-021015
  bool NullityMagic();       // 특수 보호 계열 마법
  bool CancelDivine();
  bool RecurrenceDivine();
  bool PerfectProtect();     // 예외 보호 계열 마법 
   bool AuthorityOfAthyas();	// 030415 kyo 
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_PROTECTIONMAGIC_H__B3EDBBDD_81DB_480A_9922_D200B57AA76F__INCLUDED_)
