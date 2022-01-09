// ItemMgr.h: interface for the CItemMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMMGR_H__CA091E53_778D_4AE5_85BC_B3E5D27503CF__INCLUDED_)
#define AFX_ITEMMGR_H__CA091E53_778D_4AE5_85BC_B3E5D27503CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum eGenderType
{
	FEMALE	=0,
	MALE	=1,
};

enum eSpellType
{
	WIZARD_SPELL   = 0,
	PRIEST_SPELL   = 1,
	ACTIVE_COMBAT  = 3,
	PASSIVE_COMBAT = 4,
	RECOVERY_COMBAT= 5,// 030415 kyo
};

const int  MAX_MONEY		= 1000000;
const int  MAX_DIVIDE_ITEM	= 1000000;

enum eDefinedItemNo
{
	IN_COIN				= 10029,
	IN_COINS			= 10030,

	IN_NEW_COIN			= 10122,
	IN_NEW_COINS		= 10123,

	IN_RED_SIGNET		= 10058,
	IN_BLUE_SIGNET		= 10059,
	IN_STEAL_RESULT		= 10180,//백장미
};


//give rare item 에서 쓰는거
#define RARE_ABLE					0x0001	//구현 되어 있나? 
#define RARE_ABLE_SQUAD				0x0002	//부대레어
#define RARE_ABLE_GAMBLE_ROOM_4		0x0004	//겜블 4번
#define RARE_ABLE_GAMBLE_ROOM_5		0x0008	//겜블 5번
#define RARE_ABLE_OTHER				0x0010	//

////////////////////////////////////////////////////////////////////////////////////////
//				CharacterItemMgr 에서 사용
////////////////////////////////////////////////////////////////////////////////////////
const int ITEM_EQUIP_MAX	=8;
const int ITEM_QUICK_MAX	=6;
const int ITEM_INV_MAX		=72;	//3*3*8
const int ITEM_BANK_MAX		=90;	//5*3*6
const int ITEM_HAND_MAX		=1;
const int ITEM_BUFFER_MAX	=1;

enum eHighLv
{
	H_LV_NO_HIGH_ITEM = 0,
	H_LV_HIGH_ITEM = 1,
	H_LV_LEGEND_ITEM = 2,
};

///////////////////////////////////////////////////////////////////////////
//			ITEMATTR 관련 구조체
///////////////////////////////////////////////////////////////////////////
typedef struct ITEMATTR//011130 lsw
{
	short int		item_no;
	unsigned int	attr[6];//이거 unsigned 뺄것 .. 나중에
} ItemAttr, *lpItemAttr;

const int SIZE_OF_INV			=(ITEM_INV_MAX		* sizeof( ITEMATTR ));
const int SIZE_OF_EQUIP			=(ITEM_EQUIP_MAX	* sizeof( ITEMATTR ));
const int SIZE_OF_QUICK			=(ITEM_QUICK_MAX	* sizeof( ITEMATTR ));
const int SIZE_OF_BANKITEM		=(ITEM_BANK_MAX		* sizeof( ITEMATTR ));
const int SIZE_OF_HAND			=(ITEM_HAND_MAX		* sizeof( ITEMATTR ));
const int SIZE_OF_BUFFER		=(ITEM_BUFFER_MAX	* sizeof( ITEMATTR )); 

#define	BASIC_ANI_MAX			4//아이템에 의해서 보이는 애니메이션 갯수 WT_ARMOR, WT_HELMET, WT_WEAPON, WT_SHIELD 4가지
#define	EXTENSION_ANI_MAX		3//아이템에 의해서 보이는 애니메이션 확장팩 ㅡㅡ;; 망또와 찡..

typedef struct ITEMRAREMAIN
{
	DWORD	grade			: 4;//등급
	DWORD	soksung1		: 8;//속성1
	DWORD	soksung2		: 8;//속성2 멀티 속성
	DWORD	soksung3		: 8;//시간등급
	DWORD	iHighLevel		: 3;//플러스 타입 값이 있다면 IsDynamicRare-->는 반드시 0 이어야 한다
	DWORD	IsDynamicRare	: 1;//1 이면 기능 아이템 0 이면 레어 아이템
}RareMain, *LPRareMain;

