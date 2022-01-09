#include "..\stdafx.h"
#include "..\LowerLayers\packed.h"
#include "..\LowerLayers\servertable.h"
#include "..\LowerLayers\mylog.h"
#include "CItem.h"
#include "Chatting.h"
#include "UserManager.h"
#include "BattleManager.h"

extern void	SendCharacterCondition( CHARLIST*, int);
extern void UserAddBasicData(  t_connection c[], int from, int to, t_server_user_add *p );

SORT_DATA PC_TABLE[DRAGON_MAX_CONNECTIONS_ + 1];
SORT_DATA NPC_TABLE[MAX_NPC_LIST];
SORT_DATA ITEM_TABLE[MAX_ITEM_LIST];
CMD_AREA CMD_AREA_TABLE[MAX_CMD_AREA];

int PC_COUNT = 0;
int NPC_COUNT = 0;
int ITEM_COUNT = 0;
int CMD_AREA_COUNT = 0;
// 2000/11/09, chan78 :: New CastMe2Other()
void CastMe2Other(int cn, t_packet *packet)
{
	t_packet* a = packet;
	
	if (connections[cn].dwAgentConnectionIndex)
	{
		for (int i = 0; i < MAX_AREA_BUFFER; ++i) 
		{
			if (connections[cn].area_tbl[i].bAlive == TRUE)
			{
				if (connections[cn].area_tbl[i].Type == 1)
				{
					if (connections[connections[cn].area_tbl[i].ID].dwAgentConnectionIndex)
					{
						DWORD dwTargetAgentConnectionIndex = connections[connections[cn].area_tbl[i].ID].dwAgentConnectionIndex;
						DWORD dwTargetID = connections[connections[cn].area_tbl[i].ID].dwIDForAgent;
						
						if (g_pServerTable->GetPackedMsg( dwTargetAgentConnectionIndex))
						{
							if (g_pServerTable->GetPackedMsg(dwTargetAgentConnectionIndex)->GetUserNum() == 0)
							{
								g_pServerTable->GetPackedMsg(dwTargetAgentConnectionIndex)->SetMsg((char*)packet, sizeof(t_header) + packet->h.header.size);
							}

							g_pServerTable->GetPackedMsg(dwTargetAgentConnectionIndex)->AddUser(dwTargetID);
						} 
						else 
						{
							MyLog(LOG_FATAL, "Oops!!!(CI:%d, CN:%d)'s ->GetPackedMsg() is NULL", dwTargetAgentConnectionIndex, dwTargetID);
						}
					}
				}
			}		
		}
	}
	
	g_pServerTable->SendPackedMsg();
	return;
}


