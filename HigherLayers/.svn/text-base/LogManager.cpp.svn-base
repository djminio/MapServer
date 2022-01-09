// LogManager.cpp: implementation of the CLogManager class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "LogManager.h"

#include <direct.h>

#define BEGIN_SELECT(e) switch (e) {
#define CASE_SELECT(e)  case(e): return CS(e);
#define END_SELECT(e)   } return CS(e);

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CLogManager::CLogManager()
{
	m_mpBuffer.clear();
}

CLogManager::~CLogManager()
{
	m_mpBuffer.clear();
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CLogManager::CreateMainPath(const char* pPath)
{	// 기본 디렉토리 생성
	_mkdir(pPath);

	time_t nowTime;
	time(&nowTime);

	tm* pNow = localtime(&nowTime);
	const int nYear = pNow->tm_year + 1900;
	const int nMon = pNow->tm_mon + 1;
	// 월별 디렉토리 생성
	char szBuffer[MAX_PATH];
	memset(szBuffer, 0, MAX_PATH);
	sprintf_s(szBuffer, sizeof(szBuffer),"%s\\%04d_%02d", pPath, nYear, nMon);
	_mkdir(szBuffer);
	//
	m_strRoot = szBuffer;
}

void CLogManager::CreateSubPath(int nType, const char* pPath)
{
	string strTemp = m_strRoot;
	strTemp += "\\";
	strTemp += pPath;
	_mkdir(strTemp.c_str());
	//
	m_mpBuffer.insert(PAIR_LOG(nType, strTemp));
}

void CLogManager::SaveDupe(CHARLIST* pTarget)
{	//< CSD-030808
	string strPath;

	if (!GetLogPath(LT_DUPE, strPath))
	{
		return;
	}
	//< CSD-040224
	FILE* pFile = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");

	if(pFile == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, pTarget->Name);
		pFile = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (pFile == NULL)
	{
		pFile = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pTarget->GetServerID()].id), "at+");

		if (pFile == NULL)
		{
			return;
		}
	}
	//> CSD-040224
	fprintf(pFile, "[%04d.%02d.%02d %02d:%02d:%02d]", g_year, g_mon + 1, g_day, g_hour, g_min, g_sec);
	fprintf(pFile, "MapName(%s), IP(%s), Name(%s)\n", connections[pTarget->GetServerID()].mapname, 
		                                              connections[pTarget->GetServerID()].ip_address,
													  connections[pTarget->GetServerID()].name);
	fclose(pFile);
}	//> CSD-030808

void CLogManager::SaveTeleport(CHARLIST* pTarget, int newx, int newy)
{	//< CSD-030808
	string strPath;

	if (!GetLogPath(LT_TELEPORT, strPath))
	{
		return;
	}
	//< CSD-040224
	FILE* pFile = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");

	if(pFile == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, pTarget->Name);
		pFile = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (pFile == NULL)
	{
		pFile = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pTarget->GetServerID()].id), "at+");

		if (pFile == NULL)
		{
			return;
		}
	}
	//> CSD-040224
	fprintf(pFile, "[%04d.%02d.%02d %02d:%02d:%02d]\n", g_year, g_mon + 1, g_day, g_hour, g_min, g_sec);
	fprintf(pFile, "MapName(%s), IP(%s), Name(%s), OldX(%d), OldY(%d), NewX(%d) NewY(%d)\n", 
													  connections[pTarget->GetServerID()].mapname, 
		                                              connections[pTarget->GetServerID()].ip_address,
													  connections[pTarget->GetServerID()].name,
													  pTarget->OldMoveSx, pTarget->OldMoveSy,
													  newx, newy);
	fprintf(pFile, "MoveLength(%d), MovePathCount(%d), MoveType(%d)\n\n",
													  pTarget->MoveLength,
													  pTarget->MovePathCount,
													  pTarget->MoveType);
	fclose(pFile);
}	//> CSD-030808

