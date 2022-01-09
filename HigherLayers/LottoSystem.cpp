// LottoSystem.cpp: implementation of the CLottoSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "LottoSystem.h"
#include "..\LowerLayers\servertable.h"
#include "CItem.h"
#include "UserManager.h"
#include <direct.h>

#define		ARENA_MAP_PORT	5570
#define		BROADCAST_TIME	1200000 //방송 시간은 20분 간격이다.
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CLottoSystem* CLottoSystem::m_pClass = NULL;
CLottoSystem::CLottoSystem()
{
	Clear();
	srand(timeGetTime());
	m_pClass = this;

	m_nLottoNumberCount = 4;// 이값은 테이블에서 읽어 온다.
//	m_nGiveItemNumber = SADONIX_NO;//기본은 사드 오닉스를 준다. 하지만 테이블에서 읽어온다.
	memset(m_anItemCount4Grade,0,sizeof(int)*5);//등수별로 지급되는 아이템 갯수.
	memset(m_anWinNumberCount4Grade,0,sizeof(int)*5);//몇개의 번호가 맞아야 몇등인가..
	memset(m_anGiveItemNumber,0,sizeof(int)*5);//등수별로 지급되는 아이템 번호.
	m_nLottoPay = 10000; //디폴트는 10000 크릿이다.//디비에 저장 되어 있는 값을 로드해야 한다.

	memset(&m_LotteryStart,0,sizeof(DATECHECK));
	memset(&m_LotteryEnd,0,sizeof(DATECHECK));

	_mkdir("LotteryItem");
	m_bWinnerChecking = false;		// BBD 040127	로또 확인중인지의 플래그
}

CLottoSystem::~CLottoSystem()
{
	Clear();
}

//복권 시스템은 콜로서스 아레나에서 한번만 만들어져야 한다.
bool CLottoSystem::Create()
{
	if(m_pClass || ::g_pServerTable->GetOwnServerData()->wPort != ARENA_MAP_PORT )return false;
	new CLottoSystem;
	return true;
}

int CLottoSystem::CreateOneNumber()
{
	int nNumber = 0;
	while(1)
	{
		nNumber = RandomNumber();
		if(!IsMember(nNumber))break;
	}
	
	return nNumber;
}

int CLottoSystem::RandomNumber()
{
	int	nNumber = rand()%MAX_MAXIMUM_NUMBER + 1;
	return nNumber;
}

bool CLottoSystem::IsMember(int nNumber)
{
	for(int i = 0;i < 6;i++)
	{
		if(m_anNumbers[i] == nNumber) return true;
	}

	return false;
}

void CLottoSystem::Generate6Number()
{
	for(int i = 0;i < m_nLottoNumberCount;i++)
	{
		m_anNumbers[i] = CreateOneNumber();
	}

	Sort(m_anNumbers,m_nLottoNumberCount);

	m_bGenNumbers = true; // 번호를 한번 만들었다.
}




int CLottoSystem::Check6Number(int an6Numbers[])
{
	int nCount = 0;
	for(int i = 0; i < m_nLottoNumberCount;i++)
	{
		if(IsMember(an6Numbers[i]))
			nCount++;
	}

	return nCount;
}


int CLottoSystem::SetLottoID(int nID)
{
	m_nRunID = nID; //1이상이 들어가야 로또가 진행되고 있는중이다.

	return m_nRunID;
}


void CLottoSystem::Sort(int aNumbers[], int size)
{
	if(size > 0)
	{
		int nMin = this->GetMinNumber(aNumbers,size);
		int i;
		for(i = 0;i < size;i++)
		{
			if(nMin == aNumbers[i]) break;			
		}

		int nTemp = aNumbers[i];
		aNumbers[i] = aNumbers[0];
		aNumbers[0] = nTemp;
		Sort(aNumbers + 1,size - 1);
	}
}

int CLottoSystem::GetMinNumber(int aNumbers[], int size)
{
	int nMin = 46;
	for(int i = 0;i < size;i++)
	{
		nMin = __min(aNumbers[i],nMin);
	}

	return nMin;
}

