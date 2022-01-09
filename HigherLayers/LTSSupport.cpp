#include "..\stdafx.h"

#include <stdio.h>
#include "../LowerLayers/MyLog.h"
#include "../LowerLayers/servertable.h"

#include "SealStone.h"
#include "CItem.h"
#include "NationSys.h"
#include "LTSSupport.h"
#include "Scrp_exe.h"
#include "dungeon.h"
#include "LogManager.h"
#include "AIManager.h"
#include "UserManager.h"

extern void RecvReqResetAbility( short int cn );
extern bool isLevelUp(CHARLIST*);

//int localWarMsgDebug = 0; //Eleval - To stop spamming Local War Msg Log after 3 days
#define	ABILITY_REDISTRIBUTION_MONEY			1000000000		// 10억 크릿

cLocalWarfield*			g_pLocalWarfield=NULL;
cEventLocalWarfield*	g_pEventLocalWarfield=NULL;				// 020115 LTS
cDragonLordWar			g_pDragonLordWar;					// LTS DRAGONLORD

int					g_isLocalWarServer=0;
int					g_LocalWarBegin=0;
int					g_EventLocalWarBegin=0;						// 020115 LTS
int					g_EventLocalWarStatus=0;					// 020115 LTS
int					g_EventLocalWarStep=0;						// 020115 LTS
int					g_isEventLocalWarServer=0;					// 020115 LTS
int					g_EventMapPort=0;							// 020115 LTS	
POINTS				g_StartPoint[6];							// 나라별 2개씩
DWORD				g_AbilityRedistributionMoney[10];
int					g_LocalWarResult[3]={1,1,1};				// LTS NEW LOCALWAR
POINTS				g_LocalWarMovePoint[7][3];					// LTS NEW LOCALWAR



// Extern Variable Define _______________________________________________________________
extern cNation*				g_pNation;
extern WORD					g_wMapServerPort;
extern int*					g_pNationWar_Exp;		// LTS NEW LOCALWAR

// Extern Variable Define End ___________________________________________________________
// Extern Function Define________________________________________________________________
extern bool			isNationManageServer();
extern void			SendPacket2NWManager(t_packet* p);
extern void			SendPacket2Maps(t_packet* p);						
extern void			SetShape( unsigned char *equip, short int cn );
extern void			SendPacket2NationClients(const int Nation,t_packet* p);	//020903 lsw
extern void			SendPacket2NationMaps(const int Nation,t_packet* p);//020903 lsw
extern CHARLIST* CheckServerId(WORD id);
extern ItemAttr*	SearchInv( CHARLIST *ch );			// 011214 LTS
extern void			MoveEmptyInv( ItemAttr *item, CHARLIST *ch );		// 011215 LTS
extern void			SendPacket2NWMap(const WORD Port,t_packet* p);			// 020115 LTS//020903 lsw

// Extern Function Define End____________________________________________________________

extern unsigned short	g_DefensePoint[];	// BBD 040401

int Pos(char* PosString,char KeyChar)					// LTS NEW LOCALWAR		// 범용적인 함수 아님
{
	int StringLength=strlen(PosString);
	for (int i=0;i<StringLength;i++)
	{
		if (PosString[i]==KeyChar) return i;
	}
	return -1;
}

void DeleteString(char* DeleteString,int Pos)			// LTS NEW LOCALWAR		// 범용적인 함수 아님
{
	char tempString[MAX_PATH] = {0,};
	int StringLength=strlen(DeleteString);
	int j=0;
	for (int i=Pos;i<StringLength;i++)
	{
		tempString[j]=DeleteString[i];
		j++;
	}
	tempString[j]=NULL;
	memset(DeleteString,0,MAX_PATH);
	sprintf_s(DeleteString, MAX_PATH, tempString);
}

void CopyString(char* SrcString,char* DestString,int Pos)			// LTS NEW LOCALWAR		// 범용적인 함수 아님
{
	int i;
	for (i=0;i<Pos;i++)
	{
		DestString[i]=SrcString[i];
	}
	DestString[i]=NULL;
}

int InitMapServerConfigINI()	// 020115 LTS		// LTS NEW LOCALWAR
{
	// 030919 HK YGI
//	char	NetworkDir[MAX_PATH];
//	char	MapServerConfigFileName[MAX_PATH];
	char	tempString[MAX_PATH];
	char	tempString2[MAX_PATH];

	SERVER_DATA	*pData=g_pServerTable->GetOwnServerData();
	short MapServerPort=pData->wPort;


/*	if (GetPrivateProfileString("Network","path","",NetworkDir,MAX_PATH, MAP_SERVER_INI_)<=0)
	{
		MyLog(0,"Error!! Check MapServerConfig.ini Path Setting!!");
		return FALSE;
	}
	sprintf(MapServerConfigFileName,"%s/data/MapServerConfig.ini",NetworkDir);*/// 030919 HK YGI
	int i;
	for (i=0;i<10;i++)
	{
		sprintf_s(tempString, sizeof(tempString),"NeedMoney%02d",i);
		g_AbilityRedistributionMoney[i]=(int)GetPrivateProfileInt("AbilityRedistribution",tempString,0,MapServerConfigFileName);
	}

	int Position;
	char tempString3[MAX_PATH];
	for (i=0;i<7;i++)
	{
		sprintf_s(tempString, sizeof(tempString),"LocalWarfield%dBYMovePoint",i);
		GetPrivateProfileString("LocalWarMovePoint",tempString,"Error",tempString2,MAX_PATH,MapServerConfigFileName);
		if (strcmp(tempString2,"Error")==0)
		{
			JustMsg("Error!! LocalWarMovePoint Read Error!! Check MapServerConfig.ini");
			return FALSE;
		}

		Position=Pos(tempString2,',');
		CopyString(tempString2,tempString3,Position);
		g_LocalWarMovePoint[i][0].x=atoi(tempString3);
		DeleteString(tempString2,Position+1);
		g_LocalWarMovePoint[i][0].y=atoi(tempString2);

		sprintf_s(tempString, sizeof(tempString),"LocalWarfield%dZYMovePoint",i);
		GetPrivateProfileString("LocalWarMovePoint",tempString,"Error",tempString2,MAX_PATH,MapServerConfigFileName);
		if (strcmp(tempString2,"Error")==0)
		{
			JustMsg("Error!! LocalWarMovePoint Read Error!! Check MapServerConfig.ini");
			return FALSE;
		}

		Position=Pos(tempString2,',');
		CopyString(tempString2,tempString3,Position);
		g_LocalWarMovePoint[i][1].x=atoi(tempString3);
		DeleteString(tempString2,Position+1);
		g_LocalWarMovePoint[i][1].y=atoi(tempString2);

		sprintf_s(tempString, sizeof(tempString),"LocalWarfield%dYLMovePoint",i);
		GetPrivateProfileString("LocalWarMovePoint",tempString,"Error",tempString2,MAX_PATH,MapServerConfigFileName);
		if (strcmp(tempString2,"Error")==0)
		{
			JustMsg("Error!! LocalWarMovePoint Read Error!! Check MapServerConfig.ini");
			return FALSE;
		}

		Position=Pos(tempString2,',');
		CopyString(tempString2,tempString3,Position);
		g_LocalWarMovePoint[i][2].x=atoi(tempString3);
		DeleteString(tempString2,Position+1);
		g_LocalWarMovePoint[i][2].y=atoi(tempString2);
	}
	return TRUE;
}

DWORD GetAbilityRedistributionMoney(t_connection c[], int cn)		// 020115 LTS
{	//< CSD-030806
	const int Index = c[cn].chrlst.GetLevel()/10;
	
	if (Index >= 9)
	{
		return g_AbilityRedistributionMoney[9];
	}
	
	return g_AbilityRedistributionMoney[Index];
}	//> CSD-030806

void RecvCMD_CHECK_ABILITY_CHANGE_MONEY( t_packet *p, t_connection c[], int cn )	// 020115 LTS
{
	t_packet packet;
	packet.h.header.type=CMD_CHECK_ABILITY_CHANGE_MONEY;				
	packet.h.header.size=sizeof(t_CommonDataC);
	CHARLIST *ch = CheckServerId(cn);
	if(!ch){return;}
	if (!ch->IsDual()) // 듀얼이 아니면 실행
	{
		SendCMD_COMFORM_RESET_ABILITY(ch);//020820 lsw
		return;
	}
	else 
	{
		packet.u.NationWar.CommonDataC.Data=5;		// 듀얼실패
	}
		
	QueuePacket(c,cn,&packet,1);
}


void MakeSealStoneNumber(char* SrcString,int SealNo[4])				// LTS NEW LOCALWAR		// 범용적인 함수 아님
{
	int Position;
	char tempString[MAX_PATH];
	char tempSrcString[MAX_PATH];

	strcpy_s(tempSrcString, sizeof(tempSrcString),SrcString);
	
	int i;
	for(i=0;i<3;i++)
	{
		Position=Pos(tempSrcString,',');
		CopyString(tempSrcString,tempString,Position);
		SealNo[i]=atoi(tempString);
		DeleteString(tempSrcString,Position+1);
	}

	SealNo[i]=atoi(tempSrcString);
}

// Where : Call From Main.cpp InitDrmapServerDatas()
// Usage : Initializing Local War System
bool InitLocalWarSystem()				// LTS NEW LCOALWAR
{
	// 030919 HK YGI
//	char	NetworkDir[MAX_PATH];
//	char	MapServerConfigFileName[MAX_PATH];
	char	KeyName[MAX_PATH];
	int		LocalWarfieldSize=0;
	int		tempPort;
	int		MapServerPort;
	char	tempString[MAX_PATH];

	SERVER_DATA	*pData=g_pServerTable->GetOwnServerData();
	MapServerPort=pData->wPort;


/*	if (GetPrivateProfileString("Network","path","",NetworkDir,MAX_PATH, MAP_SERVER_INI_)<=0)
	{
		MyLog(0,"Mapserver.ini의  Path설정이 잘못되어있습니다.");
		return false;
	}
	sprintf(MapServerConfigFileName,"%s/data/MapServerConfig.ini",NetworkDir);*/// 030919 HK YGI
	LocalWarfieldSize=(DWORD)GetPrivateProfileInt("LocalWarfield","LocalWarfieldSize",0,MapServerConfigFileName);
	if (!LocalWarfieldSize)
	{
		MyLog(0,"%s의 [LocalWarfield],LocalWarfieldSize의 설정이 잘못되어있습니다.",MapServerConfigFileName);
		return false;
	}

	for (int i=0;i<LocalWarfieldSize;i++)
	{
		sprintf_s(KeyName, sizeof(KeyName),"LocalWarfield%dPort",i);
		tempPort=(DWORD)GetPrivateProfileInt("LocalWarfield",KeyName,0,MapServerConfigFileName);
		if (tempPort) 
		{
			if (tempPort==MapServerPort)
			{
				if (NULL==g_pLocalWarfield)
				{
					g_pLocalWarfield=new cLocalWarfield;
					LocalWarfield_Info tempLF;

					g_isLocalWarServer=1;

					tempLF.Index=i;
					sprintf_s(KeyName, sizeof(KeyName),"LocalWarfield%dName",i);
					if (GetPrivateProfileString("LocalWarfield",KeyName,"",tempLF.MapName,20,MapServerConfigFileName)<=0)
					{
						return false;
					}
					sprintf_s(KeyName, sizeof(KeyName),"LocalWarfield%dPort",i);
					tempLF.Port=(DWORD)GetPrivateProfileInt("LocalWarfield",KeyName,0,MapServerConfigFileName);
					if (!tempLF.Port)
					{
						return false;
					}
					sprintf_s(KeyName, sizeof(KeyName),"LocalWarfield%dPhase",i);
					tempLF.Phase=(DWORD)GetPrivateProfileInt("LocalWarfield",KeyName,0,MapServerConfigFileName);
					if (!tempLF.Phase)
					{
						if (i) return false;
					}

					sprintf_s(KeyName, sizeof(KeyName),"LocalWarfield%dBYSealStone",i);
					GetPrivateProfileString("LocalWarfield",KeyName,NULL,tempString,MAX_PATH,MapServerConfigFileName);
					if (tempString==NULL)
					{
						return false;
					}

					MakeSealStoneNumber(tempString,tempLF.SealNo[0]);

					sprintf_s(KeyName, sizeof(KeyName),"LocalWarfield%dZYSealStone",i);
					GetPrivateProfileString("LocalWarfield",KeyName,NULL,tempString,MAX_PATH,MapServerConfigFileName);
					if (tempString==NULL)
					{
						return false;
					}

					MakeSealStoneNumber(tempString,tempLF.SealNo[1]);

					sprintf_s(KeyName, sizeof(KeyName),"LocalWarfield%dYLSealStone",i);
					GetPrivateProfileString("LocalWarfield",KeyName,NULL,tempString,MAX_PATH,MapServerConfigFileName);
					if (tempString==NULL)
					{
						return false;
					}

					MakeSealStoneNumber(tempString,tempLF.SealNo[2]);

					g_pLocalWarfield->SetLocalWarfieldInfo(tempLF);
				}
				return true;
			}
		}
		else
		{
			return false;
		}
	}
	return true;
}

void CloseLocalWarSystem()
{
	SAFE_DELETE(g_pLocalWarfield);
}

int CanLocalWarAttack(CHARLIST* pAttacker,CHARLIST* pDefencer)		// 020115 LTS 최성동 ,임상우공동 
{
	if (!g_LocalWarBegin) return FALSE;				// 국지전이 시작되지 않았다면 공격됨
	if (!g_isLocalWarServer) return FALSE;			// 국지전 서버가 아니면 공격됨 

	if (pAttacker->JoinLocalWar)
	{
		if (pDefencer->JoinLocalWar)	// 둘다 국지전에 참여했다면
		{
			if (pDefencer->name_status.nation==pAttacker->name_status.nation) return true;	// 같은나라이면 공격불가
			else return FALSE;				// 공격가능
		} else return TRUE;				// 방어자가 국지전에 참여 하지 않았다면 공격불가
	}
	else 
	{
		if (pDefencer->JoinLocalWar)	// 방어자가 국지전에 참여 했다면
		{
			return TRUE;				// 공격안됨
		} else return FALSE;			// 둘다 참여하지 않았다면 공격됨
	}
}

int CheckLocalWarAttacker(CHARLIST* pAttacker)						// 020115 LTS 최성동 공동
{
	if (!g_LocalWarBegin) return TRUE;
	if (!g_isLocalWarServer) return TRUE;
	if (pAttacker->JoinLocalWar) return FALSE;
	else return TRUE;
}

void IncGainedFame(CHARLIST* ch,int Value)
{
	if (ch->GainedFame>=9) return;
	MyLog(0,"Gained Fame 변경 , 이름 : %s, Old : %d, New : %d",ch->Name,ch->GainedFame,ch->GainedFame+Value);
	ch->GainedFame+=Value;
}

void SendPacket2LocalWarJoinClients(t_packet* p)
{	//< CSD-CN-031213
	CUserManager::HASH_USER user = g_pUserManager->GetUserSet();

	for (CUserManager::ITOR_USER i = user.begin(); i != user.end(); ++i)
	{
		const int idUser = i->second;
		CHARLIST* pTarget = CheckServerId(idUser);

		if (pTarget != NULL && pTarget->JoinLocalWar)
		{
			QueuePacket(connections, idUser, p, 1);
		}
	}
}	//> CSD-CN-031213

void CheckLocalWarDeath(CHARLIST* pCaster,CHARLIST* pTarget)	// 011217 LTS
{
	const int PointTBL[11] = { 1,1,1,2,2,2,2,2,3,3,4 };			// 점수계산		// 011219 LTS
	short tempLevel;
	short tempIndex;
	unsigned short NationPoint[3]= {0,0,0};					// 020130 LTS
	t_packet packet;

	if (!g_isLocalWarServer) return;						// 국지전서버가 아닌면 
	if (!g_pLocalWarfield->GetLocalWarStatus()) return;		// 전쟁중이 아니면 
	if (pCaster==NULL||pTarget==NULL) return;				// 011217 LTS

	if (!pCaster->IsPlayer()) return;						// 020130 LTS
	if (!pTarget->IsPlayer()) return;

	if (pCaster->name_status.nation==pTarget->name_status.nation) return;	//같은 국가이면 일반과 동일처리 

	if (pCaster->JoinLocalWar&&pTarget->JoinLocalWar)
	{
		//둘다 전쟁에 참여했다.
		tempLevel = pTarget->GetLevel() - pCaster->GetLevel(); // CSD-030806
		tempIndex=(int)tempLevel/10;
		if (tempLevel>0)	//방어자의 레벨이 높다 // 공격자의 점수획득
		{
			switch (pCaster->name_status.nation)	// 국가점수를 구한다.
			{
			case NW_BY : NationPoint[0]+=PointTBL[tempIndex]; break;
			case NW_ZY : NationPoint[1]+=PointTBL[tempIndex]; break;
			case NW_YL : NationPoint[2]+=PointTBL[tempIndex]; break;
			}
			if (tempIndex>0) 
			{
				IncGainedFame(pCaster,3);
				IncGainedFame(pTarget,-3);
			}
			else
			{
				if (tempIndex==0)
				{
					IncGainedFame(pCaster,2);
					IncGainedFame(pTarget,-2);
				}
			}
			
		}
		else				//공격자의 레벨이 높다	// 방어자의 점수 
		{
			if (tempIndex<-1) 
			{
				IncGainedFame(pCaster,0);
				IncGainedFame(pTarget,0);
			}
			else
			{
				if (tempIndex==0)
				{
					IncGainedFame(pCaster,1);
					IncGainedFame(pTarget,-1);
				}
			}
		}
	}
/*	else							// 020130 LTS
	{
		if (pCaster->JoinLocalWar)
		{
			//공격자가 전쟁참여
			IncGainedFame(pCaster,-2);	//전쟁모드가 아닌사람을 죽이면 -2명성치감소
			switch (pCaster->name_status.nation)	// 국가점수를 구한다.
			{
			case NW_BY : NationPoint[0]-=5; break;
			case NW_ZY : NationPoint[1]-=5; break;
			case NW_YL : NationPoint[2]-=5; break;
			}
		}
		else
		{
			//방어자가 전쟁참여
			switch (pCaster->name_status.nation)	// 국가점수를 구한다.
			{
			case NW_BY : NationPoint[0]-=5; break;
			case NW_ZY : NationPoint[1]-=5; break;
			case NW_YL : NationPoint[2]-=5; break;
			}
		}
	}*/

	//MyLog(0,"Send Nation Point 1 : %d, 2 : %d, 3 : %d",NationPoint[0],NationPoint[1],Nationpoint[2]);
	packet.h.header.type=CMD_LOCALWAR_NATION_POINT_CHANGED;
	packet.u.LocalWar.NationPointChanged.NationPoint[0]=NationPoint[0];
	packet.u.LocalWar.NationPointChanged.NationPoint[1]=NationPoint[1];
	packet.u.LocalWar.NationPointChanged.NationPoint[2]=NationPoint[2];
	packet.h.header.size=sizeof(t_NationPointChanged);

	SendPacket2NWManager(&packet);
}

