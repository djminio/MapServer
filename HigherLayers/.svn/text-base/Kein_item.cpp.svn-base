#include "..\stdafx.h"
#include "DefaultHeader.h"
#include "CItem.h"
#include "OP_Magic.h"

extern int GetBoxItemId_SQL( );
extern struct CItem_List Item_Ref ;
extern void getItemIndex(int, int &, int &) ;

////////////////////////////////////////////////////////////////////////////////
//
//	아이템 번호와 타입이 허용된 값을 넘었는지를 판단하는 함수
//
//		파라메터는 타입과 번호를 입력 받고
//		포인터로 리턴한다.	잘 못 됐을 경우 NULL리턴
//
////////////////////////////////////////////////////////////////////////////////
CItem *ItemUnit( ItemAttr item )
{
	int type, num;
	getItemIndex( item.item_no, type, num );
	return ItemUnit( type, num );
}
//011030 lsw >
CItem *ItemUnit( int item_no )
{
	return ItemUnit( item_no/1000, item_no%1000);
}
//011030 lsw <

CItem *ItemUnit( int type, int item_no )
{
	if( (type<0) || (type>10) ) return NULL;
	if( (item_no<=0) || (item_no > Item_Ref.nItem[type]) ) return NULL;

	switch( type )
	{
		case 	PLANT		:	return &CPlant		[item_no];	break;
		case 	MINERAL		:	return &CMineral	[item_no];	break;
		case 	HERB		:	return &CHerb		[item_no];	break;
		case 	COOK		:	return &CCook		[item_no];	break;
		case 	POTION		:	return &CPotion		[item_no];	break;
		case 	TOOL		:	return &CTool		[item_no];	break;
		case 	WEAPON		:	return &CWeapon		[item_no];	break;
		case 	DISPOSABLE	:	return &CDisposable	[item_no];	break;
		case 	ARMOR		:	return &CArmor		[item_no];	break;
		case 	ACCESSORY	:	return &CAccessory	[item_no];	break;
		case 	ETC			:	return &CEtc		[item_no];	break;
	}
	return NULL;
//	return Item_Ref.Item_Info[type][item_no];
}

DWORD CreateItemDuration( int type, int itemno )			// 0620 YGI
{
	static int bonus = 0;
	
	CItem *t;

	t = ItemUnit( type, itemno );
	if( t == NULL ) return 0;

	DWORD basic_dur;
	
	 if( type == DISPOSABLE )
		basic_dur = t->GetItemQuantity();
	else if( t->GetRbutton() == DIVIDE_ITEM ) 
	{
		basic_dur = t->GetDuration();
		if( basic_dur == -1 ) basic_dur = 1;
	}
	else basic_dur = MAKELONG( t->GetDuration(), t->GetDuration() );

	return basic_dur;
}

ItemAttr GenerateItem( const int iItemFullNo, const int iCustomType, const DWORD dwValue )
{
	return ItemMgr.GenerateItem(iItemFullNo,iCustomType,dwValue);
}

ItemAttr GenerateItem( const int iItemFullNo )
{
	return ItemMgr.GenerateItem(iItemFullNo);
}

// 010109 YGI
int DeleteItem( ItemAttr *item )
{
	int type, num;
	if( !item ) return 0;		// NULL일 경우 
	getItemIndex( item->item_no, type, num );

	CItem *t = ItemUnit( type, num );
	if( !t ) 
	{
		memset( item, 0, sizeof( ItemAttr ) );
		return 0;
	}

	if( t->GetItemKind() == IK_BOX ) 
		SendDeleteBox( item->attr[IATTR_ID] );
	t->ThisMapValue--;

	memset( item, 0, sizeof( ItemAttr ) );
	return 1;
}

int GetWeight( ItemAttr item )
{
	int type, num;
	getItemIndex( item.item_no, type, num );

	CItem *t = ItemUnit( type, num );
	if( !t ) return 1;

	int weight = t->GetWeight();

	if( type == DISPOSABLE)
	{
		int dur = t->GetItemQuantity();
	}
	if( t->GetRbutton() == DIVIDE_ITEM )
	{
		int dur = t->GetItemDuration();
		if( dur > 0 ) 
			weight *= item.attr[IATTR_MUCH] / dur;
		else weight *= item.attr[IATTR_MUCH];
	}

	return weight;
}