bool CLottoSystem::LoadTable(HDBC hDragonDB)
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
	char		szQuerry[512];

	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT * FROM Lotto_Setting");
	SQLAllocStmt(hDragonDB, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);

	if (!SQLOK(retCode))
	{
		MyLog(0,"Lotto_Setting Table ....Loading Error!!!!");
		SQLFreeStmt(hStmt, SQL_DROP);
		return 0;
	}
	retCode = SQLFetch(hStmt);
	if (!SQLOK(retCode))
	{
		MyLog(0,"Lotto_Setting Table ....No Data!!!!");
		SQLFreeStmt(hStmt, SQL_DROP);
		return 0;
	}
	SQLGetData(hStmt,1,SQL_C_LONG,&m_nLottoNumberCount,0,&cbValue);

	SQLGetData(hStmt,2,SQL_C_LONG,&m_anGiveItemNumber[0],0,&cbValue);
	SQLGetData(hStmt,3,SQL_C_LONG,&m_anGiveItemNumber[1],0,&cbValue);
	SQLGetData(hStmt,4,SQL_C_LONG,&m_anGiveItemNumber[2],0,&cbValue);
	SQLGetData(hStmt,5,SQL_C_LONG,&m_anGiveItemNumber[3],0,&cbValue);
	SQLGetData(hStmt,6,SQL_C_LONG,&m_anGiveItemNumber[4],0,&cbValue);

	SQLGetData(hStmt,7,SQL_C_LONG,&m_anItemCount4Grade[0],0,&cbValue);
	SQLGetData(hStmt,8,SQL_C_LONG,&m_anItemCount4Grade[1],0,&cbValue);
	SQLGetData(hStmt,9,SQL_C_LONG,&m_anItemCount4Grade[2],0,&cbValue);
	SQLGetData(hStmt,10,SQL_C_LONG,&m_anItemCount4Grade[3],0,&cbValue);
	SQLGetData(hStmt,11,SQL_C_LONG,&m_anItemCount4Grade[4],0,&cbValue);

	SQLGetData(hStmt,12,SQL_C_LONG,&m_anWinNumberCount4Grade[0],0,&cbValue);
	SQLGetData(hStmt,13,SQL_C_LONG,&m_anWinNumberCount4Grade[1],0,&cbValue);
	SQLGetData(hStmt,14,SQL_C_LONG,&m_anWinNumberCount4Grade[2],0,&cbValue);
	SQLGetData(hStmt,15,SQL_C_LONG,&m_anWinNumberCount4Grade[3],0,&cbValue);
	SQLGetData(hStmt,16,SQL_C_LONG,&m_anWinNumberCount4Grade[4],0,&cbValue);

	SQLGetData(hStmt,17,SQL_C_LONG,&m_nLottoPay,0,&cbValue);


	SQLFreeStmt(hStmt, SQL_DROP);


	

	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT * FROM Lottery_Date");
	SQLAllocStmt(hDragonDB, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);

	if (!SQLOK(retCode))
	{
		MyLog(0,"Lottery_Date Table ....Loading Error!!!!");
		SQLFreeStmt(hStmt, SQL_DROP);
		return 0;
	}	
	retCode = SQLFetch(hStmt);
	if (!SQLOK(retCode))
	{
		MyLog(0,"Lottery_Date Table ....No Data!!!!");		
		SQLFreeStmt(hStmt, SQL_DROP);
		return 0;
	}
	
	while(SQLOK(retCode))
	{
		SQLGetData(hStmt,2,SQL_C_LONG,&m_LotteryStart.nDay,0,&cbValue);
		SQLGetData(hStmt,3,SQL_C_LONG,&m_LotteryStart.nHour,0,&cbValue);
		SQLGetData(hStmt,4,SQL_C_LONG,&m_LotteryStart.nMin,0,&cbValue);
		SQLGetData(hStmt,5,SQL_C_LONG,&m_LotteryEnd.nDay,0,&cbValue);
		SQLGetData(hStmt,6,SQL_C_LONG,&m_LotteryEnd.nHour,0,&cbValue);
		SQLGetData(hStmt,7,SQL_C_LONG,&m_LotteryEnd.nMin,0,&cbValue);

		stLOTTERY_DATE	Lottery_date;
		Lottery_date.stLotteryStart = m_LotteryStart;
		Lottery_date.stLotteryEnd	= m_LotteryEnd;

		this->m_vtLottery_date.push_back(Lottery_date);
				
		retCode = SQLFetch(hStmt);
	}

	SQLFreeStmt(hStmt, SQL_DROP);



	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT * FROM Lotto_Event");
	SQLAllocStmt(hDragonDB, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);

	if (!SQLOK(retCode))
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return 0;
	}	
	
	retCode = SQLFetch(hStmt);

	LOTTO_EVENT_INFO	Lotto_Info;
	memset(&Lotto_Info,0,sizeof(Lotto_Info));

	int nLine = 1;

	if(!SQLOK(retCode))//테이블에 내용이 없다.헉. 만들어야지.
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		MyLog(0,"CLottoSystem::LoadTable() No data!!!!!.....Please Make One Data");
		return true;
	}

	while (SQLOK(retCode))
	{		
		memset(&m_Lotto_Info,0,sizeof(m_Lotto_Info));
		
		SQLGetData(hStmt, 1, SQL_C_LONG, &m_Lotto_Info.nLottoID, 0, &cbValue);//LottoID 마지막 회차의 로또 아이디가 로드 된다.
		SQLGetData(hStmt, 2, SQL_C_LONG, &m_Lotto_Info.StartDate.tm_year, 0, &cbValue);
		SQLGetData(hStmt, 3, SQL_C_LONG, &m_Lotto_Info.StartDate.tm_mon, 0, &cbValue);
		SQLGetData(hStmt, 4, SQL_C_LONG, &m_Lotto_Info.StartDate.tm_wday, 0, &cbValue);
		SQLGetData(hStmt, 5, SQL_C_LONG, &m_Lotto_Info.LotteryDate.tm_year, 0, &cbValue);
		SQLGetData(hStmt, 6, SQL_C_LONG, &m_Lotto_Info.LotteryDate.tm_mon, 0, &cbValue);
		SQLGetData(hStmt, 7, SQL_C_LONG, &m_Lotto_Info.LotteryDate.tm_wday, 0, &cbValue);
		SQLGetData(hStmt, 8, SQL_C_LONG, &m_Lotto_Info.nWinNumCount, 0, &cbValue);

		SQLGetData(hStmt, 9, SQL_C_LONG, &m_Lotto_Info.anWinNumbers[0], 0, &cbValue);
		SQLGetData(hStmt, 10, SQL_C_LONG, &m_Lotto_Info.anWinNumbers[1], 0, &cbValue);
		SQLGetData(hStmt, 11, SQL_C_LONG, &m_Lotto_Info.anWinNumbers[2], 0, &cbValue);
		SQLGetData(hStmt, 12, SQL_C_LONG, &m_Lotto_Info.anWinNumbers[3], 0, &cbValue);
		SQLGetData(hStmt, 13, SQL_C_LONG, &m_Lotto_Info.anWinNumbers[4], 0, &cbValue);
		SQLGetData(hStmt, 14, SQL_C_LONG, &m_Lotto_Info.anWinNumbers[5], 0, &cbValue);
		SQLGetData(hStmt, 15, SQL_C_LONG, &m_Lotto_Info.anWinNumbers[6], 0, &cbValue);
		SQLGetData(hStmt, 16, SQL_C_LONG, &m_Lotto_Info.anWinNumbers[7], 0, &cbValue);
		SQLGetData(hStmt, 17, SQL_C_LONG, &m_Lotto_Info.anWinNumbers[8], 0, &cbValue);
		SQLGetData(hStmt, 18, SQL_C_LONG, &m_Lotto_Info.anWinNumbers[9], 0, &cbValue);
	
		retCode = SQLFetch(hStmt);		
	}
	
	
	SQLFreeStmt(hStmt, SQL_DROP);
	

	return true;	
}

