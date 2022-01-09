#ifndef __MenuServer_h__
#define __MenuServer_h__


#define KM_INFO	0
#define KM_FAIL	1
#define KM_OK	2
#define KM_BBS	3

#define QUEST_START			51

/////////////////////////////////////////
// BBS 경우.. 주로 추카하는데 사용하게 될듯 ( 이름하고만 같이 넘어간다. )
#define BA_GM_REGIST	1	// gm 추카 전체 공지




//###########################################################
// SendServerResult에서 사용할 define
#define CM_REPAIR			0		//	CM_ (Client Menu)
#define CM_LEARNSKILL		1
#define CM_LEARN_MAGIC		2
#define CM_PREVENTING		3		// 예방
#define CM_CURE_DISEASE		4		// 치료
#define CM_INN_SLEEP		5		// 쉬기
#define CM_INN_REST			6		// 잠자기
#define CM_OPEN_SUPPER		7		// 신전에서 음식 제공
 
//###########################################################
// 신전에서 사용할 예배 타입
#define DAY_MEETING			100
#define MONTH_MEETING		200
#define YEAR_MEETING		300
#define MONTH_MEETING_ALAM	400


//###########################################################
#define INN_SINGLE	10
#define INN_DOUBLE	20
#define INN_MULTI	30

/////////////////////// GOD Define ////////////////////////
#define EDELBLHOY		1
#define TEFFERY			2
#define LETTY			3
///////////////////////////////////////////////////////////

/////////////////// store item type /////////////////////// 상점의 입장에서
#define	SIT_NULL				0		// null
#define SIT_SELL_BUY			1		// 사고 팔기 가능
#define SIT_ONLY_SELL			2		// 상점에서 팔기만
#define SIT_ONLY_BUY			3		// 상점에서 사주기만
#define SIT_EVENT_ITEM			4		// 이벤트용


//////////////////////////////////////////////////////////

#define	DUR_RATIO			.70
#define	RARE_DUR_RATIO		.50//020505 lsw


#define RELATION		100
#define PARTY			200
#define EMPLOYMENT		300


////////////////////////////////////////////////////////////
// POS type 2
#define PT_HAND			0
#define PT_INV			1
#define PT_EQUIP		2
#define PT_QUICK		3
#define PT_BANK			4
#define PT_BOX			5	
#define PT_MAKE			6
#define PT_OTHER_CH		7
#define PT_CHARACTER	8
#define PT_GROUND		9
#define PT_USE			10
#define PT_BUY			11
#define PT_SELL			12
#define PT_DELETE		13
#define PT_EXCHANGE		14

#define MAX_WORD		500
#define ET_ADD_FIGHT_MEMBER		9777


///////////////////////////////////////////////////////
// 살아나는 맵 정의
#define MAX_ALIVE_MAP		50
struct AliveMapZone
{
	int map_number;
	int x;
	int y;
	int xl;
	int yl;
	int is_all;
	int alive_word_type;
};


class CAliveMapZoneAll
{
public :
	AliveMapZone m_Map[MAX_ALIVE_MAP];
	int m_Max;		// 최대
public : 
	CAliveMapZoneAll() { m_Max = 0; }
	int IsPossableAlive( int map_number, CHARLIST *ch  );
	int LoadAliveMapZone();
	int CheckAlive( int ch_x, int ch_y, int map_number, int word_type );		// 살아 날수 있는지
};


extern CAliveMapZoneAll	g_AliveMap;
extern NPCLev_to_Exp		NPC_Lev_Ref[Num_Of_NPC_Lev];
extern NPC_Generation		NPC_Gen_Ref[Num_Of_NPC_Generation] ;

extern void MakeStoreList( void );
extern void CallSmallMenu( int cn, int storelistindex, int menu );
extern void RecvItemBuy( int cn, t_client_item_buy *p );
extern void RecvItemSell( int cn, t_client_item_sell *p );

extern void SendRumor( t_connection c[],  int cn );

extern int	SearchInv_invert( ItemAttr inv[3][3][8], int &a, int &b, int &c );
extern void SendLevelUpMsgAndPoint( CHARLIST *chr );

extern MAGICTABLE Magic_Ref[] ;
extern DiseaseTable	disease_tbl[6];
extern ItemAttr *GetItemByPOS( int cn, POS item );
extern ItemAttr *GetItemByPOS( CHARLIST *ch, POS item );
extern struct GodTable g_GodTable[MAX_GOD];


//////////////////////////////////////////////////////////////////

extern void SendMessageTitle( int type, int count, t_connection c[], int cn );
extern void SendCharInfomation( int type, t_connection c[], int cn );
extern void SendOtherInv( int type, int other_id, t_connection c[], int cn );
extern int	RecvDivideItem( kein_item_attr_pos *item, t_connection c[], int cn );
extern void RecvBankInfoDeposit( DWORD dwMoney, t_connection c[], int cn );
extern void SendBankRepayment( t_connection c[], int cn );
extern void RecvCharInfoBasic( t_char_info_basic *p, t_connection c[], int cn ) ; // 클라이언트에서 보내준 능력치
extern void RecvBoxOpen( lpITEM box, t_connection c[], int cn );
extern void RecvLearnSkill( t_client_learn_skill *p, t_connection c[], int cn  );		//###1213 수정


extern void SendOtherInv( int type, CHARLIST *npc, t_connection c[], int cn );					//####1213_2
extern void RecvLearnItemByGuild( t_server_learn_skillitem *p, t_connection c[], int cn );		//####1213_2
// 020428 YGI acer		//####1213_2
extern int SendDeleteItem( ItemAttr *pItem, POS *pos, CHARLIST *ch, bool broken = false );
extern void SendDecreaseItem( POS item_pos, DWORD dur, t_connection c[], int cn );	// 아이템 감소 내구력 전송 함수