int GetTotalItemWeight( CHARLIST *ch )
{
	int weight = 0;
	int a, b, c;
	for(a=0;a<3;a++)
		for(b=0;b<3;b++)
			for(c=0;c<8;c++)
				if(ch->inv[a][b][c].item_no) 
					weight += GetWeight( ch->inv[a][b][c] );
	for(a=0;a<6;a++)
		if(ch->quick[a].item_no)	weight += GetWeight(ch->quick[a]);
	for(a=0;a<8;a++)
		if(ch->equip[a].item_no)	weight += GetWeight(ch->equip[a]);
	if( ch->handheld.item_no ) weight += GetWeight( ch->handheld );

	return weight;
}



/////////////////////////////////////////////////
// 장착 이미지를 가져온다
/////////////////////////////////////////////////
int GetItemView( int item_no, int is_man )
{
	CItem *t;
	int type, num;
	getItemIndex( item_no, type, num );
	
	t = ItemUnit( type, num );
	if( !t ) return 0;
	else return t->GetItemShow( is_man );
}
/////////////////////////////////// //0101 //////////////////////////////////////



//////////////////////////// Item POS Set /////////////////////////////////
void SetItemPos(int type, POS* IP_set)
{
	IP_set->type = (char)type;
	IP_set->p1 = 0;
	IP_set->p2 = 0;
	IP_set->p3 = 0;
}

void SetItemPos(int type, int p, POS* IP_set)
{
	IP_set->type = (char)type;
	IP_set->p1 = 0;
	IP_set->p2 = 0;
	IP_set->p3 = (short int)p;
}

void SetItemPos(int type, int p1, int p2, int p3, POS* IP_set)
{
	IP_set->type = ( char )type;
	IP_set->p1 = (short int)p1;
	IP_set->p2 = (short int)p2;
	IP_set->p3 = (short int)p3;
}

DWORD GetMoneyByItem( CHARLIST *ch )
{
	return (LocalMgr.IsChangeMoney())?GetNewMoneyByItem(ch):GetMoneyByOldItem(ch);
}

DWORD GetNewMoneyByItem( CHARLIST *ch )		// 인벤에 있는 돈만
{
	DWORD money=0;

	int type, num;
	CItem *t;

	int weight = 0;
	for( int i=0; i<3; i++ )
		for( int j=0; j<3; j++ )
			for( int k=0; k<8; k++ )
			{
				ItemAttr &item = ch->inv[i][j][k];
				if( item.item_no )
				{
					weight += GetWeight( item );
					getItemIndex( item.item_no, type, num );
					t = ItemUnit( type, num );
					if(t && t->GetItemKind() == IK_NEW_MONEY )
					{
						if( item.item_no == IN_NEW_COIN && item.attr[IATTR_MUCH] > 5)	// 동전
						{
							item.item_no = IN_NEW_COINS;		// 돈 더미로 바꿈
						}
						else if( item.item_no == IN_NEW_COINS && item.attr[IATTR_MUCH] <= 5 ) // 동전 더미
						{
							item.item_no = IN_NEW_COIN;
						}
						money += item.attr[IATTR_MUCH];
					}
				}
			}
	ch->itemweight = weight;
	return money;
}

DWORD GetMoneyByOldItem( CHARLIST *ch )
{
	DWORD money=0;

	int type, num;
	CItem *t;

	int weight = 0;
	for( int i=0; i<3; i++ )
		for( int j=0; j<3; j++ )
			for( int k=0; k<8; k++ )
			{
				ItemAttr &item = ch->inv[i][j][k];
				if( item.item_no )
				{
					weight += GetWeight( item );
					getItemIndex( item.item_no, type, num );
					t = ItemUnit( type, num );
					if(t && t->GetItemKind() == IK_MONEY )
					{
						if( item.item_no == IN_COIN && item.attr[IATTR_MUCH] > 5)	// 동전
						{
							item.item_no = IN_COINS;		// 돈 더미로 바꿈
						}
						else if( item.item_no == IN_COINS && item.attr[IATTR_MUCH] <= 5 ) // 동전 더미
						{
							item.item_no = IN_COIN;
						}
						money += item.attr[IATTR_MUCH];
					}
				}
			}
	ch->itemweight = weight;
	return money;
}

