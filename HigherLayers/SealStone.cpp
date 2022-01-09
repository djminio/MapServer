#include "..\stdafx.h"
#include "DefaultHeader.h"

#define __SEALSTONE__
#include "SealStone.h"

#include "SealStoneGameServer.h"

#include "..\LowerLayers\servertable.h"

#include "NationSys.h"						// LTS LOCALWAR
#include "LTSSupport.h"

#include "..\LowerLayers\MyLog.h"
#include "UserManager.h"
	
extern void SendCMD_SEALSTONE_BROKED(SealStoneStatus* tp);		// LTSSupport.cpp		// LTS LOCALWAR
extern void SendCMD_SEALSTONE_ATTACKED(SealStoneStatus* tp);

extern cNation*	g_pNation;						// LTS NEW LOCALWAR	

//--------------------------------------------------------------------
//	
//   User Functions..
//	
	
/*---------------------------------------------------------------------
	돌이 리젠되거나 
    돌이 부서지면 게임서버에서 보내준다. 
	
  status : 0 --> 부서짐   1 : 리젠됨..
---------------------------------------------------------------------*/
void RecvSealStoneStatus( t_packet *p )			// LTS LCOALWAR
{	
	t_sealstone_status *tp;
	int status;
	int i;
	
	tp = &p->u.sealstone_status ;

	switch( tp->status )
	{
		// 죽었다. 
	case 0 :		status = 0;		break;
		// 되살아났다. 
	case 1 :		status = 1;		break;
		// 타격을 받았다. 
	case 2 :		status = 2;		break;		// LTS LOCALWAR

	}

	switch( tp->sealstonesprno )//<<< // 030516 kyo
	{
	case 98 :	// 바이서스
		{
			SealStone[ N_VYSEUS].status[tp->id] = status;
			if (g_pNation!=NULL) 
			{
				g_pNation->SetSealStoneStatus(0,tp->id,status,tp->Hp);	// LTS NEW LOCALWAR
			}
			break;
		}	
	case 99 :// 자이펀
		{
			SealStone[ N_ZYPERN].status[tp->id] = status;
			if (g_pNation!=NULL)
			{
				g_pNation->SetSealStoneStatus(1,tp->id,status,tp->Hp);	// LTS NEW LOCALWAR
			}
			break;
		}
	case 91 :// 일스
		{
			SealStone[ N_YLLSE ].status[tp->id] = status;
			
			if (g_pNation!=NULL) 
			{
				g_pNation->SetSealStoneStatus(2,tp->id,status,tp->Hp);	// LTS NEW LOCALWAR
			}
			break;
		}
	}//>>>// 030516 kyo

/*	if (g_pNation!=NULL)
	{
		g_pNation->CheckSealStoneStatus();
	}*/

	SealStoneStatus sealStoneStatus;
	sealStoneStatus.ID = tp->id;
	sealStoneStatus.SprNo = tp->sealstonesprno;
	sealStoneStatus.MapNumber = tp->mapnumber;
	sealStoneStatus.X = tp->x;
	sealStoneStatus.Y = tp->y;
	sealStoneStatus.Status = tp->status;

	if( status == 0 )
	{
		for(i = 0; i < LOGIN_MAX_CONNECTIONS; ++ i)		// 011213 LTS
		{
			if( connections[i].dwAgentConnectionIndex )
			{
				if( connections[i].server_check ) QueuePacket( connections, i, p, 1 );
			}
		}
				// 여기서 전체메세지를 보낸다.
 		SendCMD_SEALSTONE_BROKED(&sealStoneStatus);
	}


	if ( status == 2 )	SendCMD_SEALSTONE_ATTACKED(&sealStoneStatus);
}	
	
