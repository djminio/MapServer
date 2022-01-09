#include "..\stdafx.h"
#include "..\LowerLayers\MyLog.h"
#include "AdventManager.h"
#include "LogManager.h"

CHARLIST NPCList[MAX_NPC_LIST];
int CurrentNPCNo = 0; // 현재 NPC의 총수.
int Max_NPC_Generation = 0;

void InitNPCList()
{	
	for (int i = 0; i < MAX_NPC_LIST; ++i)
	{
		NPCList[i].bAlive = REMOVE_;
	}
}	
	
int GetAliveNPCList()
{	
	for (int i = NPC_LIST_START; i < MAX_NPC_LIST; ++i)
	{
		if (NPCList[i].bAlive == REMOVE_)
		{
			//MyLog(LOG_NORMAL, "<%d>GetAliveNPCList : %d", i, CurrentNPCNo); // CSD-LOG
			return i;
		}
	}

	return -1;
}
	
int AddNPCList(int npcindex, int x, int y)
{	//< CSD-031013
	return (GetAliveNPCList() < 0) ? -1:0;
}	//> CSD-031013

void RemoveNPCList(int Num)
{	//< CSD-031013
	if (NPCList[Num].bAlive == REMOVE_)
	{
		return;
	}
	// NPC발생하는 위치의 현재 발생된 값을 하나 감소 해준다. ( 그래야 또 발생하지 .... )
	CHARLIST* pNpc = &NPCList[Num];
	if (!pNpc) return;

	switch (pNpc->generationtype)
	{								
	case GT_SKB_FILE:	
		{
			const int nIndex = pNpc->generationpos;

			if (nIndex < 0 || nIndex >= NPCgeneratePositionMax)
			{
				MyLog(LOG_DEBUG, "NPCgeneratePosition[%d] is invalided!", nIndex);
				return;
			}

			if (--NPCgeneratePosition[nIndex].curnpc < 0) 
			{
				MyLog(LOG_DEBUG, "NPCgeneratePosition[%d].curnpc < 0", nIndex);
				return;
			}

			break;
		}
	case GT_EVENT_MONSTER: // LTS AI2
		{
			g_pAdventManager->RemoveEventMonster(pNpc);
			break;
		}
	case GT_SCRIPT:
	case GT_SKILL_SUMMON:
	case GT_SCENARIO_MONSTER:
	case GT_EVENT_NPC:
	case GT_HUNT_MONSTER:
		{
			break;
		}
	}
	
	if (pNpc->eventno > 0)
	{
		g_pLogManager->SaveDeadEventNpc(pNpc);
	}	
	
	memset(pNpc, 0, sizeof(CHARLIST));
	--CurrentNPCNo;
	//MyLog(LOG_NORMAL, "<%d>RemoveNPCList : %d", Num, CurrentNPCNo);	// CSD-LOG		
	SetTileOccupied(pNpc->MoveSx, pNpc->MoveSy, false);
}	//> CSD-031013
	
//021030 YGI
// 아이템 떨어 뜨리지 않는다.
// 캐릭터 눈앞에서 갑자기 없어진다.
void DeleteNpc(int index)
{	
	CHARLIST *pNpc = &NPCList[index];
	if (!pNpc) return;

	if (REMOVE_ == pNpc->bAlive) //이미 리무브 상태라면 이것을 호출 하지 않는다.
	{
		return;
	}

	pNpc->bAlive = BUFE_;
	pNpc->deadcount = 0;
	RemoveNPCList(index);
	SetArea(REMOVE_NPC_AREA, index);
}