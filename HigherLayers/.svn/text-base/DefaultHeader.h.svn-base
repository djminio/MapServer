#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sqlext.h>
#include <winerror.h>
#include <crtdbg.h>

#define _NATION_APPLY_

#define ATTACKPOWER_PRO_	140
#define GET_EXP_PC_AND_PC_	30// PC°£¿¡´Â °æÇèÄ¡¸¦ NPC¿¡ ÀÇÇØ ¾ò´Â ¼öÄ¡¿¡ ¾Æ·¡¼öÄ¡·Ñ ³ª´©¾î ÁØ´Ù. ( µÇ°Ô ÂÉ±ÝÁØ´Ù. )
#define ULTRA_PK_NUM_		100// ±Ø¾ÇPKÀÎ°æ¿ì ¾Æ·¡ ¼öÄ¡¸¦ ³Ñ°ÔµÇ¸é name_status.ultra_pk == 1ÀÌµÇ°í °ø°ÝÀ» ¸øÇÏ°Ô µÈ´Ù. 

///////////////////////////////////////////////////////////////////////////////
//< CSD-030517
const int Num_Of_Effect = 700;
const int Num_Of_Magic = 251;
const int Num_Of_NPC_Type = 100;
const int Num_Of_NPC_Lev = 256;
const int Num_Of_NPC_Name = 7932;
const int Num_Of_NPC_Generation	= 398;

const int MAX_LOG_TYPE = 4;     // CSD-TW-030622
const int MAX_COMBAT_LEVEL = 3; // CSD-TW-030622
const int MAX_NATION = 7;
const int MAX_AREA_BUFFER =	1000;
const int MAX_MAUL = 10;
const int MAX_MAP_IN_MAUL = 20;
const int MAX_GAMBLE_ITEM_KIND = 8;
const int MAX_GAMBLE_ITEM_TYPE  = 7;
const int MAX_GUARDIAN_GUILD = 3;
const int MAX_MAXIMUM_NUMBER = 24; // CSD-TW-030704
const int MAX_CDUNGEONBASIC_INT = 6; // CSD-030723
const int MAX_PARTY_MEMBER = 6; // CSD-030723
const int MAX_SKILL_NO	= 200; // CSD-030723
const int MAX_DIR_TABLE = 256; // CSD-030723
const int MAX_SHORTPATH = 100; // CSD-030723 // LTS AI
const int MAX_LEARN_ITEM = 1000; // CSD-030723
const int MAX_PC_CONTROL_NPC = 32; // CSD-030723
const int MAX_CLASS = 5; // CSD-030723
const int MAX_RESIST_TYPE = 6; // CSD-030723
const int MAX_GENERATE_POSITION_ = 100; // CSD-031106
const DWORD MAX_BANK_MONEY = 4000000000; // CSD-030723
//< CSD-030723
const double HT_NORMAL_CHANGE_VALUE = 0.003;	//°¡¸¸È÷ ÀÖÀ»¶§ ±ðÀÏ °ª
const double HT_WALK_CHANGE_VALUE = 0.013;	//ÇÇ½º¸ðµå °ÉÀ»¶§
const double HT_RUN_CHANGE_VALUE = 0.030;	//ÇÇ½º¸ðµå ¶Û¶§
const double HT_BATTLE_WALK_CHANGE_VALUE = 0.020;	//¹èÆ²¸ðµå °ÉÀ»¶§
const double HT_BATTLE_RUN_CHANGE_VALUE = 0.040;	//¹èÆ²¸ðµå ¶Û¶§
const double HT_ATTACK_CHANGE_VALUE = 0.030;	//¹°¸®°ø°Ý½Ã
const double HT_MAGIC_CHANGE_VALUE = 0.030;	//¸¶¹ý»ç¿ë½Ã
const double HT_SKILL_CHANGE_VALUE = 0.050;	//Á÷¾÷½Ã
// Á×¾úÀ»¶§ ¾à 5ÃÊ¸¶´Ù CheckÇÑ´Ù. 
const int DEAD_CHARACTER_CHECK_TIME = 5;
// Á×Àº´ÙÀ½ »ç¶ó½Ã°Å³ª, Ghost·Î ¹Ù²î´Â ½Ã°£À» Á¤ÇÑ´Ù. 
// ´ÜÀ§´Â DEAD_CHARACTER_CHECK_TIME(ÃÊ)ÀÌ´Ù. 
const int DEAD_PC_COUNT_ = 65;
const int DEAD_NPC_COUNT_BOTTOM_ = 5; // NPC : ((DEAD_PC_COUNT_ - DEAD_NPC_COUNT_BOTTOM_ ) * DEAD_CHARACTER_CHECK_TIME )ÃÊ°£ ½ÃÃ¼ÀÎ»óÅÂ°¡ Áö¼ÓµÈ´Ù. 
const int DEAD_PC_COUNT_BOTTOM_ = DEAD_PC_COUNT_ - 5;	// PC´Â ½ÃÃ¼ÀÎ»óÅÂ¸¦ (5 * DEAD_CHARACTER_CHECK_TIME)ÃÊ°£¸¸ Áö¼ÓµÈ´Ù. 
const int DEAD_NPC_ANIMAL_COUNT_ = 25;
const int DEAD_NPC_HUNT_MONSTER_COUNT_ = 6; // CSD-040407
const int DEAD_NPC_MONSTER_COUNT_ = 15;
const int DEAD_SEALSTONE_COUNT_ = (2*60*60)/5;		// LTS NEW LOCALWAR
//> CSD-030723
const int CIB_POTION_BOX = 70; // CSD-030723
const int LOTTO_PAY = 20000; // CSD-TW-030704 : ·Î¶Ç ±¸ÀÔ ºñ¿ë
const int GUILD_WEAR01	   = 102; // ÁöÅ°¹Ì ±æµåº¹

const int PS_EXP		= 10;  // ±×³É °æÇèÄ¡
const int PS_TACTIC	= 20;  // ÅÃÆ½ °æÇèÄ¡
const int PS_MAGIC_TAC	= 30;  // ¸¶¹ý ÅÃÆ½ °æÇèÄ¡
const int NO_TAC		= 13;  // ÀüÅõ¿¡ »ç¿ëµÇ´Â ÅÃÆ½¼ö
const int ULTRA_NK     = 100; // Ultra NK ¼öÄ¡
const int ENABLE_DUMMY_LEVEL = 10; // CSD-030806
const int ENABLE_DUEL_LEVEL = 10; // CSD-030806
const int ENABLE_GM_LEVEL = 20; // CSD-030806
const int ENABLE_NATIONWAR_LEVEL = 10; // CSD-030806
const int ENABLE_PARTY_LEVEL = 35; // CSD-030806
const int ENABLE_FREE_LEVEL = 50; // CSD-030806
const int ENABLE_HOMETOWN_LEVEL = 20; // CSD-030806
const int BUILD_GUILD_LEVEL = 40; // CSD-030806
const int CROSSING_CLASS_LEVEL = 101; // CSD-030806

enum LOG_TYPE 
{
	LT_ACCELATER = 0,
	LT_BATTLE = 1,
	LT_CHANGE = 2,
	LT_CHECK = 3,
	LT_LEVEL_UP = 4,
	LT_NORMALCHAT = 5,
	LT_SCRIPT	= 6,	// 030826 kyo
	LT_AUCTION	= 7,	// BBD 040226
	LT_EVENTITEM = 8,	// BBD 040308
	LT_LOCAL_WAR = 9,   // CSD-040407
	LT_DUPE = 10,		// Thralas
	LT_TELEPORT = 11,	// Finito
	LT_ETC = 12,
	LT_EARCHAT = 13,	// Finito
	LT_ATTACKRANGE = 14, // Finito 25/08/07
	LT_TIMECHECKS = 15, // Finito 25/08/07
	LT_TILECHECKS = 16, // Finito 26/08/07
	LT_GUILDCHAT = 17, // Finito 18/09/07
	LT_PARTYCHAT = 18, // Finito 18/09/07 
	LT_COLOSSUSCHAT = 19, // Finito 18/09/07 
	LT_VENDOR = 20, // Rogerio
	LT_HACK = 21, // Rogerio 
	LT_MANTLEDYE = 22, // Finito
	LT_NATIONTRANSFER = 23, // Finito 12/06/09
	LT_SEXCHANGE = 24, // Finito 12/06/09
	LT_TACTICUP = 25, //Eleval 15/08/09 - Log when Tactic Level Up >= 100
};

//<! BBD 040308 ÀÌº¥Æ® ¾ÆÀÌÅÛ ·Î±× Å¸ÀÔ
enum EVENTITEM_LOG_TYPE
{
	// EILT = Event Item Log Type
	EILT_INVALID_CONNECTION = 0,		// ÄÜ³Ø¼ÇÀÌ ²÷¾îÁ³´Ù
	EILT_INVALID_NAME = 1,				// ÀÌ¸§ÀÌ ´Ù¸£´Ù
	EILT_NOTENOUGH_INVENTORY = 2,		// ¿äÃ»ÇÑ »çÀÌ ÀÎº¥À» Ã¤¿ü´Ù.
};
//> BBD 040308 ÀÌº¥Æ® ¾ÆÀÌÅÛ ·Î±× Å¸ÀÔ

//<! BBD 040226 ¿Á¼Ç·Î±× Å¸ÀÔ
enum AUCTION_LOG_TYPE
{
	// °Å·¡°¡ ¿ÏÀüÈ÷ Á¾·áµÉ¶§¿¡ ·Î±×°¡ »ý°Ü¾ß ÇÔ
	ACLT_SELLER_REGIST = 0,			// ÆÄ´Â ³ðÀÌ µî·ÏÇÒ¶§ »ý±â´Â ·Î±×
	ACLT_SELLER_CANCLE = 1,			// ÆÄ´Â ³ðÀÌ Ãë¼ÒÇÒ¶§ »ý±â´Â ·Î±×
	ACLT_BUYER_TAKE = 2,			// »ç´Â ³ðÀÌ ¹°°ÇÀ» °¡Á®°¨À¸·Î °Å·¡°¡ Á¾·áµÉ¶§ »ý±â´Â ·Î±×
	ACLT_SELLER_TAKE = 3,			// ÆÄ´Â ³ðÀÌ ´ë±ÝÀ» È¸¼öÇÔÀ¸·Î °Å·¡°¡ Á¾·áµÉ¶§ »ý±â´Â ·Î±× 
};
//> BBD 040226 ¿Á¼Ç·Î±× Å¸ÀÔ

enum DAY_TYPE
{	//< CSD-030804
	SUNDAY = 0,
	MONDAY = 1,
	TUESDAY = 2,
	WEDNESDAY = 3,
	THURSDAY = 4,
	FRIDAY = 5,
	SATURDAY = 6
};	//> CSD-030804

enum NATION_TYPE
{	
	N_NEUTRAL = 0,
	N_NONSTER = 2,
	N_VYSEUS = 3,
	N_ZYPERN = 4,
	N_HEGEMONIA = 5,
	N_YLLSE = 6,
	N_NATION_ALL = 7
};

enum HOUSE_TYPE
{	//< CSD-030723
	BLACKSMITH = 0,	 // ´ëÀå°£
	FARM = 1,		 // ³óÀå
	RESTAURANT = 2,	 // À½½ÄÁ¡
	HERBSHOP = 3,	 // ¾àÃÊÁ¡
	SHAMBLES = 4,	 // ÇªÁÖ°£
	CANDLE = 5,		 // ¾çÃÊÁ¡
	SLOPSHOP = 6,	 // ¿Ê°¡°Ô 
	ALCHEMY = 7,	 // È° È­»ì
	WOODWORKING = 8, // ¸ñ°ø¼Ò
	BOWCRAFT = 9,	 // È° È­»ì
	SHIPYARD = 10,	 // Á¶¼±¼Ò
	PUB = 11,	     // ÁÖÁ¡
	HOSPITAL = 12,	 // º´¿ø
	STORE = 13,		 // ÀâÈ­Á¡
	INN = 14,		 // ¿©°ü
	MAGIC_GUILD = 15,
	STORE_TOTAL = 17
};	//> CSD-030723

enum LIFE_TYPE
{	//< CSD-030723
	REMOVE_ = 0,
	ALIVE_ = 1,	
	DEAD_ = 2,
	BUFE_ = 3
};	//> CSD-030723

enum NK_TYPE
{	
	NK_TYPE_STEAL_ = 0,
	NK_TYPE_KILL_ = 1,
	NK_TYPE_NO_KILL_ = 2
};

enum CLASS_TYPE
{	//< CSD-030723
	WARRIOR	= 0,
	THIEF = 1,
	ARCHER = 2,
	WIZARD = 3,
	PRIEST = 4,
	CREATURE = 5
};	//> CSD-030723

