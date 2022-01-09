// ExchangeMgr.cpp: implementation of the CExchangeMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "CItem.h"
#include "Auction.h"
#include "DefaultHeader.h"
#include "DR_NETWORK.h"
#include "GMMGR.h"
#include "ExchangeMgr.h"

extern bool g_IsSQuestATM;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExchangeMgr::CExchangeMgr()
{

}

CExchangeMgr::~CExchangeMgr()
{

}


// 011120 YGI
int RecvItemExchange( K_ITEM *k_item, int you_id, int cn , const int iExchangeType)//거래를 하고 싶다는 요구
{
	//////////////////// exist check //////////////
	CHARLIST *ch	= ::CheckServerId( cn );
	CHARLIST *you	= ::CheckServerId( you_id );
	if( !ch || !you ) {return 0;}

	if( ch->IsDead() || you->IsDead() )//죽었다면.
	{
		return 0;
	}

	if (!CanSaveUserData(ch, 6) || !CanSaveUserData(you, 6))
	{	//< CSD-040212
		return 0;
	}	//> CSD-040212

	if(!ch->IsTradeOn() || !you->IsTradeOn())
	{
		if(!ch->IsTradeOn()){::OutMessage(ch,2,920);}//교환 거부 상태 확인후 아이템을 거래 하십시요.
		else				{::OutMessage(ch,2,921);}//상대가 교환을 거부

		if(!you->IsTradeOn())	{::OutMessage(you,2,920);}//교환 거부 상태 확인후 아이템을 거래 하십시요.
		else					{::OutMessage(you,2,921);}//상대가 교환을 거부
		
		return 0;
	}

	if( EXCHANGE_STATE_READY != you->GetExchangeState() )		// 그사람이 누군가와 거래를 하고 있으면
	{
		SendExchangeItemResult( cn, 0 ); 
		return 0;
	}
	if( EXCHANGE_STATE_READY != ch->GetExchangeState() )	// 내가 누군가와 거래하고 있으면서 
	{
		SendExchangeItemResult( cn, 1 ); // 다른 사람하고 거래하고 있다는 것~
		return 0;
	}
	if( ch->fight_flag > 0 )
	{
		SendExchangeItemResult( cn, 5 ); // 1:1 결투 중이다.
		return 0;
	}
	if( you->fight_flag > 0 )
	{
		SendExchangeItemResult( cn, 5 ); // 1:1 결투 중이다.
		return 0;
	}
	if( !you->IsPlayer())
	{
		SendExchangeItemResult( cn, 3 );		// 인간이 아냐~
		return 0;
	}
	if(!connections[you_id].dwAgentConnectionIndex || connections[you_id].state < CONNECT_JOIN) // 그 사람은 현재 접속중이 아니다.	
	{
		SendExchangeItemResult( cn, 4 );
		return 0;
	}

	if( EXCHANGE_STATE_READY != ch->GetExchangeState() 
	&&	you_id == ch->GetExchangeTargetId())	// 그 사람과 거래중일때 처리 메뉴에 올려 놓는 거와 같은 처리 필요
	{		
		return 0;
	}

	//////////// 교환 시작 성공 루틴 ////////////////
	ch->ClearExchangeInfo();//순서 주의 클리어 시키고 값 셋팅이니까 내리지 말것
	you->ClearExchangeInfo();

	switch(iExchangeType)
	{
	case EXCHANGE_TYPE_NORMAL:
		{	//교환을 처음 시작할 때는 정보를 모두 클리어 시킴
			::memset(k_item,0,sizeof(K_ITEM));
			::memset(ch->give,0,sizeof(K_ITEM)*MAX_EXCHANGE_ITEM);
			::memset(ch->recv,0,sizeof(K_ITEM)*MAX_EXCHANGE_ITEM);
			::memset(you->give,0,sizeof(K_ITEM)*MAX_EXCHANGE_ITEM);
			::memset(you->recv,0,sizeof(K_ITEM)*MAX_EXCHANGE_ITEM);
		}break;
	case EXCHANGE_TYPE_MERCHANT_DIRECT:
		{//검사가 없다. 서버내에서 호출 하기 때문이다
			ch->give[0] = *k_item;
			you->recv[0] = *k_item;
		}break;//021121 lsw
	default:
		{
			return 0;
		}break;
	}

	ch->SetExchangeTarget(you_id,you->Name);
	you->SetExchangeTarget(cn,ch->Name);

	ch->SetExchangeType(iExchangeType);//거래타입 셋팅
	you->SetExchangeType(iExchangeType);

	ch->SetExchangeState(EXCHANGE_STATE_EXCHANGE_NOW_SIGN_NO);//보통 거래로 셋팅
	you->SetExchangeState(EXCHANGE_STATE_EXCHANGE_NOW_SIGN_NO);
	
	::SendExchangeItemOk( you_id, cn, k_item ,iExchangeType);// 성공 // 메뉴를 띄운다.
	::SendExchangeItemReq( you_id, cn, k_item ,iExchangeType);
	return 1;
}

