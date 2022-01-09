// LottoSystem.h: interface for the CLottoSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOTTOSYSTEM_H__42D51FB0_EFA1_4F33_AC72_242893C6C971__INCLUDED_)
#define AFX_LOTTOSYSTEM_H__42D51FB0_EFA1_4F33_AC72_242893C6C971__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
using namespace	std;

typedef struct tag_Lottery_Event_Time
{
	DATECHECK	stLotteryStart;
	DATECHECK	stLotteryEnd;
}stLOTTERY_DATE;

typedef vector<stLOTTERY_DATE>	VT_LOTTERY_DATE;
typedef vector<stLOTTERY_DATE>::iterator	ITER_LOTTERY_DATE;
class CLottoSystem
{
	enum
	{
		EVENT = 1,//사람들이 응모하길 기다리고 있다.
		LOTTERIED = 2,//추첨이 되었다.상품을 주어야지.
	};

	DATECHECK		m_LotteryStart;
	DATECHECK		m_LotteryEnd;

	VT_LOTTERY_DATE		m_vtLottery_date;
		
	int		m_EventStatus;
	static	CLottoSystem*	m_pClass;
	int		m_anNumbers[10];
	LOTTO_EVENT_INFO	m_Lotto_Info;
	bool	m_bGenNumbers;
	bool	m_bWinnerChecking;		// BBD 040127	로또 확인중 플래그
public:
	void CheckIsDelOk(bool bIsOk);	// BBD 040127	로또 확인절차가 끝났을때 호출
	int FindSameDayIndex(int nDay);
	int GetNextLotteryIndex();
	int GetLottoID();
	void BroadCastLotteryMsg();
	DWORD m_dwBroadCastTime;
	void RecvOpenLottoMenu(int cn);
	int m_nLottoPay;
	int m_anWinNumberCount4Grade[5];
	int	m_anGiveItemNumber[5];//등수별 아이템 번호.
	int m_anItemCount4Grade[5];
	int m_nLottoNumberCount;
//	int m_nGiveItemNumber;
	void RecvCheckOpenWinnerMenu(t_WINNER_MENU* pWinnerMenu);
	void RecvOpenWinnerMenu(int cn);
	void RecvLottoInfo(LOTTO_EVENT_INFO* pLottoInfo);
	void RecvCheckOKWinner(t_CHECK_WINNER* pCheckOK);
	void RecvCheckWinner(int cn);
	void RecvCanBuyLotto(t_BUY_LOTTO* pCanBuyLotto);
	void RecvBuyLotto(t_BUY_LOTTO* pBuyLotto);
	void RecvDBNewEvent(LOTTO_EVENT_INFO* pInfo);
	void SendDBNewEvent();
	void SendDBLottery();
	tm GetCurDate();//soto-LottoADD
	void RunProc();
	void Clear();
	bool LoadTable(HDBC hDragonDB);
	int GetMinNumber(int aNumbers[],int size);
	void Sort(int aNubers[],int size);
	int SetLottoID(int nID);
	int Check6Number(int an6Numbers[]);
	void Generate6Number();
	bool IsMember(int nNumber);
	static	CLottoSystem*	GetClass(){return m_pClass;}
	static	bool	Create();
	static	void	Destroy()	{if(m_pClass){delete m_pClass;m_pClass = NULL;}}
		
	
	CLottoSystem();
	virtual ~CLottoSystem();

private:
	int m_nRunID;
	int RandomNumber();
	int CreateOneNumber();
};
extern inline CLottoSystem*	LottoSystem(){return CLottoSystem::GetClass();}


#endif // !defined(AFX_LOTTOSYSTEM_H__42D51FB0_EFA1_4F33_AC72_242893C6C971__INCLUDED_)