extern void RecvLvUpPointEach( unsigned char type, t_connection c[], int cn );
extern void RecvLvDownPointEach( unsigned char type, t_connection c[], int cn );
extern void RecvLearnItem( k_client_learn_item *i, t_connection c[], int cn );		// 아이템 배우기 		
extern void SendServerResult( BYTE type, BYTE result, int cn );		// 어떤 결과를 보내줄때..
extern void RecvItemRepair( t_client_item_sell *p, t_connection c[], int cn );
extern void RecvItemRepairHammer( t_client_item_sell *p, t_connection c[], int cn );
extern void RecvMagicItem( t_client_item_sell *p, t_connection c[], int cn );

//###############################################################################
// global function extern

extern int DeleteItem( ItemAttr *item );
extern void GetItemDuration( ItemAttr item, WORD &d_curr, WORD &d_max );	// 아이템 내구력 뽑아 오기
extern void SendStatusOpen( t_connection c[], int cn );
extern void RecvCharInfoPlus( int type, t_connection c[], int cn );
extern void RecvBankInfoDefrayal( DWORD dwMoney, t_connection c[], int cn );
extern void SendBankInventory( int count, t_connection c[], int cn );
extern void RecvDoParty( k_client_do_party *p, t_connection c[], int cn, int party_cn = 0, int check_nation = 1);
extern void RecvDeleteParty( k_client_delete_party *p, t_connection c[], int cn );

extern int GetMagicNumBySerial( int serial );
extern int CanLearnMagic( int num, CHARLIST *ch );		// 그 마법을 배울수 있나?
extern void RecvLearnMagic( short int num, t_connection c[], int cn );
extern void SendLearnMagicOk( int num , int cn);
extern void RecvPreventing( short int num, t_connection c[], int cn );
extern void RecvCureDisease( short int num, t_connection c[], int cn );
extern int GetDisease_SQL( );

extern void SendItemValue( int item_no, int cn );
extern void SendItemValue_Sell( POS *pos, int cn );
extern void SendItemValue_Repair( int item_no, int cn );	
extern CHARLIST* SearchIdByName(const char* pName); // CSD-030320
//extern int SendPartyInfo( int type, int ct, t_connection c[], int cn );	// 파티 종류, 파티 배열 번호 
extern int SendPartyInfo( int type, int ct, t_connection c[], const char* szName);	// 파티 종류, 파티 배열 번호
extern void RecvClientInn( char type, int cn );
extern void SendServerWeakUp( int cn );
extern void SendJoinGameWho( int cn );
extern void SendExitGameWho( int cn );
extern void SendMyPartyExist( int cn );			// 새로들어온 캐릭터에게 파티원이 있는 지를 보내준다.
extern void RecvReleaseParty( char *name, int cn );
extern void SendPartyJoinOkSet( int cn );
extern void SendAbilityOpen( const int cn );

extern void SendSubtractMoney( DWORD money, int cn );
extern void SendAddMoney( DWORD money, int cn );
extern void RecvLearnAll( int cn ) ;
extern void RecvLearnAllNot( int cn );

extern int RecvItemExchange( K_ITEM *k_item, int you_id, int cn , const int iExchangeType);
extern void SendExchangeItemResult( int cn, int result );
extern void SendExchangeItemReq( int you_id, int cn, const K_ITEM *item ,const int iExchangeType);	// cn이 너에게 아이템을 교환하자고 요청했다.
extern void SendExchangeItemOk( int you_id, int cn, const K_ITEM *item ,const int iExchangeType);	// 아이템 교환 메뉴를 띄워라...
extern void SendExchangeCancel( short int you, short int me );
extern void RecvExchangeStateSign( const short int you, const short int me , const bool bIsOkSign);//030108 lsw

extern void RecvExchangeEach( K_ITEM *k_item, int you_id, int cn );		// 올려 놓기 시작한 물건들
extern void RecvExchange( short int you, short int me );
extern void RecvExchangeItemDel( POS pos, short int you_id, short int me );
extern int CheckCurrStats( CHARLIST *ch );			// 초기 캐릭터 접속후 캐릭터 상태계산
extern void SendServerAc( int cn );

extern int ChangeItemAttr( short int cn, int type, POS pos, int change );		// chagne값엔 부호를 확실히 한다.
extern inline void CallClient( short int id, short int type );
extern void SendPacketDefault( int type, void *msg, int size, short int cn );
extern void SendServerDmg( int cn );
extern void SendServerKillMon( short int cn );
extern int SetTactics( int spell_type, int sex, int tac_type );
extern int SetMySkill( CHARLIST *ch );
extern void RecvPartyInfoServerToServer( k_server_server_party_info *p );
extern void ReqPartyDB( short int client_id, char para, char *name );

extern int EqualPosByPos( const POS *pos1, const POS *pos2 );
//extern SendPartyInfo3( short int type, char ct,  short int cn );
extern int SendPartyInfo3( short int type, char ct,  const char* szName );

extern int IsMySightInWho( char *name, short int cn)		;
extern int IsMySightInWhoById( short int check_id, short int cn );
extern void SendReqPartyInfoOfOtherChar( char *party_name, char *my_name, short int server_id );
extern void RecvPartyTogetherByLoginServer( short int server_id, char *name );
extern int InitPartyMemberServerToServer( CHARLIST *ch ); // CSD-HK-030829 //  이름만 디비에 저장하고 이름만 꺼내온다.
extern int EndsetPartyMenberForUpdate(CHARLIST *ch); // CSD-HK-030829
extern int RecvNoParty(char *name, short int server_id); // CSD-HK-030829 //잘 못된 이름을 가지고 있을때... 지운다.
extern void SendServerMagery( short int cn );
extern void SendExchangeItemDel( const short int you, const K_ITEM item );		// 상대편에게 넘어갈 아이템중 삭제할 아이템..
extern void RecvBbsMessageReq( int type, char ct, short int cn );	// 공지사항등 내용 전송
//extern void SendLoginPutBoxItem( DWORD box_id, int para, ItemAttr *lpItem );//020303 lsw