void CLottoSystem::Clear()
{
	memset(&m_Lotto_Info,0,sizeof(m_Lotto_Info));
	memset(m_anNumbers,0,sizeof(int)*10);
	m_pClass = NULL;
	m_EventStatus = EVENT;
	m_bGenNumbers = false;
	m_dwBroadCastTime = 0;//soto-HK030924
}

//< soto_LottoADD
void CLottoSystem::RunProc()
{
	tm	CurDate = GetCurDate();

	if(m_EventStatus == LOTTERIED)//추첨이 된기간이다.
	{
		if(timeGetTime() - m_dwBroadCastTime >= BROADCAST_TIME)
		{
			m_dwBroadCastTime = timeGetTime();
			BroadCastLotteryMsg();//메세지를 전파 하자.
		}

		int nEventCount = this->m_vtLottery_date.size();
		for(int i = 0; i < nEventCount;++i)
		{
			DATECHECK LotteryEnd = this->m_vtLottery_date[i].stLotteryEnd;

			if(CurDate.tm_wday == LotteryEnd.nDay)//추첨 끝날 일이랑 같으면.
			{
				if(CurDate.tm_hour == LotteryEnd.nHour)//추첨 끝날 시간이랑 같을때.
				{
					if(CurDate.tm_min == LotteryEnd.nMin)//새 이벤트의 때가 되었다.
					{
						SendDBNewEvent();//새 이벤트를 진행하자.
						break;
					}				
				}
			}
		}
	}
	else//복권 등록 기간이다.
	{
		int nEventCount = this->m_vtLottery_date.size();
		
		for(int i = 0; i < nEventCount;++i)
		{
			DATECHECK LotteryStart = this->m_vtLottery_date[i].stLotteryStart;
			DATECHECK LotteryEnd = this->m_vtLottery_date[i].stLotteryEnd;		// BBD 040211 추첨종료시간
			if(CurDate.tm_wday == LotteryStart.nDay)//추첨 시작일이랑 같으면.
			{
				if(CurDate.tm_hour >= LotteryStart.nHour &&
					CurDate.tm_hour <= LotteryEnd.nHour)	// BBD 040211 추첨기간 사이(시간) 이면
				{
					if(CurDate.tm_min >= LotteryStart.nMin &&
						CurDate.tm_min < LotteryEnd.nMin)	// BBD 040211 추첨기간 사이(분) 이면
					{
						if(m_Lotto_Info.nLottoID)
						{
							SendDBLottery();//추첨하자.
							break;
						}
					}
				}
			}
		}
	}
}