void SendExchangeItemOk( int you_id, int cn, const K_ITEM *item , const int iExchangeType)	
{	// 아이템 교환 메뉴를 띄워라...
	t_packet packet;
	
	packet.h.header.type = CMD_EXCHANGE_ITEM_READY;
	packet.u.kein.exchange_item_start.item = *item;
	packet.u.kein.exchange_item_start.you_id = you_id;
	packet.u.kein.exchange_item_start.iExchangeType = iExchangeType;
	packet.h.header.size = sizeof( k_exchange_item_start );

	::QueuePacket(connections, cn, &packet, 1);
}

void SendExchangeItemReq( int you_id, int cn, const K_ITEM *item , const int iExchangeType)	
{	// cn이 너에게 아이템을 교환하자고 요청했다.
	t_packet packet;

	packet.h.header.type = CMD_EXCHANGE_ITEM_REQ;
	packet.u.kein.exchange_item_start.item = *item;
	packet.u.kein.exchange_item_start.you_id = cn;
	packet.u.kein.exchange_item_start.iExchangeType = iExchangeType;
	packet.h.header.size = sizeof( k_exchange_item_start );

	::QueuePacket(connections, you_id, &packet, 1);
}

void SendExchangeItemResult( int cn, int result )
{
	t_packet packet;
	packet.h.header.type = CMD_EXCHANGE_ITEM_RESULT;
	packet.u.kein.try_item_trade.result = (char) result;
	packet.h.header.size = sizeof(k_server_money);

	::QueuePacket(connections, cn, &packet, 1);
}

void RecvExchangeStateSign( const short int you, const short int me , const bool bIsOkSign)
{//교환 OK 사인이 왔어
	CHARLIST *me_ch = ::CheckServerId(me);
	CHARLIST *you_ch = ::CheckServerId(you);

	if( !me_ch || !you_ch )			 //  그런 놈 없당~
	{
		::SendExchangeCancel( you, me );
		return;
	}

	if( EXCHANGE_STATE_READY != me_ch->GetExchangeState() 
	&&	EXCHANGE_STATE_READY != you_ch->GetExchangeState() 
	&& 	me_ch->GetExchangeTargetId() == you 
	&& 	you_ch->GetExchangeTargetId() == me )
	{
		int iSendPacketType = 0;
		if(bIsOkSign)
		{
			iSendPacketType = CMD_EXCHANGE_OK_SIGN;
			me_ch->SetExchangeState(EXCHANGE_STATE_EXCHANGE_NOW_SIGN_YES);//Change SignState
			if(EXCHANGE_STATE_EXCHANGE_NOW_SIGN_YES == you_ch->GetExchangeState())
			{// 2 Guys All Yes Sign
				RecvExchange( you, me );//ItemExchange

				//Eleval 11/08/09 - to remove a char from quest if he exchanges anything
				if(g_IsSQuestATM && (MapNumber < 73 || MapNumber > 78) && (me_ch->s_quest_floor != 99 && you_ch->s_quest_floor != -99)) //Check if the char is in quest map first
				{
					me_ch->s_quest_floor = -99;
					you_ch->s_quest_floor = -99;
					t_packet packet;
					packet.h.header.type = CMD_ASK_SQUEST_TELEPORT;
					packet.h.header.size = sizeof(t_ask_squest_tp);
					packet.u.ask_squest_tp.questfloor = -99;
					packet.u.ask_squest_tp.cn = -5;
					sprintf(packet.u.ask_squest_tp.charname, "%s", me_ch->Name);
					packet.u.ask_squest_tp.map_port = GetOwnPort();

					t_packet packet2;
					packet2.h.header.type = CMD_ASK_SQUEST_TELEPORT;
					packet2.h.header.size = sizeof(t_ask_squest_tp);
					packet2.u.ask_squest_tp.questfloor = -99;
					packet2.u.ask_squest_tp.cn = -5;
					sprintf(packet2.u.ask_squest_tp.charname, "%s", you_ch->Name);
					packet.u.ask_squest_tp.map_port = GetOwnPort();

					//QueuePacket(connections, DB_DEMON, &packet, 1);
					if (CheckMapPort(MN_SCENARIO_MAP))
					{
						RecvAskSQuestTeleport(&packet);
						RecvAskSQuestTeleport(&packet2);
					}
					else
					{
						SendPacket2Map(&packet, MN_SCENARIO_MAP);
						SendPacket2Map(&packet2, MN_SCENARIO_MAP);
					}
				}
				//End of it
			}
		}
		else
		{
			iSendPacketType = CMD_EXCHANGE_CANCEL_SIGN;
			me_ch->SetExchangeState(EXCHANGE_STATE_EXCHANGE_NOW_SIGN_NO);//Change SignState
		}
	
		t_packet packet;// 둘이 거래를 하고 있는 중이야 한다.
		packet.h.header.type = iSendPacketType;
		packet.h.header.size = sizeof( 0 );
		::QueuePacket(connections, you, &packet, 1);
	}
	else 
	{
		::SendExchangeCancel( you, me );
	}
	return;
}

