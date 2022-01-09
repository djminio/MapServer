#if !defined(AFX_CITEM_H__3DCF6E55_A881_498E_9E4E_A342F8E62D81__INCLUDED_)
#define AFX_CITEM_H__3DCF6E55_A881_498E_9E4E_A342F8E62D81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h>
#ifndef _DRAGON_PROTOCOL_
#include "dr_network.h"
#endif
	
#include "kein_item.h"
//11025 lsw >
#include "skill_lsw.h"
//11025 lsw <

#define		ITEM_NAME_MAX		31

#define OK			1
#define NOT		0	


////////////////////////////////////////////
// 무기 장착 (equip) 
enum eWTTypes
{
	WT_WEAPON	= 0,
	WT_SHIELD	= 1,
	WT_HELMET	= 2,
	WT_ARMOR	= 3,
	WT_NECK		= 4,
	WT_SHOES	= 5,
	WT_UNIQUE1	= 6,
	WT_UNIQUE2	= 7, 
	WT_POTION	= 8,
};

enum WearTypeGroup
{
	WEAR_ANYTHING				=0,
	WEAR_ONLY_INVEN				=1,
	WEAR_RIGHT_HAND				=2,
	WEAR_LEFT_HAND				=3,
	WEAR_HEAD					=4,
	WEAR_BODY					=5,
	WEAR_NECK					=6,
	WEAR_SHOES					=7,
	WEAR_UNIQUE					=8,
	WEAR_BELT					=9,
	WEAR_TWO_HAND				=10
};
//////////////////////////////////////////////
#define	PLANT			0
#define	MINERAL			1
#define	HERB			2
#define	COOK			3 
#define	POTION			4
#define	TOOL			5
#define	WEAPON			6
#define	DISPOSABLE		7
#define	ARMOR			8
#define	ACCESSORY		9
#define	ETC				10	//기타 물품

#define OBJECTITEM		13

const int IATTR_ITEMNO		=	-1;
enum IA2_TYPE
{
	IA2_HIDE			= 0x00000001,
	IA2_NOTMOVE			= 0x00000002,
	IA2_POISONED		= 0x00000004,
	IA2_OPENED			= 0x00000008,
	IA2_BROKEN			= 0x00000010,
	IA2_HOUSE			= 0x00000020,
	IA2_MAGICED			= 0x00000040,
	IA2_CANNOTUSE		= 0x00000080,
	IA2_NOIDENTIFIED	= 0x00000100,
	IA2_DOOR			= 0x00000200,
	IA2_BOX				= 0x00000400,
	IA2_CHAIR			= 0x00000800,
	IA2_TABLE			= 0x00001000,
	IA2_FARM0			= 0x00002000,
	IA2_FARM1			= 0x00004000,
	IA2_FARM2			= 0x00008000,
	IA2_FARM3			= 0x00010000,
	IA2_FARM4			= 0x00020000,
	IA2_FARM5			= 0x00040000,
	IA2_FARM6			= 0x00080000,
	IA2_FARM7			= 0x00100000,
	IA2_FIRE			= 0x00200000,
	IA2_TRAPED0			= 0x00400000,
	IA2_TRAPED1			= 0x00800000,
	IA2_TRAPED2			= 0x01000000,
	IA2_FIRST_ITEM		= 0x02000000,
	IA2_SEAL_NOR		= 0x04000000,
	IA2_SEAL_SPC		= 0x08000000,
	IA2_COLOSSUS_STONE	= 0x10000000,
	IA2_SCENARIO_POTAL	= 0x20000000,
};

enum ITEM_STRUCT_INDEX
{
	IATTR_DURATION		=0,
	IATTR_LIMIT			=1,
	IATTR_ATTR			=2,
	IATTR_ID			=3,
	IATTR_RESERVED0		=4,
	IATTR_RESERVED1		=5,
};
const int IATTR_MUCH			= IATTR_DURATION;
const int IATTR_SEAL_STONE_NO	= IATTR_LIMIT; 
const int IATTR_SPRITE_TYPE		= IATTR_LIMIT;
const int IATTR_RARE_MAIN		= IATTR_ID;
const int IATTR_TREASURE_MAP	= IATTR_RESERVED0;	// 040105 YGI 보물
const int IATTR_MALL_TIMED		= IATTR_RESERVED0;  // Used by items in the mall that have a time limit

// 아이템 Type이 13이상인 경우 해당
#define ITEMTYPE_NORMAL		0
#define ITEMTYPE_CHAIR		1
#define ITEMTYPE_TABLE		2
#define ITEMTYPE_DOOR		3
#define ITEMTYPE_BOX		4

#define ITEMTYPE_SEAL_NOR	79
#define ITEMTYPE_SEAL_SPC	80

#define ITEMTYPE_COLOSSUS_STONE		87




#define Num_Of_Item_Type		11
#define MAX_ITEM_NUM			1000

#define Num_Of_CItem_Plant		14
#define Num_Of_CItem_Mineral	12
#define Num_Of_CItem_Herb		20
#define Num_Of_CItem_Cook		27
#define Num_Of_CItem_Potion		6 
#define Num_Of_CItem_Tool		27
#define Num_Of_CItem_Weapon		50
#define Num_Of_CItem_Disposable	19
#define Num_Of_CItem_Armor		40
#define Num_Of_CItem_Accessory	18
#define Num_Of_CItem_Etc		47
                                  

//########### 오른쪽 마우스 버튼 클릭후 실행하는 종류 정의 ###########
#define		USE_ITEM			1		// 소비되어 사라진다.
#define		DIVIDE_ITEM			3		// 아이템의 갯수를 분리한다.
#define		REMAIN_ITEM			4		// 빈병 처리
#define		MAGIC_BOOK_ITEM		5		// 마법책이 뜬다
#define		DIARY_ITEM			6		// 일기책이 뜬다.
#define		BOX_ITEM			7		// 박스가 열린다.
#define		DUAL_CHANGE_ITEM	8

class CItem
{	
public:
	//int Item_id ;
	int m_Much;			//1214
	int ThisMapValue;
	int cur_value;
	
protected :	
	
	// 31
public:
	virtual int getAllData(void) { printf("\nCITEM") ; return 1 ; }
	virtual void print_data(void) { printf("\nCITEM_PRINT_DATA") ; }
	virtual int is_equipable(int a, t_chr_status_info *b, POS c, POS d) { return 0 ; }
	virtual int use(int a, t_chr_status_info *b, POS c, POS d) { return 0 ; }
	virtual int GetValue() { return 0 ; }
	virtual int GetDuration() { return 1; }
	virtual int GetCirculation() { return 1;}
	
	virtual int UseItem( CHARLIST *ch) 	{ return 0;	}
	virtual int EatItem( CHARLIST *ch) 	{ return 0;	}
	virtual int GetUseJunk()			{ return 0; }
	virtual int GetRbutton()			{ return 0; }
	virtual int GetItemDuration()		{ return 1; }
	virtual int GetItemKind()			{ return 0; }
	virtual int GetKindOfMagic()		{ return 0; }
	virtual int GetIncrDamage()			{ return 0; }
	
	virtual int GetLevel()				{ return 0; }
	virtual int EquipItem(CHARLIST *ch) { return 0; }		// 001219_2 YGI
	virtual int PlusHp(CHARLIST *ch) { return 0; }		
	virtual int ReleaseItem(CHARLIST *ch ){return 0;}	//020314 lsw
	virtual int GetWearAble()			{ return 0; }
	
	// 1221 KHS
	virtual int Getbody() { return  0; }
	virtual int Getarms() { return  0; }
	virtual int Gethead() { return  0; }
	virtual int Getleg () { return  0; }
	virtual int Getneck() { return  0; }
	virtual int Gethand() { return  0; }
	virtual int GetDamage		() { return  0; }
	virtual int GetHit_rate		() { return  0; }
	virtual int GetDefense_rate	() { return  0; }
	virtual int GetAR_or_DH		() { return  0; }
	virtual int GetDefense_power() { return  0; }
	
	virtual int GetSkill_Ability(){ return  0; }
	virtual int GetRepairAble() { return 0; }
	