cLocalWarfield::cLocalWarfield()
{
	m_tLocalWarfieldInfo.Index=0;
	m_tLocalWarfieldInfo.Port=0;
	m_tLocalWarfieldInfo.Phase=0;
	ClearManCount();
	ClearSealStonePtr();							// LTS NEW LOCALWAR
//	ClearPoint();
}

void cLocalWarfield::ClearSealStonePtr()			// LTS NEW LOCALWAR
{
	for (int i=0;i<NW_NATION_COUNT;i++)
	{
		for (int j=0;j<LOCALWAR_SEALSTONE_MAX;j++)
		{
			m_SealStonePtr[i][j]=NULL;
		}
	}
}

int	cLocalWarfield::ConvertNation2Index(int Nation) 
{
	switch (Nation) 
	{
	case NW_BY :	return 0; 
	case NW_ZY :	return 1;
	case NW_YL :	return 2;
	}
	return 0;
}

void cLocalWarfield::ClearManCount()
{
	m_ManCount[0]=0;					// LOOP UNROLL
	m_ManCount[1]=0;
	m_ManCount[2]=0;
}

/*void cLocalWarfield::ClearPoint()
{
	m_LocalWarPoint[0]=0;				// LOOP UNROLL
	m_LocalWarPoint[1]=0;
	m_LocalWarPoint[2]=0;
}*/

void cLocalWarfield::SetLocalWarfieldInfo(LocalWarfield_Info LF)		// LTS NEW LOCALWAR
{
	m_tLocalWarfieldInfo.Index=LF.Index;
	m_tLocalWarfieldInfo.Phase=LF.Phase;
	m_tLocalWarfieldInfo.Port=LF.Port;
	strcpy_s(m_tLocalWarfieldInfo.MapName, sizeof(m_tLocalWarfieldInfo.MapName),LF.MapName);
	for (int i=0;i<NW_NATION_COUNT;i++)
	{
		for (int j=0;j<LOCALWAR_SEALSTONE_MAX;j++)
		{
			m_SealStoneNo[i][j]=LF.SealNo[i][j];
		}
	}
}

int cLocalWarfield::CheckAllSealStonebroked(int NationIndex)				// LTS NEW LOCALWAR
{
	for (int i=0;i<LOCALWAR_SEALSTONE_MAX;i++)								// NPCList[]에서 지워진다면... 안된다.
	{																		// 국지전의 결계석은 Remove되지 않는다. 확인해보자	
		if (m_SealStonePtr[NationIndex][i])
		{
			if (!m_SealStonePtr[NationIndex][i]->IsDead())
			{
				return FALSE;
			}
		}
	}

/*	t_packet packet;
	packet.h.header.type=CMD_SEALSTONE_ALL_BROKED;
	packet.u.LocalWar.CommonDataC.Data=TRUE;
	packet.h.header.size=sizeof(t_CommonDataC);
	SendPacket2Map(&packet,NATION_MANAGE_SERVER);*/


	return TRUE;
}

extern int GetNationIndex(int Nation);						// LTS NEW LOCALWAR


void cLocalWarfield::CheckSealStonePtr(CHARLIST* ch)			// LTS NEW LOCALWAR BUG
{
	int NationIndex;

	switch(ch->SprNo)
	{
	case 98 : NationIndex=0; break;						// BY
	case 99 : NationIndex=1; break;						// ZY
	case 91 : NationIndex=2; break;						// YL
	}
		
	for (int i=0;i<LOCALWAR_SEALSTONE_MAX;i++)
	{
		if (ch->eventno==m_SealStoneNo[NationIndex][i])				// 0은 자동으로 NULL이된다.
		{
			if (m_SealStonePtr[NationIndex][i]==NULL)
			{
				m_SealStonePtr[NationIndex][i]=ch;
				MyLog(0,"SEALSTONE WAS POINTED!! EventNo : %d, NationIndex:%d, Locate:%d",ch->eventno,NationIndex,i);
				return;
			}
		}
		
	}
}

CHARLIST* cLocalWarfield::GetSealStonePtr( const int nNation, const int nStoneNum )
{// 030506 kyo
	if( 0 <= nNation && nNation <= NW_NATION_COUNT && 0<= nStoneNum && nStoneNum <= LOCALWAR_SEALSTONE_MAX )
	{
		return m_SealStonePtr[nNation][nStoneNum];
	}
	else
		return NULL;
}


void cLocalWarfield::RecoveryAllSealStone()
{// 030520 kyo //모든 결계석의 HP를 HPMAX로 채운다. 
	CHARLIST *ch;
	for (int i=0;i<NW_NATION_COUNT;i++)
	{
		for (int j=0;j<LOCALWAR_SEALSTONE_MAX;j++)
		{
			ch = GetSealStonePtr( i, j );
			if( !ch) 
			{
				continue;
			}
			ch->Hp = ch->HpMax;
		}
	}
}

void GiveSealStonePoint(CHARLIST* ach,CHARLIST* dch)
{
	switch (dch->SprNo)
	{
	case 91 : 
		MyLog(0,"Gained Fame 변경 , 이름 : %s, Old : %d, New : %d",ach->Name,ach->GainedFame,ach->GainedFame+3);
		if (ach->name_status.nation!=NW_YL) ach->GainedFame+=3; 
		break;
	case 98 : 
		MyLog(0,"Gained Fame 변경 , 이름 : %s, Old : %d, New : %d",ach->Name,ach->GainedFame,ach->GainedFame+3);
		if (ach->name_status.nation!=NW_BY) ach->GainedFame+=3; 
		break;
	case 99 : 
		MyLog(0,"Gained Fame 변경 , 이름 : %s, Old : %d, New : %d",ach->Name,ach->GainedFame,ach->GainedFame+3);
		if (ach->name_status.nation!=NW_ZY) ach->GainedFame+=3; 
		break;
	}
}

void SendCMD_LOCALWAR_STARTUP_STATUS(CHARLIST* ch)	//맨처음 로긴시 보낸다.
{
	t_packet packet;

	packet.h.header.type=CMD_LOCALWAR_STARTUP_STATUS;
	packet.u.LocalWar.CommonDataC.Data=g_LocalWarBegin;
	packet.h.header.size=sizeof(t_CommonDataC);

	QueuePacket(connections,ch->GetServerID(),&packet,1);
}

void SendCMD_ANSWER_LOCAL_DELIVERY(int UserID,int MapServerPort,t_packet* p)
{
	t_packet DeliveryPacket;

	DeliveryPacket.h.header.type=CMD_ANSWER_LOCAL_DELIVERY;
	DeliveryPacket.u.LocalWar.Delivery_A.UserID=UserID;
	DeliveryPacket.u.LocalWar.Delivery_A.Size=sizeof(t_header)+p->h.header.size;
	//memcpy(DeliveryPacket.u.LocalWar.Delivery_A.Data,(char*)p,DeliveryPacket.u.LocalWar.Delivery_A.Size);
	DeliveryPacket.u.LocalWar.Delivery_A.nType = p->h.header.type; // 030518 kyo	
	memcpy(DeliveryPacket.u.LocalWar.Delivery_A.Data,(char*)(p->u.data),DeliveryPacket.u.LocalWar.Delivery_A.Size); // 030518 kyo데이타만 복사
	DeliveryPacket.h.header.size=sizeof(t_Delivery_A)-100+DeliveryPacket.u.LocalWar.Delivery_A.Size;
	g_pServerTable->SendRajaPacketToOtherMapServer(MapServerPort, (char *)&DeliveryPacket, DeliveryPacket.h.header.size+sizeof(t_header) );
}

void SendCMD_REQUEST_LOCAL_DELIVERY(t_packet *p, t_connection c[], int cn)
{
	t_packet packet;

	packet.h.header.type=CMD_REQUEST_LOCAL_DELIVERY;
	packet.u.LocalWar.Delivery_R.Port=g_wMapServerPort;
	packet.u.LocalWar.Delivery_R.UserID=cn;
	packet.u.LocalWar.Delivery_R.Size=sizeof(t_header)+p->h.header.size;
	memcpy(packet.u.LocalWar.Delivery_R.Data,(char*)p,packet.u.LocalWar.Delivery_R.Size);
	packet.h.header.size=sizeof(t_Delivery_R)-MAX_STRING_PK+packet.u.LocalWar.Delivery_R.Size;  //100은 버퍼의 수이다. MAX_STRING_PK (NetWork4.h)//020903 lsw
	g_pServerTable->SendRajaPacketToOtherMapServer( NATION_MANAGE_SERVER, (char *)&packet, packet.h.header.size+sizeof(t_header) );
}

void SendCMD_REQUEST_LOCAL_DELIVERY(WORD Port,t_packet *p, t_connection c[], int cn)
{
	t_packet packet;

	packet.h.header.type=CMD_REQUEST_LOCAL_DELIVERY;
	packet.u.LocalWar.Delivery_R.Port=g_wMapServerPort;
	packet.u.LocalWar.Delivery_R.UserID=cn;
	packet.u.LocalWar.Delivery_R.Size=sizeof(t_header)+p->h.header.size;
	memcpy(packet.u.LocalWar.Delivery_R.Data,(char*)p,packet.u.LocalWar.Delivery_R.Size);
	packet.h.header.size=sizeof(t_Delivery_R)-MAX_STRING_PK+packet.u.LocalWar.Delivery_R.Size;  //100은 버퍼의 수이다. MAX_STRING_PK (NetWork4.h)//020903 lsw
	g_pServerTable->SendRajaPacketToOtherMapServer( Port, (char *)&packet, packet.h.header.size+sizeof(t_header) );
}


void ProcessCMD_LOCALWAR_STATUS(t_packet *ReturnPacket,t_packet* ReceivePacket)
{
	ReturnPacket->h.header.type=CMD_LOCALWAR_STATUS;
	ReturnPacket->u.LocalWar.CommonDataC.Data=g_pNation->GetLocalWarStatus();
	ReturnPacket->h.header.size=sizeof(t_CommonDataC);
}

void ProcessCMD_REQUEST_LOCALWAR_INFO(t_packet *ReturnPacket,t_packet* ReceivePacket)		// LTS NEW LOCALWAR
{
	const char EventNo[3][9]={	{ 1, 2, 6, 7, 8,24,25,26,27},   // NW_BY 
								{ 8, 9,18,19,20,24,25,26,27},	// NW_ZY
								{ 9,10,13,14,15,25,26,27,28} };	// NW_YL
	ReturnPacket->h.header.type=CMD_ANSWER_LOCALWAR_INFO;
	ReturnPacket->h.header.size=sizeof(t_LocalWarInfo);

	for (int i=0;i<7;i++)
	{
		ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarInfo1[i].Index=i;
		ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarInfo1[i].ManCount[0]=g_pNation->GetLocalWarfieldManCount(i,0);
		ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarInfo1[i].ManCount[1]=g_pNation->GetLocalWarfieldManCount(i,1);
		ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarInfo1[i].ManCount[2]=g_pNation->GetLocalWarfieldManCount(i,2);
	}


	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarPointChanged.RemainTime=g_pNation->GetLocalWarRemainTime()-g_curr_time;
	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarPointChanged.Point[0]=g_pNation->GetLocalWarPoint(0);
	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarPointChanged.Point[1]=g_pNation->GetLocalWarPoint(1);
	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarPointChanged.Point[2]=g_pNation->GetLocalWarPoint(2);


	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[0][0]=SealStone[NW_BY].status[EventNo[0][0]];		// LOOP UNROLL
	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[0][1]=SealStone[NW_BY].status[EventNo[0][1]];
	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[0][2]=SealStone[NW_BY].status[EventNo[0][2]];
	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[0][3]=SealStone[NW_BY].status[EventNo[0][3]];
	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[0][4]=SealStone[NW_BY].status[EventNo[0][4]];
	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[0][5]=SealStone[NW_BY].status[EventNo[0][5]];
	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[0][6]=SealStone[NW_BY].status[EventNo[0][6]];
	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[0][7]=SealStone[NW_BY].status[EventNo[0][7]];
	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[0][8]=SealStone[NW_BY].status[EventNo[0][8]];

	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[1][0]=SealStone[NW_ZY].status[EventNo[1][0]];		// LOOP UNROLL
	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[1][1]=SealStone[NW_ZY].status[EventNo[1][1]];
	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[1][2]=SealStone[NW_ZY].status[EventNo[1][2]];
	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[1][3]=SealStone[NW_ZY].status[EventNo[1][3]];
	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[1][4]=SealStone[NW_ZY].status[EventNo[1][4]];
	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[1][5]=SealStone[NW_ZY].status[EventNo[1][5]];
	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[1][6]=SealStone[NW_ZY].status[EventNo[1][6]];
	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[1][7]=SealStone[NW_ZY].status[EventNo[1][7]];
	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[1][8]=SealStone[NW_ZY].status[EventNo[1][8]];

	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[2][0]=SealStone[NW_YL].status[EventNo[2][0]];		// LOOP UNROLL
	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[2][1]=SealStone[NW_YL].status[EventNo[2][1]];
	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[2][2]=SealStone[NW_YL].status[EventNo[2][2]];
	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[2][3]=SealStone[NW_YL].status[EventNo[2][3]];
	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[2][4]=SealStone[NW_YL].status[EventNo[2][4]];
	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[2][5]=SealStone[NW_YL].status[EventNo[2][5]];
	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[2][6]=SealStone[NW_YL].status[EventNo[2][6]];
	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[2][7]=SealStone[NW_YL].status[EventNo[2][7]];
	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[2][8]=SealStone[NW_YL].status[EventNo[2][8]];

	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarDefencePoint[0]=g_pNation->GetLocalWarDefencePoint(0);		// LTS NEW LOCALWAR
	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarDefencePoint[1]=g_pNation->GetLocalWarDefencePoint(1);		// LTS NEW LOCALWAR
	ReturnPacket->u.LocalWar.LocalWarInfo.LocalWarDefencePoint[2]=g_pNation->GetLocalWarDefencePoint(2);		// LTS NEW LOCALWAR
}


void ProcessCMD_EVENT_LOCALWAR_JOIN(t_packet *ReturnPacket,t_packet* ReceivePacket)
{
	int Level=ReceivePacket->u.LocalWar.EventJoin.Level;
	int Nation=ReceivePacket->u.LocalWar.EventJoin.Nation;
	if (!g_isEventLocalWarServer) return;

	if (!g_pEventLocalWarfield->CheckLevel(Level))						// 레벨의 범위가 틀리다면
	{
		ReturnPacket->h.header.type=CMD_EVENT_LOCALWAR_JOIN_RESULT;
		ReturnPacket->u.LocalWar.CommonDataC.Data=2;					// 0 : 실패 1 : 성공 2 : 레벨실패 3 : 인원실패
		ReturnPacket->h.header.size=sizeof(t_CommonDataC);
		return;
	}

	if (!g_pEventLocalWarfield->IncManCount(Nation))					// 인원제한에 걸렸다면 
	{
		ReturnPacket->h.header.type=CMD_EVENT_LOCALWAR_JOIN_RESULT;
		ReturnPacket->u.LocalWar.CommonDataC.Data=3;					// 0 : 실패 1 : 성공 2 : 레벨실패 3 : 인원실패
		ReturnPacket->h.header.size=sizeof(t_CommonDataC);
		return;
	}

	ReturnPacket->h.header.type=CMD_EVENT_LOCALWAR_JOIN_RESULT;			// 이상없다면 	
	ReturnPacket->u.LocalWar.CommonDataC.Data=1;						// 0 : 실패 1 : 성공 2 : 레벨실패 3 : 인원실패
	ReturnPacket->h.header.size=sizeof(t_CommonDataC);
}

void RecvCMD_REQUEST_LOCAL_DELIVERY(t_packet *p, t_connection c[], int cn)
{
	t_packet	tempPacket;
	t_packet	SendPacket;

	WORD	MapServerPort;
	DWORD	UserID;

	MapServerPort=p->u.LocalWar.Delivery_R.Port;
	UserID=p->u.LocalWar.Delivery_R.UserID;

	memcpy((char*)&tempPacket,p->u.LocalWar.Delivery_R.Data,p->u.LocalWar.Delivery_R.Size);

	switch (tempPacket.h.header.type)
	{
	case CMD_LOCALWAR_STATUS :  
			ProcessCMD_LOCALWAR_STATUS(&SendPacket,&tempPacket);
			SendCMD_ANSWER_LOCAL_DELIVERY(UserID,MapServerPort,&SendPacket);
			break;
	case CMD_REQUEST_LOCALWAR_INFO :
			ProcessCMD_REQUEST_LOCALWAR_INFO(&SendPacket,&tempPacket);
			SendCMD_ANSWER_LOCAL_DELIVERY(UserID,MapServerPort,&SendPacket);
			break;
	case CMD_EVENT_LOCALWAR_JOIN :
			ProcessCMD_EVENT_LOCALWAR_JOIN(&SendPacket,&tempPacket);
			SendCMD_ANSWER_LOCAL_DELIVERY(UserID,MapServerPort,&SendPacket);
			break;
	}
}

void SetEventLocalWarJoinResult(int UserID,int Result)
{
	switch(Result)
	{
	case 0 :
	case 2 :
	case 3 : connections[UserID].chrlst.EventLocalWarJoin=0;	break;
	case 1 : connections[UserID].chrlst.EventLocalWarJoin=1;	break;
	}

}

void RecvCMD_ANSWER_LOCAL_DELIVERY(t_packet *p, t_connection c[], int cn)
{
	DWORD		UserID;
	t_packet	tempPacket;

	UserID=p->u.LocalWar.Delivery_A.UserID;
	tempPacket.h.header.size=p->u.LocalWar.Delivery_A.Size;// 030518 kyo
	tempPacket.h.header.type=p->u.LocalWar.Delivery_A.nType; // 030518 kyo
	//memcpy((char*)&tempPacket,p->u.LocalWar.Delivery_A.Data,p->u.LocalWar.Delivery_A.Size);
	memcpy((char*)&(tempPacket.u),(p->u.LocalWar.Delivery_A.Data),p->u.LocalWar.Delivery_A.Size); // 030518 kyo
	switch (tempPacket.h.header.type)
	{
	case CMD_EVENT_LOCALWAR_JOIN_RESULT : SetEventLocalWarJoinResult(UserID,tempPacket.u.LocalWar.CommonDataC.Data); break;
	}
	QueuePacket(c,UserID,&tempPacket,1);
}

