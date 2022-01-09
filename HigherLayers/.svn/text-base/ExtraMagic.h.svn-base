// ExtraMagic.h: interface for the CExtraMagic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXTRAMAGIC_H__CBC255C8_0BBD_42E9_B761_616AD99FCDC4__INCLUDED_)
#define AFX_EXTRAMAGIC_H__CBC255C8_0BBD_42E9_B761_616AD99FCDC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Battle.h"
///////////////////////////////////////////////////////////////////////////////
// 기타계열 마법 
class CExtraMagic : public TBinder<CMagic, CExtraMagic>
{ 
public:
	CExtraMagic();
	virtual ~CExtraMagic();
	
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
	bool Phasing();             // 시공 계열 마법
	bool Teleport();
	bool MemorizingLocation();
	bool TownPortal();
	bool MultiPort();
	bool VampireDodge();
	bool LowMonsterRecall();    // 소환 계열 마법
	bool MiddleMonsterRecall();
	bool SummoningSkeleton();
	bool SummoningUndead();
	bool RecallFollow();
	bool MagicalTrap();         // 기타 계열 마법
	bool MagicDetect();
	bool Anthem();
	bool InfluenceDivine();
	bool VoiceSoul();
	bool Appreciate();
	bool Pray();				// 마나 올리는 계열 마법 // 030415 kyo
	
protected:
	enum {MAX_COUNT = 10};
	void GetArea(int& rX, int& rY);
	bool GetRecallPlace(int nSrcX, int nSrcY, int& rDstX, int& rDstY); // CSD-030324
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_EXTRAMAGIC_H__CBC255C8_0BBD_42E9_B761_616AD99FCDC4__INCLUDED_)