	virtual int Getrange() { return 0; }
	virtual int GetWeight() { return 0; }
	virtual int GetItemShow( int is_man ) { return 0; }
	
	virtual int GetItemImage() { return 0; }
	virtual char *GetHanName() { return NULL; }

  virtual int GetPlusLife() const { return 0; }
  virtual int GetPlusMana() const { return 0; }
  virtual int GetPlusDivine() const { return 0; }
  virtual int GetPlusHungry() const { return 0; }

	virtual int GetObjectNumber() { return 0; }
	virtual int Getmouse_MRD(){ return 0;}
	virtual int Getmouse_MR_Effect(){ return 0;}
	virtual int GetItemQuantity(){ return 0; }
	// 001220 zhh
	virtual int Getmouse_Mr_Wave(){ return 0; }
	virtual int GetCureDisease1(){ return 0; }
	virtual int GetCureDisease2(){ return 0; }
	virtual int GetImunityCure2(){ return 0; }
	virtual int GetImunityCure3(){ return 0; }
	virtual int GetNeed3Lv(){ return 0; }
	virtual int GetLegendAble(){ return 0; }
	virtual int GetLegendGroup(){ return 0; }
	virtual int GetNeedDualStep(){ return 0; }

	virtual int GetClassWarriorAble(){ return 0;}
	virtual int GetClassArcherAble(){ return 0;}
	virtual int GetClassThiefAble(){ return 0;}
	virtual int GetClassWizardAble(){ return 0;}
	virtual int GetClassClericAble(){ return 0;}
	virtual int GetStaticFunctionNo(){ return 0;}//030327 lsw
};	
	
	
	
	
	
class CItem_Plant :	public CItem
{	
//protected:			
public:
	int Item_id   ;
	char Name[ITEM_NAME_MAX] ;
	char Han_Name[ITEM_NAME_MAX] ;
	int lv   ;
	int Image_Number   ;
	int hp   ;
	int ap   ;
	int mp   ;
	int nutrition1   ;
	int nutrition2   ;
	int nutrition3   ;
	int anti_poison   ;
	int anti_paralysis   ;
	int anti_magic   ;
	int anti_fire   ;
	int anti_ice   ;
	int anti_electric   ;
	int Time_maintain   ;
	int cure_disease1   ;
	int cure_disease2   ;
	int cure_disease3   ;
	int cure_disease4   ;
	int cure_disease5   ;
	int take_disease1   ;
	int take_disease2   ;
	int take_disease3   ;
	int take_disease4   ;
	int take_disease5   ;
	int weight   ;
	int circulation_limit   ;
	int max_duration   ;
	int Maximum   ;
	int object   ;
	int wear_able   ;
	int base_value   ;
	int base_store   ;
	int store_const   ;
	int store_plus_const   ;
	int Mouse_MR;
	int Mouse_MRD;
	int Mouse_MR_Effect;
	int Mouse_Mr_Wave;
	int	Item_kind;
	
public:
	virtual int getAllData(void) ;
	virtual void print_data(void) {
		printf("\n%d %s %s %d", Item_id, Name, Han_Name, lv) ;
	}
	virtual int use(int a, t_chr_status_info *b, POS c, POS d) ;
	
	virtual int GetValue() 
	{
		int value;
		value =  base_value -( m_Much * store_const / 100000);
		return value; 
	}
	virtual int GetDuration() { return max_duration; }
	virtual int GetCirculation() { return circulation_limit;}
	virtual int EatItem( CHARLIST *ch);
	virtual int GetRbutton() { return Mouse_MR; }
	virtual int GetItemKind() { return Item_kind; }
	virtual int GetLevel() { return lv; }
	virtual int GetWearAble() { return wear_able; }
	virtual int GetWeight() { return weight; }
	virtual int GetItemImage() { return Image_Number; }
	
	virtual char *GetHanName() { return Han_Name; }
	virtual int GetObjectNumber() { return object; }
	virtual int Getmouse_MRD(){ return Mouse_MRD;}
	virtual int Getmouse_MR_Effect(){ return Mouse_MR_Effect;}
	
	virtual int Getmouse_Mr_Wave(){ return Mouse_Mr_Wave; }

	virtual int GetCureDisease1(){ return cure_disease1; }//020528 lsw
	virtual int GetCureDisease2(){ return cure_disease2; }//020610 lsw
	
};	
	
	
	
	
	

class CItem_Mineral : public CItem
{
//protected :
public :
	int Item_id   ;
	char Name[ITEM_NAME_MAX] ;
	char Han_Name[ITEM_NAME_MAX]  ;
	int lv   ;
	int Image_Number   ;
	int weight   ;
	int object   ;
	int wear_able   ;
	int base_value   ;
	int base_store   ;
	int store_const   ;
	int store_plus_const   ;
	int Mouse_MR;
	int Mouse_MRD;
	int Mouse_MR_Effect;
	int Mouse_Mr_Wave;
	int Item_kind;



public:
	virtual int getAllData(void) ;
 	virtual void print_data(void) {
		printf("\n%d %s %s %d", Item_id, Name, Han_Name, lv) ;
	}
	virtual int use(int a, t_chr_status_info *b, POS c, POS d) ;
	virtual int GetValue() 
	{
							int value;
							value =  base_value -( m_Much * store_const / 100000);
							return value; 
	}
	virtual int GetRbutton() { return Mouse_MR; }

	virtual int GetItemKind() { return Item_kind; }
	virtual int GetLevel() { return lv; }
	virtual int GetWearAble() { return wear_able; }
	virtual int GetWeight() { return weight; }
	virtual int GetItemImage() { return Image_Number; }
	virtual char *GetHanName() { return Han_Name; }
	virtual int GetObjectNumber() { return object; }
	virtual int Getmouse_MRD(){ return Mouse_MRD;}
	virtual int Getmouse_MR_Effect(){ return Mouse_MR_Effect;}

};










class CItem_Herb : public CItem
{
//protected :
public :
	int Item_id   ;
	char Name[ITEM_NAME_MAX]  ;
	char Han_Name[ITEM_NAME_MAX]  ;
	int lv   ;
	int Image_Number   ;
	int hp   ;
	int ap   ;
	int mp   ;
	int nutrition1   ;
	int nutrition2   ;
	int nutrition3   ;
	int anti_poison   ;
	int anti_paralysis   ;
	int anti_magic   ;
	int anti_fire   ;
	int anti_ice   ;
	int anti_electric   ;
	int Time_maintain   ;
	int cure_disease1   ;
	int cure_disease2   ;
	int cure_disease3   ;
	int cure_disease4   ;
	int cure_disease5   ;
	int cure_disease6   ;
	int take_disease1   ;
	int take_disease2   ;
	int take_disease3   ;
	int take_disease4   ;
	int take_disease5   ;
	int take_disease6   ;
	int Change_str   ;
	int Change_dex   ;
	int Change_con   ;
	int Change_wis   ;
	int Change_int   ;
	int Change_cha   ;
	int Change_Movp   ;
	int Change_end   ;
	int Change_mor   ;
	int Change_luc   ;
	int Change_WS   ;
	int Change_PS   ;
	int Time_maintain2   ;
	int Cure_poison   ;
	int Cure_stone_paralysis   ;
	int Cure_Mad   ;
	int Cure_sluggish   ;
	int Cure_Active   ;
	int ChangeNatural2   ;
	int Time_maintain3   ;
	int weight   ;
	int circulation_limit   ;
	int object   ;
	int wear_able   ;
	int base_value   ;
	int base_store   ;
	int store_const   ;
	int store_plus_const   ;
	int Mouse_MR;
	int Mouse_MRD;
	int Mouse_MR_Effect;
	int Mouse_Mr_Wave;
	int Item_kind;




public:
	virtual int getAllData(void) ;
 
	virtual void print_data(void) {
		printf("\n%d %s %s %d", Item_id, Name, Han_Name, lv) ;
	}
	virtual int use(int a, t_chr_status_info *b, POS c, POS d) ;
	virtual int GetValue() 
	{
							int value;
							value =  base_value -( m_Much * store_const / 100000);
							return value; 
	}
	virtual int UseItem( CHARLIST *ch);
	virtual int EatItem( CHARLIST *ch);
	virtual int GetCirculation() { return circulation_limit;}
	virtual int GetRbutton() { return Mouse_MR; }

