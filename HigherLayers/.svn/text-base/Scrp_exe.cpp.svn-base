#include "..\stdafx.h"
#include "..\LowerLayers\mylog.h"
#include "..\LowerLayers\servertable.h"

#include "Map.h"
#include "MenuDefine.h"
#include "Chatting.h"
#include "Scrp_int.h"
#include "quest_cpp.h"
#include "OP_magic.h" 
#include "citem.h"
#include "SealStoneGameServer.h"
#include "Scrp_exe.h"
#include "NationSys.h"	// 010904 LTS
#include "teambattle.h"
#include "LTSSupport.h"
#include "QuestFunction.h"	// 021015 kyo
#include "eventmgr.h"
#include "SymbolItemMgr.h"	// 030419 kyo
#include "ArenaManager.h"
#include "BattleManager.h"
#include "UserManager.h"
#include "KickOffAll.h"		// BBD 040329

int var[ DRAGON_MAX_CONNECTIONS_][ _SCRIPT_VARABLE_POINT_ ] = {0,};
char String[ MAX_STRING_]  = {0,};

int EventPC, EventNPC[ DRAGON_MAX_CONNECTIONS_] = {0,};

std::vector<Npc_Quest_Item> NpcQuestItem;

extern bool g_IsSQuestATM;

extern void DeleteDoublePill(t_connection c[], int cn); // Add by rogerio
extern void UpdateCharacterData(t_connection c[], int cn, bool bDirect = false);
extern void SendCMD_MAGIC_POSIFTION( t_server_magic_position *p);
extern void SendCharInfoSkill( t_connection c[], int cn );
extern void makemyjobexpsetting( int skillno , CHARLIST *ch );
extern void SendLearnSkillOk( int skill_no, t_connection c[], int cn );
extern void RecvCMD_CLIENT_CHARACTER_DEMAND( int cn, t_client_character_demand *p ); 

extern CKickOffAll	g_KickMgr;	// BBD 040329
extern bool isLevelUp(CHARLIST*); //Eleval

extern void getItemIndex(int, int &, int &) ; // For use with sLearnItem 

void LogInFile( string szFilePath,  char *szFormat, ... ) //kyo
{
	FILE *fp;
	if(!(fp=fopen(szFilePath.c_str(), "a+")))
	{
		return;
	}

	char szMessage[512];//
	va_list val;
	va_start(val, szFormat);
	vsprintf_s(szMessage, sizeof(szMessage), szFormat, val);
	va_end(val);

	time_t nowTime;
	struct tm *now;
	time( &nowTime );
	now = localtime(&nowTime);
	int year = 0, mon = 0, day = 0;
	int hour = 0, min = 0, sec = 0;
	// Make it usable.
	
	year = now->tm_year + 1900;
	mon  = now->tm_mon + 1;
	day  = now->tm_mday;
#ifdef BUFF_BUG_CHECKS // Finito 24/08/07 buff bug checks to take away 12 hours when time changes
	if (now->tm_hour - g_hour >= 12  && g_hour != -1)
	{
		hour = now->tm_hour - 12;
	}
	else
	{
		hour = now->tm_hour;
	}
#else
	hour = now->tm_hour;
#endif
	min  = now->tm_min;
	sec  = now->tm_sec;
	char szLog[512];
	sprintf_s( szLog, sizeof(szLog), "%d %d/%d %d:%d:%d\t%s\n", year, mon, day, hour, min, sec, szMessage);

	if( !fputs( szLog, fp ) )
	{
		fclose(fp);
		return;
	}

	fclose(fp);			
}

void ReCallScript( void )
{	//< CSD-CN-031213
	CUserManager::HASH_USER mpUser = g_pUserManager->GetUserSet();

	for (CUserManager::ITOR_USER i = mpUser.begin(); i != mpUser.end(); ++i)
	{
		const int idUser = i->second;

		if (!var[idUser][1])
		{
			continue;
		}

		CHARLIST* pUser = CheckServerId(idUser);
		
		if (pUser != NULL)
		{
			if (var[idUser][3] < g_curr_time )
			{
				EventPC = idUser;
				Script(var[idUser][1]);
			}
		}
	}
}	//> CSD-CN-031213

int SearchItemByInv( int item_no, CHARLIST *ch )
{
	
	for( int a=0; a<3; a++ )
		for( int b=0; b<3; b++ )
			for( int c=0; c<8; c++ )
			{
				if( ch->inv[a][b][c].item_no == item_no ) 
				{
					return 1;
				}
			}
			
			return 0;
}

int SearchItemByInv( int item_no, CHARLIST *ch, int &a, int &b, int &c )
{
	for( a=0; a<3; a++ )
		for( b=0; b<3; b++ )
			for( c=0; c<8; c++ )
			{
				if( ch->inv[a][b][c].item_no == item_no ) 
				{
					return 1;
				}
			}
			
			return 0;
}


int SearchItemByEquip( int item_no, CHARLIST *ch )
{
	for( int a=0; a<8; a++ )
	{
		if( ch->equip[a].item_no == item_no ) return 1;
	}
	
	return 0;
}

int SearchItemByQuick( int item_no, CHARLIST *ch )
{
	for( int a=0; a<6; a++ )
	{
		if( ch->quick[a].item_no == item_no ) return 1;
	}
	
	return 0;
}

// 021107 YGI
int SendDial( short int cn, int npc_index, char *msg, int length )
{
	if( !CheckServerIndex( cn ) ) return 0;
	
	t_packet packet;
	packet.h.header.type = CMD_CHAT_DATA;
	{
		packet.u.server_chat_data.server_id = npc_index+10000;
		packet.u.server_chat_data.type		= (unsigned char)CHATTYPE_NORMAL;
		packet.u.server_chat_data.length = (unsigned char)length;
		memset(packet.u.server_chat_data.text, 0, TEXT_LENGTH );
		memcpy(packet.u.server_chat_data.text, msg, length);
	}
	packet.h.header.size = sizeof(t_server_chat_data)-TEXT_LENGTH+length;
	QueuePacket(connections, cn, &packet, 1);
	return 1;
}


/*­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å
ÇÔ¼ö¸í : void sDialogD( void )
¼³  ¸í : ´ëÈ­¸¦ ÇÏ´Âµ¥ ´ëÈ­ÇÏ´Â ÀÚÀÇ ¹æÇâÀÌ ÁÖÀÎ°øÀ» ÇâÇÑ´Ù.
ÀÎ  ÀÚ :
¸®  ÅÏ :
­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å­å*/
void sDialOnlyPC( void )// 0
{		
	int length;
	
	// Client¿¡ ´ëÈ­¸¦ º¸³»ÁØ´Ù....
	GetString( String );
	length = strlen( String );
	SendDial( EventPC, EventNPC[EventPC], String, strlen( String ) );
	
	
	
	return;
}		

// Added by chan78 at 2000/11/28
void sSendMessageBox()			// 001212 YGI
{
	int length;
	t_packet packet;
	
	// Client¿¡ ´ëÈ­¸¦ º¸³»ÁØ´Ù....
	GetString( String );
	
	length = strlen( String );
	packet.h.header.type = CMD_SCRIPT_MESSAGE_BOX;
	{
		memcpy(packet.u.kein.default_msg, String, length);
	}
	packet.h.header.size = length+1;
	
	if(connections[EventPC].dwAgentConnectionIndex && connections[ EventPC].state >= CONNECT_JOIN)
		QueuePacket(connections, EventPC, &packet, 1);
	
	return;
}

void sSendSound()			// 001222 YGI
{
	t_packet packet;
	// Client¿¡ ´ëÈ­¸¦ º¸³»ÁØ´Ù....
	int type = GetInt();
	int sound = GetInt();
	
	CHARLIST *ch = CheckServerId( EventPC );
	if( !ch ) return;
	
	packet.h.header.type = CMD_SEND_SOUND;
	{
		packet.u.kein.default_short_int = sound;
	}
	packet.h.header.size = sizeof( short int );
	
	switch (type)
	{	//< CSD-CN-031213
	case 0:	
		{	// ÀÚ±âÀÚ½ÅÇÑÅ×¸¸
			QueuePacket(connections, EventPC, &packet, 1); 
			break;		
		}
	case 1:		
		{	// ÁÖº¯»ç¶÷µé±îÁö
			CastMe2Other(EventPC, &packet);
			QueuePacket(connections, EventPC, &packet, 1); 
			break;
		}
	case 2:
		{	// ÇöÀç ¸Ê
			g_pUserManager->SendPacket(&packet);
			break; 
		}
	case 3:
		{
			g_pUserManager->SendPacket(&packet);
			SendNationPacket(&packet, ch->name_status.nation);	
			break;
		}
	case 4:
		{	// ¸ğµç¸Ê
			g_pUserManager->SendPacket(&packet);
			g_pServerTable->BroadCastToEveryServer((char*)&packet, (sizeof(t_header)+packet.h.header.size), SERVER_TYPE_MAP);
			break;
		}
	}	//> CSD-CN-031213
}

void sSendEffect()			// 010522 YGI
{
	int type = GetInt();
	int effect = GetInt();
	CHARLIST *ch = CheckServerId( EventPC );
	if( !ch ) return;
	
	t_packet p;
	p.h.header.type = CMD_SEND_EFFECT;
	p.u.kein.send_effect.effect_num = effect;
	p.u.kein.send_effect.target_id = EventPC;
	// 040105 YGI º¸¹°
	p.u.kein.send_effect.x = 0;
	p.u.kein.send_effect.y = 0;
	p.h.header.size = sizeof( k_send_effect );
	
	QueuePacket(connections, EventPC, &p, 1);
	
	if( type == 1 )	// ÁÖº¯»ç¶÷¿¡°Ôµµ
	{
		CastMe2Other( EventPC, &p );
	}
}


void sDialInAreaPC( void )
{
	GetString( String );
}

void sDialEveryone( void )
{
	GetString( String );
}


void sAddItem( void )// 1
{
	int x, y;
	ItemAttr i;
	
	
	i.item_no = GetInt();
	i.attr[0] = GetInt();
	i.attr[1] = GetInt();
	i.attr[2] = GetInt();
	i.attr[3] = GetInt();
	i.attr[4] = GetInt();
	i.attr[5] = GetInt();
	
	x =		 GetInt();
	y =		 GetInt();
	
	AddItemList(i.item_no, i.attr, 0, x, y, 0,0);
}


void sAddNpc( void)// 2
{
	int npcindex, x, y;
	
	npcindex =	GetInt();
	x =			GetInt();
	y =			GetInt();		  
	
	AddNPCList( npcindex, x, y );
}

void sSetQuestNumStep(void)
{		
	int quest_no=GetInt();
	int quest_value=GetInt();
	//	EventPC		// ½ºÅ©¸³Æ®¸¦ ÁøÇàÇÏ°í ÀÖ´Â Ä³¸¯ÅÍ ¾ÆÀÌµğ¸¦ °¡Áö°í ÀÖ´Ù.
	//	connections	// ÇöÀç ¼­¹ö¿¡ Á¢¼ÓÇØÀÖ´Â Å¬¶óÀÌ¾ğÆ® ±¸Á¶Ã¼ ( Ä³¸¯ÅÍ Á¤º¸µµ °¡Áö°í ÀÖ´Ù. )
	
	//	CHARLIST *ch = &connections[EventPC].chrlst;
	//	if( !ch ) return;
	
	var[EventPC][quest_no]=quest_value;
	//return ch->var[quest_no] = quest_value;
	
}

void sSetQuestNumStep(int quest_no,int quest_value)
{	
	// EventPC		// ½ºÅ©¸³Æ®¸¦ ÁøÇàÇÏ°í ÀÖ´Â Ä³¸¯ÅÍ ¾ÆÀÌµğ¸¦ °¡Áö°í ÀÖ´Ù.
	// connections	// ÇöÀç ¼­¹ö¿¡ Á¢¼ÓÇØÀÖ´Â Å¬¶óÀÌ¾ğÆ® ±¸Á¶Ã¼ ( Ä³¸¯ÅÍ Á¤º¸µµ °¡Áö°í ÀÖ´Ù. )
	// CHARLIST *ch = &connections[EventPC].chrlst;
	// if( !ch ) return;
	var[EventPC][quest_no] = quest_value;
}

void sCmpQuestNumStep( int quest_no, int quest_value)
{
	if(var[EventPC][quest_no] - quest_value > 0)
	{
		sYES=2;
	}
	if(var[EventPC][quest_no] - quest_value == 0)
	{
		sYES=1;
	}
	if(var[EventPC][quest_no] - quest_value < 0)
	{
		sYES=0;
	}
}

void sCmpQuestNumStep( void )
{
	int quest_no=GetInt();
	int quest_value=GetInt();
	if(var[EventPC][quest_no] - quest_value > 0)
	{
		sYES=2;
	}
	if(var[EventPC][quest_no] - quest_value == 0)
	{
		sYES=1;
	}
	if(var[EventPC][quest_no] - quest_value < 0)
	{
		sYES=0;
	}
}

void sMapName( void )// 3
{
	GetString( MapName );
	CharUpper( MapName );
	
	strcpy_s( MAPNAME, sizeof(MAPNAME), MapName );
	
	
	
	int i;
	for( i = 0 ; i < MAX_MAP_  ; i ++)
	{
		if( strcmp( MapInfo[i].mapfile, MAPNAME ) ==0 ) 
		{
			MapNumber = i;
			return;
		}
	}
	
	if( i == MAX_MAP_ ) Error( "*** Error : Can NOT find the MapNumber of '%s'", MapName );
}

extern void DeleteSquadMember(const int cn); // 010904 LTS//020903 lsw
void DecLocalWarfieldMemberCount(const int cn);	// LTS LOCALWAR
extern void CheckMapMove(int ID);			// LTS DRAGONLORDWAR
// Modified by chan78 at 2000/11/27

int MapMove(int idUser, const char* pMapName, int nX, int nY)
{	//< CSD-HK-030829

	//<! BBD 040329		À¯Àú¹æÃâÁß ¸Ê¹«ºê ±İÁöÇÑ´Ù
	if(g_KickMgr.IsKicking())
	{
		return 0;
	}
	//> BBD 040329		À¯Àú¹æÃâÁß ¸Ê¹«ºê ±İÁöÇÑ´Ù

	CHARLIST* pUser = CheckServerId(idUser);
	
	if (pUser == NULL)
	{
		return 0;
	}
	// ¸ÊÀÌµ¿½Ã Ä³¸¯ÅÍ µ¥ÀÌÅ¸ ÀúÀå
	DeleteDoublePill(connections, idUser); // Add by rogerio
	UpdateCharacterData(connections, idUser);

	LPMAPINFO pMapInfo = GetMapInfoByMapName(pMapName);
	
	if (pMapInfo == NULL)
	{
		return 0;
	}

	MAP_MOVE_INFO infMapMove;
	infMapMove.idUser = idUser;
	infMapMove.nMapPort = pMapInfo->port;
	infMapMove.nX = nX;
	infMapMove.nY = nY;
	g_pUserManager->Push(infMapMove);
	return 1;
}	//> CSD-HK-030829

