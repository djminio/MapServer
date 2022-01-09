// AIManager.h: interface for the CAIManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AIMANAGER_H__35FD94E6_6CDA_44B0_B856_4555FCD844C6__INCLUDED_)
#define AFX_AIMANAGER_H__35FD94E6_6CDA_44B0_B856_4555FCD844C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////
// NPC AI 관리를 위한 클래스
class CAIManager : public TSingleton<CAIManager> 
{
public:
	CAIManager();
	virtual ~CAIManager();

public :
	int					Load_AI_Data();
	int					LoadMagicBag();
	void				Clear_AI_Data();
	void				ClearMagicBag();
	int					LoadBossData();
	int					LoadAttackBag();
	int					LoadActionBag();
	void				ClearBossData();
	void				ClearAttackBag();
	void				ClearActionBag();


	inline int**		GetNpcAIData();
	inline int*			GetNpcAIData(int AI_Type);
	inline int			GetNpcAIData(int AI_Type,int Field);

	inline void			SetAI(CHARLIST* Npc,int AIPattern);
	inline void			SetTarget(CHARLIST* Npc,int Target);
	inline void			SendBossAction(CHARLIST* Npc,int AttackMethod);

	CHARLIST*			GetDragonPtr(CHARLIST* Npc);							// LTS DRAGON MODIFY
	void				CheckDeath(CHARLIST* Defender);							// LTS DRAGON MODIFY

	void				ProcessAIBAttacked(CHARLIST* Npc);
	void				ProcessNPCAI(CHARLIST* Npc);

private:
	inline int			GetMagicBagData(int Row,int Col);

	inline bool	isProcess(CHARLIST* Npc);

	inline int			CheckAIDelayTime(CHARLIST* Npc);
	inline void			SetAIDelayTime(CHARLIST* Npc,DWORD time);
	inline void			SetAIDelayTime(CHARLIST* Npc,int Status,int StatusSub);

	inline int			GetNPCSearchRange(CHARLIST* Npc);

	inline void			SetNextAI(CHARLIST* Npc);
	inline void			SetAIDefault(CHARLIST* Npc);
	inline void			SetAIBDefault(CHARLIST* Npc);

	inline void			SetAIMove(CHARLIST* Npc);
	inline void			SetAIBMove(CHARLIST* Npc);

	inline void			SelectAttackMethod(CHARLIST* Npc);
	inline void			SelectBAttackMethod(CHARLIST* Npc);
	inline void			ClearAttackMethod(CHARLIST* Npc);
	inline int			AttackableRange(CHARLIST* Npc);
	inline int			GetAttackRange(CHARLIST* Npc);
	inline int			GetMagicAttackRange(CHARLIST* Npc,int MethodField);
	inline int			GetPhysicalAttackRange(CHARLIST* Npc);
	inline int			GetSpecialAttackRange(CHARLIST* Npc,int Method);
	inline int			CheckAttackMethod(CHARLIST* Npc,int MethodField);
	inline int			CheckBAttackMethod(CHARLIST* Npc);
	inline int			GetAttackChange(CHARLIST* Npc);
	inline int			CheckSummon(CHARLIST* Npc);
	
	inline CHARLIST*	GetTarget(int TargetID);
	inline int			FindTarget(CHARLIST* Npc);
	inline void			FindTarget(CHARLIST* Npc,int& Target);
	inline void			ClearTarget(CHARLIST* Npc);
	
	inline void			RandomMove(CHARLIST* Npc);
	inline void			TargetMove(CHARLIST* Npc);
	inline void			ClearMovePattern(CHARLIST* Npc);

	inline int			CheckAndDoAttack(CHARLIST* Npc);
	inline void			SendAttack(CHARLIST* Npc);
	inline void			SendBAttack(CHARLIST* Npc);
	inline void			SendMagicAttack(CHARLIST* Npc,int Field);
	inline void			SendMagicBAttack(CHARLIST* Npc);

	inline void			SendPhysicalAttack(CHARLIST* Npc);
	inline void			CheckAndSendAttack(CHARLIST* Npc,int MethodField);
	inline void			CheckAndSendBAttack(CHARLIST* Npc);
	inline void			SelectSummonNPC(CHARLIST* Npc);
	inline void			SendSummon(CHARLIST* Npc,int NpcIndex);
	inline void			SendNPCAction(CHARLIST* Npc,int Action);
	inline void			SendBossAction(CHARLIST* Npc);

	void				CheckMoveWander(CHARLIST* Npc);
	void				CheckMoveAccess(CHARLIST* Npc);

	inline void			SetRunStatus(CHARLIST* Npc);
	inline void			SetRecoveryStatus(CHARLIST* Npc);
	inline void			CheckStatusUp(CHARLIST* Npc);

	inline int**		GetBossStatusData();
	inline int*			GetBossStatusData(int TypeNo);
	inline int			GetBossStatusData(int TypeNo,int Field);
	inline int			GetAttackBagNo(CHARLIST* Npc);
	inline int			GetAttackMethodFromAttackBag(int AttackBagNo);

	inline int			CheckHPGrade(CHARLIST* Npc);
	inline void			CheckHPStatus(CHARLIST* Npc);
	inline int			inHPRange(CHARLIST* Npc,int sField,int eField,int Percent);
	inline void			OnHPAction(CHARLIST* Npc,int HPGrade);
	inline void			OnCloseHPAction(CHARLIST* Npc);
	inline void			SetActionFunc(CHARLIST* Npc,int HPGrade,int* Action);
	inline void			CallActionFunc(CHARLIST* Npc,int* Action);
	inline void			CheckAndModifyTarget(CHARLIST* Npc);

	inline CHARLIST*	GetNearNPC(CHARLIST* Npc,int IndexNo,int Range);		// LTS DRAGON MODIFY
	inline int			CheckDragonPtr(CHARLIST* Npc);							// LTS DRAGON MODIFY

	inline void			CheckSummonDeath(CHARLIST* Defender);						// LTS DRAGON MODIFY
	inline void			CheckDragonHeartDeath(CHARLIST* Defenderc);					// LTS DRAGON MODIFY

	inline void			ProcessAIWander(CHARLIST* Npc);
	inline void			ProcessAIAccess(CHARLIST* Npc);
	inline void			ProcessAIAttack(CHARLIST* Npc);
	inline void			ProcessAIAttacked(CHARLIST* Npc);
	inline void			ProcessAIMoving(CHARLIST* Npc);

	inline void			ProcessAIBWander(CHARLIST* Npc);
	inline void			ProcessAIBAccess(CHARLIST* Npc);
	inline void			ProcessAIBAttack(CHARLIST* Npc);
	inline void			ProcessAIBMoving(CHARLIST* Npc);
	inline void			ProcessAIBBackDraw(CHARLIST* Npc);
	inline void			ProcessAIBWait(CHARLIST* Npc);
	inline void			ProcessAIBAvoid(CHARLIST* Npc);

private:
	int**				m_pNpc_AI_Data;
	int					m_iAI_DataCount;
	int**				m_pMagicBag;
	int					m_iMagicBagCount;

	int**				m_pBossStatusData;
	int					m_iBossStatusCount;
	int**				m_pAttackBag;
	int					m_iAttackBagCount;
	int**				m_pActionBag;
	int					m_iActionBagCount;

	CHARLIST*			m_pDragon;							// LTS DRAGON MODIFY
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_AIMANAGER_H__35FD94E6_6CDA_44B0_B856_4555FCD844C6__INCLUDED_)
