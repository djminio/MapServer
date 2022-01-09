// RareEffectMgr.h: interface for the CRareEffectMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAREEFFECTMGR_H__D6CBF8EF_AAC2_4E8E_823B_4ED7AE440D83__INCLUDED_)
#define AFX_RAREEFFECTMGR_H__D6CBF8EF_AAC2_4E8E_823B_4ED7AE440D83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STATICRARE	false
#define DYNAMICRARE	true

enum eLegendNeed
{
	LEGEND_NEED_WEAPON	= 0x00000001,
	LEGEND_NEED_SHIELD	= 0x00000002,
	LEGEND_NEED_HELMET	= 0x00000004,
	LEGEND_NEED_ARMOR	= 0x00000008,
	LEGEND_NEED_NECK	= 0x00000010,
	LEGEND_NEED_SHOES	= 0x00000020,
	LEGEND_NEED_UNIQUE	= 0x00000040
};

enum eRareType
{
	RARE_POWER				=1,	//	short	nPower				//아이템의 물리적 Damage 증가	
	RARE_ACCURATE			=2,	//	char	nAccurate			//물리적 공격 성공률 증가 (타격시 miss 확률 감소)	
	RARE_WISE				=3,	//	short	nWise				//마법 캐스팅 시간 00만큼 단축	
	RARE_BEAR				=4,	//	short	nBear				//캐스팅 유지 시간 00만큼 증가	
	RARE_DISTANT			=5,	//	char	cDistant			//공격 범위가 멀었을 때의 데미지 증가 (현재 100%/90%/85%로 감소)	
	RARE_VIGOR				=6,	//	short	nVigor				//아이템의 물리적 Damage % 증가	
	RARE_CURSE				=7,	//	short	nCurse				//자신이 시전한 저주 계열 마법 효과 지속 시간 00 증가	
	RARE_AID				=8,	//	short	nAid				//자신이 시전한 보조 계열 마법 효과 지속 시간 00 증가	
	RARE_MASKING			=9,	//	short	nMasking			//자신이 시전한 보호 계열 마법 효과 지속 시간 00 증가	
	RARE_PIERCING			=10,	//	char	cPiercing			//00% 확률로 상대의 보호 계열 마법을 무시하여 물리적 데미지를 입힘	
	RARE_BASIC_RESIST		=11,//	char	cBasicResist		//00% 확률로 상대의 마법 저항력을 무시(기본저항력)하여 마법 데미지를 입힘	
	RARE_HARDEST			=12,	//	short	nHardest			//ac 증가	
	RARE_SMART				=13,	//	char	cSmart				//물리적 공격 회피율 증가 (상대가 물리적 타격시 miss 확률 증가)	
	RARE_RESIST_FREEZE		=14,	//	short	nResist_freeze		//얼음저항력 증가	
	RARE_RESIST_VENOM		=15,	//	short	nResist_venom		//독저항력 증가	
	RARE_RESIST_LIGHTNING	=16,	//	short	nResist_lightning	//전격저항력 증가	
	RARE_RESIST_BLAZE		=17,	//	short	nResist_blaze		//불저항력 증가	
	RARE_RESIST_PARALYSIS	=18,	//	short	nResist_paralysis	//마비저항력 증가	
	RARE_RESIST_HOLY		=19,	//	short	nResist_holy		//신법저항력 증가	
	RARE_RESIST_ALL			=20,	//	short	nResist_all			//모든 저항력 증가	//위에꺼 따다 씀
	RARE_SPEED				=21,	//	short	nSpeed				//이동 속도 증가	
	RARE_LIFE				=22,	//	short	nLife				//Life max 증가	
	RARE_MANA				=23,	//	short	nMana				//Mana max 증가	
	RARE_DIVINE				=24,	//	short	nDivine				//Divine max 증가	
	RARE_HEALTH				=25,	//	short	nHealth				//Health max 증가	
	RARE_BROOD				=26,	//	short	nBrood				//Life max % 증가	
	RARE_MIND				=27,	//	short	nMind				//Mana max % 증가	
	RARE_SPIRIT				=28,	//	short	nSpirit				//Divine max % 증가	
	RARE_YOUTH				=29,	//	short	nYouth				//Health max % 증가	
	RARE_FATAL				=30,	//	short	nFatal				//Life 1회 자동 회복량 증가	
	RARE_MAGIC				=31,	//	short	nMagic				//Mana 1회 자동 회복량 증가	
	RARE_NATURE				=32,	//	short	nNature				//Divine 1회 자동 회복량 증가	
	RARE_STRENGTH			=33,	//	short	nStrength			//str 증가	
	RARE_DEXTERITY			=34,	//	short	nDexterity			//dex 증가	
	RARE_CONSTITUTION		=35,	//	short	nConstitution		//con 증가	
	RARE_WISDOM				=36,	//	short	nWisdom				//wis 증가	
	RARE_INTELLIGENCE		=37,	//	short	nIntelligence		//int 증가	
	RARE_CHARISMA			=38,	//	short	nCharisma			//cha 증가	
	RARE_ENDURANCE			=39,	//	short	nEndurance			//endu 증가	
	RARE_MOVEMENT			=40,	//	short	nMovement			//movp 증가	
	RARE_MORAL				=41,	//	short	nMoral				//mor 증가	
	RARE_LUCKY				=42,	//	short	nLucky				//luck 증가	
	RARE_WIZARD				=43,	//	short	nWizard				//WS 증가	
	RARE_PRIEST				=44,	//	short	nPriest				//PS 증가	
	RARE_MAGIC_POWER		=45,	//	short	nMagic_power		//위자드 계열 마법 데미지 증폭	
	RARE_MAGIC_VIGOR		=46,	//	char	cMagic_vigor		//위자드 계열 마법 데미지 00% 증폭	
	RARE_MAGIC_STAY			=47,	//	short	nMagic_stay			//위자드 계열 마법 지속 시간 증폭	
	RARE_NATURE_POWER		=48,	//	short	nNature_power		//프리스트 계열 마법 데미지 증폭	
	RARE_NATURE_VIGOR		=49,	//	char	cNature_vigor		//프리스트 계열 마법 데미지 00% 증폭	
	RARE_NATURE_STAY		=50,	//	short	nNature_stay		//프리스트 계열 마법 지속시간 증폭	
	RARE_LIFE_RECOVERY		=51,	//	char	cLife_recovery		//Life 자동 회복량 최대치 증가 (전체의 00%까지 자동 회복)
	RARE_MANA_RECOVERY		=52,	//	char	cMana_recovery		//Mana 자동 회복량 최대치 증가 (전체의 00%까지 자동 회복)
	RARE_CRITICAL			=53,	//	char 	cCritical			//크리티컬 데미지 나올 확률 00% 증가	
	RARE_GUARDIAN			=54,	//	short	nGuardian			//캐릭터의 물리적 방어력 증가	
	RARE_EVASION			=55,	//	char	nEvasion			//공격 마법 회피율 증가 (상대의 공격 마법 무효화 확률)	
	RARE_56					=56,	//		56					//	
	RARE_57					=57,	//		57					//	
	RARE_58					=58,	//		58					//	
	RARE_59					=59,	//		59					//	
	RARE_60					=60,	//		60					//	
	RARE_61					=61,	//		61					//	
	RARE_62					=62,	//		62					//	
	RARE_63					=63,	//		63					//	
	RARE_64					=64,	//		64					//	
	RARE_65					=65,	//		65					//	
	RARE_66					=66,	//		66					//	
	RARE_67					=67,	//		67					//	
	RARE_WITCH				=68,	//	short	nWitch				//타격 시 상대의 Mana를 00만큼 뺏어옴	
	RARE_DEVIL				=69,	//	char	cDevil				//타격 시 상대의 Mana를 00% 만큼 뺏어옴	
	RARE_DESIRE				=70,	//	short	nDesire				//타격 시 상대의 Life를 부가적으로 00만큼 뺏어옴	
	RARE_ROBBER				=71,	//	char	cRobber				//타격 시 상대의 Life를 부가적으로 00% 만큼 뺏어옴	
	RARE_EVIL				=72,	//	short	nEvil				//타격 시 상대의 Divine을 부가적으로 00만큼 뺏어옴	
	RARE_SATAN				=73,	//	char	cSatan				//타격 시 상대의 Divine을 부가적으로 00% 만큼 뺏어옴	
	RARE_WEARY				=74,	//	short	nWeary				//타격 시 상대의 Health를 부가적으로 00만큼 뺏어옴	
	RARE_FATIGUE			=75,	//	char	cFatigue			//타격 시 상대의 Health를 부가적으로 00% 만큼 뺏어옴	
	RARE_COUNTERATTACK		=76,	//	short	nCounterattack		//공격자에게 00의 데미지를 돌려줌	
	RARE_STRIKE				=77,	//	char	cStrike				//공격자의 데미지에서 00%를 돌려줌	
//	RARE_78					=78,	//		83					//	
	RARE_SNAG				=79,	//	short	nSnag				//타격 시 상대를 00(시간)동안 마비시킴	
	RARE_VERTIGLY			=80,	//	short	nVertigly			//타격 시 상대를 00(시간)동안 혼란에 걸리게 함	
	RARE_POISON				=81,	//	short	nPoison				//타격 시 상대를 00(시간)동안 중독에 걸리게 함	
	RARE_STIFF				=82,	//	short	nStiff				//타격 시 상대를 00(시간)동안 석화에 걸리게 함	
	RARE_SLACK				=83,	//	short	nSlack				//타격 시 상대를 00(시간)동안 속도저하에 걸리게 함	
//	RARE_84					=84,	//		84					//	
	RARE_GHOST				=85,	//	char	bGhost				//착용하게 되면 유령을 볼 수 있음	
	RARE_LANDMINE			=86,	//	char	bLandmine			//일반 트랩을 감지할 수 있음	
	RARE_TRAPING			=87,	//	char	bTraping			//마법 트랩을 감지할 수 있음	
	RARE_DETECT				=88,	//	char	bDetect				//모든 트랩을 감지할 수 있음	//위에꺼 따다 씀 
	