int GotoMap(int cn, int nMapPort, int nMoveX, int nMoveY)
{	//< CSD-HK-030829
	CHARLIST *ch = ::CheckServerId(cn);
	if( !ch ) return 0;

	if (g_curr_time - ch->Last_MapChangeTime >= 10 )
	{
		;
	}
	else
	{
		t_packet p;
		p.h.header.type = CMD_CHANGE_MAP_FAIL_TIME;
		p.h.header.size = sizeof(k_mapchange_timer);
		
		p.u.kein.mapchange_timer.second = 10 - (g_curr_time - ch->Last_MapChangeTime);	
		::QueuePacket(connections,cn,&p,1);
		return 0;
	}

	LPMAPINFO pMapInfo = GetMapInfoByMapPort(nMapPort);
	
	if (!pMapInfo)
	{
		return 0;
	}
	
	const int iLimitLvMin = pMapInfo->LvMin;//030116 lsw
	const int iLimitLvMax = pMapInfo->LvMax;
	if (!ch->IsCounselor() && !ch->IsReporter() && //Eleval 18/08/09
		((iLimitLvMin && iLimitLvMin > ch->GetLevel()) || (iLimitLvMax && iLimitLvMax < ch->GetLevel())))
	{
		t_packet p;
		p.h.header.type = CMD_MAP_MOVE_LV_LIMIT;
		p.h.header.size = sizeof(MAPMOVELVLIMIT);
		
		p.u.Hwoa.rang.MapMoveLvLimit.iLvMin = iLimitLvMin;
		p.u.Hwoa.rang.MapMoveLvLimit.iLvMax = iLimitLvMax;
		
		::QueuePacket(connections,cn,&p,1);
		return 0;
	}
	
	if( !g_LightVersion.IsAbleMapMove( ch, pMapInfo->mapfile ) )
	{	// ¶óÀÌÆ® ¹öÀüÀÌ¶ó ½ÇÆĞ
		ch->Message( MK_WARNING, 5, 28 );
		ch->Message( MK_WARNING, 5, 29 );
		return 0;		// ¶óÀÌÆ® ¹öÀüÀÌ¶ó ½ÇÆĞ..
	}
	
	if (!ch->IsCounselor() && !ch->IsReporter()) // ¿î¿µÀÚ³ª µµ¿ì¹Ì, ±âÀÚ´Â µé¾î°¥¼ö ÀÖ´Ù.
	{
		const int type = pMapInfo->m_MoveType; //020903 lsw

		switch (type)
		{
		case MMT_NORMAL: 
			{
				break;		// ÀÌµ¿ÇÏ´Âµ¥ ¾Æ¹«·± Á¦¾àÀÌ ¾ø´Ù.
			}
		case MMT_FAIL: 		// ¾ø´Â ¸ÊÀÌ´Ù.
		case MMT_NOT_MOVE: 
			{
				CallClient(cn, CMD_CHANGE_MAP_FAIL);
				return 0;		// ÀÌµ¿ÇÒ ¼ö ¾ø´Ù.
			}		
		}
	}
	
	if (ch->IsDead())
	{
		::SendViewTypeChange( ch->GetServerID(), VIEWTYPE_GHOST_ );//030102 lsw
	}
	
	t_packet dummy_packet;
	memcpy(connections[cn].mapname, pMapInfo->mapfile, 20);
	// ¸ÊÃ¼ÀÎÁö ½ÇÆĞ»óÈ²À» À§ÇÑ ¹é¾÷.
	ch->MapChange_MoveGox = ch->MoveGox;
	ch->MapChange_MoveGoy = ch->MoveGoy;
	ch->MapChange_X = ch->X;
	ch->MapChange_Y = ch->Y;
	ch->MapChange_MoveSx = ch->MoveSx;
	ch->MapChange_MoveSy = ch->MoveSy;
	ch->MapChange_MoveLength = ch->MoveLength;
	ch->MapChange_MovePathCount = ch->MovePathCount;
	// »õ ¸ÊÀ» À§ÇÑ ¼¼ÆÃ.
	ch->MovePathCount	= 0;
	ch->MoveLength		= 0;
	ch->MoveGox = ch->X = nMoveX * TILE_SIZE + 16;
	ch->MoveGoy = ch->Y = nMoveY * TILE_SIZE + 16;
	ch->MoveSx = nMoveX;
	ch->MoveSy = nMoveY;
	ch->OldMoveSx = nMoveX; // Finito 25/08/07
	ch->OldMoveSy = nMoveY; // Finito 25/08/07
	memcpy( ch->MapName, pMapInfo->mapfile,20 );
	// Packet Setting
	memcpy(dummy_packet.u.server_change_map.mapname, pMapInfo->mapfile, 20);
	dummy_packet.u.server_change_map.x = nMoveX;
	dummy_packet.u.server_change_map.y = nMoveY;
	// Build Packet by Querying.
	if(GetChangeMapPort(connections, cn, &dummy_packet) != 1) 
	{
		MyLog( LOG_FATAL, "sMapMove() :: Failed To Query" );
		goto NO_PORT_;
	}	
	// Copy it connections[]
	memcpy( &connections[cn].server_change_map, &dummy_packet.u.server_change_map, sizeof(t_server_change_map));
	// Get My Own Agent ServerID
	WORD wMyAgentServerID;
	if( g_pServerTable->GetServerData( connections[cn].dwAgentConnectionIndex ) )
	{	
		wMyAgentServerID = g_pServerTable->GetServerData( connections[cn].dwAgentConnectionIndex )->wPort;
	}	
	else
	{	
		wMyAgentServerID = 0;
	}	
	
	connections[cn].state = CONNECT_CHANGING_MAP;
	if (NewSendGameToLoginChangeMapData(cn, wMyAgentServerID, WORD(dummy_packet.u.server_change_map.port)))
	{
		DeleteSquadMember(cn);
		DecLocalWarfieldMemberCount(cn);
		CheckMapMove(cn);								// LTS DRAGONLORDWAR
		return 1;
	}

NO_PORT_:	
	// Ä¿³Ø¼Ç °ª º¹±¸
	connections[cn].state = CONNECT_JOIN;
	strcpy_s(connections[cn].mapname, sizeof(connections[cn].mapname), MapName);
	// ¹é¾÷ÇØ³õ¾Ò´ø °ªÀ¸·Î µÇµ¹¸®±â.
	ch->MoveGox = ch->MapChange_MoveGox;
	ch->MoveGoy = ch->MapChange_MoveGoy;
	ch->X = ch->MapChange_X;
	ch->Y = ch->MapChange_Y;
	ch->MoveSx = ch->MapChange_MoveSx;
	ch->MoveSy = ch->MapChange_MoveSy;
	ch->MoveGox = ch->X;
	ch->MoveGoy = ch->Y;
	ch->OldMoveSx = ch->MoveGox; // Finito 25/08/07
	ch->OldMoveSy = ch->MoveGoy; // Finito 25/08/07
	ch->MoveLength = ch->MapChange_MoveLength;
	ch->MovePathCount = ch->MapChange_MovePathCount;
	
	CallClient( cn, CMD_CHANGE_MAP_FAIL);
	return 0;
}	//> CSD-HK-030829

// 021117 YGI
void sMapMove( void )// 4
{
	k_map_move data;
	//char mapname[ 20];
	//short int	 x, y;
	
	GetString( data.mapname );				
	_strupr_s( data.mapname, sizeof(data.mapname) );					
	data.x = GetInt();
	data.y = GetInt();
	
	//SendPacketDefault( CMD_MOVEMAP_FOR_LIGHT_VERSION, &data, sizeof( k_map_move )-20+strlen( data.mapname ), EventPC );
	//sYES = 1;
	
	sYES = MapMove( EventPC, data.mapname, data.x, data.y );
}

void sIsLevel( void )
{	//< CSD-030806
	const int lev = GetInt();
	// EventPCÀÇ ·¹º§ÀÌ levº¸´Ù ³ôÀ¸¸é 1¸¦ sYES¿¡ ³Ö°í ±×·¸Áö ¾ÊÀ¸¸é 0À» ³Ö´Â´Ù. 
	if (connections[EventPC].chrlst.GetLevel() >= lev)
	{
		sYES = 1;
	}
	else 
	{
		sYES = 0;
	}
	
}	//> CSD-030806

void sIsJobPC( void )
{
	int job = GetInt()-1+20;//Á÷¾÷Àº 0¹ø ºÎÅÏµ¥... 1¹øºÎÅÍ·Î Àâ¾ÒÀ½... 
	// EventPCÀÇ Á÷¾÷ÀÌ jobÀÌ¸é  1À»  sYES¿¡ ³Ö°í ±×·¸Áö ¾ÊÀ¸¸é 0À» ³Ö´Â´Ù. 
	
	if( connections[ EventPC].chrlst.Job == job )
	{
		sYES = 1;
	}
	else 
	{
		sYES = 0;
	}
	
}

void sGetGender( void )			// 010210 YGI
{
	// 030930 kyo <<
	CHARLIST *ch = CheckServerId(EventPC);
	if( NULL == ch)
	{
		return;
	}
	// 030930 kyo >>

	sYES = connections[ EventPC].chrlst.Gender;		// 0 : ¿©ÀÚ, 1 : ³²ÀÚ
}

void sQuestComplete(int magic_no)
{	//< CSD-TW-030606
	CHARLIST* pCaster = CheckServerId(EventPC);
	
	if (pCaster == NULL)
	{
		return;
	}

	t_packet packet;
	packet.h.header.type = CMD_MAGIC_EXECUTE;
	packet.h.header.size = sizeof(t_server_magic_execute);
	packet.u.magic.server_magic_execute.nMagic	= BYTE(magic_no);
	packet.u.magic.server_magic_execute.idCaster = pCaster->GetServerID();
	packet.u.magic.server_magic_execute.idTarget = pCaster->GetServerID();
	packet.u.magic.server_magic_execute.nX = pCaster->X ;
	packet.u.magic.server_magic_execute.nY = pCaster->Y ;
	QueuePacket(connections, EventPC, &packet, 1);
	CastMe2Other( EventPC, &packet);
}	//> CSD-TW-030606

void sQuestComplete(void)
{
	short int command_type = GetInt();
	t_client_character_demand p;
	p.target_id=connections[EventPC].chrlst.GetServerID();
	p.command_type = command_type;
	RecvCMD_CLIENT_CHARACTER_DEMAND( EventPC, &p);
	
}
void sQuestSound(void)
{
	//15001
	//
	short int command_type = GetInt();
	t_client_character_demand p;
	p.target_id=connections[EventPC].chrlst.GetServerID();
	p.command_type = command_type;
	RecvCMD_CLIENT_CHARACTER_DEMAND( EventPC, &p);
	
}
void sQuestTeleport(void)
{
	int commandNum = GetInt();
	int tile_X=GetInt();
	int tile_Y=GetInt();
	
	t_server_magic_position	smp;
	
	//Å¬¶óÀÌ¾ğÆ®·Î º¸³¾ ÆĞÅ¶ ¼ÂÆÃ
	smp.nType =	PT_TELEPORT;
	smp.idTarget = EventPC;
	smp.nX = tile_X;
	smp.nY = tile_Y;

	int target_id  = EventPC;
	LPCHARLIST ch;
	int area_cmd;
	int id = -1;

	if( target_id >= 10000 )
	{	
		id = target_id - 10000;
		ch = CheckNpcId( id );
		if( !ch ) return;
		area_cmd = MOVE_NPC_AREA;
	}	
	else
	{	
		id = target_id;
		ch = CheckServerId( id );
		if( !ch ) return;
		area_cmd = MOVE_PC_AREA;
	}
	
	ch->MoveLength = ch->MovePathCount = 0;
	ch->MoveGox = ch->X = smp.nX*TILE_SIZE;
	ch->MoveGoy = ch->Y = smp.nY*TILE_SIZE;
	ch->MoveSx = smp.nX;
	ch->MoveSy = smp.nY;
	ch->OldMoveSx = smp.nX;
	ch->OldMoveSy = smp.nY;
	SetArea( area_cmd, id );

	SendCMD_MAGIC_POSITION(&smp);
}

void QuestTeleport( const int nCn, const int nX, const int nY ) 
{// 031030 kyo
	
	CHARLIST *ch = CheckServerId( nCn );
	if( !ch ) return;
	
	t_server_magic_position	smp;
	
	//Å¬¶óÀÌ¾ğÆ®·Î º¸³¾ ÆĞÅ¶ ¼ÂÆÃ
	smp.nType =	PT_TELEPORT;
	smp.idTarget = nCn;
	smp.nX = nX;
	smp.nY = nY;

	ch->MoveLength = ch->MovePathCount = 0;
	ch->MoveGox = ch->X = smp.nX*TILE_SIZE;
	ch->MoveGoy = ch->Y = smp.nY*TILE_SIZE;
	ch->MoveSx = smp.nX;
	ch->MoveSy = smp.nY;
	ch->OldMoveSx = smp.nX;
	ch->OldMoveSy = smp.nY;
	SetArea( MOVE_PC_AREA, nCn );

	SendCMD_MAGIC_POSITION(&smp);
}

void sIsInvHavePC(void)
{		
	int ItemNum = GetInt();
	int ret;
	CHARLIST	*caster;
	caster=&connections[EventPC].chrlst;
	if (!caster) return;
	ret = SearchItemByInv(ItemNum,caster);
	if( ret==1 )
	{
		sYES=1;
	}
	else
	{
		ret = SearchItemByEquip(ItemNum,caster);
		if( ret==1 )
		{
			sYES=2;
		}
		else
		{
			ret = SearchItemByQuick(ItemNum,caster);
			if( ret==1 )
			{
				sYES=3;
			}
			else
			{	
				sYES=0;
			}
		}
	}
}

//YGI acer
void sSetItemInvPC(void)
{
	int change_no = GetInt();
	int item_no = GetInt();
	int item_dur = GetInt();
	//	int item_durmax = GetInt();
	
	CHARLIST *ch =&connections[EventPC].chrlst;
	if (!ch) return;
	for( int a=0; a<3; a++ )
		for( int b=0; b<3; b++ )
			for( int c=0; c<8; c++ )
				if( ch->inv[a][b][c].item_no == change_no ) 
				{
					SendItemEventLog( &ch->inv[a][b][c], EventPC, SN_NOT_USER, SILT_DELETE_BY_SCRIPT, 2 );//³Ö±âÀü.

					ItemAttr target = GenerateItem( item_no, IATTR_MUCH,item_dur);//020509 lsw
					
					ch->inv[a][b][c] = target;
					if(target.item_no)
					{
						SendItemEventLog( &ch->inv[a][b][c], EventPC, SN_NOT_USER, SILT_MAKE_BY_SCRIPT, 2 );//³ÖÀºÈÄ
					}
					
					POS pos;
					SetItemPos( INV, a, b, c, &pos );
					SendServerEachItem(  &pos, &target, EventPC );		// 001028 YGI
					
					sYES=1;
					return;
				} 
				
				
				sYES=0;
}

void sSetSkillPC(void)
{
	int SkillNum = GetInt();
	int SkillLearn = GetInt();
	CHARLIST *ch =&connections[EventPC].chrlst;
	if( !ch ) return;
	
	if( SkillLearn ) 
	{
		ch->Skill[SkillNum ] = true;
		makemyjobexpsetting( SkillNum, ch );
		SendLearnSkillOk(SkillNum , connections, EventPC );	
	}
	return;
	
}

void Recv_CMD_QUEST_JOB_FINISH( int cn, int type  )
{	
	sCmpQuestNumStep(52,1997);
	if(sYES==1)
	{
		sSetQuestNumStep(52,1998);
	}
	
}



void sIsItemHavePC( void )
{		
	int ItemNum = GetInt();
	int ItemPos = GetInt();
	//ÇØ´çÇÏ´Â ¾ÆÀÌÅÛÀÌ ÇØ´ç Àåºñ¿¡  Âø¿ëÇÏ°í ÀÖÀ¸¸é.. ¿¡½º¾Æ´Ï¸é ³ë
	if(connections[ EventPC].chrlst.equip[ItemPos].item_no == ItemNum)
	{
		sYES = 1;
	}
	else 
	{
		sYES = 0;
	}
	return;
	
	
}	
void sIsClassPC( void )
{		
	int ClassNum = GetInt();
	
	//ÇØ´çÇÏ´Â Å¬·¡½º¸é.. ¿¡½º¾Æ´Ï¸é ³ë
	if(connections[EventPC].chrlst.Class == ClassNum)
	{
		sYES = 1;
	}
	else 
	{
		sYES = 0;
	}
	return;
	
	
}

