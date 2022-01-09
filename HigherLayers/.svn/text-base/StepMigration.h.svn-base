// StepMigration.h: interface for the CStepMigration class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STEPMIGRATION_H__B536FC5C_83D1_450D_A21F_BA83C2EAA7E7__INCLUDED_)
#define AFX_STEPMIGRATION_H__B536FC5C_83D1_450D_A21F_BA83C2EAA7E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////
//
class CStepMigration  
{
public:
	CStepMigration();
	virtual ~CStepMigration();

public:
	void Load(char* pData);
	void Save(char* pData);

protected:
	void CheckStepInfo(char* pData);

private:
	void CheckClassStep(int nStepType);
	void CheckDualClass(int nStepType);
	void CheckExpStep(int nStepType);
	void CheckTacStep(int nStepType);
	void CheckCombatPoint(int nStepType);

private:
	vector<char> m_vtBuffer;
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_STEPMIGRATION_H__B536FC5C_83D1_450D_A21F_BA83C2EAA7E7__INCLUDED_)
