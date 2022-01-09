// AIManager.cpp: implementation of the CAIManager class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "AIManager.h"

#include "AIDefine.h"
#include "Npc_Pattern.h"
#include "BattleManager.h"

///////////////////////////////////////////////////////////////////////////////
// Global Member
///////////////////////////////////////////////////////////////////////////////

#define MAX_ACTION_FUNC		12

extern NPC_Generation		NPC_Gen_Ref[Num_Of_NPC_Generation];
extern HENV					hEnv;
extern HDBC					hDBC;
extern MAGICTABLE			Magic_Ref[Num_Of_Magic]; 
CAIManager*					g_NPC_AI;

extern void KillMonster(WORD wNumber);		// LTS DRAGON MODIFY

// 030919 HK YGI
char NetworkDir[MAX_PATH];
char MapServerConfigFileName[MAX_PATH];
void LoadAIConfig()							// LTS DRAGON MODIFY
{
	if (GetPrivateProfileString("Network","path","",NetworkDir,MAX_PATH,MAP_SERVER_INI_)<=0)
	{
		MyLog(0,"Error!! Check...  Mapserver.ini Path Setting..!!");
		return;
	}
	sprintf_s(MapServerConfigFileName, sizeof(MapServerConfigFileName),"%s/data/MapServerConfig.ini",NetworkDir);
	MyLog( 0, "MapServerConfigFileName is %s", MapServerConfigFileName ); // 031110 YGI
}
////////////////////////////////////////////

int InitAIData()
{
	int Check_OK=TRUE;
	
	if (!g_pAIManager->Load_AI_Data()) Check_OK=FALSE;
	if (!g_pAIManager->LoadBossData()) Check_OK=FALSE;
	if (!g_pAIManager->LoadAttackBag()) Check_OK=FALSE;
	if (!g_pAIManager->LoadActionBag()) Check_OK=FALSE;

	return Check_OK;
}

void ClearAIData()
{
	SAFE_DELETE(g_NPC_AI);
}

void ClearAI()
{
	SAFE_DELETE(g_NPC_AI);
}

void ReloadAIData()	// LTS AI2
{	//< CSD-030417
	if (g_pAIManager->Load_AI_Data())
	{
		MyLog(0,"NPC AI Data Load Complete!!");
	}
	else
	{
		MyLog(0,"FAILURE!! NPC AI Data Load!!");
	}
}	//> CSD-030417

void SendNPCAction(CHARLIST* Npc,int Action)				// LTS DRAGON MODIFY
{
	t_packet packet;
	packet.h.header.type=CMD_AI_NPC_ACTION;
	packet.u.LocalWar.AINPCAction.ID=Npc->GetServerID();
	packet.u.LocalWar.AINPCAction.Action=Action;
	packet.h.header.size=sizeof(t_AINPCAction);
	
	CastNPC2Other(Npc->GetServerID(),&packet);
}

CHARLIST* GetNearNPC(CHARLIST* Npc,int NPCIndex,int Range)
{
	for (int i=NPC_LIST_START;i<MAX_NPC_LIST;i++)
	{
		if (NPCIndex==NPCList[i].npc_index)
		{
			int tempRange=(Npc->X-NPCList[i].X)*(Npc->X-NPCList[i].X) + (Npc->Y-NPCList[i].Y)*(Npc->Y-NPCList[i].Y);
			tempRange=sqrt((float)tempRange);
			if (tempRange<Range)
			{
				return &NPCList[i];
			}
		}
	}
	return NULL;
}


void ACAvoid(CHARLIST* Npc,int Param1,int Param2)
{	//< CSD-030417
	// AI딜레이타임이 없으므로 즉시 실행된다.
	g_pAIManager->SetAI(Npc,NPC_PATTERN_BOSS_AVOID);
}	//> CSD-030417

extern void SendCharacterCondition(CHARLIST* lpChar, int change_HP);
void ACHPRecovery(CHARLIST* Npc,int Param1,int Param2)
{
	if (Npc->HorseNo>3) return;
	int HPAmount=Npc->HpMax*Param1/100;
	Npc->Hp=HPAmount;
	SendCharacterCondition(Npc,Npc->Hp);			// 
	Npc->HorseNo++;
}

void ACStatusUp(CHARLIST* Npc,int Param1,int Param2)
{
	Npc->SetNpcStatusUp(100, 30,30,30);					//확인 필요
}

void MakeFogLocation(CHARLIST* Npc,int RangeX,int RangeY,POINTS* Location)		// LTS NEW AI	// Range Unit : Dot
{
	int BaseX=Npc->X-(RangeX>>1);
	int BaseY=Npc->Y-(RangeY>>1);
	
	for (int i=0;i<FOG_MAX;i++)
	{
		Location[i].x=BaseX+rand()%RangeX;
		Location[i].y=BaseY+rand()%RangeY;
	}
}

void MakeDragonFogLocation(CHARLIST* Npc,int RangeX,int RangeY,POINTS* Location)	// LTS DRAGON MODIFY
{
	int BaseX=Npc->X-(RangeX-300);
	int BaseY=Npc->Y-300;
	
	for (int i=0;i<FOG_MAX;i++)
	{
		Location[i].x=BaseX+rand()%RangeX;
		Location[i].y=BaseY+rand()%RangeY;
	}
}


void ACFog(CHARLIST* Npc,int Param1,int Param2)
{
	t_packet packet;
	
	packet.h.header.type=CMD_AI_FOG;
	packet.u.LocalWar.AIFog.Type=Param1;
	packet.u.LocalWar.AIFog.Delay=Param2;
	MakeFogLocation(Npc,800,600,packet.u.LocalWar.AIFog.Location);	// 한화면 
	packet.h.header.size=sizeof(t_AIFog);
	
	CastNPC2Other(Npc->GetServerID(),&packet);
}

void ACDragonFog(CHARLIST* Npc,int Param1,int Param2)				// LTS DRAGON MODIFY
{
	t_packet packet;
	
	packet.h.header.type=CMD_AI_FOG;
	packet.u.LocalWar.AIFog.Type=Param1;
	packet.u.LocalWar.AIFog.Delay=Param2;
	MakeDragonFogLocation(Npc,1024,768,packet.u.LocalWar.AIFog.Location);	// 한화면 
	packet.h.header.size=sizeof(t_AIFog);
	
	CastNPC2Other(Npc->GetServerID(),&packet);
}

void ACWait(CHARLIST* Npc,int Param1,int Param2)
{	//< CSD-030417
	// AI딜레이타임이 없으므로 즉시 실행된다.
	g_pAIManager->SetAI(Npc,NPC_PATTERN_BOSS_WAIT);
}	//> CSD-030417

void ACSound(CHARLIST* Npc,int Param1,int Param2)
{
	t_packet packet;
	
	packet.h.header.type=CMD_AI_SOUND;
	packet.u.LocalWar.CommonDataDw.Data=Param1;
	packet.h.header.size=sizeof(t_CommonDataDw);
	
	CastNPC2Other(Npc->GetServerID(),&packet);
}

void ACSummon(CHARLIST* Npc,int Param1,int Param2) // LTS NPC_BUG
{	//< CSD-030417
	if (Param2 < 0) 
	{
		return;
	}
	
	MyLog(0, "Summon Call No : %d, Count %d", Param1, Param2);
	
	int nX = Npc->X - (6*TILE_SIZE);
	int nY = Npc->Y + (6*TILE_SIZE);

	switch (Npc->npc_index)
	{
	case DRAGON_INDEX_NO:
	case DRAGON_HEART_INDEX_NO:
		{
			for (int i = 0; i < Param2; ++i)
			{
				g_pBattleManager->SendNpcRecall(Npc->GetServerID(), 95, Param1, nX, nY);
			}

			break;
		}
	default:
		{
			for (int i = 0; i < Param2; ++i)
			{
				if (NPC_NearCh(Npc, Npc, &nX, &nY))
				{
					g_pBattleManager->SendNpcRecall(Npc->GetServerID(), 95, Param1, nX, nY);
				}
			}

			break;
		}
	}
}	//> CSD-030417

void ACEarthQuake(CHARLIST* Npc,int Param1,int Param2)		// LTS DRAGON MODIFY
{
	t_packet packet;
	packet.h.header.type=CMD_AI_EARTHQUAKE;
	packet.u.LocalWar.CommonDataC.Data=Param1;
	packet.h.header.size=sizeof(t_CommonDataC);
	
	CastNPC2Other(Npc->GetServerID(),&packet);
}

void ACAttack(CHARLIST* Npc,int Param1,int Param2)
{	//< CSD-030417
	if (DRAGON_INDEX_NO == Npc->npc_index)
	{
		g_pAIManager->SendBossAction(Npc, Param1);
	}

	g_pBattleManager->SendNpcMagic(Npc->GetServerID(), Param1, Npc->X, Npc->Y, true);
}	//> CSD-030417

void ACSummon2(CHARLIST* Npc,int Param1,int Param2)			// LTS DRAGON BUG
{	//< CSD-030417
	if (Param2 < 0)
	{
		return;
	}
	
	MyLog(0, "Summon Call No : %d, Count %d", Param1, Param2);

	int nX = Npc->X - (6*TILE_SIZE);
	int nY = Npc->Y + (6*TILE_SIZE);
	
	switch (Npc->npc_index)
	{
	case DRAGON_INDEX_NO:
	case DRAGON_HEART_INDEX_NO:
		{
			for (int i=0; i < Param2; ++i)
			{
				g_pBattleManager->SendNpcRecall(Npc->GetServerID(), 95, Param1, nX, nY);
			}

			break;
		}
	default:
		{
			for (int i = 0; i < Param2; ++i)
			{
				if (NPC_NearCh(Npc,Npc,&nX,&nY))
				{
					g_pBattleManager->SendNpcRecall(Npc->GetServerID(), 95, Param1, nX, nY);
				}
			}

			break;
		}
	}
}	//> CSD-030417

void ACDragonTelePort(CHARLIST* Npc,int Param1,int Param2)		// LTS DRAGON MODIFY
{	//< CSD-030417
	if (DRAGON_HEART_INDEX_NO != Npc->npc_index)
	{
		return;
	}
	
	POINTS	WithNPCPoint;
	if (Npc->MoveSx==444)
	{
		Npc->X=489*TILE_SIZE;
		Npc->Y=75*TILE_SIZE;
		WithNPCPoint.x=492*TILE_SIZE;
		WithNPCPoint.y=73*TILE_SIZE;
		MyLog(0,"TelePort Call Old : %d:%d, %d:%d",Npc->MoveSx,Npc->MoveSy,Npc->X/TILE_SIZE,Npc->Y/TILE_SIZE);
	}
	else
	{
		Npc->X=444*TILE_SIZE;
		Npc->Y=30*TILE_SIZE;
		WithNPCPoint.x=446*TILE_SIZE;
		WithNPCPoint.y=27*TILE_SIZE;
		MyLog(0,"TelePort Call Old : %d:%d, %d:%d",Npc->MoveSx,Npc->MoveSy,Npc->X/TILE_SIZE,Npc->Y/TILE_SIZE);
	}
	
	CHARLIST* WithNPC = g_pAIManager->GetDragonPtr(Npc);
	
	if (WithNPC!=NULL) 
	{
		SendNPCAction(WithNPC,9);
		g_pBattleManager->SendNpcMagic(WithNPC->GetServerID(),105,Npc->X,Npc->Y,true);
		g_pBattleManager->SendNpcTeleport(Npc->GetServerID(), Npc->X, Npc->Y);
		g_pBattleManager->SendNpcTeleport(WithNPC->GetServerID(), WithNPCPoint.x, WithNPCPoint.y);	
	}
	else
	{
		g_pBattleManager->SendNpcTeleport(Npc->GetServerID(), Npc->X, Npc->Y);		// 만약을 위한 코드 
	}
}	//> CSD-030417