void sIsSkillHavePC(void)
{
	int SkillNum = GetInt();
	//ÇØ´ç ±â¼úÀÌ ÀÖÀ¸¸é ¿¡½º ¾Æ´Ï¸é ³ë 
	if(connections[EventPC].chrlst.Skill[SkillNum] == 1)
	{
		sYES = 1;
	}
	else
	{
		sYES = 0;
	}
	return;
}

void sCheckNation(void)
{
	int NationNum = GetInt();
	// °°Àº ³ª¶ó¶ó¸é ¿¹½º  
	
	if(connections[EventPC].chrlst.name_status.nation == (DWORD )NationNum)
	{
		sYES = 1;
	}
	else
	{
		sYES = 0;
	}
	return;
}	


// Monster°¡ ¹ß»ıÇÒ Àå¼Ò¿Í Sprite¹øÈ£¸¦ Á¤ÀÇÇÑ´Ù. 
void sInitMonsterGeneratedPosition( void )
{
	int x = GetInt();
	int y = GetInt();
	int npcindex = GetInt();
	
	return;//
	
	NPCGeneratePosition( npcindex, x, y, 0, 0 );
}

void sIsPlayArenaGame()		// ÀüÅõ¸Ê ½ÃÀÛÀÎÁö ¾Æ´ÑÁö
{	//< CSD-030517
	const int nGame = GetInt();
	CBaseArena* pGame = g_pArenaManager->GetGameInfo(nGame);

	if (pGame == NULL)
	{
		sYES = 0;
		return;
	}

	if (!pGame->IsPlayGame())
	{
		sYES = 0;
	}
	else
	{
		sYES = 1;
	}
}	//> CSD-030517

void sAddFightMap( void )
{	//< CSD-RESERVE

}	//> CSD-RESERVE

void sDeleteFightMap( void )				// 914 YGI
{	//< CSD-RESERVE

}	//> CSD-RESERVE

void sIsFullFightMapTeam( void )		// 0915 YGI
{	//< CSD-RESERVE

}	//> CSD-RESERVE

int callmenudef[]= 
{ 
	MN_QUEST			,	// ÀÓ½Ã 5°³ YGI  0923
	MN_FAME				,
	MN_QUEST_INDEX		,
	MN_RELATION			,
	MN_MAN_TO_MAN		,
	
	MN_SKILLGUILD_BUY	,		
	MN_SKILLGUILD_SELL	,	
	MN_SKILLGUILD_SKILL	,
	MN_SKILLGUILD_ITEM	,	
	MN_SKILLGUILD_REPAIR,		// 9
	MN_BANK_DEPOSIT,
	MN_BANK_DEFRAYAL,
	MN_BANK_CUSTODY,
	MN_BANK_MAIN,
	MN_BANK_LOAN,
	MN_BANK_REPAYMENT,
	MN_BANK_AUCTION,
	MN_BANK_AUCTION_SALE,
	MN_BLACKSMITH			,
	MN_FARM		,			// 19
	MN_RESTAURANT,
	MN_HERBSHOP,	
	MN_SHAMBLES	,			// 22
	MN_CANDLE	,			// 23
	MN_SLOPSHOP	,
	MN_ALCHEMY	,
	MN_WOODWORKING,			// 26
	MN_BOWCRAFT	,
	MN_SHIPYARD	,
	MN_TOWNHALL ,		// 29
	MN_TOWNHALL_PUBLICNOTICE ,
	MN_TOWNHALL_BUILDING,
	MN_TOWNHALL_COMMERCE1,
	MN_TOWNHALL_COMMERCE2,
	MN_TOWNHALL_COMMERCE3,
	MN_TOWNHALL_HOUSEREPAIR,
	MN_TOWNHALL_GUILD,
	MN_TOWNHALL_FORUM,
	MN_TOWNHALL_EVENT,
	MN_TOWNHALL_RANK1,		// 39
	MN_TOWNHALL_RANK2,
	MN_TOWNHALL_NOTICE4,
	MN_TOWNHALL_NOTICE3,
	MN_TOWNHALL_NOTICE2,
	MN_TOWNHALL_NOTICE1,
	MN_TOWNHALL_COMMERCE4,
	MN_TOWNHALL_CONSTRUCTION,
	MN_MY_HOME,
	MN_MY_HOME_EXPLAN,
	MN_HOUSE,				// 49
	MN_MY_HOME_IN,
	MN_HOME_PERSON,
	MN_GOD1,
	MN_GOD2,
	MN_GOD3,
	MN_MAGICBOOK,
	MN_FUNCTION,
	MN_DRINK_MAIN,
	MN_DRINK_RUMOR	,
	MN_DRINK_BUY	,		// 59
	MN_SHOP_MAIN,			//60
	MN_SHOP_SELL,
	MN_SHOP_BUY	,
	MN_HEAL_MAIN,			//63
	MN_HEAL_HEALING,
	MN_HEAL_PREVENTING,
	MN_HEAL_LEARN,
	MN_INN_MAIN,			// 67
	MN_INN_REST,
	MN_INN_SLEEP,			// 69
	MN_SYSTEM_LOGOUT,		// 70
	MN_MAGIC_GUILD,			// 71 ¸¶¹ı»ç ±æµå		// 0208_2 YGI
	MN_ACADEMY,				// 72 ¾ÆÄ«µ¥¹Ì			// 0208_2 YGI
	
	MN_GOD_EDELBLHOY,		// 73	¿¡µ¨ºê·ÎÀÌ ½ÅÀü		// 0410 YGI
	MN_GOD_TEFFERY,			// 74	Å×Æä¸® ½ÅÀü			// 0410 YGI
	MN_GOD_LETTY,			// 75	·¹Æ¼ ½ÅÀü			// 0410 YGI
	
	MN_GUILD_ADMIN,			// 76	GM µî·Ï ¸Ş´º 		// 1027 YGI
	MN_SALVATION2,			// 77	Àû½ÊÀÚ ¸Ş´º			//  YGI				// 010522 YGI
	MN_CHANGE_MONEY,		// 78						// 010204 YGI
	
	MN_MERCHANT_MAIN,		// 79	// »óÀÎ ¸Ş´º ¸ŞÀÎ		// 010410 YGI
	MN_ARENA_LOBBY,			// 80	// ¹èÆ²¸Ş´º // CSD-030509
	
	MN_LADDER_RANK,			// 81	// 010511 YGI
	MN_GOD_OREM,			// 82		// 010525 YGI
	
	MN_NATION_MAIN_VA,		// 83
	MN_NATION_MAIN_ZY,		// 84
	MN_NATION_MAIN_ILL,		// 85

	MN_MERCHANT_OLDMAIN,	// 86	// »óÀÎµî·Ï¸Ş´º µÇÃ£±â ¸Ş´º // BBD 040303 
	MN_ITEM_MALL,			// 87 the new item mall
};		

// 010530 YGI
extern void SendCallSmallMenu( int cn, int storelistindex, int menu );
void sCallMenu( void  )
{
	int menu = GetInt();
	SendCallSmallMenu( EventPC, 0, menu );
}

void sGetNationByMap( void )	// Æ¯Á¤ ¸ÊÀÇ ±¹Àû¾Ë¾Æ ¿À±â
{
	char	mapname[30];
	GetString( mapname );
	
	s_Nation = GetNationByMapName( mapname );
}

void sCalcFriendShip( void )	// Æ¯Á¤ ±¹°¡ÀÇ Àû´ë¼öÄ¡ ¼öÁ¤ÇÏ±â	// »©°í ½ÍÀ»¶§´Â -°ªÀ» ³Ö¾î ÁØ´Ù.
{
	int nation = GetInt();
	int friend_ship = GetInt();
	
	CHARLIST *ch = CheckServerId( EventPC );
	if( !ch ) return;
	
	sYES = 1;
	
	short int *ch_friend_ship;
	switch( nation )
	{
	case N_VYSEUS :	ch_friend_ship = &ch->nk[ N_VYSEUS]; break;
	case N_ZYPERN : ch_friend_ship = &ch->nk[ N_ZYPERN]; break;
	case N_YLLSE : ch_friend_ship =  &ch->nk[ N_YLLSE]; break;
		
	default : sYES = 0; return;
	}
	
	*ch_friend_ship += friend_ship;
	return;
}

void sCompFriendShip( void )	// Àû´ë¼öÄ¡ ºñ±³ÇÏ±â
{
	int nation = GetInt();
	int comp_data = GetInt();
	
	CHARLIST *ch = CheckServerId( EventPC );
	if( !ch ) return;
	
	short int *ch_friend_ship;
	switch( nation )
	{
	case N_VYSEUS :	ch_friend_ship = &ch->nk[ N_VYSEUS]; break;
	case N_ZYPERN : ch_friend_ship = &ch->nk[ N_ZYPERN]; break;
	case N_YLLSE  : ch_friend_ship = &ch->nk[ N_YLLSE]; break;
		
	default : sYES = -2; return;
	}
	
	if( *ch_friend_ship > comp_data ) sYES = 1;
	else if( *ch_friend_ship == comp_data ) sYES = 0;
	else sYES = -1;
}

// 010531			YGI
void sCompLadderScore( void )
{
	int score = GetInt();
	
	CHARLIST *ch = CheckServerId( EventPC );
	if( !ch ) return;
	
	sYES = ch->LadderScore - score;
}


///////////////////////////////////////////////////////////////////////////x

void sCallSmallMenu( void )
{
	int index = GetInt();	// »óÁ¡ List Index..
	int menu = GetInt();	// SmallMenu Index..

	
//<soto-030430
	if(256 == menu || 257 == menu || 258 == menu)
	{
		CallSmallMenu( EventPC, index, menu );
		return;
	}
//>soto-030430

	
	//020707 YGI
	if( menu < 1000 )
	{
		CallSmallMenu( EventPC, index, callmenudef[ menu] );
	}
	else
	{
		SendCallSmallMenu( EventPC, index, menu-1000 );
	}
}

void sWeatherOn( void )
{
	g_weatherflag = 1;
}

void sWeatherOff( void )
{
	g_weatherflag = 0;
}

void sRecallScriptStart( void )
{
	int aftertime;
	
	sRECALL_SCRIPT_COUNT	= GetInt();	
	aftertime				= GetInt();
	
	sRECALL_SCRP_NO  = NScriptNo; //    (var[EventPC][1])
	sRECALL_CURTIME = g_curr_time ;
	sRECALL_AFTTIME = g_curr_time + aftertime;
}
void sRecallScriptEnd( void )
{
	sRECALL_SCRIPT_COUNT = 0;
	sRECALL_SCRP_NO = 0;
}

void sMaxMonsterGeneration( void )
{
	Max_NPC_Generation = GetInt();	
	
	if( Max_NPC_Generation > MAX_NPC_LIST ) Max_NPC_Generation = MAX_NPC_LIST;
}

void sLoadItemValueOneMaul( void )
{
	int maulno = GetInt();
}

void srandom( void )		// 0131 YGI
{
	int start, end;
	start = GetInt();
	end = GetInt();
	var[EventPC][5] = (rand()%( end - start )) + start;
	sYES = var[EventPC][5]; // Add by rogerio
}

void sEventMsg( void )// 0
{		
	// Client¿¡ ´ëÈ­¸¦ º¸³»ÁØ´Ù....	
	GetString( String );
	
	if(connections[ EventPC].dwAgentConnectionIndex && connections[ EventPC].state >= CONNECT_JOIN)
	{
		SendBBS2( String, (short)EventPC );
		//SendBBS2¶ó´Â ÀÌº¥Æ® ¸Ş¼¼Áö ¹ß»ı ÇÔ¼ö¸¦ ½ÇÇàÇÑ´ç.
		//QueuePacket(connections, EventPC, &packet, 1);
	}	
	
	return;
}		

void sReturnSealStoneBreakable( void )
{		
	switch( ReturnSealStoneBreakable() )
	{	
	case CMD_STEALSTONE_BREAKABLE	 :			sYES = 1;	break;
	case CMD_STEALSTONE_NON_BREAKABLE_PEACE	:	sYES = 2;	break;
	case CMD_STEALSTONE_NON_BREAKABLE:			sYES = 0;	break;
	default :									sYES = 0;	break;
	}	
}		

// 020428 YGI acer
void sSaveItemAtDb( void )
{
	int a, b, c;
	POS pos;
	ItemAttr *pItem;
	
	int item_no = GetInt();
	CHARLIST *ch = CheckServerId( EventPC );
	if( !ch ) goto ERROR__;
	
	if( !SearchItemByInv( item_no, ch, a, b, c ) ) goto ERROR__;
	
	// ¸ÕÀú ¾ÆÀÌÅÛÀ» Áö¿ì°í µğºñ·Î º¸³½´Ù.
	
	SetItemPos( INV, a, b, c, &pos );
	pItem = GetItemByPOS( EventPC, pos );
	if( !pItem ) goto ERROR__;
	
	// 020428 YGI acer
	SendItemEventLog( pItem, EventPC, 0, SILT_DELETE_BY_SCRIPT, 7);	 //YGI acer
	SendDeleteItem( pItem, &pos, ch );
	SendSaveEventItem( ch, item_no, 1);		// ÀÌº¥Æ® ¾ÆÀÌÅÛ µğºñ¿¡ ÀúÀå
	
	sYES = 1;
	return;
	
ERROR__ :
	sYES = 0;
	return;
}

void sCheckMoney( void )	// ÇÇ½ÃÀÇ µ·À» ¾Ë¾Æ¿À±â
{	
	int money = GetInt();
	CHARLIST *ch = CheckServerId( EventPC );
	if( !ch ) return;
	
	DWORD pc_money;
	
	s_PcMoney = pc_money = GetMoneyByItem( ch );
	
	//sYes = pc_money - money;		// ´õ¸¹ÀÌ °¡Áö°í ÀÖÀ»°æ¿ì¿£ ¾çÀÇ Á¤¼ö
	if( pc_money > money ) sYES = 1;		// ´õ¸¹ÀÌ °¡Áö°í ÀÖÀ»°æ¿ì
	else if( pc_money == money ) sYES = 0;
	else sYES = -1;
}

// ÇÇ½Ã¿¡°Ô¼­ µ· ±ï±â
void sSubtractMoney( void )
{
	int money = GetInt();
	CHARLIST *ch = CheckServerId( EventPC );
	if( !ch ) return;
	
	DWORD pc_money;
	pc_money = GetMoneyByItem( ch );
	if( pc_money < money ) money = pc_money;
	
	SubtractMoney( money, ch );
}

