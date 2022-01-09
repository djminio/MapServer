// ItemMgr.cpp: implementation of the CItemMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "CItem.h"
#include "ItemMgr.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CItemMgr	ItemMgr;

CItemMgr::CItemMgr()
{
	DeleteItem(NULLITEM);
}

CItemMgr::~CItemMgr()
{

}

CItem* CItemMgr::ConvertItemNoToCItem(const int iItemNo)
{
	const int iType	= 	iItemNo/1000;
	const int iNo	= 	iItemNo%1000;

	if(	(iType<0)	||	(iType>10) )	{return NULL;}
	if(	iNo<=0)							{return NULL;}
	if(	iNo > Item_Ref.nItem[iType])	{return NULL;}

	switch( iType )
	{
		case 	PLANT		:	return &CPlant		[iNo];	break;
		case 	MINERAL		:	return &CMineral	[iNo];	break;
		case 	HERB		:	return &CHerb		[iNo];	break;
		case 	COOK		:	return &CCook		[iNo];	break;
		case 	POTION		:	return &CPotion		[iNo];	break;
		case 	TOOL		:	return &CTool		[iNo];	break;
		case 	WEAPON		:	return &CWeapon		[iNo];	break;
		case 	DISPOSABLE	:	return &CDisposable	[iNo];	break;
		case 	ARMOR		:	return &CArmor		[iNo];	break;
		case 	ACCESSORY	:	return &CAccessory	[iNo];	break;
		case 	ETC			:	return &CEtc		[iNo];	break;
		default				:	return NULL;
	}
	return NULL;
}

bool CItemMgr::SwapItem(ITEMATTR &src,ITEMATTR &target)
{
	ITEMATTR temp;

// 이거는 나중에 넣읍시다..
	if(!CheckFaultItem(src,true))	{return false;}
	if(!CheckFaultItem(target,true)){return false;}

	temp = src;
	src = target;
	target = temp;

	return true;
}

int CItemMgr::GetItemWeight(const ITEMATTR item )
{
	int iDur = 0;
	int iType = item.item_no/1000;

	CItem *t = ConvertItemNoToCItem( item.item_no );
	if( !t ) return 0;

	int iWeight = t->GetWeight();

	if( iType == DISPOSABLE){iDur = t->GetItemQuantity();}
	
	if( t->GetRbutton() == DIVIDE_ITEM )
	{
		iDur= t->GetItemDuration();
		if(iDur){iWeight *= item.attr[IATTR_MUCH] / iDur;}
		else{iWeight *= item.attr[IATTR_MUCH];}
	}
	return iWeight;
}

int CItemMgr::GetItemValue(const ITEMATTR item)
{
	CItem *t = ConvertItemNoToCItem(item.item_no);
	if(!t){return 0;}
	
	int			iMuch	= 0;
	int			iPrice	= 0;
	const int	iType = item.item_no/1000;
	const int	iBase	= t->GetValue();
	
	if( t->GetRbutton()==DIVIDE_ITEM )		// 수량을 의미...
	{
		if( iType == DISPOSABLE ) 
		{
			iMuch = t->GetItemQuantity();	// dispoable일 경우
		}
		else
		{
			iMuch = t->GetItemDuration();		// 10개 단위
		}
		if( !iMuch ){ iMuch = 1;}
		iPrice = (unsigned int)((iBase * .9+.5)*(item.attr[IATTR_MUCH]/(float)iMuch));
		return iPrice;
	}
	
	unsigned short wNow, wMax;
	GetItemDur(item,wNow,wMax);

	unsigned int wDBMax = t->GetDuration();

	if( wMax < wDBMax ) wMax = wDBMax;

	if( wMax )
	{	
		/*
		if( (item->attr[IATTR_LIMIT]-g_curr_time) > 0 )		// 유통기한이 남아 있어야 한다. 이 개념 사라짐 나중을 위해서 주석 처리
		{
			price = (int)( (float)(base*.9)*d_curr/(float)d_max+.5 );
			return price;
		}
		*/
		iPrice = (unsigned int)( (float)(iBase*.9)*wNow/(float)wMax+.5 );		// 임시 
		return iPrice;
	}
	return 0;
}