	virtual int GetItemKind() { return Item_kind; }
	virtual int GetLevel() { return lv; }
	virtual int GetWearAble() { return wear_able; }
	virtual int GetWeight() { return weight; }
	virtual int GetItemImage() { return Image_Number; }
	virtual char *GetHanName() { return Han_Name; }
	virtual int GetObjectNumber() { return object; }
	virtual int Getmouse_MRD(){ return Mouse_MRD;}
	virtual int Getmouse_MR_Effect(){ return Mouse_MR_Effect;}
	virtual int GetCureDisease1(){ return cure_disease1; }//020528 lsw
	virtual int GetCureDisease2(){ return cure_disease2; }//020610 lsw
	virtual int GetStaticFunctionNo(){ return cure_disease1;}//030327 lsw
};
class CItem_Cook : public CItem
{
//protected :
public :
	int Item_id   ;
	char Name[ITEM_NAME_MAX]  ;
	char Han_Name[ITEM_NAME_MAX]  ;
	int lv   ;
	int Image_Number   ;
	int hp   ;
	int ap   ;
	int mp   ;
	int nutrition1   ;
	int nutrition2   ;
	int nutrition3   ;
	int anti_poison   ;
	int anti_paralysis   ;
	int anti_magic   ;
	int anti_fire   ;
	int anti_ice   ;
	int anti_electric   ;
	int Time_maintain   ;
	int cure_disease1   ;
	int cure_disease2   ;
	int cure_disease3   ;
	int cure_disease4   ;
	int cure_disease5   ;
	int cure_disease6   ;
	int take_disease1   ;
	int take_disease2   ;
	int take_disease3   ;
	int take_disease4   ;
	int take_disease5   ;
	int take_disease6   ;
	int Change_str   ;
	int Change_dex   ;
	int Change_con   ;
	int Change_wis   ;
	int Change_int   ;
	int Change_cha   ;
	int Change_end   ;
	int Change_Movp   ;
	int Change_mor   ;
	int Change_luc   ;
	int Change_WS   ;
	int Change_Ps   ;
	int Time_maintain2;
	int weight   ;
	int circulation_limit   ;
	int object   ;
	int wear_able   ;
	int base_value   ;
	int base_store   ;
	int store_const   ;
	int store_plus_const   ;
	int Mouse_MR;
	int Mouse_MRD;
	int Mouse_MR_Effect;
	int Mouse_Mr_Wave;
	int Use_Junk;
	int Item_kind;



public:
	virtual int getAllData(void) ;
	virtual void print_data(void) {
		printf("\n%d %s %s %d", Item_id, Name, Han_Name, lv) ;
	}
	virtual int use(int A, t_chr_status_info *B, POS C, POS D) ;
	virtual int GetValue() 
	{
							int value;
							value =  base_value -( m_Much * store_const / 100000);
							return value; 
	}

	virtual int UseItem( CHARLIST *ch);
	virtual int EatItem( CHARLIST *ch);
	virtual int GetCirculation() { return circulation_limit;}
	virtual int GetUseJunk() { return Use_Junk; }
	virtual int GetRbutton() { return Mouse_MR; }

	virtual int GetItemKind() { return Item_kind; }
	virtual int GetLevel() { return lv; }
	virtual int GetWearAble() { return wear_able; }
	virtual int GetWeight() { return weight; }
	virtual int GetItemImage() { return Image_Number; }
	virtual char *GetHanName() { return Han_Name; }
	virtual int GetObjectNumber() { return object; }
	virtual int Getmouse_MRD(){ return Mouse_MRD;}
	virtual int Getmouse_MR_Effect(){ return Mouse_MR_Effect;}
	virtual int GetCureDisease1(){ return cure_disease1; }//020528 lsw
	virtual int GetCureDisease2(){ return cure_disease2; }//020610 lsw
	virtual int GetStaticFunctionNo(){ return cure_disease1;}//030327 lsw
};

class CItem_Potion	: public CItem
{
//protected :
public :
	int Item_id   ;
	char Name[ITEM_NAME_MAX]  ;
	char Han_Name[ITEM_NAME_MAX]  ;
	int lv   ;
	int Image_Number   ;
	int hp   ;
	int ap   ;
	int mp   ;
	int nutrition1   ;
	int nutrition2   ;
	int nutrition3   ;
	int anti_poison   ;
	int anti_paralysis   ;
	int anti_magic   ;
	int anti_fire   ;
	int anti_ice   ;
	int anti_electric   ;
	int Time_maintain   ;
	int cure_disease1   ;
	int cure_disease2   ;
	int cure_disease3   ;
	int cure_disease4   ;
	int cure_disease5   ;
	int cure_disease6   ;
	int take_disease1   ;
	int take_disease2   ;
	int take_disease3   ;
	int take_disease4   ;
	int take_disease5   ;
	int take_disease6   ;
	int Change_str   ;
	int Change_dex   ;
	int Change_con   ;
	int Change_wis   ;
	int Change_int   ;
	int Change_cha   ;
	int Change_end   ;
	int Change_movp   ;
	int Change_mor   ;
	int Change_luc   ;
	int Change_WS   ;
	int Change_PS   ;
	int Time_maintain2  ;
	int Cure_poison   ;
	int Cure_stone_paralysis   ;
	int Cure_Mad   ;
	int Cure_sluggish   ;
	int Cure_Active   ;
	int ChangeNatural2   ;
	int Time_maintain3   ;
	int weight   ;
	int circulation_limit   ;
	int object   ;
	int wear_able   ;
	int base_value   ;
	int base_store   ;
	int store_const   ;
	int store_plus_const   ;
	int Mouse_MR;
	int Mouse_MRD;
	int Mouse_MR_Effect;
	int Mouse_Mr_Wave;
	int Use_Junk;
	int Item_kind;



public:
	virtual int getAllData(void) ;
	virtual void print_data(void) {
		printf("\n%d %s %s %d", Item_id, Name, Han_Name, lv) ;
	}
	virtual int is_equipable(int a, t_chr_status_info *b, POS c, POS d) ;
	virtual int use(int a, t_chr_status_info *b, POS c, POS d) ;
	virtual int GetValue() 
	{
							int value;
							value =  base_value -( m_Much * store_const / 100000);
							return value; 
	}
	virtual int EatItem( CHARLIST *ch);
	virtual int UseItem( CHARLIST *ch);

	virtual int GetUseJunk() { return Use_Junk; }
	virtual int GetRbutton() { return Mouse_MR; }