void (*ActionFunc[MAX_ACTION_FUNC])(CHARLIST* Npc,int Param1,int Param2) =
{
	ACAvoid,
	ACHPRecovery,
	ACStatusUp,
	ACFog,
	ACWait,
	ACSound,
	ACSummon,
	ACEarthQuake,
	ACAttack,
	ACSummon2,
	ACDragonTelePort,
	ACDragonFog
};

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CAIManager::CAIManager()
{
	m_pNpc_AI_Data=NULL;
	m_iAI_DataCount=0;
	m_pMagicBag=NULL;
	m_iMagicBagCount=0;
	m_pBossStatusData=NULL;
	m_iBossStatusCount=0;
	m_pAttackBag=NULL;
	m_iAttackBagCount=0;
	m_pActionBag=NULL;
	m_iActionBagCount=0;
	m_pDragon=NULL;
}

CAIManager::~CAIManager()
{
	Clear_AI_Data();
	ClearBossData();
}

void CAIManager::Clear_AI_Data()
{
	if (m_pNpc_AI_Data)
	{
		for (int i=0;i<m_iAI_DataCount;i++)
		{
			
			if (m_pNpc_AI_Data[i])
			{
				SAFE_DELETE_ARRAY(m_pNpc_AI_Data[i]);
			}
		}
		SAFE_DELETE_ARRAY(m_pNpc_AI_Data);
	}
	ClearMagicBag();
}	

void CAIManager::ClearBossData()
{
	if (m_pBossStatusData)
	{
		for (int i=0;i<m_iBossStatusCount;i++)
		{
			
			if (m_pBossStatusData[i])
			{
				SAFE_DELETE_ARRAY(m_pBossStatusData[i]);
			}
		}
		SAFE_DELETE_ARRAY(m_pBossStatusData);
	}
	
	ClearAttackBag();
	ClearActionBag();
}	

void CAIManager::ClearMagicBag()
{
	if (m_pMagicBag)
	{
		for (int i=0;i<m_iMagicBagCount;i++)
		{
			SAFE_DELETE_ARRAY(m_pMagicBag[i]);
		}
		SAFE_DELETE_ARRAY(m_pMagicBag);
	}
}


void CAIManager::ClearAttackBag()
{
	if (m_pAttackBag)
	{
		for (int i=0;i<m_iAttackBagCount;i++)
		{
			SAFE_DELETE_ARRAY(m_pAttackBag[i]);
		}
		SAFE_DELETE_ARRAY(m_pAttackBag);
	}
}

void CAIManager::ClearActionBag()
{
	if (m_pActionBag)
	{
		for (int i=0;i<m_iActionBagCount;i++)
		{
			SAFE_DELETE_ARRAY(m_pActionBag[i]);
		}
		SAFE_DELETE_ARRAY(m_pActionBag);
	}
}

int CAIManager::Load_AI_Data()
{
	HSTMT	hStmt=NULL;
	RETCODE	ret;
	SWORD	nClos;
	char	query_stmt[80]={0,};
	SDWORD	cbValue;
	int		RowCount;
	
	if (m_pNpc_AI_Data)								// 이미 데이터가 있다면 프리해준다..
	{
		Clear_AI_Data();
	}
	
	SQLAllocStmt(hDBC,&hStmt);
	
	wsprintf(query_stmt,"select count(AI_Type) as FieldCount from NPC_AI_CONTROL");   
	
	ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
	
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"Load_AI_Data(), NPC_AI_CONTROL Table Query Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return FALSE;
	}
	
	ret=SQLFetch(hStmt);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"Load_AI_Data(), NPC_AI_CONTROL Table Fetch Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}
	ret=SQLGetData(hStmt,1,SQL_C_SLONG,&RowCount,sizeof(int),&cbValue);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
	{
		MyLog(0,"Load_AI_Data(), NPC_AI_CONTROL Table SQL Return Error(%d)!!",ret);
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}                
	
	SQLFreeStmt(hStmt,SQL_DROP);
	
	m_pNpc_AI_Data=new int*[RowCount+1];			// 필드수만큼 AI_Type생성 
	
	if (!m_pNpc_AI_Data) 
	{
		MyLog(0,"Load_AI_Data(), m_pNpc_AI_Data Memory Alloc Error!!",ret);
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}
	
	for (int i=0;i<RowCount+1;i++)
	{
		m_pNpc_AI_Data[i]=NULL;						// 메모리 초기화
	}
	
	SQLAllocStmt(hDBC,&hStmt);
	
	wsprintf(query_stmt,"select * from NPC_AI_CONTROL order by AI_Type");   
	
	ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
	
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"Load_AI_Data(), NPC_AI_CONTROL Table Query Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return FALSE;
	}
	
	SQLNumResultCols(hStmt,&nClos);
	
	ret=SQLFetch(hStmt);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"Load_AI_Data(), NPC_AI_CONTROL Table Fetch Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}
	
	m_iAI_DataCount=1;
	while (ret==SQL_SUCCESS)
	{
		m_pNpc_AI_Data[m_iAI_DataCount]=new int[nClos];
		if (!m_pNpc_AI_Data[m_iAI_DataCount])
		{
			MyLog(0,"Load_AI_Data(), m_pNpc_AI_Data Memory Alloc Error!!",ret);
			SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}
		
		for (int i=0;i<nClos;i++)
		{
			ret=SQLGetData(hStmt,i+1,SQL_C_SLONG,&m_pNpc_AI_Data[m_iAI_DataCount][i],sizeof(int),&cbValue);	
			if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
			{
				MyLog(0,"Load_AI_Data(), NPC_AI_CONTROL Table SQL Return Error(%d)!!",ret);
				SQLFreeStmt(hStmt,SQL_DROP);
				return FALSE;
			}                
		}
		m_iAI_DataCount++;
		ret=SQLFetch(hStmt);
	}
    SQLFreeStmt(hStmt,SQL_DROP);
	return LoadMagicBag();
}

int CAIManager::LoadMagicBag()
{
	HSTMT	hStmt=NULL;
	RETCODE	ret;
	SWORD	nClos;
	char	query_stmt[80]={0,};
	SDWORD	cbValue;
	int		RowCount;
	
	if (m_pMagicBag)								// 이미 데이터가 있다면 프리해준다..
	{
		ClearMagicBag();
	}
	
	SQLAllocStmt(hDBC,&hStmt);
	
	wsprintf(query_stmt,"select count([index]) as FieldCount from Magic_Bag");   
	
	ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
	
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"LoadMagicBag(), Magic Bag Table Query Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return FALSE;
	}
	
	ret=SQLFetch(hStmt);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"LoadMagicBag(), Magic Bag Table Fetch Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}
	ret=SQLGetData(hStmt,1,SQL_C_SLONG,&RowCount,sizeof(int),&cbValue);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
	{
		MyLog(0,"LoadMagicBag(), Magic Bag Table SQL Return Error(%d)!!",ret);
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}                
	
	SQLFreeStmt(hStmt,SQL_DROP);
	
	m_pMagicBag=new int*[RowCount+1];			// 필드수만큼 AI_Type생성 
	
	if (!m_pMagicBag) 
	{
		MyLog(0,"LoadMagicBag(), m_pMagicBag Memory Alloc Error!!",ret);
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}
	
	for (int i=0;i<RowCount+1;i++)
	{
		m_pMagicBag[i]=NULL;						// 메모리 초기화
	}
	
	SQLAllocStmt(hDBC,&hStmt);
	
	wsprintf(query_stmt,"select * from Magic_bag order by [index]");   
	
	ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
	
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"LoadMagicBag(), Magic Bag Table  Query Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return FALSE;
	}
	
	SQLNumResultCols(hStmt,&nClos);
	
	ret=SQLFetch(hStmt);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"LoadMagicBag(), Magic Bag Table Fetch Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}
	
	m_iMagicBagCount=1;
	while (ret==SQL_SUCCESS)
	{
		m_pMagicBag[m_iMagicBagCount]=new int[10];
		if (!m_pMagicBag[m_iMagicBagCount])
		{
			MyLog(0,"LoadMagicBag(), m_pMagicBag Memory Alloc Error!!",ret);
			SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}
		ret=SQLGetData(hStmt, 4,SQL_C_SLONG,&m_pMagicBag[m_iMagicBagCount][ 0],sizeof(int),&cbValue);	
		ret=SQLGetData(hStmt, 5,SQL_C_SLONG,&m_pMagicBag[m_iMagicBagCount][ 1],sizeof(int),&cbValue);	
		ret=SQLGetData(hStmt, 6,SQL_C_SLONG,&m_pMagicBag[m_iMagicBagCount][ 2],sizeof(int),&cbValue);	
		ret=SQLGetData(hStmt, 7,SQL_C_SLONG,&m_pMagicBag[m_iMagicBagCount][ 3],sizeof(int),&cbValue);	
		ret=SQLGetData(hStmt, 8,SQL_C_SLONG,&m_pMagicBag[m_iMagicBagCount][ 4],sizeof(int),&cbValue);	
		ret=SQLGetData(hStmt, 9,SQL_C_SLONG,&m_pMagicBag[m_iMagicBagCount][ 5],sizeof(int),&cbValue);	
		ret=SQLGetData(hStmt,10,SQL_C_SLONG,&m_pMagicBag[m_iMagicBagCount][ 6],sizeof(int),&cbValue);	
		ret=SQLGetData(hStmt,11,SQL_C_SLONG,&m_pMagicBag[m_iMagicBagCount][ 7],sizeof(int),&cbValue);	
		ret=SQLGetData(hStmt,12,SQL_C_SLONG,&m_pMagicBag[m_iMagicBagCount][ 8],sizeof(int),&cbValue);	
		ret=SQLGetData(hStmt,13,SQL_C_SLONG,&m_pMagicBag[m_iMagicBagCount][ 9],sizeof(int),&cbValue);	
		if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
		{
			MyLog(0,"Load_AI_Data(), NPC_AI_CONTROL Table SQL Return Error(%d)!!",ret);
			SQLFreeStmt(hStmt,SQL_DROP);
			return FALSE;
		}                
		m_iMagicBagCount++;
		ret=SQLFetch(hStmt);
	}
    SQLFreeStmt(hStmt,SQL_DROP);
	return TRUE;
}