void CastNPC2Other(int npc, t_packet *packet)
{	// NPC를 가지고 있는 모든 PC에게 뿌려준다. 
	 for (int i = 0; i < PC_COUNT; ++i)
	 {
		for (int j = 0; j < MAX_AREA_BUFFER; ++j)
		{
			if (connections[PC_TABLE[i].ID].area_tbl[j].bAlive == TRUE)
			{
				if (connections[PC_TABLE[i].ID].area_tbl[j].Type == 2 && connections[PC_TABLE[i].ID].area_tbl[j].ID == npc && connections[PC_TABLE[i].ID].dwAgentConnectionIndex) 
				{
					if (g_pServerTable->GetPackedMsg(connections[PC_TABLE[i].ID].dwAgentConnectionIndex)) 
					{
						if (g_pServerTable->GetPackedMsg(connections[PC_TABLE[i].ID].dwAgentConnectionIndex)->GetUserNum() == 0) 
						{
							g_pServerTable->GetPackedMsg(connections[PC_TABLE[i].ID].dwAgentConnectionIndex)->SetMsg((char*)packet, sizeof(t_header) + packet->h.header.size);
						}

						g_pServerTable->GetPackedMsg(connections[PC_TABLE[i].ID].dwAgentConnectionIndex)->AddUser(connections[PC_TABLE[i].ID].dwIDForAgent);
					} 
					else 
					{
						MyLog(LOG_FATAL, "Oops!!!(CI:%d, CN:%d)'s ->GetPackedMsg() is NULL", PC_TABLE[i].ID, connections[PC_TABLE[i].ID].dwAgentConnectionIndex);
					}
				}
			}
		}
	 }

	g_pServerTable->SendPackedMsg();
	return;
}
// Modified by chan78 at 2000/11/09
void SendModifyPositionNPC( t_connection c[], int ch )
{
	t_packet packet;
	packet.h.header.type = CMD_MODIFY_POSITION;
	packet.h.header.size = sizeof( t_server_modify_position );
	packet.u.server_modify_position.server_id = ch + 10000;
	packet.u.server_modify_position.gox	= NPCList[ch].MoveGox;
	packet.u.server_modify_position.goy	= NPCList[ch].MoveGoy;

	for (int i = 0; i < PC_COUNT; ++i)
	{
		for (int j = 0; j < MAX_AREA_BUFFER; ++j)
		{
			if (c[PC_TABLE[i].ID].area_tbl[j].bAlive == TRUE)
			{
				if (c[PC_TABLE[i].ID].area_tbl[j].Type == 2 && c[PC_TABLE[i].ID].area_tbl[j].ID == ch && c[PC_TABLE[i].ID].dwAgentConnectionIndex != 0)
				{
					if (g_pServerTable->GetPackedMsg(c[PC_TABLE[i].ID].dwAgentConnectionIndex)) 
					{
						if (g_pServerTable->GetPackedMsg(c[PC_TABLE[i].ID].dwAgentConnectionIndex)->GetUserNum() == 0) 
						{
							g_pServerTable->GetPackedMsg(c[PC_TABLE[i].ID].dwAgentConnectionIndex)->SetMsg( (char*)&packet, (sizeof(t_header)+packet.h.header.size));
						}

						g_pServerTable->GetPackedMsg(c[PC_TABLE[i].ID].dwAgentConnectionIndex)->AddUser(c[PC_TABLE[i].ID].dwIDForAgent);
					} 
					else 
					{
						MyLog(LOG_FATAL, "Oops!!!(CI:%d, CN:%d)'s ->GetPackedMsg() is NULL", PC_TABLE[i].ID, connections[PC_TABLE[i].ID].dwAgentConnectionIndex);
					}
				}
			}
		}
	}

	g_pServerTable->SendPackedMsg();
}
// Modified by chan78 at 2000/11/09
void RecvNPCModifyPos( int cn, int id, int mx, int my )
{	
	id -= 10000;

	if (id < NPC_LIST_START || id >= MAX_NPC_LIST)
	{	
		return;
	}	

	if (!connections[cn].dwAgentConnectionIndex || connections[cn].state < CONNECT_JOIN)
	{
		return;
	}

	if (NPCList[id].ctrlpc != cn)
	{
		return;
	}
	
	NPCList[id].MoveGox = NPCList[id].MoveSx = mx;
	NPCList[id].MoveGoy = NPCList[id].MoveSy = my;
	NPCList[id].MoveGox = mx*TILE_SIZE;
	NPCList[id].MoveGoy = my*TILE_SIZE;
	NPCList[id].MoveLength = NPCList[id].MovePathCount=  0;
	SetArea(MOVE_NPC_AREA, id);
	return;// 0906 KHS
	
	t_packet packet;
	packet.h.header.type = CMD_MODIFY_POSITION;
	packet.h.header.size = sizeof(t_server_modify_position);
	packet.u.server_modify_position.server_id = id + 10000;
	packet.u.server_modify_position.gox	= NPCList[id].MoveGox;
	packet.u.server_modify_position.goy	= NPCList[id].MoveGoy;
		
	for (int i = 0; i < PC_COUNT; ++i)
	{	
		for (int j = 0; j < MAX_AREA_BUFFER; ++j)
		{
			if (connections[PC_TABLE[i].ID].area_tbl[j].bAlive == TRUE)
			{
				if (connections[PC_TABLE[i].ID].area_tbl[j].Type == 2 && connections[PC_TABLE[i].ID].area_tbl[j].ID == id && connections[PC_TABLE[i].ID].dwAgentConnectionIndex)
				{
					if (g_pServerTable->GetPackedMsg(connections[PC_TABLE[i].ID].dwAgentConnectionIndex)) 
					{
						if (g_pServerTable->GetPackedMsg(connections[PC_TABLE[i].ID].dwAgentConnectionIndex)->GetUserNum() == 0) 
						{
							g_pServerTable->GetPackedMsg(connections[PC_TABLE[i].ID].dwAgentConnectionIndex)->SetMsg( (char*)&packet, (sizeof(t_header)+packet.h.header.size));
						}

						g_pServerTable->GetPackedMsg(connections[PC_TABLE[i].ID].dwAgentConnectionIndex)->AddUser(connections[PC_TABLE[i].ID].dwIDForAgent);
					} 
					else 
					{
						MyLog(LOG_FATAL, "Oops!!!(CI:%d, CN:%d)'s ->GetPackedMsg() is NULL", PC_TABLE[i].ID, connections[PC_TABLE[i].ID].dwAgentConnectionIndex);
					}
				}
			}
		}
	}

	g_pServerTable->SendPackedMsg();
}
	
// Modified by chan78 at 2000/11/09
void SendMoveNPCArea(t_connection c[], int npc )
{
	t_packet packet;
	int i, j;

	if( PC_COUNT <= 0 )  return;

	packet.h.header.type = CMD_MOVE;
	if( SettingMoveData_( npc+10000, &NPCList[npc], &packet ) == 0 ) return;

	for(i = 0; i < PC_COUNT; i ++)
	{
		for(j = 0; j < MAX_AREA_BUFFER; j ++)
		{
			if(c[PC_TABLE[i].ID].area_tbl[j].bAlive == TRUE)
			{
				if(c[PC_TABLE[i].ID].area_tbl[j].Type == 2 && c[PC_TABLE[i].ID].area_tbl[j].ID == npc && c[PC_TABLE[i].ID].dwAgentConnectionIndex)
				{
					if( g_pServerTable->GetPackedMsg( c[PC_TABLE[i].ID].dwAgentConnectionIndex ) ) 
					{
						if( g_pServerTable->GetPackedMsg( c[PC_TABLE[i].ID].dwAgentConnectionIndex )->GetUserNum() == 0 ) 
						{
							g_pServerTable->GetPackedMsg( c[PC_TABLE[i].ID].dwAgentConnectionIndex )->SetMsg( (char*)&packet, (sizeof(t_header)+packet.h.header.size) );
						}
						g_pServerTable->GetPackedMsg( c[PC_TABLE[i].ID].dwAgentConnectionIndex )->AddUser( c[PC_TABLE[i].ID].dwIDForAgent );
					} 
					else 
					{	//010815 lsw 메세지 조금 수정
						MyLog( LOG_FATAL, "SendMoveNPCArea Failed.(CI:%d, CN:%d)'s ->GetPackedMsg() is NULL Agent has problem", PC_TABLE[i].ID, connections[PC_TABLE[i].ID].dwAgentConnectionIndex );
					}
				}
			}
		}
	}
	g_pServerTable->SendPackedMsg();
};