void RecvCMD_LOCALWAR_STATUS(t_packet *p, t_connection c[], int cn )
{
	t_packet packet;

	if (isNationManageServer())
	{
		ProcessCMD_LOCALWAR_STATUS(&packet,p);
		QueuePacket(c,cn,&packet,1);
	}
	else SendCMD_REQUEST_LOCAL_DELIVERY(p,c,cn);
}

void RecvCMD_REQUEST_LOCALWAR_INFO(t_packet *p, t_connection c[], int cn )
{
	t_packet packet;
	if (isNationManageServer())
	{
		ProcessCMD_REQUEST_LOCALWAR_INFO(&packet,p);
		QueuePacket(c,cn,&packet,1);
	}
	else SendCMD_REQUEST_LOCAL_DELIVERY(p,c,cn);
}

void IncLocalWarfieldMemberCount(int cn)
{
	if (g_isLocalWarServer)
	{
		if (connections[cn].chrlst.JoinLocalWar)
		{
			t_packet packet;
			g_pLocalWarfield->IncManCount(g_pLocalWarfield->ConvertNation2Index(connections[cn].chrlst.name_status.nation));
			packet.h.header.type=CMD_INC_USER_LOCALWARFIELD;
			packet.u.LocalWar.LocalWarUserAdd.Index=g_pLocalWarfield->GetLocalWarfieldIndex();
			packet.u.LocalWar.LocalWarUserAdd.Nation=connections[cn].chrlst.name_status.nation;
			packet.h.header.size=sizeof(t_LocalWarUserAdd);
			SendPacket2NWManager(&packet);
		}
	}
}

void DecLocalWarfieldMemberCount(int cn)
{
	t_packet packet;

	if (g_isLocalWarServer)
	{
		if (connections[cn].chrlst.JoinLocalWar)
		{
			g_pLocalWarfield->DecManCount(g_pLocalWarfield->ConvertNation2Index(connections[cn].chrlst.name_status.nation));
			packet.h.header.type=CMD_DEC_USER_LOCALWARFIELD;
			packet.u.LocalWar.LocalWarUserAdd.Index=g_pLocalWarfield->GetLocalWarfieldIndex();
			packet.u.LocalWar.LocalWarUserAdd.Nation=connections[cn].chrlst.name_status.nation;
			packet.h.header.size=sizeof(t_LocalWarUserAdd);
			SendPacket2NWManager(&packet);
		}
	}

}

void RecvCMD_LOCALWAR_JOIN(t_packet *p, t_connection c[], int cn )
{
	LPCHARLIST ch = ::CheckServerId(cn);
	if(!ch){return;}

	t_packet packet, packet2;
	if (!ch->JoinLocalWar)
	{
		if (ch->name_status.nation!=MapInfo[MapNumber].nation)
		{	// 국적이 다른 맵에서는 참여할수 없다.
			packet.h.header.type=CMD_LOCALWAR_JOIN_RESULT;
			packet.u.LocalWar.CommonDataC.Data=FALSE;
			packet.h.header.size=sizeof(t_CommonDataC);
			QueuePacket(c,cn,&packet,1);
			return;
		}
		ch->JoinLocalWar=TRUE;
		packet.h.header.type=CMD_LOCALWAR_JOIN_RESULT;
		packet.u.LocalWar.CommonDataC.Data=TRUE;
		packet.h.header.size=sizeof(t_CommonDataC);
		QueuePacket(c,cn,&packet,1);
		g_pLogManager->SaveLocalWarInfo(ch); // CSD-040407

		packet2.h.header.type=CMD_USER_JOIN_LOCALWAR;			//유저가 국지전에 조인 했다고 알려준다.
		packet2.u.LocalWar.UserJoinLocalWar.ServerID=cn;
		packet2.h.header.size=sizeof(t_UserJoinLocalWar);
		CastMe2Other( cn, &packet2);

		IncLocalWarfieldMemberCount(cn);

	}
}


void RecvCMD_REQUEST_COMPARE_PHASE(t_packet *p, t_connection c[], int cn )
{
	int NewPhase;

	if (!connections[cn].chrlst.JoinLocalWar) return;

	if (g_isLocalWarServer)
	{
		if (connections[cn].chrlst.name_status.nation!=MapInfo[MapNumber].nation)
		{
			NewPhase=g_pLocalWarfield->GetLocalWarfieldPhase();
			switch (connections[cn].chrlst.CurrentPhase)
			{
			case 0 : 
				switch(NewPhase)
				{
				case 1 : connections[cn].chrlst.CurrentPhase=1; break;
				case 2 : connections[cn].chrlst.CurrentPhase=2; break;
				}
				break;
			case 1 : 
				switch(NewPhase)
				{
				case 2 : connections[cn].chrlst.CurrentPhase=2; break;
				}
				break;
			default : connections[cn].chrlst.CurrentPhase=NewPhase; 
			}
		}

		IncLocalWarfieldMemberCount(cn);
	}
	else
	{
		if (connections[cn].chrlst.JoinLocalWar) connections[cn].chrlst.CurrentPhase=0;
	}
}

void RecvCMD_INC_USER_LOCALWARFIELD(t_packet *p, t_connection c[], int cn )
{
	if (!isNationManageServer()) return; // finito 190707 prevents map from crashing if this packet accidently goes to wrong server
	int Index=p->u.LocalWar.LocalWarUserAdd.Index;
	int NationIndex=g_pNation->ConvertNation2Index(p->u.LocalWar.LocalWarUserAdd.Nation);
	int Nation=p->u.LocalWar.LocalWarUserAdd.Nation;

	g_pNation->IncLocalWarfieldManCount(Index,NationIndex);
	MyLog(0,"참여인원 변동 __ 번호 : %d, 국적 : %d , 인원 : %d",Index,Nation,g_pNation->GetLocalWarfieldManCount(Index,NationIndex));
}

void RecvCMD_DEC_USER_LOCALWARFIELD(t_packet *p, t_connection c[], int cn )
{
	if (!isNationManageServer()) return; // finito 190707 prevents map from crashing if this packet accidently goes to wrong server
	int Index=p->u.LocalWar.LocalWarUserAdd.Index;
	int NationIndex=g_pNation->ConvertNation2Index(p->u.LocalWar.LocalWarUserAdd.Nation);
	int Nation=p->u.LocalWar.LocalWarUserAdd.Nation;

	g_pNation->DecLocalWarfieldManCount(Index,NationIndex);
	MyLog(0,"참여인원 변동 __ 번호 : %d, 국적 : %d , 인원 : %d",Index,Nation,g_pNation->GetLocalWarfieldManCount(Index,NationIndex));
}

void SendCMD_LOCALWAR_BEGIN2Client()
{	//< CSD-CN-031213
	t_packet p;
	p.h.header.type = CMD_LOCALWAR_BEGIN;
	p.h.header.size = 0;
	g_pUserManager->SendPacket(&p);
}	//> CSD-CN-031213

// finito 060707 informs clients of the remaining hours until localwar starts
void SendCMD_LOCALWAR_BEGIN_TIME2Client(t_packet *p)
{
	g_pUserManager->SendPacket(p);
}

void SendCMD_LOCALWAR_FAME_EXP(CHARLIST* ch,int Fame,int Exp)
{
	t_packet packet;

	packet.h.header.type=CMD_LOCALWAR_FAME_EXP;
	packet.u.LocalWar.FameExpChange.GainedFame=Fame;
	packet.u.LocalWar.FameExpChange.GainedExp=Exp;
	packet.h.header.size=sizeof(t_FameExpChange);

	QueuePacket(connections,ch->GetServerID(),&packet,1);
}

void CalcGainedFame(CHARLIST* ch)// LTS DUAL FAME
{
	short OldFame = ch->fame;
	if (ch->GainedFame>=9) 
	{//듀얼페임이 9이상이면 페임은 12, 듀얼페임은 8증가
		SaveChangeFame(ch,ch->fame,ch->fame+12,LF_LOCALWAR);// 020909 LTS
		ch->fame+=12;// 스파이 게임을 위한 값을 증가시킨다.
		MyLog(0,"Fame Changed , Name:[%s], Nation:[%d], OldFame:[%d], NewFame:[%d], Differ:[%d]",ch->Name,ch->name_status.nation,OldFame,ch->fame,ch->fame-OldFame);	
		const int iOldDF = var[ch->GetServerID()][DUAL_FAME_FIELD];
		const int iNewDF = iOldDF+8;
		var[ch->GetServerID()][DUAL_FAME_FIELD] = iNewDF;
		
		::SaveChangeDualFame( ch, iOldDF, iNewDF, LDF_LOCALWAR);
		::MyLog(0,"Increase User Spy Game Point, User : %s, OldPoint %d, NewPoint %d",ch->Name,iOldDF,iNewDF);
	}
	else 
	{
		if (ch->GainedFame>0)
		{
			SaveChangeFame(ch,ch->fame,ch->fame+ch->GainedFame,LF_LOCALWAR);		// 020909 LTS
			ch->fame+=ch->GainedFame;		
			MyLog(0,"Fame Changed , Name:[%s], Nation:[%d], OldFame:[%d], NewFame:[%d], Differ:[%d]",ch->Name,ch->name_status.nation,OldFame,ch->fame,ch->fame-OldFame);		
		}
		
		int tempValue=2+ch->GainedFame;
		if (tempValue>=8)
		{
			const int iOldDF = var[ch->GetServerID()][DUAL_FAME_FIELD];
			const int iNewDF = iOldDF+8;
			var[ch->GetServerID()][DUAL_FAME_FIELD] = iNewDF;
			
			::SaveChangeDualFame( ch, iOldDF, iNewDF, LDF_LOCALWAR);
			::MyLog(0,"Increase User Spy Game Point, User : %s, OldPoint %d, NewPoint %d",ch->Name,iOldDF,iNewDF);
		}
		else
		{
			if (tempValue>1)	// 합이 양인 사람에게 
			{
				const int iOldDF = var[ch->GetServerID()][DUAL_FAME_FIELD];
				const int iNewDF = iOldDF+tempValue;
				var[ch->GetServerID()][DUAL_FAME_FIELD] = iNewDF;
				
				::SaveChangeDualFame( ch, iOldDF, iNewDF, LDF_LOCALWAR);
				::MyLog(0,"Increase User Spy Game Point, User : %s, OldPoint %d, NewPoint %d",ch->Name,iOldDF,iNewDF);
			}
			else				// 합이 음인 사람에게
			{
				const int iOldDF = var[ch->GetServerID()][DUAL_FAME_FIELD];
				const int iNewDF = iOldDF+2;
				var[ch->GetServerID()][DUAL_FAME_FIELD] = iNewDF;
				
				::SaveChangeDualFame( ch, iOldDF, iNewDF, LDF_LOCALWAR);
				::MyLog(0,"Increase User Spy Game Point, User : %s, OldPoint %d, NewPoint %d",ch->Name,iOldDF,iNewDF);
			}
		}
	}

	int tempFame=ch->fame-OldFame;
	
	if (tempFame>=5&&tempFame<=9) 
	{
		int tempExp_Grade = ch->GetLevel() - 4; // CSD-030806

		if (tempExp_Grade>110) {tempExp_Grade=110;}

		if (ch->ChangeUpAddExpBonus(g_pNationWar_Exp[tempExp_Grade]))
		{	//< CSD-TW-030624
			::isLevelUp(ch);
			SendCMD_LOCALWAR_FAME_EXP(ch,tempFame,g_pNationWar_Exp[tempExp_Grade]);
		}	//> CSD-TW-030624
		else
		{
		  SendCMD_LOCALWAR_FAME_EXP(ch,tempFame,0);
		}
	}
	else if (tempFame>=10&&tempFame<=11)
	{
		int tempExp_Grade = ch->GetLevel() - 2; // CSD-030806
		if (tempExp_Grade>110){ tempExp_Grade=110;}
				
		if (ch->ChangeUpAddExpBonus(g_pNationWar_Exp[tempExp_Grade]))
		{	//< CSD-TW-030624
			::isLevelUp(ch);
			SendCMD_LOCALWAR_FAME_EXP(ch,tempFame,g_pNationWar_Exp[tempExp_Grade]);
		}	//> CSD-TW-030624
		else
		{
		  SendCMD_LOCALWAR_FAME_EXP(ch,tempFame,0);
		}
	}
	else if (tempFame==12)
	{
		int tempExp_Grade = ch->GetLevel(); // CSD-030806

		if (tempExp_Grade>110){tempExp_Grade=110;}
		
		if (ch->ChangeUpAddExpBonus(g_pNationWar_Exp[tempExp_Grade]))
		{	//< CSD-TW-030624
			::isLevelUp(ch);
			SendCMD_LOCALWAR_FAME_EXP(ch,tempFame,g_pNationWar_Exp[tempExp_Grade]);
		}	//> CSD-TW-030624
		else
		{
		  SendCMD_LOCALWAR_FAME_EXP(ch,tempFame,0);
		}
	}
	else 
	{
		SendCMD_LOCALWAR_FAME_EXP(ch,tempFame,0);
	}
}

//020828 lsw
void CalcMapUsersGainedFame(int Nation)
{	//< CSD-HK-030829
	for (int i = DRAGON_CONNECTIONS_START; i < DRAGON_MAX_CONNECTIONS; ++i)
	{
		CHARLIST* pUser = CheckServerId(i);
		
		if (pUser != NULL)
		{
			if (pUser->JoinLocalWar && pUser->name_status.nation == Nation) 
			{
				CalcGainedFame(pUser);
			}
			else					// 스파이 게임을 위한 값을 업데이트 한다.
			{
				if (pUser->JoinLocalWar)
				{
					int tempValue = pUser->GainedFame + 2;
					
					if (tempValue >= 8)
					{
						const int iOldDF = var[i][DUAL_FAME_FIELD];
						const int iNewDF = iOldDF + 8;
						var[pUser->GetServerID()][DUAL_FAME_FIELD] = iNewDF;
						
						::SaveChangeDualFame(pUser, iOldDF, iNewDF, LDF_LOCALWAR);
						::MyLog(0,"Increase User Spy Game Point, User : %s, OldPoint %d, NewPoint %d", pUser->Name, iOldDF, iNewDF);
					}
					else
					{
						if (tempValue>1)		// 0보다 큰사람에게.... 얻은 값을 
						{
							const int iOldDF = var[i][DUAL_FAME_FIELD];
							const int iNewDF = iOldDF + tempValue;
							var[i][DUAL_FAME_FIELD] = iNewDF;
							
							::SaveChangeDualFame(pUser, iOldDF, iNewDF, LDF_LOCALWAR);
							::MyLog(0,"Increase User Spy Game Point, User : %s, OldPoint %d, NewPoint %d", pUser->Name, iOldDF, iNewDF);
						}
						else					// 0보다 작은 사람은.. 무조건2
						{
							const int iOldDF = var[i][DUAL_FAME_FIELD];
							const int iNewDF = iOldDF + 2;
							var[i][DUAL_FAME_FIELD] = iNewDF;
							
							::SaveChangeDualFame(pUser, iOldDF, iNewDF, LDF_LOCALWAR);
							::MyLog(0,"Increase User Spy Game Point, User : %s, OldPoint %d, NewPoint %d", pUser->Name, iOldDF, iNewDF);
						}
					}
				}
			}
		}
	}
}	//> CSD-HK-030829

void SendCMD_LOCALWAR_END2Client(t_packet* p)
{	//< CSD-CN-031213 : 현재 맵의 국지전에 참가한 모든 유저의 참가를 취소한다. 
	g_pUserManager->SendPacket(p);

	CUserManager::HASH_USER user = g_pUserManager->GetUserSet();

	for (CUserManager::ITOR_USER i = user.begin(); i != user.end(); ++i)
	{
		CHARLIST* pUser = CheckServerId(i->second);
		
		if (pUser != NULL && pUser->JoinLocalWar)
		{
			CheckAndSendChangeEquip(pUser, 1);
			//pUser->JoinLocalWar = 0; // 030520 kyo 
		}
	}
}	//> CSD-CN-031213

void RecvCMD_LOCALWAR_BEGIN( t_packet *p, t_connection c[], int cn )
{
	SendCMD_LOCALWAR_BEGIN2Client();

	g_LocalWarBegin=1;
//	MyLog(0,"Local War Status : %d",g_LocalWarBegin);

	if (g_isLocalWarServer)
	{
		g_pLocalWarfield->SetLocalWarStatus(1);
		g_pLocalWarfield->RecoveryAllSealStone(); // 030520 kyo //모든 결계석의 HP를 회복
	}
}

// finito 060707 localwar begin time announcement 
void RecvCMD_LOCALWAR_BEGIN_TIME( t_packet *p, t_connection c[], int cn )
{
	SendCMD_LOCALWAR_BEGIN_TIME2Client(p);
}

void GiveLife2SealStone()
{
	for (int i=0;i<MAX_NATION;i++)
		for (int j=0;j<MAX_SEALSTONE_NATION_;j++)
		{
			SealStone[i].status[j]=1;	// Alive;
		}
}

void SendCMD_SEALSTONE_BROKED(SealStoneStatus* tp)
{
	t_packet packet;

	packet.h.header.type=CMD_SEALSTONE_BROKED;
	switch (tp->SprNo)
	{
	case 98 : packet.u.LocalWar.SealStoneBroked.Nation=NW_BY; break;
	case 99 : packet.u.LocalWar.SealStoneBroked.Nation=NW_ZY; break;
	case 91 : packet.u.LocalWar.SealStoneBroked.Nation=NW_YL; break;
	}
	packet.u.LocalWar.SealStoneBroked.MapNumber=tp->MapNumber;
	packet.u.LocalWar.SealStoneBroked.LocateX=tp->X;
	packet.u.LocalWar.SealStoneBroked.LocateY=tp->Y;
	packet.h.header.size=sizeof(t_SealStoneBroked);

	g_pUserManager->SendPacket(&packet); // CSD-CN-031213
	SendPacket2Maps(&packet);
}

void RecvCMD_SEALSTONE_BROKED( t_packet *p, t_connection c[], int cn )
{
	if (MapInfo[MapNumber].nation==p->u.LocalWar.SealStoneBroked.Nation) 
	{	//< CSD-CN-031213
		g_pUserManager->SendPacket(p);
		return;
	}	//> CSD-CN-031213

	if (MapInfo[MapNumber].nation==0)
	{
		SendPacket2NationClients(p->u.LocalWar.SealStoneBroked.Nation,p);
	}
}

