// StepMigration.cpp: implementation of the CStepMigration class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "StepMigration.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CStepMigration::CStepMigration()
{
	m_vtBuffer.clear();
}

CStepMigration::~CStepMigration()
{
	m_vtBuffer.clear();
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CStepMigration::Load(char* pData)
{	// 데이타베이스에서 문자열 저장은 숫자 0을 NULL로 인식하므로 1씩 빼서 로드
	char aBuffer[MAX_STEP];
	memcpy(aBuffer, pData, MAX_STEP);

	for (int i = 0; i < MAX_STEP; ++i)
	{
		*(pData + i) -= 1;
	}

	memcpy(aBuffer, pData, MAX_STEP);
	CheckStepInfo(pData);
}

void CStepMigration::Save(char* pData)
{	// 데이타베이스에서 문자열 저장은 숫자 0을 NULL로 인식하므로 1씩 더해서 저장
	CheckStepInfo(pData);

	char aBuffer[MAX_STEP];
	memcpy(aBuffer, pData, MAX_STEP);

	for (int i = 0; i < MAX_STEP; ++i)
	{
		*(pData + i) += 1;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Protected Method
///////////////////////////////////////////////////////////////////////////////

void CStepMigration::CheckStepInfo(char* pData)
{	// 캐릭터의 단계 정보 검사
	m_vtBuffer.assign(pData, pData + MAX_STEP);
	
	CheckClassStep(CLS_STEP);
	CheckDualClass(DUAL_CLS);
	CheckCombatPoint(CSP_STEP);
	CheckExpStep(EXP_STEP);

	for (int i = T01_STEP; i <= T13_STEP; ++i)
	{
		CheckTacStep(i);
	}

	copy(m_vtBuffer.begin(), m_vtBuffer.end(), pData);
	*(pData + MAX_STEP) = 0;

	m_vtBuffer.clear();
}

///////////////////////////////////////////////////////////////////////////////
// Private Method
///////////////////////////////////////////////////////////////////////////////

void CStepMigration::CheckClassStep(int nStepType)
{	// 클래스 단계 검사
	const char cStep = m_vtBuffer[nStepType];

	if (cStep >= 0 && cStep <= 5)
	{
		return;
	}

	MyLog(LOG_NORMAL, "[%d]Class Step : %d", nStepType, cStep);
	m_vtBuffer[nStepType] = 0;
}

void CStepMigration::CheckDualClass(int nStepType)
{	// 듀얼 클래스 종류 검사
	const char cStep = m_vtBuffer[nStepType];

	if (cStep >= WARRIOR && cStep <= PRIEST)
	{
		return;
	}

	MyLog(LOG_NORMAL, "[%d]Dual Class : %d", nStepType, cStep);
	m_vtBuffer[nStepType] = WARRIOR;
}

void CStepMigration::CheckExpStep(int nStepType)
{	// 경험치 단계 검사
	const char cStep = m_vtBuffer[nStepType];

	if (cStep >= 0 && cStep <= NPC_Lev_Ref[MAX_EXP_LEVEL].nStep)
	{	// 데이타베이스에서 저장이 되지 않는 문자
		switch (cStep)
		{
		case 31:
			{
				m_vtBuffer[nStepType] = 32;
				break;
			}
		case 38:
			{
				m_vtBuffer[nStepType] = 39;
				break;
			}
		}
		
		return;
	}

	MyLog(LOG_NORMAL, "[%d]Experience Step : %d", nStepType, cStep);
	m_vtBuffer[nStepType] = 0;
}

void CStepMigration::CheckTacStep(int nStepType)
{	// 경험치 단계 검사
	const char cStep = m_vtBuffer[nStepType];
	
	if (cStep >= 0 && cStep <= NPC_Lev_Ref[MAX_TAC_LEVEL].nStep)
	{	// 데이타베이스에서 저장이 되지 않는 문자
		switch (cStep)
		{
		case 31:
			{
				m_vtBuffer[nStepType] = 32;
				break;
			}
		case 38:
			{
				m_vtBuffer[nStepType] = 39;
				break;
			}
		}
		
		return;
	}

	MyLog(LOG_NORMAL, "[%d]Tactic Step : %d", nStepType, cStep);
	m_vtBuffer[nStepType] = 0;
}

void CStepMigration::CheckCombatPoint(int nStepType)
{	// 전투스킬 포인터 검사
	const char cStep = m_vtBuffer[nStepType];

	if (cStep >= 0 && cStep <= NPC_Lev_Ref[MAX_EXP_LEVEL].nCspMax)
	{
		return;
	}

	MyLog(LOG_NORMAL, "[%d]Combat Point : %d", nStepType, cStep);
	m_vtBuffer[nStepType] = 0;
}