#include "skill_lsw.h"

const int MAX_NAME = 21;

#define CMD_SKILL_RARE_UPGRADE					5600
#define CMD_SKILL_RARE_UPGRADE_RESULT			5601

typedef struct HWOARANG_SANGWOO_SKILLRAREUPGRADE
{	
	short int iSkillNo;
	
	ItemAttr	SourceItem;

	int iBuffer1;
	int iBuffer2;

	POS pos[MAX_UPGRADE_RARE];

}t_SkillRareUpgrade;


typedef struct HWOARANG_SANGWOO_SKILLRAREUPGRADE_RESULT
{	
	short int iSuccess;
	
	ItemAttr	SourceItem;

}t_SkillRareUpgradeResult;


#define CMD_RARE_UPGRADE_BBS				2003
#define CMD_RARE_UPGRADE_BBS_MAP_TO_MAP		2004
typedef struct HWOARANG_SANGWOO_RAREUPGRADE_BBS
{	
	char	name[20];
	ItemAttr	Item;
}t_RareUpgradeBBS;

typedef struct HWOARANG_SANGWOO_RAREUPGRADE_BBS_MAP_TO_MAP
{	
	char	name[20];
	ItemAttr	Item;
}t_RareUpgradeBBSMapToMap;

#define CMD_CONFORM_SADONIX		9009
typedef struct CONFORMSADONIX
{
	int		cn;
	char	name[20];
	int		kind;
	ItemAttr item;
	int		x;
	int		y;
}t_ConformSadonix,*LPCONFORMSADONIX;

#define CMD_SADONIX_COUNT_UPDATE		9010
typedef struct SADONIXCOUNTUPDATE
{
	short nSadTime;
}t_SadonixCountUpdate,*LPSADONIXCOUNTUPDATE;

#define CMD_MOVE_TO_HOMETOWN			2005
#define CMD_MOVE_TO_HOMETOWN_WITH_ALIVE	2007
extern void MoveToHomeTown(CHARLIST *Caster);
extern void MoveToHomeTownWithAilve(CHARLIST *Caster);

#define CMD_DAYLIGHT					2006//광원 처리 보내기
typedef struct DAYLIGHT
{
	short int nDayLightControl;
}t_DayLight,*LPDAYLIGHT;

#define CMD_GET_SKILL_POINT				9018	// this packet is received when the client wants the SMP.

#define CMD_NOW_SKILL_POINT				9019//020815 lsw
typedef struct NOWSKILLPOINT
{
	ItemAttr	item;
	int			iNowPoint;	
}t_NowSkillPoint,*LPNOWSKILLPOINT;

#define CMD_COMFORM_RESET_ABILILTY				9020//020820 lsw
typedef struct COMFORMRESETABILILTY
{
	int cn;
	char szName[21];
	int iIsAble;//020821-1 lsw
}t_ComformResetAbililty,*LPCOMFORMRESETABILILTY;

extern void SendCMD_COMFORM_RESET_ABILITY(CHARLIST *ch);//020820 lsw

#define CMD_MERCHANT_BUY_ITEM_SEARCH_CLIENT			16000//020815 lsw
//<soto-030511 수정.
typedef struct SEARCHPACKETCLIENT
{
	char	szKeyWord[21];	//키워드 검색에 들어갈
	char	szMerchant[21]; //상인이름 검색에 들어갈
	int		nPeriod;		//등록 기간 설정.// 0:1일, 1:3일,  2:일주일.
	int		iIndex;
	int		iKey;
}t_SearchPacketClient,*lpSEARCHPACKETCLIENT;
//>soto-030511 수정.  


#define CMD_MERCHANT_BUY_ITEM_SEARCH_SERVER			16002//020815 lsw
typedef struct SEARCHPACKETSERVER
{
	int iCn;
	char szName[21];
	t_SearchPacketClient	ClientMsg;
}t_SearchPacketServer,*lpSEARCHPACKETSERVER;

typedef struct SEARCHRESULT
{
	int			iIndex;
	char		szSellerName[21];
	char		szBuyerName[21];
	int			iSellValue;//판매금액은 20억을 넘기지 않습니다.
	int			iIsEnd;
	int			iSellerTake;
	int			iBuyerTake;
	ItemAttr	m_ResultItem;
}t_SearchResult,*lpSEARCHRESULT;

