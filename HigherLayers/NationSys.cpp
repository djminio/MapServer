#include "..\stdafx.h"
#include "DefaultHeader.h"
#include "NationSys.h"
#include "..\LowerLayers\//MyLog.h"
#include "SealStone.h"
#include "..\LowerLayers\servertable.h"
#include "Scrp_exe.h"
#include "CItem.h"//020505 lsw
#include "LogManager.h"
#include "UserManager.h"

cWarfield*		g_pWarfield=NULL;
cNation*		g_pNation=NULL;
CNewWarfield*	g_pNewWarfield=NULL;
__int64			g_WarTime=0;		// 전쟁시간			// 011015 LTS	// g_curr_time의 버그로 교체 예정

WORD			g_wMapServerPort=0;

int				g_pLoadingPoint[NW_WARFIELD_COUNT][NW_SQUAD_MAX];			// g_pWarfield, g_pNation이 없는 경우
t_Warfield		g_pWarfieldStatus[NW_WARFIELD_COUNT];						// g_pWarfield, g_pNation이 없는 경우
int*			g_pNationWar_Exp=NULL;								// LTS NEW LOCALWAR
int				g_pNewWarfieldStatus[MAX_NEW_WARFIELD];
//int				bMoveLevelMax[MAX_NEW_WARFIELD];		// MAX_NEW_WARFIELD
//int				bMoveLevelMin[MAX_NEW_WARFIELD];		// MAX_NEW_WARFIELD
int				MoveLevelMax[MAX_NEW_WARFIELD];		// MAX_NEW_WARFIELD
int				MoveLevelMin[MAX_NEW_WARFIELD];		// MAX_NEW_WARFIELD

int				HonorWon[DRAGON_MAX_CONNECTIONS_]; //Eleval 07/09/09 - To store the honor won by a player during the warfield
extern int		var[ DRAGON_MAX_CONNECTIONS_][ _SCRIPT_VARABLE_POINT_ ];

#ifdef WARFIELD_SERVER_
#define MAX_STORED_ACTIONS	10
enum PK_Actions_Type
{
	PKA_RESS,
	PKA_KILL
};
struct PK_Actions
{
	char target_name[20];
	int action;
};

PK_Actions PK_Actions_tbl[DRAGON_MAX_CONNECTIONS_][MAX_STORED_ACTIONS] = {0, };
int PKA_Number[DRAGON_MAX_CONNECTIONS_] = {0, };
#endif



// Tile Concern__________________________________________________________________________________	
const POINTS		Map0_133[MAP0_133]={	// 010927 LTS
									{131,259},{131,260},{131,261},{131,262},{131,263},{131,264},{131,265},
									{131,266},{132,258},{132,259},{132,260},{132,261},{132,262},{132,263},
									{132,264},{132,265},{133,257},{133,258},{133,259},{133,260},{133,261},
									{133,262},{133,263},{133,264},{134,256},{134,257},{134,258},{134,259},
									{134,260},{134,261},{134,262},{134,263}		};

const POINTS		Map0_138[MAP0_138]={
									{135,255},{135,256},{135,257},{135,258},{135,259},{135,260},{135,261},
									{135,262},{136,255},{136,256},{136,257},{136,258},{136,259},{136,260},
									{136,261},{137,256},{137,257},{137,258},{137,259},{137,260},{138,257},
									{138,258},{138,259},{139,258}	};

const POINTS		Map0_203[MAP0_203] ={	
									{200,192},{200,193},{200,194},{200,195},{200,196},{200,197},{200,198},
									{200,199},{201,192},{201,193},{201,194},{201,195},{201,196},{201,197},
									{201,198},{202,193},{202,194},{202,195},{202,196},{202,197},{203,194},
									{203,195},{203,196},{204,195} 	};

const POINTS		Map0_198[MAP0_198] = {	
									{199,193},{199,194},{199,195},{199,196},{199,197},{199,198},{199,199},
									{199,200},{198,194},{198,195},{198,196},{198,197},{198,198},{198,199},
									{198,200},{198,201},{197,195},{197,196},{197,197},{197,198},{197,199},
									{197,200},{197,201},{197,202},{196,196},{196,197},{196,198},{196,199},
									{196,200},{196,201},{196,202},{196,203},{195,196},{195,197}				};

const POINTS		Map0_263[MAP0_263] ={	
									{261,133},{261,134},{261,135},{261,136},{261,137},{261,138},{261,139},
									{261,140},{262,132},{262,133},{262,134},{262,135},{262,136},{262,137},
									{262,138},{262,139},{263,131},{263,132},{263,133},{263,134},{263,135},
									{263,136},{263,137},{263,138},{264,130},{264,131},{264,132},{264,133},
									{264,134},{264,135},{264,136},{264,137}									};

const POINTS		Map0_268[MAP0_268] ={	// 010927 LTS
									{265,129},{265,130},{265,131},{265,132},{265,133},{265,134},{265,135},
									{265,136},{266,129},{266,130},{266,131},{266,132},{266,133},{266,134},
									{266,135},{267,130},{267,131},{267,132},{267,133},{267,134},{268,131},
									{268,132},{268,133}														};
//________________________________________________________________________________________________

const POINTS		Map1_450[MAP1_450]=
								{	{451,521},{451,522},{451,523},{451,524},{451,525},{451,526},
									{450,522},{450,523},{450,524},{450,525},{450,526},{450,527},
									{449,523},{449,524},{449,525},{449,526},{449,527},{449,528},
									{448,524},{448,525},{448,526},{448,527},{448,528},{448,529},
									{447,525},{447,526},{447,527},{447,528},{447,529},{446,526},
									{446,527},{446,527}											};

const POINTS		Map1_527[MAP1_527]=		// 010927 LTS
								{	{528,525},{528,526},{528,527},{528,528},{528,529},{528,530},
									{527,526},{527,527},{527,528},{527,529},{527,530},{527,531},
									{526,527},{526,528},{526,529},{526,530},{526,531},{526,532},
									{525,528},{525,529},{525,530},{525,531},{525,532},{525,533},
									{524,529},{524,530},{524,531},{524,532},{524,533},{523,530},
									{523,531},{523,532}											};

const POINTS		Map1_524[MAP1_524]=
								{	{525,451},{525,452},{525,453},{525,454},{525,455},{525,456},
									{524,452},{524,453},{524,454},{524,455},{524,456},{524,457},
									{523,453},{523,454},{523,455},{523,456},{523,457},{523,458},
									{522,454},{522,455},{522,456},{522,457},{522,458},{522,459},
									{521,455},{521,456},{521,457},{521,458},{521,459},{520,456},
									{520,457},{520,458}											};

const POINTS		Map1_E450[15]=
								{	{451,523},{451,524},{451,525},{450,524},{450,525},{450,526},
								    {449,525},{449,526},{449,527},{448,526},{448,527},{448,528},
									{447,527},{447,528},{447,529}								};
const POINTS		Map1_E527[15]=
								{	{528,527},{528,528},{528,529},{527,528},{527,529},{527,530},
									{526,529},{526,530},{526,531},{525,530},{525,531},{525,532},
									{524,531},{524,532},{524,533}								};
const POINTS		Map1_E524[15]=
								{   {525,453},{525,454},{525,455},{524,454},{524,455},{524,456},
									{523,455},{523,456},{523,457},{522,456},{522,457},{522,458},
									{521,457},{521,458},{521,459}								};
//________________________________________________________________________________________________
const POINTS		Map2_114[MAP2_114]=
								{	{112,166},{113,165},{114,164},{115,163},{112,165},{113,164},
									{114,163},{115,162},{112,164},{113,163},{114,162},{115,161},
									{112,163},{113,162},{114,161},{115,160},{112,163},{113,162},
									{114,161},{115,160},{112,162},{113,161},{114,160},{115,159},
									{112,161},{113,160},{114,159},{115,158},{111,160},{112,159},
									{113,158},{114,157}											};

const POINTS		Map2_119[MAP2_119]=
								{	{116,162},{117,161},{118,160},{119,159},{116,161},{117,160},
									{118,159},{119,158},{116,160},{117,159},{118,158},{116,159},
									{117,158},{118,157},{116,159},{117,158},{118,157},{116,158},
									{117,157},{118,156},{116,157},{117,156},{118,155},{115,156},
									{116,155},{117,154}											};

const POINTS		Map2_151[MAP2_151]=
								{	{149,130},{150,129},{151,128},{152,127},{149,129},{150,128},
									{151,127},{152,126},{149,128},{150,127},{151,126},{152,125},
									{149,127},{150,126},{151,125},{152,124},{149,126},{150,125},
									{151,124},{152,123},{149,125},{150,124},{151,123},{152,122},
									{149,124},{150,123},{151,122},{152,121}						};

const POINTS		Map2_156[MAP2_156]=
								{	{153,126},{154,127},{155,128},{156,127},{153,125},{154,124},
									{155,123},{156,122},{153,124},{154,123},{155,122},{153,123},
									{154,122},{155,121},{153,122},{154,121},{155,120},{153,121},
									{154,120},{155,119},{153,120},{154,119},{155,118}			};

const POINTS		Map2_274[MAP2_274]=
								{	{272,286},{273,285},{274,284},{275,283},{272,285},{273,284},
									{274,283},{275,282},{272,284},{273,283},{274,282},{275,281},
									{272,283},{273,282},{274,281},{275,280},{272,282},{273,281},
									{274,280},{275,279},{271,282},{272,281},{273,280},{274,279},
									{275,278},{271,281},{272,280},{273,279},{274,278},{275,277} };

const POINTS		Map2_279[MAP2_279]=
								{	{276,282},{277,281},{278,280},{279,279},{276,281},{277,280},
									{278,279},{279,278},{276,280},{277,279},{278,278},{276,279},
									{277,278},{278,277},{276,278},{277,277},{276,277},{277,276},
									{276,276}													};

const POINTS		Map2_E114[8]=
								{	{112,165},{113,164},{114,163},{115,162},{112,164},{113,163},
									{114,162},{115,161}											};

const POINTS		Map2_E119[8]=
								{	{116,161},{117,160},{118,159},{119,158},{116,160},{117,159},
									{118,158},{119,157}											};
const POINTS		Map2_E151[8]=
								{	{149,129},{150,128},{151,127},{152,126},{149,128},{150,127},
									{151,126},{152,125}											};

const POINTS		Map2_E156[9]=
								{	{153,125},{154,124},{155,123},{156,122},{157,121},{153,124},
									{154,123},{155,122},{156,121}								};

const POINTS		Map2_E274[8]=
								{	{272,285},{273,284},{274,283},{275,282},{272,284},{273,283},
									{274,282},{275,281}											};

const POINTS		Map2_E279[8]={	{276,281},{277,280},{278,279},{279,278},{276,280},{277,279},
									{278,278},{279,277}											};
// Tile Concern End_______________________________________________________________________________

const POINTS AttackPoint[3][4]	= { { {321,338}, {182,357}, {382,169}, {382,169} },		// Nation1
									{ {629,629}, {425,657}, {667,445}, {667,445} },		// Nation2
									{ {415,407}, {388,303}, { 11, 11}, { 16,126} } };	// Nation3
const POINTS DefencePoint[3]	= { {127,120},		// Nation1
									{449,456},		// Nation2
									{214,312}, };	// Nation3

const POINT  NEW_BYPoint={20,20};
const POINT  NEW_ZYPoint={231,20};
// Extern Function________________________________________________________________________________

extern tagNationInfo	NationInfo[MAX_NATION];			//국가정보 구조체..
extern bool			g_MainNationMap[MAX_NATION];			// 담당하는 나라 번호
extern void SaveNationInfo( int nation );		// 디비 데몬에 저장한다.
extern int ExistHe( char *name );//020903 lsw
extern void MakeSealStoneNumber(char* SrcString,int SealNo[4]);		// LTS NEW LOCALWAR
extern void LoadAIConfig();										// LTS AI
extern int g_LocalWarBegin;
extern void SendCMD_LOCALWAR_BEGIN2Client();		// LTSSupport.cpp
extern void SendCMD_LOCALWAR_END2Client();
extern void SendCMD_LOCALWAR_RESULT();
extern void GiveLife2SealStone();
extern void CheckAndFreeLocalWar(int NationIndex,int Result);					// LTS NEW LOCALWAR
extern CHARLIST* CheckServerId(WORD id);
extern void			SendCharacterCondition( CHARLIST*, int);
extern bool			NewSendGameToLoginChangeMapData( short int cn, WORD wMyAgentServerID, WORD wTargetServerID);
extern inline void	CallClient( short int id, short int type );
extern void			tempCount( CHARLIST *ch, int send  );
extern void			SquadRareReSet(int iBattleType, int cn,int iSquadItemStatus);
extern void			MovePc( short int cn, int x, int y);				// 011015 LTS
extern void			GetSquadRare(char *Squadrare,int squadNo/*char [3]*/);
extern int			CheckHandleByLTSSupport( t_packet *p, t_connection c[], int cn );	// 011112 LTS
extern void			DecLocalWarfieldMemberCount(const int cn); 
extern void			SendCMD_LOCALWAR_END();
extern void ClearAIData();		// LTS NEW AI
extern void SendCMD_REQUEST_DELIVERY(const WORD Port,t_packet *p, t_connection c[], const int cn);//020903 lsw
extern void ProcessCMD_REQUEST_WARFIELD_STATUS(t_packet *ReturnPacket,t_packet* ReceivePacket);		// 밑에선언 

extern int CheckMaxPoint(int nPoint1, int nPoint2); // 030506 kyo
extern int CheckMaxArrayPoint(int nIndex1, int nIndex2, unsigned short Array_1[3] ,unsigned short Array_2[3], unsigned short Array_3[3]);// 030506 kyo
extern cLocalWarfield* g_pLocalWarfield; // 030506 kyo
extern bool isLevelUp(CHARLIST*);


// Extern Function End____________________________________________________________________________

void SaveHonorWon(short int cn, int amount); //To save & log the won honor
void GiveHonorFromEvent(CHARLIST *pCaster, CHARLIST *pTarget, int event_type);

// Extern Variable _______________________________________________________________________________
extern HENV	hEnv;
extern HDBC hDBC;
// Extern Variable End____________________________________________________________________________

int NW_SQUADLEADER_LEVEL_MIN	=	60;
int NW_SQUADLEADER_LADDER_MIN	=	0;
int NW_COMMANDER_LEVEL_MIN		=	70;
int NW_COMMANDER_LADDER_MIN		=	2000;
int NW_COMMANDER_FAME_MIN		=	1200;

// Squad Condition
int NW_SQUAD1_LEVEL_MIN			=	60;
int NW_SQUAD1_LEVEL_MAX			=	255;
int NW_SQUAD2_LEVEL_MIN			=	60;
int NW_SQUAD2_LEVEL_MAX			=	255;
int NW_SQUAD3_LEVEL_MIN			=	60	;
int NW_SQUAD3_LEVEL_MAX			=	255	;
int NW_SQUAD4_LEVEL_MIN			=	60	;
int NW_SQUAD4_LEVEL_MAX			=	255	;

int NW_SQUAD1_MAN_MIN			=	5;
int NW_SQUAD1_MAN_MAX			=	70;
int NW_SQUAD2_MAN_MIN			=	5;
int NW_SQUAD2_MAN_MAX			=	70;
int NW_SQUAD3_MAN_MIN			=	5;
int NW_SQUAD3_MAN_MAX			=	70;
int NW_SQUAD4_MAN_MIN			=	5;
int NW_SQUAD4_MAN_MAX			=	70;

int NW_NATION_VOTE_MIN			=	0;

void InitNationValues()
{
	if(LocalMgr.IsFreeBeta())
	{
		NW_NATION_VOTE_MIN				=5;
	}
	else
	{
		NW_NATION_VOTE_MIN				=10;//50;
	}

	if(LocalMgr.IsAbleNation(CHINA|TAIWAN|HONGKONG))
	{
		NW_SQUADLEADER_LEVEL_MIN	=	60;
		NW_SQUADLEADER_LADDER_MIN	=	0;
		NW_COMMANDER_LEVEL_MIN		=	70;
		NW_COMMANDER_LADDER_MIN		=	2000;
		NW_COMMANDER_FAME_MIN		=	1200;

		NW_SQUAD1_LEVEL_MIN			=	60;
		NW_SQUAD1_LEVEL_MAX			=	255;
		NW_SQUAD2_LEVEL_MIN			=	60;
		NW_SQUAD2_LEVEL_MAX			=	255;
		NW_SQUAD3_LEVEL_MIN			=	60	;
		NW_SQUAD3_LEVEL_MAX			=	255	;
		NW_SQUAD4_LEVEL_MIN			=	60	;
		NW_SQUAD4_LEVEL_MAX			=	255	;

		NW_SQUAD1_MAN_MIN			=	5;
		NW_SQUAD1_MAN_MAX			=	70;
		NW_SQUAD2_MAN_MIN			=	5;
		NW_SQUAD2_MAN_MAX			=	70;
		NW_SQUAD3_MAN_MIN			=	5;
		NW_SQUAD3_MAN_MAX			=	70;
		NW_SQUAD4_MAN_MIN			=	5;
		NW_SQUAD4_MAN_MAX			=	70;
	}
	//< BBD 031214 need thai seting
	else if(LocalMgr.IsAbleNation(THAI))
	{
		NW_SQUADLEADER_LEVEL_MIN	=	20;
		NW_SQUADLEADER_LADDER_MIN	=	0;
		NW_COMMANDER_LEVEL_MIN		=	70;
		NW_COMMANDER_LADDER_MIN		=	2000;
		NW_COMMANDER_FAME_MIN		=	1200;

		NW_SQUAD1_LEVEL_MIN			=	20;
		NW_SQUAD1_LEVEL_MAX			=	50;
		NW_SQUAD2_LEVEL_MIN			=	40;
		NW_SQUAD2_LEVEL_MAX			=	70;		
		NW_SQUAD3_LEVEL_MIN			=	60;		
		NW_SQUAD3_LEVEL_MAX			=	90;		
		NW_SQUAD4_LEVEL_MIN			=	80;		
		NW_SQUAD4_LEVEL_MAX			=	255;	

		NW_SQUAD1_MAN_MIN			=	5;
		NW_SQUAD1_MAN_MAX			=	30;		
		NW_SQUAD2_MAN_MIN			=	5;
		NW_SQUAD2_MAN_MAX			=	60;		
		NW_SQUAD3_MAN_MIN			=	5;
		NW_SQUAD3_MAN_MAX			=	80;		
		NW_SQUAD4_MAN_MIN			=	5;
		NW_SQUAD4_MAN_MAX			=	90;		
	}
	//> BBD 031214 need thai seting
	else
	{
		NW_SQUADLEADER_LEVEL_MIN	=	20;
		NW_SQUADLEADER_LADDER_MIN	=	0;
		NW_COMMANDER_LEVEL_MIN		=	70;
		NW_COMMANDER_LADDER_MIN		=	2000;
		NW_COMMANDER_FAME_MIN		=	1200;

		NW_SQUAD1_LEVEL_MIN			=	20;
		NW_SQUAD1_LEVEL_MAX			=	50;
		NW_SQUAD2_LEVEL_MIN			=	40;
		NW_SQUAD2_LEVEL_MAX			=	70;		
		NW_SQUAD3_LEVEL_MIN			=	60;		
		NW_SQUAD3_LEVEL_MAX			=	90;		
		NW_SQUAD4_LEVEL_MIN			=	80;		
		NW_SQUAD4_LEVEL_MAX			=	255;	

		NW_SQUAD1_MAN_MIN			=	5;
		NW_SQUAD1_MAN_MAX			=	80;		
		NW_SQUAD2_MAN_MIN			=	5;
		NW_SQUAD2_MAN_MAX			=	70;		
		NW_SQUAD3_MAN_MIN			=	5;
		NW_SQUAD3_MAN_MAX			=	60;		
		NW_SQUAD4_MAN_MIN			=	5;
		NW_SQUAD4_MAN_MAX			=	50;		
	}
}

// Usage : Checking Nation Server
bool isNationManageServer()
{
	if (g_wMapServerPort==NATION_MANAGE_SERVER) 
	{
		return true;
	}
	else 
	{
		return false;
	}
	return false;
}


int LoadNewWarMaxMin()
{
	// 030919 HK YGI
//	char	NetworkDir[MAX_PATH];
//	char	MapServerConfigFileName[MAX_PATH];
/*
	if (GetPrivateProfileString("Network","path","",NetworkDir,MAX_PATH,MAP_SERVER_INI_)<=0)
	{
		MyLog(0,"Error!! Check...  Mapserver.ini Path Setting..!!");
		return false;
	}
	sprintf(MapServerConfigFileName,"%s/data/MapServerConfig.ini",NetworkDir);
*/// 030919 HK YGI
	char tempString[MAX_PATH];

	for (int i=0;i<MAX_NEW_WARFIELD;i++)
	{
		MyLog(LOG_NORMAL, "sprintf_s(tempString, sizeof(tempString),'LevelMin%d',i);"); //Eleval 22/08/09 - To debug Nation War (have to be removed later)
		sprintf_s(tempString, sizeof(tempString),"LevelMin%d",i);
		MoveLevelMin[i]=GetPrivateProfileInt("NewWarMove",tempString,10,MapServerConfigFileName);

		MyLog(LOG_NORMAL, "sprintf_s(tempString, sizeof(tempString),'LevelMax%d',i);"); //Eleval 22/08/09 - To debug Nation War (have to be removed later)
		sprintf_s(tempString, sizeof(tempString),"LevelMax%d",i);
		MoveLevelMax[i]=GetPrivateProfileInt("NewWarMove",tempString,255,MapServerConfigFileName);
	}

	return true;
}

// Usage : Checking Nation Warfield Server

bool isNationWarfieldServer()
{
	if ((g_wMapServerPort==WARFIELD_PORT1)||
		(g_wMapServerPort==WARFIELD_PORT2)||
		(g_wMapServerPort==WARFIELD_PORT3)) 
	{
		return true;
	}
	else
	{
		return false;
	}
	return false;
}

bool isNewWarfieldServer()
{
	if (g_wMapServerPort<BASE_NEW_WARFIELD_PORT||g_wMapServerPort>=(BASE_NEW_WARFIELD_PORT+MAX_NEW_WARFIELD))
		return false;
	return true;
}

int CheckJoinReinforce(bool type=true)			// 일반전쟁 true
{
	if (!type)
		return 0;

	if ((g_pWarfieldStatus[0].Possession==g_pWarfieldStatus[1].Possession)&&
		(g_pWarfieldStatus[1].Possession==g_pWarfieldStatus[2].Possession))
	{
		return g_pWarfieldStatus[0].Possession;
	}
	return 0;
}

int GetYL_JoinNation()	// 011020 LTS
{
	if ((g_pWarfieldStatus[0].Possession==g_pWarfieldStatus[1].Possession)&&
		(g_pWarfieldStatus[1].Possession==g_pWarfieldStatus[2].Possession))
	{
		if (g_pWarfieldStatus[0].Possession==NW_BY) return NW_ZY;
		else return NW_BY;
	}
	return 0;
}

// Very often call Function

int CheckNationStatus(const int type,const int StatusNo) // type 0 : EveryServer 1 : NationManageServer
{
	int WarfieldNo=-1;
	if (type)
	{
		if (g_pNation->GetWarfieldStatus(0)==StatusNo) WarfieldNo=0;
		if (g_pNation->GetWarfieldStatus(1)==StatusNo) WarfieldNo=1;
		if (g_pNation->GetWarfieldStatus(2)==StatusNo) WarfieldNo=2;
	}
	else
	{
		if (g_pWarfieldStatus[0].Status==StatusNo) WarfieldNo=0;
		if (g_pWarfieldStatus[1].Status==StatusNo) WarfieldNo=1;
		if (g_pWarfieldStatus[2].Status==StatusNo) WarfieldNo=2;
	}
    return WarfieldNo;
}

bool CheckWarLoopProcess()		// 전쟁 루프가 진행되는지 확인한다.
{
	if (isNationManageServer())
	{
		if (g_pNation->GetWarfieldStatus(0)>NW_PEACE) return true;
		if (g_pNation->GetWarfieldStatus(1)>NW_PEACE) return true;
		if (g_pNation->GetWarfieldStatus(2)>NW_PEACE) return true;
		if (g_pNation->GetNewWarfieldStatus(0)>NW_PEACE) return true;

	}
	else
	{
		if (g_pWarfieldStatus[0].Status>NW_PEACE) return true;
		if (g_pWarfieldStatus[1].Status>NW_PEACE) return true;
		if (g_pWarfieldStatus[2].Status>NW_PEACE) return true;
		if (g_pNewWarfieldStatus[0]>NW_PEACE) return true;
	}
	return false;
}

int ReturnWarfieldNoByWarLoopProcess()		// CheckWarLoopProcess와 쌍으로 쓰인다.. 포함해도 된다..
{
	if (isNationManageServer())
	{
		if (g_pNation->GetWarfieldStatus(0)>NW_PEACE) return 0;
		if (g_pNation->GetWarfieldStatus(1)>NW_PEACE) return 1;
		if (g_pNation->GetWarfieldStatus(2)>NW_PEACE) return 2;
		if (g_pNation->GetNewWarfieldStatus(0)>NW_PEACE) return 3;
	}
	else
	{
		if (g_pWarfieldStatus[0].Status>NW_PEACE) return 0;
		if (g_pWarfieldStatus[1].Status>NW_PEACE) return 1;
		if (g_pWarfieldStatus[2].Status>NW_PEACE) return 2;
		if (g_pNewWarfieldStatus[0]>NW_PEACE) return 3;
	}
	return -1;
}

bool isAttacker(int WarfieldNo,t_connection c[],int cn)		// 어태커와 디펜서를 판단하는 근거는 모든 맵서버의 g_pWarfieldStatus[].Possession에 있다.
{
	int Nationality=c[cn].chrlst.name_status.nation;		// 음.. 용병의 참여에 따라...
	if (NW_YL==Nationality)									// 일스이면..
	{
		Nationality=c[cn].chrlst.NWCharacter.YL_JoinNation; //나라번호가 날아온다
	}
	if (g_pWarfieldStatus[WarfieldNo].Possession==Nationality) return false;
	return true;
}

bool isAttacker(CHARLIST* pCaster)
{
	int Nationality=pCaster->name_status.nation;			// 음.. 용병의 참여에 따라...
	if (NW_YL==Nationality)									// 일스이면..
	{
		Nationality=pCaster->NWCharacter.YL_JoinNation;		//나라번호가 날아온다
	}
	if (g_pWarfieldStatus[g_wMapServerPort-BASE_WARFIELD_PORT].Possession==Nationality) return false;
	return true;
}

bool isAttacker(int WarfieldNo,int Nation)					// 일스의 경우 무조건 공격자로 판정을 받는다.
{
	if (g_pWarfieldStatus[WarfieldNo].Possession==Nation) return false;
	return true;
}

bool IsWar()
{
	if (isNationWarfieldServer())
	{
		if (g_pWarfield->GetStatus() == NW_WAR)
		{
			return true;//020903 lsw
		}
	}

	if (isNewWarfieldServer())
	{
		if (g_pNewWarfield->GetWarfieldStatus() == NW_WAR)
		{
			return true;
		}
	}

	return false;
}

bool isMySquad(CHARLIST* pCaster,CHARLIST* pTarget)
{
	if (!isNationWarfieldServer()) return false;
	if (g_pWarfield->GetStatus()!=NW_WAR) return false;
	if (pCaster->name_status.nation==pTarget->name_status.nation)	// 같은 나라이다 
	{
		if (pCaster->NWCharacter.SquadNo==pTarget->NWCharacter.SquadNo)		// 부대 번호가 같다.
		{
			if (pCaster->NWCharacter.SquadNo!=0) return true;
		}
	}
	return false;
}

void ClearNationWar_Exp()		// LTS NEW LOCALWAR
{
	SAFE_DELETE_ARRAY(g_pNationWar_Exp);
}

bool LoadNationWar_Exp()		// LTS NEW LOCALWAR
{
	HSTMT	hStmt=NULL;
	RETCODE	ret;
	SWORD	nClos;
	char	query_stmt[80]={0,};
	SDWORD	cbValue;
	int		FieldCount;
	int		tempExpGrade;

	SQLAllocStmt(hDBC,&hStmt);

	wsprintf(query_stmt,"select Count(Exp_Grade) as FieldCount from NationWar_Exp");   
	ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"NationWar_Exp Table Query Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	SQLNumResultCols(hStmt,&nClos);

	ret=SQLFetch(hStmt);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"NationWar_Exp Table Fetch Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}
	ret=SQLGetData(hStmt,1,SQL_C_SLONG,&FieldCount,sizeof(int),&cbValue);	// 필드카운트 구하기
	if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
	{
		MyLog(0,"NationWar_Exp Table SQL Return Error(%d)!!",ret);
	    SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}                
    SQLFreeStmt(hStmt,SQL_DROP);

	SQLAllocStmt(hDBC,&hStmt);

	g_pNationWar_Exp=new int[FieldCount+1];					// 메모리 할당	// 0부터 시작하므로 
	if (!g_pNationWar_Exp) 
	{
		MyLog(0,"Memory Allocation Error for NationWar Exp Table!!");
		return false;
	}

	wsprintf(query_stmt,"select * from NationWar_Exp order by Exp_Grade");   
	ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"NationWar_Exp Table Query Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}
	SQLNumResultCols(hStmt,&nClos);
	ret=SQLFetch(hStmt);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"NationWar_Exp Table Fetch Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}
	while (ret==SQL_SUCCESS)
	{
		ret=SQLGetData(hStmt,1,SQL_C_SLONG,&tempExpGrade,sizeof(int),&cbValue);
		if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
		{
			MyLog(0,"NationWar_Exp Table SQL Return Error(%d)!!",ret);
		    SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}                
		ret=SQLGetData(hStmt,3,SQL_C_SLONG,&g_pNationWar_Exp[tempExpGrade],sizeof(int),&cbValue);
		if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
		{
			MyLog(0,"NationWar_Exp Table SQL Return Error(%d)!!",ret);
		    SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}                
		ret=SQLFetch(hStmt);
	}
    SQLFreeStmt(hStmt,SQL_DROP);
	return true;
}

void ReLoadNationData()
{
	if (isNationWarfieldServer()) g_pWarfield->InitWarLoopDefine();
}

bool CanDestroyTarget(CHARLIST* pCaster,CHARLIST* pTarget)		// CAttackMagic::Result
{
	if (isNationWarfieldServer())
	{
		if (isAttacker(pCaster))								// Attacker	// 수호석 성문의 경우
		{
			if (64==pTarget->SprNo)								// 보스 수호석 
			{
				for (int i=0;i<NW_MAX_NPC;i++)					// NPC전체를 순회 (20개)
				{
					if (63==g_pWarfield->GetWeaponIndex(i))		//보조 수호석
					{
						if (1==g_pWarfield->GetWeaponStatus(i)) 
						{
							return false;  //살아 있다면 
						}
					}
				}
				return true;									//다죽었다면..
			}
			else 
			{
				return	true;
			}// 보스 수호석이 아니면 깰수 있다.
		}
		else 
		{
			return false;
		}// Defencer     //수호석 성문의  경우 깰수 없다
	}
	if (isNewWarfieldServer())
	{
		return g_pNewWarfield->CanAttackGuard(pCaster,pTarget);
	}
	return false;
}

void CountNationDeath(CHARLIST* pCaster,CHARLIST* pTarget)
{
	if (!isNationWarfieldServer()) return;
	if (pTarget->name_status.nation==NW_YL)
	{
		if (isAttacker(g_pWarfield->GetWarfieldCode(),pTarget->NWCharacter.YL_JoinNation)) 
			 g_pWarfield->IncKilledCount(NW_ATTACKER);
		else g_pWarfield->IncKilledCount(NW_DEFENCER);
	}
	else
	{
		if (isAttacker(g_pWarfield->GetWarfieldCode(),pTarget->name_status.nation))
			 g_pWarfield->IncKilledCount(NW_ATTACKER);
		else g_pWarfield->IncKilledCount(NW_DEFENCER);
	}

#ifdef WARFIELD_SERVER_
	if(stricmp(connections[pCaster->GetServerID()].ip_address, connections[pCaster->GetServerID()].ip_address) == 0)
		return;
	GiveHonorFromEvent(pCaster, pTarget, PKA_KILL);
#endif
}

void UpdateWarTime()			// 011015 LTS 
{
	struct tm *today;
	time_t lTime;
	__int64 curr_time;
	__int64 starttime;

	__int64 year,mon,yday,day,hour,min,sec;

	time(&lTime);
	today = localtime(&lTime);

	year = today->tm_year + 1900;
	mon  = today->tm_mon;
	yday = today->tm_yday;
	day  = today->tm_mday;
#ifdef BUFF_BUG_CHECKS // Finito 24/08/07 buff bug checks to take away 12 hours when time changes
	if (today->tm_hour - g_hour >= 12  && g_hour != -1)
	{
		hour = today->tm_hour - 12;
	}
	else
	{
		hour = today->tm_hour;
	}
#else
	hour = today->tm_hour;
#endif
	min  = today->tm_min;
	sec  = today->tm_sec;
		
		//-----------------------------------------------

	curr_time = ((__int64)(g_year - 1) * 31536000) 
			      + ((g_yday) * 86400) 
			      + (g_hour * 3600) 
			      + (g_min * 60) 
			      + g_sec;

	starttime  = ((__int64)(1998) * 31536000) 
			      + ((365-61) * 86400) 
			      + (24 * 3600) 
			      + (0 * 60) 
			      + 0;
	g_WarTime = (curr_time - starttime );
	//MyLog(0,"Time : %ld",g_WarTime);		// LTS TEMP
};

// ScripFunction _____________________________________________________________
bool CheckDefence(int cn)
{
	if (!isNationWarfieldServer()) 
	{
		return false;			//문제가 있다. 공격자가 아니다.
	}
	
	if (isAttacker(g_wMapServerPort-BASE_WARFIELD_PORT,connections,cn)) 
	{
		return false;
	}
	else 
	{
		return true;
	}
	return false;
}

bool CanGoBonus()
{
	if (isNationWarfieldServer())
	{
		return g_pWarfield->isBonusTime();
	}

	if (isNewWarfieldServer())
	{
		if (g_pNewWarfield->GetWarfieldStatus() == NW_SOPEN)
		{
			return true;
		}
	}

	return false;
}
// ___________________________________________________________________________

int CheckNationStatus(const int StatusNo)
{
	int WarfieldNo=-1;
	if (isNationManageServer())
	{
		if (g_pNation->GetWarfieldStatus(0)==StatusNo)
		{
			WarfieldNo=0;
		}
		if (g_pNation->GetWarfieldStatus(1)==StatusNo) 
		{
			WarfieldNo=1;
		}
		if (g_pNation->GetWarfieldStatus(2)==StatusNo) 
		{
			WarfieldNo=2;
		}
	}
	else
	{
		if (g_pWarfieldStatus[0].Status==StatusNo) 
		{
			WarfieldNo=0;
		}
		if (g_pWarfieldStatus[1].Status==StatusNo) 
		{
			WarfieldNo=1;
		}
		if (g_pWarfieldStatus[2].Status==StatusNo) 
		{
			WarfieldNo=2;
		}
	}
    return WarfieldNo;
}

void InitConnectionsSquadData(const int cn)// 커낵션의 NWCharacter를 초기화 한다.
{
	if (connections[cn].chrlst.name_status.nation!=NW_YL)				// 011020 LTS	// 일스이면 AM_I_DEFEAT에서 초기화 한다 
	memset(&connections[cn].chrlst.NWCharacter,0,sizeof(DWORD));		// 010915 LTS
}


void DeleteSquadMember(const int cn)    // DragonServer.cpp Extern이기 때문에 클래스에 포함시키지 않았다.
{
	if (isNationWarfieldServer())
	{
		g_pWarfield->DeleteSquadMember(cn);
		g_pWarfield->DecNationMemberCount(connections[cn].chrlst.name_status.nation);
	}
	if (isNewWarfieldServer())
	{
		g_pNewWarfield->DeleteMember(cn);
	}
}


bool LoadWarfieldPossession()		// 스타트업시 DB에서 WarfieldPossession의 값을 읽어온다.
{
	HSTMT	hStmt=NULL;
	RETCODE	ret;
	SWORD	nClos;
	char	query_stmt[80]={0,};
	SDWORD	cbValue;

	SQLAllocStmt(hDBC,&hStmt);

	wsprintf(query_stmt,"select WarfieldCode,NationCode from WarfieldTBL order by WarFieldCode");   

	ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
 
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"LoadWarfieldPossession(), WarField Table Query Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	SQLNumResultCols(hStmt,&nClos);

	ret=SQLFetch(hStmt);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"LoadWarfieldPossession(), WarField Table Fetch Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	int index=0;

	while (ret==SQL_SUCCESS)
	{
		ret=SQLGetData(hStmt,2,SQL_C_SLONG,&g_pWarfieldStatus[index].Possession,sizeof(int),&cbValue);
		if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
		{
			MyLog(0,"LoadWarfieldPossession(), Warfield Table SQL Return Error(%d)!!",ret);
			SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}                
		index++;
		if (index>=3) break;
		ret=SQLFetch(hStmt);
	}
    SQLFreeStmt(hStmt,SQL_DROP);
	return true;
}

// Where : Call From Main.cpp InitDrmapServerDatas()
// Usage : Initializing Nation War System



bool InitNationSystem()
{
	SERVER_DATA	*pData=g_pServerTable->GetOwnServerData();
	g_wMapServerPort=pData->wPort;

	if (isNationManageServer())
	{
		g_pNation=new cNation;
		if (g_pNation==NULL) 
		{
			return false;
		}
	}
	if (isNationWarfieldServer())
	{
		g_pWarfield=new cWarfield;
		if (g_pWarfield==NULL) 
		{
			return false;
		}
	}
	if (isNewWarfieldServer())
	{
		g_pNewWarfield=new CNewWarfield;
		if (g_pNewWarfield==NULL)
		{
			return false;
		}
		if (!g_pNewWarfield->InitWarfield())
		{
			return false;
		}

	}

	//LoadAIConfig(); // 030919 HK YGI

	if (!LoadNewWarMaxMin())
		return false;
	if (!LoadNationWar_Exp()) 
	{
		return false;						// LTS NEW LOCALWAR
	}
	return LoadWarfieldPossession();
}

// Where : Call From SealStone.cpp CheckNationWar()
// Usage : Update Nation War System

bool UpdateNationWarData()
{
	if (isNationManageServer())
	{
		if (g_pNation==NULL)
		{
			MyLog(0,"g_pNation was not Initiaized..!!");
			return false;
		}
		g_pNation->UpdateStatus();
		return true;
	}
	if (isNationWarfieldServer())
	{
		if (g_pWarfield==NULL)
		{
			MyLog(0,"g_pWarLoop was not Intialized..!!");
			return false;
		}
		g_pWarfield->UpdateWarfieldStatus();
	}
	if (isNewWarfieldServer())
	{
		if (g_pNewWarfield==NULL)
		{
			JustMsg("g_pNewWarfield Not Inialized..!!");
			return false;
		}
		g_pNewWarfield->Update();
	}
	return true;
}

// Where : Call From MapServer.cpp EndMapServer()
// Usage : Close Nation War System
void CloseNationSystem()		// LTS 대만//020205 lsw
{
	SAFE_DELETE(g_pWarfield);
	SAFE_DELETE(g_pNation);
	SAFE_DELETE(g_pNewWarfield);

	ClearNationWar_Exp();			// LTS NEW LOCALWAR
	ClearAIData();					// LTS NEW AI
}

void SendCMD_NW_MAP_MOVE_FAIL(t_connection c[],const int cn,const int Type)		// 011213 LTS
{
	t_packet packet;
	packet.h.header.type=CMD_NW_MAP_MOVE_FAIL;   // 맵이동이 안됩니다.
	packet.u.NationWar.CommonDataC.Data=Type;
	packet.h.header.size=sizeof(t_CommonDataC);
	QueuePacket(c,cn,&packet,1);
}

void SendCMD_CHECK_WARFIELD_NATION_MEMBERCOUNT(t_packet *p,t_connection c[],const int cn)
{
	t_packet packet;
	packet.h.header.type=CMD_CHECK_WARFIELD_NATION_MEMBERCOUNT;
	packet.u.NationWar.CheckNationMemberCount.Nation=c[cn].chrlst.name_status.nation;
	packet.u.NationWar.CheckNationMemberCount.WarfieldNo=p->u.NationWar.NWMapMove.WarfieldNo;
	packet.h.header.size=sizeof(t_CheckNationMemberCount);

	int WarfieldNo=p->u.NationWar.NWMapMove.WarfieldNo;
	if (WarfieldNo>=0&&WarfieldNo<=2)
		SendCMD_REQUEST_DELIVERY(BASE_WARFIELD_PORT+p->u.NationWar.NWMapMove.WarfieldNo,&packet,c,cn);
	else
	if (WarfieldNo>=3)
		SendCMD_REQUEST_DELIVERY(BASE_NEW_WARFIELD_PORT+WarfieldNo-3,&packet,c,cn);
}

void RecvCMD_NW_MAP_MOVE(t_packet *p,t_connection c[],const int cn)
{
	LPCHARLIST ch = ::CheckServerId(cn);
	if(!ch){return;}
	const int WarfieldNo=p->u.NationWar.NWMapMove.WarfieldNo;
	int WarfieldStatus = 0;

	if (isNationManageServer())	
	{
		if (WarfieldNo>=0&&WarfieldNo<=2)
		{
			WarfieldStatus=g_pNation->GetWarfieldStatus(WarfieldNo);	// 전쟁터 상태를 얻어온다.
		}
		else if (WarfieldNo>=3)
		{
			WarfieldStatus=g_pNation->GetNewWarfieldStatus(WarfieldNo);	// 전쟁터 상태를 얻어온다.
		}
	}
	else 
	{
		if (WarfieldNo>=0&&WarfieldNo<=2)
		{
			WarfieldStatus=g_pWarfieldStatus[WarfieldNo].Status;
		}
		else if (WarfieldNo>=3)
		{
			/* // 030515 kyo
			if (isNewWarfieldServer())	// 030509 kyo		//전쟁서버이면
			{
				WarfieldStatus=g_pNewWarfield->GetWarfieldStatus();				
			}
			else
			*/
			WarfieldStatus=g_pNewWarfieldStatus[WarfieldNo-3];
		}
	}

	if (ch->GetLevel() <= ENABLE_NATIONWAR_LEVEL)
	{	//< CSD-030806 : 전쟁터 이동 레벨제한
		SendCMD_NW_MAP_MOVE_FAIL(c,cn,1); 
		return; 
	}	//> CSD-030806

	if (WarfieldStatus!=NW_WAR) {SendCMD_NW_MAP_MOVE_FAIL(c,cn,4); return; }	// 전쟁 준비기간이 아니면

	if (WarfieldNo>=0&&WarfieldNo<=2)
	{
		if (ch->name_status.nation!=NW_YL)
		{
			if (!ch->IsReporter())											// 리포터가 아니면
			{
				if (!ch->NWCharacter.SquadNo)										//부대번호가 없다면
				{ 
					if (!ch->NWCharacter.isCommander) {SendCMD_NW_MAP_MOVE_FAIL(c,cn,2); return; }	// 사령관리 아니라면 리턴
				}
			}
		}  //조건 검사가 끝났다.
	}
	else
	if (WarfieldNo>=3)
	{
		if (ch->GetLevel() < MoveLevelMin[WarfieldNo - 3] || ch->GetLevel() > MoveLevelMax[WarfieldNo - 3])
		{	//< CSD-030806
			SendCMD_NW_MAP_MOVE_FAIL(c,cn,1); 
			return;
		}	//> CSD-030806
	}

	SendCMD_CHECK_WARFIELD_NATION_MEMBERCOUNT(p,c,cn);
}

void CallNWMapMove(const int WarfieldNo,t_connection c[],const int cn)  //클라이언트의 인터페이스 맵이동 // 011217 LTS
{
	char tempMapName[20] = {0,};
	int WarfieldStatus;
	if (isNationManageServer())
	{
		if (WarfieldNo>=0&&WarfieldNo<=2)
			WarfieldStatus=g_pNation->GetWarfieldStatus(WarfieldNo);	// 전쟁터 상태를 얻어온다.
		else
		if (WarfieldNo>=3)
			WarfieldStatus=g_pNation->GetNewWarfieldStatus(WarfieldNo-3);	// 전쟁터 상태를 얻어온다.

	}
	else 
	{
		if (WarfieldNo>=0&&WarfieldNo<=2)
			WarfieldStatus=g_pWarfieldStatus[WarfieldNo].Status;
		else
		if (WarfieldNo>=3)
		{
			/* // 030515 kyo
			if (isNewWarfieldServer())	// 030509 kyo		//전쟁서버이면
			{
				WarfieldStatus=g_pNewWarfield->GetWarfieldStatus();
			}
			else
			*/
			WarfieldStatus=g_pNewWarfieldStatus[WarfieldNo-3];
				
		}
	}

	MyLog(LOG_NORMAL, "sprintf_s(tempMapName, sizeof(tempString),'Nation1-5');"); //Eleval 22/08/09 - To debug Nation War (have to be removed later)

	if (WarfieldNo==0) sprintf_s(tempMapName, sizeof(tempMapName),"Nation1");
	if (WarfieldNo==1) sprintf_s(tempMapName, sizeof(tempMapName),"Nation2");
	if (WarfieldNo==2) sprintf_s(tempMapName, sizeof(tempMapName),"Nation3");
	if (WarfieldNo==3) sprintf_s(tempMapName, sizeof(tempMapName),"Nation4");

	if (WarfieldStatus!=NW_WAR)
	{
		if(WarfieldNo>=3)			// 평화시 어떻게 하지??? ㅡ.ㅡ
		{
			/*int Nation=connections[cn].chrlst.name_status.nation;
			if (Nation==NW_YL)
				Nation=connections[cn].chrlst.NWCharacter.YL_JoinNation;
			switch(Nation)
			{
			case NW_BY : MapMove(cn,tempMapName,NEW_BYPoint.x,NEW_BYPoint.y); break;
			case NW_ZY : MapMove(cn,tempMapName,NEW_ZYPoint.x,NEW_ZYPoint.y); break;
			}*/
			SendCMD_NW_MAP_MOVE_FAIL(c,cn,3);	// 011213 LTS
		}
		else
		{
			if (!isAttacker(WarfieldNo,c,cn))
			{
				POINTS tempPoint = DefencePoint[WarfieldNo];
				MapMove(cn,tempMapName,tempPoint.x,tempPoint.y);
			}
			else SendCMD_NW_MAP_MOVE_FAIL(c,cn,3);	// 011213 LTS
		}
	}
	else
	{
		if (WarfieldNo>=3)
		{
			int Nation=connections[cn].chrlst.name_status.nation;
			if (Nation==NW_YL)
				Nation=connections[cn].chrlst.NWCharacter.YL_JoinNation;
			switch(Nation)
			{
			case NW_BY : MapMove(cn,tempMapName,NEW_BYPoint.x,NEW_BYPoint.y); break;
			case NW_ZY : MapMove(cn,tempMapName,NEW_ZYPoint.x,NEW_ZYPoint.y); break;
			}
		}
		else
		{
			if (isAttacker(WarfieldNo,c,cn))
			{
				int LoadPoint=g_pLoadingPoint[WarfieldNo][c[cn].chrlst.NWCharacter.SquadNo]-1;	
				if (LoadPoint<0||LoadPoint>3) LoadPoint=0;
				POINTS tempPoint = AttackPoint[WarfieldNo][LoadPoint];	// 로딩포인트가 1에서 시작된다면 에러 발생
				MapMove(cn,tempMapName,tempPoint.x,tempPoint.y);
			}
			else
			{		//방어자이다...
				POINTS tempPoint = DefencePoint[WarfieldNo];
				MapMove(cn,tempMapName,tempPoint.x,tempPoint.y);
			}
		}
	}
}

void FindCommander(const char* CommanderName) // 맵서버에 해당이름의 유저가 있는지 확인한다.
{	//< CSD-CN-031213
	CUserManager::HASH_USER mpUser = g_pUserManager->GetUserSet();

	for (CUserManager::ITOR_USER i = mpUser.begin(); i != mpUser.end(); ++i)
	{
		const int idUser = i->second;
		CHARLIST* pUser = CheckServerId(idUser);
		
		if (pUser != NULL)
		{
			if (0 == strcmp(connections[idUser].name, CommanderName))
			{
				pUser->NWCharacter.isCommander = 1;
	
				t_packet packet;
				packet.h.header.type = CMD_YOU_ARE_SQUAD_COMMANDER;
				packet.h.header.size = sizeof(t_CommonDataC);

				if (NW_YL == pUser->name_status.nation) 
				{
					packet.u.NationWar.CommonDataC.Data = CheckJoinReinforce();
				}
				else 
				{
					packet.u.NationWar.CommonDataC.Data = 0;
				}
				
				QueuePacket(connections, idUser, &packet, 1);
				break;
			}
		}
	}
}	//> CSD-CN-031213

void SendPacket2NWManager(t_packet* p)
{
	g_pServerTable->SendRajaPacketToOtherMapServer( NATION_MANAGE_SERVER, (char *)p, p->h.header.size+sizeof(t_header) );
}

// Send Packet to Special Map Server
void SendPacket2NWMap(const WORD Port,t_packet* p)				
{
	g_pServerTable->SendRajaPacketToOtherMapServer( Port, (char *)p, p->h.header.size+sizeof(t_header) );
//	g_pServerTable->SendRajaPacketToOtherMapServer( (WORD)Port, (char *)p, p->h.header.size+sizeof(t_header) );
}

// Send Packet to All Map Servers
void SendPacket2Maps(t_packet* p)						
{
	g_pServerTable->BroadCastToEveryServer((char *)p, p->h.header.size+sizeof(t_header),SERVER_TYPE_MAP);
}

// Send Packet to Parts of Map Servers , Parameter Dependence
void SendPacket2NationMaps(const int Nation,t_packet* p)		
{
	for(int i=0 ; i< MAX_MAP_;i++)
	{
		if (Nation == MapInfo[i].nation)//020903 lsw
		g_pServerTable->SendRajaPacketToOtherMapServer( (WORD)MapInfo[i].port, (char *)p, p->h.header.size+sizeof(t_header) );
	}
}

// Send Several Packet to Parts of Map Servers
void SendPacket2NationMaps(const int Nation1,t_packet* p1,const int Nation2,t_packet* p2)//020903 lsw
{
	for(int i=0 ; i< MAX_MAP_;i++)
	{
		if (Nation1 ==MapInfo[i].nation)
		{
			g_pServerTable->SendRajaPacketToOtherMapServer( (WORD)MapInfo[i].port, (char *)p1, p1->h.header.size+sizeof(t_header) );
		}
		if (Nation2 ==MapInfo[i].nation)
		{
			g_pServerTable->SendRajaPacketToOtherMapServer( (WORD)MapInfo[i].port, (char *)p2, p2->h.header.size+sizeof(t_header) );
		}

	}
}
// Send Several Packet to Parts of Map Servers
void SendPacket2NationMaps(const int Nation1,t_packet* p1,const int Nation2,t_packet* p2,const int Nation3,t_packet* p3)	//020903 lsw
{
	for(int i=0 ; i< MAX_MAP_;i++)
	{
		if (Nation1==MapInfo[i].nation)
		{
			g_pServerTable->SendRajaPacketToOtherMapServer( (WORD)MapInfo[i].port, (char *)p1, p1->h.header.size+sizeof(t_header) );
		}
		if (Nation2==MapInfo[i].nation)
		{
			g_pServerTable->SendRajaPacketToOtherMapServer( (WORD)MapInfo[i].port, (char *)p2, p2->h.header.size+sizeof(t_header) );
		}
		if (Nation3==MapInfo[i].nation)
		{
			g_pServerTable->SendRajaPacketToOtherMapServer( (WORD)MapInfo[i].port, (char *)p3, p3->h.header.size+sizeof(t_header) );
		}
	}
}

void SendPacket2NationClients(const int Nation,t_packet* p)	
{	//< CSD-CN-031213
	CUserManager::HASH_USER user = g_pUserManager->GetUserSet();

	for (CUserManager::ITOR_USER i = user.begin(); i != user.end(); ++i)
	{
		const int idUser = i->second;
		CHARLIST* pUser = CheckServerId(idUser);
		
		if (pUser != NULL && Nation == pUser->name_status.nation)
		{
			QueuePacket(connections, idUser, p, 1);
		}
	}
}	//> CSD-CN-031213

// Send Several Packet to Partial Clients
void SendPacket2NationClients(const int Nation1,t_packet* p1,const int Nation2,t_packet* p2)		
{	//< CSD-CN-031213
	CUserManager::HASH_USER user = g_pUserManager->GetUserSet();

	for (CUserManager::ITOR_USER i = user.begin(); i != user.end(); ++i)
	{
		const int idUser = i->second;
		CHARLIST* pUser = CheckServerId(idUser);
	
		if (pUser != NULL)
		{
			if (Nation1 == pUser->name_status.nation)
			{
				QueuePacket(connections, idUser, p1, 1);
			}
		
			if (Nation2 == pUser->name_status.nation) 
			{
				QueuePacket(connections, idUser, p2, 1);
			}
		}
	}
}	//> CSD-CN-031213
// Send War BBS Packet to All Map Servers
void SendWarBBS2Maps(t_WarBBS* WarBBS)
{
	t_packet packet;

	packet.h.header.type=CMD_WAR_BBS;
	packet.u.NationWar.WarBBS.WarfieldNo=	WarBBS->WarfieldNo;
	packet.u.NationWar.WarBBS.BBSType=		WarBBS->BBSType;
	packet.u.NationWar.WarBBS.LanNo=		WarBBS->LanNo;
	packet.u.NationWar.WarBBS.ArgType=		WarBBS->ArgType;
	packet.u.NationWar.WarBBS.Size=			WarBBS->Size;
	memcpy(packet.u.NationWar.WarBBS.Arg,WarBBS->Arg,WarBBS->Size);
	packet.h.header.size=sizeof(t_WarBBS)-MAX_STRING_PK+packet.u.NationWar.WarBBS.Size;
	SendPacket2Maps(&packet);
}

// Send War BBS to Map Server 
void SendWarBBS2NationMaps(const int Nation,t_WarBBS* WarBBS)
{
	t_packet p1;

	p1.h.header.type=CMD_WAR_BBS;
	p1.u.NationWar.WarBBS.WarfieldNo=	WarBBS->WarfieldNo;
	p1.u.NationWar.WarBBS.BBSType	=	WarBBS->BBSType;
	p1.u.NationWar.WarBBS.LanNo		=	WarBBS->LanNo;
	p1.u.NationWar.WarBBS.ArgType	=	WarBBS->ArgType;
	p1.u.NationWar.WarBBS.Size		=	WarBBS->Size;
	memcpy(p1.u.NationWar.WarBBS.Arg,WarBBS->Arg,WarBBS->Size);
	p1.h.header.size=sizeof(t_WarBBS)-MAX_STRING_PK+p1.u.NationWar.WarBBS.Size;
	SendPacket2NationMaps(Nation,&p1);
}

// Send War BBS to Map Servers 

void SendWarBBS2NationMaps(const int DefenceNation,t_WarBBS* WarBBSAttack,t_WarBBS* WarBBSDefence,t_WarBBS* WarBBSReinforce) //방어국 , 공격국 , 중립국 에 메시지를 보냄..
{
	t_packet AttackerPacket,DefencerPacket,ReinforcePacket;

	int AttackNation,ReinforceNation;

	AttackerPacket.h.header.type=CMD_WAR_BBS;
	AttackerPacket.u.NationWar.WarBBS.WarfieldNo=WarBBSAttack->WarfieldNo;
	AttackerPacket.u.NationWar.WarBBS.BBSType=WarBBSAttack->BBSType;
	AttackerPacket.u.NationWar.WarBBS.LanNo=WarBBSAttack->LanNo;
	AttackerPacket.u.NationWar.WarBBS.ArgType=WarBBSAttack->ArgType;
	AttackerPacket.u.NationWar.WarBBS.Size=WarBBSAttack->Size;
	memcpy(AttackerPacket.u.NationWar.WarBBS.Arg,WarBBSAttack->Arg,WarBBSAttack->Size);
	AttackerPacket.h.header.size=sizeof(t_WarBBS)-MAX_STRING_PK+AttackerPacket.u.NationWar.WarBBS.Size;

	DefencerPacket.h.header.type=CMD_WAR_BBS;
	DefencerPacket.u.NationWar.WarBBS.WarfieldNo=WarBBSDefence->WarfieldNo;
	DefencerPacket.u.NationWar.WarBBS.BBSType=WarBBSDefence->BBSType;
	DefencerPacket.u.NationWar.WarBBS.LanNo=WarBBSDefence->LanNo;
	DefencerPacket.u.NationWar.WarBBS.ArgType=WarBBSDefence->ArgType;
	DefencerPacket.u.NationWar.WarBBS.Size=WarBBSDefence->Size;
	memcpy(DefencerPacket.u.NationWar.WarBBS.Arg,WarBBSDefence->Arg,WarBBSDefence->Size);
	DefencerPacket.h.header.size=sizeof(t_WarBBS)-MAX_STRING_PK+DefencerPacket.u.NationWar.WarBBS.Size;

	ReinforcePacket.h.header.type=CMD_WAR_BBS;
	ReinforcePacket.u.NationWar.WarBBS.WarfieldNo=WarBBSReinforce->WarfieldNo;
	ReinforcePacket.u.NationWar.WarBBS.BBSType=WarBBSReinforce->BBSType;
	ReinforcePacket.u.NationWar.WarBBS.LanNo=WarBBSReinforce->LanNo;
	ReinforcePacket.u.NationWar.WarBBS.ArgType=WarBBSReinforce->ArgType;
	ReinforcePacket.u.NationWar.WarBBS.Size=WarBBSReinforce->Size;
	memcpy(ReinforcePacket.u.NationWar.WarBBS.Arg,WarBBSReinforce->Arg,WarBBSReinforce->Size);
	ReinforcePacket.h.header.size=sizeof(t_WarBBS)-MAX_STRING_PK+ReinforcePacket.u.NationWar.WarBBS.Size;

	ReinforceNation=NW_YL;

	if (DefenceNation==NW_BY) 
	{
		AttackNation=NW_ZY;
	}
	else 
	{
		AttackNation=NW_BY;
	}
	SendPacket2NationMaps(AttackNation,&AttackerPacket,DefenceNation,&DefencerPacket,ReinforceNation,&ReinforcePacket);
}

// Send War BBS to Clients Divided Nation
void SendWarBBS2NationClients(const int Nation,t_WarBBS* WarBBS)
{
	t_packet packet;

	packet.h.header.type=CMD_WAR_BBS;
	packet.u.NationWar.WarBBS.WarfieldNo=WarBBS->WarfieldNo;
	packet.u.NationWar.WarBBS.BBSType=WarBBS->BBSType;
	packet.u.NationWar.WarBBS.LanNo=WarBBS->LanNo;
	packet.u.NationWar.WarBBS.ArgType=WarBBS->ArgType;
	packet.u.NationWar.WarBBS.Size=WarBBS->Size;
	memcpy(packet.u.NationWar.WarBBS.Arg,WarBBS->Arg,WarBBS->Size);
	packet.h.header.size=sizeof(t_WarBBS)-MAX_STRING_PK+packet.u.NationWar.WarBBS.Size;
	SendPacket2NationClients(Nation,&packet);
}

// Send Several War BBS to Clients
void SendWarBBS2NationClients(const int DefenceNation,t_WarBBS* WarBBSAttack,t_WarBBS* WarBBSDefence)
{
	t_packet AttackerPacket,DefencerPacket;

	int AttackNation;

	AttackerPacket.h.header.type=CMD_WAR_BBS;
	AttackerPacket.u.NationWar.WarBBS.WarfieldNo=WarBBSAttack->WarfieldNo;
	AttackerPacket.u.NationWar.WarBBS.BBSType=WarBBSAttack->BBSType;
	AttackerPacket.u.NationWar.WarBBS.LanNo=WarBBSAttack->LanNo;
	AttackerPacket.u.NationWar.WarBBS.ArgType=WarBBSAttack->ArgType;
	AttackerPacket.u.NationWar.WarBBS.Size=WarBBSAttack->Size;
	memcpy(AttackerPacket.u.NationWar.WarBBS.Arg,WarBBSAttack->Arg,WarBBSAttack->Size);
	AttackerPacket.h.header.size=sizeof(t_WarBBS)-MAX_STRING_PK+AttackerPacket.u.NationWar.WarBBS.Size;

	DefencerPacket.h.header.type=CMD_WAR_BBS;
	DefencerPacket.u.NationWar.WarBBS.WarfieldNo=WarBBSDefence->WarfieldNo;
	DefencerPacket.u.NationWar.WarBBS.BBSType=WarBBSDefence->BBSType;
	DefencerPacket.u.NationWar.WarBBS.LanNo=WarBBSDefence->LanNo;
	DefencerPacket.u.NationWar.WarBBS.ArgType=WarBBSDefence->ArgType;
	DefencerPacket.u.NationWar.WarBBS.Size=WarBBSDefence->Size;
	memcpy(DefencerPacket.u.NationWar.WarBBS.Arg,WarBBSDefence->Arg,WarBBSDefence->Size);
	DefencerPacket.h.header.size=sizeof(t_WarBBS)-MAX_STRING_PK+DefencerPacket.u.NationWar.WarBBS.Size;

	if (DefenceNation==NW_BY) 
	{
		AttackNation=NW_ZY;
	}
	else 
	{
		AttackNation=NW_BY;
	}

	SendPacket2NationClients(AttackNation,&AttackerPacket,DefenceNation,&DefencerPacket);
}
// Send War BBS to Clients 
void SendWarBBS2Clients(t_WarBBS* WarBBS)
{
	t_packet packet;

	packet.h.header.type=CMD_WAR_BBS;
	packet.u.NationWar.WarBBS.WarfieldNo=WarBBS->WarfieldNo;
	packet.u.NationWar.WarBBS.BBSType=WarBBS->BBSType;
	packet.u.NationWar.WarBBS.LanNo=WarBBS->LanNo;
	packet.u.NationWar.WarBBS.ArgType=WarBBS->ArgType;
	packet.u.NationWar.WarBBS.Size=WarBBS->Size;
	memcpy(packet.u.NationWar.WarBBS.Arg,WarBBS->Arg,WarBBS->Size);
	packet.h.header.size=sizeof(t_WarBBS)-MAX_STRING_PK+packet.u.NationWar.WarBBS.Size;
	g_pUserManager->SendPacket(&packet); // CSD-CN-031213
}
// Where : Call From CheckHandleByNationWar
// Usage : Packet Process

void SendCMD_REQUEST_DELIVERY(const WORD Port,t_packet *p, t_connection c[], const int cn)
{
	t_packet packet;

	packet.h.header.type=CMD_REQUEST_DELIVERY;
	packet.u.NationWar.Delivery_R.Port=g_wMapServerPort;
	packet.u.NationWar.Delivery_R.UserID=cn;
	packet.u.NationWar.Delivery_R.Size=sizeof(t_header)+p->h.header.size;
	memcpy(packet.u.NationWar.Delivery_R.Data,(char*)p,packet.u.NationWar.Delivery_R.Size);
	packet.h.header.size=sizeof(t_Delivery_R)-MAX_STRING_PK+packet.u.NationWar.Delivery_R.Size;  //100은 버퍼의 수이다. MAX_STRING_PK (NetWork4.h)//020903 lsw
	g_pServerTable->SendRajaPacketToOtherMapServer(Port, (char *)&packet, packet.h.header.size+sizeof(t_header) );
}

void SendCMD_ANSWER_DELIVERY(const int UserID,const WORD MapServerPort,t_packet* p)
{
	t_packet DeliveryPacket;

	DeliveryPacket.h.header.type=CMD_ANSWER_DELIVERY;
	DeliveryPacket.u.NationWar.Delivery_A.UserID=UserID;
	DeliveryPacket.u.NationWar.Delivery_A.Size=sizeof(t_header)+p->h.header.size;
	memcpy(DeliveryPacket.u.NationWar.Delivery_A.Data,(char*)p,DeliveryPacket.u.NationWar.Delivery_A.Size);
	DeliveryPacket.h.header.size=sizeof(t_Delivery_A)-100+DeliveryPacket.u.NationWar.Delivery_A.Size;
	g_pServerTable->SendRajaPacketToOtherMapServer(MapServerPort, (char *)&DeliveryPacket, DeliveryPacket.h.header.size+sizeof(t_header) );
}

void SendCMD_REQUEST_CHECK_WARNO(const int cn)			// MenuServer.cpp SendJoinGameWho() Call	// 011028 LTS
{
	// 처음 로긴했다.
	LPCHARLIST ch = ::CheckServerId(cn);
	if(!ch){return;}
	t_packet packet;
	if (isNationManageServer())
	{
		DWORD tempCount=g_pNation->GetWarCount();								// LTS NEW_NATION_WAR

		if (tempCount!=ch->NWCharacter.WarNo)		// 전쟁번호가 틀리다면 
		{
			memset(&ch->NWCharacter,0,sizeof(NW_Character));			// 캐릭터의 NWCHaracter를 초기화 해라.
			ch->NWCharacter.WarNo=tempCount;						// LTS NEW_NATION_WAR
			packet.h.header.type=CMD_CLEAR_WAR_CHARACTER_DATA;						// 클라리언트도 초기화 한다.
			packet.u.NationWar.CommonDataC.Data=tempCount;							// LTS NEW_NATION_WAR
			packet.h.header.size=sizeof(t_CommonDataC);
			QueuePacket(connections,cn,&packet,1);
		}
		else // 전쟁 번호가 같다면 // 근데 평화기간이면 // 부대마크를 지우도록 한다.
		{
			if ((g_pWarfieldStatus[0].Status<2)&&
				(g_pWarfieldStatus[1].Status<2)&&
				(g_pWarfieldStatus[2].Status<2))
			{
				memset(&ch->NWCharacter,0,sizeof(DWORD));			// 캐릭터의 NWCHaracter를 초기화 해라.
				ch->NWCharacter.WarNo=tempCount;						// LTS NEW_NATION_WAR
				packet.h.header.type=CMD_CLEAR_WAR_CHARACTER_DATA;						// 클라리언트도 초기화 한다.
				packet.u.NationWar.CommonDataC.Data=tempCount;							// LTS NEW_NATION_WAR
				packet.h.header.size=sizeof(t_CommonDataC);
				QueuePacket(connections,cn,&packet,1);
			}
		}
	}
	else
	{
		packet.h.header.type=CMD_REQUEST_CHECK_WARNO;								// 전쟁번호를 관리서버에게 요구하고 
		packet.h.header.size=sizeof(t_RequestCheckWarNo);

		packet.u.NationWar.RequestCheckWarNo.Port=g_wMapServerPort;					// RecvCMD_ANSWER_CHECK_WARNO에서 처리한다.
		packet.u.NationWar.RequestCheckWarNo.ServerID=cn;
		SendPacket2NWManager(&packet);
	}
}

void SendCMD_SQUAD_LODING_POINT(const int WarfieldNo)			// 관리서버에서 전쟁서버로 전쟁시작하라고 할때 보내는 패킷 
{
	t_packet packet;
	if (!isNationManageServer()) return;

	int WFieldNo=WarfieldNo-BASE_WARFIELD_PORT;
	packet.h.header.type=CMD_SQUAD_LOADING_POINT;		// NW_MAN->NW_WAR
	packet.u.NationWar.SquadLoadingPoint.SquadLoadingPoint[NW_ATTACKER][0]=g_pNation->GetSquadLoadingPoint(WFieldNo,NW_ATTACKER,0);
	packet.u.NationWar.SquadLoadingPoint.SquadLoadingPoint[NW_ATTACKER][1]=g_pNation->GetSquadLoadingPoint(WFieldNo,NW_ATTACKER,1);
	packet.u.NationWar.SquadLoadingPoint.SquadLoadingPoint[NW_ATTACKER][2]=g_pNation->GetSquadLoadingPoint(WFieldNo,NW_ATTACKER,2);
	packet.u.NationWar.SquadLoadingPoint.SquadLoadingPoint[NW_ATTACKER][3]=g_pNation->GetSquadLoadingPoint(WFieldNo,NW_ATTACKER,3);
	packet.u.NationWar.SquadLoadingPoint.SquadLoadingPoint[NW_ATTACKER][4]=g_pNation->GetSquadLoadingPoint(WFieldNo,NW_ATTACKER,4);
	packet.u.NationWar.SquadLoadingPoint.SquadLoadingPoint[NW_DEFENCER][0]=g_pNation->GetSquadLoadingPoint(WFieldNo,NW_DEFENCER,0);
	packet.u.NationWar.SquadLoadingPoint.SquadLoadingPoint[NW_DEFENCER][1]=g_pNation->GetSquadLoadingPoint(WFieldNo,NW_DEFENCER,1);
	packet.u.NationWar.SquadLoadingPoint.SquadLoadingPoint[NW_DEFENCER][2]=g_pNation->GetSquadLoadingPoint(WFieldNo,NW_DEFENCER,2);
	packet.u.NationWar.SquadLoadingPoint.SquadLoadingPoint[NW_DEFENCER][3]=g_pNation->GetSquadLoadingPoint(WFieldNo,NW_DEFENCER,3);
	packet.u.NationWar.SquadLoadingPoint.SquadLoadingPoint[NW_DEFENCER][4]=g_pNation->GetSquadLoadingPoint(WFieldNo,NW_DEFENCER,4);
	packet.u.NationWar.SquadLoadingPoint.SquadLoadingPoint[NW_REINFORCE][0]=g_pNation->GetSquadLoadingPoint(WFieldNo,NW_REINFORCE,0);
	packet.u.NationWar.SquadLoadingPoint.SquadLoadingPoint[NW_REINFORCE][1]=g_pNation->GetSquadLoadingPoint(WFieldNo,NW_REINFORCE,1);
	packet.u.NationWar.SquadLoadingPoint.SquadLoadingPoint[NW_REINFORCE][2]=g_pNation->GetSquadLoadingPoint(WFieldNo,NW_REINFORCE,2);
	packet.u.NationWar.SquadLoadingPoint.SquadLoadingPoint[NW_REINFORCE][3]=g_pNation->GetSquadLoadingPoint(WFieldNo,NW_REINFORCE,3);
	packet.u.NationWar.SquadLoadingPoint.SquadLoadingPoint[NW_REINFORCE][4]=g_pNation->GetSquadLoadingPoint(WFieldNo,NW_REINFORCE,4);
	packet.h.header.size=sizeof(t_SquadLoadingPoint);
	g_pServerTable->SendRajaPacketToOtherMapServer((WORD)WarfieldNo,(char *)&packet, packet.h.header.size+sizeof(t_header) );
}

void SendCMD_SQUAD_RARE(const WORD WarfieldNo)
{
	t_packet packet;
	if (!isNationManageServer()) return;

	packet.h.header.type=CMD_SQUAD_RARE;

	const int WFieldNo=WarfieldNo-BASE_WARFIELD_PORT;

	for (int i=0;i<NW_SQUAD_KIND;i++)		// LOOP UNROLL이 속도가 빠르나 보기 힘들다. 
	{
		for (int j=0;j<NW_SQUAD_MAX;j++)
		{
			for (int k=0;k<NW_SQUAD_RARE_MAX;k++)
			{
				packet.u.NationWar.NWSquadRare.SquadRare[i][j][k]=g_pNation->GetSquadRareSok(WFieldNo,i,j,k);
			}
		}
	}
	//011014 lsw >
	packet.h.header.size=sizeof(t_NWSquadRare);
	//011014 lsw <
//	g_pServerTable->SendRajaPacketToOtherMapServer((WORD)WarfieldNo,(char *)&packet, packet.h.header.size+sizeof(t_header) );
	g_pServerTable->SendRajaPacketToOtherMapServer(WarfieldNo,(char *)&packet, packet.h.header.size+sizeof(t_header) );
}

void RecvCMD_REQUEST_CHECK_WARNO(t_packet* p,t_connection c[],const int cn)				// 오직 관리서버만..
{
	t_packet packet;

	if (!isNationManageServer()) return;

	packet.h.header.type=CMD_ANSWER_CHECK_WARNO;
	DWORD tempWarNo=g_pNation->GetWarCount();											// LTS NEW_NATION_WAR
	packet.u.NationWar.AnswerCheckWarNo.WarNo=(BYTE)tempWarNo;								// LTS NEW_NATION_WAR	
	packet.u.NationWar.AnswerCheckWarNo.ServerID=p->u.NationWar.RequestCheckWarNo.ServerID;
	packet.h.header.size=sizeof(t_AnswerCheckWarNo);	

	SendPacket2NWMap(p->u.NationWar.RequestCheckWarNo.Port,&packet);
}

void RecvCMD_ANSWER_CHECK_WARNO(t_packet* p,t_connection c[],const int cn)			// 011028 LTS
{// 관리서버에서 전쟁번호가 왔다.
	const int iUserCn	= p->u.NationWar.AnswerCheckWarNo.ServerID;
	const int iWarNo	= p->u.NationWar.AnswerCheckWarNo.WarNo;

	LPCHARLIST ch = ::CheckServerId(iUserCn);
	if(!ch)
	{
		::MyLog(0,"RecvCMD_ANSWER_CHECK_WARNO() Error WarNo[%d] CN[%d] ",iWarNo,iUserCn);
		return;
	}

	t_packet packet;
	if (iWarNo != ch->NWCharacter.WarNo)		
	{																				// 전쟁번호가 틀리다면 
		::memset(&ch->NWCharacter,0,sizeof(NW_Character));				// 전쟁을 초기화 해라.
		ch->NWCharacter.WarNo=iWarNo;
		packet.h.header.type=CMD_CLEAR_WAR_CHARACTER_DATA;
		packet.u.NationWar.CommonDataC.Data=iWarNo;
		packet.h.header.size=sizeof(t_CommonDataC);
		::QueuePacket(connections,iUserCn,&packet,1);
	}
	else
	{
		if ((g_pWarfieldStatus[0].Status<2)&&
			(g_pWarfieldStatus[1].Status<2)&&
			(g_pWarfieldStatus[2].Status<2))
		{
			::memset(&ch->NWCharacter,0,sizeof(NW_Character));				// 전쟁을 초기화 해라.
			ch->NWCharacter.WarNo=iWarNo;
			packet.h.header.type=CMD_CLEAR_WAR_CHARACTER_DATA;
			packet.u.NationWar.CommonDataC.Data=iWarNo;
			packet.h.header.size=sizeof(t_CommonDataC);
			::QueuePacket(connections,iUserCn,&packet,1);
		}
	}
}

void SendCMD_CLEAR_SQUAD_DATA(t_connection c[],const int cn)								// NWCharacter 를 초기화 하라고 메시지를 보낸다.
{
	// 전쟁이 끝났을때.
	t_packet packet;
	packet.h.header.type=CMD_CLEAR_SQUAD_DATA;
	packet.h.header.size=0;
	QueuePacket(c,cn,&packet,1);
}

void SendCMD_CLEAR_SQUAD_DATA(const int Nation)		// 0: ALL 3,4 : WarfieldNo
{
	t_packet packet;

	packet.h.header.type=CMD_CLEAR_SQUAD_DATA;
	packet.h.header.size=0;

	if (0==Nation)
	{
		SendPacket2Maps(&packet); //전쟁 후 
	}
	else 
	{
		SendPacket2NationMaps(Nation,&packet); 
	}

}

void SendWarBeginEndMessage2Client(const int type)
{
	t_packet p;

	if (type) 
	{
		p.h.header.type=CMD_WAR_BEGIN;
	}
	else 
	{
		p.h.header.type=CMD_WAR_END;
	}

	p.h.header.size=0;
	g_pUserManager->SendPacket(&p); // CSD-CN-031213

	if (!type)				//전쟁이 끝났으면.. 옷을 갈아입어라...
	{
		for( int i = DRAGON_CONNECTIONS_START; i<DRAGON_MAX_CONNECTIONS; i++ )
		{
			CHARLIST* ch=CheckServerId(i);
			if(ch)	
			{
				CheckAndSendChangeEquip(ch,1);
			}
		}
	}
}


void SendWarBeginEndMessage2Client(const int type,t_connection c[],const int cn)
{
	t_packet p;

	if (type) p.h.header.type=CMD_WAR_BEGIN;
	else p.h.header.type=CMD_WAR_END;
	p.h.header.size=0;
	QueuePacket(c,cn,&p,1);
}



void RecvCMD_CHECK_WARFIELD_STATUS(t_packet *p,t_connection c[],const int cn)
{
	if (isNationManageServer()) 
	{
		g_pNation->CheckWarFieldStatus(p->u.NationWar.WarfieldStatusChange.WarfieldNo,p->u.NationWar.WarfieldStatusChange.Status);
	}
}

void RecvCMD_CHECK_WARFIELD_STATUS_RESULT(t_packet *p,t_connection c[],const int cn)
{
	if (isNationWarfieldServer())
	{
		g_pWarfield->ChangeWarfieldStatus(p->u.NationWar.CommonDataC.Data);
	}
}

void RecvCMD_WARFIELD_STATUS(t_packet *p,t_connection c[],const int cn)
{
	if (isNationManageServer())
	{
		int temp=p->u.NationWar.WarfieldStatusChange.WarfieldNo-BASE_WARFIELD_PORT;
		g_pNation->SetWarFieldStatus(temp,p->u.NationWar.WarfieldStatusChange.Status);
		if (p->u.NationWar.WarfieldStatusChange.Status==NW_VOTE) g_pNation->InitVoteData(temp); //전쟁참여 투표데이터를 초기화한다.
	}
}

void RecvCMD_WARBBS(t_packet *p)
{
	switch (p->u.NationWar.WarBBS.BBSType)
	{
	case BBS_COMMANDER_ELECT : 
		{
			char tempCommander[80];
			char* tempChar;
			memcpy(tempCommander,p->u.NationWar.WarBBS.Arg,p->u.NationWar.WarBBS.Size);
			tempChar=tempCommander;
			*(tempChar+p->u.NationWar.WarBBS.Size)='\0';
			FindCommander(tempCommander);	break;
		}
	}
	
	g_pUserManager->SendPacket(p); // CSD-CN-031213
}

void RecvCMD_REQUEST_WARFIELD_STATUS(t_packet *p, t_connection c[], const int cn)
{
	t_packet packet;
	if (isNationManageServer())  //관리서버이므로 즉시 클라이언트에 해당 패킷을 보낸다.
	{
		ProcessCMD_REQUEST_WARFIELD_STATUS(&packet,p);
		QueuePacket(c,cn,&packet,1);
	}
	else 
	{
		SendCMD_REQUEST_DELIVERY(NATION_MANAGE_SERVER,p,c,cn);	//관리서버가 아니므로 Map용패킷으로 바꾸고 전송한다.
	}
}

void SendCMD_WARFIELD_WAR_DATA1(t_packet* p)
{
	if (!isNationWarfieldServer()) return;

	int i,GuardStoneCount=0,CastleGateWayCount=0;
	int tempIndex;
	int count=0;
	const int GuardSize	=	g_pWarfield->GetGuardStoneCount(); 
	const int CastleSize=	g_pWarfield->GetCastleDoorCount();
	if(0 >= GuardSize || 0 >= CastleSize)
	{
		return;
	}

	char* GuardStoneStatus		=new char[GuardSize];
	char* CastleGateWayStatus	=new char[CastleSize];

	for (i=0;i<NW_MAX_NPC;i++)
	{
		tempIndex=g_pWarfield->GetWeaponIndex(i);
		if (tempIndex==63)		//수호석 //보스는 죽으면 끝이다 무조건 살아있다고!!
		{			
			GuardStoneStatus[GuardStoneCount]=g_pWarfield->GetWeaponStatus(i);
			GuardStoneCount++;
		}
		if (tempIndex>=65&&tempIndex<=67)
		{
			CastleGateWayStatus[CastleGateWayCount]=g_pWarfield->GetWeaponStatus(i);
			CastleGateWayCount++;
		}
	}

	if (GuardStoneCount!=GuardSize) 
	{
		MyLog(0,"GuardStoneCount Check Error!!");
	}
	if (CastleGateWayCount!=CastleSize) 
	{
		MyLog(0,"CastleGateWayCount Check Error");
	}
	p->h.header.type=CMD_WARFIELD_WAR_DATA1;
	(char)p->u.data[count]=GuardSize;
	for (i=0;i<GuardSize;i++)
	{
		count++;
		(char)p->u.data[count]=GuardStoneStatus[i];
	}
	count++;
	(char)p->u.data[count]=CastleGateWayCount;
	for (i=0;i<CastleSize;i++)
	{
		count++;
		(char)p->u.data[count]=CastleGateWayStatus[i];
	}
	p->h.header.size=1+1+GuardSize+CastleSize;

	SAFE_DELETE(GuardStoneStatus);
	SAFE_DELETE(CastleGateWayStatus);
}

void SendCMD_WARFIELD_WAR_DATA2(t_packet* p)
{
	if (!isNationWarfieldServer()) return;

	p->h.header.type=CMD_WARFIELD_WAR_DATA2;
	p->u.NationWar.WarfieldData2.RemainTime=g_pWarfield->GetLoopTime()-g_WarTime;
	p->u.NationWar.WarfieldData2.AttackerCount=g_pWarfield->GetAllSquadCurrentMemberCount(NW_ATTACKER);
	p->u.NationWar.WarfieldData2.DefencerCount=g_pWarfield->GetAllSquadCurrentMemberCount(NW_DEFENCER);
	p->u.NationWar.WarfieldData2.ReinforceCount=g_pWarfield->GetAllSquadCurrentMemberCount(NW_DEFENCER);

	p->h.header.size=sizeof(t_WarfieldData2);
}

void SendCMD_WARFIELD_WAR_DATA3(t_packet* p,int WarfieldNo,int Nation)			// 국적이 필요하다.
{
	if (!isNationWarfieldServer()) 
	{
		return;
	}

	p->h.header.type=CMD_WARFIELD_WAR_DATA3;
	
	if (isAttacker(WarfieldNo,Nation)) 
	{
		p->u.NationWar.WarfieldData3.Type=NW_ATTACKER;
	}
	else 
	{
		p->u.NationWar.WarfieldData3.Type=NW_DEFENCER;
	}

	p->u.NationWar.WarfieldData3.MemberCount[0]=g_pWarfield->GetSquadCurrentMemberCount(p->u.NationWar.WarfieldData3.Type,0);		// LOOP UNROLL	//NW_SQUAD
	p->u.NationWar.WarfieldData3.MemberCount[1]=g_pWarfield->GetSquadCurrentMemberCount(p->u.NationWar.WarfieldData3.Type,1);
	p->u.NationWar.WarfieldData3.MemberCount[2]=g_pWarfield->GetSquadCurrentMemberCount(p->u.NationWar.WarfieldData3.Type,2);
	p->u.NationWar.WarfieldData3.MemberCount[3]=g_pWarfield->GetSquadCurrentMemberCount(p->u.NationWar.WarfieldData3.Type,3);
	p->u.NationWar.WarfieldData3.MemberCount[4]=g_pWarfield->GetSquadCurrentMemberCount(p->u.NationWar.WarfieldData3.Type,4);
	p->h.header.size=sizeof(t_WarfieldData3);
}

void SendCMD_UPDATE_WARTIME(const int WarfieldNo,const int Status,const DWORD RemainTime)
{
	t_packet packet;
	packet.h.header.type=CMD_SET_REMAIN_TIME;
	packet.u.NationWar.RemainTime.WarfieldNo=WarfieldNo;
	packet.u.NationWar.RemainTime.Status=Status;
	packet.u.NationWar.RemainTime.RemainTime=RemainTime;
	packet.h.header.size=sizeof (t_RemainTime);
	SendPacket2NWManager(&packet);
}

bool CheckCharSquadLeaderCondition(t_connection c[], const int cn )		// 011016 LTS
{	//< CSD-030806
	if ((c[cn].chrlst.GetLevel()>=NW_SQUADLEADER_LEVEL_MIN) &&
		(c[cn].chrlst.LadderScore>=NW_SQUADLEADER_LADDER_MIN)) 
	{
		return true;
	}

	return false;
}	//> CSD-030806

bool CheckJoinSquadCondition(const int SquadNo,const int MemberCount)			// 011028 LTS
{	//< CSD-030806
	switch (SquadNo)		//공격자와 방어자의 구성이 같다.
	{
	case 1:
		{
			if (MemberCount <= NW_SQUAD1_MAN_MAX)
			{
				return true;
			}

			break;
		}
	case 2:
		{
			if (MemberCount <= NW_SQUAD2_MAN_MAX)
			{
				return true;
			}

			break;
		}
	case 3:
		{
			if (MemberCount <= NW_SQUAD3_MAN_MAX)
			{
				return true;
			}

			break;
		}
	case 4:
		{
			if (MemberCount <= NW_SQUAD4_MAN_MAX)
			{
				return true;
			}
		
			break;
		}
	}

	return false;
}	//> CSD-030806

bool CheckJoinLevel(const int SquadNo,t_connection c[],const int cn)	// 011016 LTS
{	//< CSD-030806
	CHARLIST* pApplicant = &c[cn].chrlst;
	if (!pApplicant) return false;

	switch (SquadNo)
	{
	case 1:
		{
			if (pApplicant->GetLevel() >= NW_SQUAD1_LEVEL_MIN && pApplicant->GetLevel() <= NW_SQUAD1_LEVEL_MAX) 
			{
				return true;
			}

			break;
		}
	case 2:
		{
			if (pApplicant->GetLevel() >= NW_SQUAD2_LEVEL_MIN && pApplicant->GetLevel() <= NW_SQUAD2_LEVEL_MAX)
			{
				return true;
			}
			
			break;
		}
	case 3:
		{
			if (pApplicant->GetLevel() >= NW_SQUAD3_LEVEL_MIN && pApplicant->GetLevel() <= NW_SQUAD3_LEVEL_MAX)
			{
				return true;
			}

			break;
		}
	case 4:
		{
			if (pApplicant->GetLevel() >= NW_SQUAD4_LEVEL_MIN && pApplicant->GetLevel() <= NW_SQUAD4_LEVEL_MAX)
			{
				return true;
			}

			break;
		}
	}
	
	return false;
}	//> CSD-030806

// Where : Call From DragonServer.cpp HandleCommand()
// Usage : Packet Process

void ProcessCMD_REQUEST_WARFIELD_STATUS(t_packet *ReturnPacket,t_packet* ReceivePacket)		// 전쟁터상태
{
	ReturnPacket->h.header.type=CMD_ANSWER_WARFIELD_STATUS;
	ReturnPacket->u.NationWar.WarfieldStatus.PacketStatus=ReceivePacket->u.NationWar.WarfieldStatus.PacketStatus;
	ReturnPacket->u.NationWar.WarfieldStatus.Status[0]=g_pNation->GetWarfieldStatus(0);
	ReturnPacket->u.NationWar.WarfieldStatus.Status[1]=g_pNation->GetWarfieldStatus(1);
	ReturnPacket->u.NationWar.WarfieldStatus.Status[2]=g_pNation->GetWarfieldStatus(2);
	ReturnPacket->h.header.size=sizeof(t_WarfieldStatus);
}

void ProcessCMD_REQUEST_VOTE_DATA(t_packet *ReturnPacket,t_packet* ReceivePacket)			// 전쟁터참여
{
	ReturnPacket->h.header.type=CMD_ANSWER_VOTE_DATA;
	ReturnPacket->u.NationWar.VoteData.WarfieldNo=ReceivePacket->u.NationWar.CommonDataC.Data;
	ReturnPacket->u.NationWar.VoteData.VoteCount=g_pNation->GetVoteData(ReceivePacket->u.NationWar.CommonDataC.Data);
	ReturnPacket->h.header.size=sizeof(t_VoteData);
}

void ProcessCMD_REQUEST_SQUAD_INFO(t_packet *ReturnPacket,t_packet* ReceivePacket)			// 부대정보
{
	const int Nation=ReceivePacket->u.NationWar.RequestSquadInfo.Nation;
	const int WarfieldNo=ReceivePacket->u.NationWar.RequestSquadInfo.WarfieldNo;
	const int SquadNo=ReceivePacket->u.NationWar.RequestSquadInfo.SquadNo;

	t_SquadInformation* tempSquadInfo;
	tempSquadInfo=g_pNation->GetSquadInfo(Nation,WarfieldNo,SquadNo); 

	ReturnPacket->h.header.type=CMD_ANSWER_SQUAD_INFO;
	ReturnPacket->u.NationWar.SquadInfo.SquadNo=SquadNo;
	ReturnPacket->u.NationWar.SquadInfo.ChipID=tempSquadInfo->CO_ID;
	memcpy(ReturnPacket->u.NationWar.SquadInfo.ChipName,tempSquadInfo->CO_Name,NW_NAME_MAX);
	ReturnPacket->u.NationWar.SquadInfo.SoldierCount=tempSquadInfo->SoliderCount;
	ReturnPacket->u.NationWar.SquadInfo.Sok[0]=tempSquadInfo->Sok[0];
	ReturnPacket->u.NationWar.SquadInfo.Sok[1]=tempSquadInfo->Sok[1];
	ReturnPacket->u.NationWar.SquadInfo.Sok[2]=tempSquadInfo->Sok[2];
	ReturnPacket->u.NationWar.SquadInfo.LoadingPoint=tempSquadInfo->LoadingPoint;
	ReturnPacket->h.header.size=sizeof(t_SquadInfo);
}

void ProcessCMD_REQUEST_SQUAD_INFO2(t_packet *ReturnPacket,t_packet* ReceivePacket)
{
	const int Nation=ReceivePacket->u.NationWar.RequestSquadInfo.Nation;
	const int WarfieldNo=ReceivePacket->u.NationWar.RequestSquadInfo.WarfieldNo;
	const int SquadNo=ReceivePacket->u.NationWar.RequestSquadInfo.SquadNo;

	ReturnPacket->h.header.type=CMD_ANSWER_SQUAD_INFO2;
	strcpy_s(ReturnPacket->u.NationWar.SquadInfo2.CommanderName, sizeof(ReturnPacket->u.NationWar.SquadInfo2.CommanderName),
	g_pNation->GetCommanderName(WarfieldNo,Nation));
	ReturnPacket->u.NationWar.SquadInfo2.SquadCount[0]=g_pNation->GetSquadMemberCount(Nation,WarfieldNo,0);
	ReturnPacket->u.NationWar.SquadInfo2.SquadCount[1]=g_pNation->GetSquadMemberCount(Nation,WarfieldNo,1);
	ReturnPacket->u.NationWar.SquadInfo2.SquadCount[2]=g_pNation->GetSquadMemberCount(Nation,WarfieldNo,2);
	ReturnPacket->u.NationWar.SquadInfo2.SquadCount[3]=g_pNation->GetSquadMemberCount(Nation,WarfieldNo,3);
	ReturnPacket->u.NationWar.SquadInfo2.SquadCount[4]=g_pNation->GetSquadMemberCount(Nation,WarfieldNo,4);
	ReturnPacket->h.header.size=sizeof(t_SquadInfo2);
}

void ProcessCMD_REQUEST_COMMANDER_VOTE_DATA(t_packet *ReturnPacket,t_packet* ReceivePacket)	// 사령관 투표 
{
	const int Nation=ReceivePacket->u.NationWar.RequestCommanderVoteData.Nation;
	const int WarfieldNo=ReceivePacket->u.NationWar.RequestCommanderVoteData.WarfieldNo;
	const int CandidaterNo=ReceivePacket->u.NationWar.RequestCommanderVoteData.CandidaterNo;
	t_CommanderInformation* tempCandidaterInfo;

	tempCandidaterInfo=g_pNation->GetCandidaterInfo(Nation,WarfieldNo,CandidaterNo); //스쿼드번호를 나타내는 구조체가 없다.. 되면 만들자..
	ReturnPacket->h.header.type=CMD_ANSWER_COMMANDER_VOTE_DATA;
	ReturnPacket->u.NationWar.CommanderVoteData.VoteNo=CandidaterNo;
	ReturnPacket->u.NationWar.CommanderVoteData.CandidaterID=tempCandidaterInfo->CO_ID;
	//memcpy(ReturnPacket->u.NationWar.CommanderVoteData.Name,tempCandidaterInfo->CO_Name,NW_NAME_MAX);

	MyLog(LOG_NORMAL, "sprintf_s(ReturnPacket->u.NationWar.CommanderVoteData.Name, sizeof(ReturnPacket->u.NationWar.CommanderVoteData.Name),'%s',tempCandidaterInfo->CO_Name);"); //Eleval 22/08/09 - To debug Nation War (have to be removed later)
	if(strlen(tempCandidaterInfo->CO_Name) > 0) sprintf_s(ReturnPacket->u.NationWar.CommanderVoteData.Name, sizeof(ReturnPacket->u.NationWar.CommanderVoteData.Name),"%s",tempCandidaterInfo->CO_Name); //Eleval 20/08/09 - Added a check for Name size in case of empty var
	else sprintf(ReturnPacket->u.NationWar.CommanderVoteData.Name, " "); //Eleval 20/08/09 - if string empty, put a " " in it
	ReturnPacket->u.NationWar.CommanderVoteData.Level=tempCandidaterInfo->btLevel; // CSD-030806
	ReturnPacket->u.NationWar.CommanderVoteData.Fame=tempCandidaterInfo->Fame;
	ReturnPacket->u.NationWar.CommanderVoteData.Face=tempCandidaterInfo->Face;
	ReturnPacket->u.NationWar.CommanderVoteData.Gender=tempCandidaterInfo->Gender;
	ReturnPacket->u.NationWar.CommanderVoteData.Class=tempCandidaterInfo->Class;		// 011016 LTS
	ReturnPacket->u.NationWar.CommanderVoteData.nDual=tempCandidaterInfo->nDual;		// 011020 LTS
	ReturnPacket->u.NationWar.CommanderVoteData.nStep=tempCandidaterInfo->nStep;		// 011020 LTS
	ReturnPacket->u.NationWar.CommanderVoteData.LadderScore=tempCandidaterInfo->LadderScore;
	ReturnPacket->u.NationWar.CommanderVoteData.VoteCount=tempCandidaterInfo->VoteCount;
	ReturnPacket->h.header.size=sizeof(t_CommanderVoteData);
}
void ProcessCMD_REQUEST_COMMANDER_VOTE_DATA2(t_packet *ReturnPacket,t_packet* ReceivePacket)
{
	const int Nation=ReceivePacket->u.NationWar.RequestCommanderVoteData.Nation;
	const int WarfieldNo=ReceivePacket->u.NationWar.RequestCommanderVoteData.WarfieldNo;
	const int CandidaterNo=ReceivePacket->u.NationWar.RequestCommanderVoteData.CandidaterNo;

	ReturnPacket->h.header.type=CMD_ANSWER_COMMANDER_VOTE_DATA2;

	strcpy_s(ReturnPacket->u.NationWar.CommanderVoteData2.CandidaterName[0], sizeof(ReturnPacket->u.NationWar.CommanderVoteData2.CandidaterName[0]),
		g_pNation->GetCandidaterName(WarfieldNo,Nation,0));
	ReturnPacket->u.NationWar.CommanderVoteData2.VoteCount[0]=g_pNation->GetCandidaterVoteCount(WarfieldNo,Nation,0);

	strcpy_s(ReturnPacket->u.NationWar.CommanderVoteData2.CandidaterName[1], sizeof(ReturnPacket->u.NationWar.CommanderVoteData2.CandidaterName[1]),
		g_pNation->GetCandidaterName(WarfieldNo,Nation,1));
	ReturnPacket->u.NationWar.CommanderVoteData2.VoteCount[1]=
		g_pNation->GetCandidaterVoteCount(WarfieldNo,Nation,1);

	strcpy_s(ReturnPacket->u.NationWar.CommanderVoteData2.CandidaterName[2], sizeof(ReturnPacket->u.NationWar.CommanderVoteData2.CandidaterName[2]),
		g_pNation->GetCandidaterName(WarfieldNo,Nation,2));
	ReturnPacket->u.NationWar.CommanderVoteData2.VoteCount[2]=
		g_pNation->GetCandidaterVoteCount(WarfieldNo,Nation,2);

	strcpy_s(ReturnPacket->u.NationWar.CommanderVoteData2.CandidaterName[3], sizeof(ReturnPacket->u.NationWar.CommanderVoteData2.CandidaterName[3]),
		g_pNation->GetCandidaterName(WarfieldNo,Nation,3));
	ReturnPacket->u.NationWar.CommanderVoteData2.VoteCount[3]=
		g_pNation->GetCandidaterVoteCount(WarfieldNo,Nation,3);

	strcpy_s(ReturnPacket->u.NationWar.CommanderVoteData2.CandidaterName[4], sizeof(ReturnPacket->u.NationWar.CommanderVoteData2.CandidaterName[4]),
		g_pNation->GetCandidaterName(WarfieldNo,Nation,4));
	ReturnPacket->u.NationWar.CommanderVoteData2.VoteCount[4]=
		g_pNation->GetCandidaterVoteCount(WarfieldNo,Nation,4);

	strcpy_s(ReturnPacket->u.NationWar.CommanderVoteData2.CandidaterName[5], sizeof(ReturnPacket->u.NationWar.CommanderVoteData2.CandidaterName[5]),
		g_pNation->GetCandidaterName(WarfieldNo,Nation,5));
	ReturnPacket->u.NationWar.CommanderVoteData2.VoteCount[5]=
		g_pNation->GetCandidaterVoteCount(WarfieldNo,Nation,5);

	ReturnPacket->h.header.size=sizeof(t_CommanderVoteData2);
}


bool CheckJoinLevel(const int SquadNo,const int Level)	// 011028 LTS
{
	switch (SquadNo)
	{
	case 1 :
		if (Level>=NW_SQUAD1_LEVEL_MIN&&Level<=NW_SQUAD1_LEVEL_MAX) return true;
		return false;
	case 2 :
		if (Level>=NW_SQUAD2_LEVEL_MIN&&Level<=NW_SQUAD2_LEVEL_MAX) return true;
		return false;
	case 3 :
		if (Level>=NW_SQUAD3_LEVEL_MIN&&Level<=NW_SQUAD3_LEVEL_MAX) return true;
		return false;
	case 4 :
		if (Level>=NW_SQUAD4_LEVEL_MIN&&Level<=NW_SQUAD4_LEVEL_MAX) return true;
		return false;
	}
	return true;

}

void ProcessCMD_REG_SQUAD_LEADER(t_packet *ReturnPacket,t_packet* ReceivePacket)	// 부대장 등록	// 011028 LTS
{
	const int Nation=ReceivePacket->u.NationWar.RegSquadLeader.Nation;
	const int WarfieldNo=ReceivePacket->u.NationWar.RegSquadLeader.WarfieldNo;
	const int SquadNo=ReceivePacket->u.NationWar.RegSquadLeader.SquadNo;
	const int Level=ReceivePacket->u.NationWar.RegSquadLeader.Level;

	if(	!CheckJoinLevel(SquadNo,Level))//레벨이 되지 않는다.
	{
		ReturnPacket->h.header.type=CMD_SQUAD_JOIN_RESULT;
		ReturnPacket->u.NationWar.CommonDataC.Data=2;
		ReturnPacket->h.header.size=sizeof(t_CommonDataC);
		return;
	}

	int SquadMemberCount=SquadMemberCount=g_pNation->GetSquadMemberCount(Nation,WarfieldNo,SquadNo);
																					//일스의 경우 받을 때 일스의 국가번호를 받는다.
	if (SquadMemberCount>0)															// 이미 부대장이 존재 
	{
		if (CheckJoinSquadCondition(SquadNo,SquadMemberCount))		// 단순히 부대의 편성 조건을 따진다.
		{
			g_pNation->IncSquadMemberCount(Nation,WarfieldNo,SquadNo);
			ReturnPacket->h.header.type=CMD_JOIN_SQUAD;
			if (Nation==NW_YL) ReturnPacket->u.NationWar.JoinSquad.Nation=GetYL_JoinNation();		// 011020 LTS															// 일스 부대 편성을 위한 
			else ReturnPacket->u.NationWar.JoinSquad.Nation=Nation;
			ReturnPacket->u.NationWar.JoinSquad.WarfieldNo=WarfieldNo;
			ReturnPacket->u.NationWar.JoinSquad.SquadNo=SquadNo;
			ReturnPacket->h.header.size=sizeof(t_JoinSquad);
		}
		else
		{
			ReturnPacket->h.header.type=CMD_SQUAD_JOIN_RESULT;
			ReturnPacket->u.NationWar.CommonDataC.Data=3;
			ReturnPacket->h.header.size=sizeof(t_CommonDataC);
		}
	}
	else																			// 부대장 
	{
		t_CommanderInformation SquadLeaderInfo;

		g_pNation->IncSquadMemberCount(Nation,WarfieldNo,SquadNo);
		
		SquadLeaderInfo.CO_ID=1;						
		memcpy(SquadLeaderInfo.CO_Name,ReceivePacket->u.NationWar.RegSquadLeader.ChipName,NW_NAME_MAX);
		SquadLeaderInfo.Fame=ReceivePacket->u.NationWar.RegSquadLeader.Fame;
		SquadLeaderInfo.btLevel=ReceivePacket->u.NationWar.RegSquadLeader.Level;
		SquadLeaderInfo.Face=ReceivePacket->u.NationWar.RegSquadLeader.Face;
		SquadLeaderInfo.Gender=ReceivePacket->u.NationWar.RegSquadLeader.Gender;
		SquadLeaderInfo.Class=ReceivePacket->u.NationWar.RegSquadLeader.Class;		// 011016 LTS
		SquadLeaderInfo.nDual=ReceivePacket->u.NationWar.RegSquadLeader.nDual;		// 011020 LTS
		SquadLeaderInfo.nStep=ReceivePacket->u.NationWar.RegSquadLeader.nStep;		// 011020 LTS
		SquadLeaderInfo.LadderScore=ReceivePacket->u.NationWar.RegSquadLeader.LadderScore;
		g_pNation->SetSquadLeader(Nation,WarfieldNo,SquadNo,&SquadLeaderInfo);

		ReturnPacket->h.header.type=CMD_YOU_ARE_SQUAD_LEADER;							//전쟁번호가 필요하다..
		if (Nation==NW_YL)	// 일스 부대 편성을 위한 //일스의 경우 받을 때 일스의 국가번호를 받는다.
		{
			ReturnPacket->u.NationWar.JoinSquad.Nation=GetYL_JoinNation();	// 011020 LTS
		}
		else ReturnPacket->u.NationWar.JoinSquad.WarfieldNo=Nation;	// 일스의 부대편성 때분이다.
		ReturnPacket->u.NationWar.JoinSquad.WarfieldNo=WarfieldNo;
		ReturnPacket->u.NationWar.JoinSquad.SquadNo=SquadNo;
		ReturnPacket->h.header.size=sizeof(t_JoinSquad);
	}	
}

void ProcessCMD_CHANGE_SQUAD(t_packet *ReturnPacket,t_packet* ReceivePacket)		// 부대 변경
{
	const int Nation=ReceivePacket->u.NationWar.ChangeSquad.Nation;
	const int WarfieldNo=ReceivePacket->u.NationWar.ChangeSquad.WarfieldNo;
	const int NewSquadNo=ReceivePacket->u.NationWar.ChangeSquad.NewSquadNo;
	const int OldSquadNo=ReceivePacket->u.NationWar.ChangeSquad.OldSquadNo;

//	g_pNation->DecSquadMemberCount(Nation,WarfieldNo,OldSquadNo);	// 부대의 종류에 따라 틀려진다.	// 011020 LTS
	g_pNation->IncSquadMemberCount(Nation,WarfieldNo,NewSquadNo);	// 부대의 종류에 따라 틀려진다.
				
	ReturnPacket->h.header.type=CMD_JOIN_SQUAD;
	if (Nation==NW_YL)	// 일스 부대 편성을 위한 //일스의 경우 받을 때 일스의 국가번호를 받는다.
	{
		ReturnPacket->u.NationWar.JoinSquad.Nation=GetYL_JoinNation();		// 011020 LTS
	}
	else ReturnPacket->u.NationWar.JoinSquad.WarfieldNo=Nation;	// 일스의 부대편성 때분이다.
	ReturnPacket->u.NationWar.JoinSquad.WarfieldNo=WarfieldNo;
	ReturnPacket->u.NationWar.JoinSquad.SquadNo=NewSquadNo;
	ReturnPacket->h.header.size=sizeof(t_JoinSquad);
}

void ProcessCMD_SET_SQUAD_LOADING_POINT(t_packet *ReturnPacket,t_packet* ReceivePacket)
{
	g_pNation->SetSquadLoadingPoint(
				ReceivePacket->u.NationWar.SetLoadingPoint.Nation,
				ReceivePacket->u.NationWar.SetLoadingPoint.WarfieldNo,
				ReceivePacket->u.NationWar.SetLoadingPoint.SquadNo,
				ReceivePacket->u.NationWar.SetLoadingPoint.LoadingPoint);
	g_pLoadingPoint[ReceivePacket->u.NationWar.SetLoadingPoint.WarfieldNo][ReceivePacket->u.NationWar.SetLoadingPoint.SquadNo]=ReceivePacket->u.NationWar.SetLoadingPoint.LoadingPoint;

	ReturnPacket->h.header.type=CMD_CHANGE_LOADING_POINT;
	ReturnPacket->u.NationWar.SetLoadingPoint.WarfieldNo=ReceivePacket->u.NationWar.SetLoadingPoint.WarfieldNo;
	ReturnPacket->u.NationWar.SetLoadingPoint.SquadNo=ReceivePacket->u.NationWar.SetLoadingPoint.SquadNo;
	ReturnPacket->u.NationWar.SetLoadingPoint.LoadingPoint=ReceivePacket->u.NationWar.SetLoadingPoint.LoadingPoint;
	ReturnPacket->h.header.size=sizeof(t_SetLoadingPoint);
	SendPacket2NationMaps(NW_BY,ReturnPacket);    //우쒸 나중에 고치자..   //바이서스 일스 자이번에 다 패킷을 보낸다...
	SendPacket2NationMaps(NW_ZY,ReturnPacket);    // 부하가 걸리면.. 로딩점은 한번만.. 수정이 가능하도록 하자.. ㅜ.ㅜ 
	SendPacket2NationMaps(NW_YL,ReturnPacket);
}

void ProcessCMD_REQUEST_NATION_WAR_COUNT(t_packet *ReturnPacket,t_packet* ReceivePacket)
{
	ReturnPacket->h.header.type=CMD_ANSWER_NATION_WAR_COUNT;
	ReturnPacket->u.NationWar.CommonDataDw.Data=g_pNation->GetWarCount();
	ReturnPacket->h.header.size=sizeof(t_CommonDataDw);
}

void ProcessCMD_REQUEST_WARFIELD_INFO(t_packet *ReturnPacket,t_packet* ReceivePacket)
{
	ReturnPacket->h.header.type=CMD_ANSWER_WARFIELD_INFO;			// 현재 상태가 2 이상인 워필드의 방어국과 공격국을 구한다.
	ReturnPacket->u.NationWar.WarfieldInfo.DefenceWarfieldNo=g_pNation->GetWarfieldNationCode(ReceivePacket->u.NationWar.CommonDataC.Data);
	if (ReturnPacket->u.NationWar.WarfieldInfo.DefenceWarfieldNo==NW_BY) 
	{
		ReturnPacket->u.NationWar.WarfieldInfo.AttackWarfieldNo=NW_ZY;
	}
	else 
	{
		ReturnPacket->u.NationWar.WarfieldInfo.AttackWarfieldNo=NW_BY;
	}
	ReturnPacket->h.header.size=sizeof(t_WarfieldInfo);
}

void ProcessCMD_REQUEST_COMMANDER_REG_DATA(t_packet *ReturnPacket,t_packet* ReceivePacket)
{
	t_CommanderInformation		tempCommanderInformation;
	const int Nation=ReceivePacket->u.NationWar.RequestCommanderRegData.Nation;
	const int WarfieldNo=ReceivePacket->u.NationWar.RequestCommanderRegData.WarfieldNo;
	const int CandidaterNo=ReceivePacket->u.NationWar.RequestCommanderRegData.CandidaterNo;
	const bool bCanReg=g_pNation->CanRegCommanderCandidater(Nation,WarfieldNo,CandidaterNo);

	if (bCanReg) // 등록할 수 있다면.... 랜에서는 이미 누가 정보를 갱신했을 수 있다. 
	{
		tempCommanderInformation.CO_ID=1;					//내용저장
		memcpy(tempCommanderInformation.CO_Name,ReceivePacket->u.NationWar.RequestCommanderRegData.Name,NW_NAME_MAX);
		tempCommanderInformation.btLevel=ReceivePacket->u.NationWar.RequestCommanderRegData.Level;
		tempCommanderInformation.Fame=ReceivePacket->u.NationWar.RequestCommanderRegData.Fame;
		tempCommanderInformation.Face=ReceivePacket->u.NationWar.RequestCommanderRegData.Face;
		tempCommanderInformation.Gender=ReceivePacket->u.NationWar.RequestCommanderRegData.Gender;
		tempCommanderInformation.Class=ReceivePacket->u.NationWar.RequestCommanderRegData.Class;		// 011016 LTS
		tempCommanderInformation.nDual=ReceivePacket->u.NationWar.RequestCommanderRegData.nDual;		// 011020 LTS
		tempCommanderInformation.nStep=ReceivePacket->u.NationWar.RequestCommanderRegData.nStep;		// 011020 LTS
		tempCommanderInformation.LadderScore=ReceivePacket->u.NationWar.RequestCommanderRegData.LadderScore;	// 011022 LTS
		g_pNation->SetCandidaterInfo(Nation,WarfieldNo,CandidaterNo,&tempCommanderInformation);

		ReturnPacket->h.header.type=CMD_ANSWER_COMMANDER_REG_DATA;  
		ReturnPacket->u.NationWar.CommonDataC.Data=1;				//등록했다는 메시지 전송
		ReturnPacket->h.header.size=sizeof(t_CommonDataC);
	}
	else
	{
		ReturnPacket->h.header.type=CMD_ANSWER_COMMANDER_REG_DATA; //이미 등록되었다고 반송
		ReturnPacket->u.NationWar.CommonDataC.Data=0;
		ReturnPacket->h.header.size=sizeof(t_CommonDataC);
	}
}

void ProcessCMD_REQUEST_CHECK_WARFIELD_POSSESSION(t_packet *ReturnPacket,t_packet* ReceivePacket)
{
	ReturnPacket->h.header.type=CMD_ANSWER_CHECK_WARFIELD_POSSESSION;
	ReturnPacket->u.NationWar.WarfieldPossession.RequestType=ReceivePacket->u.NationWar.CommonDataC.Data;
	ReturnPacket->u.NationWar.WarfieldPossession.Possession[0]=g_pNation->GetWarfieldNationCode(0);	// LOOP UNROLL
	ReturnPacket->u.NationWar.WarfieldPossession.Possession[1]=g_pNation->GetWarfieldNationCode(1);
	ReturnPacket->u.NationWar.WarfieldPossession.Possession[2]=g_pNation->GetWarfieldNationCode(2);
	ReturnPacket->h.header.size=sizeof(t_WarfieldPossession);
}

void ProcessCMD_REQUEST_WARFIELD_POSSESSION(t_packet *ReturnPacket,t_packet* ReceivePacket)
{
	ReturnPacket->h.header.type=CMD_ANSWER_WARFIELD_POSSESSION;
	ReturnPacket->u.NationWar.WarfieldPossession.Possession[0]=g_pNation->GetWarfieldNationCode(0);
	ReturnPacket->u.NationWar.WarfieldPossession.Possession[1]=g_pNation->GetWarfieldNationCode(1);
	ReturnPacket->u.NationWar.WarfieldPossession.Possession[2]=g_pNation->GetWarfieldNationCode(2);
	ReturnPacket->h.header.size=sizeof(t_WarfieldPossession);
}


void ProcessJoinVote(const int WarfieldNo, const int UserNation,t_packet* ReturnPacket)	// 0 : Delivery 1 : Normal
{
	if (!isNationManageServer()) return;
	if (UserNation==NW_YL)
	{
		ReturnPacket->h.header.type=CMD_ANSWER_NOTICE;
		ReturnPacket->u.NationWar.NoticeJoinVoteD.Type=NW_NOTICE_JOIN_DEFENCE;
		ReturnPacket->u.NationWar.NoticeJoinVoteD.Nation=g_pNation->GetAttacker(WarfieldNo);
		ReturnPacket->h.header.size=sizeof(t_NoticeJoinVoteD);
		return;
	}
	if (isAttacker(WarfieldNo,UserNation))
	{
		ReturnPacket->h.header.type=CMD_ANSWER_NOTICE;
		ReturnPacket->u.NationWar.NoticeJoinVoteA.Type=NW_NOTICE_JOIN_ATTACK;
		ReturnPacket->u.NationWar.NoticeJoinVoteA.WarfieldNo=WarfieldNo;
		ReturnPacket->u.NationWar.NoticeJoinVoteA.VoteCount=g_pNation->GetVoteData(WarfieldNo);
		ReturnPacket->u.NationWar.NoticeJoinVoteA.RemainTime=g_pNation->GetRemainTime(WarfieldNo);	// 011020 LTS
		ReturnPacket->h.header.size=sizeof(t_NoticeJoinVoteA);
		return;
	}
	else
	{
		ReturnPacket->h.header.type=CMD_ANSWER_NOTICE;
		ReturnPacket->u.NationWar.NoticeJoinVoteD.Type=NW_NOTICE_JOIN_DEFENCE;
		ReturnPacket->u.NationWar.NoticeJoinVoteD.Nation=g_pNation->GetAttacker(WarfieldNo);
		ReturnPacket->h.header.size=sizeof(t_NoticeJoinVoteD);
	}
}

void ProcessElectCommanderBefore(const int WarfieldNo,const int UserNation,t_packet* ReturnPacket )
{
	if (UserNation==NW_YL)	// 일스이면 
	{
		ReturnPacket->h.header.type=CMD_ANSWER_NOTICE;
		ReturnPacket->u.NationWar.NoticePrepare1Reinforce.Type=NW_NOTICE_PREPARE1_REINFORCE;
		ReturnPacket->u.NationWar.NoticePrepare1Reinforce.AttackNation=g_pNation->GetAttacker(WarfieldNo);
		ReturnPacket->u.NationWar.NoticePrepare1Reinforce.DefenceNation=g_pNation->GetDefender(WarfieldNo);
		ReturnPacket->u.NationWar.NoticePrepare1Reinforce.WarfieldNo=WarfieldNo;
		ReturnPacket->u.NationWar.NoticePrepare1Reinforce.RemainTime=g_pNation->GetRemainTime(WarfieldNo);
		ReturnPacket->h.header.size=sizeof(t_NoticePrepare1Reinforce);
		return;
	}

	ReturnPacket->h.header.type=CMD_ANSWER_NOTICE;
	ReturnPacket->u.NationWar.NoticePrepare1.Type=NW_NOTICE_PREPARE1;
	ReturnPacket->u.NationWar.NoticePrepare1.WarfieldNo=WarfieldNo;
	if (isAttacker(WarfieldNo,UserNation)) 
	{
		memcpy(ReturnPacket->u.NationWar.NoticePrepare1.LeadCandidaterName,g_pNation->GetLeadCandidaterName(WarfieldNo,NW_ATTACKER),NW_NAME_MAX);
	}
	else 
	{
		memcpy(ReturnPacket->u.NationWar.NoticePrepare1.LeadCandidaterName,g_pNation->GetLeadCandidaterName(WarfieldNo,NW_DEFENCER),NW_NAME_MAX);
	}
	ReturnPacket->u.NationWar.NoticePrepare1.RemainTime=g_pNation->GetRemainTime(WarfieldNo);
	ReturnPacket->h.header.size=sizeof(t_NoticePrepare1);
}

void ProcessElectCommanderAfter(const int WarfieldNo,const int UserNation,t_packet* ReturnPacket  )
{
	if (UserNation==NW_YL)	// 일스이면 
	{
		ReturnPacket->h.header.type=CMD_ANSWER_NOTICE;
		ReturnPacket->u.NationWar.NoticePrepare2Reinforce.Type=NW_NOTICE_PREPARE2_REINFORCE;
		ReturnPacket->u.NationWar.NoticePrepare2Reinforce.AttackNation=g_pNation->GetAttacker(WarfieldNo);
		ReturnPacket->u.NationWar.NoticePrepare2Reinforce.DefenceNation=g_pNation->GetDefender(WarfieldNo);
		ReturnPacket->u.NationWar.NoticePrepare2Reinforce.WarfieldNo=WarfieldNo;
		ReturnPacket->u.NationWar.NoticePrepare2Reinforce.RemainTime=g_pNation->GetRemainTime(WarfieldNo);;
		ReturnPacket->h.header.size=sizeof(t_NoticePrepare2Reinforce);
		return;
	}

	ReturnPacket->h.header.type=CMD_ANSWER_NOTICE;
	ReturnPacket->u.NationWar.NoticePrepare2.Type=NW_NOTICE_PREPARE2;
	ReturnPacket->u.NationWar.NoticePrepare2.WarfieldNo=WarfieldNo;
	memcpy(ReturnPacket->u.NationWar.NoticePrepare2.CommanderName,g_pNation->GetCommanderName(WarfieldNo,UserNation),NW_NAME_MAX);	// 의심
	ReturnPacket->u.NationWar.NoticePrepare2.RemainTime=g_pNation->GetRemainTime(WarfieldNo);;
	ReturnPacket->h.header.size=sizeof(t_NoticePrepare2);
}


void ProcessElectCommanderBeforeAll(const int WarfieldNo,const int UserNation,t_packet* ReturnPacket  )
{
	if (UserNation==NW_YL)	// 일스이면 
	{
		ReturnPacket->h.header.type=CMD_ANSWER_NOTICE;
		ReturnPacket->u.NationWar.NoticePrepare3Reinforce.Type=NW_NOTICE_PREPARE3_REINFORCE;
		ReturnPacket->u.NationWar.NoticePrepare3Reinforce.DefenceNation=g_pNation->GetDefender(WarfieldNo);
		ReturnPacket->u.NationWar.NoticePrepare3Reinforce.RemainTime=g_pNation->GetRemainTime(WarfieldNo);	// 020115 LTS
		ReturnPacket->h.header.size=sizeof(t_NoticePrepare3Reinforce);
		return;
	}

	ReturnPacket->h.header.type=CMD_ANSWER_NOTICE;
	ReturnPacket->u.NationWar.NoticePrepare3.Type=NW_NOTICE_PREPARE3;
	ReturnPacket->u.NationWar.NoticePrepare3.WarfieldNo=WarfieldNo;
	if (isAttacker(WarfieldNo,UserNation)) memcpy(ReturnPacket->u.NationWar.NoticePrepare3.LeadCandidaterName,g_pNation->GetLeadCandidaterName(WarfieldNo,NW_ATTACKER),NW_NAME_MAX);
	else memcpy(ReturnPacket->u.NationWar.NoticePrepare3.LeadCandidaterName,g_pNation->GetLeadCandidaterName(WarfieldNo,NW_DEFENCER),NW_NAME_MAX);
	ReturnPacket->u.NationWar.NoticePrepare3.RemainTime=g_pNation->GetRemainTime(WarfieldNo);;
	ReturnPacket->h.header.size=sizeof(t_NoticePrepare3);
}

void ProcessElectCommanderAfterAll(const int WarfieldNo,const int UserNation,t_packet* ReturnPacket  )
{
	if (UserNation==NW_YL)	// 일스이면 
	{
		ReturnPacket->h.header.type=CMD_ANSWER_NOTICE;
		ReturnPacket->u.NationWar.NoticePrepare4Reinforce.Type=NW_NOTICE_PREPARE4_REINFORCE;
		ReturnPacket->u.NationWar.NoticePrepare4Reinforce.DefenceNation=g_pNation->GetDefender(WarfieldNo);
		ReturnPacket->u.NationWar.NoticePrepare4Reinforce.RemainTime=g_pNation->GetRemainTime(WarfieldNo);;
		ReturnPacket->h.header.size=sizeof(t_NoticePrepare4Reinforce);
		return;
	}

	ReturnPacket->h.header.type=CMD_ANSWER_NOTICE;
	ReturnPacket->u.NationWar.NoticePrepare4.Type=NW_NOTICE_PREPARE4;
	ReturnPacket->u.NationWar.NoticePrepare4.WarfieldNo=WarfieldNo;
	memcpy(ReturnPacket->u.NationWar.NoticePrepare4.CommanderName,g_pNation->GetCommanderName(WarfieldNo,UserNation),NW_NAME_MAX);	// 의심
	ReturnPacket->u.NationWar.NoticePrepare4.RemainTime=g_pNation->GetRemainTime(WarfieldNo);;
	ReturnPacket->h.header.size=sizeof(t_NoticePrepare4);
}

void ProcessWar(const int WarfieldNo,const int UserNation,t_packet* ReturnPacket )
{
	if (WarfieldNo>=0&&WarfieldNo<=2)
	{
		if (UserNation==NW_YL)
		{
			ReturnPacket->h.header.type=CMD_ANSWER_NOTICE;
			ReturnPacket->u.NationWar.NoticeWar1Reinforce.Type=NW_NOTICE_WAR1_REINFORCE;
			ReturnPacket->u.NationWar.NoticeWar1Reinforce.WarfieldNo=WarfieldNo;
			ReturnPacket->u.NationWar.NoticeWar1Reinforce.AttackNation=g_pNation->GetAttacker(WarfieldNo);
			ReturnPacket->u.NationWar.NoticeWar1Reinforce.DefenceNation=g_pNation->GetDefender(WarfieldNo);
			ReturnPacket->u.NationWar.NoticeWar1Reinforce.RemainTime=g_pNation->GetRemainTime(WarfieldNo);;
			ReturnPacket->h.header.size=sizeof(t_NoticeWar1Reinforce);
			return;
		}
		if (isAttacker(WarfieldNo,UserNation))
		{
			ReturnPacket->h.header.type=CMD_ANSWER_NOTICE;
			ReturnPacket->u.NationWar.NoticeWar1A.Type=NW_NOTICE_WAR1A;
			ReturnPacket->u.NationWar.NoticeWar1A.WarfieldNo=WarfieldNo;
			ReturnPacket->u.NationWar.NoticeWar1A.DefenceNation=g_pNation->GetDefender(WarfieldNo);
			ReturnPacket->u.NationWar.NoticeWar1A.RemainTime=g_pNation->GetRemainTime(WarfieldNo);;
			ReturnPacket->h.header.size=sizeof(t_NoticeWar1A);
		}
		else
		{
			ReturnPacket->h.header.type=CMD_ANSWER_NOTICE;
			ReturnPacket->u.NationWar.NoticeWar1D.Type=NW_NOTICE_WAR1D;
			ReturnPacket->u.NationWar.NoticeWar1D.WarfieldNo=WarfieldNo;
			ReturnPacket->u.NationWar.NoticeWar1D.AttackNation=g_pNation->GetAttacker(WarfieldNo);
			ReturnPacket->u.NationWar.NoticeWar1D.RemainTime=g_pNation->GetRemainTime(WarfieldNo);;
			ReturnPacket->h.header.size=sizeof(t_NoticeWar1D);
		}
	}
	else
	if (WarfieldNo>=3)
	{
		ReturnPacket->h.header.type=CMD_ANSWER_NOTICE;
		ReturnPacket->u.NationWar.NoticeWar1D.Type=NW_NOTICE_WAR1D;
		ReturnPacket->u.NationWar.NoticeWar1D.WarfieldNo=WarfieldNo;
		ReturnPacket->u.NationWar.NoticeWar1D.AttackNation=0;
		if (isNewWarfieldServer()) // 030509 kyo 설원전쟁터일때는 설원전쟁터 시간을 리턴한다. 
		{
			ReturnPacket->u.NationWar.NoticeWar1D.RemainTime=g_pNewWarfield->GetRemainTime();
		}
		else
		{
			ReturnPacket->u.NationWar.NoticeWar1D.RemainTime=g_pNation->GetRemainTime(WarfieldNo);
		}

		
		
		ReturnPacket->h.header.size=sizeof(t_NoticeWar1D);
	}
}

void ProcessWarAll(const int WarfieldNo,const int UserNation,t_packet* ReturnPacket )
{
	if (UserNation==NW_YL)
	{
		ReturnPacket->h.header.type=CMD_ANSWER_NOTICE;
		ReturnPacket->u.NationWar.NoticeWar2Reinforce.Type=NW_NOTICE_WAR2_REINFORCE;
		ReturnPacket->u.NationWar.NoticeWar2Reinforce.WarfieldNo=WarfieldNo;
		ReturnPacket->u.NationWar.NoticeWar2Reinforce.DefenceNation=g_pNation->GetDefender(WarfieldNo);
		ReturnPacket->u.NationWar.NoticeWar2Reinforce.RemainTime=g_pNation->GetRemainTime(WarfieldNo);;
		ReturnPacket->h.header.size=sizeof(t_NoticeWar2Reinforce);
		return;
	}
	if (isAttacker(WarfieldNo,UserNation))
	{
		ReturnPacket->h.header.type=CMD_ANSWER_NOTICE;
		ReturnPacket->u.NationWar.NoticeWar2A.Type=NW_NOTICE_WAR2A;
		ReturnPacket->u.NationWar.NoticeWar2A.WarfieldNo=WarfieldNo;
		ReturnPacket->u.NationWar.NoticeWar2A.DefenceNation=g_pNation->GetDefender(WarfieldNo);
		ReturnPacket->u.NationWar.NoticeWar2A.RemainTime=g_pNation->GetRemainTime(WarfieldNo);;
		ReturnPacket->h.header.size=sizeof(t_NoticeWar2A);
	}
	else
	{
		ReturnPacket->h.header.type=CMD_ANSWER_NOTICE;
		ReturnPacket->u.NationWar.NoticeWar2D.Type=NW_NOTICE_WAR2D;
		ReturnPacket->u.NationWar.NoticeWar2D.WarfieldNo=WarfieldNo;
		ReturnPacket->u.NationWar.NoticeWar2D.AttackNation=g_pNation->GetAttacker(WarfieldNo);
		ReturnPacket->u.NationWar.NoticeWar2D.DefenceNation=g_pNation->GetDefender(WarfieldNo);
		ReturnPacket->u.NationWar.NoticeWar2D.RemainTime=g_pNation->GetRemainTime(WarfieldNo);;
		ReturnPacket->h.header.size=sizeof(t_NoticeWar2D);
	}
}

void ProcessPrepare(const int WarfieldNo,const int UserNation,t_packet* ReturnPacket)
{
	if (CheckJoinReinforce())	//전쟁터가 한국가에 점령되었다	// 리턴값 0,3,4,6
	{
		if (!g_pNation->ElectCommander(WarfieldNo))	ProcessElectCommanderBeforeAll(WarfieldNo,UserNation,ReturnPacket);//사령관 선출 전이다
		else ProcessElectCommanderAfterAll(WarfieldNo,UserNation,ReturnPacket);	// 사령관이 선출되었다
	}
    else	// 모든 전쟁터가 한국가에 점령되지 않았다.
	{
		if (!g_pNation->ElectCommander(WarfieldNo))	ProcessElectCommanderBefore(WarfieldNo,UserNation,ReturnPacket);//사령관 선출 전이다
		else ProcessElectCommanderAfter(WarfieldNo,UserNation,ReturnPacket);	// 사령관이 선출되었다
	}
}

void ProcessWarMan(const int WarfieldNo,const int UserNation,t_packet* ReturnPacket)
{	
	if (WarfieldNo>=0&&WarfieldNo<=2)
	{
		if (CheckJoinReinforce((WarfieldNo) ? true:false)) // CSD-030314
		{
			ProcessWarAll(WarfieldNo,UserNation,ReturnPacket);	//전쟁터를 통일했다면
		}
		else 
		{
			ProcessWar(WarfieldNo,UserNation,ReturnPacket);			// 전쟁터를 통일하지 못했다
		}
	}
	else
	if (WarfieldNo>=3)
	{
		ProcessWar(WarfieldNo,UserNation,ReturnPacket);			// 전쟁터를 통일하지 못했다
	}
}

void ProcessSOpen(int WarfieldNo,t_packet* ReturnPacket)
{
	if (WarfieldNo>=3)
	{
		ReturnPacket->h.header.type=CMD_ANSWER_NOTICE;
		ReturnPacket->u.NationWar.NoticeSOpen.Type=NW_NOTICE_SOPEN;
		ReturnPacket->u.NationWar.NoticeSOpen.WarfieldNo=WarfieldNo;
		ReturnPacket->h.header.size=sizeof(NOTICE_SOPEN);
		return;
	}
	ReturnPacket->h.header.type=CMD_ANSWER_NOTICE;
	ReturnPacket->u.NationWar.NoticeSOpen.Type=NW_NOTICE_SOPEN_ERROR;
	ReturnPacket->u.NationWar.NoticeSOpen.WarfieldNo=WarfieldNo;
	ReturnPacket->h.header.size=sizeof(NOTICE_SOPEN);
}

void ProcessPeace(const int UserNation,t_packet* ReturnPacket)			// 011025 LTS
{
	if (UserNation==NW_YL)
	{
		char ReturnV[4];
		ReturnPacket->h.header.type=CMD_ANSWER_NOTICE;
		ReturnPacket->u.NationWar.NoticeWarPeace.Type=NW_NOTICE_PEACE2_REINFORCE;
		ReturnPacket->u.NationWar.NoticeWarPeace.WarNo=g_pNation->GetWarNo();
		g_pNation->GetWarRemainTime(ReturnV);
		ReturnPacket->u.NationWar.NoticeWarPeace.RemainTime[0][0]=ReturnV[0];
		ReturnPacket->u.NationWar.NoticeWarPeace.RemainTime[0][1]=ReturnV[1];
		ReturnPacket->u.NationWar.NoticeWarPeace.RemainTime[1][0]=ReturnV[2];
		ReturnPacket->u.NationWar.NoticeWarPeace.RemainTime[1][1]=ReturnV[3];
		g_pNation->GetNewWarRemainTime(ReturnV);
		ReturnPacket->u.NationWar.NoticeWarPeace.RemainTime[2][0]=ReturnV[0];
		ReturnPacket->u.NationWar.NoticeWarPeace.RemainTime[2][1]=ReturnV[1];
		ReturnPacket->u.NationWar.NoticeWarPeace.SOpenWarfieldNo=-1;
		ReturnPacket->h.header.size=sizeof(NOTICE_WAR_PEACE);
	}
	else
	{
		char ReturnV[4];
		ReturnPacket->h.header.type=CMD_ANSWER_NOTICE;
		ReturnPacket->u.NationWar.NoticeWarPeace2.Type=NW_NOTICE_PEACE2;
		ReturnPacket->u.NationWar.NoticeWarPeace.WarNo=g_pNation->GetWarNo();
		g_pNation->GetWarRemainTime(ReturnV);
		ReturnPacket->u.NationWar.NoticeWarPeace.RemainTime[0][0]=ReturnV[0];
		ReturnPacket->u.NationWar.NoticeWarPeace.RemainTime[0][1]=ReturnV[1];
		ReturnPacket->u.NationWar.NoticeWarPeace.RemainTime[1][0]=ReturnV[2];
		ReturnPacket->u.NationWar.NoticeWarPeace.RemainTime[1][1]=ReturnV[3];
		g_pNation->GetNewWarRemainTime(ReturnV);
		ReturnPacket->u.NationWar.NoticeWarPeace.RemainTime[2][0]=ReturnV[0];
		ReturnPacket->u.NationWar.NoticeWarPeace.RemainTime[2][1]=ReturnV[1];
		ReturnPacket->u.NationWar.NoticeWarPeace.SOpenWarfieldNo=-1;
		ReturnPacket->h.header.size=sizeof(NOTICE_WAR_PEACE);
	}

	if (g_pNation->CheckSecretPlaceOpen())
	{
		int SWarfieldNo=g_pNation->GetSecretOpenedWarfieldNo();
		ReturnPacket->u.NationWar.NoticeWarPeace.SOpenWarfieldNo=SWarfieldNo;
	}
}

void ProcessWarControl(t_packet* ReceivedPacket,t_packet* ReturnPacket)
{
	if (!isNationWarfieldServer()) 
	{
		return;
	}

	if (!ReceivedPacket->u.NationWar.WarControl.Type)	// View
	{
		ReturnPacket->h.header.type=CMD_WAR_CONTROL;
		ReturnPacket->u.NationWar.WarControl.Type=0;	
		ReturnPacket->u.NationWar.WarControl.WarfieldNo=0;	//필요없는 정보 
		ReturnPacket->u.NationWar.WarControl.Status=g_pWarfield->GetStatus();
		ReturnPacket->u.NationWar.WarControl.Active=g_pWarfield->GetLoopActive();
		ReturnPacket->h.header.size=sizeof(t_WarControl);
	}
	else								// Set
	{
		switch(ReceivedPacket->u.NationWar.WarControl.Active)
		{
		case 0 : 
			g_pWarfield->SetLoopActive(false); 
			MyLog(0,"NationWar was Stop By Manager Request : %d,%d,%d,%d",
				ReceivedPacket->u.NationWar.WarControl.Active,
				ReceivedPacket->u.NationWar.WarControl.WarfieldNo,
				ReceivedPacket->u.NationWar.WarControl.Status,
				ReceivedPacket->u.NationWar.WarControl.Active);
			break;
		case 1 : 
			g_pWarfield->SetLoopActive(true); 
			MyLog(0,"NationWar was Stop By Manager Request : %d,%d,%d,%d",
				ReceivedPacket->u.NationWar.WarControl.Active,
				ReceivedPacket->u.NationWar.WarControl.WarfieldNo,
				ReceivedPacket->u.NationWar.WarControl.Status,
				ReceivedPacket->u.NationWar.WarControl.Active);
			break;
		case 2 : break;	// 현재 기능 없음 
		}
		ReturnPacket->h.header.type=CMD_WAR_CONTROL;
		ReturnPacket->u.NationWar.WarControl.Type=0;	
		ReturnPacket->u.NationWar.WarControl.WarfieldNo=0;	//필요없는 정보 
		ReturnPacket->u.NationWar.WarControl.Status=g_pWarfield->GetStatus();
		ReturnPacket->u.NationWar.WarControl.Active=g_pWarfield->GetLoopActive();
		ReturnPacket->h.header.size=sizeof(t_WarControl);
	}
}

void ProcessWarLoopTime(t_packet* ReceivedPacket,t_packet* SendPacket)
{
	if (!isNationManageServer()) 
	{
		
		return;
	}

	if (!ReceivedPacket->u.NationWar.WarLoopTime.Type)		// == 0
	{
/*		SendPacket->h.header.type=CMD_WAR_LOOP_TIME;
		SendPacket->u.NationWar.WarLoopTime.Type=0;			// 필요없는 정보 
		SendPacket->u.NationWar.WarLoopTime.WarfieldNo=0;	// 필요없는 정보 
		SendPacket->u.NationWar.WarLoopTime.Status=g_pWarfield->GetStatus();
		SendPacket->u.NationWar.WarLoopTime.LoopTime=g_pWarfield->GetWarLoopTime();
		SendPacket->h.header.size=sizeof(t_WarLoopTime);*/
	}
	else
	{
		g_pNation->CheckAndSendWarStart(ReceivedPacket);
/*		SendPacket->h.header.type=CMD_WAR_LOOP_TIME;
		SendPacket->u.NationWar.WarLoopTime.Type=0;			// 필요없는 정보 
		SendPacket->u.NationWar.WarLoopTime.WarfieldNo=0;	// 필요없는 정보 
		SendPacket->u.NationWar.WarLoopTime.Status=g_pWarfield->GetStatus();
		SendPacket->u.NationWar.WarLoopTime.LoopTime=g_pWarfield->GetWarLoopTime();
		SendPacket->h.header.size=sizeof(t_WarLoopTime);*/
	}
}

void ProcessNationMemberCount(t_packet* ReceivedPacket,t_packet* SendPacket)
{
	int Nation=ReceivedPacket->u.NationWar.CheckNationMemberCount.Nation;
	int WarfieldNo=ReceivedPacket->u.NationWar.CheckNationMemberCount.WarfieldNo;
	int CurrentNationMemberCount;

	if (isNationWarfieldServer()) 
	{
		CurrentNationMemberCount=g_pWarfield->GetNationMemberCount(Nation);
		if (CurrentNationMemberCount>=g_pWarfield->GetNationMemberMax(Nation))
		{
			SendPacket->h.header.type=CMD_CHECK_WARFIELD_NATION_MEMBERCOUNT;
			SendPacket->u.NationWar.CheckNationMemberCountR.Result=FALSE;
			SendPacket->u.NationWar.CheckNationMemberCountR.WarfieldNo=WarfieldNo;
			SendPacket->h.header.size=sizeof(t_CheckNationMemberCountR);
		}
		else
		{
			SendPacket->h.header.type=CMD_CHECK_WARFIELD_NATION_MEMBERCOUNT;
			SendPacket->u.NationWar.CheckNationMemberCountR.Result=TRUE;
			SendPacket->u.NationWar.CheckNationMemberCountR.WarfieldNo=WarfieldNo;
			SendPacket->h.header.size=sizeof(t_CheckNationMemberCountR);
		}
	}

	if (isNewWarfieldServer())
	{
		CurrentNationMemberCount=g_pNewWarfield->GetNationMemberCount(Nation);
		if (CurrentNationMemberCount>=g_pNewWarfield->GetNationMemberMax(Nation))
		{
			SendPacket->h.header.type=CMD_CHECK_WARFIELD_NATION_MEMBERCOUNT;
			SendPacket->u.NationWar.CheckNationMemberCountR.Result=FALSE;
			SendPacket->u.NationWar.CheckNationMemberCountR.WarfieldNo=WarfieldNo;
			SendPacket->h.header.size=sizeof(t_CheckNationMemberCountR);
		}
		else
		{
			SendPacket->h.header.type=CMD_CHECK_WARFIELD_NATION_MEMBERCOUNT;
			SendPacket->u.NationWar.CheckNationMemberCountR.Result=TRUE;
			SendPacket->u.NationWar.CheckNationMemberCountR.WarfieldNo=WarfieldNo;
			SendPacket->h.header.size=sizeof(t_CheckNationMemberCountR);
		}
	}
}

void ProcessNewWarfieldStatus(t_packet* ReceivedPacket,t_packet* SendPacket)
{
	SendPacket->h.header.type=CMD_ANSWER_NEW_WARFIELD_STATUS;
	for (int i=0;MAX_NEW_WARFIELD > i ;i++)
	{
		SendPacket->u.NationWar.NWStatus.Status[i]=g_pNation->GetNewWarfieldStatus(i);
	}
	SendPacket->h.header.size=sizeof(NEW_WARFIELD_STATUS);
}

void ProcessNewWarfieldData(t_packet* ReceivedPacket,t_packet* SendPacket)
{
	CGuardStone* lpGuard;
	CTeam* lpTeam;
	SendPacket->h.header.type=CMD_ANSWER_NEW_WARFIELD_DATA;
	
	SendPacket->u.NationWar.NWarfieldData1.RemainTime=g_pNewWarfield->GetRemainTime();
	lpGuard=g_pNewWarfield->GetGuard(0);
	lpGuard->GetStatus((char*)&SendPacket->u.NationWar.NWarfieldData1.GuardStatus[0]);
	lpGuard=g_pNewWarfield->GetGuard(1);
	lpGuard->GetStatus((char*)&SendPacket->u.NationWar.NWarfieldData1.GuardStatus[1]);
	lpTeam=g_pNewWarfield->GetTeam(0);
	SendPacket->u.NationWar.NWarfieldData1.TeamCount[0]=lpTeam->GetTeamCount();
	lpTeam=g_pNewWarfield->GetTeam(1);
	SendPacket->u.NationWar.NWarfieldData1.TeamCount[1]=lpTeam->GetTeamCount();
	SendPacket->h.header.size=sizeof(NEW_WARFIELD_DATA1);
}

void RecvCMD_REQUEST_DELIVERY(t_packet *p, t_connection c[], const int cn)
{
	t_packet	tempPacket;
	t_packet	SendPacket;

	const WORD	MapServerPort=p->u.NationWar.Delivery_R.Port;;
	const DWORD	UserID=p->u.NationWar.Delivery_R.UserID;
	memcpy((char*)&tempPacket,p->u.NationWar.Delivery_R.Data,p->u.NationWar.Delivery_R.Size);

	switch (tempPacket.h.header.type)
	{
		case CMD_REQUEST_WARFIELD_STATUS :  
			ProcessCMD_REQUEST_WARFIELD_STATUS(&SendPacket,&tempPacket);
			SendCMD_ANSWER_DELIVERY(UserID,MapServerPort,&SendPacket);
			break;

		case CMD_REQUEST_VOTE_DATA :
			ProcessCMD_REQUEST_VOTE_DATA(&SendPacket,&tempPacket);
			SendCMD_ANSWER_DELIVERY(UserID,MapServerPort,&SendPacket);
			break;
		
		case CMD_VOTING_DATA :						//선거기간이 아니라면? LTS TEMP
			g_pNation->IncVoteData(tempPacket.u.NationWar.CommonDataC.Data);
			break;
		
		case CMD_REQUEST_SQUAD_INFO :				
			ProcessCMD_REQUEST_SQUAD_INFO(&SendPacket,&tempPacket);
			SendCMD_ANSWER_DELIVERY(UserID,MapServerPort,&SendPacket);
			ProcessCMD_REQUEST_SQUAD_INFO2(&SendPacket,&tempPacket);
			SendCMD_ANSWER_DELIVERY(UserID,MapServerPort,&SendPacket);
			break;

		case CMD_REQUEST_COMMANDER_VOTE_DATA :		
			ProcessCMD_REQUEST_COMMANDER_VOTE_DATA(&SendPacket,&tempPacket);
			SendCMD_ANSWER_DELIVERY(UserID,MapServerPort,&SendPacket);
			ProcessCMD_REQUEST_COMMANDER_VOTE_DATA2(&SendPacket,&tempPacket);
			SendCMD_ANSWER_DELIVERY(UserID,MapServerPort,&SendPacket);
			break;

		case CMD_COMMANDER_VOTING_DATA :			
			g_pNation->IncCommanderVoteData(
				tempPacket.u.NationWar.RequestCommanderVoteData.Nation,
				tempPacket.u.NationWar.RequestCommanderVoteData.WarfieldNo,
				tempPacket.u.NationWar.RequestCommanderVoteData.CandidaterNo);
			break;

		case CMD_REG_SQUAD_LEADER :
			ProcessCMD_REG_SQUAD_LEADER(&SendPacket,&tempPacket);
			SendCMD_ANSWER_DELIVERY(UserID,MapServerPort,&SendPacket);
			break;
			
		case CMD_CHANGE_SQUAD :			
			ProcessCMD_CHANGE_SQUAD(&SendPacket,&tempPacket);
			SendCMD_ANSWER_DELIVERY(UserID,MapServerPort,&SendPacket);
			break;

		case CMD_SET_SQUAD_LOADING_POINT :	
			ProcessCMD_SET_SQUAD_LOADING_POINT(&SendPacket,&tempPacket);
			break;

		case CMD_REQUEST_NATION_WAR_COUNT :
			ProcessCMD_REQUEST_NATION_WAR_COUNT(&SendPacket,&tempPacket);
			SendCMD_ANSWER_DELIVERY(UserID,MapServerPort,&SendPacket);
			break;

		case CMD_REQUEST_WARFIELD_INFO :
			ProcessCMD_REQUEST_WARFIELD_INFO(&SendPacket,&tempPacket);
			SendCMD_ANSWER_DELIVERY(UserID,MapServerPort,&SendPacket);
			break;

		case CMD_REQUEST_COMMANDER_REG_DATA :
			ProcessCMD_REQUEST_COMMANDER_REG_DATA(&SendPacket,&tempPacket);
			SendCMD_ANSWER_DELIVERY(UserID,MapServerPort,&SendPacket);
			break;

		case CMD_REQUEST_CHECK_WARFIELD_POSSESSION :
			ProcessCMD_REQUEST_CHECK_WARFIELD_POSSESSION(&SendPacket,&tempPacket);
			SendCMD_ANSWER_DELIVERY(UserID,MapServerPort,&SendPacket);
			break;
		case CMD_REQUEST_WARFIELD_POSSESSION :
			ProcessCMD_REQUEST_WARFIELD_POSSESSION(&SendPacket,&tempPacket);
			SendCMD_ANSWER_DELIVERY(UserID,MapServerPort,&SendPacket);
			break;
		case CMD_REQUEST_WARFIELD_WAR_DATA :
			{
				t_packet packet1,packet2,packet3;

				SendCMD_WARFIELD_WAR_DATA1(&packet1);
				SendCMD_ANSWER_DELIVERY(UserID,MapServerPort,&packet1);
				SendCMD_WARFIELD_WAR_DATA2(&packet2);
				SendCMD_ANSWER_DELIVERY(UserID,MapServerPort,&packet2);
				SendCMD_WARFIELD_WAR_DATA3(&packet3,tempPacket.u.NationWar.RequestWarfieldWarData.WarfieldNo,tempPacket.u.NationWar.RequestWarfieldWarData.Nation);
				SendCMD_ANSWER_DELIVERY(UserID,MapServerPort,&packet3);
				return;
			}
			break;
		case CMD_WAR_CONTROL :
			ProcessWarControl(&tempPacket,&SendPacket);
			SendCMD_ANSWER_DELIVERY(UserID,MapServerPort,&SendPacket);
			break;
		case CMD_WAR_LOOP_TIME :
			ProcessWarLoopTime(&tempPacket,&SendPacket);
//			SendCMD_ANSWER_DELIVERY(UserID,MapServerPort,&SendPacket);
			break;
		case CMD_CHECK_WARFIELD_NATION_MEMBERCOUNT :
			ProcessNationMemberCount(&tempPacket,&SendPacket);
			SendCMD_ANSWER_DELIVERY(UserID,MapServerPort,&SendPacket);
			break;
		case CMD_REQUEST_NEW_WARFIELD_STATUS :
			ProcessNewWarfieldStatus(&tempPacket,&SendPacket);
			SendCMD_ANSWER_DELIVERY(UserID,MapServerPort,&SendPacket);
			break;
		case CMD_REQUEST_NEW_WARFIELD_DATA :
			ProcessNewWarfieldData(&tempPacket,&SendPacket);
			SendCMD_ANSWER_DELIVERY(UserID,MapServerPort,&SendPacket);
			break;

	}
}
void RecvCMD_ANSWER_DELIVERY(t_packet *p, t_connection c[], const int cn)
{
	const DWORD		UserID=p->u.NationWar.Delivery_A.UserID;

	t_packet	tempPacket;
	memcpy((char*)&tempPacket,p->u.NationWar.Delivery_A.Data,p->u.NationWar.Delivery_A.Size);

	switch (tempPacket.h.header.type)		// 맵서버에 세팅해야 되는 정보가 있는 경우 
	{
	case CMD_YOU_ARE_SQUAD_LEADER :
		{
			t_WarBBS WarBBS;
			c[UserID].chrlst.NWCharacter.isSquadLeader=1;
			if (c[UserID].chrlst.name_status.nation==NW_YL)		// 일스의 경우 맵이동시 공격자와 방어자를 구분하기 위해.
			{
				c[UserID].chrlst.NWCharacter.YL_JoinNation=tempPacket.u.NationWar.JoinSquad.Nation;	// 조인해야할 국적을 받았다.
			}
			c[UserID].chrlst.NWCharacter.WarfieldNo=tempPacket.u.NationWar.JoinSquad.WarfieldNo;
			c[UserID].chrlst.NWCharacter.SquadNo=tempPacket.u.NationWar.JoinSquad.SquadNo;

			WarBBS.WarfieldNo=p->u.NationWar.JoinSquad.WarfieldNo;			// 전쟁서버에 있으면 부대장이 되었다고 알려준다.
			WarBBS.BBSType=BBS_SQUADLEADER_ELECT;
			WarBBS.LanNo=0;
			WarBBS.ArgType=BBS_ALPHA;
			WarBBS.Size=strlen(c[UserID].name);
			memcpy(WarBBS.Arg,c[UserID].name,WarBBS.Size);
			SendWarBBS2Clients(&WarBBS);
		}
		break;
	//case CMD_CHANGE_SQUAD :	//패킷이 오지 않는다.. Change도 Join으로 변환되어 온다.
	case CMD_JOIN_SQUAD :
		if (c[UserID].chrlst.name_status.nation==NW_YL)		// 일스의 경우 맵이동시 공격자와 방어자를 구분하기 위해.
		{
			c[UserID].chrlst.NWCharacter.YL_JoinNation=tempPacket.u.NationWar.JoinSquad.Nation; // 조인해야할 국적을 받았다.
		}
		c[UserID].chrlst.NWCharacter.WarfieldNo=tempPacket.u.NationWar.JoinSquad.WarfieldNo;
		c[UserID].chrlst.NWCharacter.SquadNo=tempPacket.u.NationWar.JoinSquad.SquadNo;
		break;
	case CMD_CHECK_WARFIELD_NATION_MEMBERCOUNT :
		if (tempPacket.u.NationWar.CheckNationMemberCountR.Result)	// 맵이동 허락 
		{
			CallNWMapMove(tempPacket.u.NationWar.CheckNationMemberCountR.WarfieldNo,c,UserID);
		}
		else														// 맵이동 불가
		{
			SendCMD_NW_MAP_MOVE_FAIL(c,UserID,5);	// 맵이동 인원이 초과되었음	
		}
		break;
	}

	QueuePacket(c,UserID,&tempPacket,1);
}

void SendCMD_REQUEST_DELIVERY(t_packet *p, t_connection c[], const int cn)
{
	t_packet packet;

	packet.h.header.type=CMD_REQUEST_DELIVERY;
	packet.u.NationWar.Delivery_R.Port=g_wMapServerPort;
	packet.u.NationWar.Delivery_R.UserID=cn;
	packet.u.NationWar.Delivery_R.Size=sizeof(t_header)+p->h.header.size;
	memcpy(packet.u.NationWar.Delivery_R.Data,(char*)p,packet.u.NationWar.Delivery_R.Size);
	packet.h.header.size=sizeof(t_Delivery_R)-MAX_STRING_PK+packet.u.NationWar.Delivery_R.Size;  //100은 버퍼의 수이다. MAX_STRING_PK (NetWork4.h)//020903 lsw
	g_pServerTable->SendRajaPacketToOtherMapServer( NATION_MANAGE_SERVER, (char *)&packet, packet.h.header.size+sizeof(t_header) );
}

void SendCMD_REQUEST_DELIVERY_NOTICE(t_packet *p, t_connection c[], const int cn)
{
	t_packet packet;

	packet.h.header.type=CMD_REQUEST_DELIVERY_NOTICE;
	packet.u.NationWar.Delivery_Notice.Port=g_wMapServerPort;
	packet.u.NationWar.Delivery_Notice.UserID=cn;
	packet.u.NationWar.Delivery_Notice.Nation=c[cn].chrlst.name_status.nation;
	packet.u.NationWar.Delivery_Notice.Size=sizeof(t_header)+p->h.header.size;
	memcpy(packet.u.NationWar.Delivery_Notice.Data,(char*)p,packet.u.NationWar.Delivery_Notice.Size);
	packet.h.header.size=sizeof(t_Delivery_Notice)-MAX_STRING_PK+packet.u.NationWar.Delivery_Notice.Size;  //100은 버퍼의 수이다. MAX_STRING_PK (NetWork4.h)//020903 lsw
	g_pServerTable->SendRajaPacketToOtherMapServer( NATION_MANAGE_SERVER, (char *)&packet, packet.h.header.size+sizeof(t_header) );
}

void RecvCMD_REQUEST_VOTE_DATA(t_packet *p, t_connection c[], const int cn)
{
	t_packet packet;
	if (isNationManageServer())  
	{
		ProcessCMD_REQUEST_VOTE_DATA(&packet,p);
		QueuePacket(c,cn,&packet,1);
	}
	else 
	{
		SendCMD_REQUEST_DELIVERY(p,c,cn);
	}
}

void RecvCMD_VOTING_DATA(t_packet *p, t_connection c[], const int cn)
{
	CHARLIST* ch = CheckServerId(cn);
	if(!ch)return;

	if (ch->NWCharacter.DoWarJoinVote) 
	{
		return;
	}
	
	ch->NWCharacter.DoWarJoinVote=true; // 투표했다고 기록

	if (isNationManageServer())	//패킷 자동 보내기에 의해 무지막지 올라갈수 있다.
	{
		g_pNation->IncVoteData(p->u.NationWar.CommonDataC.Data);
		//MyLog(1,"%d번 국가의 전쟁투표가 %d입니다.",p->u.NationWar.CommonDataC.Data,g_pNation->GetVoteData(p->u.NationWar.CommonDataC.Data));
	}
	else 
	{
		SendCMD_REQUEST_DELIVERY(p,c,cn);
	}
}

void RecvCMD_REQUEST_VOTE_RESULT(t_packet *p, t_connection c[], const int cn )
{
	t_packet packet;
	WORD MapServerPort=p->u.NationWar.CommonDataC.Data+5991; //나중에 바꾸자. 

	packet.h.header.type=CMD_ANSWER_VOTE_RESULT;
	packet.u.NationWar.VoteData.WarfieldNo=p->u.NationWar.CommonDataC.Data;
	packet.u.NationWar.VoteData.VoteCount=g_pNation->GetVoteData(p->u.NationWar.CommonDataC.Data);
	packet.h.header.size=sizeof(t_VoteData);
	g_pServerTable->SendRajaPacketToOtherMapServer( MapServerPort, (char *)&packet, packet.h.header.size+sizeof(t_header) );
}

void RecvCMD_REQUEST_SQUAD_INFO(t_packet *p, t_connection c[], const int cn )
{
	t_packet packet;

	if (isNationManageServer())	//패킷 자동 보내기에 의해 무지막지 올라갈수 있다.
	{
		ProcessCMD_REQUEST_SQUAD_INFO(&packet,p);
		QueuePacket(c,cn,&packet,1);
		ProcessCMD_REQUEST_SQUAD_INFO2(&packet,p);
		QueuePacket(c,cn,&packet,1);
	}
	else 
	{
		SendCMD_REQUEST_DELIVERY(p,c,cn);
	}
}

void RecvCMD_REQUEST_COMMANDER_VOTE_DATA( t_packet *p, t_connection c[], const int cn )
{
	t_packet packet1, packet2;
	const int Nation=p->u.NationWar.RequestCommanderVoteData.Nation;
	const int WarfieldNo=p->u.NationWar.RequestCommanderVoteData.WarfieldNo;
	const int CandidaterNo=p->u.NationWar.RequestCommanderVoteData.CandidaterNo;

	if (isNationManageServer())	//패킷 자동 보내기에 의해 무지막지 올라갈수 있다.
	{
		t_CommanderInformation* tempCandidaterInfo;
		tempCandidaterInfo=g_pNation->GetCandidaterInfo(Nation,WarfieldNo,CandidaterNo); //스쿼드번호를 나타내는 구조체가 없다.. 되면 만들자..
		packet1.h.header.type=CMD_ANSWER_COMMANDER_VOTE_DATA;
        packet1.u.NationWar.CommanderVoteData.VoteNo=CandidaterNo;
		packet1.u.NationWar.CommanderVoteData.CandidaterID=tempCandidaterInfo->CO_ID;
		memcpy(packet1.u.NationWar.CommanderVoteData.Name,tempCandidaterInfo->CO_Name,NW_NAME_MAX);
		packet1.u.NationWar.CommanderVoteData.Level=tempCandidaterInfo->btLevel; // CSD-030806
		packet1.u.NationWar.CommanderVoteData.Fame=tempCandidaterInfo->Fame;
		packet1.u.NationWar.CommanderVoteData.VoteCount=tempCandidaterInfo->VoteCount;
		packet1.u.NationWar.CommanderVoteData.Face=tempCandidaterInfo->Face;
		packet1.u.NationWar.CommanderVoteData.Gender=tempCandidaterInfo->Gender;
		packet1.u.NationWar.CommanderVoteData.Class=tempCandidaterInfo->Class;		// 011016 LTS
		packet1.u.NationWar.CommanderVoteData.nDual=tempCandidaterInfo->nDual;		// 011020 LTS
		packet1.u.NationWar.CommanderVoteData.nStep=tempCandidaterInfo->nStep;		// 011020 LTS
		packet1.u.NationWar.CommanderVoteData.LadderScore=tempCandidaterInfo->LadderScore;
		packet1.h.header.size=sizeof(t_CommanderVoteData);
		QueuePacket(c,cn,&packet1,1);

		packet2.h.header.type=CMD_ANSWER_COMMANDER_VOTE_DATA2;
		strcpy_s(packet2.u.NationWar.CommanderVoteData2.CandidaterName[0], sizeof(packet2.u.NationWar.CommanderVoteData2.CandidaterName[0]),g_pNation->GetCandidaterName(WarfieldNo,Nation,0));
		packet2.u.NationWar.CommanderVoteData2.VoteCount[0]=g_pNation->GetCandidaterVoteCount(WarfieldNo,Nation,0);
		strcpy_s(packet2.u.NationWar.CommanderVoteData2.CandidaterName[1], sizeof(packet2.u.NationWar.CommanderVoteData2.CandidaterName[1]),g_pNation->GetCandidaterName(WarfieldNo,Nation,1));
		packet2.u.NationWar.CommanderVoteData2.VoteCount[1]=g_pNation->GetCandidaterVoteCount(WarfieldNo,Nation,1);
		strcpy_s(packet2.u.NationWar.CommanderVoteData2.CandidaterName[2], sizeof(packet2.u.NationWar.CommanderVoteData2.CandidaterName[2]),g_pNation->GetCandidaterName(WarfieldNo,Nation,2));
		packet2.u.NationWar.CommanderVoteData2.VoteCount[2]=g_pNation->GetCandidaterVoteCount(WarfieldNo,Nation,2);
		strcpy_s(packet2.u.NationWar.CommanderVoteData2.CandidaterName[3], sizeof(packet2.u.NationWar.CommanderVoteData2.CandidaterName[3]),g_pNation->GetCandidaterName(WarfieldNo,Nation,3));
		packet2.u.NationWar.CommanderVoteData2.VoteCount[3]=g_pNation->GetCandidaterVoteCount(WarfieldNo,Nation,3);
		strcpy_s(packet2.u.NationWar.CommanderVoteData2.CandidaterName[4], sizeof(packet2.u.NationWar.CommanderVoteData2.CandidaterName[4]),g_pNation->GetCandidaterName(WarfieldNo,Nation,4));
		packet2.u.NationWar.CommanderVoteData2.VoteCount[4]=g_pNation->GetCandidaterVoteCount(WarfieldNo,Nation,4);
		strcpy_s(packet2.u.NationWar.CommanderVoteData2.CandidaterName[5], sizeof(packet2.u.NationWar.CommanderVoteData2.CandidaterName[5]),g_pNation->GetCandidaterName(WarfieldNo,Nation,5));
		packet2.u.NationWar.CommanderVoteData2.VoteCount[5]=g_pNation->GetCandidaterVoteCount(WarfieldNo,Nation,5);
		packet2.h.header.size=sizeof(t_CommanderVoteData2);

		QueuePacket(c,cn,&packet2,1);

	}
	else 
	{
		SendCMD_REQUEST_DELIVERY(p,c,cn);
	}
}

void RecvCMD_COMMANDER_VOTING_DATA(t_packet *p, t_connection c[], const int cn )
{

	CHARLIST *ch = CheckServerId(cn);
	if(!ch) 
	{
		return;
	}
	if (ch->NWCharacter.DoCommanderVote) 
	{
		return;
	}
	ch->NWCharacter.DoCommanderVote=true;
	if (isNationManageServer())	//패킷 자동 보내기에 의해 무지막지 올라갈수 있다.
	{
		g_pNation->IncCommanderVoteData(
			p->u.NationWar.RequestCommanderVoteData.Nation,
			p->u.NationWar.RequestCommanderVoteData.WarfieldNo,
			p->u.NationWar.RequestCommanderVoteData.CandidaterNo);
	}
	else 
	{
		SendCMD_REQUEST_DELIVERY(p,c,cn);
	}
}

void RecvCMD_JOIN_SQUAD( t_packet *p, t_connection c[], const int cn )		// 클라리언트에게서 받았다.
{
	t_packet packet;

	const int Nation=p->u.NationWar.JoinSquad.Nation;
	const int WarfieldNo=p->u.NationWar.JoinSquad.WarfieldNo;
	const int SquadNo=p->u.NationWar.JoinSquad.SquadNo;

	if (isNationManageServer())	//패킷 자동 보내기에 의해 무지막지 올라갈수 있다.
	{
		if (!CheckJoinLevel(SquadNo,c,cn)) return;		// 011015 LTS //클라이언트에서 막았다.

		const int SquadMemberCount=g_pNation->GetSquadMemberCount(Nation,WarfieldNo,SquadNo);

		if (SquadMemberCount>0)
		{
			if (CheckJoinSquadCondition(SquadNo,SquadMemberCount))		// 단순히 부대의 편성 조건을 따진다.
			{
				g_pNation->IncSquadMemberCount(Nation,WarfieldNo,SquadNo);
				if (c[cn].chrlst.name_status.nation==NW_YL)	c[cn].chrlst.NWCharacter.YL_JoinNation=GetYL_JoinNation();// 유저가 지원군이면..	// 011020 LTS
				c[cn].chrlst.NWCharacter.WarfieldNo=WarfieldNo;
				c[cn].chrlst.NWCharacter.SquadNo=SquadNo;
			}
			else
			{
				packet.h.header.type=CMD_SQUAD_JOIN_RESULT;
				packet.u.NationWar.CommonDataC.Data=3;
				packet.h.header.size=sizeof(t_CommonDataC);
				QueuePacket(c,cn,&packet,1);
			}
		}
		else //부대장이된다 // 부대장이 되었다고 패킷을 보낸다.
		{
			if (!CheckJoinLevel(SquadNo,c,cn)) return;		// 011015 LTS //클라이언트에서 막았다.
			if (CheckCharSquadLeaderCondition(c,cn))
			{
				t_WarBBS WarBBS;
				t_CommanderInformation SquadLeaderInfo;
				g_pNation->IncSquadMemberCount(Nation,WarfieldNo,SquadNo);					// 부대의 종류에 따른다 공격자, 방어자, 지원군 

				SquadLeaderInfo.CO_ID=1;
				memcpy(SquadLeaderInfo.CO_Name,c[cn].name,NW_NAME_MAX);
				SquadLeaderInfo.Fame=c[cn].chrlst.fame;
				SquadLeaderInfo.btLevel=c[cn].chrlst.GetLevel(); // CSD-030806
				SquadLeaderInfo.Face=c[cn].chrlst.Face;
				SquadLeaderInfo.Gender=c[cn].chrlst.Gender;
				SquadLeaderInfo.Class=c[cn].chrlst.Class;				// 011016 LTS
				SquadLeaderInfo.nDual=c[cn].chrlst.GetDualClass();			// 011020 LTS
				SquadLeaderInfo.nStep=c[cn].chrlst.GetClassStep();			// 011020 LTS
				SquadLeaderInfo.LadderScore=c[cn].chrlst.LadderScore;
				g_pNation->SetSquadLeader(Nation,WarfieldNo,SquadNo,&SquadLeaderInfo);		// 부대의 종류에 맞게 스쿼드 리더를 정한다.

				packet.h.header.type=CMD_YOU_ARE_SQUAD_LEADER; //전쟁번호가 필요하다..
				if (c[cn].chrlst.name_status.nation==NW_YL) // 011020 LTS
				{
					packet.u.NationWar.JoinSquad.Nation=GetYL_JoinNation();	// 011020 LTS
					c[cn].chrlst.NWCharacter.YL_JoinNation=packet.u.NationWar.JoinSquad.Nation;
				}
				else packet.u.NationWar.JoinSquad.Nation=Nation;		// 일스의 부대편성 때문이다.
				packet.u.NationWar.JoinSquad.WarfieldNo=WarfieldNo;
				packet.u.NationWar.JoinSquad.SquadNo=SquadNo;
				packet.h.header.size=sizeof(t_JoinSquad);
				QueuePacket(c,cn,&packet,1);

				c[cn].chrlst.NWCharacter.isSquadLeader=1;
				c[cn].chrlst.NWCharacter.WarfieldNo=WarfieldNo;
				c[cn].chrlst.NWCharacter.SquadNo=SquadNo;

				WarBBS.WarfieldNo=p->u.NationWar.JoinSquad.WarfieldNo;			// 전쟁서버에 있으면 부대장이 되었다고 알려준다.
				WarBBS.BBSType=BBS_SQUADLEADER_ELECT;
				WarBBS.LanNo=0;
				WarBBS.ArgType=BBS_ALPHA;
				WarBBS.Size=strlen(c[cn].name);
				memcpy(WarBBS.Arg,c[cn].name,WarBBS.Size);
				SendWarBBS2Clients(&WarBBS);
			}
			else			//부대장 등록 조건이 안돤다
			{
				packet.h.header.type=CMD_SQUAD_JOIN_RESULT;
				packet.u.NationWar.CommonDataC.Data=2;
				packet.h.header.size=sizeof(t_CommonDataC);
				QueuePacket(c,cn,&packet,1);
			}
		}
	}
	else 
	{
		if (CheckCharSquadLeaderCondition(c,cn))
		{
			packet.h.header.type=CMD_REG_SQUAD_LEADER;
			packet.u.NationWar.RegSquadLeader.Nation=c[cn].chrlst.name_status.nation;	// 011020 LTS
			packet.u.NationWar.RegSquadLeader.WarfieldNo=p->u.NationWar.JoinSquad.WarfieldNo;
			packet.u.NationWar.RegSquadLeader.SquadNo=p->u.NationWar.JoinSquad.SquadNo;
			packet.u.NationWar.RegSquadLeader.ChipID=cn;
			packet.u.NationWar.RegSquadLeader.Face=c[cn].chrlst.Face; // 010924 LTS
			packet.u.NationWar.RegSquadLeader.Gender=c[cn].chrlst.Gender; // 010924 LTS
			packet.u.NationWar.RegSquadLeader.Class=c[cn].chrlst.Class; // 011016 LTS
			packet.u.NationWar.RegSquadLeader.nDual=c[cn].chrlst.GetDualClass();	// 011020 LTS
			packet.u.NationWar.RegSquadLeader.nStep=c[cn].chrlst.GetClassStep();	// 011020 LTS
			packet.u.NationWar.RegSquadLeader.LadderScore=c[cn].chrlst.LadderScore; // 010924 LTS
			packet.u.NationWar.RegSquadLeader.Level=c[cn].chrlst.GetLevel(); // CSD-030806
			packet.u.NationWar.RegSquadLeader.Fame=c[cn].chrlst.fame;				// 011028 LTS
			memcpy(packet.u.NationWar.RegSquadLeader.ChipName,c[cn].name,NW_NAME_MAX);
			packet.h.header.size=sizeof(t_RegSquadLeader);

			SendCMD_REQUEST_DELIVERY(&packet,c,cn);
		}
		else
		{
			packet.h.header.type=CMD_SQUAD_JOIN_RESULT;
			packet.u.NationWar.CommonDataC.Data=2;
			packet.h.header.size=sizeof(t_CommonDataC);
			QueuePacket(c,cn,&packet,1);
		}
	}
}

void RecvCMD_CHANGE_SQUAD(t_packet *p, t_connection c[], const int cn )
{
	if (isNationManageServer())	//패킷 자동 보내기에 의해 무지막지 올라갈수 있다.
	{
		g_pNation->IncSquadMemberCount(
			p->u.NationWar.ChangeSquad.Nation,
			p->u.NationWar.ChangeSquad.WarfieldNo,
			p->u.NationWar.ChangeSquad.NewSquadNo);
		//g_pNation->DecSquadMemberCount(		// 011020 LTS
		//	p->u.NationWar.ChangeSquad.Nation,
		//	p->u.NationWar.ChangeSquad.WarfieldNo,
		//	p->u.NationWar.ChangeSquad.OldSquadNo);

		CHARLIST* ch = CheckServerId(cn);
		if( !ch) 
		{
			return;
		}
		if (ch->name_status.nation==NW_YL)
		{
			ch->NWCharacter.YL_JoinNation=CheckJoinReinforce();
		}
		ch->NWCharacter.WarfieldNo=p->u.NationWar.JoinSquad.WarfieldNo;
		ch->NWCharacter.SquadNo=p->u.NationWar.JoinSquad.SquadNo;
	}
	else SendCMD_REQUEST_DELIVERY(p,c,cn);
}

void RecvCMD_SET_SQUAD_LOADING_POINT(t_packet *p, t_connection c[], const int cn )
{
	if (isNationManageServer())	//패킷 자동 보내기에 의해 무지막지 올라갈수 있다.
	{
		t_packet packet;
		ProcessCMD_SET_SQUAD_LOADING_POINT(&packet,p);
		g_pLoadingPoint[p->u.NationWar.SetLoadingPoint.WarfieldNo][p->u.NationWar.SetLoadingPoint.SquadNo]=p->u.NationWar.SetLoadingPoint.LoadingPoint;
	}
	else SendCMD_REQUEST_DELIVERY(p,c,cn);
}

bool CheckCharCommanderCondition(t_connection c[], const int cn)
{
	CHARLIST* ch = CheckServerId(cn);
	if(!ch)
	{
		return false;
	}
	if ((ch->GetLevel()>=NW_COMMANDER_LEVEL_MIN)&&
		(ch->fame>=NW_COMMANDER_FAME_MIN)&&
		(ch->LadderScore>=NW_COMMANDER_LADDER_MIN)) 
	{	//< CSD-030806
		return true;
	}	//> CSD-030806

	return false;
}

void RecvCMD_REQUEST_COMMANDER_REG_DATA(t_packet *p, t_connection c[], const int cn )
{
	t_packet packet;
	t_packet SendPacket;

	if (isNationManageServer())			//국가전관리 서버인가?
	{
		if (CheckCharCommanderCondition(c,cn))
		{
			const int Nation=p->u.NationWar.RequestCommanderVoteData.Nation;			// 원래 Vote데이터로 들어온다
			const int WarfieldNo=p->u.NationWar.RequestCommanderVoteData.WarfieldNo;
			const int CandidaterNo=p->u.NationWar.RequestCommanderVoteData.CandidaterNo;

			const bool bCanReg=g_pNation->CanRegCommanderCandidater(Nation,WarfieldNo,CandidaterNo);

			if (bCanReg) // 등록할 수 있다면.... 랜에서는 이미 누가 정보를 갱신했을 수 있다. 
			{
				t_CommanderInformation		tempCommanderInformation;
				tempCommanderInformation.CO_ID=1;					//내용저장
				strcpy_s(tempCommanderInformation.CO_Name, sizeof(tempCommanderInformation.CO_Name),c[cn].chrlst.Name);
				tempCommanderInformation.btLevel=c[cn].chrlst.GetLevel(); // CSD-030806
				tempCommanderInformation.Fame=c[cn].chrlst.fame;
				tempCommanderInformation.Face=c[cn].chrlst.Face;
				tempCommanderInformation.Gender=c[cn].chrlst.Gender;
				tempCommanderInformation.Class=c[cn].chrlst.Class;		// 011016 LTS
				tempCommanderInformation.nDual=c[cn].chrlst.GetDualClass();		// 011020 LTS
				tempCommanderInformation.nStep=c[cn].chrlst.GetClassStep();		// 011020 LTS
				tempCommanderInformation.LadderScore=c[cn].chrlst.LadderScore;
				g_pNation->SetCandidaterInfo(Nation,WarfieldNo,CandidaterNo,&tempCommanderInformation);

				packet.h.header.type=CMD_ANSWER_COMMANDER_REG_DATA;  
				packet.u.NationWar.CommonDataC.Data=1;				//등록했다는 메시지 전송
				packet.h.header.size=sizeof(t_CommonDataC);
				QueuePacket(c,cn,&packet,1);
				c[cn].chrlst.NWCharacter.isCommanderCandidater=true;
			}
			else
			{
				packet.h.header.type=CMD_ANSWER_COMMANDER_REG_DATA; //이미 등록되었다고 반송
				packet.u.NationWar.CommonDataC.Data=0;
				packet.h.header.size=sizeof(t_CommonDataC);
				QueuePacket(c,cn,&packet,1);
			}

		}
		else
		{
			packet.h.header.type=CMD_ANSWER_COMMANDER_REG_DATA; // 능력치 부족이라고 전송 
			packet.u.NationWar.CommonDataC.Data=2;				
			packet.h.header.size=sizeof(t_CommonDataC);
			QueuePacket(c,cn,&packet,1);
		}
	}
	else 
	{
		if (!c[cn].chrlst.NWCharacter.isCommanderCandidater)
		{
			if (CheckCharCommanderCondition(c,cn))
			{
				SendPacket.h.header.type=p->h.header.type;
				SendPacket.u.NationWar.RequestCommanderRegData.Nation=p->u.NationWar.RequestCommanderVoteData.Nation;
				SendPacket.u.NationWar.RequestCommanderRegData.WarfieldNo=p->u.NationWar.RequestCommanderVoteData.WarfieldNo;
				SendPacket.u.NationWar.RequestCommanderRegData.CandidaterNo=p->u.NationWar.RequestCommanderVoteData.CandidaterNo;
				memcpy(SendPacket.u.NationWar.RequestCommanderRegData.Name,c[cn].name,NW_NAME_MAX);
				SendPacket.u.NationWar.RequestCommanderRegData.Level=c[cn].chrlst.GetLevel(); // CSD-030806
				SendPacket.u.NationWar.RequestCommanderRegData.Fame=c[cn].chrlst.fame;
				SendPacket.u.NationWar.RequestCommanderRegData.Face=c[cn].chrlst.Face;
				SendPacket.u.NationWar.RequestCommanderRegData.Gender=c[cn].chrlst.Gender;
				SendPacket.u.NationWar.RequestCommanderRegData.Class=c[cn].chrlst.Class;		// 011016 LTS
				SendPacket.u.NationWar.RequestCommanderRegData.nDual=c[cn].chrlst.GetDualClass();	// 011020 LTS
				SendPacket.u.NationWar.RequestCommanderRegData.nStep=c[cn].chrlst.GetClassStep();	// 011020 LTS
				SendPacket.u.NationWar.RequestCommanderRegData.LadderScore=c[cn].chrlst.LadderScore;
				SendPacket.h.header.size=sizeof(t_RequestCommanderRegData);
				SendCMD_REQUEST_DELIVERY(&SendPacket,c,cn);
			}
			else
			{
				packet.h.header.type=CMD_ANSWER_COMMANDER_REG_DATA; // 능력치 부족이라고 전송 
				packet.u.NationWar.CommonDataC.Data=2;				
				packet.h.header.size=sizeof(t_CommonDataC);
				QueuePacket(c,cn,&packet,1);
			}
		}
		else
		{
			SendPacket.h.header.type=CMD_ANSWER_COMMANDER_REG_DATA;
			SendPacket.u.NationWar.CommonDataC.Data=0;
			SendPacket.h.header.size=sizeof(t_CommonDataC);
			QueuePacket(c,cn,&SendPacket,1);
		}
	}
}

void RecvCMD_CHANGE_LOADING_POINT(t_packet *p, t_connection c[], const int cn )
{
	const int WarfieldNo = p->u.NationWar.SetLoadingPoint.WarfieldNo;
	const int SquadNo = p->u.NationWar.SetLoadingPoint.SquadNo;
	const int LoadingPoint = p->u.NationWar.SetLoadingPoint.LoadingPoint;
	g_pLoadingPoint[WarfieldNo][SquadNo]=LoadingPoint;
}

void RecvCMD_NATION_WAR_COUNT(t_packet *p, t_connection c[], const int cn )
{
	t_packet packet;

	if (isNationManageServer())
	{
		packet.h.header.type=CMD_ANSWER_NATION_WAR_COUNT;
		packet.u.NationWar.CommonDataDw.Data=g_pNation->GetWarCount();
		packet.h.header.size=sizeof(t_CommonDataDw);
		QueuePacket(c,cn,&packet,1);
	}
	else SendCMD_REQUEST_DELIVERY(p,c,cn);
}

void SendCMD_CHANGED_WARFIELD_STATUS()   // Nation Manage Server-> 맵서버 만 보낸다.
{
	t_packet packet;
	int i = 0, WarCount= 0;

	if (!isNationManageServer()) return;

	WarCount=g_pNation->GetWarCount();							// LTS NEW_NATION_WAR

	packet.h.header.type=CMD_CHANGED_WARFIELD_STATUS;

	packet.u.NationWar.WarfieldStatusM.WarNo=WarCount;
	for (i=0;i<NW_WARFIELD_COUNT;i++)
	{
		g_pWarfieldStatus[i].Status=g_pNation->GetWarfieldStatus(i);  //내것 세팅...
		packet.u.NationWar.WarfieldStatusM.Status[i]=g_pNation->GetWarfieldStatus(i);
	}
	packet.h.header.size=sizeof(t_WarfieldStatus);

	g_pServerTable->BroadCastToEveryServer( (char*)&packet, (sizeof(t_header)+packet.h.header.size), SERVER_TYPE_MAP );

	for(i = DRAGON_CONNECTIONS_START; i<DRAGON_MAX_CONNECTIONS; i++ )			// 관리서버에 있는 유저를 위한 전쟁번호 세팅 
	{
		if (WarCount!=connections[i].chrlst.NWCharacter.WarNo)
		{
			memset(&connections[i].chrlst.NWCharacter,0,sizeof(DWORD));
			connections[i].chrlst.NWCharacter.WarNo=WarCount;
		}
	}
}

void RecvCMD_CHANGED_WARFIELD_STATUS(t_packet *p, t_connection c[], const int cn )	// 일반서버에서 받는다.. 메뉴 로딩포인트 땜시 그런다..
{	
	int i;
	for (i=0;i<NW_WARFIELD_COUNT;i++)
	{
		g_pWarfieldStatus[i].Status=p->u.NationWar.WarfieldStatusM.Status[i];
	}

	for(i = DRAGON_CONNECTIONS_START; i<DRAGON_MAX_CONNECTIONS; i++ )			// 전쟁번호를 갱신한다. 로긴해 있는 유저 
	{
		if (connections[i].chrlst.NWCharacter.WarNo!=p->u.NationWar.WarfieldStatusM.WarNo)
		{
			memset(&connections[i].chrlst.NWCharacter,0,sizeof(DWORD));
			connections[i].chrlst.NWCharacter.WarNo=p->u.NationWar.WarfieldStatusM.WarNo;
		}
	}
}

void RecvCMD_MAP_NATION_CODE_CHANGED(t_packet *p, t_connection c[], const int cn )	// 서버의 동기화 부분이다.. MapInfo
{
	t_packet packet;

	int i = 0;

	if (isNationManageServer())   //관리서버이면.. 다른 맵서버 모두에게 보낸다.
	{
		g_pNation->SetWarfieldNationCode(p->u.NationWar.WarfieldStatusChange.WarfieldNo,p->u.NationWar.WarfieldStatusChange.Status);
		g_pWarfieldStatus[p->u.NationWar.WarfieldStatusChange.WarfieldNo].Possession=p->u.NationWar.WarfieldStatusChange.Status;
		for(i = 0 ;  i< MAX_MAP_ ; i ++ )
		{
			if (MapInfo[i].port==(BASE_WARFIELD_PORT+p->u.NationWar.WarfieldStatusChange.WarfieldNo)) MapInfo[i].nation=p->u.NationWar.WarfieldStatusChange.Status;
			g_pServerTable->SendRajaPacketToOtherMapServer( (WORD)MapInfo[i].port, (char *)p, p->h.header.size+sizeof(t_header) );
		}
		packet.h.header.type=CMD_SET_WARFIELD_POSSESSION;
		packet.u.NationWar.SWarfieldPossession.WarfieldNo=p->u.NationWar.WarfieldStatusChange.WarfieldNo;
		packet.u.NationWar.SWarfieldPossession.Possession=MapInfo[i].nation=p->u.NationWar.WarfieldStatusChange.Status;
		packet.h.header.size=sizeof(t_SWarfieldPossession);
		QueuePacket(connections, DB_DEMON, &packet, 1);

	}
	else  //관리서버로 부터 받았다.
	{
		g_pWarfieldStatus[p->u.NationWar.WarfieldStatusChange.WarfieldNo].Possession=p->u.NationWar.WarfieldStatusChange.Status;
		for(i = 0 ;  i< MAX_MAP_ ; i ++ )
		{
			if (MapInfo[i].port==(BASE_WARFIELD_PORT+p->u.NationWar.WarfieldStatusChange.WarfieldNo))
			{
				MapInfo[i].nation=p->u.NationWar.WarfieldStatusChange.Status;
			}
		}
	}
}

void RecvCMD_REQUEST_TILE_DONT(t_packet *p, t_connection c[], const int cn ) //전쟁서서만 패킷을 받는다. // 맵이동을 했는데 전쟁서버이면 받는다.
{
	if (!isNationWarfieldServer()) return;

	MyLog(LOG_NORMAL, "Requesting Tile Dont");
	g_pWarfield->SendTileDont(p,c,cn); //전쟁서버에 들어오면 한번 보내고 그후 CMD_TILE_DONT_CHANGED를 실행 
	if (g_pWarfield->GetStatus()==NW_WAR) 
	{
		SendWarBeginEndMessage2Client(1,c,cn);		//이동했는데..전쟁중이면.. 전쟁이 시작되었다고 알림..(공격자, 맵이동 방어자)
		g_pWarfield->InsertSquadMember(cn,c[cn].chrlst.bAlive);	// 부대에 삽입한다.		// 전쟁시작 시점에 부대에 편성이 되지 않은 유저는 
		g_pWarfield->IncNationMemberCount(c[cn].chrlst.name_status.nation);
	}																					// 부대 리스트에서도 지워지지 않는다(없다)
}

void RecvCMD_SQUAD_CHAT(t_packet *p, t_connection c[], const int cn )
{
	t_packet packet;

	if (!isNationWarfieldServer()&&!isNewWarfieldServer()) return;
	CHARLIST *ch = CheckServerId(cn);
	if(!ch)
	{
		return;
	}
	char szAddExplain[20]={0,};	//사령관 부대원 붙을꺼. 

	if (isNationWarfieldServer())
	{
		switch (p->u.NationWar.SquadChat.Type)
		{
		case 0 :	// "/*1"
			if (!(ch->NWCharacter.isCommander||ch->NWCharacter.isSquadLeader)) return; //부적절한 사용
			if (ch->NWCharacter.isCommander) 
			{
				if(LocalMgr.IsAbleNation(KOREA))//030102 lsw Local
				{
					MyLog(LOG_NORMAL, "sprintf_s(szAddExplain, sizeof(szAddExplain),'(사령관)'1"); //Eleval 22/08/09 - To debug Nation War (have to be removed later)
					::sprintf_s(szAddExplain, sizeof(szAddExplain),"(사령관)");
				}
				else
				{
					MyLog(LOG_NORMAL, "sprintf_s(szAddExplain, sizeof(szAddExplain),'(Commander)'1"); //Eleval 22/08/09 - To debug Nation War (have to be removed later)
					::sprintf_s(szAddExplain,sizeof(szAddExplain), "(Commander)");
				}
			}
			else 
			{
				if(LocalMgr.IsAbleNation(KOREA))//030102 lsw Local
				{
					MyLog(LOG_NORMAL, "sprintf_s(szAddExplain, sizeof(szAddExplain),'(캧'eAa)'2"); //Eleval 22/08/09 - To debug Nation War (have to be removed later)
					::sprintf_s(szAddExplain, sizeof(szAddExplain),"(부대장)");
				}
				else
				{
					MyLog(LOG_NORMAL, "sprintf_s(szAddExplain, sizeof(szAddExplain),'(SquadLeader)'2"); //Eleval 22/08/09 - To debug Nation War (have to be removed later)
					::sprintf_s(szAddExplain, sizeof(szAddExplain),"(SquadLeader)");
				}
			}
			break;
		case 1 :	// "/*2"
		case 2 :	// "/*3"
		case 3 :	// "/*4"
			{
				if (!ch->NWCharacter.isCommander ) 
				{
					return;	// 부적절
				}
				if(LocalMgr.IsAbleNation(KOREA))//030102 lsw Local
				{
					MyLog(LOG_NORMAL, "sprintf_s(szAddExplain, sizeof(szAddExplain),'(사령관)'3"); //Eleval 22/08/09 - To debug Nation War (have to be removed later)
					::sprintf_s(szAddExplain, sizeof(szAddExplain),"(사령관)");
				}
				else
				{
					MyLog(LOG_NORMAL, "sprintf_s(szAddExplain, sizeof(szAddExplain),'(Commander)'3"); //Eleval 22/08/09 - To debug Nation War (have to be removed later)
					::sprintf_s(szAddExplain, sizeof(szAddExplain),"(Commander)");
				}
			}break;
		case 4 :	// "/*"
			{
				if (0 == ch->NWCharacter.SquadNo) 
				{
					return;	// 0번부대는 부대가 아님 유저의 모임이다.
				}
				if(LocalMgr.IsAbleNation(KOREA))//030102 lsw Local
				{
					MyLog(LOG_NORMAL, "sprintf_s(szAddExplain, sizeof(szAddExplain),'(캧'e퓇)'4"); //Eleval 22/08/09 - To debug Nation War (have to be removed later)
					::sprintf_s(szAddExplain, sizeof(szAddExplain),"(부대원)");
				}
				else
				{
					MyLog(LOG_NORMAL, "sprintf_s(szAddExplain, sizeof(szAddExplain),'(SquadMember)'4"); //Eleval 22/08/09 - To debug Nation War (have to be removed later)
					::sprintf_s(szAddExplain, sizeof(szAddExplain),"(SquadMember)");
				}
			}break;
		default:
			{
				return;//이런 스타일은 없다//020903 lsw
			}break;
		}
	}
	else
	{
		if(LocalMgr.IsAbleNation(KOREA))//030102 lsw Local
		{
			MyLog(LOG_NORMAL, "sprintf_s(szAddExplain, sizeof(szAddExplain),'(*I'e퓇)'5"); //Eleval 22/08/09 - To debug Nation War (have to be removed later)
			::sprintf_s(szAddExplain, sizeof(szAddExplain),"(부대원)");
		}
		else
		{
			MyLog(LOG_NORMAL, "sprintf_s(szAddExplain, sizeof(szAddExplain),'(SquadMember)'5"); //Eleval 22/08/09 - To debug Nation War (have to be removed later)
			::sprintf_s(szAddExplain, sizeof(szAddExplain),"(SquadMember)");
		}
	}

	char tempMessage[MAX_STRING_PK+100] = {0,};

	p->u.NationWar.SquadChat.Message[MAX_STRING_PK-1]= 0;
	MyLog(LOG_NORMAL, "sprintf_s(tempMessage, sizeof(tempMessage), '%s %s:%s', c[cn].name,szAddExplain, p->u.NationWar.SquadChat.Message)"); //Eleval 22/08/09 - To debug Nation War (have to be removed later)
	sprintf_s(tempMessage, sizeof(tempMessage),"%s %s:%s",c[cn].name,szAddExplain, p->u.NationWar.SquadChat.Message);//이름 직책
	
	const int iMaxSize = MAX_STRING_PK;//이게 패킷에 char 배열 크기 이기 때문이다
	const int iNowSize = strlen(tempMessage)+1;//+1은 널 문자
	if( iMaxSize <= iNowSize)//길이 길면 리턴이다
	{
		return;
	}

	packet.h.header.type=CMD_SQUAD_CHAT;
	packet.h.header.size = sizeof(t_SquadChat) - MAX_STRING_PK + iNowSize;
	
	packet.u.NationWar.SquadChat.Type = p->u.NationWar.SquadChat.Type;
	packet.u.NationWar.SquadChat.Size=iNowSize;
	MyLog(LOG_NORMAL, "packet.u.NationWar.SquadChat.Message, sizeof(packet.u.NationWar.SquadChat.Message),tempMessage"); //Eleval 22/08/09 - To debug Nation War (have to be removed later)
	sprintf_s(packet.u.NationWar.SquadChat.Message, sizeof(packet.u.NationWar.SquadChat.Message),tempMessage);

	if (isNationWarfieldServer())
	{
	switch(packet.u.NationWar.SquadChat.Type)
	{
	case 0 : // "/*1" // 사령관,부대장에게.. 보냄..
		if (isAttacker(g_wMapServerPort-BASE_WARFIELD_PORT,c,cn)) 
		{
			g_pWarfield->SendSquadMessage(NW_COMMANDER,NW_ATTACKER,&packet);
		}
		else 
		{
			g_pWarfield->SendSquadMessage(NW_COMMANDER,NW_DEFENCER,&packet);
		}
		break;
	case 1 : // "/*2" // 부대 전체에게 보냄.. 0번 부대 제외
		if (isAttacker(g_wMapServerPort-BASE_WARFIELD_PORT,c,cn))
		{
			for (int i=1;i<NW_SQUAD_MAX;i++) 
			{
				g_pWarfield->SendSquadMessage(NW_ATTACKER,i,&packet);	
			}
		}
		else
		{
			for (int i=1;i<NW_SQUAD_MAX;i++) 
			{
				g_pWarfield->SendSquadMessage(NW_DEFENCER,i,&packet);	// LOOP UNROLL
			}
		}
		break;
	case 2 : // "/*3" // 전쟁터에 있는 자국민 전체에게 보냄
		if (isAttacker(g_wMapServerPort-BASE_WARFIELD_PORT,c,cn))
		{
			for (int i=0;i<NW_SQUAD_MAX;i++) 
			{
				g_pWarfield->SendSquadMessage(NW_ATTACKER,i,&packet);	// LOOP UNROLL
			}
		}
		else
		{
			for (int i=0;i<NW_SQUAD_MAX;i++) 
			{
				g_pWarfield->SendSquadMessage(NW_DEFENCER,i,&packet);	// LOOP UNROLL
			}
		}
		break;
	case 3 : // "/*4" // 자국민 전체에게 보낸다. 
		{
			t_WarBBS tempWarBBS;

			tempWarBBS.WarfieldNo=g_wMapServerPort-BASE_WARFIELD_PORT;
			tempWarBBS.BBSType=BBS_COMMANDER_CHAT;
			tempWarBBS.LanNo=0;
			tempWarBBS.ArgType=BBS_ALPHA;	//문자열
			tempWarBBS.Size=strlen(tempMessage);
			memcpy(tempWarBBS.Arg,tempMessage,tempWarBBS.Size);
			SendWarBBS2NationMaps(c[cn].chrlst.name_status.nation,&tempWarBBS);
		}
		break;
	case 4 :	// "/*"	//부대원 채팅 
		packet.u.NationWar.SquadChat.Type=4;
		if (isAttacker(g_wMapServerPort-BASE_WARFIELD_PORT,c,cn)) 
		{
			g_pWarfield->SendSquadMessage(NW_ATTACKER,c[cn].chrlst.NWCharacter.SquadNo,&packet);
		}
		else 
		{
			g_pWarfield->SendSquadMessage(NW_DEFENCER,c[cn].chrlst.NWCharacter.SquadNo,&packet);
		}
		break;
	}
	}
	else
	{
		g_pNewWarfield->SendSquadChat(&connections[cn].chrlst,&packet);
		
	}
}

void ClearSquadData()
{
	for (int i=DRAGON_CONNECTIONS_START; i<DRAGON_MAX_CONNECTIONS; i++ )
	{
		InitConnectionsSquadData(i);
	}
}

void RecvCMD_CLEAR_SQUAD_DATA( t_packet *p, t_connection c[], const int cn )
{	//< CSD-CN-031213
	if (isNationManageServer())
	{
		SendPacket2Maps(p);				// 맵서버에게 보내고 
	}

	ClearSquadData();
	g_pUserManager->SendPacket(p); 
}	//> CSD-CN-031213

void RecvCMD_REQUEST_WARFIELD_INFO(t_packet *p, t_connection c[], const int cn )
{
	t_packet packet;

	if (isNationManageServer())
	{
		ProcessCMD_REQUEST_WARFIELD_INFO(&packet,p);
		QueuePacket(c,cn,&packet,1);
	}
	else SendCMD_REQUEST_DELIVERY(p,c,cn);
}

void SendCMD_REQUEST_COMMANDER_VOTING_RESULT()
{
	t_packet packet;

	packet.h.header.type=CMD_REQUEST_COMMANDER_VOTING_RESULT;
	packet.u.NationWar.CommonDataC.Data=g_pWarfield->GetWarfieldCode();
	packet.h.header.size=sizeof(t_CommonDataC);

	g_pServerTable->SendRajaPacketToOtherMapServer(NATION_MANAGE_SERVER, (char *)&packet,packet.h.header.size+sizeof(t_header) );
}

void SendCMD_ANSWER_COMMANDER_VOTING_RESULT(const int WarfieldNo,const int Kind,char *tempMessage) //Eleval 22/08/09 - Put 728 to prevent buffer too small thing.
{
	t_packet packet;
	t_packet DemonPacket;

	t_CommanderInformation* Commander;

	Commander=g_pNation->GetCommanderInfoByKind(Kind,WarfieldNo);
	
	MyLog(LOG_NORMAL, "tempMessage, sizeof(tempMessage),'%s',Commander->CO_Name 2"); //Eleval 22/08/09 - To debug Nation War (have to be removed later)
	if (Commander->CO_ID==1)
	{
		
		if(strlen(Commander->CO_Name) > 0)sprintf(tempMessage, "%s",Commander->CO_Name);			// 사령관의 름만 복사해 준다. Eleval 20/08/09 - To prevent from buging in case it's void
		else sprintf(tempMessage, " "); //Eleval 20/08/09 - To prevent from buging in case it's void

		DemonPacket.h.header.type=CMD_SET_COMMANDER;
		strcpy_s(DemonPacket.u.NationWar.SetCommander.CommanderName, sizeof(DemonPacket.u.NationWar.SetCommander.CommanderName),Commander->CO_Name);
		DemonPacket.h.header.size=sizeof(t_SetCommander);
		QueuePacket(connections, DB_DEMON, &DemonPacket, 1);
	}
	else 
		sprintf_s(tempMessage, sizeof(tempMessage),"");								// 사령관의 이름이 없다면 널을 복사한다.

	packet.h.header.type=CMD_ANSWER_COMMANDER_VOTING_RESULT;	
	packet.u.NationWar.CommanderVotingResult.Type=Kind;
	packet.u.NationWar.CommanderVotingResult.ID=Commander->CO_ID;
	strcpy_s(packet.u.NationWar.CommanderVotingResult.Name, sizeof(packet.u.NationWar.CommanderVotingResult.Name),Commander->CO_Name);
	packet.u.NationWar.CommanderVotingResult.Level=Commander->btLevel; // CSD-030806
	packet.u.NationWar.CommanderVotingResult.Fame=Commander->Fame;
	packet.u.NationWar.CommanderVotingResult.Face=Commander->Face;
	packet.u.NationWar.CommanderVotingResult.Gender=Commander->Gender;
	packet.u.NationWar.CommanderVotingResult.Class=Commander->Class;		// 011016 LTS
	packet.u.NationWar.CommanderVotingResult.nDual=Commander->nDual;		// 011020 LTS
	packet.u.NationWar.CommanderVotingResult.nStep=Commander->nStep;		// 011020 LTS
	packet.u.NationWar.CommanderVotingResult.LadderScore=Commander->LadderScore;
	packet.h.header.size=sizeof(t_CommanderVotingResult);
	g_pServerTable->SendRajaPacketToOtherMapServer(WarfieldNo+BASE_WARFIELD_PORT, (char *)&packet,packet.h.header.size+sizeof(t_header) );
}

void CheckCommanderVotingResult(const int WarfieldNo,const int Kind)
{
	int	tempVoteCount=0;
	int	MaxVoteCount=0;
	int CandidaterNo=-1;

	for (int i=0;i<NW_CANDIDATE_MAX;i++)		
	{
		tempVoteCount=g_pNation->GetCommanderVoteDataByKind(WarfieldNo,Kind,i);
		if (tempVoteCount>MaxVoteCount) 
		{
			MaxVoteCount=tempVoteCount;
			CandidaterNo=i;
		}
	}
	g_pNation->SetCommander(WarfieldNo,Kind,CandidaterNo);	// 제일 투표수가 많은 사령관을 지정한다.
}

void RecvCMD_REQUEST_COMMANDER_VOTING_RESULT(t_packet *p, t_connection c[], const int cn ) //사령관투표결과
{
	int BYCandidaterNo=-1,ZYCandidaterNo=-1,YLCandidaterNo=-1;
	int BY_Squad,ZY_Squad,YL_Squad;
	int WarfieldNo=p->u.NationWar.CommonDataC.Data;
	int tempVoteCount=0,MaxVoteCount=0;
	char tempMessage[3][MAX_STRING_PK];
	t_WarBBS	WarBBS[NW_SQUAD_KIND];
	int MapSquadKind;

	if (isNationManageServer())
	{
		BY_Squad=g_pNation->GetSquadKind(NW_BY,WarfieldNo);
		ZY_Squad=g_pNation->GetSquadKind(NW_ZY,WarfieldNo);
		YL_Squad=g_pNation->GetSquadKind(NW_YL,WarfieldNo);

		CheckCommanderVotingResult(WarfieldNo,BY_Squad);
		CheckCommanderVotingResult(WarfieldNo,ZY_Squad);
		CheckCommanderVotingResult(WarfieldNo,YL_Squad);

		SendCMD_ANSWER_COMMANDER_VOTING_RESULT(WarfieldNo,BY_Squad,tempMessage[BY_Squad]);		// tempMessage에 값을 저장하고 돌아온다.
		SendCMD_ANSWER_COMMANDER_VOTING_RESULT(WarfieldNo,ZY_Squad,tempMessage[ZY_Squad]);
		if (CheckJoinReinforce()>0) SendCMD_ANSWER_COMMANDER_VOTING_RESULT(WarfieldNo,YL_Squad,tempMessage[YL_Squad]); // 용병부대 편성일때 보낸다 // 011020 LTS

		WarBBS[NW_ATTACKER].WarfieldNo=WarfieldNo;
		WarBBS[NW_ATTACKER].BBSType=BBS_COMMANDER_ELECT;
		WarBBS[NW_ATTACKER].LanNo=0;
		WarBBS[NW_ATTACKER].ArgType=BBS_ALPHA;
		WarBBS[NW_ATTACKER].Size=strlen(tempMessage[NW_ATTACKER]);
		memcpy(WarBBS[NW_ATTACKER].Arg,tempMessage[NW_ATTACKER],WarBBS[NW_ATTACKER].Size);
	
		WarBBS[NW_DEFENCER].WarfieldNo=WarfieldNo;
		WarBBS[NW_DEFENCER].BBSType=BBS_COMMANDER_ELECT;
		WarBBS[NW_DEFENCER].LanNo=0;
		WarBBS[NW_DEFENCER].ArgType=BBS_ALPHA;
		WarBBS[NW_DEFENCER].Size=strlen(tempMessage[NW_DEFENCER]);
		memcpy(WarBBS[NW_DEFENCER].Arg,tempMessage[NW_DEFENCER],WarBBS[NW_DEFENCER].Size);
		
		WarBBS[NW_REINFORCE].WarfieldNo=WarfieldNo;
		WarBBS[NW_REINFORCE].BBSType=BBS_COMMANDER_ELECT;
		WarBBS[NW_REINFORCE].LanNo=0;
		WarBBS[NW_REINFORCE].ArgType=BBS_ALPHA;
		WarBBS[NW_REINFORCE].Size=strlen(tempMessage[NW_REINFORCE]);
		memcpy(WarBBS[NW_REINFORCE].Arg,tempMessage[NW_REINFORCE],WarBBS[NW_REINFORCE].Size);

		if (CheckJoinReinforce()>0) SendWarBBS2NationMaps(g_pNation->GetDefender(WarfieldNo),&WarBBS[NW_ATTACKER],&WarBBS[NW_DEFENCER],&WarBBS[NW_REINFORCE]); //맵에 방송을 보낸다.	// 011020 LTS
		else // 011020 LTS
		{
			SendWarBBS2NationMaps(g_pNation->GetAttacker(WarfieldNo),&WarBBS[NW_ATTACKER]);	// 011020 LTS
			SendWarBBS2NationMaps(g_pNation->GetDefender(WarfieldNo),&WarBBS[NW_DEFENCER]);
		}
		MapSquadKind=g_pNation->GetSquadKind(MapInfo[0].nation,WarfieldNo);

		SendWarBBS2NationClients(MapInfo[0].nation,&WarBBS[MapSquadKind]);
		FindCommander(tempMessage[MapSquadKind]);
	}
}

void RecvCMD_ANSWER_COMMANDER_VOTING_RESULT(t_packet *p, t_connection c[], const int cn )  // 011016 LTS
{
	t_CommanderInformation Commander;

	if (p->u.NationWar.CommanderVotingResult.ID==1)		// 등록된 유저이다.
	{
		Commander.CO_ID=p->u.NationWar.CommanderVotingResult.ID;
		memcpy(Commander.CO_Name,p->u.NationWar.CommanderVotingResult.Name,NW_NAME_MAX);
		Commander.btLevel=p->u.NationWar.CommanderVotingResult.Level;
		Commander.Fame=p->u.NationWar.CommanderVotingResult.Fame;
		Commander.Face=p->u.NationWar.CommanderVotingResult.Face;
		Commander.Gender=p->u.NationWar.CommanderVotingResult.Gender;
		Commander.Class=p->u.NationWar.CommanderVotingResult.Class;			// 011016 LTS
		Commander.nDual=p->u.NationWar.CommanderVotingResult.nDual;			// 011020 LTS
		Commander.nStep=p->u.NationWar.CommanderVotingResult.nStep;			// 011020 LTS
		Commander.LadderScore=p->u.NationWar.CommanderVotingResult.LadderScore;
		
		switch (p->u.NationWar.CommanderVotingResult.Type)
		{
		case NW_ATTACKER : g_pWarfield->SetCommander(NW_ATTACKER,&Commander); break;
		case NW_DEFENCER : g_pWarfield->SetCommander(NW_DEFENCER,&Commander); break;
		case NW_REINFORCE : g_pWarfield->SetCommander(NW_REINFORCE,&Commander); break;
		}
	}
}

void RecvCMD_REQUEST_CHECK_WARFIELD_POSSESSION(t_packet *p, t_connection c[], const int cn )
{
	t_packet packet;
	if (isNationManageServer())
	{
		ProcessCMD_REQUEST_CHECK_WARFIELD_POSSESSION(&packet,p);
		QueuePacket(c,cn,&packet,1);
	}
	else SendCMD_REQUEST_DELIVERY(p,c,cn);
}

void RecvCMD_FREELANCER_SQUAD_JOIN(t_packet *p, t_connection c[], const int cn )		// 011019 LTS
{
	if (CheckJoinReinforce()>0) return;	// 용병부대 편성을 할수 있는 조건이면 무시한다.
	CHARLIST* ch = CheckServerId(cn);
	if(!ch)
	{
		return;
	}
	if (ch->name_status.nation!=NW_YL) return;			// 일스가 아니면 무시한다.
	if (ch->NWCharacter.YL_JoinNation>0) return;		// 011020 LTS		// 이미 부대편성이 되었있는 유저이다.
	ch->NWCharacter.YL_JoinNation=p->u.NationWar.CommonDataC.Data; // Nationality
}

void RecvCMD_REQUEST_WARFIELD_POSSESSION(t_packet *p, t_connection c[], const int cn )
{
	t_packet packet;
	if (isNationManageServer())
	{
		ProcessCMD_REQUEST_WARFIELD_POSSESSION(&packet,p);
		QueuePacket(c,cn,&packet,1);
	}
	else SendCMD_REQUEST_DELIVERY(p,c,cn);
}

void SendCMD_WARFIELD_WAR_DATA(t_packet *p, t_connection c[], const int cn )
{
	t_packet packet1,packet2,packet3;

	SendCMD_WARFIELD_WAR_DATA1(&packet1);
	QueuePacket(c,cn,&packet1,1);
	SendCMD_WARFIELD_WAR_DATA2(&packet2);
	QueuePacket(c,cn,&packet2,1);
	SendCMD_WARFIELD_WAR_DATA3(&packet3,p->u.NationWar.CommonDataC.Data,c[cn].chrlst.name_status.nation);
	QueuePacket(c,cn,&packet3,1);
}


void RecvCMD_REQUEST_WARFIELD_WAR_DATA(t_packet *p, t_connection c[], const int cn )		//전쟁터로 패킷을 보내서 받은다음.. 처리한다.
{
	const int MapServerPort=p->u.NationWar.CommonDataC.Data+BASE_WARFIELD_PORT;
	
	t_packet packet;

	if (isNationWarfieldServer())
	{
		if (g_wMapServerPort==MapServerPort)
		{
			//해당 맵서버이다.
			SendCMD_WARFIELD_WAR_DATA(p,c, cn );
		}
		else 
		{
			packet.h.header.type=p->h.header.type;
			packet.u.NationWar.RequestWarfieldWarData.WarfieldNo=p->u.NationWar.CommonDataC.Data;
			packet.u.NationWar.RequestWarfieldWarData.Nation=c[cn].chrlst.name_status.nation;
			packet.h.header.size=sizeof(t_RequestWarfieldWarData);

			SendCMD_REQUEST_DELIVERY(MapServerPort,p,c,cn);	// 전송한다. // 부하가 걸리면.. NationManageServer로 변경.. 
		}
	}																		// 패킷량이 많아진다. (Warfield->NationManage Report)		
	else 
	{
		packet.h.header.type=p->h.header.type;
		packet.u.NationWar.RequestWarfieldWarData.WarfieldNo=p->u.NationWar.CommonDataC.Data;
		packet.u.NationWar.RequestWarfieldWarData.Nation=c[cn].chrlst.name_status.nation;
		packet.h.header.size=sizeof(t_RequestWarfieldWarData);

		SendCMD_REQUEST_DELIVERY(MapServerPort,&packet,c,cn);	// 전송한다.
	}
}

void RecvCMD_STARTUP_NATION_CODE_CHANGE(t_packet *p, t_connection c[], const int cn )	// 맵서버의 StartUP시의 워필드의 소유에 대한 것이다.
{
	int i = 0;

	if (isNationManageServer())   //관리서버이면.. 다른 맵서버 모두에게 보낸다.
	{
		g_pNation->SetWarfieldNationCode(p->u.NationWar.WarfieldStatusChange.WarfieldNo,p->u.NationWar.WarfieldStatusChange.Status);
		for(i = 0 ;  i< MAX_MAP_ ; i ++ )
		{
			if (MapInfo[i].port==(BASE_WARFIELD_PORT+p->u.NationWar.WarfieldStatusChange.WarfieldNo)) MapInfo[i].nation=p->u.NationWar.WarfieldStatusChange.Status;
			g_pServerTable->SendRajaPacketToOtherMapServer( (WORD)MapInfo[i].port, (char *)p, p->h.header.size+sizeof(t_header) );
		}
	}
	else  //관리서버로 부터 받았다.
	{
		g_pWarfieldStatus[p->u.NationWar.WarfieldStatusChange.WarfieldNo].Possession=p->u.NationWar.WarfieldStatusChange.Status;
		for(i = 0 ;  i< MAX_MAP_ ; i ++ )
		{
			if (MapInfo[i].port==(BASE_WARFIELD_PORT+p->u.NationWar.WarfieldStatusChange.WarfieldNo))
			{
				MapInfo[i].nation=p->u.NationWar.WarfieldStatusChange.Status;
			}
		}
	}
}

void SendCMD_WAR_END(const int WarfieldNo,const WORD Port)//020903 lsw
{
	t_packet packet;
	packet.h.header.type=CMD_WAR_END;
	packet.u.NationWar.CommonDataC.Data=WarfieldNo;
	packet.h.header.size=sizeof(t_CommonDataC);

	g_pServerTable->SendRajaPacketToOtherMapServer(Port, (char *)&packet, packet.h.header.size+sizeof(t_header) );
}

void RecvCMD_WAR_END( t_packet *p, t_connection c[], const int cn )
{
	if (isNationManageServer())
	{
		g_pNation->ClearWarfieldData(p->u.NationWar.CommonDataC.Data);
	}
}

void SendCMD_REQUEST_SUBSTRACT_MONEY(const int Nation,const int Type,const int Port,const double Money)	// LTS MONEY
{
	t_packet packet;

	packet.h.header.type=CMD_REQUEST_SUBSTRACT_MONEY;
	packet.u.NationWar.RequestSubstractMoney.Nation=Nation;
	packet.u.NationWar.RequestSubstractMoney.Type=Type;
	packet.u.NationWar.RequestSubstractMoney.MapServerPort=Port;
	packet.u.NationWar.RequestSubstractMoney.Money=Money;
	packet.h.header.size=sizeof(t_RequestSubstractMoney);

	g_pServerTable->SendRajaPacketToOtherMapServer( (WORD)NationInfo[Nation].main_map_port, (char *)&packet, packet.h.header.size+sizeof(t_header) );
	MyLog(0,"Money Spread : Request Money : %d",DWORD(Money));
}

// 010915 LTS
void RecvCMD_REQUEST_SUBSTRACT_MONEY( t_packet *p, t_connection c[], const int cn )		// LTS MONEY
{
	t_packet packet;
	const int Nation=p->u.NationWar.RequestSubstractMoney.Nation;
	const int Type=p->u.NationWar.RequestSubstractMoney.Type;
	const int Port=p->u.NationWar.RequestSubstractMoney.MapServerPort;
	const DWORD Money=(DWORD)p->u.NationWar.RequestSubstractMoney.Money;
	char temp[100]= {0,};

	t_WarBBS WarBBS;	// 돈울 썼다고 알림 

	if( g_MainNationMap[Nation] )		// 이곳에서 담당하는 곳이라면
	{
		if (NationInfo[Nation].nation_money>Money)		//돈이 크다 
		{
			NationInfo[Nation].nation_money-=Money;
			SaveNationInfo(Nation);
			
			packet.h.header.type=CMD_ANSWER_SUBSTRACT_MONEY;
			packet.u.NationWar.AnswerSubstractMoney.Type=Type;
			packet.u.NationWar.AnswerSubstractMoney.Result=1;
			packet.h.header.size=sizeof(t_AnswerSubstractMoney);
			g_pServerTable->SendRajaPacketToOtherMapServer(Port, (char *)&packet, packet.h.header.size+sizeof(t_header) );

			WarBBS.WarfieldNo=Port-BASE_WARFIELD_PORT;
			WarBBS.BBSType=BBS_NATION_MONEY_USE;
			WarBBS.LanNo=Type;
			WarBBS.ArgType=BBS_ALPHA;
			MyLog(LOG_NORMAL, "temp, sizeof(temp), '%d', Money"); //Eleval 22/08/09 - To debug Nation War (have to be removed later)
			sprintf_s(temp, sizeof(temp),"%d",Money);
			WarBBS.Size=strlen(temp)+2;				// 011022 LTS
			WarBBS.Arg[0]=Nation;
			WarBBS.Arg[1]=Port-BASE_WARFIELD_PORT;	// 011022 LTS
			memcpy(&WarBBS.Arg[2],temp,WarBBS.Size);// 011022 LTS
			SendWarBBS2Maps(&WarBBS);
			SendWarBBS2Clients(&WarBBS);
			MyLog(0,"Money Spread : Substract Money : %d",Money);
		}
		else
		{
			packet.h.header.type=CMD_ANSWER_SUBSTRACT_MONEY;
			packet.u.NationWar.AnswerSubstractMoney.Type=Type;
			packet.u.NationWar.AnswerSubstractMoney.Result=0;
			packet.h.header.size=sizeof(t_AnswerSubstractMoney);
			g_pServerTable->SendRajaPacketToOtherMapServer(Port, (char *)&packet, packet.h.header.size+sizeof(t_header) );

			WarBBS.WarfieldNo=Port-BASE_WARFIELD_PORT;
			WarBBS.BBSType=BBS_NATION_MONEY_NOT_USE;
			WarBBS.LanNo=Type;
			WarBBS.ArgType=BBS_ALPHA;
			MyLog(LOG_NORMAL, "temp, sizeof(temp), '%d', Money 2"); //Eleval 22/08/09 - To debug Nation War (have to be removed later)
			sprintf_s(temp, sizeof(temp),"%d",Money);
			WarBBS.Size=strlen(temp)+2;				// 011022 LTS
			WarBBS.Arg[0]=Nation;
			WarBBS.Arg[1]=Port-BASE_WARFIELD_PORT;	// 011022 LTS
			memcpy(&WarBBS.Arg[2],temp,WarBBS.Size);// 011022 LTS
			SendWarBBS2Maps(&WarBBS);
			SendWarBBS2Clients(&WarBBS);
			MyLog(0,"Money Spread : Can not Substract Money : %d",Money);
		}
	}
	else								// 아니면
	{									// 담당 맵으로 보낸다.
		g_pServerTable->SendRajaPacketToOtherMapServer( (WORD)NationInfo[Nation].main_map_port, (char *)p, p->h.header.size+sizeof(t_header) );
	}
}

void RecvCMD_ANSWER_SUBSTRACT_MONEY( t_packet *p, t_connection c[], const int cn )
{
	MyLog(0,"Money spread : RecvCMD_ANSWER_SUBSTRACT_MONEY Type : %d , Result : %d",p->u.NationWar.AnswerSubstractMoney.Type,p->u.NationWar.AnswerSubstractMoney.Result);
	switch(p->u.NationWar.AnswerSubstractMoney.Type)
	{
	case 0 :	// 전쟁에서 져서
		MyLog(0,"Money Spread : Switch 0 Type : %d , Result : %d",p->u.NationWar.AnswerSubstractMoney.Type,p->u.NationWar.AnswerSubstractMoney.Result);
		if (p->u.NationWar.AnswerSubstractMoney.Result)			// 010927 LTS
		{
			if (isNationWarfieldServer())	//전쟁 서버이면 돈을 뿌리려고 한상태.
			{
				g_pWarfield->MoneySpread(15000000);				// 010927 LTS
			}
		}
		break;

	case 1 :	// 사령관이 아이템 구매	// 여기로 들어오지 않는다. // Delivery packet 처리 예정
	case 2 :	// 사령관이 무기 구매
		break;
	}
}

void RecvCMD_REQUEST_WAR_GIVE_LIFE( t_packet *p, t_connection c[], const int cn )			// 011015 LTS
{
	if (!isNationWarfieldServer()&&!isNewWarfieldServer()) return;
	if (isNationWarfieldServer())
		if (g_pWarfield->GetStatus()!=NW_WAR) return;
	if (isNewWarfieldServer())
		if (g_pNewWarfield->GetWarfieldStatus()!=NW_WAR) return;

	if (c[cn].chrlst.Hp>0) return;

	if (isNationWarfieldServer())
	{
		if (isAttacker(g_pWarfield->GetWarfieldCode(),c,cn))	//공격자
		{
			POINTS AttackerGiveLifePoint=g_pWarfield->GetLivePoints(NW_ATTACKER);
			MovePc(cn,AttackerGiveLifePoint.x,AttackerGiveLifePoint.y);
		}
		else	// 방어자 
		{
			POINTS DefencerGiveLifePoint=g_pWarfield->GetLivePoints(NW_DEFENCER);
			MovePc(cn,DefencerGiveLifePoint.x,DefencerGiveLifePoint.y);
		}
	}
	else
	{
		POINT MovePoint=g_pNewWarfield->GetLivePoint(&c[cn].chrlst);
		MovePc(cn,MovePoint.x,MovePoint.y);
	}

	SkillMgr.CharacterToAlive(&c[cn].chrlst, 1);// 살려준다.//020501 lsw
}

void RecvCMD_WAR_GIVE_LIFE2(t_packet *p, t_connection c[], int cn )				// 011213 LTS
{

	const int GiveLifePoint=p->u.NationWar.CommonDataC.Data;

	if (GiveLifePoint<0||GiveLifePoint>3) return;
	if (!isNationWarfieldServer()) return;
	if (g_pWarfield->GetStatus()!=NW_WAR) return;
	if (c[cn].chrlst.Hp>0) return;

	if (isAttacker(g_pWarfield->GetWarfieldCode(),c,cn))	//공격자
	{
		POINTS AttackerGiveLifePoint=AttackPoint[g_pWarfield->GetWarfieldCode()][GiveLifePoint];
		MovePc(cn,AttackerGiveLifePoint.x,AttackerGiveLifePoint.y);
	}
	else	// 방어자 
	{
		POINTS DefencerGiveLifePoint=DefencePoint[g_pWarfield->GetWarfieldCode()];
		MovePc(cn,DefencerGiveLifePoint.x,DefencerGiveLifePoint.y);
	}
	SkillMgr.CharacterToAlive( &c[cn].chrlst, 1 );//020501 lsw
}

void KickDefeatUser2Home(t_connection c[],const int cn)						// 011015 LTS
{
	switch (c[cn].chrlst.name_status.nation)
	{
		case 3 :	MapMove(cn,"MA-IN",245,115); break;
		case 4 :	MapMove(cn,"RENES_C",210,175); break;
		case 6 :	MapMove(cn,"BARANTAN",325,98);break;
	}
}

void RecvCMD_CHECK_AM_I_DEFEAT(t_packet *p, t_connection c[], const int cn )		// 011015 LTS
{
	CHARLIST *ch = CheckServerId(cn);
	if(!ch){return;}

	if (!isNationWarfieldServer()) 
	{
		if (ch->name_status.nation==NW_YL) // 011020 LTS				// 전쟁터 맵에 없는 유저이면 초기화 해준다.
			memset(&ch->NWCharacter,0,sizeof(DWORD));		// 011020 LTS	//버튼을 누른후 초기화 한다.
		return;
	}

	if (ch->name_status.nation==NW_YL)		// 일스이면 패배한 용병들만 보낸다
	{
		if (ch->NWCharacter.YL_JoinNation!=g_pWarfield->GetNationCode()) 
		{
			KickDefeatUser2Home(c,cn);
		}
		memset(&ch->NWCharacter,0,sizeof(DWORD));		// 011020 LTS	//버튼을 누른후 초기화 한다.
		return;			// 011020 LTS
	}
	if (ch->name_status.nation!=g_pWarfield->GetNationCode()) 
	{
		KickDefeatUser2Home(c,cn);
	}
}

void RecvCMD_SQUAD_LOADING_POINT( t_packet *p, t_connection c[], const int cn )	// NW_MAN -> NW_WAR
{
	if (!isNationWarfieldServer()) return;			//부대 로딩포인트를 저장한다	// LOOP UNROLL이 빠르다 

	g_pWarfield->SetSquadLoadingPoint(NW_ATTACKER,0,p->u.NationWar.SquadLoadingPoint.SquadLoadingPoint[NW_ATTACKER][0]);
	g_pWarfield->SetSquadLoadingPoint(NW_ATTACKER,1,p->u.NationWar.SquadLoadingPoint.SquadLoadingPoint[NW_ATTACKER][1]);
	g_pWarfield->SetSquadLoadingPoint(NW_ATTACKER,2,p->u.NationWar.SquadLoadingPoint.SquadLoadingPoint[NW_ATTACKER][2]);
	g_pWarfield->SetSquadLoadingPoint(NW_ATTACKER,3,p->u.NationWar.SquadLoadingPoint.SquadLoadingPoint[NW_ATTACKER][3]);
	g_pWarfield->SetSquadLoadingPoint(NW_ATTACKER,4,p->u.NationWar.SquadLoadingPoint.SquadLoadingPoint[NW_ATTACKER][4]);

	g_pWarfield->SetSquadLoadingPoint(NW_DEFENCER,0,p->u.NationWar.SquadLoadingPoint.SquadLoadingPoint[NW_DEFENCER][0]);
	g_pWarfield->SetSquadLoadingPoint(NW_DEFENCER,1,p->u.NationWar.SquadLoadingPoint.SquadLoadingPoint[NW_DEFENCER][1]);
	g_pWarfield->SetSquadLoadingPoint(NW_DEFENCER,2,p->u.NationWar.SquadLoadingPoint.SquadLoadingPoint[NW_DEFENCER][2]);
	g_pWarfield->SetSquadLoadingPoint(NW_DEFENCER,3,p->u.NationWar.SquadLoadingPoint.SquadLoadingPoint[NW_DEFENCER][3]);
	g_pWarfield->SetSquadLoadingPoint(NW_DEFENCER,4,p->u.NationWar.SquadLoadingPoint.SquadLoadingPoint[NW_DEFENCER][4]);

	g_pWarfield->SetSquadLoadingPoint(NW_REINFORCE,0,p->u.NationWar.SquadLoadingPoint.SquadLoadingPoint[NW_REINFORCE][0]);
	g_pWarfield->SetSquadLoadingPoint(NW_REINFORCE,1,p->u.NationWar.SquadLoadingPoint.SquadLoadingPoint[NW_REINFORCE][1]);
	g_pWarfield->SetSquadLoadingPoint(NW_REINFORCE,2,p->u.NationWar.SquadLoadingPoint.SquadLoadingPoint[NW_REINFORCE][2]);
	g_pWarfield->SetSquadLoadingPoint(NW_REINFORCE,3,p->u.NationWar.SquadLoadingPoint.SquadLoadingPoint[NW_REINFORCE][3]);
	g_pWarfield->SetSquadLoadingPoint(NW_REINFORCE,4,p->u.NationWar.SquadLoadingPoint.SquadLoadingPoint[NW_REINFORCE][4]);
}


void RecvCMD_SQUAD_RARE(t_packet *p, t_connection c[], const int cn )
{
	if (!isNationWarfieldServer()) return;

	for (int i=0;i<NW_SQUAD_KIND;i++)		// LOOP UNROLL이 속도가 빠르나 보기 힘들다. 
	{
		for (int j=0;j<NW_SQUAD_MAX;j++)
		{
			for (int k=0;k<NW_SQUAD_RARE_MAX;k++)
			{
				g_pWarfield->SetSquadRareSok(i,j,k,p->u.NationWar.NWSquadRare.SquadRare[i][j][k]);
			}
		}
	}
}

void RecvCMD_REQUEST_NOTICE(t_packet *p, t_connection c[], const int cn )
{
	if (isNationManageServer())
	{
		t_packet SendPacket;
		int Nation=c[cn].chrlst.name_status.nation;
		if (CheckWarLoopProcess())	//전쟁루프가 진행중이면 평화상태가 아니다.
		{
			int WarfieldNo=ReturnWarfieldNoByWarLoopProcess();
			int Status=g_pNation->GetWarfieldStatus(WarfieldNo);
			switch(Status)
			{
			case 2 : ProcessJoinVote(WarfieldNo,Nation,&SendPacket); break;
			case 3 : ProcessPrepare(WarfieldNo,Nation,&SendPacket);	break;
			case 4 : ProcessWarMan(WarfieldNo,Nation,&SendPacket); break;
			}
		}
		else ProcessPeace(Nation,&SendPacket);						// 평화상태이다.
		QueuePacket(c,cn,&SendPacket,1);

	}
	else SendCMD_REQUEST_DELIVERY_NOTICE(p,c,cn);
}

void RecvCMD_REQUEST_DELIVERY_NOTICE( t_packet *p, t_connection c[], const int cn )
{
	t_packet	tempPacket;
	t_packet	SendPacket;

	const int Port=p->u.NationWar.Delivery_Notice.Port;
	const int ServerID=p->u.NationWar.Delivery_Notice.UserID;
	const int Nation=p->u.NationWar.Delivery_Notice.Nation;

	const int iSize = p->u.NationWar.Delivery_Notice.Size;
	if(iSize >= sizeof(t_packet))
	{
		return;
	}
	memcpy((char*)&tempPacket,p->u.NationWar.Delivery_Notice.Data,iSize);
	switch (tempPacket.h.header.type)
	{
	case CMD_REQUEST_NOTICE :
			{
				if (CheckWarLoopProcess())	//전쟁루프가 진행중이면 평화상태가 아니다.
				{
					int WarfieldNo=ReturnWarfieldNoByWarLoopProcess();
					int Status;
					if (WarfieldNo>=0&&WarfieldNo<=2)
						Status=g_pNation->GetWarfieldStatus(WarfieldNo);
					else
					if (WarfieldNo>=3)
						Status=g_pNation->GetNewWarfieldStatus(WarfieldNo-3);

					switch(Status)
					{
					case 2 : ProcessJoinVote(WarfieldNo,Nation,&SendPacket); break;
					case 3 : ProcessPrepare(WarfieldNo,Nation,&SendPacket);	break;
					case 4 : ProcessWarMan(WarfieldNo,Nation,&SendPacket); break;
					case NW_SOPEN : ProcessSOpen(WarfieldNo,&SendPacket); break;
					}
				}
				else ProcessPeace(Nation,&SendPacket);						// 평화상태이다.
				SendCMD_ANSWER_DELIVERY(ServerID,Port,&SendPacket);
			}
			break;
	}
}

void RecvCMD_SET_REMAIN_TIME(t_packet *p, t_connection c[], const int cn )
{
	if (!isNationManageServer()) 
	{
		return;
	}
	int WarfieldNo=p->u.NationWar.RemainTime.WarfieldNo;
	g_pNation->SetRemainTime(WarfieldNo,p->u.NationWar.RemainTime.RemainTime);
	//MyLog(0,"전쟁번호 : 상황시간 %d전쟁터:%d상태:%d남은시간",p->u.NationWar.RemainTime.WarfieldNo,p->u.NationWar.RemainTime.Status,p->u.NationWar.RemainTime.RemainTime);
}

void RecvCMD_WAR_CONTROL(t_packet *p, t_connection c[], const int cn )		// 011025 LTS
{
	t_packet packet;
	const int Port=BASE_WARFIELD_PORT+p->u.NationWar.WarControl.WarfieldNo;

//	if (!isNationWarfieldServer()) return;				//전쟁서버가 아니면 리턴
//	if (g_pWarfield->GetWarfieldCode()!=p->u.NationWar.WarControl.WarfieldNo) return;	// 해당 전쟁터가 아니면 리턴

	if (p->u.NationWar.WarControl.WarfieldNo==3)	// 결계석 디버그용
	{
		if (!isNationWarfieldServer()) 
      return;
		g_pWarfield->WeaponDataList();
    return;
		
	}

	if (isNationWarfieldServer())
	{
		if (g_pWarfield->GetWarfieldCode()==p->u.NationWar.WarControl.WarfieldNo)
		{
			ProcessWarControl(p,&packet);
			QueuePacket(c,cn,&packet,1);
		}
		else SendCMD_REQUEST_DELIVERY(Port,p,c,cn);
	}
	else SendCMD_REQUEST_DELIVERY(Port,p,c,cn);
}

void RecvCMD_WAR_LOOP_TIME( t_packet *p, t_connection c[], const int cn )	// 011025 LTS
{
	CHARLIST* ch = CheckServerId( cn );
	if( !ch ) return;
	if( !ch->IsCounselor() ) return;

	if (isNationManageServer())
	{
		g_pNation->CheckAndSendWarStart(p);
	}
	else SendCMD_REQUEST_DELIVERY(NATION_MANAGE_SERVER,p,c,cn);
}

void RecvCMD_SECRET_OPEN( t_packet *p, t_connection c[], const int cn )//020205 lsw
{
	if (!isNationManageServer()) return;
	
	switch(p->u.NationWar.SecretOpen.WarfieldNo)
	{
		case 0: 
		case 1: 
		case 2:
			{
				g_pNation->SetSecretOpen(p->u.NationWar.SecretOpen.WarfieldNo,((p->u.NationWar.SecretOpen.isOpen)?true:false));
			}break;

		default:
			{
				MyLog(LOG_NORMAL,"||	==============!!!Error!!!=================	||");
				MyLog(LOG_NORMAL,"||	RecvCMD_SECRET_OPEN()  WarfieldNo ===> %d \n	||",p->u.NationWar.SecretOpen.WarfieldNo);
				MyLog(LOG_NORMAL,"||	==========================================	||");
			}break;
	}
}

void RecvCMD_START_WAR_LOOP(t_packet *p, t_connection c[], const int cn )		// LTS NEW_NATION_WAR
{
	if (!isNationWarfieldServer())
		return;

	g_pWarfield->SetStatusVote();												// Warfield 를 투표상태로 만든다.
}

void RecvCMD_NWARFIELD_START_WAR(t_packet *p, t_connection c[], const int cn )		// New Warfield Start Packet
{	//< CSD-CN-031213
	if (isNewWarfieldServer())			//전쟁서버이면
	{	// 전쟁관련 초기화를 한다.
		g_pNewWarfield->SetWarfieldStatus(NW_WAR);									// War Start
	}
	
	g_pNewWarfieldStatus[p->u.NationWar.NewWarStart.WarfieldNo]=NW_WAR;
	g_pUserManager->SendPacket(p); 
}	//> CSD-CN-031213

void SendTileDontInfo(t_connection c[], const int cn )
{
	if (!isNewWarfieldServer())
		return;

	t_packet packet;
	packet.h.header.type=CMD_ANSWER_NEW_WARFIELD_TILE_DONT;
	g_pNewWarfield->GetTileDont(&packet.u.NationWar.DontData);
	packet.h.header.size=sizeof(DONT_DATA);

	QueuePacket(c,cn,&packet,1);
}

void RecvCMD_REQUEST_NEW_WARFIELD_TILE_DONT(t_packet *p, t_connection c[], const int cn )
{
	MyLog(0, "Tile Dont Request");

	SendTileDontInfo(c,cn);
	// 부대에 조인시킨다.
	// 전쟁이 사작했다는 메세지를 보내준다.
	if (g_pNewWarfield->GetWarfieldStatus() == NW_WAR)
	{
		//이동했는데..전쟁중이면.. 전쟁이 시작되었다고 알림..(공격자, 맵이동 방어자)
		SendWarBeginEndMessage2Client(1, c, cn); 
		
		switch (c[cn].chrlst.name_status.nation)
		{
		case NW_BY:
			{
				g_pNewWarfield->GetTeam(0)->InsertTeamMember(cn);
				break;
			}
		case NW_ZY:
			{
				g_pNewWarfield->GetTeam(1)->InsertTeamMember(cn);
				break;
			}
		case NW_YL:
			{
				if (c[cn].chrlst.NWCharacter.YL_JoinNation == NW_BY)
				{
					g_pNewWarfield->GetTeam(0)->InsertTeamMember(cn);
				}
				else
				{
					g_pNewWarfield->GetTeam(1)->InsertTeamMember(cn);
				}

				break;
			}
		}
	}
}

void RecvCMD_NWARFIELD_END_WAR(t_packet *p, t_connection c[], const int cn )
{	//< CSD-CN-031213
	g_pNewWarfieldStatus[p->u.NationWar.CommonDataC.Data] = NW_PEACE;
	g_pUserManager->SendPacket(p); 
}	//> CSD-CN-031213

void RecvCMD_CHECK_DEFEAT( t_packet *p, t_connection c[], const int cn )
{
	if (!isNewWarfieldServer())
		return;

	g_pNewWarfield->CheckAndKickUser(cn);
}

void RecvCMD_LOOPTIME_CHANGE(t_packet *p, t_connection c[], const int cn )
{
	if (isNationWarfieldServer())
		g_pWarfield->LoopTimeChange(p);
	if (isNewWarfieldServer())
		g_pNewWarfield->LoopTimeChange(p);
}

void RecvCMD_WAR_RESULT( t_packet *p, t_connection c[], const int cn )
{	//< CSD-CN-031213
	g_pUserManager->SendPacket(p); 
}	//> CSD-CN-031213

void RecvCMD_NWARFIELD_STATUS_CHANGE(t_packet *p, t_connection c[], const int cn )
{
	g_pNewWarfieldStatus[p->u.NationWar.WarfieldStatusChange.WarfieldNo]=p->u.NationWar.WarfieldStatusChange.Status;
	if (isNationManageServer())
		g_pNation->SetNewWarfieldStatus(p->u.NationWar.WarfieldStatusChange.WarfieldNo,p->u.NationWar.WarfieldStatusChange.Status);
}

void RecvCMD_REQUEST_NEW_WARFIELD_STATUS(t_packet *p, t_connection c[], const int cn )
{
	if (isNationManageServer())
	{
		t_packet packet;
		ProcessNewWarfieldStatus(NULL,&packet);
		QueuePacket(c,cn,&packet,1);
	}
	else
		SendCMD_REQUEST_DELIVERY(NATION_MANAGE_SERVER,p,c,cn);
}

void RecvCMD_REQUEST_NEW_WARFIELD_DATA(t_packet *p, t_connection c[], const int cn )
{
	if (isNewWarfieldServer())
	{
		t_packet packet;
		ProcessNewWarfieldData(p,&packet);
		QueuePacket(c,cn,&packet,1);
	}
	else
		SendCMD_REQUEST_DELIVERY(BASE_NEW_WARFIELD_PORT+p->u.NationWar.CommonDataC.Data-3,p,c,cn);
}

void RecvCMD_WAR_YL_JOIN( t_packet *p, t_connection c[], const int cn )
{
	if (c[cn].chrlst.name_status.nation!=NW_YL)
		return;

	c[cn].chrlst.NWCharacter.YL_JoinNation=p->u.NationWar.CommonDataC.Data;
	t_packet packet;
	packet.h.header.type=CMD_WAR_YL_JOIN;
	packet.u.NationWar.CommonDataC.Data=p->u.NationWar.CommonDataC.Data;
	packet.h.header.size=sizeof(t_CommonDataC);

	QueuePacket(c,cn,&packet,1);
}

void RecvCMD_GUARD_BROKE(t_packet *p, t_connection c[], const int cn )
{	//< CSD-CN-031213
	g_pUserManager->SendPacket(p); 
}	//> CSD-CN-031213

int CheckHandleByNationWar( t_packet *p, t_connection c[], const int cn )
{
	const int iType = p->h.header.type;
	switch(iType)
	{ 
	case CMD_CHECK_WARFIELD_STATUS : 
											RecvCMD_CHECK_WARFIELD_STATUS(p,c,cn); 
											break; 
	case CMD_CHECK_WARFIELD_STATUS_RESULT : 
											RecvCMD_CHECK_WARFIELD_STATUS_RESULT(p,c,cn); 
											break; 
	case CMD_WARFIELD_STATUS : 
											RecvCMD_WARFIELD_STATUS(p,c,cn); 
											break;
	case CMD_WAR_BBS : 
											RecvCMD_WARBBS(p); 
											break;
	case CMD_REQUEST_WARFIELD_STATUS :
											RecvCMD_REQUEST_WARFIELD_STATUS(p,c,cn);
											break;
	case CMD_REQUEST_DELIVERY :
											RecvCMD_REQUEST_DELIVERY(p,c,cn);
											break;
	case CMD_ANSWER_DELIVERY :
											RecvCMD_ANSWER_DELIVERY(p,c,cn);
											break;
	case CMD_REQUEST_VOTE_DATA :
											RecvCMD_REQUEST_VOTE_DATA(p,c,cn);
											break;
	case CMD_VOTING_DATA :
											RecvCMD_VOTING_DATA(p,c,cn);
											break;
	case CMD_REQUEST_VOTE_RESULT :
											RecvCMD_REQUEST_VOTE_RESULT(p,c,cn);
											break;
	case CMD_ANSWER_VOTE_RESULT :
											g_pWarfield->RecvCMD_ANSWER_VOTE_RESULT(p,c,cn); //전쟁서버가 요구하는 패킷 
											break;
	case CMD_REQUEST_SQUAD_INFO :
											RecvCMD_REQUEST_SQUAD_INFO(p,c,cn);
											break;
	case CMD_REQUEST_COMMANDER_VOTE_DATA :
											RecvCMD_REQUEST_COMMANDER_VOTE_DATA(p,c,cn);
											break;
	case CMD_COMMANDER_VOTING_DATA :
											RecvCMD_COMMANDER_VOTING_DATA(p,c,cn);
											break;
	case CMD_JOIN_SQUAD :
											RecvCMD_JOIN_SQUAD(p,c,cn);
											break;
	case CMD_CHANGE_SQUAD :
											RecvCMD_CHANGE_SQUAD(p,c,cn);
											break;
	case CMD_SET_SQUAD_LOADING_POINT :
											RecvCMD_SET_SQUAD_LOADING_POINT(p,c,cn);
											break;
	case CMD_REQUEST_COMMANDER_REG_DATA :
											RecvCMD_REQUEST_COMMANDER_REG_DATA(p,c,cn);
											break;
	case CMD_CHANGE_LOADING_POINT :
											RecvCMD_CHANGE_LOADING_POINT(p,c,cn);
											break;
	case CMD_REQUEST_NATION_WAR_COUNT :
											RecvCMD_NATION_WAR_COUNT(p,c,cn);
											break;
	case CMD_NW_MAP_MOVE :
											RecvCMD_NW_MAP_MOVE(p,c,cn);
											break;
	case CMD_CHANGED_WARFIELD_STATUS :
											RecvCMD_CHANGED_WARFIELD_STATUS(p,c,cn);
											break;
	case CMD_MAP_NATION_CODE_CHANGED :
											RecvCMD_MAP_NATION_CODE_CHANGED(p,c,cn);
											break;
	case CMD_REQUEST_TILE_DONT :
											RecvCMD_REQUEST_TILE_DONT(p,c,cn);
											break;
	case CMD_SQUAD_CHAT :
											RecvCMD_SQUAD_CHAT(p,c,cn);
											break;
	case CMD_CLEAR_SQUAD_DATA :
											RecvCMD_CLEAR_SQUAD_DATA(p,c,cn);
											break;
	case CMD_REQUEST_WARFIELD_INFO :
											RecvCMD_REQUEST_WARFIELD_INFO(p,c,cn);
											break;
	case CMD_REQUEST_COMMANDER_VOTING_RESULT :
											RecvCMD_REQUEST_COMMANDER_VOTING_RESULT(p,c,cn);
											break;
	case CMD_ANSWER_COMMANDER_VOTING_RESULT :
											RecvCMD_ANSWER_COMMANDER_VOTING_RESULT(p,c,cn);
											break;
	case CMD_REQUEST_CHECK_WARFIELD_POSSESSION :
											RecvCMD_REQUEST_CHECK_WARFIELD_POSSESSION(p,c,cn);
											break;
	case CMD_FREELANCER_SQUAD_JOIN :
											RecvCMD_FREELANCER_SQUAD_JOIN(p,c,cn);
											break;
	case CMD_REQUEST_WARFIELD_POSSESSION :
											RecvCMD_REQUEST_WARFIELD_POSSESSION(p,c,cn);
											break;
	case CMD_REQUEST_WARFIELD_WAR_DATA :
											RecvCMD_REQUEST_WARFIELD_WAR_DATA(p,c,cn);
											break;
	case CMD_REQUEST_CHECK_WARNO :			
											RecvCMD_REQUEST_CHECK_WARNO(p,c,cn);
											break;
	case CMD_ANSWER_CHECK_WARNO :
											RecvCMD_ANSWER_CHECK_WARNO(p,c,cn);
											break;
	case CMD_STARTUP_NATION_CODE_CHANGE	:
											RecvCMD_STARTUP_NATION_CODE_CHANGE(p,c,cn);
											break;
	case CMD_WAR_END :
											RecvCMD_WAR_END(p,c,cn);
											break;
	case CMD_REQUEST_SUBSTRACT_MONEY :
											RecvCMD_REQUEST_SUBSTRACT_MONEY(p,c,cn);
											break;
	case CMD_ANSWER_SUBSTRACT_MONEY :
											RecvCMD_ANSWER_SUBSTRACT_MONEY(p,c,cn);
											break;
	case CMD_REQUEST_WAR_GIVE_LIFE :		// 011015 LTS			
											RecvCMD_REQUEST_WAR_GIVE_LIFE(p,c,cn);
											break;
	case CMD_CHECK_AM_I_DEFEAT :
											RecvCMD_CHECK_AM_I_DEFEAT(p,c,cn);
											break;
	case CMD_SQUAD_LOADING_POINT :
											RecvCMD_SQUAD_LOADING_POINT(p,c,cn);
											break;
	case CMD_SQUAD_RARE :
											RecvCMD_SQUAD_RARE(p,c,cn);
											break;
	case CMD_REQUEST_NOTICE :
											RecvCMD_REQUEST_NOTICE(p,c,cn);
											break;
	case CMD_REQUEST_DELIVERY_NOTICE :
											RecvCMD_REQUEST_DELIVERY_NOTICE(p,c,cn);
											break;
	case CMD_SET_REMAIN_TIME :
											RecvCMD_SET_REMAIN_TIME(p,c,cn);
											break;
	case CMD_WAR_CONTROL :													// 011020 LTS
											RecvCMD_WAR_CONTROL(p,c,cn);
											break;
	case CMD_WAR_LOOP_TIME :												// 011025 LTS	
											RecvCMD_WAR_LOOP_TIME(p,c,cn);
											break;
	case CMD_SECRET_OPEN :
											RecvCMD_SECRET_OPEN(p,c,cn);	// 011025 LTS
											break;
	case CMD_WAR_GIVE_LIFE2 :					
											RecvCMD_WAR_GIVE_LIFE2(p,c,cn);	// 011213 LTS
											break;
	case CMD_START_WAR_LOOP :
											RecvCMD_START_WAR_LOOP(p,c,cn);		// LTS NEW_NATION_WAR
											break;
	case CMD_NWARFIELD_START_WAR :
											RecvCMD_NWARFIELD_START_WAR(p,c,cn);		// LTS NEW_NATION_WAR
											break;
	case CMD_REQUEST_NEW_WARFIELD_TILE_DONT :
											RecvCMD_REQUEST_NEW_WARFIELD_TILE_DONT(p,c,cn);
											break;
	case CMD_CHECK_DEFEAT :
											RecvCMD_CHECK_DEFEAT(p,c,cn);
											break;
	case CMD_LOOPTIME_CHANGE :
											RecvCMD_LOOPTIME_CHANGE(p,c,cn);
											break;
	case CMD_WAR_RESULT :
											RecvCMD_WAR_RESULT(p,c,cn);
											break;
	case CMD_NWARFIELD_STATUS_CHANGE :
											RecvCMD_NWARFIELD_STATUS_CHANGE(p,c,cn);
											break;
	case CMD_REQUEST_NEW_WARFIELD_STATUS :
											RecvCMD_REQUEST_NEW_WARFIELD_STATUS(p,c,cn);
											break;
	case CMD_REQUEST_NEW_WARFIELD_DATA :
											RecvCMD_REQUEST_NEW_WARFIELD_DATA(p,c,cn);
											break;
	case CMD_WAR_YL_JOIN :
											RecvCMD_WAR_YL_JOIN(p,c,cn);
											break;
	case CMD_NWARFIELD_END_WAR :
											RecvCMD_NWARFIELD_END_WAR(p,c,cn);
											break;
	case CMD_GUARD_BROKE :			
											RecvCMD_GUARD_BROKE(p,c,cn);
											break;
	default : CheckHandleByLTSSupport(p,c,cn);								// 011112 LTS
	}
	return 1;
}

// Name  : cWarfield Class
// Usage : Class Member Funtion Definition

cWarfield::cWarfield()
{
	int i,j=0;
	InitWarfield();								// 멤버변수를 초기화 한다.
	InitSquad();								// 부대편성정보를 초기화한다. 
	InitCommanderCandidater();					//  사령관 후보데이터를 초기화한다.
	ClearCommander();							//사령관데이터를 초기화 한다.
	for (i=0;i<MAX_MONSTER_POINT;i++)
	{	
		m_tMonsterPoint[i].Points=NULL;			// 타일돈트 관련 지점이다..
	}

	if (!isNationManageServer())				// 관리서버가 아니고 국가전 전쟁터이면 실행한다.
	{
		ClearSquadCount();						// 부대의 인원수 카운트를  초기화 한다.
		LoadWarfieldData(g_wMapServerPort);  //관리서버이면.. 자동으로 만들어 주어야 한다.
		LoadWarfieldPossession();					// 전쟁터 테이터를 읽어온다.	바로위와 같은거다.
		LoadWarfieldWeaponData(g_wMapServerPort); //관리서버도 필요한가?		
		LoadWarfieldMonsterData(g_wMapServerPort);	//몬스터 데이터를 로드한다.
		LoadNationWar_Exp();						// 전쟁 경험치를 얻어온다.			// 020115 LTS
		FirstMakeWeaponData();						// 전쟁무기, 성문,수호석 등을 배치한다. 소환몬스터이다.
		for (i=2;i<m_cNpcCount+2;i++)
		{
			if (NPCList[i].SprNo==63) m_cGuardStoneCount++;			// 가드스톤의 수를 센다
			if (NPCList[i].SprNo==64) m_cGuardStoneBossCount++;		// 가드스톤 보스의 수를 센다
			if (NPCList[i].SprNo>=65&&NPCList[i].SprNo<=67) m_cCastleDoorCount++;	// 성문의 수를 센다.
		}

		LoadMonsterPoint();						// 성문의 타일돈트를 메모리에 넣는다. // 추후 파일에서 읽어온다.
//		LoadMonsterPoint2();
	}
	if (isNationWarfieldServer())	InitWarLoop(); // 국가전 서버이면 루프 초기 시작 시간을 구한다.
	else m_Status=1;								// 관리서버이면 무조건 현재의 상태를 1로세트한다.
}


cWarfield::~cWarfield()
{
	Destroy();
}

void cWarfield::Destroy()
{
	for (int i=0;i<MAX_MONSTER_POINT;i++)
	{
		if (m_tMonsterPoint[i].Points!=NULL) 
		{
			SAFE_DELETE(m_tMonsterPoint[i].Points);
		}
	}
	DeleteAllKiller();
	DeleteAllMonsterData();
	DeleteAllSquadMember();
}

cWarfield& cWarfield::operator[](int nIndex)
{
  static cWarfield cWar_Field_temp;
	if( nIndex < 0 || nIndex >= 3 ) return cWar_Field_temp;
	return *(this+nIndex);
}

bool cWarfield::InitWarfield()
{
	m_cLoopActive=1;
	m_cGuardStoneBrokenCount=0;
	m_cCastleDoorBrokenCount=0;
	m_cGuardStoneCount=0;
	m_cCastleDoorCount=0;
	m_cGuardStoneBossCount=0;
	m_bAttackerWin=false;
	m_bRequestCommanderVotingResult=false;
	m_bFirstMakeMonster=false;						//몬스터가 만들어 진 적이 없다.
	m_bBonusActive=false;
	m_dRemainTime=0;
	memset(&m_tWeaponData,0,sizeof(m_tWeaponData));   // WeaponData의 초기화 
	memset(&m_bCommanderExist,0,sizeof(bool)*2);	// LTS TEMP
	memset(&m_bSquadLeaderExist,0,sizeof(bool)*NW_SQUAD_KIND*NW_SQUAD_MAX);
	InitWarLoopDefine();
	return true;
}

void cWarfield::InitWarLoopDefine()	// 011105 LTS
{
/*	char	NetworkDir[MAX_PATH];
	char	MapServerConfigFileName[MAX_PATH];

	if (GetPrivateProfileString("Network","path","",NetworkDir,MAX_PATH,MAP_SERVER_INI_)<=0)
	{
		MyLog(0,"Error!! Check Mapserver.ini's Path Set.");
		return ;
	}

	sprintf(MapServerConfigFileName,"%s/data/MapServerConfig.ini",NetworkDir);
*/// 030919 HK YGI
	m_LoopTimeDefine.NWPeaceTime1=(DWORD)GetPrivateProfileInt("WarLoopDefine","PeaceTime1",0,MapServerConfigFileName);		// 전쟁후의 평화시간 
	if (!m_LoopTimeDefine.NWPeaceTime1) m_LoopTimeDefine.NWPeaceTime1=NW_PEACETIME1;

	m_LoopTimeDefine.NWPeaceTime2=(DWORD)GetPrivateProfileInt("WarLoopDefine","PeaceTime2",0,MapServerConfigFileName);		// 산중전쟁터의 최초 전쟁루프 시작시간 
	if (!m_LoopTimeDefine.NWPeaceTime2) m_LoopTimeDefine.NWPeaceTime2=NW_PEACETIME2;

	m_LoopTimeDefine.NWPeaceTime3=(DWORD)GetPrivateProfileInt("WarLoopDefine","PeaceTime3",0,MapServerConfigFileName);		// 평원전쟁터의 최초 전쟁루프 시작시간 
	if (!m_LoopTimeDefine.NWPeaceTime3) m_LoopTimeDefine.NWPeaceTime3=NW_PEACETIME3;

	m_LoopTimeDefine.NWPeaceTime4=(DWORD)GetPrivateProfileInt("WarLoopDefine","PeaceTime4",0,MapServerConfigFileName);		// 섬  전쟁터의 최초 전쟁루프 시작시간 
	if (!m_LoopTimeDefine.NWPeaceTime4) m_LoopTimeDefine.NWPeaceTime4=NW_PEACETIME4;

	m_LoopTimeDefine.NWVoteTime=(DWORD)GetPrivateProfileInt("WarLoopDefine","VoteTime",0,MapServerConfigFileName);		// 전쟁 투표 시간
	if (!m_LoopTimeDefine.NWVoteTime) m_LoopTimeDefine.NWVoteTime=NW_VOTETIME;

	m_LoopTimeDefine.NWPrepareTime=(DWORD)GetPrivateProfileInt("WarLoopDefine","PrepareTime",0,MapServerConfigFileName);		// 전쟁 준비 시간 
	if (!m_LoopTimeDefine.NWPrepareTime) m_LoopTimeDefine.NWPrepareTime=NW_PREPARETIME;

	m_LoopTimeDefine.NWWarTime=(DWORD)GetPrivateProfileInt("WarLoopDefine","WarTime",0,MapServerConfigFileName);			// 전쟁 시간 
	if (!m_LoopTimeDefine.NWWarTime) m_LoopTimeDefine.NWWarTime=NW_WARTIME;

	m_LoopTimeDefine.NWBonusTime=(DWORD)GetPrivateProfileInt("WarLoopDefine","BonusTime",0,MapServerConfigFileName);;		// 비밀지역 오픈시간
	if (!m_LoopTimeDefine.NWBonusTime) m_LoopTimeDefine.NWBonusTime=NW_BONUS;

	m_LoopTimeDefine.NWVoteMin=(DWORD)GetPrivateProfileInt("WarLoopDefine","VoteMin",0,MapServerConfigFileName);;		// 전쟁 참여 투표 인원 // LTS LOCALWAR
	if (!m_LoopTimeDefine.NWVoteMin) m_LoopTimeDefine.NWVoteMin=NW_NATION_VOTE_MIN;

	m_LoopTimeDefine.NWBYJoinMAX=(WORD)GetPrivateProfileInt("WarLoopDefine","BYJoinMax",0,MapServerConfigFileName);;		// 전쟁 참여 투표 인원 // LTS LOCALWAR
	if (!m_LoopTimeDefine.NWBYJoinMAX) m_LoopTimeDefine.NWBYJoinMAX=NW_BY_JOIN_MAX;

	m_LoopTimeDefine.NWZYJoinMAX=(WORD)GetPrivateProfileInt("WarLoopDefine","ZYJoinMax",0,MapServerConfigFileName);;		// 전쟁 참여 투표 인원 // LTS LOCALWAR
	if (!m_LoopTimeDefine.NWZYJoinMAX) m_LoopTimeDefine.NWZYJoinMAX=NW_BY_JOIN_MAX;

	m_LoopTimeDefine.NWYLJoinMAX=(WORD)GetPrivateProfileInt("WarLoopDefine","YLJoinMax",0,MapServerConfigFileName);;		// 전쟁 참여 투표 인원 // LTS LOCALWAR
	if (!m_LoopTimeDefine.NWYLJoinMAX) m_LoopTimeDefine.NWYLJoinMAX=NW_YL_JOIN_MAX;

}

void cWarfield::InitWarLoop()	// 011023 LTS
{
	m_cLoopActive=1;
//	m_LoopTime=g_WarTime+NW_FIRST_TIME;	// 전쟁루프 초기 시작시간을 구한다.			// LTS NEW_NATION_WAR
	m_LoopTime=0;																	// LTS NEW_NATION_WAR	
//	m_dRemainTime=m_LoopTime-g_WarTime;												// LTS NEW_NATION_WAR
	m_dRemainTime=0;																// LTS NEW_NATION_WAR
	m_Status=NW_PEACE;																// LTS NEW_NATION_WAR
	m_bBonusActive=false;										//보너스 상태를 초기화
	m_dBonusLoopTime=0;											// 보너스 루프 초기화 
}

void cWarfield::ClearCommander()
{
	// 사령관 정보를 초기화 한다. 바이서스,자이펀,일스
	memset(&m_tSquad[NW_ATTACKER].Commander,0,sizeof(t_CommanderInformation));	// LOOP UNROLL
	memset(&m_tSquad[NW_DEFENCER].Commander,0,sizeof(t_CommanderInformation));	
	memset(&m_tSquad[NW_REINFORCE].Commander,0,sizeof(t_CommanderInformation));	
}

void cWarfield::ClearSquadCount()
{
	g_SquadCount[NW_ATTACKER][0]=0;		// LOOP UNROLL
	g_SquadCount[NW_ATTACKER][1]=0;
	g_SquadCount[NW_ATTACKER][2]=0;
	g_SquadCount[NW_ATTACKER][3]=0;
	g_SquadCount[NW_ATTACKER][4]=0;

	g_SquadCount[NW_DEFENCER][0]=0;
	g_SquadCount[NW_DEFENCER][1]=0;
	g_SquadCount[NW_DEFENCER][2]=0;
	g_SquadCount[NW_DEFENCER][3]=0;
	g_SquadCount[NW_DEFENCER][4]=0;

	g_SquadCount[NW_REINFORCE][0]=0;
	g_SquadCount[NW_REINFORCE][1]=0;
	g_SquadCount[NW_REINFORCE][2]=0;
	g_SquadCount[NW_REINFORCE][3]=0;
	g_SquadCount[NW_REINFORCE][4]=0;

	m_KilledCount[NW_ATTACKER]=0;
	m_KilledCount[NW_DEFENCER]=0;
	m_KilledCount[NW_REINFORCE]=0;

	m_JoinSquadCount[NW_ATTACKER]=0;		// 011016 LTS
	m_JoinSquadCount[NW_DEFENCER]=0;
	m_JoinSquadCount[NW_REINFORCE]=0;

	m_NationMemberCount[0]=0;
	m_NationMemberCount[1]=0;
	m_NationMemberCount[2]=0;
}

void cWarfield::ClearCommander(int Kind)						// 선택된 나라의 종류에 따라 사령관 정보를 초기화 한다.
{
	memset(&m_tSquad[Kind].Commander,0,sizeof(t_CommanderInformation));
}

bool cWarfield::LoadMonsterPoint()				// 수호석,성문에 의한 돈트의 처리 
{
	//추후 파일에서 읽어 온다. 
	switch (m_WarfieldCode)
	{
	case 0 :
		m_tMonsterPoint[0].MonsterIndex=17;
		m_tMonsterPoint[0].Size=MAP0_133;
		m_tMonsterPoint[0].Points=new POINTS[m_tMonsterPoint[0].Size];
		memcpy(m_tMonsterPoint[0].Points,Map0_133,sizeof(POINTS)*m_tMonsterPoint[0].Size);
		
		m_tMonsterPoint[1].MonsterIndex=18;
		m_tMonsterPoint[1].Size=MAP0_138;
		m_tMonsterPoint[1].Points=new POINTS[m_tMonsterPoint[1].Size];
		memcpy(m_tMonsterPoint[1].Points,Map0_138,sizeof(POINTS)*m_tMonsterPoint[1].Size);

		m_tMonsterPoint[2].MonsterIndex=19;
		m_tMonsterPoint[2].Size=MAP0_203;
		m_tMonsterPoint[2].Points=new POINTS[m_tMonsterPoint[2].Size];
		memcpy(m_tMonsterPoint[2].Points,Map0_203,sizeof(POINTS)*m_tMonsterPoint[2].Size);

		m_tMonsterPoint[3].MonsterIndex=20;
		m_tMonsterPoint[3].Size=MAP0_198;
		m_tMonsterPoint[3].Points=new POINTS[m_tMonsterPoint[3].Size];
		memcpy(m_tMonsterPoint[3].Points,Map0_198,sizeof(POINTS)*m_tMonsterPoint[3].Size);

		m_tMonsterPoint[4].MonsterIndex=41;
		m_tMonsterPoint[4].Size=MAP0_263;
		m_tMonsterPoint[4].Points=new POINTS[m_tMonsterPoint[4].Size];
		memcpy(m_tMonsterPoint[4].Points,Map0_263,sizeof(POINTS)*m_tMonsterPoint[4].Size);

		m_tMonsterPoint[5].MonsterIndex=42;
		m_tMonsterPoint[5].Size=MAP0_268;
		m_tMonsterPoint[5].Points=new POINTS[m_tMonsterPoint[5].Size];
		memcpy(m_tMonsterPoint[5].Points,Map0_268,sizeof(POINTS)*m_tMonsterPoint[5].Size);

		break;
	case 1 :
		m_tMonsterPoint[0].MonsterIndex=21;
		m_tMonsterPoint[0].Size=MAP1_450;
		m_tMonsterPoint[0].Points=new POINTS[m_tMonsterPoint[0].Size];
		memcpy(m_tMonsterPoint[0].Points,Map1_450,sizeof(POINTS)*m_tMonsterPoint[0].Size);

		m_tMonsterPoint[1].MonsterIndex=22;
		m_tMonsterPoint[1].Size=MAP1_527;
		m_tMonsterPoint[1].Points=new POINTS[m_tMonsterPoint[1].Size];
		memcpy(m_tMonsterPoint[1].Points,Map1_527,sizeof(POINTS)*m_tMonsterPoint[1].Size);

		m_tMonsterPoint[2].MonsterIndex=23;
		m_tMonsterPoint[2].Size=MAP1_524;
		m_tMonsterPoint[2].Points=new POINTS[m_tMonsterPoint[2].Size];
		memcpy(m_tMonsterPoint[2].Points,Map1_524,sizeof(POINTS)*m_tMonsterPoint[2].Size);
		break;
	case 2 : 
		m_tMonsterPoint[0].MonsterIndex=39;
		m_tMonsterPoint[0].Size=MAP2_274;
		m_tMonsterPoint[0].Points=new POINTS[m_tMonsterPoint[0].Size];
		memcpy(m_tMonsterPoint[0].Points,Map2_274,sizeof(POINTS)*m_tMonsterPoint[0].Size);

		m_tMonsterPoint[1].MonsterIndex=40;
		m_tMonsterPoint[1].Size=MAP2_279;
		m_tMonsterPoint[1].Points=new POINTS[m_tMonsterPoint[1].Size];
		memcpy(m_tMonsterPoint[1].Points,Map2_279,sizeof(POINTS)*m_tMonsterPoint[1].Size);

		m_tMonsterPoint[2].MonsterIndex=37;
		m_tMonsterPoint[2].Size=MAP2_151;
		m_tMonsterPoint[2].Points=new POINTS[m_tMonsterPoint[2].Size];
		memcpy(m_tMonsterPoint[2].Points,Map2_151,sizeof(POINTS)*m_tMonsterPoint[2].Size);

		m_tMonsterPoint[3].MonsterIndex=38;
		m_tMonsterPoint[3].Size=MAP2_156;
		m_tMonsterPoint[3].Points=new POINTS[m_tMonsterPoint[3].Size];
		memcpy(m_tMonsterPoint[3].Points,Map2_156,sizeof(POINTS)*m_tMonsterPoint[3].Size);

		m_tMonsterPoint[4].MonsterIndex=35;
		m_tMonsterPoint[4].Size=MAP2_114;
		m_tMonsterPoint[4].Points=new POINTS[m_tMonsterPoint[4].Size];
		memcpy(m_tMonsterPoint[4].Points,Map2_114,sizeof(POINTS)*m_tMonsterPoint[4].Size);

		m_tMonsterPoint[5].MonsterIndex=36;
		m_tMonsterPoint[5].Size=MAP2_119;
		m_tMonsterPoint[5].Points=new POINTS[m_tMonsterPoint[5].Size];
		memcpy(m_tMonsterPoint[5].Points,Map2_119,sizeof(POINTS)*m_tMonsterPoint[5].Size);
		break;
	}

	return true;
}

void cWarfield::CheckCastleGateWay()		// 전쟁서버전체의 NPC(WaeponTable) 루프를 돌며		// 011105 LTS
{
	for (int i=2;i<m_cNpcCount+2;i++)						// 이전에 살아있는놈이 죽었다면.. 처리를 해준다. WarUpdate에서 호출된다.
	{
		if (NPCList[i].SprNo>=65&&NPCList[i].SprNo<=67)		// 성문인가?
		{
			if (NPCList[i].bAlive>1)						// 죽어있다.
			{				
				if (m_tWeaponData[i-2].Status)				// 살아있다고 체크되어있다.
				{ //이전엔 살아있었다.				//성문만 체크한다..
					t_WarBBS WarBBS;
					m_tWeaponData[i-2].Status=0;						// 죽었다고 체크한다.
					InsertKiller(NPCList[i].KillerName,NPCList[i].SprNo);	//  죽인 유저를 기록한다.
					MyLog(0,"<_________________________________________________");
					MyLog(0,"GatewWay : SprNo : %d, Killed By User : %s",NPCList[i].SprNo,NPCList[i].KillerName);
					MyLog(0,"__________________________________________________>");

					SetTileDont(m_tWeaponData[i-2].MonPointIndex,0);	// 돈트를 푼다.
					SendTileDontChange(m_tWeaponData[i-2].MonPointIndex,0);		// 돈트가 변경되었다고 클라이언트에게 보낸다.

					WarBBS.WarfieldNo=m_WarfieldCode;
					WarBBS.BBSType=BBS_CASTLEGATEWAY_BROKED;
					WarBBS.LanNo=0;
					WarBBS.ArgType=BBS_NUM;
					//WarBBS.Size=1;		// 인자의 갯수 
					WarBBS.Size=10;		// 인자의 갯수			// 011022 LTS <
					WarBBS.Arg[0]=i-2;	// LTS TEMP
					int ServerID=NPCList[i].GetServerID();
					memcpy(&WarBBS.Arg[1],&ServerID,sizeof(int));
					WORD Locationx,Locationy,Index;				
					Locationx=m_tWeaponData[i-2].Locationx;	
					Locationy=m_tWeaponData[i-2].Locationy; 
					Index=m_tWeaponData[i-2].Phase;
					memcpy(&WarBBS.Arg[5],&Locationx,sizeof(WORD));
					memcpy(&WarBBS.Arg[7],&Locationy,sizeof(WORD));
					memcpy(&WarBBS.Arg[9],&Index,sizeof(WORD));	// 011022 LTS >

					SendWarBBS2Maps(&WarBBS);	// 성문이 깨어졌다고 모든 맵서버에게 알린다.
					SendWarBBS2Clients(&WarBBS);		// 성문이 깨어졌다고 방송을 보낸다. // 인자는 의미가 없다.				

					WarBBS.WarfieldNo=m_WarfieldCode;
					WarBBS.BBSType=BBS_KILLER;
					WarBBS.LanNo=1;				// 1 : 성문, 2 : 수호석, 3 : 사령관, 4 : 부대장
					WarBBS.ArgType=BBS_ALPHA;
					WarBBS.Size=strlen(NPCList[i].KillerName);
					memcpy(WarBBS.Arg,NPCList[i].KillerName,WarBBS.Size);
					SendWarBBS2Clients(&WarBBS);		// 성문을 깬 사람의 이름을 보낸다.

				}
			}
		}
		if (NPCList[i].SprNo==63||NPCList[i].SprNo==64)	// 수호석인가?
		{
			if (NPCList[i].bAlive>1)
			{
				if (m_tWeaponData[i-2].Status)				// 살아있다고 체크되어있다.
				{ //이전엔 살아있었다.				
					t_WarBBS WarBBS;
					m_tWeaponData[i-2].Status=0;						// 죽었다고 체크한다.
					InsertKiller(NPCList[i].KillerName,NPCList[i].SprNo);	// 죽인 유저를 기록한다. 
					MyLog(0,"<_________________________________________________");
					MyLog(0,"GuardStone : Sprite No : %d, Killed By User : %s",NPCList[i].SprNo,NPCList[i].KillerName);
					MyLog(0,"__________________________________________________>");

					CHARLIST *killerch = CheckServerName(NPCList[i].KillerName);
					
					if(killerch)
						HonorWon[killerch->GetServerID()] += 2;

					WarBBS.WarfieldNo=g_wMapServerPort-BASE_WARFIELD_PORT;
					WarBBS.BBSType=BBS_GUARDSTONE_BROKED;
					WarBBS.LanNo=0;
					WarBBS.ArgType=BBS_NUM;
					//WarBBS.Size=1;		// 인자의 갯수 
					WarBBS.Size=10;		// 인자의 갯수			// 011022 LTS <
					WarBBS.Arg[0]=i-2;	// LTS TEMP
					int ServerID=NPCList[i].GetServerID();
					memcpy(&WarBBS.Arg[1],&ServerID,sizeof(int));
					WORD Locationx,Locationy,Index;				
					Locationx=m_tWeaponData[i-2].Locationx;	
					Locationy=m_tWeaponData[i-2].Locationy; 
					Index=m_tWeaponData[i-2].Phase;
					memcpy(&WarBBS.Arg[5],&Locationx,sizeof(WORD));
					memcpy(&WarBBS.Arg[7],&Locationy,sizeof(WORD));
					memcpy(&WarBBS.Arg[9],&Index,sizeof(WORD));	// 011022 LTS >

					SendWarBBS2Maps(&WarBBS);				// 수호석이 깨어졌다고 모든 맵서버에게 알린다.
					SendWarBBS2Clients(&WarBBS);		// 수호석이 깨어졌다고 방송을 보낸다. // 인자는 의미가 없다.

					WarBBS.WarfieldNo=m_WarfieldCode;
					WarBBS.BBSType=BBS_KILLER;
					WarBBS.LanNo=2;				// 1 : 성문, 2 : 수호석, 3 : 사령관, 4 : 부대장
					WarBBS.ArgType=BBS_ALPHA;
					WarBBS.Size=strlen(NPCList[i].KillerName);
					memcpy(WarBBS.Arg,NPCList[i].KillerName,WarBBS.Size);
					SendWarBBS2Clients(&WarBBS);		// 수호석을 깬 사람의 이름을 보낸다.
				}
			}
		}
	}
}

void cWarfield::CheckGroup(SquadList* g_SquadList)
{
	for (SquadListItor itor=g_SquadList->begin();itor!=g_SquadList->end();itor++)
	{
		if (connections[(*itor)->GetData()].chrlst.bAlive==1)	(*itor)->SetStatus(1);
		if (connections[(*itor)->GetData()].chrlst.bAlive>1)	//죽어있다.
		{
			if ((*itor)->GetStatus()==1)					// 예전상태가 살아있는 상태면, 방금 죽었다는 소리.
			{
				t_WarBBS WarBBS;
				int ServerID=(*itor)->GetData();

				(*itor)->SetStatus(connections[ServerID].chrlst.bAlive);

				WarBBS.WarfieldNo=m_WarfieldCode;
				WarBBS.BBSType=BBS_COMMANDER_DEATH;
				if (isAttacker(m_WarfieldCode,connections,ServerID))
				{											//받는 곳에서는 단순하게 LanNo에 따라.. 표시만 다르게 하면된다.	
					if (connections[ServerID].chrlst.NWCharacter.isCommander) WarBBS.LanNo=0;
					else WarBBS.LanNo=1;					//공격국 사령관 , 부대장, 방어국 사령관,부대장, 지원군 사령관,부대장으로 나뉨
				}
				else
				{
					if (connections[ServerID].chrlst.NWCharacter.isCommander) WarBBS.LanNo=2;
					else WarBBS.LanNo=3;					//공격국 사령관 , 부대장, 방어국 사령관,부대장, 지원군 사령관,부대장으로 나뉨
				}
				WarBBS.ArgType=BBS_ALPHA;
				WarBBS.Size=strlen(connections[ServerID].name);
				memcpy(WarBBS.Arg,connections[ServerID].name,WarBBS.Size);

				SendWarBBS2Maps(&WarBBS);	// 받는 쪽에서 처리한다.
				SendWarBBS2Clients(&WarBBS);

				WarBBS.WarfieldNo=m_WarfieldCode;			// 사령관 부대장을 죽인놈이라고 메시지를 보낸다.	
				WarBBS.BBSType=BBS_KILLER;
				WarBBS.LanNo=3;				// 1 : 성문, 2 : 수호석, 3 : 사령관, 4 : 부대장
				WarBBS.ArgType=BBS_ALPHA;
				WarBBS.Size=strlen(connections[ServerID].chrlst.KillerName);
				memcpy(WarBBS.Arg,connections[ServerID].chrlst.KillerName,WarBBS.Size);
				SendWarBBS2Clients(&WarBBS);		// 수호석을 깬 사람의 이름을 보낸다.
			}
		}
	}
}

void cWarfield::CheckCommanderGroup()
{
	for(int i=0;i<NW_SQUAD_KIND;i++)	CheckGroup(&g_CommanderList[i]);	 // 사령관 스쿼드리더의 상태를 관찰한다.
}

void cWarfield::SendTileDontChange(int Mono,int type)	// 몬스터 번호에 의해 타일돈트가 풀렸다 쳤다를 클라이언트게 알려준다.
{
	t_packet packet;

	packet.h.header.type=CMD_TILE_DONT_CHANGED;
	packet.u.NationWar.TileDont.WarfieldNo=m_WarfieldCode;
	packet.u.NationWar.TileDont.TypeNumber=Mono;	//	
	packet.u.NationWar.TileDont.Value=type;
	packet.h.header.size=sizeof(t_TileDont);
	g_pUserManager->SendPacket(&packet); // CSD-CN-031213
}

void cWarfield::SetTileDont(int MonNo,int type) //몬스터 SQL 필드 INDEX, 0,1 : 
{
	for (int i=0;i<MAX_MONSTER_POINT;i++)				// Mono NPC번호에 의해.. 타일 돈트를 치고 푼다.
	{
		if (m_tMonsterPoint[i].MonsterIndex==MonNo)
		{
			for (int j=0;j<m_tMonsterPoint[i].Size;j++)
			{
				TileMap[m_tMonsterPoint[i].Points[j].x][m_tMonsterPoint[i].Points[j].y].attr_dont=type;
			}
		}
	}
}

void cWarfield::SendAllTileDont(int type)   // 모든 타일 돈트를 풀었다 쳤다 한다.
{
	int i,j;

	for (i=0;i<MAX_MONSTER_POINT;i++) 
	{
		for (j=0;j<m_cNpcCount;j++)
		{
			if (m_tMonsterPoint[i].MonsterIndex==m_tWeaponData[j].MonPointIndex)
			{
				SendTileDontChange(m_tMonsterPoint[i].MonsterIndex,m_tWeaponData[j].Status);
			}
		}
	}
}


void cWarfield::SendTileDont(t_packet *p, t_connection c[], int cn )	// 요청한 유저에게 모든 몬스터의 상태를 보내준다
{
	// 클라이언트는 몬스터의 상태에 따라 타일돈트를 풀고 닫는다.
	t_packet packet;
	int i,j;

	for (i=0;i<MAX_MONSTER_POINT;i++) 
	{
		for (j=0;j<m_cNpcCount;j++)
		{
			if (m_tMonsterPoint[i].MonsterIndex==m_tWeaponData[j].MonPointIndex) 
			{
				packet.h.header.type=CMD_ANSWER_TILE_DONT;
				packet.u.NationWar.TileDont.WarfieldNo=m_WarfieldCode;
				packet.u.NationWar.TileDont.TypeNumber=m_tMonsterPoint[i].MonsterIndex;
				packet.u.NationWar.TileDont.Value=m_tWeaponData[j].Status;
				packet.h.header.size=sizeof(t_TileDont);
				QueuePacket(c,cn,&packet,1);

				MyLog(LOG_NORMAL, "Tile Dont changed with value: %d", m_tWeaponData[j].Status);
			}
		}
	}
}

void cWarfield::SetCommander(int Kind,int CandidaterNo)				// 후보를 사령관으로 임명한다.		// 011016 LTS
{
	m_tSquad[Kind].Commander.CO_ID=m_tSquad[Kind].Candidater[CandidaterNo].CO_ID;
	strcpy_s(m_tSquad[Kind].Commander.CO_Name, sizeof(m_tSquad[Kind].Commander.CO_Name),m_tSquad[Kind].Candidater[CandidaterNo].CO_Name);
	m_tSquad[Kind].Commander.Fame=m_tSquad[Kind].Candidater[CandidaterNo].Fame;
	m_tSquad[Kind].Commander.btLevel=m_tSquad[Kind].Candidater[CandidaterNo].btLevel; // CSD-030806
	m_tSquad[Kind].Commander.Face=m_tSquad[Kind].Candidater[CandidaterNo].Face;
	m_tSquad[Kind].Commander.Class=m_tSquad[Kind].Candidater[CandidaterNo].Class;
	m_tSquad[Kind].Commander.Gender=m_tSquad[Kind].Candidater[CandidaterNo].Gender;
	m_tSquad[Kind].Commander.nDual=m_tSquad[Kind].Candidater[CandidaterNo].nDual;	// 011020 LTS
	m_tSquad[Kind].Commander.nStep=m_tSquad[Kind].Candidater[CandidaterNo].nStep;	// 011020 LTS
	m_tSquad[Kind].Commander.LadderScore=m_tSquad[Kind].Candidater[CandidaterNo].LadderScore;
}

void cWarfield::SetCommander(int Kind,t_CommanderInformation* Commander)	// 후보를 사령관으로 임명한다.
{
	memcpy(&m_tSquad[Kind].Commander,Commander,sizeof(t_CommanderInformation));
}

bool cWarfield::LoadWarfieldData(WORD PortNum)
{
	HSTMT	hStmt=NULL;
	RETCODE	ret;
	SWORD	nClos;
	char	query_stmt[80]={0,};
	SDWORD	cbValue;

	SQLAllocStmt(hDBC,&hStmt);

	wsprintf(query_stmt,"select * from WarFieldTBL where Port=%d order by WarfieldCode",PortNum);

	ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
 
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"WarField Table Query Error!!");
		return false;
	}

	SQLNumResultCols(hStmt,&nClos);

	ret=SQLFetch(hStmt);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"WarField Table Fetch Error!!");
	    SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	ret=SQLGetData(hStmt,2,SQL_C_SLONG,&m_WarfieldCode,sizeof(int),&cbValue);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
	{
		MyLog(0,"Warfield Table SQL Return Error(%d)!!",ret);
	    SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}                //WarFieldTBL의 Name
	ret=SQLGetData(hStmt,3,SQL_C_CHAR,&m_pWarfieldName,sizeof(m_pWarfieldName),&cbValue);
	EatRearWhiteChar(m_pWarfieldName);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
	{
		MyLog(0,"Warfield Table SQL Return Error(%d)!!",ret);
	    SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}                //WarFieldTBL의 NationCode 
	ret=SQLGetData(hStmt,4,SQL_C_SLONG,&m_NationCode,sizeof(int),&cbValue);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
	{
		MyLog(0,"Warfield Table SQL Return Error(%d)!!",ret);
	    SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}               //WarFieldTBL의 Port          
	ret=SQLGetData(hStmt,5,SQL_C_SLONG,&m_MapPortNum,sizeof(int),&cbValue);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
	{
		MyLog(0,"Warfield Table SQL Return Error(%d)!!",ret);
	    SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}               //WarFieldTBL의 Attacker Live Point X
	ret=SQLGetData(hStmt,7,SQL_C_SLONG,&m_LivePoints[NW_ATTACKER].x,sizeof(DWORD),&cbValue);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
	{
		MyLog(0,"Warfield Table SQL Return Error(%d)!!",ret);
	    SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}				//WarFieldTBL의 Attacker Live Point Y
	ret=SQLGetData(hStmt,8,SQL_C_SLONG,&m_LivePoints[NW_ATTACKER].y,sizeof(DWORD),&cbValue);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
	{
		MyLog(0,"Warfield Table SQL Return Error(%d)!!",ret);
	    SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}				//WarFieldTBL의 Defencer Live Point X
	ret=SQLGetData(hStmt,9,SQL_C_SLONG,&m_LivePoints[NW_DEFENCER].x,sizeof(DWORD),&cbValue);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
	{
		MyLog(0,"Warfield Table SQL Return Error(%d)!!",ret);
	    SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}				//WarFieldTBL의 Defencer Live Point Y
	ret=SQLGetData(hStmt,10,SQL_C_SLONG,&m_LivePoints[NW_DEFENCER].y,sizeof(DWORD),&cbValue);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
	{
		MyLog(0,"Warfield Table SQL Return Error(%d)!!",ret);
	    SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}				//WarFieldTBL의 Reinforce Live Point X
	ret=SQLGetData(hStmt,11,SQL_C_SLONG,&m_LivePoints[NW_REINFORCE].x,sizeof(DWORD),&cbValue);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
	{
		MyLog(0,"Warfield Table SQL Return Error(%d)!!",ret);
	    SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}				//WarFieldTBL의 Reinforce Live Point Y
	ret=SQLGetData(hStmt,12,SQL_C_SLONG,&m_LivePoints[NW_REINFORCE].y,sizeof(DWORD),&cbValue);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
	{
		MyLog(0,"Warfield Table SQL Return Error(%d)!!",ret);
	    SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}


    SQLFreeStmt(hStmt,SQL_DROP);
	return true;
}

bool cWarfield::LoadWarfieldWeaponData(WORD PortNum)			// 011022 LTS 함수 교체 
{
	HSTMT	hStmt=NULL;
	RETCODE	ret;
	SWORD	nClos;
	char	query_stmt[80]={0,};
	SDWORD	cbValue;

	SQLAllocStmt(hDBC,&hStmt);

	wsprintf(query_stmt,"select * from WeaponTBL where WarfieldCode=%d order by [index]",PortNum-BASE_WARFIELD_PORT);   

	ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
 
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"WarField Table Query Error!!");
		return false;
	}

	SQLNumResultCols(hStmt,&nClos);

	ret=SQLFetch(hStmt);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"WarField Table Fetch Error!!");
		return false;
	}

	int index=0;

	while (ret==SQL_SUCCESS)
	{
		ret=SQLGetData(hStmt,1,SQL_C_SLONG,&m_tWeaponData[index].MonPointIndex,sizeof(int),&cbValue);
		if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
		{
			MyLog(0,"Warfield Table SQL Return Error(%d)!!",ret);
		    SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}                
		ret=SQLGetData(hStmt,3,SQL_C_SLONG,&m_tWeaponData[index].NPC_Index,sizeof(int),&cbValue);
		if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
		{
			MyLog(0,"Warfield Table SQL Return Error(%d)!!",ret);
		    SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}                
		ret=SQLGetData(hStmt,4,SQL_C_SLONG,&m_tWeaponData[index].Phase,sizeof(int),&cbValue);	// 추가 몬스터 인덱스 
		if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
		{
			MyLog(0,"Warfield Table SQL Return Error(%d)!!",ret);
		    SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}                
		ret=SQLGetData(hStmt,5,SQL_C_SLONG,&m_tWeaponData[index].Locationx,sizeof(int),&cbValue);
		if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
		{
			MyLog(0,"Warfield Table SQL Return Error(%d)!!",ret);
		    SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}                
		ret=SQLGetData(hStmt,6,SQL_C_SLONG,&m_tWeaponData[index].Locationy,sizeof(int),&cbValue);
		if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
		{
			MyLog(0,"Warfield Table SQL Return Error(%d)!!",ret);
		    SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}                
		ret=SQLGetData(hStmt,7,SQL_C_SLONG,&m_tWeaponData[index].EventNo,sizeof(int),&cbValue);
		if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
		{
			MyLog(0,"Warfield Table SQL Return Error(%d)!!",ret);
		    SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}                
		ret=SQLGetData(hStmt,8,SQL_C_SLONG,&m_tWeaponData[index].GenerationPos,sizeof(int),&cbValue);
		if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
		{
			MyLog(0,"Warfield Table SQL Return Error(%d)!!",ret);
		    SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}                
		ret=SQLGetData(hStmt,9,SQL_C_SLONG,&m_tWeaponData[index].GenerationType,sizeof(int),&cbValue);
		if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
		{
			MyLog(0,"Warfield Table SQL Return Error(%d)!!",ret);
		    SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}                

		index++;
		ret=SQLFetch(hStmt);
	}
    SQLFreeStmt(hStmt,SQL_DROP);
	m_cNpcCount=index;
	return true;
}

bool cWarfield::LoadWarfieldMonsterData(WORD PortNum)
{
	HSTMT	hStmt=NULL;
	RETCODE	ret;
	SWORD	nClos;
	char	query_stmt[80]={0,};
	SDWORD	cbValue;
	t_WeaponStruct Monster;

	SQLAllocStmt(hDBC,&hStmt);

	wsprintf(query_stmt,"select * from WarfieldMonsterTBL where WarfieldCode=%d order by [index]",PortNum-BASE_WARFIELD_PORT);   

	ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
 
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"WarFieldMonster Table Table Query Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	SQLNumResultCols(hStmt,&nClos);

	ret=SQLFetch(hStmt);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"WarFieldMonster Table Fetch Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	while (ret==SQL_SUCCESS)
	{
		Monster.NPC_ID=0;
		ret=SQLGetData(hStmt,1,SQL_C_SLONG,&Monster.MonPointIndex,sizeof(int),&cbValue);
		if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
		{
			MyLog(0,"WarfieldMonster Table SQL Return Error(%d)!!",ret);
		    SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}                
		ret=SQLGetData(hStmt,3,SQL_C_SLONG,&Monster.NPC_Index,sizeof(int),&cbValue);
		if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
		{
			MyLog(0,"WarfieldMonster Table SQL Return Error(%d)!!",ret);
		    SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}                
		ret=SQLGetData(hStmt,4,SQL_C_SLONG,&Monster.Locationx,sizeof(int),&cbValue);
		if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
		{
			MyLog(0,"WarfieldMonster Table SQL Return Error(%d)!!",ret);
		    SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}                
		ret=SQLGetData(hStmt,5,SQL_C_SLONG,&Monster.Locationy,sizeof(int),&cbValue);
		if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
		{
			MyLog(0,"WarfieldMonster Table SQL Return Error(%d)!!",ret);
		    SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}                
		ret=SQLGetData(hStmt,6,SQL_C_SLONG,&Monster.EventNo,sizeof(int),&cbValue);
		if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
		{
			MyLog(0,"WarfieldMonster Table SQL Return Error(%d)!!",ret);
		    SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}                
		ret=SQLGetData(hStmt,7,SQL_C_SLONG,&Monster.GenerationPos,sizeof(int),&cbValue);
		if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
		{
			MyLog(0,"WarfieldMonster Table SQL Return Error(%d)!!",ret);
		    SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}                
		ret=SQLGetData(hStmt,8,SQL_C_SLONG,&Monster.GenerationType,sizeof(int),&cbValue);
		if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
		{
			MyLog(0,"WarfieldMonster Table SQL Return Error(%d)!!",ret);
		    SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}       
		
		InsertMonsterData(Monster);		// Monster List에 포함 시킨다.

		ret=SQLFetch(hStmt);
	}
    SQLFreeStmt(hStmt,SQL_DROP);
	return true;
}

void cWarfield::InitSquad()							// 부대정보를 초기화 한다
{
	int i,j;

	for (i=0;i<NW_SQUAD_KIND;i++)			// LTS SQUAD CHANGED
	{
		for (j=0;j<NW_SQUAD_MAX;j++)
		{
			m_tSquad[i].SquadInfo[j].SquadNo=j;
			m_tSquad[i].SquadInfo[j].CO_ID=0;
			m_tSquad[i].SquadInfo[j].CO_Name[0]=NULL;
			m_tSquad[i].SquadInfo[j].SoliderCount=0;
			m_tSquad[i].SquadInfo[j].Sok[0]=0;
			m_tSquad[i].SquadInfo[j].Sok[1]=0;
			m_tSquad[i].SquadInfo[j].Sok[2]=0;
			m_tSquad[i].SquadInfo[j].LoadingPoint=0;
		}
	}	
}

void cWarfield::InitCommanderCandidater()	// memset으로 바꾼다.	// 사령관 후보를 초기화 한다.
{
	int i,j;

	
	for (i=0;i<NW_SQUAD_KIND;i++)
	{
		for (j=0;j<NW_CANDIDATE_MAX;j++)
		{
			m_tSquad[i].Candidater[j].VoteNo=j;
			m_tSquad[i].Candidater[j].CO_ID=0;
			ZeroMemory(m_tSquad[i].Candidater[j].CO_Name, sizeof(m_tSquad[i].Candidater[j].CO_Name));
			m_tSquad[i].Candidater[j].btLevel=0;
			m_tSquad[i].Candidater[j].Fame=0;
			m_tSquad[i].Candidater[j].VoteCount=0;
		}
	}
}

void cWarfield::SetStatus(char Status)		// 전쟁서버의 상태를 설정한다.		
{
	m_Status=Status; 
	if (Status==1)							// 전쟁서버의 상태가 평화상태이면
	{
		InitSquad();						//	부대정보를 초기화 하고 사령관 후보를 초기화 한다.
		InitCommanderCandidater();
	}
}

t_SquadInformation* cWarfield::GetSquadInfo(int Kind,int SquadNo)	//	LTS SQUAD CHANGED	// 부대정보를 리턴한다.
{
	return &m_tSquad[Kind].SquadInfo[SquadNo];
}

void cWarfield::SetSquadLeader(int Kind,int SquadNo,t_CommanderInformation* SquadLeaderInfo)	// 부대장을 설정한다.
{
	m_tSquad[Kind].SquadInfo[SquadNo].CO_ID=SquadLeaderInfo->CO_ID;
    memcpy(m_tSquad[Kind].SquadInfo[SquadNo].CO_Name,SquadLeaderInfo->CO_Name,NW_NAME_MAX);
	m_tSquad[Kind].SquadInfo[SquadNo].SquadNo=SquadNo;
	m_tSquad[Kind].SquadInfo[SquadNo].LoadingPoint=0;  //로딩포인트의 초기화 
//	m_tSquad[Kind].SquadInfo[SquadNo].Sok[0]=0; //속성결정.. DICE조정..
//	m_tSquad[Kind].SquadInfo[SquadNo].Sok[1]=0; //속성결정.. DICE조정..
//	m_tSquad[Kind].SquadInfo[SquadNo].Sok[2]=0; //속성결정.. DICE조정..
}

void cWarfield::FirstMakeWeaponData()		// 전쟁서버의 NPC를 생성한다. 살아있다면... Give Life 된다.
{
	int Num;
	int CreateResult;
	int i;
	for (i=0;i<m_cNpcCount;i++)
	{
		Num=GetAliveNPCList();
		m_tWeaponData[i].NPC_ID=Num;
		CreateResult=NPC_Create(Num,m_tWeaponData[i].NPC_Index,m_tWeaponData[i].Locationx,m_tWeaponData[i].Locationy,m_tWeaponData[i].EventNo,m_tWeaponData[i].GenerationPos,m_tWeaponData[i].GenerationType);
		if (!CreateResult)
		{
			MyLog(0,"NPC CREATE FAILED!!, Set Dead!! NPC Index : %d",m_tWeaponData[i].NPC_Index);
			m_tWeaponData[i].Status=0; //죽어있다
			SetTileDont(m_tWeaponData[i].MonPointIndex,0);		
		}
		else
		{
			m_tWeaponData[i].Status=1; //살아있다.
			SetTileDont(m_tWeaponData[i].MonPointIndex,1);
		}
	}

	MyLog(0,"GUARD STONE AND CASTLE GATEWAY LIST___________________________");
	for (i=0;i<m_cNpcCount;i++)
	{
		MyLog(0,"ID : %03d, Index : %03d, LoacationX : %03d, LocationY : %03d",m_tWeaponData[i].NPC_ID,m_tWeaponData[i].NPC_Index,m_tWeaponData[i].Locationx,m_tWeaponData[i].Locationy); 
		MyLog(0,"ID : %03d, Index : %03d, LoacationX : %03d, LocationY : %03d",m_tWeaponData[i].NPC_ID,NPCList[m_tWeaponData[i].NPC_ID].npc_index,NPCList[m_tWeaponData[i].NPC_ID].X/32,NPCList[m_tWeaponData[i].NPC_ID].Y/32); 
	}
	MyLog(0,"GUARD STONE AND CASTLE GATEWAY LIST COMPLETE___________________");
}

void cWarfield::MakeWeaponData()		// 전쟁서버의 모든 NPC를 다시 생성한다. 살아있으면 GiveLife된다.
{
	int CreateResult;
	int i;
	for (i=0;i<m_cNpcCount;i++)
	{
		m_tWeaponData[i].Status=1;
		CreateResult=NPC_Create(m_tWeaponData[i].NPC_ID,m_tWeaponData[i].NPC_Index,m_tWeaponData[i].Locationx,m_tWeaponData[i].Locationy,m_tWeaponData[i].EventNo,m_tWeaponData[i].GenerationPos,m_tWeaponData[i].GenerationType);
		if (!CreateResult)
		{
			MyLog(0,"NPC CREATE FAILED!! NPC INDEX : %d",m_tWeaponData[i].NPC_Index);
		}
		SendCharacterCondition(&NPCList[m_tWeaponData[i].NPC_ID],0);
	}
	SendAllTileDont(1);					// 전쟁서버에 있는 모든 유저에게 타일 돈트가 변경되었음을 알린다.

	MyLog(0,"GUARD STONE AND CASTLE GATEWAY LIST___________________________");
	for (i=0;i<m_cNpcCount;i++)
	{
		MyLog(0,"ID : %03d, Index : %03d, LoacationX : %03d, LocationY : %03d",m_tWeaponData[i].NPC_ID,m_tWeaponData[i].NPC_Index,m_tWeaponData[i].Locationx,m_tWeaponData[i].Locationy); 
		MyLog(0,"ID : %03d, Index : %03d, LoacationX : %03d, LocationY : %03d",m_tWeaponData[i].NPC_ID,NPCList[m_tWeaponData[i].NPC_ID].npc_index,NPCList[m_tWeaponData[i].NPC_ID].X/32,NPCList[m_tWeaponData[i].NPC_ID].Y/32); 
	}
	MyLog(0,"GUARD STONE AND CASTLE GATEWAY LIST COMPLETE___________________");

}

void cWarfield::WeaponDataList()		// 전쟁서버의 모든 NPC를 다시 생성한다. 살아있으면 GiveLife된다.
{
	MyLog(0,"GUARD STONE AND CASTLE GATEWAY DEBUG LIST__________________________");
	for (int i=0;i<m_cNpcCount;i++)
	{
		MyLog(0,"WEAPON DATA ID : %03d, Index : %03d, LoacationX : %03d, LocationY : %03d",
      m_tWeaponData[i].NPC_ID,
      m_tWeaponData[i].NPC_Index,
      m_tWeaponData[i].Locationx,
      m_tWeaponData[i].Locationy); 

		MyLog(0,"NPC DATA ID : %03d, Index : %03d, LoacationX : %03d, LocationY : %03d HP : %d Status : %d",
      m_tWeaponData[i].NPC_ID,
      NPCList[m_tWeaponData[i].NPC_ID].npc_index,
      NPCList[m_tWeaponData[i].NPC_ID].X/32,
      NPCList[m_tWeaponData[i].NPC_ID].Y/32,
      NPCList[m_tWeaponData[i].NPC_ID].Hp,
      NPCList[m_tWeaponData[i].NPC_ID].Status
      ); 
	}
	MyLog(0,"GUARD STONE AND CASTLE GATEWAY DEBUG LIST COMPLETE___________________");
}

void cWarfield::SendCheckWarfieldStatus()	// 전쟁서버 -> 관리서버 다른서버의 상태 즉, 나의 상태를 바꾸려함을 체크한다.
{
	t_packet packet;

	packet.h.header.type=CMD_CHECK_WARFIELD_STATUS;
	packet.u.NationWar.WarfieldStatusChange.WarfieldNo=g_wMapServerPort;
	packet.u.NationWar.WarfieldStatusChange.Status=m_Status;
	packet.h.header.size=sizeof(t_WarfieldStatusChange);

	g_pServerTable->SendRajaPacketToOtherMapServer( NATION_MANAGE_SERVER, (char *)&packet, packet.h.header.size+sizeof(t_header) );

}

void cWarfield::SendWarfieldStatus()		// 전쟁서버 -> 관리서버 내 상태를 보낸다.
{
	t_packet packet;

	packet.h.header.type=CMD_WARFIELD_STATUS;
	packet.u.NationWar.WarfieldStatusChange.WarfieldNo=g_wMapServerPort;
	packet.u.NationWar.WarfieldStatusChange.Status=m_Status;
	packet.h.header.size=sizeof(t_WarfieldStatusChange);
	
	g_pServerTable->SendRajaPacketToOtherMapServer( NATION_MANAGE_SERVER, (char *)&packet, packet.h.header.size+sizeof(t_header) );
}

void cWarfield::ChangeWarfieldStatus(int Result)		//결과에 의해 상태를 바꾼다.
{
	char tempNationCode = 0;

	if (m_NationCode==NW_BY) tempNationCode=NW_ZY;		// 방어자가 바이서스이면 공격자는 자이펀이다.
	else tempNationCode=NW_BY;							// 위 상황과 반대이다,

	switch (m_Status)
	{
	case NW_WAIT_VOTE :									// 투표를 기다리는 상태인데..
		if (Result)										// 결과가 투표로 들어가도 좋다면...	
		{
			t_WarBBS WarBBS;

			m_Status=NW_VOTE;							// 상태를 투표상태로 바꾼다.
			m_LoopTime=g_WarTime+m_LoopTimeDefine.NWVoteTime;			// 투표시간을 세트한다.	// 011101 LTS
			m_dRemainTime=m_LoopTime-g_WarTime;

			WarBBS.WarfieldNo=m_WarfieldCode;			//"%d국의 %d전쟁터로 처들어갈 의견을 모으고 있습니다."
			WarBBS.BBSType=BBS_WAR_JOIN_VOTE;
			WarBBS.LanNo=0;
			WarBBS.ArgType=BBS_NUM;
			WarBBS.Size=1;
			WarBBS.Arg[0]=m_NationCode;

			SendWarBBS2NationMaps(tempNationCode,&WarBBS);			// 공격자 맵에만 보낸다. 이때... 전쟁 상황버튼을 클라이언트에서 감빡이게 하면 된다.
			SendWarBBS2NationClients(tempNationCode,&WarBBS);		// 전쟁맵에 있는 공격국 유저에게도 보낸다.
		}
		else											// 결과가 투표로 들어가면 안된다이면 지금 다른 전쟁루프가 돌고 있다.
		{
			m_Status=NW_PEACE;							// 상태를 평화상태로 바꾼다.
			m_LoopTime=g_WarTime+m_LoopTimeDefine.NWPeaceTime1;		// 평화시간으로 세트한다.	// 011101 LTS
			m_dRemainTime=m_LoopTime-g_WarTime;
		}
		break;
	case NW_WAIT_PREPARE :
		m_Status=NW_PREPARE;							// 전쟁 준비상태로 세트한다.
		m_LoopTime=g_WarTime+m_LoopTimeDefine.NWPrepareTime;			// 준비 시간을 세트한다.	// 011101 LTS
		m_dRemainTime=m_LoopTime-g_WarTime;

		break;
	case NW_WAIT_WAR :
		{
			t_WarBBS WarBBS;
			
			m_Status=NW_WAR;							// 전쟁상태로 세트한다.
			m_LoopTime=g_WarTime+m_LoopTimeDefine.NWWarTime;			// 전쟁시간을 세트한다.	// 011101 LTS
			m_dRemainTime=m_LoopTime-g_WarTime;

			ClearSquadCount();							//스쿼드카운트를 초기화한다. // 011016 LTS

			PrepareSquadForWar();						// 전쟁터에 이미 있는 유저를 부대에 편성시킨다.

			WarBBS.WarfieldNo=m_WarfieldCode;			// "%d국의 %d전쟁터에 전쟁이 시작되었습니다."
			WarBBS.BBSType=BBS_WAR_BEGIN;
			WarBBS.LanNo=0;
			WarBBS.ArgType=BBS_NUM;
			WarBBS.Size=16;
			WarBBS.Arg[0]=m_NationCode;
			WarBBS.Arg[1]=GetSquadLoadingPoint(NW_ATTACKER,0);		//캐릭의 로딩포인트 정보 
			WarBBS.Arg[2]=GetSquadLoadingPoint(NW_ATTACKER,1);		// 맵서버 부하를 피하기 위해 클라이언트에게 보낸다
			WarBBS.Arg[3]=GetSquadLoadingPoint(NW_ATTACKER,2);
			WarBBS.Arg[4]=GetSquadLoadingPoint(NW_ATTACKER,3);
			WarBBS.Arg[5]=GetSquadLoadingPoint(NW_ATTACKER,4);
			WarBBS.Arg[6]=GetSquadLoadingPoint(NW_DEFENCER,0);
			WarBBS.Arg[7]=GetSquadLoadingPoint(NW_DEFENCER,1);
			WarBBS.Arg[8]=GetSquadLoadingPoint(NW_DEFENCER,2);
			WarBBS.Arg[9]=GetSquadLoadingPoint(NW_DEFENCER,3);
			WarBBS.Arg[10]=GetSquadLoadingPoint(NW_DEFENCER,4);
			WarBBS.Arg[11]=GetSquadLoadingPoint(NW_REINFORCE,0);
			WarBBS.Arg[12]=GetSquadLoadingPoint(NW_REINFORCE,1);
			WarBBS.Arg[13]=GetSquadLoadingPoint(NW_REINFORCE,2);
			WarBBS.Arg[14]=GetSquadLoadingPoint(NW_REINFORCE,3);
			WarBBS.Arg[15]=GetSquadLoadingPoint(NW_REINFORCE,4);
			SendWarBBS2Maps(&WarBBS);					// 방어국 ,공격국,지원군국에게 모두 전쟁시작 메시지를 보낸다.
			SendWarBBS2Clients(&WarBBS);				// 맵에 있는 유저에게 전쟁이 시작됨을 알려준다.. // 옷을 갈아입는 부분이 있다.
			SendWarBeginEndMessage2Client(1);			// LTS TEMP
		}
		break;
	case NW_WAIT_PEACE :
		m_Status=NW_PEACE;								// 평화상태로 세트한다.
		m_LoopTime=g_WarTime+m_LoopTimeDefine.NWPeaceTime1;			//평화시간을 세트한다.		// 011101 LTS
		m_dRemainTime=m_LoopTime-g_WarTime;
		InitSquad();									// 부대정보를 클리어
		InitCommanderCandidater();						// 사령관 후보 데이터를 클리어
		break;
	}
	SendWarfieldStatus();
}

void cWarfield::SetStatusVote()								// 관리서버에서 언제시작할지를 체크하다. 보내준다.
{
	t_WarBBS WarBBS;
	char tempNationCode = 0;

	if (m_NationCode==NW_BY) tempNationCode=NW_ZY;			// 방어자가 바이서스이면 공격자는 자이펀이다.
	else tempNationCode=NW_BY;								// 위 상황과 반대이다,

	UpdateWarTime(); 
	
	m_Status=NW_VOTE;										// 상태를 투표상태로 바꾼다.
	m_LoopTime=g_WarTime+m_LoopTimeDefine.NWVoteTime;		// 투표시간을 세트한다.	// 011101 LTS
	m_dRemainTime=m_LoopTime-g_WarTime;
	
	WarBBS.WarfieldNo=m_WarfieldCode;						//"%d국의 %d전쟁터로 처들어갈 의견을 모으고 있습니다."
	WarBBS.BBSType=BBS_WAR_JOIN_VOTE;
	WarBBS.LanNo=0;
	WarBBS.ArgType=BBS_NUM;
	WarBBS.Size=1;
	WarBBS.Arg[0]=m_NationCode;

	SendWarBBS2NationMaps(tempNationCode,&WarBBS);			// 공격자 맵에만 보낸다. 이때... 전쟁 상황버튼을 클라이언트에서 감빡이게 하면 된다.
	SendWarBBS2NationClients(tempNationCode,&WarBBS);		// 전쟁맵에 있는 공격국 유저에게도 보낸다.	
}

void cWarfield::SendCMD_REQUEST_VOTE_RESULT()			// 전쟁서버 -> 관리서버 ,전쟁 투표 결과를 요청한다.
{
	t_packet packet;
	
	packet.h.header.type=CMD_REQUEST_VOTE_RESULT;
	packet.u.NationWar.CommonDataC.Data=g_wMapServerPort-BASE_WARFIELD_PORT; 
	packet.h.header.size=sizeof(t_CommonDataC);

	g_pServerTable->SendRajaPacketToOtherMapServer( NATION_MANAGE_SERVER, (char *)&packet, packet.h.header.size+sizeof(t_header) );
}

void cWarfield::RecvCMD_ANSWER_VOTE_RESULT(t_packet *p, t_connection c[], int cn )		// 관리서버로 부터 전쟁 투표 결과를 받았다.
{
	char tempNationCode = 0;								

	if (m_NationCode==NW_BY) 
	{
		tempNationCode=NW_ZY;		//방어국적 공격국작의 판별
	}
	else
	{
		tempNationCode=NW_BY;
	}

	if (p->u.NationWar.VoteData.VoteCount>=m_LoopTimeDefine.NWVoteMin)// LTS LOCALWAR			//투표를 했으므로 해당 조건이 되었다. 전쟁을 선포한다 //추후 조건조정	// 011016 LTS
	{
		t_WarBBS WarBBS;								//전쟁투표 조건

		m_Status=NW_WAIT_PREPARE;						// 전쟁 준비 대기 상태로 세트 
		SendCheckWarfieldStatus();						// 관리서버에 전쟁 준비로 들어간다고 확인을 받는다.

		WarBBS.WarfieldNo=m_WarfieldCode;				// 전쟁터 번호 
		WarBBS.BBSType=BBS_WAR_PREPARE;
		WarBBS.LanNo=0;
		WarBBS.ArgType=BBS_NUM;
		WarBBS.Size=2;
		WarBBS.Arg[0]=tempNationCode;					// 공격국 
		WarBBS.Arg[1]=m_NationCode;						// 방어국 	

		SendWarBBS2Maps(&WarBBS);						// 클라이언트에서 받은 다음 번역해서 쓴다.
		SendWarBBS2Clients(&WarBBS);
	}
	else												// 전쟁을 선포하지 못했다. //평화상태 
	{
		t_WarBBS WarBBS;

		m_Status=NW_PEACE;								// 투표결과가 기준치 이하여서 평화상태로 세트
		m_LoopTime=g_WarTime+m_LoopTimeDefine.NWPeaceTime1;			// 평화시간 세트	// 011101 LTS
		m_dRemainTime=m_LoopTime-g_WarTime;
		SendCheckWarfieldStatus();					
		ClearAllSquad(tempNationCode);					// 투표한걸 지우라고 한다. //투표한 나라에게만 보낸다. 

		WarBBS.WarfieldNo=m_WarfieldCode;
		WarBBS.BBSType=BBS_PEACE;
		WarBBS.LanNo=0;
		WarBBS.ArgType=BBS_NUM;
		WarBBS.Size=0;									// LTS TEMP		// Arg가 필요 없다.

		SendWarBBS2NationMaps(tempNationCode,&WarBBS);			// 투표국에 메시지 전송 
		SendWarBBS2NationClients(tempNationCode,&WarBBS);		// 현재맵의 공격국 유저에게 보낸다.

		ActiveBonusTime();										// 보너스 타임을 시작한다.

		WarBBS.BBSType=BBS_BONUS_OPEN;
		WarBBS.LanNo=0;
		WarBBS.ArgType=BBS_NUM;
		WarBBS.Size=0;
		SendWarBBS2NationMaps(m_NationCode,&WarBBS);			// 보너스 타임이 시작되었다고 알림
		SendWarBBS2NationClients(m_NationCode,&WarBBS);			// 보너스 타임이 시작되었다고 알림
	}
}

void cWarfield::AllGuardStoneWasBroked() //가드스톤이 모드 깨졌다면... 전쟁을 종료시킨다.
{
	bool CheckGuardStone=true;

	for (int i=2;i<m_cNpcCount+2;i++)		// NPC루프를 돌고.. 가드스톤이 다깨졌으면..
	{
		if (NPCList[i].SprNo==63||NPCList[i].SprNo==64)
		{
			if (NPCList[i].bAlive==1) CheckGuardStone=false;
		}
	}

	if (CheckGuardStone)
	{
		m_LoopTime=g_WarTime;   // 시간을 초기화..
		m_dRemainTime=m_LoopTime-g_WarTime;
		m_bAttackerWin=true;		// 공격자 승리를 세트한다.
	}
}

void cWarfield::WarUpdate()
{
	CheckCommanderGroup();			// 사령관, 부대장들의 상태를 조사한다.
	CheckCastleGateWay();
	AllGuardStoneWasBroked();		// 모든 수호석이 깨어 졌는지 검사한다.
}

void cWarfield::KickDefeatUser2Home()		// 패배한 유저들을 고향으로 쫗는다.
{
	for( int i = DRAGON_CONNECTIONS_START; i<DRAGON_MAX_CONNECTIONS; i++ )
	{
		if (CheckServerId(i))
		{
			if (connections[i].chrlst.name_status.nation!=m_NationCode)
			{
				switch (connections[i].chrlst.name_status.nation)
				{
					case 3 :	MapMove(i,"MA-IN",250,112); break;
					case 4 :	MapMove(i,"RENES_C",100,100); break;
					case 6 :	
						if (connections[i].chrlst.NWCharacter.YL_JoinNation!=m_NationCode) MapMove(i,"BARANTAN",100,100);
						break;
				}
			}
		}
	}
}

void cWarfield::MoneySpread(double TotalMoney)		// 돈을 뿌린다.		// 사전에 국고에서 확인해야 한다.
{
	int Money=TotalMoney/50;

	const POINTS SpreadPosition[NW_WARFIELD_COUNT][2]= { {{155,163}, {176,171}},{{479,485},{500,506}},{{197,192},{221,227}} };

	int GapX=SpreadPosition[m_WarfieldCode][1].x-SpreadPosition[m_WarfieldCode][0].x;
	int GapY=SpreadPosition[m_WarfieldCode][1].y-SpreadPosition[m_WarfieldCode][0].y;

	int PositionX;
	int PositionY;

	MyLog(0,"Inside Money Spread Routine....");

	ItemAttr item;
	if(LocalMgr.IsChangeMoney())//030102 lsw
	{
		item=GenerateItem(IN_NEW_COINS,IATTR_MUCH,Money);//020509 lsw 
	}
	else
	{
		item=GenerateItem(IN_COINS,IATTR_MUCH,Money);//020509 lsw 
	}

	for (int i=0;i<50;i++)
	{
		if (item.item_no)
		{
			PositionX=(rand()%GapX)*32+(SpreadPosition[m_WarfieldCode][0].x)*32;
			PositionY=(rand()%GapX)*32+(SpreadPosition[m_WarfieldCode][0].y)*32;
			AddItemList( item.item_no, item.attr, 0, PositionX, PositionY, 0, 0 );
		}
	}
}

void cWarfield::UpdateProgressofWar()
{
	m_dRemainTime=m_LoopTime-g_WarTime;				// 남은 시간 조정
	
	switch (m_Status)								// today Notice정보용...
	{
	case NW_VOTE :
		{
			if (m_dRemainTime%NW_REMAIN_TIME_UPDATE==0) SendCMD_UPDATE_WARTIME(m_WarfieldCode,m_Status,m_dRemainTime);
		}
		break;
		
	case NW_PREPARE :								// 전쟁 준비 상태가 진행중이다.
		{
			if (m_dRemainTime%NW_REMAIN_TIME_UPDATE==0) SendCMD_UPDATE_WARTIME(m_WarfieldCode,m_Status,m_dRemainTime);
			if (!m_bRequestCommanderVotingResult)		//사령관 투표 보내줘 패킷을 보내지 않았다면..
			{											//전쟁 종료시 초기화된다.
				__int64 CommanderElectTime=m_LoopTimeDefine.NWPrepareTime;		// 011101 LTS
				CommanderElectTime>>=1; // 50%			//사령관 투표를 마무리 한다. 
				if ((m_LoopTime-g_WarTime)<CommanderElectTime) 
				{
					SendCMD_REQUEST_COMMANDER_VOTING_RESULT();		// 전쟁준비 기간의 50%가 지났다면..
					m_bRequestCommanderVotingResult=true;			// 사령관 투표의 결과를 요청한다.
				}
			}
		}
		break;
		
	case NW_WAR :
		{
			if (m_dRemainTime%NW_REMAIN_TIME_UPDATE==0) SendCMD_UPDATE_WARTIME(m_WarfieldCode,m_Status,m_dRemainTime);
			WarUpdate();			// 전쟁 중이면.. 전쟁의 상태를 업데이트 한다.				
		}
		break;
	}
	
}

void cWarfield::UpdateStatusofWar()
{
	SERVER_DATA	*pData=g_pServerTable->GetConnectedServerData((WORD)NATION_MANAGE_SERVER);
	if (pData==NULL)
		return;

	switch (m_Status)		// 상태가 진행중이 아니면.. 대기상태이다.
	{
	case NW_VOTE :
			SendCMD_REQUEST_VOTE_RESULT();
			MakeWeaponData();						// 전쟁무기의 상태를 바꾼다.      // 결계석 버그 때문에...
		break;
	case NW_PREPARE :
			m_Status=NW_WAIT_WAR;
			SendCheckWarfieldStatus();
		break;
	case NW_WAR :
			if (m_bAttackerWin)	SendCMD_REQUEST_SUBSTRACT_MONEY(m_NationCode,0,g_wMapServerPort,50000000);	// 국가세팅이 되기 전이므로 
		    else
			{
				if (m_NationCode==NW_ZY) SendCMD_REQUEST_SUBSTRACT_MONEY(NW_BY,0,g_wMapServerPort,50000000);	//공격국의 패배이면 
				else SendCMD_REQUEST_SUBSTRACT_MONEY(NW_ZY,0,g_wMapServerPort,50000000);	
			}
			//MoneySpread(m_bAttackerWin);			// 돈을 뿌려준다.
			ProcessNationWarEnd(m_bAttackerWin);	// 전쟁종료처리를 한다.
			MakeWeaponData();						// 전쟁무기의 상태를 바꾼다.
			DeleteAllSquadMember();					// 부대 해쉬를 지운다.
			ClearCommander();						// 사령관 정보를 지운다.
//			KickDefeatUser2Home();					// 패배한 유저를 맵에서 내보낸다.		// 011015 LTS
			m_cGuardStoneBrokenCount=0;				// 관련 클래스 변수를 초기화 한다.
			m_cCastleDoorBrokenCount=0;
			m_bRequestCommanderVotingResult=false;
			m_bAttackerWin=false;
			ClearSquadCount();						// 부대 인원수 카운트를 초기화 한다.
			DeleteAllKiller();						// 성문, 수호석을 부순 유저리스트 삭제
			m_Status=NW_WAIT_PEACE;					// 평화 상태로 바꾼다.
			SendCheckWarfieldStatus();				// 관리서버에 상태변경 메시지를 보낸다.
			ActiveBonusTime();						//보너스 타임을 오픈한다.
		break;
	}

}

void cWarfield::UpdateWarfieldStatus()					// LTS NEW_NATION_WAR
{	//< CSD-030804
	UpdateWarTime(); // 전쟁 시간을 갱신

	if (m_Status == NW_PEACE)
	{	// 평화상태이면 스킵
		return; 
	}

	if (m_LoopTime > g_WarTime)
	{	// 상태가 진행중이면
		UpdateProgressofWar();
	}
	else
	{	// 상태가 바뀌었다면..
		UpdateStatusofWar();
	}
}	//> CSD-030804

void cWarfield::SendStartUpMapPossession()		// 스타트업시 
{
	t_packet packet;
	if (g_pWarfield->GetNationCode()==NW_BY)	// 데이터 베이스에서 읽어왔다.
	{
		g_pWarfieldStatus[m_WarfieldCode].Possession=NW_BY;
	}
	else									
	{
		g_pWarfieldStatus[m_WarfieldCode].Possession=NW_ZY;
	}

	MapInfo[MapNumber].nation=g_pWarfield->GetNationCode(); //자기 자신을 위한 세팅 

	packet.h.header.type=CMD_STARTUP_NATION_CODE_CHANGE;
	packet.u.NationWar.WarfieldStatusChange.WarfieldNo=g_wMapServerPort-BASE_WARFIELD_PORT;
	packet.u.NationWar.WarfieldStatusChange.Status=g_pWarfield->GetNationCode();
	packet.h.header.size=sizeof(t_WarfieldStatusChange);

	g_pServerTable->SendRajaPacketToOtherMapServer( NATION_MANAGE_SERVER, (char *)&packet, packet.h.header.size+sizeof(t_header) );
}

void cWarfield::SendMapPossessChanged()		//전쟁에 의해 맵의 소유가 바뀌었다고 전달.. 
{
	t_packet packet;

	if (g_pWarfield->GetNationCode()==NW_BY) //이전의 소유자가 바이서스면.. 자이펀으로 소유를 바꾼다.
	{
		g_pWarfield->SetNationCode(NW_ZY);   //국적을 세팅하고 관리서버로 보낸다.
		g_pWarfieldStatus[m_WarfieldCode].Possession=NW_ZY;
	}
	else									//이전의 소유자가 자이펀이면 바이서스로 바꾼다.
	{
		g_pWarfield->SetNationCode(NW_BY);
		g_pWarfieldStatus[m_WarfieldCode].Possession=NW_BY;
	}

	MapInfo[MapNumber].nation=g_pWarfield->GetNationCode(); //자기 자신을 위한 세팅 

	packet.h.header.type=CMD_MAP_NATION_CODE_CHANGED;
	packet.u.NationWar.WarfieldStatusChange.WarfieldNo=g_wMapServerPort-BASE_WARFIELD_PORT;
	packet.u.NationWar.WarfieldStatusChange.Status=g_pWarfield->GetNationCode();
	packet.h.header.size=sizeof(t_WarfieldStatusChange);

	g_pServerTable->SendRajaPacketToOtherMapServer( NATION_MANAGE_SERVER, (char *)&packet, packet.h.header.size+sizeof(t_header) );
}

void cWarfield::ClearAllSquad(int Nation)
{
	SendCMD_CLEAR_SQUAD_DATA(Nation); // All Maps

	for (int i=DRAGON_CONNECTIONS_START; i<DRAGON_MAX_CONNECTIONS; i++ )
	{
		if (CheckServerId(i))
		{								// 유저의 NWCharacter정보를 초기화 한다.	
			InitConnectionsSquadData(i); //먼저 해줘야 한다.. 다음에 또하더라도.. // 유저가 맵이동을 하기전에..
			SendCMD_CLEAR_SQUAD_DATA(connections,i); // All Clients
		}
	}
}

void cWarfield::GiveSquadPoint2Other(int cn,int Point)	// 유저의 부대원에게 점수를 준다.			// 011101 LTS // 함수교체 
{
	SquadList* g_SquadList;
	if (connections[cn].chrlst.NWCharacter.SquadNo==0) return;	// 부대번호가 0이면 나누어 줄수 없다.

	if (isAttacker(m_WarfieldCode,connections,cn)) g_SquadList=&g_AttackSquadList[connections[cn].chrlst.NWCharacter.SquadNo];
	else g_SquadList=&g_DefenceSquadList[connections[cn].chrlst.NWCharacter.SquadNo];
		
	for (SquadListItor itor=g_SquadList->begin();itor!=g_SquadList->end();itor++)
	{
		if (cn!=(*itor)->GetData())	
		{
//			MyLog(0,"부대 점수 부여 이름 : %s, 부대코드 : %d,  OldFame : %d, NewFame : %d",connections[(*itor)->GetData()].name,connections[(*itor)->GetData()].chrlst.NWCharacter.SquadNo,connections[(*itor)->GetData()].chrlst.fame,connections[(*itor)->GetData()].chrlst.fame+Point);
			connections[(*itor)->GetData()].chrlst.GainedFame+=Point;
		}
	}
}

void cWarfield::GiveGuildPoint2Other(int cn,int Point)	// 유저의 길드원에게 점수를 준다.		// 011110 LTS // 함수교체 
{	//< CSD-030324
	int GuildCode=connections[cn].chrlst.GetGuildCode();

	if (GuildCode==0) return;	// 길드코드가 0이면 나누어 줄수 없다

	for (int i=DRAGON_CONNECTIONS_START; i<DRAGON_MAX_CONNECTIONS; i++ )
	{
		if (CheckServerId(i))
		{					
			if (connections[i].chrlst.GetGuildCode()==GuildCode) 
			{
//				MyLog(0,"길드 점수 부여 이름 : %s, 길드코드 : %d,  OldFame : %d, NewFame : %d",connections[i].name,GuildCode,connections[i].chrlst.fame,connections[i].chrlst.fame+Point);
				connections[i].chrlst.GainedFame+=Point;
			}
		}
	}
}	//> CSD-030324

void cWarfield::ReturnFame()		// 명성치를 계산해 준다.		// 011105 LTS // 함수교체 
{	//< CSD-030324
//	MyLog(0,"명성치 계산_____________________________________________________");
	for (KillerListItor itor=g_KillerList.begin();itor!=g_KillerList.end();itor++)
	{
		int ServerID=ExistHe((char*)(*itor)->GetUserName());
		if (ServerID>0)
		{
			switch((*itor)->GetKillSprNo())
			{
			case 65	:						// 성문, 일반 수호석이면..
			case 66 :
			case 67 : 
			case 63 : 
				switch(connections[ServerID].chrlst.NWCharacter.SquadNo)	//부대 번호에 따라 틀리다.
				{	
				case 0 :												// 부대편성이 되지 않은 유저이면
					if (connections[ServerID].chrlst.GetGuildCode())	// 길드 코드를 가지고 있다면..
					{
//						MyLog(0," 성문,수호석 죽인 유저(길드번호 : %d) : %s OldFame : %d, NewFame : %d 시작 _________________________",connections[ServerID].chrlst.GetGuildCode(),connections[ServerID].name,connections[ServerID].chrlst.fame,connections[ServerID].chrlst.fame+6);
//						MyLog(0," 길드에게 나눠줌 시작 _________________________");
						connections[ServerID].chrlst.GainedFame+=6;					// 자신은 6 포인트를 가지고 
						GiveGuildPoint2Other(ServerID,4);						// 다른 길드에게는 4를 준다.
//						MyLog(0," 길드에게 나눠줌 끝 ___________________________");
					}
					else 
					{
//						MyLog(0," 길드번호 없는 유저 시작 ___________________________");
//						MyLog(0," 성문,수호석 죽인 유저 : %s OldFame : %d, NewFame : %d 시작 _________________________",connections[ServerID].name,connections[ServerID].chrlst.fame,connections[ServerID].chrlst.fame+10);
						connections[ServerID].chrlst.GainedFame+=10;					// 개인이므로 10을 가진다.
//						MyLog(0," 길드번호 없는 유저 끝   ___________________________");
					}
					break;
				case 1 :
				case 2 :
				case 3 :
				case 4 :
//					MyLog(0," 성문,수호석 죽인 유저(부대번호 : %d) : %s OldFame : %d, NewFame : %d 시작 _________________________",connections[ServerID].chrlst.NWCharacter.SquadNo,connections[ServerID].name,connections[ServerID].chrlst.fame,connections[ServerID].chrlst.fame+5);
//					MyLog(0," 부대에게 나눠줌 시작 _________________________");
					connections[ServerID].chrlst.GainedFame+=5;						// 자신은 5를 가지고 	
					GiveSquadPoint2Other(ServerID,4);							// 부대에게는 4를 나누어 준다.
//					MyLog(0," 부대에게 나눠줌 끝 _________________________");
					break;
				}
				break;
			case 64 :					// 보스 수호석을 깼다.
				switch(connections[ServerID].chrlst.NWCharacter.SquadNo)	//부대 번호에 따라 틀리다.
				{	
				case 0 :												// 부대편성이 되지 않은 유저이면
					if (connections[ServerID].chrlst.GetGuildCode())	// 길드 코드를 가지고 있다면..
					{
//						MyLog(0," 최종 수호석 죽인 유저(길드번호 : %d) : %s OldFame : %d, NewFame : %d 시작 _________________________",connections[ServerID].chrlst.GetGuildCode(),connections[ServerID].name,connections[ServerID].chrlst.fame,connections[ServerID].chrlst.fame+12);
//						MyLog(0," 길드에게 나눠줌 시작 _________________________");
						connections[ServerID].chrlst.GainedFame+=12;					// 자신은 12 포인트를 가지고 
						GiveGuildPoint2Other(ServerID,8);						// 다른 길드에게는 8를 준다.
//						MyLog(0," 길드에게 나눠줌 끝 ___________________________");
					}
					else 
					{
//						MyLog(0," 길드번호 없는 유저 시작 ___________________________");
//						MyLog(0," 최종 수호석 죽인 유저 : %s OldFame : %d, NewFame : %d 시작 _________________________",connections[ServerID].name,connections[ServerID].chrlst.fame,connections[ServerID].chrlst.fame+20);
						connections[ServerID].chrlst.GainedFame+=20;					// 개인이므로 20을 가진다.
//						MyLog(0," 길드번호 없는 유저 끝   ___________________________");
					}
					break;
				case 1 :
				case 2 :
				case 3 :
				case 4 :
//					MyLog(0," 최종,수호석 죽인 유저(부대번호 : %d) : %s OldFame : %d, NewFame : %d 시작 _________________________",connections[ServerID].chrlst.NWCharacter.SquadNo,connections[ServerID].name,connections[ServerID].chrlst.fame,connections[ServerID].chrlst.fame+10);
//					MyLog(0," 부대에게 나눠줌 시작 _________________________");
					connections[ServerID].chrlst.GainedFame+=10;						// 자신은 10를 가지고 	
					GiveSquadPoint2Other(ServerID,8);							// 부대에게는 8를 나누어 준다.
//					MyLog(0," 부대에게 나눠줌 끝 _________________________");
					break;
				}
				break;
			}
		}
		else
		{
//			MyLog(0," 전쟁터에 없는 유저 시작 _________________________");
//			MyLog(0," 전쟁터 없는 유저 이름 : %s, 죽인 스프라이트 번호 : %d",(*itor)->GetUserName(),(*itor)->GetKillSprNo());
//			MyLog(0," 전쟁터에 없는 유저 끝   _________________________");
		}
	}
//	MyLog(0,"명성치 계산 끝__________________________________________________");
}	//> CSD-030324

void cWarfield::ClearGainedFame()				// 020115 LTS
{
	for (int i=DRAGON_CONNECTIONS_START; i<DRAGON_MAX_CONNECTIONS; i++ )
	{
		if (CheckServerId(i)) connections[i].chrlst.GainedFame=0;
	}
}

void cWarfield::CalcNationWar_Exp()			// 020115 LTS		// 국가전 버그
{
	for (int i=DRAGON_CONNECTIONS_START; i<DRAGON_MAX_CONNECTIONS; i++ )
	{
		CHARLIST *ch = ::CheckServerId(i);
		if(!ch){ continue; }
	
		if(m_NationCode == ch->name_status.nation)
		{
			int Exp_Grade = ch->GetLevel(); // CSD-030806
			int tempExp_Grade=ch->GainedFame/5;
			switch (tempExp_Grade)
			{
			case 0 :	{ Exp_Grade-=2; }break;	// 0  -  4
			case 1 :	{ Exp_Grade-=0; }break;	// 5  -  9
			case 2 : 
			case 3 :	{ Exp_Grade+=2; }break;	// 10 - 19
			case 4 : 
			case 5 :	{ Exp_Grade+=3; }break;	// 20 - 29
			default :	{ Exp_Grade+=4; }break;	// 30 -
			}

			if (ch->NWCharacter.isCommander) {Exp_Grade+=5;}	//사령관 ,부대장 중복
			if (ch->NWCharacter.isSquadLeader) {Exp_Grade+=2;}
			
			::MyLog(0,"FameChange Name : %s, OLD : %d, NEW : %d, Gap : %d, Exp_Grade : %d",
						connections[i].name, ch->fame, ch->fame+ch->GainedFame,
						ch->GainedFame, Exp_Grade);
			SaveChangeFame(ch,ch->fame,ch->fame+ch->GainedFame,LF_NATIONWAR);		// 020909 LTS
			
			ch->fame+=ch->GainedFame;
			
			if (Exp_Grade>110) {Exp_Grade=110;}
		    if (ch->ChangeUpAddExpBonus(g_pNationWar_Exp[Exp_Grade]))
			{	//< CSD-TW-030624
				::isLevelUp(ch);
				t_packet packet;
				packet.h.header.type=CMD_FAME_EXP_CHANGE;
				packet.u.NationWar.FameExpCahnge.GainedFame=ch->GainedFame;
				packet.u.NationWar.FameExpCahnge.GainedExp=g_pNationWar_Exp[Exp_Grade];
				packet.h.header.size=sizeof(t_FameExpChange);
				QueuePacket(connections,i,&packet,1);
				MyLog(0,"Exp Change OLD : %d, New : %d, Gap : %d",
						ch->Exp,ch->Exp+g_pNationWar_Exp[Exp_Grade],g_pNationWar_Exp[Exp_Grade]);
						ch->Exp+=g_pNationWar_Exp[Exp_Grade];
			}	//> CSD-TW-030624
		    else
			{
				t_packet packet;
				packet.h.header.type=CMD_FAME_EXP_CHANGE;
				packet.u.NationWar.FameExpCahnge.GainedFame=ch->GainedFame;
				packet.u.NationWar.FameExpCahnge.GainedExp=0;
				packet.h.header.size=sizeof(t_FameExpChange);
				QueuePacket(connections,i,&packet,1);
			}
			ch->GainedFame=0;								//다시 초기화 
		}
		else
		{
			if (ch->name_status.nation==NW_YL)
			{
				if (ch->NWCharacter.YL_JoinNation==m_NationCode)
				{
					int Exp_Grade = ch->GetLevel(); // CSD-030806
					int tempExp_Grade=ch->GainedFame/5;
					switch (tempExp_Grade)
					{
					case 0 : Exp_Grade-=2;	break;	// 0  -  4
					case 1 : Exp_Grade-=0;	break;	// 5  -  9
					case 2 : 
					case 3 : Exp_Grade+=2;	break;	// 10 - 19
					case 4 : 
					case 5 : Exp_Grade+=3;	break;	// 20 - 29
					default : Exp_Grade+=4;		// 30 -
					}
					
					if (ch->NWCharacter.isCommander) {Exp_Grade+=5;}	//사령관 ,부대장 중복
					if (ch->NWCharacter.isSquadLeader) {Exp_Grade+=2;}
					
					MyLog(0,"FameChange Name : %s, OLD : %d, NEW : %d, Gap : %d, Exp_Grade : %d",
							connections[i].name, ch->fame,
							ch->fame+ch->GainedFame, ch->GainedFame, Exp_Grade);
					SaveChangeFame(ch,ch->fame,ch->fame+ch->GainedFame,LF_NATIONWAR);		// 020909 LTS
					ch->fame+=ch->GainedFame;
					if (Exp_Grade>110) {Exp_Grade=110;}

					if (ch->ChangeUpAddExpBonus(g_pNationWar_Exp[Exp_Grade]))
					{	//< CSD-TW-030624
						::isLevelUp(ch);
						t_packet packet;
						packet.h.header.type=CMD_FAME_EXP_CHANGE;
						packet.u.NationWar.FameExpCahnge.GainedFame=ch->GainedFame;
						packet.u.NationWar.FameExpCahnge.GainedExp=g_pNationWar_Exp[Exp_Grade];
						packet.h.header.size=sizeof(t_FameExpChange);
						QueuePacket(connections,i,&packet,1);
						MyLog(0,"Exp Change OLD : %d, New : %d, Gap : %d",
								ch->Exp,ch->Exp+g_pNationWar_Exp[Exp_Grade],g_pNationWar_Exp[Exp_Grade]);
								ch->Exp+=g_pNationWar_Exp[Exp_Grade];
					}	//> CSD-TW-030624
					else
					{
						t_packet packet;
						packet.h.header.type=CMD_FAME_EXP_CHANGE;
						packet.u.NationWar.FameExpCahnge.GainedFame=ch->GainedFame;
						packet.u.NationWar.FameExpCahnge.GainedExp=0;
						packet.h.header.size=sizeof(t_FameExpChange);
						QueuePacket(connections,i,&packet,1);
					}
					ch->GainedFame=0;								//다시 초기화 
				}
			}
		}
	}
}

void cWarfield::CalcFame(bool AttackerWin)		// 명성치를 계산해 준다.	// 020115 LTS // 함수교체 
{
	ClearGainedFame();	// 임시 명성치 초기화

	for (int i=DRAGON_CONNECTIONS_START; i<DRAGON_MAX_CONNECTIONS; i++ )
	{
		CHARLIST *ch = ::CheckServerId(i);
		if(!ch){continue;}

		HonorWon[i] += 2;

		if (AttackerWin)	// 공격국의 승리
		{
			switch (ch->name_status.nation)
			{
			case NW_BY : 
				if (m_NationCode==NW_BY) 
				{
					ch->GainedFame+=3;		// 방어국 바이서스가 패배했으므로 0
					SaveHonorWon(ch->GetServerID(), HonorWon[ch->GetServerID()]); //Eleval 07/09/09 - To save the won honor
				}
				else ch->GainedFame+=0;							// 공격국의 승리 
				break;
			case NW_ZY :										// 방어국 자이펀이 패배 했으므로 0
				if (m_NationCode==NW_ZY) 
				{
					ch->GainedFame+=3;
					SaveHonorWon(ch->GetServerID(), HonorWon[ch->GetServerID()]); //Eleval 07/09/09 - To save the won honor
				}
				else ch->GainedFame+=0;							// 공격국의 승리
				break;
			case NW_YL :										// 일스이면
				switch (ch->NWCharacter.YL_JoinNation)
				{
				case NW_BY : 
					if (m_NationCode==NW_BY)
					{
						ch->GainedFame+=3;	// 방어국 바이서스가 패배 했으므로 가담자이면 0
						SaveHonorWon(ch->GetServerID(), HonorWon[ch->GetServerID()]); //Eleval 07/09/09 - To save the won honor
					}
					else ch->GainedFame+=0;						// 공격국이 승리	
					break;
				case NW_ZY : 
					if (m_NationCode==NW_ZY) 
					{
						ch->GainedFame+=3;	// 방어국 자이펀이 패배했으므로 가담자 0
						SaveHonorWon(ch->GetServerID(), HonorWon[ch->GetServerID()]); //Eleval 07/09/09 - To save the won honor
					}
					else ch->GainedFame+=0;						// 공격국의 승리
					break;
				}
				break;
			}
		}
		else		//방어국의 승리 
		{
			switch (ch->name_status.nation)
			{
			case NW_BY : 
				if (m_NationCode==NW_BY) 
				{
					ch->GainedFame+=5;
					SaveHonorWon(ch->GetServerID(), HonorWon[ch->GetServerID()]); //Eleval 07/09/09 - To save the won honor
				}
				else ch->GainedFame+=0;
				break;
			case NW_ZY : 
				if (m_NationCode==NW_ZY) 
				{
					ch->GainedFame+=5;
					SaveHonorWon(ch->GetServerID(), HonorWon[ch->GetServerID()]); //Eleval 07/09/09 - To save the won honor
				}
				else ch->GainedFame+=0;
				break;
			case NW_YL : 
				switch (ch->NWCharacter.YL_JoinNation)
				{
				case NW_BY : 
					if (m_NationCode==NW_BY) 
					{
						ch->GainedFame+=5;
						SaveHonorWon(ch->GetServerID(), HonorWon[ch->GetServerID()]); //Eleval 07/09/09 - To save the won honor
					}
					else ch->GainedFame+=0;
					break;
				case NW_ZY : 
					if (m_NationCode==NW_ZY) 
					{
						ch->GainedFame+=5;
						SaveHonorWon(ch->GetServerID(), HonorWon[ch->GetServerID()]); //Eleval 07/09/09 - To save the won honor
					}
					else ch->GainedFame+=0;
					break;
				}
				break;
			}
		}
	}

	if (AttackerWin) 
	{
//		MyLog(0,"공격자가 이겼습니다. 명성치 계산루틴을 수행 합니다.__________");
		ReturnFame(); //곌계석, 수호석 죽인놈 계산..  // 승리했을때만 계산한다.	// 
//		MyLog(0,"명성치 계산루틴을 종료 합니다._______________________________");
	}

	CalcNationWar_Exp();

	for(int i = 0; i < DRAGON_MAX_CONNECTIONS_ ; i ++) //Eleval 07/09/09 - To reset the honor won on the warfield
	{
		HonorWon[i] = 0;
	}

}

void cWarfield::ProcessNationWarEnd(bool AttackerWin)	//전쟁의 종료를 처리한다.	// 아직소유가 바뀌지 않았다	// 011016 LTS
{
	t_WarBBS WarBBS;
	ClearAllSquad(0); // 모든 나라..
	short BYCount,ZYCount,BYDeathCount,ZYDeathCount,YL_BYCount,YL_ZYCount;

	if (m_NationCode==NW_BY)	// 소유자가 바이였다
	{
		BYCount=g_SquadCount[NW_DEFENCER][0]+g_SquadCount[NW_DEFENCER][1]+g_SquadCount[NW_DEFENCER][2]+
			    g_SquadCount[NW_DEFENCER][3]+g_SquadCount[NW_DEFENCER][4];
		ZYCount=g_SquadCount[NW_ATTACKER][0]+g_SquadCount[NW_ATTACKER][1]+g_SquadCount[NW_ATTACKER][2]+
				g_SquadCount[NW_ATTACKER][3]+g_SquadCount[NW_ATTACKER][4];
		BYDeathCount=m_KilledCount[NW_DEFENCER];
		ZYDeathCount=m_KilledCount[NW_ATTACKER];
		YL_BYCount=m_JoinSquadCount[NW_DEFENCER];
		YL_ZYCount=m_JoinSquadCount[NW_ATTACKER];
	}
	else						// 소유자가 자이였다
	{
		ZYCount=g_SquadCount[NW_DEFENCER][0]+g_SquadCount[NW_DEFENCER][1]+g_SquadCount[NW_DEFENCER][2]+
				g_SquadCount[NW_DEFENCER][3]+g_SquadCount[NW_DEFENCER][4];
		BYCount=g_SquadCount[NW_ATTACKER][0]+g_SquadCount[NW_ATTACKER][1]+g_SquadCount[NW_ATTACKER][2]+
		        g_SquadCount[NW_ATTACKER][3]+g_SquadCount[NW_ATTACKER][4];
		ZYDeathCount=m_KilledCount[NW_DEFENCER];
		BYDeathCount=m_KilledCount[NW_ATTACKER];
		YL_ZYCount=m_JoinSquadCount[NW_DEFENCER];
		YL_BYCount=m_JoinSquadCount[NW_ATTACKER];
	}
	
	if (AttackerWin) SendMapPossessChanged();				// 공격국이 승리를 했으면..		// 맵소유변경을 관리서버에게 알려준다.

	SendWarBeginEndMessage2Client(0);						// 맵에 있는 녀석들에게 유저에게 전쟁이 끝났음을 알린다.	// 옷 갈아입어라
	CalcFame(AttackerWin);									// 명성치를 계산한다. 
	SendCMD_WAR_END(m_WarfieldCode,NATION_MANAGE_SERVER);	// 관리서버에게 워필드초기화를 요청
											
	WarBBS.WarfieldNo=m_WarfieldCode;						// "%d국이 %d전쟁터에서 승리하였습니다."
	WarBBS.BBSType=BBS_WAR_VICTORY;
	WarBBS.LanNo=0;											// 010924 LST
	WarBBS.ArgType=BBS_NUM;
	WarBBS.Size=14;
	WarBBS.Arg[0]=m_NationCode;								// 전쟁에 승리한 나라..
	WarBBS.Arg[1]=AttackerWin;
	if (BYCount<0) BYCount=0;
	memcpy(&WarBBS.Arg[2],&BYCount,sizeof(short));	// 공격자 카운트
	if (ZYCount<0) ZYCount=0;
	memcpy(&WarBBS.Arg[4],&ZYCount,sizeof(short));	// 공격자 카운트
	if (YL_BYCount<0) YL_BYCount=0;
	memcpy(&WarBBS.Arg[6],&YL_BYCount,sizeof(short));	// 공격자 카운트
	if (BYDeathCount<0) BYDeathCount=0;
	memcpy(&WarBBS.Arg[8],&BYDeathCount,sizeof(short));	// 공격자 카운트
	if (ZYDeathCount<0) ZYDeathCount=0;
	memcpy(&WarBBS.Arg[10],&ZYDeathCount,sizeof(short));	// 공격자 카운트
	if (YL_ZYCount<0) YL_ZYCount=0;
	memcpy(&WarBBS.Arg[12],&YL_ZYCount,sizeof(short));	// 공격자 카운트

	SendWarBBS2Maps(&WarBBS);								// 공격국, 방어국, 중립국 맵에 메시지를 보낸다,
	SendWarBBS2Clients(&WarBBS);							// 전쟁서버 유저에게 보낸다.
	
}

bool cWarfield::CanRegCommanderCandidater(int Kind,int CandidaterNo)		// 사령관 후보에 등록할수 있는지 확인한다.
{
	if (m_tSquad[Kind].Commander.CO_ID>0) return false;				// 사령관이 있다.
	if (m_tSquad[Kind].Candidater[CandidaterNo].CO_ID>0) return false;	
	else return true;
}


void cWarfield::SetCandidaterInfo(int Kind,int CandidaterNo,t_CommanderInformation* CandidaterInfo)	// 사령관 후보를 세트한다.
{
	m_tSquad[Kind].Candidater[CandidaterNo].CO_ID=CandidaterInfo->CO_ID;
	memcpy(m_tSquad[Kind].Candidater[CandidaterNo].CO_Name,CandidaterInfo->CO_Name,NW_NAME_MAX);
	m_tSquad[Kind].Candidater[CandidaterNo].Fame=CandidaterInfo->Fame;
	m_tSquad[Kind].Candidater[CandidaterNo].btLevel=CandidaterInfo->btLevel; // CSD-030806
	m_tSquad[Kind].Candidater[CandidaterNo].Face=CandidaterInfo->Face;
	m_tSquad[Kind].Candidater[CandidaterNo].Gender=CandidaterInfo->Gender;
	m_tSquad[Kind].Candidater[CandidaterNo].Class=CandidaterInfo->Class;	// 011016 LTS
	m_tSquad[Kind].Candidater[CandidaterNo].nDual=CandidaterInfo->nDual;	// 011020 LTS
	m_tSquad[Kind].Candidater[CandidaterNo].nStep=CandidaterInfo->nStep;	// 011020 LTS
	m_tSquad[Kind].Candidater[CandidaterNo].LadderScore=CandidaterInfo->LadderScore;
}


void cWarfield::CommanderEnter(int Type)
{
	// Type : NW_ATTACKER : 공격자, NW_DEFENCER : 방어자, NW_REINFORCE : 지원군
	m_bCommanderExist[Type]=true;				// LTS TEMP
}
void cWarfield::CommanderLeave(int Type)
{
	m_bCommanderExist[Type]=false;
}
void cWarfield::SquadLeaderEnter(int Type,int SquadNo)
{
	// SquadNo : 0:부대없음 1:1번부대, 2:2번부대.... 4:4번부대
	// 참조 
	m_bSquadLeaderExist[Type][SquadNo]=true;
}
void cWarfield::SquadLeaderLeave(int Type,int SquadNo)
{
	m_bSquadLeaderExist[Type][SquadNo]=false;
}


void cWarfield::DisplaySquadMember(int Type,int SquadNo)		// 
{
	switch (Type)
	{
	case NW_ATTACKER  :	DisplayMember(&g_AttackSquadList[SquadNo]);		break;
	case NW_DEFENCER  :	DisplayMember(&g_DefenceSquadList[SquadNo]);	break;
	case NW_REINFORCE :	DisplayMember(&g_ReinforceSquadList[SquadNo]);	break;
	case NW_COMMANDER :	DisplayMember(&g_CommanderList[SquadNo]);		break;
	}
}
void cWarfield::DisplayMember(SquadList* g_SquadList)	// 스쿼드의 멤버를 보여준다. // 디버그용
{
	MyLog(0,"Squad Member List-----------------------");
	for (SquadListItor itor=g_SquadList->begin();itor!=g_SquadList->end();itor++)
	{
		MyLog(0,"Squad Member : %d",(*itor)->GetData());
	}
	MyLog(0,"Squad Member List End--------------------");
}

void cWarfield::DeleteSquadMember(const int cn)    // 캐릭터의 커넥션 ID로 알맞게 SQUAD HASH에서 삭제한다.	// 011016 LTS//020903 lsw
{
	if (isAttacker(g_wMapServerPort-BASE_WARFIELD_PORT,connections,cn))		// 부대 해쉬에서 지운다.
	{
		CHARLIST * ch = CheckServerId(cn);
		if(!ch)
		{
			return;
		}
		DeleteSquadMember(NW_ATTACKER,ch->NWCharacter.SquadNo,cn);
		if (ch->name_status.nation!=NW_YL) 
		{
			g_SquadCount[NW_ATTACKER][ch->NWCharacter.SquadNo]--;	//부대 인원수를 감소시킨다.
		}
		else
		{
			m_JoinSquadCount[NW_ATTACKER]--;
		}
		if (ch->NWCharacter.isCommander||ch->NWCharacter.isSquadLeader)
		{
			DeleteSquadMember(NW_COMMANDER,NW_ATTACKER,cn);		// 사령관 해쉬에서도 지운다.
		}
		if (ch->NWCharacter.isCommander) 
		{
			CommanderLeave(NW_ATTACKER);		// 사령관이 떠나면 호출해주는 함수 
		}

		if (ch->NWCharacter.isSquadLeader) 
		{
			SquadLeaderLeave(NW_ATTACKER,ch->NWCharacter.SquadNo);
		}
	}
	else
	{
		CHARLIST * ch = CheckServerId(cn);
		if(!ch)
		{
			return;
		}

		DeleteSquadMember(NW_DEFENCER,ch->NWCharacter.SquadNo,cn);
		if (ch->name_status.nation!=NW_YL) 
		{
			g_SquadCount[NW_DEFENCER][ch->NWCharacter.SquadNo]--;	// 부대인원수를 감소시킨다.
		}
		else 
		{
			m_JoinSquadCount[NW_DEFENCER]--;
		}

		if (ch->NWCharacter.isCommander||ch->NWCharacter.isSquadLeader)
		{
			DeleteSquadMember(NW_COMMANDER,NW_DEFENCER,cn);					// 사령관 해쉬에서도 지운다.
		}
		if (ch->NWCharacter.isCommander) 
		{
			CommanderLeave(NW_DEFENCER);
		}
		if (ch->NWCharacter.isSquadLeader) 
		{
			SquadLeaderLeave(NW_DEFENCER,connections[cn].chrlst.NWCharacter.SquadNo);
		}
	}
}

void cWarfield::DeleteSquadMember(const int Type,const int SquadNo, const int ID)//020903 lsw
{
	switch (Type)
	{
	case NW_ATTACKER  :	DeleteMember(&g_AttackSquadList[SquadNo],ID);		break;
	case NW_DEFENCER  :	DeleteMember(&g_DefenceSquadList[SquadNo],ID);		break;
	case NW_REINFORCE :	DeleteMember(&g_ReinforceSquadList[SquadNo],ID);	break;
	case NW_COMMANDER :	DeleteMember(&g_CommanderList[SquadNo],ID);		break;
	}

}
void cWarfield::DeleteMember(SquadList* g_SquadList,int ID) // Type : Attacker, Defender
{
	for (SquadListItor itor=g_SquadList->begin();itor!=g_SquadList->end();)
	{
		if ((*itor)->GetData()==ID)
		{
			SAFE_DELETE((*itor));
			itor=g_SquadList->erase(itor);
		}
    else 
      itor++;
	}
}

void cWarfield::SendSquadMessage(int Type,int SquadNo,t_packet* p)
{
	switch (Type)
	{
	case NW_ATTACKER  :	cWarfield::SendMessage(&g_AttackSquadList[SquadNo],p);		break;
	case NW_DEFENCER  :	cWarfield::SendMessage(&g_DefenceSquadList[SquadNo],p);		break;
	case NW_REINFORCE :	cWarfield::SendMessage(&g_ReinforceSquadList[SquadNo],p);	break;
	case NW_COMMANDER :	cWarfield::SendMessage(&g_CommanderList[SquadNo],p);		break;
	}
}

void cWarfield::SendMessage(SquadList* g_SquadList,t_packet* p)
{
	for (SquadListItor itor=g_SquadList->begin();itor!=g_SquadList->end();itor++)
	{
		QueuePacket(connections,(*itor)->GetData(),p,1);
	}
}

void cWarfield::DeleteSquadHash(SquadList* g_SquadList)
{
	for (SquadListItor itor=g_SquadList->begin();itor!=g_SquadList->end();)
	{
		SAFE_DELETE((*itor));
		itor=g_SquadList->erase(itor);
	}
}

void cWarfield::DeleteAllSquadMember()
{
	int i;
	for (i=0;i<NW_SQUAD_MAX;i++)			// Squad Hash를 지운다.
	{
		DeleteSquadHash(&g_AttackSquadList[i]);
		DeleteSquadHash(&g_DefenceSquadList[i]);
		DeleteSquadHash(&g_ReinforceSquadList[i]);
	}
	for (i=0;i<NW_SQUAD_KIND;i++)				// 부대장, 커맨더의 Squad hash를 지운다.
	{
		DeleteSquadHash(&g_CommanderList[i]);
	}
}

void cWarfield::InsertSquadMember(int cn,int Status)	// 011016 LTS
{
	CHARLIST *ch = CheckServerId(cn);
	if(!ch) {return;}

	if (isAttacker(g_wMapServerPort-BASE_WARFIELD_PORT,connections,cn))		// 부대 해쉬에서 지운다.
	{
		InsertSquadMember(NW_ATTACKER,ch->NWCharacter.SquadNo,cn);	//일스든 ,공격국이든 부대에 집어넣는다.
		if (ch->name_status.nation!=NW_YL) g_SquadCount[NW_ATTACKER][ch->NWCharacter.SquadNo]++;	// 부대 인원수 카운트를 증가시킨다.
		else m_JoinSquadCount[NW_ATTACKER]++;	//일수용병수를 센다 
		if (ch->NWCharacter.isCommander||ch->NWCharacter.isSquadLeader)
			InsertSquadMember(NW_COMMANDER,NW_ATTACKER,cn,Status);	
		// 사령관 해쉬에서도 지운다.
		if (ch->NWCharacter.isCommander) CommanderEnter(NW_ATTACKER);	//사령관이 들어오면 함수를 호출한다.
		if (ch->NWCharacter.isSquadLeader) SquadLeaderEnter(NW_ATTACKER,ch->NWCharacter.SquadNo);

		SquadRareReSet(NW_ATTACKER, cn,0 );
	}
	else
	{
		InsertSquadMember(NW_DEFENCER,ch->NWCharacter.SquadNo,cn);
		if (ch->name_status.nation!=NW_YL) g_SquadCount[NW_DEFENCER][ch->NWCharacter.SquadNo]++;	// 부대 인원수 카운트를 증가시킨다.
		else m_JoinSquadCount[NW_DEFENCER]++;	//일수용병수를 센다 
		if (ch->NWCharacter.isCommander||ch->NWCharacter.isSquadLeader)
			InsertSquadMember(NW_COMMANDER,NW_DEFENCER,cn,Status);					// 사령관 해쉬에서도 지운다.
		if (ch->NWCharacter.isCommander) CommanderEnter(NW_DEFENCER);	//사령관이 들어오면 함수를 호출한다.
		if (ch->NWCharacter.isSquadLeader) SquadLeaderEnter(NW_DEFENCER,ch->NWCharacter.SquadNo);
		
		SquadRareReSet(NW_DEFENCER, cn,0);
	}
}

void cWarfield::InsertSquadMember(int Type,int SquadNo,int ID,int Status)
{
	switch (Type)
	{
	case NW_ATTACKER	: InsertMember(&g_AttackSquadList[SquadNo],ID); break;			// Attacker
	case NW_DEFENCER	: InsertMember(&g_DefenceSquadList[SquadNo],ID); break;			// Defencer
	case NW_REINFORCE	: InsertMember(&g_ReinforceSquadList[SquadNo],ID);break;		// Reinforce
	case NW_COMMANDER	: InsertMember(&g_CommanderList[SquadNo],ID,Status);break;				// Attack SquadLeader, Commander
	}
}

void cWarfield::InsertMember(SquadList* g_SquadList,int ID,int Status) // Type : Attacker, Defender
{
	g_SquadList->push_back(new SquadMember(ID,Status));
}

void cWarfield::DeleteAllKiller()
{
	for (KillerListItor itor=g_KillerList.begin();itor!=g_KillerList.end();)
	{
		SAFE_DELETE((*itor));
		itor=g_KillerList.erase(itor);
	}
}

void cWarfield::DeleteKiller(char* Name)
{
	for (KillerListItor itor=g_KillerList.begin();itor!=g_KillerList.end();)
	{
		if (strcmp((*itor)->GetUserName(),Name)==0)
		{
			SAFE_DELETE((*itor));
			itor=g_KillerList.erase(itor);
		}
    else 
      itor++;
	}
}

void cWarfield::InsertKiller(char* Name,int KillSprNo)
{
	g_KillerList.push_back(new cKiller(Name,KillSprNo));
}

void cWarfield::InsertMonsterData(t_WeaponStruct Monster)
{
	m_MonsterList.push_back(new cMonster(Monster));
}

void cWarfield::DeleteAllMonsterData()
{
	for (MonsterListItor itor=m_MonsterList.begin();itor!=m_MonsterList.end();)
	{
		SAFE_DELETE((*itor));
		itor=m_MonsterList.erase(itor);
	}
}

void cWarfield::FirstMakeMonster()		// 모든  NPC (성문, 수호석이 살아있을때 만들어진다.)
{
	int Num;
	t_WeaponStruct Monster;
	for (MonsterListItor itor=m_MonsterList.begin();itor!=m_MonsterList.end();itor++)
	{
		Num=GetAliveNPCList();
		(*itor)->SetMonsterID(Num);
		Monster=(*itor)->GetMonsterData();
		NPC_Create(Monster.NPC_ID,Monster.NPC_Index,Monster.Locationx,Monster.Locationy,Monster.EventNo,Monster.GenerationPos,Monster.GenerationType);
	}
	m_bFirstMakeMonster=true;
}

void cWarfield::MakeMonster()
{
	t_WeaponStruct Monster;
	for (MonsterListItor itor=m_MonsterList.begin();itor!=m_MonsterList.end();itor++)
	{
		Monster=(*itor)->GetMonsterData();
		NPC_Create(Monster.NPC_ID,Monster.NPC_Index,Monster.Locationx,Monster.Locationy,Monster.EventNo,Monster.GenerationPos,Monster.GenerationType);
	}
}

void cWarfield::PrepareSquadForWar()			// 전쟁터에 상주해 있는 인원을 부대에 편성한다.
{
	for( int i = DRAGON_CONNECTIONS_START; i<DRAGON_MAX_CONNECTIONS; i++ )
	{
		if( CheckServerId( i ) ) InsertSquadMember(i,connections[i].chrlst.bAlive);
	}
}

char* cWarfield::GetLeadCandidaterName(int Kind)
{
	if (m_Status!=NW_PREPARE) return NULL;
	int CandidaterNo=-1;
	int tempVoteCount=-1;
	for(int i=0;i<NW_CANDIDATE_MAX;i++)
	{
		if (m_tSquad[Kind].Candidater[i].VoteCount>tempVoteCount) 
		{
			CandidaterNo=i;
			tempVoteCount=m_tSquad[Kind].Candidater[i].VoteCount;
		}
	}
	if (CandidaterNo>=0)
	{
		return m_tSquad[Kind].Candidater[CandidaterNo].CO_Name;
	}
	else return NULL;
}

DWORD cWarfield::GetWarLoopTime() // 011025 LTS
{ 
	return DWORD((m_LoopTime-g_WarTime)/60); 
}				

void cWarfield::SetWarLoopTime(DWORD LoopTime) // 011025 LTS
{ 
	m_LoopTime=g_WarTime+LoopTime*60; 
}		

void cWarfield::SendSecretActiveMessage2Manager()
{
	t_packet packet;

	packet.h.header.type=CMD_SECRET_OPEN;			//비밀지역이 열렸다고 보낸다.
	packet.u.NationWar.SecretOpen.WarfieldNo=m_WarfieldCode;
	packet.u.NationWar.SecretOpen.isOpen=m_bBonusActive;
	packet.h.header.size=sizeof(t_SecretOpen);
	SendPacket2NWManager(&packet);

}

void cWarfield::ActiveBonusTime()	
{ 
	m_bBonusActive=true; 
	m_dBonusLoopTime=0; 
	if (m_bFirstMakeMonster) MakeMonster();
	else FirstMakeMonster();

	SendSecretActiveMessage2Manager();
}

void cWarfield::IncNationMemberCount(int Nation)
{
	switch (Nation)
	{
	case NW_BY : m_NationMemberCount[0]++; break;
	case NW_ZY : m_NationMemberCount[1]++; break;
	case NW_YL : m_NationMemberCount[2]++; break;
	}
}

void cWarfield::DecNationMemberCount(int Nation)
{
	switch (Nation)
	{
	case NW_BY : if (m_NationMemberCount[0]>0) m_NationMemberCount[0]--; break;
	case NW_ZY : if (m_NationMemberCount[1]>0) m_NationMemberCount[1]--; break;
	case NW_YL : if (m_NationMemberCount[2]>0) m_NationMemberCount[2]--; break;
	}
}

int cWarfield::GetNationMemberCount(int Nation)
{
	switch (Nation)
	{
	case NW_BY : return m_NationMemberCount[0];
	case NW_ZY : return m_NationMemberCount[1];
	case NW_YL : return m_NationMemberCount[2];
	}
	return 0;
}

int cWarfield::GetNationMemberMax(int Nation)
{
	switch (Nation)
	{
	case NW_BY : return m_LoopTimeDefine.NWBYJoinMAX;
	case NW_ZY : return m_LoopTimeDefine.NWZYJoinMAX;
	case NW_YL : return m_LoopTimeDefine.NWYLJoinMAX;
	}
	return NW_BY_JOIN_MAX;			
}

void cWarfield::LoopTimeChange(t_packet* p)
{
	m_LoopTime=g_WarTime+p->u.NationWar.WarLoopTime.LoopTime;
}

// Name  : cNation Class
// Usage : Class Member Funtion Definition
cNation::cNation()
{
	Warfield=NULL;
	m_WarStart=0;
	m_LocalWarStart=0;
	m_pWarStartInfo = NULL; // CSD-030715
	InitNation();
	ClearLocalWarPoint();
	LoadWarfieldPossession();					// 전쟁터 테이터를 읽어온다.
	ClearLocalWarResult();						// LTS NEW LOCALWAR
}

cNation::~cNation()
{
	// 워필드먼저 데스트로이 시키고 한다.
	for (int i=0;i<NW_WARFIELD_COUNT;i++)
	{
		Warfield[i].Destroy();
	}

	SAFE_DELETE_ARRAY(Warfield);
	SAFE_DELETE_ARRAY(m_pStartWarTime);
	SAFE_DELETE_ARRAY(LocalWarfield);
	SAFE_DELETE(m_pWarStartInfo); // CSD-030723
}

void cNation::ClearLocalWarResult()				// LTS NEW LOCALWAR
{
	for (int i=0;i<NW_NATION_COUNT;i++)
	{
		m_LocalWarResult[i]=TRUE;
	}
}

bool cNation::InitNation()						// 011028 LTS
{
	Warfield=new cWarfield[NW_WARFIELD_COUNT];	// 전쟁터를 생성한다. 메모리가 중복 된다.
	if (Warfield==NULL) return false;			// 추후 간소 클래스로 바꾸자.	

	for (int i = 0; i < NW_WARFIELD_COUNT; ++i)
	{	//< CSD-TW-030702
		m_VoteData[i] = 0;
	}	//> CSD-TW-030702

	LoadWarNo();

	if (!LoadWarfieldData()) 
	{
		JustMsg("Warfield Data Load Error!!");
		return false;
	}

	if (!LoadWarStartupData())							// LTS NEW_NATION_WAR
	{
		JustMsg("WarStartup Data Load Error!!");
		return false;
	}
	if (!this->LoadNewWarStartupData())
	{
		JustMsg("WarStartup Data Load Error!!");
		return false;
	}

	if (!InitLocalWarfield()) {return false;}		// 국지전 초기화 
	if (!InitNewWarfield()){return false;}
	ClearAllLocalSealStoneHP();// 030516 kyo
	return true;
}

bool cNation::LoadWarfieldData()
{
	for (int i=0;i<NW_WARFIELD_COUNT;i++)
	{
		if (!Warfield[i].LoadWarfieldData(BASE_WARFIELD_PORT+i)) 
		{
			return false;	// DB에서 로딩한다.
		}
	}
	return true;
}

void ReLoadLocalWarfieldData()	// 리로드 한다.
{
	if (isNationManageServer()) 
	{
		g_pNation->InitLocalWarfield();
	}
}

bool cNation::InitLocalWarfield()		// LTS NEW LOCALWAR
{	//< CSD-030715
	// 030919 HK YGI
//	char	NetworkDir[MAX_PATH];
//	char	MapServerConfigFileName[MAX_PATH];
	char	KeyName[MAX_PATH];
	char	tempString[MAX_PATH];
	int		j;

	m_LocalWarfieldSize=0;

	/*
	if (GetPrivateProfileString("Network","path","",NetworkDir,MAX_PATH,MAP_SERVER_INI_)<=0)
	{
		MyLog(0,"Mapserver.ini의  Path설정이 잘못되어있습니다.");
		return false;
	}
	sprintf(MapServerConfigFileName,"%s/data/MapServerConfig.ini",NetworkDir);
*/ // 030919 HK YGI

	m_LocalWarfieldSize=(DWORD)GetPrivateProfileInt("LocalWarfield","LocalWarfieldSize",0,MapServerConfigFileName);
	if (!m_LocalWarfieldSize)
	{
		MyLog(0,"%s의 [LocalWarfield],LocalWarfieldSize의 설정이 잘못되어있습니다.",MapServerConfigFileName);
		return false;
	}

	LocalWarfield=new cLocalWarfield[m_LocalWarfieldSize];

	int BYIndex=0,ZYIndex=0,YLIndex=0;

	MyLog(LOG_NORMAL, "KeyName, sizeof(KeyName),'LocalWarfield%dName',i"); //Eleval 22/08/09 - To debug Nation War (have to be removed later)

	for (int i=0;i<m_LocalWarfieldSize;i++)
	{
		LocalWarfield_Info tempLF;
		
		tempLF.Index=i;
		sprintf_s(KeyName, sizeof(KeyName),"LocalWarfield%dName",i);
		if (GetPrivateProfileString("LocalWarfield",KeyName,"",tempLF.MapName,20,MapServerConfigFileName)<=0)
		{
			return false;
		}
		sprintf_s(KeyName, sizeof(KeyName),"LocalWarfield%dPort",i);
		tempLF.Port=(DWORD)GetPrivateProfileInt("LocalWarfield",KeyName,0,MapServerConfigFileName);
		if (!tempLF.Port)
		{
			return false;
		}
		sprintf_s(KeyName, sizeof(KeyName),"LocalWarfield%dPhase",i);
		tempLF.Phase=(DWORD)GetPrivateProfileInt("LocalWarfield",KeyName,0,MapServerConfigFileName);
		if (!tempLF.Phase)
		{
			if (i) return false;
		}
		
		sprintf_s(KeyName, sizeof(KeyName),"LocalWarfield%dBYSealStone",i);
		GetPrivateProfileString("LocalWarfield",KeyName,NULL,tempString,MAX_PATH,MapServerConfigFileName);
		if (tempString==NULL)
		{
			return false;
		}
		
		MakeSealStoneNumber(tempString,tempLF.SealNo[0]);

		for (j=0;j<4;j++)		// 국지전 관리서버 국지전 승패를 확인하기 위한 데이터를 확인한다.
		{
			if (tempLF.SealNo[0][j]!=0)
			{
				LocalWarSealStoneStatus[0][BYIndex].Status=TRUE;						// 기본적으로 살아있다고 세팅
				LocalWarSealStoneStatus[0][BYIndex++].SealStoneNo=tempLF.SealNo[0][j];
			}

		}
		
		sprintf_s(KeyName, sizeof(KeyName),"LocalWarfield%dZYSealStone",i);
		GetPrivateProfileString("LocalWarfield",KeyName,NULL,tempString,MAX_PATH,MapServerConfigFileName);
		if (tempString==NULL)
		{
			return false;
		}
		
		MakeSealStoneNumber(tempString,tempLF.SealNo[1]);

		for (j=0;j<4;j++)
		{
			if (tempLF.SealNo[1][j]!=0)
			{
				LocalWarSealStoneStatus[1][ZYIndex].Status=TRUE;						// 기본적으로 살아있다고 세팅
				LocalWarSealStoneStatus[1][ZYIndex++].SealStoneNo=tempLF.SealNo[1][j];
			}

		}
		
		sprintf_s(KeyName, sizeof(KeyName),"LocalWarfield%dYLSealStone",i);
		GetPrivateProfileString("LocalWarfield",KeyName,NULL,tempString,MAX_PATH,MapServerConfigFileName);
		if (tempString==NULL)
		{
			return false;
		}
		
		MakeSealStoneNumber(tempString,tempLF.SealNo[2]);
		
		for (j=0;j<4;j++)
		{
			if (tempLF.SealNo[2][j]!=0)
			{
				LocalWarSealStoneStatus[2][YLIndex].Status=TRUE;						// 기본적으로 살아있다고 세팅
				LocalWarSealStoneStatus[2][YLIndex++].SealStoneNo=tempLF.SealNo[2][j];
			}

		}
		
		LocalWarfield[i].SetLocalWarfieldInfo(tempLF);		// 국가관리서버에는 결계석의 상태정보가 안들어가도 된다.
	}

  	return true;
}	//> CSD-030715

void cNation::SetWarFieldStatus(int WarfieldNo,int Status)				// 011028 LTS	// 함수교체 
{
	if (Warfield==NULL) return;		// 예외처리.	멥버변수...	
	if (WarfieldNo>NW_WARFIELD_COUNT) return;		// 상태가 잘못 들어왔다면.. 리턴..	
	if (!((Status==1) || (Status==2) || (Status==3) || (Status==4) || (Status==7) || (Status==8) ||  (Status==9) || (Status==10))) return;
	//if (Warfield[WarfieldNo].GetStatus()==NW_WAR)
	//{
	//	m_WarNo++;							// 전쟁 번호를 증가 시킨다.
	//	if (m_WarNo>126) m_WarNo=0;			// 전쟁번호는 0..126이 최고다.. 7Bit NW_Character에 의존..
	//	MyLog(1,"전쟁번호 : %d",m_WarNo);
	//}
	Warfield[WarfieldNo].SetStatus((char)Status); // 상태를 세트한다.
	if (Warfield[WarfieldNo].GetStatus()==NW_VOTE)
	{
		m_WarNo++;							// 전쟁 번호를 증가 시킨다.
		if (m_WarNo>126) m_WarNo=0;			// 전쟁번호는 0..126이 최고다.. 7Bit NW_Character에 의존..
		MyLog(1,"WarNo : %d",m_WarNo);
	}
	SendCMD_CHANGED_WARFIELD_STATUS();		// 전쟁터의 상태가 바뀌었다고 맵서버들에게 알린다.
											// 먼저 소유가 변경된다음 보내야 한다. cWarfield::Update
}

void cNation::CheckWarFieldStatus(int WarfieldNo,int Status)
{	//< CSD-030812
	const int nFieldNo = WarfieldNo - BASE_WARFIELD_PORT;
	Dbg_Assert(nFieldNo >= 0 && nFieldNo < NW_WARFIELD_COUNT);

	bool checkok = true;

	switch (nFieldNo)
	{
	case 0: 
		if (Warfield[1].GetStatus()!=1) checkok=false;
		if (Warfield[2].GetStatus()!=1) checkok=false;
		break;
	case 1:
		if (Warfield[0].GetStatus()!=1) checkok=false;
		if (Warfield[2].GetStatus()!=1) checkok=false;
		break;
	case 2:
		if (Warfield[0].GetStatus()!=1) checkok=false;
		if (Warfield[1].GetStatus()!=1) checkok=false;
		break;
	}

	if (checkok)	//상황을 바꿀수 있는데...
	{
		if (Status==NW_WAIT_WAR) 
		{
			SendCMD_SQUAD_LODING_POINT(WarfieldNo);			// 전쟁을 기다리는 중이면.. 부대의 로딩포인트 정보를 보낸다.
			SendCMD_SQUAD_RARE(WarfieldNo);					// 부대속성을 보낸다 
		}

		switch(Status)
		{
//		case NW_WAIT_VOTE : Warfield[WarfieldNo-BASE_WARFIELD_PORT].SetStatus(NW_VOTE); break;
		case NW_WAIT_PREPARE:	
			{	// 부대의 레어속성을 결정한다.
				Warfield[nFieldNo].SetStatus(NW_PREPARE);
				
				GetSquadRare(GetSquadRareSok2(nFieldNo,NW_ATTACKER,1),1);
				GetSquadRare(GetSquadRareSok2(nFieldNo,NW_ATTACKER,2),2);
				GetSquadRare(GetSquadRareSok2(nFieldNo,NW_ATTACKER,3),3);
				GetSquadRare(GetSquadRareSok2(nFieldNo,NW_ATTACKER,4),4);

				GetSquadRare(GetSquadRareSok2(nFieldNo,NW_DEFENCER,1),1);
				GetSquadRare(GetSquadRareSok2(nFieldNo,NW_DEFENCER,2),2);
				GetSquadRare(GetSquadRareSok2(nFieldNo,NW_DEFENCER,3),3);
				GetSquadRare(GetSquadRareSok2(nFieldNo,NW_DEFENCER,4),4);

				GetSquadRare(GetSquadRareSok2(nFieldNo,NW_REINFORCE,1),1);
				GetSquadRare(GetSquadRareSok2(nFieldNo,NW_REINFORCE,2),2);
				GetSquadRare(GetSquadRareSok2(nFieldNo,NW_REINFORCE,3),3);
				GetSquadRare(GetSquadRareSok2(nFieldNo,NW_REINFORCE,4),4);
				break;
			}	
		case NW_WAIT_WAR: 
			{
				Warfield[nFieldNo].SetStatus(NW_WAR);
				break;
			}
		case NW_WAIT_PEACE:
			{
				Warfield[nFieldNo].SetStatus(NW_PEACE);
				break;
			}
		}	

		MyLog(0,"NationWar Status Change  : Warfield No :%d, Status : %d",WarfieldNo,Status);
	}	

	t_packet packet;
	packet.h.header.type=CMD_CHECK_WARFIELD_STATUS_RESULT;
	packet.u.NationWar.CommonDataC.Data=checkok;
	packet.h.header.size=sizeof(t_CommonDataC);

	g_pServerTable->SendRajaPacketToOtherMapServer((WORD)WarfieldNo,(char *)&packet, packet.h.header.size+sizeof(t_header) );
}	//> CSD-030812

int cNation::GetWarfieldStatus(int WarfieldNo)
{	
	return Warfield[WarfieldNo].GetStatus();
}

void cNation::ClearWarfieldData(int WarfieldNo)
{
	Warfield[WarfieldNo].InitSquad();
	Warfield[WarfieldNo].InitCommanderCandidater();
	Warfield[WarfieldNo].ClearCommander();
}

short cNation::GetLocalWarfieldManCount(int NationIndex) 
{
	short tempCount=0;

	tempCount+=LocalWarfield[0].GetManCount(NationIndex);		// LOOP UNROLL
	tempCount+=LocalWarfield[1].GetManCount(NationIndex);
	tempCount+=LocalWarfield[2].GetManCount(NationIndex);
	tempCount+=LocalWarfield[3].GetManCount(NationIndex);
	tempCount+=LocalWarfield[4].GetManCount(NationIndex);
	tempCount+=LocalWarfield[5].GetManCount(NationIndex);
	tempCount+=LocalWarfield[6].GetManCount(NationIndex);

	return tempCount; 
}

void cNation::ClearLocalWarfieldManCount()
{
	LocalWarfield[0].ClearManCount();							// LOOP UNROLL
	LocalWarfield[1].ClearManCount();
	LocalWarfield[2].ClearManCount();
	LocalWarfield[3].ClearManCount();
	LocalWarfield[4].ClearManCount();
	LocalWarfield[5].ClearManCount();
	LocalWarfield[6].ClearManCount();
}

/*short cNation::GetLocalWarfieldPoint(int NationIndex)
{
	short tempPoint=0;

	tempPoint+=LocalWarfield[0].GetPoint(NationIndex);		// LOOP UNROLL
	tempPoint+=LocalWarfield[1].GetPoint(NationIndex);		
	tempPoint+=LocalWarfield[2].GetPoint(NationIndex);		
	tempPoint+=LocalWarfield[3].GetPoint(NationIndex);		
	tempPoint+=LocalWarfield[4].GetPoint(NationIndex);		
	tempPoint+=LocalWarfield[5].GetPoint(NationIndex);		
	tempPoint+=LocalWarfield[6].GetPoint(NationIndex);		

	return tempPoint;

}*/

/*void cNation::ClearLocalWarfieldPoint()
{
	LocalWarfield[0].ClearPoint();							// LOOP UNROLL
	LocalWarfield[1].ClearPoint();
	LocalWarfield[2].ClearPoint();
	LocalWarfield[3].ClearPoint();
	LocalWarfield[4].ClearPoint();
	LocalWarfield[5].ClearPoint();
	LocalWarfield[6].ClearPoint();
}*/




void cNation::SetLocalWarStatus2(int isStart)	// SendSealStoneBlock()에서 들어온다 시간에 의한	// LTS NEW LOCALWAR
{
	static int Start=0;
	if (Start==isStart) return;					// 시간에 의한 Status가 변함이 없다.
	MyLog(0,"LOCALWAR:: SetLocalWarStatus2   Now = %d,  After = %d", Start, isStart); // 030518 kyo
	Start=isStart;								// 시간에 의한 Status가 변했다.
	SetLocalWarStatus(Start);
}

void cNation::SetLocalWarStatus(int isStart)
{
	if (m_LocalWarStart == isStart) 
	{
		return;
	}

	g_LocalWarBegin=m_LocalWarStart=isStart; 
	MyLog(0,"LocalWar Status : %d",g_LocalWarBegin);

	if (m_LocalWarStart) 
	{
		ClearLocalWarfieldManCount();		// LTS BUG
		ClearLocalWarPoint();
		ClearLocalWarResult();
		ClearLocalWarRemainTime();
		GiveLife2SealStone();			// Status Change, GiveLife
		ClearLocalWarDefencePoint();			// LTS NEW LOCALWAR
		ClearSealStoneStatus();           // LTS NEW LOCALWAR BUG
		SendCMD_LOCALWAR_BEGIN2Client();

		t_packet packet;
		packet.h.header.type=CMD_LOCALWAR_BEGIN;
		packet.h.header.size=0;
		SendPacket2Maps(&packet);
		
		if (m_pWarStartInfo != NULL && m_pWarStartInfo->IsActive())
		{	//< CSD-030723
			g_LocalWarRemainTime = g_curr_time + m_pWarStartInfo->GetRemainTime();
		}	//> CSD-030723
	}
	else
	{
		SendCMD_LOCALWAR_END();	//결과를 보낸후 아래에 포인터를 클리어한다.
		ClearLocalWarfieldManCount();
		ClearLocalWarPoint();
		ClearLocalWarResult();
		ClearLocalWarRemainTime();
		g_LocalWarBegin=0;
//		MyLog(0,"Local War Status : %d",g_LocalWarBegin);
	}	
}

void cNation::IncLocalWarfieldManCount(int Index,int NationIndex)
{
	LocalWarfield[Index].IncManCount(NationIndex);
}

void cNation::DecLocalWarfieldManCount(int Index,int NationIndex) 
{ 
	LocalWarfield[Index].DecManCount(NationIndex); 
}

void cNation::SetSealStoneStatus(int NationIndex,int SealNo,int Status, int nHP)			// LTS NEW LOCALWAR
{
	if (!isNationManageServer()) return;
	if (NationIndex<0||NationIndex>3) return;

	MyLog(0,"SealStone Status NationIndex : %d, SealNo : %d, Status : %d, HP : %d",NationIndex,SealNo,Status,nHP); // 030516 kyo

	for (int i=0;i<9;i++)
	{
		if (LocalWarSealStoneStatus[NationIndex][i].SealStoneNo==SealNo)
		{
			LocalWarSealStoneStatus[NationIndex][i].Status=Status;
			LocalWarSealStoneStatus[NationIndex][i].Hp		=nHP; // 030516 kyo
		}
	}

	CalcDefencePoint(NationIndex);

	if (m_LocalWarDefencePoint[NationIndex]==0)			// 해당 나라의 결계석이 모두 깨어졌다면..
	{
		SetLocalWarResult(NationIndex,FALSE);
	}

	MyLog(0,"Nation Index :%d, Defence Point : %d",NationIndex,m_LocalWarDefencePoint[NationIndex]);

	switch(CheckLocalWarResult())						// 시간이 남아있지만... 끝났을때...
	{
	case 0 :	MyLog(0,"LocalWar Nation Was Same Time Defeat!!"); break;
	case 1 :	SetLocalWarStatus(FALSE); break;// 전쟁 종료하고 승리처리한다.
	case 2 :	// 아직 남았다.
	case 3 :	break;// 아직남았다. 
	}


}

int cNation::CheckAllSealStoneBroked(int NationIndex)							// LTS NEW LOCALWAR
{
	if (NationIndex<0||NationIndex>3) return -1;

	for (int i=0;i<9;i++)
	{
		if (LocalWarSealStoneStatus[NationIndex][i].Status!=0)
		{
			return FALSE;
		}
	}
	return TRUE;
}


void cNation::SetLocalWarResult(int NationIndex,int Result)						// LTS NEW LOCALWAR
{
	t_packet packet;
	m_LocalWarResult[NationIndex]=Result;
	packet.h.header.type=CMD_LOCALWAR_NATION_RESULT;
	packet.u.LocalWar.LocalWarResult.NationIndex=NationIndex;
	packet.u.LocalWar.LocalWarResult.Result=Result;
	packet.h.header.size=sizeof(t_LocalWarResult);
	CheckAndFreeLocalWar(NationIndex,Result);
	g_pUserManager->SendPacket(&packet); // CSD-CN-031213
	SendPacket2Maps(&packet);													// 패킷을 다른맵서버에 보낸다.
}

void cNation::CheckSealStoneStatus()											// LTS NEW LOCALWAR
{
	if (!isNationManageServer()) return;

	for (int i=0;i<3;i++)
	{
		if (CheckAllSealStoneBroked(i))
		{
			SetLocalWarResult(i,FALSE);
//			SendNationWarStatus(); 	// 패했다는 정보를 보낸다.
		}
/*		int ReturnValue=CheckAllSealStoneBroked(i);

		switch(ReturnValue)
		{
		//case -1		: MyLog(
		case TRUE	:	SendNationWarStatus(); break;	// 패했다는 정보를 보낸다.
		case FALSE	:
		}*/
	}
}

void SendCMD_LOCALWAR_DEFENCE_POINT(unsigned short Point1,unsigned short Point2,unsigned short Point3)		// LTS NEW LOCALWAR
{	
	t_packet packet;
	packet.h.header.type=CMD_LOCALWAR_DEFENCE_POINT;
	packet.u.LocalWar.LocalWarDefencePoint.Point[0]=Point1;
	packet.u.LocalWar.LocalWarDefencePoint.Point[1]=Point2;
	packet.u.LocalWar.LocalWarDefencePoint.Point[2]=Point3;
	packet.h.header.size=sizeof(t_LocalWarDefencePoint);
	g_pUserManager->SendPacket(&packet); // CSD-CN-031213
	SendPacket2Maps(&packet);
}

void cNation::CalcDefencePoint(int NationIndex)			// LTS NEW LOCALWAR
{
	int AliveCount=0;
	if (NationIndex<0||NationIndex>3) return;

	for (int i=0;i<9;i++)
	{
		if (LocalWarSealStoneStatus[NationIndex][i].Status)
		{
			AliveCount++;
		}
	}

	if (AliveCount>8) m_LocalWarDefencePoint[NationIndex]=100;
	else
	{
		m_LocalWarDefencePoint[NationIndex]=AliveCount*10;
	}

	SendCMD_LOCALWAR_DEFENCE_POINT(m_LocalWarDefencePoint[0],m_LocalWarDefencePoint[1],m_LocalWarDefencePoint[2]);
}

//< LTH-040308-KO 일스의 판정 비정상으로 수정
// 7:앞의 국가승리, 8:뒤의 국가승리, 9:두나라 같음
int cNation::GetPointMAXIndex()
{//0:바이서스 1:자이펀 2:일스 3:바이서스<(자이펀=일스) 4:자이펀<(바이서스=일스) 5:일스<(자이펀=바이서스) 6: 모두동일
	int nIndex=0;
	nIndex= CheckMaxArrayPoint(0,1,m_LocalWarDefencePoint,m_LocalWarPoint,m_nAllLocalWarSealStoneHP);
	if( nIndex == 7 )
	{//바이서스 > 자이펀
		nIndex = CheckMaxArrayPoint(0,2,m_LocalWarDefencePoint,m_LocalWarPoint,m_nAllLocalWarSealStoneHP);
		if( nIndex == 7 )
		{//바이서스 > 자이펀 > 일스
			return 0;	// LTH-040308-KO 직접 수자를 넣었다 바이서스 승리
		}
		else if( nIndex == 8 )
		{//일스 > 바이서스 > 자이펀
			return 2;	// LTH-040308-KO 일스의 승리
		}
		else if( nIndex == 9)
		{//(일스 == 바이서스) > 자이펀
			return 4;
		}

	}
	else if( nIndex == 8 )
	{//자이펀 > 바이서스
		nIndex = CheckMaxArrayPoint(1,2,m_LocalWarDefencePoint,m_LocalWarPoint,m_nAllLocalWarSealStoneHP);
		if( nIndex == 7 )
		{//자이펀 >일스> 바이서스
			return 1;
		}
		else if( nIndex == 8 )
		{//일스 > 자이펀 > 바이서스
			return 2;
		}
		else if( nIndex == 9)
		{//(일스==자이펀) > 바이서스
			return 3;
		}

	}
	else if( nIndex == 9)
	{//바이서스 == 자이펀 
		nIndex = CheckMaxArrayPoint(2,1,m_LocalWarDefencePoint,m_LocalWarPoint,m_nAllLocalWarSealStoneHP);
		if( nIndex == 7 )
		{//일스 > (바이서스 == 자이펀)
			return 2;
		}
		else if( nIndex == 8 )
		{//바이서스 == 자이펀 > 일스
			return 5;
		}
		else if( nIndex == 9)
		{//바이서스 == 자이펀 == 일스
			return 6;
		}
	}
	return nIndex;
}
//> LTH-040308-KO

int cNation::GetPointMAXIndex(unsigned short Point[3])
{
	int Index=0;
	if (Point[0]<=Point[1]) Index=1;
	if (Point[Index]<=Point[2]) Index=2;
	return Index;
}

int cNation::GetPointEqual(int Index,unsigned short Point[3])
{
	int cPoint=Point[Index];
	int count=0;
	for (int i=0;i<3;i++)
	{
		if (i!=Index)
		{
			if (cPoint==Point[i])
			{
				count++;
			}
		}
	}
	return count;
}

int cNation::GetLPointEqual(int Index,unsigned short Point[3],unsigned short DPoint[3])
{
	int cPoint=Point[Index];
	int count=0;
	for (int i=0;i<3;i++)
	{
		if (i!=Index)
		{
			if (DPoint[Index]==DPoint[i])
			{
				if (cPoint==Point[i])
				{
					count++;
				}
			}
		}
	}
	return count;
}

void cNation::GetAllLocalSealStoneHP()
{//모든 결계석의 HP합 // 300509 kyo
	// 030517 kyo
	for( int nNation=0; nNation <= NW_NATION_COUNT; nNation++)
	{
		for( int nStone=0; nStone <= LOCALWAR_SEALSTONE_MAX; nStone++)
		{
			if (LocalWarSealStoneStatus[nNation][nStone].Status)
			{	//< CSD-030804
				m_nAllLocalWarSealStoneHP[nNation] += LocalWarSealStoneStatus[nNation][nStone].Hp;				
			}	//> CSD-030804
		}
	}
}


void cNation::ClearAllLocalSealStoneHP()
{// 030506 kyo
	for( int i=0;i<NW_NATION_COUNT;i++)
	{
		m_nAllLocalWarSealStoneHP[i]=0;
	}
}

int cNation::CheckWinNation()
{
	//1.방어도를 계산해서 높은 국가리턴 
	//2.방어도가 같으면 국가전 점수 높은 국가리턴 
	//3.국가점이 같으면 hp잔여량 계산 높은 국가리턴 

	//<< 030506 kyo
	//ClearAllLocalSealStoneHP(); // 030516 kyo
	GetAllLocalSealStoneHP();

	//leave log
	MyLog(0, "LocalWar End :::: BY DefencePoint = %d, WarPoint = %d, Seal All HP = %d", m_LocalWarDefencePoint[0], m_LocalWarPoint[0],  m_nAllLocalWarSealStoneHP[0] );
	MyLog(0, "LocalWar End :::: ZY DefencePoint = %d, WarPoint = %d, Seal All HP = %d", m_LocalWarDefencePoint[1], m_LocalWarPoint[1],  m_nAllLocalWarSealStoneHP[1] );
	MyLog(0, "LocalWar End :::: YL DefencePoint = %d, WarPoint = %d, Seal All HP = %d", m_LocalWarDefencePoint[2], m_LocalWarPoint[2],  m_nAllLocalWarSealStoneHP[2] );

	int Index=GetPointMAXIndex();
	ClearAllLocalSealStoneHP(); // 030518 kyo
	return Index;
	//>> 030506 kyo

/* 030506 kyo
	int Index=GetPointMAXIndex(m_LocalWarDefencePoint);
	int Count=GetPointEqual(Index,m_LocalWarDefencePoint);
	int LIndex=0;

	switch(Count)
	{
	case 0 : return Index; break; //한 국가가승리했다. 동점없슴.
	case 1 : //2국가의 방어도가 같을때
			 LIndex=GetPointMAXIndex(m_LocalWarPoint);
			 Count=GetLPointEqual(Index,m_LocalWarPoint,m_LocalWarDefencePoint);
			 switch(Count)
			 {
			 case 0 : return LIndex;	break;
			 case 1 : return -1;		break;	
			 }

		break;
	case 2 : //3국가가 다 같을때
			 Index=GetPointMAXIndex(m_LocalWarPoint);
			 //printf("LocalWar Point MAX Index : %d\n",Index);
		     Count=GetPointEqual(Index,m_LocalWarPoint);
			 switch (Count)
			 {
			 case 0 : return Index;		break;
			 case 1 : 
			 case 2 : return -1;		break;
			 }
			 break;
	}
	return Index;	// 여기까지 안온다.
	*/
}

int cNation::CheckLocalWarResult()
{
	int Count=0;
	for (int i=0;i<NW_NATION_COUNT;i++)
	{
		if (m_LocalWarResult[i])
		{
			Count++;
		}
	}
	return Count;
}

void cNation::ClearSealStoneStatus()
{
	for (int i=0;i<NW_NATION_COUNT;i++)
	{
		for (int j=0;j<9;j++)
		{
			LocalWarSealStoneStatus[i][j].Status=TRUE;		
		}
	}
}

int cNation::GetLocalWarDefencePoint(int NationIndex)
{
	if (NationIndex<0||NationIndex>3) return 0; 
	return m_LocalWarDefencePoint[NationIndex];
}

void cNation::GetDayofWeek()
{
	struct tm* NewTime;
	time_t LongTime;
	time(&LongTime);
	NewTime=localtime(&LongTime);
	m_tTimeData.Year=NewTime->tm_year+1900;
	m_tTimeData.Month=NewTime->tm_mon+1;
	m_tTimeData.Day=NewTime->tm_mday;
	m_tTimeData.DayofWeek=NewTime->tm_wday;
}

void cNation::CheckAndActive(int Index)
{
	if (m_tTimeData.DayofWeek!=m_pStartWarTime[Index].DayofWeek)
	{
		return;
	}
	
	if (g_hour!=m_pStartWarTime[Index].DHour)
	{
		return;
	}
	
	int WarfieldNo=m_WarNo%3;

	SERVER_DATA	*pData=NULL;
	pData=g_pServerTable->GetConnectedServerData(WORD(BASE_WARFIELD_PORT+WarfieldNo));
	if (pData==NULL)
	{
		MyLog(0,"SERVER NOT READY PORT : %d, Waitting Next DB Time",BASE_WARFIELD_PORT+WarfieldNo);
		return;
	}

	if (CheckWarStart())
	{
		MyLog(0,"AllReady Start War!! Waitting Next DB Time!!");
		return;
	}

	MyLog(0,"WarNo : %d, %d Warfield War Started by DayofWeek And Time",m_WarNo,WarfieldNo);
	m_VoteData[WarfieldNo]=0;
	SaveWarNo();
	m_WarNo++; 			//전쟁번호를 바꾸어준다.

	t_packet packet;											// 시작하라고 패킷을 보낸다.

	packet.h.header.type=CMD_START_WAR_LOOP;
	packet.h.header.size=0;

	Warfield[WarfieldNo].SetStatus(NW_VOTE);
	SendPacket2Map(&packet,BASE_WARFIELD_PORT+WarfieldNo);

}

void cNation::NewCheckAndActive(int Index)
{
	if (m_tTimeData.DayofWeek!=m_pNewStartWarTime[Index].DayofWeek)
	{
		return;
	}
	
	if (g_hour!=m_pNewStartWarTime[Index].DHour)
	{
		return;
	}

	int WarfieldNo=BASE_NEW_WARFIELD_PORT-BASE_NEW_WARFIELD_PORT;
	SERVER_DATA	*pData=NULL;
	pData=g_pServerTable->GetConnectedServerData(WORD(BASE_NEW_WARFIELD_PORT+WarfieldNo));
	if (pData==NULL)
	{
		MyLog(0,"SERVER NOT READY PORT : %d, Waitting Next DB Time",WarfieldNo+BASE_NEW_WARFIELD_PORT);
		return;
	}

	if (CheckWarStart())
	{
		MyLog(0,"AllReady Start War!! Waitting Next DB Time!!");
		return;
	}


	MyLog(0,"New Warfield Start Nation War by DB");

	t_packet packet;											// 시작하라고 패킷을 보낸다.

	packet.h.header.type=CMD_NWARFIELD_START_WAR;
	packet.u.NationWar.NewWarStart.WarfieldNo=WarfieldNo;
	packet.u.NationWar.NewWarStart.LevelMin=MoveLevelMin[WarfieldNo];
	packet.u.NationWar.NewWarStart.LevelMax=MoveLevelMax[WarfieldNo];
	packet.h.header.size=sizeof(NEWWAR_START);

	SendPacket2Maps(&packet);									// 모든 맵에서 보낸다.
}


void cNation::CheckStartWarfieldWar()
{
	static int oldHour=0;
	if (g_hour==oldHour) 
	{
		return;
	}

	GetDayofWeek();
	oldHour=g_hour;

	int i;
	for (i=0;i<m_iStartWarTimeCount;i++)			// 전쟁시작한다.
	{
		CheckAndActive(i);
	}

	for (i=0;i<m_iNewStartWarTimeCount;i++)
	{
		NewCheckAndActive(i);
	}
}

cWarfield* cNation::GetWarfield(int WarfieldNo)
{
	if (WarfieldNo<0||WarfieldNo>=NW_WARFIELD_COUNT)
		return NULL;

	return &Warfield[WarfieldNo];
}

void cNation::UpdateStatus()
{
	CheckStartWarfieldWar();
}

bool cNation::LoadWarStartupData()				// LTS NEW_NATION_WAR
{	//< CSD-030804
	HSTMT	hStmt=NULL;
	RETCODE	ret;
	char	query_stmt[80]={0,};
	SDWORD	cbValue;

	SQLAllocStmt(hDBC,&hStmt);

	wsprintf(query_stmt,"select count(*) as NumDayHour from WarStartupTBL where Type=0");   

	ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
 
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"LoadWarStartupData(), WarField Table Query Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	ret=SQLFetch(hStmt);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"LoadWarStartupData(), WarField Table Fetch Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	ret=SQLGetData(hStmt,1,SQL_C_SLONG,&m_iStartWarTimeCount,sizeof(int),&cbValue);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
	{
		MyLog(0,"LoadWarStartupData(), Warfield Table SQL Return Error(%d)!!",ret);
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}                

	SQLFreeStmt(hStmt,SQL_DROP);

	m_pStartWarTime=new StartWarTime[m_iStartWarTimeCount];

	SQLAllocStmt(hDBC,&hStmt);

	wsprintf(query_stmt,"select * from WarStartupTBL where Type=0 ORDER BY DayofWeek, DHour");   

	ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
 
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"LoadWarStartupData(), WarField Table Query Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	ret=SQLFetch(hStmt);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"LoadWarStartupData(), WarField Table Fetch Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	MyLog(LOG_NORMAL, "<!-- NATIONWAR Startup Information -->");

	int nIndex = 0, index=0;

	while (ret==SQL_SUCCESS)
	{
		ret=SQLGetData(hStmt,1,SQL_C_SLONG,&nIndex, 0, &cbValue);
		if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
		{
			MyLog(0,"LoadWarStartupData(), Warfield Table SQL Return Error(%d)!!",ret);
			SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}                
		ret=SQLGetData(hStmt,3,SQL_C_SLONG,&m_pStartWarTime[index].DayofWeek,sizeof(int),&cbValue);
		if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
		{
			MyLog(0,"LoadWarStartupData(), Warfield Table SQL Return Error(%d)!!",ret);
			SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}                
		ret=SQLGetData(hStmt,4,SQL_C_SLONG,&m_pStartWarTime[index].DHour,sizeof(int),&cbValue);
		if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
		{
			MyLog(0,"LoadWarStartupData(), Warfield Table SQL Return Error(%d)!!",ret);
			SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}                
		
		MyLog(LOG_NORMAL, "[%d]%s(%02d:%02d - %02d:%02d)", 
			  nIndex,
			  g_pLogManager->ConvertToDay(m_pStartWarTime[index].DayofWeek),
			  m_pStartWarTime[index].DHour, 
			  0, 
			  (m_pStartWarTime[index].DHour + 2)%24, 
			  0);

		++index;
		ret=SQLFetch(hStmt);
	}

    SQLFreeStmt(hStmt,SQL_DROP);
	return true;
}	//> CSD-030804

void cNation::LoadWarNo()
{
	HSTMT	hStmt=NULL;
	RETCODE	ret;
	char	query_stmt[80]={0,};
	SDWORD	cbValue;

	SQLAllocStmt(hDBC,&hStmt);

	wsprintf(query_stmt,"select * From WarStartupTBL where Type=-1");   

	ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
 
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"LoadWarNo(), WarStartupTBL Query Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		m_WarNo=0;
		return;
	}

	ret=SQLFetch(hStmt);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"LoadWarNo(), WarStartupTBL Fetch Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		m_WarNo=0;
		return;
	}

	ret=SQLGetData(hStmt,3,SQL_C_SLONG,&m_WarNo,sizeof(int),&cbValue);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
	{
		MyLog(0,"LoadWarNo(), WarStartupTBL Table SQL Return Error(%d)!!",ret);
		SQLFreeStmt(hStmt,SQL_DROP);
		m_WarNo=0;
		return;
	}                

	m_WarNo++;

	SQLFreeStmt(hStmt,SQL_DROP);
}

bool cNation::LoadNewWarStartupData()				// LTS NEW_NATION_WAR
{	//< CSD-030804
	HSTMT	hStmt=NULL;
	RETCODE	ret;
	char	query_stmt[80]={0,};
	SDWORD	cbValue;

	SQLAllocStmt(hDBC,&hStmt);

	wsprintf(query_stmt,"select count(*) as NumDayHour from WarStartupTBL where Type=1");   

	ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
 
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"LoadWarStartupData(), WarField Table Query Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	ret=SQLFetch(hStmt);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"LoadWarStartupData(), WarField Table Fetch Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	ret=SQLGetData(hStmt,1,SQL_C_SLONG,&m_iNewStartWarTimeCount,sizeof(int),&cbValue);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
	{
		MyLog(0,"LoadWarStartupData(), Warfield Table SQL Return Error(%d)!!",ret);
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}                

	SQLFreeStmt(hStmt,SQL_DROP);

	m_pNewStartWarTime=new StartWarTime[m_iNewStartWarTimeCount];

	SQLAllocStmt(hDBC,&hStmt);

	wsprintf(query_stmt,"select * from WarStartupTBL where Type=1 ORDER BY DayofWeek, DHour");   

	ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
 
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"LoadWarStartupData(), WarField Table Query Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	ret=SQLFetch(hStmt);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"LoadWarStartupData(), WarField Table Fetch Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	MyLog(LOG_NORMAL, "<!-- SNOWYFIELDWAR Startup Information -->");

	int nIndex = 0, index=0;

	while (ret==SQL_SUCCESS)
	{
		ret=SQLGetData(hStmt,1,SQL_C_SLONG,&nIndex, 0, &cbValue);
		ret=SQLGetData(hStmt,3,SQL_C_SLONG,&m_pNewStartWarTime[index].DayofWeek,sizeof(int),&cbValue);
		ret=SQLGetData(hStmt,4,SQL_C_SLONG,&m_pNewStartWarTime[index].DHour,sizeof(int),&cbValue);
		if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
		{
			MyLog(0,"LoadNewWarStartupData(), Warfield Table SQL Return Error(%d)!!",ret);
			SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}

		MyLog(LOG_NORMAL, "[%d]%s(%02d:%02d - %02d:%02d)", 
			  nIndex,
			  g_pLogManager->ConvertToDay(m_pNewStartWarTime[index].DayofWeek),
			  m_pNewStartWarTime[index].DHour, 
			  0, 
			  (m_pNewStartWarTime[index].DHour + 2)%24, 
			  0);

		index++;
		ret=SQLFetch(hStmt);
	}

    SQLFreeStmt(hStmt,SQL_DROP);
	return true;
}	//> CSD-030804

void cNation::SaveWarNo()
{
	t_packet packet;

	packet.h.header.type=CMD_SAVE_WARNO;
	packet.u.NationWar.CommonDataDw.Data=m_WarNo;
	packet.h.header.size=sizeof(t_CommonDataDw);
	QueuePacket(connections, DB_DEMON, &packet, 1);
}

bool cNation::CheckWarStart()
{
	bool WarStart=false;

	MyLog(0,"Warfield Status List __________________");
	MyLog(0,"Warfield : 0, Status : %d",Warfield[0].GetStatus());
	MyLog(0,"Warfield : 1, Status : %d",Warfield[1].GetStatus());
	MyLog(0,"Warfield : 2, Status : %d",Warfield[2].GetStatus());
	MyLog(0,"Warfield Status List End_______________");

	if (Warfield[0].GetStatus()!=NW_PEACE)
	{
		WarStart=true;	
	}
	if (Warfield[1].GetStatus()!=NW_PEACE)
	{
		WarStart=true;
	}
	if (Warfield[2].GetStatus()!=NW_PEACE)
	{
		WarStart=true;
	}

	if (m_NewWarfieldStatus[0]!=NW_PEACE)
	{
		WarStart=true;
	}


	return WarStart;
}

int cNation::GetNewWarfieldStatus(int Index)
{
	if (Index < 0 || Index >= MAX_NEW_WARFIELD)
	{
		return 0;
	}
	return m_NewWarfieldStatus[Index];
}

void cNation::SetNewWarfieldStatus(int Index,int Status)
{
	if (Index<0||Index>=MAX_NEW_WARFIELD)
		return;
	m_NewWarfieldStatus[Index]=Status;
}

bool cNation::InitNewWarfield()
{
	m_NewWarfieldStatus[0]=NW_PEACE;

	return true;
}

void cNation::CheckAndSendWarStart(t_packet* p)
{
	int WarfieldNo=p->u.NationWar.WarLoopTime.WarfieldNo;
	int Status;
	int Port;
	t_packet packet;											// 시작하라고 패킷을 보낸다.
	SERVER_DATA	*pData=NULL;

	switch (WarfieldNo)
	{
	case 0 :
	case 1 :
	case 2 :
			Status=Warfield[WarfieldNo].GetStatus();
			Port=BASE_WARFIELD_PORT+WarfieldNo;
			break;
	case 3 :
			Status=m_NewWarfieldStatus[WarfieldNo-3];
			Port=BASE_NEW_WARFIELD_PORT+WarfieldNo-3;
			break;
	}

	switch(Status)
	{
	case NW_PEACE :
		{
			if (CheckWarStart())
			{ 
				MyLog(0,"ALLREADY IN WAR LOOP!!, Waitting Next WarControl!!");
				return;
			}
			pData=g_pServerTable->GetConnectedServerData((WORD)Port);
			if (pData==NULL)
			{
				MyLog(0,"SERVER NOT READY PORT : %d, Waitting Next WarControl",Port);
				return;
			}
			MyLog(0,"WarNo : %d, %d Warfield War Started by WarControl",m_WarNo,WarfieldNo);
			if (WarfieldNo>=0&&WarfieldNo<=2)
			{
				m_VoteData[WarfieldNo]=0;
				SaveWarNo();
				m_WarNo++; 											//전쟁번호를 바꾸어준다.
			}

			if (WarfieldNo>=0&&WarfieldNo<=2)
			{
				packet.h.header.type=CMD_START_WAR_LOOP;
				packet.h.header.size=0;
				Warfield[WarfieldNo].SetStatus(NW_VOTE);
				SendPacket2Map(&packet,Port);
			}
			else
			{
				packet.h.header.type=CMD_NWARFIELD_START_WAR;
				packet.u.NationWar.NewWarStart.WarfieldNo=WarfieldNo-3;
				packet.u.NationWar.NewWarStart.LevelMin=MoveLevelMin[WarfieldNo-3];
				packet.u.NationWar.NewWarStart.LevelMax=MoveLevelMax[WarfieldNo-3];
				packet.h.header.size=sizeof(NEWWAR_START);
				SendPacket2Maps(&packet);									// 모든 맵에서 보낸다.
				m_NewWarfieldStatus[Port-BASE_NEW_WARFIELD_PORT]=NW_WAR;
			}
			
		}
		break;
	case NW_VOTE :
	case NW_PREPARE :
	case NW_WAR :
	case NW_SOPEN :
		{
			p->h.header.type=CMD_LOOPTIME_CHANGE;
			SendPacket2Map(p,Port);
		}
		break;
	}
}

void cNation::GetWarRemainTime(char* ReturnStr)
{
	GetDayofWeek();
	int WriteCount=0;
	for (int i=0;i<m_iStartWarTimeCount;i++)				// 순서대로 정렬한다.
	{
		if (m_tTimeData.DayofWeek==m_pStartWarTime[i].DayofWeek)
		{
			if (g_hour<=m_pStartWarTime[i].DHour)
			{
				if ((i+1)==m_iStartWarTimeCount)		// 마지막에 위치한경우 
				{
					ReturnStr[0]=m_pStartWarTime[i].DayofWeek;
					ReturnStr[1]=m_pStartWarTime[i].DHour;
					ReturnStr[2]=m_pStartWarTime[0].DayofWeek;
					ReturnStr[3]=m_pStartWarTime[0].DHour;
					return;
				}
				else									// 아닌경우
				{
					ReturnStr[0]=m_pStartWarTime[i].DayofWeek;
					ReturnStr[1]=m_pStartWarTime[i].DHour;
					ReturnStr[2]=m_pStartWarTime[i+1].DayofWeek;
					ReturnStr[3]=m_pStartWarTime[i+1].DHour;
					return;
				}
			}	
		}
		else
			if (m_tTimeData.DayofWeek<m_pStartWarTime[i].DayofWeek)
			{
				if ((i+1)==m_iStartWarTimeCount)		// 마지막에 위치한경우 
				{
					ReturnStr[0]=m_pStartWarTime[i].DayofWeek;
					ReturnStr[1]=m_pStartWarTime[i].DHour;
					ReturnStr[2]=m_pStartWarTime[0].DayofWeek;
					ReturnStr[3]=m_pStartWarTime[0].DHour;
					return;
				}
				else									// 아닌경우
				{
					ReturnStr[0]=m_pStartWarTime[i].DayofWeek;
					ReturnStr[1]=m_pStartWarTime[i].DHour;
					ReturnStr[2]=m_pStartWarTime[i+1].DayofWeek;
					ReturnStr[3]=m_pStartWarTime[i+1].DHour;
					return;
				}
			}
			else											// 작은경우
			{
				if ((i+1)==m_iStartWarTimeCount)				// 끝까지 갔는데두 없으면 
				{
					if (m_iStartWarTimeCount==1)			// 데이터가 한개뿐 없으면 
					{
					ReturnStr[0]=m_pStartWarTime[0].DayofWeek;
					ReturnStr[1]=m_pStartWarTime[0].DHour;
					ReturnStr[2]=9;								// NONE
					ReturnStr[3]=0;								// 대중 없음
					}
					else
					{
						if (m_iStartWarTimeCount>1)
						{
							ReturnStr[0]=m_pStartWarTime[0].DayofWeek;
							ReturnStr[1]=m_pStartWarTime[0].DHour;
							ReturnStr[2]=m_pStartWarTime[1].DayofWeek;
							ReturnStr[3]=m_pStartWarTime[1].DHour;
						}
					}
				}
			}

	}
}

void cNation::GetNewWarRemainTime(char* ReturnStr)
{
	GetDayofWeek();
	int WriteCount=0;
	for (int i=0;i<m_iNewStartWarTimeCount;i++)				// 순서대로 정렬한다.
	{
		if (m_tTimeData.DayofWeek==m_pNewStartWarTime[i].DayofWeek)					// 같은 날이면
		{
			if (g_hour<=m_pNewStartWarTime[i].DHour)								// 이미 지난 전쟁인지 확인
			{
				ReturnStr[0]=m_pNewStartWarTime[i].DayofWeek;
				ReturnStr[1]=m_pNewStartWarTime[i].DHour;
				return;
			}	
		}
		else
		if (m_tTimeData.DayofWeek<m_pNewStartWarTime[i].DayofWeek)				// 높은 날이면 
		{
			ReturnStr[0]=m_pNewStartWarTime[i].DayofWeek;
			ReturnStr[1]=m_pNewStartWarTime[i].DHour;
			return;
		}
		else											// 작은경우
		{
			if ((i+1)==m_iNewStartWarTimeCount)				// 끝까지 갔는데두 없으면 
			{
				ReturnStr[0]=m_pNewStartWarTime[0].DayofWeek;
				ReturnStr[1]=m_pNewStartWarTime[0].DHour;
			}
		}
	}
}

void SendFameChange(LPCHARLIST lpChar,int AddFame)
{
	t_packet packet;
	packet.h.header.type=CMD_FAME_CHANGE;
	packet.u.NationWar.CommonDataDw.Data=AddFame;
	packet.h.header.size=sizeof(t_CommonDataDw);

	QueuePacket(connections,lpChar->GetServerID(),&packet,1);
}
// Class CTeam ___________________________________________________________________________________

CTeam::CTeam()
{
	m_iTeamNo =0; // 030509 kyo
	m_iTeamMemberCount=0; // 030509 koy
	m_iDeathCount=0;
}

CTeam::~CTeam()
{
	Clear();
}

void CTeam::Clear()
{
	SquadMember* lpSquad;
	for (TeamItor itor=m_TeamList.begin();itor!=m_TeamList.end();)
	{
		lpSquad=(*itor++);
		m_TeamList.remove(lpSquad);
		SAFE_DELETE(lpSquad);
	}
	m_iDeathCount=0;      
	m_NationMember[0]=0;
	m_NationMember[1]=0;
	m_NationMember[2]=0;
}

void CTeam::InsertTeamMember(int cn)
{
	SquadMember* lpSquadMember=new SquadMember;
	lpSquadMember->SetData(cn);
	m_TeamList.push_back(lpSquadMember);
	switch (connections[cn].chrlst.name_status.nation)
	{
	case  NW_BY : m_NationMember[0]++; break;
	case  NW_ZY : m_NationMember[1]++; break;
	case  NW_YL : m_NationMember[2]++; break;
	}
	MyLog(0,"%s Join Team %d",connections[cn].name,m_iTeamNo);
	MyLog(0,"List Size : %d",m_TeamList.size());
}

void CTeam::DeleteMember(int cn)
{
	int CheckID;
	SquadMember* lpSquadMember;
	for (TeamItor itor=m_TeamList.begin();itor!=m_TeamList.end();)
	{
		CheckID=(*itor)->GetData();
		if (CheckID==cn)
		{
			switch (connections[cn].chrlst.name_status.nation)
			{
			case  NW_BY : m_NationMember[0]--; break;
			case  NW_ZY : m_NationMember[1]--; break;
			case  NW_YL : m_NationMember[2]--; break;
			}
			lpSquadMember=(*itor);
			m_TeamList.remove(lpSquadMember);
			SAFE_DELETE(lpSquadMember);
			return;
		}
		else
			itor++; 
	}
}

int CTeam::GetTeamCount()
{
	return m_TeamList.size();
}

int CTeam::GetTeamNo()
{
	return m_iTeamNo;
}

void CTeam::SetTeamNo(int TeamNo)
{
	m_iTeamNo=TeamNo;
}

bool CTeam::CheckTeam(int cn)
{
	for (TeamItor itor=m_TeamList.begin();itor!=m_TeamList.end();itor++)
	{
		if (cn==(*itor)->GetData())
		{
			return true;
		}
	}
	return false;
}

// 030624 YGI
// 각자 마을로 살려서 보낸다.
bool MoveToVillage(int server_id)
{	//< CSD-030804
	CHARLIST *ch=CheckServerId( server_id );
	if( !ch ) return false;
	// 030625 YGI
	MoveToHomeTownWithAilve( ch );
	return true;
}	//> CSD-030804

void CTeam::KickUser(int ID)
{	//< CSD-030804
	::MoveToVillage(ID);
}	//> CSD-030804

void CTeam::KickTeamAll()
{
	for (TeamItor itor=m_TeamList.begin();itor!=m_TeamList.end();itor++)
	{
		KickUser((*itor)->GetData());
	}
}

void CTeam::SendMessage(t_packet *p)
{
	int cn;
	for (TeamItor itor=m_TeamList.begin();itor!=m_TeamList.end();itor++)
	{
		cn=(*itor)->GetData();
		QueuePacket(connections,cn,p,1);
	}
}

void CTeam::GiveFame(LPCHARLIST pCaster,LPCHARLIST pTarget,int Range,int AddFame)
{
	int cn;
	DWORD TileRange=(Range*TILE_SIZE)*(Range*TILE_SIZE);
	DWORD TargetRange;
	int   sX=pTarget->X;
	int   sY=pTarget->Y;
	int	  dX,dY;
	LPCHARLIST	lpChar;
	for (TeamItor itor=m_TeamList.begin();itor!=m_TeamList.end();itor++)
	{
		cn=(*itor)->GetData();
		if (pCaster->GetServerID() != cn)
		{
			lpChar=&connections[cn].chrlst;
			dX=lpChar->X;
			dY=lpChar->Y;
			TargetRange=(sX-dX)*(sX-dX)+(sY-dY)*(sY-dY);
			if (TileRange>TargetRange)
			{
				SaveChangeFame(lpChar,lpChar->fame,lpChar->fame+AddFame,LF_NATIONWAR);
				MyLog(0," Fame Change, Name : %s, OldFame : %d, NewFame : %d, Gap : %d",lpChar->Name,lpChar->fame,lpChar->fame+AddFame,AddFame);
				lpChar->fame+=AddFame;
				SendFameChange(lpChar,AddFame);
			}
		}
	}
}

void CTeam::GiveFame2All(int AddFame)
{
	LPCHARLIST	lpChar;
	int cn;
	for (TeamItor itor=m_TeamList.begin();itor!=m_TeamList.end();itor++)
	{
		cn=(*itor)->GetData();
		lpChar=&connections[cn].chrlst;
		SaveChangeFame(lpChar,lpChar->fame,lpChar->fame+AddFame,LF_NATIONWAR);
		MyLog(0," Fame Change, Name : %s, OldFame : %d, NewFame : %d, Gap : %d",lpChar->Name,lpChar->fame,lpChar->fame+AddFame,AddFame);
		lpChar->fame+=AddFame;
		SendFameChange(lpChar,AddFame);
	}
}

int CTeam::GetNationCount(int Nation)
{
	LPCHARLIST lpChar;
	int NationCount=0;
	for (TeamItor itor=m_TeamList.begin();itor!=m_TeamList.end();itor++)
	{
		lpChar=&connections[(*itor)->GetData()].chrlst;
		if (Nation==lpChar->name_status.nation)
		{
			NationCount++;
		}
	}
	return NationCount;
}

int CTeam::GetNationMemberCount(int Nation)
{
	switch (Nation)
	{
	case NW_BY : return m_NationMember[0];	case NW_ZY : return m_NationMember[1];
	case NW_YL : return m_NationMember[2];
	}
	return 0;
}

void CTeam::AddDesthCount()
{
	m_iDeathCount++;
}

int CTeam::GetDeathCount()
{
	return m_iDeathCount;
}

POINT CTeam::GetLivePoint()
{
	return m_LivePoint;
}

void CTeam::RemoveArmor()
{
	int ID;
	for (TeamItor itor=m_TeamList.begin();itor!=m_TeamList.end();itor++)
	{
		ID=(*itor)->GetData();
		CheckAndSendChangeEquip(&connections[ID].chrlst,1);
	}
}

void CTeam::SetLivePoint(POINT lPoint)
{
	m_LivePoint=lPoint;
}

// Class CGuardStone _____________________________________________________________________________

CGuardStone::CGuardStone()
{
	m_pGData		=NULL;
	m_iWarfieldNo	=-1;
	m_iTeamNo		=-1;
}

CGuardStone::~CGuardStone()
{
	Clear();
}

LPGDATA CGuardStone::GetGData(const int nIndex)
{// 030506 kyo
	if( !m_pGData || nIndex < 0	||	nIndex >= m_iGDataCount) 
	{
		return NULL;
	}

	LPGDATA pGData = &m_pGData[nIndex];
	return pGData;
}


void CGuardStone::Clear()
{
	m_iGDataCount	= 0;
	m_iTypeCount[0]	= 0;
	m_iTypeCount[1]	= 0;
	m_iTypeCount[2]	= 0;

	if (m_pGData)
	{
		SAFE_DELETE_ARRAY(m_pGData);
	}
}

void CGuardStone::SetWarfieldNo(int WarfieldNo)
{
	m_iWarfieldNo=WarfieldNo;
}

int CGuardStone::GetWarfieldNo()
{
	return m_iWarfieldNo;
}

void CGuardStone::SetTeamNo(int TeamNo)
{
	m_iTeamNo=TeamNo;
}

int CGuardStone::GetTeamNo()
{
	return m_iTeamNo;
}

int CGuardStone::SetGDataSize()
{
	HSTMT	hStmt=NULL;
	RETCODE	ret;
	SWORD	nClos;
	char	query_stmt[200]={0,};
	SDWORD	cbValue;
	int ReturnValue=0;

	SQLAllocStmt(hDBC,&hStmt);

	wsprintf(query_stmt,"select Count(*) as DataCount from NewWeaponDataTBL where WarfieldNo=%d and TeamNo=%d",m_iWarfieldNo,m_iTeamNo);

	ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
 
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"NewWeaponDataTBL Table Query Error!!");
		return 0;
	}

	SQLNumResultCols(hStmt,&nClos);

	ret=SQLFetch(hStmt);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"NewWeaponDataTBL Table Fetch Error!!");
	    SQLFreeStmt(hStmt,SQL_DROP);
		return 0;
	}

	ret=SQLGetData(hStmt,1,SQL_C_SLONG,&ReturnValue,sizeof(int),&cbValue);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
	{
		MyLog(0,"NewWeaponDataTBL Table SQL Return Error(%d)!!",ret);
	    SQLFreeStmt(hStmt,SQL_DROP);
		return 0;
	}                //WarFieldTBL의 Name

    SQLFreeStmt(hStmt,SQL_DROP);

	return ReturnValue;
}

bool CGuardStone::LoadGData()
{
	HSTMT	hStmt=NULL;
	RETCODE	ret;
	SWORD	nClos;
	char	query_stmt[80]={0,};
	SDWORD	cbValue;

	SQLAllocStmt(hDBC,&hStmt);

	wsprintf(query_stmt,"select * from NewWeaponDataTBL where WarfieldNo=%d and TeamNo=%d Order by NPCType",m_iWarfieldNo,m_iTeamNo);

	ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
 
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"NewWeaponDataTBL Table Query Error!!");
		return false;
	}

	SQLNumResultCols(hStmt,&nClos);

	ret=SQLFetch(hStmt);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"NewWeaponDataTBL Table Fetch Error!!");
	    SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	int index=0;

	while (ret==SQL_SUCCESS)
	{
		ret=SQLGetData(hStmt,4,SQL_C_SLONG,&m_pGData[index].NPCType,sizeof(int),&cbValue);
		ret=SQLGetData(hStmt,5,SQL_C_SLONG,&m_pGData[index].NpcIndex,sizeof(int),&cbValue);
		ret=SQLGetData(hStmt,6,SQL_C_SLONG,&m_pGData[index].GenPosX,sizeof(int),&cbValue);
		ret=SQLGetData(hStmt,7,SQL_C_SLONG,&m_pGData[index].GenPosY,sizeof(int),&cbValue);
		ret=SQLGetData(hStmt,8,SQL_C_SLONG,&m_pGData[index].DontNo,sizeof(int),&cbValue);
		if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
		{
			MyLog(0,"NewWeaponDataTBL Table Fetch Error!!");
			SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}
		index++;
		ret=SQLFetch(hStmt);
	}
    SQLFreeStmt(hStmt,SQL_DROP);
	return true;
}

bool CGuardStone::LoadData()
{
	Clear();
	if (m_iWarfieldNo==-1||m_iTeamNo==-1)
	{
		JustMsg("Class CGuardStone Load Data, WarfieldNo : %d, TeamNo : %d",m_iWarfieldNo,m_iTeamNo);
		return false;
	}

	m_iGDataCount=SetGDataSize();

	if (m_iGDataCount==0)
	{
		JustMsg("Class CGuardStone Load Data, SetGDataSize Error : %d,",m_iGDataCount);
		return false;
	}

	m_pGData=new GDATA[m_iGDataCount];
	if (m_pGData==NULL)
	{
		JustMsg("Class CGuardStone Load Data, m_pGData Memory Alloc Error");
		return false;
	}

	if (!LoadGData())
	{
		JustMsg("Class CGuardStone LoadData, LoadGData() Error");
		return false;
	}

	MakeDontTable();

	return true;
}

bool CGuardStone::MakeGuardStone(const int iIndex, const int iCallType)//030102 lsw //FirstMakeNPCPtr() MakeNPCPtr() 에서만 호출 합니다.
{
	if( iIndex < 0
	||	iIndex >= m_iGDataCount)//인덱스가 이상하다면
	{
		JustMsg("Class CGuardStone,MakeGuardStone() Error, Index Error(%d In = %d Cmp = %d)!!", 
			iCallType,iIndex,m_iGDataCount);
		return false;
	}

	LPGDATA pGData = &m_pGData[iIndex];
	
	pGData->Status = ALIVE_;
	
	const int iGuardStoneId = pGData->NPC_ID;
	const int iGenResult	= ::NPC_Create(	iGuardStoneId,pGData->NpcIndex,
											pGData->GenPosX,pGData->GenPosY,
											0,0,GT_SKILL_SUMMON);
	if (!iGenResult)
	{
		JustMsg("Class CGuardStone,MakeGuardStone() Error, NPC Create Error(%d)!!", iCallType);
		return false;
	}

	CHARLIST *pGuardStone = ::CheckNpcId(iGuardStoneId);
	if(!pGuardStone)
	{
		JustMsg("Class CGuardStone,MakeGuardStone() Error, CheckNpcId(%d, NpcId = %d)!!", iCallType,iGuardStoneId);
		return false;
	}
	//< CSD-030211
	int iRookieNationWarSealStoneHp = (pGuardStone->HpMax*10)/100;  // 021216 kyo 설원전쟁터에서 성문, 수호석, 최후결계석 hp를 기존의 1/10으로

	if (LocalMgr.IsAbleNation(TAIWAN))
	{
		iRookieNationWarSealStoneHp = pGuardStone->HpMax/3;
	}
	//> CSD-030211
	pGuardStone->Hp		= iRookieNationWarSealStoneHp;
	pGuardStone->HpMax	= iRookieNationWarSealStoneHp;
	return true;
}

void CGuardStone::FirstMakeNPCPtr()			
{
	for (int i=0;i<m_iGDataCount;i++)
	{
		m_pGData[i].NPC_ID = ::GetAliveNPCList();
		if (m_pGData[i].NPC_ID==-1)
		{
			JustMsg("Class CGuardStone,FirstMakeNPCPtr() Error, No More NPC Generation!!");
			return;
		}
		if(!MakeGuardStone(i,0))
		{
			return;
		}
	}
}

void CGuardStone::MakeNPCPtr()
{
	for (int i=0;i<m_iGDataCount;i++)
	{
		if(!MakeGuardStone(i,1))
		{
			return;
		}
	}
}

bool CGuardStone::CheckTypeAllBroke(int Type)
{
	int CheckType;
	for (int i=0;i<m_iGDataCount;i++)
	{
		CheckType=m_pGData[i].NPCType;
		if (Type==CheckType)
		{
			if (m_pGData[i].Status==ALIVE_)
			{
				return false;
			}
		}
	}
	return true;
}

bool CGuardStone::CheckNPC(int NpcNo)
{
	for (int i=0;i<m_iGDataCount;i++)
	{
		if (m_pGData[i].NPC_ID==NpcNo)
		{
			return true;
		}
	}
	return false;
}

void SendDontChange(int DontNo,int Status)
{
	t_packet packet;

	packet.h.header.type=CMD_DONT_CHANGE;
	packet.u.NationWar.DontChange.DontNo=DontNo;
	packet.u.NationWar.DontChange.Status=Status;
	packet.h.header.size=sizeof(DONT_CHANGE);
	g_pUserManager->SendPacket(&packet); // CSD-CN-031213
}

void CGuardStone::SendBrokeBBS(int Team,int Type,int No,LPCHARLIST pTarget)
{
	t_packet packet;

	packet.h.header.type=CMD_GUARD_BROKE;
	packet.u.NationWar.GuardBroke.BrokeTeamNo=Team;
	packet.u.NationWar.GuardBroke.Type=Type;
	packet.u.NationWar.GuardBroke.GuardNo=No;
	packet.u.NationWar.GuardBroke.X=pTarget->MoveSx;
	packet.u.NationWar.GuardBroke.Y=pTarget->MoveSy;
	packet.h.header.size=sizeof(GUARD_BROKE);

	SendPacket2Maps(&packet);
	g_pUserManager->SendPacket(&packet); // CSD-CN-031213
}

bool CGuardStone::CheckAndUpdateStatus(LPCHARLIST lpChar)
{
	for (int i=0;i<m_iGDataCount;i++)
	{
		if (m_pGData[i].NPC_ID==lpChar->GetServerID())
		{
			m_pGData[i].Status=0;
			switch(lpChar->SprNo)
			{
			case 64 :		// 최종
				break;
			case 63 :		// 수호석
				SendBrokeBBS(GetTeamNo(),0,i,lpChar);
				break;
			case 65 :		// 성문 
			case 66 :
			case 67 :
				SendBrokeBBS(GetTeamNo(),1,i,lpChar);
				break;
			}
			if (m_pGData[i].DontNo>=0)
			{
				SendDontChange(m_pGData[i].DontNo,m_pGData[i].Status);
			}
			return true;
		}
	}
	return false;
}

void CGuardStone::MakeDontTable()
{
	return;													// 데이터를 만들기 위해서 필요한 코드
	HSTMT	hStmt=NULL;
	RETCODE	ret;
	SWORD	nClos;
	char	query_stmt[80]={0,};
	SDWORD	cbValue;

	SQLAllocStmt(hDBC,&hStmt);

	wsprintf(query_stmt,"SELECT DISTINCT DontNo FROM DontTable");

	ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
 
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"MakeDontTable(), DontTable Query Error!!");
		return;
	}

	SQLNumResultCols(hStmt,&nClos);

	ret=SQLFetch(hStmt);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"MakeDontTable(), DontTable Fetch Error!!");
	    SQLFreeStmt(hStmt,SQL_DROP);
		return;
	}

	int Size=0;

	while (ret==SQL_SUCCESS)
	{
		if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
		{
			MyLog(0,"MakeDontTable(), DontTable Fetch Error!!");
			SQLFreeStmt(hStmt,SQL_DROP);
			return;
		}
		Size++;
		ret=SQLFetch(hStmt);
	}
    SQLFreeStmt(hStmt,SQL_DROP);

	MyLog(0,"Size : %d",Size);

	int *FieldSize;

	POINT	** lpPoint;

	lpPoint=new POINT*[Size];
	FieldSize=new int[Size];

	int i;
	for (i=0;i<Size;i++)
	{
		SQLAllocStmt(hDBC,&hStmt);
		wsprintf(query_stmt,"SELECT   COUNT(*) AS fieldcount FROM DontTable WHERE DontNo=%d",i);
		
		ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
		ret=SQLFetch(hStmt);
		ret=SQLGetData(hStmt,1,SQL_C_SLONG,&FieldSize[i],sizeof(int),&cbValue);
		if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
		{
			MyLog(0,"MakeDontTable(), DontTable Fetch Error!!");
			SQLFreeStmt(hStmt,SQL_DROP);
			return;
		}
	    SQLFreeStmt(hStmt,SQL_DROP);

		lpPoint[i]=new POINT[FieldSize[i]];

		SQLAllocStmt(hDBC,&hStmt);
		wsprintf(query_stmt,"SELECT * FROM DontTable WHERE DontNo=%d",i);
		ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
		ret=SQLFetch(hStmt);
		int Index=0;
		while (ret==SQL_SUCCESS)
		{
			ret=SQLGetData(hStmt,3,SQL_C_SLONG,&lpPoint[i][Index].x,sizeof(int),&cbValue);
			ret=SQLGetData(hStmt,4,SQL_C_SLONG,&lpPoint[i][Index].y,sizeof(int),&cbValue);
			if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
			{
				MyLog(0,"MakeDontTable(), DontTable Fetch Error!!");
				SQLFreeStmt(hStmt,SQL_DROP);
				return;
			}
			Index++;
			ret=SQLFetch(hStmt);
		}
	    SQLFreeStmt(hStmt,SQL_DROP);
	}

	FILE* fp=fopen("Output/LTS000.Dat","wb");
	if (fp)
	{
		fwrite(&Size,sizeof(int),1,fp);
		for (i=0;i<Size;i++)
		{
			fwrite(&FieldSize[i],sizeof(int),1,fp);
			fwrite(lpPoint[i],sizeof(POINT),FieldSize[i],fp);
		}
		fclose(fp);

	}

	for (i=0;i<Size;i++)
	{
		SAFE_DELETE_ARRAY(lpPoint[i]);
	}
	SAFE_DELETE_ARRAY(lpPoint);
	SAFE_DELETE_ARRAY(FieldSize);
}

void CGuardStone::GetTileDont(DONT_DATA* lpDontData)
{
	int Size=0;
	for (int i=0;i<m_iGDataCount;i++)
	{
		if (m_pGData[i].DontNo>=0)
		{
			lpDontData->DontNo[Size]=m_pGData[i].DontNo;
			lpDontData->Status[Size++]=m_pGData[i].Status;
		}
	}
	lpDontData->Size=Size;
}

void CGuardStone::GetStatus(char* lpStatus)				// 무리가 있다.  // 정해져 있다는 가정
{
	for (int i=0;i<m_iGDataCount;i++)
	{
		lpStatus[i]=m_pGData[i].Status;
	}
}

// Class CNewWarfield ____________________________________________________________________________

CNewWarfield::CNewWarfield()
{
	m_pTeam			= NULL;
	m_pGuard		= NULL;
	m_iStatus		= WARFIELD_PEACE;
	m_64LoopTime	= 0;
}

CNewWarfield::~CNewWarfield()
{
	TeamClear();
	GuardClear();
	DeleteAllMonsterData();
}

bool CNewWarfield::InitWarfield()
{
	m_iWarfieldNo=g_wMapServerPort-BASE_NEW_WARFIELD_PORT;
	SetTeamCount(2);
	m_pTeam[0].SetTeamNo(0);
	m_pTeam[1].SetTeamNo(1);
	m_pGuard[0].SetWarfieldNo(0);
	m_pGuard[0].SetTeamNo(0);
	m_pGuard[0].FirstMakeNPCPtr();
	m_pGuard[1].SetWarfieldNo(0);
	m_pGuard[1].SetTeamNo(1);
	m_pGuard[1].FirstMakeNPCPtr();
	LoadTimeData();
	SetWarfieldStatus(NW_PEACE);
	if (!LoadWarfieldData())
	{
		return false;
	}
	if (!LoadSMonster(m_iWarfieldNo))
	{
		return false;
	}
	return true;
}

long CNewWarfield::GetAllGDataTypeHP(const int nTeamNum , const int nType)
{// 030506 kyo 
	long lAllHP=0;
	for (int i=0;i<m_pGuard[nTeamNum].GetCount();i++)
	{//모든 결계에관한 것들중에서
		if( m_pGuard[nTeamNum].GetGData(i) )
		{//리스트에 존재하며
			if( nType == m_pGuard[nTeamNum].GetGData(i)->NPCType )
			{//해당타입인것의
				CHARLIST *pGuardStone = ::CheckNpcId(m_pGuard[nTeamNum].GetGData(i)->NPC_ID) ;
				if( pGuardStone )
				{//모든 HP계산
					lAllHP += pGuardStone->Hp;
				}
			}
		}
	}
	return lAllHP;
}


bool CNewWarfield::LoadWarfieldData()
{
	HSTMT	hStmt=NULL;
	RETCODE	ret;
	char	query_stmt[80]={0,};
	SDWORD	cbValue;

	SQLAllocStmt(hDBC,&hStmt);

	wsprintf(query_stmt,"select * from WarFieldTBL where Port=%d order by WarfieldCode",m_iWarfieldNo+BASE_NEW_WARFIELD_PORT);

	ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
 
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		JustMsg("CNewWarfield::LoadWarfieldData(),WarField Table Query Error!!");
		return false;
	}

	ret=SQLFetch(hStmt);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		JustMsg("CNewWarfield::LoadWarfieldData(),WarField Table Query Error!!");
	    SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	POINT	Team[2];
	ret=SQLGetData(hStmt,7,SQL_C_SLONG,&Team[0].x,sizeof(DWORD),&cbValue);
	ret=SQLGetData(hStmt,8,SQL_C_SLONG,&Team[0].y,sizeof(DWORD),&cbValue);
	ret=SQLGetData(hStmt,9,SQL_C_SLONG,&Team[1].x,sizeof(DWORD),&cbValue);
	ret=SQLGetData(hStmt,10,SQL_C_SLONG,&Team[1].y,sizeof(DWORD),&cbValue);

	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		JustMsg("CNewWarfield::LoadWarfieldData(),WarField Table Query Error!!");
	    SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	m_pTeam[0].SetLivePoint(Team[0]);
	m_pTeam[1].SetLivePoint(Team[1]);

    SQLFreeStmt(hStmt,SQL_DROP);

	return true;
}

bool CNewWarfield::LoadSMonster(WORD WarfieldNo)
{
	HSTMT	hStmt=NULL;
	RETCODE	ret;
	char	query_stmt[80]={0,};
	SDWORD	cbValue;
	t_WeaponStruct Monster;

	SQLAllocStmt(hDBC,&hStmt);

	wsprintf(query_stmt,"select * from WarfieldMonsterTBL where WarfieldCode=%d order by [index]",WarfieldNo+3);   

	ret=SQLExecDirect(hStmt,(UCHAR*)query_stmt,SQL_NTS);
 
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"WarFieldMonster Table Table Query Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}


	ret=SQLFetch(hStmt);
	if (ret!=SQL_SUCCESS_WITH_INFO && ret !=SQL_SUCCESS)
	{
		MyLog(0,"WarFieldMonster Table Fetch Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	while (ret==SQL_SUCCESS)
	{
		Monster.NPC_ID=0;
		ret=SQLGetData(hStmt,1,SQL_C_SLONG,&Monster.MonPointIndex,sizeof(int),&cbValue);
		ret=SQLGetData(hStmt,3,SQL_C_SLONG,&Monster.NPC_Index,sizeof(int),&cbValue);
		ret=SQLGetData(hStmt,4,SQL_C_SLONG,&Monster.Locationx,sizeof(int),&cbValue);
		ret=SQLGetData(hStmt,5,SQL_C_SLONG,&Monster.Locationy,sizeof(int),&cbValue);
		ret=SQLGetData(hStmt,6,SQL_C_SLONG,&Monster.EventNo,sizeof(int),&cbValue);
		ret=SQLGetData(hStmt,7,SQL_C_SLONG,&Monster.GenerationPos,sizeof(int),&cbValue);
		ret=SQLGetData(hStmt,8,SQL_C_SLONG,&Monster.GenerationType,sizeof(int),&cbValue);
		if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
		{
			MyLog(0,"WarfieldMonster Table SQL Return Error(%d)!!",ret);
		    SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}       
		
		InsertMonsterData(Monster);		// Monster List에 포함 시킨다.

		ret=SQLFetch(hStmt);
	}
    SQLFreeStmt(hStmt,SQL_DROP);
	return true;
}

void CNewWarfield::TeamClear()
{
	SAFE_DELETE_ARRAY(m_pTeam);

	m_iTeamCount=0;
}

void CNewWarfield::GuardClear()
{
	SAFE_DELETE_ARRAY(m_pGuard);
}

void CNewWarfield::SetTeamCount(int Count)
{
	TeamClear();
	GuardClear();

	m_iTeamCount=Count;

	m_pTeam=new CTeam[Count];
	m_pGuard=new CGuardStone[Count];

	for (int i=0;i<Count;i++)		// InitData()
	{
		m_pTeam[i].SetTeamNo(i);
		m_pGuard[i].SetWarfieldNo(m_iWarfieldNo);
		m_pGuard[i].SetTeamNo(i);
		m_pGuard[i].LoadData();
	}
}

LPTEAM CNewWarfield::GetTeam(int TeamNo)
{
	if (TeamNo<0&&TeamNo>=m_iTeamCount)
		return NULL;

	return &m_pTeam[TeamNo];
}

LPGUARDSTONE CNewWarfield::GetGuard(int TeamNo)
{
	if (TeamNo<0&&TeamNo>=m_iTeamCount)
		return NULL;

	return &m_pGuard[TeamNo];
}

void CNewWarfield::LoadTimeData()
{
/*	char	NetworkDir[MAX_PATH];
	char	MapServerConfigFileName[MAX_PATH];

	if (GetPrivateProfileString("Network","path","",NetworkDir,MAX_PATH,MAP_SERVER_INI_)<=0)
	{
		JustMsg("Error!! Check Mapserver.ini's Path Set.");
		return ;
	}

	sprintf(MapServerConfigFileName,"%s/data/MapServerConfig.ini",NetworkDir);
*/// 030919 HK YGI

	m_Time.WarTime=(DWORD)GetPrivateProfileInt("WarLoopDefine","WarTime",7200,MapServerConfigFileName);		// 전쟁시간
	m_Time.SOpenTime=(DWORD)GetPrivateProfileInt("WarLoopDefine","BonusTime",3600,MapServerConfigFileName);		// 비밀지역 열리는 시간

	MemberMAX[0]=(DWORD)GetPrivateProfileInt("NewWarMove","BYJoinMax",50,MapServerConfigFileName);
	MemberMAX[1]=(DWORD)GetPrivateProfileInt("NewWarMove","ZYJoinMax",50,MapServerConfigFileName);
	MemberMAX[2]=(DWORD)GetPrivateProfileInt("NewWarMove","YLJoinMax",50,MapServerConfigFileName);
}

void SendNewWarEnd()
{
	t_packet packet;

	packet.h.header.type=CMD_NWARFIELD_END_WAR;
	packet.h.header.size=0;

	SendPacket2Maps(&packet);
	g_pUserManager->SendPacket(&packet); // CSD-CN-031213
}

void CNewWarfield::SetWarfieldStatus(int Status)			// Packet Concern....
{
	switch(Status)
	{
	case NW_PEACE	:	
		{
			m_pGuard[0].MakeNPCPtr(); 
			m_pGuard[1].MakeNPCPtr();
			m_pTeam[0].Clear();
			m_pTeam[1].Clear();
		}
		break;
	case NW_WAR		:	
		{
			m_64LoopTime=g_curr_time+m_Time.WarTime;
			m_pGuard[0].MakeNPCPtr(); 
			m_pGuard[1].MakeNPCPtr();
			KickAllUser2Home();
		}
		break;
	case NW_SOPEN	: 
		{
			if (m_iStatus==NW_WAR)
			{
				SendNewWarEnd();// End Process
				m_pTeam[0].RemoveArmor();
				m_pTeam[1].RemoveArmor();
				MakeMonster();
			}
			m_64LoopTime=g_WarTime+m_Time.SOpenTime; break;
		}
	}
	m_iStatus=Status;
	t_packet packet; 
	packet.h.header.type=CMD_NWARFIELD_STATUS_CHANGE;
	packet.u.NationWar.WarfieldStatusChange.WarfieldNo=m_iWarfieldNo;
	packet.u.NationWar.WarfieldStatusChange.Status=Status;
	packet.h.header.size=sizeof(t_WarfieldStatusChange) ;
	SendPacket2Maps(&packet);
}

int CNewWarfield::GetWarfieldStatus()
{
	return m_iStatus;
}

void CNewWarfield::UpdatePeace()
{
}

void CNewWarfield::UpdateWar()
{
	int RemainTime = m_64LoopTime - g_WarTime;
	
	if (RemainTime%NW_REMAIN_TIME_UPDATE == 0) 
	{
		//SendCMD_UPDATE_WARTIME(m_iWarfieldNo + 3, m_iStatus, RemainTime);
		SendCMD_UPDATE_WARTIME(m_iWarfieldNo + 3, m_iStatus, GetRemainTime()); // 030520 kyo
	}

	if (RemainTime < 0)
	{
		SetWarfieldStatus(NW_SOPEN);
	
		CheckWinTeam();
		/*kyo
		if (m_pTeam[0].GetTeamCount()>=m_pTeam[1].GetTeamCount())
		{
			SetWinTeam(1);
			m_pTeam[0].GiveFame2All(2);
			m_pTeam[1].GiveFame2All(5);
		}
		else
		{
			SetWinTeam(0);
			m_pTeam[1].GiveFame2All(2);
			m_pTeam[0].GiveFame2All(5);
		}
		*/
		SendWarResult();
		return;
	}

	if (m_pGuard[0].CheckTypeAllBroke(0))
	{
		SetWinTeam(1);
		SetWarfieldStatus(NW_SOPEN);
		m_pTeam[0].GiveFame2All(2);
		m_pTeam[1].GiveFame2All(5);
		SendWarResult();
		// m_pTeam[1].KickTeamAll();
		// BroadCast.....
	}

	if (m_pGuard[1].CheckTypeAllBroke(0))
	{
		SetWinTeam(0);
		SetWarfieldStatus(NW_SOPEN);
		m_pTeam[1].GiveFame2All(2);
		m_pTeam[0].GiveFame2All(5);
		SendWarResult();
		// m_pTeam[0].KickTeamAll();
		// BroadCast.....
	}
	// Update War Status
	// Check Killed SealStone.... And GateWay

}

void CNewWarfield::GetAllGuardsHP()
{// 030506 kyo
	ClearFinalGStoneHP();
	ClearAllNormalGStoneHP();
	ClearAllGatelHP();
	SetFinalGStoneHP( 0, GetAllGDataTypeHP(0,0) );
	SetFinalGStoneHP( 1, GetAllGDataTypeHP(1,0) );
	SetAllNormalGStoneHP(0, GetAllGDataTypeHP(0,1) );
	SetAllNormalGStoneHP(1, GetAllGDataTypeHP(1,1) );
	SetAllGatelHP(0, GetAllGDataTypeHP(0,2) );
	SetAllGatelHP(1, GetAllGDataTypeHP(1,2) );

	MyLog(0,"NewWarField End:: Team_0 FinalHP = %d, AllGStoneHP = %d, AllGateHP = %d",GetFinalGStoneHP(0),GetAllNormalGStoneHP(0),GetAllGatelHP(0) );
	MyLog(0,"NewWarField End:: Team_1 FinalHP = %d, AllGStoneHP = %d, AllGateHP = %d",GetFinalGStoneHP(1),GetAllNormalGStoneHP(1),GetAllGatelHP(1) );
}

void CNewWarfield::CheckWinTeam()
{// 030506 kyo
	GetAllGuardsHP();
	//int nIndex = CheckMaxPoint<long>( GetFinalGStoneHP(0), GetFinalGStoneHP(1) );
	int nIndex = CheckMaxPoint( GetFinalGStoneHP(0), GetFinalGStoneHP(1) );
	if( nIndex == 0 )
	{//0팀승리
		goto WIN_TEAM_0;
	}
	else if( nIndex == 1)
	{
		goto WIN_TEAM_1;
	}
	else if( nIndex  == 2 )
	{
		//nIndex = CheckMaxPoint<long>( GetAllNormalGStoneHP(0), GetAllNormalGStoneHP(1) ); 
		nIndex = CheckMaxPoint( GetAllNormalGStoneHP(0), GetAllNormalGStoneHP(1) ); 
		if( nIndex == 0 )
		{
			goto WIN_TEAM_0;
		}
		else if( nIndex ==1 )
		{
			goto WIN_TEAM_1;
		}
		else if( nIndex ==2 )
		{
			//nIndex = CheckMaxPoint<long>( GetAllNormalGStoneHP(0), GetAllNormalGStoneHP(1) ); 
			nIndex = CheckMaxPoint( GetAllGatelHP(0), GetAllGatelHP(1) ); 
			if( nIndex == 0 )
			{
				goto WIN_TEAM_0;
			}
			else if( nIndex ==1 )
			{
				goto WIN_TEAM_1;
			}
			else 
			{//모든 경우가 같은 경우
				goto WIN_TEAM_DEFAULT;
			}	
		}
	}

WIN_TEAM_0:
	SetWinTeam(0);
	m_pTeam[0].GiveFame2All(5);
	m_pTeam[1].GiveFame2All(2);
	MyLog(0,"NewWarField END:: Win Team team_0");// 030509 kyo 
	return;
WIN_TEAM_1:
	SetWinTeam(1);
	m_pTeam[0].GiveFame2All(2);
	m_pTeam[1].GiveFame2All(5);
	MyLog(0,"NewWarField END:: Win Team team_1");// 030509 kyo 
	return;
WIN_TEAM_DEFAULT:
	SetWinTeam(2);
	m_pTeam[0].GiveFame2All(2);
	m_pTeam[1].GiveFame2All(2);
	MyLog(0,"NewWarField END:: Win Team NOT");// 030509 kyo 
	return;
}

void CNewWarfield::ClearFinalGStoneHP()
{	
	for(int i=0;i<2/*m_iTeamCount*/;i++)
	{
		m_nFinalGStoneHP[i]=0;
	}	
}

void CNewWarfield::ClearAllNormalGStoneHP()
{	
	for(int i=0;i<2/*m_iTeamCount*/;i++)
	{
		m_nAllNormalGStoneHP[i]=0;
	}	
}

void CNewWarfield::ClearAllGatelHP()
{	
	for(int i=0;i<2/*m_iTeamCount*/;i++)
	{
		m_nAllGatelHP[i]=0;
	}	
}

long CNewWarfield::GetFinalGStoneHP(const int nTeamNum )
{
	return m_nFinalGStoneHP[nTeamNum];
}

long CNewWarfield::GetAllNormalGStoneHP(const int nTeamNum )
{
	return m_nAllNormalGStoneHP[nTeamNum];
}

long CNewWarfield::GetAllGatelHP(const int nTeamNum )
{
	return m_nAllGatelHP[nTeamNum ];
}

void CNewWarfield::SetFinalGStoneHP(const int nTeamNum, long lAllHP )	
{
	m_nFinalGStoneHP[nTeamNum] = lAllHP;
}

void CNewWarfield::SetAllNormalGStoneHP(const int nTeamNum, long lAllHP )
{
	m_nAllNormalGStoneHP[nTeamNum] = lAllHP;
}

void CNewWarfield::SetAllGatelHP(const int nTeamNum, long lAllHP )	
{
	m_nAllGatelHP[nTeamNum] = lAllHP;
}


void CNewWarfield::UpdateSOpen()
{
	int RemainTime=m_64LoopTime-g_WarTime;
	if (RemainTime<0)
	{
		SetWarfieldStatus(NW_PEACE);
		return;
	}
}

void CNewWarfield::Update()
{
	UpdateWarTime();

	switch (m_iStatus)
	{
	case NW_PEACE: UpdatePeace(); break;
	case NW_WAR:   UpdateWar();   break;
	case NW_SOPEN: UpdateSOpen(); break;
	}
}

void CNewWarfield::KickAllUser2Home()
{
	for( int i = DRAGON_CONNECTIONS_START; i<DRAGON_MAX_CONNECTIONS; i++ )
	{
		if (CheckServerId(i))
		{
			switch (connections[i].chrlst.name_status.nation)
			{
			case NW_BY : MapMove(i,"MA-IN",245,115);		break;
			case NW_ZY : MapMove(i,"RENES_C",210,175);	break;
			case NW_YL : MapMove(i,"BARANTAN",325,98);	break;
			}
		}
	}
}

int CNewWarfield::GetTeamNo(LPCHARLIST lpChar)
{
/*	if (m_pTeam[0].CheckTeam(cn))					// 정규화, 속도저하
		return 0;
	if (m_pTeam[1].CheckTeam(cn))
		return 1;*/
	switch(lpChar->name_status.nation)
	{
	case NW_BY :	return 0;
	case NW_ZY :	return 1;
	case NW_YL :
		if (lpChar->NWCharacter.YL_JoinNation==NW_BY)
			return 0;
		else
			return 1;
	}
	MyLog(0,"CNewWarfield GetTeamNo(),illegal Nation : %d, SprNo : %d",lpChar->SprNo,lpChar->name_status.nation);
	return 1;
}

bool CNewWarfield::isMyTeam(LPCHARLIST lpChar,int NPCNo)
{
	int TeamNo=GetTeamNo(lpChar);
	return m_pGuard[TeamNo].CheckNPC(NPCNo);
}

bool isMyTeam(LPCHARLIST pCaster,LPCHARLIST pTarget)			// CBattleMgr::IsColleague
{
	int pCasterNation=pCaster->name_status.nation;
	int pTargetNation=pTarget->name_status.nation;

	switch(pCasterNation)
	{
	case NW_BY :
		switch(pTargetNation)
		{
		case NW_BY : return true;
		case NW_ZY : return false;
		case NW_YL :
			if (pTarget->NWCharacter.YL_JoinNation==NW_BY)
				return true;
			else 
				return false;
		}
		break;
	case NW_ZY :
		switch(pTargetNation)
		{
		case NW_BY : return false;
		case NW_ZY : return true;
		case NW_YL :
			if (pTarget->NWCharacter.YL_JoinNation==NW_BY)
				return false;
			else 
				return true;
		}
	case NW_YL :
		switch (pCaster->NWCharacter.YL_JoinNation)
		{
		case NW_BY :
			switch(pTargetNation)
			{
			case NW_BY : return true;
			case NW_ZY : return false;
			case NW_YL :
				if (pTarget->NWCharacter.YL_JoinNation==NW_BY)
					return true;
				else 
					return false;
			}
		case NW_ZY :
			switch(pTargetNation)
			{
			case NW_BY : return false;
			case NW_ZY : return true;
			case NW_YL :
				if (pTarget->NWCharacter.YL_JoinNation==NW_BY)
					return false;
				else 
					return true;
			}
		}
	}

	MyLog(0,"CTeam isMyTeam(), illegal Nation pCaster : %d,pTarget : %d",pCasterNation,pTargetNation);
	return false;
}

bool CNewWarfield::CanAttackGuard(LPCHARLIST pCaster,LPCHARLIST pTarget)
{
	int CasterTeamNo=GetTeamNo(pCaster); 
	int TargetTeamNo;
	if (m_pGuard[0].CheckNPC(pTarget->GetServerID()))
		TargetTeamNo=0;
	if (m_pGuard[1].CheckNPC(pTarget->GetServerID())) 
		TargetTeamNo=1;

	if (CasterTeamNo==TargetTeamNo)		// 같은 팀이다.. 
		return false;

	switch (pTarget->SprNo)
	{
	case 64 :
		return m_pGuard[TargetTeamNo].CheckTypeAllBroke(1);
	case 63 :
		return true;
	case 67 :
		return true;
	}

	MyLog(0,"CNewWarfield CanAttackGuard(), illegal SprNo : %d",pTarget->SprNo);
	return false;
}

void CNewWarfield::SetWinTeam(int TeamNo)
{
	m_iWinTeam=TeamNo;
}

int CNewWarfield::GetWinTeam()
{
	return m_iWinTeam;
}

void CNewWarfield::GiveFame(int TeamNo,LPCHARLIST pCaster,LPCHARLIST pTarget)
{
	int Range=GIVE_FAME_RANGE;
	switch (pTarget->SprNo)
	{
	case 64 :	
		SaveChangeFame(pCaster,pCaster->fame,pCaster->fame+20,LF_NATIONWAR);
		pCaster->fame+=20; 	
		SendFameChange(pCaster,20);
		m_pTeam[TeamNo].GiveFame(pCaster,pTarget,Range,4);
		break;
	case 63 :
		SaveChangeFame(pCaster,pCaster->fame,pCaster->fame+15,LF_NATIONWAR);
		pCaster->fame+=15; 									
		SendFameChange(pCaster,15);
		m_pTeam[TeamNo].GiveFame(pCaster,pTarget,Range,3);
		break;
	case 65 :
	case 66 :
	case 67 :
		SaveChangeFame(pCaster,pCaster->fame,pCaster->fame+10,LF_NATIONWAR);
		pCaster->fame+=10; 									
		SendFameChange(pCaster,10);
		m_pTeam[TeamNo].GiveFame(pCaster,pTarget,Range,3);
		break;
	}
}

void SendGuardKiller(LPCHARLIST pCaster,LPCHARLIST pTarget)
{
	t_packet packet;

	packet.h.header.type=CMD_GUARD_KILLER;
	packet.u.NationWar.GuardKiller.KillerID=pCaster->GetServerID();
	packet.u.NationWar.GuardKiller.GuardID=pTarget->GetServerID();
	packet.h.header.size=sizeof(GUARD_KILLER);

	QueuePacket(connections,pCaster->GetServerID(),&packet,1);
	CastMe2Other(pCaster->GetServerID(),&packet);
}

void CNewWarfield::UpdateGuardStatus(LPCHARLIST pCaster,LPCHARLIST pTarget)
{
	if (m_pGuard[0].CheckAndUpdateStatus(pTarget))
	{	
		if (pCaster==NULL)
			return;
		GiveFame(1,pCaster,pTarget);
		SendGuardKiller(pCaster,pTarget);
		return;
	}
	if (m_pGuard[1].CheckAndUpdateStatus(pTarget))
	{
		if (pCaster==NULL)
			return;
		GiveFame(0,pCaster,pTarget);
		SendGuardKiller(pCaster,pTarget);
		return;
	}
	MyLog(0,"CNewWarfield UpdateGuardStoneStatus() Error!!, NPC Index : %d,SprNo : %d",pTarget->GetServerID(),pTarget->SprNo);
}

void UpdateGuardStoneStatus(LPCHARLIST pCaster,LPCHARLIST pTarget)				// Kill Character Call
{
	if (!isNewWarfieldServer())
		return;

	g_pNewWarfield->UpdateGuardStatus(pCaster,pTarget);
}

void CNewWarfield::CountDeath(LPCHARLIST lpChar)
{
	int TeamNo=GetTeamNo(lpChar);
	m_pTeam[TeamNo].AddDesthCount();
}

void CountNewWarfieldDeath(LPCHARLIST Attacker,LPCHARLIST Defencer)
{
	if (!isNewWarfieldServer())
		return;

	g_pNewWarfield->CountDeath(Defencer);
}

void CNewWarfield::SendWarResult()
{
	t_packet packet;

	packet.h.header.type=CMD_WAR_RESULT;
	packet.u.NationWar.WarResult.WarfieldPort=BASE_NEW_WARFIELD_PORT+m_iWarfieldNo;
	packet.u.NationWar.WarResult.VictoryTeam=m_iWinTeam;
	packet.u.NationWar.WarResult.BYCount=m_pTeam[0].GetNationCount(NW_BY);
	packet.u.NationWar.WarResult.YLBYCount=m_pTeam[0].GetNationCount(NW_YL);
	packet.u.NationWar.WarResult.ZYCount=m_pTeam[1].GetNationCount(NW_ZY);
	packet.u.NationWar.WarResult.YLZYCount=m_pTeam[1].GetNationCount(NW_YL);
	packet.u.NationWar.WarResult.BYDeathCount=m_pTeam[0].GetDeathCount();
	packet.u.NationWar.WarResult.ZYDeathCount=m_pTeam[1].GetDeathCount();
	packet.h.header.size=sizeof(WAR_RESULT);

	SendPacket2Maps(&packet);
	g_pUserManager->SendPacket(&packet); // CSD-CN-031213
}

void CNewWarfield::CheckAndKickUser(int cn)
{	//< CSD-030804
	CHARLIST *ch = CheckServerId( cn );
	
	if (ch == NULL)
	{	
		return; 	
	}
	
	const int TeamNo = GetTeamNo(ch);
	
	if (m_iWinTeam != TeamNo)
	{
		m_pTeam[TeamNo].KickUser(cn);
	}
}	//> CSD-030804

void CNewWarfield::GetTileDont(DONT_DATA* lpDontData)
{
	DONT_DATA	TeamData[2];
	int i,TotalSize,Index;
	m_pGuard[0].GetTileDont(&TeamData[0]);
	m_pGuard[1].GetTileDont(&TeamData[1]);

	TotalSize=lpDontData->Size=TeamData[0].Size+TeamData[1].Size;
	
	Index=0;
	for (i=0;i<TeamData[0].Size;i++)
	{
		lpDontData->DontNo[Index]=TeamData[0].DontNo[i];
		lpDontData->Status[Index]=TeamData[0].Status[i];
		Index++;
	}
	for (i=0;i<TeamData[1].Size;i++)
	{
		lpDontData->DontNo[Index]=TeamData[1].DontNo[i];
		lpDontData->Status[Index]=TeamData[1].Status[i];
		Index++;
	}
}

void CNewWarfield::DeleteMember(int cn)
{
	int TeamNo=GetTeamNo(&connections[cn].chrlst);

	m_pTeam[TeamNo].DeleteMember(cn);

	MyLog(0,"Remove Team Member Team : %d, ID : %d",TeamNo,cn);
}

void CNewWarfield::LoopTimeChange(t_packet* p)
{
	m_64LoopTime=g_WarTime+p->u.NationWar.WarLoopTime.LoopTime;	
	MyLog(0,"LoopTime Change : %d",p->u.NationWar.WarLoopTime.LoopTime);
}

void CNewWarfield::SendSquadChat(LPCHARLIST lpChar,t_packet* p)
{
	int TeamNo=GetTeamNo(lpChar);

	m_pTeam[TeamNo].SendMessage(p);
}

POINT CNewWarfield::GetLivePoint(LPCHARLIST lpChar)
{
	int TeamNo=GetTeamNo(lpChar);

	return m_pTeam[TeamNo].GetLivePoint();
}

int CNewWarfield::GetNationMemberMax(int Nation)
{
	switch (Nation)
	{
	case NW_BY : return MemberMAX[0];
	case NW_ZY : return MemberMAX[1];
	case NW_YL : return MemberMAX[2];
	}
	return  0;
}

int CNewWarfield::GetNationMemberCount(int Nation)
{
	int MemberCount=m_pTeam[0].GetNationMemberCount(Nation)+m_pTeam[1].GetNationMemberCount(Nation);
	return MemberCount;
}

DWORD CNewWarfield::GetRemainTime()
{
 	//return m_64LoopTime-g_WarTime;//-g_curr_time;
	// 030508 kyo g_WarTime가 먼진 잘 몰르게찌만 틀린시간을 나타내었따.
	if( m_64LoopTime<g_curr_time)
	{
		m_64LoopTime=0;
		return 0;
	}
	return m_64LoopTime-g_curr_time;
}

void CNewWarfield::InsertMonsterData(t_WeaponStruct Monster)
{
	m_MonsterList.push_back(new cMonster(Monster));
}

void CNewWarfield::DeleteAllMonsterData()
{
	for (MonsterListItor itor=m_MonsterList.begin();itor!=m_MonsterList.end();)
	{
		SAFE_DELETE((*itor));
		itor=m_MonsterList.erase(itor);
	}
}

void CNewWarfield::MakeMonster()		
{
	int Num;
	t_WeaponStruct Monster;
	for (MonsterListItor itor=m_MonsterList.begin();itor!=m_MonsterList.end();itor++)
	{
		Num=GetAliveNPCList();
		(*itor)->SetMonsterID(Num);
		Monster=(*itor)->GetMonsterData();
		NPC_Create(Monster.NPC_ID,Monster.NPC_Index,Monster.Locationx,Monster.Locationy,Monster.EventNo,Monster.GenerationPos,Monster.GenerationType);
	}
}

void cWarfield::RemoveWonHonor(int cn)
{
	if(cn >= 0 && cn < DRAGON_MAX_CONNECTIONS_)
		HonorWon[cn] = 0;
}


void GiveHonorFromEvent(CHARLIST *pCaster, CHARLIST *pTarget, int event_type)
{
#ifdef WARFIELD_SERVER_

	if(!pCaster && !pTarget)
		return;

	int already = 0;

	for(int i = 0; i < MAX_STORED_ACTIONS ; i++)
	{
		if(stricmp(PK_Actions_tbl[pCaster->GetServerID()][i].target_name, pTarget->Name) == 0)
			already++;

		if(already >= 3) return;
	}

	HonorWon[pCaster->GetServerID()] -= already;

	switch(event_type)
	{
	case PKA_KILL:
		if(pCaster->GetLevel() > pTarget->GetLevel())
			if(pCaster->GetLevel() <= pTarget->GetLevel() + 5)
			{
				HonorWon[pCaster->GetServerID()] += 1;
				if(pCaster->Class == PRIEST)
				{
					HonorWon[pCaster->GetServerID()] += 1;
					if(pTarget->Class != WARRIOR)
					if((pTarget->Skill[pTarget->GetTacticsKind()] / 200) * 150 + 10 > pCaster->GetLevel())
						HonorWon[pCaster->GetServerID()] += 2;
					else
						if((pTarget->Skill[pTarget->GetTacticsKind()] / 200) * 150 > pCaster->GetLevel())
						HonorWon[pCaster->GetServerID()] += 2;
				}
			}
			else
			{
				if(pTarget->Class != WARRIOR)
					if((pTarget->Skill[pTarget->GetTacticsKind()] / 200) * 150 + 10 > pCaster->GetLevel())
						HonorWon[pCaster->GetServerID()] += 1;
					else
						if((pTarget->Skill[pTarget->GetTacticsKind()] / 200) * 150 > pCaster->GetLevel())
						HonorWon[pCaster->GetServerID()] += 1;
			}
		else if(pCaster->GetLevel() < pTarget->GetLevel())
			if(pCaster->GetLevel() >= pTarget->GetLevel() - 5)
			{
				HonorWon[pCaster->GetServerID()] += 1;
				if(pCaster->Class == PRIEST)
				{
					HonorWon[pCaster->GetServerID()] += 1;
					if(pTarget->Class != WARRIOR)
					if((pTarget->Skill[pTarget->GetTacticsKind()] / 200) * 150 + 10 > pCaster->GetLevel())
						HonorWon[pCaster->GetServerID()] += 2;
					else
						if((pTarget->Skill[pTarget->GetTacticsKind()] / 200) * 150 > pCaster->GetLevel())
						HonorWon[pCaster->GetServerID()] += 2;
				}
				else
				{
					if(pTarget->Class == PRIEST)
					{
						if((pTarget->GetLevel() / 150) * 200 > pCaster->Skill[pCaster->GetTacticsKind()])
							HonorWon[pCaster->GetServerID()] += 1;
						else
							HonorWon[pCaster->GetServerID()] += 2;
					}
					else
					{
						if((pTarget->Skill[pTarget->GetTacticsKind()] < pCaster->Skill[pCaster->GetTacticsKind()]))
							if((pCaster->Skill[pTarget->GetTacticsKind()] - (pTarget->Skill[pTarget->GetTacticsKind()] < 5)))
								HonorWon[pCaster->GetServerID()] += 1;
							else {}
						else
							HonorWon[pCaster->GetServerID()] += 3;
					}
				}
			}
			else
			{
				HonorWon[pCaster->GetServerID()] += 2;
				if(pCaster->Class == PRIEST)
				{
					HonorWon[pCaster->GetServerID()] += 1;
					if(pTarget->Class != WARRIOR)
					if((pTarget->Skill[pTarget->GetTacticsKind()] / 200) * 150 + 10 > pCaster->GetLevel())
						HonorWon[pCaster->GetServerID()] += 2;
					else
						if((pTarget->Skill[pTarget->GetTacticsKind()] / 200) * 150 > pCaster->GetLevel())
						HonorWon[pCaster->GetServerID()] += 2;
				}
			}
		else if(pCaster->GetLevel() == pTarget->GetLevel())
		{
			HonorWon[pCaster->GetServerID()] += 1;
			if(pCaster->Class == PRIEST)
				{
					HonorWon[pCaster->GetServerID()] += 1;
					if(pTarget->Class != WARRIOR)
					if((pTarget->Skill[pTarget->GetTacticsKind()] / 200) * 150 + 10 > pCaster->GetLevel())
						HonorWon[pCaster->GetServerID()] += 2;
					else
						if((pTarget->Skill[pTarget->GetTacticsKind()] / 200) * 150 > pCaster->GetLevel())
							HonorWon[pCaster->GetServerID()] += 2;
				}
			else
			{
				if(pCaster->Skill[pCaster->GetTacticsKind()] > pTarget->Skill[pTarget->GetTacticsKind()])
					HonorWon[pCaster->GetServerID()] += 1;
				else
					HonorWon[pCaster->GetServerID()] += 2;
			}
		}

		int current_index = PKA_Number[pCaster->GetServerID()];

		PK_Actions_tbl[pCaster->GetServerID()][current_index].action = event_type;
		strcpy(PK_Actions_tbl[pCaster->GetServerID()][current_index].target_name, pTarget->Name);
		PKA_Number[pCaster->GetServerID()]++;
		if(PKA_Number[pCaster->GetServerID()] >= MAX_STORED_ACTIONS)
			PKA_Number[pCaster->GetServerID()] = 0;
		break;
	}

#endif
}

void SaveChangeHonor(CHARLIST* pTarget, int old_honor, int new_honor)
{	//< CSD-040224
	string strPath;

	if (!g_pLogManager->GetLogPath(LT_CHANGE, strPath))
	{
		return;
	}

	FILE* fp = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");

	if(fp == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, pTarget->Name);
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (fp == NULL)
	{
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pTarget->GetServerID()].id), "at+");

		if (fp == NULL)
		{
			return;
		}
	}
	
	char why[MAX_PATH] = {0,};

	

	::fprintf(fp, "@ChangeHonor %02d-%02d-%02d %02d:%02d:%02d	(x:%d y:%d)	(old)%d	(new)%d\n",
			  g_year-2000, g_mon + 1, g_day,
			  g_hour, g_min, g_sec,
			  pTarget->X, pTarget->Y, old_honor, new_honor);
	::fclose( fp );
}

void SaveHonorWon(short int cn, int amount) //To save & log the won honor
{
	CHARLIST *ch = CheckServerId(cn);
	if(!ch) return;

	SaveChangeHonor(ch, var[cn][HONOR_QUEST_NO], var[cn][HONOR_QUEST_NO] += HonorWon[cn]);

	var[cn][HONOR_QUEST_NO] += HonorWon[cn]; //Eleval 07/09/09 - Honor Won
	var[cn][HONOR_MAX_QUEST_NO] += HonorWon[cn]; //Eleval 07/09/09 - Honor Won
}