enum CHAR_STATUS_TYPE
{	//< CSD-030723
	CS_NOMAL = 0,	// Á¤»ó »óÅÂ
	CS_REST = 1,	// ÈÞ½Ä »óÅÂ
	CS_SLEEP = 2	// ³·¿¡ ÀÚ±â »óÅÂ
};	//> CSD-030723

enum VIEW_TYPE
{	
	VIEWTYPE_NORMAL_ = 0,
	VIEWTYPE_CAMELEON_ = 1,
	VIEWTYPE_GHOST_	= 2,
	VIEWTYPE_TREE_	= 3,
	VIEWTYPE_OBSERVE_ = 4,
	VIEWTYPE_CONFUSION_	= 5,
	VIEWTYPE_POISON_ = 6,
	VIEWTYPE_STONE_	= 7,
	VIEWTYPE_SLOW_ = 8,
	VIEWTYPE_FROZEN_ = 9,
	VIEWTYPE_CURSE_	= 10,
	VIEWTYPE_TRANSPARENCY_ = 11,
	VIEWTYPE_SELECT_ = 12,
	VIEWTYPE_STAND_UP_ = 13
};

enum CONDITION_TYPE
{	//< CSD-030723
	CON_NORMAL = 0x00,	// Á¤»óÀÇ »óÅÂ	 00000000
	CON_CURSE = 0x01,	// ÀúÁÖÀÇ »óÅÂ	 00000001
	CON_FROZEN = 0x02,	// ³Ãµ¿ÀÇ »óÅÂ   00000010
	CON_SLOW = 0x04,	// °¨¼ÓÀÇ »óÅÂ   00000100
	CON_DAZE = 0x08,	// È¥¶õÀÇ »óÅÂ	 00001000
	CON_POISON = 0x10,	// Áßµ¶ÀÇ »óÅÂ	 00010000
	CON_STONE = 0x20,	// ¼®È­ÀÇ »óÅÂ	 00100000
	CON_DEATH = 0x40,	// Á×À½ÀÇ »óÅÂ	 01000000
	CON_DARKNESS = 0x80 // ¾îµÒÀÇ »óÅÂ   10000000
};	//> CSD-030723

enum HUNGRY_TYPE
{	//< CSD-030723
	HT_NORMAL = 0, // (-(HT_WALK * 40.))
	HT_WALK = 1,
	HT_RUN = 2,
	HT_BATTLE_WALK = 3,
	HT_BATTLE_RUN = 4,
	HT_ATTACK = 5,
	HT_MAGIC = 6,
	HT_SKILL = 7,
	HT_COMBAT = 8
};	//> CSD-030723

enum DIRECTION
{	//< CSD-030723
	DIRECTION_SAME = 0,
	DIRECTION_UP = 4,
	DIRECTION_RIGHTUP = 5,
	DIRECTION_RIGHT	= 6,
	DIRECTION_RIGHTDOWN	= 7,
	DIRECTION_DOWN = 0,
	DIRECTION_LEFTDOWN = 1,
	DIRECTION_LEFT = 2,
	DIRECTION_LEFTUP = 3	
};	//> CSD-030723

enum HIT_TYPE
{ 
	HIT_FAILED = 0,
	HIT_AND_DEAD = 1,
	HIT_AND_NOTDEAD = 2,
	HIT_AND_ALIVE = 3,
	HIT_GHOSTRESURRECTION = 4,
	HIT_SUMMONING = 5
};

enum RESIST_TYPE
{	//< CSD-030723
	RESIST_UNKNOWN = 0,
	RESIST_FIRE = 1,
	RESIST_ICE = 2,
	RESIST_ELECT = 3,
	RESIST_POISON = 4,
	RESIST_CURSE = 5,
	RESIST_HOLY = 6,
	RESIST_IGNORE = 7
};	//> CSD-030723

enum eRESIST_TYPE
{
	RT_POISON	=0,
	RT_CURSE	=1,
	RT_HOLY 	=2,
	RT_FIRE		=3,
	RT_ICE		=4,
	RT_ELECT	=5
};

enum TACTIC_TYPE
{ 
	CRAPPLE_ = 0,
	SWORD_ = 1,
	ARCHERY_ = 2,
	FENCING_ = 3,
	MACE_ = 4,
	PIERCE_ = 5,
	WHIRL_ = 6,		
	HURL_ = 7,
	PARRYING_ = 8,
	D_SWORD_ = 9,
	D_MACE_ = 10,
	MAGERY_ = 11,
	ORISON_ = 12
};

enum JOB_TYPE
{	//< CSD-030723
	J_FAMER = 20,
	J_MINNER = 21,
	J_FISHER = 22,
	J_LUMBERJACK = 23,
	J_BUTCHER = 24,
	J_HERB_DIGGER = 25,
	J_HERDSMAN = 26,
	J_CARPENTER = 27,
	J_BLACKSMITH = 28,
	J_COOKER = 29,
	J_TAILOR = 30,
	J_BOWCRAFT = 31,
	J_ALCHEMIST = 32,
	J_CANDLEMAKER = 33,
	J_MERCHANT = 34
};	//> CSD-030723

enum MAGIC_TYPE
{ 
	MT_PROTECTION = 2,
	MT_ASSISTANCE = 3,
	MT_CURSE = 4
};

enum CHAR_STEP_TYPE
{	//< CSD-TW-030620
	CLS_STEP = 0,  // µà¾ó Å¬·¡½º ´Ü°è
	DUAL_CLS = 1,  // µà¾ó Å¬·¡½º Á¾·ù
	EXP_STEP = 2,  // °æÇèÄ¡ ´Ü°è
	T01_STEP = 3,  // ÅÃÆ½ °æÇèÄ¡ ´Ü°è
	T02_STEP = 4,
	T03_STEP = 5,
	T04_STEP = 6,
	T05_STEP = 7,
	T06_STEP = 8,
	T07_STEP = 9,
	T08_STEP = 10,
	T09_STEP = 11,
	T10_STEP = 12,
	T11_STEP = 13,
	T12_STEP = 14,
	T13_STEP = 15,
	CSP_STEP = 16, // ÀüÅõ½ºÅ³ Æ÷ÀÎÅÍ
	MAX_STEP = 17  // ÃÖ´ë µ¥ÀÌÅ¸¼ö
};	//> CSD-TW-030620

enum WEAPON_RANGE_TYPE
{	//< CSD-031007
	WR_SHORT = 0,
	WR_MIDDLE = 1,
	WR_LONG1 = 2,
	WR_LONG2 = 3,
	WR_LONG3 = 4,
	WR_EFFECT = 5
};	//> CSD-031007

enum BATTLE_TYPE
{	//< CSD-031007
	// ¹°¸®Àû °ø°Ý 
	PHYSICAL = 0,                  
	// ¸¶¹ý
	LIFE_UP = 1,                   // Ã¼·ÂÈ¸º¹ 
	PROTECTION = 2,                // º¸È£
	SHIELD = 3,                    // º¸È£¸·
	DECIPHER = 4,                  // ÇØµ¶ ¸¶¹ý
	HEAL_DISEASE = 5,              // Áúº´Ä¡·á
	GREAT_HEAL = 6,                // ´ëÃ¼·ÂÈ¸º¹
	SLOW = 10,                     // ¼ÓµµÀúÇÏ
	CONFUSION = 11,                // È¥¶õ
	INTOXICATION = 12,             // Áßµ¶
	STONELY =	13,                // ¼®È­
	ABSOLUTE_PARALYSIS = 14,       // Àý´ë¸¶ºñ
	TOXIC_CLOUD = 15,              // µ¶±¸¸§  
	DISEASE_INDUCE = 16,           // Áúº´À¯¹ß 
	VAMPIRE_TOUCH = 17,            // ¹ìÆÄÀÌ¾îÀÇ ¼Õ±æ
	SPIDER_WEB = 19,               // °Å¹ÌÁÙ  
	MAGICAL_TRAP = 20,             // ¸¶¹ýÆ®·¦
	ENCHANT_WEAPON = 21,           // ÀÎÃ¾Æ®¿þÆù
	TRANSPARENCY = 22,             // Åõ¸íÁÖ¹®
	NULLITY_MAGIC = 24,            // ¸¶¹ý¹«È¿È­  
	MINOR_AMPLIFY = 25,            // ¼ÒÁõÆø
	MAGIC_AMPLIFY = 26,            // ¸¶¹ýÁõÆø
	FIRE_RESIST = 28,              // ºÒ¿¡ ´ëÇÑ ÀúÇ×  
	FIRE_ARROW = 29,               // È­¿°È­»ì 
	FIRE_BALL= 30,                 // È­¿°±¸
	SCORCHER = 31,                 // È­¿°±º(È­¿°¹æ»ç)
	CIRCLE_FLAME= 33,              // ¹æ»çÈ­¿°
	FIRE_WALL = 34,                // È­¿°º®
	FLAME_WAVE = 35,               // ¿òÁ÷ÀÌ´Â È­¿°º®
	APOCALYPSE = 36,               // Áö¿ÁÀÇ ºÒ
	ICE_RESIST = 37,               // ºù°è ÀúÇ×
	ICE_ARROW = 38,                // ¾óÀ½È­»ì
	ICE_BALL = 39,                 // ¾óÀ½±¸ 
	FREEZE_5TILE_RADIUS = 41,      // ¹Ý°æ 5Å¸ÀÏ ¾ó¸²	
	ICE_WALL = 42,                 // ¾óÀ½º®  	
	CIRCLE_ICE = 43,               // ¿øÇü¾óÀ½ °ø°Ý
	ICE_STORM = 44,                // ¾óÀ½ÆøÇ³
	FREEZE_9TILE_RADIUS = 45,      // ¹Ý°æ 9Å¸ÀÏ ¾ó¸®±â	
	LIGHT = 46,                    // ºû
	ELECT_RESIST = 47,             // Àü°Ý¿¡ ´ëÇÑ ÀúÇ×
	LIGHTNING_BOLT = 48,           // º­¶ô
	CHAIN_LIGHTNING = 50,          // À¯µµ Àü°Ý
	CIRCLE_LIGHTNING = 51,         // ¿øÇü Àü°Ý 
	LIGHTNING = 52,                // Á÷¼± Àü°Ý
	TWINKLE_STAR = 53,             // ¼öÈ£¼º
	LIGHTNING_5TILE_RADIUS = 54,   // ¹Ý°æ 5Å¸ÀÏ Àü°Ý
	LOW_MONSTER_RECALL = 55,       // Àú±Þ ¸ó½ºÅÍ ¼ÒÈ¯
	MIDDLE_MONSTER_RECALL = 58,    // Áß±Þ ¸ó½ºÅÍ ¼ÒÈ¯
	SNOWSTORM	= 59,			   // ´«º¸¶óÀÇ ÆøÇ³
	ICE_BREATH2	= 60,			   // ¾ÆÀÌ½º ºê·¹½º2
	BLOOD_LIGHTNING = 61,		   // ºí·¯µå ¶óÀÌÆ®´×
	PHASING = 64,                  // Àå¼ÒÀÌµ¿
	MEMORIZING_LOCATION = 65,      // ÀÌµ¿Àå¼Ò±â¾ï
	TELEPORT = 66,                 // ¼ø°£ÀÌµ¿
	MULTI_PORT = 67,               // º¹¼öÂ÷¿øÀÌµ¿
	TOWN_PORTAL = 68,              // Â÷¿øÀÌµ¿
	MAGIC_DETECT = 75,             // ¸¶¹ýÅ½Áö
	REMOVE_CURSE = 78,             // ÀúÁÖÇØÁ¦
	PROTECT_RANGE_WEAPON = 79,     // Àå°Å¸® ¹«±â·ÎºÎÅÍ º¸È£
	PHEONIX = 80,				   // ºÒ»õ
	METEOR_STRIKE = 81,			   // ¿î¼® // 030509 kyo //030506ÀÌÀü 110
	LOST_SIGHT = 82,               // ½Ã·Â»ó½Ç   
	SPEED_UP = 83,                 // ¼ÓµµÁõ°¡
	GREAT_THUNDER_BOLT = 84,       // ´ëÀü°Ý
	GREAT_METEOR = 85,             // ´ë¿î¼® 
	DRAGON_SCALE = 86,             // µå·¡°ïÀÇ ºñ´Ã
	DEATH_FIRE = 87,               // µ¥¾²ÆÄÀÌ¾î
	SUMMONING_SKELETON = 88,       // µ¥¾²³ªÀÌÆ® ¼ÒÈ¯
	FLYING_SPARK = 89,             // ÇÃ¶óÀ× ½ºÆÄÅ©
	VAMPIRE_POISON = 90,           // ¹ìÆÄÀÌ¾îÀÇ µ¶
	VAMPIRE_DODGE = 91,            // ¹ìÆÄÀÌ¾î È¸ÇÇ
	SUMMONING_UNDEAD = 92,         // ¾ðµ¥µå ¼ÒÈ¯
	FEATHER_OF_CURSE = 93,         // ¸¶ºñÀÇ ±êÅÐ
	CURSE_OF_HELL = 94,            // ÇïÀÇ ÀúÁÖ
	RECALL_FOLLOW = 95,            // ºÎÇÏ ¼ÒÈ¯
	FIRE_EXPLOSION = 96,           // È­¿°Æø¹ß
	FIRE_STORM = 97,               // ÆÄÀÌ¾î½ºÅè
	SHADOW_OF_EVIL = 98,           // ¾ÇÀÇ ±×¸²ÀÚ
	FREEZING = 99,                 // ÁÖÀ§ ¾ó¸²
	BLITZ_SPEAR = 100,             // ¹ø°³Ã¢
	LIGHTNING_CLOUD = 101,         // Àü°Ý ±¸¸§
	FIRE_BREATH = 102,             // È­¿° ºê·¹¾²
	ICE_BREATH = 103,              // ¾óÀ½ ºê·¹¾²
	LIGHTNING_BREATH = 104,        // Àü°Ý ºê·¹¾²
	FLAME_PILLAR = 105,            // È­¿° ±âµÕ
	FLAME_POUR = 106,              // È­¿° ÆøÇ³ 
	OCTA_LIGHTNING = 107,          // ¹æ»ç Àü°Ý
	BLOOD_SPRINKLE = 108,          // ÇÇ»Ñ¸®±â
	GAZER	= 109,				   // °ÔÀÌÀú
	FLAME_ARROW = 110,             // È­¿°È­»ì 