// 010129 KHS		
// ¸ó½ºÅÍ°¡ ¹ß»ı Á¶°ÇÀ» ÀĞ¾îµéÀÎ´Ù. 
// ±âÁ¸¿¡ NPCÀÇ ¹ß»ıÀº .SKBÈ­ÀÏ¿¡ µé¾î ÀÖ´Â Data¸¦ ÀÌ¿ëÇÏ´Â°ÍÀ» ¿øÄ¢À¸·Î ÇÏÁö¸¸
// QuestÁ¶°Ç¿¡ ¸Â´Â NPC¸¦ ¹ß»ı½ÃÅ°±â À§ÇØ Ãß°¡ ÇÏ¿´´Ù. 
// ÀÌ´Â 10ºĞ¸¶´Ù Á¶°ÇÀ» °Ë»öÇÏ¿© Á¶°Ç¿¡ ¸ÂÀ¸¸é ¹ß»ıÇÑ´Ù. 
void sGenMonSpec( void )
{	
	
/*
int x, y;
int monsprno;
int lev;
int rate;
int time;
int iftime;
int turn;

  x			= GetInt();	// ¸ÊÀÇ ÁÂÇ¥.(´ÜÀ§: Tile )
  y			= GetInt();
  monsprno	= GetInt();	// ¸ó½ºÅÍÀÇ ¹øÈ£.
  lev			= GetInt();	// ¹ß»ıÇÒ MonsterÀÇ Level
  rate		= GetInt();	// ¹ß»ıÈ°·ü ( 10000 = 100% )
  
	genyear		= GetInt();	// ¹ß»ıÁ¶°Ç½Ã°¢½Ã°£( ÀÌ ½Ã°£ÀÌÈÄ ºÎÅÍ ¾Æ·¡ ½Ã°£¿¡ µû¶ó ¹ß»ıÇÑ´Ù. )
	genmon		= GetInt();	
	genday		= GetInt();	
	genhour		= GetInt();	
	genmin		= GetInt();	
	
	  iftime		= GetInt();	// À§ÀÇ gen*ÀÇ ½Ã°£ÀÇ Â÷ÀÌ¸¦ ¹ß»ıÁ¶°ÇÀ¸·Î ÇÑ´Ù. 
	  
		turn		= GetInt();	// ¸îÅÏ¸¶´Ù ¹ß»ıÇÏ´Â°¡.( 1ÀÌ¸é 10ºĞ¸¶´Ù ¹ß»ıÇÑ´Ù. )
	*/
}

void sGetGuild()		// 010511 YGI
{	//< CSD-030324
	CHARLIST *ch = CheckServerId( EventPC );
	if( !ch ) return;
	
	sYES = ch->GetGuildCode();
	return;
}	//> CSD-030324

extern int g_pLoadingPoint[NW_NATION_COUNT][NW_SQUAD_MAX]; // 010904 LTS

void sCheckLoadingPoint() // 010904 LTS	
{
	CHARLIST* ch=CheckServerId(EventPC);   //°ø°İÀÚ ¹æ¾îÀÚ¸¦ ºĞ¸®ÇØ¾ß ÇÑ´Ù... ¤Ì.¤Ì 
	if (!ch) return;
	sYES=g_pLoadingPoint[ch->NWCharacter.WarfieldNo][ch->NWCharacter.SquadNo];	// 010915 LTS
}

extern bool CheckDefence(int cn); // 010904 LTS
extern bool CanGoBonus();			// 010915 LTS

void sCheckDefence()			// ¹æ¾îÀÚ ÀÎÁö È®ÀÎÇÑ´Ù. 
{
	if (CheckDefence(EventPC)) sYES=1;
	else sYES=0;
}

void sCanGoBonus()
{
	if (CanGoBonus()) sYES=1;
	else sYES=0;
}

void sFameDown()
{
	int tempValue=GetInt();
	SaveChangeFame(&connections[EventPC].chrlst,connections[EventPC].chrlst.fame,connections[EventPC].chrlst.fame-tempValue,LF_SCRIPT);		// 020909 LTS
	connections[EventPC].chrlst.fame-=tempValue;

	char FameNotice[128]; //Eleval
	sprintf(FameNotice, "Your fame has decreased by %d.", tempValue); //Eleval
	if(connections[ EventPC].dwAgentConnectionIndex && connections[ EventPC].state >= CONNECT_JOIN)
	{
		SendBBS2( FameNotice, (short)EventPC );
	}	

}

void sFameUp()
{
	int tempValue=GetInt();
	SaveChangeFame(&connections[EventPC].chrlst,connections[EventPC].chrlst.fame,connections[EventPC].chrlst.fame+tempValue,LF_SCRIPT);		// 020909 LTS
	connections[EventPC].chrlst.fame+=tempValue;

	char FameNotice[128]; //Eleval
	sprintf(FameNotice, "Your fame has raised by %d.", tempValue); //Eleval
	if(connections[ EventPC].dwAgentConnectionIndex && connections[ EventPC].state >= CONNECT_JOIN)
	{
		SendBBS2( FameNotice, (short)EventPC );
	}	
}

void sFameCheck()
{
	int tempValue=GetInt();
	if (connections[EventPC].chrlst.fame>=tempValue) sYES=1;
	else sYES=0;
}
//_________________________________________________________________	// 010915 LTS

// CSB AddCode
// CSB AddCode
// CSB AddCode
extern void (*SFunction[_SCRIPT_FUNCTION_POINT_ ])(void);

QT QTickData[DRAGON_MAX_CONNECTIONS_];
DWORD QTemp, QTick = 1;

// ÃÊ´ç ÇÑ¹ø¾¿ ÁøÀÔµÊ
void QuestSecond()	// 1000 ms
{	
	QTick++;
	
	for (QTemp = 0; QTemp < DRAGON_MAX_CONNECTIONS; QTemp++) 
	{	
		if (QTick == QTickData[QTemp].Tick) 
		{		
			if (!QTickData[QTemp].Func) 
			{
				continue;
			}
			
			QTickData[QTemp].Func();
		}
	}
}		

void TimerFirstFunc( void ) 
{	
	// CSB ÄÚµùÇÊ¿ä
	
	// ³ªÁß¿¡ ÀÌ°÷¿¡ ÇØ´ç À¯Àú( QTemp )ÀÇ Äù½ºÆ® ¾ÆÀÌÅÛ(±â¹Ğ¹®¼­?)
	// °¡ ÀÖ´ÂÁö Ã¼Å©ÇÏ¿© ¾ø¾ÖÁÖ°í À¯Àú¿¡°Ô ´Ê¾ú´Ù´Â ¸Ş½ÃÁö¸¦ º¸³» ÁÖ¾î¾ßÇÑ´Ù.
	// Äù½ºÆ® ½ºÅÜ(Äù½ºÆ® ¹øÈ£ ¹ÌÁ¤)À» Æ¯Á¤ ¼ıÀÚ(¹ÌÁ¤--;)·Î ¼ÂÆÃÇØÁÖ¾î¾ß ÇÑ´Ù.
	// 2~3ÁÙ ÄÚµù ¿ä¸Á ( ¼öÁ¤ µÉÁöµµ ¸ğ¸§ )
	
	// ¿¹Á¦(½ÇÁ¦)
	// if( ±â¹Ğ¹®¼­°¡ ÀÖ´Â°¡? ) {
	//		Äù½ºÆ® ½ºÅÜ ¼³Á¤
	//		2ºĞ Áö³²À» À¯Àú¿¡°Ô ¾Ë¸²
	//		±â¹Ğ¹®¼­ »èÁ¦
	// } 
	
	int change_no = 0;
	if( SearchItemByInv(10176,&connections[QTemp].chrlst) ) change_no = 10176;
	if( SearchItemByInv(10177,&connections[QTemp].chrlst) ) change_no = 10177;
	if( SearchItemByInv(10178,&connections[QTemp].chrlst) ) change_no = 10178;
	if( change_no ) {
		
		var[QTemp][ 56 ] = 10;
		
		
		int item_no = 0;
		int item_dur = 0;
		
		CHARLIST *ch =&connections[QTemp].chrlst;
		if (!ch) return;
		for( int a=0; a<3; a++ )
			for( int b=0; b<3; b++ )
				for( int c=0; c<8; c++ )
					if( ch->inv[a][b][c].item_no == change_no ) 
					{
						ItemAttr target = GenerateItem( item_no, IATTR_MUCH, item_dur);//020509 lsw
						ch->inv[a][b][c] = target;
						POS pos;
						SetItemPos( INV, a, b, c, &pos );
						SendServerEachItem(  &pos, &target, QTemp );
						sYES=1;
						return;
					} 
					
	}
	
	int length;
	t_packet packet;
	
	// Client¿¡ ´ëÈ­¸¦ º¸³»ÁØ´Ù....
	strcpy_s( String, sizeof(String), "2ºĞÀÌ °æ°ú" );
	
	length = strlen( String );
	packet.h.header.type = CMD_SCRIPT_MESSAGE_BOX;
	{
		memcpy(packet.u.kein.default_msg, String, length);
	}
	packet.h.header.size = length+1;
	
	if(connections[QTemp].dwAgentConnectionIndex && connections[ QTemp].state >= CONNECT_JOIN)
		QueuePacket(connections, QTemp, &packet, 1);
	
	return;
	
}

void TimerSecondFunc( void )
{
	
	// ÀÌ°÷¿¡ ÄÚµù.
	
}

// Æ¯Á¤ ½Ã°£ÈÄ ½ÇÇàµÉ¼ö ÀÖ´Â ÇÔ¼ö ¸®½ºÆ®
void (*TimerFunc[_SCRIPT_FUNCTION_POINT_])(void) =
{	
	TimerFirstFunc,	 // 0 ¹ø Å¸ÀÌ¸Ó ÇÔ¼ö
	TimerSecondFunc, // 1 ¹ø Å¸ÀÌ¸Ó ÇÔ¼ö
};

// À¯Àúº° Å¸ÀÌ¸Ó ¼³Á¤
void sSetTimer()
{
	QTickData[EventPC].Tick = QTick + GetInt();	   // ¸î ÃÊÈÄ ½ÇÇà (ÃÖ´ë ¿ÀÂ÷ ¹üÀ§ 1ÃÊ)
	GetInt();									   // ¿©ºĞ
	QTickData[EventPC].Func = TimerFunc[GetInt()]; // ÃÊÈÄ ½ÇÇàµÉ ÇÔ¼ö
	QTickData[EventPC].ExtInt = GetInt();		   // ½ÇÇàÈÄ »ç¿ëµÉ È®Àå°ª(Integer)
	GetString(QTickData[EventPC].ExtChar);		   // ½ÇÇàÈÄ »ç¿ëµÉ È®Àå°ª(String)
	
}
// À¯ÀúÀÇ Å¸ÀÌ¸Ó ÁßÁö
void sStopTimer()
{
	QTickData[EventPC].Func = NULL;
}
// ·¹º§ °¡Á®¿À±â - yes ¿¡ ·¹º§ ÀúÀå
void sGetLevel()	
{	//< CSD-030806
	sYES = connections[EventPC].chrlst.GetLevel();
}	//> CSD-030806
// Å¬·¡½º °¡Á®¿À±â - yes ¿¡ Å¬·¡½º°ª ÀúÀå
void sGetClass()
{
	sYES = connections[EventPC].chrlst.Class;
}
// ¾îºô¸®Æ¼ °¡Á®¿À±â - yes ¿¡ ¾îºô¸®Æ¼ ¹øÈ£ ÀúÀå
void sGetAbil()
{
	int abil = GetInt();
	sYES = 0;
	
	switch( abil ) 
	{
	case 1:  sYES = connections[EventPC].chrlst.Str;   break;
	case 2:  sYES = connections[EventPC].chrlst.Con;   break;
	case 3:  sYES = connections[EventPC].chrlst.Dex;   break;
	case 4:  sYES = connections[EventPC].chrlst.MoveP; break;
	case 5:  sYES = connections[EventPC].chrlst.Wis;   break;
	case 6:  sYES = connections[EventPC].chrlst.Int;   break;
	case 7:  sYES = connections[EventPC].chrlst.wsps;  break;
	case 8:  sYES = connections[EventPC].chrlst.Char;  break;
	case 9:  sYES = connections[EventPC].chrlst.Endu;  break;
	case 10: sYES = connections[EventPC].chrlst.Moral; break;
	case 11: sYES = connections[EventPC].chrlst.Luck;  break;
	}
}
// Æ¯Á¤ À¯Àú¿Í ÆÄÆ¼¸¦ ¸Î¾ú´ÂÁö È®ÀÎ - yes¿¡ ÀÖÀ¸¸é 1 ¾øÀ¸¸é 0
void sCheckParty()
{
	char id[0xff];
	int ti;
	
	GetString( id );
	
	sYES = 0;
	for( ti = 0; ti < MAX_PARTY_MEMBER; ti++ ) 
	{
		if( connections[ EventPC ].chrlst.party[ti].On
			&& !strcmp( connections[ EventPC ].chrlst.party[ti].Name, id ) ) sYES = 1;
	}
	
	
}
// Æ¯Á¤ À¯ÀúÀÇ ±¹°¡¸¦ °¡Á®¿È - yes ¿¡ ±¹°¡ ¹øÈ£
void sGetNat()
{
	sYES = connections[ EventPC ].chrlst.name_status.nation;
}
// ½ºÆç Á¾·ù ¾ò¾î¿À±â - yes ¿¡ ½ºÆç Á¾·ù ¹øÈ£ ¹İÈ¯
void sGetSpell()
{
	sYES = connections[ EventPC ].chrlst.Spell;
}
// ÆÄÆ¼ ¼ö ¹İÈ¯ - yes ¿¡ ÇöÀç ¸Î°í ÀÖ´Â ÆÄÆ¼¼ö ¹İÈ¯
void sCheckPartyCount()
{
	int ti;
	
	sYES = 0;
	for( ti = 0; ti < MAX_PARTY_MEMBER; ti++ ) {
		if( connections[ EventPC ].chrlst.party[ti].On ) sYES++;
	}
}
// ÃÖ±Ù »ç¿ëÇÑ ¸¶¹ı - yes ¿¡ ÃÖ±Ù¿¡(¸¶Áö¸·¿¡) »ç¿ëÇÑ ¸¶¹ı¹øÈ£ ¹İÈ¯
void sCheckLastMagic()
{
	sYES = connections[ EventPC ].chrlst.GetMagic();
}
// ÅÃÆ½ °¡Á®¿À±â - yes ¿¡ ÀÎÀÚ°ªÀ¸·Î ¹ŞÀº ÅÃÆ½°ª ¹İÈ¯
void sGetTactic()
{
	int tac = GetInt();	
	sYES = connections[ EventPC ].chrlst.Skill[ 87+tac ];
}
// ¸ğµç ÅÃÆ½Áß Æ¯Á¤ ¼öÄ¡ÀÌ»óÀÇ ÅÃÆ½ÀÌ ÀÖ´ÂÁö Ã¼Å© - yes ¿¡ ¾øÀ¸¸é 0 ÀÖÀ¸¸é 1 ¹İÈ¯
void sCheckTacticPoint()
{
	int mode = GetInt();
	int point = GetInt();
	int ti;
	
	sYES = 0;
	for( ti = 0; ti < 13; ti++ ) {
		
		if( ( mode == 0 && point > connections[ EventPC ].chrlst.Skill[ 88+ti ] )
			|| ( mode == 1 && point == connections[ EventPC ].chrlst.Skill[ 88+ti ] )
			|| ( mode == 2 && point < connections[ EventPC ].chrlst.Skill[ 88+ti ] ) ) {
			
			sYES = 1;
			break;
			
		}
		
	}
	
}


// µà¾ó ÀÎÁö ¾Æ´ÑÁö - yes ¿¡ ÀÎÀÚ°ªÀ¸·Î 0Àº ¾Æ´Ô 1Àº µà¾ó ¹İÈ¯
void sIsDual()
{
	sYES = connections[ EventPC ].chrlst.IsDual();
}


// »ó¿ì ¿äÃ»
void sCallCustomSmallMenu( void )
{
	
	int iMenuNo = GetInt();
	int iPicNo = GetInt();
	int iWidth = GetInt();
	int iHeight = GetInt();
	int iExt = GetInt();
	
	t_packet p;
	p.h.header.type = CMD_CALL_SMALLMENUCUSTOM;
	{
		p.u.server_call_smallmenucustom.iMenuNo = iMenuNo;
		p.u.server_call_smallmenucustom.iPicNo = iPicNo;
		p.u.server_call_smallmenucustom.iWidth = iWidth;
		p.u.server_call_smallmenucustom.iHeight = iHeight;
		p.u.server_call_smallmenucustom.iExt = iExt;
	}
	p.h.header.size = sizeof( t_server_call_smallmenucustom );
	
	QueuePacket(connections, EventPC, &p, 1);
	
}