void SendCMD_SEALSTONE_ATTACKED(SealStoneStatus* tp)
{
	t_packet packet;

	packet.h.header.type=CMD_SEALSTONE_ATTACKED;
	switch (tp->SprNo)
	{
	case 98 : packet.u.LocalWar.SealStoneAttacked.Nation=NW_BY; break;
	case 99 : packet.u.LocalWar.SealStoneAttacked.Nation=NW_ZY; break;
	case 91 : packet.u.LocalWar.SealStoneAttacked.Nation=NW_YL; break;
	}
	packet.u.LocalWar.SealStoneAttacked.MapNumber=tp->MapNumber;
	packet.u.LocalWar.SealStoneAttacked.LocateX=tp->X;
	packet.u.LocalWar.SealStoneAttacked.LocateY=tp->Y;
	packet.h.header.size=sizeof(t_SealStoneAttacked);

	g_pUserManager->SendPacket(&packet); // CSD-CN-031213
	SendPacket2Maps(&packet);
}

void RecvCMD_SEALSTONE_ATTACKED( t_packet *p, t_connection c[], int cn )
{
	if (MapInfo[MapNumber].nation==p->u.LocalWar.SealStoneAttacked.Nation) 
	{	//< CSD-CN-031213
		g_pUserManager->SendPacket(p);
		return; 
	}	//> CSD-CN-031213

	if (MapInfo[MapNumber].nation == 0)
	{
		SendPacket2NationClients(p->u.LocalWar.SealStoneAttacked.Nation,p);
	}
}

void SendCMD_LOCALWAR_POINT_CHANGED()
{
	t_packet packet;

	packet.h.header.type=CMD_LOCALWAR_POINT_CHANGED;
	packet.u.LocalWar.LocalWarPointChanged.RemainTime=g_pNation->GetLocalWarRemainTime()-g_curr_time;
	packet.u.LocalWar.LocalWarPointChanged.Point[0]=g_pNation->GetLocalWarPoint(0);
	packet.u.LocalWar.LocalWarPointChanged.Point[1]=g_pNation->GetLocalWarPoint(1);
	packet.u.LocalWar.LocalWarPointChanged.Point[2]=g_pNation->GetLocalWarPoint(2);
	packet.h.header.size=sizeof(t_LocalWarPointChanged);

	SendPacket2Maps(&packet);
	g_pUserManager->SendPacket(&packet); // CSD-CN-031213
}

int LocalWarPointMAXIndex()						// LTS NEW LOCALWAR
{
	if (!isNationManageServer()) return 0;

	short Point[3], MAXPointIndex=0;
	Point[0]=g_pNation->GetLocalWarPoint(0);
	Point[1]=g_pNation->GetLocalWarPoint(1);
	Point[2]=g_pNation->GetLocalWarPoint(2);

	if (Point[MAXPointIndex]<Point[1]) MAXPointIndex=1;
	if (Point[MAXPointIndex]<Point[2]) MAXPointIndex=2;

	return MAXPointIndex;
}

int CheckMaxPoint(int nPoint1, int nPoint2) 
{	// 030506 kyo
	if (nPoint1 > nPoint2)
	{
		return 3;	// LTH-040308-KO 상위 함수와 구분짓기위해. 3:nPoint1>nPoint2, 4:nPoint1<nPoint2, 5:nPoint1=nPoint2
	}
	else if (nPoint1 < nPoint2)
	{
		return 4;	// LTH-040308-KO 상위 함수와 구분짓기위해
	}
	else
	{
		return 5;	// LTH-040308-KO 상위 함수와 구분짓기위해
	}
}

//< LTH-040308-KO 상위 함수의 숫자와 구분짓기 위해 반환하는 수를 바꾸고 일스도 제대로 비교하게 만듬
// 6:nIndex1>nIndex2, 7:nIndex1<nIndex2, 8:nIndex1=nIndex2
int CheckMaxArrayPoint(int nIndex1, int nIndex2, unsigned short Array_1[3] ,unsigned short Array_2[3], unsigned short Array_3[3])
{//3배열의 nIndex1과 nIndex2중에 큰 값을 리턴 0: nIndex1, 1:nIndex2, 2:두 값이 동일
	int rt = CheckMaxPoint(Array_1[nIndex1], Array_1[nIndex2]);

	switch (rt)
	{
	case 5:
		{
			rt = CheckMaxPoint(Array_2[nIndex1], Array_2[nIndex2]);

			switch (rt)
			{
			case 5:
				{
					rt = CheckMaxPoint( Array_3[nIndex1], Array_3[nIndex2] );

					switch (rt)
					{
					case 5: return 9;
					case 3: return 7;
					case 4: return 8;
					}

					break;
				}
			case 3: return 7;
			case 4: return 8;
			}

			break;
		}
	case 3: return 7;
	case 4: return 8;
	}

	return -1;
}
//> LTH-040308-KO


void SendCMD_LOCALWAR_END_WIN( const int nWinNation)
{// 030518 kyo
	t_packet packet;
	packet.h.header.type=CMD_LOCALWAR_END;
	packet.u.LocalWar.CommonDataC.Data=nWinNation;	
	packet.h.header.size=sizeof(t_CommonDataC);
	SendPacket2Maps(&packet);//모든 맵에 보내서 페임계산	
}

void SendCMD_LOCALWAR_END()						// LTS NEW LOCALWAR  // 국지전 종료 
{	//const NationTBL[3]={ 3,4,6 };
	const int NationTBL[NW_COMMANDER]={ NW_BY,NW_ZY,NW_YL}; // 03518 kyo
	const char NationTBLStr[NW_COMMANDER][8]={"NW_BY","NW_ZY","NW_YL"}; // 030515 kyo

	//t_packet packet;
	_ASSERTE(g_pNation);// 030518 kyo

	//int MAXPointIndex=g_pNation->CheckWinNation();
	int nWinNation=g_pNation->CheckWinNation();	
	
	// 모든 상황이 같다면 모든 국가가 승리하게 한다. 
	// 0:바이서스 1:자이펀 2:일스 3:(자이펀=일스)>바이서스 4:(바이서스=일스)>자이펀 5:(자이펀=바이서스)>일스 6: 모두동일
	// 여러국가의 동시 승리일 경우 국가번호의 +값으로 계산 
	// ex) 3,4,6 : 한국가(NationTBL[nWinNation])  7(바+자),9(자+일),10(바+일),13(다)
	switch( nWinNation )
	{
	case 0:
	case 1:
	case 2:
		{
			//packet.u.LocalWar.CommonDataC.Data = NationTBL[nWinNation];
			SendCMD_LOCALWAR_END_WIN(NationTBL[nWinNation]);// 030518 kyo
			MyLog(0, "LocalWar End:: Win Team %s", NationTBLStr[nWinNation] );// 030515 kyo
			break;
		}
	case 3:
		{
			//packet.u.LocalWar.CommonDataC.Data = NationTBL[1]+NationTBL[2];
			SendCMD_LOCALWAR_END_WIN(NationTBL[1]);// 030518 kyo 
			SendCMD_LOCALWAR_END_WIN(NationTBL[2]);// 030518 kyo 
			MyLog(0, "LocalWar End:: Win Team=%s && Win Team=%s", NationTBLStr[1], NationTBLStr[2] );// 030515 kyo
			break;
		}
	case 4:
		{
			//packet.u.LocalWar.CommonDataC.Data = NationTBL[0]+NationTBL[2];
			SendCMD_LOCALWAR_END_WIN(NationTBL[0]);// 030518 kyo 
			SendCMD_LOCALWAR_END_WIN(NationTBL[2]);// 030518 kyo 
			MyLog(0, "LocalWar End:: Win Team=%s && Win Team=%s", NationTBLStr[0], NationTBLStr[2] );// 030515 kyo
			break;
		}
	case 5:
		{
			//packet.u.LocalWar.CommonDataC.Data = NationTBL[0]+NationTBL[1];
			SendCMD_LOCALWAR_END_WIN(NationTBL[0]);// 030518 kyo 
			SendCMD_LOCALWAR_END_WIN(NationTBL[1]);// 030518 kyo 
			MyLog(0, "LocalWar End:: Win Team=%s && Win Team=%s", NationTBLStr[0], NationTBLStr[1] );// 030515 kyo
			break;
		}
	case 6:
		{
			//packet.u.LocalWar.CommonDataC.Data = NationTBL[0]+NationTBL[1]+NationTBL[2];			
			//MyLog(0, "LocalWar End:: Win Team=%s && Win Team=%s && Win Team=%s", NationTBLStr[0], NationTBLStr[1], NationTBLStr[2] );// 03515 kyo
			SendCMD_LOCALWAR_END_WIN(0);// 030518 kyo 			
			MyLog(0, "LocalWar End:: All Lose ~~~~~ ");
			break;
		}
	default:
		{
		//_ASSERT(NULL);
		break;
		}
	
	}
	/*
	packet.h.header.type=CMD_LOCALWAR_END;
	//packet.u.LocalWar.CommonDataC.Data=NationTBL[MAXPointIndex];
	packet.u.LocalWar.CommonDataC.Data=NationTBL[nWinNation];	
	packet.h.header.size=sizeof(t_CommonDataC);
	//if (MAXPointIndex>=0) CalcMapUsersGainedFame(NationTBL[MAXPointIndex]);
	//CalcMapUsersGainedFame(NationTBL[nWinNation]); //RecvCMD_LOCALWAR_END()에서 페임계산한다. 중복계산댄다.
	//SendCMD_LOCALWAR_END2Client(&packet);// 현재 맵의 국지전에 참가한 모든 유저의 참가를 취소한다. 중복됨
	SendPacket2Maps(&packet);//모든 맵에 보내서 페임계산
	*/	
}

void ReturnLocalwarHome()
{	//< CSD-CN-031213
	CUserManager::HASH_USER user = g_pUserManager->GetUserSet();

	for (CUserManager::ITOR_USER i = user.begin(); i != user.end(); ++i)
	{
		const int idUser = i->second;
		CHARLIST* pUser = CheckServerId(idUser);
		
		if (pUser != NULL && pUser->JoinLocalWar)
		{
			pUser->JoinLocalWar = 0;
			CheckAndSendChangeEquip(pUser, 1);
			
			LPMAPINFO pMapInfo = GetMapInfoByMapPort(g_MapPort);
	
			if (!pMapInfo)
			{
				continue;
			}

			if (pMapInfo->nation == 0 || pMapInfo->nation == pUser->name_status.nation)
			{
				continue;
			}
			// 적국인 경우만 자기의 국가로 이동
			switch (pUser->name_status.nation)
			{
			case NW_BY:	
				{
					MapMove(idUser, "MA-IN", 245, 115);
					break;
				}
			case NW_ZY:	
				{
					MapMove(idUser, "RENES_C", 210, 175); 
					break;
				}
			case NW_YL:	
				{
					MapMove(idUser, "BARANTAN", 325, 98); 
					break;
				}
			}
		}
	}
}	//> CSD-CN-031213

void RecvCMD_LOCALWAR_END(t_packet *p, t_connection c[], int cn )
{	//< CSD-031007
	g_LocalWarBegin=0;
	//	MyLog(0,"Local War Status : %d",g_LocalWarBegin);
	::CalcMapUsersGainedFame(p->u.LocalWar.CommonDataC.Data);
	::SendCMD_LOCALWAR_END2Client(p);

	if (g_isLocalWarServer)
	{
		g_pLocalWarfield->ClearManCount();
		g_pLocalWarfield->SetLocalWarStatus(0);
	}

	::ReturnLocalwarHome();// 030518 kyo	
}	//> CSD-031007

void RecvCMD_LOCALWAR_NATION_POINT_CHANGED(t_packet *p, t_connection c[], int cn )
{
	t_packet packet;

	g_pNation->IncLocalWarPoint(0,p->u.LocalWar.NationPointChanged.NationPoint[0]);
	g_pNation->IncLocalWarPoint(1,p->u.LocalWar.NationPointChanged.NationPoint[1]);
	g_pNation->IncLocalWarPoint(2,p->u.LocalWar.NationPointChanged.NationPoint[2]);

	packet.h.header.type=CMD_LOCALWAR_POINT_BBS;
	packet.u.LocalWar.LocalWarPointChanged.RemainTime=g_pNation->GetLocalWarRemainTime()-g_curr_time;
	packet.u.LocalWar.LocalWarPointChanged.Point[0]=g_pNation->GetLocalWarPoint(0);
	packet.u.LocalWar.LocalWarPointChanged.Point[1]=g_pNation->GetLocalWarPoint(1);
	packet.u.LocalWar.LocalWarPointChanged.Point[2]=g_pNation->GetLocalWarPoint(2);
	packet.h.header.size=sizeof(t_LocalWarPointChanged);

	SendPacket2LocalWarJoinClients(&packet);
	SendPacket2Maps(&packet);
}

void RecvCMD_LOCALWAR_POINT_BBS(t_packet *p, t_connection c[], int cn )
{
	SendPacket2LocalWarJoinClients(p);
}

void RecvCMD_ACTION_STATUS(t_packet *p, t_connection c[], int cn )
{
	connections[cn].chrlst.nCurrentAction=p->u.LocalWar.ActionStatus.nCurrentAction;
	p->u.LocalWar.ActionStatus.ServerID=cn;
	CastMe2Other(cn,p);
}

void RecvCMD_SITDOWN_DIRECTION_CHANGED(t_packet *p, t_connection c[], int cn )
{
	connections[cn].chrlst.Direction=(DIRECTION)p->u.LocalWar.SDChanged.Direction;
	p->u.LocalWar.SDChanged.ServerID=cn;
	CastMe2Other(cn,p);
}

// 020428 YGI acer
void RecvCMD_HERO_ACTION_CHANGED(t_packet *p, t_connection c[], int cn )
{ 
	CHARLIST* ch=&connections[cn].chrlst;

	if (p->u.LocalWar.CommonDataC.Data==ACTION_SITDOWN)
	{
		MoveEmptyInv( &ch->equip[ WT_SHOES], ch );
		POS pos;
		SetItemPos( EQUIP, WT_SHOES, &pos );
		// 020428 YGI acer
		SendDeleteItem( &ch->equip[ WT_SHOES], &pos, ch );
		CheckAndSendChangeEquip( ch, 1 );
	}
}

// Event LocalWar System _____________________________________________________________________________

bool LoadStartUpPoint()
{
	// 030919 HK YGI
//	char	NetworkDir[MAX_PATH];
//	char	MapServerConfigFileName[MAX_PATH];
	int		i;
	char	tempString[MAX_PATH];

	/*
	if (GetPrivateProfileString("Network","path","",NetworkDir,MAX_PATH, MAP_SERVER_INI_)<=0)
	{
		MyLog(0,"Mapserver.ini의  Path설정이 잘못되어있습니다.");
		return false;
	}
	sprintf(MapServerConfigFileName,"%s/data/MapServerConfig.ini",NetworkDir); */// 030919 HK YGI
	
	for (i=0;i<6;i++)	// 나라번호
	{
		sprintf_s(tempString, sizeof(tempString),"EventMap%dStartPointX",i);
		g_StartPoint[i].x=(short)GetPrivateProfileInt("EventLocalWar",tempString,0,MapServerConfigFileName);
		if (!g_StartPoint[i].x)
		{
			MyLog(0,"%s의 [EventLocalWar], %s 설정이 잘못되어있습니다.",MapServerConfigFileName,tempString);
			return false;
		}
		sprintf_s(tempString, sizeof(tempString),"EventMap%dStartPointY",i);
		g_StartPoint[i].y=(short)GetPrivateProfileInt("EventLocalWar",tempString,0,MapServerConfigFileName);
		if (!g_StartPoint[i].y)
		{
			MyLog(0,"%s의 [EventLocalWar], %s 설정이 잘못되어있습니다.",MapServerConfigFileName,tempString);
			return false;
		}
	}

	return true;
}

bool InitEventLocalWarSystem()								// 020115 LTS		// LTS BUG
{
	// 030919 HK YGI
//	char	NetworkDir[MAX_PATH];
//	char	MapServerConfigFileName[MAX_PATH];
	int		EventLocalWarfieldPort=0;
	int		MapServerPort;

	SERVER_DATA	*pData=g_pServerTable->GetOwnServerData();
	MapServerPort=pData->wPort;

/*
	if (GetPrivateProfileString("Network","path","",NetworkDir,MAX_PATH,MAP_SERVER_INI_)<=0)
	{
		MyLog(0,"Mapserver.ini의  Path설정이 잘못되어있습니다.");
		return false;
	}
	sprintf(MapServerConfigFileName,"%s/data/MapServerConfig.ini",NetworkDir);*/
	EventLocalWarfieldPort=(DWORD)GetPrivateProfileInt("EventLocalWar","EventLocalWarfieldPort",0,MapServerConfigFileName);
	if (!EventLocalWarfieldPort)
	{
		MyLog(0,"%s의 [EventLocalWar],EventLocalWarfieldPort의 설정이 잘못되어있습니다.",MapServerConfigFileName);
		return false;
	}

	if (MapServerPort==EventLocalWarfieldPort)		// 이벤트 맵이라면 
	{
		g_isEventLocalWarServer=true;
		g_pEventLocalWarfield=new cEventLocalWarfield;
		g_EventMapPort=EventLocalWarfieldPort;
		if (!g_pEventLocalWarfield->CheckLoadData())
		{
			JustMessage("이벤트 맵이지만 환경파일(MapseverConfig.ini)를 읽지 못했습니다.");
		}
	}
	else 
	{
		g_EventMapPort=EventLocalWarfieldPort;		// 지금 이벤트 진행중인가? 확인시 또는 참가 신청 인원 파악.
		if (!LoadStartUpPoint()) 
		{
			JustMessage("환경파일(MapseverConfig.ini)의 EventLocalWar를 읽지 못했습니다.");
		}
	}
	return true;
}

void CloseEventLocalWarSystem()
{
	SAFE_DELETE(g_pEventLocalWarfield);
}

void UpdateEventLocalWarTime()
{
	if (g_isEventLocalWarServer)
	{
		if(g_pEventLocalWarfield)
		{
			g_pEventLocalWarfield->UpdateEventLocalWarTime();
		}
		else
		{
			::MyLog(1,"WARNING!!! -- g_pEventLocalWarfield is Null -- WARNING!!!");
		}
	}
}

int	 GetNationIndex(int Nation)
{
	switch (Nation)
	{
	case NW_BY : return 0;
	case NW_ZY : return 1;
	case NW_YL : return 2;
	}
	return 0;
}

int GetNationFromIndex(int NationIndex)			// LTS NEW LOCALWAR
{
	switch(NationIndex)
	{
	case 0 : return NW_BY;
	case 1 : return NW_ZY;
	case 2 : return NW_YL;
	}
	return 3;
}