void SendAttackNPCArea( t_connection c[], int npc )
{
	g_pBattleManager->SendNpcAttack(npc);
}

// Modified by chan78 at 2000/11/09
void JustMessage( char *txt, ... )
{
	return;

	char Str[TEXT_LENGTH];
	memset(Str, 0, TEXT_LENGTH);
	va_list val;
	va_start(val, txt);
	vsprintf_s(Str, sizeof(Str), txt, val);
	va_end(val);

	int length = strlen( Str );
	if(length <= 0 || length >= TEXT_LENGTH) return;
	
	t_packet packet;
	packet.h.header.size = sizeof(t_server_chat_data)-TEXT_LENGTH+length;
	packet.h.header.type = CMD_CHAT_DATA;
	packet.u.server_chat_data.length = (unsigned char)length;
	packet.u.server_chat_data.type = (unsigned char)CHATTYPE_NORMAL;
	memset(packet.u.server_chat_data.text, 0, TEXT_LENGTH);
	memcpy(packet.u.server_chat_data.text, Str, length);

	for (int i = DRAGON_CONNECTIONS_START; i < DRAGON_MAX_CONNECTIONS; ++i)
	{
		if(connections[i].dwAgentConnectionIndex && connections[i].state >= CONNECT_JOIN)
		{
			packet.u.server_chat_data.server_id = i;
			
			if( g_pServerTable->GetPackedMsg( connections[i].dwAgentConnectionIndex ) ) 
			{
				if( g_pServerTable->GetPackedMsg( connections[i].dwAgentConnectionIndex )->GetUserNum() == 0 ) 
				{
					g_pServerTable->GetPackedMsg( connections[i].dwAgentConnectionIndex )->SetMsg( (char*)&packet, (sizeof(t_header)+packet.h.header.size) );
				}

				g_pServerTable->GetPackedMsg( connections[i].dwAgentConnectionIndex )->AddUser( connections[i].dwIDForAgent );
			} 
			else 
			{
				MyLog( LOG_FATAL, "Oops!!!(CI:%d, CN:%d)'s ->GetPackedMsg() is NULL", PC_TABLE[i].ID, connections[PC_TABLE[i].ID].dwAgentConnectionIndex );
			}
		}
	}

	g_pServerTable->SendPackedMsg();
}

void SendNPCChatArea( int npc, char *String, ...  )
{
	va_list val;
	char Str[ TEXT_LENGTH];	
	t_packet packet;		
									
	if( String == NULL ) return;
							
	va_start( val, String );
	vsprintf_s( Str, sizeof(Str), String, val );
	va_end( val );			
							
	int length = strlen( Str );
	if( length == 0 ) return;
							
	packet.h.header.type = CMD_CHAT_DATA;
	{						
		packet.u.server_chat_data.server_id = npc+10000;
		packet.u.server_chat_data.type		= (unsigned char)CHATTYPE_NORMAL;
		packet.u.server_chat_data.length	= (unsigned char)length;
		memcpy(packet.u.server_chat_data.text, Str, length);
	}						
	packet.h.header.size = sizeof(t_server_chat_data)-TEXT_LENGTH+length;


							
	CastNPC2Other( npc, &packet );	
}

// Modified by chan78 at 2000/11/09
void SendChatArea(const int cn, t_packet *packet)
{
	const int type		= (int)packet->u.client_chat_data.type;
	packet->u.client_chat_data.text[TEXT_LENGTH-1] = 0;//혹시나 하는 상황 때문에 마지막을 널문자로 채운다
	const int iLen = ::strlen(packet->u.client_chat_data.text);

	if(iLen <= 0 || iLen >= TEXT_LENGTH)
	{
		return;
	}
	
	packet->h.header.type					= CMD_CHAT_DATA;
	packet->h.header.size = sizeof(t_server_chat_data)-TEXT_LENGTH+iLen+1;

	packet->u.server_chat_data.server_id	= cn;
	packet->u.server_chat_data.type			= type;
	packet->u.server_chat_data.length		= iLen;///다행히 위에서 255로 막는다

	t_connection *pConnect = &connections[cn];
	for( int i = 0; i < MAX_AREA_BUFFER; i++ ) 
	{
		if( TRUE != pConnect->area_tbl[i].bAlive){continue;}
		if( pConnect->area_tbl[i].Type == 1
		&&	pConnect->dwAgentConnectionIndex) 
		{
			const DWORD dwTargetAgentConnectionIndex = connections[pConnect->area_tbl[i].ID].dwAgentConnectionIndex;
			const DWORD dwTargetID = connections[pConnect->area_tbl[i].ID].dwIDForAgent;
			// Modified by chan78 at 2000/12/13 :: 패치됨...
			if( g_pServerTable->GetPackedMsg( dwTargetAgentConnectionIndex ) )
			{
				if( g_pServerTable->GetPackedMsg( dwTargetAgentConnectionIndex )->GetUserNum() == 0 ) 
				{
					g_pServerTable->GetPackedMsg( dwTargetAgentConnectionIndex )->SetMsg( (char*)packet, (sizeof(t_header)+packet->h.header.size) );
				}
				g_pServerTable->GetPackedMsg( dwTargetAgentConnectionIndex )->AddUser( dwTargetID );
			} 
			else 
			{
//					MyLog( LOG_FATAL, "Oops!!!(CI:%d, CN:%d)'s ->GetPackedMsg() is NULL", PC_TABLE[i].ID, connections[PC_TABLE[i].ID].dwAgentConnectionIndex );
				MyLog( LOG_FATAL, "Oops!!!(CI:%d, CN:%d)'s ->GetPackedMsg() is NULL", dwTargetAgentConnectionIndex, dwTargetID );
			}
		}
	}
	g_pServerTable->SendPackedMsg();
}
	
