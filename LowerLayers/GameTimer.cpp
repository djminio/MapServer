//
// GameTimer.cpp
//

// --------------------
// 동적 작업분할 시스템
// --------------------
#include "GameTimer.h"
#include "..\stdafx.h"
#include <Mmsystem.h>
#include "monitor.h"
#include "mylog.h"
#include "..\HigherLayers\WeatherControl.h"
#include "..\HigherLayers\NPC_Pattern.h"
#include "..\HigherLayers\DefaultHeader.h"
#include "..\HigherLayers\Scrp_exe.h"
#include "..\HigherLayers\Citem.h"
#include "..\HigherLayers\Op_magic.h"
#include "..\HigherLayers\area.h"
#include "..\HigherLayers\SealStone.h"
#include "..\HigherLayers\QuestFunction.h"	// 021017 kyo
#include "..\HigherLayers\ArenaManager.h"
#include "..\HigherLayers\BattleManager.h"
#include "..\HigherLayers\UserManager.h"

void CheckElapsedTime_NK_Down( void );
void prepare(t_connection c[]);

CGameTimer GameTimerList[NUM_OF_GTES];	// GameTimerList 배열, NUM_OF_GAME_TIMERS 는 게임타이머의 추가, 삭제가 있을때 꼭 고쳐줘야 한다.(중요)
DWORD end_of_tick = 0;					// SetGameTimer()에서 자동 갱신해 주므로 고칠 필요 없다.
DWORD dwTimerFlag = 0;
bool IsGameTimerWorking = false;

extern void UpdateEventMonster();		// LTS AI2

extern void CheckMallItemTimeElapsed();

void NPCRegen( DWORD all, DWORD now )
{
	static DWORD lasttime = 0;
	static DWORD curtime = 0;
	int total = 40;

	int from;
	int to;

	static int increasement;

	curtime = timeGetTime();
	static int counter = 0;

	static int run = 0;

	if( all > 1 ) {
		if( now == 0 ) MyLog( LOG_IGNORE, "NPCRegen::Called %d(%ldms) (%d/%d) Started", counter, (curtime-lasttime), now+1, all);
		if( now == (all-1) ) MyLog( LOG_IGNORE, "NPCRegen::Called %d(%ldms) (%d/%d) Finished", counter, (curtime-lasttime), now+1, all);
		//MyLog( LOG_NORMAL, "NPCRegen::Called %d(%ldms) (%d/%d)", counter, (curtime-lasttime), now+1, all);
	} else {
		MyLog( LOG_NORMAL, "NPCRegen::Called %d(%ldms)", counter, (curtime-lasttime));
	}
	counter++;

	lasttime = curtime;

	if( now == 0 ) {
		increasement = (int)((float)total/(float)all);
	}

	from = now*increasement;
	// 마지막엔 나머지를 다 처리
	if( now == (all-1) ) { to = total; run++; MyLog(LOG_IGNORE, "OKOKOKOKOK"); }

	else to = from+increasement;

	for( int i = from; i < to; i++ ) {
		if( run < 3 ) {
			Sleep(100);
		} else Sleep(10);
	}	
		
	return;
}		
		
// Modified by chan78 at 2000/11/29, 일단 분류해 둠.
void very_short_terms()		// 150 ms
{	//< CSD-HK-030829		
	prepare(connections);
	g_pUserManager->Execute();
}	//> CSD-HK-030829
		
void short_terms()		// 300 ms
{		
	MovingNPC();
	MovingCharacter();
	RunArea(connections);
	return;
}		
		
void normal_terms()		// 450 ms
{
	ReCallScript();
	CheckSkillTimeElapsed();
}
	
void long_terms()		// 600 ms
{
	NPC_CheckRemove();
	SendWeatherRoutine(connections);
	NPC_AutoCreate();
	SendAliveCheckRoutine(connections);
	return;
}		
	
extern void CheckTimeForMenu_Kein();		// 010419 YGI		
extern void UpdateDragonLordWarTime();		// LTS DRAGONLORD

void very_long_terms()	// 750 ms
{		
	g_pBattleManager->Elapse();    
	g_pArenaManager->Ready();   // CSD-030521
	g_pArenaManager->Execute(); // CSD-030516
	CheckNationWar();	
	CheckMallItemTimeElapsed();
	CharacterAutoUpdate();
	CheckItemTimeElapsed();
	CheckTimeForMenu_Kein(); // 010419 YGI
	UpdateEventMonster();		 // LTS AI2
	UpdateDragonLordWarTime();		// LTS DRAGONLORD
	g_QuestInMap.RunQuestTimer();// 021104 kyos
	CheckElapsedTime_NK_Down();

	RandomWeatherSystem.Proc(); //Eleval 11/08/09 - To Update Weather
	return;
}		
		
		
CGameTimer::CGameTimer()
{		
	sTimerProcessName[0] = '\0';
	dwTickSize = 0;
	bIsDividable = false;
	dwExpectedTime = 0;
	wNumOfDividedBy = 1;
	wNextNumOfDividedBy = 0;
	wCurrentProcessPos = 0;
	dwTotalTakeTime = 0;
	DividableFuncToCall = NULL;
	NormalFuncToCall = NULL;
}	
	