int CAIManager::LoadBossData()
{
	HSTMT	hStmt=NULL;
	RETCODE	ret;
	SWORD	nClos;
	char	query_stmt[80]={0,};
	SDWORD	cbValue;
	int		RowCount;
	
	if (m_pBossStatusData)								// 이미 데이터가 있다면 프리해준다..
	{
		ClearBossData();
	}
	
	SQLAllocStmt(hDBC,&hStmt);
	
	wsprintf(query_stmt,"select count(TypeNo) as FieldCount from AI_BossStatus");   
	
	ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
	
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"LoadBossData(), AI_BossStatus Table Query Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return FALSE;
	}
	
	ret=SQLFetch(hStmt);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"LoadBossData(), AI_BossStatus Table Fetch Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}
	ret=SQLGetData(hStmt,1,SQL_C_SLONG,&RowCount,sizeof(int),&cbValue);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
	{
		MyLog(0,"LoadBossData(), AI_BossStatus Table SQL Return Error(%d)!!",ret);
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}                
	
	SQLFreeStmt(hStmt,SQL_DROP);
	
	m_pBossStatusData=new int*[RowCount+1];			// 필드수만큼 AI_Type생성 
	
	if (!m_pBossStatusData) 
	{
		MyLog(0,"LoadBossData(), m_pBossStatusData Memory Alloc Error!!",ret);
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}
	
	for (int i=0;i<RowCount+1;i++)
	{
		m_pBossStatusData[i]=NULL;						// 메모리 초기화
	}
	
	SQLAllocStmt(hDBC,&hStmt);
	
	wsprintf(query_stmt,"select * from AI_BossStatus order by TypeNo");   
	
	ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
	
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"LoadBossData(), AI_BossStatus Table Query Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return FALSE;
	}
	
	SQLNumResultCols(hStmt,&nClos);
	
	ret=SQLFetch(hStmt);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"LoadBossData(), AI_BossStatus Table Fetch Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}
	
	m_iBossStatusCount=1;
	while (ret==SQL_SUCCESS)
	{
		m_pBossStatusData[m_iBossStatusCount]=new int[nClos];
		if (!m_pBossStatusData[m_iBossStatusCount])
		{
			MyLog(0,"LoadBossData(), m_pBossStatusData Memory Alloc Error!!",ret);
			SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}
		
		for (int i=0;i<nClos;i++)
		{
			ret=SQLGetData(hStmt,i+1,SQL_C_SLONG,&m_pBossStatusData[m_iBossStatusCount][i],sizeof(int),&cbValue);	
			if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
			{
				MyLog(0,"LoadBossData(), AI_BossStatus Table SQL Return Error(%d)!!",ret);
				SQLFreeStmt(hStmt,SQL_DROP);
				return FALSE;
			}                
		}
		m_iBossStatusCount++;
		ret=SQLFetch(hStmt);
	}
    SQLFreeStmt(hStmt,SQL_DROP);
	return TRUE;
}

int CAIManager::LoadAttackBag()
{
	HSTMT	hStmt=NULL;
	RETCODE	ret;
	SWORD	nClos;
	char	query_stmt[80]={0,};
	SDWORD	cbValue;
	int		RowCount;
	
	if (m_pAttackBag)								// 이미 데이터가 있다면 프리해준다..
	{
		ClearAttackBag();
	}
	
	SQLAllocStmt(hDBC,&hStmt);
	
	wsprintf(query_stmt,"select count(TypeNo) as FieldCount from AI_PhysicalAttack_Bag");   
	
	ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
	
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"LoadAttackBag(), AI_PhysicalAttack_Bag Table Query Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return FALSE;
	}
	
	ret=SQLFetch(hStmt);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"LoadAttackBag(), AI_PhysicalAttack_Bag Table Fetch Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}
	ret=SQLGetData(hStmt,1,SQL_C_SLONG,&RowCount,sizeof(int),&cbValue);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
	{
		MyLog(0,"LoadAttackBag(), AI_PhysicalAttack_Bag Table SQL Return Error(%d)!!",ret);
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}                
	
	SQLFreeStmt(hStmt,SQL_DROP);
	
	m_pAttackBag=new int*[RowCount+1];			// 필드수만큼 AI_Type생성 
	
	if (!m_pAttackBag) 
	{
		MyLog(0,"LoadAttackBag(), m_pAttackBag Memory Alloc Error!!",ret);
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}
	
	for (int i=0;i<RowCount+1;i++)
	{
		m_pAttackBag[i]=NULL;						// 메모리 초기화
	}
	
	SQLAllocStmt(hDBC,&hStmt);
	
	wsprintf(query_stmt,"select * from AI_PhysicalAttack_Bag order by TypeNo");   
	
	ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
	
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"LoadAttackBag(), AI_PhysicalAttack_Bag Table Query Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return FALSE;
	}
	
	SQLNumResultCols(hStmt,&nClos);
	
	ret=SQLFetch(hStmt);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"LoadAttackBag(), AI_PhysicalAttack_Bag Table Fetch Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}
	
	m_iAttackBagCount=1;
	while (ret==SQL_SUCCESS)
	{
		m_pAttackBag[m_iAttackBagCount]=new int[5];
		if (!m_pAttackBag[m_iAttackBagCount])
		{
			MyLog(0,"LoadAttackBag(), AI_PhysicalAttack_Bag Memory Alloc Error!!",ret);
			SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}
		ret=SQLGetData(hStmt, 3,SQL_C_SLONG,&m_pAttackBag[m_iAttackBagCount][ 0],sizeof(int),&cbValue);	
		ret=SQLGetData(hStmt, 4,SQL_C_SLONG,&m_pAttackBag[m_iAttackBagCount][ 1],sizeof(int),&cbValue);	
		ret=SQLGetData(hStmt, 5,SQL_C_SLONG,&m_pAttackBag[m_iAttackBagCount][ 2],sizeof(int),&cbValue);	
		ret=SQLGetData(hStmt, 6,SQL_C_SLONG,&m_pAttackBag[m_iAttackBagCount][ 3],sizeof(int),&cbValue);	
		ret=SQLGetData(hStmt, 7,SQL_C_SLONG,&m_pAttackBag[m_iAttackBagCount][ 4],sizeof(int),&cbValue);	
		if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
		{
			MyLog(0,"LoadAttackBag(), AI_PhysicalAttack_Bag Table SQL Return Error(%d)!!",ret);
			SQLFreeStmt(hStmt,SQL_DROP);
			return FALSE;
		}                
		m_iAttackBagCount++;
		ret=SQLFetch(hStmt);
	}
    SQLFreeStmt(hStmt,SQL_DROP);
	return TRUE;
}

int CAIManager::LoadActionBag()
{
	HSTMT	hStmt=NULL;
	RETCODE	ret;
	SWORD	nClos;
	char	query_stmt[80]={0,};
	SDWORD	cbValue;
	int		RowCount;
	
	if (m_pActionBag)								// 이미 데이터가 있다면 프리해준다..
	{
		ClearActionBag();
	}
	
	SQLAllocStmt(hDBC,&hStmt);
	
	wsprintf(query_stmt,"select count(TypeNo) as FieldCount from AI_ActionBag");   
	
	ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
	
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"LoadActionBag(), AI_ActionBag Table Query Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return FALSE;
	}
	
	ret=SQLFetch(hStmt);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"LoadActionBag(), AI_ActionBag Table Fetch Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}
	ret=SQLGetData(hStmt,1,SQL_C_SLONG,&RowCount,sizeof(int),&cbValue);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
	{
		MyLog(0,"LoadActionBag(), AI_ActionBag Table SQL Return Error(%d)!!",ret);
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}                
	
	SQLFreeStmt(hStmt,SQL_DROP);
	
	m_pActionBag=new int*[RowCount+1];			// 필드수만큼 AI_Type생성 
	
	if (!m_pActionBag) 
	{
		MyLog(0,"LoadActionBag(), m_pActionBag Memory Alloc Error!!",ret);
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}
	
	for (int i=0;i<RowCount+1;i++)
	{
		m_pActionBag[i]=NULL;						// 메모리 초기화
	}
	
	SQLAllocStmt(hDBC,&hStmt);
	
	wsprintf(query_stmt,"select * from AI_ActionBag order by TypeNo");   
	
	ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
	
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"LoadAttackBag(), AI_ActionBag Table Query Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return FALSE;
	}
	
	SQLNumResultCols(hStmt,&nClos);
	
	ret=SQLFetch(hStmt);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"LoadActionBag(), AI_ActionBag Table Fetch Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}
	
	m_iActionBagCount=1;
	while (ret==SQL_SUCCESS)
	{
		m_pActionBag[m_iActionBagCount]=new int[3];
		if (!m_pActionBag[m_iActionBagCount])
		{
			MyLog(0,"LoadActionBag(), AI_ActionBag Memory Alloc Error!!",ret);
			SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}
		ret=SQLGetData(hStmt, 3,SQL_C_SLONG,&m_pActionBag[m_iActionBagCount][ 0],sizeof(int),&cbValue);	// Func No
		if (m_pActionBag[m_iActionBagCount][ 0]>MAX_ACTION_FUNC)
		{
			MyLog(0,"LoadActionBag(), AI_ActionBag Function Number Error!!",ret);
			SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}
		ret=SQLGetData(hStmt, 4,SQL_C_SLONG,&m_pActionBag[m_iActionBagCount][ 1],sizeof(int),&cbValue);	// Param1
		ret=SQLGetData(hStmt, 5,SQL_C_SLONG,&m_pActionBag[m_iActionBagCount][ 2],sizeof(int),&cbValue);	// Param2
		
		if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
		{
			MyLog(0,"LoadActionBag(), AI_ActionBag Table SQL Return Error(%d)!!",ret);
			SQLFreeStmt(hStmt,SQL_DROP);
			return FALSE;
		}                
		m_iActionBagCount++;
		ret=SQLFetch(hStmt);
	}
    SQLFreeStmt(hStmt,SQL_DROP);
	return TRUE;
}

inline int** CAIManager::GetNpcAIData()
{
	return m_pNpc_AI_Data;
}

inline int* CAIManager::GetNpcAIData(int AI_Type)
{
	return m_pNpc_AI_Data[AI_Type];
}

inline int CAIManager::GetNpcAIData(int AI_Type,int Field)
{
	if (m_iAI_DataCount==NULL) return 0;
	
	if (m_iAI_DataCount<AI_Type)
	{
		//	    JustMsg("Error!! AI_Type Range Fault!! See NPC_Generation_SP's Sel_age_min, max");
		return 0;
	}
	return m_pNpc_AI_Data[AI_Type][Field];
}