	virtual int GetItemKind() { return Item_kind; }
	virtual int GetLevel() { return lv; }
	virtual int GetWearAble() { return wear_able; }
	virtual int GetWeight() { return weight; }
	virtual int GetItemImage() { return Image_Number; }
	virtual char *GetHanName() { return Han_Name; }
	virtual int GetObjectNumber() { return object; }
	virtual int Getmouse_MRD(){ return Mouse_MRD;}
	virtual int Getmouse_MR_Effect(){ return Mouse_MR_Effect;}
	virtual int GetCureDisease1(){ return cure_disease1; }//020528 lsw
	virtual int GetCureDisease2(){ return cure_disease2; }//020610 lsw
	virtual int GetStaticFunctionNo(){ return cure_disease1;}//030327 lsw
};
class CItem_Tool : public CItem
{
//protected :
public :
	int Item_id   ;
	char Name[ITEM_NAME_MAX]  ;
	char Han_Name[ITEM_NAME_MAX] ;
	int lv   ;
	int Image_Number   ;
	int Repairable   ;
	int Repair_min   ;
	int Repair_max   ;
	int Repair_Skill2   ;
	int Repair_Skill2_min   ;
	int Repair_Res1   ;
	int Amt_of_Repair_Res1   ;
	int Repair_Res2   ;
	int Amt_of_Repair_Res2   ;
	int Repair_Res3   ;
	int Amt_of_Repair_Res3   ;
	int Repair_Tool   ;
	int Amt_of_abrasion   ;
	int Change_str   ;
	int Change_dex   ;
	int Change_con   ;
	int Change_wis   ;
	int Change_int   ;
	int Change_cha   ;
	int Change_end   ;
	int Change_movp   ;
	int Change_mor   ;
	int Change_luc   ;
	int Change_ws   ;
	int Change_ps   ;
	int Change_mag   ;
	int Change_dev   ;
	int shape_woman   ;
	int shape_man   ;
	int success_rate   ;
	int abrasion_per_turn   ;
	int weight   ;
	int max_duration   ;
	int object   ;
	int wear_able   ;
	int base_value   ;
	int base_store   ;
	int store_const   ;
	int store_plus_const   ;
	int Mouse_MR;
	int Mouse_MRD;
	int Mouse_MR_Effect;
	int Mouse_Mr_Wave;
	int Item_kind;



public:
	virtual int getAllData(void) ;
	virtual void print_data(void) {
		printf("\n%d %s %s %d", Item_id, Name, Han_Name, lv) ;
	}
	virtual int is_equipable(int a, t_chr_status_info *b, POS c, POS d) ;
	virtual int GetValue() 
	{
							int value;
							value =  base_value -( m_Much * store_const / 100000);
							return value; 
	}
	virtual int GetDuration() { return max_duration; }
	virtual int GetRbutton() { return Mouse_MR; }
	virtual int GetItemKind() { return Item_kind; }
	virtual int GetLevel() { return lv; }
	virtual int GetWearAble() { return wear_able; }
	virtual int GetRepairAble() { return Repairable; }
	virtual int GetWeight() { return weight; }
	virtual int GetItemShow( int is_man ) { return is_man? shape_man : shape_woman; }
	virtual int GetItemImage() { return Image_Number; }
	virtual char *GetHanName() { return Han_Name; }

	virtual int GetObjectNumber() { return object; }
	virtual int Getmouse_MRD(){ return Mouse_MRD;}
	virtual int Getmouse_MR_Effect(){ return Mouse_MR_Effect;}

};
class CItem_Weapon : public CItem
{
//protected :
public :
	int Item_id   ;
	char Name[ITEM_NAME_MAX]  ;
	char Han_Name[ITEM_NAME_MAX] ;
	int lv   ;
	int Image_Number   ;
	int Repairable   ;
	int Repair_min   ;
	int Repair_max   ;
	int Repair_Skill2   ;
	int Repair_Skill2_min   ;
	int Repair_Res1   ;
	int Amt_of_Repair_Res1   ;
	int Repair_Res2   ;
	int Amt_of_Repair_Res2   ;
	int Repair_Res3   ;
	int Amt_of_Repair_Res3   ;
	int Repair_Tool   ;
	int Amt_of_abrasion   ;
	int Need2_lv   ;
	int Need2_str   ;
	int Need2_dex   ;
	int Need2_con   ;
	int Need2_wis   ;
	int Need2_int   ;
	int Need2_cha   ;
	int Need2_end   ;
	int Need2_Movp   ;
	int Need2_mor   ;
	int Need2_luc   ;
	int Need2_ws   ;
	int Need2_ps   ;
	int Need2_fame   ;
	int Need2_rank   ;
	int Need2_gender   ;
	int Need2_race   ;
	int Need2_min_age   ;
	int Need2_max_age   ;
	int Need3_lv   ;
	int Need3_str   ;
	int Need3_dex   ;
	int Need3_con   ;
	int Need3_wis   ;
	int Need3_int   ;
	int Need3_cha   ;
	int Need3_end   ;
	int Need3_Movp   ;
	int Need3_mor   ;
	int Need3_luc   ;
	int Need3_ws   ;
	int Need3_ps   ;
	int Need3_fame   ;
	int Need3_rank   ;
	int Need3_gender   ;
	int Need3_race   ;
	int Need3_min_age   ;
	int Need3_max_age   ;
	int Class_Warrior   ;
	int Class_Archer   ;
	int Class_Thief   ;
	int Class_Wizard   ;
	int Class_Cleric   ;
	int Change_str   ;
	int Change_dex   ;
	int Change_con   ;
	int Change_wis   ;
	int Change_int   ;
	int Change_cha   ;
	int Change_end   ;
	int Change_Movp   ;
	int Change_mor   ;
	int Change_luc   ;
	int Change_ws   ;
	int Change_ps   ;
	int Change_mag   ;
	int Change_dev   ;
	int casting_Time   ;
	int Change_Life_max   ;
	int Change_mana_max   ;
	int Change_Divine_Max   ;
	int Change_health_max   ;
	int Increase_Poisoning   ;
	int Increase_Stone   ;
	int Increase_Magic   ;
	int Increase_Fire   ;
	int Increase_Ice   ;
	int Increase_lightning   ;
	int imunity_Cure_1   ;
	int Imunity_Cure_2   ;
	int Imunity_Cure_3   ;
	int Imunity_Cure_4   ;
	int Imunity_Cure_5   ;
	int Imunity_Cure_6   ;
	int SkillEffect   ;
	int shape_woman   ;
	int shape_man   ;
	int Skill_Ability   ;
	int Damage   ;
	int Hit_rate   ;
	int Defense_rate   ;
	int ArorDH   ;
	int nRange; // CSD-031101
	int nWeaponSpeed; // CSD-031101
	int effect   ;
	int weight   ;
	int max_duration   ;
	int object   ;
	int wear_able   ;
	int base_value   ;
	int base_store   ;
	int store_const   ;
	int store_plus_const   ;
	int Item_kind;
	int Mouse_MR;
	int Mouse_MRD;
	int Mouse_MR_Effect;
	int Mouse_Mr_Wave;
	int kind_of_magic;
	int Incr_damage;


	// 34
public:
	virtual int getAllData(void) ;
	//virtual void print_data(void) {printf("\n %d %s %s %d %d %d %d %d %d\n", Item_id, Name, Han_Name, Image_Number, Fire_lim, Tool2, Change_mag, Change_dev, Need2_str) ;}
	//void print_data(void) {printf("\n %d %s %s %d %d %d %d %d %d %d", Id, Name, HanName, Image_Number, Level, Skill1, Skill1_Min, Skill1_Max, Skill2, Skill2_Min) ;}
	//void print_data(void) {printf("\n %d %s %s %d", Id, Name, HanName, Image_Number) ;}
	//virtual void  printAllMembersOffset(void) ;
	virtual int is_equipable(int a, t_chr_status_info *b, POS c, POS d) ;
	virtual int GetValue() 
	{
							int value;
							value =  base_value -( m_Much * store_const / 100000);
							return value; 
	}
	virtual int GetDuration	() { return max_duration; }
	virtual int EquipItem	( CHARLIST *ch);
	virtual int PlusHp(CHARLIST *ch); // 0720 YGI
//	virtual int CurrEquip(CHARLIST *ch ){ return 0; }	//020303 lsw
	virtual int ReleaseItem(CHARLIST *ch );			// 0206 YGI
	virtual int GetRbutton() { return Mouse_MR; }
	virtual int GetItemKind() { return Item_kind; }
	virtual int GetKindOfMagic() { return kind_of_magic; }
	virtual int GetIncrDamage() { return Incr_damage; }
	virtual int GetLevel() { return lv; }
	virtual int GetWearAble() { return wear_able; }

	virtual int GetSkill_Ability(){ return  Skill_Ability; }
	virtual int GetRepairAble() { return Repairable; }

	virtual int Getrange() { return nRange; } // CSD-031101
	virtual int GetWeight() { return weight; }
	virtual int GetItemShow( int is_man ) { return is_man? shape_man : shape_woman; }
	virtual int GetItemImage() { return Image_Number; }
	virtual char *GetHanName() { return Han_Name; }

  virtual int GetPlusLife() const { return Change_Life_max; }
  virtual int GetPlusMana() const { return Change_mana_max; }
  virtual int GetPlusDivine() const { return Change_Divine_Max; }
  virtual int GetPlusHungry() const { return Change_health_max; }

