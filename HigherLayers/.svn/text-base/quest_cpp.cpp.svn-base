#include "..\stdafx.h"
#include "DefaultHeader.h"
#include "MenuDefine.h"
#include "ItemList.h"
#include "CItem.h"
#include "Scrp_exe.h"
#include "quest_cpp.h"


void SendBBS2( char *text, short int cn )
{
	if (strlen( text ) >= 260) return; // finito 070707

	t_packet p;
	p.h.header.type = CMD_BBS_QUEST;
		strcpy_s( p.u.server_bbs.bbs, sizeof(p.u.server_bbs.bbs), text );
	p.h.header.size = sizeof( t_server_bbs ) - MAX_PATH + strlen( text );
	QueuePacket( connections, cn , &p, 1 );
}