inline int CAIManager::GetMagicBagData(int Row,int Col)
{
	if (Row<0 || Row>m_iMagicBagCount) return 0;
	if (Col<0 || Col>9) return 0;
	
	return m_pMagicBag[Row][Col];
}

inline int CAIManager::CheckAIDelayTime(CHARLIST* Npc)
{
	if (global_time-Npc->aitimedelay>Npc->aidelayhowmuch)
	{
		return TRUE;
	}
	return FALSE;
}

inline void CAIManager::SetAIDelayTime(CHARLIST* Npc,int Status,int StatusSub)
{	//< CSD-021021
	Npc->aitimedelay = global_time;
	Npc->aidelayhowmuch = GetNpcAIData(NPC_Gen_Ref[Npc->npc_index].nAINo, Status) + rand()%(GetNpcAIData(NPC_Gen_Ref[Npc->npc_index].nAINo, StatusSub)+1);
}	//> CSD-021021

inline void CAIManager::SetAIDelayTime(CHARLIST* Npc,DWORD time)		//현재 상태에서 AI딜레이시간만... 늘린다.
{
	Npc->aidelayhowmuch=Npc->aidelayhowmuch+time;
}

inline bool	CAIManager::isProcess(CHARLIST* Npc)
{	//< CSD-030306
	if (!Npc->IsNpc()) 
	{
		return false;
	}
	
	if (!CheckAIDelayTime(Npc)) 
	{
		return false;
	}
	
	if (Npc->patterntype < NPC_PATTERN_WANDER)		// 기존의 AI타입..
	{
		return false;
	}

	const int nDiffX = Npc->MoveGox/TILE_SIZE - Npc->homex;
	const int nDiffY = Npc->MoveGoy/TILE_SIZE - Npc->homey;
	const int nDist = sqrt(pow(nDiffX, 2.0) + pow(nDiffY, 2.0));
		
	if	(nDist > 25)
	{	//< CSD-030314
		g_pBattleManager->SendNpcTeleport(Npc->GetServerID(), Npc->homex*TILE_SIZE, Npc->homey*TILE_SIZE);
		return false;
	}	//> CSD-030314
	
	return true;
}	//> CSD-030306

inline int  CAIManager::GetNPCSearchRange(CHARLIST* Npc)
{	//< CSD-021021
	return GetNpcAIData(NPC_Gen_Ref[Npc->npc_index].nAINo,FSEARCH_RANGE);
}	//> CSD-021021

inline void CAIManager::SetNextAI(CHARLIST* Npc)
{
	if (!Npc->ChairNum) return;
	if (Npc->patterntype<NPC_PATTERN_ATTACK)
	{
		Npc->patterntype++;									// 604도 증가되면??
	}
}

inline void CAIManager::SetAI(CHARLIST* Npc,int AIPattern)			// LTS DRAGON MODIFY
{
	if (!Npc->ChairNum) return;
	Npc->patterntype=AIPattern;
}

inline void CAIManager::SetAIMove(CHARLIST* Npc)
{
	if (!Npc->ChairNum) return;
	
	Npc->EventPoint=Npc->patterntype;								// Old AI 저장
	Npc->patterntype=NPC_PATTERN_MOVING;
}

inline void CAIManager::SetAIBMove(CHARLIST* Npc)
{
	if (!Npc->ChairNum) return;
	
	Npc->EventPoint=Npc->patterntype;								// Old AI 저장
	Npc->patterntype=NPC_PATTERN_BOSS_MOVING;
}


inline void CAIManager::SetAIDefault(CHARLIST* Npc)
{
	Npc->patterntype=NPC_PATTERN_WANDER;
}

inline void CAIManager::SetAIBDefault(CHARLIST* Npc)
{
	Npc->patterntype=NPC_PATTERN_BOSS_WANDER;
}

inline void CAIManager::SetTarget(CHARLIST* Npc,int Target)
{
	if (Target!=-1)
	{
		Npc->targetid=Target;
	}
}

inline void CAIManager::ClearTarget(CHARLIST* Npc)
{
	Npc->targetid=-1;
}

#define	HOME_DIST_1AREA					245
#define HOME_DIST_2AREA					625

inline void CAIManager::RandomMove(CHARLIST* Npc)
{
	int tempValue=rand()%100;
	int Percent=GetNpcAIData(NPC_Gen_Ref[Npc->npc_index].nAINo,FWANDER_RATE); // CSD-021021
	if (tempValue<Percent)						
	{	
		
		bool soFar=false;
		int dir;
		int how;
		int tx=Npc->MoveGox/TILE_SIZE;
		int ty=Npc->MoveGoy/TILE_SIZE;
		
		const DWORD area1 = sqrt(pow(Npc->X/TILE_SIZE - Npc->homex, 2.0) + pow(Npc->Y/TILE_SIZE - Npc->homey, 2.0));
		const DWORD area2 = sqrt(pow(tx - Npc->homex, 2.0) + pow(ty - Npc->homey, 2.0));
		
		if( area1 > HOME_DIST_1AREA )
		{	
			if( area2 > HOME_DIST_2AREA )
			{	//< CSD-030314
				g_pBattleManager->SendNpcTeleport(Npc->GetServerID(), Npc->homex*32, Npc->homey*32);
				return;
			}	//> CSD-030314
		}
		
		dir = rand()%8;
		how = rand()%8+2;
		if (!soFar)
		{
			tx = Npc->MoveSx;
			ty = Npc->MoveSy;
		}
		switch( dir )
		{
		case 0 :			ty +=how; break;
		case 1 : tx -=how;	ty +=how; break;
		case 2 : tx -=how;            break;
		case 3 : tx -=how;	ty -=how; break;
		case 4 :			ty -=how; break;
		case 5 : tx +=how;	ty -=how; break;
		case 6 : tx +=how;		      break;
		case 7 : tx +=how;	ty +=how; break;
		}

		if(tx <= 1) tx = 2; //Eleval 26/05/09 to prevent monsters from being stuck in the map limits
		if(ty <= 1) ty = 2; //Eleval 26/05/09 to prevent monsters from being stuck in the map limits
		if(tx >= g_Map.file.wWidth - 2) tx = g_Map.file.wWidth - 3; //Eleval 26/05/09 to prevent monsters from being stuck in the map limits
		if(ty >= g_Map.file.wHeight - 2) ty = g_Map.file.wHeight - 3; //Eleval 26/05/09 to prevent monsters from being stuck in the map limits
		
		if( NPC_MakePathBumn( Npc, tx, ty, how ) )
		{
			if (Npc->patterntype>=NPC_PATTERN_BOSS_WANDER)
				SetAIBMove(Npc);										// Npc AI Status : Move
			else
				SetAIMove(Npc);										// Npc AI Status : Move
			SendMoveNPCArea( connections, Npc->GetServerID());
		}
	}
}

inline void CAIManager::TargetMove(CHARLIST* Npc)		// LTS DRAGON MODIFY
{
	int TargetX,TargetY;
	
	if (NPC_NearCh(Npc,&connections[Npc->targetid].chrlst,&TargetX,&TargetY))
	{
		if (NPC_MakePath(Npc,TargetX,TargetY,rand()%6+2))			// 타켓으로 부터의 위치 랜덤
		{
			if (Npc->patterntype>=NPC_PATTERN_BOSS_WANDER) SetAIBMove(Npc);
			else SetAIMove(Npc);
			SetAIDelayTime(Npc,FSTATUS05_DELAY,FSTATUS05_SUBDELAY);		// STATUS05 MOVE
			SendMoveNPCArea( connections, Npc->GetServerID());
		}
		else 
		{
			if (Npc->patterntype>=NPC_PATTERN_BOSS_WANDER) SetAIBDefault(Npc);
			else SetAIDefault(Npc);
			
		}
	}
	else
	{
		if (Npc->patterntype>=NPC_PATTERN_BOSS_WANDER) SetAIBDefault(Npc);
		else SetAIDefault(Npc);
	}
}

inline void CAIManager::ClearMovePattern(CHARLIST* Npc)
{
	Npc->EventPoint=0;
}

inline int CAIManager::GetAttackChange(CHARLIST* Npc)
{
	int LifePercent=Npc->Hp/Npc->HpMax*100;
	if (LifePercent<=90&&LifePercent>70)
	{
		return 1;
	}
	else
		if (LifePercent<=70&&LifePercent>50)
		{
			return 2;
		}
		else
			if (LifePercent<=50&&LifePercent>30)
			{
				return 3;
			}
			else
				if (LifePercent<=30&&LifePercent>10)
				{
					return 4;
				}
				else
					if (LifePercent<=10&&LifePercent>0)
					{
						return 5;
					}
					return 0;
}

inline int CAIManager::CheckSummon(CHARLIST* Npc)
{
	//MyLog(0,"Npc SprNo : %d, Summon : %d",Npc->SprNo,Npc->ElementType);
	if (!GetNpcAIData(NPC_Gen_Ref[Npc->npc_index].nAINo,FSUMMON_MAX))	// CSD-021021
	{
		return FALSE;
	}
	else																		// 소환몬이다.	
	{ // 이미 소환 다했음
		if (Npc->ElementType>=GetNpcAIData(NPC_Gen_Ref[Npc->npc_index].nAINo,FSUMMON_MAX))	// CSD-021021 
		{ 
			return FALSE;
		}
		
		if (Npc->scan_time>g_curr_time)			// 한번 소환한다음 시간이 지나지 않았다
		{
			return FALSE;
		}
		else
		{
			if (rand()%100<GetNpcAIData(NPC_Gen_Ref[Npc->npc_index].nAINo,FSUMMON_PERCENT)) // CSD-021021		 
			{
				return TRUE;
			}
			else 
			{
				return FALSE;
			}
		}
	}
}

inline void CAIManager::CheckSummonDeath(CHARLIST* Defender)						// LTS DRAGON MODIFY
{
	if (Defender->itemweight>=NPC_LIST_START&&Defender->itemweight<MAX_NPC_LIST)
	{
		CHARLIST* ch=&NPCList[Defender->itemweight];		//소환한 주인을 찾는다.
		
		if (!ch->IsDead())							//죽지않았거나
		{
			if (ch->ElementType>0)					// 소환된 수가 있다면 	
			{
				ch->ElementType--;
			}
		}
	}
}