	AREA_BAT_ATTACK = 111,         // Áö¿ª ¹ÚÁã°ø°Ý
	AREA_PARALYSIS = 112,          // Áö¿ª ¸¶ºñ
	AREA_POISON = 113,             // Áö¿ª Áßµ¶
	AREA_CONFUSION = 114,          // Áö¿ª È¥¶õ
	AREA_SLOW = 115,               // Áö¿ª ¼ÓµµÀúÇÏ

	RESTORE_AUTHORITY = 151,       // È¸º¹ÀÇ±Ç´É  
	PROTECT_DANGER = 152,          // À§ÇèÀ¸·ÎºÎÅÍ º¸È£
	BLESS = 153,                   // ½ÅÀÇ Ãàº¹
	RECOVER_POISON = 154,          // ÇØµ¶ÀÇ ±Ç´É
	CURE_DISEASE = 155,            // Áúº´Ä¡·áÀÇ ±Ç´É
	MIRACLE_RECOVER = 156,     	   // ¿ÏÄ¡ÀÇ ±âÀû
	PERFECT_PROTECT = 157,         // ¿ÏÀüÇÑ °á°è  
	GREAT_SHIELD = 158,            // º¸È£ÀÇ ±âÀû
	CANCEL_DIVINE = 159,           // ¹Ý½Å·Â ¹«È¿È­
	RECURRENCE_DIVINE = 160,       // È¸±ÍÀÇ ±Ç´É
	PRAY_POWER = 161,              // ±âµµÀÇ ¿ª»ç
	HOLY_LIGHT = 162,              // ¼º½º·¯¿î ºû  
	ANTHEM = 163,                  // ¼º°¡
	RELIEF_AUTHORITY = 164,        // ±¸¿øÀÇ ºû 
	HOLY_FIRE = 165,               // ¼º½º·¯¿î ºÒ
	GREAT_PRAY = 166,              // À§´ëÇÑ ±âµµ  
	APPRECIATE = 167,              // ¹Ù¸¥ ±ú´ÞÀ½
	INFLUENCE_DIVINE = 168,        // ½Å¿¡ÀÇ °¨È­
	VOICE_SOUL = 169,              // ¿µÈ¥ÀÇ ¼Ò¸®
	HOLY_RAGE = 170,               // ¼º½º·± ºÐ³ë
	TURN_UNDEAD = 171,             // ÅÏ ¾ðµ¥µå
	CURSE = 172,                   // ½ÅÀÇ ÀúÁÖ 
	AUTHORITY_RECOVER_SIGHT = 173, // ½Ã·ÂÈ¸º¹ÀÇ ±Ç´É	
	DIVINE_POWER = 174,            // µð¹ÙÀÎ ÆÄ¿ö
	AUTHORITY_LUCK = 175,          // Àý´ë Çà¿îÀÇ ±Ç´É	
	AUTHORITY_LOST_SIGHT = 177,    // ½Ã·Â»ó½ÇÀÇ ±Ç´É
	PRAY = 178,	                   // ±âµµ
	HOLY_CURE = 179,               // ½Å¼ºÇÑ Ä¡À¯
	AUTHORITY_SIMUNIAN = 181,      // ½Ã¹«´Ï¾ÈÀÇ ±Ç´É
	AUTHORITY_CHARISNUMEN = 182,   // Ä«¸®½º ´©¸àÀÇ ±Ç´É
	AUTHORITY_LETHY = 183,         // ·¹Æ¼ÀÇ ±Ç´É
	AUTHORITY_OF_ATHYAS	= 185,     // ¾Æ»þ½ºÀÇ ±Ç´É
	AUTHORITY_OF_WHOARENCHA	= 187, // È­·»Â÷ÀÇ ±Ç´É
	RAGE_STONE = 189,              // ´ëÁöÀÇ ºÐ³ë
	LIFE_DRAIN = 190,              // »ý¸í·Â Å»Ãë  
	RAGE_BLAST = 191,              // È­½ÅÀÇ ºÐ³ë
	RAGE_LIGHT_BALL = 192,         // ¼º·ÉÀÇ ºÐ³ë
	THROW_ARROW = 194,             // È­»ì
	THROW_KNIFE = 195,             // ºñµµ
	// ¹ø°³ ¼Ó¼º ÀüÅõ½ºÅ³
	LIGHTNING_BOOM = 215,          
	THUNDER_BLOW = 216,
	LIGHTNING_SHOCK = 217,
	THUNDER_STRIKE = 218,
	LIGHTNING_SHIELD = 219,
	LIGHTNING_EXTREME = 220,
	// ´ëÁö ¼Ó¼º ÀüÅõ½ºÅ³
	BLOOD_EARTH = 221,             
	GROUND_ATTACK = 222, 
	BLOOD_WILL = 223,
	STONE_ARMOR = 224,
	SIMUNIAN_BOSOM = 225,
	EARTH_EXTREME = 226,
	// È­¿° ¼Ó¼º ÀüÅõ½ºÅ³ 
	SWORD_N_ROSES = 227,
	MULTIPLE_FIRE = 228, 
	FLEET_MISSILE = 229,
	CONVERTING_ARMOR = 230, 
	BURNING_BLOODY= 231,
	FIRE_EXTREME = 232,
	// ¾îµÒ ¼Ó¼º ÀüÅõ½ºÅ³
	POISONING_NOVA = 233,
	CHERROY_SHADE = 234, 
	DARK_BLADE = 235, 
	TYBERN_GIFT = 236,
	ACUQUIRE_DARK = 237,
	DARK_EXTREME = 238,
	// ¾óÀ½ ¼Ó¼º ÀüÅõ½ºÅ³
	ICING_BLAST = 239,
	CRITICAL_HIDING = 240, 
	SHARK_MISSILE = 241, 
	HORN_OF_ICEBERG = 242, 
	ICE_SHIELD = 243,
	ICE_EXTREME = 244,
	// ¹Ù¶÷ ¼Ó¼º ÀüÅõ½ºÅ³
	CHARGING = 245,
	DOUBLE_ATTACK = 246, 
	WHILWIND = 247,
	TWISTER = 248, 
	GUST = 249, 
	WIND_EXTREME = 250,
	// ÀÌÆåÆ® ¾ÆÀÌÅÛ
	ITEM_FIRE_EFFECT = 610, // CSD-030916
	ITEM_ICE_EFFECT = 611, // CSD-030916
	ITEM_LIGHTNING_EFFECT = 612, // CSD-030916	
	// ÀÌº¥Æ®
	THROW_SNOW = 193,
	THROW_WATER_BALL = 661,
	THROW_BIG_GULP = 663,
	// ¾ÆÀÌÅÛ È¿°ú
	ITEM_EFFECT_FIRE = 605,
	ITEM_EFFECT_ICE = 606,
	ITEM_EFFECT_LIGHTNING = 607
};	//> CSD-031007

enum COMBAT_RESULT
{
	CSR_NORMAL,    // Á¤»óÀûÀÎ °ü°è
	CSR_PRIOR,     // ½ÂÆÐ°ü°è¿¡¼­ ½Â
	CSR_POSTERIOR, // ½ÂÆÐ°ü°è¿¡¼­ ÆÐ
	CSR_CONFLICT,  // ´ë¸³ÀûÀÎ °ü°è
	CSR_EQUAL      // µ¿µîÇÑ °ü°è
};

enum VEHICLE_KIND
{
	HORSE = 1,  // ¸»
	BULL = 2,   // È²¼Ò
	PHANTOM = 3 // ÆÒÅÒ
};

enum NPC_KIND
{	//< CSD-TW-030606
	DUMMY = 0,
	HUMAN = 1,
	GUARD = 2,
	ORC_DUMMY = 3,
	LIFELESS = 4,
	HOBBIT = 5,
	DWARF = 6,
	DRAGON = 7,
	ANIMAL = 8,
	NORM_MON = 9,
	SEALSTONE = 11,
	GUARDSTONE = 12,
	//CASTLEGATEWAY = 12
	//WARARMS = 12
};	//> CSD-TW-030606

enum ATTR_TYPE
{	//< CSD-031027
	AT_NORMAL = 0,
	AT_FIRE = 1,
	AT_ICE = 2,
	AT_LIGHTING = 3,
	AT_UNDEAD = 4,
	AT_PIERCING = 5,
	AT_ANTIMAGIC = 6,
	AT_BOSS = 7,
	AT_SUMMON = 8,
	AT_TAMING = 9,
	AT_SEMIBOSS = 10
};	//> CSD-031027

enum NPC_PARAMETER_TYPE
{	//< CSD-031106 : Client¿¡¼­ º¯È­µÈ NPC ÆÄ¶ó¸ÞÅÍ¸¦ Server·Î º¸³»ÁØ´Ù. 
	NPC_PARAMETER_TARGETID = 0,
	NPC_PARAMETER_TARGETNAME = 1,
	NPC_PARAMETER_BOSSID = 2,
	NPC_PARAMETER_HOSTID = 3,
	NPC_PARAMETER_TAME_CMD = 4,
	NPC_PARAMETER_PATTERNTYPE = 5,
	NPC_PARAMETER_OLDPATTERNTYPE = 6,
	NPC_PARAMETER_ATTRIBUTE = 7
};	//> CSD-031106

enum ARENA_TYPE
{
	AT_UNKNOWN = 0,
	AT_CORE = 1,
	AT_STONE = 2,
	AT_MONSTER = 3,
	AT_LMS = 4
};

enum ARENA_STATE
{
	CS_NOW_PEACE	= 0, // ÆòÈ­·Î¿î »óÅÂ(º¸Åë¸Ê°ú ´Ù¸¦°Ô ¾ø´Â »óÅÂ
	CS_NOW_REGISTER	= 1, // °¢°¢ ÆÀ¿¡ µî·ÏÇÏ´Â »óÅÂ. 
	CS_NOW_FIGHT	= 2  // ÀüÅõ ½ÃÀÛ »óÅÂ
};

enum ARENA_RESULT
{
	FST_WIN = 0,
	FST_DEFEAT = 1,
	FST_DISCONNECT = 2
};

enum ARENA_ITEM
{
	GT_RESOURCE = 5,
	GT_ARENA_LOW = 6,
	GT_ARENA_HIGH = 7
};
//> CSD-030517
///////////////////////////////////////////////////////////////////////////////

struct AREA
{	
	BOOL	  bAlive;
	short int Type;
	short int ID;
};

///////////////////////////////////////////////////////////////////////////////
//< CSD-030723
enum eBillingType
{	//-°ªÀ¸·Î ±¸¼º ÇØ¾ß ÇÕ´Ï´Ù. CheckPW_TotalDB_SQLÀÌ ³²Àº ³¯Â¥(¾ç¼ö)¸¦ ¸®ÅÏÇÏ±â ¶§¹®ÀÔ´Ï´Ù.
	BT_WRONG_ID					= -1,//ID Æ²¸²
	BT_WRONG_PW					= -2,//PW Æ²¸²

	BT_NEED_PAY					= -3,//µ· ºÎÁ·

