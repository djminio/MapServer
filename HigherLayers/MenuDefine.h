
#define GABX_SCREEN			80		// 800*600 으로 바뀔때 640*480과의 차이
#define GABY_SCREEN			60

#define MAX_FIELD			50 //하나의 메뉴에 들어갈수 있는 최대 구성요소
#define MAX_MENU			250 //최대 메뉴 갯수
#define MAX_STRING			500 //구성요소가 가지고 있을수 있는 문자열 최대 길이
#define MAX_CHEAKARRAY		3 //배열방식으로 체크할때 사용하는 배열의 최대갯수
#define MAX_SHN				50  //FT_HIDE_NOMAL_  에 사용할 구조체 최대 갯수
#define MAX_SHN_FIELD		15  //FT_HIDE_NOMAL_  에 사용할 구조체가 가질수 있는 최대 요소수
#define MAX_CHARACTERDATA	12 //nCharacterData에 들어갈 최대 캐릭터 데이터수
#define MAX_MENUDATA		40  //한블럭의 그림 최대 개수
#define MAX_FILENAME		100
#define MAX_LEARN_ITEM		1000
#define SELECT_LEARN_ITEM_MAX	200
#define MEMORIZE_MAX			200		// 현재 메모라이즈한 마법의 최대값...

// #define MAX_MAININTERFACE 100

#define MAX_SYSDAT				1100		// sysmenu.dat 에서 사용하는 이미지 총 갯수, 이 수 다음은 업데이트용이다...

#define MAX_IMAGE2				40

#define MAX_MAGIC				200			// 매직 최대수 

#define GOD_MENU_IMG_MAX		50
#define TIME_MENU_IMG_MAX		10
#define MEMBOOK_MENU_IMG_MAX	10
#define MAGICBOOK_MENU_IMG_MAX	20
#define MAIL_MENU_IMG_MAX		40
#define START_ETC_IMG_MAX		40			
#define START_ETC2_IMG_MAX		10
#define JOB_ETC_IMG_MAX			20
#define START_MAP_IMG_MAX		35			// 15개 사용중
#define CHAT_MENU_IMG_MAX		10
#define MAIN_ETC_IMG_MAX		70
#define SMALL_MAP_MAX			50			// 스몰 맵 스프라이트 맥스		
#define SMALL_ETC_MAX			20
#define SKILL_MENU_IMG_MAX		30			// 단축 스킬 맵..
#define PUBLIC_BBS_MENU_IMG_MAX 10			// 공지 사항( 게시판 )
#define HUNTER_MENU_IMG_MAX		40			// 현상범 게시판
#define QUEST_MENU_MAX			10
#define FAME_MENU_MAX			1
#define COLOR_CHANGE_MENU_MAX	10			// 0905 kkh 추가 
#define ITEM_MAKE_IMAGE_MAX		15			// 0908 KKH 추가 
#define NATION_MENU_IMG_MAX		15			// 국가 헌장문 게시판 메뉴
#define START_FIGHT_IMG_MAX		20			// 결투 시작
#define STOP_FIGHT_IMG_MAX		10			// 결투 끝났을때..
#define FIGHTMAP_TIME_IMG_MAX	40			// 시간 표시 할때
#define FIGHTMAP_WIN_IMG_MAX	45			// 이겼을때
#define FIGHTMAP_LOSE_IMG_MAX	40			// 졌을때

#define VOTE_MENU_IMG_MAX		20			// 결투 끝났을때..
#define GUILD_MARK_MAX			512			// guild mark

#define GUILD_ADMIN_MAX			20			// guild 관리소 메인
#define GUILD_SKILL_LIST_MAX	20			// 그랜드 마스터 등록
#define GUILD_ADMIN_ETC_MAX		10			// guild 관리소 메인 추가 이미지
#define HELP_PCX_MAX			20			// tip 버튼 그림
#define BANK_MAX				10			// 은행 한글화 
#define SALVATION_MAX			20			// 구세군 메뉴
#define SALVATION_ANI_MAX		10			// 적십자 캐릭터
#define TEAM_BATTLE_MAX			10			// 팀배틀 
#define SELECT_SERVER_IMG_MAX	10			// 서버 선택 메뉴
#define CAHNGE_MONEY_MAX		10

#define GUILD_WAIT_SPR_MAX		30
#define MERCHANT_SPR_MAX		15




#define MAX_MESSAGE				200

/////////////////////////////////////////////////////////////////////////////////
//메뉴 번호 설정

#define MN_MAINSTART_BACK				MAX_MENU-1
#define MN_MAKECHARACTER_BASIC             0     //케릭터 만들기 메뉴에서 가장 바닥에 깔리는 부분
#define MN_MAINSTART_BASIC                 1     //메인 스타트 메뉴의 시작부분
#define MN_MAINSTART_START                 2     //메인 스타트 메뉴에서 START버튼을 누르면 나오는 메뉴

#define MN_MAKECHARACTER_CHEAK1            3     //캐릭터 만들기 메뉴에서 설정확인창 1번
#define MN_MAKECHARACTER_CHEAK3            4     //캐릭터 만들기 메뉴에서 설정확인창 3번
#define MN_MAKECHARACTER_GENDER            5     //캐릭터 만들기 메뉴에서 성별설정메뉴
#define MN_MAKECHARACTER_NAME              6     //캐릭터 만들기 메뉴에서 이름설정메뉴
#define MN_MAKECHARACTER_FACE              7     //캐릭터 만들기 메뉴에서 얼굴설정메뉴
#define MN_MAKECHARACTER_ARIGEMENT         8     //캐릭터 만들기 메뉴에서 가치관설정메뉴
#define MN_MAKECHARACTER_CLASS             9     //캐릭터 만들기 메뉴에서 클래스설정메뉴
#define MN_MAKECHARACTER_SPELL             10    //캐릭터 만들기 메뉴에서 마법설정메뉴
#define MN_MAKECHARACTER_COLOR             11    //캐릭터 만들기 메뉴에서 색깔설정메뉴
#define MN_MAKECHARACTER_TACTICS_MAN       12    //캐릭터 만들기 메뉴에서 남자 전투기술설정메뉴
#define MN_MAKECHARACTER_TACTICS_WOMAN     13    //캐릭터 만들기 메뉴에서 여자 전투기술설정메뉴
#define MN_MAKECHARACTER_JOB               14    //캐릭터 만들기 메뉴에서 직업설정메뉴
#define MN_MAKECHARACTER_ABILITY           15    //캐릭터 만들기 메뉴에서 능력치 설정메뉴

#define MN_MAININTERFACE                   16    //메인 인터페이스 메뉴
#define MN_STATUS						   17    //스테이터스 메뉴
#define MN_ABILITY                         18    //어빌리티 메뉴
#define MN_NATION_MAIN_VA					19
#define MN_NATION_MAIN_ZY					20
#define MN_NATION_MAIN_ILL					21


#define MN_ITEM                            25    //아이템 메뉴
#define MN_ARIGEMENT                       26    //스테이터스나 어빌리티 메뉴에서 문장을 선택하면 나오는 메뉴
#define MN_DAIRY_DAIRY                     27    //dairy중 dairy
#define MN_DAIRY_DAIRY_WRITE               28    //dairy중 dairy중 write
#define MN_DAIRY_DAIRY_READ                29    //dairy중 dairy중 read
#define MN_DAIRY_CARD                      30    //dairy중 명함 만들기
#define MN_DAIRY_CARD_DEVEL                31    //dairy중 명함 만들기 중 만들기
#define MN_DAIRY_CARD_READ                 32    //dairy중 명함 만들기 중 read
#define MN_DAIRY_CARD_SEND                 33    //dairy중 명함 만들기 중 send
#define MN_DAIRY_LETTER                    34    //dairy중 편지 쓰기
#define MN_DAIRY_LETTER_WRITE              35    //dairy중 편지 쓰기중 쓰기
#define MN_DAIRY_LETTER_READLIST           36    //dairy중 편지 쓰기중 읽기
#define MN_DAIRY_LETTER_READLIST_READ      37    //dairy중 편지 쓰기중 읽기
#define MN_DAIRY_LETTER_CONVLIST           38    //dairy중 편지 쓰기중 확인 
#define MN_DAIRY_LETTER_CONVLIST_READ      39    //dairy중 편지 쓰기중 확인 

//#define MN_TEMPLE_INTRODUCTION             40    //신전 메뉴중 INTRODUCTION메뉴
//#define MN_TEMPLE_OPERATION				   41    //신전 메뉴중 OPERATION
//#define MN_TEMPLE_TEMPLE1                  42    //신전 메뉴중 5개의 목록이 있는TEMPLE
//#define MN_TEMPLE_TEMPLE2                  43    //신전 메뉴중 2개의 목록이 있는TEMPLE
//#define MN_TEMPLE_DONE                     44    //신전 메뉴중 DONE