extern void SendLoginServerPreachName( k_server_preach_name *p, short int cn );
extern void RecvAndSendPreachName( k_loginserver_preach_name_result *p);	// 결과를 받아서 클라이언트로 보낸다.
extern void RecvGodRegistOk( char believe_god, short int cn );		// 등록
extern void RecvGodRegistCancel( short int cn );	// 파계
extern void SendLoginServePreachNameOk( k_server_preach_name *p, short int cn );		// 이 함수는 전도한 사람에게 신앙수치를 올려주는 루틴이다.
extern void SendLoginEvangelistCreate( char *my_name, char *eva_name );
extern void SendLoginEvangelistDelete( char *my_name );

extern void CheckTimeForMenu_Kein();
extern void RecvToolPassword( char *password , short int cn);
extern void LoadGetGodTable();
extern int CheckGodMeetingTime( CHARLIST *ch );

extern void SendServerMessage( int type, char *szTemp, short int cn );
extern void SendLoginServerForCharUpdate( short int cn );
extern void  RecvLoginServerGetCharInfoByKein( k_get_char_info *p );
extern void SendReqGetCharInfoEtc( short int server_id, char *name );
extern void SendServerKillPc( short int cn );

extern void RecvCanMemorize( t_connection c[], short int cn );
extern void RecvEarMessage( char *name, char *message, short int cn );
extern void FindWhoEarMessageTarget( k_login_to_gameserver_ear_message *p ); // 귀속말을 받을 사람이 이 게임 서버에 존재하는지 판단
extern void SendEarMessageResult( int type, char *name, short int cn );
extern void SendEarMessageResult2( int type, int ret, short int cn );

extern void SendLoginEarMessageResult( k_send_ear_message_result *p, short int cn );		// 클라이언트에서 보낸 답신을 로그인으로 보내고
extern void RecvLoginToGameEarMessageResult( short int server_id, char ret );		// 로그인서버에서 보내온 답장을 클라이언트로 보내준다.
extern void SendGameToLoginExistName( char *name, short int cn );
extern void CheckMoveItem( char *name, int source_type, int dest_type, ItemAttr item, DWORD money);
extern void CheckEarChatingData( char *send_name, char *recv_name, char *message );		// 채팅하는 말 저장 하기
extern void closeconnection_kein(t_connection c[], int cn, int errnum);

bool NewSendGameToLoginChangeMapData( short int cn, WORD wMyAgentServerID, WORD wTargetServerID);
void NewRecvChangeMapData1( k_change_map1 *p, DWORD dwNewCN );
void NewRecvChangeMapData2( k_change_map2 *p, DWORD dwNewCN );
void NewRecvChangeMapData3( k_change_map3 *p, DWORD dwNewCN );
void NewRecvChangeMapData4( k_change_map4 *p, DWORD dwNewCN );
void NewRecvChangeMapData5( k_change_map5 *p, DWORD dwNewCN );
void NewRecvChangeMapData6( k_change_map6 *p, DWORD dwNewCN );
void NewRecvChangeMapData7( k_change_map7 *p, DWORD dwNewCN );
void NewRecvChangeMapData8( k_change_map8 *p, DWORD dwNewCN );
void NewRecvChangeMapData9( k_change_map9 *p, DWORD dwNewCN );

extern int CalcCharAbilityByChangemap( CHARLIST *ch );	// 캐릭터 초기 설정

extern void RecvChangeMapData9( k_change_map9 *p );		// 캐릭터 데이타 8 받기
extern int  LoadFilterWord( );
extern void ConvString( char *szStr ); // 욕설 필터
extern int CompPartyBoth( CHARLIST *ch_1, CHARLIST *ch_2 );		// 서로 파티를 맺었는가...
extern void SendSkillAll( short int cn );
extern void RecvAndSendFaithUp( char *name, short int up_point );
extern void RecvDivineUp( short int cn );			// 기도문에 의한 마나 충족
extern void LoadMeetingMessage();
extern int GetMeetMessageRandomNumber( int god_type, int meet_type );		// 예배 메시지 번호 가져오기 ( 내용있는 것중에서 랜덤으로 )
extern void RecvOpenSupper( short int cn );
extern void RecvGetItemOfSupper( short int item_no, short int cn );
extern void RecvChangeMapConfirm( short int cn );
extern void CheckInitSkill( CHARLIST *ch );		 // 클레스 스킬은 확실하게...
extern void RecvReqCharInfo( int cn, int target );
extern void RecvOhMyParty( char *my_name, char *party_name );
extern void SetShape( unsigned char *equip, short int cn );
extern void MoveEmptyInv( ItemAttr *item, CHARLIST *ch );		// 아이템을 비어 있는 곳에 옮기고 지운다.
extern int GetItemJoinNumber( int item_num );
extern void SortingItem( ItemAttr *item_list, int *list_int, int &max );
extern void MakeFileItemList( ItemAttr *data, int max, char *filename );
extern void SetTacSkillLevel( CHARLIST *ch );
extern void CheckAndSendChangeEquip( CHARLIST *ch, int send = 0  );
extern void RecvRepPartyInfo( char *party_name, short int cn );
extern void CheckBackupItemForCharacter( CHARLIST *ch );
extern void ReturnResultByName( char *name, int cmd_type );		// 이름으로 아이디를 찾아서 프로토콜을 전송해준다.
extern void CheckMyMailOnStartGame( short int cn );				// 아이템 
extern void makemyjobexpsetting( int skillno , CHARLIST *ch );
extern void SendToLoginserver( short int cn );
extern void SendLoginToClientPublicBbsOnlyOne( t_packet *p );
extern void SendLoginServerHunterRegist( k_hunter_regist *p, int size, short int cn );
extern void SendLoginServerReqHunterList( int id, short int cn );
extern void RecvLoginServerAndSendHunterList( k_login_game_hunter_list *login_list );
extern void RecvHunterList( int id, short int cn );
extern void RecvHunterListOk( k_send_kill_hunter_list *p );
extern void ReturnDangerByHunter( short int target_id, char type);
extern int IsCheckPartyLevel( int type, CHARLIST *target_ch, CHARLIST *ch );

