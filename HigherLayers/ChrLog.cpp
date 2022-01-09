#include "..\stdafx.h"
#include <mmsystem.h>
#include "DefaultHeader.h"
#include "Scrp_int.h"
#include "Scrp_Exe.h"
#include "NPC_Pattern.h"
#include "map.h"

#include "OP_Magic.h"
#include "CItem.h"
#include "Map.h"
#include "Chatting.h"

#include "RajaSystem.h"
#include "SealStoneGameServer.h"
#include "SealStone.h"
#include "..\LowerLayers\mylog.h"
#include "..\LowerLayers\packed.h"
#include "..\LowerLayers\servertable.h"
#include "dr_network.h"


#define __CHR_LOG_H__
#include "ChrLog.h"


// state :	CHR_LOG_LOGIN_		1
//			CHR_LOG_LOGOUT_		2
//			CHR_LOG_LEVELUP_	3


void LogCharacterStatusBasic( t_connection c[], int id, int state )
{
	t_packet p;
	t_server_chr_log_basic_data *tp;
	LPCHARLIST ch = &c[ id].chrlst;

	p.h.header.type = CMD_CHR_LOG_CHAR_DB;
	tp = &p.u.server_chr_log_basic_data;

	if( ch->HpMax ==     0 )	return;
	if( ch->HungryMax == 0 )	return;
	if( ch->ManaMax ==   0 )	return;

	strcpy_s( tp->id, sizeof(tp->id),		c[ id].id );
	strcpy_s( tp->name, sizeof(tp->name),	c[ id].name );

	tp->state			= state;

	tp->btLevel = ch->GetLevel(); // CSD-030806
	tp->fame_pk			= ch->fame_pk;

	tp->Str  				= ch->Str;
	tp->Con  				= ch->Con;
	tp->Dex  				= ch->Dex;
	tp->Wis  				= ch->Wis;
	tp->Int  				= ch->Int;
	tp->MoveP				= ch->MoveP;
	tp->Char 				= ch->Char;
	tp->Endu 				= ch->Endu;
	tp->Moral				= ch->Moral;
	tp->Luck 				= ch->Luck;
	tp->wsps 				= ch->wsps;

	tp->Tactics 			= ch->Tactics;				// 선택한 전투기술 (보여주기 위함)
	memcpy( &tp->nation, &ch->name_status, sizeof( DWORD ) );		// 1004 YGI

	tp->Money			= ch->Money;
	tp->HpMax			= ch->HpMax ;
	tp->ManaMax			= ch->ManaMax;

	memcpy( tp->MapName, MapName, NM_LENGTH );

	tp->HungryMax = ch->HungryMax;
	//030227 lsw
	tp->nPoison = ch->GetBasicResist(RT_POISON); // 저주계열 마법에 대한 저항력
	tp->nCurse	= ch->GetBasicResist(RT_CURSE);	 //	저주계열 마법에 대한 저항력
	tp->nHoly	= ch->GetBasicResist(RT_HOLY);    // 신력계열 공격 마법에 대한 저항력
	tp->nFire	= ch->GetBasicResist(RT_FIRE);     //	불계열 공격 마법에 대한 저항력
	tp->nIce	= ch->GetBasicResist(RT_ICE);       //	얼음계열 공격 마법에 대한 저항력
	tp->nElect	= ch->GetBasicResist(RT_ELECT);   //	전격계열 공격 마법에 대한 저항력
	
	tp->X = ch->X;
	tp->Y = ch->Y;
	tp->reserved_point  = ch->GetReservedPoint();
	tp->BankMoney		= ch->GetBankMoney();
	tp->Exp				= ch->Exp;

	p.h.header.size = sizeof( t_server_chr_log_basic_data);

	QueuePacket( connections, DB_DEMON, &p, 1 );
}