void SendEventPoint(int UserID,int Point)
{
	t_packet packet;
	packet.h.header.type=CMD_EVENT_POINT_CHANGE;
	packet.u.LocalWar.EventPoint.Point=Point;
	packet.h.header.size=sizeof(t_EventPoint);
	QueuePacket(connections,UserID,&packet,1);

}
void CheckEventLocalWarDeath(CHARLIST* Attacker,CHARLIST* Defencer)
{
	if (!g_isEventLocalWarServer) return;
	if (g_pEventLocalWarfield->GetEventStatus()!=3) return;

	const int LevelGap = Defencer->GetLevel() - Attacker->GetLevel(); // CSD-030806
	MyLog(0,"Event LocalWar Death_______");
	MyLog(0," Attacker : %s, Level : %d, Defender : %s, Level : %d, Level Gap : %d",
		    Attacker->Name, Attacker->GetLevel(),
			Defencer->Name, Defencer->GetLevel(),
			Defencer->GetLevel() - Attacker->GetLevel()); // CSD-030806
	MyLog(0," Attacker Old Event Poind : %d",Attacker->EventPoint);
	if (LevelGap<=(-30))						{ Attacker->EventPoint+=1;  }
	if ((LevelGap>=(-29))&&(LevelGap<=(-10)))	{ Attacker->EventPoint+=3;  }
	if ((LevelGap>=(-9))&&(LevelGap<=(0)))		{ Attacker->EventPoint+=5;  }
	if ((LevelGap>=(1))&&(LevelGap<=(20)))		{ Attacker->EventPoint+=10; }
	if (LevelGap>=(21))							{ Attacker->EventPoint+=15; }
	MyLog(0," Attacker New Event Point : %d",Attacker->EventPoint);
	SendEventPoint(Attacker->GetServerID(),Attacker->EventPoint);
}

bool CanUseRevivalPosion()
{
	if(g_isEventLocalWarServer)
	{
		if( g_pEventLocalWarfield->GetEventStatus()==3 ) return false;
	}
	return true;
}
int CheckEventAttack()
{
	if (!g_isEventLocalWarServer) return FALSE;        // 이벤트 서버가 아니면 공격할수 있음
	if (g_pEventLocalWarfield->GetEventStatus()!=3) return TRUE;	// 이벤트 서버이고 이벤트의 상태가 전쟁시작이 아니면 공격할수 없음
	return FALSE;										//이벤트서버의 전쟁중엔 공격할수 있음										
}

int CheckEventWarDoing()
{
	if (!g_isEventLocalWarServer) return false;
	if (g_pEventLocalWarfield->GetEventStatus()!=3) return false;
	return true;
}


void ClearEventJoin(CHARLIST* ch,int EventNo)
{
	t_packet packet;
	packet.h.header.type=CMD_DELETE_EVENT_JOIN;
	strcpy_s(packet.u.LocalWar.DeleteEventJoin.UserName, sizeof(packet.u.LocalWar.DeleteEventJoin.UserName),ch->Name);
	packet.u.LocalWar.DeleteEventJoin.EventNo=EventNo;
	packet.h.header.size=sizeof(t_DeleteEventJoin);
	QueuePacket(connections, DB_DEMON, &packet, 1);

	switch (EventNo)
	{
	case 1 :	ch->EventJoin.Event1=0;	break;
	case 2 :	ch->EventJoin.Event2=0; break;
	case 3 :	ch->EventJoin.Event3=0; break;
	case 4 :	ch->EventJoin.Event4=0; break;
	case 5 :	ch->EventJoin.Event5=0; break;
	case 6 :	ch->EventJoin.Event6=0; break;
	case 7 :	ch->EventJoin.Event7=0; break;
	case 8 :	ch->EventJoin.Event8=0; break;
	}
}

int CheckEventJoin(CHARLIST* ch,int EventNo)
{
	switch (EventNo)
	{
	case 1 : return ch->EventJoin.Event1;
	case 2 : return ch->EventJoin.Event2;
	case 3 : return ch->EventJoin.Event3;
	case 4 : return ch->EventJoin.Event4;
	case 5 : return ch->EventJoin.Event5;
	case 6 : return ch->EventJoin.Event6;
	case 7 : return ch->EventJoin.Event7;
	default : return ch->EventJoin.Event8;
	}
}

void ReLoadEventLocalWarData()			// LTS BUG
{
	if (!g_isEventLocalWarServer) return;
	g_pEventLocalWarfield->LoadEventLocalWarfieldData();
}

cEventLocalWarfield::cEventLocalWarfield()
{
	InitEventLocalWarfield();
	LoadEventLocalWarfieldData();
}

cEventLocalWarfield::~cEventLocalWarfield()
{

}

void cEventLocalWarfield::InitEventLocalWarfield()
{
	m_EventStep=0;
	m_EventStatus=0;			
	m_EventEndTime=0;
	m_CheckLoadData=1;	
	m_ManCount[0]=0;
	m_ManCount[1]=0;
	m_ManCount[2]=0;
	memset(&m_tData,0,sizeof(m_tData));
}


void cEventLocalWarfield::LoadEventLocalWarfieldData()				// LTS BUG
{
	// 030919 HK YGI
//	char	NetworkDir[MAX_PATH];
//	char	MapServerConfigFileName[MAX_PATH];
	int		i,j;
	char	tempString[MAX_PATH],tempString2[MAX_PATH];

	/*
	if (GetPrivateProfileString("Network","path","",NetworkDir,MAX_PATH,MAP_SERVER_INI_)<=0)
	{
		MyLog(0,"Mapserver.ini의  Path설정이 잘못되어있습니다.");
		m_CheckLoadData=0;
		return;
	}
	sprintf(MapServerConfigFileName,"%s/data/MapServerConfig.ini",NetworkDir);*/

	m_tData.EventActive=(char)GetPrivateProfileInt("EventLocalWar","EventActive",4,MapServerConfigFileName);
	if (m_tData.EventActive==4)
	{
		MyLog(0,"%s의 [EventLocalWar],EventActive 설정이 잘못되어있습니다.",MapServerConfigFileName);
		m_CheckLoadData=0;
		return;
	}
	m_tData.EventStartTime=(char)GetPrivateProfileInt("EventLocalWar","EventStartTime",0,MapServerConfigFileName);
	if (!m_tData.EventStartTime)
	{
		MyLog(0,"%s의 [EventLocalWar],EventStartTime 설정이 잘못되어있습니다.",MapServerConfigFileName);
		m_CheckLoadData=0;
		return;
	}
	m_tData.EventRegTime=(char)GetPrivateProfileInt("EventLocalWar","EventRegTime",0,MapServerConfigFileName);
	if (!m_tData.EventRegTime)
	{
		MyLog(0,"%s의 [EventLocalWar],EventRegTime 설정이 잘못되어있습니다.",MapServerConfigFileName);
		m_CheckLoadData=0;
		return;
	}
	m_tData.EventMapMoveTime=(char)GetPrivateProfileInt("EventLocalWar","EventMapMoveTime",0,MapServerConfigFileName);
	if (!m_tData.EventMapMoveTime)
	{
		MyLog(0,"%s의 [EventLocalWar],EventMapMoveTime 설정이 잘못되어있습니다.",MapServerConfigFileName);
		m_CheckLoadData=0;
		return;
	}
	m_tData.EventTime=(char)GetPrivateProfileInt("EventLocalWar","EventTime",0,MapServerConfigFileName);
	if (!m_tData.EventTime)
	{
		MyLog(0,"%s의 [EventLocalWar],EventTime 설정이 잘못되어있습니다.",MapServerConfigFileName);
		m_CheckLoadData=0;
		return;
	}
	m_tData.EventDelayTime=(char)GetPrivateProfileInt("EventLocalWar","EventDelayTime",0,MapServerConfigFileName);
	if (!m_tData.EventDelayTime)
	{
		MyLog(0,"%s의 [EventLocalWar],EventDelayTime 설정이 잘못되어있습니다.",MapServerConfigFileName);
		m_CheckLoadData=0;
		return;
	}
	for (i=0;i<3;i++)	
	{
		sprintf_s(tempString, sizeof(tempString),"Event%dUserLevelMin",i);
		m_tData.EventUserLevelMin[i]=(unsigned char)GetPrivateProfileInt("EventLocalWar",tempString,0,MapServerConfigFileName);
		if (!m_tData.EventUserLevelMin[i])
		{
			MyLog(0,"%s의 [EventLocalWar], %s 설정이 잘못되어있습니다.",MapServerConfigFileName,tempString);
			m_CheckLoadData=0;
			return;
		}
		sprintf_s(tempString, sizeof(tempString),"Event%dUserLevelMax",i);
		m_tData.EventUserLevelMax[i]=(unsigned char)GetPrivateProfileInt("EventLocalWar",tempString,0,MapServerConfigFileName);
		if (!m_tData.EventUserLevelMax[i])
		{
			MyLog(0,"%s의 [EventLocalWar], %s 설정이 잘못되어있습니다.",MapServerConfigFileName,tempString);
			m_CheckLoadData=0;
			return;
		}
	}

	for (i=0;i<6;i++)	// 나라번호
	{
		sprintf_s(tempString, sizeof(tempString),"EventMap%dStartPointX",i);
		m_tData.StartPoint[i].x=(short)GetPrivateProfileInt("EventLocalWar",tempString,0,MapServerConfigFileName);
		if (!m_tData.StartPoint[i].x)
		{
			MyLog(0,"%s의 [EventLocalWar], %s 설정이 잘못되어있습니다.",MapServerConfigFileName,tempString);
			m_CheckLoadData=0;
			return;
		}
		sprintf_s(tempString, sizeof(tempString),"EventMap%dStartPointY",i);
		m_tData.StartPoint[i].y=(short)GetPrivateProfileInt("EventLocalWar",tempString,0,MapServerConfigFileName);
		if (!m_tData.StartPoint[i].y)
		{
			MyLog(0,"%s의 [EventLocalWar], %s 설정이 잘못되어있습니다.",MapServerConfigFileName,tempString);
			m_CheckLoadData=0;
			return;
		}
	}

	for (i=0;i<3;i++)	// 나라번호
	{
		for (j=0;j<3;j++)
		{
			switch(j)
			{
			case 0 : sprintf_s(tempString2, sizeof(tempString2),"BY"); break;
			case 1 : sprintf_s(tempString2, sizeof(tempString2),"ZY"); break;
			case 2 : sprintf_s(tempString2, sizeof(tempString2),"YL"); break;
			}
			sprintf_s(tempString, sizeof(tempString),"Event%dRegUser%sMAX",i,tempString2);
			m_tData.EventRegMax[i][j]=(unsigned char)GetPrivateProfileInt("EventLocalWar",tempString,0,MapServerConfigFileName);
			if (!m_tData.EventRegMax[i][j])
			{
				MyLog(0,"%s의 [EventLocalWar], %s 설정이 잘못되어있습니다.",MapServerConfigFileName,tempString);
				m_CheckLoadData=0;
				return;
			}
		}
	}

	for (i=0;i<3;i++)
	{
		switch(i)
		{
		case 0 : sprintf_s(tempString2, sizeof(tempString2),"BY"); break;
		case 1 : sprintf_s(tempString2, sizeof(tempString2),"ZY"); break;
		case 2 : sprintf_s(tempString2, sizeof(tempString2),"YL"); break;
		}
		sprintf_s(tempString, sizeof(tempString),"EventMap%sOutPointX",tempString2);
		m_tData.OutPoint[i].x=(short)GetPrivateProfileInt("EventLocalWar",tempString,0,MapServerConfigFileName);
		if (!m_tData.OutPoint[i].x)
		{
			MyLog(0,"%s의 [EventLocalWar], %s 설정이 잘못되어있습니다.",MapServerConfigFileName,tempString);
			m_CheckLoadData=0;
			return;
		}
		sprintf_s(tempString, sizeof(tempString),"EventMap%sOutPointY",tempString2);
		m_tData.OutPoint[i].y=(short)GetPrivateProfileInt("EventLocalWar",tempString,0,MapServerConfigFileName);
		if (!m_tData.OutPoint[i].y)
		{
			MyLog(0,"%s의 [EventLocalWar], %s 설정이 잘못되어있습니다.",MapServerConfigFileName,tempString);
			m_CheckLoadData=0;
			return;
		}
	}

	for (i=0;i<3;i++)
	{
		switch(i)
		{
		case 0 : sprintf_s(tempString2, sizeof(tempString2),"BY"); break;
		case 1 : sprintf_s(tempString2, sizeof(tempString2),"ZY"); break;
		case 2 : sprintf_s(tempString2, sizeof(tempString2),"YL"); break;
		}
		sprintf_s(tempString, sizeof(tempString),"EventJoin%sLocationX",tempString2);
		m_tData.EventJoinLocation[i].x=(short)GetPrivateProfileInt("EventLocalWar",tempString,0,MapServerConfigFileName);
		if (!m_tData.EventJoinLocation[i].x)
		{
			MyLog(0,"%s의 [EventLocalWar], %s 설정이 잘못되어있습니다.",MapServerConfigFileName,tempString);
			m_CheckLoadData=0;
			return;
		}
		sprintf_s(tempString, sizeof(tempString),"EventJoin%sLocationY",tempString2);
		m_tData.EventJoinLocation[i].y=(short)GetPrivateProfileInt("EventLocalWar",tempString,0,MapServerConfigFileName);
		if (!m_tData.EventJoinLocation[i].y)
		{
			MyLog(0,"%s의 [EventLocalWar], %s 설정이 잘못되어있습니다.",MapServerConfigFileName,tempString);
			m_CheckLoadData=0;
			return;
		}
	}

	m_tData.EventMessageTime=(char)GetPrivateProfileInt("EventLocalWar","EventMessageTime",0,MapServerConfigFileName);
	if (m_tData.EventMessageTime==0)
	{
		MyLog(0,"%s의 [EventLocalWar],EventMessageTime 설정이 잘못되어있습니다.",MapServerConfigFileName);
		m_CheckLoadData=0;
		return;
	}
}

void cEventLocalWarfield::InitEvent()	// 한이벤트가 지날때마다 초기화 한다.
{
	m_ManCount[0]=0;
	m_ManCount[1]=0;
	m_ManCount[2]=0;
	ClearUserALL();
}

void cEventLocalWarfield::ClearUserCloth()
{	//< CSD-CN-031213
	CUserManager::HASH_USER user = g_pUserManager->GetUserSet();

	for (CUserManager::ITOR_USER i = user.begin(); i != user.end(); ++i)
	{
		const int idUser = i->second;
		CHARLIST* pUser = ::CheckServerId(idUser);
		
		if (pUser != NULL)
		{
			::CheckAndSendChangeEquip(pUser, 1);
		}
	}
}	//> CSD-HK-031213

void cEventLocalWarfield::SetEventLocalWarStatus(int Status)
{
/*	t_packet packet;
	
	m_EventStatus=Status;
	packet.h.header.type=CMD_EVENT_LOCALWAR_STATUS_CHANGE;
	packet.u.LocalWar.CommonDataC.Data=m_EventStatus;
	packet.h.header.size=sizeof(t_CommonDataC);

	SendPacket2Clients(&packet);
	SendPacket2Maps(&packet);*/

//	switch (m_EventStatus)
//	{
//	case 3 : m_EventEndTime=g_curr_time+EventTime; break;	//국지전이 시작되었다면 
//	case 4 : ClearUserCloth(); break;
//	}
}

void cEventLocalWarfield::SendEventDoingMessage()
{
	t_packet packet;

	DWORD RemainTime=m_EventEndTime-g_curr_time;

	packet.h.header.type=CMD_EVENT_REMAIN_TIME;
	packet.u.LocalWar.EventRemainTime.RemainTime=RemainTime;
	packet.h.header.size=sizeof(t_EventRemainTime);
	g_pUserManager->SendPacket(&packet); // CSD-CN-031213
}

void cEventLocalWarfield::UpdateEventLocalWarTime()		// LTS BUG
{
	static DWORD RemainTime;
	if (!m_tData.EventActive) return;

	if (!m_EventStatus)				//이벤트가 진행중이 아니다.
	{
		if (g_hour==m_tData.EventStartTime)
		{
			m_EventStatus=EVENT_JOIN;
			m_EventStep=EVENT_1;		
			m_EventEndTime=g_curr_time+(m_tData.EventRegTime*60);
			EventLocalWarMessage();
			return;
		}
		else return;
	}

	if (m_EventStatus==EVENT_DOING)	//메시지를 보낸다
	{
		if (g_curr_time>RemainTime)
		{
			RemainTime=g_curr_time+(m_tData.EventMessageTime*60);
			SendEventDoingMessage();
		}
	}

	if (g_curr_time<m_EventEndTime) return;	

	if (m_EventStatus>EVENT_REST)
	{
		if (m_EventStep>=EVENT_3)
		{
			m_EventStep=EVENT_1;
			m_EventStatus=EVENT_NOTHING;	
			EventLocalWarMessage();
		}
		else
		{
			m_EventStep++;
			m_EventStatus=EVENT_JOIN;
			m_EventEndTime=g_curr_time+(m_tData.EventRegTime*60);
			EventLocalWarMessage();
		}
	}
	else
	{
		m_EventStatus++;
		switch(m_EventStatus)
		{	
		case EVENT_JOIN		: m_EventEndTime=g_curr_time+(m_tData.EventRegTime*60);		break;
		case EVENT_MOVE		: m_EventEndTime=g_curr_time+(m_tData.EventMapMoveTime*60);	break;
		case EVENT_DOING	: m_EventEndTime=g_curr_time+(m_tData.EventTime*60);			break;
		case EVENT_REST		: m_EventEndTime=g_curr_time+(m_tData.EventDelayTime*60);	break;
		}
		EventLocalWarMessage();
	}
}

extern int g_year;
extern int g_mon;
extern int g_day;

void cEventLocalWarfield::LogJoinUser()			// LTS HORSERIDER
{
	FILE*	fp;
	char	FileName[MAX_PATH];

	sprintf_s(FileName, sizeof(FileName),"./OutPut/EventLW%d%d%d.Log",g_year,g_mon,g_day);
	
	fp=fopen(FileName,"a");
	if (!fp) return;
	fprintf(fp,"Join User Recording (%d-%d-%d, Event No: %d, Event Status : %d)\n",g_year,g_mon,g_day,m_EventStep,m_EventStatus);
	fprintf(fp,"______________________\n");
	
	for  (int i = DRAGON_CONNECTIONS_START; i < DRAGON_MAX_CONNECTIONS; ++i)
	{	//< CSD-HK-030829
		CHARLIST* pUser = ::CheckServerId(i);
		
		if (pUser != NULL)
		{
			fprintf(fp," User Name : %s\n", pUser->Name);
			ClearEventJoin(pUser, 6);			// 서바이벌 이벤트 번호
			pUser->EventLocalWarJoin = 0;		// 초기화
		}
	}	//> CSD-HK-030829

	fprintf(fp,"______________________\n");
	fprintf(fp,"Join User Record Complete..\n");
	fprintf(fp,"______________________\n\n\n");
	fclose(fp);
}