extern void RecvReqManToMan( short int target_id, short int cn );
extern void SendReqManToManResult( char ret, short int cn );
extern void RecvManToManConsent( short int target_id, short int cn );
extern void RecvReqQuestIndex( char page, short int cn );
extern void SendLearnedItem( t_connection c[], short int cn )	;
extern void SendDeleteBox( DWORD box_id );
extern bool IsConnectLogin(int idUser); // CSD-030515 
extern bool IsConnectJoin(int idUser);  // CSD-030515
extern CHARLIST* CheckServerId(WORD id);
extern CHARLIST* CheckServerName( const char* szName );// kyo 
extern CHARLIST* GetCharListPtr(WORD idObject);
extern CHARLIST* CheckNpcId(short int cn); // CSD-031014
extern CHARLIST* CheckNpcName(const char* pName); // CSD-031014
extern bool CheckServerIndex( short int id );
//extern void RecvManToManItemEach( short int target_id, short int cn );//021030 lsw 이제 아이템을 올리지 못합니다.
extern int SendServerItemEachAuto( CHARLIST *ch, ItemAttr *item );		// 빈곳을 찾아 보내준다. 빈곳이 있으면 1을 리턴 없으면 0을 리턴
extern void ResultManToMan( short int cn, short int target_id, int flag );					// 1:1 에서 결과가 나왔을 경우 아이템 셋팅~
extern void SendTeamBattleMessageAddCharOk( char team_num, short int cn );
extern void CheckFightMap( char *mapname, short int &x, short int &y, CHARLIST *ch );
extern void ResetManToManItemWhenCloseConnect( CHARLIST *you );
//011022 lsw >
extern void SendStartSkillExp( int inc, t_skillexp exp, t_connection c[], int cn );
//011022 lsw <
extern void RecvChangeScriptValue( t_packet *p, short int cn);
extern bool IsSameNation( CHARLIST *party_ch, CHARLIST *ch );
// 020428 YGI acer
extern int	BreakItemByDuration( ItemAttr *item, CHARLIST *ch, POS pos );
extern void SendItemAttrDurationChange( t_connection c[], int cn, int pos );
extern void SendChangeDurationByKein( POS *pos, DWORD dur, int cn );
extern void CheckBoxAndDownDuration( );
extern void SendCheckName( short int cn, char *name );		// 디비 데몬으로 캐릭터 존재 여부 요청 루틴
extern void SendCheckNameForNation( char *name, short int cn );
extern void RecvAndSendLoginCheckRegistCandidate( short int cn );
extern void CheckVote( short int cn );
extern void RecvAndSendCheckVote_Ok( t_packet *p, short int cn );
extern void RecvRegistCandidate( short int cn );
extern void RecvAndSendRegistCandidateResult( t_packet *p );
extern void RecvSetValueOfVote( t_packet *p );
extern void SendVoteGame2Login( t_packet *t, short int cn );
extern int ReturnCheckVoteNot( short int client_id, int ret );
extern void ReqVoteScore( short int cn );
extern void SendClientRepVoteScore( t_packet *p );
extern void RecvGetSkillInfo( t_packet *p, short int cn );
extern void SendSkillInfo( short int target_id, int top_exp, int top_job, short int cn ,const bool bCast= false);//021113 lsw
extern void RecvClienNationMassage( t_packet *p, short int cn );
extern void RecvAndSendNationMassage( t_packet *p );
extern void RecvDemonYouAreKing( t_packet *p, int type );
extern void RecvLoginYouAreKing( t_packet *p, int type );
extern void RecvGuildExit( t_packet *p, short int cn );		// 길드 탈퇴 명령
extern void RecvGuildJoin( t_packet *p, short int cn );
extern void RecvGuildJoinResult( t_packet *p );
extern void RecvGuildJoinConfirmOk( t_packet *p, short int cn );
extern void RecvGuildJoinConfirmNot( t_packet *p, short int cn );
extern void RecvScanTarget( short int cn );
extern void RecvGuildAllMsg( t_packet *p, short int cn );
extern void RecvGuildAllMsg_FromServer( t_packet *p );
extern void RecvChangeGuildEachLevel( t_packet *p, short int cn );
extern void SendBbsAll( char *name, int type );
extern void RecvGmRegist( t_packet *p, short int cn, int makemode = 0 );
extern void DirectSend( t_packet *p, int type = 0 );
extern void RecvReqStopWarGame2Login( t_packet *p, short int cn );
extern void RecvRepStopWarAccept( t_packet *p, short int cn );		// 승낙한 사실을 로그인 서버로 날려모두에게 글을 뿌려주게 한다.
extern void RecvRepStopWarRefusal( t_packet *p );
extern void RecvReqStopWarGame2Client( t_packet *p );		// 휴전할 상대국의 왕에게 의향을 물어본다.
extern void RecvRepStopWarGame2Client( t_packet *p );		// 모두에게 휴전의 사실을 보내준다.

extern void SendServerItemAttr( short int cn, int type, POS pos, ItemAttr *item );

extern void RecvAndReqSalvation( short int cn );
extern void RecvAndSaveSalvation( t_packet *p, short int cn );
extern void Game2ClinetSalvation( t_packet *p );
extern void RecvDB2MapItem( t_packet *p );

extern void RecvChangeMoney( t_packet *p, short int cn );
extern void SendEatChcolate( char *name, DWORD limits, DWORD cn  );
extern void RecvEatChocolateOk( t_packet *p );

extern void RecvPayRumor( short int cn );
extern void RecvAndSendRumor( t_packet *p );
extern void MovePc( short int cn, int x, int y);

extern void SendOpenSChoolData( int cn, int item_no, int npc_id );
extern void RecvOpenSchoolResult( t_packet *p );
extern void RecvGuildExitGlobal( t_packet *p, short int cn );

extern void RecvGuildExitGlobalFromDB( t_packet *p );

extern void SendEatCandy( char *name, DWORD limit, DWORD total_id );
extern inline void SendNationPacket( t_packet *p, int nation );