void LogCharacterDataBinary( t_connection c[], int id )
{														
	t_packet p;										
	LPCHARLIST ch = &c[id].chrlst;			
										
	if( ch->HpMax ==     0 )	return;
	if( ch->HungryMax == 0 )	return;
	if( ch->ManaMax ==   0 )	return;

	p.h.header.type = CMD_CHR_LOG_BINARY_DATA0;														
		memcpy(p.u.server_chr_log_binary_data0.name, c[id].name, NM_LENGTH);
		memcpy(p.u.server_chr_log_binary_data0.Skill,			(UCHAR *)ch->Skill,			SIZE_OF_SKILL );
		memcpy(p.u.server_chr_log_binary_data0.skillexp,		(UCHAR *)ch->skillexp,		SIZE_OF_SKILL_EXP );
		memcpy(p.u.server_chr_log_binary_data0.tac_skillEXP,	(UCHAR *)ch->tac_skillEXP,	SIZE_OF_TAC_SKILL_EXP );
	p.h.header.size = sizeof( t_server_chr_log_binary_data0 );
	QueuePacket( c, DB_DEMON, &p, 1 );

	p.h.header.type = CMD_CHR_LOG_BINARY_DATA1;	
		memcpy(p.u.server_chr_log_binary_data1.name, c[id].name, NM_LENGTH);
		memcpy(p.u.server_chr_log_binary_data1.equip,			(UCHAR *)ch->equip,			SIZE_OF_EQUIP);
		memcpy(p.u.server_chr_log_binary_data1.quick,			(UCHAR *)ch->quick,			SIZE_OF_QUICK);
	p.h.header.size = sizeof( t_server_chr_log_binary_data1 );
	QueuePacket( c, DB_DEMON, &p, 1 );
	
	p.h.header.type = CMD_CHR_LOG_BINARY_DATA2;	
		memcpy(p.u.server_chr_log_binary_data2.name, c[id].name, NM_LENGTH);
		memcpy(p.u.server_chr_log_binary_data2.inv,			(UCHAR *)ch->inv,				SIZE_OF_INV );
	p.h.header.size = sizeof( t_server_chr_log_binary_data2 );	
	QueuePacket( c, DB_DEMON, &p, 1 );

	p.h.header.type = CMD_CHR_LOG_BINARY_DATA4;	
		memcpy(p.u.server_chr_log_binary_data3.name,	c[id].name, NM_LENGTH);
		memcpy(p.u.server_chr_log_binary_data3.bankitem,		(UCHAR *)ch->bank,		SIZE_OF_BANKITEM );
	p.h.header.size = sizeof( t_server_chr_log_binary_data3 );		
	QueuePacket( c, DB_DEMON, &p, 1 );
}





int CheckingChrLogAvailable( void )
{
	ChrLogStart_Login	= GetPrivateProfileInt( "chr_log_server", "chr_log_start_login",	0, MAP_SERVER_INI_ );
	ChrLogStart_Logout	= GetPrivateProfileInt( "chr_log_server", "chr_log_start_logout",	0, MAP_SERVER_INI_ );
	ChrLogStart_Levelup = GetPrivateProfileInt( "chr_log_server", "chr_log_start_levelup",	0, MAP_SERVER_INI_ );

	if( ChrLogStart_Login )	MyLog( LOG_NORMAL, "Start to USE 'Log Server' When Log In" );
	if( ChrLogStart_Logout)	MyLog( LOG_NORMAL, "Start to USE 'Log Server' When Log Out" );
	if( ChrLogStart_Levelup)MyLog( LOG_NORMAL, "Start to USE 'Log Server' When Level Up" );

	return 1;
}


void SendLog2ChrLogDB( t_connection c[], int cn, int type )
{
	return;//

	switch( type )
	{
		case CHR_LOG_LOGIN_		:	if( !ChrLogStart_Login )	return;
		case CHR_LOG_LOGOUT_	:	if( !ChrLogStart_Logout)	return;
		case CHR_LOG_LEVELUP_	:	if( !ChrLogStart_Levelup)	return;
	}

	LogCharacterStatusBasic( c, cn, type );
	LogCharacterDataBinary( c, cn );
}