//#define MN_GUILD_INTRODUCTION              45    //길드 메뉴중 INTRODUCTION메뉴
//#define MN_GUILD_OPERATION				   46    //길드 메뉴중 OPERATION
//#define MN_GUILD_GUILD1                    47    //길드 메뉴중 5개의 목록이 있는TEMPLE
//#define MN_GUILD_GUILD2                    48    //길드 메뉴중 2개의 목록이 있는TEMPLE
//#define MN_GUILD_DONE                      49    //길드 메뉴중 DONE


#define MN_DEFAULT_MESSAGEBOX			40		// default message box
#define MN_VIEW_TIP						41		// tip 보기 버튼
#define MN_SALVATION2					42		// 구세군 메뉴
#define MN_SALVATION					49		// 구세군 메뉴

//#define MN_TEAM_BATTLE_INFO				43		// 팀배틀 정보 보여주기( 팀등록 ) 메뉴
#define MN_CHANGE_MONEY					43
#define MN_SELECT_SERVER				44		// 서버 선택 메뉴
#define MN_DRINK_RUMOR_VIEW				45
#define MN_ARENA_LOBBY					46 // CSD-030509
#define MN_MERCHANT_MAIN				47		// 상인 메뉴 메인
#define MN_MERCHANT_BBS					48		// 공지 메뉴


#define MN_FACE                            50    //메인 인터페이스 메뉴에서 얼굴 표정 선택 메뉴 뛰우기

#define MN_BANK_DEPOSIT					   51    //은행 메뉴에서 예금
#define MN_BANK_DEFRAYAL                   52    //은행 메뉴에서 출금
#define MN_BANK_CUSTODY                    53    //은행 메뉴에서 보관
#define MN_BANK_MAIN	                   54    //은행 메뉴에서 찾기
#define MN_BANK_LOAN                       55    //은행 메뉴에서 대출
#define MN_BANK_REPAYMENT                  56    //은행 메뉴에서 상환
#define MN_BANK_AUCTION                    57    //은행 메뉴에서 경매

#define MN_LOGO_START_SAMSUNG				58		// 삼성 로고

#define MN_RANK                            59    //사냥 대회때에 스는 랭크 메뉴

#define MN_PARTY						   60    //메인 인터페이스 메뉴중 파티원 메뉴

#define MN_SKILL_MAIN					61	 //스킬 메뉴중 MAIN, 4개의 서브메뉴가 있다.
#define MN_SKILL_CLASS					62		//스킬 메뉴중 Class 메뉴
#define MN_SKILL_JOB					63		//스킬 메뉴중 Job 스킬 메뉴 
#define MN_SKILL_GENERAL				64		//스킬 메뉴중 General 메뉴
#define MN_SKILL_KNOWLEDGE				65		//스킬 메뉴중 Knowledge 메뉴

#define MN_DUNGEON_ARTIFICIAL			66		//던전 메뉴중 아티피컬 메뉴
#define MN_DUNGEON_BLAZE				67		//던전 메뉴중 블레이즈 메뉴
#define MN_DUNGEON_FREEZE				68		//던전 메뉴중 프리즈 메뉴

#define MN_LOGO_START					70		//회사 로고
#define MN_LODING						71		// 메인 화면으로 넘어가기전에 로딩 화면
#define MN_LOGIN						72		// 새로 만들 것인가 로드할 것인가...
#define MN_LOGON						73		// 로드할때, 뜨는 메뉴, 여기서 직접 게임으로 들어갈 수 있음.

#define MN_SELECT						74		//접속 모드
#define MN_LOCATION						75		//시작 장소 선택

#define	MN_BANK_AUCTION_SALE			76		// 경매 메뉴중 팔기 메뉴

#define MN_PARTY_FACE1					77		// 파티원들 조그만 얼굴 창
#define MN_PARTY_FACE2					78
#define MN_PARTY_FACE3					79
#define MN_PARTY_FACE4					80
#define MN_PARTY_FACE5					81
#define MN_PARTY_FACE6					82

#define MN_GOD_EDELBLHOY				83		// 에델 브로이 신전
#define MN_GOD_TEFFERY					84		// 테페리 신전
#define MN_GOD_LETTY					85		// 레띠 신전
#define MN_GOD_OREM						87		// 오렘 신전




#define MN_MESSAGEBOX_BIG				88		// 독립적으로 호출할 수 있는 메시지 박스
#define MN_MESSAGEBOX_SMALL				89		
#define MN_YESNO						90		//YES-NO MENU

#define MN_DISTANCE_TABLE				91		//	이정표
#define MN_STORE_SIGN					92		//	간판(?)

#define MN_MAGIC_GUILD					93		// 마법사 길드
#define MN_ACADEMY						94		// 아카데미
#define MN_SKILLGUILD_KNOWLAGE			95		// Learn Knowlage

#define MN_AWAKE						96		// 휴식에서 , 잠자기에서 깨어나는 메뉴... 버튼..

#define MN_TOWNHALL_BBS1				97		// ok, cancel있는 게시판
#define MN_TOWNHALL_BBS2				98		// join, cancel 있는 게시판
#define MN_TOWNHALL_BBS3				99		// write, exit 있는 게시판
														
#define MN_BLACKSMITH					100		// 직업 길드 메뉴
#define MN_FARM							101
#define MN_RESTAURANT					102
#define MN_HERBSHOP						103
#define MN_SHAMBLES						104
#define MN_CANDLE						105
#define MN_SLOPSHOP						106
#define MN_ALCHEMY						107
#define MN_WOODWORKING					108
#define MN_BOWCRAFT						109
#define MN_SHIPYARD						110

#define MN_SKILLGUILD_BUY				111
#define MN_SKILLGUILD_SELL				112
#define MN_SKILLGUILD_SKILL				113
#define MN_SKILLGUILD_ITEM				114
#define MN_SKILLGUILD_REPAIR			115

#define MN_ITEM_EXPLAN					116

#define MN_TOWNHALL                     117    //TOWNHALL인터페이스
#define MN_TOWNHALL_PUBLICNOTICE        118    //TOWNHALL메뉴에서 PUBLICNOTICE메뉴
#define MN_TOWNHALL_BUILDING            119    //TOWNHALL메뉴에서 BUILDING메뉴
#define MN_TOWNHALL_COMMERCE1           120    //TOWNHALL메뉴에서 COMMERCE1메뉴
#define MN_TOWNHALL_COMMERCE2           121    //TOWNHALL메뉴에서 COMMERCE2메뉴
#define MN_TOWNHALL_COMMERCE3           122    //TOWNHALL메뉴에서 COMMERCE3메뉴       //설명창
#define MN_TOWNHALL_HOUSEREPAIR         123    //TOWNHALL메뉴에서 HOUSE REPAIR메뉴
#define MN_TOWNHALL_GUILD               124    //TOWNHALL메뉴에서 Organize Guild메뉴
#define MN_TOWNHALL_FORUM               125    //TOWNHALL메뉴에서 FORUM메뉴
#define MN_TOWNHALL_EVENT               126    //TOWNHALL메뉴에서 EVENT메뉴
#define MN_TOWNHALL_RANK1               127    //TOWNHALL메뉴에서 RANK1메뉴
#define MN_TOWNHALL_RANK2               128    //TOWNHALL메뉴에서 RANK2메뉴
#define MN_TOWNHALL_NOTICE4             129    //TOWNHALL메뉴에서 NOTICE4메뉴
#define MN_TOWNHALL_NOTICE3             130    //TOWNHALL메뉴에서 NOTICE3메뉴
#define MN_TOWNHALL_NOTICE2             131    //TOWNHALL메뉴에서 NOTICE2메뉴
#define MN_TOWNHALL_NOTICE1             132    //TOWNHALL메뉴에서 NOTICE1메뉴
#define MN_TOWNHALL_COMMERCE4           133    //TOWNHALL메뉴에서 COMMERCE4메뉴   //설명창
#define MN_TOWNHALL_CONSTRUCTION        134    //TOWNHALL메뉴에서 CONSTRUCTION메뉴

#define MN_MY_HOME						135		// 설명창 띄우기
#define	MN_MY_HOME_EXPLAN				136		// 설명쓰기 에디트 창
#define MN_HOUSE						137		// 건물시스템
#define MN_MY_HOME_IN					138
#define MN_HOME_PERSON					139

#define	MN_CHATTING						140		// 채팅창		

#define MN_GOD1                         141     //신전메뉴1
#define MN_GOD2                         142     //신전메뉴2
#define MN_GOD3                         143     //신전메뉴3
#define MN_LADDER_RANK					144

#define MN_MAGICBOOK                    145     //마법책
#define MN_FUNCTION                     146     //행사 접수실

#define MN_SYSTEM                       147     //시스템 메뉴
#define MN_SYSTEM_OPTION                148     //시스템 메뉴에서 옵션메뉴
#define MN_SYSTEM_HELP                  149     //시스템 메뉴에서 help메뉴
#define MN_SYSTEM_HELP_KEY              150     //시스템 메뉴에서 헬프에서key
#define MN_SYSTEM_HELP_MESSAGE			151     //시스템 메뉴에서 헬프에서명령어
#define MN_SYSTEM_HELP_MACRO            152     //시스템 메뉴에서 헬프에서 메크로
#define MN_SYSTEM_LOGOUT                153