extern void RecvReqLoan( t_packet *p, short int cn );
extern void RecvLoanInfo( short int cn );
extern NPCLev_to_Exp		NPC_Lev_Ref[Num_Of_NPC_Lev];
extern void RecvRepayment( t_packet *p, short int cn );
extern void RecvMerchantBBS( t_packet *p, short int cn );

extern void RecvMerchantBBS( t_packet *p, short int cn );
extern void RecvMerchantBBSAllMap( t_packet *p, short int cn );
extern void RecvPartyAllFree( short int cn );
extern void InitJoinGameWho( CHARLIST *ch );		// 누군가 이 맵으로 들어왔다.		// 010410 YGI
extern void RecvMerchantItemBBS( t_packet *p, short int cn );
extern void RecvMerchantItemBBSAllMap( t_packet *p, short int cn );
extern int UpdateLadderScore( CHARLIST *ch );
extern void RecvReqRankLadder( t_packet *p, short int cn );
extern void RecvDirectClient( t_packet *p );

extern void RecvDirectDB( t_packet *p, short int cn );
extern inline int GetOwnPort();
extern void RecvReqVisa( t_packet *p, short int cn );
extern void RecvBuyVisa( t_packet *p, short int cn );

extern void RecvNationInfo( t_packet *p );		// 이 cn은 게임서버
extern inline int GetNationByMapName( char *mapname );
extern void RecvReqManToManRefusal( short int target_id, short int cn );
extern void RecvReqManToManRefusal( short int target_id, CHARLIST *ch );

extern void RecvExitGuildHimSelf( short int cn );		// 스스로 길드 탈퇴 명령어 // 010613 YGI
extern void RecvReqistGuildImsi( t_packet *p, short int cn );
extern void RecvReqistGuildImsiOk( t_packet *p );
extern void RecvReqistGuild( t_packet *p, short int cn );
extern void RecvSetGuildInfo( t_packet *p );
extern void RecvTest( t_packet *p, short int cn );


extern void RecvGuildInfo(t_packet *p, short int cn);
extern void SendGuildName(int guild_code, short int cn); // CSD-030326
extern void RecvGuildNumberInfo( t_packet *p );
extern void RecvGuildDegreeInfo( t_packet *p );
extern void RecvGuildIntroduction( short int cn );

extern void SendSaveGuildMemberList( char *target_name, int old_guild_code, int guild_code, int degree );
extern void RecvDb2ClientGuildMail( t_packet *p );

extern void KillWho( CHARLIST *death_ch, CHARLIST *attack_ch );

extern void TransDbGuildMailId( t_packet *p, short int cn );
extern void RecvGameToClientRepGuildMailTitle( int type, k_login_to_game_guild_mail_title *t );

extern void TransDB_DeleteGuildMail( DWORD mail_id, int cn );
extern void RecvCheckGuild( t_packet *p, short int cn );

extern void RecvGuildChatMap( t_packet *p );
extern void RecvGuildChat( t_packet *p, short int cn );
extern void RecvReqResetAbility( short int cn );
extern int SendCharInfo( short int cn );
extern void RecvDeleteGuild( t_packet *p, short int cn );
extern void RecvDeleteGuildOk( t_packet *p );
extern void RecvGuildDegreeInfo2( t_packet *p );
extern void RecvGetGhostQuestInfo( short int cn );

// 011019 ygi
extern void RecvSearchDualCharacter( t_packet *p, short int cn );
extern void RecvChangeColor( t_packet *p, short int cn );
extern inline int ChoiceParam( int *arg, int arg_count, int cmp_value );
extern inline void SendDirectMap( t_packet *p, short int port );
extern void RecvGetNationMoney( short int cn, int nation = 0 );


////////////////////////////////////////////////////////////
struct k_money_to_fame_basic
{
	DWORD money;
	int fame;
};

class CMoneyToFame	// 기부액에 따른 명성 증가치
{
	int m_nCount;
	k_money_to_fame_basic	data[10];
public : 
	CMoneyToFame() 
	{
		Refresh();
	}
	int Refresh()
	{
		memset( data, 0, sizeof( k_money_to_fame_basic ) );
		return 1;
	}
	int LoadData();
	int GetFame( DWORD money );
};

extern bool CheckContinueLint( char check );
extern CMoneyToFame g_Money_to_Fame;
extern int InitRefreshMenu();
extern int InitOnlyStart_kein();
extern void __stdcall MakeMapDataFile(DWORD dwValue);//020511 lsw;	// VK_F7
extern ItemAttr MakeGambleItem( int house, int grade, int item_no );

struct k_GambleRateDataBasic
{
	int rune2band[8];
	int lv2band[10];		// 레벨에 따른 밴드 더해짐수
	int choice_house[10][5];	// 방을 선택할 확률
	int house_band_class[5][11][6];
	int item2house_type[10][6];
	int give_gamble_item_rate[16][2];
	int map2band[6][2];
	int create_today_item[8];
	int get_today_item;
};

class CGambleRateData : public k_GambleRateDataBasic
{
public :
	int LoadData();
	int SaveData();
};

extern CGambleRateData g_game_rate_data;
extern void SaveGambleLogFile( CHARLIST *ch, int house, int class_type, int user_count, ItemAttr *item, int resource_type, int lv_band );
extern void RecvGambleItem( t_packet *p, short int cn );
extern int InitDataFromNewDay( int day, int month );
extern void RecvGambleStart( short int cn );
extern void SendMessageGambleItem( CHARLIST *ch, ItemAttr *item );
extern void RecvLogoutChar( t_packet *p, short int cn );
extern void SendPutMenuString( int type, int str_num, short int cn );
extern void SendDirectClientOtherMap( int type, short int server_id, short int port, void *msg, int size );
extern int RecvSEND_DIRECT_CLIENT_TO_MAP( t_packet *p );
extern void RecvReporterMode( t_packet *p, short int cn );

extern void SendPacket2Maps(t_packet* p);
extern void RecvDeleteTodayGambleItem();
extern void RecvGetTodayGambleItem( short int cn );
extern void RecvSetTodayGambleItem( t_packet *p );
extern void RecvChangePartySelectExp( t_packet *p, short int cn );
extern int WinOfRandFunction( int count );