void CLogManager::SaveEtc(CHARLIST* pTarget, char *info)
{	//< CSD-030808
	string strPath;

	if (!GetLogPath(LT_ETC, strPath))
	{
		return;
	}
	//< CSD-040224
	FILE* pFile = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");

	if(pFile == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, pTarget->Name);
		pFile = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (pFile == NULL)
	{
		pFile = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pTarget->GetServerID()].id), "at+");

		if (pFile == NULL)
		{
			return;
		}
	}
	//> CSD-040224
	fprintf(pFile, "[%04d.%02d.%02d %02d:%02d:%02d]", g_year, g_mon + 1, g_day, g_hour, g_min, g_sec);
	fprintf(pFile, "Map: %s (%d,%d), IP: %s, Name: %s Id: %s Info: %s\n", pTarget->MapName,
														pTarget->X,
													  pTarget->Y,
		                                              connections[pTarget->GetServerID()].ip_address,
													  pTarget->Name,
													  connections[pTarget->GetServerID()].id,
													  info);
	fclose(pFile);
}	//> CSD-030808

void CLogManager::SaveAttackRange(CHARLIST* pTarget, char* info)
{	//< CSD-030808
	string strPath;

	if (!GetLogPath(LT_ATTACKRANGE, strPath))
	{
		return;
	}
	//< CSD-040224
	FILE* pFile = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");

	if(pFile == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, pTarget->Name);
		pFile = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (pFile == NULL)
	{
		pFile = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pTarget->GetServerID()].id), "at+");

		if (pFile == NULL)
		{
			return;
		}
	}
	//> CSD-040224
	fprintf(pFile, "[%04d.%02d.%02d %02d:%02d:%02d]", g_year, g_mon + 1, g_day, g_hour, g_min, g_sec);
	fprintf(pFile, "Map: %s (%d,%d), IP: %s, Name: %s Id: %s Info: %s\n", pTarget->MapName,
														pTarget->X,
													  pTarget->Y,
		                                              connections[pTarget->GetServerID()].ip_address,
													  pTarget->Name,
													  connections[pTarget->GetServerID()].id,
													  info);
	fclose(pFile);
}	//> CSD-030808

void CLogManager::SaveTimeChecks(CHARLIST* pTarget, char *info)
{	//< CSD-030808
	string strPath;

	if (!GetLogPath(LT_TIMECHECKS, strPath))
	{
		return;
	}
	//< CSD-040224
	FILE* pFile = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");

	if(pFile == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, pTarget->Name);
		pFile = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (pFile == NULL)
	{
		pFile = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pTarget->GetServerID()].id), "at+");

		if (pFile == NULL)
		{
			return;
		}
	}
	//> CSD-040224
	fprintf(pFile, "[%04d.%02d.%02d %02d:%02d:%02d]", g_year, g_mon + 1, g_day, g_hour, g_min, g_sec);
	fprintf(pFile, "Map: %s (%d,%d), IP: %s, Name: %s Id: %s Info: %s\n", pTarget->MapName,
														pTarget->X,
													  pTarget->Y,
		                                              connections[pTarget->GetServerID()].ip_address,
													  pTarget->Name,
													  connections[pTarget->GetServerID()].id,
													  info);
	fclose(pFile);
}	//> CSD-030808

void CLogManager::SaveTileChecks(CHARLIST* pTarget, char *info)
{	//< CSD-030808
	string strPath;

	if (!GetLogPath(LT_TILECHECKS, strPath))
	{
		return;
	}
	//< CSD-040224
	FILE* pFile = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");

	if(pFile == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, pTarget->Name);
		pFile = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (pFile == NULL)
	{
		pFile = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pTarget->GetServerID()].id), "at+");

		if (pFile == NULL)
		{
			return;
		}
	}
	//> CSD-040224
	fprintf(pFile, "[%04d.%02d.%02d %02d:%02d:%02d]", g_year, g_mon + 1, g_day, g_hour, g_min, g_sec);
	fprintf(pFile, "Map: %s (%d,%d), IP: %s, Name: %s Id: %s Info: %s\n", pTarget->MapName,
														pTarget->X,
													  pTarget->Y,
		                                              connections[pTarget->GetServerID()].ip_address,
													  pTarget->Name,
													  connections[pTarget->GetServerID()].id,
													  info);
	fclose(pFile);
}	//> CSD-030808