	virtual int GetObjectNumber() { return object; }
	virtual int Getmouse_MRD(){ return Mouse_MRD; }
	virtual int Getmouse_MR_Effect(){ return Mouse_MR_Effect;}
	/////////////////////// 0611 lkh 추가 ///////////////////////
	virtual int GetDamage() { return Damage; }
	virtual int GetAR_or_DH() { return  ArorDH; }		// 0915 YGI
	virtual int GetImunityCure2(){ return Imunity_Cure_2; }//020610 lsw
	virtual int GetImunityCure3(){ return Imunity_Cure_3; }//020610 lsw
	virtual int GetLegendAble(){ return Need2_race; }//021113 lsw
	virtual int GetLegendGroup(){ return Need2_min_age; }//021113 lsw
	virtual int GetNeedDualStep(){ return Need2_max_age; }//021113 lsw

	virtual int GetClassWarriorAble(){ return Class_Warrior;}
	virtual int GetClassArcherAble(){ return Class_Archer;}
	virtual int GetClassThiefAble(){ return Class_Thief;}
	virtual int GetClassWizardAble(){ return Class_Wizard;}
	virtual int GetClassClericAble(){ return Class_Cleric;}
	virtual int GetNeed3Lv(){ return Need3_lv; }//0201014 lsw
};

class CItem_Disposable : public CItem
{
//protected :
public :
	int Item_id   ;
	char Name[ITEM_NAME_MAX] ;
	char Han_Name[ITEM_NAME_MAX] ;
	int lv   ;
	int Image_Number   ;
	int Need3_lv   ;
	int Need3_str   ;
	int Need3_dex   ;
	int Need3_con   ;
	int Need3_wis   ;
	int Need3_int   ;
	int Need3_cha   ;
	int Need3_end   ;
	int Need3_Movp   ;
	int Need3_mor   ;
	int Need3_luc   ;
	int Need3_ws   ;
	int Need3_ps   ;
	int Need3_fame   ;
	int Need3_rank   ;
	int Need3_gender   ;
	int Need3_race   ;
	int Need3_min_age   ;
	int Need3_max_age   ;
	int Class_Warrior   ;
	int Class_Archer   ;
	int Class_Thief   ;
	int Class_Wizard   ;
	int Class_Cleric   ;
	int shape_woman   ;
	int shape_man   ;
	int Skill_Ability   ;
	int Damage   ;
	int Hit_rate   ;
	int fire_speed   ;
	int nRange;	// CSD-031101
	int damage_rage   ;
	int Through   ;
	int nWeaponSpeed; // CSD-031101
	int effect_num   ;
	int weight   ;
	int object   ;
	int wear_able   ;
	int base_value   ;
	int base_store   ;
	int store_const   ;
	int store_plus_const   ;
	int Mouse_MR;
	int Mouse_MRD;
	int Mouse_MR_Effect;
	int Mouse_Mr_Wave;
	int Quantity   ;
	int Item_kind   ;



public:
	virtual int getAllData(void) ;
	virtual void print_data(void) {
		printf("\n%d %s %s %d", Item_id, Name, Han_Name, lv) ;
	}
	virtual int is_equipable(int a, t_chr_status_info *b, POS c, POS d) ;
	virtual int GetValue() 
	{
		int value;
		if( Quantity ) value =  (base_value -( m_Much * store_const / 100000) ) /*/ Quantity */;
		else value =  (base_value -( m_Much * store_const / 100000) ) ;
		return value; 
	}


	virtual int EquipItem(CHARLIST *ch);			// 010605_2 YGI
	virtual int ReleaseItem(CHARLIST *ch );			// 0206 YGI
	virtual int GetRbutton() { return Mouse_MR; }
	virtual int GetItemKind() { return Item_kind; }
	virtual int GetLevel() { return lv; }
	virtual int GetWearAble() { return wear_able; }
	virtual int GetSkill_Ability(){ return  Skill_Ability; }
	virtual int Getrange() { return nRange; }
	virtual int GetWeight() { return weight; }
	virtual int GetItemShow( int is_man ) { return is_man? shape_man : shape_woman; }
	virtual int GetItemImage() { return Image_Number; }
	virtual char *GetHanName() { return Han_Name; }
	virtual int GetObjectNumber() { return object; }
	virtual int Getmouse_MRD(){ return Mouse_MRD;}
	virtual int Getmouse_MR_Effect(){ return Mouse_MR_Effect;}
	virtual int GetItemQuantity(){ return Quantity; }
	//////////////// 0612 LKH 추가 ///////////////////
	virtual int GetDamage		() { return  Damage			; }
	virtual int GetNeed3Lv(){ return Need3_lv; }//0201014 lsw
};


class CItem_Armor : public CItem
{
//protected :
public :
	int Item_id   ;
	char Name[ITEM_NAME_MAX]  ;
	char Han_Name[ITEM_NAME_MAX]  ;
	int lv   ;
	int Image_Number   ;
	int Repairable   ;
	int Repair_min   ;
	int Repair_max   ;
	int Repair_Skill2   ;
	int Repair_Skill2_min   ;
	int Repair_Res1   ;
	int Amt_of_Repair_Res1   ;
	int Repair_Res2   ;
	int Amt_of_Repair_Res2   ;
	int Repair_Res3   ;
	int Amt_of_Repair_Res3   ;
	int Repair_Tool   ;
	int Amt_of_abrasion   ;
	int Need2_lv   ;
	int Need2_str   ;
	int Need2_dex   ;
	int Need2_con   ;
	int Need2_wis   ;
	int Need2_int   ;
	int Need2_cha   ;
	int Need2_end   ;
	int Need2_Movp   ;
	int Need2_mor   ;
	int Need2_luc   ;
	int Need2_ws   ;
	int Need2_ps   ;
	int Need2_fame   ;
	int Need2_rank   ;
	int Need2_gender   ;
	int Need2_race   ;
	int Need2_min_age   ;
	int Need2_max_age   ;
	int Need3_lv   ;
	int Need3_str   ;
	int Need3_dex   ;
	int Need3_con   ;
	int Need3_wis   ;
	int Need3_int   ;
	int Need3_cha   ;
	int Need3_end   ;
	int Need3_Movp   ;
	int Need3_mor   ;
	int Need3_luc   ;
	int Need3_ws   ;
	int Need3_ps   ;
	int Need3_fame   ;
	int Need3_rank   ;
	int Need3_gender   ;
	int Need3_race   ;
	int Need3_min_age   ;
	int Need3_max_age   ;
	int Class_Warrior   ;
	int Class_Archer   ;
	int Class_Thief   ;
	int Class_Wizard   ;
	int Class_Cleric   ;
	int Change_str   ;
	int Change_dex   ;
	int Change_con   ;
	int Change_wis   ;
	int Change_int   ;
	int Change_cha   ;
	int Change_end   ;
	int Change_Movp   ;
	int Change_mor   ;
	int Change_luc   ;
	int Change_ws   ;
	int Change_ps   ;
	int Change_mag   ;
	int Change_dev   ;
	int casting_Time   ;
	int Change_Life_max   ;
	int Change_mana_max   ;
	int Change_Divine_Max   ;
	int Change_health_max   ;
	int Increase_Poisoning   ;
	int Increase_Stone   ;
	int Increase_Magic   ;
	int Increase_Fire   ;
	int Increase_Ice   ;
	int Increase_lightning   ;
	int imunity_Cure_1   ;
	int Imunity_Cure_2   ;
	int Imunity_Cure_3   ;
	int Imunity_Cure_4   ;
	int Imunity_Cure_5   ;
	int Imunity_Cure_6   ;
	int SkillEffect   ;
	int shape_woman   ;
	int shape_man   ;
	int Skill_Ability   ;
	int Damage   ;
	int Hit_rate   ;
	int Defense_rate   ;
	int AR_or_DH   ;
	int Defense_power   ;
	int body   ;
	int arms   ;
	int head   ;
	int leg   ;
	int neck   ;
	int hand   ;
	int weight   ;
	int max_duration   ;
	int object   ;
	int wear_able   ;
	int base_value   ;
	int base_store   ;
	int store_const   ;
	int store_plus_const   ;
	int Mouse_MR;
	int Mouse_MRD;
	int Mouse_MR_Effect;
	int Mouse_Mr_Wave;
	int Item_kind   ;
	int kind_of_magic;
	int Incr_damage;

public:
	virtual int getAllData(void) ;
	virtual void print_data(void) {
		printf("\n%d %s %s %d", Item_id, Name, Han_Name, lv) ;
	}
	virtual int is_equipable(int a, t_chr_status_info *b, POS c, POS d) ;
	virtual int GetValue() 
	{
							int value;
							value =  base_value -( m_Much * store_const / 100000);
							return value; 
	}
	virtual int GetDuration	()	{ return max_duration; }
	virtual int EquipItem	(CHARLIST *ch);
	virtual int PlusHp(CHARLIST *ch); // 0720 YGI
//	virtual int CurrEquip(CHARLIST *ch ){ return 0; }	//020303 lsw
	virtual int ReleaseItem(CHARLIST *ch );			// 0206 YGI

//	virtual int GetUseJunk() { return Use_Junk; }
	virtual int GetRbutton() { return Mouse_MR; }
	virtual int GetItemKind() { return Item_kind; }
	virtual int GetKindOfMagic() { return kind_of_magic; }
	virtual int GetIncrDamage() { return Incr_damage; }
	virtual int GetLevel() { return lv; }
	virtual int GetWearAble() { return wear_able; }