#define MN_DRINK_MAIN					154		// 주점
#define MN_DRINK_RUMOR					155		//
#define MN_DRINK_BUY					156		//

#define MN_SHOP_MAIN					157		// 잡화상
#define MN_SHOP_BUY						158		//
#define MN_SHOP_SELL					159		//
#define MN_HEAL_MAIN					160		// 병원
#define MN_HEAL_HEALING					161		// 치료
#define MN_HEAL_PREVENTING				162		// 예방
#define MN_HEAL_LEARN					163		// 스킬 배움
#define MN_SKILL_LEARN					164		// 스킬 배움
#define MN_INN_MAIN						165
#define MN_INN_REST						166
#define MN_INN_SLEEP					167
#define MN_SELECT_CHARACTER				168		// 캐러 선택창
#define MN_TACTICS						169		// 택틱스 남
#define	MN_LEVELUP						170		// 단지 멥에서 클릭 했을 때 주인공이 움직이는 것을 막기 위해
#define MN_MAKECHARACTER_AGE			171		// 캐릭터 만들기에서 나이 설정
#define MN_MAKECHARACTER_SPELL2			172		// wizard나 priest에서 spell 결정할 때...
#define	MN_INFO							173
#define MN_CREDIT						174
#define MN_OKCANCEL_MESSAGEBOX			175		// OK_CANCEL 달린 메시지 창
#define MN_OK_MESSAGEBOX				176		// OK만 달린 메시지 창
#define MN_ITEM_MESSAGEBOX				177		// 아이템 설명 창
#define MN_OPEN_BOX						178		// 박스 아이템 창
#define MN_DIVIDE_ITEM					179		// 아이템을 값을 나누는 창
#define MN_MAGIC_BOOK					180		// 매직 책
#define MN_ITEM_MAKE					181		// 아이템 만들기용 작업대
#define MN_ITEM_MAKE_SELECT				182		// 만들수 있는 아이템 출력
#define MN_ITEM_MAKE_OK					183		// 아이템 만들기에 성공했을 경우 
#define MN_CHARACTER_ITEM				184		// 죽은 캐릭터 뒤지거나 훔쳐보기 기술을 사용했을 때 뜨는 인벤창
#define MN_ITEM_SELECT					185		// 스킬 선택시 사용 아이템 보여주기 선택 창 
#define MN_LEARN_MAGIC_BOOK				186		// 매직 배우기...
#define MN_MEM_BOOK						187		// 메모라이즈

#define MN_SMALL_MAP					188		// 뷰 스몰 멥
#define MN_EXCHANGE_ITEM				189		// 아이템 교환

#define MN_BETA_TEST					190		// 테스터 선택
#define MN_BETA_TEST_VIEW				191		// 베타 테스트 관련 글 보여주기

#define MN_GOD_INTRUDUCTION				192		// 신전 설명 메뉴
#define MN_GOD_MEETING					193		// 신전 예배 안내
#define MN_GOD_REGIST1					194		// 등록 설정 wizard

#define MN_GOD_PREACH					195		// 전도한 사람 등록 메뉴
#define MN_GOD_REGIST_ERROR				196		// 신전 등록시 예외상황 ( 이미 등록되어 있다. 다른 신전에 등록되어 있다..
#define MN_GOD_REGIST_MESSAGE			197		// 등록 취소 등의 메시지 처리
#define MN_GOD_PREACH_ERROR1			198
#define MN_GOD_PREACH_RESULT			199
#define MN_GOD_MEETING_MAIN				200		//	시간이 되면 뜨는 예배 메뉴
#define MN_GOD_SUPPER					201		//	신전에서 음식 제공 메뉴

#define MN_TIME_DISPLAY_MENU			202		// 시간 표시 메뉴

#define MN_MAIL_MENU					203
#define MN_MAIL_WRITE					204
#define MN_MAIL_READ					205
#define MN_MAIL_BIRD					206		// 비둘기 

#define MN_RESIST_FRIEND				207		// 내 친구 등록 메뉴
#define	MN_MAIN_EXPLAIN					208		// 메인 메뉴 설명 창
		
#define MN_QUICK_MAGIC_SCROLL			209		// 퀵 매직 배열에서 5개 이상 넘어갈경우 스크롤 처리
#define MN_QUICK_MAGIC_PLUS				210		// 저장 매직 배열 메뉴 
#define MN_TIME_DISPLAY_ON_OFF			211		// 메뉴 디스플레이 메뉴 활성화 버튼을 가지고 있는 메뉴

#define MN_START_MAP					212		// 처음 시작 메뉴 선택
#define MN_PUBLIC_BBS					213		// 공지사항 메뉴
#define MN_QUICK_SKILL					214		// 퀵스킬 창...

#define MN_HUNTER_MAIN					215		// 현상범 게시판 메인
#define MN_HUNTER_REGIST				216		// 현상범 등록 메뉴
#define	MN_HUNTER_LIST					217		// 현상범 리스트 메뉴

#define	MN_QUEST						218		// 퀘스트 메뉴
#define	MN_FAME							219		// 명성 메뉴 
#define MN_QUEST_INDEX					220		// 퀘스트 인덱스 

#define MN_RELATION						221		// 사제 메뉴
#define	MN_MAN_TO_MAN					222		// 1:1 결투 시스템

#define MN_RESET_JOB					223		// 직업 재 선택
#define MN_RESET_JOB_EXPLAIN			224		// 직업 재 선택 설명 글

#define MN_CHARACTER_COLOR_CHANGE		225		// 0905 kkh 추가 
#define MN_SELECT_NATION				226		// 나라 선택
#define MN_NATION_EXPLAIN				227		// 나라 설명

#define MN_NATION_BBS					228		// 나라 헌장문 게시판
#define MN_FIGHT_MAP_SIGN				229		// 배틀존 전광판 

#define MN_SCENARIO						230		// 시나리오 출력 메뉴

#define	MN_VOTE_REGIST					231		// 후보 등록
#define MN_VOTE_SUPPORT					232		// 투표
#define MN_VOTE_SIGN					233		// 득표

#define MN_GUILD_ADMIN					234		// 길드 관리소 메인
#define MN_GM_LIST						235		// 그랜드 마스터 등록
#define MN_GM_ITEM_EXPLAIN				236		// 그랜드 마스터 조건 아이템 설명
#define MN_CHANGE_JOB					237		// 직업 선택 메뉴
#define MN_DISPLAY_SKILL_EXP			238		// 스킬 경험치 보여주기 메뉴
#define MN_DISPLAY_TAC_EXP				239
#define MN_DISPLAY_MAGIC_EXP			240


#define MN_LUCKY_MENU					256
#define MN_LOTTO_MENU					257

#define MN_REVIVAL						67


#define MN_MERCHANT_OLDMAIN				295		// BBD 040303	비집고 들어갈 번호가 불안해서 번호가 이렇게까지...ㅜ ㅜ

#define MN_ITEM_MALL					298 // the new item mall

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
//FT_HIDE_NOMAL_에 사용할 구조체 번호 설정
#define HN_MAKECHARACTER_GENDER_RECT          0     //캐릭터 만들기 메뉴에서 성별 설정때 나오는 선택 사각형 
#define HN_MAKECHARACTER_GENDER_TEXT          20    //캐릭터 만들기 메뉴에서 성별 설정때 나오는 선택 글자

#define HN_MAKECHARACTER_FACE                 1     

#define HN_MAKECHARACTER_ARIGEMENT_TEXT       21    //캐릭터 만들기 메뉴에서 가치관 설정때 나오는 글자
#define HN_MAKECHARACTER_ARIGEMENT_RECT       2     //캐릭터 만들기 메뉴에서 가치관 설정때 나오는 사각형

#define HN_MAKECHARACTER_CLASS_TEXT           22    //캐릭터 만들기 메뉴에서 클래스 설정대 나오는 글자
#define HN_MAKECHARACTER_CLASS_RECT           3     //캐릭터 만들기 메뉴에서 클래스 설정때 나오는 사가형

#define HN_MAKECHARACTER_SPELL_RECT           4     //캐릭터 만들기 메뉴에서 SPELL 설정때 나오는 사각형
#define HN_MAKECHARACTER_SPELL_TEXT           23    //캐릭터 만들기 메뉴에서 SPELL 설정때 나오는 글자

#define HN_MAKECHARACTER_COLOR_SELECT           5	  //머리색깔과 장식색깔의 점찍기
//#define HN_MAKECHARACTER_COLOR_HAIR           5     //캐릭터 만들기 메뉴에서 선택된 머리 색깔의 점찍기
//#define HN_MAKECHARACTER_COLOR_SKIN           6     //캐릭터 만들기 메뉴에서 선택된 피부 색깔의 점찍기
//#define HN_MAKECHARACTER_COLOR_CLOTHES        7     //캐릭터 만들기 메뉴에서 선택된 옷 색깔의 점찍기
//#define HN_MAKECHARACTER_COLOR_ORNAMENT       8     //캐릭터 만들기 메뉴에서 선택된 장식 색깔의 점찍기

