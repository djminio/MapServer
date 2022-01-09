// CoreArena.h: interface for the CCoreArena class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COREARENA_H__CDA23117_22AC_4BD7_95A4_02DFA6D8759C__INCLUDED_)
#define AFX_COREARENA_H__CDA23117_22AC_4BD7_95A4_02DFA6D8759C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseArena.h"

///////////////////////////////////////////////////////////////////////////////
//
class CCoreArena : public CBaseArena
{
public:
	CCoreArena(int nIndex, int nType);
	virtual ~CCoreArena();

public:
	virtual void StartGame(DWORD dwTime);
	virtual void EndGame(CArenaInfo* pArena);
	virtual bool PlayGame(DWORD dwTime);

protected:
	virtual void DoResult(CArenaTeam* pTeam);
	virtual void DoVictory(CArenaTeam* pTeam);
	virtual void DoDefeat(CArenaTeam* pTeam);

private:
	bool m_bGameOver;
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_COREARENA_H__CDA23117_22AC_4BD7_95A4_02DFA6D8759C__INCLUDED_)