	BT_COMMERCIAL_TIME_REMAIN	= -4,//³²Àº ½Ã°£ ÀÖ´Â³ð
	BT_COMMERCIAL_IP			= -20,//IP Á¾·®Á¦
	BT_FREE						= -100,//°øÂ¥
	BT_WAIT_BILLING_MSG			= -200,//´ë¸¸ °°Àº °÷ÀÇ Ã³¸® ¹æ½Ä ºô¸µÀ» ±â´Ù¸®µµ·Ï Ã³¸®
};	//DBDemon°ú Ç×»ó µ¿±âÈ­ ½ÃÅ°½Ê½Ã¿ä

enum eBankMoneyChangeType
{
	BMCT_ARENA_GAMBLE_DIVIDEND	=	1,
	BMCT_ARENA_GAMBLE_BETTING	=	2,
	BMCT_BANK_DEPOSIT			=	3,
	BMCT_BANK_DEFRAYAL			=	4,
	BMCT_AUCTION_ITEM_BUY		=	5,
	BMCT_AUCTION_ITEM_REGISTER	=	6,
	BMCT_BUY_LOTTO				=	7,
	BMCT_RESET_ABILITY			=	8,
	BMCT_CHAR_LOGIN				=	9,
	BMCT_RECV_MAP_CHANGE		=	10,
	BMCT_RECV_CHAR_INFO_BASIC	=	11,
	BMCT_SKILL_RESET			=	12,
};

enum eSendCharInfoType//021030 lsw 
{
	STR			=0,
	DEX			=1,
	CON			=2,
	WIS			=3,
	INT_		=4,
	CHA			=5,
	MOVP		=6,
	ENDU		=7,
	MOR			=8,
	WSPS		=9,
	LUCK		=10,
	HEALTH		=11,
	MOV			=12,
	AC			=13,
	FAME		=14,
	DAMAGE		=15,
	HIT			=16,
	TACTICS		=17,
	MONEY		=18,
	LVPOINT		=19,
	LEV			=20,
	EXP			=21,
	HP	  		=22,
	MAX_HP		=23,
	MP			=24,
	MAX_MP		=25,
	SP			=26,
	MAX_SP		=27,
	CP      	=28,
	MAX_CP  	=29,
	BP	    	=30,
	MAX_BP  	=31,
	R_FIR		=32,
	R_ICE		=33,
	R_ELE		=34,
	DAM_MIN		=35,
	DAM_MAX		=36,
	HIT_RAT		=37,
	DEF_RAT		=38,
	TAC_L1		=39,
	TAC_L2		=40,
	TAC_L3		=41,
	TAC_L4		=42,
	TAC_L5		=43,
	TAC_L6		=44,
	TAC_L7		=45,
	TAC_L8		=46,
	TAC_L9		=47,
	TAC_L10		=48,
	TAC_L11		=49,
	TAC_L12		=50,
	TAC_L13		=51,
	TAC_E1		=52,
	TAC_E2		=53,
	TAC_E3		=54,
	TAC_E4		=55,
	TAC_E5		=56,
	TAC_E6		=57,
	TAC_E7		=58,
	TAC_E8		=59,
	TAC_E9		=60,
	TAC_E10		=61,
	TAC_E11		=62,
	TAC_E12		=63,
	TAC_E13 	=64,
	BANKMONEY	=65,
	FAITH		=66,
	VIEWGHOST	=67,
	VIEWBOMB	=68,
	VIEWTRAP	=69,
};

enum eMkType//021030 lsw 
{
	MK_NORMAL      =0,	// ÀÏ¹ÝÀûÀÎ °æ¿ì
	MK_WARNING     =1,	// À§Çè, °æ°í
	MK_SHORTAGE    =2,	// ºÎÁ·
	MK_INFORMATION =3,	// Á¤º¸
};