void ExchangeCancel(CHARLIST *ch)//SendExchangeCancel() 와 셋트 입니다. 떨어지지 않게 주의 하세요
{
	if(!ch){return;}
	switch(ch->GetExchangeType())
	{
	case EXCHANGE_TYPE_MERCHANT_DIRECT:	//상인과 직접 거래
		{//취소 될때 IS_END_BUYING 를 풀어 주자
			if(	AUCTION_DB == ch->give[0].item_pos.type)//판매 물품중
			{
				// 030929 HK YGI
				int iIndex = MAKELONG( ch->give[0].item_pos.p1, ch->give[0].item_pos.p2 );
				Auction.SendCMD_MERCHANT_ITEM_BUY_COMFORM_RESULT(
					ch,iIndex,ch->give[0].item_attr,false);
			}
			if(	AUCTION_DB == ch->recv[0].item_pos.type)
			{
				// 030929 HK YGI
				int iIndex = MAKELONG( ch->recv[0].item_pos.p1, ch->recv[0].item_pos.p2 );
				Auction.SendCMD_MERCHANT_ITEM_BUY_COMFORM_RESULT(
					ch,iIndex,ch->recv[0].item_attr,false);
			}

		}break;
	default:
		{
		}break;
	}

	ch->ClearExchangeInfo();

	t_packet packet;
	packet.h.header.type = CMD_EXCHANGE_CANCEL;
	packet.h.header.size = sizeof( 0 );
	::QueuePacket(connections, ch->GetServerID(), &packet, 1);
}

void SendExchangeCancel( short int you, short int me)
{//iExchangeType이 상인과 거래일때 iIndex값을 넣고
	CHARLIST *pChMe		= ::CheckServerId(me);
	CHARLIST *pChYou	= ::CheckServerId(you);

	if( pChMe )		// 아이디가 0 이면 안된다.
	{
		::ExchangeCancel(pChMe); 
	}
	if( pChYou )
	{	
		::ExchangeCancel(pChYou);
	}
}