tm CLottoSystem::GetCurDate()
{
	tm	Time;
	time_t	LongTime;
	time(&LongTime);
	Time = *localtime(&LongTime);

	return Time;
}


void CLottoSystem::SendDBLottery()
{
	t_packet p; 
	p.h.header.type = CMD_LOTTERY_INFO;
	p.h.header.size = sizeof(LOTTO_EVENT_INFO);

	LOTTO_EVENT_INFO	Info;
	memcpy(&Info,&m_Lotto_Info,sizeof(LOTTO_EVENT_INFO));
	
	if(!m_bGenNumbers)Generate6Number(); //한번만 만든다.

	Info.nWinNumCount = m_nLottoNumberCount;
	memcpy(&Info.anWinNumbers,&m_anNumbers,sizeof(int)*this->m_nLottoNumberCount);
	Info.LotteryDate = GetCurDate();
			
	memcpy(&p.u.Lotto_Info,&Info,sizeof(LOTTO_EVENT_INFO));

	QueuePacket(connections,DB_DEMON,&p,1);
}

void CLottoSystem::SendDBNewEvent()
{
	t_packet	p;

	tm	Date = GetCurDate();


	Date.tm_year = Date.tm_year + 1900;//soto-LottoADD
		
	LOTTO_EVENT_INFO	Lotto_Info;memset(&Lotto_Info,0,sizeof(Lotto_Info));

	Lotto_Info.nLottoID = m_Lotto_Info.nLottoID + 1;
	memcpy(&Lotto_Info.StartDate,&Date,sizeof(tm));

	p.h.header.type = CMD_NEW_EVENT;
	p.h.header.size = sizeof(Lotto_Info);

	memcpy(&p.u.Lotto_Info,&Lotto_Info,sizeof(Lotto_Info));
	
	QueuePacket(connections,DB_DEMON,&p,1);

	MyLog(0,"Lotto : NewEvent Start!!!!  EventNumber : '%d' ",Lotto_Info.nLottoID);
}