extern int g_AI_Select;	// LTS AI

void RemoveNPCControlData(int cn,int NPC_ID)	// LTS NPC BUG
{
	for (int i = 0; i < MAX_PC_CONTROL_NPC; ++i)
	{
		if (connections[cn].chrlst.ctrlnpc[i] == NPC_ID)
		{
			connections[cn].chrlst.ctrlnpc[i] = -1;
			NPCList[NPC_ID].ctrlpc = -1; //문제가 생길지 모른다.
			return;
		}
	}
}

void AddNPCControlData(int cn,int NPC_ID)		// LTS NPC BUG
{
	
}

void CheckNPCControlData(int cn,int NPC_ID)	// LTS NPC BUG	// 2번째 인자 방금 죽은 NPC
{
	for (int i = 0; i < NPC_COUNT; ++i)
	{	// 내영역에 AI할당 못받은놈들...을 재할당 
		if (NPC_TABLE[i].X >= connections[cn].chrlst.X-ADD_AREA_X && 
			NPC_TABLE[i].X <= connections[cn].chrlst.X+ADD_AREA_X && 
			NPC_TABLE[i].Y >= connections[cn].chrlst.Y-ADD_AREA_Y && 
			NPC_TABLE[i].Y <= connections[cn].chrlst.Y+ADD_AREA_Y)
		{	
			if (!NPCList[NPC_TABLE[i].ID].IsDead())
			{
				if (NPCList[NPC_TABLE[i].ID].ctrlpc == -1)
				{
					AddNPCControlData(cn,NPC_TABLE[i].ID);
				}
			}
		}
	}
	
}

extern void CheckDungeonObject( int id, short int cn );	// 020620 YGI 

int SendAddArea(t_connection c[], int cn, int type, int id)
{	
	t_packet packet;
	
	if (type == 1)
	{
		if (c[id].chrlst.bAlive == REMOVE_)
		{
			return 0;
		}

		packet.h.header.type = CMD_USER_ADD;
		packet.h.header.size = sizeof(t_server_user_add);
		UserAddBasicData(c, id, cn, &packet.u.server_user_add); // NPC_Pattern.cpp
		QueuePacket(c, cn, &packet, 1);
		
		if (c[id].chrlst.MovePathCount) // 현재 이동하고 있는 중이면....
		{
			if (SettingMoveData_(id, &c[id].chrlst, &packet))
			{
				packet.h.header.type = CMD_MOVE;
				QueuePacket(c, cn, &packet, 1);
			}
		}
	}	
	else if (type == 2)
	{	
		if (NPCList[id].bAlive == REMOVE_) 
		{
			return 0;
		}
		
		packet.h.header.type = CMD_NPC_ADD;
		packet.h.header.size = sizeof(t_server_npc_add);
		NPCAddBasicData(id, &packet.u.server_npc_add);
		QueuePacket(c, cn, &packet, 1);
		
		if (NPCList[id].patterntype == NPC_PATTERN_TAME_)
		{
			SendHostName(cn, &NPCList[id]);
		}
		
		if (NPCList[id].MovePathCount) // 현재 이동하고 있는 중이면....	// LTS NPC BUG
		{
			if (SettingMoveData_(id + 10000, &NPCList[id], &packet))
			{
				packet.h.header.type = CMD_MOVE;
				QueuePacket(c, cn, &packet, 1);
			}
		}

		if (NPCList[id].patterntype != NPC_PATTERN_NO_MOVE_)
		{
			if (NPCList[id].ctrlpc == -1)
			{
				if (!NPCList[id].ChairNum)				// LTS AI2   // 서버 담당 AI이면 컨트롤 할 필요가 없다.
				{
					
					for (int i = 0 ; i < MAX_PC_CONTROL_NPC; ++i)
					{
						if (c[cn].chrlst.ctrlnpc[i] == -1)
						{
							SendNPCControlData(cn, id);
							c[cn].chrlst.ctrlnpc[i]	= id;
							NPCList[ id].ctrlpc	= cn;
							break;
						}
					}	
				}
			}
		}
	}
	else if (type == 3)
	{	
		if (ItemList[ id].bAlive == 0) 
		{
			return 0;
		}
		//이 아래가 다 바뀌었습니다... this lsw 
		t_server_item_add* pi = &packet.u.server_item_add;
		pi->item_id			= id;
		pi->item.item_no	= ItemList[id].item_no;
		pi->item.attr[0]	= ItemList[id].attr[0];
		pi->item.attr[1]	= ItemList[id].attr[1];
		pi->item.attr[2]	= ItemList[id].attr[2];
		pi->item.attr[3]	= ItemList[id].attr[3];
		pi->item.attr[4]	= ItemList[id].attr[4];
		pi->item.attr[5]	= ItemList[id].attr[5];
		pi->offx			= ItemList[id].offx;
		pi->offy			= ItemList[id].offy;
		pi->x				= ItemList[id].x;
		pi->y				= ItemList[id].y;
		// 020620 YGI 
		CheckDungeonObject(id,cn);
		BYTE itempack[50];
		int iZipSize= Zip((BYTE*)pi, sizeof(t_server_item_add), itempack, 100);
		t_server_item_add_ziped* pii = &packet.u.server_item_add_ziped;
		pii->zipedpack[0] = iZipSize;
		
		packet.h.header.type = CMD_ITEM_ADD_ZIPED;		//압축된 아이템 패킷
		packet.h.header.size = iZipSize + 1;
		memcpy(pii->zipedpack,itempack,iZipSize);
		QueuePacket(c, cn, &packet, 1);
	}
	
	return 1;
}