void cEventLocalWarfield::LogVictoryUser()	// LTS HORSERIDER
{
	FILE*	fp;
	char	FileName[MAX_PATH];

	sprintf_s(FileName, sizeof(FileName),"./OutPut/EventLWVictory%d%d%d.Log",g_year,g_mon,g_day);
	
	fp=fopen(FileName,"a");
	if (!fp) return;
	fprintf(fp,"Victory User Recording (%d-%d-%d, Event No : %d, Event Status : %d)\n",g_year,g_mon,g_day,m_EventStep,m_EventStatus);
	fprintf(fp,"______________________\n");
	MyLog(0,"Victory User List");
	MyLog(0,"_____________________________");
	
	for (int i = DRAGON_CONNECTIONS_START; i < DRAGON_MAX_CONNECTIONS; ++i)
	{	//< CSD-HK-030829
		CHARLIST* pUser = ::CheckServerId(i);
		
		if (pUser != NULL && pUser->bAlive == ALIVE_)
		{
			fprintf(fp," User Name : %s, Event Point : %d\n", pUser->Name, pUser->EventPoint);
			MyLog(0," User Name : %s, Event Point : %d\n", pUser->Name, pUser->EventPoint);
		}
	}	//> CSD-HK-030829

	MyLog(0,"_____________________________");
	MyLog(0,"Victory User Listing Complete..\n");
	fprintf(fp,"______________________\n");
	fprintf(fp,"Victory User Recording Complete..\n");
	fprintf(fp,"______________________\n\n\n");
	fclose(fp);
}


void cEventLocalWarfield::EventLocalWarMessage()
{
	t_packet packet;

	if (m_EventStep<0||m_EventStep>3) return;						
	if (m_EventStatus<0||m_EventStatus>5) return;

	switch(m_EventStatus)
	{
	case EVENT_JOIN		:	InitEvent(); 			break;
	case EVENT_DOING	:	LogJoinUser();			break;
	case EVENT_REST		:	ClearUserCloth();
							LogVictoryUser();
							ClearManCount();		break;
							
	}



	g_EventLocalWarStatus=m_EventStatus;
	g_EventLocalWarStep=m_EventStep;
	packet.h.header.type=CMD_EVENT_LOCALWAR_MESSAGE;
	packet.u.LocalWar.EventLocalWarMessage.EventStep=m_EventStep;
	packet.u.LocalWar.EventLocalWarMessage.EventStatus=m_EventStatus;
	packet.u.LocalWar.EventLocalWarMessage.MinLevel=m_tData.EventUserLevelMin[m_EventStep];
	packet.u.LocalWar.EventLocalWarMessage.MaxLevel=m_tData.EventUserLevelMax[m_EventStep];
	packet.u.LocalWar.EventLocalWarMessage.EventJoinLocation[0].x=m_tData.EventJoinLocation[0].x;
	packet.u.LocalWar.EventLocalWarMessage.EventJoinLocation[0].y=m_tData.EventJoinLocation[0].y;
	packet.u.LocalWar.EventLocalWarMessage.EventJoinLocation[1].x=m_tData.EventJoinLocation[1].x;
	packet.u.LocalWar.EventLocalWarMessage.EventJoinLocation[1].y=m_tData.EventJoinLocation[1].y;
	packet.u.LocalWar.EventLocalWarMessage.EventJoinLocation[2].x=m_tData.EventJoinLocation[2].x;
	packet.u.LocalWar.EventLocalWarMessage.EventJoinLocation[2].y=m_tData.EventJoinLocation[2].y;
	packet.h.header.size=sizeof(t_EventLocalWarMessage);
	g_pUserManager->SendPacket(&packet); // CSD-CN-031213
	SendPacket2Maps(&packet);

	MyLog(0,"Step : %d, Status : %d",m_EventStep,m_EventStatus);

}

void cEventLocalWarfield::SendEvent1Message(int Status)
{
	switch(Status)
	{
	case EVENT_NOTHING	: break;
	case EVENT_JOIN		: break;
	case EVENT_MOVE		: break;
	case EVENT_DOING	: break;
	case EVENT_REST		: break;
	}
}

void cEventLocalWarfield::SendEvent2Message(int Status)
{
	switch(Status)
	{
	case EVENT_JOIN		: break;
	case EVENT_MOVE		: break;
	case EVENT_DOING	: break;
	case EVENT_REST		: break;
	}
}

void cEventLocalWarfield::SendEvent3Message(int Status)
{
	switch(Status)
	{
	case EVENT_JOIN		: break;
	case EVENT_MOVE		: break;
	case EVENT_DOING	: break;
	case EVENT_REST		: break;
	}
}

int	 cEventLocalWarfield::GetNationIndex(int Nation)
{
	switch (Nation)
	{
	case NW_BY : return 0;
	case NW_ZY : return 1;
	case NW_YL : return 2;
	}
	return 0;
}

void cEventLocalWarfield::ClearUserALL()
{	//< CSD-CN-031213
	CUserManager::HASH_USER user = g_pUserManager->GetUserSet();

	for (CUserManager::ITOR_USER i = user.begin(); i != user.end(); ++i)
	{
		const int idUser = i->second;
		CHARLIST* pUser = ::CheckServerId(idUser);
		
		if (pUser != NULL)
		{
			switch (GetNationIndex(pUser->name_status.nation))
			{
			case 0: 
				{
					::MapMove(idUser, "MA-IN", m_tData.OutPoint[0].x, m_tData.OutPoint[0].y);
					break;
				}
			case 1: 
				{
					::MapMove(idUser, "RENES_C", m_tData.OutPoint[1].x, m_tData.OutPoint[1].y);
					break;
				}
			case 2: 
				{
					::MapMove(idUser, "BARANTAN", m_tData.OutPoint[2].x, m_tData.OutPoint[2].y);
					break;
				}
			}
		}
	}
}	//> CSD-CN-031213

inline bool cEventLocalWarfield::CheckLevel(int Level)
{
	if (Level>=m_tData.EventUserLevelMin[m_EventStep]&&Level<=m_tData.EventUserLevelMax[m_EventStep]) return true;
	return false;
}

inline bool cEventLocalWarfield::IncManCount(int Nation)
{
	int NationIndex=GetNationIndex(Nation);

	if (m_ManCount[0]>=m_tData.EventRegMax[m_EventStep][0]) return false;
	m_ManCount[0]++;
	MyLog(0,"참여 예약 인원 변경 : %d",m_ManCount[0]);
	return true;
}
/*
void RecvCMD_EVENT_LOCALWAR_CONTROL(t_packet *p, t_connection c[], int cn )
{
	if (g_isEventLocalWarServer)
	{
		g_pEventLocalWarfield->SetEventLocalWarStatus(p->u.LocalWar.CommonDataC.Data);
		g_EventLocalWarBegin=p->u.LocalWar.CommonDataC.Data;
	}
	else
	{
		p->h.header.type=CMD_EVENT_LOCALWAR_CONTROL_D;
		SendPacket2NWMap(g_EventMapPort,p);
	}
}

void RecvCMD_EVENT_LOCALWAR_CONTROL_D(t_packet *p, t_connection c[], int cn )
{
	if (g_isEventLocalWarServer)
	{
		g_pEventLocalWarfield->SetEventLocalWarStatus(p->u.LocalWar.CommonDataC.Data);
		g_EventLocalWarBegin=p->u.LocalWar.CommonDataC.Data;
	}
}*/
/*
void RecvCMD_EVENT_LOCALWAR_STATUS_CHANGE(t_packet *p, t_connection c[], int cn )
{
	g_EventLocalWarBegin=p->u.LocalWar.CommonDataC.Data;
	SendPacket2Clients(p);
}*/

void RecvCMD_EVENT_LOCALWAR_JOIN(t_packet *p, t_connection c[], int cn )
{
	t_packet packet;

	if (c[cn].chrlst.EventLocalWarJoin) return;

	if (CheckEventJoin(&connections[cn].chrlst,6))			// 다음엔 여기만 무조건 참으로 해주면 이벤트를 할수 있다.
	{
		packet.h.header.type=CMD_EVENT_LOCALWAR_JOIN;
		packet.u.LocalWar.EventJoin.Nation=c[cn].chrlst.name_status.nation;
		packet.u.LocalWar.EventJoin.Level=c[cn].chrlst.GetLevel(); // CSD-030806
		packet.h.header.size=sizeof(t_EventJoin);
		SendCMD_REQUEST_LOCAL_DELIVERY(g_EventMapPort,&packet,c,cn);
	}
	else
	{
		packet.h.header.type=CMD_EVENT_LOCALWAR_JOIN_RESULT;	//홈페이지에서 참여신청을 하지 않았다.
		packet.u.LocalWar.CommonDataC.Data=0;		// 0 : 실패 1 : 성공 2 : 레벨실패 3 : 인원실패
		packet.h.header.size=sizeof(t_CommonDataC);
		QueuePacket(c,cn,&packet,1);
	}
}

void RecvCMD_EVENT_LOCALWAR_MESSAGE(t_packet *p, t_connection c[], int cn )
{
	g_EventLocalWarStatus=p->u.LocalWar.EventLocalWarMessage.EventStatus;
	g_EventLocalWarStep=p->u.LocalWar.EventLocalWarMessage.EventStep;
	g_pUserManager->SendPacket(p); // CSD-CN-031213
}

void RecvCMD_EVENT_LOCALWAR_MOVE(t_packet *p, t_connection c[], int cn ) 
{
	if (c[cn].chrlst.EventLocalWarJoin)
	{
		c[cn].chrlst.EventPoint=0;		//이벤트 포인트를 초기화 한다.
		int temp=rand()%6;
		MapMove(cn,"EventLW",g_StartPoint[temp].x,g_StartPoint[temp].y);
	}
	else
	{
		t_packet packet;
		packet.h.header.type=CMD_EVENT_LOCALWAR_MOVE_RESULT;
		packet.u.LocalWar.CommonDataC.Data=0;
		packet.h.header.size=sizeof(t_CommonDataC);
		QueuePacket(c,cn,&packet,1);
	}
}

void RecvCMD_AI_DATA_RELOAD()			// LTS AI2 TEMP
{	//< CSD-030417
	if (!g_pAIManager->Load_AI_Data())
	{
		JustMsg("AI_Data Loading Fault!!");
	}
	else 
	{
		MyLog(0,"AI Data Loaded Complete!!");
	}
}	//> CSD-030417

extern void ReloadEventMonsterData();
extern void ReloadAIData();
extern void __stdcall ReLoadGameServerDataByKeyInput(DWORD dwValue);//020511 lsw;

void RecvCMD_EVENT_MONSTER(t_packet *p, t_connection c[], int cn )		// LTS AI2 TEMP
{
	ReloadEventMonsterData();
	ReloadAIData();
	ReLoadGameServerDataByKeyInput(0);//020511 lsw
}

int CheckSendAction(int cn,int Action)		// LTS ACTION
{	//< CSD-030806
	switch(Action)
	{
	case 24 :	// ACTION ANGER
		if (connections[cn].chrlst.GetLevel()>=61) return true;
		else return false;
		break;
	case 25 :	// ACTION VICTORY
		if (connections[cn].chrlst.GetLevel()>=41) return true;
		else return false;
		break;
	}
	return true;
}	//> CSD-030806

void RecvCMD_COMMUNITY_ACTION( t_packet *p, t_connection c[], int cn )		// LTS ACTION
{
	t_packet packet;
	if (CheckSendAction(cn,p->u.LocalWar.CommunityActionS.Action))		// 퀘스트 수행했나 검사
	{
		packet.h.header.type=CMD_COMMUNITY_ACTION;
		packet.u.LocalWar.CommunityAction.ID=cn;
		packet.u.LocalWar.CommunityAction.Action=p->u.LocalWar.CommunityActionS.Action;
		packet.u.LocalWar.CommunityAction.Direction=p->u.LocalWar.CommunityActionS.Direction;
		packet.u.LocalWar.CommunityAction.Result=TRUE;
		packet.h.header.size=sizeof(t_CommunityAction);
		QueuePacket(c,cn,&packet,1);
		CastMe2Other(cn,&packet);
}
	else
	{
		packet.h.header.type=CMD_COMMUNITY_ACTION;
		packet.u.LocalWar.CommunityAction.ID=cn;
		packet.u.LocalWar.CommunityAction.Action=p->u.LocalWar.CommunityActionS.Action;
		packet.u.LocalWar.CommunityAction.Direction=p->u.LocalWar.CommunityActionS.Direction;
		packet.u.LocalWar.CommunityAction.Result=FALSE;
		packet.h.header.size=sizeof(t_CommunityAction);
		QueuePacket(c,cn,&packet,1);
	}
}

void CheckAndFreeLocalWar(int NationIndex,int Result)		// LTS NEW LOCALWAR
{	//< CSD-CN-031213
	if (Result) 
	{
		return;
	}
	// 해당맵서버의 국지전 참여 유저들중 패배한 유저들을 초기화 한다.
	const int Nation = GetNationFromIndex(NationIndex);

	CUserManager::HASH_USER user = g_pUserManager->GetUserSet();

	for (CUserManager::ITOR_USER i = user.begin(); i != user.end(); ++i)
	{
		const int idUser = i->second;
		CHARLIST* pUser = CheckServerId(idUser);
		
		if (pUser != NULL && pUser->name_status.nation == Nation && pUser->JoinLocalWar)
		{
			pUser->JoinLocalWar = 0;
			// 복장 변경
			CheckAndSendChangeEquip(pUser, 1);  
			// 적국인 경우 자기 국가로 이동
			LPMAPINFO pMapInfo = GetMapInfoByMapPort(g_MapPort);
	
			if (!pMapInfo)
			{
				continue;
			}

			if (pMapInfo->nation == 0 || pMapInfo->nation == pUser->name_status.nation)
			{
				continue;
			}
			
			switch (pUser->name_status.nation)
			{
			case NW_BY:	
				{
					::MapMove(idUser, "MA-IN", 245, 115);   
					break;
				}
			case NW_ZY:	
				{
					::MapMove(idUser, "RENES_C", 210, 175); 
					break;
				}
			case NW_YL:	
				{
					::MapMove(idUser, "BARANTAN", 325, 98); 
					break;
				}
			}
		}
	}
}	//> CSD-CN-031213

void RecvCMD_LOCALWAR_NATION_RESULT(t_packet *p, t_connection c[], int cn )		// LTS NEW LOCALWAR
{
	int NationIndex=p->u.LocalWar.LocalWarResult.NationIndex;
	int Result=p->u.LocalWar.LocalWarResult.Result;
	if (NationIndex<0||NationIndex>3) return;
	g_LocalWarResult[NationIndex]=Result;
	
	CheckAndFreeLocalWar(NationIndex,Result);
	g_pUserManager->SendPacket(p); // CSD-CN-031213	
}

void RecvCMD_LOCALWAR_MAPMOVE (t_packet *p, t_connection c[], int cn )	// LTS NEW LOCALWAR
{
	const int	Possession[7]={0,3,4,6,3,4,6};		//맵의 소유 
	const char  MapName[7][MAX_PATH]={ "BIG_G","HU_VM","RENES_OUT","BIG_TREE","SOURCE","COLOR","SEDLESS" };
	int			MoveMap=p->u.LocalWar.CommonDataC.Data;
	char		tempMapName[MAX_PATH];
	int			NationIndex=GetNationIndex(c[cn].chrlst.name_status.nation);
	t_packet	packet;

	if (c[cn].chrlst.name_status.nation!=Possession[MoveMap])	// 선택된 맵이 국적이 다르면
	{
		if (Possession[MoveMap]==0)								// 중립맵일때만..
		{
			if (c[cn].chrlst.JoinLocalWar)
			{
				strcpy_s(tempMapName, sizeof(tempMapName),MapName[MoveMap]);
				MapMove(cn,tempMapName,g_LocalWarMovePoint[MoveMap][NationIndex].x,g_LocalWarMovePoint[MoveMap][NationIndex].y);
			}
			else
			{
				packet.h.header.type=CMD_LOCALWAR_MAPMOVE_RESULT;
				packet.u.LocalWar.CommonDataC.Data=1;
				packet.h.header.size=sizeof(t_CommonDataC);
				QueuePacket(c,cn,&packet,1);
			}
		}
		else
		{
			packet.h.header.type=CMD_LOCALWAR_MAPMOVE_RESULT;
			packet.u.LocalWar.CommonDataC.Data=2;
			packet.h.header.size=sizeof(t_CommonDataC);
			QueuePacket(c,cn,&packet,1);
			
		}
	}
	else														// 같은 국적의 맵이면
	{
		if (c[cn].chrlst.JoinLocalWar)
		{
			strcpy_s(tempMapName, sizeof(tempMapName),MapName[MoveMap]);
			MapMove(cn,tempMapName,g_LocalWarMovePoint[MoveMap][NationIndex].x,g_LocalWarMovePoint[MoveMap][NationIndex].y);
		}
		else
		{
			packet.h.header.type=CMD_LOCALWAR_MAPMOVE_RESULT;
			packet.u.LocalWar.CommonDataC.Data=1;
			packet.h.header.size=sizeof(t_CommonDataC);
			QueuePacket(c,cn,&packet,1);
		}
	}
}

int GetRider(CHARLIST* ch)
{	//< CSD-030806
	switch (ch->HorseNo)
	{
	case 1:
		{
			if (ch->GetLevel()>=51&&ch->GetLevel()<61)
			{
				return 1;
			}
			else if (ch->GetLevel()>=61&&ch->GetLevel()<71)
			{
				return 2;
			}
			else if (ch->GetLevel()>=71)
			{
				return 3;
			}

			break;
		}
	case 2:
		{
			if (ch->GetLevel()>=76&&ch->GetLevel()<86)
			{
				return 1;
			}
			else if (ch->GetLevel()>=86&&ch->GetLevel()<96)
			{
				return 2;
			}
			else if (ch->GetLevel()>=96)
			{
				return 3;
			}

			break;
		}
	case 3:
		{
			if (ch->GetLevel()>=100&&ch->GetLevel()<120)
			{
				return 1;
			}
			else if (ch->GetLevel()>=120&&ch->GetLevel()<160)
			{
				return 2;
			}
			else if (ch->GetLevel()>=160)
			{
				return 3;
			}

			break;
		}
	}

	return 1;
}	//> CSD-030806

int GetRiderGrade(CHARLIST* ch)
{
	if (ch->fame>=1800&&ch->fame<2200)
	{
		return 1;
	}
	else
	if (ch->fame>=2200)
	{
		return 2;
	}
	return 0;
}