	RARE_HIGH_POWER			=101,	//020730 lsw 하이 레어의 시작점
	RARE_HIGH_SHIELD		=102,	
	RARE_HIGH_BLOOD			=103,
	RARE_HIGH_MAGIC			=104,
	RARE_HIGH_NATURE		=105,
	RARE_HIGH_RESIST_ALL	=106,
};
const int MAX_STATIC_RARE	= RARE_HIGH_POWER-1;//020725 lsw
const int START_HIGH_RARE	= RARE_HIGH_POWER;//020725 lsw

const int RARE_HP_STEAL_RAND			=20;//HP 뺏을 확률( /100)
const int RARE_MP_STEAL_RAND			=20;//MP 뺏을 확률( /100)
const int RARE_DIVINE_STEAL_RAND		=20;//DIVINE 뺏을 확률( /100)
const int RARE_HUNGRY_STEAL_RAND		=20;//HUNGRY 뺏을 확률( /100)
const int RARE_COUNTERATTCK_RAND		=20;//반격 할 확률( /100)
const int RARE_AUTO_CURSE				=20;//반격 할 확률( /100)

const int MAX_IMR_FIELD_CT = 7;
typedef struct ITEMMULTIRARE
{
	int iGrade;
	int iAble;
	int aItemNo[MAX_IMR_FIELD_CT];//0 바이, 1 자이, 일스
	int aItemCt[MAX_IMR_FIELD_CT];//0 바이, 1 자이, 일스
	int	iAddCt;
}t_ItemMultiRare,*LPITEMMULTIRARE;
const int MAX_ITEM_MULTI_RARE= 17;//테이블의 카운트