CGameTimer::~CGameTimer()
{	
}	
	
DWORD CGameTimer::GetExpectedTime()
{	
	return dwExpectedTime;
}		
void CGameTimer::SetGameTimer(char *name, DWORD ticksize, bool dividable, DWORD expecttime, MYFUNC func)
{	
	if( strlen(name) > TIMER_PROCESS_NAME_LENGTH ) {
		strncpy_s(sTimerProcessName, name, TIMER_PROCESS_NAME_LENGTH);
		sTimerProcessName[TIMER_PROCESS_NAME_LENGTH] = '\0';
	} else strcpy_s(sTimerProcessName, sizeof(sTimerProcessName), name);
	dwTickSize = ticksize;
	if( dwTickSize > end_of_tick ) end_of_tick = dwTickSize;
	bIsDividable = dividable;
	dwExpectedTime = expecttime;

	DividableFuncToCall = func;
	NormalFuncToCall = NULL;

	MyLog(LOG_IGNORE, "%s, (tick)%d, (able)%d, (expect)%d are ADDED", name, ticksize, dividable, expecttime);
	return;
}	
	
void CGameTimer::SetGameTimer(char *name, DWORD ticksize, bool dividable, DWORD expecttime, VOIDFUNCS func)
{	
	if( strlen(name) > TIMER_PROCESS_NAME_LENGTH ) {
		strncpy_s(sTimerProcessName, name, TIMER_PROCESS_NAME_LENGTH);
		sTimerProcessName[TIMER_PROCESS_NAME_LENGTH] = '\0';
	} else strcpy_s(sTimerProcessName, sizeof(sTimerProcessName), name);
	dwTickSize = ticksize;
	if( dwTickSize > end_of_tick ) end_of_tick = dwTickSize;
	bIsDividable = dividable;
	dwExpectedTime = expecttime;

	DividableFuncToCall = NULL;
	NormalFuncToCall = func;

	MyLog(LOG_IGNORE, "%s, (tick)%d, (able)%d, (expect)%d are ADDED", name, ticksize, dividable, expecttime);
	return;
}	
	
bool CGameTimer::IsTicToRun(DWORD tick)
{
	if( !bIsDividable && !(tick % dwTickSize) ) return true;
	if( bIsDividable ) {
		if( wCurrentProcessPos ) return true;
		// (tick%dwTickSzie) 나머지가 
		else if( (dwTickSize-(tick%dwTickSize)) == (DWORD)(dwTickSize - (wNumOfDividedBy/2)) ) return true;
	}
	return false;
}

WORD CGameTimer::Run()
{
	DWORD StartTime, FinishTime, TakeTime;
	static int counter = 0;
	StartTime = timeGetTime();							// 여기서부터 소요 시간 체크
	
	// Run
	if( bIsDividable ) {
		DividableFuncToCall( wNumOfDividedBy, wCurrentProcessPos );
		if ( wNumOfDividedBy > 1 ) {
			wCurrentProcessPos++;

			// 다 했으면 마무리.
			if( wCurrentProcessPos == wNumOfDividedBy )
				wCurrentProcessPos = 0;
		}
	} else {
		NormalFuncToCall();
	}

	FinishTime = timeGetTime();							// 소요시간 체크 끝
	
	TakeTime = FinishTime - StartTime;					// 걸린 시간을 계산한다.

	// 소요시간 기록 남기기
	if( !bIsDividable || wNumOfDividedBy == 1 ) {
		dwTotalTakeTime = TakeTime;						// 분할 불가능한 함수이거나 분할가능하지만 분할되지 않은 작업이면 이번에 걸린 시간을 dwTotalTakeTime에 넣는다.
	} else {
		counter++;
		dwTotalTakeTime = dwTotalTakeTime + TakeTime;	// 분할된 작업은 한단위의 작업이 끝날때까지 걸린시간을 더한다.
		if (wCurrentProcessPos == 0 ) {
			MyLog( LOG_IGNORE, "TotalTakeTime(%d/%d)", counter, dwTotalTakeTime );
			counter = 0;
		}
	}

	if( bIsDividable && !wCurrentProcessPos ) {			// 분할가능한 펑션이고, 단위작업이 진행중이 아니면 분할/합침 여부를 판단한다.
		if ( wNumOfDividedBy > 1 ) {					// 분할된 작업의 경우 한 작업당 소요시간의 평균을 구한다.
			dwTotalTakeTime = dwTotalTakeTime / wNumOfDividedBy;
		}
		DWORD dwTakeTimeAvr = dwTotalTakeTime;			// 평균값으로 사용.
		dwTotalTakeTime = 0;							// 꼭 클리어해준다.

		bool bIsExcess;
		DWORD dwDiffTime;

		if( dwTakeTimeAvr > dwExpectedTime ) {
			if( wNumOfDividedBy == dwTickSize )			// 이미 최대값 이상의 시간이 걸리고 있으면...
				return (WORD)TakeTime;
			bIsExcess = true;
			dwDiffTime = dwTakeTimeAvr - dwExpectedTime;
		} else if( dwTakeTimeAvr < dwExpectedTime ) {
			if( wNumOfDividedBy <= 1 )					// 1 이하면 더 나눌수 없다.
				return (WORD)TakeTime;
			bIsExcess = false;
			dwDiffTime = dwExpectedTime - dwTakeTimeAvr;
		} else {
			return (WORD)TakeTime;
		}

		float factor = (float)dwDiffTime / (float)dwExpectedTime;

		if( (bIsExcess &&(factor > 0)) || (!bIsExcess && (factor > 0.25)) ) {
			if( bIsExcess )
				factor = (float)1.0 + factor;
			else
				factor = (float)1.0 - factor;
			
			wNumOfDividedBy = (int)((float)wNumOfDividedBy * factor);
			MyLog( LOG_NORMAL, "TakeTimeAvr(%d), ExpectedTime(%d), Factor(%f), Changed to(%d)", dwTakeTimeAvr, dwExpectedTime, factor, wNumOfDividedBy);
			if( wNumOfDividedBy > dwTickSize ) {	// wNumOfDividedBy는 dwTickSize보다 클 수 없다.
				MyLog( LOG_IMPORTANT, "GTE(%s) takes too long time but can't be devided more(%d:%d)", sTimerProcessName, wNumOfDividedBy, dwTickSize);
				wNumOfDividedBy = (WORD)dwTickSize;
			}
		}
	}
	return (WORD)TakeTime;
}