/////////////////////////////////////////////////////////////////////////
//	캐릭터가 가지고 있는 아이템 멥에 떨구기
///////////////////////////////////////////////////////////////////////
int DropItem( int x, int y, ItemAttr *item )
{
	return AddItemList( item->item_no, item->attr, 0, x, y, 0, 0 );
}


/////////////////////////////////////////////////////////////////////
//
// 돈 아이템에 관련한 메소드들 ( 돈 빼고 더하기 함수 )
//
////////////////////////////////////////////////////////////////////

// 010210 YGI
int SubtractOldMoney( DWORD money, CHARLIST *ch )
{
	CHARLIST *check_ch = CheckServerId( ch->GetServerID());
	if( check_ch != ch ) return -1;
	if( !money ) return 1;

	int a, b, c;
	ItemAttr *item = SearchItemOfInvByKind( IK_MONEY, ch, a, b, c );
	if( !item ) return -1;		// 돈 아이템이 하나도 없음. -->SCharacterData.nMoney = 0;		==> 문제 있음...

	POS pos;
	SetItemPos( INV, a, b, c, &pos);
	if( item->attr[IATTR_MUCH] <= money )
	{
		money -= item->attr[IATTR_MUCH];
		// 020428 YGI acer
		SendDeleteItem( item, &pos, ch );
		SubtractOldMoney( money, ch );
	}
	else 
	{
		item->attr[IATTR_MUCH] -= money;
		SendChangeDurationByKein( &pos, item->attr[IATTR_MUCH], ch->GetServerID());
		return 1;
	}
	return 1;
}


// 010109 YGI
int SubtractMoney( DWORD money, CHARLIST *ch, int type )
{
	CHARLIST *check_ch = CheckServerId( ch->GetServerID());
	if( check_ch != ch ) return -1;

	if(ch->Money < money ) return -1;	// 돈이 부족함
	if( !money ) return 1;

	int a = 0, b = 0, c = 0;
	ItemAttr *item  = NULL;
	if(LocalMgr.IsChangeMoney())//030102 lsw
	{
		item = SearchItemOfInvByKind( IK_NEW_MONEY, ch, a, b, c );
	}
	else
	{
		item = SearchItemOfInvByKind( IK_MONEY, ch, a, b, c );
	}
	if( !item ) return -1;		// 돈 아이템이 하나도 없음. -->SCharacterData.nMoney = 0;		==> 문제 있음...

	POS pos;
	SetItemPos( INV, a, b, c, &pos);
	if( item->attr[IATTR_MUCH] <= money )
	{
		money -= item->attr[IATTR_MUCH];
		// 020428 YGI acer
		SendDeleteItem( item, &pos, ch );
		return SubtractMoney( money, ch );
	}
	else 
	{
		item->attr[IATTR_MUCH] -= money;
		SendChangeDurationByKein( &pos, item->attr[IATTR_MUCH], ch->GetServerID());
		return 1;
	}
	return 1;
}
int SubtractMoney( DWORD money, CHARLIST *ch )
{
	int ret = SubtractMoney( money, ch, 1 );	// 재귀 호출을 모두 끝내고
	ch->Money = GetMoneyByItem( ch );			// 돈 계산은 한번만 한다.
	return ret;
}

int SearchItemPosByNumber(ItemAttr *item, int &a, int &b, int &c, CHARLIST *ch)
{
	for(a=0;a<3;a++)
		for(b=0;b<3;b++)
			for(c=0;c<8;c++)
				if(ch->inv[a][b][c].item_no == item->item_no && 
					ch->inv[a][b][c].attr[IATTR_RARE_MAIN] == item->attr[IATTR_RARE_MAIN] )
					return 1;
	return 0;
}