inline void  CAIManager::SelectAttackMethod(CHARLIST* Npc)				
{
	if (CheckSummon(Npc))							// 소환 몬스터이면 소환을 결정한다.
	{
		Npc->fame=4;
		return;
	}
	
	int MagicChange=GetAttackChange(Npc);
	int AI_Type=NPC_Gen_Ref[Npc->npc_index].nAINo; // CSD-021021
	int AttackChance=GetNpcAIData(AI_Type,FATTACK_CHANCE);
	
	int AttackMethodRate1=GetNpcAIData(AI_Type,FATTACK_RATE1)-(MagicChange*AttackChance);
	if (AttackMethodRate1<0) AttackMethodRate1=0;
	
	int AttackMethodRate2=GetNpcAIData(AI_Type,FATTACK_RATE2)+(MagicChange*AttackChance*0.5);
	if (AttackMethodRate2<0) AttackMethodRate2=0;
	
	int AttackMethodRate3=GetNpcAIData(AI_Type,FATTACK_RATE3)+(MagicChange*AttackChance);
	if (AttackMethodRate3<0) AttackMethodRate3=0;
	
	
	int DivValue=AttackMethodRate1+AttackMethodRate2+AttackMethodRate3;
	int RandValue = 0;
	if (DivValue>0) RandValue=rand()%DivValue;
	
	if (RandValue>=0&&RandValue<AttackMethodRate1)
	{
		Npc->fame=1;
	}
	else
		if (RandValue>=AttackMethodRate1&&RandValue<(AttackMethodRate1+AttackMethodRate2))
		{
			Npc->fame=2;
		}
		else
			if (RandValue>=(AttackMethodRate1+AttackMethodRate2)&&RandValue<=DivValue)
			{
				Npc->fame=3;
			}
}

inline int CAIManager::GetAttackBagNo(CHARLIST*Npc)
{
	int AttackMethod=Npc->mantle+FBATTACK_METHOD1-1;	// mantle이 1부터 시작
	return m_pBossStatusData[Npc->CurrentPhase][AttackMethod];
	
}

inline int CAIManager::GetAttackMethodFromAttackBag(int AttackBagNo)
{
	if (AttackBagNo<=0) return -1;
	int RandValue=rand()%5;
	return m_pAttackBag[AttackBagNo][RandValue];
}


inline void  CAIManager::SelectBAttackMethod(CHARLIST* Npc)		// 보스몬스터의 공격방법설정
{
	int AttackBagNo=GetAttackBagNo(Npc);						// NPC FAME이 공격방법을 지정하는 곳이다.
	Npc->fame=GetAttackMethodFromAttackBag(AttackBagNo);
	if (Npc->fame>=201)
	{
		int MagicBagNo=Npc->fame-200;
		if (MagicBagNo<1||MagicBagNo>m_iMagicBagCount) 
		{
			MyLog(0,"BOSS ACTION NPC MAGIC BAG INDEX ERROR.. CHECK ACTION NPC AND MAGICBAG");
			return;
		}
		int MagicNo=m_pMagicBag[Npc->fame-200][rand()%10];
		Npc->fame=MagicNo;
	}
}


inline void  CAIManager::ClearAttackMethod(CHARLIST* Npc)			// LTS DRAGON MODIFY
{
	Npc->fame=0;												// 공격 방법 초기화
}

inline CHARLIST* CAIManager::GetTarget(int TargetID)
{
	if (TargetID<0)
	{
		return NULL;
	}
	if (TargetID>=10000)								// NPC이면 
	{
		int tempTargetID=TargetID-10000;				// NPC번호 추출 
		if (NPCList[tempTargetID].bAlive>=DEAD_)		// 죽었으면 	
		{
			return NULL;								// NULL리턴
		}
		else
		{
			return &NPCList[tempTargetID];				// 살았으면 CHARLIST* 리턴
		}
	}
	else
	{
		if (connections[TargetID].dwAgentConnectionIndex&&connections[TargetID].state>=CONNECT_JOIN)		
		{   //< CSD-030509 : 연결상태 
			if (connections[TargetID].chrlst.bAlive>=DEAD_ || 
				connections[TargetID].chrlst.GetState() == CON_STONE ||
				connections[TargetID].chrlst.viewtype == VIEWTYPE_OBSERVE_)
			{
				return NULL;
			}
			else
			{
				return &connections[TargetID].chrlst;
			}
		}	//> CSD-030509
		else
		{
			return NULL;
		}
	}
}

inline int CAIManager::FindTarget(CHARLIST* Npc)
{
	Npc->targetid=NPC_IsWhoNearPC(Npc,GetNPCSearchRange(Npc));	// 15
	return Npc->targetid;
}

inline void CAIManager::FindTarget(CHARLIST* Npc,int& Target)
{
	Target=NPC_IsWhoNearPC(Npc,GetNPCSearchRange(Npc));	// 15
}  

inline int CAIManager::GetPhysicalAttackRange(CHARLIST* Npc)
{	//< CSD-021021
	return GetNpcAIData(NPC_Gen_Ref[Npc->npc_index].nAINo,FATTACK_RANGE);	// 물리공격
}	//> CSD-021021

inline int CAIManager::GetMagicAttackRange(CHARLIST* Npc,int MethodField)
{
	int MagicNo=GetNpcAIData(NPC_Gen_Ref[Npc->npc_index].nAINo,MethodField); // CSD-021021
	if (MagicNo>=201)
	{
		int Row=MagicNo-200;
		int Col=rand()%10;
		Npc->fame_pk=GetMagicBagData(Row,Col);								//번호 저장
		return Magic_Ref[Npc->fame_pk].invalid_TargetIT;				
	}
	else
	{
		return Magic_Ref[MagicNo].invalid_TargetIT;
	}
}

inline int CAIManager::GetSpecialAttackRange(CHARLIST* Npc,int Method)
{
	return 0;
}

inline int CAIManager::CheckAttackMethod(CHARLIST* Npc,int MethodField)
{
	if (GetNpcAIData(NPC_Gen_Ref[Npc->npc_index].nAINo,MethodField)==0) // CSD-021021
	{
		return GetPhysicalAttackRange(Npc);
	}
	else
	{
		return GetMagicAttackRange(Npc,MethodField);
	}
}

inline int CAIManager::CheckBAttackMethod(CHARLIST* Npc)
{
	if (Npc->fame==0)
	{
		return GetPhysicalAttackRange(Npc);
	}
	else
	{
		return  Magic_Ref[Npc->fame].invalid_TargetIT;
	}
}


inline int CAIManager::GetAttackRange(CHARLIST* Npc)
{
	switch(Npc->fame)													// 공격 방법
	{
	case 1 :	return CheckAttackMethod(Npc,FATTACK_METHOD1);
	case 2 :	return CheckAttackMethod(Npc,FATTACK_METHOD2);			// 마법공격
	case 3 :	return CheckAttackMethod(Npc,FATTACK_METHOD3);			// 특수공격	
	default :	return CheckBAttackMethod(Npc);
	}
}

inline int CAIManager::AttackableRange(CHARLIST* Npc)
{
	CHARLIST *ch;
	int tempRange;
	
	ch=GetTarget(Npc->targetid);
	if (!ch) return FALSE;
	if (Npc->fame==4) return TRUE;										// 소환이면 레인지 필요없음.
	
	tempRange=(ch->X-Npc->X)*(ch->X-Npc->X) + (ch->Y-Npc->Y)*(ch->Y-Npc->Y);
	if(tempRange<GetAttackRange(Npc)) 
	{
		return TRUE;
	}	
	return FALSE;
}

inline int CAIManager::CheckAndDoAttack(CHARLIST* Npc)
{	//< CSD-030120
	if (AttackableRange(Npc))										// 공격 가능 범위이면 
	{
		if (Npc->patterntype >= NPC_PATTERN_BOSS_WANDER) 
		{
			SendBAttack(Npc);											// 공격을 한다.
		}
		else 
		{
			SendAttack(Npc);											// 공격을 한다.
		}
		
		ClearMovePattern(Npc);										// 이동중 공격에 해당하면 예전의 상태를 클리어 한다.	
		// 공격방법을 변경하도록 함
		if (Npc->patterntype >= NPC_PATTERN_BOSS_WANDER) 
		{
			//SetAI(Npc,NPC_PATTERN_BOSS_ATTACK);
			SetAI(Npc, NPC_PATTERN_BOSS_ACCESS);
		}
		else 
		{ 
			//SetAI(Npc,NPC_PATTERN_ATTACK); // AIAttack으로 전환 한다.
			SetAI(Npc,NPC_PATTERN_ACCESS); 
		}
		
		SetAIDelayTime(Npc,FSTATUS03_DELAY,FSTATUS03_SUBDELAY);		// AI 딜레이 타임을 정한다.	// FSTATUS03 ATTACK
		return TRUE;
	}
	
	return FALSE;
}	//> CSD-030120

inline void CAIManager::SendMagicAttack(CHARLIST* Npc,int Field)
{
	int MagicNo=GetNpcAIData(NPC_Gen_Ref[Npc->npc_index].nAINo,Field); // CSD-021021
	if (MagicNo>=201)							// 201 : SStart Number of Special Attack 
	{
		g_pBattleManager->SendNpcMagic(Npc->GetServerID(),Npc->fame_pk);				
	}
	else
	{
		g_pBattleManager->SendNpcMagic(Npc->GetServerID(),MagicNo);
	}
}

inline void CAIManager::SendMagicBAttack(CHARLIST* Npc)
{
	g_pBattleManager->SendNpcMagic(Npc->GetServerID(),Npc->fame);
}


inline void CAIManager::SendPhysicalAttack(CHARLIST* Npc)
{
	SendAttackNPCArea(connections,Npc->GetServerID()); 
}

inline void CAIManager::CheckAndSendAttack(CHARLIST* Npc,int MethodField)
{
	if (GetNpcAIData(NPC_Gen_Ref[Npc->npc_index].nAINo,MethodField)==0) // CSD-021021
	{
		SendPhysicalAttack(Npc);
		if (Npc->patterntype>=NPC_PATTERN_BOSS_WANDER) SelectBAttackMethod(Npc);
		
	}
	else
	{
		SendMagicAttack(Npc,MethodField);
		if (Npc->patterntype>=NPC_PATTERN_BOSS_WANDER) SelectBAttackMethod(Npc);
		else SelectAttackMethod(Npc);				// 한번 공격을 했으면... 다른방법으로 바꾼다..
		SetAIDelayTime(Npc,1000);				// 테이블화 작업 필요..
	}
}

inline void CAIManager::CheckAndSendBAttack(CHARLIST* Npc)
{
	if (Npc->fame==0)
	{
		SendPhysicalAttack(Npc);
		SelectBAttackMethod(Npc);
	}
	else
	{
		SendMagicBAttack(Npc);
		SelectBAttackMethod(Npc);				// 한번 공격을 했으면... 다른방법으로 바꾼다..
		SetAIDelayTime(Npc,1000);				// 테이블화 작업 필요..
	}
}


inline void CAIManager::SendSummon(CHARLIST* Npc,int NpcIndex)			// LTS DRAGON MODIFY
{	//< CSD-030306
	int id	= GetAliveNPCList();
	int x = Npc->MoveSx;
	int y = Npc->MoveSy;
	
	if (id>0)
	{
		if (NPC_AdjustPosition(0,&x,&y))
		{
			g_pBattleManager->SendNpcRecall(Npc->GetServerID(), 95, NpcIndex, x, y);
			MyLog(0, "Summon Call No : %d, Count 1", NpcIndex);
		}
	}
}	//> CSD-030306

