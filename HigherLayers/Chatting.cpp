#include "..\stdafx.h"
#include "DefaultHeader.h"
#include "Chatting.h"
#include "UserManager.h"
#include "CounselorManager.h"
extern bool youvegotitInventory( int cn, ItemAttr *item );
extern bool youvegotitInventory( int cn, int target_id );

void SendChatSetResult(int cn, int result)
{
	t_packet p;
	p.h.header.type = CMD_CHAT_SET_RESULT;
	p.h.header.size = sizeof(t_chat_set_result);
	p.u.chat_set_result.result = result;
	QueuePacket(connections, cn, &p, 1);
}

void RecvChatSet( int cn, t_chat_set *p )
{	//< CSD-030415
	switch (p->chattype)
	{								
	case CHATTYPE_PARTY:	
		{
			int i;
			for (i = 0; i < MAX_PARTY_MEMBER; ++i)
			{	//0213 YGI
				if( connections[cn].chrlst.party[i].On ) break;
			}

			if( i != MAX_PARTY_MEMBER ) // party있음. 
			{
				connections[cn].chrlst.chattype = p->chattype;
				SendChatSetResult( cn, 10 );
			}
			else
			{
				connections[cn].chrlst.chattype = CHATTYPE_NORMAL;
				SendChatSetResult( cn, 11 );
			}
			break;
		}
	case CHATTYPE_WHISPER:
	case CHATTYPE_GUILD:
		{
			int tempid = -1;

			if (p->chatname[0])
			{
				tempid = g_pUserManager->GetServerID(p->chatname);
			}
				
			int result = 0;

			if (tempid != -1)
			{	
				connections[cn].chrlst.whisperid = tempid;
				strcpy_s(connections[cn].chrlst.chatname, sizeof(connections[cn].chrlst.chatname), p->chatname);
				connections[cn].chrlst.chattype = p->chattype;
				result = 1;
				SendChatSetResult(cn, result);
			}	
			else 
			{	
				if (p->chatname[0] == 0)	
				{
					SendChatSetResult(cn, 2); // 기본설정으로 바뀌었습니다. 
				}
				else
				{
					SendChatSetResult(cn, 0);	// '%s'란 이름을 가진사람이 없습니다. 
				}

				connections[cn].chrlst.chattype = CHATTYPE_NORMAL;
			}	

			break;	
		}						
	case CHATTYPE_NORMAL:	
		{
			connections[cn].chrlst.chattype = p->chattype;
			SendChatSetResult(cn, 2);// 기본설정으로 바뀌었습니다. 
			break;
		}
	}								
}	//> CSD-030415								
									
// 현재 귓속말로 설정이 되어 있다면......
void SendWhisper(t_connection c[], int cn, t_packet *packet)
{				
	const int range	= (int)packet->u.client_chat_data.range;
	const int type	= (int)packet->u.client_chat_data.type;
	
	int length	= (int)packet->u.client_chat_data.length;
	
	if (length <= 0 || length >= TEXT_LENGTH)
	{
		return;
	}
	
	int wid = c[cn].chrlst.whisperid;
	
	if (IsConnectJoin(wid))
	{	//< CSD-030515
		wid  = g_pUserManager->GetServerID(c[cn].chrlst.chatname);
	}	//> CSD-030515
	
	if (IsConnectJoin(wid))
	{	//< CSD-030515
		CHARLIST* wch = &c[wid].chrlst; 
		CHARLIST* ch  = &c[ cn].chrlst;
								
		if (strcmp(wch->Name, c[cn].chrlst.chatname) == 0)
		{					
			t_packet p;
			p.h.header.type = CMD_CHAT_DATA;
			p.h.header.size = sizeof(t_server_chat_data)-TEXT_LENGTH+ length;
			p.u.server_chat_data.server_id	= cn;
			p.u.server_chat_data.type		= CHATTYPE_WHISPER; 
			p.u.server_chat_data.length = (unsigned char)length;
			char temp[FILENAME_MAX + NM_LENGTH + 5];
			sprintf_s(temp, sizeof(temp), "'%s':%s", c[cn].chrlst.Name, packet->u.client_chat_data.text);
			strncpy_s(p.u.server_chat_data.text, temp, TEXT_LENGTH);
			p.u.server_chat_data.text[TEXT_LENGTH - 1] = 0;
			QueuePacket(c, wid, &p, 1);
			return;
		}		
	}	//> CSD-030515
				
	c[cn].chrlst.chattype = CHATTYPE_NORMAL;
	SendChatSetResult(cn, 2);
}

void SendChatParty( t_connection c[], int cn, t_packet *packet )
{	//< CSD-030415
	char temp[FILENAME_MAX + NM_LENGTH + 5];
	
	int range	= (int)packet->u.client_chat_data.range;
	int type	= (int)packet->u.client_chat_data.type;
	int length	= (int)packet->u.client_chat_data.length;
	if(length <= 0 || length >= TEXT_LENGTH)	return;

	CHARLIST* pUser = &c[cn].chrlst;

	for (int i = 0 ; i < MAX_PARTY_MEMBER; ++i)
	{
		if (pUser->party[i].On)
		{
			const int idParty = g_pUserManager->GetServerID(pUser->party[i].Name);
			
			if (idParty != -1)
			{
				t_packet p;

				sprintf_s(temp, sizeof(temp), "%s: %s", c[cn].chrlst.Name, packet->u.client_chat_data.text);
				strncpy_s(p.u.server_chat_data.text, temp, TEXT_LENGTH);
				p.u.server_chat_data.text[TEXT_LENGTH - 1] = 0;
				length = strlen(p.u.server_chat_data.text);

				p.h.header.type = CMD_CHAT_DATA;
				p.h.header.size = sizeof(t_server_chat_data) - TEXT_LENGTH + length;
				p.u.server_chat_data.server_id = idParty;
				p.u.server_chat_data.type = CHATTYPE_PARTY; 
				p.u.server_chat_data.length = (unsigned char)length;
				QueuePacket(c, idParty, &p, 1);
			}
		}
	}
}	//> CSD-030415

void SendCounselorChat(string chatData, ChatTypes chatType)
{
	int length	= (int)chatData.length() - 1;
	if(length <= 0 || length >= TEXT_LENGTH) return;

	t_packet p;	
	p.h.header.type					= CMD_CHAT_DATA;
	p.u.server_chat_data.server_id	= 1000; // force client to use the name of character who recv packet
	p.u.server_chat_data.type = chatType; 
	p.u.server_chat_data.length		= (unsigned char)chatData.length();
	memcpy(p.u.server_chat_data.text, chatData.c_str(), length );
	p.h.header.size = sizeof(t_server_chat_data)-TEXT_LENGTH+length;
	g_pCounselorManager->SendPacket(&p);
}