typedef struct t_POS
{
	char type ; 
	short int p1, p2, p3 ;
} POS ;

typedef struct tag_K_ITEM
{
	ITEMATTR	item_attr;
	POS			item_pos;
}K_ITEM, *lpITEM;
// 040105 YGI 보물
///////////////////////////////////////////////////////
struct CTreasureMapItem
{
	DWORD mapnumber	:7;
	DWORD x			:10;
	DWORD y			:10;
//	DWORD use		:1;	// 사용했는가?
	DWORD isChecked	:1;	// 생성한 후에 좌표를 얻어 왔는가?
	DWORD offset	:2;
	DWORD offset2	:2;
};

extern CTreasureMapItem GetTreasureAttr( ItemAttr *item );
///////////////////////////////////////////////////////////////////////////
//		스킬 관련
///////////////////////////////////////////////////////////////////////////
const int	MAX_SKILL_EXP	=	45;
typedef struct SkillExp
{
	DWORD	skillexp:	20 ;//스킬 경험치
	DWORD	rare	:	12 ;//레어 성공률
}t_skillexp, *LPSKILLEXP;

typedef struct SkillExp2
{
	DWORD	skillexp	:	20	;//스킬 경험치
	DWORD	makecount	:	6	;//만든 갯수
	DWORD	day 		:	3	;//저장된 날짜
	DWORD	month 		:	3	;//저장된 날짜
}t_skillexp2, *LPSKILLEXP2;

