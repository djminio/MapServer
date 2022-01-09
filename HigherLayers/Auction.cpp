// Auction.cpp: implementation of the CAuction class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "CItem.h"
#include "Auction.h"
#include "DefaultHeader.h"
#include "DR_NETWORK.h"
#include "GMMGR.h"
#include "LogManager.h"	// BBD 040226
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CAuction Auction;

const SUPERMONEY SM1(BOND_MONEY_1000	,1000000000);
const SUPERMONEY SM2(BOND_MONEY_500		,500000000);
const SUPERMONEY SM3(BOND_MONEY_100		,100000000);
const SUPERMONEY SM4(BOND_MONEY_50		,50000000);
const SUPERMONEY SM5(BOND_MONEY_10		,10000000);
const SUPERMONEY SM6(BOND_MONEY_5		,5000000);

const SUPERMONEY aBondMoney[BOND_MONEY_MAX] = {SM1,SM2,SM3,SM4,SM5,SM6};

CAuction::CAuction()
{
	Clear();
}

CAuction::~CAuction()
{
	Clear();
}

void CAuction::Clear()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 구매가능 품목 리스트 받기 시작
////////////////////////////////////////////////////////////////////////////////////////////////////////////

//soto-030514
void CAuction::RecvCMD_MERCHANT_BUY_LIST_REQUEST(const int cn,t_packet &p)//리스트 요청(구매자가 보는것//검색기가 호출
{
	
	CHARLIST *ch = CheckServerId(cn);
	if(!ch) {return;}
	//클라이언트용 패킷을 받아서 서버용 패킷으로 만든다
	t_packet sp;
	sp.h.header.type = CMD_MERCHANT_BUY_ITEM_SEARCH_SERVER;
	sp.h.header.size = sizeof(SEARCHPACKETSERVER);

	memcpy(sp.u.SearchPacketServer.szName,ch->Name,20);
	sp.u.SearchPacketServer.szName[20] = 0;	
	sp.u.SearchPacketServer.iCn	= ch->GetServerID();

	memcpy(&sp.u.SearchPacketServer.ClientMsg, &p.u.SearchPacketClient,sizeof(t_SearchPacketClient));
	
	
	::QueuePacket(connections,DB_DEMON,&sp,1);
}