void CLottoSystem::RecvDBNewEvent(LOTTO_EVENT_INFO* pInfo)
{
	m_EventStatus = EVENT;
	m_bGenNumbers = false;
	memcpy(&m_Lotto_Info,pInfo,sizeof(LOTTO_EVENT_INFO));
}

void CLottoSystem::RecvBuyLotto(t_BUY_LOTTO *pBuyLotto)
{
	int cn = ExistHe(pBuyLotto->strCharName);
		
	if(cn)
	{
		t_packet	p;
		p.h.header.type = CMD_LOTTO_BUY;
		p.h.header.size = sizeof(t_BUY_LOTTO);

		CHARLIST*	pChar = ::CheckServerId(cn);
		if(pChar)
		{
//			tm Date = GetCurDate();
			t_BUY_LOTTO	BuyLotto; memcpy(&BuyLotto,pBuyLotto,sizeof(BuyLotto));

			if(m_EventStatus == LOTTERIED)//당첨 완료 기간이다.
			{
				memcpy(&p.u.Lotto_Buy,pBuyLotto,sizeof(t_BUY_LOTTO));
				p.u.Lotto_Buy.anLottoNumber[0] = -1;//번호의 맨앞이 -1이면 
				QueuePacket(connections,cn,&p,1);
				return;
			}
			else
			{
				if(LocalMgr.IsAbleNation(TAIWAN | HONGKONG | CHINA))//과금 문제로 무조건 DBDEMON으로 유청을 한다.
				{
					
					t_packet p2;
					p2.h.header.type = CMD_CAN_BUY;
					p2.h.header.size = sizeof(t_BUY_LOTTO);
					
					pBuyLotto->nLottoID = this->m_Lotto_Info.nLottoID;//soto-LottoADD

					memcpy(&p2.u.Lotto_Buy,pBuyLotto,sizeof(t_BUY_LOTTO));
					

					QueuePacket(connections,DB_DEMON,&p2,1);
				}
				else
				{
					if(pChar->GetBankMoney() < m_nLottoPay)//돈이 모자라다.
					{
						memcpy(&p.u.Lotto_Buy,pBuyLotto,sizeof(t_BUY_LOTTO));
						p.u.Lotto_Buy.anLottoNumber[0] = 0;//번호의 맨앞이 0이면 돈이 모자라다.
						QueuePacket(connections,cn,&p,1);
					}
					else//살수 있는 조건이 충족 되었다. 마지막으로 몇장을 샀고 중복 번호가 있는지 확인 해야 한다.
					{
						//여기서만 DB_DEMON으로 확인 요청을 한다.
						t_packet p2;
						p2.h.header.type = CMD_CAN_BUY;
						p2.h.header.size = sizeof(t_BUY_LOTTO);
						
						pBuyLotto->nLottoID = this->m_Lotto_Info.nLottoID;//soto-LottoADD

						memcpy(&p2.u.Lotto_Buy,pBuyLotto,sizeof(t_BUY_LOTTO));
						

						QueuePacket(connections,DB_DEMON,&p2,1);
					}
				}
			}
		}
	}	
}

