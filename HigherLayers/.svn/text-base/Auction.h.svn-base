// Auction.h: interface for the CAuction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUCTION_H__65C7ED7E_9020_467F_AB76_8528EE975EC8__INCLUDED_)
#define AFX_AUCTION_H__65C7ED7E_9020_467F_AB76_8528EE975EC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Dr_Network.h"

enum BondMoneyNo
{
	BOND_MONEY_1000 = 10219,	//10억 크릿 본드	
	BOND_MONEY_500	= 10220,	//5억 크릿 본드	
	BOND_MONEY_100	= 10221,	//1억 크릿 본드	
	BOND_MONEY_50	= 10222,	//5천만 크릿 본드	
	BOND_MONEY_10	= 10223,	//1천만 크릿 본드	
	BOND_MONEY_5	= 10224,	//5백만 크릿 본드	
};

typedef struct SUPERMONEY
{
	int iItemNo;
	int iValue; 
	SUPERMONEY(const int iInput,const int iInput2)
	{
		iItemNo =iInput;
		iValue	=iInput2;
	}
}t_SuperMoney,*lpSUPERMONEY;

const int BOND_MONEY_MAX = 6;
extern const SUPERMONEY aBondMoney[BOND_MONEY_MAX];

enum IsEndType
{
	IS_END_ALL_RIGHT	= 0,	//뭐든 할 수 있는 상태
	IS_END_DELETING		= 1,	//삭제중
	IS_END_BUYING		= 3,	//구매중
	IS_END_WAIT_TAKE	= 4	,	//가져가기를 바라는 상태
	IS_END_DELETE_COMPLETE = 5,	// BBD 040303
	IS_END_GIVING		= 6,	// BBD 040303 지급프로세스에 들어가있다
};

const int AUCTION_CANCEL_MONEY = 100000;
class CAuction  
{
	enum ItemType
	{
		NORMAL_ITEM	= 0,
		RARE_ITEM	= 1,
		HIGH_ITEM	= 2,
		DYNAMIC_RARE_ITEM	= 3,
		SET_ITEM	= 4
	};

	enum ItemLevelType
	{
		LVMIN		= 0,
		LV10TO20	= 1,
		LV20TO30	= 2,
		LV30TO40	= 3,
		LV40TO50	= 4,
		LV50TO60	= 5,
		LV60TO70	= 6,
		LV70TO80	= 7,
		LV80TO90	= 8,
		LV90TO100	= 9,
		LV100TO110	= 10,
		LV110TO120	= 11,
		LV120TO130	= 12,
		LV130TO140	= 13,
		LV140TO150	= 14,
		LVMAX		= 15
	};
	
public:
	CAuction();
	virtual ~CAuction();
private://검색결과물
	void Clear();
public:
//---->구매자용
	void RecvCMD_MERCHANT_BUY_LIST_REQUEST(const int cn,t_packet &p);//리스트 요청(구매자가 보는것//검색기가 호출
	void RecvCMD_MERCHANT_BUY_LIST_REQUEST_RESULT(t_packet &p);//리스트 결과(구매자가 보는것(DB_DEMON이 보내오는것을 클라이언트로 전송

	void RecvCMD_MERCHANT_ITEM_BUY(const int cn,t_packet &p);//구매하기(구매자가 선택
	void RecvCMD_MERCHANT_ITEM_BUY_COMFORM_RESULT(const int iCn,t_packet &p);
	void RecvCMD_MERCHANT_ITEM_BUY_RESULT(t_packet &p);//구매하기(구매자가 선택
	void SendCMD_MERCHANT_ITEM_BUY_COMFORM_RESULT(CHARLIST *ch,const int iIndex, const ITEMATTR &Item, const bool bSuccess);
//<----
//---->판매자용
	void RecvCMD_MERCHANT_SELL_LIST_REQUEST(const int cn,t_packet &p);//리스트 요청(판매자가 보는것
	void RecvCMD_MERCHANT_SELL_LIST_REQUEST_RESULT(t_packet &p);//리스트 결과(판매자가 보는것//디비데몬에게만 받음

	void RecvCMD_MERCHANT_SELL_ITEM_DELETE(const int cn,t_packet &p);//판매 아이템을 삭제//보낸사람 클라이언트(C)
	void RecvCMD_MERCHANT_SELL_ITEM_DELETE_COMFORM_RESULT(const int cn,t_packet &p);//판매 아이템을 삭제//보낸사람 디비(D)
	void RecvCMD_MERCHANT_SELL_ITEM_DELETE_RESULT(const int cn,t_packet &p);//판매 아이템을 삭제//보낸사람 디비(D)
	
	void RecvCMD_MERCHANT_SELL_ITEM_REGISTER(const int iCn, t_packet &p);//판매 아이템을 등록//C
	void RecvCMD_MERCHANT_SELL_ITEM_REGISTER_COMFORM_RESULT(const int iCn, t_packet &p);//D
	void RecvCMD_MERCHANT_SELL_ITEM_REGISTER_RESULT(t_packet &p);//판매 아이템을 등록//D
//<----
//---->거래결과
	//리스트 검색
	void RecvCMD_MERCHANT_RESULT_LIST_REQUEST(const int cn,t_packet &p);
	void RecvCMD_MERCHANT_RESULT_LIST_REQUEST_RESULT(t_packet &p);

	void RecvCMD_MERCHANT_RESULT_TAKE(const int cn,t_packet &p);
	void RecvCMD_MERCHANT_RESULT_TAKE_COMFORM_RESULT(const int cn,t_packet &p);
	void RecvCMD_MERCHANT_RESULT_TAKE_RESULT(t_packet &p);
//<----
//---->개인적 아이템 구매 리스트
	void RecvCMD_MERCHANT_DIRECT_EXCHANGE_LIST_REQUSET(const int cn,t_packet &p);
	void RecvCMD_MERCHANT_DIRECT_EXCHANGE_LIST_REQUSET_RESULT(t_packet &p);
//<----
	void RecvCMD_EXCHANGE_BOND_MONEY(const int cn,t_packet &p);
//<! BBD 040303		머천트백업 테이블로을 사용하는 함수군
	void RecvCMD_MERCHANT_BACKUP_TAKE_REQUEST(const int cn,t_packet &p);

//> BBD 040303		머천트백업 테이블로을 사용하는 함수군

};
extern CAuction Auction;
#endif // !defined(AFX_AUCTION_H__65C7ED7E_9020_467F_AB76_8528EE975EC8__INCLUDED_)