// 021117 YGI
void sSetRareItemInvPc(void)
{
	const int iItemNo	= GetInt();
	const int iTryMulti	= GetInt();
	const int iGradeMin	= GetInt();
	const int iGradeMax	= GetInt();
	const int iMutant	= GetInt();
	
	CHARLIST *ch = ::CheckServerId( EventPC );
	if( !ch ) return;
	
	ItemAttr item;//021213 lsw 0À¸·Î ³ª´²Áö´Â ¹®Á¦ ¼öÁ¤
	const int iGradeGab = iGradeMax-iGradeMin;
	const int iResultGrade = iGradeMin + ((0 < iGradeGab)?rand()%iGradeGab:0);
	const int ret = ItemMgr.MakeDefaultRareItem( &item, iItemNo, iMutant, iResultGrade, iTryMulti );
	if( ret == 1 )
	{
		::SendItemEventLog( &item, EventPC, SN_NOT_USER, SILT_MAKE_BY_SCRIPT, 2);
		::MoveEmptyInv( &item, ch );
		sYES = 1;
		return;
	}
	sYES=0;
}

// 021106 kyo
int SetRareItemInvPc( const int cn, const int iItemNo, const int iGradeMin, const int iGradeMax, const int iTryMulti, const int iDur, const int iHigh)
{
	if( iHigh != 1 && iHigh != 0 )
		return 0;
	
	int a,b,c;
	CHARLIST *ch =CheckServerId(cn);//&connections[cn].chrlst;
	if( !ch ) return -1;
	
	RareMain RareAttr;
	ItemAttr  Item = ItemMgr.GiveRareItem(iItemNo, iDur ,RARE_ABLE,iTryMulti, iGradeMin, iGradeMax,RareAttr,iHigh);//020725 lsw
	
	if( Item.item_no )
	{
		if( SearchInv( ch->inv, a, b, c ))
		{
			ch->inv[a][b][c] = Item;
			POS pos;
			SetItemPos( INV, a, b, c, &pos );
			SendServerEachItem(  &pos, &Item, cn);		// 001028 YGI
			return 1;
		}
	}
	return 0;
}

extern int g_LocalWarBegin;										// LTS LOCALWAR	
extern cNation* g_pNation;
extern cLocalWarfield* g_pLocalWarfield;						// LTS NEW LOCALWAR

extern bool isNationManageServer();	
extern void SendPacket2NWManager(t_packet* p);

void sLocalWarMode()											
{
	MyLog(0,"LocalWarMode Call LocalWar Status : %d, User Name : %s, JoinLocalWar : %d",g_LocalWarBegin,connections[EventPC].name,connections[EventPC].chrlst.JoinLocalWar);
	if (g_LocalWarBegin&&connections[EventPC].chrlst.JoinLocalWar) sYES=1;
	else sYES=0;
}

void sFameCheck2()
{
	int Type=GetInt();
	int Value=GetInt();
	
	switch(Type)
	{
	case 1 : 
		if (connections[EventPC].chrlst.fame>=Value) sYES=1;
		else sYES=0;
		break;
	case 2 :
		if (connections[EventPC].chrlst.GainedFame>=Value) sYES=1;
		else sYES=0;
		break;
	}
}

void sFameUp2()
{
	int Type=GetInt();
	int Value=GetInt();
	
	switch (Type)
	{
	case 1 : 
		SaveChangeFame(&connections[EventPC].chrlst,connections[EventPC].chrlst.fame,connections[EventPC].chrlst.fame+Value,LF_SCRIPT);		// 020909 LTS
		connections[EventPC].chrlst.fame+=Value;		break;
	case 2 : 
		connections[EventPC].chrlst.GainedFame+=Value;	break;
	}
}

void sNationBelong()
{
	int Nation=connections[EventPC].chrlst.name_status.nation;
	if (Nation==NW_YL)	
		Nation=connections[EventPC].chrlst.NWCharacter.YL_JoinNation;
	
	switch (Nation)
	{
	case NW_BY : sYES=1; break;
	case NW_ZY : sYES=2; break;
	default : sYES=3; break;
	}
}

void FameUp2(const int cn, const int Type, const int Value)	// 021107 kyo
{
	switch (Type)
	{
	case 1 : 
		SaveChangeFame(&connections[cn].chrlst,connections[cn].chrlst.fame,connections[cn].chrlst.fame+Value,LF_SCRIPT);		// 020909 LTS
		connections[cn].chrlst.fame+=Value;		break;
	case 2 : 
		connections[cn].chrlst.GainedFame+=Value;	break;
	}
}

void sFameDown2()
{
	int Type=GetInt();
	int Value=GetInt();
	
	switch (Type)
	{
	case 1 : 
		SaveChangeFame(&connections[EventPC].chrlst,connections[EventPC].chrlst.fame,connections[EventPC].chrlst.fame-Value,LF_SCRIPT);		// 020909 LTS
		connections[EventPC].chrlst.fame-=Value;		break;
	case 2 : connections[EventPC].chrlst.GainedFame-=Value;	break;
	}
}


void sLocalWarPointUp()
{
	t_packet packet;
	
	int NationIndex=GetInt();
	int Value=GetInt();
	
	switch (NationIndex)
	{
	case 3 : NationIndex=0; break;
	case 4 : NationIndex=1; break;
	case 6 : NationIndex=2;	break;
	}
	
	if (isNationManageServer())
	{
		g_pNation->IncLocalWarPoint(NationIndex,Value);
	}
	else
	{
		packet.h.header.type=CMD_LOCALWAR_NATION_POINT_CHANGED;
		switch(NationIndex)
		{
		case 0 :
			packet.u.LocalWar.NationPointChanged.NationPoint[0]=Value;
			packet.u.LocalWar.NationPointChanged.NationPoint[1]=0;
			packet.u.LocalWar.NationPointChanged.NationPoint[2]=0;
			break;
		case 1 :
			packet.u.LocalWar.NationPointChanged.NationPoint[0]=0;
			packet.u.LocalWar.NationPointChanged.NationPoint[1]=Value;
			packet.u.LocalWar.NationPointChanged.NationPoint[2]=0;
			break;
		case 2 :
			packet.u.LocalWar.NationPointChanged.NationPoint[0]=0;
			packet.u.LocalWar.NationPointChanged.NationPoint[1]=0;
			packet.u.LocalWar.NationPointChanged.NationPoint[2]=Value;
			break;
		}
		packet.h.header.size=sizeof(t_NationPointChanged);
		SendPacket2NWManager(&packet);
	}
}

void sLocalWarPointDown()
{
	t_packet packet;
	
	int NationIndex=GetInt();
	int Value=GetInt();
	
	Value*=-1;	//À½¼ö·Î ¸¸µé¾î ÁØ´Ù.
	
	switch (NationIndex)
	{
	case 3 : NationIndex=0; break;
	case 4 : NationIndex=1; break;
	case 6 : NationIndex=2;	break;
	}
	
	if (isNationManageServer())
	{
		g_pNation->IncLocalWarPoint(NationIndex,Value);
	}
	else
	{
		packet.h.header.type=CMD_LOCALWAR_NATION_POINT_CHANGED;
		switch(NationIndex)
		{
		case 0 :
			packet.u.LocalWar.NationPointChanged.NationPoint[0]=Value;
			packet.u.LocalWar.NationPointChanged.NationPoint[1]=0;
			packet.u.LocalWar.NationPointChanged.NationPoint[2]=0;
		case 1 :
			packet.u.LocalWar.NationPointChanged.NationPoint[0]=0;
			packet.u.LocalWar.NationPointChanged.NationPoint[1]=Value;
			packet.u.LocalWar.NationPointChanged.NationPoint[2]=0;
		case 2 :
			packet.u.LocalWar.NationPointChanged.NationPoint[0]=0;
			packet.u.LocalWar.NationPointChanged.NationPoint[1]=0;
			packet.u.LocalWar.NationPointChanged.NationPoint[2]=Value;
		}
		packet.h.header.size=sizeof(t_NationPointChanged);
		SendPacket2NWManager(&packet);
	}
}
/*
extern t_Warfield	g_pWarfieldStatus[NW_NATION_COUNT];		
extern cWarfield*	g_pWarfield;
extern bool			isNationWarfieldServer();

  
	void sWarfieldMany()				// 011213 LTS		// ÇãÁ¢
	{
	int PoseNo=GetInt();
	int BYPose=0,ZYPose=0;
	int Nation;
	
	  if (!isNationWarfieldServer())  {sYES=0; return; }
	  
		if (g_pWarfieldStatus[0].Possession==NW_BY) BYPose++;
		else ZYPose++;
		if (g_pWarfieldStatus[1].Possession==NW_BY) BYPose++;
		else ZYPose++;
		if (g_pWarfieldStatus[2].Possession==NW_BY) BYPose++;
		else ZYPose++;
		
		  Nation=g_pWarfieldStatus[g_pWarfield->GetWarfieldCode()].Possession;
		  if (Nation==NW_BY)
		  {
		  if (BYPose>=PoseNo) sYES=1;
		  else sYES=0; 
		  }
		  else
		  {
		  if (ZYPose>=PoseNo) sYES=1;
		  else sYES=0; 
		  }
		  }
		  
			void sWarfieldOwner()
			{
			int WarfieldNo=GetInt();
			int Nation=GetInt();
			
			  if (Nation==g_pWarfieldStatus[WarfieldNo].Possession) sYES=1;
			  else sYES=0;
			  }
*/
void sSetOXSelect()
{
	int type = GetInt();
	
	CHARLIST *ch = CheckServerId( EventPC );
	
	if( !ch ) return;
	SetOXSelect( ch , type );
}

// 020620 YGI-------------------------
extern void SendCheckGotoScenarioDungeon( short int cn, char *map_name, int x, int y );
void sMoveDungeon()
{
	char map_name[20];
	GetString( map_name);
	int x = GetInt();
	int y = GetInt();
	SendCheckGotoScenarioDungeon( EventPC, map_name, x, y );
}
//-------------------------------------

// 020707 YGI
void sDualCheck()
{
	int type = GetInt();
	
	CHARLIST *ch = CheckServerId( EventPC );
	if( !ch ) return;
	
	sYES = (type == ch->GetClassStep()) ? 1:0;
}

// ¿¥Æ÷¸®¾Æ·Î ÀÌµ¿
void sGotoGuildHouse()					// ±è¿µÁÖ
{	//< CSD-030324
	CHARLIST *ch = CheckServerId( EventPC );
	if( !ch ) return;
	
	sYES = g_GuildHouse.GoToHouse( ch );
	
	if (sYES)
	{
		t_packet packet;
		packet.h.header.type=CMD_GOTO_GUILD_HOUSE_RESULT;
		packet.u.LocalWar.CommonDataDw.Data=ch->GetGuildCode();
		packet.h.header.size=sizeof(t_CommonDataDw);
		QueuePacket(connections,ch->GetServerID(),&packet,1);
	}
}	//> CSD-030324

// ÀÚ±â ±æµå ÁıÀÎÁö È®ÀÎ
void sIsMyGuildHouse()			//±è¿µÁÖ
{
	int event_no = GetInt();
	CHARLIST *ch = CheckServerId( EventPC );
	if( !ch ) return;
	
	sYES = g_GuildHouse.IsMyGuildHouse( ch, event_no );
	
	CGuildHouseUnit *house = g_GuildHouse.GetPointByEventNo( MapName, event_no );
	if (!sYES)
	{
		if (house) house->GoToHouse(ch->GetServerID());
	}
	
	t_packet packet;
	packet.h.header.type=CMD_CHECK_GUILD_HOUSE_RESULT;
	if (house) packet.u.LocalWar.CommonDataDw.Data=house->guild_code;
	else packet.u.LocalWar.CommonDataDw.Data=0;
	packet.h.header.size=sizeof(t_CommonDataDw);
	QueuePacket(connections,ch->GetServerID(),&packet,1);
}

extern int g_EventLocalWarStatus;
extern int g_EventLocalWarStep;

void sCheckEventLocalWarStatus()			// 020115 LTS
{
	sYES=g_EventLocalWarStatus;
}

void sCheckEventLocalWarStep()			// 020115 LTS
{
	sYES=g_EventLocalWarStep;
}

void sCheckEventJoin()					// 020115 LTS	//ÀÌº¥Æ® ¹øÈ£¸¦ ³ÖÀ¸¸é ÂüÀÎÁö ¾Æ´ÑÁö¸¦ ¸®ÅÏ
{
	int EventNo=GetInt();
	switch (EventNo)
	{
	case 1	: sYES=connections[EventPC].chrlst.EventJoin.Event1; break;
	case 2	: sYES=connections[EventPC].chrlst.EventJoin.Event2; break;
	case 3	: sYES=connections[EventPC].chrlst.EventJoin.Event3; break;
	case 4	: sYES=connections[EventPC].chrlst.EventJoin.Event4; break;
	case 5	: sYES=connections[EventPC].chrlst.EventJoin.Event5; break;
	case 6	: sYES=connections[EventPC].chrlst.EventJoin.Event6; break;
	case 7	: sYES=connections[EventPC].chrlst.EventJoin.Event7; break;
	default : sYES=connections[EventPC].chrlst.EventJoin.Event8; break;
	}
}
void sClearEventJoin()
{
	int EventNo=GetInt();
	t_packet packet;
	packet.h.header.type=CMD_DELETE_EVENT_JOIN;
	strcpy_s(packet.u.LocalWar.DeleteEventJoin.UserName, sizeof(packet.u.LocalWar.DeleteEventJoin.UserName),connections[EventPC].name);
	packet.u.LocalWar.DeleteEventJoin.EventNo=EventNo;
	packet.h.header.size=sizeof(t_DeleteEventJoin);
	QueuePacket(connections, DB_DEMON, &packet, 1);
	
	switch (EventNo)
	{
	case 1	:	connections[EventPC].chrlst.EventJoin.Event1=0;	break;
	case 2	:	connections[EventPC].chrlst.EventJoin.Event2=0; break;
	case 3	:	connections[EventPC].chrlst.EventJoin.Event3=0; break;
	case 4	:	connections[EventPC].chrlst.EventJoin.Event4=0; break;
	case 5	:	connections[EventPC].chrlst.EventJoin.Event5=0; break;
	case 6	:	connections[EventPC].chrlst.EventJoin.Event6=0; break;
	case 7	:	connections[EventPC].chrlst.EventJoin.Event7=0; break;
	case 8	:	connections[EventPC].chrlst.EventJoin.Event8=0; break;
	}
}

extern int	 GetNationIndex(int Nation);			// LTS NEW LOCALWAR

void sCheckAllSealStoneBroked()						// LTS NEW LOCALWAR
{
	int Nation=GetInt();
	
	if (!g_pLocalWarfield)
	{
		MyLog(0," Scrip Call Error!! CheckAllSealStoneBroked()");
		sYES=0;
	}
	sYES=g_pLocalWarfield->CheckAllSealStonebroked(GetNationIndex(Nation));
}

void sSetFame()										// LTS NEW LOCALWAR
{
	CHARLIST *ch = CheckServerId( EventPC );
	int sFame=GetInt();
	if( !ch ) return;
	MyLog(0,"User Fame Setting Changed By Script......");
	MyLog(0,"UserName : %s, OldFame : %d, NewFame : %d",ch->Name,ch->fame,sFame);
	ch->fame=sFame;
}

