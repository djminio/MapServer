// PhysicalAttack.h: interface for the CPhysicalAttack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHYSICALATTACK_H__562FB52A_67CE_44B8_82E6_8CCA090DFDA2__INCLUDED_)
#define AFX_PHYSICALATTACK_H__562FB52A_67CE_44B8_82E6_8CCA090DFDA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Battle.h"
///////////////////////////////////////////////////////////////////////////////
// 물리적 공격
class CPhysicalAttack : public TBinder<CStrike, CPhysicalAttack>
{
public:
	CPhysicalAttack();
	virtual ~CPhysicalAttack();
	
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
	bool AttackShort();  // 단거리 물리적 공격
	bool AttackMiddle(); // 중거리 물리적 공격
	bool AttackLong();   // 장거리 물리적 공격
	bool AttackEvent();  // CSD-030716
	bool AttackEffect(); // CSD-031007
	
protected:
	bool IsRange() const;
	void LogIsRange();
	bool IsMiss() const;
	
private:
	void Correct(int& rDamage);
	bool Result(int nDamage);
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_PHYSICALATTACK_H__562FB52A_67CE_44B8_82E6_8CCA090DFDA2__INCLUDED_)