int Zip( BYTE *pbIn, int iSize, BYTE *pbOut, int iMax)
{	// this lsw 패킷 압축(?) 함수 //클라이언트에는 unzip 함수 필요
	int iStatusSize = (iSize - 1)/8 + 1;
	pbOut[0] = iStatusSize;
	BYTE cFlag = 0x01;

	int iStatusPos = 1;				// output buf의 flag index
	int iOutPos = iStatusSize + 1;	// output buf의 index
	memset(&pbOut[1], 0, iStatusSize);

	for (int i = 0; i < iSize; ++i)
	{
		if (pbIn[i])
		{
			pbOut[iStatusPos] |= cFlag;
			pbOut[iOutPos++] = pbIn[i];
		}

		cFlag = cFlag << 1;

		if (!cFlag)
		{
			cFlag = 0x01;
			++iStatusPos;
		}
	}

	return iOutPos;
}

int SendRemoveArea(t_connection c[], int cn, int type, int id)
{
	switch (type)
	{
	case 1:
		{
			t_packet packet;
			packet.h.header.type = CMD_USER_REMOVE;
			packet.h.header.size = sizeof(t_server_user_remove);
			packet.u.server_user_remove.server_id = id;
			QueuePacket(c, cn, &packet, 1);
			break;
		}
	case 2:
		{
			t_packet packet;
			packet.h.header.type = CMD_NPC_REMOVE;
			packet.h.header.size = sizeof(t_server_npc_remove);
			packet.u.server_npc_remove.server_id = id + 10000;
			QueuePacket(c, cn, &packet, 1);
			
			if (NPCList[id].ctrlpc == cn)
			{
				NPCList[id].ctrlpc = -1;
				
				for (int i = 0 ; i < MAX_PC_CONTROL_NPC ; ++i)
				{
					if (c[cn].chrlst.ctrlnpc[i] == id)
					{
						c[cn].chrlst.ctrlnpc[i] = -1;
					}
				}
			}

			break;
		}
	case 3:
		{
			t_packet packet;
			packet.h.header.type = CMD_ITEM_REMOVE;
			packet.h.header.size = sizeof(t_server_item_remove);
			packet.u.server_item_remove.item_id	= id;
			QueuePacket(c, cn, &packet, 1);
			break;
		}
	}

	return 1;
}

int GetAreaTableCount(t_connection c[], int cn, int type)
{
	if (type == 0) // PC
	{
		for (int i = 0; i < MAX_AREA_BUFFER; ++i)
		{
			if (c[cn].area_tbl[i].bAlive == FALSE)
			{
				return i;
			}
		}
	}
	else // NPC
	{
		for (int i = 0; i < MAX_AREA_BUFFER; ++i)
		{
			if (NPCList[cn].area_tbl[i].bAlive == FALSE)
			{
				return i;
			}
		}
	}
	
	return -1;
}

int AddAreaNPCTable(t_connection c[], int cn, int type, int id)
{
	for (int i = 0; i < MAX_AREA_BUFFER; ++i)
	{
		if (NPCList[id].area_tbl[i].bAlive == TRUE)
		{
			if (NPCList[id].area_tbl[i].Type == 1 && NPCList[id].area_tbl[i].ID == cn)
			{
				return 0;
			}
		}
	}

	int cnt = GetAreaTableCount(c, id, 1);

	if (cnt < 0)
	{
		return -1;
	}

	NPCList[id].area_tbl[cnt].Type = 1;
	NPCList[id].area_tbl[cnt].ID = cn;
	NPCList[id].area_tbl[cnt].bAlive = TRUE;
	return 1;
}

int AddAreaTable(t_connection c[], int cn, int type, int id)
{	
	if (type == 2) // NPC
	{
		AddAreaNPCTable(c, cn, type, id);
	}
	
	for (int i = 0; i < MAX_AREA_BUFFER; ++i)
	{
		if (c[cn].area_tbl[i].bAlive == TRUE)
		{
			if (c[cn].area_tbl[i].Type == type && c[cn].area_tbl[i].ID == id)
			{
				return 0;
			}
		}
	}
	
	const int cnt = GetAreaTableCount(c, cn, 0);

	if (cnt < 0)
	{
		return -1;
	}

	c[cn].area_tbl[cnt].Type = type;
	c[cn].area_tbl[cnt].ID = id;
	c[cn].area_tbl[cnt].bAlive = TRUE;
	SendAddArea(c, cn, type, id);
	
	return 1;
}