struct SMapPotalUnit
{
	char m_MapFile[50];
	int m_MapNumber;
	int m_Money;
	int x;
	int y;
	int m_Level;

	SMapPotalUnit()
	{
		m_MapNumber = 0;
		m_MapFile[0] = 0;
		m_Money = 0;
		x = 0;
		y = 0;
		m_Level = 0;
	}
};


class CMapPotalNation
{
public :
	SMapPotalUnit m_MapNation[MAX_POTAL_MAP];
	int m_Count;

public :
	CMapPotalNation()
	{
		clear();
	}
	int clear() { m_Count = 0; return 1; }
	int InsertMap( char *map_name, int map_number, int money, int x, int y, int level );
	char *GetPotalMap( int num ) 
	{
		if( num < 0 || num >= MAX_POTAL_MAP ) return NULL;
		return m_MapNation[num].m_MapFile;
	}
	int GetCount() { return m_Count; } // 클라이언트 전송을 위해
	int GetMapNumber( int num )
	{
		if( num < 0 || num >= MAX_POTAL_MAP ) return 0;
		return m_MapNation[num].m_MapNumber;
	}
	int GetMapMoney( int num )
	{
		if( num < 0 || num >= MAX_POTAL_MAP ) return 0;
		return m_MapNation[num].m_Money;
	}
	int GetMapLevel ( int num )
	{
		if( num < 0 || num >= MAX_POTAL_MAP ) return 0;
		return m_MapNation[num].m_Level;
	}
	int GetMapXY( int num, int &x, int &y )
	{
		if( num < 0 || num >= MAX_POTAL_MAP ) return 0;
		x = m_MapNation[num].x;
		y = m_MapNation[num].y;
		return 1;
	}
};

extern CMapPotalNation g_MapPotal[MAX_NATION];
extern int LoadMapPotal();
extern void RecvGetPotalMap( short int cn );
extern void RecvGotoPotal( t_packet *p, short int cn );
extern void RecvSetMapType( t_packet *p, short int cn );
extern int SaveLogDefault( char *file_name, char *str, int is_first );
extern void SendSaveEventItem( CHARLIST *ch, int item_no, int count );
extern void RecvOXSelect( t_packet *p, short int cn );
extern void SetOXSelect( CHARLIST *ch, int type );
extern void RecvOXAnser( t_packet *p, short int cn );
extern int UserAllMapMove( int type, char *map_name, int x, int y );
extern void RecvCheckSkill( short int cn );
extern void RecvPutGuildItem( t_packet *p, short int cn );
extern void RecvGetGuildItemClient( t_packet *p, short int cn );
extern void RecvGetGuildItemMap( t_packet *p );
extern int CheckMapPort( int port );


extern char YearMeetingText[MAX_GOD][4][MAX_GOD_TEXT];
extern char MonthMeetingText[MAX_GOD][12][MAX_GOD_TEXT];


extern void RecvChangeGuildMaster( short int cn );
extern void SendChangeGuildDegree( int server_id, int guild_code, int guild_degree, short int cn );

extern void RecvChangeGuildMasterAllMap( t_packet *p );
extern int IsFightMap();

extern void RecvCheckGuildChange( t_packet *p, short int cn );


 // guild_degree_power
#define GDP_JOINING				0	// 길드 가입				
#define GDP_BBS					1	// 길드 전체 메시지			
#define GDP_CHANGE_DEGREE		2	// 길드원 직책변경			
#define GDP_GUILD_BOX			3	// 길드 창고 이용			
#define GDP_EXIT_GUILD_HIM		4	// 강제 길드 탈퇴 시키기	
#define GDP_CHANGE_MARK			5	// 길드 마크 변경			
#define GDP_CHANGE_DEGREE_INFO	6	// 길드 직책 명 변경		
#define GDP_GUILD_CHANGE_MASTER	7	// 길드 마스터 변경			
#define GDP_GUILD_BREAK			8	// 길드 제거				
#define GDP_REGIST_SUB_MASTER	9	// 부길마 등록				
#define GDP_EXIT_GUILD_SELF		10	// 스스로 길드 탈퇴			
#define GDP_BUY_GUILD_HOUSE		11	// 엠포리아 구입
#define GDP_MAX					11

extern bool CheckGuildPower( int type, CHARLIST *ch );
extern void RecvGetNationItem( short int cn );
extern void RecvBuyNationItem( short int cn );
extern void RecvCheckColossusMap( t_packet *p, short int cn );

#define ST_SEND_MAP_ALL	0		// 모든 맵에 보낼때
extern void SendPacket2Map( t_packet *p, int port );


extern void RecvInviteColossus( t_packet *p, short int cn );
extern void RecvInviteColossusReply( t_packet *p );
extern void RecvJoinColossusMap( t_packet *p, short int cn );

extern inline bool IsColleagueWhenColossus( CHARLIST *a, CHARLIST *d );
extern void RecvAttackColossusItem( t_packet *p, short int cn );
extern void CallMenu( short int menu, short int cn );
extern void RecvRevivalColossus( t_packet *p, short int cn );
extern void RecvGuildHouseInfo( t_packet *p, short int cn );
extern void RecvBuyGuildHouse( t_packet *p, short int cn );
extern int GetItemMuchByMuchItem( CHARLIST *ch, int item_no );
extern void RecvChangeGuildHouse( t_packet *p, short int cn );
extern void RecvCheckGuildHouse( t_packet *p, short int cn );


class CPotionBox
{
	/*
	enum POTION_TYPE
	{
		pt_red = 100,
		pt_blue = 101,
		pt_bread = 102
	};
	*/
	WORD item_no;
	WORD much;
	CHARLIST *hero;

	//POTION_TYPE box_type;
	int SetData( int data )
	{
		item_no = LOWORD( data );
		much = HIWORD( data );
		return 1;
	}
public:
/*	CPotionBox()
	{
		//box_type = 0;
		item_no = 0;
		much = 0;
	}*/
	CPotionBox( /*POTION_TYPE type,*/ CHARLIST *ch )
	{
		//box_type = type;
		int data;
		//switch( type )
		{
		//case pt_red : data = ch->arigement; break;
		//case pt_blue : data = ch->social_status; break;
		//case pt_bread : 
			data = ch->Sight; //break;
		}
		hero = ch;
		SetData( data );
	}
	