#define CMD_MERCHANT_BUY_ITEM_SEARCH_RESULT			16001//020815 lsw
#define CMD_MERCHANT_SELL_LIST_REQUEST_RESULT		16004
#define CMD_MERCHANT_RESULT_LIST_REQUEST			16030
#define CMD_MERCHANT_RESULT_LIST_REQUEST_RESULT		16031
const int MAX_SEARCH_RESULT_LIST = 24;
typedef struct SEARCHRESULTLIST
{
	int	 iCn;
	char szName[21];
	int iKey;
	SEARCHRESULT ResultList[MAX_SEARCH_RESULT_LIST];
}t_SearchResultList,*lpSEARCHRESULTLIST;
//

//구매자용 아이템 구입을 하겠다는 패킷
#define CMD_MERCHANT_ITEM_BUY					16005//클라이언트 에게 받고 
#define CMD_MERCHANT_ITEM_BUY_RESULT			16006//클라이언트로 보내고 
#define CMD_MERCHANT_ITEM_BUY_COMFORM			16007//디비와 통신
#define CMD_MERCHANT_ITEM_BUY_COMFORM_RESULT	16008//디비와 통신
typedef struct MERCHANT_ITEM_BUY
{
	int			iIndex;//구입할 아이템의 인덱스 값
	int			iCn;//커넥션 값
	char		szSellerName[21];//판매자 이름
	char		szBuyerName[21];//구매자 이름
	DWORD		dwSellValue;//판매금액은 20억을 넘기지 않습니다.
	ItemAttr	SellItem;
	int			iKey;
}t_MerchantItemBuy,*lpMERCHANTITEMBUY;

//

//판매자용 구조체
#define CMD_MERCHANT_SELL_ITEM_INFO_REQUEST		16003
typedef struct SELLERITEMREQUEST
{
	int iCn;
	int iIndex;
	int iKey;
	char szName[21];//자신의 이름을 보내서 아이템을 검색 합니다.
}t_SellerItemRequest,*lpSELLERITEMREQUEST;

//아이템 등록
#define CMD_MERCHANT_SELL_ITEM_REGISTER					16009//판매아이템을 등록 합니다.
#define CMD_MERCHANT_SELL_ITEM_REGISTER_RESULT			16010//판매아이템을 등록 합니다.
#define CMD_MERCHANT_SELL_ITEM_REGISTER_COMFORM			16011//판매아이템을 등록 합니다.(디비에 가능한지 요청 합니다.
#define CMD_MERCHANT_SELL_ITEM_REGISTER_COMFORM_RESULT	16012//판매아이템을 등록 합니다.(디비에 돈을 등록 하고 등등 ㅡㅡ;.. 어지러워라..
typedef struct SELLERITEMREGISTER
{
	int			iCn;
	char		szName[21];
	int			iFKRareType;
	int			iFKLevel;
	int			iFKTacticType;
	int			iFKWearType;
	POS			pos;
	DWORD		dwSellValue;
	ItemAttr	SellItem;
	int			iKey;
}t_SellerItemRegister,*lpSELLERITEMREGISTER;

#define CMD_MERCHANT_SELL_ITEM_DELETE					16020//판매를 취소합니다.
#define CMD_MERCHANT_SELL_ITEM_DELETE_RESULT			16021//판매를 취소합니다.
#define CMD_MERCHANT_SELL_ITEM_DELETE_COMFORM			16022//판매를 취소합니다.//디비에 요청
#define CMD_MERCHANT_SELL_ITEM_DELETE_COMFORM_RESULT	16023//판매를 취소합니다.//디비에 아이템을 지우고 클라이언트에게 지급
#define CMD_MERCHANT_RECORD_DEL_CANCLE					16024	// BBD 040226 판매 취소로 레코드 삭제 요청.
#define CMD_MERCHANT_RECORD_DEL_CANCLE_RESULT			16025	// BBD 040226 판매 취소로 레코드 삭제 결과. 아직 사용치 않음
#define CMD_MERCHANT_RECORD_DEL_COMPLETE				16026	// BBD 040226 거래 종료로 레코드 삭제 요청.
#define CMD_MERCHANT_RECORD_DEL_COMPLETE_RESULT			16027	// BBD 040226 거래 종료로 레코드 삭제 결과. 아직 사용치 않음