inline void CAIManager::SelectSummonNPC(CHARLIST* Npc)		// LTS DRAGON MODIFY
{	//< CSD-021021
	Npc->scan_time=g_curr_time+GetNpcAIData(NPC_Gen_Ref[Npc->npc_index].nAINo,FSUMMON_DELAY);		// 소환한 마지막 시간 기억
	int Npc_Index=rand()%6;			// 소환할 NPC선택
	switch(Npc_Index)
	{
	case 0 : break;			// MISS
	case 1 : SendSummon(Npc,GetNpcAIData(NPC_Gen_Ref[Npc->npc_index].nAINo,FSUMMON_NPC1)); break;
	case 2 : SendSummon(Npc,GetNpcAIData(NPC_Gen_Ref[Npc->npc_index].nAINo,FSUMMON_NPC2)); break;
	case 3 : SendSummon(Npc,GetNpcAIData(NPC_Gen_Ref[Npc->npc_index].nAINo,FSUMMON_NPC3)); break;
	case 4 : SendSummon(Npc,GetNpcAIData(NPC_Gen_Ref[Npc->npc_index].nAINo,FSUMMON_NPC4)); break;
	case 5 : SendSummon(Npc,GetNpcAIData(NPC_Gen_Ref[Npc->npc_index].nAINo,FSUMMON_NPC5)); break;
	}
	SelectAttackMethod(Npc);
}	//> CSD-021021

inline void CAIManager::SendAttack(CHARLIST* Npc)	
{
	switch(Npc->fame)										//공격 방법에 따라
	{
	case 1 :	CheckAndSendAttack(Npc,FATTACK_METHOD1); break;
	case 2 :	CheckAndSendAttack(Npc,FATTACK_METHOD2); break;
	case 3 :	CheckAndSendAttack(Npc,FATTACK_METHOD3); break;
	case 4 :    SelectSummonNPC(Npc); break;
	}
}

inline void CAIManager::SendNPCAction(CHARLIST* Npc,int Action)			// LTS DRAGON MODIFY
{
	t_packet packet;
	packet.h.header.type=CMD_AI_NPC_ACTION;
	packet.u.LocalWar.AINPCAction.ID=Npc->GetServerID();
	packet.u.LocalWar.AINPCAction.Action=Action;
	packet.h.header.size=sizeof(t_AINPCAction);
	
	CastNPC2Other(Npc->GetServerID(),&packet);
}

inline void CAIManager::SendBossAction(CHARLIST* Npc,int AttackMethod)
{
	switch(AttackMethod)
	{
	case 102 : SendNPCAction(Npc,4);	break;
	case 105 : SendNPCAction(Npc,4);	break;
	case 106 : SendNPCAction(Npc,8);	break;
	}
}

inline void CAIManager::SendBossAction(CHARLIST* Npc)				// LTS DRAGON ACTION
{
	if (Npc->IsCasting(g_curr_time))
	{
		switch(Npc->fame)
		{
		case 102 : SendNPCAction(Npc,4);	break;
		case 105 : SendNPCAction(Npc,4);	break;
		case 106 : SendNPCAction(Npc,8);	break;
		}
	}
}

inline void CAIManager::SendBAttack(CHARLIST* Npc)				// LTS DRAGON MODIFY
{
	SendBossAction(Npc);
	CheckAndSendBAttack(Npc);
}


inline void CAIManager::SetRunStatus(CHARLIST* Npc)
{	//< CSD-021021
	if (Npc->GetNature()==NAT_SPEED_UP) return;
	if (rand()%100<GetNpcAIData(NPC_Gen_Ref[Npc->npc_index].nAINo,FRUN_PERCENT))
	{ 
		Npc->SetNpcRun(GetNpcAIData(NPC_Gen_Ref[Npc->npc_index].nAINo,FRUN_DELAY)+g_curr_time,
			GetNpcAIData(NPC_Gen_Ref[Npc->npc_index].nAINo,FRUN_SPEED));
	}
}	//> CSD-021021

inline void CAIManager::SetRecoveryStatus(CHARLIST* Npc)
{	//< CSD-021021
	if (Npc->GetNature()==NAT_LIFE_UP) return;
	if (Npc->HpMax==Npc->Hp) return;
	if (rand()%100<GetNpcAIData(NPC_Gen_Ref[Npc->npc_index].nAINo,FRECOVERY_PERCENT))
	{
		Npc->SetNpcRecovery(GetNpcAIData(NPC_Gen_Ref[Npc->npc_index].nAINo,FRECOVERY_DELAY)+g_curr_time,
			GetNpcAIData(NPC_Gen_Ref[Npc->npc_index].nAINo,FRECOVERY_AMOUNT)*Npc->HpMax/100);
	}
}	//> CSD-021021

inline void CAIManager::CheckStatusUp(CHARLIST* Npc)
{	//< CSD-021021
	if (Npc->GetNature()==NAT_STATUS_UP) return;
	int LifePercent=Npc->Hp/Npc->HpMax*100;
	if (LifePercent<GetNpcAIData(NPC_Gen_Ref[Npc->npc_index].nAINo,FSTATUSUP_LIFE_PERCENT))		
	{
		if (rand()%100>GetNpcAIData(NPC_Gen_Ref[Npc->npc_index].nAINo,FSTATUSUP_PERCENT))
		{
			Npc->SetNpcStatusUp(GetNpcAIData(NPC_Gen_Ref[Npc->npc_index].nAINo,FSTATUSUP_DELAY)+g_curr_time,
				GetNpcAIData(NPC_Gen_Ref[Npc->npc_index].nAINo,FSTATUSUP1),
				GetNpcAIData(NPC_Gen_Ref[Npc->npc_index].nAINo,FSTATUSUP2),
				GetNpcAIData(NPC_Gen_Ref[Npc->npc_index].nAINo,FSTATUSUP3));
		}
	}
}	//> CSD-021021

inline void CAIManager::ProcessAIWander(CHARLIST* Npc)				// LTS DRAGON MODIFY
{
	ClearAttackMethod(Npc);											// 공격 방법을 초기화 한다.
	
	SetAIDelayTime(Npc,FSTATUS01_DELAY,FSTATUS01_SUBDELAY);			// 다음 AI딜레이 타임을 정한다.		// FSTATUS01 WANDER
	
	if (FindTarget(Npc)!=-1)										// 근처에 타겟이 있다.
	{
		SetAI(Npc,NPC_PATTERN_ACCESS);								// 다음 AI를 수행한다.	
		SetTarget(Npc,Npc->targetid);								// 타겟을 지정한다.	
		SetAIDelayTime(Npc,FSTATUS02_DELAY,FSTATUS02_SUBDELAY);		// AI 딜레이 타임을 정한다.	// FSTATUS02 ACCESS
	}
	else
	{
		RandomMove(Npc);
	}
}

inline void CAIManager::ProcessAIAccess(CHARLIST* Npc)
{
	SetAIDelayTime(Npc,FSTATUS02_DELAY,FSTATUS02_SUBDELAY);				// AI 딜레이 타임을 정한다.	// FSTATUS02 ACCESS
	CHARLIST* Target=GetTarget(Npc->targetid);
	if (Target!=NULL)
	{
		SelectAttackMethod(Npc);										// 공격 방법을 선택
		if (!CheckAndDoAttack(Npc))
		{
			int Target_ID;												// 주위에 더 가까운놈이 있는지 검사
			FindTarget(Npc,Target_ID);
			if (Target_ID!=-1)											// 더 가까운 적이 없다 
			{
				if (!CheckAndDoAttack(Npc))
				{
					SetTarget(Npc,Target_ID);
					TargetMove(Npc);									// 목표 근처로 이동한다.
				}
			}
		}
	}
	else
	{
		if (FindTarget(Npc)==-1)										//주위에 적이 없으면 
		{
			SetAIDefault(Npc);											//배회 상태로 한다.
			ClearTarget(Npc);											// 타겟 초기화
			SetAIDelayTime(Npc,FSTATUS01_DELAY,FSTATUS01_SUBDELAY);		
		}
		else
		{
			TargetMove(Npc);											// 목표 근처로 이동한다.
		}
	}
}

inline void CAIManager::ProcessAIAttack(CHARLIST* Npc)						// LTS DRAGON MODIFY
{
	SetAIDelayTime(Npc,FSTATUS03_DELAY,FSTATUS03_SUBDELAY);				// AI딜레이 타임을 계산		// FSTATUS03 ATTACK
	CHARLIST* Target=GetTarget(Npc->targetid);
	if (Target!=NULL)
	{
		if (!CheckAndDoAttack(Npc))										// 공격 가능 범위이면 
		{
			if (FindTarget(Npc)!=-1)
			{
				if (!CheckAndDoAttack(Npc))
				{
					SetAI(Npc,NPC_PATTERN_ACCESS);
				}
			}
			else 
			{
				SetAIDefault(Npc);
				ClearTarget(Npc);										// 타겟 초기화
				SetAIDelayTime(Npc,FSTATUS01_DELAY,FSTATUS01_SUBDELAY);
			}
		}
	}
	else
	{
		if (FindTarget(Npc)==-1)
		{
			SetAIDefault(Npc);											// 배회 상태로 한다.
			ClearTarget(Npc);											// 타겟 초기화
			SetAIDelayTime(Npc,FSTATUS01_DELAY,FSTATUS01_SUBDELAY);
		}
		else
		{
			SetAI(Npc,NPC_PATTERN_ACCESS);								// 적이 있으면 
		}
	}
}

inline void CAIManager::ProcessAIAttacked(CHARLIST* Npc)			// LTS DRAGON MODIFY
{
	SetAIDelayTime(Npc,FSTATUS04_DELAY,FSTATUS04_SUBDELAY);		// AI딜레이 타임을 계산		// FSTATUS04 ATTACKED
	CHARLIST* Target=GetTarget(Npc->targetid);
	if (Target!=NULL)
	{
		if (!CheckAndDoAttack(Npc))								// 공격 가능 범위가 아니면 
		{
			SetAI(Npc,NPC_PATTERN_ACCESS);
			SetRunStatus(Npc);									// 달리기 적용
			CheckStatusUp(Npc);									// 광폭적용
		}
	}
	else
	{
		SetAIDefault(Npc);										// 배회 상태로 한다.
		ClearTarget(Npc);										// 타겟 초기화
		SetAIDelayTime(Npc,FSTATUS01_DELAY,FSTATUS01_SUBDELAY);														
	}
}