// 010223 YGI
void RecvExchangeEach( K_ITEM *k_item, int you_id, int cn )		// 올려 놓기 시작한 물건들
{
	CHARLIST *me_ch	= ::CheckServerId( cn );
	CHARLIST *you_ch = ::CheckServerId( you_id );

	if( !me_ch || !you_ch )			 //  그런 놈 없당~
	{
		::SendExchangeCancel( you_id, cn );
		return;
	}

	CItem *ii = ItemUnit( k_item->item_attr.item_no );
	if( NULL == ii ) return;

	// finito 040707 check for noexchange item kind
	if (IsNoExchange(&k_item->item_attr))
	{
		SendPutMenuString( KM_FAIL, 245, cn );
		return;
	}
	// end of check
	
	switch(k_item->item_pos.type)
	{
	case EQUIP:
		{
			if( k_item->item_pos.type == EQUIP )
			{
				int a, b, c;
				if( ::SearchInv( me_ch->inv, a, b, c ) )
				{
					::SetItemPos( INV, a, b, c, &k_item->item_pos );
				}
				else return;
			}
		}break;
	case GROUND:
	case BOX:
	case OTHER_CH:
	case MAKE:
	case BANK:
		{
			return;
		}break;
	default:
		{
		}break;
	}

	const ItemAttr *target= ::GetItemByPOS( cn, k_item->item_pos );		// 정말로 그 아이템을 가지고 있느냐?

	if( !target
	||	!target->item_no 
	|| ::memcmp( target,&k_item->item_attr,sizeof(ITEMATTR) ) )
	{
		return;
	}

	k_item->item_attr = *target;

	int i;
	for( i=0; i<MAX_EXCHANGE_ITEM ; i++ )
	{
		if( ::EqualPosByPos(&me_ch->give[i].item_pos, &k_item->item_pos) ) 
		{
			return;// 같은 곳의 아이템이 올라 왔다.
		}
	}

	if(AUCTION_DB == me_ch->give[0].item_pos.type)
	{
		return;
	}//0번에 경매품이 있으면 아이템 못올린다
	
	for( i=0; i<MAX_EXCHANGE_ITEM; i++ )
	{
		if( !me_ch->give[i].item_attr.item_no )
		{
			me_ch->give[i] = *k_item;
			you_ch->recv[i] = *k_item;
			break;
		}
	}
	
	t_packet packet;
	packet.h.header.type = CMD_EXCHANGE_ITEM_EACH;
	packet.u.kein.transfer_item.item = *k_item;//030108 lsw
	packet.h.header.size = sizeof( k_transfer_item );
	::QueuePacket(connections, you_id, &packet, 1);
}

void RecvExchange_Sub(CHARLIST *pCaster,CHARLIST *pTarget,const int i)
{
	if(0 > i || MAX_EXCHANGE_ITEM < i){return;}
	const int iCasterCn = pCaster->GetServerID();
	const int iTargetCn = pTarget->GetServerID();
	if (!pCaster || !pTarget) return;

	if( pCaster->give[i].item_attr.item_no )
	{
		ItemAttr *del_item = ::GetItemByPOS( iCasterCn, pCaster->give[i].item_pos );
		
		if(	AUCTION_DB == pCaster->give[0].item_pos.type)//판매 물품중
		{//교환 성공 옥션에 있는거 구매 가능으로 올려라
			// 030929 HK YGI
			int iIndex = MAKELONG(pCaster->give[0].item_pos.p1, pCaster->give[0].item_pos.p2 );
			Auction.SendCMD_MERCHANT_ITEM_BUY_COMFORM_RESULT(
				pCaster, iIndex,pCaster->give[0].item_attr,true);
		}

		if( !del_item ) return;

		::SendItemEventLog( del_item, iCasterCn, iTargetCn, SILT_PUT_OTHER_CH, 1 ); //YGI acer
		::SendItemEventLog( del_item, iTargetCn, iCasterCn, SILT_GET_OTHER_CH, 1 ); //YGI acer

		int a, b, c;
		int ret = SearchInv( pTarget->inv, a, b, c );
		if( !ret ) 
		{
			// Finito - Remove droping of items if no inventory room
			// Instead show a message saying not enough room to both players
			//::SendItemEventLog( del_item , iTargetCn, iCasterCn, SILT_PUT_GROUND_MAX_INVEN, 1 ); //YGI acer
			//::DropItem( pTarget->X, pTarget->Y, del_item );
			OutMessage(iTargetCn, 2, 14);
			OutMessage(iCasterCn, 2, 15);
			return;
		}
		else 
		{
			ItemAttr *recv_item = &pTarget->inv[a][b][c];
			*recv_item = *del_item;
			POS pos;
			::SetItemPos( INV, a, b, c, &pos );
			::SendServerEachItem( &pos, recv_item , iTargetCn );
		}
		::memset( del_item, 0, sizeof( ItemAttr ) );
		::SendServerEachItem( &pCaster->give[i].item_pos, del_item, iCasterCn );
	}
}

