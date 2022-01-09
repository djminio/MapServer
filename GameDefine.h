// GameDefine.h: interface for the CGameDefine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEDEFINE_H__64C05F0A_ACA0_429D_9FBA_0A73F1F6D04D__INCLUDED_)
#define AFX_GAMEDEFINE_H__64C05F0A_ACA0_429D_9FBA_0A73F1F6D04D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLogManager;
class CUserManager;
class CCounselorManager;
class CDualManager;
class CGuildManager;
class CAIManager;
class CAdventManager;
class CArenaManager;
class CRegenManager;
class CBattleManager;
///////////////////////////////////////////////////////////////////////////////
//	Game Factory 추상 클래스
class IGameFactory
{
public:
	virtual ~IGameFactory() {}

public:
	virtual CLogManager* CreateLogManager() = 0;
	virtual void DestroyLogManager(CLogManager* pManager) = 0;
	
	virtual CUserManager* CreateUserManager() = 0;
	virtual void DestroyUserManager(CUserManager* pManager) = 0;

	virtual CCounselorManager* CreateCounselorManager() = 0;
	virtual void DestroyCounselorManager(CCounselorManager* pManager) = 0;

	virtual CDualManager* CreateDualManager() = 0;
	virtual void DestroyDualManager(CDualManager* pManager) = 0;

	virtual CGuildManager* CreateGuildManager() = 0;
	virtual void DestroyGuildManager(CGuildManager* pManager) = 0;

	virtual CAIManager* CreateAIManager() = 0;
	virtual void DestroyAIManager(CAIManager* pManager) = 0;

	virtual CAdventManager* CreateAdventManager() = 0;
	virtual void DestroyAdventManager(CAdventManager* pManager) = 0;

	virtual CArenaManager* CreateArenaManager() = 0;
	virtual void DestroyArenaManager(CArenaManager* pManager) = 0;

	virtual CRegenManager* CreateRegenManager() = 0;
	virtual void DestroyRegenManager(CRegenManager* pManager) = 0;

	virtual CBattleManager* CreateBattleManager() = 0;
	virtual void DestroyBattleManager(CBattleManager* pManager) = 0;
};
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Game System 추상 클래스
class IGameSystem
{
public:
	virtual ~IGameSystem() {}
	
public:
	virtual void SetFactory(IGameFactory* pFactory) = 0;

	virtual CLogManager* GetLogManager() = 0;
	virtual CUserManager* GetUserManager() = 0;
	virtual CCounselorManager* GetCounselorManager() = 0;
	virtual CDualManager* GetDualManager() = 0;
	virtual CGuildManager* GetGuildManager() = 0;
	virtual CAIManager* GetAIManager() = 0;
	virtual CAdventManager* GetAdventManager() = 0;
	virtual CArenaManager* GetArenaManager() = 0;
	virtual CRegenManager* GetRegenManager() = 0;
	virtual CBattleManager* GetBattleManager() = 0;
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_GAMEDEFINE_H__64C05F0A_ACA0_429D_9FBA_0A73F1F6D04D__INCLUDED_)