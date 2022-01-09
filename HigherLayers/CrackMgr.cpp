// CrackMgr.cpp: implementation of the CCrackMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "../stdafx.h"
#include "CrackMgr.h"

#include "LogManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const	int DEFAULT_CHECK_ACCEL_TIME	= 35000;//반드시 CHECK_ACCEL_RECOMMAND_TIME보다 5초 이상 커야 한다 단위는 ms
const	int CHECK_ACCEL_RECOMMAND_TIME	= 30000;//30 초
const	int CHECK_ACCEL_PING_TIME		= 1800;

CCrackMgr CrackMgr;
CCrackMgr::CCrackMgr()
{

}

CCrackMgr::~CCrackMgr()
{

}

void CCrackMgr::AutoCheckAccelator(CHARLIST *ch)
{
	const int cn = ch->GetServerID();
	const DWORD dwRealTime = timeGetTime();
	const int iMyState = connections[cn].state;

	if(iMyState==CONNECT_JOIN  || iMyState==CONNECT_JOIN_START)//게임 중일 경우만 체크 한다
	{
		if( dwRealTime >=  ch->dwCheckAccelTime)//시간 체크
		{
			if( ch->dwSendedAccelTime )//아까 보냈는데 새로 보낼때 까지 응답이 없으면
			{
				return;
			}
			t_packet	packet;
			ch->dwCheckAccelTime = dwRealTime + DEFAULT_CHECK_ACCEL_TIME;
			ch->dwSendedAccelTime = dwRealTime;
			packet.h.header.type = CMD_CHECK_ACCEL;
			packet.h.header.size = sizeof(t_server_check_accel);
			packet.u.server_check_accel.dwCheckAccelNowtime	= ch->dwSendedAccelTime;//보내는 시간.
			::QueuePacket(connections, cn, &packet, 1);
		}
	}
	else//게임중이 아니면
	{
		ch->dwSendedAccelTime = 0;//보낸건 무효화 시킨다
	}	
	return;
}

void CCrackMgr::ComformAccelator( const int cn, const t_packet *packet )
{
	CHARLIST *ch = ::CheckServerId(cn);
	if(!ch) {return;}

	char msg[300] = {0,};
	
	const DWORD dwRealtime = timeGetTime();	
	const DWORD dwRecvSendedServerTime	=	packet->u.server_check_accel.dwCheckAccelNowtime;
	const DWORD dwSendedAccelTime		=	ch->dwSendedAccelTime;
	const int iMyState = connections[cn].state;
	//괜찮은 영역은 (지금 90초전- 핑타임)부터 (지금까지) 이다
	
	if(iMyState==CONNECT_JOIN  || iMyState==CONNECT_JOIN_START)//게임 중일 경우만 체크 한다
	{
		if	((	dwRealtime-dwRecvSendedServerTime < CHECK_ACCEL_RECOMMAND_TIME)//-CHECK_ACCEL_PING_TIME )//CHECK_ACCEL_RECOMMAND_TIME 이전 보다 작다//CHECK_ACCEL_PING_TIME 핑있다고 친다
			||(	dwSendedAccelTime != dwRecvSendedServerTime))//아까 보낸 값과 다른 값이 들어온다
		{
			sprintf_s( msg, sizeof(msg),"Accel Time Error 90 Never.. \nSended = %d , Recved = %d , Value = %d",dwSendedAccelTime, dwRecvSendedServerTime, dwRealtime-dwRecvSendedServerTime);

			if(g_accelator_user_closeconnection )	
			{	
				SendHackingUser( cn, HACKING_ACCELATOR_, msg );
				closeconnection( connections, cn, -208 );
			}
		}
		ch->dwSendedAccelTime = 0;//받았으니까 보냈던 시간을 클리어 해준다// 위치 변경 조심. 지우지도 말고. 주석 처리 하지 말것
	}
	else
	{
		ch->dwSendedAccelTime = 0;
	}
}

void CCrackMgr::SendHackingUser( const int cn, const int type, const char *cause )
{
	t_packet p;
	CHARLIST * ch =	::CheckServerId(cn);
	if(!ch){return;}
	if(ch->SprType == SPRITETYPE_NPC ) {return;}

	p.h.header.type = CMD_HACKING_USER;
	p.h.header.size = sizeof( t_hacking_user );
	t_hacking_user *tp = &p.u.hacking_user;
	tp->type = type;//	short	type;
	::memcpy( tp->id, connections[cn].id ,ID_LENGTH);//	char	id[ 20];
	::memcpy( tp->name, connections[cn].name ,NM_LENGTH);//	char	name[20];
	::memcpy( tp->ip, connections[cn].ip_address,IP_LENGTH );//	char	ip[20];
	::memcpy( tp->cause, cause ,100);//	char	cause[100];

	QueuePacket( connections, DB_DEMON, &p, 1 );
}

void CCrackMgr::RecvAccelType(int cn, t_packet *p)
{	//< CSD-030808
	CHARLIST* pTarget = ::CheckServerId(cn);

	if (pTarget == NULL)
	{
		return;
	}

	t_packet t;
	t.h.header.type = CMD_SEND_ACCEL_TYPE;
	t.h.header.size = 0;
	QueuePacket(connections,cn,&t,1);
	
	g_pLogManager->SaveAccelType(pTarget, p->u.Accel_Type.strMsg);
}	//> CSD-030808