inline void CAIManager::CheckMoveWander(CHARLIST* Npc)
{
	SetAIDelayTime(Npc,FSTATUS01_DELAY,FSTATUS01_SUBDELAY);		// AI딜레이 타임을 계산		// Wander와 같은 값
	if (FindTarget(Npc)!=-1)									// 근처에 타겟이 있다.
	{	
		if (Npc->EventPoint>=NPC_PATTERN_BOSS_WANDER)
		{
			ClearMovePattern(Npc);									//	예전 AI를 클리어 
			SetAI(Npc,NPC_PATTERN_BOSS_ACCESS);
		}
		else
		{
			ClearMovePattern(Npc);									//	예전 AI를 클리어 
			SetAI(Npc,NPC_PATTERN_ACCESS);
		}
		SetTarget(Npc,Npc->targetid);							// 타겟을 지정한다.	
	}
	else
	{
		if (!NPC_IsMoving(Npc))									// NPC가 움직이지 않고 잇다면
		{	
			SetAI(Npc,Npc->EventPoint);							// 원래 배회 ID로 세팅을 한다.	
			ClearMovePattern(Npc);						
		}
	}
}

inline void CAIManager::CheckAndModifyTarget(CHARLIST* Npc)
{
	LPCHARLIST Target=&connections[Npc->targetid].chrlst;
	int TargetX,TargetY;
	int TargetMoveX=Npc->tx*TILE_SIZE;
	int TargetMoveY=Npc->ty*TILE_SIZE;
	
	int ChangeRange=(4*32)*(4*32);					// 캐릭터의 이동하려는 좌표가 기존보다 3타일이상 차이난다면 패스빌드한다.
	int ResultRange=(Target->MoveGox-TargetMoveX)*(Target->MoveGox-TargetMoveX)+
		(Target->MoveGoy-TargetMoveY)*(Target->MoveGoy-TargetMoveY);
	
	if (ResultRange>ChangeRange)  //이동하는 위치가 다르다면 이동을 수정한다.
	{
		if (NPC_NearCh(Npc,Target,&TargetX,&TargetY))
		{
			if (NPC_MakePath(Npc,TargetX,TargetY,rand()%6+2))			// 타켓으로 부터의 위치 랜덤
			{
				SendModifyPositionNPC(connections,Npc->GetServerID());		
				SendMoveNPCArea( connections, Npc->GetServerID());
			}
		}
	}
}

inline void CAIManager::CheckMoveAccess(CHARLIST* Npc)
{
	SetAIDelayTime(Npc,FSTATUS05_DELAY,FSTATUS05_SUBDELAY); 	// AI딜레이 타임을 계산		// FSTATUS05 ACCESS MOVE
	
	if (!CheckAndDoAttack(Npc))							// 공격 가능 범위이면 
	{
		if (!NPC_IsMoving(Npc))							// NPC가 움직이지 않고 잇다면
		{	
			SetAI(Npc,Npc->EventPoint);					// 원래 배회 ID로 세팅을 한다.	
			ClearMovePattern(Npc);						
		}
		else											// 움직이고 있다면
		{
			switch (NPC_Gen_Ref[Npc->npc_index].nAttr)
			{	//< CSD-031027
			case AT_PIERCING:
			case AT_ANTIMAGIC:
			case AT_BOSS:
			case AT_SEMIBOSS:
				{
					CheckAndModifyTarget(Npc);
					break;
				}
			}	//> CSD-031027
			
		}
	}
	else
	{
	}
}


inline void CAIManager::ProcessAIMoving(CHARLIST* Npc)			// LTS DRAGON MODIFY	
{
	if (!Npc->EventPoint) return;							// NPC_MOVING으로 간 속성 Wander, Access, Attack
	switch (Npc->EventPoint)
	{
	case NPC_PATTERN_WANDER		 :	CheckMoveWander(Npc);	break;
	case NPC_PATTERN_ACCESS		 :	CheckMoveAccess(Npc);	break;
	}
}

inline int** CAIManager::GetBossStatusData()
{
	return m_pBossStatusData;
}

inline int* CAIManager::GetBossStatusData(int TypeNo)
{
	return m_pBossStatusData[TypeNo];
}

inline int CAIManager::GetBossStatusData(int TypeNo,int Field)
{
	return m_pBossStatusData[TypeNo][Field];
}

inline int CAIManager::inHPRange(CHARLIST* Npc,int sField,int eField,int Percent)
{
	if (GetBossStatusData(Npc->CurrentPhase,sField)>Percent&&GetBossStatusData(Npc->CurrentPhase,eField)<=Percent) return TRUE;
	return FALSE;
}

inline int CAIManager::CheckHPGrade(CHARLIST* Npc)		// LTS DRAGON MODIFY
{
	int HPPercent=(Npc->Hp*100)/Npc->HpMax;
	
	if (HPPercent==100) return 1;
	if (inHPRange(Npc,FBHP_GRADE1,FBHP_GRADE2,HPPercent)) return 1;
	if (inHPRange(Npc,FBHP_GRADE2,FBHP_GRADE3,HPPercent)) return 2;
	if (inHPRange(Npc,FBHP_GRADE3,FBHP_GRADE4,HPPercent)) return 3;
	if (inHPRange(Npc,FBHP_GRADE4,FBHP_GRADE5,HPPercent)) return 4;
	if (inHPRange(Npc,FBHP_GRADE5,FBHP_GRADE6,HPPercent)) return 5;
	if (GetBossStatusData(Npc->CurrentPhase,FBHP_GRADE6)>HPPercent&&0<=HPPercent) return 6;
	
	return 0;
}

inline void CAIManager::SetActionFunc(CHARLIST* Npc,int HPGrade,int* Action)
{
	switch(HPGrade)
	{
	case 0 :
		Action[0]=GetBossStatusData(Npc->CurrentPhase,FBACTION0_METHOD1);
		Action[1]=GetBossStatusData(Npc->CurrentPhase,FBACTION0_METHOD2);
		Action[2]=GetBossStatusData(Npc->CurrentPhase,FBACTION0_METHOD3);					break;
	case 1 :
		Action[0]=GetBossStatusData(Npc->CurrentPhase,FBACTION1_METHOD1);
		Action[1]=GetBossStatusData(Npc->CurrentPhase,FBACTION1_METHOD2);
		Action[2]=GetBossStatusData(Npc->CurrentPhase,FBACTION1_METHOD3);					break;
	case 2 :
		Action[0]=GetBossStatusData(Npc->CurrentPhase,FBACTION2_METHOD1);
		Action[1]=GetBossStatusData(Npc->CurrentPhase,FBACTION2_METHOD2);
		Action[2]=GetBossStatusData(Npc->CurrentPhase,FBACTION2_METHOD3);					break;
	case 3 :
		Action[0]=GetBossStatusData(Npc->CurrentPhase,FBACTION3_METHOD1);
		Action[1]=GetBossStatusData(Npc->CurrentPhase,FBACTION3_METHOD2);
		Action[2]=GetBossStatusData(Npc->CurrentPhase,FBACTION3_METHOD3);					break;
	case 4 :
		Action[0]=GetBossStatusData(Npc->CurrentPhase,FBACTION4_METHOD1);
		Action[1]=GetBossStatusData(Npc->CurrentPhase,FBACTION4_METHOD2);
		Action[2]=GetBossStatusData(Npc->CurrentPhase,FBACTION4_METHOD3);					break;
	case 5 :
		Action[0]=GetBossStatusData(Npc->CurrentPhase,FBACTION5_METHOD1);
		Action[1]=GetBossStatusData(Npc->CurrentPhase,FBACTION5_METHOD2);
		Action[2]=GetBossStatusData(Npc->CurrentPhase,FBACTION5_METHOD3);					break;
	case 6 :
		Action[0]=GetBossStatusData(Npc->CurrentPhase,FBACTION6_METHOD1);
		Action[1]=GetBossStatusData(Npc->CurrentPhase,FBACTION6_METHOD2);
		Action[2]=GetBossStatusData(Npc->CurrentPhase,FBACTION6_METHOD3);					break;
	}
}


inline void CAIManager::CallActionFunc(CHARLIST* Npc,int* Action)
{
	if (Action[0]>0) (*ActionFunc[m_pActionBag[Action[0]][0]-1])(Npc,m_pActionBag[Action[0]][1],m_pActionBag[Action[0]][2]);
	if (Action[1]>0) (*ActionFunc[m_pActionBag[Action[1]][0]-1])(Npc,m_pActionBag[Action[1]][1],m_pActionBag[Action[1]][2]);
	if (Action[2]>0) (*ActionFunc[m_pActionBag[Action[2]][0]-1])(Npc,m_pActionBag[Action[2]][1],m_pActionBag[Action[2]][2]);
}

inline void CAIManager::OnHPAction(CHARLIST* Npc,int HPGrade)		// LTS DRAGON MODIFY
{
	if (HPGrade!=0&&Npc->EventLocalWarJoin) return;								// 이미 HP그레이드 상태를 실행한놈이다.
	
	int Action[3];
	
	SetActionFunc(Npc,HPGrade,Action);
	
	t_packet  packet;
	packet.h.header.type=CMD_AI_ACTION_TEST;
	if (Action[0]>0) packet.u.LocalWar.ActionTest.Action[0]=m_pActionBag[Action[0]][0];
	else packet.u.LocalWar.ActionTest.Action[0]=0;
	if (Action[1]>0) packet.u.LocalWar.ActionTest.Action[1]=m_pActionBag[Action[1]][0];
	else packet.u.LocalWar.ActionTest.Action[1]=0;
	if (Action[2]>0) packet.u.LocalWar.ActionTest.Action[2]=m_pActionBag[Action[2]][0];
	else packet.u.LocalWar.ActionTest.Action[2]=0;
	packet.h.header.size=sizeof(t_ActionTest);
	
	CastNPC2Other(Npc->GetServerID(),&packet);
	
	MyLog(0,"NPC : %d, HPAction : %d,%d,%d",Npc->SprNo,Action[0],Action[1],Action[2]);
	
	CallActionFunc(Npc,Action);
	
	Npc->EventLocalWarJoin=TRUE;	// 이미 처리했다고 기록 
}

inline void CAIManager::OnCloseHPAction(CHARLIST* Npc)
{
	int Action[3];
	
	SetActionFunc(Npc,Npc->mantle,Action);
	
	if (Action[0]==6||Action[0]==1)	// 액션중 회피이거나 대기이면 
	{
		SetAI(Npc,NPC_PATTERN_BOSS_ACCESS);
	}
	if (Action[1]==6||Action[1]==1)	// 액션중 회피이거나 대기이면 
	{
		SetAI(Npc,NPC_PATTERN_BOSS_ACCESS);
	}
	if (Action[2]==6||Action[2]==1)	// 액션중 회피이거나 대기이면 
	{
		SetAI(Npc,NPC_PATTERN_BOSS_ACCESS);
	}
	
	Npc->EventLocalWarJoin=FALSE;
}

