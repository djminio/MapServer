// GameFactory.cpp: implementation of the CGameFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameFactory.h"

#include "HigherLayers/LogManager.h"
#include "HigherLayers/UserManager.h"
#include "HigherLayers/CounselorManager.h"
#include "HigherLayers/DualManager.h"
#include "HigherLayers/GuildManager.h"
#include "HigherLayers/AIManager.h"
#include "HigherLayers/AdventManager.h"
#include "HigherLayers/ArenaManager.h"
#include "HigherLayers/RegenManager.h"
#include "HigherLayers/BattleManager.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CGameFactory::CGameFactory()
{

}
	
CGameFactory::~CGameFactory()
{

}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

CLogManager* CGameFactory::CreateLogManager()
{
	return new CLogManager();
}

void CGameFactory::DestroyLogManager(CLogManager* pManager)
{
	Dbg_Assert(pManager != NULL);
	delete pManager;
}
	
CUserManager* CGameFactory::CreateUserManager()
{
	return new CUserManager();
}

void CGameFactory::DestroyUserManager(CUserManager* pManager)
{
	Dbg_Assert(pManager != NULL);
	delete pManager;
}

CCounselorManager* CGameFactory::CreateCounselorManager()
{
	return new CCounselorManager();
}

void CGameFactory::DestroyCounselorManager(CCounselorManager* pManager)
{
	Dbg_Assert(pManager != NULL);
	delete pManager;
}
	
CDualManager* CGameFactory::CreateDualManager()
{
	return new CDualManager();
}

void CGameFactory::DestroyDualManager(CDualManager* pManager)
{
	Dbg_Assert(pManager != NULL);
	delete pManager;
}

CGuildManager* CGameFactory::CreateGuildManager()
{
	return new CGuildManager();
}

void CGameFactory::DestroyGuildManager(CGuildManager* pManager)
{
	Dbg_Assert(pManager != NULL);
	delete pManager;
}

CAIManager* CGameFactory::CreateAIManager()
{
	return new CAIManager();
}

void CGameFactory::DestroyAIManager(CAIManager* pManager)
{
	Dbg_Assert(pManager != NULL);
	delete pManager;
}

CAdventManager* CGameFactory::CreateAdventManager()
{
	return new CAdventManager();
}

void CGameFactory::DestroyAdventManager(CAdventManager* pManager)
{
	Dbg_Assert(pManager != NULL);
	delete pManager;
}

CArenaManager* CGameFactory::CreateArenaManager()
{
	return new CArenaManager();
}

void CGameFactory::DestroyArenaManager(CArenaManager* pManager)
{
	Dbg_Assert(pManager != NULL);
	delete pManager;
}

CRegenManager* CGameFactory::CreateRegenManager()
{
	return new CRegenManager();
}

void CGameFactory::DestroyRegenManager(CRegenManager* pManager)
{
	Dbg_Assert(pManager != NULL);
	delete pManager;
}

CBattleManager* CGameFactory::CreateBattleManager()
{
	return new CBattleManager();
}
	
void CGameFactory::DestroyBattleManager(CBattleManager* pManager)
{
	Dbg_Assert(pManager != NULL);
	delete pManager;
}
