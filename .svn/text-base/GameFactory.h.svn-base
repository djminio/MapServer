// GameFactory.h: interface for the CGameFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEFACTORY_H__3FB0BFE5_ABF8_4108_AF4F_91B4C7A17B0F__INCLUDED_)
#define AFX_GAMEFACTORY_H__3FB0BFE5_ABF8_4108_AF4F_91B4C7A17B0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameDefine.h"

///////////////////////////////////////////////////////////////////////////////
// 게임 관련 Manager 생성 관리 클래스
class CGameFactory : public IGameFactory
{
public:
	CGameFactory();
	virtual ~CGameFactory();

public:
	virtual CLogManager* CreateLogManager();
	virtual void DestroyLogManager(CLogManager* pManager);
	
	virtual CUserManager* CreateUserManager();
	virtual void DestroyUserManager(CUserManager* pManager);

	virtual CCounselorManager* CreateCounselorManager();
	virtual void DestroyCounselorManager(CCounselorManager* pManager);

	virtual CDualManager* CreateDualManager();
	virtual void DestroyDualManager(CDualManager* pManager);

	virtual CGuildManager* CreateGuildManager();
	virtual void DestroyGuildManager(CGuildManager* pManager);

	virtual CAIManager* CreateAIManager();
	virtual void DestroyAIManager(CAIManager* pManager);

	virtual CAdventManager* CreateAdventManager();
	virtual void DestroyAdventManager(CAdventManager* pManager);

	virtual CArenaManager* CreateArenaManager();
	virtual void DestroyArenaManager(CArenaManager* pManager);

	virtual CRegenManager* CreateRegenManager();
	virtual void DestroyRegenManager(CRegenManager* pManager);

	virtual CBattleManager* CreateBattleManager();
	virtual void DestroyBattleManager(CBattleManager* pManager);
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_GAMEFACTORY_H__3FB0BFE5_ABF8_4108_AF4F_91B4C7A17B0F__INCLUDED_)