void CLottoSystem::RecvCanBuyLotto(t_BUY_LOTTO *pCanBuyLotto)
{
	int cn = ExistHe(pCanBuyLotto->strCharName);
	
	if(cn)
	{
		CHARLIST *pChar = ::CheckServerId(cn);
		if(pChar)
		{
			t_packet	p;
			p.h.header.type = CMD_LOTTO_BUY;
			p.h.header.size = sizeof(t_BUY_LOTTO);
			
			memcpy(&p.u.Lotto_Buy,pCanBuyLotto,sizeof(t_BUY_LOTTO));

			if(p.u.Lotto_Buy.anLottoNumber[0] > 0)//살수 있다.//캐릭터와DB_DEMON 에게 동시에 보낸다.
			{
				p.u.Lotto_Buy.m_nLottoPay = m_nLottoPay;
				
				if(LocalMgr.IsAbleNation(KOREA | CHINA | THAI | HONGKONG | USA | JAPAN))//soto-LottoADD 대만은 포인트로 한다.
				{									
					pChar->DecBankMoney(m_nLottoPay, BMCT_BUY_LOTTO); // CSD-030723
				}
				else
				{
					
				}

				QueuePacket(connections,cn,&p,1);
				QueuePacket(connections,DB_DEMON,&p,1);
			}
			else
			{
				QueuePacket(connections,cn,&p,1);//캐릭터에게만 보낸다.
			}
		}
	}
}

void CLottoSystem::RecvCheckWinner(int cn)
{

	if(m_EventStatus == LOTTERIED)
	{
		//<!	BBD 040127	로또 프로세스 변경
		if(m_bWinnerChecking)
		{
			// 로또 채킹중이다 기둘리라는 시그널을 보내세
			t_packet	p;
			p.h.header.type = CMD_WINNER_CHECK;
			p.h.header.size = sizeof(t_CHECK_WINNER);

			memset(&p.u.Check_Winner,0,sizeof(t_CHECK_WINNER));

			p.u.Check_Winner.nWinItemCount = -10;	//	<--- 이넘이 채킹중임을 표시한다

			QueuePacket(connections,cn,&p,1);	//클라이언트에게..채킹중이라는 메세지.
			return;
		}
		//>		BBD 040127	로또 프로세스 변경
		
		t_packet	p;
		p.h.header.type = CMD_WINNER_CHECK;
		p.h.header.size = sizeof(t_CHECK_WINNER);

		memset(&p.u.Check_Winner,0,sizeof(t_CHECK_WINNER));

		p.u.Check_Winner.nLottoID = m_Lotto_Info.nLottoID;//soto-LottoADD
		strcpy_s(p.u.Check_Winner.strCharName, sizeof(p.u.Check_Winner.strCharName),::CheckServerId(cn)->Name);
		memcpy(p.u.Check_Winner.anWinNumbers,m_Lotto_Info.anWinNumbers,sizeof(int)*10);
		

		QueuePacket(connections,DB_DEMON,&p,1);//데몬에다...확인 요청을 한다.

		//<! BBD 040127	로또 블럭 로그 남김
		m_bWinnerChecking = true;	// true일때 블럭중이다
		MyLog(LOG_NORMAL, "Start Checking Lotto now UserID = %s",
			p.u.Check_Winner.strCharName);
		//> BBD 040127	로또 블럭 로그 남김
	}
	else
	{
		t_packet	p;
		p.h.header.type = CMD_WINNER_CHECK;
		p.h.header.size = sizeof(t_CHECK_WINNER);

		memset(&p.u.Check_Winner,0,sizeof(t_CHECK_WINNER));

		strcpy_s(p.u.Check_Winner.strCharName, sizeof(p.u.Check_Winner.strCharName),::CheckServerId(cn)->Name);
		memset(p.u.Check_Winner.anWinNumbers,0,sizeof(int)*10);
		p.u.Check_Winner.nWinItemCount = -3;

//		p.u.Check_Winner.anWinNumbers[0]
		QueuePacket(connections,cn,&p,1);//클라이언트에게..추첨 기간이 아니라는 메세지.


	}
}