int	CItemMgr::GetItemDur(const ITEMATTR item, unsigned short &NowDur,unsigned short &MaxDur)
{
	NowDur = 0;
	MaxDur = 0;
	CItem *t = ConvertItemNoToCItem(item.item_no);
	if(!t){return 0;}
	if( t->GetRbutton()==DIVIDE_ITEM ){return 0;}//나누어지는 아이템 이므로 듀레이션이 아니다
	
	NowDur = LOWORD( item.attr[IATTR_DURATION] );
	MaxDur = HIWORD( item.attr[IATTR_DURATION] );
	return 1;
}

int	CItemMgr::GetItemMuch(const ITEMATTR item, int &NowMuch)//나누어 지는 아이템일 경우
{
	NowMuch = 0;
	CItem *t = ConvertItemNoToCItem(item.item_no);
	if(!t){return 0;}
	if( t->GetRbutton()!=DIVIDE_ITEM ){return 0;}//나누어지는 아이템이 아니니까 값을 받지 못함
	NowMuch = item.attr[IATTR_MUCH];
	return 1;
}

int CItemMgr::ChangeItemAttribute(ITEMATTR &TargetItem, const int iType, const int iAttr)
{
	if(!(&TargetItem)){return 0;}//대상의 주소가 옳은지 검사

	switch(iType)
	{
	case IATTR_ITEMNO:		{	TargetItem.item_no				= iAttr;	}	break;
	case IATTR_MUCH:		{	TargetItem.attr[IATTR_MUCH]		= iAttr;	}	break;
	case IATTR_LIMIT:		{	TargetItem.attr[IATTR_LIMIT]	= iAttr;	}	break;
	case IATTR_ATTR:		{	TargetItem.attr[IATTR_ATTR]		= iAttr;	}	break;
	case IATTR_ID:			{	TargetItem.attr[IATTR_RARE_MAIN]= iAttr;	}	break;//0020101 lsw
	case IATTR_RESERVED0:	{	TargetItem.attr[IATTR_RESERVED0]= iAttr;	}	break;
	case IATTR_RESERVED1:	{	TargetItem.attr[IATTR_RESERVED1]= iAttr;	}	break;
	default :				{	return 0;	}									break;
	}
	return CheckFaultItem(TargetItem,true);//바꾸고 나서 이상한 놈은 지워 버린다 지워지면 0 리턴 안지워지면 1 리턴
}

int CItemMgr::ChangeItemAttribute(ITEMATTR &TargetItem, const ITEMATTR SourceItem)
{	
	if(!(&TargetItem)){return 0;}//대상의 주소가 옳은지 검사
	TargetItem = SourceItem;
	return 1;
}

int	CItemMgr::GetItemTactic(const int iItemNo)//없으면 주먹이 기본 택틱
{
	CItem *t = ConvertItemNoToCItem(iItemNo);
	if(!t)	{return TACTICS_Crapple;}
	else	{return t->GetSkill_Ability();}
	return TACTICS_Crapple;
}

int	CItemMgr::GetItemKind(const int iItemNo) 
{
	CItem *t = ConvertItemNoToCItem(iItemNo);

	if(!t)	{return IK_NONE;}
	else	{return t->GetItemKind();}
	
	return IK_NONE;
}

// 031009 CI YGI
int	CItemMgr::GenerateItemLimitNumber( int item_no )
{	//< CSD-031229
	/*
	k_item_limit_number limit_number;
	limit_number.who_make = MapNumber+1;
	limit_number.number = GetLimitNumber( item_no );

	if( limit_number.number == LIMIT_OF_NO_HAVE_LIMIT_ITEM )		// 리미트 번호가 없는거
	{
		limit_number.who_make = 255;
		limit_number.number = g_curr_time_with_out_year;
	}

	int limit;
	memcpy( &limit, &limit_number, sizeof( int ) );
	return limit;
	*/
	return g_curr_time;
}	//> CSD-031229