	~CPotionBox()
	{
		hero = NULL;
	}

	int UseItem();
	int DeleteItem();
	void SaveData();		// 아이템을 사용, 보관한후  저장한다.
	int GetData()
	{
		return hero->Sight;
	}
	int PutItem( ItemAttr *item );		// 아이템을 주머니속에 넣는다.
	void Empty();
};




extern void RecvPkOnOff( t_packet *p, short int cn );
extern void RecvCMD_GET_PK_ON_OFF( t_packet *p, short int cn );



// 020428 YGI acer
// SEND_ITEM_LOG_NAME2
#define SN_NOT_USER		(-1) 

enum SILT_Type
{
	SILT_PUT_OTHER_CH				=1,
	SILT_GET_OTHER_CH				=2,
	SILT_PUT_GROUND					=3,
	SILT_GET_GROUND					=4,
	SILT_PUT_BANK					=5,
	SILT_GET_BANK					=6,
	SILT_MAKE_BY_SKILL				=7,
	SILT_PUT_STORE					=8,
	SILT_GET_STORE					=9,
	SILT_DELETE_BY_DURATION			=10,
	SILT_DELETE_BY_ATTR				=11,		// 속성이 이상해서 지움
	SILT_DELETE_BY_RARE_ATTR		=12,		// 레어 속성이 이상해서 지움
	SILT_USE						=13,
	SILT_GET_OTHER_CH_INVEN			=14,		// 배낭 뒤지기로 뺏어 왔을때
	SILT_MAKE_BY_DB					=15,		// 이벤트로 넣어준 아이템
	SILT_MAKE_BY_SCRIPT				=16,		// 이벤트로 넣어준 아이템
	SILT_RESOURCE_BY_SKILL			=17,		// 스킬 사용시 재료로서 사라진다.
	SILT_GET_GUILD_WAR				=18,
	SILT_GET_GAMBLE_ITEM			=19,
	SILT_GET_TODAY_ITEM				=20,
	SILT_GET_STOLEN					=21,
	SILT_PUT_STOLEN					=22,
	SILT_PUT_GROUND_MAX_INVEN		=23,		// BBD 040226 주석추가: 인벤이 꽉차서 바닥에 떨굼, 돈일때만 쓰는듯...
	SILT_GET_MEN_TO_MEN				=24,
	SILT_PUT_MEN_TO_MEN				=25,
	SILT_NEED_ITEM					=26,
	SILT_PUT_GAMBLE_ITEM			=27,
	SILT_DROP_BY_PK					=28,
	SILT_DELETE_BY_SCRIPT			=29,
	SILT_MAKE_TO_GROUND_BY_SKILL	=30,
	SILT_PUT_GUILD_BOX				=31,
	SILT_GET_GUILD_BOX				=32,
	
	SILT_BOND_MONEY_GIVE			=33,
	SILT_TAKE_AUCTION_RESULT_ITEM	=34,
	SILT_REGISTER_AUCTION_ITEM		=35,
	SILT_EXCHANGE_BOND_MONEY		=36,
	SILT_MAKE_MULTI_RARE_RESOURCE	=37,
	SILT_MAKE_MULTI_RARE_RESULT		=38,
	SILT_CANCEL_AUCTION_SELL_ITEM	=39,

	SILT_BUY_NATION_ITEM_MONEY		=40,
	SILT_BUY_NATION_ITEM_FAME		=41,

	SILT_MAX						,
};


class C2JinSearch
{
public :
	int *item;
	int max;

public:
	C2JinSearch()
	{
		item = 0;
		max = 0;
	}
	~C2JinSearch(){ Remove(); }	
	int Load( char *filename );
	int Search( int no );
	int Sort();
	int Remove() 
	{ 
		SAFE_DELETE_ARRAY(item);
		max = 0;
		return 1;
	}
};

class CItemLog
{
public :
	int m_nDeleteDay;
	int m_nSaveLv;
	int m_nItemLv;

	C2JinSearch m_NoItemList;
	int m_aType2Lv[SILT_MAX];


public :
	CItemLog()
	{
		LoadDefault();
	}
	void LoadDefault()
	{
		m_nDeleteDay = 30;
		m_nSaveLv = 0;
		m_nItemLv = 90;
		for( int i=0; i<SILT_MAX; i++ )
		{
			m_aType2Lv[i] = 3;
		}
	}
	int GetDeleteDay() { return m_nDeleteDay; }
	int GetSaveLv() { return m_nSaveLv; }
	void Load( int port );
	bool IsSave( ItemAttr *item, int event_type, int lv );
	int GetTypeLv( int event_type ) { return m_aType2Lv[event_type]; }
};


extern int RecvSaveItemLogByEvent( t_packet *p );
extern void RecvDirectMapMove( t_packet *p, short int cn );
extern void SendItemEventLog( ItemAttr *item, short int hero_cn, short int target, int event_type, int lv ); //YGI acer
extern CItemLog	g_item_log;

// acer4
extern void RecvSearchChar( t_packet *p, short int cn );
extern int PutHimInfo( CHARLIST *him, k_char_info *him_info );
extern void RecvSearchCharMap( t_packet *p );

// 020620 YGI
extern void RecvUseGmTool_SaveLog( t_packet *p, short int cn );
const int MM_TODAY_GAMBLE_ITEM_MAIN_MAP_PORT	= 5190;
const int MM_SAVE_LOG_FILE_MAP					= 5999;
const int MN_SCENARIO_MAP						= 5720;		// 시나리오 관리맵

