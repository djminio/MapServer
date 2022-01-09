#pragma once

#define MSG_SHUT_DOWN				100	//ShutDown
#define MSG_RELOAD_GAMESERVER_DATA  101 //Reload Gameserver data
#define MSG_CHANGE_WEATHER			102
#define MSG_ECHO					103

#define MSG_RM_KICKOFF_USER			111	// BBD 040110 RM 유저 접속 종료 
#define MSG_RM_KICKOFF_USER_ALL		112	// BBD 040110 RM 전체유저 강제 접속종료 시작
#define MSG_RM_KICKOFF_USER_SEVERAL	113	// BBD 040110 RM 정해진 명수의 유저종료
#define MSG_RM_KICKOFF_AGENTCANJOIN	114	// BBD 040110	Agent에서 맵으로 조인 가능토록 허용

#pragma pack (push,1)
struct PACKET_SHUT_DOWN
{
	BYTE bPtcl;
	BYTE bHeader;
};


struct PACKET_RELOAD_GAMESERVER_DATA
{
	BYTE bPtcl;
	BYTE bHeader;
};

struct PACKET_CHANGE_WEATHER
{
	BYTE bPtcl;
	BYTE bHeader;
	BYTE bWeather;
	BYTE bStopWeather;
	DWORD dwAmount;
};



#pragma pack(pop)