typedef struct SkillExp3//021030 lsw
{
	DWORD	skillType	:	8	;//스킬타입
	DWORD	year		:	12	;//년도
	DWORD	day 		:	12	;//날짜
}t_SkillExp3,*LPSKILLEXP3;

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//		레어 이펙트
///////////////////////////////////////////////////////////////////////////
typedef struct STATICRAREEFFECT	
{	
	short	nPower				;//아이템의 물리적 Damage 증가	
	short	nAccurate			;//물리적 공격 성공률 증가 (타격시 miss 확률 감소)	
	short	nWise				;//마법 캐스팅 시간 00만큼 단축		short	nPhysicDmg			;//데미지 미리 구하기
	short	nBear				;//캐스팅 유지 시간 00만큼 증가		short	nPhysicDmgPlusRate	;//010607 lsw 데미지 퍼센트 증가율
	short	nDistant			;//공격 범위가 멀었을 때의 데미지 증가 (현재 100%/90%/85%로 감소)		
	short	nVigor				;//아이템의 물리적 Damage % 증가		short	nPhysicAttackRate	;	
	short	nCurse				;//자신이 시전한 저주 계열 마법 효과 지속 시간 00 증가		short	nPhysicDefenceRate	;	
	short	nAid				;//자신이 시전한 보조 계열 마법 효과 지속 시간 00 증가		
	short	nMasking			;//자신이 시전한 보호 계열 마법 효과 지속 시간 00 증가		short	nAc					;
	short	nPiercing			;//00% 확률로 상대의 보호 계열 마법을 무시하여 물리적 데미지를 입힘		short	nMoveSpeed			;
	short	nBasicResist		;//00% 확률로 상대의 마법 저항력을 무시(기본저항력)하여 마법 데미지를 입힘		
	short	nHardest			;//ac 증가		int		iHpMax				;
	short	nSmart				;//물리적 공격 회피율 증가 (상대가 물리적 타격시 miss 확률 증가)		int		iManaMax			;
	short	nResist_freeze		;//얼음저항력 증가		int		iDivineMax			;
	short	nResist_venom		;//독저항력 증가		int		iHealthMax			;
	short	nResist_lightning	;//전격저항력 증가			
	short	nResist_blaze		;//불저항력 증가		short	nHpMaxPlusRate		;
	short	nResist_paralysis	;//마비저항력 증가		short	nManaMaxPlusRate	;
	short	nResist_holy		;//신법저항력 증가		short	nDivineMaxPlusRate	;
	short	nResist_all			;//모든 저항력 증가
	short	nSpeed				;//이동 속도 증가	
	short	nLife				;//Life max 증가		short	nStr	;
	short	nMana				;//Mana max 증가		short	nDex	;
	short	nDivine				;//Divine max 증가		short	nCon	;
	short	nHealth				;//Health max 증가		short	nWis	;
	short	nBrood				;//Life max % 증가		short	nInt	;
	short	nMind				;//Mana max % 증가		short	nChar	;
	short	nSpirit				;//Divine max % 증가		short	nEndu	;
	short	nYouth				;//Health max % 증가		short	nMoveP	;
	short	nFatal				;//Life 1회 자동 회복량 증가		short	nMoral	;
	short	nMagic				;//Mana 1회 자동 회복량 증가		short	nLuck	;
	short	nNature				;//Divine 1회 자동 회복량 증가		short	nWs		;	
	short	nStrength			;//str 증가		short	nPs		;	
	short	nDexterity			;//dex 증가	
	short	nConstitution		;//con 증가		short	nResistIce	;
	short	nWisdom				;//wis 증가		short	nResistElect	;
	short	nIntelligence		;//int 증가		short	nResistFire	;
	short	nCharisma			;//cha 증가		short	nResistHoly	;
	short	nEndurance			;//endu 증가		short	nResistPoison;
	short	nMovement			;//movp 증가		short	nResistCurse ;
	short	nMoral				;//mor 증가	
	short	nLucky				;//luck 증가	////////////////////////////
	short	nWizard				;//WS 증가	// 마법 증폭 속성
	short	nPriest				;//PS 증가		char	cAmpIce		;
//	short	nMagic_power		;//위자드 계열 마법 데미지 증폭		char	cAmpElect	;
	short	nMagic_vigor		;//위자드 계열 마법 데미지 00% 증폭		char	cAmpFire		;
	short	nMagic_stay			;//위자드 계열 마법 지속 시간 증폭		char	cAmpHoly		;
//	short	nNature_power		;//프리스트 계열 마법 데미지 증폭		char	cAmpPoison	;
	short	nNature_vigor		;//프리스트 계열 마법 데미지 00% 증폭		char	cAmpCurse	;
	short	nNature_stay		;//프리스트 계열 마법 지속시간 증폭	
	short	nLife_recovery		;//Life 자동 회복량 최대치 증가 (전체의 00%까지 자동 회복)////////////////////////////
	short	nMana_recovery		;//Mana 자동 회복량 최대치 증가 (전체의 00%까지 자동 회복)// 흡혈 속성
	short	nCritical			;//크리티컬 데미지 나올 확률 00% 증가		short	HpSteal;
//	short	nGuardian			;//캐릭터의 물리적 방어력 증가		short	ManaSteal;
	short	nEvasion			;//공격 마법 회피율 증가 (상대의 공격 마법 무효화 확률)		short	DivineSteal;
//		56					//
//		57					//
//		58					//
//		59					//
//		60					//
//		61					//
//		62					//
//		63					//
//		64					//
//		65					//
//		66					//
//		67					//
	short	nWitch				;//타격 시 상대의 Mana를 00만큼 뺏어옴		short	CastingTimeReduceRate;
	short	nDevil				;//타격 시 상대의 Mana를 00% 만큼 뺏어옴		short	CastingTimeKeepRate;
	short	nDesire				;//타격 시 상대의 Life를 부가적으로 00만큼 뺏어옴		short	MagicContinueTimeRate;
	short	nRobber				;//타격 시 상대의 Life를 부가적으로 00% 만큼 뺏어옴	////////////////////////////
	short	nEvil				;//타격 시 상대의 Divine을 부가적으로 00만큼 뺏어옴	// 회복 속성
	short	nSatan				;//타격 시 상대의 Divine을 부가적으로 00% 만큼 뺏어옴		short	HpRecoverSpeed; 
	short	nWeary				;//타격 시 상대의 Health를 부가적으로 00만큼 뺏어옴		short	ManaRecoverSpeed; 
	short	nFatigue			;//타격 시 상대의 Health를 부가적으로 00% 만큼 뺏어옴		short	DivineRecoverSpeed; 
	short	nCounterattack		;//공격자에게 00의 데미지를 돌려줌	////////////////////////////
	short	nStrike				;//공격자의 데미지에서 00%를 돌려줌	// 반격 속성
	short	nSnag				;//타격 시 상대를 00(시간)동안 마비시킴		short	DmgReturn;
	short	nVertigly			;//타격 시 상대를 00(시간)동안 혼란에 걸리게 함		short	DmgReturnRate;
	short	nPoison				;//타격 시 상대를 00(시간)동안 중독에 걸리게 함	
	short	nStiff				;//타격 시 상대를 00(시간)동안 석화에 걸리게 함	
	short	nSlack				;//타격 시 상대를 00(시간)동안 속도저하에 걸리게 함	//011025 lsw <
//		83					//	
//		84					//	
	short	nGhost				;//착용하게 되면 유령을 볼 수 있음	
	short	nLandmine			;//일반 트랩을 감지할 수 있음		
	short	nTraping			;//마법 트랩을 감지할 수 있음		
	short	nDetect				;//모든 트랩을 감지할 수 있음	//위에꺼 따다 씀 	
	
	short	nHighPower;
	short	nHighShield;
	short	nHighBlood;
	short	nHighMagic;
	short	nHighNature;
}t_StaticRareEffect,*lpStaticRareEffect;