#define HN_MAKECHARACTER_TACTICS_MAN_RECT     9    //캐릭터 만들기 메뉴에서 택틱스 설정때 나오는 남자 사각형
#define HN_MAKECHARACTER_TACTICS_MAN_TEXT     24    //캐릭터 만들기 메뉴에서 택틱스 설정때 나오는 남자 글자

#define HN_MAKECHARACTER_TACTICS_WOMAN_RECT   10    //캐릭터 만들기 메뉴에서 택틱스 설정때 나오는 여자 사각형
#define HN_MAKECHARACTER_TACTICS_WOMAN_TEXT   25    //캐릭터 만들기 메뉴에서 택틱스 설정때 나오는 여자 글자

#define HN_MAKECHARACTER_JOB_RECT             11    //캐릭터 만들기 메뉴에서 직업 설정때 나오는 사각형
#define HN_MAKECHARACTER_JOB_TEXT             26    //캐릭터 만들기 메뉴에서 직업 설정때 나오는 글자

#define HN_MAKECHARACTER_ABILITY              12    //캐릭터 만들기 메뉴에서 능력치 설정때 나오는 능력치글자

#define HN_TEMPLE_INTRODUTION_TEXT            13    //설명 생략
#define HN_TEMPLE_OPERATION_TEXT              15    //
#define HN_TEMPLE_TEMPLE1_TEXT                17    //
#define HN_TEMPLE_TEMPLE2_TEXT                19    //

#define HN_GUILD_INTRODUTION_TEXT             27    //설명 생략
#define HN_GUILD_OPERATION_TEXT               28    //
#define HN_GUILD_GUILD1_TEXT                  29    //
#define HN_GUILD_GUILD2_TEXT                  30    //
#define HN_INVENTORY                          31    //인벤토리창의 스크롤을 위한 필드 단지 .nSHideNomalCount  .nSHideNomalPlus   .nSHideNomalStart만을 위해 사용한다
#define HN_PARTY_SELECT						  32  // 메인 인터페이스, 파티 메뉴에서 선택 버튼 라이트

#define HN_SKILL_MAIN_TEXT					33		// 스킬 메뉴중 메인 화면 글자

#define HN_TOWNHALL_TEXT                      34    //타운홀 메뉴에서 메뉴 글자
#define HN_TOWNHALL_CONSTRUCTION_TEXT         35    //타운홀 메뉴에서 construction메뉴에서 글자

#define HN_SYSTEM_TEXT                        36    //메인인터페이스의 시스템 메뉴에서  글자
#define HN_SYSTEM_HELP_TEXT                   37    //메인인터페이스의 시스템의 헬프에서 글자

#define HN_DRINK_MAIN_TEXT					38		// 독립메뉴 주점에서 선택 문자
#define HN_SHOP_MAIN_TEXT					39		// 잡화상
#define HN_HEAL_MAIN_TEXT					40		// 병원
#define HN_INN_MAIN_TEXT					41		// 여관
#define HN_INN_REST_TEXT					42		//

#define HN_HEAL_PREVENTING					43		// 병원에서 질병 예방

/////////////////////////////////////////////////////////////////////////////////
//특별히 일어나는 일 설정 예를 들면 동시에 여러 메뉴를 할성화 시키거나 어떤 조건을 체크해 메뉴를 열거나
#define SWD_ENTER_MAKECHARACTER            1      //매인스타트에서 캐릭터 만들기 모드로 들어간다
#define SWD_MAKECHARACTER_ENTERNAME        2      //성별메뉴에서 이름설정 메뉴로 들어갈때 
#define SWD_MAKECHARACTER_SPELLORCOLOR     4      //클래스 메뉴에서 선택된 클래스를 판단해 SPELL이나 칼라로 갈때
#define SWD_MAKECHARACTER_TACTICS          6      //성별을 판단해  TACTICS목록을 보여준다
#define SWD_MAKECHARACTER_ABILITY          7      //JOB에서 ABILITY메뉴로 가기


#define SWD_FOCUS_WILLDO					5
#define SWD_CALLMENU						6
#define SWD_MN_MERCHANT_BBS_OK				7		// 공지 날리기

#define SWD_MAININTERFACE_STATUS           9      //메인 인터페이스에서 스테이터스 메뉴 뛰우기
#define SWD_MAININTERFACE_ABILITYTOSTATUS  10     //어빌리티 메뉴에서 스테이터스 메뉴를 뛰울때
#define SWD_MAININTERFACE_MAGIC            11     //메인 인터페이스에서 매직 메뉴 뛰우기(프리스트나 위자드 메뉴에1로 뜬다)
#define SWD_MAININTERFACE_ITEM             12     //메인 인터페이스에서 아이템 메뉴 뛰우기
#define SWD_MAININTERFACE_ABILITY          13     //메인 인터페이스의 스테이터스에서 어빌리티 메뉴 뛰우기    
#define SWD_MAININTERFACE_ARIGEMENT        14     //스테이터스 메뉴나 어빌리티 메뉴에서 가치관 메뉴 뛰우기
#define SWD_MAININTERFACE_PARTY			   23     //메인 인터페이스에서 파티원 메뉴 띄우기

#define SWD_ARIGEMENT_STATUS               16     //아무일도 안하고 그냥 자신만 닫는다

#define SWD_MAININTERFACE_FACE             17     //메인 인터페이스 에서 얼굴 표정 선택 창 뛰우기
#define SWD_FACE_CHOOSE                    18     //표정 선택후 나가기
#define SWD_FACE_AUTOCLOSE                 19     //얼굴표정 선택 메뉴가 자동으로 사라지게 한다

#define SWD_UNMOUSEFOCUS                   20     //마우스 포커스를 풀어 모든 메뉴를 사용가능하게 한다

#define SWD_LMENUCLOSE                     21     //메인 인터페이스에서 사용하는 메뉴중 왼쪽에 뜨는 메뉴를 움직이면서 닫을대 사용
#define SWD_RMENUCLOSE                     22     //메인 인터페이스에서 사용하는 메뉴중 오른쪽에 드는 메뉴를 움직이면서 닫을때 사용

#define SWD_YESNO_YES						24		// When select "YES" at YES-NO MENU
#define SWD_YESNO_NO						25		// When select "NO" at YES-NO MENU

#define SWD_CALL_OTHER						26		// 자기와는 상관없는 곳의 메뉴 호출 하기
#define SWD_IDPASS							27		//아이디 패스워드 에디트 박스 만들기
#define SWD_TURN_BUTTON						28		//메인 인터페이스에서 번갈아 출력하는 버튼

#define SWD_IDEDITBOX						29				// 아이디 출력
#define SWD_PASSEDITBOX						30				// 패스워드 출력

#define SWD_MAKECHARACTER_GENDERBACK		31		// 성별 결정 메뉴에서 스타트 메뉴로 돌아가기

#define SWD_SMALLMENU_END					32		// 독립메뉴 끝낼때
#define SWD_BANK_OK							33
#define SWD_BANK_CANCEL						34		// 은행 메뉴에서 ok, cancel 버튼
#define SWD_BANK_ITEM						35
#define SWD_BANK_ITEM_END					36

#define SWD_TWO_MENU_OPEN					37		// 두개의 메뉴를 동시에 띄울때 nWillDo와 nSHideNomalNumber를 사용한다. 현재는 경매 사기 메뉴에서 팔기 메뉴로 바꿀때만 사용
#define SWD_SIMPLE_CLOSE					38		// 메인 인터페이스와 띄우려는 것만 놔두고 모든 메뉴를 닫을 때.. (두개 이상 띄워져 있을 경우 사용)

#define SWD_SMALL_MENU						39
#define SWD_MENU_OPEN						15		// 메인 인터페이스에서 그냥 띄우기

#define SWD_ITEM_SELLBUY_OK					40		// 사고 팔기 메뉴에서 OK 버튼 눌렀을 때

#define SWD_JOBSKILL_SCROLL_UP				41		//	사고 팔기 메뉴에서의 스크롤 버튼
#define SWD_JOBSKILL_SCROLL_DOWN			42		//				"

#define SWD_ITEM_SELLBUY_CANCEL				43		// 길드에서 사고 팔기 메뉴
#define SWD_LOGON							44		// 스타트 메뉴에서 로그온 할때 아이디 패스워드 체크
#define SWD_LOGON_OK						45		

#define SWD_TOWN_CONSTRUCTION				46

#define SWD_EDIT_BOX						47		// 메인에서 에디트 박스 띄우기
#define SWD_MY_HOME							48		// MN_MY_HOME으로 돌아가기

#define SWD_INFO_FLAG_SET					49		// 클릭되면 인터페이스 정보 플레그를 셋팅 해줄때

#define SWD_MAIN_TO_CHAT					50
#define SWD_CHAT_TO_MAIN					51

