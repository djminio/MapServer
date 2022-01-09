// KickOffAll.cpp: implementation of the CKickOffAll class.
//
//////////////////////////////////////////////////////////////////////

#include "../stdafx.h"
#include "KickOffAll.h"
#include "UserManager.h"		// BBD 040110

typedef map<string, WORD>     HASH_USER;	// BBD 040110
#define DEFAULT_KICKAMOUNT1TIMES	10	// BBD 040110

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKickOffAll::CKickOffAll()
{
	m_bIsKicking = false;
	//<! BBD 040401
	m_nKickAmount1Time = 0;
	m_bIsAllKick = false;
	//> BBD 040401
}

CKickOffAll::~CKickOffAll()
{
	//<! BBD 040401
	 m_nKickAmount1Time = 0;
	//> BBD 040401
}


/////////////////////////////////////////////////////
//BBD 040110 
// 함수명 : InitKickoffUserAll
// 기능 : 유저를 내보내기 위한 초기화를 진행
/////////////////////////////////////////////////////
bool CKickOffAll::InitKickOffUserAll()
{
//<! BBD 040401
/*
	if(!g_pUserManager)		// 유효한지 검사
	{
		return false;
	}
	HASH_USER users = g_pUserManager->GetUserSet();
	m_nConnectAmount = users.size();
*/
//>! BBD 040401

	m_nKickAmount1Time =GetPrivateProfileInt("KICKOFF_INFO", "KickAmount1Times" , 0, MapServerConfigFileName);
	
	if(!m_nKickAmount1Time) // 한번에 암두 않잘르면 할게 없더
	{
		return false;
	}

	m_bIsKicking = true;
	m_bIsAllKick = false;	// BBD 040401
	return true;
}

//////////////////////////////////////////////////////
// BBD 040110
// 함수명 : KickOffSeverlUser
// 기능 : 정해져 있는 명수의 유저를 저장하고 내보낸다
//////////////////////////////////////////////////////
void CKickOffAll::KickOffSeverlUser()
{	//<! BBD 040401
	if(m_bIsAllKick || !m_bIsKicking)	// Kick중이 아니거나 모두 킥했을때는 걍 리턴
	{
		return;
	}


/*	if(!m_nConnectAmount)	// 모두 다 잘려나갔다
	{
		return;
	}

	// 유저들을F 저장하고 잘라주자
	static int s_KickCurrent = DRAGON_CONNECTIONS_START;					// 현재 자를 콘넥션의 인덱스
	static int s_KickEnd = DRAGON_CONNECTIONS_START + m_nKickAmount1Time;	// 이번에 잘라줄 콘넥션의 Maximum
*/
	// 루프를 돌며 이번에 잘라줄 놈들을 자르자
	int count = 0;
	for(int cn = DRAGON_CONNECTIONS_START; cn < DRAGON_MAX_CONNECTIONS; ++cn)
	{
		if( connections[cn].dwAgentConnectionIndex && connections[cn].state >= CONNECT_JOIN )
		{
			CHARLIST *ch = &connections[cn].chrlst;			
			::closeconnection( connections, cn, -33 , false);		// BBD 040401
			count++;
			if(count >= m_nKickAmount1Time)
			{
				return;
			}
		}
	}
	// 여기까지 탈출한것은 전체를 돌아 콘넥션을 다 비웠다는 뜻
	m_bIsAllKick = true;
	//> BBD 040401
}

bool CKickOffAll::StopKicking()
{
	m_bIsKicking = false;
	return true;
}