void CLogManager::SaveStrikeAttack(CHARLIST* pCaster, CHARLIST* pTarget)
{	//< CSD-030804
	return;

	string strPath;

	if (!GetLogPath(LT_CHECK, strPath))
	{
		return;
	}
	//< CSD-040224
	FILE* pFile = fopen(VA("%s/%s.txt", strPath.c_str(), pCaster->Name), "at+");

	if(pFile == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, pCaster->Name);
		pFile = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (pFile == NULL)
	{
		pFile = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pCaster->GetServerID()].id), "at+");

		if (pFile == NULL)
		{
			return;
		}
	}
	//> CSD-040224
	fprintf(pFile, "[%04d.%02d.%02d %02d:%02d:%02d]", g_year, g_mon + 1, g_day, g_hour, g_min, g_sec);
	
	const int nCombat = pCaster->GetActiveCombat();
	bitset<MAX_CLASS> bsClass(Magic_Ref[nCombat].nClass);
	
	const int nClass = pCaster->Class;

	if (!bsClass[nClass])
	{
		fprintf(pFile, "Strike Attack, ");
	}
	else
	{
		fprintf(pFile, "Strike(%s), ", Magic_Ref[nCombat].Han_Name);
	}

	fprintf(pFile, "%s(%3d, %3d), ", MapName, pCaster->MoveSx, pCaster->MoveSy);
	fprintf(pFile, "%s(%d, %d), ", GetTargetName(pTarget), pTarget->MoveSx, pTarget->MoveSy);
	fprintf(pFile, "exp(%d/%d/%d), ", pCaster->GetExperienceStep(), pCaster->GetLevel(), pCaster->Exp); // CSD-030806

	const int nKind = pCaster->GetTacticsKind();
	fprintf(pFile, "%s(%d/%d/%d)\n", ConvertToTactics(nKind),
		                             pCaster->GetTacticsStep(nKind - TACTICS_Crapple),
					  			     pCaster->Skill[nKind],
									 pCaster->tac_skillEXP[nKind - TACTICS_Crapple]);
	fclose(pFile);
}	//> CSD-030804

void CLogManager::SaveThrowAttack(CHARLIST* pCaster, CHARLIST* pTarget)
{	//< CSD-030804
	return;

	string strPath;

	if (!GetLogPath(LT_CHECK, strPath))
	{
		return;
	}
	//< CSD-040224
	FILE* pFile = fopen(VA("%s/%s.txt", strPath.c_str(), pCaster->Name), "at+");

	if(pFile == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, pCaster->Name);
		pFile = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (pFile == NULL)
	{
		pFile = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pCaster->GetServerID()].id), "at+");

		if (pFile == NULL)
		{
			return;
		}
	}
	//> CSD-040224
	fprintf(pFile, "[%04d.%02d.%02d %02d:%02d:%02d]", g_year, g_mon + 1, g_day, g_hour, g_min, g_sec);
	
	const int nCombat = pCaster->GetActiveCombat();
	bitset<MAX_CLASS> bsClass(Magic_Ref[nCombat].nClass);
	
	const int nClass = pCaster->Class;

	if (!bsClass[nClass])
	{
		fprintf(pFile, "Throw Attack, ");
	}
	else
	{
		fprintf(pFile, "Throw(%s), ", Magic_Ref[nCombat].Han_Name);
	}

	fprintf(pFile, "%s(%d, %d), ", MapName, pCaster->MoveSx, pCaster->MoveSy);
	fprintf(pFile, "%s(%d, %d), ", GetTargetName(pTarget), pTarget->MoveSx, pTarget->MoveSy);
	fprintf(pFile, "exp(%d/%d/%d), ", pCaster->GetExperienceStep(), pCaster->GetLevel(), pCaster->Exp); // CSD-030806

	const int nKind = pCaster->GetTacticsKind();
	fprintf(pFile, "%s(%d/%d/%d)\n", ConvertToTactics(nKind),
		                             pCaster->GetTacticsStep(nKind - TACTICS_Crapple),
					  			     pCaster->Skill[nKind],
									 pCaster->tac_skillEXP[nKind - TACTICS_Crapple]);
	fclose(pFile);
}	//> CSD-030804

