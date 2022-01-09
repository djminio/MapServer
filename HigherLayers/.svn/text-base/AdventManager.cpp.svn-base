// AdventManager.cpp: implementation of the CAdventManager class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "AdventManager.h"

#include "..\LowerLayers\Servertable.h"		
#include "Dr_Network.h"
#include "UserManager.h"

extern HENV					hEnv;		
extern HDBC					hDBC;

int LoadGeneration()
{
	if (!g_pAdventManager->LoadGenerationData()) return FALSE;
	if (!g_pAdventManager->LoadGroupInfoData())
	{
		JustMsg("Event Monster Table was Loaded But EM_GroupInfo Not Loaded!!");
		return FALSE;
	}

	return TRUE;
}

void UpdateEventMonster()
{
	g_pAdventManager->UpdateEventMonster();		// LTS AI2
}

void CheckBossAndDeActiveGroupNo(CHARLIST* Npc)
{
	g_pAdventManager->CheckBossAndDeActiveGroupNo(Npc);
}

int CheckActivedGroup()
{
	return g_pAdventManager->CheckActivedGroup();
}

void ReloadEventMonsterData()
{
	if (g_pAdventManager->LoadGenerationData())
	{
		MyLog(0,"Event Monster Generation Data LoadComplete!!");
	}
	else
	{
		MyLog(0,"FAILURE!!!  Event Monster Generation Data Load!!");
	}
	if (g_pAdventManager->LoadGroupInfoData())
	{
		MyLog(0,"Event Monster Group Data LoadComplete!!");
	}
	else
	{
		MyLog(0,"FAILURE!!  Event Monster Group Data LoadComplete!!");
	}
}

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CAdventManager::CAdventManager()
{
	m_iDataCount=0;												// Table Row Count
	memset(&m_piActived,0,MAX_GROUP_NO*sizeof(int));			// Actived Check
	memset(&m_piGroupNo,0,MAX_GROUP_NO*sizeof(int));			// GroupNo Save
	memset(&m_piGroupDataCount,0,MAX_GROUP_NO*sizeof(int));		// GroupData Count Save
	m_pGPos=NULL;												// Table Data
	m_iGroupCount=0;
	ClearGroupInfoData();
}

