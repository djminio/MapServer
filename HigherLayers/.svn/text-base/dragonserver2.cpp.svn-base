#include "..\stdafx.h"
#include "DefaultHeader.h"
#include "Scrp_int.h"
#include "Scrp_Exe.h"
#include "NPC_Pattern.h"
#include "map.h"
#include "CItem.h"
#include "teambattle.h"		// 010210 YGI
#include "dungeon.h"		// 020620 YGI
#include "ArenaManager.h"
#include "UserManager.h"

#include "WeatherControl.h" //Eleval 13/08/09
// 020620 YGI
extern int MenuFunctions( t_packet *p, short int cn );


int CheckHandleByKein( t_packet *p, t_connection c[], int cn )
{
	switch (p->h.header.type)
	{
	case CMD_RESEND_ITEM:
		{
			RecvResendItem(cn);
			break;
		}						
	case CMD_RESEND_ITEM_EQUIP :	SendItemEquip( c, cn ); break;
	case CMD_RESEND_ITEM_QUICK :	SendItemQuick( c, cn );	break;
		
	case CMD_PUBLIC_NOTICE_TITLE_REQ	:	
	case CMD_RANK_TITLE_REQ				:
	case CMD_FORUM_TITLE_REQ			:
	case CMD_EVENT_TITLE_REQ			:
	case CMD_MYDIARY_TITLE_REQ			:
	case CMD_BBS_TITLE_REQ				:
	case CMD_ETC_TITLE_REQ				:	SendMessageTitle( p->h.header.type, p->u.kein.req_bbs_title.count, c, cn ); break;
		
	case CMD_RESEND_CHARLIST			:	SendCharInfomation( p->h.header.type, c, cn ); break;
	case CMD_OTHER_CH_INV				:	break;
	case CMD_DIVIDE_ITEM				:	RecvDivideItem( &p->u.kein.item_attr_pos, c, cn ); break;
	case REQ_MY_DEPOSIT_INFO			:	RecvBankInfoDeposit(p->u.kein.client_bank_deposit.money, c, cn ); break;
	case CMD_DEFRAYAL					:	RecvBankInfoDefrayal(p->u.kein.client_bank_deposit.money, c, cn ); break;
	case REQ_MY_REPAYMENT_INFO			:	SendBankRepayment( c, cn ); break;
	case CMD_CHAR_INFO_BASIC			:	RecvCharInfoBasic( &p->u.char_info_basic , c, cn ); break;
	case CMD_LEARN_SKILL				:	RecvLearnSkill( &p->u.client_learn_skill, c, cn );	break;
	case CMD_LVUP_POINT_EACH			:	RecvLvUpPointEach( p->u.kein.client_lvup_point_each.type, c, cn );	break;
	case CMD_LVDOWN_POINT_EACH			:	RecvLvDownPointEach( p->u.kein.client_lvup_point_each.type, c, cn );	break;
	case REQ_LEVELUP_POINT:	
		{
			c[cn].chrlst.IncReservedPoint(10); 
			SendLevelUpMsgAndPoint(&c[cn].chrlst); 
			break;
		}
	case CMD_LEARN_ITEM					:	RecvLearnItem( &p->u.kein.client_learn_item, c, cn ); break;
	case CMD_ITEMREPAIR_REQ				:	RecvItemRepair( &p->u.client_item_sell, c, cn ); break;
	case CMD_ITEMREPAIR_HAMMER_REQ		:	RecvItemRepairHammer( &p->u.client_item_sell, c, cn ); break;
	case CMD_MAGIC_ITEM					:	RecvMagicItem( &p->u.client_item_sell, c, cn ); break;

	case CMD_CHAR_INFO_TAC_SKILL_EXP	:	SendCharInfotac_skillEXP( c, cn ); break;
	case CMD_OPEN_STATUS_REQ			:	SendStatusOpen( c, cn ); break;
	case CMD_REQ_CHAR_INFO_PLUS			:	RecvCharInfoPlus( p->u.kein.client_char_info_plus.type, c, cn ); break;
	case CMD_SEND_BANK_INV				:	{for( int i=0; i<15; i++ ) SendBankInventory( i, c, cn ); break; }
	case CMD_DO_PARTY					:	RecvDoParty( &p->u.kein.client_do_party, c, cn ); break;
	case CMD_DELETE_PARTY				:	RecvDeleteParty( &p->u.kein.client_delete_party, c, cn ); break;
	case CMD_REQ_PARTY_MEMBER			:	break; RecvCharInfoPlus(CMD_PARTY , c, cn); break;		// 일단 빼고 생각
	case CMD_PREVENTING					:	RecvPreventing( p->u.kein.client_disease.disease_num, c, cn ); break;
	case CMD_CURE_DISEASE				:	RecvCureDisease( p->u.kein.client_disease.disease_num, c, cn );break;
	case CMD_REQ_ITEM_VALUE				:	SendItemValue( p->u.kein.client_item_no.item_no, cn );	break;
	case CMD_REQ_ITEM_VALUE_SELL		:	SendItemValue_Sell( &p->u.kein.server_delete_item.item_pos, cn ); break;
	case CMD_REQ_ITEM_VALUE_REPAIR		:	SendItemValue_Repair( p->u.kein.client_item_no.item_no, cn ); break;
	case CMD_REQ_MALL_ITEM_PRICE		:	SendMallItemPrice( p->u.kein.client_item_no.item_no, cn ); break;
	case CMD_CLIENT_INN					:	RecvClientInn( p->u.kein.client_inn.type, cn ); break;
	case CMD_SERVER_PARTY:
		{
			CHARLIST* ch = CheckServerId(cn);

			if (!ch)
			{
				break;
			}

			SendPartyInfo(p->u.kein.client_delete_party.type, 
				          p->u.kein.client_delete_party.ct, 
						  c, 
						  ch->Name );
			break;
		}
		
	case CMD_SERVER_WAKE_UP				:	SendServerWeakUp( cn ); break;
	case CMD_REQ_JOIN_GAME				:	SendJoinGameWho( cn ); CheckMyMailOnStartGame( cn ); break;
	case CMD_REQ_MY_PARTY				:	SendMyPartyExist( cn ); break;
	case CMD_OPEN_ABILITY				:	SendAbilityOpen( cn ); break;
	case CMD_LEARN_ALL					:	RecvLearnAll( cn ); break;			
	case CMD_LEARN_ALL_NOT				:	RecvLearnAllNot( cn ); break;
	case CMD_REQ_AC						:	SendServerAc( cn ); break;
	case CMD_REQ_DMG					:	SendServerDmg( cn ); break;
	case CMD_SERVER_KILLMON				:	SendServerKillMon( cn ); break;
	case CMD_PARTY_ACCESS				:	RecvPartyInfoServerToServer( &p->u.kein.server_server_party_info ); break;		// loginserver에서 전송받는다.
	case CMD_REQ_PARTY_TOGETHER			:	RecvPartyTogetherByLoginServer( p->u.kein.rep_party_together.server_id, p->u.kein.rep_party_together.party_name ); break;
	case CMD_PARTY_NO_NAME				:	RecvNoParty( p->u.kein.cmd_party_no_name.name, p->u.kein.cmd_party_no_name.server_id ); break;	//잘 못된 이름을 가지고 있을때... 지운다.
	case CMD_REQ_MAGERY					:	SendServerMagery( cn ); break;
	case CMD_PUBLIC_NOTICE_BBS_REQ		:	
	case CMD_RANK_BBS_REQ				:
	case CMD_FORUM_BBS_REQ				:
	case CMD_EVENT_BBS_REQ				:
	case CMD_MYDIARY_BBS_REQ			:
	case CMD_BBS_BBS_REQ				:
	case CMD_ETC_BBS_REQ				:	RecvBbsMessageReq( p->h.header.type, p->u.kein.req_bbs_title.count, cn ); break;// 내용 전송 요청 응답
	case CMD_PREACH_NAME				:	SendLoginServerPreachName( &p->u.kein.server_preach_name, cn ); break;
	case CMD_LOGIN_PREACH_NAME			:	RecvAndSendPreachName( &p->u.kein.loginserver_preach_name_result ); break;
	case CMD_GOD_REGIST_OK				:	RecvGodRegistOk( p->u.kein.client_god_regist_ok.type, cn ); break;
	case CMD_GOD_REGIST_CANCEL			:	RecvGodRegistCancel( cn ); break;
	case CMD_PREACH_NAME_OK				:	SendLoginServePreachNameOk( &p->u.kein.server_preach_name, cn ); break;
	case CMD_TOOL_PASSWORD				:	RecvToolPassword( p->u.kein.tool_password.password , cn); break;
	case CMD_GET_CHARINFO_BY_KEIN		:	RecvLoginServerGetCharInfoByKein( &p->u.kein.get_char_info ); break;
	case CMD_CAN_MEMORIZE				:	RecvCanMemorize( c, cn ); break;		// 메모라이즈 할 수 있는지 판단
	case CMD_SEND_EAR_MESSAGE			:	RecvEarMessage( p->u.kein.send_ear_message.name, p->u.kein.send_ear_message.message, cn ); break;	//1
	case CMD_SEND_EAR_MESSAGE_OTHER		:	FindWhoEarMessageTarget( &p->u.kein.login_to_gameserver_ear_message ); break;		// 귀속말을 받을 사람이 이 게임 서버에 존재하는지 판단
	case CMD_SEND_EAR_MESSAGE_OK		:	SendEarMessageResult( CMD_SEND_EAR_MESSAGE_OK, p->u.kein.loginserver_to_gameserver_ear_message.name, p->u.kein.loginserver_to_gameserver_ear_message.server_id); break;
	case CMD_SEND_EAR_MESSAGE_RESULT	:	SendLoginEarMessageResult( &p->u.kein.send_ear_message_result, cn ); break;		// 클라이언트에서 받은 메시지 보낸 결과
	case CMD_LOGIN2GAME_EAR_MESSAGE_RESULT	:	RecvLoginToGameEarMessageResult( p->u.kein.login2game_ear_message_result.send_server_id, p->u.kein.login2game_ear_message_result.ret ); break;		// 회신이 드뎌 왔다.
	case CMD_ISTHERE_CHARNAME				:	SendGameToLoginExistName( p->u.client_isthere_charname.name, cn ); break;
	case CMD_GAME2LOGIN_CHECK_NAME			:	CallClient( p->u.kein.login2game_ear_message_result.send_server_id, p->u.kein.login2game_ear_message_result.send_server_id ); break;
	case CMD_REQ_SKILL_ALL					:	SendSkillAll( cn ); break;
	case CMD_REQ_FAITH_UP					:	RecvAndSendFaithUp( p->u.kein.req_faith_up.name, p->u.kein.req_faith_up.up_point ); break;
	case CMD_DIVINE_UP						:	RecvDivineUp( cn ); break;
	case CMD_OPEN_SUPPER					:	RecvOpenSupper( cn ); break;
	case CMD_I_GET_ITEM_OF_SUPPER			:	RecvGetItemOfSupper( p->u.kein.i_get_item_of_supper.item_no, cn ); break;
	case CMD_CHANGE_MAP_CONFIRM				:	RecvChangeMapConfirm( cn ); break;
	case CMD_REQ_CHAR_INFO					:	RecvReqCharInfo( cn, p->u.debug_client_req_char_info.id );	break;
	case CMD_OH_MY_PARTY					:	RecvOhMyParty( p->u.kein.oh_my_party.my_name, p->u.kein.oh_my_party.party_name ); break;
	case CMD_REQ_PARTY_INFO					:	RecvRepPartyInfo( p->u.kein.req_party_info.party_name, cn ); break;
	case CMD_LOGIN_TO_GAME_PUBLIC_BBS:
		{	//< CSD-CN-031213
			g_pUserManager->SendPacket(p);
			break;
		}	//> CSD-CN-031213
	case CMD_START_GAME_MAIL				:	CallClient( p->u.kein.server_id.server_id, CMD_START_GAME_MAIL ); break;
	case CMD_CALL_PUBLIC_BBS				:	SendToLoginserver( cn ); break;		// 로그인 서버로 그냥 보낸다.
	case CMD_CALL_PUBLIC_BBS_ONLY_ONE		:	SendLoginToClientPublicBbsOnlyOne( p ); break;
	case CMD_HUNTER_REGIST					:	SendLoginServerHunterRegist( &p->u.kein.hunter_regist, p->h.header.size, cn ); break;
	case CMD_GET_HUNTER_LIST				:	SendLoginServerReqHunterList( p->u.kein.get_hunter_list.list_id, cn ); break;
	case CMD_SEND_HUNTER_LIST				:	RecvLoginServerAndSendHunterList( &p->u.kein.login_game_hunter_list ); break;
//	case CMD_KILL_HUNTER_LIST				:	RecvHunterList( p->u.kein.get_hunter_list.list_id, cn ); break; finito 15/08/07 client does not send this packet anymore!
	case CMD_KILL_HUNTER_LIST_OK			:	RecvHunterListOk( &p->u.kein.send_kill_hunter_list ); break;
	case CMD_KILL_HUNTER_LIST_CANCEL		:	CallClient( p->u.kein.server_id.server_id, CMD_KILL_HUNTER_LIST_CANCEL ); break;
	case CMD_KILLED_BY_HUNTER				:	ReturnResultByName( p->u.kein.kill_who.name, CMD_KILLED_BY_HUNTER ); break;	
	case CMD_DANGER_OF_HUNTER				:	ReturnDangerByHunter( p->u.kein.danger_of_hunter.target_id, p->u.kein.danger_of_hunter.type); break;
	case CMD_REQ_MAN_TO_MAN					:	RecvReqManToMan( p->u.kein.default_short_int, cn ); break;
	case CMD_REQ_MAN_TO_MAN_RESULT			:	SendReqManToManResult( p->u.kein.req_man_to_man_result.ret, p->u.kein.req_man_to_man_result.id ); break;
	case CMD_REQ_MAN_TO_MAN_CONSENT			:	RecvManToManConsent( p->u.kein.default_short_int, cn ); break;
	case CMD_REQ_MAN_TO_MAN_REFUSAL			:	RecvReqManToManRefusal( p->u.kein.default_short_int, cn ); break;		
	case CMD_REQ_QUEST_INDEX				:	RecvReqQuestIndex( p->u.kein.default_char, cn ); break;
	case CMD_SCRIPT_CHANGE					:	RecvChangeScriptValue( p, cn); break;
	case CMD_GUILD_EXIT_COMMAND_GLOBAL_OK	:
	case CMD_GUILD_EXIT_COMMAND_GLOBAL_NOT	:
	case CMD_VOTE_NOT						:
	case CMD_VOTE_OK						:
	case CMD_REGIST_CANDIDATE_OK			:
	case CMD_THIS_MAN_IS_CANDIDATE_OK		:	
	case CMD_THIS_MAN_IS_CANDIDATE_NOT		:
	case CMD_THEREIS_CHARNAME				:	
	case CMD_THEREIS_NO_CHARNAME			:	CallClient( p->u.kein.default_short_int, p->h.header.type ); break;
	case CMD_CHECK_VOTE_NOT					:	ReturnCheckVoteNot( p->u.kein.default_short_int, -3 ); break;		// 이미 선거 했다.
	case CMD_ISSAME_MY_NATION				:	SendCheckNameForNation( p->u.kein.default_name, cn ); break;
	case CMD_IS_THIS_MAN_CANDIDATE			:	RecvAndSendLoginCheckRegistCandidate( cn ); break;
	case CMD_CHECK_VOTE						:	CheckVote( cn ); break;
	case CMD_CHECK_VOTE_OK					:	RecvAndSendCheckVote_Ok( p, cn ); break;
	case CMD_REGIST_CANDIDATE				:	RecvRegistCandidate( cn ); break;	// 서버로 등록할수 있는지를 보낸다.
	case CMD_REGIST_CANDIDATE_NOT			:	RecvAndSendRegistCandidateResult( p ); break;
	case CMD_SET_VALUE_OF_VOTE				:	RecvSetValueOfVote( p ); break;		// 디비 데몬에게서 투표관련 변수 값을 가져온다.
	case CMD_RESET_VOTE_VALUE				:	CallClient( DB_DEMON, CMD_SET_VALUE_OF_VOTE ); break;	// 환경 다시 읽기
	case CMD_VOTE_GAME2LOGIN				:	SendVoteGame2Login( p, cn ); break;
	case CMD_REQ_VOTE_SCORE					:	ReqVoteScore( cn); break;
	case CMD_REP_VOTE_SCORE					:	SendClientRepVoteScore( p ); break;
	case CMD_GET_SKILL_INFO					:	RecvGetSkillInfo( p, cn ); break;
	case CMD_SEND_ALL_MSG					:	RecvClienNationMassage( p, cn ); break;	// 클라이언트에서 받아서 로그인 서버로
	case CMD_RECV_ALL_MSG					:	RecvAndSendNationMassage( p ); break;	// 로그인서버에서 받아서 클라이언트로
	case CMD_YOU_ARE_KING_LOG2LOG			:	RecvDemonYouAreKing( p, CMD_YOU_ARE_KING_LOG2CLIENT ); break;		// 5000번 포트로 보내버린다.
	case CMD_YOU_ARE_KING_LOG2CLIENT		:	RecvLoginYouAreKing( p, CMD_YOU_ARE_KING_LOG2CLIENT ); break;		// 찾아서 있으면 클라이언트로 보내준다.
	case CMD_YOU_ARE_NO_KING_LOG2LOG		:	RecvDemonYouAreKing( p, CMD_YOU_ARE_NO_KING_LOG2CLIENT ); break;
	case CMD_YOU_ARE_NO_KING_LOG2CLIENT		:	RecvLoginYouAreKing( p, CMD_YOU_ARE_NO_KING_LOG2CLIENT ); break;
	case CMD_GUILD_EXIT_COMMAND				:	RecvGuildExit( p, cn ); break;
	case CMD_GUILD_JOIN_SERVER				:	RecvGuildJoin( p, cn ); break;
	case CMD_GUILD_JOIN_SERVER_OK			:	
	case CMD_GUILD_JOIN_SERVER_NOT			:	RecvGuildJoinResult( p ); break;
	case CMD_GUILD_JOIN_CONFIRM_OK			:	RecvGuildJoinConfirmOk( p, cn ); break; 
	case CMD_GUILD_JOIN_CONFIRM_NOT			:	RecvGuildJoinConfirmNot( p, cn ); break; 
	case CMD_SCAN_TARGET					:	RecvScanTarget( cn ); break;
	case CMD_GUILD_ALL_MSG					:	RecvGuildAllMsg( p, cn ); break;
	case CMD_GUILD_ALL_MSG_RECV				:	RecvGuildAllMsg_FromServer( p ); break;
	case CMD_GUILD_EACH_LEVEL				:	RecvChangeGuildEachLevel( p, cn ); break;
	case CMD_GM_REGIST_GAMEMAKEMODE			:	RecvGmRegist( p, cn, 1 ); break;
	case CMD_GM_REGIST2						:	
	case CMD_GM_REGIST						:	RecvGmRegist( p, cn ); break;
	case CMD_BBS_ALL						:	DirectSend( p ); break;
	case CMD_COME_OUT_GM_FROM_LOGIN			:	DirectSend( p, p->h.header.type ); break;
	case CMD_REQ_STOP_WAR_TO_LOGIN			:	RecvReqStopWarGame2Login( p, cn ); break;
	case CMD_REQ_STOP_WAR_TO_CLIENT			:	RecvReqStopWarGame2Client( p ); break;		// 로그인서버에서 받은 형태
	case CMD_REP_STOP_WAR_REFUSAL_TO_LOGIN	:	RecvRepStopWarRefusal( p ); break;		// 휴전 요청이 거절당했다.
	case CMD_REP_STOP_WAR_REFUSAL_TO_CLIENT : 	CallClient( p->u.kein.default_short_int, CMD_REP_STOP_WAR_REFUSAL_TO_CLIENT ); break;		// 클라이언트로 보내준다.
	case CMD_REP_STOP_WAR_ACCEPT_TO_LOGIN	:	RecvRepStopWarAccept( p, cn ); break;		// 휴전 요청 승낙
	case CMD_REP_STOP_WAR_ACCEPT_TO_CLIENT	:	RecvRepStopWarGame2Client( p ); break;		// 모두에게 뿌려주자
	case CMD_REQ_SALVATION_MONEY			:	RecvAndReqSalvation( cn ); break;			// 클라이언트에서 db로
	case CMD_SALVATION_MONEY				:	RecvAndSaveSalvation( p, cn ); break;		// 클라이언트에서 db로
	case CMD_DB2GAME_SALVATION				:	Game2ClinetSalvation( p ); break;
	case CMD_MSG_ALL_SALVATION_TOTAL_MONEY	:	DirectSend( p ); break;
	case CMD_SERVER_EACH_ITEM				:	RecvDB2MapItem( p ); break;
	case CMD_CHANGE_MONEY					:	RecvChangeMoney( p, cn ); break;
	case CMD_EAT_CHOCOLATE_OK				:	RecvEatChocolateOk( p ); break;
	case CMD_PAY_RUMOR						:	RecvPayRumor(cn); break;
	case CMD_PAY_RUMOR_OK					:	RecvAndSendRumor( p ); break;
	case CMD_OPENSCHOOL_DATA_OK				:	RecvOpenSchoolResult( p ); break;
	case CMD_GUILD_EXIT_COMMAND_GLOBAL		:	RecvGuildExitGlobal( p, cn ); break;
	case CMD_GUILD_EXIT_COMMAND_GLOBAL_LET	:	RecvGuildExitGlobalFromDB( p ); break;
	case CMD_EAT_CANDY_OK:
		{	//< CSD-CN-031213
			g_pUserManager->SendPacket(p);
			break;
		}	//> CSD-CN-031213
	case CMD_REQ_LOAN						:	RecvReqLoan( p, cn ); break;
	case CMD_REQ_LOAN_INFO					:	RecvLoanInfo( cn ); break;
	case CMD_REQ_REPAYMENT					:	RecvRepayment( p, cn ); break;
	case CMD_ARENA_ENTER_LOBBY:
		{	//< CSD-030506
			g_pArenaManager->RecvEnterLobby(cn, &p->u.arena.arena_enter_lobby);
			break;
		}	//> CSD-030506
	case CMD_ARENA_LEAVE_LOBBY:
		{	//< CSD-030506
			g_pArenaManager->RecvLeaveLobby(cn);
			break;
		}	//> CSD-030506
	case CMD_ARENA_ENTER_TEAM:
		{	//< CSD-030506
			g_pArenaManager->RecvEnterTeam(cn, &p->u.arena.arena_enter_team);
			break;
		}	//> CSD-030506
	case CMD_ARENA_LEAVE_TEAM:
		{	//< CSD-030506
			g_pArenaManager->RecvLeaveTeam(cn, &p->u.arena.arena_leave_team);
			break;
		}	//> CSD-030506
	case CMD_ARENA_READY_GAME:
		{	//< CSD-030521
			g_pArenaManager->RecvReadyGame(cn, &p->u.arena.arena_ready_game);
			break;
		}	//> CSD-030521
	case CMD_ARENA_START_GAME:
		{	//< CSD-030506
			g_pArenaManager->RecvStartGame(cn);
			break;
		}	//> CSD-030506		
	case CMD_ARENA_ENTER_OBSERVE:
		{	//< CSD-030509
			g_pArenaManager->RecvEnterObserve(cn, &p->u.arena.arena_enter_observe);
			break;
		}	//> CSD-030509
	case CMD_ARENA_ENTER_GAMBLING:
		{	//< CSD-030509
			g_pArenaManager->RecvEnterGambling(cn, &p->u.arena.arena_enter_gambling);
			break;
		}	//> CSD-030509
	case CMD_MERCHANT_BBS					:	RecvMerchantBBS( p, cn ); break;
	case CMD_MERCHANT_BBS_ALL				:	RecvMerchantBBSAllMap( p, cn ); break;
	case CMD_MERCHANT_BBS_MAP_TO_MAP:	
		{	//< CSD-031213
			p->h.header.type = CMD_MERCHANT_BBS;		// 타입만 변환하여 모든 클라이언트로 보낸다.
			g_pUserManager->SendPacket(p);
			break;
		}	//> CSD-031213
	case CMD_PARTY_ALL_FREE					:	RecvPartyAllFree( cn ); break;
	case CMD_MERCHANT_ITEM_BBS_ALL			:	RecvMerchantItemBBSAllMap( p, cn ); break;
	case CMD_MERCHANT_ITEM_BBS				:	RecvMerchantItemBBS( p, cn ); break;
	case CMD_MERCHANT_ITEM_BBS_MAP_TO_MAP:
		{	//< CSD-031213
			p->h.header.type = CMD_MERCHANT_ITEM_BBS;
			g_pUserManager->SendPacket(p);
			break;
		}	//> CSD-031213
	case CMD_REQ_RANK_LADDER				:	RecvReqRankLadder( p, cn ); break;
	case CMD_DIRECT_CLIENT					:	RecvDirectClient( p ); break;
	case CMD_DIRECT_CLIENT_TO_DB			:	RecvDirectDB( p, cn ); break;
	case CMD_TODAY_ITEM_GET_WHO				:	RecvDeleteTodayGambleItem();// 아래 루틴 수행
	case CMD_OX_SET_TIME					:
	case CMD_OX_QUESTION					:	
	case CMD_PUT_MENU_STRING				:
	case CMD_SEND_SOUND:
		{	//< CSD-031213
			g_pUserManager->SendPacket(p);
			break;
		}	//> CSD-031213
	case CMD_REQ_VISA						:	RecvReqVisa( p, cn ); break;
	case CMD_BUY_VISA						:	RecvBuyVisa( p, cn ); break;
	case CMD_NATION_INFO_MAP_TO_MAP			:	RecvNationInfo( p ); break;
	case CMD_EXIT_GUILD_SELF				:	RecvExitGuildHimSelf( cn ); break;		// 010613 YGI
	case CMD_REGIST_GUILD					:	RecvReqistGuild( p, cn ); break;
	case CMD_REGIST_GUILD_IMSI				:	RecvReqistGuildImsi( p, cn ); break;
	case CMD_REGIST_GUILD_IMSI_OK			: 	RecvReqistGuildImsiOk( p ); break;
	case CMD_REGIST_GUILD_IMSI_OK_ALL		:	RecvSetGuildInfo( p ); break;
	case CMD_TEST							:	RecvTest( p, cn ); break;	
	case CMD_GUILD_INFO1_REQ				:	RecvGuildInfo( p, cn ); break;
	case CMD_GUILD_NUMBER_INFO_DB_TO_MAP	:	RecvGuildNumberInfo( p ); break;
	case CMD_GUILD_DEGREE_INFO			:	RecvGuildDegreeInfo( p ); break;		// 정식 길드 등록 성공
	case CMD_GUILD_DEGREE_INFO2			: RecvGuildDegreeInfo2( p ); break;		// 직책 변경
	case CMD_REQ_GUILD_INTRODUCTION			:	RecvGuildIntroduction( cn ); break;
	case CMD_UPDATE_GUILD_MAIL_ID			:	TransDbGuildMailId( p, cn ); break;
	case CMD_CHECK_MY_GUILD					:	RecvCheckGuild( p, cn ); break;
	case CMD_GUILD_CHAT_MAP					:	RecvGuildChatMap( p ); break;
	case CMD_GUILD_CHAT						:	RecvGuildChat( p, cn ); break;
	case CMD_REQ_RESET_ABILITY				:	RecvReqResetAbility( cn ); break;
	case CMD_DELETE_GUILD					:	RecvDeleteGuild( p, cn ); break;
	case CMD_DELETE_GUILD_OK				:	RecvDeleteGuildOk( p ); break;
	case CMD_GET_GHOST_QUEST_INFO			:	RecvGetGhostQuestInfo( cn ); break;
	case CMD_SHOW_ME_DUAL_CHAR				:	RecvSearchDualCharacter( p, cn ); break;
	case CMD_CHANGE_COLOR					:	RecvChangeColor( p, cn ); break;
	case CMD_GET_NATION_MONEY				:	RecvGetNationMoney( cn, p->u.kein.default_char ); break;
	case CMD_GAMBLE_ITEM					:	RecvGambleItem( p, cn ); break;
	case CMD_GAMBLE_START					:	RecvGambleStart( cn ); break;
	case CMD_SEND_LOGOUT_CHAR				:	RecvLogoutChar( p, cn ); break;
	case CMD_SEND_DIRECT_CLIENT_TO_MAP		:	RecvSEND_DIRECT_CLIENT_TO_MAP( p ); break;
	case CMD_REPORTER_MODE					:	RecvReporterMode( p, cn ); break;
	case CMD_SET_TODAY_GAMBLE_ITEM			:	RecvSetTodayGambleItem( p ); break;
	case CMD_DELETE_TODAY_GAMBLE_ITEM		:	RecvDeleteTodayGambleItem(); break;
	case CMD_GET_TODAY_GAMBLE_ITEM			:	RecvGetTodayGambleItem( cn ); break;
	case CMD_CHANGE_PARTY_SELECT_EXP		:	RecvChangePartySelectExp( p, cn ); break;
	case CMD_GOTO_POTAL						:	RecvGotoPotal( p, cn ); break;
	case CMD_SET_MAP_TYPE					:	RecvSetMapType( p, cn ); break;
	case CMD_OX_SELECT						:	RecvOXSelect( p, cn ); break;
	case CMD_OX_ANSER						:	RecvOXAnser( p, cn ); break;
	case CMD_CHECK_SKILL					:	RecvCheckSkill( cn ); break;
	case CMD_PUT_GUILD_ITEM					:	RecvPutGuildItem( p, cn ); break;
	case CMD_GET_GUILD_ITEM_CLIENT			:	RecvGetGuildItemClient( p, cn ); break;
	case CMD_GET_GUILD_ITEM_MAP				:	RecvGetGuildItemMap( p ); break;
	case CMD_CHANGE_GUILD_MASTER			:	RecvChangeGuildMaster( cn ); break;
	case CMD_CHANGE_GUILD_MASTER_ALL_MAP	:	RecvChangeGuildMasterAllMap( p ); break;
	case CMD_CHECK_CHANGE_GUILD_INFO		:	RecvCheckGuildChange( p, cn ); break;
	case CMD_GET_NATION_ITEM				:	RecvGetNationItem( cn ); break;
	case CMD_BUY_NATION_ITEM				:	RecvBuyNationItem( cn ); break;
	case CMD_CHECK_COLOSSUS_MAP				:	RecvCheckColossusMap( p, cn ); break;
	case CMD_INVITE_COLOSSUS				:	RecvInviteColossus( p, cn ); break;
	case CMD_INVITE_COLOSSUS_REPLY			:	RecvInviteColossusReply( p ); break;
	case CMD_JOIN_COLOSSUS_MAP				:	RecvJoinColossusMap( p, cn ); break;
	case CMD_ATTACK_COLOSSUS_STONE			:	RecvAttackColossusItem( p, cn ); break;
	case CMD_REVIVAL_COLOSSUS				:	RecvRevivalColossus( p, cn ); break;
	case CMD_GUILD_HOUSE_INFO				:	RecvGuildHouseInfo( p, cn ); break;
	case CMD_BUY_GUILD_HOUSE				:	RecvBuyGuildHouse( p, cn ); break;
	case CMD_CHANGE_GUILDHOUSE_INFO			:	RecvChangeGuildHouse( p, cn ); break;
	case CMD_CHECK_GUILD_HOUSE				:	RecvCheckGuildHouse( p, cn ); break;
	case CMD_RELOAD_GUILD_HOUSE				:	LoadGuildHouse(); break;
	case CMD_PK_ON_OFF						:	RecvPkOnOff( p, cn ); break;
	case CMD_GET_PK_ON_OFF					:	RecvCMD_GET_PK_ON_OFF( p, cn ); break;
	case CMD_SAVE_ITEM_LOG					:	RecvSaveItemLogByEvent( p ); break;
	case CMD_DIRECT_MAP_MOVE				:	RecvDirectMapMove( p, cn ); break;
	case CMD_SEARCH_OTHER_CHAR					:	RecvSearchChar( p, cn ); break;
	case CMD_SEARCH_OTHER_CHAR_MAP				:	RecvSearchCharMap( p ); break;
	case CMD_USE_GM_TOOL					:	RecvUseGmTool_SaveLog( p, cn ); break;
	case CMD_SERVER_KILLPC				:	break;
	case CMD_SEND_EVENT_NO					:	RecvEventNumber( p, cn ); break;	
	case CMD_SEND_DUNGEON_HISTORY			:	RecvDungeonHistory( p ); break;
	case CMD_GET_SCENARIO_INFO				:	RecvGetScenarioInfo( p, cn ); break;	// 유저한테 온 요청
	case CMD_GET_SCENARIO_INFO_MAP			:	RecvGetScenarioInfoMap( p ); break;	// 맵서버한테 온 요청
	case CMD_SCENARIO_COMMAND				:	RecvScenarioCommand( p ); break;
	case CMD_GET_SKILL_POINT				:	RecvAskSkillPoint( p, cn ); break;
	case CMD_MANTLE_DYE						:	RecvMantleDye(p, cn); break;
	case CMD_APPLICATION_OF_EXILE			:	RecvApplicationOfExile( p ); break; //Eleval 12/06/09
	case CMD_EXILE_OK						:	RecvApplicationOfExileResult( p, cn ); break; //Eleval 12/06/09
	case CMD_EXILE_NOT						:	RecvApplicationOfExileResult( p, cn ); break; //Eleval 12/06/09
	case CMD_SEX_CHANGE						:	RecvSexChange( p , cn ); break; //Eleval 12/06/09 - Sex Change
	case CMD_SEX_CHANGE_OK					:	RecvSexChangeResult( p ); break; //Eleval 12/06/09 - Sex Change
	case CMD_TOOL_PLAY_SOUND				:	RecvToolPlaySound( p, cn ); break; //Eleval 19/06/09 - Tool Play Sound
	case CMD_TOOL_TOGGLE_SANTA				:	RecvToolToggleSanta( p, cn ); break; //Eleval 22/06/09 - To let GMs removing their Santa Power
	case CMD_LMS_SET_LVL					:	RecvLMSLvlRange( p, cn ); break; //Eleval 18/07/09 - To set LMS Lvl Range
	case CMD_SET_EXP_EVENT					:	RecvSetExpEvent( p, cn ); break; //Eleval 22/07/09 - To set an Exp Event
	case CMD_SET_EXP_EVENT_ALL				:	RecvSetExpEventAll( p, cn ); break; //Eleval 22/07/09 - To set Exp Event received from another map
	case CMD_TOGGLE_PKMODE_GM				:	RecvTogglePKTool( p, cn ); break; //Eleval 24/07/09 - Tool to change battle restr in a map
	case CMD_ASK_SQUEST_TELEPORT			:	RecvAskSQuestTeleport(p); //Eleval 10/08/09 - To bring ppl back in quest
	case CMD_ASK_SQUEST_TELEPORT_RESULT		:	RecvAskSQuestTeleportResult(p); break; //Eleval 10/08/09 - To bring ppl back in quest
	case CMD_RM_CHANGE_WEATHER				:	if(p->u.RmChangeWeather.bStopWeather == -1 || p->u.RmChangeWeather.bWeather == -1 || p->u.RmChangeWeather.dwAmount == -1) RandomWeatherSystem.SendWeather(cn); break; //Eleval 13/08/09 - Weather System
	case CMD_TOOL_QUEST_CHECK				:	RecvToolQuestCheck(p, cn); break; //Eleval 13/08/09 - Tool to check Quest NPC
	case CMD_CHAR_OPTIONS_CHANGE			:	RecvCharOptionsChange(p, cn); break; //Eleval 18/08/09 - To change a char's options
	case CMD_TELEPORT_OTHER_CHAR			:	RecvTeleportToChar(p, cn); break; //Eleval 19/08/09 - Tool to teleport to a char
	case CMD_TELEPORT_OTHER_CHAR_MAP		:	RecvTeleportToCharMap(p); break; //Eleval 19/08/09 - Tool to teleport to a char
	case CMD_REQ_HONOR_AMOUNT				:	RecvReqHonorAmount(p, cn); break; //Eleval 07/09/09 - To request the player's current honor
	
	default:	
		{
			return MenuFunctions( p, cn );	// 020620 YGI
		}
	}

	return 1;
}
// 020620 YGI -------------------------
#define _MAX_MENU_FUNCTIONS		1000
void (*PMenuFunctions[_MAX_MENU_FUNCTIONS])( t_packet *p, short int cn );