void CAuction::RecvCMD_MERCHANT_BUY_LIST_REQUEST_RESULT(t_packet &p)//리스트 결과(구매자가 보는것(DB_DEMON이 보내오는것을 클라이언트로 전송
{
	const int cn = p.u.SearchResultList.iCn;
	CHARLIST * ch = CheckServerId(cn);
	if(!ch){return;}

	if(!strcmp(ch->Name,p.u.SearchResultList.szName))//이름 검색후 클라이언트로 스위칭
	{
		::QueuePacket(connections,cn,&p,1);
	}	
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 구매가능 품목 리스트 받기 끝
////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 구매 하기 시작(구매자)
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAuction::RecvCMD_MERCHANT_ITEM_BUY(const int cn,t_packet &p)//구매하기(구매자가 선택
{//이거 살 수 있냐?
	//디비에 구매 가능한지 물어보고 IS_END_BUYING 으로 셋팅 한다
	//돈을 체크한다 //인벤토리는 체크 안한다(결과물 찾기에서 한다)
	if(p.u.MerchantItemBuy.iCn != cn){return;}
	CHARLIST *ch = CheckServerId(cn);
	if(!ch){return;}
	
	if(strcmp(p.u.MerchantItemBuy.szBuyerName, ch->Name)){return;}//이름이 틀려	

	const int iSellValue = (int)p.u.MerchantItemBuy.dwSellValue;

	if(iSellValue >= 0 && ch->GetBankMoney() < iSellValue)//아이템 가격 보다 돈이 작으므로 무효
	{//은행돈 외에 인벤토리 돈을 섞으면 뺄때 어렵다.
		::OutMessage(ch,2,12);//임시,, 구입금이 모자랍니다.
		return;
	}
	else
	{//돈은 있다 가보자~
		p.h.header.type = CMD_MERCHANT_ITEM_BUY_COMFORM;//타입 전환
		::QueuePacket(connections,DB_DEMON,&p,1);//디비에게 보냅니다.
	}
}

void CAuction::RecvCMD_MERCHANT_ITEM_BUY_COMFORM_RESULT(const int iCn,t_packet &p)//구매중으로 전환 했다
{	//돈을 뺏고 돈기록 하라는 것을 디비에게 보낸다 돈 못뺏으면 기록 취소를 매긴다 IsEnd =0
	//사도 좋다는 답이 오면 IS_END_WAIT_TAKE 으로 해 주고 아니면 IS_END_ALL_RIGHT 으로 돌려준다.
	const int iUserCn = p.u.MerchantItemBuy.iCn;
	CHARLIST *ch = ::CheckServerId(iUserCn);
	if(!ch){return;}
	if(::strcmp(ch->Name,p.u.MerchantItemBuy.szBuyerName)){return;}
	const int iIndex = p.u.MerchantItemBuy.iIndex;

	const DWORD dwSellValue = p.u.MerchantItemBuy.dwSellValue;

	if(IS_END_BUYING == p.u.MerchantItemBuy.iKey)//구입 가능 상태
	{
		const DWORD dwBuyValue = dwSellValue;//dwSellValue는 DB꺼를 받아온다
		if(!dwSellValue)//직접 거래모드
		{	//상대를 찾아서 허가를 받아야 한다
			//상태 전환이 된 상태.
			//두놈에게 교환창을 띄우라고 하고.
			//거래가 취소되면 IS_END_ALL_RIGHT 로 바꾸고
			//거래가 성공 하면 IS_END_WAIT_TAKE로 만들어 준다
			K_ITEM tempK_Item;
			tempK_Item.item_attr = p.u.MerchantItemBuy.SellItem;
			tempK_Item.item_pos.type = AUCTION_DB;

			tempK_Item.item_pos.p1 = LOWORD(iIndex);	// 030929 HK YGI
			tempK_Item.item_pos.p2 = HIWORD(iIndex);	// 030929 HK YGI

			CHARLIST *pSeller = ::CheckServerId(ExistHe(p.u.MerchantItemBuy.szSellerName));
			if(	pSeller)//판매자가 있다 거리가 7 타일 이상이다
			{//021121 lsw
				int Gab = ((int )(::sqrt( ::pow((float)(pSeller->X - ch->X), (float)2)+pow((float)(pSeller->Y - ch->Y), (float)2) ) / TILE_SIZE));
				if(	7 > Gab )
				{
					if(::RecvItemExchange( &tempK_Item, iUserCn , pSeller->GetServerID() ,EXCHANGE_TYPE_MERCHANT_DIRECT))//교환요청 성공
					{//마치 상인이 신청 하는것 처럼 그렇게 해야 결과물 아이템이 걸린다 iUserCn 안바뀌게 조심
						return;
					}
				}
			}
			//판매자가 없다거나 거리가 멀다면
			p.u.MerchantItemBuy.iKey = IS_END_ALL_RIGHT;
		}
		else
		{	//< CSD-030723
			if (ch->DecBankMoney(dwBuyValue, BMCT_AUCTION_ITEM_BUY))
			{	// 돈을 받았다면
				p.u.MerchantItemBuy.iKey = IS_END_WAIT_TAKE;
			}
			else
			{
				p.u.MerchantItemBuy.iKey = IS_END_ALL_RIGHT;
			}
		}	//> CSD-030723

		p.h.header.type = CMD_MERCHANT_ITEM_BUY_COMFORM_RESULT;
		::QueuePacket(connections,DB_DEMON,&p,1);//디비에 구매가능한지 물어본다
	}
	else//구입 불가네.. 뭐 어쨋든.. 
	{//디비에다 할 일은 없고. 클라이언트에게 못사게 되었다는 말을 전해 주자
		p.h.header.type = CMD_MERCHANT_ITEM_BUY_RESULT;
		::QueuePacket(connections,iUserCn,&p,1);//디비에 구매가능한지 물어본다
	}
}

void CAuction::SendCMD_MERCHANT_ITEM_BUY_COMFORM_RESULT(CHARLIST *ch, const int iIndex, const ITEMATTR &Item, const bool bSuccess)
{//직접 거래시 취소 되거나 성공 했을때 사용 하는 부분
	//거래 취소 되거나 했을때 없에 주는 부분
	t_packet p;
	p.h.header.type = CMD_MERCHANT_ITEM_BUY_COMFORM_RESULT;
	p.h.header.size = sizeof(MERCHANT_ITEM_BUY);

	lpMERCHANTITEMBUY pSend = &p.u.MerchantItemBuy;

	pSend->iIndex = iIndex;//구입할 아이템의 인덱스 값
	pSend->iCn = ch->GetServerID();//커넥션 값
	
	::strcpy_s(pSend->szSellerName, sizeof(pSend->szSellerName),ch->Name);//판매자 이름
	::strcpy_s(pSend->szBuyerName, sizeof(pSend->szBuyerName),ch->GetExchangeTargetName());//구매자 이름

	pSend->dwSellValue = 0;//0원이다 직접거래니까..
	pSend->SellItem = Item;

	pSend->iKey = ((bSuccess)?IS_END_WAIT_TAKE:IS_END_ALL_RIGHT);

	::QueuePacket(connections,DB_DEMON,&p,1);//디비에 구매가능한지 물어본다
}

void CAuction::RecvCMD_MERCHANT_ITEM_BUY_RESULT(t_packet &p)//구매하기(구매자가 선택
{
	const int iUserCn = p.u.MerchantItemBuy.iCn;
	CHARLIST *ch = ::CheckServerId(iUserCn);
	if(!ch){return;}//사겠다던 놈이 나갔습니다.
	if(strcmp(p.u.MerchantItemBuy.szBuyerName, ch->Name)){return;}//이름이 틀려
	::QueuePacket(connections,iUserCn,&p,1);//클라이언트로 결과를 보냅니다.
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 구매 하기 끝(구매자)
////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 판매자가 팔고 있는 리스트 받기 시작(판매자)
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAuction::RecvCMD_MERCHANT_SELL_LIST_REQUEST(const int cn,t_packet &p)//리스트 요청(판매자가 보는것
{
	if(cn == p.u.SellerItemRequest.iCn)
	{
		CHARLIST * ch = CheckServerId(cn);
		if(!ch){return;}
		if(!strcmp(ch->Name,p.u.SellerItemRequest.szName))//이름 검색후 클라이언트로 스위칭
		if(!GMMgr.IsMerchant(ch)){return;}
		::QueuePacket(connections,DB_DEMON,&p,1);
	}
}

void CAuction::RecvCMD_MERCHANT_SELL_LIST_REQUEST_RESULT(t_packet &p)//리스트 결과(판매자가 보는것 디비 데몬이 보낸걸 클라이언트로 스위칭
{
	const int cn = p.u.SearchResultList.iCn;
	CHARLIST * ch = CheckServerId(cn);
	if(!ch){return;}
	if(!strcmp(ch->Name,p.u.SearchResultList.szName))//이름 검색후 클라이언트로 스위칭
	{
		if(!GMMgr.IsMerchant(ch)){return;}
		::QueuePacket(connections,cn,&p,1);
	}	
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 판매자가 팔고 있는 리스트 받기 끝(판매자)
////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 삭제 하기 시작(판매 취소)
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//C 삭제가능? ->M 자리체크하고 디비한테 물어볼께 ->D 그래 지울 수 있겠네 ->M 아이템 가져 -> D 삭제 시킬께.
void CAuction::RecvCMD_MERCHANT_SELL_ITEM_DELETE(const int cn,t_packet &p)//판매 아이템을 삭제
{	//삭제 가능 한지 물어본다 (IsEnd를 삭제중으로 체크)//유저에게 받았습니다.
	CHARLIST *ch = CheckServerId(cn);
	if(!ch){return;}
	if(strcmp(p.u.SellerItemDelete.szName, ch->Name)){return;}//이름이 틀려	

	int a=0,b=0,c=0;
	if(!GMMgr.IsMerchant(ch)){return;}
	if(::SearchInv(ch->inv,a,b,c))//빈공간이 있는가
	{
		p.h.header.type = CMD_MERCHANT_SELL_ITEM_DELETE_COMFORM;//타입 전환
		::QueuePacket(connections,DB_DEMON,&p,1);//디비에게 보냅니다.
	}
	else
	{	
		::OutMessage(ch,2,13);//임시,,인벤토리 공간이 충분치 않습니다.
	}
}

void CAuction::RecvCMD_MERCHANT_SELL_ITEM_DELETE_COMFORM_RESULT(const int cn,t_packet &p)//판매 아이템을 삭제
{//디비에게 보낸걸 받았습니다.
	const int iUserCn = p.u.SellerItemDelete.iCn;
	CHARLIST *ch = CheckServerId(iUserCn);
	if(!ch){return;}//지우겠다던놈이 나갔습니다.

	if(-1  == p.u.SellerItemDelete.iKey)
	{//삭제니 나발이니 할께 없음 지급 안함
		return;
	}
	if(strcmp(p.u.SellerItemDelete.szName, ch->Name))
	{
		return;
	}//이름이 틀려

{//아이템 지급전 체크//아이템 지급이 되면 무슨 일이 있어도 
	bool bIsFault = false;
	int a=0,b=0,c=0;//아이템 지급
	if(!::SearchInv(ch->inv,a,b,c)){bIsFault = true;return;}//이런 상황이 생기나? 아까 컴폼받기 전에 인벤토리 체크 했는데..
	POS pos;
	SetItemPos(INV,a,b,c,&pos);//pos셋팅 
	ITEMATTR *pItem = ::GetItemByPOS(ch,pos);//아이템 주소 받고
	if(!pItem){bIsFault = true;return;}//여기에 아이템이 없다?? 미친..

	if(bIsFault)
	{
		p.h.header.type = CMD_MERCHANT_SELL_ITEM_DELETE_COMFORM_RESULT;//타입 전환
		p.u.SellerItemDelete.iKey = IS_END_ALL_RIGHT;//지우기전의 거래 가능 상태로 변경
		::QueuePacket(connections,DB_DEMON,&p,1);
	}
	else
	{
		(*pItem) = p.u.SellerItemDelete.SellItem;//실제 지급 하는 부분
		::SendItemEventLog( pItem, ch->GetServerID(), 0, SILT_CANCEL_AUCTION_SELL_ITEM, 1 );//021209 lsw
		::SendServerEachItem( &pos, pItem,iUserCn);//보내기
		p.h.header.type = CMD_MERCHANT_SELL_ITEM_DELETE_COMFORM_RESULT;//타입 전환
		p.u.SellerItemDelete.iKey = IS_END_DELETE_COMPLETE;
		::QueuePacket(connections,DB_DEMON,&p,1);
	}
}
	::QueuePacket(connections,iUserCn,&p,1);
	//지급이 끝났으므로//삭제를 시킨다 (IsEnd를 IS_END_DELETE_COMPLETE로 체크)
}

void CAuction::RecvCMD_MERCHANT_SELL_ITEM_DELETE_RESULT(const int cn,t_packet &p)//판매 아이템을 삭제
{//디비데몬으로 부터 결과를 받는 부분 반드시 iKey 는 IS_END_DELETING 이 체크 되어 있어야 한다
 //삭제 결과를 클라이언트로 보냅니다.
 //지운 아이템은 캐릭터에게 넣어줍니다.
	const int iUserCn = p.u.SellerItemDelete.iCn;
	CHARLIST *ch = CheckServerId(iUserCn);
	if(!ch){return;}//지우겠다던놈이 나갔습니다.
	if(strcmp(p.u.SellerItemDelete.szName, ch->Name)){return;}//이름이 틀려

	const int iKey = p.u.SellerItemDelete.iKey;
	if(	IS_END_DELETE_COMPLETE  != iKey//삭제 성공 
	&&	IS_END_ALL_RIGHT		!= iKey)//삭제 실패 
	{//위 둘중에 하나가 아니면 에러다.디비에서는 이렇게 처리 하니까.
		MyLog(0,"Auction Critical Error!! Type => CMD_MERCHANT_SELL_ITEM_DELETE_COMFORM_RESULT");
	}
	::QueuePacket(connections,iUserCn,&p,1);

	//<! BBD 040226	등록 취소가 완결되었다. DBDemon에게 레코드를 삭제하라고 알린다.
	if(iKey == IS_END_DELETE_COMPLETE)
	{
		p.h.header.type = CMD_MERCHANT_RECORD_DEL_CANCLE;//타입 전환
		::QueuePacket(connections,DB_DEMON,&p,1);//디비에게 보냅니다.

		t_SellerItemDelete item = p.u.SellerItemDelete;
		// 들어올때 파는 놈과 사는놈이름이 바뀌어 있다.
		// 취소여서 사는놈이 파는놈으로 되고 파는놈은 신경 않쓰는듯...
		// BBD는 이런거 싫어한다
		strcpy_s(item.szSellerName, sizeof(item.szSellerName), p.u.SellerItemDelete.szName);
		memset(item.szName, 0L, sizeof(item.szName));
		
		g_pLogManager->SaveAutionLog(ACLT_SELLER_CANCLE, &(item));
	}
	//> BBD 040226	등록 취소가 완결되었다. DBDemon에게 레코드를 삭제하라고 알린다.

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 삭제 하기 끝(판매 취소)
////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 등록 하기
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAuction::RecvCMD_MERCHANT_SELL_ITEM_REGISTER(const int iCn, t_packet &p)//판매 아이템을 등록
{//클라이언트에게만 받습니다.//디비로 전송 합니다.
	const int cn = p.u.SellerItemRegister.iCn;
	CHARLIST * ch = CheckServerId(cn);
	if(!ch){return;}
	//이름 검사및 아이템 있는지 체크등등.. 
	if(strcmp(p.u.SellerItemRegister.szName, ch->Name)){return;}//이름이 틀려
	
	ITEMATTR* pItem  = ::GetItemByPOS(ch,p.u.SellerItemRegister.pos);//아이템 받는다
	if(!pItem){return;}//아이템 없네 
	if(memcmp(pItem,&p.u.SellerItemRegister.SellItem,sizeof(ITEMATTR))){return;}//아이템이 틀리네

	if(ch->GetBankMoney() < AUCTION_CANCEL_MONEY)//AUCTION_CANCEL_MONEY 보다 은행돈이 작다면 실패다
	{
		::OutMessage(ch,2,12);//돈이 모자랍니다.
		return;
	}	
	//상인인가 체크를 반드시 해 볼것
	//아이템의 클래스,레어,레벨,택틱,입은종류
	CItem *t = ::ItemUnit(*pItem);
	if(!t){return;}//장난치나.. 이따위 아이템을!

	// finito 040707 check for noexchange item kind
	if (IsNoExchange(pItem))
	{
		SendPutMenuString( KM_FAIL, 244, ch->GetServerID() );
		return;
	}
	// end of check

	DWORD dwATTR3 = pItem->attr[IATTR_RARE_MAIN];
	
	ITEMRAREMAIN *pRare = (LPRareMain)&dwATTR3;

	int iRareType = NORMAL_ITEM;
	if(dwATTR3)//레어검사
	{//	pRare
		switch(pRare->iHighLevel)
		{
		case H_LV_HIGH_ITEM:
			{
				iRareType = HIGH_ITEM;
			}break;
		case H_LV_NO_HIGH_ITEM:
			{
				iRareType = RARE_ITEM;
				if(pRare->IsDynamicRare)
				{
					iRareType = DYNAMIC_RARE_ITEM;
				}
			}break;
		case H_LV_LEGEND_ITEM:
			{
				iRareType = SET_ITEM;
			}break;
		}
	}
	else
	{
		iRareType = NORMAL_ITEM;
	}
	p.u.SellerItemRegister.iFKRareType	= iRareType;
	p.u.SellerItemRegister.iFKLevel		= (t->GetNeed3Lv()/10);
	p.u.SellerItemRegister.iFKWearType	= t->GetWearAble();
	p.u.SellerItemRegister.iFKTacticType= t->GetSkill_Ability();

	//가격 검사
	const int iSellValue = (int)p.u.SellerItemRegister.dwSellValue;
	if(iSellValue >= 0 && iSellValue < INT_MAX)//0보다 크고 INT_MAX보다 작아야 한다.. 그래야 양수다
	{	//검사 끝 
		p.h.header.type = CMD_MERCHANT_SELL_ITEM_REGISTER_COMFORM;//디비에게 갯수 체크 받으러 가자
		::QueuePacket(connections,DB_DEMON,&p,1);
	}
	return;//int 넘는 돈은 없어
}

void CAuction::RecvCMD_MERCHANT_SELL_ITEM_REGISTER_COMFORM_RESULT(const int iCn, t_packet &p)
{
	const int iUserCn = p.u.SellerItemRegister.iCn;
	CHARLIST *ch = CheckServerId(iUserCn);
	if(!ch){return;}//새끼 그사이에 나갔네..
	if(strcmp(ch->Name ,p.u.SellerItemRegister.szName)){return;}//얼레 다른놈이네.

	//수수료 체크 
	if(ch->GetBankMoney() < AUCTION_CANCEL_MONEY)//AUCTION_CANCEL_MONEY 보다 은행돈이 작다면 실패다
	{//이런 상황이 생기나? 아까 컴폼받기 전에 은행돈 체크 했는데..
		::OutMessage(ch,2,12);//돈이 모자랍니다.
		return;
	}
	
	if (ch->DecBankMoney(AUCTION_CANCEL_MONEY, BMCT_AUCTION_ITEM_REGISTER))
	{	//< CSD-030723 : 취소 수수료 받기이 있구나 마이너스 안되게끔 처리
		if (1 == p.u.SellerItemRegister.iKey)
		{	//아이템을 빼앗고 디비에게 아이템을 등록 시킵니다.
			POS pos = p.u.SellerItemRegister.pos;
			ITEMATTR* pItem  = ::GetItemByPOS(ch,pos);//아이템 받는다
			if(!pItem){return;}//아이템 없네 
			if(memcmp(pItem,&p.u.SellerItemRegister.SellItem,sizeof(ITEMATTR))){return;}//아이템이 틀리네
			//이래저래 맞으니까
			::SendItemEventLog( pItem, ch->GetServerID(), 0, SILT_REGISTER_AUCTION_ITEM, 1 );//021209 lsw
			::DeleteItem(pItem);//아이템 뺏았다
			::SendServerEachItem( &pos, pItem, iUserCn);//보내기//지워진걸 보낸다
			::QueuePacket(connections,DB_DEMON,&p,1);//디비에 등록 해달라고 한다
		}
		else
		{	//야.. 뭘 그렇게 많이 팔려구 그러냐.. 그만 팔어(갯수제한 걸림)
			::QueuePacket(connections,iUserCn,&p,1);
		}	
	}	//> CSD-030723
}


void CAuction::RecvCMD_MERCHANT_SELL_ITEM_REGISTER_RESULT(t_packet &p)//판매 아이템을 등록
{//디비에게 받아서 클라이언트로 전송 합니다.
	const int  iUserCn = p.u.SellerItemRegister.iCn;
	CHARLIST* ch = CheckServerId(iUserCn);
	if(!ch){return;}
	if(strcmp(ch->Name ,p.u.SellerItemRegister.szName)){return;}//얼레 다른놈이네.
	const int iKey = p.u.SellerItemRegister.iKey;
	if(!iKey)
	{//등록 실패인데? ㅡㅡ;.. 아이템 지웠으니까 복구 해 줘야 하네..		
		int a=0,b=0,c=0;
		POS pos;
		ITEMATTR *pNoRegItem = &p.u.SellerItemRegister.SellItem;
		if(!::SearchInv(ch->inv,a,b,c))
		{//미친놈 그사이에 안벤토리를 채웠어??
			AddItemList( pNoRegItem->item_no,pNoRegItem->attr,0, 
				ch->X+rand()%40, ch->Y+rand()%60, 0, 0,  0,0,0,0);//바닥에 떨어 졌어
		}
		else
		{//지급
			SetItemPos(INV,a,b,c,&pos);//pos 채우고
			ITEMATTR *pItem = ::GetItemByPOS(ch,pos);//아이템 주소 받고
			(*pItem) = (*pNoRegItem);//아이템 다시 지급
			::SendServerEachItem( &pos, pItem, iUserCn);//보내기//지워진걸 보낸다
		}
	}
	//<! BBD 040226 등록시 옥션로그
	else	// 등록 성공이니 로그를 남기자
	{
		t_SellerItemDelete item;
		memset(&item, 0L, sizeof(item));

		item.dwSellValue = p.u.SellerItemRegister.dwSellValue;
		item.iCn = p.u.SellerItemRegister.iCn;
		item.iIndex = 0;		//	등록시는 인덱스가 않들어온다.
		item.iKey = 0;			// 등록 잘됬으니 IsEnd는 0이겠지
		item.SellItem = p.u.SellerItemRegister.SellItem;
		strcpy_s(item.szSellerName, sizeof(item.szSellerName), p.u.SellerItemRegister.szName);
		
		g_pLogManager->SaveAutionLog(ACLT_SELLER_REGIST, &(item));
	}
	//> BBD 040226 등록시 옥션로그

	::QueuePacket(connections,iUserCn,&p,1);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 등록 하기 끝
////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 거래 결과 리스트 받기 시작
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAuction::RecvCMD_MERCHANT_RESULT_LIST_REQUEST(const int cn,t_packet &p)
{
	if(cn != p.u.SellerItemRequest.iCn){return;}

	CHARLIST * ch = CheckServerId(cn);
	if(!ch){return;}
	if(!strcmp(ch->Name,p.u.SellerItemRequest.szName))//이름 검색후 클라이언트로 스위칭

	::QueuePacket(connections,DB_DEMON,&p,1);

}

void CAuction::RecvCMD_MERCHANT_RESULT_LIST_REQUEST_RESULT(t_packet &p)
{
	const int cn = p.u.SearchResultList.iCn;
	CHARLIST * ch = CheckServerId(cn);
	if(!ch){return;}
	if(!strcmp(ch->Name,p.u.SearchResultList.szName))//이름 검색후 클라이언트로 스위칭
	{
		::QueuePacket(connections,cn,&p,1);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 거래 결과 리스트 받기 끝
////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//거래 결과물 가져가기 시작
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAuction::RecvCMD_MERCHANT_RESULT_TAKE(const int cn,t_packet &p)
{
	CHARLIST *ch = CheckServerId(cn);
	if(!ch){return;}
	if(strcmp(p.u.MerchantResultTake.szMyName, ch->Name)){return;}//이름이 틀려	

	if(!strcmp(p.u.MerchantResultTake.szMyName,p.u.MerchantResultTake.szSellerName))//판매자 이면
	{//돈 받을 수 있는지 체크
		const int dwSellValue = p.u.MerchantResultTake.dwSellValue;
		const int iNeedCount  = ConvBondMoneyCount(dwSellValue);
		const int iHaveCount  = SearchInvEmptyCount(ch->inv);
		if(iNeedCount > iHaveCount)//돈받을 칸이 모자란데
		{
			::OutMessage(ch,2,13);//임시,,인벤토리 공간이 충분치 않습니다.
			return;
		}
		else
		{
			p.u.MerchantResultTake.iKey = 1;//판매자로 설정
			p.h.header.type = CMD_MERCHANT_RESULT_TAKE_COMFORM;//타입 전환
			::QueuePacket(connections,DB_DEMON,&p,1);//디비에게 보냅니다.
		}
	}
	else//구매자 이면
	{
		int a=0,b=0,c=0;
		if(::SearchInv(ch->inv,a,b,c))//빈공간이 있는가
		{
			p.u.MerchantResultTake.iKey = 0;//구매자로 설정
			p.h.header.type = CMD_MERCHANT_RESULT_TAKE_COMFORM;//타입 전환
			::QueuePacket(connections,DB_DEMON,&p,1);//디비에게 보냅니다.
		}
		else
		{	
			::OutMessage(ch,2,13);//임시,,인벤토리 공간이 충분치 않습니다.
		}
	}
}

void CAuction::RecvCMD_MERCHANT_RESULT_TAKE_COMFORM_RESULT(const int cn,t_packet &p)
{//테이크 상태로 돌입 했다
	MERCHANTRESULTTAKE *pMRT =  &p.u.MerchantResultTake;
	const int iUserCn = pMRT->iCn;
	CHARLIST *ch = CheckServerId(iUserCn);
	if(!ch){return;}
	if(strcmp(pMRT->szMyName, ch->Name)){return;}//이름이 틀려	

	if(-1 == pMRT->iKey)//판매자 사이드
	{	//너 지급받는데 문제가 있어
		return;
	}

	if(1 == pMRT->iKey)
	{//판매자야 돈만 먹어라
		const DWORD dwSellValue = p.u.MerchantResultTake.dwSellValue;
		if(::GiveMoney(ch,dwSellValue))
		{//지급성공
			pMRT->iKey += 100;
		}
		else
		{
			pMRT->iKey += 10;
		}
	}
	else
	{
		int a=0,b=0,c=0;
		if(::SearchInv(ch->inv,a,b,c))//공간이 있구나
		{
			POS pos;
			SetItemPos(INV,a,b,c,&pos);//pos셋팅 
			ITEMATTR *pItem = ::GetItemByPOS(ch,pos);//아이템 주소 받고
			(*pItem) = pMRT->SellItem;
			::SendServerEachItem( &pos, pItem,iUserCn);//보내기
			::SendItemEventLog( pItem, ch->GetServerID(), 0, SILT_TAKE_AUCTION_RESULT_ITEM, 1 );//021209 lsw
			pMRT->iKey += 100;
		}
		else//이런 상황이 생기나? 아까 컴폼받기 전에 인벤토리 체크 했는데..
		{
			::OutMessage(ch,2,13);//임시,,인벤토리 공간이 충분치 않습니다.
			pMRT->iKey += 10;
		}
	}
	p.h.header.type = CMD_MERCHANT_RESULT_TAKE_COMFORM_RESULT;//타입 전환
	::QueuePacket(connections,DB_DEMON,&p,1);//디비에게 보냅니다.
}

void CAuction::RecvCMD_MERCHANT_RESULT_TAKE_RESULT(t_packet &p)
{
	const int cn = p.u.MerchantResultTake.iCn;
	CHARLIST * ch = CheckServerId(cn);
	if(!ch){return;}
	if(!strcmp(ch->Name,p.u.MerchantResultTake.szMyName))//이름 검색후 클라이언트로 스위칭
	{
		::QueuePacket(connections,cn,&p,1);
	}

	//<! BBD 040226 판매자이던 구매자이던 거래 결과물을 모두 잘 받았다. DBDemon에게 레코드를 삭제하라고 알린다.
	if(p.u.MerchantResultTake.iKey == 101 || p.u.MerchantResultTake.iKey == 100)
	{
		p.h.header.type = CMD_MERCHANT_RECORD_DEL_COMPLETE;//타입 전환
		::QueuePacket(connections,DB_DEMON,&p,1);//디비에게 보냅니다.
	}
	//> BBD 040226 판매자이던 구매자이던 거래 결과물을 모두 잘 받았다. DBDemon에게 레코드를 삭제하라고 알린다.

	//<! BBD 040226 어쨋거나 지급되었으니 로그를 남겨야지
	t_SellerItemDelete item;
	memset(&item, 0L, sizeof(item));

	item.iCn = p.u.MerchantResultTake.iCn;
	item.iIndex = p.u.MerchantResultTake.iIndex;
	strcpy_s(item.szSellerName, sizeof(item.szSellerName), p.u.MerchantResultTake.szSellerName);
	strcpy_s(item.szName, sizeof(item.szName), p.u.MerchantResultTake.szBuyerName);
	item.iKey = p.u.MerchantResultTake.iKey;
	item.dwSellValue = p.u.MerchantResultTake.dwSellValue;
	item.SellItem = p.u.MerchantResultTake.SellItem;

	if(p.u.MerchantResultTake.iKey == 101)	// 판매자 돈 지급 성공
	{
		g_pLogManager->SaveAutionLog(ACLT_SELLER_TAKE, &(item));

	}
	else if(p.u.MerchantResultTake.iKey == 100)	// 구매자 아이템 지급 성공
	{
		g_pLogManager->SaveAutionLog(ACLT_BUYER_TAKE, &(item));
	}
	//> BBD 040226 어쨋거나 지급되었으니 로그를 남겨야지
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 거래 결과물 가져가기 끝
////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//개인적 거래 리스트 시작
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAuction::RecvCMD_MERCHANT_DIRECT_EXCHANGE_LIST_REQUSET(const int cn,t_packet &p)
{
	const int iTargetCn =ExistHe(p.u.MerchantExchangeRequest.szSellerName);
	const CHARLIST *pTargetCh = CheckServerId(iTargetCn);
	if(!pTargetCh){return;}
	p.u.MerchantExchangeRequest.iCn = cn;
	::QueuePacket(connections,DB_DEMON,&p,1);//디비에게 보냅니다.
}

void CAuction::RecvCMD_MERCHANT_DIRECT_EXCHANGE_LIST_REQUSET_RESULT(t_packet &p)
{
	const int cn = p.u.SearchResultList.iCn;
	CHARLIST * ch = CheckServerId(cn);
	if(!ch){return;}
	if(!strcmp(ch->Name,p.u.SearchResultList.szName))//이름 검색후 클라이언트로 스위칭
	{
		::QueuePacket(connections,cn,&p,1);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//개인적 거래 리스트 끝
////////////////////////////////////////////////////////////////////////////////////////////////////////////
int SearchInvEmptyCount( ItemAttr inv[3][3][8])
{
	int iCount = 0;
	for( int a=0; a<3; a++)
	{	for( int b=0; b<3; b++)
		{	for( int c=0; c<8; c++)
			{
				if( !inv[a][b][c].item_no )
				{
					iCount++;
				}
			}
		}
	}
	return iCount;	
}

//BondMoney 처리부
int ConvBondMoneyCount(const DWORD dwMoney,const int iaBondIndex)//돈을 넣으면 받을 본드머니와 돈 아이템의 갯수를 리턴
{//코드 순서 바꾸지 말것 // iaBondIndex aBondMoney의 인덱스를 의미
	if( 0				> iaBondIndex//인덱스 이상
	||	BOND_MONEY_MAX	< iaBondIndex){return 0;}

	int iCount = 0;
	DWORD dwTemp = dwMoney;
	int iDivideValue = 0;
	for(int i = iaBondIndex; BOND_MONEY_MAX > i;i++)
	{
		iDivideValue = aBondMoney[i].iValue;//나눌 값 셋팅
		iCount += dwTemp/iDivideValue;//아이템 갯수 체크
		dwTemp%=iDivideValue;//잔여금 설정
	}
	iDivideValue = MAX_MONEY;//나눌 값 셋팅
	iCount += dwTemp/iDivideValue;//아이템 갯수 체크
	dwTemp%=iDivideValue;//잔여금 설정
	return ((dwTemp)?iCount:iCount+1);
}

int AddMoney( const DWORD money, CHARLIST *ch)		// 010210 YGI
{	//우선 지급 하고 나중에 갈라내는 방식입니다.
	if( !money ) {return 1;}
	
	int iItemKind = IK_NEW_MONEY;
	if(LocalMgr.IsChangeMoney())//030102 lsw
	{
		iItemKind = IK_NEW_MONEY;
	}
	else
	{
		iItemKind = IK_MONEY;
	}

	int a=0, b=0, c=0;
	ItemAttr *pItem = ::SearchItemOfInvByKind( iItemKind, ch, a, b, c );
	
	if( !pItem )// 돈 아이템이 하나도 없을때 새로 생성
	{
		if( !::SearchInv( ch->inv, a, b, c ) ) 		// 빈공간이 없을때 
		{
			int iItemNo = IN_NEW_COINS;
			if(LocalMgr.IsChangeMoney())//030102 lsw
			{
				iItemNo = IN_NEW_COINS;
			}
			else
			{
				iItemNo = IN_COINS;
			}

			ItemAttr temp = ::GenerateItem( iItemNo, IATTR_MUCH, money);//020509 lsw
			// 땅에 떨어 트린다.
			::SendItemEventLog( &temp, ch->GetServerID(), 0, SILT_PUT_GROUND_MAX_INVEN, 1 ); //YGI acer
			::DropItem( ch->X, ch->Y, &temp );//10억짜리 아이템이 나오는 이유
			return -1;
		}//if( !::SearchInv( ch->inv, a, b, c ) ) 		// 빈공간이 없을때 
		else//빈공간이 있을때
		{
			ItemAttr *new_item = &ch->inv[a][b][c];

			int iCoinNo = IN_NEW_COIN;
			int iCoinsNo = IN_NEW_COINS;
			if(LocalMgr.IsChangeMoney())//030102 lsw
			{
				iCoinNo = IN_NEW_COIN;
				iCoinsNo = IN_NEW_COINS;
			}
			else
			{
				iCoinNo = IN_COIN;
				iCoinsNo = IN_COINS;
			}
			if( money > 5 )
			{
				new_item->item_no = iCoinsNo;
			}
			else 
			{
				new_item->item_no = iCoinNo;
			}

			new_item->attr[IATTR_MUCH] = money;
			pItem = new_item;

			POS pos = {0,};				// 010117 YGI
			::SetItemPos( INV, a, b, c, &pos );
			::SendServerEachItem( &pos, pItem, ch->GetServerID());
		}
	}//if( !pItem )// 돈 아이템이 하나도 없을때 새로 생성
	else 
	{//돈아이템이 있을때
		pItem->attr[IATTR_MUCH] += money;
	}

	POS pos = {0,};
	::SetItemPos( INV, a, b, c, &pos );//금방 지급한 아이템을 갈라낸다	
	const int cn = ch->GetServerID();
	switch(CheckAutoDivideMoney(pItem, cn))//100만원 넘는 아이템 갈라내기
	{
	case 1:// 새로 생긴 돈 아이템을 보내준다.
		{
			::SendServerEachItem( &pos, pItem, cn);
			return 1;
		}break;
	case 2:
		{
			::SendChangeDurationByKein( &pos, pItem->attr[IATTR_MUCH], cn );
			return 2;
		}break;
	}

	return 1;
}

int GiveMoneyAble(CHARLIST *ch, const DWORD dwMoney,const int iaBondIndex)
{
	if(!dwMoney){return 1;}//0원 지급 하겠다고 하면 무조건 성공한걸로 일러라
	if(SM6.iValue > dwMoney){return 1;}//500 만원 미만은 지급 시킵니다. 바닥에 떨어지더라도//021212 lsw
	if( 0				> iaBondIndex//인덱스 이상
	||	BOND_MONEY_MAX	< iaBondIndex){return 0;}
	
	const int iNeedCount  = ::ConvBondMoneyCount(dwMoney);
	const int iHaveCount  = ::SearchInvEmptyCount(ch->inv);
	
	if(iNeedCount > iHaveCount)
	{
		::OutMessage(ch,2,13);//임시,,인벤토리 공간이 충분치 않습니다.
		return 0;
	}//돈받을 칸이 모자란데
	return 1;
}

int GiveMoney(CHARLIST *ch, const DWORD dwMoney,const int iaBondIndex)//iaBondIndex aBondMoney의 인덱스를 의미
{//저 인덱스의 아이이템 이하로 지급 하겠다는 이야기.

	if(!GiveMoneyAble(ch,dwMoney,iaBondIndex)){return 0;}
			
	DWORD dwTemp = dwMoney;
	int iDivideValue = 0;
	for(int i = iaBondIndex; BOND_MONEY_MAX > i;i++)
	{
		iDivideValue = aBondMoney[i].iValue;//나눌 값 셋팅
		const int iCount = dwTemp/iDivideValue;//아이템 갯수 체크
		for(int j =0; iCount >j;j++)
		{
			int a=0,b=0,c=0;
			if(::SearchInv(ch->inv,a,b,c))
			{
				POS pos;
				SetItemPos(INV,a,b,c,&pos);//pos 채우고
				ITEMATTR *pItem = ::GetItemByPOS(ch,pos);//아이템 주소 받고
				(*pItem) = ::GenerateItem(aBondMoney[i].iItemNo);
				::SendServerEachItem(pItem,ch);
				::SendItemEventLog( pItem, ch->GetServerID(), 0, SILT_BOND_MONEY_GIVE, 1 );//021209 lsw
			}			
		}
		dwTemp%=iDivideValue;//잔여금 설정
	}
	//본드 머니로 지급 한 후의 돈을 동전 아이템으로 지급한다
	::AddMoney(dwTemp,ch);//500 뭔원 이하의 돈이 들어갈 것이다
	ch->Money = GetMoneyByItem( ch );
	return 1;
}

void CAuction::RecvCMD_EXCHANGE_BOND_MONEY(const int cn,t_packet &p)
{
	LPEXCHANGEBONDMONEY pEBM= &p.u.Hwoa.rang.ExBondMoney;

	if(pEBM->iCn != cn){return;}//다르면 안된다.
	
	LPCHARLIST pCh = ::CheckServerId(cn);
	if(!pCh){return;}//캐릭터 받기
	
	ITEMATTR *pItem = ::GetItemByPOS(pCh,pEBM->pos);
	if(!pItem){return;}//아이템 빼기
	if(::memcmp(pItem,&pEBM->BondMoneyItem,sizeof(ITEMATTR))){return;}//아이템 비교하기
	
	const int iItemNo = pEBM->BondMoneyItem.item_no;//아이템 넘버 비교 하기
	if(pItem->item_no != iItemNo){return;}

	int iValue = 0;
	int iIndex = 0;
	switch(iItemNo)//지급 하기
	{
	case BOND_MONEY_1000://10억 크릿 본드	
		{
			iValue = aBondMoney[0].iValue;
			iIndex = 1;
		}break;
	case BOND_MONEY_500	://5억 크릿 본드	
		{
			iValue = aBondMoney[1].iValue;
			iIndex = 2;
		}break;
	case BOND_MONEY_100	://1억 크릿 본드	
		{
			iValue = aBondMoney[2].iValue;
			iIndex = 3;
		}break;
	case BOND_MONEY_50	://5천만 크릿 본드	
		{
			iValue = aBondMoney[3].iValue;
			iIndex = 4;
		}break;
	case BOND_MONEY_10	://1천만 크릿 본드	
		{
			iValue = aBondMoney[4].iValue;
			iIndex = 5;
		}break;
	case BOND_MONEY_5	://5백만 크릿 본드	
		{
			iValue = aBondMoney[5].iValue;
			iIndex = 6;
		}break;
	default://본드머니 아니면 무시 하자.
		{
			return;
		}
	}//switch(iItemNo)//지급 하기

	if(::GiveMoney(pCh, iValue,iIndex))
	{
		::SendItemEventLog( pItem, pCh->GetServerID(), 0, SILT_EXCHANGE_BOND_MONEY, 1 );//021209 lsw
		::DeleteItem(pItem);//줬으니까 아이템지워
		::SendServerEachItem(&pEBM->pos,pItem,cn);
	}

	return;
}
//<! BBD 040303		머천트백업 테이블로을 사용하는 함수군
void  CAuction::RecvCMD_MERCHANT_BACKUP_TAKE_REQUEST(const int cn,t_packet &p)
{
	//iKey에 IsEnd필드값이 들어있다
	t_MerchantResultTake * pRecord = &(p.u.MerchantResultTake);
	const int iUserCn = pRecord->iCn;
	CHARLIST *ch = CheckServerId(iUserCn);
	if(!ch){return;}
	if(strcmp(pRecord->szMyName, ch->Name)){return;}//이름이 틀려	

	int IsEnd = pRecord->iKey;

	// 로그를 위해 아이템 정보를 긁는다
	t_SellerItemDelete item;
	memset(&item, 0L, sizeof(item));

	item.dwSellValue = pRecord->dwSellValue;
	item.iCn = pRecord->iCn;
	item.iIndex = pRecord->iIndex;
	item.SellItem = pRecord->SellItem;
	strcpy_s(item.szName, sizeof(item.szName), pRecord->szBuyerName);
	strcpy_s(item.szSellerName, sizeof(item.szSellerName), pRecord->szSellerName);

	if(IsEnd == IS_END_ALL_RIGHT)
	{		// 이놈은 파는놈이 취소하는 놈이다

		pRecord->iKey = 1;		// 판매자면 1이다
		int a=0,b=0,c=0;
		if(::SearchInv(ch->inv,a,b,c))//공간이 있구나
		{
			POS pos;
			SetItemPos(INV,a,b,c,&pos);//pos셋팅 
			ITEMATTR *pItem = ::GetItemByPOS(ch,pos);//아이템 주소 받고
			(*pItem) = pRecord->SellItem;
			::SendServerEachItem( &pos, pItem,iUserCn);//보내기
			::SendItemEventLog( pItem, ch->GetServerID(), 0, SILT_TAKE_AUCTION_RESULT_ITEM, 1 );
			pRecord->iKey += 100;
			// 성공이니 로그를 남기자
			item.iKey = pRecord->iKey;
			g_pLogManager->SaveAutionLog(ACLT_SELLER_CANCLE, &(item));	
			
		}
		else// 인벤에 자리없다
		{
			::OutMessage(ch,2,13);//임시,,인벤토리 공간이 충분치 않습니다.
			pRecord->iKey += 10;
		}			
	}
	else if(IsEnd == IS_END_WAIT_TAKE)
	{
		// 이놈은 거래후 물건이나 대금을 찾는놈이다.
		if(strcmp(pRecord->szMyName, pRecord->szSellerName))
		{	// Buyer이니 물건을 지급해주자
			pRecord->iKey = 0;		// 구매자면 0이다
			int a=0,b=0,c=0;
			if(::SearchInv(ch->inv,a,b,c))//공간이 있구나
			{
				POS pos;
				SetItemPos(INV,a,b,c,&pos);//pos셋팅 
				ITEMATTR *pItem = ::GetItemByPOS(ch,pos);//아이템 주소 받고
				(*pItem) = pRecord->SellItem;
				::SendServerEachItem( &pos, pItem,iUserCn);//보내기
				::SendItemEventLog( pItem, ch->GetServerID(), 0, SILT_TAKE_AUCTION_RESULT_ITEM, 1 );

				pRecord->iKey += 100;
				// 성공이니 로그를 남기자
				item.iKey = pRecord->iKey;
				g_pLogManager->SaveAutionLog(ACLT_BUYER_TAKE, &(item));	
			}
			else// 인벤에 자리없다
			{
				::OutMessage(ch,2,13);//임시,,인벤토리 공간이 충분치 않습니다.
				pRecord->iKey += 10;
			}			
		}
		else
		{	// Seller이니 대금을 지급해주자
			pRecord->iKey = 1;		// 판매자면 1이다
			const DWORD dwSellValue = p.u.MerchantResultTake.dwSellValue;
			if(::GiveMoney(ch,dwSellValue))
			{//지급성공

				pRecord->iKey += 100;
				// 성공이니 로그를 남기자
				item.iKey = pRecord->iKey;
				g_pLogManager->SaveAutionLog(ACLT_SELLER_TAKE, &(item));	
			}
			else
			{
				::OutMessage(ch,2,13);//임시,,인벤토리 공간이 충분치 않습니다.
				pRecord->iKey += 10;
			}		
		}
	}
	p.h.header.type = CMD_MERCHANT_BACKUP_TAKE_RESPONSE;//타입 전환
	::QueuePacket(connections,DB_DEMON,&p,1);//디비에게 보냅니다.
}
//> BBD 040303		머천트백업 테이블로을 사용하는 함수군