	// 1221 KHS
	virtual int Getbody() { return  body; }
	virtual int Getarms() { return  arms; }
	virtual int Gethead() { return  head; }
	virtual int Getleg () { return  leg ; }
	virtual int Getneck() { return  neck; }
	virtual int Gethand() { return  hand; }

	virtual int GetDamage		() { return  Damage			; }
	virtual int GetHit_rate		() { return  Hit_rate		; }
	virtual int GetDefense_rate	() { return  Defense_rate	; }
	virtual int GetAR_or_DH		() { return  AR_or_DH		; }
	virtual int GetDefense_power() { return  Defense_power	; }

	virtual int GetSkill_Ability(){ return  Skill_Ability; }
	virtual int GetRepairAble() { return Repairable; }
	virtual int GetWeight() { return weight; }
	virtual int GetItemShow( int is_man ) { return is_man? shape_man : shape_woman; }
	virtual int GetItemImage() { return Image_Number; }
	virtual char *GetHanName() { return Han_Name; }

  virtual int GetPlusLife() const { return Change_Life_max; }
  virtual int GetPlusMana() const { return Change_mana_max; }
  virtual int GetPlusDivine() const { return Change_Divine_Max; }
  virtual int GetPlusHungry() const { return Change_health_max; }

	virtual int GetObjectNumber() { return object; }
	virtual int Getmouse_MRD(){ return Mouse_MRD;}
	virtual int Getmouse_MR_Effect(){ return Mouse_MR_Effect;}
	virtual int GetImunityCure2(){ return Imunity_Cure_2; }//020610 lsw
	virtual int GetImunityCure3(){ return Imunity_Cure_3; }//020610 lsw
	virtual int GetNeed3Lv(){ return Need3_lv; }//0201014 lsw
	virtual int GetLegendAble(){ return Need2_race; }//021113 lsw
	virtual int GetLegendGroup(){ return Need2_min_age; }//021113 lsw
	virtual int GetNeedDualStep(){ return Need2_max_age; }//021113 lsw
	virtual int GetClassWarriorAble(){ return Class_Warrior;}
	virtual int GetClassArcherAble(){ return Class_Archer;}
	virtual int GetClassThiefAble(){ return Class_Thief;}
	virtual int GetClassWizardAble(){ return Class_Wizard;}
	virtual int GetClassClericAble(){ return Class_Cleric;}
};


class CItem_Accessory : public CItem
{
//protected :
public :
	int Item_id   ;
	char Name[ITEM_NAME_MAX]  ;
	char Han_Name[ITEM_NAME_MAX]  ;
	int lv   ;
	int Image_Number   ;
	int Repairable   ;
	int Repair_min   ;
	int Repair_max   ;
	int Repair_Skill2   ;
	int Repair_Skill2_min   ;
	int Repair_Res1   ;
	int Amt_of_Repair_Res1   ;
	int Repair_Res2   ;
	int Amt_of_Repair_Res2   ;
	int Repair_Res3   ;
	int Amt_of_Repair_Res3   ;
	int Repair_Tool   ;
	int Amt_of_abrasion   ;
	int Need2_lv   ;
	int Need2_str   ;
	int Need2_dex   ;
	int Need2_con   ;
	int Need2_wis   ;
	int Need2_int   ;
	int Need2_cha   ;
	int Need2_end   ;
	int Need2_Movp   ;
	int Need2_mor   ;
	int Need2_luc   ;
	int Need2_ws   ;
	int Need2_ps   ;
	int Need2_fame   ;
	int Need2_rank   ;
	int Need2_gender   ;
	int Need2_race   ;
	int Need2_min_age   ;
	int Need2_max_age   ;
	int Need3_lv   ;
	int Need3_str   ;
	int Need3_dex   ;
	int Need3_con   ;
	int Need3_wis   ;
	int Need3_int   ;
	int Need3_cha   ;
	int Need3_end   ;
	int Need3_Movp   ;
	int Need3_mor   ;
	int Need3_luc   ;
	int Need3_ws   ;
	int Need3_ps   ;
	int Need3_fame   ;
	int Need3_rank   ;
	int Need3_gender   ;
	int Need3_race   ;
	int Need3_min_age   ;
	int Need3_max_age   ;
	int Class_Warrior   ;
	int Class_Archer   ;
	int Class_Thief   ;
	int Class_Wizard   ;
	int Class_Cleric   ;
	int Change_str   ;
	int Change_dex   ;
	int Change_con   ;
	int Change_wis   ;
	int Change_int   ;
	int Change_cha   ;
	int Change_end   ;
	int Change_Movp   ;
	int Change_mor   ;
	int Change_luc   ;
	int Change_ws   ;
	int Change_ps   ;
	int Change_mag   ;
	int Change_dev   ;
	int casting_Time   ;
	int Change_Life_max   ;
	int Change_mana_max   ;
	int Change_Divine_Max   ;
	int Change_health_max   ;
	int Increase_Poisoning   ;
	int Increase_Stone   ;
	int Increase_Magic   ;
	int Increase_Fire   ;
	int Increase_Ice   ;
	int Increase_lightning   ;
	int imunity_Cure_1   ;
	int Imunity_Cure_2   ;
	int Imunity_Cure_3   ;
	int Imunity_Cure_4   ;
	int Imunity_Cure_5   ;
	int Imunity_Cure_6   ;
	int Skill_effect   ;
	int Skill_Ability   ;
	int Damage   ;
	int Hit_rate   ;
	int Defense_rate   ;
	int AR_or_DH   ;
	int Defense_power   ;
	int body   ;
	int arms   ;
	int head   ;
	int leg   ;
	int neck   ;
	int hand   ;
	int JobSkill_Farming   ;
	int JobSkill_Mining   ;
	int JobSkill_Fishing   ;
	int JobSkill_Cutdown   ;
	int JobSkill_Butchering   ;
	int JobSkill_Herb_digging   ;
	int JobSkill_Herding   ;
	int JobSkill_Carpentary   ;
	int JobSkill_Blacksmithy   ;
	int JobSkill_Cooking   ;
	int JobSkill_Tailoring   ;
	int JobSkill_Bowcraft   ;
	int JobSkill_Alchemy   ;
	int JobSkill_Candle   ;
	int JobSkill_repair   ;
	int JobSkill_reutil   ;
	int JobSkill_music   ;
	int JobSkill_detect   ;
	int JobSkill_hide   ;
	int JobSkill_campiing   ;
	int JobSkill_taming   ;
	int comm_animal   ;
	int comm_human   ;
	int comm_orc   ;
	int comm_elf   ;
	int comm_fairy   ;
	int comm_hobbit   ;
	int comm_dwarf   ;
	int comm_dragon   ;
	int animal_lore   ;
	int arms_lore   ;
	int evaluate_int   ;
	int evaluate_wis   ;
	int analomy   ;
	int item_ident   ;
	int taste_ident   ;
	int Appraisal   ;
	int healing   ;
	int spirituality   ;
	int poisoning   ;
	int entrapment   ;
	int scrooling   ;
	int cunning   ;
	int bomb_play   ;
	int tactice   ;
	int crapple   ;
	int sword   ;
	int archey   ;
	int fencing   ;
	int mace   ;
	int pierce   ;
	int whid   ;
	int hurt   ;
	int parrying   ;
	int double_sword   ;
	int double_mace   ;
	int weight   ;
	int max_duration   ;
	int Durationpermin   ;
	int object   ;
	int wear_able   ;
	int base_value   ;
	int base_store   ;
	int store_const   ;
	int store_plus_const   ;
	int Mouse_MR;
	int Mouse_MRD;
	int Mouse_MR_Effect;
	int Mouse_Mr_Wave;
	int Item_kind   ;
	int kind_of_magic;
	int Incr_damage;

public:
	virtual int getAllData(void) ;
	virtual void print_data(void) {
		printf("\n%d %s %s %d", Item_id, Name, Han_Name, lv) ;
	}
	virtual int is_equipable(int a, t_chr_status_info *b, POS c, POS d) ;
	virtual int GetValue() 
	{
							int value;
							value =  base_value -( m_Much * store_const / 100000);
							return value; 
	}
	virtual int GetDuration() { return max_duration; }
	virtual int EquipItem(CHARLIST *ch);
	virtual int PlusHp(CHARLIST *ch); // 0720 YGI
//	virtual int CurrEquip(CHARLIST *ch ){ return 0; }	//020303 lsw
	virtual int ReleaseItem(CHARLIST *ch );			// 0206 YGI

//	virtual int GetUseJunk() { return Use_Junk; }
	virtual int GetRbutton() { return Mouse_MR; }
	virtual int GetItemKind() { return Item_kind; }
	virtual int GetKindOfMagic() { return kind_of_magic; }
	virtual int GetIncrDamage() { return Incr_damage; }