void sCheckQuestMode()								// LTS DRAGON MODIFY
{
	CHARLIST *ch = CheckServerId( EventPC );
	if( !ch ) 
	{
		sYES=0;
		return;
	}
	sYES=ch->QuestMode;
	MyLog(0,"%d User Set Quest Mode : %d",ch->GetServerID(),sYES);
}


void sResetCombatSkill()	//89 // 021010	kyo
{
	g_pBattleManager->RecvCombatReset( EventPC );
}

void sSpellMapMove()	//90 // 021014 kyo
{
	char szSpell[128];
	char szMapName[20];
	GetString( szSpell);
	GetString( szMapName );
	int iMap_x = GetInt();
	int iMap_y = GetInt();
	
	CHARLIST *ch = CheckServerId( EventPC );
	if( !ch ) return;
	
	/* 021128 kyo ¸Ê¼­¹ö¿¡ ºÎÇÏ¸¦ ÁØ´Ü´Ù.
	sYES =  g_QuestInMap.CheckSpell( EventPC, szSpell );
	//int port = GetMapPortByName( dungeon_name );
	if( sYES == 1)
	{//¸ÊÀÌµ¿ÇÑ´Ù.
	sYES = MapMove( EventPC, szMapName, iMap_x, iMap_y );			
	}
	*/
	
	g_QuestInMap.SendSpellMapMove( EventPC, szSpell, szMapName, iMap_x, iMap_y );  // 021128 kyo
}

void sShowStateMessage()		//91 //021015 kyo
{
	GetString( String );
	int iR = GetInt();
	int iG = GetInt();
	int iB =  GetInt();
	
	CHARLIST *ch = CheckServerId( EventPC );
	if( !ch ) return;
	
	g_QuestInMap.ShowStateMsg( EventPC, String, iR, iG, iB );
}


int SearchItemMuch( const int& iItemNum, const int& cn)
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return 0;
	
	int iRet = 0;
	//inv
	for( int a=0; a<3; a++ )
		for( int b=0; b<3; b++ )
			for( int c=0; c<8; c++ )
				if( ch->inv[a][b][c].item_no == iItemNum ) 
					iRet++;
				
					/*
					//equip
					for( a=0; a<8; a++ )
					if( ch->equip[a].item_no == iItemNum ) 
					iRet++;
					
					  //quick
					  for( a=0; a<6; a++ )
					  if( ch->quick[a].item_no == iItemNum ) 
					  iRet++;
				*/
				
				return iRet;
}

bool DeleteItemMuch( const int iItemNum, const int iNumMuch, const int cn)
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return false;
	
	int iCheckNum = 0;
	
	//Áö¿ì·Á´Â ¾ÆÀÌÅÛÀÇ °¹¼ö°¡ °¡Áø°Åº¸´Ù Àû¾î¾ßÁö
	if( iNumMuch > SearchItemMuch( iItemNum, cn ) ) return false;
	
	//¾ÆÀÌÅÛ »èÁ¦´Â ±âÁ¸ÀÇ setiteminvPc ±×´ë·Î
	//inv
	for( int a=0; a<3; a++ )
		for( int b=0; b<3; b++ )
			for( int c=0; c<8; c++ )
				if( ch->inv[a][b][c].item_no == iItemNum ) 
				{					
					ItemAttr target = GenerateItem( iItemNum, 0,0);
					SendItemEventLog( &target, cn, SN_NOT_USER, SILT_MAKE_BY_SCRIPT, 2 );
					ch->inv[a][b][c] = target;
					POS pos;
					SetItemPos( INV, a, b, c, &pos );
					SendServerEachItem(  &pos, &target, cn );		// 001028 YGI
					iCheckNum++;					
					if( iCheckNum >= iNumMuch )
						return true;
				}
				/*
				//equip
				for( a=0; a<8; a++ )
				if( ch->equip[a].item_no == iItemNum ) 
				{
				ItemAttr target = GenerateItem( iItemNum, 0,0);
				SendItemEventLog( &target, cn, SN_NOT_USER, SILT_MAKE_BY_SCRIPT, 2 );
				ch->equip[a] = target;
				POS pos;
				SetItemPos( EQUIP, a, &pos );
				SendServerEachItem(  &pos, &target, cn );		// 001028 YGI
				iCheckNum++;					
				if( iCheckNum >= iNumMuch )
				return true;
				}
				
				  //quick
				  for( a=0; a<6; a++ )
				  if( ch->quick[a].item_no == iItemNum ) 
				  {
				  ItemAttr target = GenerateItem( iItemNum, 0,0);
				  SendItemEventLog( &target, cn, SN_NOT_USER, SILT_MAKE_BY_SCRIPT, 2 );
				  ch->quick[a] = target;
				  POS pos;
				  SetItemPos( QUICK, a, &pos );
				  SendServerEachItem(  &pos, &target, cn );		// 001028 YGI
				  iCheckNum++;					
				  if( iCheckNum >= iNumMuch )
				  return true;
				  }
				*/
				if( iCheckNum >= iNumMuch)
					return true;
				else
					return false;
}


// 021117 YGI
void sCheckItemMulti()							//92 ¿©·¯°³ÀÇ ¾ÆÀÌÅÛÀÌ ÀÖ´Â°¡?
{
	int iItemNumber = GetInt();
	int iNumMuch	= GetInt();
	
	CHARLIST *ch = CheckServerId( EventPC );
	if( !ch ) return;
	
	const int iCurNum = SearchItemMuch( iItemNumber, EventPC );
	
	if( iNumMuch >  iCurNum)		sYES = 0;
	else if( iNumMuch <=  iCurNum)	sYES = 1; // 021121 kyo
	else if( iNumMuch <  iCurNum)	sYES = 2;
}

void sDeleteItemMulti()							//93 ¿©·¯°³ÀÇ ¾ÆÀÌÅÛÀ» Áö¿ì±â
{
	int iItemNumber = GetInt();
	int iDelMuch	= GetInt();
	
	CHARLIST *ch = CheckServerId( EventPC );
	if( !ch ) return;
	
	if( DeleteItemMuch( iItemNumber, iDelMuch, EventPC ) == true )
		sYES = 1;
	else
		sYES = 0;
}

void sScriptTimer()								//94 // 021020 kyo //ÃÊ·Î ½Ã°£À» °è»êÇÑ´Ù. 
{//0ÀÌ¸é Å¸ÀÌ¸Ó¸¦ ¸ØÃç¶ó
	//ÇÑ Å¬¶óÀÌ¾ğÆ®´ç ÇÏ³ª¸¸ÀÇ Å¸ÀÌ¸Ó°¡ ÁÖ¾îÁø´Ù.
	DWORD dwTime;
	
	int iTime = GetInt();
	char mapfile[30];
	GetString( mapfile );
	int x= GetInt();
	int y= GetInt();
	
	CHARLIST *ch = CheckServerId( EventPC );
	if( !ch ) return;
	
	if( iTime > 0 )
		dwTime = iTime* 1000;	//1000ÀÌ 1ÃÊ
	else
		dwTime = 0;
	
	//Å¸ÀÌ¸ÓÀÇ ½ÃÀÛ Å¸ÀÌ¸Ó¸¦ ¹ßµ¿½ÃÅ³¶§ÀÇ cn°ú name¸¦ ±â·Ï½ÃÅ²´Ù. 
	if( g_QuestInMap.StartScriptTimer( EventPC, ch->Name, dwTime , MIN_SYNCH_TIME, mapfile, x, y) )
		sYES = 1;
	else
		sYES = 0;
	
}

void sScriptCounter()				//95 // 021023 kyo 
{	//(¼ıÀÚ¼¿²¨ ¹øÈ£, Á¾·ù, °¹¼ö, ÇÃ·¯±×)
	//Á¾·ù: 0=ncp,1=¾ÆÀÌÅÛ,2=¹ÌÁ¤. 
	//ÇÃ·¯±×: 0=¸ØÃã,1=½ÃÀÛ,2=°¹¼ö¶û ÀÏÄ¡
	int iNumber = GetInt();
	int iType = GetInt();
	int iMuch = GetInt();
	int iFlag = GetInt();
	
	CHARLIST *ch = CheckServerId( EventPC );
	if( !ch ) return;
	
	if( g_QuestInMap.StartScriptCounter( EventPC, ch->Name, iNumber, iType, iMuch, iFlag ) )
		sYES = 1;
	else
		sYES = 0;
}

// 021117 YGI
void sUpRareInEquip()			//96 // 021028 kyo
{	// iii
	int iEquipPos = GetInt();
	const int iMulti = GetInt();
	const int iMin = GetInt();
	const int iMax	= GetInt();
	int iMutant	= GetInt();
	
	// start of code add by rogerio
	// 8 dont exicts
	if (iEquipPos == 8) {
		iEquipPos = rand()%8;
	}

	if (iMutant == 0){
		iMutant = rand()%88;
	}
	if (iMutant == 29 ||iMutant == 45 ||  iMutant == 48 ||  iMutant == 51 ||  iMutant == 52 ||  iMutant == 54 ||  (iMutant >= 56 && iMutant <= 67) ||  iMutant == 78 ||  iMutant == 84 ){
		iMutant = rand()%44;	
	}
	// end of code add by rogerio

	CHARLIST *ch = CheckServerId( EventPC);	
	if( !ch) return;
	
	//Â÷·Ê
	// ÀåÂø µÇ¾î ÀÖ´Â ¾ÆÀÌÅÛ ¹øÈ£¸¦ ¾ò¾î ¿Â´Ù.
	// ±× ¾ÆÀÌÅÛ ¹øÈ£¸¦ ±âÃÊ·Î ·¹¾î ¾ÆÀÌÅÛÀ» ¸¸µç´Ù.
	// ÀÎº¥Åä¸®¿¡ ³Ö¾î ÁØ´Ù.
	
	if( WT_WEAPON > iEquipPos || iEquipPos > WT_UNIQUE2 )
	{//À§Ä¡ ¿¡·¯
		sYES = 0; return;
	}
	int item_no = ch->equip[iEquipPos].item_no;
	if (item_no == 8305 || item_no == 8308 || item_no == 8309 || item_no == 8311 ||item_no == 8312 || item_no == 8313 || item_no == 8315 ){
		item_no = 8301;		// Add by rogerio
	}
	if( !item_no ) 
	{// ¾ÆÀÌÅÛÀ» ¾È µé°í ÀÖ´Ù.
		sYES = 0; return;
	}

	int grade_gab = iMax-iMin+1;
	
	if(0 >= grade_gab)
	{
		sYES = 0; return;
	}

	ItemAttr item = {0,};

	int ret = ItemMgr.MakeDefaultRareItem( &item, item_no, iMutant, rand()%grade_gab+iMin, iMulti );

	if( ret == 1 )		// ¼º°ø
	{
		SendItemEventLog( &item, EventPC, SN_NOT_USER, SILT_MAKE_BY_SCRIPT, 2 );
		MoveEmptyInv( &item, ch );
	}
	else
	{
		sYES = 2; return;
	}
	
	sYES = 1;
}

void sBossTimerCheck()		//97 // 021106 kyo
{//iiiiii (npc¹øÈ£,x,y, ÀÎ¿øÁ¦ÇÑ, Å¸ÀÌ¸Ó½Ã°£, Á¦¾à½Ã°£)
	const int iNPC = GetInt();
	const int iNpc_x = GetInt();
	const int iNpc_y = GetInt();
	const int iMaxEntree = GetInt();//Entree limit 
	const int iTime = 1000*GetInt();
	const int iLimiteTime = 1000*GetInt();	// 030123 kyo
	
	const int nFlag = GetInt();	// 031028 kyo

	CHARLIST *ch = CheckServerId(EventPC);
	if( !ch) return;
	
	int nKey = iNpc_x*1000 + iNpc_y;

	//flag 0 -> º¸½º ÇÑ¸í µîÀå
	//flag 1 -> º¸½º ¿©·µ µîÀå
	//flag 2 -> º¸½º¹æ¿¡¼­ ³ª¿À°í ´Ù ³ª¿À¸é ¹æ Á¦°Å
	if( nFlag == CBossTimer::TYPE_BOSS_OUT )
	{
		g_QuestInMap.DecreaseBossTimerUser( nKey, EventPC );
		g_QuestInMap.CheckBossTimerBoss( nKey);
		sYES= -1;
		return;
	}

	if( g_QuestInMap.GetBossTimerEntree(nKey) > iMaxEntree)
	{
		sYES = 0;
		return;
	}

	if( g_QuestInMap.GetCurrentBossTimer(nKey) <= (iTime - iLimiteTime) )	//Å¸ÀÌ¸Ó ¾øÀ¸¸é 0, ÀÖÀ¸¸é Å¸ÀÌ¸Ó½Ã°£(dwtime)
	{
		sYES = ( g_QuestInMap.StartBossTimer( EventPC, iNPC, iNpc_x, iNpc_y, iTime, iMaxEntree, nFlag) )/1000; //031028 kyo
	}
	else //Á¦¾à½Ã°£¾È¿¡ ÀÖ´Ù.
	{
		sYES = 1;
	}
}

void sIsGetMagic()
{// 030415 kyo 
	const int nMagic = GetInt();
	CHARLIST *ch = CheckServerId(EventPC);
	if( !ch) return;
	switch (ch->Spell)
	{
    case WIZARD_SPELL:
		{
			if (ch->Ws[nMagic])
			{
				sYES = 1;
			}
			break;
		}
    case PRIEST_SPELL:
		{
			if (ch->Ws[nMagic])
			{
				sYES = 1;
			}
			break;
		}
	default:
		{
			sYES = 0;	
			break;
		}
	}
}

void sDualClassCheck()
{ //100 ³»°¡ ÇØ´ç µà¾óÅ¬·¡½ºÀÎ°¡ ¸ÂÀ¸¸é 1 Æ²¸®¸é 0
	const int n1Class = GetInt();
	const int n2Class = GetInt();
	CHARLIST *ch = CheckServerId(EventPC);
	if( !ch) return;
	sYES = 0; // 030513 kyo
	if(ch->Class == n1Class || ch->Class >= 5)
	{
		if(ch->GetDualClass()== n2Class)
		{
			sYES =1;
		}
	}
	else
		sYES =0;
}

void sUpDualClassItem()
{ //101 µà¾óÅ¬·¡½º¾ÆÀÌÅÛÀ» ÇÑ´Ü°è¿Ã·ÁÁØ´Ù.
	
	const int nFlag = GetInt();
	CHARLIST *ch = CheckServerId(EventPC);
	if( !ch) return;

	if(0 == nFlag)
	{
		sYES = g_CSymbolMgr.GetSymbolGrade(ch);
	}
	else if(1==nFlag)
	{
	
		if( !g_CSymbolMgr.DoHaveSymbol(ch) )
		{
			g_CSymbolMgr.MakeSymbolItem(ch);
			sYES=0;
		}
		else
		{
			int nDualStep = g_CSymbolMgr.GetSymbolGrade(ch);
			sYES = nDualStep;
			g_CSymbolMgr.UpGradeSymbol( ch, ++nDualStep);
		}
	}

}

void sShowLearnDualMagic()
{// 102 µà¾ó¸¶¹ıÀ» ¹è¿ï¼ö ÀÖ´Â Ã¢À» ¶ç¿î´Ù.
	::SendCallDualMagicLearnMenu(EventPC);
}