int CheckAreaPC(t_connection c[], int cn, int ID)
{
	if (c[ID].chrlst.X >= c[cn].chrlst.X-ADD_AREA_X && 
	    c[ID].chrlst.X <= c[cn].chrlst.X+ADD_AREA_X &&
	    c[ID].chrlst.Y >= c[cn].chrlst.Y-ADD_AREA_Y &&
	    c[ID].chrlst.Y <= c[cn].chrlst.Y+ADD_AREA_Y)
	{
		if (AddAreaTable(c, cn, 1, ID) < 0)
		{
			return -1;
		}

		return 1;
	}

	return 0;
}
	
int CheckAreaAllPC(t_connection c[], int cn)
{	
	for (int i = 0; i < PC_COUNT; ++i)
	{
		if (PC_TABLE[i].ID != cn)
		{
			if (CheckAreaPC(c, cn, PC_TABLE[i].ID) == 1)
			{
				AddAreaTable(c, PC_TABLE[i].ID, 1, cn);
			}
		}
	}
	
	for (int j = 0; j < NPC_COUNT; ++j)
	{
		if (NPC_TABLE[j].X >= c[cn].chrlst.X-ADD_AREA_X && 
			NPC_TABLE[j].X <= c[cn].chrlst.X+ADD_AREA_X && 
			NPC_TABLE[j].Y >= c[cn].chrlst.Y-ADD_AREA_Y && 
			NPC_TABLE[j].Y <= c[cn].chrlst.Y+ADD_AREA_Y)
		{
			if (AddAreaTable(c, cn, 2, NPC_TABLE[j].ID) < 0)
			{
				break;
			}
		}
	}
	
	for (int k = 0; k < ITEM_COUNT; ++k)
	{
		if (ITEM_TABLE[k].X >= c[cn].chrlst.X-ADD_AREA_X && 
		    ITEM_TABLE[k].X <= c[cn].chrlst.X+ADD_AREA_X && 
		    ITEM_TABLE[k].Y >= c[cn].chrlst.Y-ADD_AREA_Y && 
		    ITEM_TABLE[k].Y <= c[cn].chrlst.Y+ADD_AREA_Y)
		{
			if (AddAreaTable(c, cn, 3, ITEM_TABLE[k].ID) < 0)
			{
				break;
			}
		}
	}
	
	return 1;
}

void RemoveAllPC(t_connection c[], int cn)
{
	for (int i = 0; i < PC_COUNT; ++i)
	{
		if (PC_TABLE[i].ID != cn)
		{
			for (int j = 0; j < MAX_AREA_BUFFER; ++j)
			{
				if (c[PC_TABLE[i].ID].area_tbl[j].bAlive == TRUE)
				{
					if (c[PC_TABLE[i].ID].area_tbl[j].Type == 1 && c[PC_TABLE[i].ID].area_tbl[j].ID == cn)
					{
						c[PC_TABLE[i].ID].area_tbl[j].bAlive = FALSE;
						SendRemoveArea(c, PC_TABLE[i].ID, 1, cn);
					}
				}
			}
		}
	}
}

int RemoveAreaAllPC(t_connection c[], int cn)
{
	for (int i = 0; i < MAX_AREA_BUFFER; ++i)
	{
		int x = 0, y = 0;

		if (c[cn].area_tbl[i].bAlive == TRUE)
		{
			if (c[cn].area_tbl[i].Type == 1) // PC
			{
				x = c[c[cn].area_tbl[i].ID].chrlst.X;
				y = c[c[cn].area_tbl[i].ID].chrlst.Y;
				
				if (x <= c[cn].chrlst.X-REM_AREA_X ||
					x >= c[cn].chrlst.X+REM_AREA_X ||
					y <= c[cn].chrlst.Y-REM_AREA_Y ||
					y >= c[cn].chrlst.Y+REM_AREA_Y)
				{
					c[cn].area_tbl[i].bAlive = FALSE;
					SendRemoveArea(c, cn, 1, c[cn].area_tbl[i].ID);
					
					for (int j = 0; j < MAX_AREA_BUFFER; ++j)
					{
						if (c[c[cn].area_tbl[i].ID].area_tbl[j].bAlive == TRUE)
						{
							if (c[c[cn].area_tbl[i].ID].area_tbl[j].Type == 1 && c[c[cn].area_tbl[i].ID].area_tbl[j].ID == cn)
							{
								c[c[cn].area_tbl[i].ID].area_tbl[j].bAlive = FALSE;
								SendRemoveArea(c, c[cn].area_tbl[i].ID, 1, cn);
							}
						}
					}
				}
			}
			else if(c[cn].area_tbl[i].Type == 2) // NPC
			{
				x = NPCList[c[cn].area_tbl[i].ID].X;
				y = NPCList[c[cn].area_tbl[i].ID].Y;
				
				if (x <= c[cn].chrlst.X-REM_AREA_X ||
					x >= c[cn].chrlst.X+REM_AREA_X ||
					y <= c[cn].chrlst.Y-REM_AREA_Y ||
					y >= c[cn].chrlst.Y+REM_AREA_Y)
				{
					c[cn].area_tbl[i].bAlive = FALSE;
					SendRemoveArea(c, cn, 2, c[cn].area_tbl[i].ID);
				}
			}
			else if(c[cn].area_tbl[i].Type == 3) // ITEM
			{
				x = ItemList[c[cn].area_tbl[i].ID].x;
				y = ItemList[c[cn].area_tbl[i].ID].y;
				
				if (x <= c[cn].chrlst.X-REM_AREA_X ||
					x >= c[cn].chrlst.X+REM_AREA_X ||
					y <= c[cn].chrlst.Y-REM_AREA_Y ||
					y >= c[cn].chrlst.Y+REM_AREA_Y)
				{
					c[cn].area_tbl[i].bAlive = FALSE;
					SendRemoveArea(c, cn, 3, c[cn].area_tbl[i].ID);
				}
			}
			else
			{
				continue;
			}
		}
	}
	
	return 1;
}

