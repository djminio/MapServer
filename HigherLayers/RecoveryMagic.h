// RecoveryMagic.h: interface for the CRecoveryMagic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECOVERYMAGIC_H__6B76E6F0_8DD8_43C5_AD2A_B606DDC2A5A6__INCLUDED_)
#define AFX_RECOVERYMAGIC_H__6B76E6F0_8DD8_43C5_AD2A_B606DDC2A5A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Battle.h"
///////////////////////////////////////////////////////////////////////////////
// 회복계열 마법
class CRecoveryMagic : public TBinder<CMagic, CRecoveryMagic>
{
public:
	CRecoveryMagic();
	virtual ~CRecoveryMagic();

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
  bool Healing();               // 마법 회복 계열 마법
  bool Decipher();
  bool HealDisease();
  bool GreatHeal();
  bool AutoHeal();
  bool RemoveCurse();
  bool RestoreAuthority();      // 신력 회복 계열 마법
  bool MiracleRecover();
  bool ReliefAuthority();
  bool RecoverPoison();
  bool AuthorityRecoverSight();
  bool CureDisease();
  bool AuthoritySimunian();
  bool HolyCure(); // 030415 kyo 
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_RECOVERYMAGIC_H__6B76E6F0_8DD8_43C5_AD2A_B606DDC2A5A6__INCLUDED_)
