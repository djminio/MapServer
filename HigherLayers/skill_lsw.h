#include "..\stdafx.h"
#include "Dr_NetWork.h"
#include <mmsystem.h>

#pragma once

#define MAX_UPGRADE_RARE		20//시도할때 보내는 아이템 포지션 갯수

#define MAX_RARE_GROUP			18	//레어그룹 최대치
#define MAX_RARE_PROBABILIY		110	//레어 프로버빌리티 최대치

#define TRAP_DELETE_TIME		600	//덫 지워지는 시간

#define DIVIDE_ITEM_UPGRDEABLE_MIN	2000	//업그레이드 할때 필요 갯수
#define DIVIDE_ITEM_UPGRDEABLE_MAX	5000	//업그레이드 할때 최대 갯수

extern int LoadRareProbability();
extern int LoadRaregroup();
extern int LoadItemFunctionGroup();
extern int LoadItemFunction();
extern int LoadLearnItemConvetrer();

extern int DeleteMakeItemResource( LPCHARLIST ch ,int item_no);

typedef struct RareGroupSet
{
	char rare[40]	;
	int rare_num	;
	int rare_rate	;
}t_rareGroupSet,*LPRAREGROUPSET;

typedef struct rare_group
{
	int no;
	char name[40];
	
	t_rareGroupSet rareset[8];

	char group_buffer_1[40]	;
	char group_buffer_2[40]	;
	char group_buffer_3[40]	;
	int	 group_buffer_4		;
	int	 group_buffer_5		;
	int	 group_buffer_6		;
	int	 group_buffer_7		;
	int	 group_buffer_8		;
	int	 group_buffer_9		;
	int	 group_buffer_10	;
}t_raregroup,*LPRARE_GROUP;

typedef struct rare_probability
{
	int num				;
	int exp_rare_suc[4] ;
	int max_rare_day	;
	int bonus_suc		;
	int max_suc			;
	int buffer_1		;
	int buffer_2		;
	int buffer_3		;
	int buffer_4		;
	int buffer_5		;
	int buffer_6		;
	int buffer_7		;
	int buffer_8		;
	int buffer_9		;
	int buffer_10		;
}t_rareprobability,*LPRAREPROBABILITY;
////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct item_function
{
	int		iNo;
	char	Name[40];
	char	NameAdd[40];
	char	Exp[50];//추가 설명
	int		ExpMark;
	int		iEffectNo;
	int		iUpgradeAble;
	int		iScrollNo;
	int		iMakeRand[3];
	int		iGrade[17];
	int		iTime[17];
	int		iBuffer1;
	int		iBuffer2;
}t_ItemFunction,*LPITEMFUNCTION;
#define MAX_ITEM_FUNCTION	100
extern t_ItemFunction ItemFunction[MAX_ITEM_FUNCTION];
////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct ITEMMUTANTKIND
{
	int		item_kind;
	char	exp_kind[100];
	int		upgrade_type;
	int		AddItem[4];
	int		iBuffer1;
	int		iBuffer2;
	int		iBuffer3;
	int		iBuffer4;
}t_ItemMutantKind,*LPITEM_MUTANT_KIND;
const int MAX_ITEM_MUTANT_KIND = 201;
extern t_ItemMutantKind ItemMutantKind[MAX_ITEM_MUTANT_KIND];
////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct ITEMGRADE
{
	int iGrade;
	int iAble;
	int ResourceAmount[4];
	int iBuffer1;
	int iBuffer2;
	int iBuffer3;
	int iBuffer4;
}t_ItemGrade,*LPITEM_GRADE;
const int MAX_ITEM_GRADE = 18;//MAX_ITEM_GRADE를 avail으로 체크한 값으로 받아 올수 있도록 코딩 할것//item_grade테이블에서 쓰는것
extern t_ItemGrade ItemGrade[MAX_ITEM_GRADE];
////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct ITEMRARE
{
	int		iNo		;
	int		iType	;
	char	EngName[100];
	char	LocalName[100];
	char	Explain[100];
	char	ExplainAdd[100];
	int		iExpMark;// + - 기호 표시
	int		iMakeAble;
	int		RaisePro[8];// 0번은 DefaultRaisePro
	int		iUpgradeAble;
	int		iAbleNation;//
	int		GRADE[16];//0번은 MAX 나머지는 그레이드별 값
	int		Buffer1;
	int		Buffer2;
	int		Buffer3;
	int		Buffer4;
	int		Buffer5;
	int		Buffer6;
}t_ItemRare,*LPITEMRARE;
const int MAX_ITEM_RARE		= 200;//020725 lsw
const int MAX_HIGH_RARE		= MAX_ITEM_RARE;//020725 lsw
extern t_ItemRare ItemRare[MAX_ITEM_RARE];
////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CMD_ADD_EFFECT	9115
typedef struct ADD_EFFECT
{
	WORD wEffectNo;	// 사용된 마법의 번호
	WORD idTarget;	// 마법을 사용한 대상자
	WORD wDuration;	// 유지시간
}t_AddEffect,*LPADDEFFECT;