void CLottoSystem::RecvCheckOKWinner(t_CHECK_WINNER *pCheckOK)
{
	int cn = ::ExistHe(pCheckOK->strCharName);
	if(cn)
	{
		CHARLIST*	pChar = ::CheckServerId(cn);
		if (pChar == NULL) return; // finito null check

		t_packet	p;
		p.h.header.type = CMD_WINNER_CHECK;
		p.h.header.size = sizeof(t_CHECK_WINNER);
		memcpy(&p.u.Check_Winner,pCheckOK,sizeof(t_CHECK_WINNER));

		if(pCheckOK->nWinItemCount > 0)//일단은 당첨 되었기는 한데.
		{
		//캐릭터의 아이템 창고의 빈공간을 확인할필요가 있다. 충분하면. +수 넣어 준다. 충분하지 않으면 -1
			int nBlank = 0;
			for(int i = 0;i < 5;i++)
				for(int j = 0; j < 3;j++)
					for(int k = 0;k < 6;k++)
						if(!pChar->bank[i][j][k].item_no) nBlank++;
			
			int	nNeedBlank = pCheckOK->nWinItemCount;
			if(nBlank >= pCheckOK->nWinItemCount )
			{
				QueuePacket(connections,cn,&p,1);//아이템을 만들었다.
				//아이템을 만들어 주어야 한다.

				int nItemCounter = pCheckOK->nWinItemCount;
								
				//로그를 남긴다.
				for(int nGradeItemIndex = 0;nGradeItemIndex < 5;++nGradeItemIndex)
				{
					int nGradeItemCount = 0;
					ItemAttr	Item = ::GenerateItem(m_anGiveItemNumber[nGradeItemIndex]);
					if(nGradeItemCount = pCheckOK->anWinItemsCount[nGradeItemIndex])
					{
						for(int i = 0;i < 5;i++)
						{
							for(int j = 0; j < 3;j++)
							{
								for(int k = 0;k < 6;k++)
								{
									if(!pChar->bank[i][j][k].item_no)
									{
										if(nGradeItemCount > 0)
										{
											pChar->bank[i][j][k] = Item;
											nGradeItemCount--;
										}
									}
								}
							}
						}
						FILE*	fp = NULL;
						char szFileName[512] = {0,};
						sprintf_s(szFileName, sizeof(szFileName),"./LotteryItem/_%03d_ItemGive.txt",this->m_Lotto_Info.nLottoID);
						fp = fopen(szFileName,"at+");

						GetCurDate();

						if(fp != NULL)
						{
						fprintf(fp ,"Lotto 당첨 : %s 님에게 %d개의 %d등 아이템이 지급 되었습니다.\n"
								,pChar->Name,pCheckOK->anWinItemsCount[nGradeItemIndex]
								,nGradeItemIndex+1);
						fclose(fp);
						}
						else
						{

						}
					}
				}

				p.h.header.type = CMD_DEL_LOTTO_USER;//데몬에다 삭제 요청을 해야 한다.
				QueuePacket(connections,DB_DEMON,&p,1);
				
			}
			else
			{
				pCheckOK->nWinItemCount = -1;
				pCheckOK->anWinNumbers[0] = nNeedBlank;//필요한 빈공간의 수.

				memcpy(&p.u.Check_Winner,pCheckOK,sizeof(t_CHECK_WINNER));	// BBD 040127	메모리 카피가 필요하다
				QueuePacket(connections,cn,&p,1);//실패 신호를 보내자.

				//<! BBD 040127	로또 블럭해제 로그 남김
				m_bWinnerChecking = false;			//	로또 확인 가능하게 풀어주자
				MyLog(LOG_NORMAL, "End Checking Lotto now");
				//> BBD 040127	로또 블럭해제 로그 남김
			}
		
		}
		else if(pCheckOK->nWinItemCount == 0)//꽝이다.
		{
			QueuePacket(connections,cn,&p,1);//클라이언트에게는 보냈다.
			
			p.h.header.type = CMD_DEL_LOTTO_USER;//데몬에다 삭제 요청을 해야 한다.
			QueuePacket(connections,DB_DEMON,&p,1);
		}		
	}
}