int	CItemMgr::GenerateItemDuration(const int iItemNo)
{
	int iDuration = 0;

	CItem *t = ConvertItemNoToCItem(iItemNo);
	if( t == NULL ) return 0;

	if( t->GetRbutton() == DIVIDE_ITEM ) //나눠 지는 거는 MAKELONG 안함
	{
		switch (iItemNo/1000)
		{
		case DISPOSABLE	:
			{
				iDuration = t->GetItemQuantity();
			}
			break;
		default			:
			{
				iDuration = t->GetDuration();if(iDuration < 0){iDuration = 1;}
			}
			break;
		}
	}
	else 
	{
		iDuration = MAKELONG( t->GetDuration(), t->GetDuration() );//다른 일반적 아이템은 
	}

	return iDuration;
}

#include "eventmgr.h"	// 040105 YGI 보물

ITEMATTR CItemMgr::GenerateItem(const int iItemNo)
{
	// 031110 YGI
	ITEMATTR	item = {0, };
	DeleteItem(item);

	// 031110 YGI
	if( !iItemNo ) return item;
	CItem *t = ConvertItemNoToCItem(iItemNo);

	if(!t)
	{
		MyLog( 0, "GenerateItem() : Can't generatie Item [%d]", iItemNo );
		return item;
	}
	if(!t->GetItemImage() || !t->GetObjectNumber())
	{
		MyLog( 0, "GenerateItem() : Can't generatie Item [%d] because wrong ObjectNumber", iItemNo );
		return item;
	}//오브젝트 이미지 넘버 없으면 지워짐
	
	item.item_no = iItemNo;

	int iDuration = GenerateItemDuration(iItemNo); 

	if( iDuration )		// dur이 0일경우 생성시키지 않는다.
	{
		item.attr[IATTR_DURATION]	= iDuration;
		item.attr[IATTR_LIMIT]		= GenerateItemLimitNumber( item.item_no );	// 031009 CI YGI
	}
	else 
	{
		DeleteItem(item);//클래스 내부꺼임
		return item;
	}

	if( item.item_no == IN_COINS && item.attr[IATTR_MUCH] <= 5 ) item.item_no = IN_COIN;
	else if( item.item_no == IN_COIN && item.attr[IATTR_MUCH] > 5 ) item.item_no = IN_COINS;

	if( item.item_no == IN_NEW_COINS && item.attr[IATTR_MUCH] <= 5 ) item.item_no = IN_NEW_COIN;			// 010210 YGI
	else if( item.item_no == IN_NEW_COIN && item.attr[IATTR_MUCH] > 5 ) item.item_no = IN_NEW_COINS;
	// 040105 YGI 보물
	if( IsTreasureMapItem( item.item_no ) )
	{
		CTreasureMapItem treasure_attr = {0, };
		treasure_attr.isChecked = 0;
		
		memcpy( &item.attr[IATTR_TREASURE_MAP], &treasure_attr, sizeof( DWORD ) );
	}

	CheckFaultItem(item,true);
	return 	item;
}

ITEMATTR CItemMgr::GenerateItem(const int iItemNo, const int iType , const int iCustomValue)
{
	int iCustomValueResult = 0;
	ITEMATTR  item = GenerateItem(iItemNo);
	CItem *t = ConvertItemNoToCItem(iItemNo);
	
	if(!t){return item;}

	if( iType == IATTR_MUCH)
	{//수량 부분은 커스텀 하게 셋팅 한다//iCustomValueResult 를 변화 시킨걸로
		if(t->GetRbutton() == DIVIDE_ITEM ) {iCustomValueResult = iCustomValue;}
		else								{iCustomValueResult = MAKELONG( iCustomValue, iCustomValue);}
	}
	ChangeItemAttribute(item, iType, iCustomValueResult);
	CheckFaultItem(item,true);
	return item;
}