#define SWD_NORMAL_CHECK_PUT				52		// 클릭후 다른 필드의 타입을 바꾼다.

#define	SWD_QUIT							53		// 게임 끝낼때
#define SWD_SELL_BUY						55

#define SWD_LEARN							56		// skill 배우기
#define SWD_CHARACTER_SELECT				57		// 캐릭터 선택하기
#define SWD_CHARACTER_DEL_ADD				58

#define SWD_STATUS_TACTICS					59		// 스테이터스에서 택틱스로
#define SWD_TACTICS_STATUS					60		// 택틱스에서 스테이터스로

#define	SWD_PARTY_BUTTON					61
#define	SWD_BACK_SKILL						62

#define SWD_SKILLGUILD_LEARN				63		// 스킬 배우기, 아이템 배우기
#define	SWD_LEARNSKILL_OTHER				64		// 

#define	SWD_TOWNHALL_TITLE					65		// 각종 게시판 항목 찍기 
#define SWD_TOWNGALL_TITLE_READ				66		// 게시판 제목으로 내용 출력 창으로 들어가기

#define SWD_OKCANCEL_OK						67		
#define SWD_OKCANCEL_CANCEL					68

#define SWD_BBS_READ						69		// 마을 회관에서 공지나 등등의 글을 읽을 때
#define	SWD_SUBMENU_BACK					70		// 자식메뉴에서 어미 메뉴로 돌아오기 nTemp를 이용
#define SWD_SMALLMENU_BACK					SWD_SUBMENU_BACK

#define	SWD_EVENT_JOIN						71		// 이벤트 비비에서 조인하기 

#define SWD_PARTY_RELEASE					72		// 파티원 해제

#define SWD_SELECT_HOUSEKIND				73		// 집고르기 선택
#define SWD_BUY_HOUSE						74		// 집 사기기

#define	SWD_WILLDO							75		// 다음 메뉴를 띄우기 전 셋팅을 한다. 

#define SWD_MOVE_OPEN_CLOSE					76		// 움직이며 열리거나 닫히는 메뉴 셋팅. nWillDo를 사용

#define SWD_DIVIDE_OK						77		// 아이템 나누기 창에서 OK 버튼 
#define SWD_ITEM_MAKE						78		// 아이템 메이크 버튼을 누를 경우  
#define SWD_ITEM_MAKE_CANCEL				79		// 아이템 메이크 버튼을 누를 경우  

#define SWD_ITEM_MAKE_SELECT_OK				80		// 아이템을 고른 후에 작업대 출력
#define SWD_ITEM_MAKE_AFTER					81		// 아이템 만든후 작업대 닫기

#define SWD_LEARNSKILL_OK					82		// 스킬 배우기...
#define SWD_LEARNITEM_OK					83		// 아이템 배우기...

#define SWD_ITEM_USE_SELECT_OK				84		// 스킬 사용시 사용할 아이템 선택 ok
#define SWD_SKILL_CANCEL					85		// 아이템 선택중 cancel 버튼 ==> 스킬 취소...

#define SWD_HEALING_OK						86		// 병원에서 치료
#define SWD_PREVENTING_OK					87		// 병원에서 예방 
#define SWD_INN_OK							88		// 병원에서의 기능.....
#define SWD_MEMORIZE_BUTTON					89		// 메모라이즈 행위 버튼

#define SWD_SELECT_LOCATION					90		// 시작 위치 지정

#define SWD_ITEM_EXCHANGE_CANCEL			91		// 아이템 교환 취소 버튼
#define SWD_BETA_TEST_GO					92		// 배타 테스트 설명 메뉴로 이동

#define SWD_SORRY							93		

#define SWD_GO_BBS_WRITE					94		// 쓰기 메뉴 활성화
#define SWD_BBS_WRITE_SEND					54		// 서버로 전송
#define SWD_BBS_WRITE_EXIT					95		// 쓰기 취소

#define SWD_GOD_REGIST_OK					96		// 등록 하겠습니다.

#define SWD_PREACH_OK						97		// 전도한 사람 등록
#define SWD_PREACH_CANCEL					98		// 전도한 사람이 없음.

#define SWD_GOD_REGIST_ERROR_OK				99		
#define SWD_GOD_PREACH_RESULT_OK			100

#define SWD_TIMEMENU_CLOSE					101		// time display menu close
#define SWD_ONLY_OK							102		// this is ok-button of 'okcancel message box'

#define SWD_MAIL_WRITE_EXIT					103		// 메일 보내기 메뉴 빠져 나가기
#define SWD_MAIL_WRITE_SEND					104		// 서버로 전송
#define SWD_MAIL_WRITE_DELETE				105		// 내용 지우기

#define SWD_MAIL_READ_OPEN					106		// 메일 읽기메뉴 열기
#define SWD_MAIL_DELETE						107		// 메일 삭제

#define SWD_CONNECT_RECENT					108		// 최근 로그아웃한 자리에서 접속한다.
#define SWD_MAIL_DELETE_AND_BACK			109		// 메일 읽기 메뉴에서 메일 삭제
#define SWD_READ_TO_WRITE					110		// 답장 보내기

#define SWD_SMALL_MENU_2					111		// 자신은 남고 스몰 메뉴 띄우기
#define SWD_SUPPER_BACK						112		// 신전 슈퍼에서 뒤로 돌아가기 
#define	SWD_WILLDO_WORK						113		// willdo의 값을 work로 옮긴다.
#define SWD_CONNECT							114		// 선택한 곳으로 연결
#define SWD_MAININTERFACE_SKILL				115		// 메인 메뉴에서 스킬을 열자

#define SWD_HUNTER_REGIST_OK				116		// 현상범 등록 메뉴에서...
#define SWD_HUNTER_LIST_OK					117		// 내 목록에 현상범을 등록 시킨다.

#define SWD_MAN_TO_MAN_CANCEL				118		// 1:1 결투 메뉴 취소
#define SWD_RESET_JOB						119		// 직업 재 선택하는 메뉴를 띄울지를 선택한다.

#define SWD_RESET_JOB_EXPLAIN				120		// 직업 재 선택시 설명글 다시 보여주기
#define SWD_CHARACTER_COLOR_CHANGE			121		// 0905 kkh 추가 

#define SWD_GO_VOTE							122		// vote 메뉴 띄우기( 후보 등록 메뉴를 띄울지 아닐지를 결정한다. )
#define SWD_VOTER_REGIST_OK					123		// 후보 등록하기
//#define SWD_VOTER_REGIST_CANCEL				124		// 등록 취소하기 
#define SWD_VOTER_SUPPORT_OK				125
#define SWD_VOTER_SUPPORT_SCROLL			126

#define SWD_GM_LIST_SCROLL					127		// 그랜드 마스터 등록 스크롤
#define SWD_GUILD_EXPLAIN_SCROLL			128		// 그랜드 마스터 등록 조건 아이템 설명 스크롤
#define SWD_GM_REGIST						129		// 그랜드 마스터에 등록한다.
#define SWD_NEW_ID							130		// 시작메뉴에서 now id 를 클릭했을경우 
#define SWD_DEFAULT_MESSAGEBOX_OK			131		// ok button of defualt messagebox

#define SWD_RE_LOGON						132		// 다시 4개의 캐릭터 선택 화면으로 들어간다.
#define SWD_HELP							133		// tip 보기 띄우기...
#define SWD_SALVATION_OK					134		// 기부하기
#define SWD_CHANGE_MONEY_OK					135		// 돈 바꾼다.

#define SWD_RUMOR_OK						136		// 누가 초콜렛을 가장 많이 주나...

#define SWD_BATTLE_REGIST					137		// 길드전 등록
#define SWD_REQ_LOAN						138		// 대출 메뉴 열때

#define SWD_FIGHTMAP_WAIT_MEMBER			139		// 다시 대기자 명단으로 돌아간다.
#define SWD_FIGHT_WAIT_EXIT					140		// 길드전 대기자 명단에서 빠지기..

#define SWD_CALL_SMALLMENU_TO_SMALLMENU		SWD_CALLMENU		// 스몰메뉴에서 스몰메뉴 띄우기




/////////////////////////////////////////////////////////////////////////////////
//메뉴 구성요소 타입번호 설정
#define FT_NOMAL_PUT                1  //이미지만 찍는 구성요소
#define FT_NOMAL_PUT_CENTER         2  //이미지만 찍으면서 상위 메뉴 x축가운데 찍기
 
#define FT_TEXT_PUT                 3  //글자를 출력
#define FT_TEXT_PUT_CENTER          4  // 상위메뉴 x축 가운데 부분에 영어 문자열을 출력
#define FT_TEXT_PUT_CENTER_X		41	// x 변수에 넓이를 넣어 준다.

#define FT_HIDE_PUT	                5  //마우스 L버튼이 눌렸을때 이미지를 바꿔서 보여주는 구성요소
#define FT_HIDE_PUT_CENTER          6  //FT_HIDE_PUT 와 같으나 상위 메뉴 x축가운데 찍힌다
#define FT_HIDE_PUT_AUTO            7  //마우스가 가까이만 가면 버튼을 누르지 않아도 영역체크
#define FT_HIDE_PUT_CENTER_AUTO     8  //FT_HIDE_PUT_AUTO와 같으나 x축 가운데에 찍힌다