void CLogManager::SaveMagicExecute(BYTE nMagic, CHARLIST* pCaster, CHARLIST* pTarget, int nX, int nY)
{	//< CSD-030804
	return;

	string strPath;

	if (!GetLogPath(LT_CHECK, strPath))
	{
		return;
	}	
	//< CSD-040224
	FILE* pFile = fopen(VA("%s/%s.txt", strPath.c_str(), pCaster->Name), "at+");

	if(pFile == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, pCaster->Name);
		pFile = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (pFile == NULL)
	{
		pFile = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pCaster->GetServerID()].id), "at+");

		if (pFile == NULL)
		{
			return;
		}
	}
	//> CSD-040224
	fprintf(pFile, "[%04d.%02d.%02d %02d:%02d:%02d]", g_year, g_mon + 1, g_day, g_hour, g_min, g_sec);
	fprintf(pFile, "%s, ", Magic_Ref[nMagic].Han_Name);
	fprintf(pFile, "%s(%3d, %3d), ", MapName, pCaster->MoveSx, pCaster->MoveSy);
	fprintf(pFile, "%s(%d, %d), ", GetTargetName(pTarget), nX/32, nY/32);
	fprintf(pFile, "exp(%d/%d/%d), ", pCaster->GetExperienceStep(), pCaster->GetLevel(), pCaster->Exp); // CSD-030806

	int nKind = TACTICS_Crapple;

	switch (pCaster->Spell)
	{
	case WIZARD_SPELL: nKind = TACTICS_Magery; break;
	case PRIEST_SPELL: nKind = TACTICS_Orison; break;
	}
	
	fprintf(pFile, "%s(%d/%d/%d)\n", ConvertToTactics(nKind),
		                             pCaster->GetTacticsStep(nKind - TACTICS_Crapple),
					  			     pCaster->Skill[nKind],
									 pCaster->tac_skillEXP[nKind - TACTICS_Crapple]);
	fclose(pFile);
}	//> CSD-030804

void CLogManager::SaveCombatExecute(BYTE nCombat, CHARLIST* pCaster, CHARLIST* pTarget, int nX, int nY)
{	//< CSD-030804
	return;

	string strPath;

	if (!GetLogPath(LT_CHECK, strPath))
	{
		return;
	}
	//< CSD-040224
	FILE* pFile = fopen(VA("%s/%s.txt", strPath.c_str(), pCaster->Name), "at+");

	if(pFile == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, pCaster->Name);
		pFile = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (pFile == NULL)
	{
		pFile = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pCaster->GetServerID()].id), "at+");

		if (pFile == NULL)
		{
			return;
		}
	}
	//> CSD-040224
	fprintf(pFile, "[%04d.%02d.%02d %02d:%02d:%02d]", g_year, g_mon + 1, g_day, g_hour, g_min, g_sec);
	fprintf(pFile, "%s(%d), ", Magic_Ref[nCombat].Han_Name, Magic_Ref[nCombat].nCombatCount);
	fprintf(pFile, "%s(%3d, %3d), ", MapName, pCaster->MoveSx, pCaster->MoveSy);
	fprintf(pFile, "%s(%d, %d), ", GetTargetName(pTarget), nX/32, nY/32);
	fprintf(pFile, "exp(%d/%d/%d), ", pCaster->GetExperienceStep(), 
									  pCaster->GetLevel(),
									  pCaster->Exp); // CSD-030806
	const int nKind = pCaster->GetTacticsKind();
	fprintf(pFile, "%s(%d/%d/%d)\n", ConvertToTactics(nKind),
		                             pCaster->GetTacticsStep(nKind - TACTICS_Crapple),
					  			     pCaster->Skill[nKind],
									 pCaster->tac_skillEXP[nKind - TACTICS_Crapple]);
	fclose(pFile);
}	//> CSD-030804