extern void SetMenuFunctionsAll();		// 020701 YGI
int MenuFunctions( t_packet *p, short int cn )
{
	int type = p->h.header.type;
	type -= KEIN_FUNCTIONS_START;
	if( type < 0 || type >= _MAX_MENU_FUNCTIONS ) return -31;
	if( PMenuFunctions[type] )
	{
		(*PMenuFunctions[type])( p, cn );
		return 1;
	}
	return -31;
}


void SetMenuFunctions( int type, void function(t_packet *, short int) )
{
	type -= KEIN_FUNCTIONS_START;
	if( type < 0 || type >= _MAX_MENU_FUNCTIONS ) JustMsg( "Error!! regist functions :: kein" );
	PMenuFunctions[type] = function;
}

void SetMenuFunctionsAll()
{
	SetMenuFunctions( CMD_CHECK_GOTO_SCENARIO_DUNGEON, RecvCheckGotoScenarioDungeon );
	SetMenuFunctions( CMD_CHECK_GOTO_SCENARIO_DUNGEON_RESULT, RecvCheckGotoScenarioDungeonResult );
	SetMenuFunctions( CMD_SCENARIO_MESSAGE_MAP, RecvScenarioMessageMap );
	SetMenuFunctions( CMD_TRAP_INFO, RecvScenarioDetailInfo);
	SetMenuFunctions( CMD_TRAP_INFO_MAP, RecvScenarioDetailInfoMap );
	SetMenuFunctions( CMD_SCENARIO_TRAP_KEY, RecvScenarioTrapKeyInfo);
	SetMenuFunctions( CMD_DRAGONROAD_VOTE, RecvDragonVote );
	SetMenuFunctions( CMD_SCENARIO_TIME_INFO, RecvGetScenarioTime ); // CSD-030306
	SetMenuFunctions( CMD_CHANGE_SCENARIO_TIME, RecvChangeScenarioTime );
	SetMenuFunctions( CMD_MOVEMAP_FOR_LIGHT_VERSION, RecvSetInstallVersion );		// 021117 YGI
	// 021107 YGI
	SetMenuFunctions( CMD_CHECK_EVENT_SCRIPT_ITEM, RecvScriptItem );
	SetMenuFunctions( CMD_EVENT_MOVE_MAP, RecvEventMoveMap );
	SetMenuFunctions( CMD_EVENT_MOVE_MAP_CHECK_MAP, RecvEventMoveMapCheckMap );
	SetMenuFunctions( CMD_EVENT_MOVE_MAP_OK, RecvEventMoveMapOk );
	
	SetMenuFunctions( CMD_CHECK_DOUBLE_NAME, RecvCheckDoubleName );
	SetMenuFunctions( CMD_CHECK_DOUBLE_NAME_OK, RecvCheckDoubleNameOk );
	SetMenuFunctions( CMD_CHECK_DOUBLE_NAME_DB_TO_MAP, RecvCheckDoubleNameDB2Map );
	SetMenuFunctions( CMD_CHECK_DOUBLE_NAME_WHERE_FIRST, RecvCheckDoubleNameWhenFirst );
	SetMenuFunctions( CMD_CHECK_DOUBLE_NAME_WHERE_OK, RecvCheckDoubleNameWhenFirstOk );
	// 040105 YGI 보물
	SetMenuFunctions( CMD_CHECK_TREASURE_XY, RecvCheckTreasureXY );
	SetMenuFunctions( CMD_GET_TREASURE_MAP_DETAIL, RecvGetTreasureMapDetail );
	SetMenuFunctions( CMD_GET_TREASURE_MAP_DETAIL_DB, RecvGetTreasureMapDetailDB );
}