int	CItemMgr::CheckFaultItem(ITEMATTR &item,bool bDeleteFlag)//020205 lsw
{
	int rareresult = 0;
	CItem *t = ConvertItemNoToCItem(item.item_no);
	WORD ddur=0;
	WORD dmax=0;

	if(!t)							{goto _DELETE_ITEM;}
	if(!t->GetItemImage())			{goto _DELETE_ITEM;}//아이템 이미지 넘버 없으면 지워짐
	if(!t->GetObjectNumber())		{goto _DELETE_ITEM;}//오브젝트 이미지 넘버 없으면 지워짐

	if(!item.item_no)				{goto _DELETE_ITEM;}//아이템 번호 없으면 지워짐
	if(!item.attr[IATTR_DURATION])	{goto _DELETE_ITEM;}//아이템 내구도 없으면 지워짐
	
	if(DIVIDE_ITEM == t->GetRbutton())
	{	//-값을 가진것은 지우자
		if( 0 > ((int)item.attr[IATTR_MUCH]) ){ goto _DELETE_ITEM; }
		if( MAX_DIVIDE_ITEM < (item.attr[IATTR_MUCH]) )//최대 값 보다 많으면 100 만개로 제한
		{
			item.attr[IATTR_MUCH] = MAX_DIVIDE_ITEM;
		}//100만개 보다 많으면
	}
	else
	{
		if(!GetItemDur( item, ddur, dmax ))	{	goto _DELETE_ITEM;	}//내구도를 받을 수 없다?
		if( ddur > dmax)					{	goto _DELETE_ITEM;	}//맥스 보다 현재가 많아?
	}

	if(item.attr[IATTR_RARE_MAIN])	//레어인데
	{
		RareMain *pRare = (RareMain*)&item.attr[IATTR_RARE_MAIN];
		switch(pRare->iHighLevel)
		{
		case H_LV_NO_HIGH_ITEM:
			{//기능아이템도 여기에 들어오니까 조심.  MAX_STATIC_RARE 를 넘는 Item_Fuction 이 있다면 지워질 것입니다.
				if(pRare->soksung1)
				{
					if( MAX_STATIC_RARE < pRare->soksung1)//스태틱 레어  최대치 보다 크다면
					{
						rareresult = 7;
					}
				}
				else
				{
					rareresult = 8;
				}
				if(	!pRare->grade//등급이 없거나
				||	!pRare->soksung1)//속성 메인이 없네..
				{	rareresult = 5;	}//그럼 지워야지
			}break;
		case H_LV_HIGH_ITEM:
			{
				if(pRare->soksung1)
				{
					if( START_HIGH_RARE > pRare->soksung1)//하이레어 시작점 보다 속성이 낮다면
					{
						rareresult = 7;
					}
					if(pRare->soksung2 || pRare->soksung3)//하이아이템인데 멀티 속성이라면
					{
						rareresult = 11;
					}
				}
				else//하이인데 속성이 없다면
				{
					rareresult = 8;
				}
				if(	!pRare->grade//등급이 없거나
				||	!pRare->soksung1)//속성 메인이 없네..
				{	rareresult = 5;	}//그럼 지워야지
			}break;
		case H_LV_LEGEND_ITEM:
			{
				if(	!pRare->soksung2//레전드 그룹 넘버
				&& 	!pRare->soksung3)//레전드 그룹 인덱스 없다
				{	rareresult = 10;	}//그럼 지워야지
			}break;
		default:
			{
				rareresult = 9;
			}break;
		}//switch(pRare->iHighLevel)

		if(rareresult)
		{
			item.attr[IATTR_RARE_MAIN] = 0;//레어 클리어
		}

		switch(item.item_no)
		{
		case IN_COIN:
		case IN_COINS:
		case IN_NEW_COIN:
		case IN_NEW_COINS:
			{
				item.attr[IATTR_RARE_MAIN] = 0;//레어 클리어
			}break;
		default:
			{	
			}break;
		}
	}
	return 1;

_DELETE_ITEM:
	{
		if(	bDeleteFlag)
		{
			DeleteItem(item);
		}
		return 0;
	}
}