int SearchCoinItem( int &a, int &b, int &c, CHARLIST *ch )		// 010210 YGI
{
	for(a=0;a<3;a++)
		for(b=0;b<3;b++)
			for(c=0;c<8;c++)
			{
				int iItemNo1 = IN_NEW_COIN, iItemNo2 = IN_NEW_COINS;
				if(LocalMgr.IsChangeMoney())//030102 lsw
				{
					iItemNo1 = IN_NEW_COIN;
					iItemNo2  =IN_NEW_COINS;
				}
				else
				{
					iItemNo1 = IN_COIN;
					iItemNo2  =IN_COINS;
				}
				
				if( ch->inv[a][b][c].item_no == iItemNo1 || ch->inv[a][b][c].item_no == iItemNo2 )
				{
					if( ch->inv[a][b][c].attr[IATTR_MUCH] == MAX_MONEY ) continue;
					return 1;
				}
			}
	return 0;
}

ItemAttr *SearchItemOfInvByKind( int kind, CHARLIST *ch, int &i, int &j, int &k )
{
	int type, num;
	CItem * t;
	for( i=0; i<3; i++ )
		for( j=0; j<3; j++ )
			for( k=0; k<8; k++ )
			{
				ItemAttr &item = ch->inv[i][j][k];
				if( item.item_no )
				{
					getItemIndex( item.item_no, type, num );
					t = ItemUnit( type, num );
					if( t && t->GetItemKind() == kind ) 
					{
						return &item;
					}
				}
			}
	return NULL;
}

ItemAttr *SearchItemOfInvByKind( int kind, CHARLIST *ch )
{
	int a, b, c;
	return SearchItemOfInvByKind( kind, ch, a, b, c );
}

ItemAttr *SearchInv( CHARLIST *ch )
{
	for( int a=0; a<3; a++ )
		for( int b=0; b<3; b++ )
			for( int c=0; c<8; c++ )
			{
				if( !ch->inv[a][b][c].item_no ) return &ch->inv[a][b][c];
			}
	return NULL;
}


int IsEqualPosByPos( POS first, POS second )
{
	if( first.type	!= second.type	|| 
		first.p1	!= second.p1	||
		first.p2	!= second.p2	||
		first.p3	!= second.p3 )		return 0;

	return 1;
}

void CheckItem( short int cn ) // 잘못된 아이템을 가지고 있는지
{	//021126 lsw
	CHARLIST *ch = CheckServerId(cn);
	if( !ch ) {return;}

	POS pos = {0,};

	int a=0, b=0, c=0;
	for( a=0; a<3; a++ )
	for( b=0; b<3; b++ )
	for( c=0; c<8; c++ )
	{
		ItemAttr *pItem = &ch->inv[a][b][c];
		if(pItem->item_no)
		{
			if(!ItemMgr.CheckFaultItem(*pItem,true))//020116 LSW// 10 등급 이상의 레어 및 이상한 아이템 지움 
			{	
				::SetItemPos( INV, a, b, c, &pos );
				::SendServerEachItem( &pos, pItem, cn );
				::SendItemEventLog( pItem, cn, 0, SILT_DELETE_BY_ATTR, 1 ); //YGI acer
			}
		}
	}
	
	for( a=0; a<ITEM_EQUIP_MAX; a++ )
	{
		ItemAttr *pItem = &ch->equip[a];
		if(pItem->item_no)
		{
			if(!ItemMgr.CheckFaultItem(*pItem,true))//020116 LSW// 10 등급 이상의 레어 및 이상한 아이템 지움 
			{
				::SetItemPos( EQUIP, a, &pos );
				::SendServerEachItem( &pos, pItem, cn );
				::SendItemEventLog( pItem, cn, 0, SILT_DELETE_BY_ATTR, 1 ); //YGI acer
			}
		}
	}
	
	for( a=0; a<ITEM_QUICK_MAX ; a++ )
	{
		ItemAttr *pItem = &ch->quick[a];
		if(pItem->item_no)
		{
			//soto-030513 퀵 5번은 심볼 아이템이다.
			if( a >= 5)
			{
				::SetItemPos( QUICK, a, &pos );
				::SendServerEachItem( &pos, pItem, cn );
				::SendItemEventLog( pItem, cn, 0, SILT_DELETE_BY_ATTR, 1 ); //YGI acer
			}
			else
			if(!ItemMgr.CheckFaultItem(*pItem,true))//020116 LSW// 10 등급 이상의 레어 및 이상한 아이템 지움 
			{
				::SetItemPos( QUICK, a, &pos );
				::SendServerEachItem( &pos, pItem, cn );
				::SendItemEventLog( pItem, cn, 0, SILT_DELETE_BY_ATTR, 1 ); //YGI acer
			}
		}
	}

	for( a=0; a<5; a++ )
	for( b=0; b<3; b++ )
	for( c=0; c<6; c++ )
	{
		ItemAttr *pItem = &ch->bank[a][b][c];
		if(pItem->item_no)
		{
			if(!ItemMgr.CheckFaultItem(*pItem,true))//020116 LSW// 10 등급 이상의 레어 및 이상한 아이템 지움 
			{
				::SetItemPos( BANK, a, b, c, &pos );
				::SendServerEachItem( &pos, pItem, cn );
				::SendItemEventLog( pItem, cn, 0, SILT_DELETE_BY_ATTR, 1 ); //YGI acer
			}
		}
	}

	return;
}