int CheckAreaAllNPC(t_connection c[], int n)
{
	for (int i = 0; i < PC_COUNT; ++i)
	{
		if (NPCList[n].X >= c[PC_TABLE[i].ID].chrlst.X-ADD_AREA_X && 
		    NPCList[n].X <= c[PC_TABLE[i].ID].chrlst.X+ADD_AREA_X && 
		    NPCList[n].Y >= c[PC_TABLE[i].ID].chrlst.Y-ADD_AREA_Y && 
			NPCList[n].Y <= c[PC_TABLE[i].ID].chrlst.Y+ADD_AREA_Y)
		{					   
			AddAreaTable(c, PC_TABLE[i].ID, 2, n);
		}
	}

	return 0;
}

int RemoveAllNPC(t_connection c[], int n)
{	// 모든 PC에서 NPC n을 제거한다. 
	for (int i = 0; i < PC_COUNT; ++i)
	{
		for (int j = 0; j < MAX_AREA_BUFFER; ++j)
		{
			if (c[PC_TABLE[i].ID].area_tbl[j].bAlive == TRUE)
			{
				if (c[PC_TABLE[i].ID].area_tbl[j].Type == 2 && c[PC_TABLE[i].ID].area_tbl[j].ID == n)
				{
					c[PC_TABLE[i].ID].area_tbl[j].bAlive = FALSE;
					SendRemoveArea(c, PC_TABLE[i].ID, 2, n);
				}
			}
		}
	}

	return 0;
}

int RemoveAreaAllNPC(t_connection c[], int n)
{
	for (int i = 0; i < MAX_AREA_BUFFER; ++i)
	{
		if (NPCList[n].area_tbl[i].bAlive == TRUE)
		{
			const int cn = NPCList[n].area_tbl[i].ID;
			const int x  = c[cn].chrlst.X;
			const int y  = c[cn].chrlst.Y;
			
			if (NPCList[n].X <= x-REM_AREA_X || 
				NPCList[n].X >= x+REM_AREA_X || 
				NPCList[n].Y <= y-REM_AREA_Y || 
				NPCList[n].Y >= y+REM_AREA_Y)
			{
				NPCList[n].area_tbl[i].bAlive = FALSE;
				SendRemoveArea(c, cn, 2, n);
				
				for (int j = 0; j < MAX_AREA_BUFFER; j ++)
				{
					if (c[cn].area_tbl[j].bAlive == TRUE)
					{
						if (c[cn].area_tbl[j].Type == 2 && c[cn].area_tbl[j].ID == n)
						{
							c[cn].area_tbl[j].bAlive = FALSE;
							SendRemoveArea(c, cn, 2, n);
						}
					}
				}
			}
		}
	}
	
	return 0;
}

int CheckAreaAllItem(t_connection c[], int n)
{
	for (int i = 0; i < PC_COUNT; ++i)
	{
		if (ItemList[n].x >= c[PC_TABLE[i].ID].chrlst.X-ADD_AREA_X && 
			ItemList[n].x <= c[PC_TABLE[i].ID].chrlst.X+ADD_AREA_X && 
			ItemList[n].y >= c[PC_TABLE[i].ID].chrlst.Y-ADD_AREA_Y && 
			ItemList[n].y <= c[PC_TABLE[i].ID].chrlst.Y+ADD_AREA_Y)
		{
			AddAreaTable(c, PC_TABLE[i].ID, 3, n);
		}
	}
	
	return 0;
}

int CommandAllItem(t_connection c[], int n, int command)
{
	for (int i = 0; i < PC_COUNT; ++i)
	{
		for (int j = 0; j < MAX_AREA_BUFFER; ++j)
		{
			if (c[PC_TABLE[i].ID].area_tbl[j].bAlive == TRUE)
			{	// Type이 아이템이면서 n번째 Item이면...
				if (c[PC_TABLE[i].ID].area_tbl[j].Type == 3 && c[PC_TABLE[i].ID].area_tbl[j].ID == n)
				{
					switch (command)
					{
					case CHANGE_IA2_DURATION_ITEM_AREA:
						{
							SendItemAttrChange(c, PC_TABLE[i].ID, n, IATTR_DURATION);
							break;
						}
					case CHANGE_IA2_LIMIT_ITEM_AREA:
						{
							SendItemAttrChange(c, PC_TABLE[i].ID, n, IATTR_LIMIT);
							break;
						}
					case CHANGE_IA2_ATTR_ITEM_AREA:
						{
							SendItemAttrChange(c, PC_TABLE[i].ID, n, IATTR_ATTR);
							break;
						}
					case CHANGE_IA2_ID_ITEM_AREA:
						{
							SendItemAttrChange(c, PC_TABLE[i].ID, n, IATTR_ID);
							break;
						}
					case CHANGE_IA2_RESERVED0_ITEM_AREA:
						{
							SendItemAttrChange( c, PC_TABLE[i].ID, n, IATTR_RESERVED0 );
							break;
						}
					case CHANGE_IA2_RESERVED1_ITEM_AREA:
						{
							SendItemAttrChange(c, PC_TABLE[i].ID, n, IATTR_RESERVED1);
							break;
						}
					case ITEM_EXPLOTION_AREA:
						{
							SendItemExplotion( PC_TABLE[i].ID, n );
							break;
						}
					}
				}
			}
		}
	}
	
	return 0;
}