inline void CAIManager::ProcessAIBWander(CHARLIST* Npc)			// LTS DRAGON MODIFY
{
	ClearAttackMethod(Npc);											// 공격 방법을 초기화 한다.
	
	SetAIDelayTime(Npc,FSTATUS01_DELAY,FSTATUS01_SUBDELAY);			// 다음 AI딜레이 타임을 정한다.		// FSTATUS01 WANDER
	
	if (FindTarget(Npc)!=-1)										// 근처에 타겟이 있다.
	{
		ClearMovePattern(Npc);										//	예전 AI를 클리어 
		SetAI(Npc,NPC_PATTERN_BOSS_ACCESS);
		SetTarget(Npc,Npc->targetid);								// 타겟을 지정한다.	
		OnHPAction(Npc,0);												// 강제로 0번 액션을 수행
	}
}

inline void CAIManager::ProcessAIBAccess(CHARLIST* Npc)					// LTS DRAGON MODIFY
{
	SetAIDelayTime(Npc,FSTATUS02_DELAY,FSTATUS02_SUBDELAY);				// AI 딜레이 타임을 정한다.	// FSTATUS02 ACCESS
	CHARLIST* Target=GetTarget(Npc->targetid);
	if (Target!=NULL)
	{
		SelectBAttackMethod(Npc);										// 공격 방법을 선택
		if (!CheckAndDoAttack(Npc))
		{
			int Target_ID;												// 주위에 더 가까운놈이 있는지 검사
			FindTarget(Npc,Target_ID);
			if (Target_ID!=-1)											// 더 가까운 적이 없다 
			{
				if (!CheckAndDoAttack(Npc))
				{
					SetTarget(Npc,Target_ID);
					TargetMove(Npc);									// 목표 근처로 이동한다.
				}
			}
			else
			{
				SetAIBDefault(Npc);											//배회 상태로 한다.
				ClearTarget(Npc);											// 타겟 초기화
				SetAIDelayTime(Npc,FSTATUS01_DELAY,FSTATUS01_SUBDELAY);		
			}
		}
	}
	else
	{
		if (FindTarget(Npc)==-1)										//주위에 적이 없으면 
		{
			SetAIBDefault(Npc);											//배회 상태로 한다.
			ClearTarget(Npc);											// 타겟 초기화
			SetAIDelayTime(Npc,FSTATUS01_DELAY,FSTATUS01_SUBDELAY);		
		}
		else
		{
			TargetMove(Npc);											// 목표 근처로 이동한다.
		}
	}
}

inline void CAIManager::ProcessAIBAttack(CHARLIST* Npc)
{
	SetAIDelayTime(Npc,FSTATUS03_DELAY,FSTATUS03_SUBDELAY);				// AI딜레이 타임을 계산		// FSTATUS03 ATTACK
	CHARLIST* Target=GetTarget(Npc->targetid);
	if (Target!=NULL)
	{
		if (!CheckAndDoAttack(Npc))										// 공격 가능 범위이면 
		{
			if (FindTarget(Npc)!=-1)
			{
				if (!CheckAndDoAttack(Npc))
				{
					SetAI(Npc,NPC_PATTERN_BOSS_ACCESS);
				}
			}
			else 
			{
				SetAIBDefault(Npc);
				ClearTarget(Npc);										// 타겟 초기화
				SetAIDelayTime(Npc,FSTATUS01_DELAY,FSTATUS01_SUBDELAY);
			}
		}
	}
	else
	{
		if (FindTarget(Npc)==-1)
		{
			SetAIBDefault(Npc);											// 배회 상태로 한다.
			ClearTarget(Npc);											// 타겟 초기화
			SetAIDelayTime(Npc,FSTATUS01_DELAY,FSTATUS01_SUBDELAY);
		}
		else
		{
			SetAI(Npc,NPC_PATTERN_BOSS_ACCESS);								// 적이 있으면 
		}
	}
}

void CAIManager::ProcessAIBAttacked(CHARLIST* Npc)
{
	// 공격당하면 뭔가 해준다.
}

inline void CAIManager::ProcessAIBMoving(CHARLIST* Npc)
{
	if (!Npc->EventPoint) return;							// NPC_MOVING으로 간 속성 Wander, Access, Attack
	switch (Npc->EventPoint)
	{
	case NPC_PATTERN_BOSS_WANDER :	CheckMoveWander(Npc);	break;
	case NPC_PATTERN_BOSS_ACCESS :	CheckMoveAccess(Npc);	break;
	}
}

inline void CAIManager::ProcessAIBBackDraw(CHARLIST* Npc)
{
	SetAIDelayTime(Npc,FSTATUS06_DELAY,FSTATUS06_SUBDELAY);				// AI딜레이 타임을 계산		// FSTATUS03 ATTACK
	
	if (NPC_IsMoving(Npc)) return;
	
	CHARLIST* Target=GetTarget(Npc->targetid);
	if (Target!=NULL)
	{
		if (InDistance(Npc,Target,32*10))
		{
			int ex,ey;
			if (NPC_NearBackCh(Npc,Target,&ex,&ey,10))	// 뒤로 도망
			{
				NPC_MakePath(Npc,ex,ey,rand()%10+5);
			}
			else
			{
				FindTarget(Npc);
			}
		}
	}
	else
	{
		FindTarget(Npc);
	}
}

inline void CAIManager::ProcessAIBAvoid(CHARLIST* Npc)						// 이상태는 HP액션에서 처리한다.	// 탈출은 OnHPActionClose
{
	SetAIDelayTime(Npc,FSTATUS03_DELAY,FSTATUS03_SUBDELAY);//임시로3				// AI딜레이 타임을 계산		// FSTATUS03 ATTACK
	// 몇타일 뒤로 도망후 랜덤치로 영역에 있다면 공격한다.
	if (NPC_IsMoving(Npc)) return;
	
	CHARLIST* Target=GetTarget(Npc->targetid);
	if (Target!=NULL)
	{
		if (InDistance(Npc,Target,32*5))	// 5타일 안에 적이있다면
		{
			int ex,ey;
			if (NPC_NearBackCh(Npc,Target,&ex,&ey,5))	// 뒤로 도망
			{
				NPC_MakePath(Npc,ex,ey,rand()%10+5);
				SelectBAttackMethod(Npc);
			}
		}
		else		// 5타일 안에 적이 없다면	// Search레인지를 뒤진다
		{
			if (FindTarget(Npc)!=-1)	//적이있다면
			{
				CheckAndDoAttack(Npc);	// 어택레인지에 있다면
			}
		}
	}
	else
	{
		FindTarget(Npc);	
	}
}

inline void CAIManager::ProcessAIBWait(CHARLIST* Npc)						// 이상태는 HP액션에서 처리해 준다.	// 탈출은 OnHPActionClose
{
	SetAIDelayTime(Npc,FSTATUS08_DELAY,FSTATUS08_SUBDELAY);				// AI딜레이 타임을 계산		// FSTATUS03 ATTACK
	//이상태에서는 아무것도 하지않는다.									// Attacked나 액션 상태가 바뀌면 처리된다
}

inline void CAIManager::CheckHPStatus(CHARLIST* Npc) // LTS DRAGON MODIFY
{
	int HPGrade=CheckHPGrade(Npc);
	if (HPGrade!=Npc->mantle)				// OLD HP STATUS와 비교
	{	// 새로운 HP상태에 들어옴
		if (DRAGON_HEART_INDEX_NO==Npc->npc_index)	//드래곤하트이면 HP가 체인지될때.. 처리해준다.
		{
			if (CheckDragonPtr(Npc))
			{
				m_pDragon->Hp=Npc->Hp;
			}
		}
		OnCloseHPAction(Npc);
		Npc->mantle=HPGrade;
		OnHPAction(Npc,HPGrade);
	}
}

inline CHARLIST* CAIManager::GetNearNPC(CHARLIST* Npc,int IndexNo,int Range) // LTS DRAGON MODIFY
{
	for (int i=NPC_LIST_START;i<MAX_NPC_LIST;i++)
	{
		if (IndexNo==NPCList[i].npc_index)
		{
			int tempRange=(Npc->X-NPCList[i].X)*(Npc->X-NPCList[i].X) + (Npc->Y-NPCList[i].Y)*(Npc->Y-NPCList[i].Y);
			tempRange=sqrt((float)tempRange);
			if (tempRange<Range)
			{
				return &NPCList[i];
			}
		}
	}
	return NULL;
}

inline int CAIManager::CheckDragonPtr(CHARLIST* Npc) // LTS DRAGON MODIFY, LTS DRAGONLORD
{
	if (!m_pDragon)
	{
		m_pDragon=GetNearNPC(Npc,DRAGON_INDEX_NO,3000);
		if (!m_pDragon)
		{
			m_pDragon=GetNearNPC(Npc,DRAGON_LORD_INDEX_NO,3000);
			if (!m_pDragon)
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

CHARLIST* CAIManager::GetDragonPtr(CHARLIST* Npc) // LTS DRAGON MODIFY
{
	if (CheckDragonPtr(Npc))
	{
		return m_pDragon;
	}
	return NULL;
}

inline void CAIManager::CheckDragonHeartDeath(CHARLIST* Defender) // LTS DRAGON MODIFY
{
	if (DRAGON_HEART_INDEX_NO!=Defender->npc_index) return;
	
	if (CheckDragonPtr(Defender))
	{
		KillMonster(m_pDragon->GetServerID());
	}
	
}

void CAIManager::CheckDeath(CHARLIST* Defender) // LTS DRAGON MODIFY
{
	if (!Defender->IsNpc()) return;
	
	CheckSummonDeath(Defender);
	CheckDragonHeartDeath(Defender);
}


void CAIManager::ProcessNPCAI(CHARLIST* Npc)
{
	if (!isProcess(Npc))
	{
		return;
	}

	switch (Npc->patterntype)
	{
	case NPC_PATTERN_WANDER			: ProcessAIWander(Npc);		break;
	case NPC_PATTERN_ACCESS			: ProcessAIAccess(Npc);		break;
	case NPC_PATTERN_ATTACK			: ProcessAIAttack(Npc);		break;
	case NPC_PATTERN_ATTACKED		: ProcessAIAttacked(Npc);	break;
	case NPC_PATTERN_MOVING			: ProcessAIMoving(Npc);		break;
		
	case NPC_PATTERN_BOSS_WANDER	: ProcessAIBWander(Npc);	break;
	case NPC_PATTERN_BOSS_ACCESS	: ProcessAIBAccess(Npc);	break;
	case NPC_PATTERN_BOSS_ATTACK	: ProcessAIBAttack(Npc);	break;
	case NPC_PATTERN_BOSS_MOVING	: ProcessAIBMoving(Npc);	break;
	case NPC_PATTERN_BOSS_BACKDRAW	: ProcessAIBBackDraw(Npc);	break;
	case NPC_PATTERN_BOSS_WAIT		: ProcessAIBWait(Npc);		break;
	case NPC_PATTERN_BOSS_AVOID		: ProcessAIBAvoid(Npc);		break;
	}
	
	if (Npc->patterntype>=NPC_PATTERN_BOSS_WANDER)	// AI를 끝낸다음에 상태변경을 한다.
	{
		CheckHPStatus(Npc);
	}
}