// 020428 YGI acer
int BreakItemByDuration( ItemAttr *pItem, CHARLIST *ch, POS pos )		// 내구도가 제로 일때 아이템을 부신다.
{
	SendItemEventLog( pItem, ch->GetServerID(), SN_NOT_USER, SILT_DELETE_BY_DURATION, 2 ); //YGI acer
	SendDeleteItem( pItem, &pos, ch, true );
	CheckAndSendChangeEquip( ch );

	return 1;
}

int CheckAutoDivideMoney( ItemAttr *item, const short int cn )		// 100000을 넘은 아이템 처리
{//021126 lsw
	CItem *t = ItemUnit( *item );
	if( !t ) {return 0 ;}

	if( t->GetItemKind() != IK_MONEY 
	&&	t->GetItemKind() != IK_NEW_MONEY ) {return 0;}		// 돈일 경우만
	CHARLIST *ch = ::CheckServerId( cn );
	if( !ch ) {return 0;}

	if( item->attr[IATTR_DURATION] > MAX_MONEY ) //100만원 넘어가네
	{//가를 아이템이 100만원을 넘는다
		int a=0, b=0, c=0;
		if( !::SearchMoneyItem( ch->inv, a, b, c ) )//빈공간이나 돈이 없네
		{//빈공간이나 돈이 없네
			if (!::SearchInv(ch->inv, a, b, c))
			{
				ItemAttr temp_item = *item;//돈아이템을 temp에 대입 
				item->attr[IATTR_MUCH]		= MAX_MONEY;//100만원으로 만들어 주고
				temp_item.attr[IATTR_MUCH] -= MAX_MONEY;//100만원 보다 크니까 100만원을 빼더라도 -값은 안된다
				::DropItem( ch->X, ch->Y, &temp_item );//방금전 것에서 100만원 뺀 돈을 바닥에 흘린다
				::SendItemEventLog( &temp_item , cn, cn, SILT_PUT_GROUND_MAX_INVEN, 1 );
				return 0;
			}
		}//if( !::SearchMoneyItem( ch->inv, a, b, c ) )//빈공간이나 돈이 없네

		//빈공간이나 돈이 있네
		ItemAttr *new_item = &ch->inv[a][b][c];
		if( new_item->item_no )
		{//그게 돈이라면 SearchMoneyItem에 의해 돈번호만 리턴 된다
			new_item->attr[IATTR_MUCH] += (item->attr[IATTR_MUCH] - MAX_MONEY);//100만원 넘기 때문에 100만원을 넘은 여분의 돈을 더한다
		}
		else
		{//빈칸 이라면
			*new_item = *item;
			new_item->attr[IATTR_MUCH] = (item->attr[IATTR_MUCH] - MAX_MONEY);//100만원 여분을 인벤에 넣는다
		}

		item->attr[IATTR_MUCH] = MAX_MONEY;
		::SendServerEachItem( item, ch );//결과물의 변화를 클라이언트로 날려준다
		if( new_item->attr[IATTR_MUCH] > MAX_MONEY ) 
		{//주고난 아이템이 100만 넘으면 나눠질때 까지 계속 나눈다
			::CheckAutoDivideMoney( new_item, cn );
		}
		POS pos = {0,};
		::SetItemPos( INV, a, b, c, &pos );
		::SendServerEachItem( &pos, new_item, cn );
		return 1;
	}//if( item->attr[IATTR_DURATION] > MAX_MONEY ) //100만원 넘어가네
	else//100만원 안넘네.. 나눌 필요가 없다
	{
		return 2;
	}
	return 0;
}

