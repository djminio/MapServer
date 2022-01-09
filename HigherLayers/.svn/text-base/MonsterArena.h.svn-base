// MonsterArena.h: interface for the CMonsterArena class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONSTERARENA_H__45BF01F5_5D94_42EC_ABAC_BB42600E81AB__INCLUDED_)
#define AFX_MONSTERARENA_H__45BF01F5_5D94_42EC_ABAC_BB42600E81AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseArena.h"

///////////////////////////////////////////////////////////////////////////////
//
void SaveMonsterArena(int idGame, int nPoint, CArenaTeam::LIST_MEMBER& rMember);
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
class CMonsterArena : public CBaseArena
{
public:
	CMonsterArena(int nIndex, int nType);
	virtual ~CMonsterArena();

public:
	virtual void StartGame(DWORD dwTime);
	virtual void EndGame(CArenaInfo* pArena);
	virtual bool PlayGame(DWORD dwTime);

protected:
	virtual void DoResult(CArenaTeam* pTeam);
	virtual void DoVictory(CArenaTeam* pTeam);
	virtual void DoDefeat(CArenaTeam* pTeam);
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_MONSTERARENA_H__45BF01F5_5D94_42EC_ABAC_BB42600E81AB__INCLUDED_)
