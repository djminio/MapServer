// -----------
// GameTimer.h
// -----------
// 2000/11/02, chan78
//
#pragma once
#include "..\stdafx.h"

#define GTE_ROUNDING_TIME				150			// 단위는 ms
#define GAME_TIC_SIZE			GTE_ROUNDING_TIME	//    " "

#define DEFAULT_EXPECTED_TIME			100			// 단위는 ms
// CSB ModCode 5 -> 6
#define NUM_OF_GTES						6			// very short, short, normal, long, very long
// CSB ModCode End

#define MERGE_GTE_PROCESS_AT			5			// 소요시간이 클 경우에는 바로 분할하지만,
													// 소요시간이 짧을 경우에는 이 수만큼의 평균값을 보고
													// 소요시간이 확실히 짧아졌다고 판단되면 분할한다 
#define GTE_SET		true
#define GTE_UNSET	false

#define TIMER_PROCESS_NAME_LENGTH		256

typedef void (*VOIDFUNCS)(void);
typedef void (*MYFUNC)( DWORD all, DWORD now );

class CGameTimer {
private:
	char		sTimerProcessName[TIMER_PROCESS_NAME_LENGTH+1];		// TimerProcessName
	DWORD		dwTickSize;						// 펑션이 불려질 주기, 단위는 TIC, TIC_SIZE가 바뀔때 변경해주는것을 잊지말자
	bool		bIsDividable;					// 분할작업이 가능한 펑션인가?

	MYFUNC		DividableFuncToCall;			// 분할처리가능
	VOIDFUNCS	NormalFuncToCall;				// 일반

	DWORD		dwExpectedTime;					// 함수가 실행을 끝내고 리턴할때까지의 소요시간 기대값.
												// 소요시간이 기대값보다 크면 동적으로 작업을 분할한다.
	WORD		wNumOfDividedBy;				// 이 수치만큼 현 작업을 쪼개어 처리한다. (쪼갬수)
	WORD		wNextNumOfDividedBy;			// 분할작업 도중에 쪼갬수를 변경할 수 없으므로, 분할 작업이
												// 진행중일때에는 이 값을 변경해두었다가 한 단위의 처리가 끝나면 그때
												// 쪼갬수를 변경한다.(중요)
	WORD		wCurrentProcessPos;				// 작업을 분할하여 진행중인 경우에 진행 정도를 기록할 변수.
	DWORD		dwTotalTakeTime;				// 과거 작업때 걸린 시간을 통계내는데 쓴다.

public:
	CGameTimer();								// 생성자
	~CGameTimer();								// 파괴자
	void SetGameTimer(char *name, DWORD ticksize, bool dividable, DWORD expecttime, MYFUNC func);	// Setting
	void SetGameTimer(char *name, DWORD ticksize, bool dividable, DWORD expecttime, VOIDFUNCS func);	// Setting
	bool IsTicToRun(DWORD tick);				// 실행할 틱인지 검사
	DWORD GetExpectedTime();					// return dwExpectedTime;
	WORD Run();									// 지정된 작업을 실행한다.
};

void __stdcall GameTimerProcess(DWORD dwValue);//020511 lsw;
void SettingGameTimers( void );
extern bool IsGameTimerWorking;
void StartGameTimers( void );
void StopGameTimers( void );