CAdventManager::~CAdventManager()
{
	ClearGenerationData();
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CAdventManager::ClearGenerationData()			// 클리어 오류남
{
/*	if (m_pGPos)
	{
		for (int i=0;i<m_iGroupCount;i++)
		{
			if (m_pGPos[i])
			{
//				for(int j=0;j<m_piGroupDataCount[i];j++)
//				{
					delete [] m_pGPos[i];
					//&m_pGPos[i][j]=NULL;
//				}
				m_pGPos[i]=NULL;
			}
		}
		delete [] m_pGPos;
		m_pGPos=NULL;
	}*/
}

void CAdventManager::ClearGroupInfoData()
{
	for (int i=0;i<MAX_GROUP_NO;i++)
	{
		m_tGroupInfo[i].ExecType=0;
		m_tGroupInfo[i].DayofWeek=0;
		m_tGroupInfo[i].DHour=0;
		m_tGroupInfo[i].KilledGroup=0;
	}
}

int CAdventManager::GetDataCount()
{
	return m_iDataCount;
}

int CAdventManager::LoadGenerationData()
{
	HSTMT	hStmt=NULL;
	RETCODE	ret;
	char	query_stmt[MAX_PATH]={0,};
	SDWORD	cbValue;
	int		RowCount;
	int		tGroupNo;
	int		tGroupDataCount;

	
	if (m_pGPos)								// 이미 데이터가 있다면 프리해준다..
	{
		ClearGenerationData();
	}

	SERVER_DATA *pData = g_pServerTable->GetOwnServerData();

	SQLAllocStmt(hDBC,&hStmt);

	wsprintf(query_stmt,"select DISTINCT GroupNo from Event_Monster where Map_Port=%d",pData->wPort);   

	ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
 
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"Event_Monster Table Query Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return FALSE;
	}

	ret=SQLFetch(hStmt);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"Event_Monster Table Fetch Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	m_iGroupCount=0;
	while (ret==SQL_SUCCESS)
	{
		ret=SQLGetData(hStmt,1,SQL_C_SLONG,&m_piGroupNo[m_iGroupCount],sizeof(int),&cbValue);		// 몇개의 그룹이 있는지 확인
		if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
		{
			MyLog(0,"Event_Monster Table SQL Return Error(%d)!!",ret);
			SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}                
		m_iGroupCount++;
		if (m_iGroupCount>=MAX_GROUP_NO)
		{
			MyLog(0,"Event_Monster Group Count Error!!");
			SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}
		ret=SQLFetch(hStmt);
	}

	m_pGPos=new NPCGenerationPos*[m_iGroupCount];			// 이차원 카운터 설정			// 그룹의 수만큼 자리를 할당한다.
	if (!m_pGPos)
	{
		MyLog(0,"Event_Monster Data Memory Alloc Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	int i;
	for (i=0;i<m_iGroupCount;i++)	//초기화 
	{
		m_pGPos[i]=NULL;
	}


	SQLFreeStmt(hStmt,SQL_DROP);

	m_iDataCount=0;

	for (i=0;i<m_iGroupCount;i++)
	{
		SQLAllocStmt(hDBC,&hStmt);
		tGroupNo=m_piGroupNo[i];
		wsprintf(query_stmt,"select count(Map_Port) as [RowCount] from Event_Monster where Map_Port=%d and GroupNo=%d",pData->wPort,tGroupNo);   
		ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
 
		if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
		{
			MyLog(0,"Event_Monster Table Query Error!!");
			SQLFreeStmt(hStmt,SQL_DROP);
			return FALSE;
		}

		ret=SQLFetch(hStmt);
		if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
		{
			MyLog(0,"Event_Monster Table Fetch Error!!");
			SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}
		ret=SQLGetData(hStmt,1,SQL_C_SLONG,&RowCount,sizeof(int),&cbValue);		// 그룹에 몇개의 데이터가 있는지 확인
		if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
		{
			MyLog(0,"Event_Monster Table SQL Return Error(%d)!!",ret);
			SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}                

		m_pGPos[tGroupNo]=new NPCGenerationPos[RowCount];								// 그룹데이터를 위한 공간 
		if (!m_pGPos[tGroupNo])
		{
			MyLog(0,"Event_Monster Data Memory Alloc Error!!");
			SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}

		SQLFreeStmt(hStmt,SQL_DROP);
		SQLAllocStmt(hDBC,&hStmt);

		wsprintf(query_stmt,"select * from Event_Monster where Map_Port=%d and GroupNo=%d order by [Index]",pData->wPort,tGroupNo);   
		ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
 
		if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
		{
			MyLog(0,"Event_Monster Table Query Error!!");
			SQLFreeStmt(hStmt,SQL_DROP);
			return FALSE;
		}

		ret=SQLFetch(hStmt);
		if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
		{
			MyLog(0,"Event_Monster Table Fetch Error!!");
			SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}

		m_piGroupDataCount[tGroupNo]=0;
		while (ret==SQL_SUCCESS)
		{
			tGroupDataCount=m_piGroupDataCount[tGroupNo];
			ret=SQLGetData(hStmt,3,SQL_C_SLONG,&m_pGPos[tGroupNo][tGroupDataCount].GroupNo,sizeof(int),&cbValue);	
			ret=SQLGetData(hStmt,4,SQL_C_SLONG,&m_pGPos[tGroupNo][tGroupDataCount].isBoss,sizeof(int),&cbValue);	
			ret=SQLGetData(hStmt,5,SQL_C_SLONG,&m_pGPos[tGroupNo][tGroupDataCount].NPCNo,sizeof(int),&cbValue);	
			ret=SQLGetData(hStmt,6,SQL_C_SLONG,&m_pGPos[tGroupNo][tGroupDataCount].LocationX,sizeof(int),&cbValue);	
			ret=SQLGetData(hStmt,7,SQL_C_SLONG,&m_pGPos[tGroupNo][tGroupDataCount].LocationY,sizeof(int),&cbValue);	
			ret=SQLGetData(hStmt,8,SQL_C_SLONG,&m_pGPos[tGroupNo][tGroupDataCount].EventNo,sizeof(int),&cbValue);	
			ret=SQLGetData(hStmt,9,SQL_C_SLONG,&m_pGPos[tGroupNo][tGroupDataCount].MaxNo,sizeof(int),&cbValue);	
			if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
			{
				MyLog(0,"Event_Monster Table SQL Return Error(%d)!!",ret);
				SQLFreeStmt(hStmt,SQL_DROP);
				return FALSE;
			}              
			m_piGroupDataCount[tGroupNo]++;
			m_iDataCount++;
			ret=SQLFetch(hStmt);
		}
		SQLFreeStmt(hStmt,SQL_DROP);
	}
	return TRUE;
}

int CAdventManager::LoadGroupInfoData()
{
	HSTMT	hStmt=NULL;
	RETCODE	ret;
	char	query_stmt[MAX_PATH]={0,};
	SDWORD	cbValue;
	int		tGroupNo;
	SERVER_DATA *pData = g_pServerTable->GetOwnServerData();

	ClearGroupInfoData();	// Initialize..

	SQLAllocStmt(hDBC,&hStmt);

	wsprintf(query_stmt,"select * from EM_GroupInfo where MapPort=%d order by GroupNo",pData->wPort);

	ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
 
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"EM_GroupInfo Table Query Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return FALSE;
	}

	ret=SQLFetch(hStmt);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"EM_GroupInfo Table Fetch Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	while (ret==SQL_SUCCESS)
	{
		ret=SQLGetData(hStmt,3,SQL_C_SLONG,&tGroupNo,sizeof(int),&cbValue);		// 그룹번호
		if (tGroupNo<0||tGroupNo>=MAX_GROUP_NO)
		{
			JustMsg("EM_GroupInfo Table GroupNo Fail MapPort : %d, GroupNo : %d!!",pData->wPort,tGroupNo);
			SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}
		ret=SQLGetData(hStmt,4,SQL_C_SLONG,&m_tGroupInfo[tGroupNo].ExecType,sizeof(int),&cbValue);		// ExecType
		ret=SQLGetData(hStmt,5,SQL_C_SLONG,&m_tGroupInfo[tGroupNo].DayofWeek,sizeof(int),&cbValue);		// DayofWeek
		ret=SQLGetData(hStmt,6,SQL_C_SLONG,&m_tGroupInfo[tGroupNo].DHour,sizeof(int),&cbValue);		// DHour
		ret=SQLGetData(hStmt,7,SQL_C_SLONG,&m_tGroupInfo[tGroupNo].KilledGroup,sizeof(int),&cbValue);		// KilledGroup
		if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
		{
			MyLog(0,"Event_Monster Table SQL Return Error(%d)!!",ret);
			SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}                
		ret=SQLFetch(hStmt);
	}
	SQLFreeStmt(hStmt,SQL_DROP);
	return TRUE;
}

void CAdventManager::SetActiveByGroupNo(int GroupNo)
{
	t_packet packet;
	int tempIndex=GetGroupIndex(GroupNo);
	if (tempIndex>=0)
	{
		m_piActived[tempIndex]=1;					// Group Number Active
		MyLog(0,"Event Monster Group No : %d Was Actived",tempIndex);

		
		packet.h.header.type=CMD_EVENT_MONSTER_CREATED;
		packet.h.header.size=0;
		g_pUserManager->SendPacket(&packet); // CSD-CN-031213
	}
}

void CAdventManager::SetActiveByIndex(int Index)
{
	t_packet packet;
	
	if (Index<0||Index>=MAX_GROUP_NO) return;
	if (m_piGroupDataCount[Index]<=0) return; // No Data
	
	m_piActived[Index]=1;					// Group Number Active

	MyLog(0,"Event Monster Group No : %d Was Actived",Index);
	
	packet.h.header.type=CMD_EVENT_MONSTER_CREATED;
	packet.h.header.size=0;
	g_pUserManager->SendPacket(&packet); // CSD-CN-031213
}

void CAdventManager::GetDayofWeek()
{
	struct tm* NewTime;
	time_t LongTime;
	time(&LongTime);
	NewTime=localtime(&LongTime);
	m_tTimeData.Year=NewTime->tm_year+1900;
	m_tTimeData.Month=NewTime->tm_mon+1;
	m_tTimeData.Day=NewTime->tm_mday;
	m_tTimeData.DayofWeek=NewTime->tm_wday;
}

void CAdventManager::UpdateEventMonster()		// 환경 설정이 있다면..
{
	static int oldHour=0;
	if (g_hour==oldHour) 
	{
		return;
	}

	GetDayofWeek();
	oldHour=g_hour;

	for (int i=0;i<MAX_GROUP_NO;i++)			// 그룹 발생이벤트 
	{
		switch(m_tGroupInfo[i].ExecType)		// 여기에 계속 넣으면 됨
		{
		case 1 : CheckDayAndActive(i);
		//case 2 : CheckKilledGroup(i);
		}
	}
}

void CAdventManager::SetDeActiveByGroupNo(int GroupNo)		// DeActive By GroupNo
{
	int tempIndex=GetGroupIndex(GroupNo);
	if (tempIndex>=0)
	{
		CheckKilledGroup(tempIndex);				// 이그룹이 해제되면서 문제가 발생되오야 하는게 있다면 체크
		m_piActived[tempIndex]=0;
		MyLog(0,"Event Monster Group No : %d Was DeActived",tempIndex);
	}
}

void CAdventManager::SetDeActiveByIndex(int Index)
{
	if (Index<0||Index>=MAX_GROUP_NO) return;

	CheckKilledGroup(Index);				// 이그룹이 해제되면서 문제가 발생되오야 하는게 있다면 체크
	m_piActived[Index]=0;
	MyLog(0,"Event Monster Group No : %d Was DeActived",Index);
}

int CAdventManager::GetDataCountByGroupNo(int GroupNo)
{
	int tempIndex=GetGroupIndex(GroupNo);
	if (tempIndex>=0)
	{
		return m_piGroupDataCount[tempIndex];
	}
	return -1;
}

int CAdventManager::GetDataCountByIndex(int Index)
{
	if (Index<0||Index>=MAX_GROUP_NO) 
	{
		return -1;
	}
	return m_piGroupDataCount[Index];
}

NPCGenerationPos* CAdventManager::GetGenerationPosByGroupNo(int GroupNo,int Pos)
{
	int tempIndex=GetGroupIndex(GroupNo);
	if (tempIndex<0)
	{
		return NULL;
	}
	if (Pos<0||Pos>m_piGroupDataCount[tempIndex])
	{
		return NULL;
	}
	return &m_pGPos[tempIndex][Pos];			
}

NPCGenerationPos* CAdventManager::GetGenerationPosByIndex(int Index,int Pos)
{
	if (Index<0||Index>=MAX_GROUP_NO) 
	{
		return NULL;
	}
	if (Pos<0||Pos>m_piGroupDataCount[Index])
	{
		return NULL;
	}
	return &m_pGPos[Index][Pos];			
}

int CAdventManager::CheckActivedGroup()
{
	int tempStatus;
	for (int i=0;i<MAX_GROUP_NO;i++)
	{
		tempStatus=GetActiveStatusByIndex(i);
		if (tempStatus>0)
		{
			return TRUE;
		}
	}
	return FALSE;
}

void	CAdventManager::RemoveEventMonster(CHARLIST* Npc)
{
	NPCGenerationPos* Pos;
	
	MyLog(0,"Delete NPC SPECIAL GroupNo : %d ,Type:%d",Npc->JoinLocalWar,Npc->generationpos);
	
	Pos=GetGenerationPosByIndex(Npc->JoinLocalWar,Npc->generationpos);
	if (!Pos) 
	{
		return;
	}
	Pos->CurNPC--;
//	CheckBossAndDeActiveGroupNo(Npc);
}

void CAdventManager::CheckBossAndDeActiveGroupNo(CHARLIST* Npc)
{
	if (!Npc->IsNpc()) return;		// NPC가 아니면 리턴한다.
	if (!Npc->GainedFame) return;	//보스가 아니면 리턴한다.
	
	MyLog(0,"Delete BOSS GroupNo : %d ,Type:%d",Npc->JoinLocalWar,Npc->generationpos);
	SetDeActiveByIndex(Npc->JoinLocalWar);	// 그룹번호를 중지한다.
}

void CAdventManager::CheckDayAndActive(int Index)
{
	if (m_tTimeData.DayofWeek!=m_tGroupInfo[Index].DayofWeek)
	{
		return;
	}

	if (g_hour!=m_tGroupInfo[Index].DHour)
	{
		return;
	}

	MyLog(0,"%d Group Event Monster was Created By DayofWeek And Time",Index);

	SetActiveByIndex(Index);
}

void CAdventManager::CheckKilledGroup(int Index)	// 그룹의 종료시 이벤트 
{
	for (int i=0;i<MAX_GROUP_NO;i++)
	{
		if (m_tGroupInfo[i].ExecType==2)					// 여기에 계속 넣으면 됨 (switch)
		{
			if (m_tGroupInfo[i].KilledGroup==Index)
			{
				MyLog(0,"%d Group Event Monster Was Created By %d Group Event Monster Boss Killed..",i,Index);
				SetActiveByIndex(i);
			}
		}
	}
}