int	CItemMgr::CheckAddAbleItem(const ITEMATTR SourceItem,const ITEMATTR TargetItem)
{
	CItem *tsrc		= ConvertItemNoToCItem(SourceItem);
	
	if(!tsrc){return 0;}
	if(	GetItemKind(SourceItem) == IK_MONEY		&&	GetItemKind(TargetItem) == IK_MONEY){return 1;}
	if(	GetItemKind(SourceItem) == IK_NEW_MONEY	&&	GetItemKind(TargetItem) == IK_NEW_MONEY){return 1;}	

	if(SourceItem.item_no != TargetItem.item_no)							{return 0;}
	if(tsrc->GetRbutton() != DIVIDE_ITEM)									{return 0;}
	if(SourceItem.attr[IATTR_RARE_MAIN]!=TargetItem.attr[IATTR_RARE_MAIN])	{return 0;}
	
	return 1;
}

int	CItemMgr::CombineItem(ITEMATTR &SourceItem,ITEMATTR &TargetItem)//아이템 수량 합치기
{
	if(!CheckAddAbleItem(SourceItem,TargetItem)){return 0;}

	ChangeItemAttribute(TargetItem,IATTR_MUCH,TargetItem.attr[IATTR_MUCH]+SourceItem.attr[IATTR_MUCH]);
	DeleteItem(SourceItem);

	return 1;
}


int	CItemMgr::DeleteItem(ITEMATTR &item)
{
	return ::DeleteItem( &item );
}

int CItemMgr::DecItemDur(ITEMATTR &item, const DWORD iValue)//양수로 넣을것 음수로 넣으면 내구도 증가
{
	CItem* t = ConvertItemNoToCItem(item.item_no);
	if(!t){return 0;}

	if(t->GetRbutton() == DIVIDE_ITEM)
	{
		if(item.attr[IATTR_MUCH] <= iValue)//뺄 수량이 적거나 같다.
		{
			item.attr[IATTR_MUCH]  =0;//없어졌으면	
			return 1;
		}//위조건에서 넘어 왔으면 빼도 된다
		item.attr[IATTR_MUCH] -=iValue;
		return 2;// 수량이 변했다
	}
	else
	{
		WORD wNow=0,wMax=0;
		if(!GetItemDur( item, wNow, wMax ))
		{
			return 0;
		}

		WORD dOld = wNow;

		if(wNow <= iValue)//뺄 값이 남은거 보다 많다면
		{
			DeleteItem( item );
			return 1;
		}//빼도 되는 거라면
		else
		{
			wNow -= iValue;
			
			if( wNow <= 0 ){return 0;}
			
			item.attr[IATTR_DURATION] = MAKELONG( wNow, wMax );

			if( wNow/10 == 0 ) // Duration이 0이 되었다.
			{
				return 1;
			}
			if( wNow/10 != dOld/10 )  // 번화된 속성을 PC에게 보내준다.
			{
				return 2;
			}
		}
	}
	return 0;

	
	
}

ItemAttr CItemMgr::GiveLegendItem(const int item_no, const int iDur)
{
	ItemAttr Item= {0,};
	CItem * t = ItemUnit(item_no);
	if(!t){ return Item; }
	
	const int iLegendAble = t->GetLegendAble();
	if(!iLegendAble){return Item;}

	//iDur이 0이면 기본으로 만들어 주고 아닌 경우는 그대로 세팅한다 음수이면 리턴
	if( iDur > 0)		{	Item = GenerateItem( item_no, IATTR_MUCH ,iDur); }
	else if	(iDur==0)	{	Item = GenerateItem( item_no);}
	else if	(iDur<0)	{	return Item;	}

	const int iLegendGroupNo = t->GetLegendGroup();
	if(!iLegendGroupNo){return Item;}
	//s1 --> 노말 , s2 --> 그룹넘버 --> s3-->그룹인덱스
	const int iGroupIndex = RareEM.GetAbleGroupIndex(iLegendGroupNo);
	const int iRareKind = RareEM.GetLegendBasicRareKind(iLegendGroupNo,iGroupIndex,t->GetWearAble());

	const int aSucessRate[6] = {5,10,15,30,40,60};//총합이 160
	const int iNow = rand()%160;//
	
	int iTemp = 0;
	int iResultGrade = 0;
	for(int i = 0; 6 > i; i++)
	{
		iTemp += aSucessRate[i];
		if(iTemp> iNow)
		{
			iResultGrade = i+3;//0~5 이기 때문에
			break;
		}
	}
	MakeRareAttr(Item.attr[IATTR_RARE_MAIN],iResultGrade,iRareKind,iLegendGroupNo,iGroupIndex,H_LV_LEGEND_ITEM,0);
	return Item;
}
//