void CLogManager::SaveAccelType(CHARLIST* pTarget, const char* pType)
{	//< CSD-030808
	string strPath;

	if (!GetLogPath(LT_ACCELATER, strPath))
	{
		return;
	}
	//< CSD-040224
	FILE* pFile = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");

	if(pFile == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, pTarget->Name);
		pFile = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (pFile == NULL)
	{
		pFile = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pTarget->GetServerID()].id), "at+");

		if (pFile == NULL)
		{
			return;
		}
	}
	//> CSD-040224
	fprintf(pFile, "[%04d.%02d.%02d %02d:%02d:%02d]", g_year, g_mon + 1, g_day, g_hour, g_min, g_sec);
	fprintf(pFile, "MapName(%s), IP(%s), Type(%s)\n", connections[pTarget->GetServerID()].mapname, 
		                                              connections[pTarget->GetServerID()].ip_address,
													  pType);
	fclose(pFile);
}	//> CSD-030808

void CLogManager::SaveDeadEventNpc(CHARLIST* pTarget)
{	//< CSD-031013
	string strPath;

	if (!GetLogPath(strPath))
	{
		return;
	}
	
	const char* pPath = VA("%s\\DeadEventNpc.txt", strPath.c_str());
	FILE* pFile = fopen(pPath, "at+");
	
	if (pFile == NULL)
	{
		return;
	}

	fprintf(pFile, "[%04d.%02d.%02d %02d:%02d:%02d]", g_year, g_mon + 1, g_day, g_hour, g_min, g_sec);
	fprintf(pFile, "%s(%3d, %3d), EventNo(%d), Killer(%s)\n", MapName, 
		                                                      pTarget->MoveSx, 
															  pTarget->MoveSy, 
															  pTarget->eventno,
															  pTarget->KillerName);
	fclose(pFile);
}	//> CSD-031013

void CLogManager::SaveSealStoneInfo(CHARLIST* pSource, CHARLIST* pTarget, int nDamage)
{	//< CSD-040316
	if (nDamage <= 0)
	{
		return;
	}

	string strPath;

	if (!GetLogPath(strPath))
	{
		return;
	}
	
	// 040331-YGI
	const char* pPath = VA("%s\\%d%02d%02d%02d_SealStone.txt", strPath.c_str(), g_MapPort, g_year-2000, g_mon+1, g_day);
	FILE* pFile = fopen(pPath, "at+");

	if (pFile == NULL)
	{
		return;
	}

	fprintf(pFile, "[%04d.%02d.%02d %02d:%02d:%02d]", g_year, g_mon + 1, g_day, g_hour, g_min, g_sec);
	fprintf(pFile, "%s(%3d, %3d), %s(%3d, %3d), Damage(%d)\n", pSource->Name,
		                                                       pSource->X,
											                   pSource->Y, 
															   pTarget->Name,
		                                                       pTarget->MoveSx,
											                   pTarget->MoveSy, 
												               nDamage);
	fclose(pFile);
}	//> CSD-040316