int RemoveAllItem(t_connection c[], int n)
{
	for (int i = 0; i < PC_COUNT; ++i)
	{
		for (int j = 0; j < MAX_AREA_BUFFER; ++j)
		{
			if (c[PC_TABLE[i].ID].area_tbl[j].bAlive == TRUE)
			{
				if (c[PC_TABLE[i].ID].area_tbl[j].Type == 3 && c[PC_TABLE[i].ID].area_tbl[j].ID == n)
				{
					c[PC_TABLE[i].ID].area_tbl[j].bAlive = FALSE;
					SendRemoveArea(c, PC_TABLE[i].ID, 3, n);
				}
			}
		}
	}
	
	return 0;
}

int SetArea(int Cmd, int Id)
{
	if (CMD_AREA_COUNT >= MAX_CMD_AREA)
	{
		return -1;
	}

	CMD_AREA_TABLE[CMD_AREA_COUNT].Cmd = Cmd;
	CMD_AREA_TABLE[CMD_AREA_COUNT].ID  = Id;	
	++CMD_AREA_COUNT;
					
	return 1;
}

void RunArea(t_connection c[])
{	//< CSD-CN-031213
	PC_COUNT = 0;

	CUserManager::HASH_USER mpUser = g_pUserManager->GetUserSet();

	for (CUserManager::ITOR_USER i = mpUser.begin(); i != mpUser.end(); ++i)
	{
		const int idUser = i->second;
		CHARLIST* pUser = CheckServerId(idUser);

		if (pUser != NULL)
		{	
			PC_TABLE[PC_COUNT].ID = idUser;
			PC_TABLE[PC_COUNT].X = pUser->X;
			PC_TABLE[PC_COUNT].Y = pUser->Y;
			++PC_COUNT;
		}	
	}
			
	NPC_COUNT = 0;

	for (int j = NPC_LIST_START; j < MAX_NPC_LIST; ++j)
	{	// 없는 상태( REMOVE_)이면 당연히 포함할필요 없도 BUFE_상태 즉, 
		// 클라이언트에서 NPC가 사라지는 시간을 만들기 위함 Term을 위해 BUFE_상태또한 포함한다. 
		CHARLIST* pNpc = CheckNpcId(j);

		if (pNpc != NULL && pNpc->bAlive != REMOVE_ && pNpc->bAlive != BUFE_)
		{	
			NPC_TABLE[NPC_COUNT].ID = j;
			NPC_TABLE[NPC_COUNT].X	= pNpc->X;
			NPC_TABLE[NPC_COUNT].Y	= pNpc->Y;	
			++NPC_COUNT;
		}	
	}
				
	ITEM_COUNT = 0;
				
	for (int k = 0; k < MAX_ITEM_LIST; ++k)
	{		
		if (ItemList[k].bAlive == TRUE)
		{	
			ITEM_TABLE[ITEM_COUNT].ID = k;
			ITEM_TABLE[ITEM_COUNT].X = ItemList[k].x;
			ITEM_TABLE[ITEM_COUNT].Y = ItemList[k].y;
			++ITEM_COUNT;
		}	
	}		
				
	for (int l = 0; l < CMD_AREA_COUNT; ++l)
	{
		switch (CMD_AREA_TABLE[l].Cmd)
		{
		case ADD_PC_AREA:
			{
				CheckAreaAllPC(c, CMD_AREA_TABLE[l].ID);
				break;
			}
		case MOVE_PC_AREA:
			{
				CheckAreaAllPC(c, CMD_AREA_TABLE[l].ID);
				RemoveAreaAllPC(c, CMD_AREA_TABLE[l].ID);
				break;
			}
		case REMOVE_PC_AREA:
			{
				RemoveAllPC(c, CMD_AREA_TABLE[l].ID);
				break;
			}
		case ADD_NPC_AREA:
			{
				CheckAreaAllNPC(c, CMD_AREA_TABLE[l].ID);
				break;
			}
		case MOVE_NPC_AREA:
			{
				CheckAreaAllNPC(c, CMD_AREA_TABLE[l].ID);
				RemoveAreaAllNPC(c, CMD_AREA_TABLE[l].ID);
				break;
			}
		case REMOVE_NPC_AREA:
			{
				RemoveAllNPC(c, CMD_AREA_TABLE[l].ID);
				break;
			}
		case ADD_ITEM_AREA:
			{
				CheckAreaAllItem(c, CMD_AREA_TABLE[l].ID);
				break;
			}
		case CHANGE_IA2_DURATION_ITEM_AREA:
		case CHANGE_IA2_LIMIT_ITEM_AREA:
		case CHANGE_IA2_ATTR_ITEM_AREA:
		case CHANGE_IA2_ID_ITEM_AREA:
		case CHANGE_IA2_RESERVED0_ITEM_AREA:
		case CHANGE_IA2_RESERVED1_ITEM_AREA:
		case ITEM_EXPLOTION_AREA:
			{
				CommandAllItem(c, CMD_AREA_TABLE[l].ID, CMD_AREA_TABLE[l].Cmd);
				break;
			}
		case REMOVE_ITEM_AREA:
			{
				RemoveAllItem(c, CMD_AREA_TABLE[l].ID);
				break;
			}
		}
	}

	CMD_AREA_COUNT = 0;
}	//> CSD-CN-031213