const int MAX_ITEM_LEGEND_SUB_BONUS_MAX = 4;
typedef struct ITEM_LEGEND_SUB
{
	int iGroupIndex;
	int iGroupMax;
	char szName[20];
	int iNeeds;
	int iWARHand2;
	int iWALHand3;
	int iWAHead4;
	int iWABody5;
	int iWANeck6;
	int iWAShoe7;
	int iWAUnique8;
	int iWATwoHand10;
	int aiBonus[MAX_ITEM_LEGEND_SUB_BONUS_MAX];
}t_ItemLegendSub,*LPITEMLEGENDSUB;

const int MAX_ITEM_LEGEND_SUB = 10;
const int MAX_ITEM_LEGEND	= 30;

typedef struct ITEM_LEGEND
{
	int iNo;
	int iGroupNo;
	int iGroupIndexMax;
	t_ItemLegendSub aSub[MAX_ITEM_LEGEND_SUB];
}t_ItemLegend,*LPITEMLEGEND;


const int MAX_ITEM_LEGEND_BONUS	= 1000;
const int MAX_ITEM_LEGEND_BONUS_KIND	= 6;
typedef struct ITEM_LEGEND_BONUS
{
	int iNo;
	char szName[20];
	int aiKind[MAX_ITEM_LEGEND_BONUS_KIND];
	int aiGrade[MAX_ITEM_LEGEND_BONUS_KIND];
}t_ItemLegendBonus,*LPITEMLEGENDBONUS;