ItemAttr CItemMgr::GiveRareItem(const int item_no, const int iDur,const int iCallType,const int iTryMulti, 
								const int iGradeMin, const int iGradeMax, RareMain &RareAttr,const int iHighLevel)
{
	ItemAttr Item = {0,};
	memset(&Item,0,sizeof(ItemAttr));

	int iNowGrade=0;
	
	CItem * t = ItemUnit(item_no);
	if(!t){ return Item; }
	
	const int iItemKind = t->GetItemKind();
	
	if( MAX_ITEM_MUTANT_KIND <= iItemKind)	{return Item;}
	
	const int iRareAble = ItemMutantKind[iItemKind].upgrade_type;
	
	//iDur이 0이면 기본으로 만들어 주고 아닌 경우는 그대로 세팅한다 음수이면 리턴
	if( iDur > 0)		{	Item = GenerateItem( item_no, IATTR_MUCH ,iDur); }
	else if	(iDur==0)	{	Item = GenerateItem( item_no);}
	else if	(iDur<0)	{	return Item;	}

	//맥스가 작을 경우는 리턴, 같으면 Min을 넣어주고 차이가 양수면 랜덤
	if(iGradeMax > iGradeMin)
	{
		iNowGrade = iGradeMin+rand()%(iGradeMax-iGradeMin+1);//랜덤 하면 최고값은 안나오니까.+1을 한것으로
	}
	else if( iGradeMax == iGradeMin )
	{
		iNowGrade= iGradeMin;
	}
	else //등급 최대가 최하보다 작은게 어딧냐
	{
		memset(&Item,0,sizeof(ItemAttr)); return Item;
	}
	
	int iRare1 = 0, iRare2 = 0, iRare3 = 0;
	
	if( iRareAble )//레어 가능 일때
	{
		switch( iTryMulti )
		{
		case 1:
			{
				iRare1 = GetAbleRareKind(iCallType,iHighLevel);
			}break;
		case 2:
			{
				iRare1 = GetAbleRareKind(iCallType,iHighLevel);
				iRare2 = GetAbleRareKind(iCallType,iHighLevel);
			}break;
		case 3:
			{
				iRare1 = GetAbleRareKind(iCallType,iHighLevel);
				iRare2 = GetAbleRareKind(iCallType,iHighLevel);
				iRare3 = GetAbleRareKind(iCallType,iHighLevel);
			}break;
		default:
			{
			}break;							
		}
	}
	MakeRareAttr(Item.attr[IATTR_RARE_MAIN] ,iNowGrade,iRare1,iRare2,iRare3,iHighLevel,false);//020314 lsw
	RareAttr = *(RareMain*)(&Item.attr[IATTR_RARE_MAIN]);
/*
	Send_RareItemMakeLog(	cn,	item_no,
							0,0,0,0,
							0,0,0,0,
							item.attr[IATTR_LIMIT],
							item.attr[IATTR_MUCH],
							0,type,kind,//포트
							0,0,0,0,
							3000,0
							);*/
	return Item;
}

int CItemMgr::AbleMakeOnThisMap(const int iMapNation, const int iNo)
{	
	const int iAbleNation  = ItemRare[iNo].iAbleNation;
	switch(iMapNation)
	{
	case NW_BY:
		{
			if((iAbleNation%1000)/100)
			{
				return true;
			}
		}break;
	case NW_ZY:
		{
			if((iAbleNation%100)/10)
			{
				return true;
			}
		}break;
	case NW_YL:
		{
			if((iAbleNation%10)/1)
			{
				return true;
			}
		}break;
	case 0:
		{
			if((iAbleNation%10000)/1000)//중립국
			{
				return true;
			}
		}break;

	default :
		{
			return false;
		}break;
	}
	return false;
}