int GetHorseGrade(CHARLIST* ch)
{
	if (ch->fame>=1700&&ch->fame<2100)
	{
		return 1;
	}
	else
	if (ch->fame>=2100)
	{
		return 2;
	}
	return 0;
}

void RecvCMD_LOCALWAR_CHAT(t_packet *p, t_connection c[], int cn )	// LTS NEW LOCALWAR
{	//< CSD-CN-031213
	if (g_LocalWarBegin)	// 국지전이 시작되었고
	{
		CHARLIST *pCaster = CheckServerId(cn);

		if (pCaster == NULL)
		{
			return;
		}
		//if(localWarMsgDebug < 2)
			//MyLog(0, "Local War Message - '%s': '%s', MapNumber: %d", pCaster->Name, p->u.LocalWar.LocalWarChat.Message, MapNumber); //Eleval to debug Local War Chat

		if (g_LocalWarResult[GetNationIndex(pCaster->name_status.nation)])	
		{	// 해당국가가 패배하지 않았다면..
			CUserManager::HASH_USER user = g_pUserManager->GetUserSet();

			for (CUserManager::ITOR_USER i = user.begin(); i != user.end(); ++i)
			{
				const int idUser = i->second;
				CHARLIST* pUser = CheckServerId(idUser);
		
				if (pUser != NULL)
				{
					if	(pUser->JoinLocalWar &&	pUser->name_status.nation == pCaster->name_status.nation)
					{
						//if(localWarMsgDebug < 2)
							//MyLog(0, "Local War Message Sent to '%s'", pUser->Name); //Eleval to debug Local War Chat
						QueuePacket(c, idUser, p, 1);
					}
				}
			}
		}

		p->h.header.type = CMD_LOCALWAR_CHAT_B;
		SendPacket2Maps(p);
	}
}	//> CSD-CN-031213

void RecvCMD_LOCALWAR_CHAT_B(t_packet *p, t_connection c[], int cn )
{	//< CSD-CN-031213
	if (g_LocalWarBegin)
	{
		const int Nation = GetNationFromIndex(p->u.LocalWar.LocalWarChat.NationIndex);
		p->h.header.type = CMD_LOCALWAR_CHAT;
		//if(localWarMsgDebug < 2)
			//MyLog(0, "Local War Message From Other Map - '%s', MapNumber: %d from cn: %d", p->u.LocalWar.LocalWarChat.Message, MapNumber, cn); //Eleval to debug Local War Chat

		CUserManager::HASH_USER user = g_pUserManager->GetUserSet();

		for (CUserManager::ITOR_USER i = user.begin(); i != user.end(); ++i)
		{
			const int idUser = i->second;
			CHARLIST* pUser = CheckServerId(idUser);
	
			if (pUser != NULL)
			{
				if	(pUser->JoinLocalWar &&	pUser->name_status.nation == Nation)
				{
					//if(localWarMsgDebug < 2)
						//MyLog(0, "Local War Message From Other Map Sent to '%s'", pUser->Name); //Eleval to debug Local War Chat
					QueuePacket(c, idUser, p, 1);
				}
			}
		}
	}
}	//> CSD-CN-031213

void RecvCMD_LOCALWAR_DEFENCE_POINT(t_packet *p, t_connection c[], int cn )		// LTS NEW LOCALWAR
{	//< CSD-CN-031213

	//<! BBD 040401
	// 전역변수에 방어력을 받아둔다.
	for(int i = 0; i < NW_NATION_COUNT; i++)
	{
		g_DefensePoint[i] = p->u.LocalWar.LocalWarDefencePoint.Point[i];
	}
	//> BBD 040401

	g_pUserManager->SendPacket(p);
}	//> CSD-CN-031213

void RecvCMD_SET_QUESTMODE( t_packet *p, t_connection c[], int cn )				// LTS DRAGON MODIFY
{
	CHARLIST *ch = ::CheckServerId(cn);//030127 lsw
	if(!ch){return;}
	ch->QuestMode = p->u.LocalWar.CommonDataC.Data;
	MyLog(0,"%d User Set Quest Mode : %d",ch->GetServerID(),ch->QuestMode);
}

void CheckDragonLordWarTeam(CHARLIST *a,CHARLIST *d)			// LTS DRAGONLORD
{
	if (d->DragonLordWarTeam<=0) return;

	g_pDragonLordWar.FindAndDeleteMember(d->GetServerID());
	
}

cDragonLordWar::cDragonLordWar()
{
	m_bisActive=false;
	m_64Timer=0;
	TeamCount[0]=0;
	TeamCount[1]=0;
	SetWarResult(WAR_NOT_END);
}

cDragonLordWar::~cDragonLordWar()
{
	ClearMemberList();
}

void cDragonLordWar::ClearMemberList()
{
	MemberListItor itor;
	// BLUETEAM
	for (itor=TeamMemberList[BLUETEAM].begin();itor!=TeamMemberList[BLUETEAM].end();)
	{
		itor=TeamMemberList[BLUETEAM].erase(itor);//030127 lsw
	}
	//REDTEAM
	for (itor=TeamMemberList[REDTEAM].begin();itor!=TeamMemberList[REDTEAM].end();)
	{
		itor=TeamMemberList[REDTEAM].erase(itor);//030127 lsw
	}
}

void cDragonLordWar::SetWarResult(char Result)
{
	m_cResult=Result;
}

int cDragonLordWar::GetWarResult()
{
	return m_cResult;
}


void cDragonLordWar::ClearMemberCount()
{
	TeamCount[BLUETEAM]=0;
	TeamCount[REDTEAM]=0;
}

void cDragonLordWar::SetTimer(DWORD WarTime)
{
	m_64Timer=g_curr_time+WarTime;
}

DWORD cDragonLordWar::GetRemainTime()
{
	return (DWORD)(m_64Timer-g_curr_time);

}

void cDragonLordWar::SetMessageTime(DWORD MsgTime)
{
	MessageTime=MsgTime;
}

void cDragonLordWar::SetReturnFunc(void(*pFunc)())
{
	DeActiveFunc=pFunc;
}

void cDragonLordWar::SetMessageFunc(void(*pMsgFunc)())
{
	MessageFunc=pMsgFunc;
}

void cDragonLordWar::CheckMemberCount()
{
	if (TeamCount[BLUETEAM]==0&&TeamCount[REDTEAM]==0)	// ALL LOOSER
	{
		SetWarResult(WAR_RESULT_LOOSE_ALL);
	}
	if (TeamCount[BLUETEAM]==0)								// RED WIN
	{
		SetWarResult(WAR_RESULT_RED_WIN);
	}
	if (TeamCount[REDTEAM]==0)								// BLUE WIN
	{
		SetWarResult(WAR_RESULT_BLUE_WIN);						
	}

	switch (m_cResult)
	{
	case WAR_NOT_END :				return;
	case WAR_RESULT_LOOSE_ALL	:
	case WAR_RESULT_RED_WIN		:
	case WAR_RESULT_BLUE_WIN	:	DeActiveWar(); break;
	}
}

void cDragonLordWar::SetKickPoint(POINT* kPoint)
{
	KickPoint[0]=kPoint[0];
	KickPoint[1]=kPoint[1];
	KickPoint[2]=kPoint[2];
}

void cDragonLordWar::ActiveWar(DWORD WarTime,void(*pFunc)(),DWORD MessageTime,void(*pMsgFunc)(),POINT* kPoint,int RedTeamCount,int* RedTeam,int BlueTeamCount,int* BlueTeam)
{
	ClearMemberCount();
	ClearMemberList();
	SetTeamList(REDTEAM,RedTeamCount,RedTeam);
	SetTeamList(BLUETEAM,BlueTeamCount,BlueTeam);
	SetReturnFunc(pFunc);
	SetMessageFunc(pMsgFunc);
	PutArmor();
	SetTimer(WarTime);
	SetMessageTime(MessageTime);
	SetKickPoint(kPoint);
	SetWarResult(WAR_NOT_END);
	m_bisActive=true;
}

void cDragonLordWar::DeActiveWar()
{
	m_bisActive=false;
	SetTimer(0);
	RemoveArmor();
	if (m_cResult==WAR_RESULT_LOOSE_ALL) KickTeamAll();
	DeActiveFunc();
}

void cDragonLordWar::PushMember(int Type,int ID)
{
	if (Type!=REDTEAM&&Type!=BLUETEAM) return;

	CHARLIST *pCh = CheckServerId(ID);
	if(!pCh)
	{
		return;
	}
	pCh->DragonLordWarTeam=Type;
	TeamMemberList[Type].push_back(ID);//030127 lsw
}

void cDragonLordWar::FindAndDeleteMember(int ID,bool Kick)
{
	MemberListItor itor;

	// BLUETEAM
	if (!TeamMemberList[BLUETEAM].empty())
	{
		for (itor=TeamMemberList[BLUETEAM].begin();itor!=TeamMemberList[BLUETEAM].end();itor++)
		{
			if (ID == (*itor))
			{
				MyLog(0,"Dragon Lord War BLUE TEAM Defeat User : %s",connections[ID].name);
				itor=TeamMemberList[BLUETEAM].erase(itor);
				TeamCount[BLUETEAM]--;
				connections[ID].chrlst.DragonLordWarTeam=0;				// 초기화
				if (Kick) KickUser(ID);									// MAP MOVE		// 맵무브할때도 호출한다.
				return;
			}
		}
	}
	//REDTEAM
	if (!TeamMemberList[REDTEAM].empty())
	{
		for (itor=TeamMemberList[REDTEAM].begin();itor!=TeamMemberList[REDTEAM].end();itor++)
		{
			if (ID == (*itor))
			{
				MyLog(0,"Dragon Lord War RED TEAM Defeat User : %s",connections[ID].name);
				itor=TeamMemberList[REDTEAM].erase(itor);
				TeamCount[REDTEAM]--;
				connections[ID].chrlst.DragonLordWarTeam=0;				// 초기화
				if (Kick) KickUser(ID);									// MAP MOVE		// 맵무브할때도 호출한다.
				return;
			}
		}
	}
	MyLog(0,"DRAGON LOAD WAR MEMBER DELETE ERROR : CAN NOT FOUND MEMBER!! MEMBERNAME : %s",connections[ID].name);
}

void cDragonLordWar::SetTeamList(int Type,int Count,int* Member)
{
	if (Type!=REDTEAM&&Type!=BLUETEAM) return;
	if (Count<=0||Count>=DRAGON_MAX_CONNECTIONS) return;

	TeamCount[Type]=Count;
	for (int i=0;i<Count;i++)
	{
		PushMember(Type,Member[i]);
	}
}

bool cDragonLordWar::GetWarStatus()
{
	return m_bisActive;
}

int cDragonLordWar::GetTeamMemberCount(int Type)
{
	if (Type!=REDTEAM&&Type!=BLUETEAM) return 0;
	return TeamCount[Type];
}

int cDragonLordWar::ReturnWinMemberCount()
{
	if (GetWarStatus()) return 0;				// Not Finished..
	if (GetWarResult()==WAR_RESULT_LOOSE_ALL) return 0;	// ALL LOOSER

	return GetTeamMemberCount(m_cResult);
}

void cDragonLordWar::ReturnWinMemerList(int* Member)
{
	if (GetWarStatus())	{	
		Member=NULL;
		return;				// Not Finished..
	}
	if (GetWarResult()==WAR_RESULT_LOOSE_ALL) {
		Member=NULL;
		return;	// ALL LOOSER
	}

	int Count=0;
	
	MemberListItor itor;

	for (itor=TeamMemberList[GetWarResult()].begin();itor!=TeamMemberList[GetWarResult()].end();itor++)
	{
		if (Count>=ReturnWinMemberCount()) 
		{
			MyLog(0,"Dragon Lord War Getting Win Member List Error : Member List Count Bigger than MemberCount");
			return;
		}
		Member[Count]=(*itor);//030127 lsw
		Count++;
	}
}

void cDragonLordWar::CheckRemainTime()
{
	static int CallMessageFunc=false;
	DWORD RemainTime=m_64Timer-g_curr_time;
	DWORD ModeTime=RemainTime%MessageTime;
	if (ModeTime<(MessageTime/2))
	{
		if (!CallMessageFunc)
		{
			CallMessageFunc=true;
			MessageFunc();
		}
	}
	else
	{
		CallMessageFunc=false;
	}
}

void cDragonLordWar::UpdateWarTimer()
{
	if (!GetWarStatus()) return;				// Not Start
	if (g_curr_time<m_64Timer)  {
		CheckMemberCount();						// Check MemberCount	maybe Call DeActive
		CheckRemainTime();
		return;									// Not On Time Yet
	}
	SetWarResult(WAR_RESULT_LOOSE_ALL);
	DeActiveWar();								// Just Loose
}

void cDragonLordWar::KickUser(int ID)
{
	CHARLIST *ch=&connections[ID].chrlst;
	aliveCharacter(ch);

	t_packet packet;
	packet.h.header.type = CMD_PLEASE_GIVE_LIFE;
	packet.h.header.size = sizeof( t_please_give_life );
	packet.u.please_give_life.id = ch->GetServerID();
	packet.u.please_give_life.who = -1;
	packet.u.please_give_life.nLife	= ch->Hp;
	packet.u.please_give_life.nMana	= ch->Mana;
	packet.u.please_give_life.nHungry	= ch->Hungry;
	QueuePacket( connections, ch->GetServerID(), &packet,1 );
	CastMe2Other( ch->GetServerID(), &packet );

	SendTeamChange(ID,NONE_ARMOR);
	CheckAndSendChangeEquip(ch,1);

	switch(ch->name_status.nation)
	{
	case NW_BY : MapMove(ID,"HEGMONIA",KickPoint[0].x,KickPoint[0].y); break;
	case NW_ZY : MapMove(ID,"HEGMONIA",KickPoint[1].x,KickPoint[1].y); break;
	case NW_YL : MapMove(ID,"HEGMONIA",KickPoint[2].x,KickPoint[2].y); break;
	}
	
	if( g_pDungeonMap ) g_pDungeonMap->CloseConnect( ID );		// 리스트에서 삭제
}

void cDragonLordWar::KickTeam(int Type)
{
	for (MemberListItor itor=TeamMemberList[Type].begin();itor!=TeamMemberList[Type].end();itor++)
	{
		KickUser((*itor));// MAP MOVE//030127 lsw
	}
}

void cDragonLordWar::KickTeamAll()
{
	KickTeam(BLUETEAM);
	KickTeam(REDTEAM);
}

void cDragonLordWar::SendTeamChange(int ID,int TeamNo)
{
	t_packet packet;

	packet.h.header.type=CMD_DRAGONLORDWAR_TEAM_CHANGE;
	packet.u.LocalWar.LordWarTeamChange.ID=ID;
	packet.u.LocalWar.LordWarTeamChange.TeamNo=TeamNo;
	packet.h.header.size=sizeof(t_LordWarTeamChange);

	QueuePacket(connections,ID,&packet,1);
	CastMe2Other(ID,&packet);
}

void cDragonLordWar::PutArmor()
{
	MemberListItor itor;
	for (itor=TeamMemberList[BLUETEAM].begin();itor!=TeamMemberList[BLUETEAM].end();itor++)
	{
		CHARLIST *pCh = CheckServerId((*itor));//030127 lsw
		if(!pCh)
		{
			continue;
		}
		pCh->DragonLordWarTeam=BLUE_ARMOR;
		SendTeamChange(ID,pCh->DragonLordWarTeam);
		CheckAndSendChangeEquip(pCh,1);
	}
	for (itor=TeamMemberList[REDTEAM].begin();itor!=TeamMemberList[REDTEAM].end();itor++)
	{
		CHARLIST *pCh = CheckServerId((*itor));//030127 lsw
		if(!pCh)
		{
			continue;
		}
		pCh->DragonLordWarTeam=RED_ARMOR;
		SendTeamChange(ID,pCh->DragonLordWarTeam);
		CheckAndSendChangeEquip(pCh,1);
	}
}

void cDragonLordWar::RemoveArmor()
{
	MemberListItor itor;
	for (itor=TeamMemberList[BLUETEAM].begin();itor!=TeamMemberList[BLUETEAM].end();itor++)
	{
		CHARLIST *pCh = CheckServerId((*itor));
		if(!pCh)
		{
			continue;
		}
		pCh->DragonLordWarTeam=NONE_ARMOR;
		SendTeamChange(ID,pCh->DragonLordWarTeam);
		CheckAndSendChangeEquip(pCh,1);//030127 lsw
	}
	for (itor=TeamMemberList[REDTEAM].begin();itor!=TeamMemberList[REDTEAM].end();itor++)
	{
		CHARLIST *pCh = CheckServerId((*itor));
		if(!pCh)
		{
			continue;
		}
		pCh->DragonLordWarTeam=NONE_ARMOR;
		SendTeamChange(ID,pCh->DragonLordWarTeam);
		CheckAndSendChangeEquip(pCh,1);//030127 lsw
	}
}

void CheckMapMove(int ID)
{
	if (connections[ID].chrlst.DragonLordWarTeam>0)					// LTS DRAGONLORDWAR
	{
		g_pDragonLordWar.FindAndDeleteMember(ID,FALSE);
	}
}

void UpdateDragonLordWarTime()
{
	g_pDragonLordWar.UpdateWarTimer();
}

void EndWar()
{
	MyLog(0,"DragonLord War Ended....!!");
}

void MsgFunc()
{
	MyLog(0,"MessageFunc....!!");
}

void RecvCMD_ACTIVE_DRAGONLORD_WAR(t_packet *p, t_connection c[], int cn )
{
	POINT MovePoint[3] = { {100,100}, {100,100},{100,100} };
	int	  RedTeam=2;
	int   BlueTeam=3;
	g_pDragonLordWar.ActiveWar(300,EndWar,10,MsgFunc,MovePoint,1,&RedTeam,1,&BlueTeam);
	MyLog(0,"DragonLord War Started....!!");
}
  
void RecvCMD_REQUEST_CHECK_DUAL_FAME(t_packet *p, t_connection c[], int cn )    // LTS DUAL FAME
{
	t_packet packet;

	packet.h.header.type=CMD_ANSWER_CHECK_DUAL_FAME;
	packet.u.LocalWar.CommonDataDw.Data=var[c[cn].chrlst.GetServerID()][DUAL_FAME_FIELD];
	packet.h.header.size=sizeof(t_CommonDataDw);

	QueuePacket(c,cn,&packet,1);

}