class CRareEffectMgr
{
public:
	CRareEffectMgr();
	virtual ~CRareEffectMgr();
private:
	ITEMMULTIRARE m_MRData[MAX_ITEM_MULTI_RARE];
	ITEM_LEGEND  m_aItemLegend[MAX_ITEM_LEGEND];
	ITEM_LEGEND_BONUS	m_aLegendBonus[MAX_ITEM_LEGEND_BONUS];
public:
	int		ResetRareEffect(void* ch);//레어 이펙트 리셋
	void	ClearRareEffect(void* ch);//레어 이펙트 제거
	void	ClearDynamicRareEffect(void* ch);//기능 아이템만 클리어

	int		GetStaticRareEffectValue(const int iKind, const int iGrade)const;
	int		GetDynamicRareEffectValue(const int iKind, const int iGrade)const;
	
	int		GetDynamicRareValue(const int iDRType,const	DYNAMICRAREEFFECT	&DR) const;
	int		GetRareDisplayEffectNo		(const bool bIsDynamicRare,const DYNAMICRAREEFFECT &DR)const;	// 사용된 마법의 번호
	int		GetRareDisplayEffectTime	(const DYNAMICRAREEFFECT &DR)const;	// 유지시간
	
	int		ApplyStaticRareEffect(STATICRAREEFFECT	&SR, const int iKind, const int iGrade,bool bSymbol = false);//soto-1.02
	int		AddANDCheckStaticRareMaxValue(const int iKind,short &nNow, const int iAddValue,bool bSymbol = false);//soto-1.02
	short	GetStaticRareMaxValue(const int iKind)const;

	int		ApplyDynamicRareEffect(void* ch,const int iKind, const int iGrade,const int iOrder);//iOrder 에 0을 넣으면 순간 적인 효과를 보는 것만 적용
	int		AddANDCheckDynamicRareMaxValue(const int iKind,const int iValue);
	int		GetDynamicRareMaxValue(const int iKind)const ;

	int		CheckFaultStaticRareType(const int iKind)const;
	int		CheckFaultDynamicRareType(const int iKind)const;
	int		CheckFaultRareGrade(const int iGrade)const;

	int		CorrectByStaticRare(void *a, void *d, int &iDmg);

