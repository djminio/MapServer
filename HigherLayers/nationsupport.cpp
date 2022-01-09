#include "..\stdafx.h"
#include "DefaultHeader.h"
#include "CItem.h"
#include "Op_Magic.h"
#include "Op_Battle.h"
#include "Map.h"
#include "ChrLog.h"
#include "nationsys.h"

extern cWarfield*	g_pWarfield;

extern bool isNationWarfieldServer();
extern bool isNationManageServer();

#define MAX_SQUAD_1_RARE_CALSS	5
#define MAX_SQUAD_2_RARE_CALSS	4
#define MAX_SQUAD_3_RARE_CALSS	3
#define MAX_SQUAD_4_RARE_CALSS	2

#define MAX_SQUAD_NUMBER		4
#define MAX_SQUAD_RARE			3

int		iSquadRareType[MAX_SQUAD_NUMBER][MAX_SQUAD_RARE];
bool	bSquadRared[MAX_SQUAD_NUMBER];

//기획자가 제공한 고정적인 맥스값.
const int	iSquadMaxRareClass[MAX_SQUAD_NUMBER]	=	{MAX_SQUAD_1_RARE_CALSS,MAX_SQUAD_2_RARE_CALSS,MAX_SQUAD_3_RARE_CALSS,MAX_SQUAD_4_RARE_CALSS};
//상황에 따라 변하는 맥스값.
int		iSquadNowMaxRareClass[MAX_SQUAD_NUMBER]	=	{MAX_SQUAD_1_RARE_CALSS,MAX_SQUAD_2_RARE_CALSS,MAX_SQUAD_3_RARE_CALSS,MAX_SQUAD_4_RARE_CALSS}; 
int		iSquadNowRareClass[MAX_SQUAD_NUMBER]	=	{0,0,0,0}; 

void SquadRarePlus(CHARLIST *ch, int iRareType, int iClass)
{
	if(!isNationWarfieldServer()) return;//여기가 국가전 맵 아니면 절대로 가중치를 주지 않는다.	
	
	int iPlusValue = ItemRare[iRareType].GRADE[iClass];
}


//전쟁터 맵 서버만 씀 이거는... 클래스만 정함 레어 타입은 상관 없음
int CheckSquadRareGrade( int  iBattleType,int iSquadNo, int iManCount, bool bCommanderLogin, bool bSquadLeaderLogin, int iSquadItemStatus)
{
	int iRareSquadGradeNow = 0;
	//레어 들어갈 공식을 체크 하자
	//부대장이 등록 됨과 동시에 레어 타입은 나오게 된다
	if(!isNationWarfieldServer()) return 0;//사람수 체크 해서 기본적인걸 모두 체킹 해 둔다	
	
	int MemberCount = g_pWarfield->GetSquadCurrentMemberCount(iBattleType,iSquadNo);
	
	if( MemberCount )//조건
	{	
		if(MemberCount < 19 )
			iRareSquadGradeNow = 1;
		if(MemberCount < 29 && MemberCount > 19 )
			iRareSquadGradeNow = 2;
		if(MemberCount < 39 && MemberCount > 29 )
			iRareSquadGradeNow = 3;
		if(MemberCount < 49 && MemberCount > 39)
			iRareSquadGradeNow = 4;
		if(MemberCount > 49 )
			iRareSquadGradeNow = 5;

		iSquadNowRareClass[iSquadNo] = iRareSquadGradeNow ;//이놈의 값을 바꿔 주면 된다
	}

	//사령관이 있으면 맥스 값에 true 를 더해준다 ( +1 되는것 )
	iSquadNowMaxRareClass[iSquadNo] = iSquadNowMaxRareClass[iSquadNo]+bCommanderLogin;

	if( bSquadLeaderLogin )//부대장이 있으면 등급을 준다
	{
		if(iSquadItemStatus)//사기 상승 아이템을 쓰면 이 값이 +1 이 되고 사기 저하 아이템을 쓰면 -1이 된다.
		{
			iSquadNowRareClass[iSquadNo] += iSquadItemStatus;//리셋 스타일이므로 걱정없이 한다.
			if( iSquadNowRareClass[iSquadNo] > iSquadNowMaxRareClass[iSquadNo])
			{
				iSquadNowRareClass[iSquadNo]=iSquadNowMaxRareClass[iSquadNo];
			}
			else if( iSquadNowRareClass[iSquadNo] < 0)
			{
				iSquadNowRareClass[iSquadNo] =0;
			}

		}
	}
	else
	{
		iSquadNowRareClass[iSquadNo] = 0;//부대장이 없으면 제로다.
	}
	return iRareSquadGradeNow = iSquadNowRareClass[iSquadNo];
}