void CLogManager::SaveLocalWarInfo(CHARLIST* pTarget)
{	//< CSD-040407
	string strPath;

	if (!GetLogPath(strPath))
	{
		return;
	}
	
	// 040331-YGI
	const char* pPath = VA("%s\\%02d%02d%02d.txt", strPath.c_str(), g_year - 2000, g_mon + 1, g_day);
	
	FILE* pFile = fopen(pPath, "at+");

	if (pFile == NULL)
	{
		return;
	}

	string strNation;

	switch (pTarget->name_status.nation)
	{
	case NW_BY:	
		{
			strNation = "NW_BY";
			break;
		}
	case NW_ZY:	
		{
			strNation = "NW_ZY";
			break;
		}
	case NW_YL:	
		{
			strNation = "NW_YL";
			break;
		}
	default:
		{
			strNation = "UNKNOWN";
			break;
		}
	}

	fprintf(pFile, "[%04d.%02d.%02d %02d:%02d:%02d]", g_year, g_mon + 1, g_day, g_hour, g_min, g_sec);
	fprintf(pFile, "Nation(%s), Map(%d), %s(%3d, %3d)\n", strNation.c_str(),
		                                                  g_MapPort,
										                  pTarget->Name,
		                                                  pTarget->X,
											              pTarget->Y);
	fclose(pFile);
}	//> CSD-040407

const char* CLogManager::ConvertToDay(int nDay) const
{	//< CSD-030804
	BEGIN_SELECT(nDay)
		CASE_SELECT(SUNDAY)
		CASE_SELECT(MONDAY)
		CASE_SELECT(TUESDAY)
		CASE_SELECT(WEDNESDAY)
		CASE_SELECT(THURSDAY)
		CASE_SELECT(FRIDAY)
		CASE_SELECT(SATURDAY)
	END_SELECT(UNKNOWNDAY)
}	//> CSD-030804

const char* CLogManager::ConvertToTactics(int nKind) const
{	//< CSD-030804
	BEGIN_SELECT(nKind)
		CASE_SELECT(TACTICS_Crapple)
		CASE_SELECT(TACTICS_swordmanship)
		CASE_SELECT(TACTICS_Archery)
		CASE_SELECT(TACTICS_Fencing)
		CASE_SELECT(TACTICS_Mace_fighting)
		CASE_SELECT(TACTICS_Pierce)
		CASE_SELECT(TACTICS_Whirl)
		CASE_SELECT(TACTICS_Hurl)
		CASE_SELECT(TACTICS_Parrying)
		CASE_SELECT(TACTICS_Double_Swordmanship)
		CASE_SELECT(TACTICS_Double_MaceFighting)
		CASE_SELECT(TACTICS_Magery)
		CASE_SELECT(TACTICS_Orison)
	END_SELECT(SKILL_UNKNOWN)
}	//> CSD-030804

///////////////////////////////////////////////////////////////////////////////
// Private Method
///////////////////////////////////////////////////////////////////////////////

const char* CLogManager::GetTargetName(CHARLIST* pTarget) const
{	//< CSD-030804
	if (pTarget->IsPlayer())
	{
		return pTarget->Name;
	}

	return NPC_Gen_Ref[pTarget->npc_index].Name;
}	//> CSD-030804