int CItemMgr::GetAbleRareKind(const int iCallType,const int iHighLevel)
{
	int oklist[MAX_ITEM_RARE+1] = {0,};//기준이되는 리스트 No로들어 있는거.
	int gamble[MAX_ITEM_RARE+1] = {0,};//나올 확률 계산을 위해서
	int iNo = 0;
	int iMax = 0;

	switch( iHighLevel)
	{
	case H_LV_NO_HIGH_ITEM:
		{
			iNo		=0;
			iMax	=MAX_STATIC_RARE;
		}break;
	case H_LV_HIGH_ITEM:
		{
			iNo		= START_HIGH_RARE;
			iMax	= MAX_HIGH_RARE;
		}break;
	
	default:
		{
			return 0;
		}break;
	}
	
	int iList= 0;
	for( ;iNo < iMax ; iNo++)
	{
		if(! (ItemRare[iNo].iMakeAble & iCallType))					{continue;} //만들기가 가능 한가? 
		if(! AbleMakeOnThisMap(MapInfo[MapNumber].nation,iNo))		{continue;} //이국가에서 나오나??

		if( iCallType & RARE_ABLE )
		{
			if( iCallType == RARE_ABLE )
			{
				gamble[iList] = ItemRare[iNo].RaisePro[0];	
			}
			else if	(iCallType & RARE_ABLE_SQUAD)//국가
			{	
				gamble[iList] = ItemRare[iNo].RaisePro[1];	
			}//부대 레어
			else if	(iCallType & RARE_ABLE_GAMBLE_ROOM_4)//겜블4번방 
			{	
				gamble[iList] = ItemRare[iNo].RaisePro[2];	
			}
			else if	(iCallType & RARE_ABLE_GAMBLE_ROOM_5)//겜블5번방
			{	
				gamble[iList] = ItemRare[iNo].RaisePro[3];	
			}
		}

		if( gamble[iList] )
		{
			oklist[iList] = ItemRare[iNo].iNo;// 리스트에 가능 속성 넘버 추가	
			gamble[MAX_ITEM_RARE] += gamble[iList];//확률 토탈 계산
			iList++;
		}
	}				

	if(gamble[MAX_ITEM_RARE]<=0)
	{
		return 0;//이럴일은 거의 없다..아니.. 아예.. 그러나.. 
	}

	int NowRating= rand()%gamble[MAX_ITEM_RARE];
	int NowSok=0;
	int CheckRating=0;
	
	for(NowSok=0 ; NowSok<MAX_ITEM_RARE; NowSok++)
	{
		CheckRating+=gamble[NowSok];
		if(NowRating<CheckRating)
		{
			break;
		}
	}

	return oklist[NowSok];
}

bool CItemMgr::MakeRareAttr(unsigned int &attr3 ,const int iNowGrade,
							const int iSokSung1,const int iSokSung2,const int iSokSung3,
							const int iHighLevel,const bool bIsDynamicRare)const
{
	attr3  = 0;
	if( iNowGrade	>15			||	iNowGrade	<=	0	)	{ return false;}
	if( iSokSung1	>UCHAR_MAX	||	iSokSung1	<=	0	)	{ return false;}
	if( iSokSung2	>UCHAR_MAX	||	iSokSung2	<	0	)	{ return false;}
	if( iSokSung3	>UCHAR_MAX	||	iSokSung3	<	0	)	{ return false;}
	if(	iHighLevel	>7			||	iHighLevel	<	0	)	{ return false;}

	((LPRareMain)&attr3)->grade			= iNowGrade	;
	((LPRareMain)&attr3)->soksung1		= iSokSung1	;
	((LPRareMain)&attr3)->soksung2		= iSokSung2	;
	((LPRareMain)&attr3)->soksung3		= iSokSung3	;
	((LPRareMain)&attr3)->iHighLevel	= iHighLevel;
	((LPRareMain)&attr3)->IsDynamicRare = bIsDynamicRare;
	return true;
}