void SaveChangeFame(CHARLIST* pTarget, int old_fame, int new_fame, eLF_TYPE type)
{	//< CSD-040224
	string strPath;

	if (!g_pLogManager->GetLogPath(LT_CHANGE, strPath))
	{
		return;
	}

	FILE* fp = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");

	if(fp == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, pTarget->Name);
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (fp == NULL)
	{
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pTarget->GetServerID()].id), "at+");

		if (fp == NULL)
		{
			return;
		}
	}
	
	char why[MAX_PATH] = {0,};

	switch (type)
	{
	case LF_DUAL: 
		{	// 듀얼 소비
			strcpy_s(why, sizeof(why), "upgrade dual");
			break;
		}
	case LF_LOCALWAR:
		{	// 국지전
			strcpy_s(why, sizeof(why), "LocalWar");
			break;
		}
	case LF_ITEMBUY:
		{	// 국가 고급 아이템
			strcpy_s(why, sizeof(why), "Nation Item Buy");
			break;
		}
	case LF_DONATION:
		{	// 국가 기부
			strcpy_s(why, sizeof(why), "Nation Doantion");
			break;
		}
	case LF_NATIONWAR:
		{	// 국가전
			strcpy_s(why, sizeof(why), "Nation War");
			break;
		}
	case LF_SCRIPT:
		{	// 스크립트 펑션
			strcpy_s(why, sizeof(why), "Script Func");
			break;
		}
	default:
		{
			strcpy_s(why, sizeof(why), "default");
			break;
		}
	}

	::fprintf(fp, "@ChangeFame	why=%s %02d-%02d-%02d %02d:%02d:%02d	(x:%d y:%d)	(old)%d	(new)%d\n",
			  why, 
			  g_year-2000, g_mon + 1, g_day,
			  g_hour, g_min, g_sec,
			  pTarget->X, pTarget->Y, old_fame, new_fame);
	::fclose( fp );
}	//> CSD-040224

void SaveChangeClassStep(CHARLIST* pTarget, int nPrevStep, int nNextStep)
{	//< CSD-TW-030622
	string strPath;

	if (!g_pLogManager->GetLogPath(LT_CHANGE, strPath))
	{
		return;
	}
	//< CSD-040224
	FILE* fp = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");

	if(fp == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, pTarget->Name);
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (fp == NULL)
	{
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pTarget->GetServerID()].id), "at+");

		if (fp == NULL)
		{
			return;
		}
	}
	//> CSD-040224
	::fprintf(fp, "@ChangeStep	%02d-%02d-%02d %02d:%02d:%02d	(old)%d	(new)%d\n",
		      g_year - 2000, g_mon + 1, g_day, g_hour, g_min, g_sec,
			  nPrevStep, nNextStep);
	::fclose(fp);
}	//> CSD-TW-030622

void SaveLogHackHits(CHARLIST* pTarget)
{	//< CSD-TW-030622
	string strPath;

	if (!g_pLogManager->GetLogPath(LT_HACK, strPath))
	{
		return;
	}
	//< CSD-040224
	FILE* fp = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");

	if(fp == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, pTarget->Name);
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (fp == NULL)
	{
		fp = fopen(VA("%s/%sHits.txt", strPath.c_str(), connections[pTarget->GetServerID()].id), "at+");

		if (fp == NULL)
		{
			return;
		}
	}
	//> CSD-040224
	fprintf(fp, "%04d.%02d.%02d %02d:%02d:%02d - %s	%d x %d\n", g_year, g_mon + 1, g_day, g_hour, g_min, g_sec, pTarget->MapName, pTarget->MoveSx, pTarget->MoveSy); // CSD-040202	
	::fclose(fp);
}	//> CSD-TW-030622

void SaveChangeClassQuest(CHARLIST* pTarget, const char* pMsg, int nQuestNo, int nQcount, int nStart)
{	//< CSD-TW-030701
	string strPath;

	if (!g_pLogManager->GetLogPath(LT_CHANGE, strPath))
	{
		return;
	}
	//< CSD-040224
	FILE* fp = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");

	if(fp == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, pTarget->Name);
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (fp == NULL)
	{
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pTarget->GetServerID()].id), "at+");

		if (fp == NULL)
		{
			return;
		}
	}
	//> CSD-040224	
	::fprintf(fp, "@ChangeQuest[%d]	%02d-%02d-%02d %02d:%02d:%02d	Msg=%s, Qcount=%d, Start=%d\n",
			  nQuestNo,       
		      g_year - 2000, g_mon + 1, g_day, g_hour, g_min, g_sec,
			  pMsg, nQcount, nStart);
	::fclose(fp);
}	//> CSD-TW-030701

void SaveChangeDualFame(CHARLIST* pTarget, const int nOldDualFame, const int nNewDualFame, eLDF_TYPE type)
{	//< CSD-040224
	string strPath;

	if (!g_pLogManager->GetLogPath(LT_CHANGE, strPath))
	{
		return;
	}
	
	FILE* fp = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");

	if(fp == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, pTarget->Name);
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (fp == NULL)
	{
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pTarget->GetServerID()].id), "at+");

		if (fp == NULL)
		{
			return;
		}
	}
	
	char why[MAX_PATH] = {0,};

	switch (type)
	{
	case LDF_LOCALWAR: 
		{	// 듀얼 소비
			::strcpy_s(why, sizeof(why), "LocalWar");
			break;
		}
	case LDF_QUEST:
		{	// 국지전
			::strcpy_s(why, sizeof(why), "Quest");
			break;
		}
	default:
		{
			::strcpy_s(why, sizeof(why), "default");
			break;
		}
	}

	::fprintf(fp, "@ChangeDualFame	why=%s %02d-%02d-%02d %02d:%02d:%02d	(x:%d y:%d)	(old)%d	(new)%d\n",
			  why, 
			  g_year - 2000, g_mon + 1, g_day, 
			  g_hour, g_min, g_sec,
			  pTarget->X, pTarget->Y, nOldDualFame, nNewDualFame);
	::fclose( fp );
}	//> CSD-040224

void SaveScriptLog(CHARLIST* pTarget, const int nQuestNo, const int nQuestStep, const char* msg) //kyo
{	//< CSD-040224
	string strPath;

	if (!g_pLogManager->GetLogPath(LT_SCRIPT, strPath))
	{
		return;
	}
	
	FILE* fp = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");

	if(fp == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, pTarget->Name);
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (fp == NULL)
	{
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pTarget->GetServerID()].id), "at+");

		if (fp == NULL)
		{
			return;
		}
	}
	
	::fprintf(fp, "@ScriptLog[%d]:%d %02d-%02d-%02d %02d:%02d:%02d	(x:%d y:%d)	(Msg)%s	\n",
			  nQuestNo, nQuestStep,
			  g_year - 2000, g_mon + 1, g_day, 
			  g_hour, g_min, g_sec,
			  pTarget->X, pTarget->Y, msg);
	::fclose( fp );
}	//> CSD-040224

int CheckHandleByLTSSupport( t_packet *p, t_connection c[], int cn )
{
	switch (p->h.header.type)
	{
	case CMD_CHECK_ABILITY_CHANGE_MONEY :
												RecvCMD_CHECK_ABILITY_CHANGE_MONEY(p,c,cn);
												break;
	case CMD_LOCALWAR_STATUS :
												RecvCMD_LOCALWAR_STATUS(p,c,cn);
												break;
	case CMD_REQUEST_LOCAL_DELIVERY :
												RecvCMD_REQUEST_LOCAL_DELIVERY(p,c,cn);
												break;
	case CMD_ANSWER_LOCAL_DELIVERY :
												RecvCMD_ANSWER_LOCAL_DELIVERY(p,c,cn);
												break;
	case CMD_REQUEST_LOCALWAR_INFO :
												RecvCMD_REQUEST_LOCALWAR_INFO(p,c,cn);
												break;
	case CMD_LOCALWAR_JOIN :
												RecvCMD_LOCALWAR_JOIN(p,c,cn);
												break;
	case CMD_REQUEST_COMPARE_PHASE :
												RecvCMD_REQUEST_COMPARE_PHASE(p,c,cn);
												break;
	case CMD_INC_USER_LOCALWARFIELD :
												RecvCMD_INC_USER_LOCALWARFIELD(p,c,cn);
												break;
	case CMD_DEC_USER_LOCALWARFIELD :
												RecvCMD_DEC_USER_LOCALWARFIELD(p,c,cn);
												break;
	case CMD_LOCALWAR_BEGIN :
												RecvCMD_LOCALWAR_BEGIN(p,c,cn);
												break;
	case CMD_LOCALWAR_END :
												RecvCMD_LOCALWAR_END(p,c,cn);
												break;
	case CMD_LOCALWAR_BEGIN_TIME :				
												RecvCMD_LOCALWAR_BEGIN_TIME(p,c,cn);
												break;
	case CMD_SEALSTONE_BROKED :
												RecvCMD_SEALSTONE_BROKED(p,c,cn);
												break;
	case CMD_SEALSTONE_ATTACKED :
												RecvCMD_SEALSTONE_ATTACKED(p,c,cn);
												break;
	case CMD_LOCALWAR_NATION_POINT_CHANGED :
												RecvCMD_LOCALWAR_NATION_POINT_CHANGED(p,c,cn);
												break;
	case CMD_LOCALWAR_POINT_BBS :
												RecvCMD_LOCALWAR_POINT_BBS(p,c,cn);
												break;
	case CMD_ACTION_STATUS :
												RecvCMD_ACTION_STATUS(p,c,cn);
												break;
	case CMD_SITDOWN_DIRECTION_CHANGED :
												RecvCMD_SITDOWN_DIRECTION_CHANGED(p,c,cn);
												break;
	case CMD_HERO_ACTION_CHANGED :
												RecvCMD_HERO_ACTION_CHANGED(p,c,cn);
												break;
/*	case CMD_EVENT_LOCALWAR_CONTROL :	
												RecvCMD_EVENT_LOCALWAR_CONTROL(p,c,cn);
												break;
	case CMD_EVENT_LOCALWAR_CONTROL_D :		
												RecvCMD_EVENT_LOCALWAR_CONTROL_D(p,c,cn);
												break;
	case CMD_EVENT_LOCALWAR_STATUS_CHANGE :
												RecvCMD_EVENT_LOCALWAR_STATUS_CHANGE(p,c,cn);
												break;*/
	case CMD_EVENT_LOCALWAR_JOIN :
												RecvCMD_EVENT_LOCALWAR_JOIN(p,c,cn);
												break;
	case CMD_EVENT_LOCALWAR_MESSAGE :
												RecvCMD_EVENT_LOCALWAR_MESSAGE(p,c,cn);
												break;
	case CMD_EVENT_LOCALWAR_MOVE :
												RecvCMD_EVENT_LOCALWAR_MOVE(p,c,cn);
												break;
	case CMD_AI_DATA_RELOAD :																// LTS AI2 TEMP
												RecvCMD_AI_DATA_RELOAD();
												break;
	case CMD_EVENT_MONSTER :
												RecvCMD_EVENT_MONSTER(p,c,cn);		// LTS AI2 TEMP
												break;
	case CMD_COMMUNITY_ACTION :														// LTS ACTION
												RecvCMD_COMMUNITY_ACTION(p,c,cn);
												break;
	case CMD_LOCALWAR_NATION_RESULT :
												RecvCMD_LOCALWAR_NATION_RESULT(p,c,cn);	// LTS NEW LOCALWAR
												break;
	case CMD_LOCALWAR_MAPMOVE :
												RecvCMD_LOCALWAR_MAPMOVE(p,c,cn);	// LTS NEW LOCALWAR
												break;
	case CMD_LOCALWAR_CHAT	:
												RecvCMD_LOCALWAR_CHAT(p,c,cn);		// LTS NEW LOCALWAR
												break;
	case CMD_LOCALWAR_CHAT_B :
												RecvCMD_LOCALWAR_CHAT_B(p,c,cn);		// LTS NEW LOCALWAR
												break;
	case CMD_LOCALWAR_DEFENCE_POINT :
												RecvCMD_LOCALWAR_DEFENCE_POINT(p,c,cn);		// LTS NEW LOCALWAR
												break;
	case CMD_SET_QUESTMODE :
												RecvCMD_SET_QUESTMODE(p,c,cn);			// LTS DRAGON MPDIFY
												break;	
	case CMD_ACTIVE_DRAGONLORD_WAR :
												RecvCMD_ACTIVE_DRAGONLORD_WAR(p,c,cn);	// LTS DRAGONLORD
												break;
  case CMD_REQUEST_CHECK_DUAL_FAME :                            // LTS DUAL FAME
												RecvCMD_REQUEST_CHECK_DUAL_FAME(p,c,cn);
	default : return -31;
	}
	return 1;
}

void SaveChangeBankMoney(const int idTarget, const DWORD nOldBankMoney, const DWORD nNewBankMoney ,const eBankMoneyChangeType BMCT)
{	//< CSD-040324
	string strPath;

	if (!g_pLogManager->GetLogPath(LT_CHANGE, strPath))
	{
		return;
	}

	FILE* fp = fopen(VA("%s/%s.txt", strPath.c_str(), connections[idTarget].name), "at+");

	if(fp == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, connections[idTarget].name);
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (fp == NULL)
	{
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), connections[idTarget].id), "at+");

		if (fp == NULL)
		{
			return;
		}
	}
	
	string strWhy = "";

	switch(BMCT)
	{
	case BMCT_ARENA_GAMBLE_DIVIDEND:
		{
			strWhy = "Arena Gamble Dividend";
			break;
		}
	case BMCT_ARENA_GAMBLE_BETTING:
		{
			strWhy = "Arena Gamble Betting";
			break;
		}
	case BMCT_BANK_DEPOSIT:
		{
			strWhy = "Bank Deposit";
			break;
		}
	case BMCT_BANK_DEFRAYAL:
		{
			strWhy = "Bank Gamble Defrayal";
			break;
		}
	case BMCT_AUCTION_ITEM_BUY:
		{
			strWhy = "Auction Item Buy ";
			break;
		}
	case BMCT_AUCTION_ITEM_REGISTER:
		{
			strWhy = "Auction Item Register";
			break;
		}
	case BMCT_BUY_LOTTO:
		{
			strWhy = "Buy Lotto";
			break;
		}
	case BMCT_RESET_ABILITY:
		{
			strWhy = "Reset Ability";
			break;
		}
	case BMCT_CHAR_LOGIN:
		{
			strWhy = "User Login";
			break;
		}
	case BMCT_RECV_MAP_CHANGE:
		{
			strWhy = "User MapMove";
			break;
		}
	case BMCT_RECV_CHAR_INFO_BASIC:
		{
			strWhy = "Recv CharInfo Basic";
			break;
		}
	default:
		{
			strWhy = "UnKnown";
			break;
		}
	}

	::fprintf(fp, "@ChangeBankMoney	%d-%02d-%02d %02d:%02d:%02d	(Old)%d	(New)%d %s\n",
			  g_year - 2000, g_mon + 1, g_day, 
			  g_hour, g_min, g_sec,
			  nOldBankMoney, nNewBankMoney, strWhy.c_str());
	::fclose(fp);
}	//> CSD-040324

void SaveChangeBattingMoney(CHARLIST* pTarget, DWORD dwOldMoney, DWORD dwNewMoney)
{	//< CSD-TW-030624
	string strPath;

	if (!g_pLogManager->GetLogPath(LT_CHANGE, strPath))
	{
		return;
	}
	//< CSD-040224
	FILE* fp = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");

	if(fp == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, pTarget->Name);
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (fp == NULL)
	{
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pTarget->GetServerID()].id), "at+");

		if (fp == NULL)
		{
			return;
		}
	}
	//> CSD-040224
	::fprintf(fp, "@ChangeBattingMoney	%d-%02d-%02d %02d:%02d:%02d	BankMoney (Before)%d (After)%d\n",
			  g_year - 2000, g_mon + 1, g_day, g_hour, g_min, g_sec,
			  dwOldMoney, dwNewMoney);
	::fclose( fp );
}	//> CSD-TW-030624

void SaveChangeDividendMoney(CHARLIST* pTarget, DWORD dwOldMoney, DWORD dwNewMoney, bool bBank)
{	//< CSD-TW-030624
	string strPath;

	if (!g_pLogManager->GetLogPath(LT_CHANGE, strPath))
	{
		return;
	}
	//< CSD-040224
	FILE* fp = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");

	if(fp == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, pTarget->Name);
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (fp == NULL)
	{
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pTarget->GetServerID()].id), "at+");

		if (fp == NULL)
		{
			return;
		}
	}
	//> CSD-040224
	fprintf(fp, "@ChangeDividendMoney	%d-%02d-%02d %02d:%02d:%02d	",
			  g_year-2000, g_mon+1, g_day, g_hour, g_min, g_sec);

	if (bBank)
	{
		fprintf(fp, "BankMoney (Before)%d (After)%d\n", dwOldMoney, dwNewMoney);
	}
	else
	{
		fprintf(fp, "InventoryMoney (Before)%d (After)%d\n", dwOldMoney, dwNewMoney);
	}

	::fclose( fp );
}	//> CSD-TW-030624

// 030506 YGI
void SaveChangeNationItem(CHARLIST* pTarget, int type, ItemAttr* item)
{	//< CSD-040224
	string strPath;

	if (!g_pLogManager->GetLogPath(LT_CHANGE, strPath))
	{
		return;
	}

	FILE* fp = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");

	if(fp == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, pTarget->Name);
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (fp == NULL)
	{
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pTarget->GetServerID()].id), "at+");

		if (fp == NULL)
		{
			return;
		}
	}

	CItem* t = ItemUnit(item->item_no);

	::fprintf(fp, "@ChangeNationItem	%d-%02d-%02d %02d:%02d:%02d	(Type)%s	(item:%s)%d	%d	%d	%d\n",
			  g_year-2000, g_mon+1, g_day, 
			  g_hour, g_min, g_sec, type?"FAME":"MONEY", t?t->GetHanName():"WRONG ITEM", item->item_no,
			  item->attr[0], item->attr[1], item->attr[2] );
	::fclose(fp);
}	//> CSD-040224

void SaveQuestExp(CHARLIST* pTarget, int nKind, int nParty, int nAmount, int nLoops) //Eleval 16/08/09 - Saves the Exp Won from a quest
{	//< CSD-TW-030701
	string strPath;

	if (!g_pLogManager->GetLogPath(LT_CHANGE, strPath))
	{
		return;
	}
	//< CSD-040224
	FILE* fp = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");

	if(fp == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, pTarget->Name);
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (fp == NULL)
	{
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pTarget->GetServerID()].id), "at+");

		if (fp == NULL)
		{
			return;
		}
	}
	//> CSD-040224	
	::fprintf(fp, "@ExpFromQuest(Amount: %d, Kind: %d, Party: %d, Loops: %d	[%02d-%02d-%02d %02d:%02d:%02d]\n",
			  nAmount, nKind, nParty, nLoops,      
		      g_year - 2000, g_mon + 1, g_day, g_hour, g_min, g_sec);
	::fclose(fp);
}	//> CSD-TW-030701