//<! BBD 040226	옥션로그를 위해생성한 함수
///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////
void CLogManager::SaveAutionLog(int type, t_SellerItemDelete *pRecordInfo)
{

	// 로그 타입 분류
	char szType[64];
	switch(type)
	{
	case ACLT_SELLER_REGIST:
		sprintf_s(szType, sizeof(szType), "<<SELLER_REGIST>>");
		break;
	case ACLT_SELLER_CANCLE:
		sprintf_s(szType, sizeof(szType), "<<SELLER_CANCLE>>");
		break;
	case ACLT_BUYER_TAKE:
		sprintf_s(szType, sizeof(szType), "<<BUYER_TAKE>>");
		break;
	case ACLT_SELLER_TAKE:
		sprintf_s(szType, sizeof(szType), "<<SELLER_TAKE>>");
		break;
	default:
		return;
	}

	// 타입이 정상임
	string strPath;

	if (!GetLogPath(LT_AUCTION, strPath))
	{
		return;
	}

	// 경로에 파일을 연다
	const char* pPath = VA("%s\\AuctionLog_%04d_%02d_%02d.txt", strPath.c_str(), g_year, g_mon + 1, g_day);
	FILE* pFile = fopen(pPath, "at+");
	
	if (pFile == NULL)
	{
		return;
	}


	//로그를 한줄씩 남긴다
	fprintf(pFile, "[%04d.%02d.%02d %02d:%02d:%02d]\t", g_year, g_mon + 1, g_day, g_hour, g_min, g_sec);

	// 지저분하니깐 여기서 받아오자
	char * Seller = pRecordInfo->szSellerName;
	char * Buyer = pRecordInfo->szName;
	int money = pRecordInfo->dwSellValue;
	int item_no = pRecordInfo->SellItem.item_no;
	int limit = pRecordInfo->SellItem.attr[1];
	RareMain *pRare = (RareMain *)&(pRecordInfo->SellItem.attr[3]);

	// 본격적으로 로그를 남겨보자
	fprintf(pFile, "Type:%s\tSeller:%s\tBuyer:%s\tMoney:%d\tItemNo:%d\tLimit:%d\tGrade:%d\tSok1:%d\tSok2:%d\tSok3:%d\tHigh:%d\tDynamic:%d\t\n",
		szType, Seller, Buyer, money, item_no, limit, pRare->grade, pRare->soksung1, pRare->soksung2, pRare->soksung3, pRare->iHighLevel, pRare->IsDynamicRare);

	fclose(pFile);
}
//> BBD 040226	옥션로그를 위해생성한 함수

//<! BBD 040308 이벤트 아이템 로스트 로그
// 맵서버가 이벤트 아이템 지급에 실패했을때 남기는 로그
void CLogManager::SaveEventItemLostLog(int type, char * szName, int nIndex)
{
	// 로그 타입 분류
	char szReason[64];
	switch(type)
	{
	case EILT_INVALID_CONNECTION:
		sprintf_s(szReason, sizeof(szReason), "<<Disconnected>>");
		break;
	case EILT_INVALID_NAME:
		sprintf_s(szReason, sizeof(szReason), "<<Invalid name>>");
		break;
	case EILT_NOTENOUGH_INVENTORY:
		sprintf_s(szReason, sizeof(szReason), "<<Not Enough Inven>>");
		break;
	default:
		sprintf_s(szReason, sizeof(szReason), "");
		break;
	}

	string strPath;
	if (!GetLogPath(LT_EVENTITEM, strPath))
	{
		return;
	}

	// 경로에 파일을 연다
	const char* pPath = VA("%s\\EventItemLog_%04d_%02d_%02d.txt", strPath.c_str(), g_year, g_mon + 1, g_day);
	FILE* pFile = fopen(pPath, "at+");
	
	if (pFile == NULL)
	{
		return;
	}

	//로그를 한줄씩 남긴다
	fprintf(pFile, "[%04d.%02d.%02d %02d:%02d:%02d]\t", g_year, g_mon + 1, g_day, g_hour, g_min, g_sec);

	fprintf(pFile, "Reason : %s\tName : %s\tItemIndex : %d\t\n", szReason, szName, nIndex);
	
	fclose(pFile);
}
//> BBD 040308 이벤트 아이템 로스트 로그

inline void RewritePathWithSpecialCharacters( char *file, char *charname) //Eleval 16/08/09 - for chars who's name has special chars
{
	char newCharName[64] = "";

	for(int i = 0; charname[i] != '\0' ; i++)
	{
		if(charname[i] != 92 && charname[i] != 47 && charname[i] != 58 && charname[i] != 42 && charname[i] != 63 && charname[i] != 34 && charname[i] != 60 && charname[i] != 62 && charname[i] != 124)
		{
			char temp[8] = "";
			sprintf(temp, "%c", charname[i]);
			strcat(newCharName, temp);
		}
		else
		{
			char SpecialChar[16] = "";
			sprintf(SpecialChar, "%c%d", 37, charname[i]);
			strcat(newCharName, SpecialChar);
		}
	}

	sprintf(file, "%s", newCharName);
}