enum eSpriteType
{
	SPRITETYPE_NONE			=0,
	SPRITETYPE_CHARACTER	=1,
	SPRITETYPE_NPC			=2,
};
//Skill No
enum eSkillNoIndex
{
	SKILL_UNKNOWN							=0,	// 030117 kyo 
	SKILL_DETECT_GHOST						=1,	
	SKILL_TALK_WITH_GHOST					=2,	
	SKILL_POISONING							=3,	
	SKILL_DETECT_POISON						=4,	
	SKILL_FIND_TRAPS						=5,	
	SKILL_REMOVE_TRAPS						=6,	
	SKILL_ENTRAPMENT						=7,	
	//	SKILL_MAGIC_SCROLLING					=8,	
	//	SKILL_ABILITY_SCROLLING					=9,	
	//	SKILL_EMERGENCY_ALRAM					=10,	
	//	SKILL_FIRST_AID							=11,	
	//	SKILL_FAST_FOOT							=12,	
	//	SKILL_SECRETHEAR						=13,	
	//	SKILL_WALLCLIMBING						=14,	
	//	SKILL_INTUITION							=15,	
	SKILL_DETECT_BOMB						=16,	
	SKILL_BOMB_IN_BOX						=17,	
	SKILL_REMOVE_BOMB						=18,	
	SKILL_STEALING							=19,	
	//	SKILL_LOCKPICKING						=20,	
	SKILL_RECLAIM							=21,	
	SKILL_SOW								=22,	
	//	SKILL_CULTIVATE							=23,	
	SKILL_HAVEST							=24,	
	//	SKILL_DETECT_MINERAL					=25,	
	SKILL_MINING							=26,	
	SKILL_REFINE							=27,	
	SKILL_FISH_HOOKING						=28,	
	//	SKILL_BAITMAKING						=29,	
	SKILL_NETTING							=30,	
	//	SKILL_SAILING							=31,	
	SKILL_CUTTING							=32,	
	SKILL_LUMBERING							=33,	
	SKILL_BUTCHERING						=34,	
	//	SKILL_DETECT_HERB						=35,	
	SKILL_DIGGING							=36,	
	SKILL_MAKEPOWDER						=37,	
	//	SKILL_HERDING							=38,	
	//	SKILL_MILKING							=39,	
	//	SKILL_MIDWIFERY							=40,	
	SKILL_SHEAR								=41,	
	SKILL_CARPENTRY							=42,	
	SKILL_BLACKSMITHY						=43,	
	SKILL_COOKING							=44,	
	SKILL_CLOTHING							=45,	
	//	SKILL_DYEING							=46,	
	SKILL_TAILORING							=47,	
	SKILL_BOWCRAFTING						=48,	
	SKILL_FLETCHING							=49,	
	SKILL_MAKEPOTION						=50,	
	SKILL_MAKEMETAL							=51,	
	SKILL_MAKEBOTTLE						=52,	
	SKILL_CANDLEMAKING						=53,	
	//	SKILL_WAXING							=54,	
	//	SKILL_REPAIRNG							=55,	
	//	SKILL_REUTILIZING						=56,	
	//	SKILL_PLAYING							=57,	
	//	SKILL_PEACEMAKING						=58,	
	//	SKILL_PARALYSIS							=59,	
	//	SKILL_HALLUCINATION						=60,	
	//	SKILL_PROVOCATION						=61,	
	//	SKILL_ENTICEMENT						=62,	
	//	SKILL_NOISE								=63,	
	//	SKILL_TRACKING							=64,	
	SKILL_SNOOPING							=65,	
	//	SKILL_SEARCH							=66,	
	SKILL_MERCHANT							=67,	
	SKILL_BLACKSMITHY_MASTER				=68,	//´ëÀåÀåÀÌ ±â¼º	
	SKILL_CARPENTRY_MASTER					=69,	//¸ñ¼ö ±â¼º	
	SKILL_BOWCRAFTING_MASTER				=70,	//È°Á¦ÀÛ»ç ±â¼º	
	SKILL_TAILORING_MASTER					=71,	//Àç´Ü»ç ±â¼º
	//	SKILL_HIDE								=72,	
	//	SKILL_CAMPING							=73,	
	SKILL_MAKEFIRE							=74,	
	SKILL_TAMING							=75,	
	//	SKILL_COMMUNICATE_WITH_ANIMAL			=76,	
	//	SKILL_COMMUNICATE_WITH_OTHER_TRIBE		=77,	
	//	SKILL_ANIMAL_LORE						=78,	
	//	SKILL_ARMS_LORE							=79,	
	SKILL_EVALUATE_INTELIGENCE				=80,	
	SKILL_EVALUATE_WISDOM					=81,	
	SKILL_ANATOMY_JUDGEMENT					=82,	
	SKILL_ITEM_IDENTIFICATION				=83,	
	//	SKILL_TASTE_IDENTIFICATION				=84,	
	//	SKILL_APPRAISAL							=85,	
	//	SKILL_HEALING							=86,	
	SKILL_TACTICS							=87,//021113 lsw
	TACTICS_Crapple							=88,
	TACTICS_swordmanship					=89,
	TACTICS_Archery							=90,
	TACTICS_Fencing							=91,	
	TACTICS_Mace_fighting					=92,
	TACTICS_Pierce							=93,
	TACTICS_Whirl							=94,
	TACTICS_Hurl							=95,
	TACTICS_Parrying						=96,
	TACTICS_Double_Swordmanship				=97,
	TACTICS_Double_MaceFighting				=98,	
	TACTICS_Magery							=99,
	TACTICS_Orison							=100,
	//	SKILL_THREAD							=101,	
	//	SKILL_COLORING							=102,	
	//	SKILL_JUMP								=103,	
	//	SKILL_RIDING							=104,	
	SKILL_GHOST_RESURRECTION				=105,	
	SKILL_GM_CARPENTER						=106,	
	SKILL_GM_BLACKSMITH						=107,	
	SKILL_GM_COOK							=108,	
	SKILL_GM_TAILOR							=109,	
	SKILL_GM_BOWCRAFT						=110,	
	SKILL_GM_ALCHEMIST						=111,	
	SKILL_GM_CANDLEMAKER					=112,	
};
// ¾ÆÀÌÅÛ kind
enum eItemKindType
{
	IK_NONE							=0,	//	non
	IK_GON							=1,	//	°ï			
	IK_PENCING						=2,	//	Ææ½Ì°Ë		
	IK_ONE_SWORD					=3,	//	ÇÑ¼Õ°Ë
	IK_ONE_AXE						=4,	//	ÇÑ¼Õµµ³¢
	IK_ONE_MACE						=5,	//	ÇÑ¼ÕÃ¶Åð
	IK_CLUB							=6,	//	¸ùµÕÀÌ
	IK_WHIRL						=7,	//	ºÀ
	IK_STAFE						=8,	//	ÁöÆÎÀÌ
	IK_SPEAR						=9,	//	º¸ÅëÃ¢
	IK_FORK							=10,	//	Æ÷Å©
	IK_HURL							=11,	//	ºñ°Ë,´øÁö´Â¹«±â
	IK_SHORT_BOW					=12,	//	¼ÒÇüÈ°
	IK_TWO_SWORD					=13,	//	¾ç¼Õ°Ë
	IK_TWO_AXE						=14,	//	¾ç¼Õµµ³¢
	IK_TWO_MACE						=15,	//	¾ç¼Õ Ã¶Åð
	IK_TWO_ARROW					=16,	//	È­»ì
	IK_IRON_ARMOR					=17,	//	¼è°©¿Ê
	IK_LEATHER_ARMOR				=18,	//	°¡Á×°©¿Ê
	IK_RING							=19,	//	¹ÝÁö
	IK_SHIELD						=20,	//	¹æÆÐ
	IK_SHOES						=21,	//	½Å¹ß
	IK_ROBE							=22,	//	·Îºê
	IK_FULL_HELMET					=23,	//	ÇÃÇï¸ä
	IK_HALF_HELMET					=24,	//	ÇÏÇÁÇï¸ä
	IK_NORMAL_CLOTHE				=25,	//	º¸Åë¿Ê
	IK_AMULET						=26,	//	¸ñ°Å¸®
	IK_ARMLET						=27,	//	ÆÈÂî
	IK_MUSIC						=28,	//	¾Ç±â
	IK_SCROLL						=29,	//	½ºÅ©·Ñ
	IK_LONG_BOW						=30,	//	´ëÇüÈ°
	IK_BOX							=31,	//	»óÀÚ
	IK_GRAIN						=32,	//	°î½Ä
	IK_POTION						=33,	//	Æ÷¼Ç
	IK_FOOD_MASS					=34,	//	µ¢¾î¸® À½½Ä
	IK_PLATE_FOOD					=35,	//	Àï¹Ý À½½Ä
	IK_DRINK						=36,	//	À½·á
	IK_ARROW_POINT					=37,	//	È­»ì ÃË
	IK_FEATHER						=38,	//	±êÅÐ		
	IK_SEED							=39,	//  ¾¾¾Ñ.
	IK_MONEY						=40,	//	µ·
	IK_MINERAL						=41,	//	±¤¼®
	IK_MINERAL_LUMP					=42,	//	°¡°øÇÑ ±¤¼®( Ã¶±«, ±Ý±«... )
	IK_HERB							=43,	//	¾àÃÊ					// 0626 YGI
	IK_HERB_POWDER					=44,	//	¾àÃÊ °¡·ç
	IK_WOOD_BEFOR_USE				=45,		//°¡°øÀüÀÇ ³ª¹«
	IK_WOOD_AFTER_USE				=46,		//°¡°øÈÄÀÇ ³ª¹«
	IK_TENT							=47,		//ÅÙÆ®·ù
	IK_TOOL_RECLAI					=48,		//µµ±¸ °³°£¿ë
	IK_TOOL_HAVEST					=49,		//µµ±¸ Ãß¼ö¿ë
	IK_TOOL_MINING					=50,		//µµ±¸ Ã¤±¤¿ë
	IK_TOOL_REFINE					=51,		//µµ±¸ Á¦·Ã¿ë
	IK_TOOL_FISH_HOOKING			=52,		//µµ±¸ ³¬½Ã¿ë
	IK_TOOL_CUTTING					=53,		//µµ±¸ ¹ú¸ñ¿ë
	IK_TOOL_LUMBERING				=54,		//µµ±¸ ¸ñÀç°¡°ø¿ë
	IK_TOOL_BUTCHERING				=55,		//µµ±¸ µµÃà¿ë
	IK_TOOL_DIGGING					=56,		//µµ±¸ ¾àÃÊÃ¤Áý¿ë
	IK_TOOL_MAKEPOWDER				=57,		//µµ±¸ ¾àÀç¸¸µé±â¿ë
	IK_TOOL_SHEAR					=58,		//µµ±¸ °¡À§·ù
	IK_TOOL_CARPENTRY				=59,		//µµ±¸ ¸ñ°ø¿ë
	IK_TOOL_BLACKSMITHY				=60,		//µµ±¸ Ã¶°ø¿ë
	IK_TOOL_COOKING_FIRE			=61,		//µµ±¸ ´ë¿ì´Â¿ä¸®¿ë
	IK_TOOL_COOKING_CUT				=62,		//µµ±¸ ½á´Â¿ä¸®¿ë
	IK_TOOL_PIN						=63,		//µµ±¸ ¹Ù´Ã·ù
	IK_TOOL_BOWCRAFTING				=64,		//µµ±¸ È°Á¦ÀÛ¿ë
	IK_TOOL_FIRE_BOAD				=65,		//µµ±¸ ºÒ´ë
	IK_TOOL_CANDLEMAKING			=66,		//µµ±¸ ¾çÃÊÁ¦ÀÛ¿ë
	IK_TOOL_OILING					=67,		//µµ±¸ ¹Ð³³°¡°ø¿ë
	IK_TOOL_TAMING					=68,		//µµ±¸ Å×ÀÌ¹Ö¿ë
	IK_TOOL_NETTING					=69,		//µµ±¸ ±×¹°·ù
	IK_TOOL_MAKEFIRE				=70,		//µµ±¸ ºÎ½Ëµ¹·ù
	IK_WORK_BOAD_BLAST_FURNACE 		=71,		//ÀÛ¾÷´ë ¿ë±¤·Î
	IK_WORK_BOAD_BAETLE				=72,		//ÀÛ¾÷´ë ¹èÆ²
	IK_WORK_BOAD_BLOCK				=73,		//ÀÛ¾÷´ë ¼±¹Ý
	IK_WORK_BOAD_ANVIL				=74,		//ÀÛ¾÷´ë ¸ð·ç
	IK_WORK_BOAD_BRAZIER			=75,		//ÀÛ¾÷´ë È­´ö
	IK_WORK_BOAD_TEST_TUBE			=76,		//ÀÛ¾÷´ë ½ÃÇè°ü·ù
	IK_WORK_BOAD_SPINNING_WHEEL		=77,		//ÀÛ¾÷´ë ¹°·¹
	IK_WORK_BOAD_FIRE_BOAD			=78,		//ÀÛ¾÷´ë ºÒ´ë
	IK_TOOL_BUCKET					=79,		//µµ±¸ ¾çµ¿ÀÌ
	IK_TOOL_RECYCLE					=80,		//µµ±¸ ÀçÈ°¿ë
	IK_TOOL_REPAIR					=81,		//µµ±¸ ¼ö¸®¿ë
	IK_IRON_SHIELD					=82,		//¼è¹æÆÐ	
	IK_IRON_SHOES					=83,		//¼è ½Å¹ß	
	IK_CAP							=84,		//¸ðÀÚ	
	IK_GOKAL						=85,		//µÎ°Ç	
	IK_PRIEST_ROBE					=86,		//¼ºÁ÷·Îºê	
	IK_CANDLE						=87,		//¾çÃÊ	
	IK_MAN_TO_MAN					=88,		//°áÅõ¼­	
	IK_CRACKER						=89,		// ÆøÁ×			// 010210 YGI
	IK_RAJA_EMBLEM					=90,		// ¶óÀÚ ¿¥ºí·½
	IK_POTION_CHANGE_COLOR_CLOTHES	=91,	// ¿Ê ¿°»ö¾à
	IK_POTION_CHANGE_COLOR_HAIR		=92,	// ÇÇºÎ ¿°»ö¾à
	IK_NEW_MONEY					=93,	// ½Å±Çµ·
	IK_LOCK_STEAL					=94,	// µµµÏ¹æÁö µ£
	IK_MAP							=95,	//	Áöµµ
	IK_GAUNTLET						=96,		//°ÇÆ²·¿
	IK_GOOD_FOOD					=97,		//°í±ÞÀ½½Ä
	IK_GOOD_POTION					=98,		//°í±ÞÆ÷¼Ç
	IK_GOOD_CANDLE					=99,		//°í±Þ¾çÃÊ
	IK_UNIQ_RESOURCE				=100,		//À¯´ÏÅ©Àç·á
	IK_GENERAL_GAMBLE				=101,		//ÀÏ¹Ý°×ºí¾ÆÀÌÅÛ
	IK_GRADE_GAMBLE					=102,		//±×·¹ÀÌµå°×ºí¾ÆÀÌÅÛ
	IK_CLOAK						=103,		//¸ÁÅä
	IK_CHAIR						=104,		//ÀÇÀÚ			// LTS SITDOWN
	IK_BOMB							=105,		//011212 lsw 
	IK_TRAP							=106,		//011212 lsw 
	IK_THROW_EVENT					=107,		// CSD-030723 : ÀÌº¥Æ®¿ë ´øÁö±â ¹°°Ç
	IK_POTION_BOX					=108,		// Æ÷¼Ç ÁÖ¸Ó´Ï		020418 YGI
	IK_FALG							=109,		// ±ê¹ß
	IK_SADDLE						=110,		// ¸»¾ÈÀå
	IK_DUAL_GAUNTLET				=111,	
	IK_DUAL_SWORD					=112,	
	IK_DUAL_BOW						=113,	
	IK_DUAL_FENCING					=114,	
	IK_DUAL_AXE						=115,	
	IK_DUAL_SPEAR					=116,	
	IK_DUAL_KNIFE					=117,	
	IK_DUAL_DOUBLE_SWORD			=118,	
	IK_DUAL_DOUBLE_AXE				=119,	
	IK_DUAL_MAGICIAN_STAFF			=120,	
	IK_DUAL_PRIEST_STAFF			=121,	
	IK_DUAL_SHIELD					=122,	
	IK_DUAL_ARMOR					=123,	
	IK_DUAL_ARCHER_LEATHER			=124,	
	IK_DUAL_MAGICIAN_ROBE			=125,	
	IK_DUAL_HELM					=126,	
	IK_DUAL_ARCHER_CAP				=127,	
	IK_DUAL_MAGICIAN_HAT			=128,	
	IK_DUAL_BOOTS					=129,	
	IK_DUAL_ARCHER_SHOES			=130,	
	IK_DUAL_MAGICIAN_SHOES			=131,	
	IK_DUAL_THIEF_SHOES				=132,	
	IK_DUAL_PRIEST_SHOES			=133,	
	IK_DUAL_THIEF_CAP				=134,	
	IK_DUAL_PRIEST_HAT				=135,	
	IK_DUAL_THIEF_LEATHER			=136,	
	IK_DUAL_PRIEST_ROBE				=137,	
	IK_DUAL_THIEF_SHIELD			=138,	
	IK_DUAL_ARROW					=139,	
	IK_ABYSS_MAZE					=140,	//¾Æºñ½º ¹Ì±Ã¿ë ¾ÆÀÌÅÛ	
	IK_UNDEAD_DUNGEON				=141,	//¾ðµ¥µå ´øÀü¿ë ¾ÆÀÌÅÛ	
	IK_GREAT_MAZE					=142,	//´ë¹Ì±Ã ¿ë ¾ÆÀÌÅÛ	
	IK_COMBATSKILL_BOOK				=143,
	IK_TREASURE_MAP					=145,	// º¸¹°Áöµµ		// 040105 YGI º¸¹°
	IK_POTION_BAG					=147,	// BBD 040213 ³»±¸µµ ÀÖ´Â Æ÷¼Ç
	IK_NOEXCHANGE					=148,   // Fintio 040707 prevent from exchanging item (Auction/Sell/Trade/Drop/Etc)
	IK_SEX_CHANGE					=151, //Eleval 12/06/09 - Sex Change
	IK_NATION_CHANGE				=157, //Eleval 12/06/09 - Application of Exile
	IK_MANTLEDYE					=159,   // Finito 25/06/08 changes colour of a mantle
	IK_SPECIALPOTION				=160,	// Rogério 06/05/08 for cp potion and pots whit duration
};

