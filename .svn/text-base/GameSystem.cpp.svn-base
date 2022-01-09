// GameSystem.cpp: implementation of the CGameSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameSystem.h"


///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CGameSystem::CGameSystem()
{
	m_pFactory = NULL;
	m_pLogManager = NULL;
	m_pUserManager = NULL;
	m_pDualManager = NULL;
	m_pGuildManager = NULL;
	m_pAIManager = NULL;
	m_pAdventManager = NULL;
	m_pArenaManager = NULL;
	m_pRegenManager = NULL;
	m_pBattleManager = NULL;
}

CGameSystem::~CGameSystem()
{
	m_pFactory->DestroyLogManager(m_pLogManager);
	m_pFactory->DestroyUserManager(m_pUserManager);
	m_pFactory->DestroyDualManager(m_pDualManager);
	m_pFactory->DestroyGuildManager(m_pGuildManager);
	m_pFactory->DestroyAIManager(m_pAIManager);
	m_pFactory->DestroyAdventManager(m_pAdventManager);
	m_pFactory->DestroyArenaManager(m_pArenaManager);
	m_pFactory->DestroyRegenManager(m_pRegenManager);
	m_pFactory->DestroyBattleManager(m_pBattleManager);

	m_pFactory = NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CGameSystem::SetFactory(IGameFactory* pFactory)
{
	Dbg_Assert(pFactory != NULL);
	m_pFactory = pFactory;

	m_pLogManager = pFactory->CreateLogManager();
	Dbg_Assert(m_pLogManager != NULL);
	m_pUserManager = pFactory->CreateUserManager();
	Dbg_Assert(m_pUserManager != NULL);
	m_pCounselorManager = pFactory->CreateCounselorManager();
	Dbg_Assert(m_pCounselorManager != NULL);
	m_pDualManager = pFactory->CreateDualManager();
	Dbg_Assert(m_pDualManager != NULL);
	m_pGuildManager = pFactory->CreateGuildManager();
	Dbg_Assert(m_pGuildManager != NULL);
	m_pAIManager = pFactory->CreateAIManager();
	Dbg_Assert(m_pAIManager != NULL);
	m_pAdventManager = pFactory->CreateAdventManager();
	Dbg_Assert(m_pAdventManager != NULL);
	m_pArenaManager = pFactory->CreateArenaManager();
	Dbg_Assert(m_pArenaManager != NULL);
	m_pRegenManager = pFactory->CreateRegenManager();
	Dbg_Assert(m_pRegenManager != NULL);
	m_pBattleManager = pFactory->CreateBattleManager();
	Dbg_Assert(m_pBattleManager != NULL);
}

CLogManager* CGameSystem::GetLogManager()
{
	Dbg_Assert(m_pLogManager != NULL);
	return m_pLogManager;
}

CUserManager* CGameSystem::GetUserManager()
{
	Dbg_Assert(m_pUserManager != NULL);
	return m_pUserManager;
}

CCounselorManager* CGameSystem::GetCounselorManager()
{
	Dbg_Assert(m_pCounselorManager != NULL);
	return m_pCounselorManager;
}

CDualManager* CGameSystem::GetDualManager()
{
	Dbg_Assert(m_pDualManager != NULL);
	return m_pDualManager;
}

CGuildManager* CGameSystem::GetGuildManager()
{
	Dbg_Assert(m_pGuildManager != NULL);
	return m_pGuildManager;
}

CAIManager* CGameSystem::GetAIManager()
{
	Dbg_Assert(m_pAIManager != NULL);
	return m_pAIManager;
}

CAdventManager* CGameSystem::GetAdventManager()
{
	Dbg_Assert(m_pAdventManager != NULL);
	return m_pAdventManager;
}

CArenaManager* CGameSystem::GetArenaManager()
{
	Dbg_Assert(m_pArenaManager != NULL);
	return m_pArenaManager;
}
	
CRegenManager* CGameSystem::GetRegenManager()
{
	Dbg_Assert(m_pRegenManager != NULL);
	return m_pRegenManager;
}

CBattleManager* CGameSystem::GetBattleManager()
{
	Dbg_Assert(m_pBattleManager != NULL);
	return m_pBattleManager;
}