void sLogDualQuest()
{// 103 iisi µà¾óÄù½ºÆ®¸¸ ·Î±×¸¦ ³²±ä´Ù.
	const int nQnum = GetInt();	//Äù½ºÆ® ¹øÈ£
	const int nQCount = GetInt();	//¼öÇàÈ½¼ö.
	char szMsg[128];
	GetString( szMsg);	//¸Ş½ÃÁö ³»¿ë
	const int nStart = GetInt();	//½ÃÀÛÀÎÁö ¾Æ´ÑÁö

	CHARLIST *ch = CheckServerId( EventPC);
	if( !ch ) return;
	//< CSD-TW-030618
	//::LogInFile("./changelog/DualQuestLog.txt", "chName=%s, QuestNum=%d, Qcount=%d, Msg=%s, Start=%d", ch->Name, nQnum, nQCount, szMsg, nStart);
	SaveChangeClassQuest(ch, szMsg, nQnum, nQCount, nStart);
}	//> CSD-TW-030618

void sLogForScript()
{// 104 is ½ºÅ©¸³Æ®¸¦ ¼öÇàÇÑ ·Î±×¸¦ ³²±ä´Ù.  // 030826 kyo
	const int nQnum = GetInt();
	char szMsg[128];
	GetString(szMsg);

	if( 0>nQnum|| _SCRIPT_VARABLE_POINT_<=nQnum ) 
	{
		return;
	}
	CHARLIST *ch = CheckServerId( EventPC);
	if( !ch ) return;

	SaveScriptLog(ch, nQnum, var[EventPC][nQnum], szMsg);//kyo
}


//<! BBD 040308	ÀÌº¥Æ® ¾ÆÀÌÅÛ Áö±ŞÀ» ÇØÁÖ´Â ÇÔ¼ö
void sEventItemInsert()
{
	GetString( String );
	
	CHARLIST *ch = CheckServerId( EventPC );
	if (ch == NULL) return; // finito null check

	if(connections[ EventPC].dwAgentConnectionIndex && connections[ EventPC].state >= CONNECT_JOIN)
	{
		//SendBBS2¶ó´Â ÀÌº¥Æ® ¸Ş¼¼Áö ¹ß»ı ÇÔ¼ö¸¦ ½ÇÇàÇÑ´ç.
		SendBBS2( String, (short)EventPC );
		
		if(!ch->IsEventRecv())
		{
			// ÀÎº¥ÀÇ ºóÄ­ ¼¼±â
			int blankcount = 0;
			for(int a=0; a<3; a++)
			{
				for(int b=0; b<3; b++)
				{
					for(int c=0; c<8; c++)
					{
						if( !ch->inv[a][b][c].item_no )
						{
							blankcount++;
						}
					}
				}
			}
			
			if(blankcount < 10)	// 10Ä­ ÀÌ»óÀÎ°¡?
			{
				::OutMessage(ch,2,13);//ÀÓ½Ã,,ÀÎº¥Åä¸® °ø°£ÀÌ ÃæºĞÄ¡ ¾Ê½À´Ï´Ù.
				return;
			}


			t_packet	p;
			p.h.header.type = CMD_EVENTRAREITEM_REQ;
			p.h.header.size = sizeof(t_EVENT_RARE_ITEM_LIST);
			
			p.u.Event_Item_List.nCn = EventPC;
			strcpy_s(p.u.Event_Item_List.szChrName, sizeof(p.u.Event_Item_List.szChrName), ch->Name);
			strcpy_s(p.u.Event_Item_List.szLoginID, sizeof(p.u.Event_Item_List.szLoginID), connections[ EventPC].id);
			
			QueuePacket(connections,DB_DEMON,&p,1);

			ch->SetEventRecv(true);
		}
		
	}	
}
//> BBD 040308	ÀÌº¥Æ® ¾ÆÀÌÅÛ Áö±ŞÀ» ÇØÁÖ´Â ÇÔ¼ö

//<! kjy 040406  Ä³¸¯ÅÍ ´É·ÂÄ¡ °¡Á®¿À´Â ÇÔ¼ö
void sGetCharacterStatus()
{
	int type = GetInt();
	int type2 = GetInt();
	CHARLIST *ch = CheckServerId( EventPC );
	if ( !ch ) return;

	sYES = 0;
	switch( type )
	{
		case STR:	sYES = 	ch->Str;		break;
		case CON:	sYES =	ch->Con;		break;
		case DEX:	sYES =	ch->Dex;		break;
		case WIS:	sYES =	ch->Wis;		break;
		case INT_:	sYES =	ch->Int;		break;
		case MOVP:	sYES =	ch->MoveP;		break;
		case CHA:	sYES =	ch->Char;		break;
		case ENDU:	sYES =	ch->Endu;		break;
		case MOR:	sYES =	ch->Moral;		break;
		case LUCK:	sYES =	ch->Luck;		break;
		case WSPS:	sYES =	ch->wsps;		break;
	}

	if ( sYES < type2 )
	{
		sYES = 0;
	}
	else if ( sYES >= type2 )
	{
		sYES = 1;
	}
} 
//> kjy 040406  Ä³¸¯ÅÍ ´É·ÂÄ¡ °¡Á®¿À´Â ÇÔ¼ö


//<! kjy 040406  ·¹¾î ¾ÆÀÌÅÛ °³¼ö È®ÀÎ ÇÔ¼ö
void sSearchItemMuchForRare()
{
	int item_number = GetInt();
	int rare_type	= GetInt();
	int grade		= GetInt();
	int tt			= GetInt();

	CHARLIST *ch = CheckServerId( EventPC );
	if( !ch ) return;

	int iRet = 0;
	
	// º£³¶ ¼ÓÀÇ ¸ğµç ¾ÆÀÌÅÛ °Ë»ö..
	for( int a=0; a<3; a++ )
		for( int b=0; b<3; b++ )
			for( int c=0; c<8; c++ )
			{
				ItemAttr *pItem = &ch->inv[a][b][c];
				
				if( pItem->item_no == item_number ) 
				{
					RareMain *pRare = (RareMain *)&pItem->attr[IATTR_RARE_MAIN];
	
					if ( pRare->soksung1 == rare_type && pRare->grade == grade )
					{
						iRet++;
					}
				}
			}

	if ( iRet < tt )		sYES = 0;
	else if ( iRet >= tt )	sYES = 1;
}
//> kjy 040406  ·¹¾î ¾ÆÀÌÅÛ °³¼ö È®ÀÎ ÇÔ¼ö

void sCheckItemQuantity()	// by Rogerio
{
	int iItemNumber = GetInt();
	int iNumMuch	= GetInt();
	
	CHARLIST *ch = CheckServerId( EventPC );
	if( !ch ) return;	

	const int iCurNum = GetItemMuchByMuchItem(ch,iItemNumber);	
		
	if( iNumMuch >  iCurNum)		sYES = 0;
	else if( iNumMuch <=  iCurNum)	sYES = 1; // 021121 kyo
	//else if( iNumMuch <  iCurNum)	sYES = 2;
}

void sDeleteItemQuantity()	// by Rogerio
{
	int iItemNumber = GetInt();
	int iDelMuch	= GetInt();
	
	CHARLIST *ch = CheckServerId( EventPC );
	if( !ch ) return;	
	
	if( SendDeleteMuchItemQuantity(ch, iItemNumber, iDelMuch ) == 1 )
		sYES = 1;
	else
		sYES = 0;
	
}

void sLearnItem()
{
	CHARLIST *ch = CheckServerId( EventPC );
	if( !ch ) return;

	sYES = 0;

	int item_no = GetInt();
	int type, num;
	
	getItemIndex( item_no, type, num );
	CItem *t = ItemUnit( type, num );
	if( !t ) return;

	int join_num = GetItemJoinNumber( item_no );		// joinÅ×ÀÌºí¿¡¼­ÀÇ ÆÄ¶ó¸ŞÅ¸ ¹øÈ£
	if( !item_join[join_num].item_id ) return;					// Check if item exists
	
	int ct = SkillMgr.FindLearnItem( ch, item_no ) ;
	if( ct == -1 ) return ;					// ¹è¿î¾ÆÀÌÅÛÀÏ °æ¿ì 0À» ¸®ÅÏÇÑ´Ù.
	if( ct == MAX_LEARN_ITEM ) return;	// return if reached max number of items that can be learnt

	if( !SkillMgr.CanLearnItem( ch, join_num ) ) return;		// Checks if user has the requirements to learn 

	ch->Item[ct] = item_no;				// Store the new learnt item into the item array of user data
	ch->ItemMax++;						// Increase number of learnt items

	// Tell client user has learnt a new item
	t_packet p;
	p.h.header.type = CMD_LEARN_ITEM;
	{
		p.u.kein.item_no.item_no = item_no;
	}
	p.h.header.size = sizeof( k_item_no );
	QueuePacket(connections,ch->GetServerID(),&p,1);

	sYES = 1;
}

extern void SaveQuestExp(CHARLIST* pTarget, int nKind, int nParty, int nAmount, int nLoops); //Eleval 16/08/09 - Log of exp earned in quest

void sAddExp() //Made by Eleval, to add exp where we want by quest hehe
{
	int exp_type = GetInt(); //Getting what kind of EXP it is (0 = no exp, 1 = what player choosed in party menu, 2 = exp, 3 = weapon, 4 = spell, 5-? = job)
	int max_amount = GetInt(); //Getting the amount of exp
	int loops = GetInt(); //Getting the number of times it should be given

	int exp_amount = max_amount / 100;

	char ExpNotice[128];

	CHARLIST *ch = CheckServerId( EventPC );
	if( !ch ) return;

	if(exp_type >= 1)
	{
		for(int i = 0 ; i < loops ; i++)
		{
			switch(ch->party_select)
			{
				case 0:
				case PS_EXP:
				{ 
					for(int i = 0 ; i < 100 ; i++)
					{
						ch->ChangeUpAddExp(exp_amount);
						isLevelUp(ch);
					}
					sprintf(ExpNotice, "You have won %d Exp. in your Main Level", max_amount);
					break;
				} 
				case PS_TACTIC:
				{ 
					const int nKind = ch->GetTacticsKind() - TACTICS_Crapple;
					for(int i = 0 ; i < 300 ; i++)
					{
						ch->IncTacticExp(nKind, exp_amount);
						ch->LevelUpTactics(nKind);
						//ch->ChangeUpTacExp(0, exp_amount*3);
					}
					int new_amout = max_amount *3;
					sprintf(ExpNotice, "You have won %d Exp. in your Weapon Level", new_amout);
					break;
				} 
				case PS_MAGIC_TAC:
				{
					const int nKind = (ch->Spell == 0) ? TACTICS_Magery - TACTICS_Crapple:
					TACTICS_Orison - TACTICS_Crapple;
					for(int i = 0 ; i < 300 ; i++)
					{
						ch->IncTacticExp(nKind, exp_amount);
						ch->LevelUpTactics(nKind);
					}
					int new_amout = max_amount *3;
					sprintf(ExpNotice, "You have won %d Exp. in your Magic Level", new_amout);
					break;
				} 
			}
			if(connections[ EventPC].dwAgentConnectionIndex && connections[ EventPC].state >= CONNECT_JOIN)
			{
				SendBBS2( ExpNotice, (short)EventPC );
			}	
		}
	}

	//MyLog(0, "'%s' has won %d Exp from Quest", ch->Name, exp_amount);

	//Need to put log in LogOutput about exp won here.
	SaveQuestExp(ch, exp_type, ch->party_select, max_amount, loops); //Log put by Eleval 16/08/09 - In LogOutput/Change
}

void sAddQuestItemToNpc() //Made by Eleval, to set a monster to put a specific item into its killer bag.
{
	int monster_id = GetInt();
	int item_id = GetInt();
	int amount = GetInt();
	int bound = GetInt();

	CHARLIST *ch = CheckServerId( EventPC );
	if(!ch) return;

	Npc_Quest_Item nqi;

		strcpy(nqi.player_name, ch->Name);
		nqi.npc_id = monster_id;
		nqi.amount = amount;
		nqi.item_id = item_id;
		nqi.bound = bound;

	NpcQuestItem.push_back(nqi);

	//MyLog(0, "QuestItemTable: player: '%s', monster: '%d', amount: '%d', item_id: '%d', bound: '%d'", NpcQuestItem[i].player_name, NpcQuestItem[i].npc_id, NpcQuestItem[i].amount, NpcQuestItem[i].item_id, NpcQuestItem[i].bound);
}

void sCheckTamedNear() //Made by Eleval, to check if a player has a specified tamed monster near him (if sent value == -1 then can be any summon)
{
	int monster_id = GetInt();

	CHARLIST *ch = CheckServerId( EventPC );
	if(!ch) return;
	CHARLIST *n = NULL;

	sYES = 0;

	if(!ch->TamedMonster.empty())
		for(int i = 0 ; (i < ch->TamedMonster.size()) ; i++)
		{	
			if(ch->TamedMonster[i] != -1)
			{
				n = &NPCList[ch->TamedMonster[i]];
				if(n->npc_index == monster_id)
				{
					int distanceX = abs(ch->X - n->X) / TILE_SIZE;
					int distanceY = abs(ch->Y - n->Y) / TILE_SIZE;

					if(distanceX <= 5 && distanceY <= 5)
						sYES = 1;
				}
			}
		}
	else
		sYES = 0;
}

void sGiveMoney() //Eleval 20/07/09 - To give money throught quests, easier than the give item script
{
	int amount = GetInt();

	CHARLIST *ch = CheckServerId( EventPC );

	if(!ch) return;
	
	::GiveMoney(ch, amount);

	char MoneyNotice[128];

	sprintf(MoneyNotice, "You've earned %d CRIT !", amount);

	if(connections[ EventPC].dwAgentConnectionIndex && connections[ EventPC].state >= CONNECT_JOIN)
	{
		SendBBS2( MoneyNotice, (short)EventPC );
	}	

	//Need to put a log here, no? (in LogOutput)
}

void sScenarioQuestTeleport() //Eleval 10/08/09 - To bring back ppl in quest
{
	int nbMap = GetInt();

	sYES = 1;

	CHARLIST *ch = CheckServerId( EventPC );

	if(!ch)
		return;

	if(g_IsSQuestATM && ch->s_quest_floor != -99)
	{

		t_packet packet;

		packet.h.header.type = CMD_ASK_SQUEST_TELEPORT;
		packet.h.header.size = sizeof(t_ask_squest_tp);
		packet.u.ask_squest_tp.cn = EventPC;
		packet.u.ask_squest_tp.nbMap = nbMap;
		sprintf(packet.u.ask_squest_tp.charname, "%s", ch->Name);
		packet.u.ask_squest_tp.map_port = GetOwnPort();

		ch->s_quest_floor = -99;

		//QueuePacket(connections, DB_DEMON, &packet, 1);
		if (CheckMapPort(MN_SCENARIO_MAP))
			RecvAskSQuestTeleport(&packet);
		else
			SendPacket2Map(&packet, MN_SCENARIO_MAP);
	}
	else
	{
		sYES = 0;
	}

}