enum eFuntionItemIndex
{
	FITEM_MOVABLE			=1,	//¸¶ºñ,¼®È­,È¥¶õ ÇØÁ¦	¸¶ºñ°è¿­ÇØÁ¦	
	FITEM_RECOVERY			=2,	//¼ÓµµÀúÇÏ ÇØÁ¦	¼ÓµµÀúÇÏ ÇØÁ¦	
	FITEM_DETOX				=3,	//ÇØµ¶ÀÛ¿ë	ÇØµ¶	
	FITEM_ENLIGHT			=4,	//½Ã·Â È¸º¹	½Ã·Â»ó½Ç	
	//FITEM_NULL			=5,	NULL	NULL	
	//FITEM_NULL			=6,	NULL	NULL	
	FITEM_REVIVAL			=7,	//ÀÚµ¿ ºÎÈ°	ÀÚµ¿ ºÎÈ°	
	FITEM_COME_BACK			=8,	//¸¶À»·ÎÀÇ ±ÍÈ¯	ÀÚ½ÅÀÌ ¼ÓÇÑ ±¹°¡ÀÇ ¸¶À»·Î µ¹¾Æ¿È 	
	FITEM_RESIST_UP			=9,	//¸ðµç °è¿­ ÀúÇ×·Â »ó½Â	¸ðµç °è¿­ ¸¶¹ý ÀúÇ×·Â »ó½Â	
	FITEM_STEALTH			=10,	//Åõ¸í »óÅÂ À¯Áö	ÀÏÁ¤ ½Ã°£ Åõ¸í »óÅÂ À¯Áö	
	FITEM_INVALID_CURSE		=11,	//ÀúÁÖ °è¿­ ¸¶¹ý ¹«È¿È­	»ó´ëÀÇ ÀúÁÖ °è¿­ ¸¶¹ý ¹«È¿È­	
	FITEM_INVALID_OFFENCE	=12,	//°ø°Ý °è¿­ ¸¶¹ý ¹«È¿È­	»ó´ëÀÇ °ø°Ý °è¿­ ¸¶¹ý ¹«È¿È­	
	FITEM_POWER_UP			=13,	//¹°¸®Àû °ø°Ý·Â Áõ°¡	¹°¸®Àû °ø°Ý·Â Áõ°¡(%)	
	FITEM_SPEED_UP			=14,	//ÀÌµ¿ ¼Óµµ Áõ°¡	ÀÌµ¿ ¼Óµµ (ÀÌµ¿·Â) Áõ°¡	
	FITEM_HEAVY_ARMOR		=15,	//AC Áõ°¡	AC ¼öÄ¡ Áõ°¡	
	FITEM_SPELL_UP			=16,	//¸¶¹ý µ¥¹ÌÁö ÁõÆø	¸¶¹ý µ¥¹ÌÁö ÁõÆø(%)	
	FITEM_SPELL_TIME_UP		=17,	//¸¶¹ý Áö¼Ó ½Ã°£ ÁõÆø	¸¶¹ý Áö¼Ó ½Ã°£ ÁõÆø(msec)	
	FITEM_CRITICAL			=18,	//CRITICAL DAMAGE 	Å©¸®Æ¼ÄÃ µ¥¹ÌÁö·Î ¹°¸®Àû °ø°Ý	
	FITEM_STROKE			=19,	//¹°¸®Àû °ø°Ý ¼º°ø·ü 100%	¹°¸®Àû °ø°Ý ¼º°ø·ü 100%	
	FITEM_AGILITY			=20,	//¹°¸®Àû °ø°Ý È¸ÇÇÀ² 70%	¹°¸®Àû °ø°Ý È¸ÇÇÀ² 70%	
	FITEM_INVALID_PROTECT	=21,	//»ó´ëÀÇ º¸È£ °è¿­ ¸¶¹ý ¹«½Ã	»ó´ëÀÇ º¸È£ °è¿­ ¸¶¹ýÀ» ¹«Á¶°Ç ¹«½ÃÇÏ¿© °ø°Ý(¹°¸®/¸¶¹ý ¸ðµÎ)	
	FITEM_INVALID_RESIST	=22,	//»ó´ëÀÇ ¸¶¹ý ÀúÇ×·Â ¹«½Ã	»ó´ëÀÇ ¸¶¹ý ÀúÇ×·Â ¹«Á¶°Ç ¹«½Ã (¸¶¹ý ÀúÇ×·Â 0À¸·Î Àû¿ë)	
	FITEM_GHOST				=23,	//À¯·ÉÀ» º¼ ¼ö ÀÖÀ½	À¯·ÉÀ» º¼ ¼ö ÀÖÀ½	
	FITEM_SAMARITAN			=24,	//NKÀÇ °ø°ÝÀ» ¹«½Ã	NKÀÇ ¹°¸®Àû/¸¶¹ý °ø°Ý ¸ðµÎ ¹«½Ã	
	FITEM_SLIGHT			=25,	//¸¶¹ý Ä³½ºÆÃ ½Ã°£ ´ÜÃà	¸¶¹ý Ä³½ºÆÃ ½Ã°£ ´ÜÃà(%)	
};
//< CSD-030723
///////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
// ¸¶¹ý Å×ÀÌºí ±¸Á¶Ã¼
struct MAGICTABLE
{
	int	serial_Num;		    // °íÀ¯¹øÈ£ : ¸¶¹ý(1 ~ 200), ÀüÅõ½ºÅ³(215 ~ 250)
	int	order_Type;		    // °è¿­ : ¸¶¹ý»ç(1 ~ 12), ¼ºÁ÷ÀÚ(21 ~ 31), ÀüÅõ½ºÅ³(61 ~ 66)
	char Han_Name[30];
	int	appoint_Type;		  // ´ë»ó ÁöÁ¤¹æ½Ä Å¸ÀÔ(0->³ªÀÚ½Å/1->´ë»óÁöÁ¤(´ÜÀÏ´ë»ó)/2->´ë»óÁöÁ¤(º¹¼ö´ë»ó)/3->Áö¿ª¸¶¹ý)
	int	basic_Level;		  // »ç¿ë ±âº» ·¹º§(1~9)
	int	point_MS;		      // Magic S point 
	int before_Magic;		  // Àü ´Ü°è ¸¶¹ý(¸ÕÀú ÀÍÇô¾ß ÇÒ ¸¶¹ý)
	int	basic_magery ;		//
	int	cast_Time;		    // Ä³½ºÆÃ ½Ã°£(Àý´ë½Ã°£)
	int	cast_Continue;	  // Ä³½ºÆÃ Áö¼Ó ½Ã°£(Àý´ë½Ã°£)
	int	continue_Time;	  // Áö¼Ó½Ã°£(0->¿µ¿øÈ÷/1~Àý´ë½Ã°£)
	int	avail_Type;		    // È¿¿ë Å¸ÀÔ(0->ÀÚ½Å/1->ÀÚ½Å ¾Æ´Ñ 1¸í/2~->À¯È¿¹üÀ§³» ´Ù¼ö)
	int	avail_Range;	    // »çÁ¤°Å¸®(Å¸ÀÏ´ÜÀ§/0->ÀÚ½Å/99->¹«ÇÑ)
	int	exhaust_MP;		    // ¼Ò¸ð MP
	int	exhaust_MS;		    // ¼Ò¸ð MS
	int	require_IT;		    // ¿ä±¸ Áö´É
	int	require_WP;		    // ¿ä±¸ ÁöÇý
	int nHDPoint; // CSD-030314 : ¸¶¹ý µ¥¹ÌÁö ±âÁØ°ª
	int nHDLimit; // CSD-030314 : ¸¶¹ý µ¥¹ÌÁö ±âÁØ°ª Á¦ÇÑÄ¡(Àý´ë°ª)
	int	point_WS;		      // WS Æ÷ÀÎÆ®(0->not available/1~)
	int	point_Move;		    // ÀÌµ¿Æ÷ÀÎÆ®(0->not available/1~)
	int	invalid_TargetIT;	// ¸¶¹ý »ç¿ë°¡´É ¹üÀ§
	int nClass;           // ÀüÅõ½ºÅ³ »ç¿ë°¡´É Å¬·¡½º  
	int nTactics;         // ÀüÅõ½ºÅ³ »ç¿ë°¡´É ¹«±âÅÃÆ½
	int nAddition;        // ÀüÅõ½ºÅ³ »ç¿ë½Ã ¼Ò¸ðµÇ´Â ¼Ò¸ð·®
	int aLevel[4];        // ÀüÅõ½ºÅ³ ·¹º§
	int aRandom[4];       // ÀüÅõ½ºÅ³ ·¹º§ ÆíÂ÷
	int Spell_Type ;		  // 0 Wizard, 1~13 priest(Godº° typeµé ¶§¹®¿¡ ´Ù¾ç
	int	Appoint_Type ;
	int	Excute_Type;
	int	magic_Type;
	int	Price;
	int	Exp;
	int	Learnable;
	int	nSummonMax;   // CSD-030314 : ¼ÒÈ¯¸¶¹ý½Ã ¼ÒÈ¯°¡´É ÃÖ´ë ¸ó½ºÅÍÀÇ ¼ö
	int nCombatCount; // CSD-030314 : »ç¿ë°¡´ÉÇÑ ÀüÅõ½ºÅ³ ¼ö
	int nResist;
	int nRiseFall;
	int nDualStep;	// 030415 kyo
};
//
/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
// 020808 YGI(º¯¼öÀÇ À§Ä¡¿Í ±¸Á¶¸¦ ¹Ù²åÀ½)
class CEffectTbl
{ 
public:
	CEffectTbl()
	{
		effect_Num = 0;
		pattern_Num = 0;
		sprite_Num = 0;
		static_Effect = false;
		sound_Num = 0;
		sound_Volume = 0;
		sound_loop = 0;
		next_Effect = 0;
		link_Frame = 0;
		trans_Type = 0;
		trans_Level = 0;
		sort_Type = 0;
		spr_Speed = 0;
		spr_Height = 0;
		object_Num = 0;
		fallow = false;
		light = 0;
		end_Time = 0;
		impect_Effect_Num = 0;
		send_result = false;
	}
	
public:
	int Check() const 
	{
		int value;
		char *pValue = (char *)&value;
		pValue[0] = Check1()+Check3();
		pValue[1] = Check2()+Check4();
		pValue[2] = 0;
		pValue[3] = 0;
		return (short int)value;
	}
	
	char Check1() const
	{
		return (char)(effect_Num+end_Time);
	}
	char Check2() const
	{
		return (char)(sort_Type+next_Effect+sort_Type+object_Num);
	}
	char Check3() const
	{
		return (char)(spr_Speed + spr_Height + object_Num);
	}
	char Check4() const
	{
		return (char)(pattern_Num + sprite_Num + link_Frame);
	}
	
public:
	short int	end_Time;			//Á¾·á±îÁöÀÇ ÃÑ ÇÁ·¹ÀÓ¼ö
	short int	light;				//±¤¿øÈ¿°ú ³ÖÀ»±î?(0->³ÖÁö ¾ÊÀ½/100ÀÚ¸®¼öÄ¡->0ÀÌ¸é Á¤»óÀû Ãâ·Â/1ÀÌ¸é ³ôÀÌ°ª ´õÇØ¼­ Ãâ·Â(1~20 »çÀÌÀÇ ±¤¿ø ´Ü°è)
	short int	sound_Volume;		//ÀÌÆåÆ®¿Í ¿¬°áµÈ »ç¿îµåÀÇ º¼·ý·¹º§	
	char		static_Effect;		//½ºÇÁ¶óÀÌÆ®ÀÇ ½ºÅ¸Æ½ ¿©ºÎ
	short int	sprite_Num;			//½ºÇÁ¶óÀÌÆ®ÀÇ °íÀ¯ ¹øÈ£
	short int	pattern_Num;		//ÆÐÅÏ(ÇÔ¼ö)ÀÇ ¹øÈ£	
	char		send_result;		//È¿°ú°¡ ¼­¹ö¿¡ °á°ú¸¦ ¿äÃ»ÇÏ´ÂÁö ¿©ºÎ
	short int	spr_Height;			//ÀÌÆÑÆ®ÀÇ ³ôÀÌ
	short int	sound_Num;			//ÀÌÆåÆ®¿Í ¿¬°áµÉ »ç¿îµåÀÇ °íÀ¯¹øÈ£
	short int	spr_Speed;			//ÀÌÆÑÆ®ÀÇ ÀÌµ¿Ä¡
	short int	impect_Effect_Num;	//Å¸°Ý½Ã ¿¬°áµÇ´Â ÀÌÆÑÆ®ÀÇ ¹øÈ£
	short int	sort_Type;			//È­¸é sorting ¹æ¹ý(9(9+0)->ÀÏ¹ÝÀû ¼ÒÆÃ/10(9+1)->¹«Á¶°Ç ÃÖ»ó/11(9+2)->¹«Á¶°Ç ÃÖÇÏ/12(9+3)->¿¡´Ï¸ÞÀÌ¼Ç È­ÀÏ ÂüÁ¶ÇÏ¿© °¡º¯¼ÒÆÃ/13(9+4)->½ÃÀüÀÚ ¾ÕµÚ¿¡ 2ÀåÀÇ ½ºÇÁ¶óÀÌÆ® ÂïÀ½)
	short int	trans_Level;		//Æ®·»½º Á¤µµ
	short int	effect_Num;			//ÀÌÆÑÆ®ÀÇ °íÀ¯(ÀÏ·Ã) ¹øÈ£
	short int	object_Num;			//ÀÌÆÑÆ® ¼Ò½ºÀÇ µ¿½ÃÃâ·Â °¹¼ö
	short int	link_Frame;			//´ÙÀ½ ÀÌÆåÆ®°¡ ½ÃÀÛµÉ ÇÁ·¹ÀÓ
	char		fallow;				//ÀÎ¹°À» µû¶ó°¥Áö->1/¸»Áö->0
	short int	next_Effect;		//´ÙÀ½ ÀÌÆåÆ® ¹øÈ£
	short int	sound_loop;			//»ç¿îµå Ãâ·Â È½¼ö
	short int	trans_Type;			//Æ®·»½º ¹æ¹ý
};