extern void Send_RareItemMakeLog(	const int cn,					const int itemno,
									const int today_count=0,		const int grade=0,
									const int mutanttype1=0,		const int mutanttype2=0,
									const int mutanttype3=0,		const int addeditem1=0,
									const int addeditem1limit=0,	const int addeditem2=0,
									const int addeditem2limit=0,	const int resultlimit=0,
									const int resultnowdur=0,		const int resultmaxdur=0,
									const int why=0,				const int why2=0,
									const int resource1limit=0,		const int resource2limit=0,
									const int resource3limit=0,		const int resource4limit=0,
									const int resource5limit=0,		const int resource6limit=0);



const int MAX_SKILL_KIND	=45;
enum eMotherSkillIndex
{
	SPRITUALITY				=0,
	POISONING				=1,
	ENTRAPMENT				=2,
	SCROLLING				=3,
	CUNNING					=4,
	BOMBPLAY				=5,
	STEALING				=6,
	FARMING					=7,
	MINING					=8,
	FISHING					=9,
	CUTDOWN					=10,
	BUTCHERING				=11,
	HERBDIGGING				=12,
	HERDING					=13,
	CARPENTRY				=14,
	BLACKSMITHY				=15,
	COOKING					=16,
	TAILORING				=17,
	BOWCRAFTING				=18,
	ALCHEMING				=19,
	CANDLEMAKING			=20,
	REPARING				=21,
	REUTILIZING				=22,
	MUSICIANSHIP			=23,
	DETECT					=24,
	MERCHANT				=25,//상인데이타 저장됨
	CAMPING					=26,
	TAIMING					=27,
	SKILL_MASTER			=28,//기성
	ANIMAL_LORE				=29,
	ARMS_LORE				=30,
	EVALUATE_INTELIGENCE	=31,
	EVALUATE_WISDOM			=32,
	ANATOMY_JUDGEMENT		=33,
	ITEM_IDENTIFICATION		=34,
	TASTE_IDENTIFICATION	=35,
	APPRAISAL				=36, // 감정( 보석류 )
	HEALING					=37,
	JUMP					=38,
	RIDING					=39
};

enum eGmQuestIndex
{
	GM_QUEST_FARMER				=1,		//	농부 
	GM_QUEST_MINDER				=2,		//	광부         
	GM_QUEST_FISHER				=3,		//어부         
	GM_QUEST_CUTTER				=4,		//나무꾼       
	GM_QUEST_BUTCHER			=5,		//도살자       
	GM_QUEST_DIGGER				=6,		//심마니       
	GM_QUEST_CARPENTER			=7,		//목수         
	GM_QUEST_BLACKSMITH			=8,		//대장장이     
	GM_QUEST_COOKER				=9,		//요리사       
	GM_QUEST_TAILOR				=10,	//재단사       
	GM_QUEST_BOWCRAFT			=11,	//활제작사     
	GM_QUEST_ALCHEME			=12,	//연금술사     
	GM_QUEST_CANDLEMAKER		=13,	//초쟁이       
	GM_QUEST_MERCHANT			=14,	//상인         
	GM_QUEST_BLACKSMITH_MASTER	=15,	//대장장이 기성
	GM_QUEST_CARPENTER_MASTER	=16,	//목수 기성    
	GM_QUEST_BOWCRAFT_MASTER	=17,	//활제작사 기성
	GM_QUEST_TAILOR_MASTER		=18,	//재단사 기성
};

const int  MAX_GM_NEED_ITEM_COUNT	=3;
typedef struct tag_gm_quest		// 그랜드 마스터에 등록하기 위한 조건등...
{//021030 lsw
	char	name[20];		
	int		skill;
	int		job_level;		// 1차 2차 직업
	int		need_money;		// 필요 돈
	
	int		need_item[MAX_GM_NEED_ITEM_COUNT];		// 필요한 아이템 No
	int		need_quantity[MAX_GM_NEED_ITEM_COUNT];	// 필요 수량
	
	int		need_questnum1;	// 우선 처리되어야할 퀘스트
	int		need_questnum2; 
}GM_QUEST;
const int  MAX_GM_MAIN	=20;
extern GM_QUEST	g_GmMain[MAX_GM_MAIN];

extern int AddMoney( const DWORD money, CHARLIST *ch);	// 010210 YGI