//전쟁타입 ( Attacker Defencer ReForce), 커넥션 넘버, 전쟁 관련 아이템(사기)
void SquadRareReSet(int iBattleType, int cn,int iSquadItemStatus)//iSquadItemStatus->전쟁 사기 아이템에 의한 클래스 변경값
{//020215 lsw

	//타입과 부대 넘버와 사령관과 리더가 있나를 체크 하여
	//부여할 레어 속성과 등급을 뽑아낸다
	CHARLIST *ch = &connections[cn].chrlst;
	if (!ch) return;
	
	int iSquadNo  = connections[cn].chrlst.NWCharacter.SquadNo;
	int iRareClass = 0;

	//이게 워필드 인가.
	if(!isNationWarfieldServer()) return;

	bool bCommanderLogin = g_pWarfield->CommanderExist(iBattleType);
	bool bSquadLeaderLogin = g_pWarfield->SquadLeaderExist(iBattleType,connections[cn].chrlst.NWCharacter.SquadNo);

	int iSquadRareClass = CheckSquadRareGrade(iBattleType ,iSquadNo , g_pWarfield->GetSquadCurrentMemberCount(iBattleType,iSquadNo),  bCommanderLogin, bSquadLeaderLogin, iSquadItemStatus);

	//타입( 공격자 , 방어자 , 용병)
	//스쿼드 넘버 에 일치 하는 사람을 검색 해 내자
	SquadList* g_SquadList;
	g_SquadList = g_pWarfield->GetSquadListPointer(iBattleType,iSquadNo);	
	
	char SquadRare[3] = {0,0,0};
	//011008 lsw 이거 주석 푸세요
	memcpy(SquadRare,g_pWarfield->GetSquadRareSok2(iBattleType,iSquadNo),3);
	
	for(SquadListItor itor=g_SquadList->begin();	itor!=g_SquadList->end()	;	itor++)
	{
		//스쿼드는 0,1,2,3 으로 해 주세요.. 
		ch = &connections[(*itor)->GetData()].chrlst;
		if(ch)
		{
				ch->SquadRare.grade		= iSquadRareClass;
				ch->SquadRare.soksung1	= SquadRare[0];
				ch->SquadRare.soksung2	= SquadRare[1];
				ch->SquadRare.soksung3	= SquadRare[2];
				ResetAbility(ch);
		}
	}
	
}

//전쟁 관리용 서버가 호출 // 속성정하기만 합니다.
void GetSquadRare(char *Squadrare,int squadNo/*char [3]*/)
{
	if( !isNationManageServer() )
		return;

	if (!Squadrare[0]) //이미 속성을 받은 경우는 속성 부여 하지 않는다
	{
		int squadrarecount =0;
		int	squadrarecountMax =0;
		
		switch (squadNo)
		{
			case 1:
			case 2:
				{
					squadrarecountMax	= 3;
				}break;
			case 3:
			case 4:
				{
					squadrarecountMax	= 2;
				}break;
			default:
				squadrarecountMax	= 0;
				break;
		}
		for(squadrarecount = 0;squadrarecount<squadrarecountMax;squadrarecount++)
		{
			//이제 속성을 받아왔다//속성이 모두 정해졌다.
			//여기서 넣어주기
			Squadrare[squadrarecount] = ItemMgr.GetAbleRareKind(RARE_ABLE|RARE_ABLE_SQUAD,0);//국가전용//020725 lsw
		}
	}
}

extern bool isNewWarfieldServer();
extern CNewWarfield* g_pNewWarfield;


// 020418 YGI
int IsStartNationWar()		// 이 맵이 전투 맵이고 현재 전투 중인가?
{
	if (isNationWarfieldServer())
	{
		if (g_pWarfield->GetStatus()==NW_WAR)
			return true;
	}
	if (isNewWarfieldServer())
	{
		if (g_pNewWarfield->GetWarfieldStatus()==NW_WAR)
			return true;
	}
	return false;
//	return ( isNationWarfieldServer() && g_pWarfield->GetStatus()==NW_WAR );
}