class CEffectTbl2
{ 
public:
	int Check() const 
	{
		int value = 0;
		for( int i=0; i<30; i++ )
			value += sprite_Name[i];
		
		return value;
	}
public:
	char sprite_Name[30];
};
//
/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
//
class CMagicTbl
{
public:
	CMagicTbl()
	{
		order_Type = 0;
		serial_Num = 0;
		basic_Level = 0;
		WS = 0;
		before_Magic = 0;
		basic_Magery = 0;
		cast_Time = 0;
		cast_Continue = 0;
		continue_Time = 0;
		avail_Type = 0;
		avail_Range = 0;
		exhaust_MP = 0;
		exhaust_WS = 0;
		require_WP = 0;
		require_IT = 0;
		point_WS = 0;
		invalid_TargetIT = 0;
		spell_Type = 0;
		appoint_Type = 0;
		excute_Type = 0;
		magic_Type = 0;
		Price = 0;
		Exp = 0;
		Learnable = 0;
		nSummonMax = 0;
		nCombatCount = 0;
		nResist = 0;
		nRiseFall = 0;
		nDualStep = 0; // 030415 kyo

	}
	
public:
	int Check() const 
	{
		return serial_Num + basic_Level + cast_Time + cast_Continue + 
			continue_Time + avail_Range + excute_Type + appoint_Type +
			avail_Type + spell_Type + magic_Type + nCombatCount;
	}
	
public:
	int	order_Type;			     // °è¿­(1->»ý/2->»ç/3->ÀÏ¹Ý/4->È­¿°/5->¾óÀ½/6->Àü°Ý/7->¼ÒÈ¯/8->½Ã°ø/9->½Å¼º/10->Á¤µ¿/11->Á¤³ä/12->Æ¯¼ö)
	int	serial_Num;			     // °íÀ¯¹øÈ£(1~)
	int	basic_Level;		     // »ç¿ë ±âº» ·¹º§(1~9)
	int	WS;					         // wizwrd spell(or priest spell) point 
	int	before_Magic;		     // Àü ´Ü°è ¸¶¹ý(¸ÕÀú ÀÍÇô¾ß ÇÒ ¸¶¹ý)
	int	basic_Magery ;		   // ±âº» ¸¶¹ý·Â
	int	cast_Time;			     // Ä³½ºÆÃ ½Ã°£(Àý´ë½Ã°£)
	int	cast_Continue;		   // Ä³½ºÆÃ Áö¼Ó ½Ã°£(Àý´ë½Ã°£)
	int	continue_Time;		   // Áö¼Ó½Ã°£(0->¼ø°£,¿µ¿øÈ÷/1~Àý´ë½Ã°£)	
	int	avail_Type;			     // È¿¿ë Å¸ÀÔ(0->ÀÚ½Å/1->ÀÚ½Å ¾Æ´Ñ 1¸í/2~->À¯È¿¹üÀ§³» ´Ù¼ö)
	int	avail_Range;		     // »çÁ¤°Å¸®(Å¸ÀÏ´ÜÀ§/0->ÀÚ½Å/99->¹«ÇÑ)		
	int	exhaust_MP;			     // ¼Ò¸ð MP
	int	exhaust_WS;			     // ¼Ò¸ð WS
	int	require_WP;			     // ¿ä±¸ ÁöÇý					
	int	require_IT;			     // ¿ä±¸ Áö´É
	int	point_WS;			       // WS Æ÷ÀÎÆ®(0->not available/1~)
	int	invalid_TargetIT;	   // ¸¶¹ý »ç¿ë°¡´É ¹üÀ§
	int	spell_Type ;		     // 0 Wizard, 1~13 priest(Godº° typeµé ¶§¹®¿¡ ´Ù¾ç)
	int	appoint_Type;		     // ´ë»ó ÁöÁ¤¹æ½Ä Å¸ÀÔ(0->³ªÀÚ½Å/1->´ë»óÁöÁ¤(´ÜÀÏ/º¹¼ö´ë»ó)/2->Æ¯Á¤ÁöÁ¡(Å¬¸¯À§Ä¡)/3->Áö¿ª¸¶¹ý)
	int	excute_Type;		     // ¸¶¹ý Ä³½ºÆÃ À¯Áö½Ã ½ÃÇàµÉ¼ö ÀÖ´Â È½¼ö(0->¿©·¯¹ø/1->ÇÑ¹ø¸¸)
	int	magic_Type;			     // ¸¶¹ý ´ëºÐ·ù(0->ºñ°ø°Ý¸¶¹ý/1->´ë»ó°ø°Ý¸¶¹ý/2->Áö¿ªº¹¼ö°ø°Ý¸¶¹ý)
	int	Price;		  // ¸¶¹ýÀ» ¹è¿ì´Âµ¥ ÇÊ¿äÇÑ ºñ¿ë
	int	Exp;		  // ÀÏ¹Ý¸¶¹ýÀÇ °æ¿ì °æÇèÇÇ
	int	Learnable;	  // °¢ ½ÅºÀ½ÅÀÇ Å¸ÀÔ¿¡ µû¶ó ¹è¿ï¼ö ÀÖ´Â °æ¿ì
	int	nSummonMax;   // CSD-030314 : ¼ÒÈ¯¸¶¹ý½Ã ¼ÒÈ¯°¡´É ÃÖ´ë ¸ó½ºÅÍÀÇ ¼ö
	int nCombatCount; // CSD-030314 : »ç¿ë°¡´ÉÇÑ ÀüÅõ½ºÅ³ ¼ö
	int nResist;      // ¸·À» ¼ö ÀÖ´Â ÀúÇ×·Â
	int nRiseFall;    // ³ô³·ÀÌ Àû¿ë ¹üÀ§
	int nDualStep;	  //¸¶¹ýÀÇ µà¾ó ´Ü°è // 030415 kyo
};
//
/////////////////////////////////////////////////////////////////////////////////////////

struct NPCName_by_Gender
{
	char Female[17] ;
	char Male[17] ;
	char Last[17] ;
};

struct NPCLev_to_Exp
{	//< CSD-030430
	int nMinExp;         // ÇöÀç ·¹º§ÀÇ ÃÖ¼Ò °æÇèÄ¡
	int nMaxExp;         // ÇöÀç ·¹º§ÀÇ ÃÖ´ë °æÇèÄ¡
	int nNeedExp;        // ´ÙÀ½ ·¹º§·Î ¿Ã¶ó°¡±â À§ÇØ È¹µæÇØ¾ßµÉ °æÇèÄ¡
	int nTrainingExp;    // ÅÃÆ½ ¼ö·ÃÀå¿¡¼­ ¾ò´Â °æÇèÄ¡
	float fExpReduction; // »ç³É½Ã Á×¾úÀ» °æ¿ì °æÇèÄ¡ »è°¨·ü
	float fNKReduction;	 // NK¿¡ Á×¾úÀ» °æ¿ì °æÇèÄ¡ »è°¨·ü
	int nTacRate;		 // ÅÃÆ½ º¸Á¤Ä¡
	int nMaxTactic;	     // ·¹º§¿¡ µû¸¥ ÅÃÆ½ ¼ö·ÃÀå¿¡¼­ ¿Ã¸± ¼ö ÀÖ´Â ÃÖ´ë ÅÃÆ½ ·¹º§
	int	nDyeingPrice;    // ¿°»ö °¡°Ý
	int nLessRate;       // ³·Àº ·¹º§ º¸Á¤Ä¡  
	int nGreatRate;      // ³ôÀº ·¹º§ º¸Á¤Ä¡  
	int nCspMax;         // ÃÖ´ë ÀüÅõ½ºÅ³ Æ÷ÀÎÅÍ È¹µæ¼ö
	int nBaseCP;         // ÀüÅõ½ºÅ³ ±âº»·®
	int bank_loan;	     // 010314 YGI
	int nStep;           // °æÇèÄ¡ ´Ü°è
	int nWarriorLife;    // Àü»çÀÇ ±âº» Life
	int nThiefLife;      // µµµÏÀÇ ±âº» Life
	int nArcherLife;     // ±Ã¼öÀÇ ±âº» Life
	int nWizardLife;     // ¸¶¹ý»çÀÇ ±âº» Life
	int nPriestLife;     // ¼ºÁ÷ÀÚÀÇ ±âº» Life
	int nWarriorExpRate; // Àü»çÀÇ °æÇèÄ¡ º¸Á¤·ü(´ÜÀ§%)
	int nThiefExpRate;   // µµµÏÀÇ °æÇèÄ¡ º¸Á¤·ü(´ÜÀ§%)
	int nArcherExpRate;  // ±Ã¼öÀÇ °æÇèÄ¡ º¸Á¤·ü(´ÜÀ§%)
	int nWizardExpRate;  // ¸¶¹ý»çÀÇ °æÇèÄ¡ º¸Á¤·ü(´ÜÀ§%)
	int nPriestExpRate;  // ¼ºÁ÷ÀÚÀÇ °æÇèÄ¡ º¸Á¤·ü(´ÜÀ§%)
};	//> CSD-030430

struct NPC_Generation
{	//< CSD-030806
	int SprNO;
	//char Han_name[31];
	char Name[31];
	//int Selectable;
	int Sel_gender;
	//char Sel_FirstName[31];
	//char Sel_LastName[31] ;
	int nNpcRace;        // NPC Á¾Á·
	int nNpcNK;          // NPC¸¦ Á×¾úÀ» ¶§ NKÃ³¸® ¿©ºÎ
	int nStep;           // Å¬·¡½º ´Ü°è
	int nAttr;           // ¸ó½ºÅÍ ¼Ó¼º
	int nHpMax;          // ¸ó½ºÅÍ HP
	int nEpMax;          // ¸ó½ºÅÍ EP
	int nStrikeDamage;   // ¹°¸®Àû °ø°Ý µ¥¹ÌÁö
	int nStrikeRandom;   // ¹°¸®Àû °ø°Ý µ¥¹ÌÁö º¯È­·®
	int nMagicDamage;    // °ø°Ý ¸¶¹ý µ¥¹ÌÁö
	int nMagicRandom;    // °ø°Ý ¸¶¹ý µ¥¹ÌÁö º¯È­·®
	int nCriticalRate;   // Å©¸®Æ¼Ä® È®·ü
	int nCriticalDamage; // Å©¸®Æ¼Ä® µ¥¹ÌÁö
	int nPiercingStrike; // ¹°¸®°ø°Ý ÇÇ¾î½Ì È®·ü
	int nPiercingMagic;  // ¸¶¹ý°ø°Ý ÇÇ¾î½Ì È®·ü
	int Sel_Job_Alchemist;
	int nAIBase;         // ±âº»ÀûÀÎ Npc AI ÆÐÅÏ 
	int nAppearType;     // º¸½º¸ó½ºÅÍ µîÀå°ü·Ã Å¸ÀÔ
	int nAIType;         // Npc AI Á¦¾î À§Ä¡
	int nAINo;           // Npc AI ÆÐÅÏ ¹øÈ£
	int nLvMin;
	int nLvMax;
	int Money_min;						
	int Money_Max;						
	int AttackDelay;	// 0811 NPC KHS	
	int AccessDelay;	// 0811 NPC KHS	
	int BackDelay;		// 0811 NPC KHS	
	int PCfindDelay;	
	int nRvAp;	      // °ø°Ý·Â º¸Á¤(´ÜÀ§%)
	int nRvLife;	    // Life º¸Á¤(´ÜÀ§%)
	int nRvExp;       // °æÇèÄ¡ º¸Á¤(´ÜÀ§%) 
	int nAc;          // ÀÏ¹Ý ¹æ¾î·Â(´ÜÀ§%)
	int nDpRate;      // µà¾ó ¹æ¾î·Â º¸Á¤·ü(´ÜÀ§%)
	int nApRate;      // µà¾ó °ø°Ý·Â º¸Á¤·ü(´ÜÀ§%)
	int nPhysical;    // ¹°¸®Àû °ø°Ý¿¡ ´ëÇÑ ¹æ¾î·Â(´ÜÀ§%)
	int nFire;	      // ºÒ°è¿­ °ø°Ý ¸¶¹ý¿¡ ´ëÇÑ ÀúÇ×·Â(´ÜÀ§%)
	int nIce;		      // ¾óÀ½°è¿­ °ø°Ý ¸¶¹ý¿¡ ´ëÇÑ ÀúÇ×·Â(´ÜÀ§%)
	int nElect;	      // Àü°Ý°è¿­ °ø°Ý ¸¶¹ý¿¡ ´ëÇÑ ÀúÇâ·Â(´ÜÀ§%)
	int nHoly;	      // ½Å·Â°è¿­ °ø°Ý ¸¶¹ý¿¡ ´ëÇÑ ÀúÇ×·Â(´ÜÀ§%)
	int nStun;        // ¸¶ºñ°è¿­ ÀúÁÖ ¸¶¹ý¿¡ ´ëÇÑ ÀúÇ×·Â(´ÜÀ§%)
	int nSlow;        // ¼ÓµµÀúÇÏ°è¿­ ÀúÁÖ ¸¶¹ý¿¡ ´ëÇÑ ÀúÇ×·Â(´ÜÀ§%)
	int nPoison;      // Áßµ¶°è¿­ ÀúÁÖ ¸¶¹ý¿¡ ´ëÇÑ ÀúÇ×·Â(´ÜÀ§%)
	int nBlind;       // ½Ã·Â»ó½Ç°è¿­ ÀúÁÖ ¸¶¹ý¿¡ ´ëÇÑ ÀúÇ×·Â(´ÜÀ§%)
	int nDrain;       // ÈíÅ»°è¿­ ÀúÁÖ ¸¶¹ý¿¡ ´ëÇÑ ÀúÇ×·Â(´ÜÀ§%)
	int nFreeze;      // ³Ãµ¿°è¿­ ÀúÁÖ ¸¶¹ý¿¡ ´ëÇÑ ÀúÇ×·Â(´ÜÀ§%)
	//int SpiritValue;
	//int SpiritValue_Range;
	//int PS[15];
	//int Sel_Dice;
	//int QuantityperLv;
	//int WS[15] ;
	//int Sel_Dice1;
	//int QuantityperLv1;
	//int Leathal_weapon1;
	//int Leathal_weapon2;
	//int Damage_weapon;
	//int Leathal_attackws[9] ;
	//int Damage_WP;
	//int Leathal_WS;
	//int Success;
	//int Leathal_ps;
	//int DamagePercent_ps;
	int Inventory[15] ;
	//int Amount_Dice;
	//int Righthand;
	//int Lefthand;
	//int Body;
	//int Helmet;
	//int leg;
	//int Leg_Probability;
	//int neck;
	//int neck_Probability	;
	//int unique1;
	//int Unique1_probalbility;
	//int Unique2;
	//int unique2_probability	;
	//int Crapple;
	//int swordmanship;
	//int Archery;
	//int Fencing;
	//int Mace_fighting;
	//int Pierce;
	//int Whirl;
	//int Hurl;
	//int Parrying;
	//int Double_Swordmanship	;
	//int Double_MaceFighting	;
	//int Magery;
	//int Orison;
	//int LV_Str;
	//int Con;
	//int Dex;
	//int Wis;
	//int Int;
	int Movp;
	//int Char;
	//int Endu;
	//int Mor;
	//int Luck;
	//int WS_PS;
	//int Fullpoint;
	//int Suplus_Resist_Death;
	//int Suplus_Resist_Ston;
	//int Suplus_Resist_Magic;
	//int Suplus_Resist_Fire;
	//int Suplus_Resist_Ice;
	//int Suplus_Resist_Light;
	//int DivideLv;
	//int Resist_Range;
	int Tamingable;
	int Taming_min;
	int Taming_max;
	int Talkanimal_min;
	int Talkanimal_max;
	//int Resist;
	//int Resist_range1;
	int ValueHP;
	int ValueSP;
	//int Rely_value;
	//int nor_range;
	//int att_range;
	int offset[20];
	//int neglect_attack;
	//int neglect_avoid;
	//int check_pass;
	//int party;
	//int enemy_no;
	//int levelgap;
	//int apgap;
	//int vpgap;
	//int opendoor;
	//int lockdoor;
	//int closedoor;
	//int breakdoor;
	//int openobj;
	//int lockbj;
	//int closeobj;
	//int breakobj;
	//int act_month;
	//int act_time;
	//int act_bi;
	//int act_chun;
	//int act_nun ;
	int Get_meat;
	int Meat_amount;
	int Get_skin;
	int Skin_amount;
	int Get_Wool;
	int Wool_amount;
	int Get_beewax;
	int BeeWax_amount;
	int mutant;
	
