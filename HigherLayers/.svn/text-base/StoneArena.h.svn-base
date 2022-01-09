// StoneArena.h: interface for the CStoneArena class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STONEARENA_H__1D802504_BCE3_4C44_AE96_B086CC7E01E2__INCLUDED_)
#define AFX_STONEARENA_H__1D802504_BCE3_4C44_AE96_B086CC7E01E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseArena.h"

///////////////////////////////////////////////////////////////////////////////
//
class CStoneArena : public CBaseArena  
{
public:
	CStoneArena(int nIndex, int nType);
	virtual ~CStoneArena();

public:
	virtual void StartGame(DWORD dwTime);
	virtual void EndGame(CArenaInfo* pArena);
	virtual bool PlayGame(DWORD dwTime);

protected:
	virtual void DoResult(CArenaTeam* pTeam);
	virtual void DoVictory(CArenaTeam* pTeam);
	virtual void DoDefeat(CArenaTeam* pTeam);

private:
	int m_aStone[4];
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_STONEARENA_H__1D802504_BCE3_4C44_AE96_B086CC7E01E2__INCLUDED_)