// 021117 YGI
int CItemMgr::MakeDefaultRareItem( ItemAttr *item, const int iItemNo, const int iSok1, const int iGrade, const int iTryMulti)
{
	int iHighLv = 0;
	bool bIsDynamic = false;

	CItem *t = ::ItemUnit( iItemNo );
	if( !t ) {	return -1;	}

	*item = GenerateItem( iItemNo );
	if( !item->item_no ){ return -1;}		// 아이템 번호가 이상하다.

	if( iSok1 >= START_HIGH_RARE )
	{
		iHighLv = H_LV_HIGH_ITEM;
	}
	else
	{
		if( t->GetRbutton() == USE_ITEM )		// 다이나믹 아이템...
		{
			bIsDynamic = true;
			iHighLv = H_LV_NO_HIGH_ITEM;
		}
	}

	int iSok2=0, iSok3=0;
	if(iSok1)
	{
		switch( iTryMulti )
		{
		case 0 :
		case 1 :
			{
			}break;
		case 2:
			{
				iSok2 = iSok1;
			}break;
		case 3 :
			{
				iSok3 = iSok2 = iSok1;
			}break;
		}

		if(H_LV_HIGH_ITEM == iHighLv){	iSok3 = iSok2 =0;}

		return (MakeRareAttr( item->attr[IATTR_RARE_MAIN], iGrade, iSok1, iSok2, iSok3, iHighLv, bIsDynamic )?1:0);
	}
	return 1;
}

int CItemMgr::GetSkillPointByItem(const ITEMATTR &item) const 
{
	CItem *t = ItemUnit(item);
	
	if(!t){return 0;}
	
	const int iLv = t->GetLevel();
	if( 0 >= iLv)
	{
		return 0;
	}
	return (iLv * 3 /10 +1);
}

int CItemMgr::GetStaticFunctionNo(const ITEMATTR &item)
{
	CItem *t = ConvertItemNoToCItem(item.item_no);
	return ((t)?t->GetStaticFunctionNo():0);
}

// 031009 CI YGI
//////////////////////////////////////////////////////////////////////////
// CItemLimitMgr member functions
int	CItemLimitMgr::GetLimitNumber( int item_no )
{
	if( !IsCanHaveLimitItem( item_no ) )
	{
		return LIMIT_OF_NO_HAVE_LIMIT_ITEM;
	}

	m_nItemLimitCount++;
	//char str_number[50];
	//sprintf( str_number, "%d", m_nItemLimitCount );
	//WritePrivateProfileString( "option","limit", str_number, MAP_SERVER_INI_ );

	// 지금까지의 리미트 번호를 DB로 보낸다.
	t_packet packet;
	packet.h.header.type = CMD_WRITE_LIMIT_NUMBER;
		packet.u.kein.save_limit_number.port = ::GetOwnPort();
		packet.u.kein.save_limit_number.limit_number = m_nItemLimitCount;
	packet.h.header.size = sizeof( k_save_limit_number );
	QueuePacket( connections, DB_DEMON, &packet, 1 );

	return m_nItemLimitCount;
}

int	CItemLimitMgr::LoadHaveLimitItem()
{ 
	m_mapHaveLimitItem.clear();

	HSTMT hStmt = NULL;
	SQLAllocStmt(hDBC, &hStmt);

	RETCODE	retCode = SQLExecDirect(hStmt, (UCHAR*)"SELECT * FROM ItemList_HaveLimit", SQL_NTS);	
	if(retCode != SQL_SUCCESS_WITH_INFO && retCode != SQL_SUCCESS) 
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return false;
	}
	
	SDWORD cbValue;
	int item_no = 0;
	int item_id= 0;
	int item_imageno= 0;
	char name[20]= {0, };
	while((retCode = SQLFetch(hStmt)) == SQL_SUCCESS)
	{
		SQLGetData(hStmt, 1, SQL_C_SLONG, &item_id, 0, &cbValue);
		SQLGetData(hStmt, 2, SQL_C_CHAR, name, 20, &cbValue);
		SQLGetData(hStmt, 3, SQL_C_SLONG, &item_imageno, 0, &cbValue);
		item_no = ((int)(item_imageno/10000))*100 + item_id;
		m_mapHaveLimitItem.insert( map<int, int>::value_type( item_no, 1 ) );
	}

	SQLFreeStmt(hStmt, SQL_DROP);
	return true;
}