	virtual int GetLevel() { return lv; }
	virtual int GetWearAble() { return wear_able; }

	// 1221 KHS
	virtual int Getbody() { return  body; }
	virtual int Getarms() { return  arms; }
	virtual int Gethead() { return  head; }
	virtual int Getleg () { return  leg ; }
	virtual int Getneck() { return  neck; }
	virtual int Gethand() { return  hand; }

	virtual int GetDamage		() { return  Damage			; }
	virtual int GetHit_rate		() { return  Hit_rate		; }
	virtual int GetDefense_rate	() { return  Defense_rate	; }
	virtual int GetAR_or_DH		() { return  AR_or_DH		; }
	virtual int GetDefense_power() { return  Defense_power	; }

	virtual int GetSkill_Ability(){ return  Skill_Ability; }
	virtual int GetRepairAble() { return Repairable; }
	virtual int GetWeight() { return weight; }
	virtual int GetItemImage() { return Image_Number; }
	virtual char *GetHanName() { return Han_Name; }

  virtual int GetPlusLife() const { return Change_Life_max; }
  virtual int GetPlusMana() const { return Change_mana_max; }
  virtual int GetPlusDivine() const { return Change_Divine_Max; }
  virtual int GetPlusHungry() const { return Change_health_max; }

	virtual int GetObjectNumber() { return object; }
	virtual int Getmouse_MRD(){ return Mouse_MRD;}
	virtual int Getmouse_MR_Effect(){ return Mouse_MR_Effect;}
	virtual int GetImunityCure2(){ return Imunity_Cure_2; }//020610 lsw
	virtual int GetImunityCure3(){ return Imunity_Cure_3; }//020610 lsw
	virtual int GetNeed3Lv(){ return Need3_lv; }//0201014 lsw
	virtual int GetLegendAble(){ return Need2_race; }//021113 lsw
	virtual int GetLegendGroup(){ return Need2_min_age; }//021113 lsw
	virtual int GetNeedDualStep(){ return Need2_max_age; }//021113 lsw
	virtual int GetClassWarriorAble(){ return Class_Warrior;}
	virtual int GetClassArcherAble(){ return Class_Archer;}
	virtual int GetClassThiefAble(){ return Class_Thief;}
	virtual int GetClassWizardAble(){ return Class_Wizard;}
	virtual int GetClassClericAble(){ return Class_Cleric;}
};




class CItem_Etc : public CItem
{
// protected :
public :
	int Item_id;
	char Name[ITEM_NAME_MAX];
	char Han_Name[ITEM_NAME_MAX];
	int lv;
	int Image_Number;
	int Repairable;
	int Repair_min;
	int Repair_max;
	int Repair_Skill2;
	int Repair_Skill2_min;
	int Repair_Res1;
	int Amt_of_Repair_Res1;
	int Repair_Res2;
	int Amt_of_Repair_Res2;
	int Repair_Res3;
	int Amt_of_Repair_Res3;
	int Repair_Tool;
	int Amt_of_abrasion;
	int Need2_lv;
	int Need2_str;
	int Need2_dex;
	int Need2_con;
	int Need2_wis;
	int Need2_int;
	int Need2_cha;
	int Need2_end;
	int Need2_Movp;
	int Need2_mor;
	int Need2_luc;
	int Need2_ws;
	int Need2_ps;
	int Need2_Magery;
	int Need2_Orison;
	int Need2_fame;
	int Need2_rank;
	int Need2_gender;
	int Need2_race;
	int Need2_min_age;
	int Need2_max_age;
	int Need3_lv;
	int Need3_str;
	int Need3_dex;
	int Need3_con;
	int Need3_wis;
	int Need3_int;
	int Need3_cha;
	int Need3_end;
	int Need3_Movp;
	int Need3_mor;
	int Need3_luc;
	int Need3_ws;
	int Need3_ps;
	int Need3_fame;
	int Need3_rank;
	int Need3_gender;
	int Need3_race;
	int Need3_min_age;
	int Need3_max_age;
	int Class_Warrior;
	int Class_Archer;
	int Class_Thief;
	int Class_Wizard;
	int Class_Cleric;
	int Change_str;
	int Change_dex;
	int Change_con;
	int Change_wis;
	int Change_int;
	int Change_cha;
	int Change_end;
	int Change_mor;
	int Change_luc;
	int Change_ws;
	int Change_ps;
	int Change_mag;
	int Change_dev;
	int casting_Time;
	int Change_Life_max;
	int Change_mana_max;
	int Change_Divine_Max;
	int Change_health_max;
	int Increase_Poisoning;
	int Increase_Stone;
	int Increase_Magic;
	int Increase_Fire;
	int Increase_Ice;
	int Increase_lightning;
	int imunity_Cure_1;
	int Imunity_Cure_2;
	int Imunity_Cure_3;
	int Imunity_Cure_4;
	int Imunity_Cure_5;
	int Imunity_Cure_6;
	int Skill_effect;
	int Substitution_magic;
	int Skill_Ability;
	int Damage;
	int Hit_rate;
	int Defense_rate;
	int AR_or_DH;
	int Defense_power;
	int body;
	int arms;
	int head;
	int leg;
	int neck;
	int hand;
	int JobSkill_Farming;
	int JobSkill_Mining;
	int JobSkill_Fishing;
	int JobSkill_Cutdown;
	int JobSkill_Butchering;
	int JobSkill_Herb_digging;
	int JobSkill_Herding;
	int JobSkill_Carpentary;
	int JobSkill_Blacksmithy;
	int JobSkill_Cooking;
	int JobSkill_Tailoring;
	int JobSkill_Bowcraft;
	int JobSkill_Alchemy;
	int JobSkill_Candle;
	int JobSkill_repair;
	int JobSkill_reutil;
	int JobSkill_music;
	int JobSkill_detect;
	int JobSkill_hide;
	int JobSkill_campiing;
	int JobSkill_taming;
	int comm_animal;
	int comm_human;
	int comm_orc;
	int comm_elf;
	int comm_fairy;
	int comm_hobbit;
	int comm_dwarf;
	int comm_dragon;
	int animal_lore;
	int arms_lore;
	int evaluate_int;
	int evaluate_wis;
	int analomy;
	int item_ident;
	int taste_ident;
	int Appraisal;
	int healing;
	int spirituality;
	int poisoning;
	int entrapment;
	int scrooling;
	int cunning;
	int bomb_play;
	int tactice;
	int crapple;
	int sword;
	int archey;
	int fencing;
	int mace;
	int pierce;
	int whid;
	int hurt;
	int parrying;
	int double_sword;
	int double_mace;
	int change_music;
	int change_detect;
	int change_hide;
	int change_amping;
	int change_taming;
	int weight;
	int max_duration;
	int object;
	int wear_able;
	int base_value;
	int base_store;
	int store_const;
	int store_plus_const;
  	int Item_kind;
	int Mouse_MR;
	int Mouse_MRD;
	int Mouse_MR_Effect;
	int Mouse_Mr_Wave;
	int Use_Junk;
	int kind_of_magic;
	int Incr_damage;




public:

	