typedef struct DYNAMICRAREEFFECT	
{	
	char	cRareType1;
	int 	iValue1;
	DWORD	iRareTime1;

	char	cRareType2;
	int 	iValue2;
	DWORD	iRareTime2;
	
	char	cRareType3;
	int 	iValue3;
	DWORD	iRareTime3;
}t_DynamicRareEffect,*lpDynamicRareEffect;

// 031009 CI YGI
struct k_item_limit_number
{
	DWORD who_make	:8;
	DWORD number 	:24;
};

///////////////////////////////////////////////////////////////////////////

const int MULTIPLE_USE_ONE_TIME_DEC_DUR =10;//여러번 먹는 아이템 한번에 깎이는 내구도
class CItem;
// 031009 CI YGI
class CItemLimitMgr
{
public :
	enum
	{
		LIMIT_OF_NO_HAVE_LIMIT_ITEM = 1004,			// 리미트 넘버를 가질 수 없는 아이템들의 리미트 번호
	};
private :
	int		m_nItemLimitCount;	// 고유 리미트 넘버 // 카운트 되는 값
	map<int, int> m_mapHaveLimitItem;		//  리미트 값을 가져야 하는 아이템 번호 모음

public :
	CItemLimitMgr() {}
	~CItemLimitMgr() { m_mapHaveLimitItem.clear(); }

	int	GetLimitNumber( int item_no );	// 맵서버당 가지고 있는 고유의 리미트 넘버 값
	int	GetLimitNumberDontCount() { return m_nItemLimitCount; };	// 카운트 하지 않는다.
	void SetLimitCount( int count )
	{
		m_nItemLimitCount = count;
	}
	int	LoadHaveLimitItem();		// 디비에서 로딩해온다.
	bool IsCanHaveLimitItem( int item_no )	// 리미트 번호를 가질수 없는 아이템 인가?
	{
		map<int, int>::iterator itor;
		if( m_mapHaveLimitItem.find( item_no ) == m_mapHaveLimitItem.end() )
		{
			return false;	// 리미트 번호를 가질 수 없는 아이템이다.
		}
		return true;		// 리미트 번호를 가질 수 있는 아이템이다.
	}
};
/////////////////////////////////////////////////

class CItemMgr  : public CItemLimitMgr	// 031009 CI YGI
{
private:
	ITEMATTR	NULLITEM;
public:
	CItemMgr();
	virtual ~CItemMgr();

public:
	
	CItem*		ConvertItemNoToCItem(const int iItemNo);
	CItem*		ConvertItemNoToCItem(const ITEMATTR item){return ConvertItemNoToCItem(item.item_no);}

	int			ChangeItemAttribute(ITEMATTR &TargetItem, const int iType, const int iAttr);
	int			ChangeItemAttribute(ITEMATTR &TargetItem, const ITEMATTR SourceItem);
	
	int			DeleteItem(ITEMATTR &item);
	
