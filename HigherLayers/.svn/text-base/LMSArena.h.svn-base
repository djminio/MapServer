// LMSArena.h: interface for the CLMSArena class.
//
//////////////////////////////////////////////////////////////////////

#ifndef LMS_ARENA_H
#define LMS_ARENA_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseArena.h"

///////////////////////////////////////////////////////////////////////////////
//
class CLMSArena : public CBaseArena
{
public:
	CLMSArena(int nIndex, int nType);
	virtual ~CLMSArena();

public:
	virtual void StartGame(DWORD dwTime);
	virtual void EndGame(CArenaInfo* pArena);
	virtual bool PlayGame(DWORD dwTime);
	virtual bool DelayGame(DWORD dwTime);

private: 
	DWORD m_dwDelaySecond;
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // LMS_ARENA_H