////////////////////// 그 아이템 그 위치에 정말로 존재하는 가? ////////////////////
bool ExistItem( short int cn, ItemAttr item, POS pos )		// 정말 존재하는 가?
{
	CHARLIST *ch = &connections[cn].chrlst;
	if( !ch ) return false;

	ItemAttr *target = GetItemByPOS( cn, pos );
	if( !target ) return false;
	
	int ret = memcmp( &item, target, sizeof( ItemAttr )  );
	if( ret == 0 ) return true;
	return false;
}

////////////////////////// 아이템 무게에 의한 movp //////////////////////
int CalcMovpByItem( CHARLIST *ch, int movp )
{
	return movp;
	int result = 0;
	int max = ch->Str * 100;
	if( ch->itemweight <= max ) return movp;

	if( max )
	{
		float rate = ( ch->itemweight - max ) / ((float)max) * 100.0f;
		int move_down = (int) ((rate*2)/100.0f*movp );
		result = movp - move_down;
	}
	if( result < 0 ) result = 0;
	return result;
}

void CheckEquipItem( CHARLIST *ch )		// 아이템을 이동했을때
{
	for( int c=0; c<8; c++ )
	{
		ItemAttr *item = &ch->equip[c];
		if( item->item_no )
		{
			CItem *t = ItemUnit(item->item_no);
			if( !t ) 
			{
				SendItemEventLog( item, ch->GetServerID(), 0, SILT_DELETE_BY_ATTR, 1 ); //YGI acer
				memset( item, 0, sizeof( ItemAttr ) );
				continue;
			}
			if( !t->GetItemImage() || !t->GetObjectNumber() ) 
			{
				SendItemEventLog( item, ch->GetServerID(), 0, SILT_DELETE_BY_ATTR, 1 ); //YGI acer
				memset( item, 0, sizeof( ItemAttr ) );
				continue;
			}
			t_chr_status_info cstatus;
			POS source = {0, };
			POS dest;
			SetItemPos( EQUIP, c, &dest );
			if( t->is_equipable( ch->GetServerID(), &cstatus, source, dest ) == NOT )
			{
				MoveEmptyInv( item, ch );
				SendServerEachItem( &dest, item, ch->GetServerID()); // 없어진 사실을 클라이언트에 알린다.
				t->ReleaseItem( ch );
				continue;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
//		아이템 장착에 의한 hp, mana 보정
void ChangePlusHp( CHARLIST *ch, int hp_max_plus, int mp_max_plus )
{
	ch->HpMax	+=	hp_max_plus;
	ch->ManaMax	+=	mp_max_plus;
}

int CItem_Weapon::PlusHp(CHARLIST *ch )
{
	if( ch->Spell )	ChangePlusHp( ch, Change_Life_max, Change_Divine_Max );
	else			ChangePlusHp( ch, Change_Life_max, Change_mana_max);
	return 1;
}

int CItem_Armor::PlusHp(CHARLIST *ch)
{
	if( ch->Spell )	ChangePlusHp( ch, Change_Life_max, Change_Divine_Max );
	else			ChangePlusHp( ch, Change_Life_max, Change_mana_max);
	return 1;
}

int CItem_Accessory::PlusHp(CHARLIST *ch)
{
	if( ch->Spell )	ChangePlusHp( ch, Change_Life_max, Change_Divine_Max );
	else			ChangePlusHp( ch, Change_Life_max, Change_mana_max);
	return 1;
}

int CItem_Etc::PlusHp(CHARLIST *ch)
{
	if( ch->Spell )	ChangePlusHp( ch, Change_Life_max, Change_Divine_Max );
	else			ChangePlusHp( ch, Change_Life_max, Change_mana_max);
	return 1;
}
////////////////////////////////////////////////////////////////////////

// 특정 아이템과 내가 가진 아이템의 갯수의 차를 리턴
int CheckInventory( CHARLIST *ch, int item, int count )
{
	int ct = 0;
	for( int a=0; a<3; a++ )
	{	for( int b=0; b<3; b++ )
		{	for( int c=0; c<8; c++ )
			{
				if(ch->inv[a][b][c].item_no == item ) 
				{
					ct++;
				}
			}
		}	
	}

	return ct - count;
}

// 특정 아이템을 원하는 갯수만큼 지운다.
int SendDeleteMuchItemQuantity( CHARLIST *ch, int item, int count )
{
	if( !count ) return 1;
	int ct = 0;

	
	for( int a=0; a<3; a++ )
		for( int b=0; b<3; b++ )
			for( int c=0; c<8; c++ )
			{
				ItemAttr *lpItem = &ch->inv[a][b][c];
				if( lpItem->item_no == item ) 
				{
					if( lpItem->attr[IATTR_MUCH] > count ) 
					{
						lpItem->attr[IATTR_MUCH] -= count;
						POS pos;
						SetItemPos( INV, a, b, c, &pos );
						// 값이 감소 됐음을 보내준다.
						SendChangeDurationByKein( &pos, lpItem->attr[IATTR_MUCH], ch->GetServerID());
						return 1;
					}
					else if( lpItem->attr[IATTR_MUCH] == count )
					{
						POS pos;
						SetItemPos( INV, a, b, c, &pos );
						// 020428 YGI acer
						SendDeleteItem( lpItem, &pos, ch );
						return 1;
					}
					else
					{
						count -= lpItem->attr[IATTR_MUCH];
						POS pos;
						SetItemPos( INV, a, b, c, &pos );
						// 020428 YGI acer
						SendDeleteItem( lpItem, &pos, ch );
						continue;
					}
				}
			}

	return 0;
}

// 특정 아이템을 원하는 갯수만큼 지운다.
int SendDeleteItemQuantity( CHARLIST *ch, int item, int count )
{
	if( !count ) return 1;
	int ct = 0;
	for( int a=0; a<3; a++ )
		for( int b=0; b<3; b++ )
			for( int c=0; c<8; c++ )
			{
				if(ch->inv[a][b][c].item_no == item )
				{
					POS pos;
					SetItemPos( INV, a, b, c, &pos );
					// 020428 YGI acer
					SendItemEventLog( &ch->inv[a][b][c], ch->GetServerID(), -2, SILT_NEED_ITEM, 6 ); //YGI acer
					SendDeleteItem( &ch->inv[a][b][c], &pos, ch );
					ct++;
					if( ct >= count ) return 1;
				}
			}

	return 0;
}

int EquipItem( CHARLIST *ch )//020303 lsw 
{
	for( int i=0; i<8; i++ )
	{
		if( ch->equip[i].item_no )
		{
			CItem *t = ItemUnit( ch->equip[i] );
			if( !t ) continue;
			switch (t->GetWearAble())
			{
				case	WEAR_ANYTHING	:
				case	WEAR_RIGHT_HAND	:
				case	WEAR_LEFT_HAND	:
				case	WEAR_HEAD		:
				case	WEAR_BODY		:
				case	WEAR_NECK		:
				case	WEAR_SHOES		:
				case	WEAR_UNIQUE		:
				case	WEAR_TWO_HAND	:
				{
					t->EquipItem( ch );
				}
				break;

				case	WEAR_BELT		:
				case	WEAR_ONLY_INVEN	:
				default:
				{
				}
				break;
			}

		}
	}
	ResetAbility(ch);
	return 1;
}

int ReleaseItem( CHARLIST *ch )
{
	for( int i=0; i<8; i++ )
	{
		if( ch->equip[i].item_no )
		{
			CItem *t = ItemUnit( ch->equip[i] );
			if( !t ) continue;
			t->ReleaseItem( ch );
		}
	}
//	ch->ResetAbility();
	ResetAbility(ch);//020215 lsw
	return 1;
}

int	DeleteINVItem(CHARLIST*	pChar,int nItemNumber,int nDelCount)
{
	int nDeletedCount = 0;
	for(int i = 0;i < 3;++i)
		for(int j = 0;j < 3;++j)
			for(int k = 0;k < 8;++k)
			{
				if(pChar->inv[i][j][k].item_no == nItemNumber)
				{
					POS pos;
					if(nDelCount == 0)
					{
						SetItemPos( INV, i, j, k, &pos );
						SendItemEventLog( &pChar->inv[i][j][k], pChar->GetServerID(), -2, SILT_NEED_ITEM, 6 );
						SendDeleteItem( &pChar->inv[i][j][k], &pos, pChar );
						++nDeletedCount;
					}
					else
					{
						if(nDelCount > nDeletedCount)
						{
							SetItemPos( INV, i, j, k, &pos );
							SendItemEventLog( &pChar->inv[i][j][k], pChar->GetServerID(), -2, SILT_NEED_ITEM, 6 );
							SendDeleteItem( &pChar->inv[i][j][k], &pos, pChar );
							++nDeletedCount;
						}
						else
						{
							continue;
						}
					}
				}
			}

	return nDeletedCount;
}

void CheckMallItemTime( short int cn )
{
	CHARLIST *ch = CheckServerId(cn);
	if( !ch ) {return;}

	POS pos = {0,};

	int a=0, b=0, c=0;
	for( a=0; a<3; a++ )
	for( b=0; b<3; b++ )
	for( c=0; c<8; c++ )
	{
		ItemAttr *pItem = &ch->inv[a][b][c];
		CItem *t = ItemUnit(pItem->item_no);
		if(pItem->item_no && pItem->attr[IATTR_MALL_TIMED] > 0 && t->GetItemKind() != IK_TREASURE_MAP)
		{
			if (pItem->attr[IATTR_ATTR] > g_curr_time)
			{
				pItem->attr[IATTR_MALL_TIMED] = pItem->attr[IATTR_ATTR] - g_curr_time;
			}
			SetItemPos( INV, a, b, c, &pos );
			SendServerEachItem( &pos, pItem, cn );
		}
	}
	
	for( a=0; a<ITEM_EQUIP_MAX; a++ )
	{
		ItemAttr *pItem = &ch->equip[a];
		CItem *t = ItemUnit(pItem->item_no);
		if(pItem->item_no && pItem->attr[IATTR_MALL_TIMED] > 0 && t->GetItemKind() != IK_TREASURE_MAP)
		{
			if (pItem->attr[IATTR_ATTR] > g_curr_time)
			{
				pItem->attr[IATTR_MALL_TIMED] = pItem->attr[IATTR_ATTR] - g_curr_time;
			}
			SetItemPos( EQUIP, a, &pos );
			SendServerEachItem( &pos, pItem, cn );
		}
	}
	
	for( a=0; a<ITEM_QUICK_MAX ; a++ )
	{
		ItemAttr *pItem = &ch->quick[a];
		CItem *t = ItemUnit(pItem->item_no);
		if(pItem->item_no && pItem->attr[IATTR_MALL_TIMED] > 0 && t->GetItemKind() != IK_TREASURE_MAP)
		{
			if (pItem->attr[IATTR_ATTR] > g_curr_time)
			{
				pItem->attr[IATTR_MALL_TIMED] = pItem->attr[IATTR_ATTR] - g_curr_time;
			}
			SetItemPos( QUICK, a, &pos );
			SendServerEachItem( &pos, pItem, cn );
		}
	}

	for( a=0; a<5; a++ )
	for( b=0; b<3; b++ )
	for( c=0; c<6; c++ )
	{
		ItemAttr *pItem = &ch->bank[a][b][c];
		CItem *t = ItemUnit(pItem->item_no);
		if(pItem->item_no && pItem->attr[IATTR_MALL_TIMED] > 0 && t->GetItemKind() != IK_TREASURE_MAP)
		{
			if (pItem->attr[IATTR_ATTR] > g_curr_time)
			{
				pItem->attr[IATTR_MALL_TIMED] = pItem->attr[IATTR_ATTR] - g_curr_time;
			}
			SetItemPos( BANK, a, b, c, &pos );
			SendServerEachItem( &pos, pItem, cn );
		}
	}

	return;
}

bool IsNoExchange(ItemAttr* item)
{
	CItem* t = ItemUnit(item->item_no);
	if (t->GetItemKind() == IK_NOEXCHANGE || 
		t->GetItemKind() != IK_TREASURE_MAP &&
		//item->attr[IATTR_ATTR] != 0)
		item->attr[IATTR_MALL_TIMED] != 0)		
	{
		return true;
	}
	return false;
}