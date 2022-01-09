/* 
	filename:	jNpcEnum.h
	coder	:	Cho Wook Rea (4MLMman@hitel.net)(icq:74456993) 
	comp.	:	esofnet 
	compiler:	vc++ 6.0
	date	:	2000-06-22���� 2:13:29
	title	:	
*/


#ifndef __jNpcEnum_h__
#define __jNpcEnum_h__
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum { // ������. �����ҽ��� ȣȯ�� ���� ����� ����.
	NPC_PATTERN_HOSTILE_			,		// ���������� �ٲ��. 
	NPC_PATTERN_PEACE_				,		// ��ȭ������ �ٲ��.
	NPC_PATTERN_FIND_CLOSE_PC_		,		// ����� pc�� ã�´�.(������ �ٶ󺻴�. )
	NPC_PATTERN_FIND_WEAK_PC_		,		// ����  pc�� ã�´�.(������ �ٶ󺻴�. )
	NPC_PATTERN_FIND_STRONG_PC_		,		// ����  pc�� ã�´�.(������ �ٶ󺻴�. )
	NPC_PATTERN_ACCESS_PC_			,		// ã�� PC�� �����Ѵ�. ( Hostile�ϰ�� ���ݹ����� ������ �����Ѵ�. peace�ϰ�� �����ص� ������ �����ϴ³��� �ٶ󺻴�.  
	NPC_PATTERN_ACCESS_PC_ATTACKED_	,		// ���� ������ Pc�� �����Ѵ�. 
	NPC_PATTERN_ATTACK_PC_			,
	NPC_PATTERN_WANDER_RANGE_1_	,// ��ȸ�Ѵ�. ( 1�� ��ȸ������ 1�̴�. )
	NPC_PATTERN_WANDER_RANGE_2_	,// ��ȭ�Ѵ�. ( 1�� ��ȸ������ 2�̴�. )
	NPC_PATTERN_WANDER_RANGE_3_		,// ��ȭ�Ѵ�. ( 1�� ��ȸ������ 3�̴�. )
	NPC_PATTERN_ESCAPE_PC_			,// ã�� PC�� ���� ��������. 
	NPC_PATTERN_ESCAPE_BATTLE_PC_	,// ��ó�� ���� PC�� ������ �ָ� ��������. 
	NPC_PATTERN_RETURN_				,// patterntype�� 0���� Setting�Ѵ�.
	NPC_PATTERN_BACKDRAW_			,// �����Ϸ��� ��ڷ� 3Ÿ�� ������.
};

enum ENpcPattern
{
	eNPC_PATTERN_BEGIN,
	NPC_PATTERN_NO_MOVE_			 =eNPC_PATTERN_BEGIN ,		// ������ �ִ´�.

	eNPC8_BEGIN,
	NPC_PATTERN_IAM_BOSS_8_			=eNPC8_BEGIN,
	NPC_PATTERN_TOBOSS_8_			,
	NPC_PATTERN_MURI_8_				,
	NPC_PATTERN_WANDER_8_			,
	NPC_PATTERN_ACCESS_PC_8_		,
	NPC_PATTERN_ATTACK_FORMATION_PC_8_,
	NPC_PATTERN_ATTACK_STANBY_PC_8_,
	NPC_PATTERN_ATTACK_READY_PC_8_,
	NPC_PATTERN_ATTACK_PC_8_		,
	NPC_PATTERN_BACKDRAW_8_			,
	eNPC8_END = NPC_PATTERN_BACKDRAW_8_,

	eNPC18_BEGIN,
	NPC_PATTERN_IAM_BOSS_18_		=eNPC18_BEGIN,
	NPC_PATTERN_TOBOSS_18_			,
	NPC_PATTERN_WANDER_18_			,
	NPC_PATTERN_MURI_18_			,
	NPC_PATTERN_ACCESS_PC_18_		,
	NPC_PATTERN_ATTACK_FORMATION_PC_18_,
	NPC_PATTERN_ATTACK_STANBY_PC_18_,
	NPC_PATTERN_ATTACK_READY_PC_18_,
	NPC_PATTERN_ATTACK_PC_18_		,
	NPC_PATTERN_BACKDRAW_18_		,
	NPC_PATTERN_TAME_				,
	eNPC18_END =NPC_PATTERN_TAME_	,

	eNPC28_BEGIN,
	NPC_PATTERN_IAM_BOSS_28_		=eNPC28_BEGIN,
	NPC_PATTERN_TOBOSS_28_		,
	NPC_PATTERN_ACCESS_28_		,
	NPC_PATTERN_WANDER_28_			,
	NPC_PATTERN_MURI_28_			,
	NPC_PATTERN_BACKDRAW_PC_28_		,
	eNPC28_END =NPC_PATTERN_BACKDRAW_PC_28_,

	eNPC38_BEGIN,
	NPC_PATTERN_ACCESS_PC_38_		=eNPC38_BEGIN,
	NPC_PATTERN_ATTACK_FORMATION_PC_38_,
	NPC_PATTERN_ATTACK_STANBY_PC_38_,
	NPC_PATTERN_ATTACK_READY_PC_38_,
	NPC_PATTERN_ATTACK_PC_38_		,
	NPC_PATTERN_WANDER_38_			,
	NPC_PATTERN_MURI_38_			,
	NPC_PATTERN_BACKDRAW_PC_38_		,
	eNPC38_END = NPC_PATTERN_BACKDRAW_PC_38_,

	eNPC_GUARD_BEGIN,
	NPC_PATTERN_IAM_BOSS_GUARD_		=eNPC_GUARD_BEGIN,
	NPC_PATTERN_TOBOSS_GUARD_		,
	NPC_PATTERN_WANDER_GUARD_		,
	NPC_PATTERN_MURI_GUARD_			,
	NPC_PATTERN_ACCESS_PC_GUARD_	,
	NPC_PATTERN_ATTACK_FORMATION_PC_GUARD_,
	NPC_PATTERN_ATTACK_STANBY_PC_GUARD_,
	NPC_PATTERN_ATTACK_READY_PC_GUARD_,
	NPC_PATTERN_ATTACK_PC_GUARD_	,
	NPC_PATTERN_BACKDRAW_PC_GUARD_	,
	eNPC_GUARD_END = NPC_PATTERN_BACKDRAW_PC_GUARD_	,

	
	NPC_PATTERN_EVENTING_MAUL_		,
	NPC_PATTERN_WANDER_MAUL_		,
	NPC_PATTERN_ATTACK_FORMATION_PC_MAUL_,
	NPC_PATTERN_ATTACK_STANBY_PC_MAUL_,
	NPC_PATTERN_ATTACK_READY_PC_MAUL_,
	NPC_PATTERN_ATTACK_MAUL_		,
	NPC_PATTERN_BACKDRAW_MAUL_		,
	NPC_PATTERN_ACCESS_PC_MAUL_		,
	NPC_PATTERN_MAX
};

struct sNPCgeneratePosition
{
	short int curnpc;
	short int npcindex;
	short int eventno;
	short int x, y;
	short int maxno;
};

enum ENpcAni
{
	eMON1_NONE ,		// �⺻����
	eMON1_BATTLE_NONE,	// �����⺻
	eMON1_ATTACKED,		// �±�
	eMON1_DIE,			// �ױ�....
	eMON1_BATTLE_WALK_,	// �⺻�ȱ�
	eMON1_BATTLE_ATTACK, // �Ѽհ���
	eMAX_ENpcAni
};

#endif //__jNpcEnum_h__