extern WORD FindMannagementMap( WORD port );
extern WORD GetManagementMapPort( WORD port );
extern void RecvEventNumber( t_packet *p, short int cn );
extern void MapServerFree_kein();
extern int LoadDungeonMapAll();
extern int RecvDungeonHistory( t_packet *p );
extern void SendCallSmallMenu( int cn, int storelistindex, int menu );
extern void SendCallDualMagicLearnMenu( int cn);//soto-030421
extern void	SendCallLuckyMenuOpen(int cn);//soto-030429
extern void RecvCallLuckyMenuOpen(int cn);//soto-030429
extern void EndManToManFight( CHARLIST *win, CHARLIST *lose );
extern void Fight_Stone_End(int cn, int flag);
extern bool IsManToManFight( CHARLIST *attack, CHARLIST *defence );
extern bool IsArenaFight(CHARLIST* pMember); // CSD-030506
extern void SaveChangeLadder( CHARLIST *ch, int old_ladder, int new_ladder, int type );
extern void SaveObtainCombat(CHARLIST* ch, int nOld, int nNew);
extern void SaveChangeCombat(CHARLIST *ch, int nCombat, int nOld, int nNew);

#define CLT_CHANGE_DUAL		1
#define CLT_WIN_COLOSSUS	2
#define CLT_LOSE_COLOSSUS	3

// 021107 YGI
extern void RecvSetInstallVersion ( t_packet *p, short int cn );	// 021117 YGI
extern void RecvResendItem( short int cn );//021030 YGI
extern void RecvScriptItem( t_packet *p, short int cn );
extern void RecvEventMoveMap( t_packet *p, short int cn );
extern void RecvEventMoveMapCheckMap( t_packet *p, short int cn );		// 다른 맵에서 올경우
extern void RecvEventMoveMapOk( t_packet *p, short int cn );

extern void CheckDoubleName( CHARLIST *ch, short int cn );
extern void RecvCheckDoubleName( t_packet *p, short int cn );
extern void RecvCheckDoubleNameOk( t_packet *p, short int cn );
extern void RecvCheckDoubleNameDB2Map( t_packet *p, short int cn );
extern void RecvCheckDoubleNameWhenFirst( t_packet *p, short int cn );
extern void RecvCheckDoubleNameWhenFirstOk( t_packet *p, short int cn );
extern void CheckDoubleNameWhenFirst( char *pName, short int server_id );
// 040105 YGI 보물
extern void RecvCheckTreasureXY( t_packet *p, short int cn );
extern void RecvGetTreasureMapDetail( t_packet *p, short int cn );
extern void RecvGetTreasureMapDetailDB( t_packet *p, short int cn );


extern int SaveLogDefault2( char *file_name, char *str );	// 030919 HK YGI

// 031009 CI YGI
extern int InitItemLimitCountFromDB( );



	/* SMP receiving function */
extern void RecvAskSkillPoint(t_packet *p, short int cn );

extern void RecvFriendStatus(t_packet *p); // finito 09/08/07
extern void RecvMantleDye( t_packet *p, short int cn ); // finito 25/06/08

extern bool IsNearStoreNPC(CHARLIST* ch, int npc_id, int server_id);

struct Store // finito 12/07/07
{
	int store_id;
	int event_no;
	int map_no;
	int map_x;
	int map_y;
};

extern std::vector<Store> g_Stores; // finito 12/07/07

extern void RecvMallItemBuy(int cn, t_MallItemBuy *p);
extern void SendMallItemPrice(int item_no, int cn);

struct MallItem
{
	ItemAttr itemAttr;	
	int price; // The amount of raja emblems it costs to buy the item
	int	time;  // Some mall items have a time limit on them
};

extern std::vector<MallItem> g_MallItems; // finito 12/07/07

extern void RecvApplicationOfExile( t_packet *p ); //Eleval 12/06/09
extern void RecvApplicationOfExileResult( t_packet *p, short int cn); //Eleval 12/06/09

extern void RecvSexChange( t_packet *p, short int cn); //Eleval 12/06/09 - Sex Change
extern void RecvSexChangeResult( t_packet *p); //Eleval 12/06/09 - Sex Change

extern void RecvToolPlaySound( t_packet *p, short int cn ); //Eleval 19/06/09 - Tool Play Sound
extern void RecvToolToggleSanta( t_packet *p, short int cn ); //Eleval 22/06/09 - To let GMs removing their Santa Power
extern void RecvLMSLvlRange( t_packet *p, short int cn ); //Eleval 18/07/09 - To set lvl range in LMS Arena
extern void RecvSetExpEvent( t_packet *p, short int cn ); //Eleval 22/07/09 - To set an Exp Event
extern void RecvSetExpEventAll( t_packet *p, short int cn); //Eleval 22/07/09 - To set and Exp Event on All maps
extern void RecvTogglePKTool( t_packet *p, short int cn ); //Eleval 24/07/09 - To change battle restrictions in a map

extern void RecvAskSQuestTeleport(t_packet *p); //Eleval 10/08/09 - To bring ppl back in quest
extern void RecvAskSQuestTeleportResult(t_packet *p); //Eleval 10/08/09 - To bring ppl back in quest
extern void RecvToolQuestCheck(t_packet *p, short int cn); //Eleval 13/08/09 - To check quest NPC
extern void RecvCharOptionsChange(t_packet *p, short int cn); //Eleval 18/08/09 - To change a char's options
extern void RecvTeleportToChar( t_packet *p, short int cn ); //Eleval 19/08/09 - Tool to teleport to a char
extern void RecvTeleportToCharMap( t_packet *p ); //Eleval 19/08/09 - Tool to teleprot to a char

struct PeopleInSQuest //Eleval 10/08/09 - To bring ppl back in quest
{
	char charname[32];
	int questfloor;
	int nbMap;
	int nY;
	int nX;
};

extern void UpdateSQuestFloor(int nbMap, char *charname, int QuestFloor, int nX, int nY, short int cn); //Eleval 10/08/09 - To bring ppl back in quest
extern void CheckIfBringableInSQuest(int nbMap, char *charname, short int ClientNumber, short int cn); //Eleval 10/08/09 - To bring ppl back in quest

extern void RecvReqHonorAmount( t_packet *p, short int cn ); //Eleval 07/09/09 - To request the player's current honor

#endif