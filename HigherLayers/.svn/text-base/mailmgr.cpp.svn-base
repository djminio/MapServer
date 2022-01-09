// MailMgr.cpp: implementation of the CMailMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "../stdafx.h"
#include "MailMgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//DB 데몬과 통신하는 클래스
CMailMgr g_MailMgr;
CMailMgr::CMailMgr()
{

}

CMailMgr::~CMailMgr()
{

}

void CMailMgr::RecvReqMailList(const int cn,t_packet *p)//리스트 요청을 받음
 {
//	CMD_MAIL_REQUEST_LIST	를 DB데몬으로
	if(::ExistHe(p->u.Hwoa.rang.ReqestMailList.szName) > 0 )//그사람이 있을 경우 디비에게 요청을 한다 // 030923 HK YGI
	{
		::QueuePacket(connections,DB_DEMON,p,1);
	}
}

void CMailMgr::RecvMailListFromDBDEMON(const int cn,t_packet *p)//결과물을 DB에게 받음
{
	MAILLISTRESULT *pMailResult = (LPMAILLISTRESULT)&p->u.Hwoa.rang.ReqestMailBodyResult;
	const int iTargetCn = ::ExistHe(pMailResult->szName);
	if(iTargetCn > 0 )//그사람이 있을 경우 받은 리스트를 보내준다	// 030923 HK YGI
	{
		::QueuePacket(connections,iTargetCn,p,1);
	}
}

bool CMailMgr::Recv(const int cn, t_packet *p)
{	//< CSD-030324
	CHARLIST *ch = ::CheckServerId( cn );
	if( !ch ) {return false;}

	MAILSEND *pMailSend = &p->u.Hwoa.rang.MailSend;

	const int iTargetCn = ::ExistHe(pMailSend->szSender);
	if(iTargetCn && cn == iTargetCn)//그사람이 있을 경우 받은 리스트를 보내준다
	{
		switch(pMailSend->iType)
		{
		case GAMEMASTER_MAIL:
			if(!ch->IsCounselor())
			{
				return false;
			}break;
		case GUILD_MAIL:
			if(!ch->GetGuildCode())
			{
				return false;
			}break;
		}
		::QueuePacket(connections, DB_DEMON, p, 1);
	}
/*
	t_packet p;

	if( type == CMD_SEND_MAIL_GUILD )
	{
		if( !ch->GetGuildCode() ) return;
		// if( !ch->name_status.degree ) return;		// 길드 등급이 0이면 메일을 못 보낸다.
		p.u.kein.game_to_login_send_mail.uni.guild_info.guild_code = ch->GetGuildCode();
		p.u.kein.game_to_login_send_mail.uni.guild_info.degree = ch->name_status.guild_master;
	}
	else
	{
		strcpy( p.u.kein.game_to_login_send_mail.uni.recv_name, t->name );
	}		

	strcpy( p.u.kein.game_to_login_send_mail.send_name, ch->Name );		
	strcpy( p.u.kein.game_to_login_send_mail.title, t->title );
	strcpy( p.u.kein.game_to_login_send_mail.body, t->body );		

	p.h.header.size = sizeof( k_game_to_login_send_mail ) - 1000 + strlen( t->body );
*/
	return true;
}	//> CSD-030324

void CMailMgr::RecvDelete(const int cn,t_packet *p)//지우기(리스트 번호를 보냅니다.)
{
	CHARLIST *ch = ::CheckServerId( cn );
	if( !ch ) {return;}
	::QueuePacket(connections, DB_DEMON, p, 1);
}

void CMailMgr::RecvRepair(const int cn,t_packet *p)//지우기(리스트 번호를 보냅니다.)
{
	CHARLIST *ch = ::CheckServerId( cn );
	if( !ch ) {return;}
	::QueuePacket(connections, DB_DEMON, p, 1);
}

void CMailMgr::RecvRequestBody(const int cn,t_packet *p)
{
	p->u.Hwoa.rang.ReqestMailBody.szName[MAX_NAME-1] = 0;
	const int iSender = ::ExistHe(p->u.Hwoa.rang.ReqestMailBody.szName);
	if( cn != iSender) {return;}

	::QueuePacket(connections, DB_DEMON, p, 1);
}

void CMailMgr::RecvRequestBodyResult(const int cn,t_packet *p)
{
	const int iTarget = ::ExistHe(p->u.Hwoa.rang.ReqestMailBodyResult.szReceiver);
	if( -1 == iTarget ) {return;}
	::QueuePacket(connections, iTarget, p, 1);
}

void SendGameToClientWhoSendMail( char *name )
{
	int server_id = ExistHe( name );
	if( server_id != -1 )
	{
		CallClient( server_id, CMD_MAIL_ALERT );
	}
}