void (*SFunction[_SCRIPT_FUNCTION_POINT_])(void) =
{	
	sDialOnlyPC,			//  0
	sDialInAreaPC,
	sDialEveryone,
	sAddItem,				//  4 //Æ¯Á¤ À§Ä¡¿¡ ¾ÆÀÌÅÛ °®´Ù ³õ±â (xÁÂÇ¥ * 32, yÁÂÇ¥ *32)
	sAddNpc,				//  5 //Æ¯Á¤ À§Ä¡¿¡ NPC °®´Ù ³õ±â	(xÁÂÇ¥ , yÁÂÇ¥)
	sMapName,
	sMapMove,				//  7 
	sIsLevel,
	sInitMonsterGeneratedPosition,
	sCallSmallMenu, 
	sWeatherOn,				// 10 // ³¯¾¾¿¡ °üÇØ BroadCastÇÑ´Ù. 
	sWeatherOff,			// 11 // ³¯¾¾¿¡ °üÇØ BroadCastÇÏÁö ¾Ê´Â´Ù.  
	sRecallScriptStart,		// 12
	sRecallScriptEnd,		// 13
	sMaxMonsterGeneration,	// 14
	sLoadItemValueOneMaul,  // 15 //031116 lsw Á¦°ÅµÈ ÇÔ¼ö.
	srandom,				// 16 // 0131 YGI
	sEventMsg,				// 17 //ÀÌº¥Æ® ¸Ş¼¼Áö¸¦ »Ñ·ÁÁØ´Ù.
	sIsItemHavePC,			// 18 // 0720 LHS Æ¯Á¤ ¾ÆÀÌÅÛÀ» °¡Áö°í ÀÖ´ÂÁö È®ÀÎÇÑ´Ù. ÀåÂø¿¡ °üÇØ¼­
	sIsClassPC,				// 19 // 0721 LHS Æ¯Á¤ Å¬·¡¼­ÀÎ°¡? i
	sSetQuestNumStep,		// 20 // 0721 LHS Äù½ºÆ® ¹øÈ£¸¦ ¼³Á¤ÇÑ´Ù. ii
	sCmpQuestNumStep,		// 21 // 0722 LHS °°Àº Äù½ºÆ® ¹øÈ£¿¡¼­ ÁøÇà ´Ü°è¸¦ È®ÀÎÇÑ´Ù. 0 ³·Àº°ª 1 °°Àº°ª 2 ³ôÀº°ª ii
	sQuestComplete,			// 22 // 0722 LHS Äù½ºÆ® ¿Ï·á½Ã ÃàÆ÷ºü¹æ~~ i
	sQuestTeleport,			// 23 // 0726 LHS Äù½ºÆ®ÅÚ·¹Æ÷Æ® iii
	sIsJobPC,				// 24 // 0728 LHS ÇØ´ç Á÷¾÷ÀÎ°¡? i	
	sIsInvHavePC,			// 25 // 0728 LHS ÄüÀÎº¥ÀÌÅ¢¿¡ ÇØ´ç ¾ÆÀÌÅÛÀÌ ÀÖ´Â°¡? i
	sSetItemInvPC,			// 26 // 0729 LHS ÀÎº¥Åä¸®Ã¢¿¡ ÇØ´ç ¾ÆÀÌÅÛÀ» ´Ù¸¥ ¾ÆÀÌÅÛÀ¸·Î º¯°æÇÑ´Ù.(¹Ù²ğ¾ÆÅÛ,¹Ù²Ü¾ÆÅÛ,¾ÆÅÛµà) iii
	sIsSkillHavePC,			// 27 // 0729 LHS ½ºÅ³À» ¹è¿ü´Â°¡? i
	sSetSkillPC,			// 28 // 0729 LHS ½ºÅ³À» ¹è¿ì°Ô ÇÑ´Ù. ¾È¹è¿î°Å·Î ÇÑ´Ù. ii(½ºÅ³,¹è¿ò/¾È¹è¿ò)	
	sQuestSound,			// 29 // 0809 LHS Äù½ºÆ®¿ë »ç¿îµå i
	sAddFightMap,			// 30 // 0905 YGI ÀüÅõ¸Ê¿¡¼­ ÆÀ µî·Ï i
	sIsPlayArenaGame,		// 31 // CSD-030517 : ¾Æ·¹³ª °æ±â ÁßÀÎÁö ¿©ºÎ °Ë»ç
	sIsFullFightMapTeam,	// 32 // 0907 YGI ÀüÅõ¸Ê¿¡¼­ ÆÀ¿ø ¸ğµÎ µî·ÏÇÑ »óÅÂÀÎÁö ¾Æ´ÑÁö i( ÆÀ ¹øÈ£ )
	sDeleteFightMap,		// 33 // 0915 YGI ÀüÅõ¸Ê¿¡¼­ ÆÀ ÇØÁ¦ ÇÏ±â 
	sCheckNation,
	sReturnSealStoneBreakable,	// 35 // 001026 KHS sYES == 1 ÀÌµ¿°¡´É, sYES == 0 ÀÌµ¿ºÒ°¡.
	// Added by chan78
	sSendMessageBox,			// 36 // 001124 YGI  // ¸»Ç³¼±ÀÌ ¶ß°Ô ÇÑ´Ù.		// 001212 YGI
	sSendSound,					// 37 // 001205 YGI		// Æ¯Á¤ »ç¿îµå ¹øÈ£ÀÇ »ç¿îµå¸¦ Ãâ·ÂÇÏ°Ô ÇÑ´Ù. 001212 YGI
	sGenMonSpec,				// 38 // 010129 KHS
	sGetGender,					// 39 // 010205 YGI				// 010210 YGI
	
	// 010303 YGI
	sSaveItemAtDb,				// 40 // Æ¯Á¤ ¾ÆÀÌÅÛÀ» µğºñ¿¡ ÀúÀåÇÑ´Ù.
	sCheckMoney,				// 41 // µ·ÀÌ ¾ó¸¶³ª ÀÖ´ÂÁö ¾Ë¾Æº»´Ù.
	sSubtractMoney,				// 42 // pcÇÑÅ×¼­ µ·À» »©³½´Ù.
	sGetGuild,					// 43 // ±æµå ÄÚµå¸¦ °¡Á®¿Â´Ù. sYES ¿¡ ÀúÀåµÈ´Ù.		// 010511 YGI
	sSendEffect,				// 44	// effect º¸¿©ÁÖ±â 010522 YGI
	
	sCallMenu,					// 45	Á÷Á¢ µğÆÄÀÎµÈ ¹øÈ£·Î ¸Ş´º¸¦ È£ÃâÇÑ´Ù.		// 010528 YGI
	sGetNationByMap,			// 46	Æ¯Á¤ ¸ÊÀÇ ±¹Àû¾Ë¾Æ ¿À±â
	sCalcFriendShip,			// 47	Æ¯Á¤ ±¹°¡ÀÇ Àû´ë¼öÄ¡ ´õÇØÁÖ±â
	sCompFriendShip,			// 48	Àû´ë¼öÄ¡ ºñ±³ÇÏ±â
	sCompLadderScore,			// 49	·¡´õ Á¡¼ö ºñ±³					// 010531			YGI
	sCheckLoadingPoint,			// 50	ºÎ´ëÀÇ °ø°İ°ÅÁ¡À» È®ÀÎ			// 010904 LTS
	sCheckDefence,				// 51	¹æ¾îÀÚ °ø°İÀÚ¸¸ ÀÌµ¿°¡´É ÇÑ Æ÷Å»..		// 010904 LTS
	sFameDown,					// 52   Ä³¸¯ÅÍÀÇ ÆäÀÓÀ» ³·Ãá´Ù			// 010915 LTS
	sFameUp,					// 53	Ä³¸¯ÅÍÀÇ ÆäÀÓÀ» ³ôÀÎ´Ù
	sFameCheck,					// 54	Ä³¸¯ÅÍÀÇ ÆäÀÓÀÌ ÁÖ¾îÁø °ªÀÌ»óÀÎÁö ÆÇº°ÆÇ´Ù
	sCanGoBonus,				// 55	// ±¹°¡Àü º¸³Ê½º ¸ÊÀ¸·Î °¥¼ö ÀÖ´Â ±â°£ÀÎÁö È®ÀÎÇÑ´Ù.
	// CSB AddCode
	sGetLevel,					// 56
	sGetClass,					// 57
	sGetAbil,					// 58
	sCheckParty,				// 59	
	sGetNat,					// 60
	sGetSpell,					// 61
	sCheckPartyCount,			// 62
	sCheckLastMagic,			// 63
	sSetTimer,					// 64
	sGetTactic,					// 65
	sCheckTacticPoint,			// 66
	sIsDual,					// 67
	sStopTimer,					// 68
	sCallCustomSmallMenu,		// 69
	
	sSetRareItemInvPc,			// 70
	sLocalWarMode,				// 71		// LTS LOCALWAR
	sFameCheck2,				// 72
	sFameUp2,					// 73
	sFameDown2,					// 74
	sLocalWarPointUp,			// 75
	sLocalWarPointDown,			// 76
	// CSB AddCode End
	// Ãß°¡½Ã ¾Æ·¡ ¸Æ½º °ªÀ» ´Ã·ÁÁØ´Ù.
	sCheckEventLocalWarStatus,	// 77		// 020115 LTS
	sCheckEventLocalWarStep,	// 78		// 020115 LTS
	sCheckEventJoin,			// 79		// 020115 LTS
	sClearEventJoin,			// 80		// 020115 LTS
	
	sSetOXSelect,				// 81		// YGI
	sGotoGuildHouse,			// 82	
	sIsMyGuildHouse,			// 83		// i ( ÀÌº¥Æ® ¹øÈ£ )
	sCheckAllSealStoneBroked,	// 84		// LTS NEW LOCALWAR
	sSetFame,					// 85		// LTS NEW LOCALWAR
	sCheckQuestMode,			// 86		// LTS DRAGON MODIFY
	// 020620 YGI
	sMoveDungeon,				// 87		// sii
	sDualCheck,					// 88		// i		// 020707 YGI
	sResetCombatSkill,    // 89    //   // 021008 KYO
	sSpellMapMove,				//90		// ssii (ÁÖ¹®, Áöµµ,Áöµµx,Áöµµy) // 021014 kyo ÁÖ¹®À» ¿Ü¸é ÀÌµ¿ÇÑ´Ù. 
	sShowStateMessage,			//91		// siii	(¸Ş½ÃÁö, R,G,B )	//»ö¿¡ ¸Â´Â ±ÛÀÚ¸¦ º¸¿©ÁØ´Ù.
	sCheckItemMulti,			//92 		// ii   ´Ù¼öÀÇ ¾ÆÀÌÅÛ È®ÀÎ (¾ÆÀÌÅÛ¹øÈ£, °¹¼ö)  yes°ª ½ÇÆĞ =0, ¼º°ø =1		
	sDeleteItemMulti,			//93		// ii	´Ù¼öÀÇ ¾ÆÀÌÅÛ »è¼¼ (¾ÆÀÌÅÛ¹øÈ£, °¹¼ö)  yes°ª ½ÇÆĞ =0, ¼º°ø =1
	sScriptTimer,				//94		// isii	(Å¸ÀÌ¸Ó ¼³Á¤ ½Ã°£, ¸ÊÀÌ¸§,¸Êx,¸Êy) //¼³Á¤ ½Ã°£: 0=ÇØÁ¦, 0!=½Ã°£
	sScriptCounter,				//95		// iiii (¼ıÀÚ¼¿²¨ ¹øÈ£, Á¾·ù, °¹¼ö, ÇÃ·¯±×) Á¾·ù: 0=ncp,1=¾ÆÀÌÅÛ,2=¾ÈÁ¤ÇØµû. ÇÃ·¯±×: 0=¸ØÃã,1=½ÃÀÛ,2=°¹¼ö¶û ÀÏÄ¡?
	sUpRareInEquip,				//96		// iii	ÀåÂø³ë¸»¾ÆÀÌÅÛÀ» ·¹¾î ¾ÆÅÛÀ¸·Î ¹Ù²Û´Ù.: ¼Ó¼º¸¸ ¿Ã¸² ( ÀåÂøÀ§Ä¡, µî±Ş, ¼Ó¼º) yes°ª 0=¾ÆÅÛ¾ø´Ù, 1=¼º°ø, 2=³ë¸»¾ÆÅÛ¾Æ´Ô
	sBossTimerCheck,			//97		// iiiiii (npc¹øÈ£,x,y, ÀÎ¿øÁ¦ÇÑ, Å¸ÀÌ¸Ó½Ã°£, Á¦¾à½Ã°£)
	// yes 0=ÀÎ¿øÃÊ°ú, 1=Á¦¾à½Ã°£¾È¿¡ ÀÖ´Ù. ±×¿Ü= ÇöÀç Å¸ÀÌ¸ÓÀÇ ½Ã°£
	sNationBelong,        // 98   yes 1 : ¹ÙÀÌ¼­½º 2:ÀÚÀÌÆİ 3 : error
	sIsGetMagic,				//99		// i (¸¶¹ı¹øÈ£) ¸¶¹ıÀ»  ¹è¿üÀ¸¸é 1, ¾Æ´Ï¸é 0 // 030415 kyo 
	sDualClassCheck,			//100		//ii ¼±ÇàÅ¬·¡½º ÈÄÇà Å¬·¡½º ¼±ÇàÅ¬·¡½º°¡ 5ÀÌ»óÀÌ¸é ¼±ÇàÅ¬·¡½º´Â ¹«½ÃÇÑ´Ù.
								//0.Àü»ç 1,µµµÏ 2. ±Ã ¼ö 3. ¸¶¹ı»ç 4.  ¼ºÁ÷ÀÚ
	sUpDualClassItem,				//101		//i µÎ¾ó¾ÆÀÌÅÛÀÌ ¾ø´Â »ç¸²Àº ÁÖ°í ÀÖ´Â »ç¶÷Àº ´Ü°è¸¦ ¿Ã¸°´Ù. syes°ªÀº ÇöÀç À¯ÀúÀÇ »óÅÂ¸¦ ¸»ÇÔ
	sShowLearnDualMagic,		//102		//null Ä³¸¯ÅÍ°¡ ¹è¿ï¼ö ÀÕ´Â ¸¶¹ıÃ¥ÀÌ º¸¿©Áø´Ù.
	sLogDualQuest,				//103		//iisi	qcount, ³»¿ë, ½ÃÀÛ(0:½ÃÀÛ, 1:³¡) 
	sLogForScript,				//104		//is	Äù½ºÆ®¹øÈ£	msg	 //ÇöÀçÀÇ »óÅÂ¸¦ ·Î±ëÇÑ´Ù. 
	sEventItemInsert,			//105		// BBD 040226	ÀÌº¥Æ®¾ÆÀÌÅÛ Áö±ŞÀ» ÇÏ´Â ÇÔ¼ö
	sGetCharacterStatus,		//106		// kjy 040406   Ä³¸¯ÅÍÀÇ ´É·ÂÄ¡¸¦ °¡Á®¿Í¼­ °ªÀ»ºñ±³  i = 0 : str, 1 : con, 2 : dex..., 10 : wsps, tt = ºñ±³ÇÒ °ª (°¡Á®¿Â ´É·ÂÄ¡°¡ ttº¸´Ù ÀÛÀ¸¸é syes°ªÀÌ 0, Å©¸é 1, °°À¸¸é 2)
	sSearchItemMuchForRare,		//107		// kjy 040406   Æ¯Á¤ Rare ¾ÆÀÌÅÛÀÇ °³¼ö¸¦ ¾ò¾î¿Í¼­ °ªÀ» ºñ±³
	sCheckItemQuantity,			//108	by Rogerio Segovia
	sDeleteItemQuantity,		//109	by Rogerio Segovia	
	sLearnItem,					//110       // Teaches player a new item that they can make using job skills
	
	sAddExp,					//111	//Eleval 15/06/09 - //Gives Exp to the player
	sAddQuestItemToNpc,			//112	//Eleval 17/06/09 - //Set an NPC to drop an item only to a player doing to quest
	sCheckTamedNear,			//113	//Eleval 21/06/09 - //Check if there's a specific tamed monster near the NPC
	sGiveMoney,					//114	//Eleval 20/07/09 - //Gives money to the player and displays a message about it.
	sScenarioQuestTeleport		//115	//Eleval 10/08/09 - //Check and teleport a character back in quest
	//MAX_SCRIPT_FUNCTION ¿¡¼­ ÇöÀç ¸Æ½º°ª È®ÀÎÇÒ²¯. 
}; // »ç¿ëÀÚ Á¤ÀÇ ÇÔ¼öµéÀÇ Æ÷ÀÎÅÍ