// 꼭 실행해줘야 한다...
void SettingGameTimers( void )
{
	// 이름, 실행할 틱 넘버, 동적작업분할가능여부, 기대시간, 호출할 함수.
	// 주의! 0부터 NUM_OF_GAME_TIMERS-1 까지 배열이 비어서는 절대 안됨.
	// 낮은 인덱스에 추가할수록 처리 우선순위가 높다. 일반적으로 호출틱이 짧을수록 낮은 인덱스에 등록하는것이 좋다.

// Commected by chan78 at 2000/11/29
//	GameTimerList[0].SetGameTimer("Run Them all", 2, false, 400, imsi );
// 1당 150 ms
	GameTimerList[0].SetGameTimer("Very Short Terms",	1, false,	100, very_short_terms	);
	GameTimerList[1].SetGameTimer("Short Terms",		2, false,	300, short_terms		);
	GameTimerList[2].SetGameTimer("Normal Terms",		3, false,	450, normal_terms		);
	GameTimerList[3].SetGameTimer("Long Terms",			4, false,	600, long_terms			);
	GameTimerList[4].SetGameTimer("Very Long Terms",	7, false,	1000, very_long_terms	);
	GameTimerList[5].SetGameTimer("Quest30Second",		10, false, 	100, QuestSecond	);

	StartGameTimers();

	return;
}

void StartGameTimers( void )
{
	IsGameTimerWorking = true;
	return;
}

void StopGameTimers( void )
{
	IsGameTimerWorking = false;
	return;
}

// GTE_SIZE 에 한번씩 호출되는 func,
// timer.cpp 참고
void __stdcall GameTimerProcess(DWORD dwValue)//020511 lsw
{
	static DWORD tick = 1;
	static int LastProcess = 0;
	WORD TakeTime;

	if( !IsGameTimerWorking ) return;

	for( int i = LastProcess; i < NUM_OF_GTES; i++ ) {

		if( !GameTimerList[i].IsTicToRun(tick) )
			continue;

		// 여기서 걸린 시간이 최대 허용 시간을 벗어나면 아직 할 일이 남아있어도 메인쓰레드로 복귀해준다.
		// tic은 증가시키지 않고 LastProcess는 i+1의 값을 갖게되므로 남아있는 작업부터 처리하게 된다.
		if( (TakeTime = GameTimerList[i].Run()) > GameTimerList[i].GetExpectedTime() ) {
			// 시간 오버를 로그하고싶으면...
			//MyLog( LOG_IGNORE, "GameTimerList[%d].Run() > dwExpectedTime(%d)", i, TakeTime );

			LastProcess = i+1;
			if( LastProcess == NUM_OF_GTES ) {		// 어차피 이번 작업이 마지막인 경우...
				LastProcess = 0;
			tick++;
			}
			return;
		}

		// LastProcess가 0이 아닌경우는 저번 호출때 할일이 남아 tic이 증가하지 않은것이므로 틱을 증가시키고
		// 원래 이번턴에 해야할 일들을 처리해준다. 가능하면 틱이 덜 밀리도록 하기 위한 처리다.
		if( i == (NUM_OF_GTES-1) ) {
			if( LastProcess != 0 ) {
				LastProcess = 0;
				tick++;
				i = 0;
			}
		}

	}

	// 해야할 모든 작업을 기대한 시간안에 끝냈다.
	LastProcess = 0;
	tick++;

	return;
}