void CLottoSystem::RecvLottoInfo(LOTTO_EVENT_INFO *pLottoInfo)
{
	m_EventStatus = LOTTERIED;
	memcpy(&m_Lotto_Info,pLottoInfo,sizeof(LOTTO_EVENT_INFO));
}

void CLottoSystem::RecvOpenWinnerMenu(int cn)
{
	t_packet p;
	p.h.header.type = CMD_CHECK_WINNER_MENU;
	p.h.header.size = sizeof(t_WINNER_MENU);
	memset(&p.u.Lotto_Winner_Menu,0,sizeof(t_WINNER_MENU));
	p.u.Lotto_Winner_Menu.nLottoID = m_Lotto_Info.nLottoID;//soto-LottoADD
	strcpy_s(p.u.Lotto_Winner_Menu.strCharName, sizeof(p.u.Lotto_Winner_Menu.strCharName),::CheckServerId(cn)->Name);
	p.u.Lotto_Winner_Menu.nMaxLottoCount = this->m_nLottoNumberCount;

	QueuePacket(connections,DB_DEMON,&p,1);
}

void CLottoSystem::RecvCheckOpenWinnerMenu(t_WINNER_MENU *pWinnerMenu)
{
	int cn = ::ExistHe(pWinnerMenu->strCharName);
	if(cn)
	{
		t_packet p;
		p.h.header.type = CMD_OPEN_WINNER_MENU;
		p.h.header.size = sizeof(t_WINNER_MENU);

		memcpy(&p.u.Lotto_Winner_Menu,pWinnerMenu,sizeof(t_WINNER_MENU));

		if(m_EventStatus == EVENT)
		{
			memset(p.u.Lotto_Winner_Menu.anWinNumbers,0,sizeof(int) * 10);
		}


		QueuePacket(connections,cn,&p,1);
	}
}

void CLottoSystem::RecvOpenLottoMenu(int cn)
{
	t_packet p;
	p.h.header.type = CMD_OPEN_LOTTOMENU;
	p.h.header.size = sizeof(t_OPEN_LOTTO_MENU);
	p.u.Lotto_Menu_Open.nMaxNumberCount = this->m_nLottoNumberCount;

	QueuePacket(connections,cn,&p,1);
}

void CLottoSystem::BroadCastLotteryMsg()
{
	t_packet p;
	p.h.header.type = CMD_LOTTERY_BROADCAST;
	p.h.header.size = sizeof(t_LOTTERY_INFO);
	p.u.Lotto_BroadCast.nLottoID = this->m_Lotto_Info.nLottoID;
	p.u.Lotto_BroadCast.nWinNumCount = this->m_Lotto_Info.nWinNumCount;
	memcpy(p.u.Lotto_BroadCast.anWinNumbers,this->m_Lotto_Info.anWinNumbers,sizeof(int)*10);

	::SendPacket2Maps(&p);
	g_pUserManager->SendPacket(&p); // CSD-CN-031213
}

int CLottoSystem::GetLottoID()
{
	return m_Lotto_Info.nLottoID;
}

int CLottoSystem::GetNextLotteryIndex()
{
	return -1;		
}

int CLottoSystem::FindSameDayIndex(int nDay)
{
	return -1;
}
//<! BBD 040127	로또 프로세스 변경
void CLottoSystem::CheckIsDelOk(bool bIsOk)
{
	// 모든 확인 절차가 끝났으므로 다른 클라이언트의 확인을 허용한다
	if(bIsOk && m_bWinnerChecking)
	{
		//<! BBD 040127	로또 블럭해제 로그 남김
		m_bWinnerChecking = false;
		MyLog(LOG_NORMAL, "End Checking Lotto now");
		//> BBD 040127	로또 블럭해제 로그 남김
	}
}
//> BBD 040127	로또 프로세스 변경