/*---------------------------------------------------------------------
	왕이 선전포고를 했다.	->  결계석이 리젠될때에는 할수 없으며
	설공하면 선전포고 했다는 메시지가 뜬다
---------------------------------------------------------------------*/
void RecvSealStoneFromKing( int nation )
{	
	if( SealStone[ nation].regentime ||  SealStone[ nation].waraftertime ) return; // 리젠하는동안에는 다시 선전포고가 되지 않는다. 
	SealStone[ nation].warlefttime = g_curr_time + SEALSTONE_WARTIME_;
	
	t_packet p;
	int i;
	
	p.h.header.type = CMD_SEALSTONE_WAR_START;
	p.h.header.size = sizeof( t_sealstone_war_start);
	p.u.sealstone_war_start.nation = nation;
	
	for(i = 0; i < LOGIN_MAX_CONNECTIONS; ++ i)
		if( connections[i].dwAgentConnectionIndex )
			if( connections[i].server_check )
				QueuePacket( connections, i, &p, 1 );
}	
	
	
void SendResultNationWar( int aresult, int anation, int bresult, int bnation )
{	
	t_packet p;
	int i;
	
	p.h.header.type = CMD_SEALSTONE_RESULT;
	p.h.header.size = sizeof( t_sealstone_result );
		p.u.sealstone_result.aresult = aresult;
		p.u.sealstone_result.anation = anation;
		p.u.sealstone_result.bresult = bresult;
		p.u.sealstone_result.bnation = bnation;
	for(i = 0; i < LOGIN_MAX_CONNECTIONS; ++ i)
		if( connections[i].dwAgentConnectionIndex )
			if( connections[i].server_check )
				QueuePacket( connections, i, &p, 1 );
}	
	
// 이제 결계석을 리젠될것이고 당분간 결계석을 부술수 없다. 
void SendSealStoneReGenStart( void )
{	
	int i;		
	t_packet p;	
				
	p.h.header.type = CMD_SEALSTONE_START_RE_GENTIME;
	p.h.header.size = 0;
	
	for(i = 0; i < LOGIN_MAX_CONNECTIONS; ++ i)
		if( connections[i].dwAgentConnectionIndex )
			if( connections[i].server_check )
				QueuePacket( connections, i, &p, 1 );
}	
	
	
// 이제 선전포고를 할수 있음을 Client까지 알린다. 	
void SendEndOfReGenTime( void )
{	
	int i;		
	t_packet p;	
	
	p.h.header.type = CMD_SEALSTONE_END_RE_GENTIME;
	p.h.header.size = 0;
	
	for(i = 0; i < LOGIN_MAX_CONNECTIONS; ++ i)
		if( connections[i].dwAgentConnectionIndex )
			if( connections[i].server_check )
				QueuePacket( connections, i, &p, 1 );
}	
	
//	현재 남은 시간을 Client까지 뿌려준다.
void SendWarLeftTime_sub( int nation )
{				
	int i;		
	t_packet p;	
				
	p.h.header.type = CMD_SEALSTONE_WAR_LEFTTIME;
	p.h.header.size = sizeof( t_sealstone_warlefttime );
		p.u.sealstone_warlefttime.nation = nation;
		p.u.sealstone_warlefttime.lefttime = SealStone[ nation].warlefttime - g_curr_time;
	for(i = 0; i < LOGIN_MAX_CONNECTIONS; ++ i)
		if( connections[i].dwAgentConnectionIndex )
			if( connections[i].server_check )
				QueuePacket( connections, i, &p, 1 );
}				