#define FT_HIDE_NOMAL_ONE_PUT       9  //메뉴구성요소중 여러개의 이미지를 가지고 있을때 여러개중1가지를 선택해서 보여줘야 하는 구성요소
#define FT_HIDE_NOMAL_GROUP_PUT     10 //메뉴구성요소중 여러개의 이미지를 가지고 있을때 여러개를 선택해서 보여줘야 하는 구성요소

#define FT_HIDE_WILLDO_PUT          11 //마우스 L버튼으로 눌렀다 때면 다른메뉴를 호출하는 형식
#define FT_HIDE_SPECIAL_WILLDO_PUT  12 //단순히 메뉴호출만 하는게 아니라 여러가지 특별한 일을 할경우
#define FT_SPECIAL_WILLDO           13 //이미지를 찍지 않고 작동한다

#define FT_HIDE_ISCROLL_PUT         14 //FT_HIDE_NOMAL_ 와 연결된 스크롤 버튼을 눌렀을때 FT_HIDE_NOMAL_가 찍힐 범위를 증가 설정
#define FT_HIDE_DSCROLL_PUT         15 //FT_HIDE_NOMAL_ 와 연결된 스크롤 버튼을 눌렀을때 FT_HIDE_NOMAL_가 찍힐 범위를 감소 설정

#define FT_THROWDICE                16 //주사위를 던져주는 타입

#define FT_DATA_PUT                 17 //한글영어 숫자 모두 출력 가능 nImageName에 DP_ 를 대입해 이용한다

#define FT_FIRE                     18 //불꽃을 찍어주는 타입

#define FT_INVENTORY                19 //아이템 메뉴에서 인벤토리 창과 관련된 필드 마우스이베트를 체크해 아이템 이동을 한다
#define FT_NO_CHECK                 20  //그룹 필드에서 어느것도 출력을 하지 않을때 사용.
#define FT_ACCEL_INVENTORY          21  //마우스 이벤트를 체크하는 단축 아이템창 

#define FT_ARMOR_INVENTORY          22  // 무기 장착 화면에서 마우스 이벤트 체크

#define FT_WILLDO_PUT				23  // 마우스 클릭후 그냥 다른 메뉴 호출 할때

#define FT_DO						24	// 그냥 뭔가를 할때,
#define FT_GAUGE					25	// 게이지 바에 뭔가를 채울때(현재는 로딩 화면만, 필요하면 변수 설정및 보수 필요)
#define FT_DO_DEFAULT				26	// FT_DO 에서 화면이 스크롤 되며 뜰때 

#define FT_IDEDITBOX				27	//아이디 입력 에디터 박스 호출
#define FT_PASSEDITBOX				28	//패스워드 입력 에디터 박스 호출
#define FT_NOMAL_PUT_TALK_BATTLE_PEACE	29		// TALK, PEACE, BATTEL 버튼 출력을 위한...

#define FT_LODING_LIGHT				30		//로딩할때, 연출...

#define FT_MAGIC_ITEM_PUT			31		// 매직 아이템 출력
#define FT_NOMAL_PUT_CENTER_1		32		// _1이 없는 것이 트랜스 출력
#define FT_MAGIC_QUICK_ITEM			33		// 퀵 매직 아이템 출력

#define FT_COLOR_GAUGE				34		// 외모 색 결정
#define FT_MENU_MOVE				35		// 메뉴를 드래그로 옮길때

#define FT_HIDE_WILLDO_AUTO_PUT		36		//	마우스 충돌때 그림을 바꿔 보여주고 클릭할때 또 그림을 바꾸면서 다른 메뉴를 콜할때
#define FT_HIDE_SPECIAL_WILLDO_AUTO_PUT  37		// 마우스 충돌 그림을 보여주고 클릭하면 여러가지 일을 할때

#define FT_HIDE_ISCROLL_AUTO_PUT		38
#define FT_HIDE_DSCROLL_AUTO_PUT		39	
#define FT_BANK_INVENTORY				40

#define FT_HIDE_AUTO_PUT			42
#define FT_JOB_ITEM					43

#define FT_TOWN_CONSTRUCTION		44	// 동서남북 체크 
#define FT_HOME_EDITBOX				45	// 내집 시스템에서 메시지 창 띄우기 버튼 깜박임
#define	FT_SWITCH_PUT				46	// 번갈아 가며 그림을 찍을 때
#define FT_SKILL_PUT				47	// 스킬 실행
#define FT_SKILL_QUICK				48

#define FT_OPTION_GAUGE				49	// 옵션 메뉴에서 게이지 바 보여주기
#define FT_SYSTEM_OPTION_SCROLL		50	// 시스템 옵션 메뉴에서 스크롤 바 움직이기

#define FT_KNOWLEDGE_ICON_PUT		51
#define FT_INN						52	// 여관에서 사용할 타입

#define FT_LEVELUP_BUTTON			53	// 게임 화면에 뜨는 버튼
#define	FT_LEVELUP					54	// 어빌리창에 뜨는 + 버튼들

#define FT_NOMAL_PUT_FX				55	// 메시지 창 뒤에 뜨는 화면 효과
#define	FT_BBS_GO					56	// 각각의 공지를 보기위한 필드

#define FT_PARTY					57	// 파티메뉴에관한 필드 타입
#define FT_PARTY_FACE				58	// 관계얼굴 보기

#define FT_OPENBOX_INV				59	// box_item창에서 선택및 화면 출력 필드
#define FT_DIVIDE_ITEM_PUT			60	// 돈 나누기 창에서 아이템 갯수 표시
#define FT_MAKE_ITEM				61	// 아이템 만들기 창에서 아이템 출력및 작업

#define FT_ITEM_MAKE_SELECT			62	// 만들수 있는 아이템 목록
#define FT_MAIN_MENU				63	// 메인 메뉴에서 파생되는 메뉴 버튼들... // FT_HIDE_SPECIAL_WILLDO_AUTO_PUT 과 같이 쓰인다.

#define FT_MAKE_ITEM_OK				64	// 만들어지는 아이템 연출 
#define FT_LEARN_ITEM				65	// 직업길드에서 아이템 배우기

#define FT_CHARACTER_ITEM			66	// 다른 캐릭터 아이템 띄우기

#define FT_TEXT_BUTTON				67	// 글자를 클릭하면 글자색을 달리 보여준다.

#define FT_ITEM_USE_SELECT			68	// 스킬 사용시 사용할 아이템 선택
#define FT_SELECT_FACE				69	// 캐릭터 만들기에서 얼굴 선택

#define FT_MAGIC_CLASS				70	// 매직 계열

#define FT_DSCROLL_BOOK				71
#define FT_ISCROLL_BOOK				72	// 마법책 넘기기

#define FT_MAGIC_BOOK_ETC			73	
#define FT_LEARN_MAGIC				74	// 마법책 인터페이스에서 마법 배우기

#define FT_HEAL_TEXT_BUTTON			75	//	치료할 병 버튼 출력

#define FT_DSCROLL_MEM_BOOK			76	//
#define FT_ISCROLL_MEM_BOOK			77	// 메모라이즈 마법 책 넘기기
#define FT_MEM_MAGIC_ICON			78	// 메모라이즈 마법 책에서 아이콘 버튼 
#define FT_MEM_BOOK_ETC				79	// 메모라이즈 마법 책에서 처음 항상 띄워놓아야할 이미지 출력

#define FT_PARTY_FACE_PUT			80	// 파티 얼굴 메뉴에서 사용

#define FT_VIEW_SMALL_MAP			81	// 뷰 스몰 멥
#define FT_MENU_MOVE_DIRECT			82	// 메뉴를 직접 움직일때

#define FT_EXCHANGE_ITEM			83	// 아이템 보여주기
#define FT_EXCHANGE_ITEM_CHECK		84	// 아이템 check
#define FT_EXCHANGE_ITEM_ETC		85

#define FT_TEST_CHECK				86	// 베타 테스트 관련~
#define FT_BETA_TEST_VIEW			87	// 베타 테스터에 대한 글 출력

#define FT_CREDIT_PUT				88
#define FT_INFO_PUT					89

#define FT_PUT_DISTANCE				90	// 이정표 출력
#define FT_STORE_SIGN				91	// 상점 간판

#define FT_AWAKE_BUTTON				92	//	깨어나는 버튼..

#define FT_GOD_INTRUDUCTION			93	// 신전 설명
#define FT_GOD_MEETING				94	// 신전 예배 설명
#define FT_GOD_REGIST_OKCANCEL		95	// 등록 하시겠습니까?

#define FT_PREACH_NAME				96	// 전도한 사람 이름을 적을 수 있게
#define FT_GOD_REGIST_ERROR			97	// 등록 에러

#define FT_QUICK_KEY				98	// 단축키