	virtual int getAllData(void) ;
	virtual void print_data(void) {
		printf("\n%d %s %s %d", Item_id, Name, Han_Name, lv) ;
	}
	virtual int is_equipable(int a, t_chr_status_info *b, POS c, POS d) ;
	virtual int GetValue() 
	{
							int value;
							value =  base_value -( m_Much * store_const / 100000);
							return value; 
	}
	virtual int EatItem( CHARLIST *ch);
	virtual int GetDuration() { return max_duration; }
	virtual int EquipItem(CHARLIST *ch);
	virtual int PlusHp(CHARLIST *ch); // 0720 YGI
//	virtual int CurrEquip(CHARLIST *ch ){ return 0; }	//020303 lsw
	virtual int ReleaseItem(CHARLIST *ch );			// 0206 YGI

	virtual int GetUseJunk() { return Use_Junk; }
	virtual int GetRbutton() { return Mouse_MR; }
	virtual int GetItemKind() { return Item_kind; }
	virtual int GetKindOfMagic() { return kind_of_magic; }
	virtual int GetIncrDamage() { return Incr_damage; }

	virtual int GetLevel() { return lv; }
	virtual int GetWearAble() { return wear_able; }
	// 1221 KHS
	virtual int Getbody() { return  body; }
	virtual int Getarms() { return  arms; }
	virtual int Gethead() { return  head; }
	virtual int Getleg () { return  leg ; }
	virtual int Getneck() { return  neck; }
	virtual int Gethand() { return  hand; }

	virtual int GetDamage		() { return  Damage			; }
	virtual int GetHit_rate		() { return  Hit_rate		; }
	virtual int GetDefense_rate	() { return  Defense_rate	; }
	virtual int GetAR_or_DH		() { return  AR_or_DH		; }
	virtual int GetDefense_power() { return  Defense_power	; }

	virtual int GetSkill_Ability(){ return  Skill_Ability; }
	virtual int GetRepairAble() { return Repairable; }
	virtual int GetWeight() { return weight; }
	virtual int GetItemImage() { return Image_Number; }
	virtual char *GetHanName() { return Han_Name; }

	virtual int GetPlusLife() const { return Change_Life_max; }
	virtual int GetPlusMana() const { return Change_mana_max; }
	virtual int GetPlusDivine() const { return Change_Divine_Max; }
	virtual int GetPlusHungry() const { return Change_health_max; }

	virtual int GetObjectNumber() { return object; }
	virtual int Getmouse_MRD(){ return Mouse_MRD;}
	virtual int Getmouse_MR_Effect(){ return Mouse_MR_Effect;}
	virtual int GetImunityCure2(){ return Imunity_Cure_2; }//020610 lsw
	virtual int GetImunityCure3(){ return Imunity_Cure_3; }//020610 lsw
	virtual int GetNeed3Lv(){ return Need3_lv; }//0201014 lsw
	virtual int GetLegendAble(){ return Need2_race; }//021113 lsw
	virtual int GetLegendGroup(){ return Need2_min_age; }//021113 lsw
	virtual int GetNeedDualStep(){ return Need2_max_age; }//021113 lsw
	virtual int GetClassWarriorAble(){ return Class_Warrior;}
	virtual int GetClassArcherAble(){ return Class_Archer;}
	virtual int GetClassThiefAble(){ return Class_Thief;}
	virtual int GetClassWizardAble(){ return Class_Wizard;}
	virtual int GetClassClericAble(){ return Class_Cleric;}

	virtual int GetStaticFunctionNo(){ return Amt_of_Repair_Res3;}//030327 lsw
};

struct CItem_List {
	CItem *Item_Info[Num_Of_Item_Type][MAX_ITEM_NUM] ;
	//CItem **Item_type ;
	int nItem[Num_Of_Item_Type] ;

} ;

///////////////////////////////////////
enum ItemPosType
{
	HAND		=0,
	INV			=1,
	EQUIP		=2,
	QUICK		=3,
	BANK		=4,
	BOX			=5,
	MAKE		=6,
	OTHER_CH	=7,
	AUCTION_DB	=8//아이템 위치가 옥션 디비에 있다 021030 lsw
};

#define CHARACTER	124
#define GROUND		125
#define USE			126


#define CC_NORMAL 0

#define OK		1
#define NOT		0	

extern ItemAttr GenerateItem( const int iItemFullNo, const int iCustomType, const DWORD dwValue );
extern ItemAttr GenerateItem( const int iItemFullNo );

extern void SendItemAttrChange( t_connection c[], int cn, int id, int attr );
extern void RecvItemDoorOpenClose( int cn, t_item_door_open_close *p );

extern CItem *ItemUnit( int item_no );//011030 lsw

extern CItem *ItemUnit( ItemAttr item );//021015 lsw
extern CItem *ItemUnit( int type, int item_no );
extern bool SearchInv( ItemAttr inv[3][3][8], int &a, int &b, int &c );
extern void CheckItemTimeElapsed( void );
extern void SendItemFoot_On_Trap0( t_connection c[], int cn, int itemid );
extern void SendItemFoot_On_Trap1( t_connection c[], int cn, int itemid );
extern DWORD CalcDurByUseItem( DWORD dur, int luck );		// 아이템 한번 사용에 따른 내구력 계산
extern int SearchInv_invert( ItemAttr inv[3][3][8], int &a, int &b, int &c );

extern int SearchMoneyItem( ItemAttr inv[3][3][8], int &a, int &b, int &c );		// 001030 YGI

extern CItem_Plant			CPlant		[MAX_ITEM_NUM];
extern CItem_Mineral		CMineral	[MAX_ITEM_NUM];
extern CItem_Herb			CHerb		[MAX_ITEM_NUM];
extern CItem_Cook			CCook		[MAX_ITEM_NUM];
extern CItem_Potion			CPotion		[MAX_ITEM_NUM];
extern CItem_Tool			CTool		[MAX_ITEM_NUM];
extern CItem_Weapon			CWeapon		[MAX_ITEM_NUM];
extern CItem_Disposable		CDisposable	[MAX_ITEM_NUM];
extern CItem_Armor			CArmor		[MAX_ITEM_NUM];
extern CItem_Accessory		CAccessory	[MAX_ITEM_NUM];
extern CItem_Etc			CEtc		[MAX_ITEM_NUM];
extern CItem_List Item_Ref ;

extern int SearchInvEmptyCount( ItemAttr inv[3][3][8]);//021014 lsw
extern int ConvBondMoneyCount(const DWORD dwMoney,const int iaBondIndex = 0);//돈을 넣으면 받을 본드머니와 돈 아이템의 갯수를 리턴// iaBondIndex aBondMoney의 인덱스를 의미
extern int GiveMoney(CHARLIST *ch, const DWORD dwMoney,const int iaBondIndex = 0);// iaBondIndex aBondMoney의 인덱스를 의미
extern int GiveMoneyAble(CHARLIST *ch, const DWORD dwMoney,const int iaBondIndex = 0);//돈을 받을 수 있는 상황인가

#endif// AFX_CITEM_H__3DCF6E55_A881_498E_9E4E_A342F8E62D81__INCLUDED_