// int nation의 나라가 상대의 결게석을 모두 부셨는가?
// 1 : 모두 부셨다. 
// 0 : 아직 남은게 있다. 
int CheckVictory( int nation )
{	
	int i;
	
	for( i = 0 ; i < MAX_SEALSTONE_NATION_ ; i ++)
	{
		if( SealStone[ antination[nation]].status[i] ) break;
	}
	if( i == MAX_SEALSTONE_NATION_ )
	{	
		return 1;	// 모든게 부셔졌다. 
	}	

	return 0; // 아직 남은게 있다. 

}	
	

				
void CheckWarLeftTime( void )
{				
	static DWORD t;
	int nation;
				
	if( g_curr_time - t > 6 )
	{			
		t = g_curr_time;
	}	
	else return;

	for( nation = N_VYSEUS ; nation <= N_ZYPERN ; nation ++ )
	{
		if( SealStone[ nation].warlefttime == 0) continue;
				
		if( SealStone[ nation].warlefttime - g_curr_time  < SEALSTONE_WARTIME_ )
		{		
			SendWarLeftTime_sub( nation );
		}		
		else	
		{		
			SealStone[				nation].warlefttime	= 0;
			SealStone[	antination[nation]].warlefttime	= 0;
			SealStone[				nation].waraftertime	= g_curr_time + SEALSTONE_WAR_AFTERTIME_;
			SealStone[	antination[nation]].waraftertime	= g_curr_time + SEALSTONE_WAR_AFTERTIME_;
				
			if( CheckVictory( nation ) == 1 ) // nation이 이겼다. 
			{	
				SendResultNationWar( SSR_VICTORY, nation, SSR_FAIL, antination[ nation]  );
			}	
			else	//부승부다. 
			{	
				SendResultNationWar( SSR_DRAW, nation, SSR_DRAW, antination[ nation]  );				
			}	
		}		
	}			
}				
				
				
// 전쟁이 끝나고 잠시조용한 시간을 갖는다.
void CheckSealStoneWarAfterTime( void )
{
	int nation;	
	static DWORD t;
		
	if( g_curr_time - t > 10 )
	{	
		t = g_curr_time;
	}	
	else return;

	for( nation = N_VYSEUS ; nation <= N_ZYPERN ; nation ++ )
	{			
		if( SealStone[ nation].waraftertime == 0) continue;
				
		if( SealStone[ nation].waraftertime - g_curr_time  < SEALSTONE_WAR_AFTERTIME_ )
		{			
		}			
		else		
		{			
			SealStone[				nation].waraftertime = 0;
			SealStone[	antination[nation]].waraftertime = 0;
			SealStone[				nation].regentime	 = g_curr_time + SEALSTONE_RE_GENTIME_;
			SealStone[	antination[nation]].regentime	 = g_curr_time + SEALSTONE_RE_GENTIME_;

			SendSealStoneReGenStart();
		}	
	}	
}

// 이시간이 지나면 다시 결계적을 부술수 있다. 
void CheckReGenTime( void )
{	
	static DWORD t;
	int    nation;

	if( g_curr_time - t > 6 )
	{						
		t = g_curr_time;
	}			
	else return;	
				
	for( nation = N_VYSEUS ; nation <= N_ZYPERN ; nation ++ )
	{			
		if( SealStone[ nation].regentime == 0) continue;
				
		if( SealStone[ nation].regentime - g_curr_time  > SEALSTONE_RE_GENTIME_)
		{			
		}			
		else		
		{			
			SealStone[				nation].regentime  = 0;
			SealStone[	antination[nation]].regentime  = 0;
			SealStone[				nation].warlefttime= 0;
			SealStone[	antination[nation]].warlefttime= 0;
			SealStone[				nation].regentime  = 0;
			SealStone[	antination[nation]].regentime  = 0;

			SendEndOfReGenTime();
		}	
	}
}	
	
//	전전포고가 된뒤에 ,모든 결계석이 깨졌는가?
void CheckSealStoneStatus( void )
{		
	int nation, j;	
	static DWORD t;
		
	if( g_curr_time - t > 10 )
	{	
		t = g_curr_time;
	}	
	else return;
		
	for( nation = N_VYSEUS ; nation <= N_ZYPERN ; nation ++ )
	{			
		if( SealStone[ nation].warlefttime ) // 전쟁선포를 한상태에서 .
		{		
			for( j = 0 ; j < MAX_SEALSTONE_NATION_ ; j ++)
			{	
				if( SealStone[ antination[ nation]].status[ j] ) break;
			}	
			if( j == MAX_SEALSTONE_NATION_ ) // 상대방의 결계석이 모두 부셔졌다면.
			{	
				// 승리...
				SendResultNationWar( SSR_VICTORY, nation, SSR_FAIL, antination[ nation]  );

				SealStone[				nation].warlefttime	= 0;
				SealStone[	antination[nation]].warlefttime	= 0;
				SealStone[				nation].waraftertime = g_curr_time + SEALSTONE_WAR_AFTERTIME_;
				SealStone[	antination[nation]].waraftertime = g_curr_time + SEALSTONE_WAR_AFTERTIME_;
			}	
		}		
	}	
}		
		
extern cNation* g_pNation;	// LTS LOCALWAR		
extern int g_LocalWarBegin;	// 011215 LTS
extern bool isNationManageServer();		// 011216 LTS

