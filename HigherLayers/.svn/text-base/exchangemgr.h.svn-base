// ExchangeMgr.h: interface for the CExchangeMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXCHANGEMGR_H__854CDDBC_E149_4C5C_9842_C8447030B821__INCLUDED_)
#define AFX_EXCHANGEMGR_H__854CDDBC_E149_4C5C_9842_C8447030B821__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const int MAX_EXCHANGE_ITEM = 20;//절대 20으로 놔둘것

enum ExchangeStateType
{
	EXCHANGE_STATE_READY = 0,//거래 대기 상태
	EXCHANGE_STATE_EXCHANGE_NOW_SIGN_NO = 2,	//거래중 상태 GetExchangeState에서 씀
	EXCHANGE_STATE_EXCHANGE_NOW_SIGN_YES= 3,	//거래중 상태 GetExchangeState에서 씀
};

enum ExchangeType
{
	EXCHANGE_TYPE_NO = 0,//거래아님 
	EXCHANGE_TYPE_NORMAL = 1,		//보통 거래
	EXCHANGE_TYPE_MERCHANT_DIRECT = 2,	//상인과 직접 거래
};

class CExchangeMgr  
{
public:
	CExchangeMgr();
	virtual ~CExchangeMgr();

};

extern void SaveChangeBankMoney(const int idTarget, const DWORD nOldBankMoney, const DWORD nNewBankMoney , const eBankMoneyChangeType BMCT); // CSD-040324

#endif // !defined(AFX_EXCHANGEMGR_H__854CDDBC_E149_4C5C_9842_C8447030B821__INCLUDED_)