	bool CanUse;
};	//> CSD-030806

///////////////////////////////////////////////////////////////////////////////
//
struct TimeData
{
	int Year;
	int Month;
	int Day;
	int DayofWeek;

	TimeData()
	{
		Year=0;
		Month=0;
		Day=0;
		DayofWeek=0;
	}
};
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//< CSD-030408 
struct REGEN_MONSTER_INFO
{
	int nMonsterNo;
	int nMonsterType;
	int nMonsterCount;
};

struct REGEN_PLACE_INFO
{
	int nNumber;
	int nPosX;
	int nPosY;
	int nScope;
};
//> CSD-030408
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//< CSD-030506
struct k_nation_item_data
{
	short int item_no;
	short int fame;
	int	money;
};
//> CSD-030506
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//< CSD-TW-030704
typedef struct tag_LottoUsers
{
	int		nLottoID;
	char	szID[20];
	TIMESTAMP_STRUCT	date;
	int		aNumbers[10];
}LOTTO_USER;

typedef struct tag_LottoEventInfo
{
	int		nLottoID;
	tm		StartDate;
	tm		LotteryDate;
	int		nWinNumCount;
	int		anWinNumbers[10]; // 10°³ Áö¸¸ ÇöÀç´Â 4°³¸¸ ¾²ÀÎ´Ù.
}LOTTO_EVENT_INFO;

typedef	struct	tag_DateCheck
{
	int		nDay;
	int		nHour;
	int		nMin;
}DATECHECK;
//> CSD-TW-030704
///////////////////////////////////////////////////////////////////////////////

struct START_UP_INFO
{	//< CSD-030723
	DWORD idIndex;
	int	  nApplyDay;  
	int   nStartHour;
	int   nStartMinute;
	int   nEndHour;
	int   nEndMinute;
};	//> CSD-030723	

struct CharacterParty
{	//< CSD-030723
	short int	On;
	char		Name[31];
	int			Face;
	int			Level;
	int			Gender;
	int			Str;
	int			Class;
};	//> CSD-030723

struct WINLOSEPOINT
{	//< CSD-030723
	DWORD win:13;
	DWORD defeat:12;
	DWORD disconnect:7;
};	//> CSD-030723

struct Event_Join							// 020115 LTS
{	//< CSD-030723
	unsigned char Event1 : 1;				// ÀÌº¥Æ® 1ÀÇ Âü¿©¿©ºÎ	
	unsigned char Event2 : 1;				// ÀÌº¥Æ® 2ÀÇ Âü¿©¿©ºÎ	
	unsigned char Event3 : 1;				// ÀÌº¥Æ® 3ÀÇ Âü¿©¿©ºÎ	
	unsigned char Event4 : 1;				// ÀÌº¥Æ® 4ÀÇ Âü¿©¿©ºÎ	
	unsigned char Event5 : 1;				// ÀÌº¥Æ® 5ÀÇ Âü¿©¿©ºÎ	
	unsigned char Event6 : 1;				// ÀÌº¥Æ® 6ÀÇ Âü¿©¿©ºÎ	
	unsigned char Event7 : 1;				// ÀÌº¥Æ® 7ÀÇ Âü¿©¿©ºÎ	
	unsigned char Event8 : 1;				// ÀÌº¥Æ® 8ÀÇ Âü¿©¿©ºÎ	
};	//> CSD-030723

struct DiseaseTable
{	//< CSD-030723							
	short int	disease_number;
	char		han_name[31];	
	char		name[31];	
	short int	lv;			
	short int	nut1_1;		
	short int	nut2_1;			
	short int	nut3_1;		
	short int	probability_1;
	short int	andor_1;	
	short int	checktime_1;
	short int	contact_char;
	short int	talk_char;	
	short int	fuck_char;	
	short int	contact_obj;
	short int	eat_obj;	
	short int	nut1_2;		
	short int	nut2_2;	
	short int	nut3_2;
	short int	probability_2;
	short int	checktime_2;
	short int	andor_2;
	short int	dec_str;
	short int	dec_dex;
	short int	dec_con;
	short int	dec_wis;
	short int	dec_int;
	short int	dec_movp;
	short int	dec_char;
	short int	dec_endu;
	short int	dec_mor;
	short int	dec_luck;
	short int	dec_ws;
	short int	dec_ps;
	short int	recoverable;
	short int	life	;
	short int	health	;
	short int	mp	;
	short int	dp	;
	short int	max_	;
	short int	damage	;
	short int	att_rate	;
	short int	depen_rate	;
	short int	ac	;
	short int	mad	;
	short int	poison	;
	short int	stony	;
	short int	sluggish	;
	short int	active	;
	short int	paralysis	;
	short int	active_time	;
	short int	maintain_time	;
	short int	natural_recovery	;
	short int	nut1_3	;
	short int	nut2_3	;
	short int	nut3_3	;
	short int	probability_3	;
	short int	checktime_3	;
	short int	andor_3	;
	short int	price_heal	;
	short int	price_prevent	;	
};	//> CSD-030723

struct GodTable
{	//< CSD-030723
	char meet_year_month;
	char meet_year_day;
	char meet_year_hour;
	char meet_month_day;
	char meet_month_hour;
	char meet_day_hour;
};	//> CSD-030723

struct NW_Character	// 010904 LTS		// LTS CHAR		
{	//< CSD-030723
	DWORD WarNo						:	7;	//	ÀüÀï¹øÈ£
	DWORD WarfieldNo					:	3;	//	ÀüÀïÅÍ ¹øÈ£
	DWORD SquadNo						:	3;	//	ºÎ´ë¹øÈ£
	DWORD isSquadLeader				:	1;	//	ºÎ´ëÀåÀÎ°¡?	
	DWORD isCommander					:	1;	//	»ç·É°üÀÎ°¡?
	DWORD isCommanderCandidater		:	1;	//  »ç·É°ü ÈÄº¸ÀÎ°¡?		// 010915 LTS
	DWORD DoWarJoinVote				:	1;	//	ÀüÀïÅÍ Âü¿© ÅõÇ¥¸¦ Çß³ª?
	DWORD DoCommanderVote				:	1;	//	»ç·É°ü ÅõÇ¥¸¦ Çß´Ù
	DWORD YL_JoinNation				:	4;	//	ÀÏ½ºÀÎ°æ¿ì ÀÚÀÌÆÝ¿¡Çß³ª? ¹ÙÀÌ¼­½º¿¡ Çß³ª?  // ±¹°¡¹øÈ£
	DWORD Reserved					:	10;	//	¿¹¾à..
};	//> CSD-030723

struct CCharRank
{	//< CSD-030723
	DWORD nation	  :3;  // ¼ÓÇÑ³ª¶ó - 0:¾øÀ½, 1, 2, 3:¹ÙÀÌ¼­½º, 4:ÀÚÀÌÆÝ, 5:Çì°Ô¸ð´Ï¾Æ, 6:ÀÏ½º
	DWORD guild_master:3;  // ±æµå ¸¶½ºÅÍ
	DWORD king		  :1;  // ³ª¶ó ¿Õ
	DWORD dwReserved  :9;  // »ç¿ëµÇÁö ¾ÊÀ½
	DWORD counselor	  :2;  // 0 : ¾øÀ½, 1: È¸»ç ¿î¿µÀÚ, 2: °ÔÀÓ³» ¿î¿µÀÚ
	DWORD ultra_nk3	  :1;  // ±Ø¾ÇÇÇÄÉÀÌ	1004 YGI
	DWORD ultra_nk4	  :1;  // ±Ø¾ÇÇÇÄÉÀÌ	1004 YGI
	DWORD ultra_nk6	  :1;  // ±Ø¾ÇÇÇÄÉÀÌ	1004 YGI
	DWORD reporter	  :1;
	DWORD imsi		  :10; // 1004 YGI
	DWORD santa_power	:2; //Eleval 22/06/09 - Then GMs can Remove Santa Power
};	//> CSD-030723

struct MAP_MOVE_INFO
{	//< CSD-HK-030829
	int idUser;
	int nMapPort;
	int nX;
	int nY;
};	//> CSD-HK-030829

#define LOG_ABNORMAL_PROTOCOL			0
#define LOG_DISCONNECT_LOGIN_SERVER		1
#define LOG_DISCONNECT_DB_DEMON			2
#define LOG_HANDLE_WRITING				3
#define LOG_EXCEPTION_FAIL				4
#define LOG_HANDLE_COMMAND				5
#define LOG_HANDLE_READING				6
#define LOG_INSERT_LOGINTABLE_ERROR		7
#define LOG_SEND_UPDATING_MESSAGE		8
#define LOG_EXCEPTION_ITEM_ABRASION_ERROR    9
#define LOG_HANDLE_WRITING_INSIDE		10

extern DWORD global_time;
extern DWORD g_alive_time;
extern DWORD g_curr_time;
extern DWORD g_curr_time_with_out_year; // 031009 CI YGI


extern int g_year;
extern int g_mon;
extern int g_yday;
extern int g_wday;
extern int g_day;
extern int g_hour;
extern int g_min;
extern int g_sec;

extern int g_count_ok;
extern int g_weatherflag;

extern bool g_block_attack;
extern bool g_block_magic;
extern bool g_block_steal;//030102 lsw
extern bool g_stop_please_give_life;
// ¸ÖÆ¼½º·¹Æ®¸¦ À§ÇÑ Setting.
extern HANDLE g_hSema;

#include "../LowerLayers/MyLog.h"
#include "CharInfo.h" // CSD-030723
#include "MagicState.h" // CSD-030723
#include "ResistState.h" // CSD-030723
#include "CharList.h" // CSD-030723
#include "nation.h"
#include "ItemMgr.h"
#include "dr_network.h"
#include "MYSQL.H"
#include "ITEMLIST.H"
#include "Hong_Sub.h"
#include "OP_Chr_Status.h"
#include "NPCLIST.H"
#include "Object.h"
#include "NPC_Pattern.h"
#include "AREA.H"
#include "SkillMgr.h" //020214 lsw skill.h »èÁ¦
#include "CrackMgr.h" //020214 lsw skill.h »èÁ¦
#include "Map.h"
#include "RajaSystem.h"
#include <mmsystem.h> //020214 lsw
#include "RareEffectMgr.h"
#include "menuserver.h"
#include "Nationsys_define.h"
#include "LocalizingMgr.h"
#include "LottoSystem.h" // CSD-TW-030704

extern CRareEffectMgr RareEM;
extern void ResetAbility(CHARLIST *ch);//020303 lsw
extern int MAX_EXP_LEVEL;  // CSD-040204
extern int MAX_TAC_LEVEL;  // CSD-040204