	int		LoadItem_Multi_Rare_Table();
	int		LoadItem_Legend_Table();
	int		LoadItem_Legend_Bonus_Item();
public:
	int		GetAbleGroupIndex(const int iGroupNo);
	int		GetLegendBasicRareKind(const int iGroupNo,const int iGroupIndex,const int iWearAble);
	ITEMMULTIRARE	GetIMR(const int iIndex)const
	{ 
		if(0 > iIndex || MAX_ITEM_MULTI_RARE <= iIndex)
		{
			return m_MRData[0];
		}
		return m_MRData[iIndex];
	}
public:
	/////////////////////////////////////////////
	inline int	GetStaticRarePower(const	STATICRAREEFFECT	&SR)		const{	return	SR.nPower			;	}
	inline int	GetStaticRareAccurate(const	STATICRAREEFFECT	&SR)		const{	return	SR.nAccurate		;	}
	inline int	GetStaticRareWise(const	STATICRAREEFFECT	&SR)			const{	return	SR.nWise			;	}
	inline int	GetStaticRareBear(const	STATICRAREEFFECT	&SR)			const{	return	SR.nBear			;	}
	inline int	GetStaticRareDistant(const	STATICRAREEFFECT	&SR)		const{	return	SR.nDistant			;	}
	inline int	GetStaticRareVigor(const	STATICRAREEFFECT	&SR)		const{	return	SR.nVigor			;	}
	inline int	GetStaticRareCurse(const	STATICRAREEFFECT	&SR)		const{	return	SR.nCurse			;	}
	inline int	GetStaticRareAid(const	STATICRAREEFFECT	&SR)			const{	return	SR.nAid				;	}
	inline int	GetStaticRareMasking(const	STATICRAREEFFECT	&SR)		const{	return	SR.nMasking			;	}
	inline int	GetStaticRarePiercing(const	STATICRAREEFFECT	&SR)		const{	return	SR.nPiercing		;	}
	inline int	GetStaticRareBasicResist(const	STATICRAREEFFECT	&SR)	const{	return	SR.nBasicResist		;	}
	inline int	GetStaticRareHardest(const	STATICRAREEFFECT	&SR)		const{	return	SR.nHardest			;	}
	inline int	GetStaticRareSmart(const	STATICRAREEFFECT	&SR)		const{	return	SR.nSmart			;	}
	inline int	GetStaticRareResist_freeze(const	STATICRAREEFFECT	&SR)const{	return	SR.nResist_freeze+SR.nResist_all;	}
	inline int	GetStaticRareResist_venom(const	STATICRAREEFFECT	&SR)	const{	return	SR.nResist_venom+SR.nResist_all;	}
	inline int	GetStaticRareResist_lightning(const	STATICRAREEFFECT	&SR)const{	return	SR.nResist_lightning+SR.nResist_all;	}
	inline int	GetStaticRareResist_blaze(const	STATICRAREEFFECT	&SR)	const{	return	SR.nResist_blaze+SR.nResist_all;	}
	inline int	GetStaticRareResist_paralysis(const	STATICRAREEFFECT	&SR)const{	return	SR.nResist_paralysis+SR.nResist_all;	}
	inline int	GetStaticRareResist_holy(const	STATICRAREEFFECT	&SR)	const{	return	SR.nResist_holy+SR.nResist_all;	}
	inline int	GetStaticRareSpeed(const	STATICRAREEFFECT	&SR)		const{	return	SR.nSpeed	;		}
	inline int	GetStaticRareLife(const	STATICRAREEFFECT	&SR)			const{	return	SR.nLife	;		}
	inline int	GetStaticRareMana(const	STATICRAREEFFECT	&SR)			const{	return	SR.nMana	;		}
	inline int	GetStaticRareDivine(const	STATICRAREEFFECT	&SR)		const{	return	SR.nDivine	;		}
	inline int	GetStaticRareHealth(const	STATICRAREEFFECT	&SR)		const{	return	SR.nHealth	;		}
	inline int	GetStaticRareBrood(const	STATICRAREEFFECT	&SR)		const{	return	SR.nBrood	;		}
	inline int	GetStaticRareMind(const	STATICRAREEFFECT	&SR)			const{	return	SR.nMind	;		}
	inline int	GetStaticRareSpirit(const	STATICRAREEFFECT	&SR)		const{	return	SR.nSpirit	;		}
	inline int	GetStaticRareYouth(const	STATICRAREEFFECT	&SR)		const{	return	SR.nYouth	;		}
	inline int	GetStaticRareFatal(const	STATICRAREEFFECT	&SR)		const{	return	SR.nFatal	;		}
	inline int	GetStaticRareMagic(const	STATICRAREEFFECT	&SR)		const{	return	SR.nMagic	;		}
	inline int	GetStaticRareNature(const	STATICRAREEFFECT	&SR)		const{	return	SR.nNature	;		}
	inline int	GetStaticRareStrength(const	STATICRAREEFFECT	&SR)		const{	return	SR.nStrength	;	}
	inline int	GetStaticRareDexterity(const	STATICRAREEFFECT	&SR)	const{	return	SR.nDexterity	;	}
	inline int	GetStaticRareConstitution(const	STATICRAREEFFECT	&SR)	const{	return	SR.nConstitution;	}
	inline int	GetStaticRareWisdom(const	STATICRAREEFFECT	&SR)		const{	return	SR.nWisdom		;	}
	inline int	GetStaticRareIntelligence(const	STATICRAREEFFECT	&SR)	const{	return	SR.nIntelligence;	}
	inline int	GetStaticRareCharisma(const	STATICRAREEFFECT	&SR)		const{	return	SR.nCharisma	;	}
	inline int	GetStaticRareEndurance(const	STATICRAREEFFECT	&SR)	const{	return	SR.nEndurance	;	}
	inline int	GetStaticRareMovement(const	STATICRAREEFFECT	&SR)		const{	return	SR.nMovement	;	}
	inline int	GetStaticRareMoral(const	STATICRAREEFFECT	&SR)		const{	return	SR.nMoral		;	}
	inline int	GetStaticRareLucky(const	STATICRAREEFFECT	&SR)		const{	return	SR.nLucky		;	}
	inline int	GetStaticRareWizard(const	STATICRAREEFFECT	&SR)		const{	return	SR.nWizard		;	}
	inline int	GetStaticRarePriest(const	STATICRAREEFFECT	&SR)		const{	return	SR.nPriest		;	}
//	inline int	GetStaticRareMagic_power(const	STATICRAREEFFECT	&SR)	const{	return	SR.nMagic_power	;	}
	inline int	GetStaticRareMagic_vigor(const	STATICRAREEFFECT	&SR)	const{	return	SR.nMagic_vigor	;	}
	inline int	GetStaticRareMagic_stay	(const	STATICRAREEFFECT	&SR)	const{	return	SR.nMagic_stay	;	}
//	inline int	GetStaticRareNature_power(const	STATICRAREEFFECT	&SR)	const{	return	SR.nNature_power;	}
	inline int	GetStaticRareNature_vigor(const	STATICRAREEFFECT	&SR)	const{	return	SR.nNature_vigor;	}
	inline int	GetStaticRareNature_stay(const	STATICRAREEFFECT	&SR)	const{	return	SR.nNature_stay;	}
//	inline int	GetStaticRareLife_recovery(const	STATICRAREEFFECT	&SR)const{	return	SR.cLife_recovery;	}
	inline int	GetStaticRareMana_recovery(const	STATICRAREEFFECT	&SR)const{	return	SR.nMana_recovery;	}
	inline int	GetStaticRareCritical(const	STATICRAREEFFECT	&SR)		const{	return	SR.nCritical	;	}
//	inline int	GetStaticRareGuardian(const	STATICRAREEFFECT	&SR)		const{	return	SR.nGuardian	;	}
	inline int	GetStaticRareEvasion(const	STATICRAREEFFECT	&SR)		const{	return	SR.nEvasion		;	}
	inline int	GetStaticRareWitch(const	STATICRAREEFFECT	&SR)		const{	return	SR.nWitch		;	}
	inline int	GetStaticRareDevil(const	STATICRAREEFFECT	&SR)		const{	return	SR.nDevil		;	}
	inline int	GetStaticRareDesire(const	STATICRAREEFFECT	&SR)		const{	return	SR.nDesire		;	}
	inline int	GetStaticRareRobber(const	STATICRAREEFFECT	&SR)		const{	return	SR.nRobber		;	}
	inline int	GetStaticRareEvil(const	STATICRAREEFFECT	&SR)			const{	return	SR.nEvil		;	}
	inline int	GetStaticRareSatan(const	STATICRAREEFFECT	&SR)		const{	return	SR.nSatan		;	}
	inline int	GetStaticRareWeary(const	STATICRAREEFFECT	&SR)		const{	return	SR.nWeary		;	}
	inline int	GetStaticRareFatigue(const	STATICRAREEFFECT	&SR)		const{	return	SR.nFatigue		;	}
	inline int	GetStaticRareCounterattack(const	STATICRAREEFFECT	&SR)const{	return	SR.nCounterattack;	}
	inline int	GetStaticRareStrike(const	STATICRAREEFFECT	&SR)		const{	return	SR.nStrike		;	}
	inline int	GetStaticRareSnag(const	STATICRAREEFFECT	&SR)			const{	return	SR.nSnag		;	}
	inline int	GetStaticRareVertigly(const	STATICRAREEFFECT	&SR)		const{	return	SR.nVertigly	;	}
	inline int	GetStaticRarePoison(const	STATICRAREEFFECT	&SR)		const{	return	SR.nPoison		;	}
	inline int	GetStaticRareStiff(const	STATICRAREEFFECT	&SR)		const{	return	SR.nStiff		;	}
	inline int	GetStaticRareSlack(const	STATICRAREEFFECT	&SR)		const{	return	SR.nSlack		;	}
	inline int	GetStaticRareGhost(const	STATICRAREEFFECT	&SR)		const{	return	SR.nGhost		;	}
	inline int	GetStaticRareLandmine(const	STATICRAREEFFECT	&SR)		const{	return	SR.nLandmine	;	}
	inline int	GetStaticRareTraping(const	STATICRAREEFFECT	&SR)		const{	return	SR.nTraping		;	}
	inline int	GetStaticRareDetect(const	STATICRAREEFFECT	&SR)		const{	return	SR.nDetect		;	}

	inline int	GetStaticRareHighPower(const	STATICRAREEFFECT	&SR)		const{	return	SR.nHighPower	;	}
	inline int	GetStaticRareHighShield(const	STATICRAREEFFECT	&SR)		const{	return	SR.nHighShield	;	}
	inline int	GetStaticRareHighBlood(const	STATICRAREEFFECT	&SR)		const{	return	SR.nHighBlood	;	}
	inline int	GetStaticRareHighMagic(const	STATICRAREEFFECT	&SR)		const{	return	SR.nHighMagic	;	}
	inline int	GetStaticRareHighNature(const	STATICRAREEFFECT	&SR)		const{	return	SR.nHighNature	;	}
	
	bool	IsHaveRivaval(void *ch);
public:
	void	AutoCheckDynamicRare(DYNAMICRAREEFFECT	&DR);
	DWORD	GetDynamicRareEffectTime(const int iKind, const int iGrade)const;//등급에 따른 유지 시간
};

#endif // !defined(AFX_RAREEFFECTMGR_H__D6CBF8EF_AAC2_4E8E_823B_4ED7AE440D83__INCLUDED_)