void RecvExchange( short int you, short int me )
{
	CHARLIST *me_ch = ::CheckServerId( me );
	CHARLIST *you_ch = ::CheckServerId( you );

	if( !me_ch || !you_ch )			 //  그런 놈 없당~
	{
		SendExchangeCancel( you, me );
		return;
	}
	const int iExchangeType = me_ch->GetExchangeType();

	if(iExchangeType != you_ch->GetExchangeType())
	{//교환 타입이 다르면
		return;
	}
	
	switch(iExchangeType)
	{
	case EXCHANGE_TYPE_NORMAL:
		{
			if( EXCHANGE_STATE_READY != me_ch->GetExchangeState() 
			&&	EXCHANGE_STATE_READY != you_ch->GetExchangeState() 
			&& 	me_ch->GetExchangeTargetId() == you 
			&&	you_ch->GetExchangeTargetId() == me )
			{
				for( int i=0; i<20; i++ )
				{
					RecvExchange_Sub(me_ch,you_ch,i);
					RecvExchange_Sub(you_ch,me_ch,i);
				}
				me_ch->ClearExchangeInfo();
				you_ch->ClearExchangeInfo();
			}
		}break;
	case EXCHANGE_TYPE_MERCHANT_DIRECT:
		{
			if( EXCHANGE_STATE_READY != me_ch->GetExchangeState() 
			&&	EXCHANGE_STATE_READY != you_ch->GetExchangeState() 
			&& 	me_ch->GetExchangeTargetId() == you 
			&&	you_ch->GetExchangeTargetId() == me )
			{
				for( int i=0; i<20; i++ )
				{
					RecvExchange_Sub(me_ch,you_ch,i);
					RecvExchange_Sub(you_ch,me_ch,i);
				}
				me_ch->ClearExchangeInfo();
				you_ch->ClearExchangeInfo();
			}
		}break;
	}
	t_packet packet;
	packet.h.header.type = CMD_EXCHANGE_ITEM;
	packet.h.header.size = sizeof( 0 );
	::QueuePacket(connections, me, &packet, 1);
	::QueuePacket(connections, you, &packet, 1);

	me_ch->Money = ::GetMoneyByItem( me_ch );
	you_ch->Money = ::GetMoneyByItem( you_ch );
}

void RecvExchangeItemDel( POS pos, short int you_id, short int me )
{
	CHARLIST *me_ch	= ::CheckServerId(me);
	CHARLIST *you_ch = ::CheckServerId(you_id);

	if( !me_ch || !you_ch )			 //  그런 놈 없당~
	{
		::SendExchangeCancel( you_id, me );
		return;
	}

	RecvExchangeStateSign( you_id, me , false);
	RecvExchangeStateSign( me, you_id , false);
	
	if(AUCTION_DB == me_ch->give[0].item_pos.type)//0번에 경매품이 있으면 나머지도 취소 할 수 없다(들어 낼 수 없다)
	{
		return;
	}

	int i;
	for( i=0; i<20; i++ )
	{
		if( IsEqualPosByPos( me_ch->give[i].item_pos, pos ) )
		{
			::memset( &me_ch->give[i], 0, sizeof( K_ITEM ) );
		}
	}

	
	for( i=0; i<20; i++ )
	{
		if( IsEqualPosByPos( you_ch->recv[i].item_pos, pos ) )
		{
			K_ITEM targetItem = you_ch->recv[i];
			::SendExchangeItemDel( you_id, targetItem );
			::memset( &you_ch->recv[i], 0, sizeof( K_ITEM ) );
		}
	}
	
	
}