#define FT_VIEW_GOD_MEETING			99	// 음악이나 예배 내용 출력

#define FT_TIME_DISPLAY				100	// 시간 표시 
#define FT_HIDE_WILLDO_AUTO_PUT_MAGIC	101	// 메직 메뉴에서 QuickKey 출력을 위해

#define FT_MENU_MOVE_DEFAULT			102	// 메뉴 움직이기
#define FT_EAR_MESSAGE					103	// 귓속말
#define FT_EAR_MESSAGE_RECV				104	// 귓속말 받기

#define FT_MAIL_WRITE_RECV				105 // 메일 보내기 버튼
#define FT_MAIL_WRITE_TITLE				106
#define FT_MAIL_WRITE_BODY				107 // 메일 보내기 메뉴 닫기
#define FT_MAIL_WRITE					108	
#define FT_MAIL_TITLE_VIEW				109	// 받은 메일 목록 보기
#define FT_MAIL_READ					110	// 받은 메일 내용 보기

#define FT_BIRD							111	// 새 
#define FT_WILLDO_WORK					112	// willdo의 값을 work에 옮긴다.
#define FT_BELIEVE_GOD					113	// 믿는신과 신앙심 찍기
#define FT_SUPPER_INV					114	// 신전에서 음식 제공 메뉴

#define FT_SMALL_MAP_HOUSE				115	// 스몰맵에서 커서 가까이 가면 밝게 빛나며 집 이름 띄워주기
#define FT_EXPLAIN_MENU					116	// 메뉴 설명 보여주기
#define FT_QUICK_MAGIC_PLUS				117	// 사용한 마법 저장
#define FT_QUICK_MAGIC_ARRAY_SCROLL		118		// 퀵 마법 배열 스크롤 다운 
#define FT_XY_DISPLAY					119		// 좌표 디스플레이 
#define FT_TIME_DISPLAY_ON_OFF			120		// 시간 메뉴 활성화 버튼
#define FT_CHAT_SCROLL					121		// 채팅시 스크롤 버튼 
#define FT_SELECT_CITY					122		// 시작화면에서 연결할 마을 선택
#define FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2	123		// 두개를 같이 찍을때
#define FT_FIRST_QUICK_KEY					124		// 가장 우선 순위로 키 처리를 해야 할때( 다른 경우를 배제 하고 꼭 이루어 져야 할 단축키 )

#define FT_PUBLIC_BBS						125		// 공지사항 띄우기

#define FT_QUICK_SKILL_PUT					126		// 스킬 메뉴
#define FT_QUICK_SKILL_SCROLL				127		// 스킬 스크롤 - 제목판
#define FT_QUICK_SKILL_SCROLL_LR			128		// 스킬 스크롤 - 왼쪽 오른쪽

#define FT_PUBLIC_BBS_NEXT					129
#define FT_HUNTER_REGIST					130		// 현상범 메뉴 보여주가
#define FT_HUNTER_LIST						131		// 현상범 리스트 보여주기
#define FT_HUNTER_LIST_SCROLL				132		// 현상범 리스트 스크롤
#define FT_DEFAULT_BUTTON					133
#define FT_QUEST_OK							134
#define FT_QUEST_SCROLL						135
#define FT_QUEST_INDEX						136
#define FT_QUEST_INDEX_SCROLL				137
#define FT_MAN_TO_MAN_CHECK					138		// 일대일 대결 체크 필드

#define FT_RESET_JOB_EXPLAIN				139		// 직업 재 선택 설명 필드
#define FT_RESET_JOB_SCROLL					140		// 직업 재 선택에 따른 설명 스크롤 필드
#define FT_MAN_TO_MAN_ITEM					141

#define FT_COLOR_CHANGE_GAUGE				142		// 0905 kkh 추가 
#define FT_SELECT_NATION					143		// 나라 선택
#define FT_SELECT_NATION_EXPLAIN			144		// 나라 설명
#define FT_SELECT_NATION_SCROLL				145

#define FT_NATION_BBS						146		// 나라 헌장문 출력
#define FT_NATION_BUTTON					147		// 나라 헌장문 버튼들
#define FT_FIGHT_MAP_SIGN					148		// 파이트 맵 전광판 사라지는것 확인

#define FT_DISPLAY_SCENARIO					149		// 시나리오 출력

#define FT_VOTE_REGIST						150		// 등록 메뉴 설명글 출력
#define FT_VOTE_SUPPORT						151		// 투표하기

#define FT_VOTE_SCORE_PUT					152		// 득표상황 보여주기
#define FT_GM_LIST							153		// 그랜드 마스터 등록하기
#define FT_GM_ITEM_EXPLAIN					154		// 그랜드 마스터 등록 조건 아이템 설명해주기
#define FT_GM_ITEM_EXPLAIN_BUTTON			155		// 버튼 클릭
#define FT_CHANGE_MAP						156
#define FT_CHANGE_MAP_BUTTON				157		// 직업 바꾸기 클릭
#define FT_TEXT_BUTTON_EXIT					158		// exit 버튼
#define FT_DISPLAY_EXP						159		// skill경험치 보여주기 (게이지 포함)
#define FT_VIEW_TIP							160		// tip 버튼 반짝이게

#define FT_PUT_TACTICS						161		// tactics 선택창에서 한글 택틱 그림 보여주기
#define FT_SCROLL_TEXT_PUT					162		// work : 페이지 수, key : 한 페이지에 출력 가능한 라인,  scroll	// cheaktype 을 1로 셋팅해주는데가 따로 존재해야 한다.
#define FT_SCROLL_BUTTON					163		// 스크롤 버튼

#define FT_SALVATION_MONEY					164		// 기부금액
#define FT_SALVATION_MONEY_SCROLL			165		// 기부금액 스크롤

#define FT_USED_MAGIC_EXPLAIN				166		// 사용 마법 설명 보여주기
#define FT_ANIMATION						167

#define FT_TEMBATLE_INFO					168		// 팀배틀에 들어온 사람 정보 보여주기

#define FT_SELECT_SERVER_BUTTON				169		// 서버 선택 메뉴 중 텍스트 버튼 
#define FT_SELECT_SERVER_SCROLL				170		

#define FT_TEMBATLE_ADD						171		// 팀에 들어간다.
#define FT_EAR_BUTTON						172		// 귓속말 button
#define FT_CHANGE_MONEY						173		// 환전소

#define FT_BATTLE_WAIT						174		// 팀 배틀 등록
#define FT_VIEW_BATTLE_MEMBER				175		// 팀 멤버 보여주기

#define FT_DELAY_BUTTON						176		// 딜레이 버튼
#define FT_HIDE_SPECIAL_WILLDO_DELAY_AUTO_PUT	177		// 시간이 지나야 체크가 가능하다 FT_DELAY_BUTTON과 쌍으로 쓰인다.
#define FT_BANK_LOAN							178
#define FT_VIEW_BATTLE_WAIT_MEMBER				179		// 대기자 명단 보여주기
#define FT_FIGHTMAP_START						180		// 배틀을 시작한다.

#define FT_CHECK_COMBO							181		// 라디오버튼 구현 nValue 와 nWillDo 를 이용하여 구현
#define FT_VIEW_BBS_MONEY						182		// 공지 가격 출력
#define FT_WRITE_BBS							183		// 에디터 박스 하나 띄우고 그 입력 스트링을 디폴트하게 출력시켜준다.

/////////////////////////////////////////////////////////////////////////////////
//출력관련 정의  DP_와 FT_는 비슷하지만 FT_는 항상 같은것을 보여주는데 쓰고 DP_는 수치등과 같이 수시로 바뀌는걸 보여줄때 슨다
#define DP_GENDER                     1             //성별
#define DP_NAME						  2             //이름
#define DP_ARIGEMENT				  3             //가치관
#define DP_CLASS					  4             //클래스
#define DP_SPELL					  5             //스펠
#define DP_TACTICS					  6             //택틱스남
#define DP_JOB						  7             //직업
#define DP_STR						  8             //STR
#define DP_CON						  9             //
#define DP_DEX						  10		    //
#define DP_WIS						  11	        //
#define DP_INT						  12            //
#define DP_CHA						  13            //
#define DP_MOVP						  14            //
#define DP_ENDU						  15		    //
#define DP_MOR						  16            //
#define DP_WSPS						  17            //
#define DP_LUCK						  18            //  
#define DP_NAMEEDITBOX                19            //이름이 써지는 칸

#define DP_FACE                       20            //얼굴 표정을 출력

#define DP_INVENTORY                  21            //아이템 메뉴에서 아이템몰록을 출력

#define DP_LEVEL                      22            //레벨을 출력
#define DP_EXP                        23            //경험치를 출력

#define DP_ARIGEMENT_BAR              24            //가치관이 나오는 메뉴에서 출력되는 막대그래프

#define DP_HP_BAR                     25            //메인 인터페이스 에서 HP막대 게이지를 보여준다
#define DP_MP_BAR                     26            //메인 인터페이스 에서 MP막대 게이지를 보여준다.