void SendSealStoneBlock( void )			// 011217 LTS
{	//< CSD-030723
	const int ret = ReturnNation2NationRelation(N_VYSEUS, N_ZYPERN);

	t_packet p;

	switch (ret)
	{												
	case NATION_RELATION_ENEMY_:
		{
			p.h.header.type = CMD_STEALSTONE_BREAKABLE;
			break;
		}
	case NATION_RELATION_FRIEND_:
		{
			CWarStartInfo* pWarStartInfo = g_pNation->GetWarStartInfo();

			if (pWarStartInfo != NULL)
			{
				if (pWarStartInfo->IsActive())
				{
					p.h.header.type = CMD_STEALSTONE_NON_BREAKABLE_PEACE;
				}
				else
				{
					p.h.header.type = CMD_STEALSTONE_NON_BREAKABLE;
				}
			}
			
			break;
		}
	default:	
		{
			return;
		}
	}					
	
	p.u.LocalWar.CommonDataC.Data=g_LocalWarBegin;
	p.h.header.size = sizeof(t_CommonDataC);
		
	SERVER_DATA *pData = g_pServerTable->GetOwnServerData();

	if( pData->wPort == NATION_MANAGE_SERVER )
	{	
		if (NULL != g_pNation)		// LTS NEW LOCALWAR
		{
			switch(p.h.header.type)
			{
			case CMD_STEALSTONE_BREAKABLE: 
				{
					g_pNation->SetLocalWarStatus2(1);
					break;
				}
			case CMD_STEALSTONE_NON_BREAKABLE:
			case CMD_STEALSTONE_NON_BREAKABLE_PEACE:
				{
					g_pNation->SetLocalWarStatus2(0);
					break;
				}
			}
			p.u.LocalWar.CommonDataC.Data = g_LocalWarBegin;
		}

		g_pUserManager->SendPacket(&p); // CSD-031213
		SealStoneBreakable = p.h.header.type;
		g_pServerTable->BroadCastToEveryServer((char*)&p, (sizeof(t_header) + p.h.header.size),  SERVER_TYPE_MAP);
	}
}	//> CSD-030723												

void CheckSealStoneBlock( void )
{	//< CSD-030804
	if (g_pServerTable == NULL)
	{
		return;
	}

	if (!isNationManageServer())
	{
		return;	// 011216 LTS			
	}

	if (!g_pNation)
	{
		return; // 011216 LTS
	}

	static DWORD t;
	static int oldh;
	static int nCount;
	
	if (g_curr_time	- t > 60 || oldh != g_hour) // LTS LOCALWAR
	{
		CWarStartInfo* pWarStartInfo = g_pNation->GetWarStartInfo();

		if (pWarStartInfo != NULL)
		{
			pWarStartInfo->Execute();
		}

		if (++nCount >= 10 || oldh != g_hour)
		{
			nCount = 0;
			SendSealStoneBlock();
		}

		t = g_curr_time;
		oldh = g_hour;
	}
}	//> CSD-030804
	
extern bool UpdateNationWarData();	// 010904 LTS // 전쟁터 맵의 업데이트 
extern void UpdateEventLocalWarTime();	// 020115 LTS
	
// 020620 YGI
void CheckNationWar( void )
{	
	UpdateNationWarData();  // 010904 LTS
	UpdateEventLocalWarTime();	// 020115 LTS
	if( g_MapPort != NATION_MANAGE_SERVER ) return;
	CheckSealStoneBlock();
	CheckSealStoneStatus();	
}	
	
//////////////////////////////////////////////////////////////////////////////
	