void SendExchangeItemDel( const short int you, const K_ITEM item )		// 상대편에게 넘어갈 아이템중 삭제할 아이템..
{
	t_packet packet;
	packet.h.header.type = CMD_EXCHANGE_ITEM_DEL;
	{
		packet.u.kein.transfer_item.item = item;
	}
	packet.h.header.size = sizeof( k_transfer_item );
	QueuePacket(connections, you, &packet, 1);	
}


void RecvReqManToMan( short int target_id, short int cn )
{
	CHARLIST *target= ::CheckServerId(target_id);
	CHARLIST *ch	= ::CheckServerId( cn );
	if(!target || !ch){return;}

	if (target->m_bIsDuelAllow == false) // finito 25/08/07 check if target is allowing duels
	{
		::SendReqManToManResult(7, cn);
		return;
	}

	if (target->GetLevel() > ch->GetLevel() + ENABLE_DUEL_LEVEL)
	{	//< CSD-030806
		::SendReqManToManResult(3, cn);
		return;
	}	//> CSD-030806

	if( EXCHANGE_STATE_READY != target->GetExchangeState() 
	||	EXCHANGE_STATE_READY != ch->GetExchangeState() ) // 물건 거래중이다
	{
		::SendReqManToManResult(4, cn);
		return;
	}

	if( target->fight_flag || ch->fight_flag )
	{
		::SendReqManToManResult(4, cn);
		return;
	}
	::SendReqManToManResult(1, cn) ;		// 신청 성공	//아이템을 걸지 않습니다.//021030 lsw

	// 결투 시스템 플레그 셋팅
	target->fight_flag	= 2;		// 결투 메뉴가 떠있는 상태 플레그 set
	ch->fight_flag		= 2;		// 결투 메뉴가 떠있는 상태 플레그 set

	t_packet packet;				// 1:1 신청
	packet.h.header.type = CMD_REQ_MAN_TO_MAN;
	packet.u.kein.req_man_to_man.id = cn;
	packet.h.header.size = sizeof( k_req_man_to_man );
	::QueuePacket(connections, target_id, &packet, 1);
}

////////////////////////////////////////////////////////
void SendReqManToManResult( char ret, short int cn )
{
	t_packet packet;				// 1:1 신청
	packet.h.header.type = CMD_REQ_MAN_TO_MAN_RESULT;
	packet.u.kein.default_char= ret;
	packet.h.header.size = sizeof( char );
	QueuePacket(connections, cn, &packet, 1);
}

void RecvManToManConsent( short int target_id, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	CHARLIST *target = CheckServerId( target_id );
	if( !ch || !target ) return;

	t_packet packet;				// 1:1 시작
	packet.h.header.type = CMD_REQ_MAN_TO_MAN_CONSENT;
	packet.h.header.size = sizeof( short int );
	packet.u.kein.default_short_int = target_id;
	QueuePacket(connections, cn, &packet, 1);

	packet.u.kein.default_short_int = cn;
	QueuePacket(connections, target_id, &packet, 1);
}

void ResultManToMan( short int cn, short int target_id, int flag )		// 1:1 에서 결과가 나왔을 경우 아이템 셋팅~
{
	CHARLIST *me = CheckServerId( cn );
	if( !me || me->fight_flag == 0 )
	{
		return;
	}
	
	CHARLIST *you = CheckServerId( target_id );
	if( !you || you->fight_flag == 0 )
	{
		return;
	}

	switch( flag )//저쪽에서 인터페이스를 닫을때 //030116 lsw
	{
	case 3:					// 비긴경우		// 각자 아이템을 각자가 가져간다.
		{
			::CallClient( target_id, CMD_REQ_MAN_TO_MAN_REFUSAL );
		}break;
	}

	me->fight_id = 0;
	me->fight_flag = 0;
	you->fight_id = 0;
	you->fight_flag = 0;
}

void RecvCMD_TRADE_MODE_CHANGE(const int iCn, t_packet *p)//030127 lsw //Trade On/Off 셋팅
{
	CHARLIST *pCh = ::CheckServerId(iCn);
	if(pCh == NULL) return; // Finito added NULL pointer check

	const bool bIsTradeOn = p->u.Hwoa.rang.TradeModeChange.bIsTradeOn;
	pCh->SetTradeMode(bIsTradeOn);

	::QueuePacket(connections, iCn, p, 1);
}
