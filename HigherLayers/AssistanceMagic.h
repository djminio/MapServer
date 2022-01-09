// AssistanceMagic.h: interface for the CAssistanceMagic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASSISTANCEMAGIC_H__A2C0E564_4D5D_437F_BBAC_7A02CE310911__INCLUDED_)
#define AFX_ASSISTANCEMAGIC_H__A2C0E564_4D5D_437F_BBAC_7A02CE310911__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Battle.h"
///////////////////////////////////////////////////////////////////////////////
// 보조계열 마법
class CAssistanceMagic : public TBinder<CMagic, CAssistanceMagic>
{ 
public:
	CAssistanceMagic();
	virtual ~CAssistanceMagic();

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
  bool SpeedUp();       // 보조 1계열 마법
  bool EnchantWeapon(); // 보조 2계열 마법
  bool PrayPower();
  bool GreatPray();
  bool MinorAmplify();  // 보조 3계열 마법
  bool MagicAmplify();
  bool Transparency();  // 보조 4계열 마법
  bool Light();         // 보조 5계열 마법 
  bool AuthorityLuck(); // 보조 6계열 마법
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_ASSISTANCEMAGIC_H__A2C0E564_4D5D_437F_BBAC_7A02CE310911__INCLUDED_)