typedef struct SELLERITEMDELETE
{
	int			iIndex;//판매물품의 인덱스
	int			iCn;
	char		szSellerName[21];
	char		szName[21];
	DWORD		dwSellValue;
	ItemAttr	SellItem;
	int			iKey;
}t_SellerItemDelete,*lpSELLERITEMDELETE;

#define CMD_MERCHANT_RESULT_TAKE						16032
#define CMD_MERCHANT_RESULT_TAKE_RESULT					16033
#define CMD_MERCHANT_RESULT_TAKE_COMFORM				16034
#define CMD_MERCHANT_RESULT_TAKE_COMFORM_RESULT			16035

#define	CMD_MERCHANT_BACKUP_LIST_REQUEST				16036	// BBD 040303
#define CMD_MERCHANT_BACKUP_LIST_RESPONSE				16037	// BBD 040303
#define	CMD_MERCHANT_BACKUP_TAKE_REQUEST				16038	// BBD 040303
#define	CMD_MERCHANT_BACKUP_TAKE_RESPONSE				16039	// BBD 040303

#define CMD_EVENTRAREITEM_REQ							16300	// BBD 040308
#define CMD_EVENTRAREITEM_RES							16301	// BBD 040308

typedef struct MERCHANTRESULTTAKE
{
	int			iIndex;//판매물품의 인덱스
	int			iCn;
	char		szMyName[21];
	char		szSellerName[21];
	char		szBuyerName[21];
	DWORD		dwSellValue;
	ItemAttr	SellItem;
	int			iSellerTake;
	int			iBuyerTake;
	int			iKey;
}t_MerchantResultTake,*lpMERCHANTRESULTTAKE;

#define CMD_MERCHANT_DIRECT_EXCHANGE_LIST_REQUSET					16090
#define CMD_MERCHANT_DIRECT_EXCHANGE_LIST_REQUSET_RESULT			16091
#define CMD_MERCHANT_EXCHANGE_RESULT								16101
typedef struct MERCHANTEXCHANGEREQUEST
{
	int			iIndex;//0으로 날리도록 하고
	int			iCn;
	char		szSellerName[21];
	char		szMyName[21];
	int			iKey;//인덱스 위인가 아래인가.
}t_MerchantExchangeRequest,*lpMERCHANTEXCHANGERequest;

const int CMD_SKILL_MASTER_MULTI_RARE_MAKE				=		16200;

typedef struct tagMakeItem		// 작업대에서 사용할 아이템 구조체
{
	int			item_no;
	int			x;
	int			y;
	RECT		rect;
	ItemAttr	attr;
	POS			pos;
} MakeItem;

typedef struct MULTIRARESRC
{
	MakeItem SrcData[2];
}t_MultiRareSrc,*LPMULTIRARESRC;

typedef struct SKILLMASTERMULTIRAREMAKE
{
	int iCn;
	int iSkillNo;
	MULTIRARESRC MRS;
}t_SkillMasterMultiRareMake,*LPSKILLMASTERMULTIRAREMAKE;

const int CMD_EXCHANGE_BOND_MONEY				=		16201;
typedef struct EXCHANGEBONDMONEY
{
	int iCn;
	POS	pos;//위치 
	ITEMATTR BondMoneyItem;//본드머니 정보
	int iKey;//여분으로 사용할 키
}t_ExchangeBondMoney,*LPEXCHANGEBONDMONEY;

const int CMD_EXCHANGE_CANCEL_SIGN	= 16202;
const int CMD_MAP_MOVE_LV_LIMIT		= 16203;

typedef struct MAPMOVELVLIMIT
{
	int iLvMin;
	int iLvMax;
}t_MapMoveLvLimit,*LPMAPMOVELVLIMIT;

const int CMD_TRADE_MODE_CHANGE				=		16204;//030127 lsw
typedef struct TRADEMODECHANGE
{
	int iCn;
	bool bIsTradeOn;
}t_TradeModeChange,*LPTRADEMODECHANGE;
// Mail System //////////////////////////////////////////////////////////////////////////////
const int MAX_MAIL_PAGE	= 12;
const int MAX_MAIL_RECEIVER = 5;
const int MAX_MAIL_BODY_LEN = 1000;

typedef struct MAILDATA
{
	int iIndex;

	int iType;
	bool bIsRead;
	
	char szSender[20];
	char szReceiver[20];
	
	char szTitle[60];
	TIMESTAMP_STRUCT	date;
	int	iBodyIndex;
}t_MailDdata, *LPMAILDATA;