#define DP_HP                         27            //HP수치를 보여준다
#define DP_MP                         28            //MP수치를 보여준다.

#define DP_MAIN_LEVEL			      29			//메인 인터페이스에서 레벨값을 보여준다.
#define DP_MAIN_STR				      30			//메인 인터페이스에서 STR값을 보여준다.


#define DP_ACCEL_INVENTORY            31             //단축 아이템창에서 실제로 아이템 이미지를 보여준다

#define DP_ARMOR_INVENTORY            32             //무기 장비 창에 실제로 아이템 이미지를 보여준다

#define DP_CONDITION                  33            //메인 인터페이스에서 캐릭터 상태를 나타내는 아이콘 출력
#define DP_FACEIMAGE				34				// 얼굴 이미지 찍기

#define DP_GUILD					35
//#define DP_WSPS						36
#define DP_TACTICE					37
#define DP_DAMAGE					38
#define DP_HEALTH					39
#define DP_MOV						40
#define DP_HEAT						41
#define DP_AC						42
#define DP_FAME						43
#define DP_MONEY					44
#define DP_IDEDITBOX				45				// 아이디 출력
#define DP_PASSEDITBOX				46				// 패스워드 출력

#define DP_HEALTH_GAUGE				47
#define DP_LEVEL_LOGON              48            //로그온 화면에서 레벨을 출력

#define DP_TEMP_MONEY				49				// 은행에서 입력받는 창에 금액 출력
#define DP_BANK_SAVE				50				//
#define DP_BANK_TOTAL				51				// 은행에서 총예금액 출력
#define DP_BANK_INVENTORY			52

#define DP_WEIGHT					53				// 무게 출력
#define DP_BANK_CURRENT_PAY			54				// 아이템 보관료
#define DP_BANK_PAY					55

#define DP_CREDIT_TEXT				56				// 신용 등급 출력
#define DP_LOAN_TOTAL				57				// 대출에 총액
#define DP_LOAN_ABLE				58				// 대출 가능액
#define DP_DAY_TEXT					59				// 상환 날짜 표시

#define DP_REPAYMENT_MONEY			60				// 먼저 갚아야할 금액
#define DP_REPAYMENT_DATE			61				// 먼저 상환해야할 기한

#define DP_GUILD_INVENTORY			62
#define DP_SELECT_ITEM				62
#define DP_CHARACTER_MONEY			63				// 캐러가 가지고 있는 돈 출력 -> nWillDo에 출력할 칸의 길이  x,y 에 좌표값을 넣어준다

#define DP_STRING_PUT				64				// 문자열 출력
#define DP_RECT_STRING_PUT          65				//신전메뉴1번에서 MEssage출력
#define DP_SKILL_ICON				66				// 스킬메뉴에서 스킬 아이콘 찍기
#define	DP_HEALING_EXPLAIN			67				// HEALING 기술 설명

#define DP_NAME_SELECT				68				// 캐릭터 선택창에서 사용할 데이터
#define DP_LAST_LOAN				69







#define DP_TACTICS_DATA				75

#define DP_ABILITY					76				// 어빌리티 창에서 사용
#define DP_DATAPUT					77				// 레벨업 포인트		// nWilldo에는 어떤 능력치 인지, nRectImage 에는 길이를 넣는다.
#define DP_RESIST					78				// 어빌리티 창에서 resist에서 사용
#define DP_EXP_NEXT					79

#define DP_AGE_MAKE					80				// 나이 결정
#define DP_AGE						81				// 나이 

#define DP_TOWNGALL_TITLE			82				// 게시판 제목 출력
#define DP_TOWNGALL_TITLE_DATE		83				// 게시판 날짜 
#define DP_BBS_PUT					84				// 게시판 내용 찍기 
#define DP_BBS_WRITE_PUT			74


#define	DP_HOUSE_PUT				85				// 빌딩보여주기
#define DP_HOUSE_REPAIR				86				// 건물 수리메뉴 

#define DP_ITEM_MESSAGE				87				// item 설명 창
#define DP_SELECT_SELECT			88				// 아이템 만들기 메뉴에서 선택한 아이템 표시
#define DP_SELECT_SKILL				89				// 스킬 배우기에서 배우고자 하는 스킬을 보여준다.
#define DP_USE_SELECT				90				// 스킬 사용시 선택된 사용 아이템 보여주기
#define DP_MAGIC_PUT				91				// 마법책 인터페이스에서 매직 아이콘 보여주기
#define DP_ITEM_PUT					92				// 정해진 좌표에 아이템 찍기

#define DP_DISEASE_PUT				93				//	내가 걸린 병 출력

#define DP_HEAL_MONEY				94				// 치료비
#define DP_PREVENT_MONEY			95				// 예방비

#define DP_INN						96				// 여관에서... 선택 막대 계속 보여주기
#define DP_MAGIC_CLASS				97				// 메모라이즈 메뉴에서 계열 보여주기...
#define DP_MEMORIZE_POINT			98				// 메모라이즈 메뉴에서 포이트 관련 수치
#define DP_NORMAL_PUT				99				// 0908 kkh 추가  보여주기
#define DP_DATAPUT_DEFAULT			100				// 가운데 정렬로 값 보여주기 value변수 사용, 칼라값 표현

#define DP_MONEY2					101				// , 를 이용해 가지고 있는 현재 돈 출력..
#define DP_BACK_IMAGE_VALUE_TEXT	102				// 투명 막 뒤에 그리고 그 위에 value 변수의 값을 출력한다.
#define DP_TITLE_PUT				103				// 메뉴등에 타이틀을 찍기 위해...



/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
// FT_DO에 쓰일 정의(1번, 3번, 26번은 사용중)
#define DO_CHARACTER				10				//캐릭터 전신 찍기
#define DO_TEXT_PUT					11				//메시지 박스에서 문자열 출력
#define DO_BANK_SCROLL_UP			35				// 은행 돈 올리고 내리기 
#define DO_BANK_SCROLL_DOWN			36
#define DO_BANK_TIME_SCROLL_UP		100				// 경매 날짜 조정하기
#define DO_BANK_TIME_SCROLL_DOWN	101
#define DO_BANK_SCROLL_LEFT			102				// 경매 물건 찾기
#define DO_BANK_SCROLL_RIGHT		103

#define DO_SKILL_ICON_SELECT		114				// 스킬 메뉴 스크롤 할 경우 이미지 바꿔주기
#define DO_SKILL_ICON				115				// 아이콘과 타이틀 바를 보여주고 아이콘 실행시키기

#define DO_FACE_MENU				200				// 표정 선택 창
#define DO_SELECT_LOAD_CHAR			210				// 로드할 캐릭터정보ㅣ

#define DO_CHATTING_VIEW			300				// 채팅창에 대화 보여주기
#define DO_CHATTING_EDITBOX			310				// 채팅창에 글 쓰기 에디터 박스

#define DO_LEARNSKILL_ICON			320				// 배울수 있는 스킬 보여주기

#define DO_DIVIDE_SCROLL_UP			330				// 돈 나누기 창에서 스크롤 버튼
#define DO_DIVIDE_SCROLL_DOWN		340


/////////////////////////////////////////////////////////////////////////////////////////////////
//움직여야 하는 메뉴 관련 선언
#define MM_STATUS_OPEN                1             //움직이는 메뉴중 스테이터스 메뉴가 등장
#define MM_STATUS_CLOSE               2             //움직이는 메뉴중 스테이터스 메뉴가 사라짐
#define MM_ABILITY_CLOSE              3             //움직이는 메뉴중 어빌리티 메뉴가 사라짐
#define MM_ARIGEMENT_CLOSE            4             //움직이는 메뉴중 가치관 메뉴가 사라짐
#define MM_MAGIC_WIZARD1_OPEN         5             //설명 생략
#define MM_MAGIC_WIZARD1_CLOSE        6             //설명 생략
#define MM_MAGIC_WIZARD2_CLOSE        7             //설명 생략
#define MM_MAGIC_WIZARD3_CLOSE        8             //설명 생략
#define MM_MAGIC_PRIEST1_OPEN         9             //    "
#define MM_MAGIC_PRIEST1_CLOSE        10            //    "
#define MM_MAGIC_PRIEST2_CLOSE        11            //    "
#define MM_MAGIC_PRIEST3_CLOSE        12            //    "
#define MM_ITEM_OPEN                  13            //    "
#define MM_ITEM_CLOSE                 14            //    "
#define MM_PARTY_OPEN				  15			//움직이는 메뉴중 파티메뉴 등장
#define MM_PARTY_CLOSE				  16			//움직이는 메뉴중 파티메뉴 사라짐
#define MM_TACTICS_CLOSE			  17			//움직이는 메뉴중 파티메뉴 사라짐

#define MM_BOX_OPEN					  18			// 박스 아이템이 열리는 경우
#define MM_BOX_CLOSE				  19			// 박스 아이템이 닫히는 경우


#define MOVE_OPEN						1		
#define MOVE_CLOSE						0