/******************************************************************************************************
기 능 : 최소의 국가상태를 설정한다.  기본적으로 모든국가는 적대국가이다.
인 자 : 없음
결 과 : 
*******************************************************************************************************/
int InitNationRelation( void )
{
	int i,j;

	for( i = 0 ; i < MAX_NATION ; i ++)
	{
		for( j = 0 ; j < MAX_NATION ; j ++)
		{
			if( i == j ) NationRelation[i][j].relation = NATION_RELATION_NO_; 
			else NationRelation[i][j].relation = NATION_RELATION_ENEMY_;
		}
	}

	return 1;
}				
				
				
/******************************************************************************************************
기	능 : 현재 국가간의 관계를 리턴한다. 
인	자 : int a_nation, b_nation : 관계를 설정할 국가 ( 국가코드,  3: 바이서스, 4:자이펀 ... )
			
결	과 : 0 : 무관계    1: 적대관계     2: 협력관계    3: 주종관계( a:주 b:종)
*******************************************************************************************************/
int ReturnNation2NationRelation( int anation, int bnation )		// 011217 LTS
{	//< CSD-030723
	if( anation > MAX_NATION ) return 0;
	if( bnation > MAX_NATION ) return 0;
		
	t_NationRelation* ta = &NationRelation[anation][bnation];
	t_NationRelation* tb = &NationRelation[bnation][anation];

	if( ta->year &&
	  ( (double)ta->year* 365*24*60*60 +	(double)ta->yday* 24*60*60 +	(double)ta->hour* 60*60 +	ta->howlong <
		(double)g_year	* 365*24*60*60 +	(double)g_yday	* 24*60*60 +	(double)g_hour	* 60*60 ) )	
	{	
		ta->relation = ta->oldrelation;
		tb->relation = tb->oldrelation;
		tb->year = 0;
		ta->year = 0;
	}	
	else
	{	
		CWarStartInfo* pWarStartInfo = g_pNation->GetWarStartInfo();

		if (pWarStartInfo != NULL && pWarStartInfo->IsActive())
		{
			return NATION_RELATION_ENEMY_;
		}

		return NATION_RELATION_FRIEND_;
	}
	
	return ta->relation;
}	//> CSD-030723
	
int SetNation2NationRelation( int a_nation, int b_nation, int relation, int year, int yday, int hour, DWORD howlong )	// 011217 LTS
{	//< CSD-030723
	if (a_nation > MAX_NATION)
	{
		return NATION_RELATION_FAIL_NO_NATION_;
	}

	if (b_nation > MAX_NATION)
	{
		return NATION_RELATION_FAIL_NO_NATION_;
	}
		
	switch (relation)
	{	
	case NATION_RELATION_FRIEND_:
		{
			if (relation == NationRelation[a_nation][b_nation].relation)
			{
				return NATION_RELATION_SAME_FAIL_;
			}

			CWarStartInfo* pWarStartInfo = g_pNation->GetWarStartInfo();

			if (pWarStartInfo != NULL && pWarStartInfo->IsActive())
			{
				return NATION_RELATION_FAIL_;
			}

			break;
		}
	case NATION_RELATION_ENEMY_:
	case NATION_RELATION_MASTER_SERVANT_:
		{
			break;		
		}
	}					

	return NATION_RELATION_SUCCESS_;
}	//> CSD-030723
		
/********************************************************************************************겜섭(맵섭)에서 사용하는 함수
기	능 : 국가간의 관계를 Loginserver로 보내 게임서버에 보내어 모든 게임섭(맵섭)이 알게 한다. 
		  
인	자 : int a_nation, b_nation : 관계를 설정할 국가 ( 국가코드,  3: 바이서스, 4:자이펀 ... )
		int relation           : 0 : 무관계    1: 적대관계     2: 협력관계    3: 주종관계( a:주 b:종)
		DWORD relationstart		:	몇초뒤에 시작할것인가
		DWORD relationhowlong	:	몇초동안 유지될것인가? ( 0이면 무한 )

결	과 : LoginServer에 보내지면, 다시 게임섭(맵섭)으로 보내지며, 다시 Client까지 결과를 보내주게 된다. 
*******************************************************************************************************/
void RecvNation2NationRelation( int cn, t_nation2nation_relation *tp )
{										
	t_packet p;
	int i;
	int ret;


	ret = SetNation2NationRelation( tp->a, tp->b, tp->relation, tp->year, tp->yday, tp->hour, tp->howlong );

	p.h.header.type = CMD_NATION2NATION_RELATION_RESULT;
	p.h.header.size = sizeof( t_nation2nation_relation_result );
			
	p.u.nation2nation_relation_result.why = ret;
			
	for( i = 0 ; i < LOGIN_MAX_CONNECTIONS ; i ++)
		if( connections[i].dwAgentConnectionIndex )
			if( connections[i].server_check )
				QueuePacket( connections, i, &p, 1 );
}
	