typedef struct MAILBODY
{
	int iIndex;
	char szBody[MAX_MAIL_BODY_LEN];
}t_MailBody, *LPMAILBODY;

const int CMD_MAIL_REQ_BODY		= 16102;//해당 메일의 바디
typedef struct REQESTMAILBODY
{
//	int iWantMailType;
	int iMailIndex;
	char szName[MAX_NAME];
}t_ReqestMailBody,*LPREQESTMAILBODY;

const int CMD_MAIL_REQ_BODY_RESULT		= 16113;//해당 메일의 바디
typedef struct REQESTMAILBODYRESULT
{
//	int iWantMailType;
	int iMailIndex;
	int iType;
	char szSender[MAX_NAME];
	char szReceiver[MAX_NAME];
	char szTitle[MAX_NAME];
	TIMESTAMP_STRUCT	date;
	char szBody[MAX_MAIL_BODY_LEN];
}t_ReqestMailBodyResult,*LPREQESTMAILBODYRESULT;

const int CMD_MAIL_REQUEST_LIST = 16114;//한페이지 분량의 메일 리스트.
typedef struct REQESTMAILLIST
{
	int iWantMailType;
	int iPageIndex;
	char szName[MAX_NAME];
}t_ReqestMailList,*LPREQESTMAILLIST;

//메일 관련 받기 (클라이언트)
const int CMD_MAIL_SEND	=10142;	// 메일 보내기
typedef struct MAILSEND
{
	int iType;
	char szTarget[MAX_NAME*MAX_MAIL_RECEIVER];//5명
	char szSender[MAX_NAME];
	char szTitle[60];
	char szBody[MAX_MAIL_BODY_LEN];
}t_MailSend,*LPMAILSEND;

const int CMD_MAIL_LIST_FROM_DBDEMON = 16100;
typedef struct MAILLISTRESULT
{
	char szName[MAX_NAME];
	int iWantViewType;
	int iWantViewPage;
	MAILDATA MailData[MAX_MAIL_PAGE];
}t_MailListResult,*LPMAILLISTRESULT;

#define CMD_REQ_RECV_MAIL_BODY				10144	// 메일 내용 요청

#define CMD_REP_RECV_MAIL_TITLE				10145
#define CMD_REP_RECV_MAIL_BODY				10146
#define	CMD_RECV_MAIL_DELETE				10147

#define CMD_SEND_MAIL_GUILD					10355
#define CMD_REQ_RECV_MAIL_TITLE_GUILD		10357
#define CMD_REP_RECV_GUILD_MAIL_TITLE		10358
#define CMD_REQ_RECV_MAIL_BODY_GUILD		10359
#define CMD_UPDATE_GUILD_MAIL_ID			10360
#define CMD_RECV_MAIL_DELETE_GUILD			10361

const int CMD_MAIL_ALERT = 16400;//메일 얼럿

const int CMD_MAIL_DELETE	=10143;	// 메일 삭제
const int CMD_MAIL_REPAIR	=10145;	// 메일 삭제
typedef struct MAILDELETE
{
	int cn;
	char szName[MAX_NAME];
	int aiMailIndex[MAX_MAIL_PAGE];
}t_MailDelete,*LPMAILDELETE;
//< CSD-030808
#define CMD_CHECK_ACCEL	6060
typedef struct server_check_accel
{
	DWORD	dwCheckAccelNowtime;
}t_server_check_accel;

#define CMD_SEND_ACCEL_TYPE 6061
typedef struct tag_ACCEL_TYPE
{
	int		nType;
	char	strMsg[128];
}t_ACCEL_TYPE;
//> CSD-030808
typedef struct HWOARANG_PACKET
{
	union			
	{
		char	data[MAX_PACKET_SIZE];
		t_SkillMasterMultiRareMake	SMMultiRareMake;
		t_ExchangeBondMoney			ExBondMoney;
		t_MapMoveLvLimit			MapMoveLvLimit;
		t_TradeModeChange			TradeModeChange;
		
		t_ReqestMailList			ReqestMailList;
		t_MailSend					MailSend;
		t_MailDelete				MailDelete;
		t_ReqestMailBody			ReqestMailBody;
		t_ReqestMailBodyResult		ReqestMailBodyResult;
	}rang;
}t_Hwoarang;