	ITEMATTR	GenerateItem(const int iItemNo);
	ITEMATTR	GenerateItem(const int iItemNo, const int iType , const int iCustomValue);//dwCustomDuration 듀레이션

//	int			CheckFaultItem(){return 0;}//전체 리스트 중에 이상한 애들을 지워낸다 내부에는 CheckFaultItem(ITEMATTR &item,bool bDeleteFlag)이 돌아간다
	int			CheckFaultItem(ITEMATTR &item,bool bDeleteFlag);//iDeleteFlag 1을 넣으면 잘못된 아이템일 경우 삭제 시켜버린다, 0을 넣으면 삭제는 시키지 않는다.
	
	int			CheckAddAbleItem(const ITEMATTR SourceItem,const ITEMATTR TargetItem);

	int			CombineItem(ITEMATTR &SourceItem,ITEMATTR &TargetItem);//아이템 수량 합치기

	bool		SwapItem(ITEMATTR &src,ITEMATTR &target);

	int			GenerateItemLimitNumber( int item_no );	// 031009 CI YGI
	int			GenerateItemDuration(const int iItemNo);
	
	virtual int	GetItemByPosition(){return 0;}//const;
	virtual int	FindEmptyPosition(){return 0;}//const;
	
	virtual int	ItemListLineUp(int iType, int iLineUpStandard){return 0;}//const;
	virtual int	ItemListEmpty(int iType){return 0;} //const;//아이템 리스트 깔끔하게 비우기
	
	virtual ITEMATTR	SearchItemByItemNo(int iType, int &iPos, int iItemNo){return NULLITEM;}//const;
	virtual ITEMATTR	SearchItemByItemKind(int iType, int &iPos, int iItemKind){return NULLITEM;}//const;
	virtual ITEMATTR	SearchItemByItemStruct(int iType, int &iPos, ItemAttr compare){return NULLITEM;}// const;

public://아이템 정보 얻는 클래스
	
	int	GetItemWeight(const int iItemNo);//아이템 테이블의 해당 번호의 기본 무게 미구현
	int	GetItemWeight(const ITEMATTR item);//아이템 구조체 자체가 가지는 무게
	
	int	GetItemDur(const ITEMATTR item, unsigned short &NowDur,unsigned short &MaxDur);//내구도용 아이템
	int	GetItemMuch(const ITEMATTR item, int &NowMuch);//나누어 지는 아이템일 경우

	int	GetItemKind(const int iItemNo);
	int	GetItemKind(const ITEMATTR item){return GetItemKind(item.item_no);}

	int	GetItemTactic(const int iItemNo);
	int	GetItemTactic(const ITEMATTR item){return GetItemTactic(item.item_no);}
	
	int	GetItemValue(const int iItemNo);
	int	GetItemValue(const ITEMATTR item);

	int	GetItemMaxMuch(const int iItemNo);
	int	GetItemMaxMuch(const ITEMATTR item){return GetItemMaxMuch(item.item_no);}

	int DecItemDur(ITEMATTR &item,const DWORD iValue);
	
	ItemAttr	GiveRareItem(const int item_no, const int iDur,const int iCallType,const int iTryMulti, const int iGradeMin, const int iGradeMax, RareMain &RareAttr,const int iHighLevel);//020725 lsw
	ItemAttr	GiveLegendItem(const int item_no, const int iDur = 0);
	int			GetAbleRareKind(const int iCallType,const int iHighLevel);
	int			AbleMakeOnThisMap(const int iMapNation,const int iNo);
	bool		MakeRareAttr(unsigned int &attr3 ,const int iNowGrade,const int iSokSung1,const int iSokSung2,const int iSokSung3,const int iHighLevel,const bool bIsDynamicRare)const;
	int			MakeDefaultRareItem( ItemAttr *item, const int iItemNo, const int iSok1, const int iGrade, const int iTryMulti = 0);
	int			GetSkillPointByItem(const ITEMATTR &item)const;
	int			GetStaticFunctionNo(const ITEMATTR &item);
};
		
extern	CItemMgr	ItemMgr;
#endif // !defined(AFX_ITEMMGR_H__CA091E53_778D_4AE5_85BC_B3E5D27503CF__INCLUDED_)
