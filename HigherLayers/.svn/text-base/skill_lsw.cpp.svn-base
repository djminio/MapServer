#include "..\stdafx.h"
#include "DefaultHeader.h"
#include "DR_NETWORK.h"
#include "CITEM.h"
#include "skill_lsw.h"
#include "MenuDefine.h"
#include "Auction.h"
#include "GmMgr.h"
#include "Menuserver.h"
#include "MailMgr.h"
#include "UserManager.h"
#include "LogManager.h"	// BBD 040308

extern void RecvCMD_COMFORM_RESET_ABILITY(t_packet &packet);//020820 lsw
extern void RecvCMD_TRADE_MODE_CHANGE(const int iCn, t_packet *p);//030127 lsw //Trade On/Off 셋팅

extern int SearchItemByInv( int item_no, CHARLIST *ch );
extern HDBC  hDBC;

t_raregroup			RareGroup[5][MAX_RARE_GROUP];
t_rareprobability	Rareprobability[MAX_RARE_PROBABILIY];
t_ItemFunction		ItemFunction[MAX_ITEM_FUNCTION];

int CanLearnSkill( CHARLIST *ch, int skill, int check_ability )
{
	if( !ch ) return 0;
	int inc = SkillTbl[skill].inclusive;	// 판단루틴 
	if( ch->Skill[skill] ) return 2;		// 이미 배웠음
	if( check_ability && ch->skillexp[inc].skillexp < (DWORD)SkillTbl[skill].need_exp ) return 3;	// 능력이 모자람
	
	return 1;		// 배우기에 가능함.
}

void SendLearnSkillOk( int skill_no, t_connection c[], int cn )
{
	t_packet packet;
	packet.h.header.type = CMD_LEARN_SKILL_OK;
	{
		packet.u.kein.server_learn_skill_ok.skill_no = skill_no;
	}
	packet.h.header.size = sizeof( k_server_learn_skill_ok );
	QueuePacket( c, cn, &packet, 1 );		// 클라이언트 전송	받은거 고대로 보내준다.
}

void SendStartSkillExp( int inc, t_skillexp exp, t_connection c[], int cn )
{
	t_packet packet;
	packet.h.header.type = CMD_SKILL_EXP;
	{
		packet.u.kein.server_skill_exp.type = inc;
		packet.u.kein.server_skill_exp.exp = exp;
	}
	packet.h.header.size = sizeof( k_server_skill_exp );
	QueuePacket( c, cn, &packet, 1 );		// 클라이언트 전송	받은거 고대로 보내준다.
}

int GetSkillMother( int kind, int skill_mother[], int max )
{
	memset( skill_mother, 0, sizeof( int )*max );
	int count = 0;
	for( int i=0; i<MAX_SKILLMAIN; i++ )
	{
		if( SkillTbl[i].MotherSkillType == kind )
		{
			skill_mother[count++] = i;
		}
	}
	return count;
}

void RecvLearnSkill( t_client_learn_skill *p, t_connection c[], int cn  )		//###1213 수정
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

//	ch->Money = GetMoneyByItem( ch );

	int inc = SkillTbl[p->skillno].inclusive;
	int money = SkillTbl[p->skillno].money;

	int skill_mother[20];
	int count_max = GetSkillMother( inc, skill_mother, 20 );

	if( ch->Money < (DWORD)money )
	{
		SendServerResult( CM_LEARNSKILL, 4, cn );
		return; // 돈이 부족해...
	}

	int befor_enough = 1;		// 전단계 스킬을 렙 50이상 익혔는가?
	int check_ability=1;		// 능력 체크를 해야 하는가?

	switch (inc)
	{
		case COOKING:
			{
				if( ch->skillexp[FARMING].skillexp/10000 < 50 && ch->skillexp[FISHING].skillexp/10000 < 50 ) befor_enough=0;
				break;
			}
		case BLACKSMITHY:
			{
				if( ch->skillexp[MINING].skillexp/10000 < 50 )	befor_enough=0;
				break;
			}
		case CANDLEMAKING:
			{
				if( ch->skillexp[FISHING].skillexp/10000 < 50 )	befor_enough=0;
				break;
			}
		case CARPENTRY:	
		case BOWCRAFTING:
			{
				if( ch->skillexp[CUTDOWN].skillexp/10000 < 50 ) befor_enough=0;
				break;
			}
		case TAILORING:
			{
				if( ch->skillexp[BUTCHERING].skillexp/10000 < 50 ) befor_enough=0;
				break;
			}
		case ALCHEMING:
			{
				if( ch->skillexp[HERBDIGGING].skillexp/10000 < 50 ) befor_enough=0;
				break;
			}
		case TAIMING : 
			{	//< CSD-030806
				if (ch->GetLevel() < 35 ) befor_enough = 0;
				check_ability = 0; // 더이상 능력 체크는 없다.
				break;
			}	//> CSD-030806
		case ANIMAL_LORE:
			{
				if( ch->skillexp[TAIMING].skillexp/10000 < 40 ) befor_enough = 0;
				break;
			}
	}

	if( !befor_enough )
	{
		SendServerResult( CM_LEARNSKILL, 3, cn );
		return;
	}

	int ret_learn = 0;
	int error_learn=0;
	for( int i=0 ; i<count_max ; i++ ) 
	{
		int ret = CanLearnSkill( ch, skill_mother[i], check_ability );
		if( ret == 1 ) 
		{
			ch->Skill[skill_mother[i]] = true;
			SendLearnSkillOk(skill_mother[i], c, cn );
			ret_learn = 1;
		}
//		else ret = error_learn;			// Commented by chan78 at 2000/11/28
		else error_learn = ret;			// 001212 YGI
	}

	if( !ret_learn )
	{
		SendServerResult( CM_LEARNSKILL, error_learn, cn );	// 2:이미 배웠음   3:능력이 부족함.
		return;
	}
	SubtractMoney( money, ch );
	//SendSubtractMoney( money, cn );

	if( ch->skillexp[inc].skillexp < 50000 ) {ch->skillexp[inc].skillexp = 50000;}
	//011022 lsw >
	SkillMgr.SendSkillExp( inc, ch->skillexp[inc], cn);
	//011022 lsw <

	SendServerResult( CM_LEARNSKILL, 1, cn );
}

void RecvGmRegist( t_packet *p, short int cn, int makemode )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) {return;}

	const int gm_list_index = p->u.kein.gm_regist.gm_list_index;
	if(0 > gm_list_index || MAX_GM_MAIN <= gm_list_index){return;}

	GM_QUEST *pGmMain = &g_GmMain[gm_list_index];

	const int iSkillNo = pGmMain->skill;

	const int ret = GMMgr.CanGmRegist( gm_list_index, ch );//pGmMain이 1 부터니까
	if(0 > ret){return;}//021212 lsw //등록 불가라면

	// finito 040707 for merchant cape item.
	ItemAttr item;  
	int a, b, c; 
	if (ret == 3) // finito 040707 If Merchant check that can recv merchant cape
	{
		item = GenerateItem(8299);
		if( !item.item_no )
		{
			SendPutMenuString( KM_FAIL, 240, cn );
			return;
		}

		int ret = SearchInv( ch->inv, a, b, c );
		if( !ret )
		{
			SendPutMenuString( KM_FAIL, 241, cn );
			return;
		}
	}

	// 아이템 삭제, 돈 삭제 및 여러 처리
	if(-1 == ::SubtractMoney( pGmMain->need_money, ch ))
	{
		return;//돈도 없는게 콱
	}


	for( int i=0; i<MAX_GM_NEED_ITEM_COUNT; i++ )
	{
		const int iItemNo = pGmMain->need_item[i];
		if( iItemNo )
		{
			::SendDeleteItemQuantity( ch, iItemNo, pGmMain->need_quantity[i] );		// 특정 아이템을 특정 갯수만큼 삭제 한다.
		}
	}

	switch(ret)
	{
	case 1:
	case 2:
	case 3:
		{
			ch->skillexp[iSkillNo].skillexp = 1000000;//011012 lsw
			ch->view_job = gm_list_index;// 보여줄 직업을 GM으로 바꾼다.
			{// 디비 저장
			t_packet packet;
			packet.h.header.type = CMD_GM_REGIST;
			packet.u.kein.gm_regist_db.gm_list = gm_list_index;
			::strcpy_s( packet.u.kein.gm_regist_db.name, sizeof(packet.u.kein.gm_regist_db.name) , ch->Name );
			packet.h.header.size = sizeof( k_gm_regist_db );
			QueuePacket(connections, DB_DEMON, &packet, 1);

			if (ret == 3)
			{
				// finito 040707 start of give merchant cape
				POS pos;
				SetItemPos( INV, a, b, c, &pos );
				ch->inv[a][b][c] = item;
				SendItemEventLog( &ch->inv[a][b][c], ch->GetServerID(), SN_NOT_USER, SILT_MAKE_BY_SCRIPT, 2 );//넣은후
				SendServerEachItem( &pos, &item, ch->GetServerID());
				SendPutMenuString( KM_OK, 242, cn );
			}
			}
		}break;
	case 4://기성의 경우는 좀 다르다
		{//기성은 디비에 저장도 안한다
			t_SkillExp3 *pExp = (t_SkillExp3*)(&ch->skillexp[iSkillNo]);
			pExp->skillType = gm_list_index;
			pExp->year = g_year;
			pExp->day = g_mon*30 +g_day+10;//10일을 준다
		}break;
	default:
		{
			return;
		}break;
	}//switch(ret)

	
	SkillMgr.SendSkillExp(iSkillNo,ch->skillexp[iSkillNo],ch->GetServerID());

	//010708 lsw 지엠 등록과 함께 스킬이 간다.
	if(ret==2)
	{
		int iCheckJob = iSkillNo;
		
		int iMy2ndSkillNo = g_GmMain[gm_list_index].skill;
		int iResultGMSkillNumber =-1;
		
		if( ch->skillexp[iCheckJob].skillexp == 1000000)//011012 lsw
		{
			int ok =0;
			switch(iCheckJob)
			{
			case CARPENTRY		:	iResultGMSkillNumber = 106;	ok = 1;
				break;
			case BLACKSMITHY	:	iResultGMSkillNumber = 107;	ok = 1;
				break;
			case COOKING		:	iResultGMSkillNumber = 108;	ok = 1;
				break;
			case TAILORING		:	iResultGMSkillNumber = 109;	ok = 1;															
				break;
			case BOWCRAFTING	:	iResultGMSkillNumber = 110;	ok = 1;
				break;
			case ALCHEMING		:	iResultGMSkillNumber = 111;	ok = 1;
				break;
			case CANDLEMAKING	:	iResultGMSkillNumber = 112;	ok = 1;
				break;
			default:
				ok = 0;
				break;
			}
			if(	ok ==1)
			{
				ch->Skill[iResultGMSkillNumber] = true;
				::SendLearnSkillOk(iResultGMSkillNumber,connections,cn);
			}
		}
	}
	return;
}

const int Convert[2][5]={//020823 lsw
		{RARE_POWER,		RARE_POWER,		RARE_POWER,		RARE_VIGOR,			RARE_POWER},
		{RARE_VIGOR,		RARE_VIGOR,		RARE_VIGOR,		RARE_PIERCING,		RARE_PIERCING}};

bool CorrectRareKind(const int iResultAttr,const int iCompareAttr)
{
	RareMain *result = ((LPRareMain)(&iResultAttr));
	RareMain *compare = ((LPRareMain)(&iCompareAttr));

	const int iResultSok = result->soksung1;
	const int iTargetSok = compare->soksung1;
	
	if( result->grade != compare->grade){return false;}
	if( result->iHighLevel != compare->iHighLevel){return false;}
	if( result->soksung2 != compare->soksung2){return false;}
	if( result->soksung3 != compare->soksung3){return false;}
	if( result->IsDynamicRare != compare->IsDynamicRare){return false;}

	for(int i = 0; i < 5; i++)
	{
		if(	Convert [0][i] == iResultSok)
		{
			if(	Convert [1][i] == iTargetSok)
			{
				return true;
			}
		}
		else if(	Convert [1][i] == iResultSok)
		{
			if(	Convert [0][i] == iTargetSok)
			{
				return true;
			}
		}
	}
	return false;
}
void ConvertRare(RareMain &rare)
{
	for(int i = 0; i < 5; i++)
	{
		if(	Convert [1][i] == rare.soksung1)
		{
			rare.soksung1 = Convert [0][i];
		}
	}
}

void RecvCMD_SKILL_RARE_UPGRADE(const int cn, t_packet *p)
{
	const int iSkillNo	= p->u.SkillRareUpgrade.iSkillNo;
	LPCHARLIST ch		= ::CheckServerId(cn);
	if(!ch)
	{
		return;
	}

	if (!ch->IsPayedUser())
	{
		return;
	}
	
	float iSuccessRate = 0.000;
	ItemAttr SourceItem = GenerateItem(p->u.SkillRareUpgrade.SourceItem.item_no);

	if( !SourceItem.item_no ) return;

	int iSourceItemMuch = 0;//item_result 꺼가 하나니까

	unsigned int iResultSok	= 0;
	RareMain TempSokSung;

	CItem *	t = ItemUnit( SourceItem.item_no);
	SourceItem.attr[IATTR_RARE_MAIN] = p->u.SkillRareUpgrade.SourceItem.attr[IATTR_RARE_MAIN];
	memcpy(&TempSokSung,&p->u.SkillRareUpgrade.SourceItem.attr[IATTR_RARE_MAIN],sizeof(int));
	const bool bIsDynamicRare = TempSokSung.IsDynamicRare;
	if( !bIsDynamicRare)
	{
		ConvertRare(TempSokSung);
	}

	if( !t ){return;}

	WORD now =0, max =0, nowtemp=0, maxtemp=0;
	DWORD resultdur =0;
	

	int why = 0;
	int resourcelimit[MAX_UPGRADE_RARE] = {0,};
	
	int a[4] = {0,};//additem 1~4
	int al[4] = {0,};//additem Limit 1~4
	int HaveResource[4] = {0,};//필요 아이템이 있나

	const int iKind = t->GetItemKind();
	const int AddItem[4] ={	ItemMutantKind[iKind].AddItem[0]/100,
							ItemMutantKind[iKind].AddItem[1]/100,
							ItemMutantKind[iKind].AddItem[2]/100,
							ItemMutantKind[iKind].AddItem[3]/100};

    bool keepMaterialItems = false;

    // Sets the iSourceItemMuch without deleting any materials (needed to work out the success rate)
    POS pos;
    ItemAttr *i;
    for (int ti = 0; ti < MAX_UPGRADE_RARE ; ti++)//스킬을 시도한 아이템을 모조리 지운다
    {
        pos = p->u.SkillRareUpgrade.pos[ti];
        switch( pos.type )
        {
        case INV :        i = &ch->inv[pos.p1][pos.p2][pos.p3];    break;
        case EQUIP :    i = &ch->equip[pos.p3];                    break;
        case QUICK :    i = &ch->quick[pos.p3];                    break;
        default: continue; break;
        }
       
        LPRareMain pRare = ((LPRareMain)(&SourceItem.attr[IATTR_RARE_MAIN]));

        if( i->item_no )
        {
            if( SourceItem.item_no == i->item_no )//011224 lsw
            {
                iSourceItemMuch++;
            }
        }
    }

    // Pre-calculate the success of skill
    const int PerpectMuch = ItemGrade[TempSokSung.grade+1].ResourceAmount[0]; // ResourceAmount[iKind-1];//1 일때만 가능 하니까 1을 빼면 0을 인덱스로 쓸 수 있다

    if( PerpectMuch  <= 0)
    {//요구 갯수가 없다..
        why |= 0x0010;
    }

	// Stop players from getting 100% by putting more source items than should be able to.
	if (iSourceItemMuch > ItemGrade[TempSokSung.grade+1].ResourceAmount[1])
	{
		return;
	}

    if( PerpectMuch )
    {//성공률을 뽑는다
        iSuccessRate = float(iSourceItemMuch)/float(PerpectMuch);
        iSuccessRate    *=    100;

        if(iSuccessRate >= 100)
        {
            iSuccessRate = 100;
        }
    }
    int iNowSuccess = (rand()%100);

	int FoundBook = 0; // 0 = No Book
	if (TempSokSung.grade >= 4) // Only use books for upgrading to g5+
	{
		if (SearchItemByInv(1053, ch))
		{
			FoundBook = 3; // 3 is the 100% success book
		}
		else if (SearchItemByInv(1051, ch))
		{	
			FoundBook = 1; // 1 is save items book
			if (iSuccessRate <= iNowSuccess)
			{
				keepMaterialItems = true;
			}
		}
	}

	int iIsCracked =0;
	for (int ti = 0; ti < MAX_UPGRADE_RARE ; ti++)//스킬을 시도한 아이템을 모조리 지운다
	{
		pos = p->u.SkillRareUpgrade.pos[ti];
		switch( pos.type )
		{
		case INV :		i = &ch->inv[pos.p1][pos.p2][pos.p3];	break;
		case EQUIP :	i = &ch->equip[pos.p3];					break;
		case QUICK :	i = &ch->quick[pos.p3];					break;
		default: continue; break;
		}
		
		LPRareMain pRare = ((LPRareMain)(&SourceItem.attr[IATTR_RARE_MAIN]));

		if( i->item_no )
		{
			if( SourceItem.item_no == i->item_no )//011224 lsw
			{
				if( 0 != pRare->iHighLevel )//레어 아이템이 아니면(플러스 아이템이거나 
				{
					iIsCracked = 1;//하이 아이템 업글 하려면
				}
				if( pRare->soksung2
				||	pRare->soksung3)
				{
					iIsCracked = 1;//멀티레어 업글 하려면 //0211120 lsw
				}

				if( SourceItem.attr[IATTR_RARE_MAIN] != i->attr[IATTR_RARE_MAIN] )
				{
					if(!CorrectRareKind(SourceItem.attr[IATTR_RARE_MAIN],i->attr[IATTR_RARE_MAIN]))
					{
						iIsCracked = 1;//MyLog(0,"업글 오기는 왔는데 엉뚱한거 아이템만 빼앗고 삭제..");	
					}
				}
				
				if(t->GetRbutton() ==DIVIDE_ITEM)
				{
					if(	DIVIDE_ITEM_UPGRDEABLE_MIN > i->attr[IATTR_DURATION]
					||	DIVIDE_ITEM_UPGRDEABLE_MAX < i->attr[IATTR_DURATION])
					{
						iIsCracked = 1;
					}
					max += i->attr[IATTR_DURATION];
				}
				else
				{
					GetItemDuration( *i, nowtemp, maxtemp);
					now += nowtemp;
					max += maxtemp;
				}
				resourcelimit[iSourceItemMuch]	= i->attr[IATTR_LIMIT]	;
				iSourceItemMuch++;
			}
			for(int addcount = 0;addcount <4;addcount++)
			{
				if(AddItem[addcount])//아이템 넘버가 있으면
				{
					if( AddItem[addcount]  == i->item_no )
					{
						if(!a[addcount])
						{
							a[addcount] = i->item_no;
							al[addcount]= i->attr[IATTR_LIMIT];
							HaveResource[addcount] = 1;
						}
					}
				}
				else// AddItem	//요구 아이템이 없기 때문에 리소스 있는걸로 체크
				{
					HaveResource[addcount] = 1;
				}
			}
			// Keep items if have book1
			if (!keepMaterialItems)
			{		
				::SendItemEventLog( i, cn, 0, SILT_RESOURCE_BY_SKILL, 1);	 //YGI acer
				::DeleteItem( i );//아이템이 있었기 때문에 지운다
			}
		}
	}

	// Delete book1 
	if (FoundBook == 1)
	{
		::SendDeleteItemQuantity(ch,1051,1);
	}

	if(iIsCracked){why |= 0x0001;}
	if(!iSourceItemMuch ){why |= 0x0002;}
	if(	!HaveResource[0]	||	!HaveResource[1]
	||	!HaveResource[2]	||	!HaveResource[3])
	{
		why |= 0x0004;
	}//요구 하는 아이템이 없다 (AddItem 이 0 이면 자동으로 1체크 하니까 || 를 쓴다

	
	if(!ItemGrade[TempSokSung.grade+1].iAble)
	{//최대로 업그레이드 가능한 등급인가?
		why |= 0x0008;//업글 불가
	}
	else
	{//등급 업그레이드가 가능
		//아이템 넘버와 개수를 가지고
		const int iItemKind = t->GetItemKind();
		if (MAX_ITEM_MUTANT_KIND < iItemKind )
		{
			why |= 0x1000;//아이템 카인드 테이블 오버 참조
		}
		
		int iKind = ItemMutantKind[iItemKind].upgrade_type ;
		if( !iKind ) 
		{
			iKind = 1;//우선 메모리 오버 참조 안하게 하기 위해서 1로 준다
			why |= 0x0100;
		}//업그레이드 안되는 아이템 카인드다
	}

	//패킷 보낼 준비
	t_packet packet;
	packet.h.header.type = CMD_SKILL_RARE_UPGRADE_RESULT;
	packet.h.header.size = sizeof( t_SkillRareUpgradeResult);
	ZeroMemory(&packet.u.SkillRareUpgradeResult,sizeof( t_SkillRareUpgradeResult));//메모리 초기화 되었음
	
	packet.u.SkillRareUpgradeResult.iSuccess = iNowSuccess;
	
	// 100% success for book3
	if (FoundBook == 3)
	{
		::SendDeleteItemQuantity(ch,1053,1);
		iSuccessRate = 100;
	}	

	if( iSuccessRate > iNowSuccess)
	{	//성공했으면 들어갈 처리		
		if (!why)//실패 이유가 셋팅 되지 않아야 줄 수 있다
		{
			if ( DIVIDE_ITEM == t->GetRbutton()) 
			{
				resultdur = (float)max/(float)iSourceItemMuch * (float)(1.100);
			}
			else
			{
				now = ((float)now/(float)iSourceItemMuch * (float)(1.100)) * 2;
				max = ((float)max/(float)iSourceItemMuch * (float)(1.100)) * 2;
				if(!now) {now = 1;}//이럴 경우가 있냐??
				if(!max) {max = 1;}
				resultdur = MAKELONG( now, max);
			}

			if( ItemMgr.MakeRareAttr(iResultSok ,TempSokSung.grade+1,TempSokSung.soksung1,0,0,0,bIsDynamicRare))
			{
				SourceItem.attr[IATTR_RARE_MAIN]	=	iResultSok;
				SourceItem.attr[IATTR_DURATION]		=	resultdur;	
			}
			else
			{
				why |= 0x2000;//레어 조건이 좀 이상한가보다
			}

			packet.u.SkillRareUpgradeResult.iSuccess			=	100			;
			packet.u.SkillRareUpgradeResult.SourceItem			=	SourceItem	;
				
			if(!resultdur )
			{//내구도가 없어??
				why |= 0x0040;
			}

			int a,b,c;
			if(!why)// 한번더 문제 없이 왔다면 
			{
				if( ::SearchInv( ch->inv, a, b, c ))//캐릭터 인벤토리에 아이템을 넣어준다
				{
					POS pos;
					::SetItemPos( INV, a, b, c, &pos );
					ch->inv[a][b][c] = SourceItem;
					::SendServerEachItem( &pos , &SourceItem , ch->GetServerID());
					
					t_packet bbspacket;
					
					//전체 공지
					bbspacket.h.header.type = CMD_RARE_UPGRADE_BBS_MAP_TO_MAP;

					::memcpy( bbspacket.u.RareUpgradeBBSMapToMap.name, ch->Name,20);
					bbspacket.h.header.size = sizeof(t_RareUpgradeBBSMapToMap );
					bbspacket.u.RareUpgradeBBSMapToMap.Item = SourceItem ;
					::SendNationPacket( &bbspacket, ch->name_status.nation );
					bbspacket.h.header.type = CMD_RARE_UPGRADE_BBS;
					g_pUserManager->SendPacket(&bbspacket); // CSD-CN-031213
				}
				else
				{
					why |= 0x0080;//자리가 없단다.. 말이 되나..
				}
			}
		}
	}
	else
	{
		why |=0x0020;//성공률 때문에 실패
	}
	
	// Informs client that materials are still in inventory when fail with book1
	if (keepMaterialItems)
	{
		for (int ti = 0; ti < MAX_UPGRADE_RARE ; ti++)
		{
			pos = p->u.SkillRareUpgrade.pos[ti];
			switch( pos.type )
			{
			case INV :		i = &ch->inv[pos.p1][pos.p2][pos.p3];	break;
			case EQUIP :	i = &ch->equip[pos.p3];					break;
			case QUICK :	i = &ch->quick[pos.p3];					break;
			default: continue; break;
			}
			
			if( i->item_no )
			{
				::SendServerEachItem( &pos , i , ch->GetServerID());
			}
		}
	}

	Send_RareItemMakeLog( cn,SourceItem.item_no,0, 
		TempSokSung.grade+1,TempSokSung.soksung1,TempSokSung.soksung2,TempSokSung.soksung3,a[0],al[0],a[1],al[1],SourceItem.attr[IATTR_LIMIT],now,max,why,2002,
		resourcelimit[0],resourcelimit[1],resourcelimit[2],
		resourcelimit[3],resourcelimit[4],resourcelimit[5]);//결과 기록

	::QueuePacket(connections, cn, &packet, 1);
	return;
}

void RecvCMD_SKILL_MASTER_MULTI_RARE_MAKE(const int cn, t_packet *p)
{
	t_SkillMasterMultiRareMake *pSMMRM = &p->u.Hwoa.rang.SMMultiRareMake;

	LPCHARLIST ch = ::CheckServerId(cn);
	if(!ch){return;}

	const int iSkillNo = pSMMRM->iSkillNo;
	const int iNowSkillNo = GMMgr.IsSkillMaster(ch);
	if(!iNowSkillNo){return;}//기성 아니다//받았는데 다르거나 하다

	if (!ch->IsPayedUser())
	{
		return;
	}
	
	//아이템 체크
	ITEMATTR RecvSrcItem[2]; //받은거
	RecvSrcItem[0]=	pSMMRM->MRS.SrcData[0].attr;
	RecvSrcItem[1]=	pSMMRM->MRS.SrcData[1].attr;

	ITEMATTR NowSrcItem[2];
	NowSrcItem[0] =	*GetItemByPOS(ch,pSMMRM->MRS.SrcData[0].pos);
	NowSrcItem[1] =	*GetItemByPOS(ch,pSMMRM->MRS.SrcData[1].pos);

	if(memcmp(RecvSrcItem,NowSrcItem,sizeof(ItemAttr))){return;}//서버와 클라이언트가 다릅니다.
	if(RecvSrcItem[0].item_no != RecvSrcItem[0].item_no){return;}//아이템 번호가 다르면 합칠 수 없습니다.
	const int iResultItemNo = pSMMRM->MRS.SrcData[0].attr.item_no;
	CItem *	t = ItemUnit( iResultItemNo);
	if( !t ){return;}

	if(DIVIDE_ITEM == t->GetRbutton())
	{
		const int iPriCt = NowSrcItem[0].attr[IATTR_MUCH];
		const int iSecCt = NowSrcItem[1].attr[IATTR_MUCH];
		if(	DIVIDE_ITEM_UPGRDEABLE_MIN > iPriCt
		||	DIVIDE_ITEM_UPGRDEABLE_MAX < iPriCt
		||	DIVIDE_ITEM_UPGRDEABLE_MIN > iSecCt
		||	DIVIDE_ITEM_UPGRDEABLE_MAX < iSecCt )
		{
			::OutMessage(cn,2,266);//021120 lsw
			return;
		}
	}

	RareMain RecvRare1 = *(RareMain*)&NowSrcItem[0].attr[IATTR_RARE_MAIN];
	RareMain RecvRare2 = *(RareMain*)&NowSrcItem[1].attr[IATTR_RARE_MAIN];
	
	if(RecvRare1.soksung2 ||RecvRare1.soksung3
	|| RecvRare2.soksung2 ||RecvRare2.soksung3)
	{
		::OutMessage(cn,2,511);//021120 lsw
		return;
	}
	if(RecvRare1.soksung1 == RecvRare2.soksung1)
	{
		::OutMessage(cn,2,890);//021120 lsw
		return;
	}

	const int iGrade = (RecvRare1.grade+RecvRare2.grade)/2;//평균

	if( 5 < iGrade )
	{
		::OutMessage(cn,2,881);//021212 lsw
		return;
	}

	DWORD	resultdur =0;
	if( DIVIDE_ITEM == t->GetRbutton()) 
	{
		resultdur = (RecvSrcItem[0].attr[IATTR_MUCH]+RecvSrcItem[1].attr[IATTR_MUCH])*(float)(1.100)/2;
	}
	else
	{
		WORD	max1= 0, now1= 0;
		WORD	max2= 0, now2= 0;
		::GetItemDuration( RecvSrcItem[0], now1,max1);
		::GetItemDuration( RecvSrcItem[1], now2,max2);

		WORD now = (float)(now1+now2)*(float)(1.100)/2;
		WORD max = (float)(max1+max2)*(float)(1.100)/2;

		if(!now) {now = 1;}//이럴 경우가 있냐??
		if(!max) {max = 1;}
		resultdur = MAKELONG( now, max);
	}
	ItemAttr ResultItem = ItemMgr.GenerateItem(iResultItemNo);//지급될 아이템 셋팅
	ResultItem.attr[IATTR_MUCH] = resultdur;//내구도 셋팅
	ItemMgr.MakeRareAttr(ResultItem.attr[IATTR_RARE_MAIN],iGrade,RecvRare1.soksung1,RecvRare2.soksung1,0,0,0);//멀티 레어 만들었다

	bool iIsHaveResource = true;
	const ITEMMULTIRARE IMR = RareEM.GetIMR(iGrade);
{//재료의 갯수 체크
	for(int xx= 0;MAX_IMR_FIELD_CT>xx;xx++)
	{//재료 체크 한다
		int iNationAddCt = 0;
		switch(ch->name_status.nation)
		{
		case NW_BY:{if(xx ==0){iNationAddCt = IMR.iAddCt;}}break;
		case NW_ZY:{if(xx ==1){iNationAddCt = IMR.iAddCt;}}break;
		case NW_YL:{if(xx ==2){iNationAddCt = IMR.iAddCt;}}break;
		}
		const int iNeedItemNo = IMR.aItemNo[xx];
		const int iNeedItemCt = IMR.aItemCt[xx] + iNationAddCt;
		if(iNeedItemNo && iNeedItemCt)
		{
			CItem *t = ::ItemUnit( iNeedItemNo );
			if(!t){continue;}
			const int iNowCt = ::CheckInventory(ch, iNeedItemNo, iNeedItemCt);//내가 원한 값에서 현재 결과를 주니까 -값이 나올 수 있다
			if(0 > iNowCt)//갯수 모자라네 ㅡㅡ;.
			{
				iIsHaveResource = false;
			}
		}
	}
}
	if(!iIsHaveResource)
	{
		::MyLog(1,"RecvCMD_SKILL_MASTER_MULTI_RARE_MAKE ,iIsHaveResource Check Error");
		return;
	}//재료 모자라 치워라
{//재료를 삭제

	POS &pos1 = pSMMRM->MRS.SrcData[0].pos;
	ItemAttr *pItem1 = GetItemByPOS(ch,pos1);
	
	::SendItemEventLog( pItem1, ch->GetServerID(), 0, SILT_MAKE_MULTI_RARE_RESOURCE, 1 ); //021209 lsw
	::DeleteItem(pItem1);
	::SendServerEachItem( &pos1, pItem1,ch->GetServerID());//보내기

	POS &pos2 = pSMMRM->MRS.SrcData[1].pos;
	ItemAttr *pItem2 = GetItemByPOS(ch,pos2);
	
	::SendItemEventLog( pItem2, ch->GetServerID(), 0, SILT_MAKE_MULTI_RARE_RESOURCE, 1 ); //021209 lsw
	::DeleteItem(pItem2);
	::SendServerEachItem( &pos2, pItem2,ch->GetServerID());//보내기

	for(int xx= 0;MAX_IMR_FIELD_CT>xx;xx++)//재료 뺀다
	{	
		int iNationAddCt = 0;
		switch(ch->name_status.nation)
		{
		case NW_BY:{if(xx ==0){iNationAddCt = IMR.iAddCt;}}break;
		case NW_ZY:{if(xx ==1){iNationAddCt = IMR.iAddCt;}}break;
		case NW_YL:{if(xx ==2){iNationAddCt = IMR.iAddCt;}}break;
		}
		const int iNeedItemNo = IMR.aItemNo[xx];
		const int iNeedItemCt = IMR.aItemCt[xx] + iNationAddCt;
		if(iNeedItemNo && iNeedItemCt)
		{
			CItem *t = ItemUnit( iNeedItemNo );
			if(!t){continue;}
			const int iNowCt = ::CheckInventory(ch,iNeedItemNo,iNeedItemCt);//내가 원한 값에서 현재 결과를 주니까 -값이 나올 수 있다
			if(0 <=iNowCt)//갯수가 맞으면 지워라
			{
				::SendDeleteItemQuantity( ch, iNeedItemNo, iNeedItemCt );		// 특정 아이템을 특정 갯수만큼 삭제 한다.
			}
		}
	}
}
	int a=0,b=0,c=0;
	if(::SearchInv(ch->inv,a,b,c))
	{
		const int iNow = rand()%100;
		if(45 > iNow)
		{
			POS pos;
			SetItemPos(INV,a,b,c,&pos);//pos셋팅 
			ITEMATTR *pItem = ::GetItemByPOS(ch,pos);//아이템 주소 받고
			(*pItem) = ResultItem;
			::SendItemEventLog( pItem, ch->GetServerID(), 0, SILT_MAKE_MULTI_RARE_RESULT, 1 ); //021209 lsw
			::SendServerEachItem( &pos, pItem,ch->GetServerID());//보내기
			::OutMessage(cn,2,901);//021120 lsw
		}
		else
		{//메세지 보낸다
			::OutMessage(cn,2,900);//021120 lsw
		}
	}
	else
	{//이련 경우는 없어요
		
	}
	
/*
	Send_RareItemMakeLog( cn,SourceItem.item_no,0, 
		TempSokSung.grade+1,TempSokSung.soksung1,TempSokSung.soksung2,TempSokSung.soksung3,a[0],al[0],a[1],al[1],SourceItem.attr[IATTR_LIMIT],now,max,why,2002,
		resourcelimit[0],resourcelimit[1],resourcelimit[2],
		resourcelimit[3],resourcelimit[4],resourcelimit[5]);//결과 기록
*/
//	::QueuePacket(connections, cn, &packet, 1);
	
	return;
}

//<! BBD 040308		데몬으로부터의 이벤트아이템 지급 응답시 콜되는 함수
void RecvCMD_EVENTRAREITEM_RES(const int cn, t_packet &p)
{
	const int MAX_EVENT_USER_FILED = 10;
	int nUserCn = p.u.Event_Item_List.nCn;
	
	CHARLIST *ch = CheckServerId(nUserCn);
	if(!ch)	// 캐릭터가 유효한가?
	{
		// 아이템이 날라갔다. 로그 남기자
		for(int i = 0; i < MAX_EVENT_USER_FILED; i++)
		{
			if(p.u.Event_Item_List.nIndex[i])
			{
				//i+1인 인덱스의 아이템이 날아갔다
				g_pLogManager->SaveEventItemLostLog(EILT_INVALID_CONNECTION, p.u.Event_Item_List.szChrName, i+1);
			}
		}
		return;
	}

	if(strcmp(p.u.Event_Item_List.szChrName, ch->Name))//이름이 틀려	
	{
		// 아이템이 날라갔다. 로그 남기자.
		for(int i = 0; i < MAX_EVENT_USER_FILED; i++)
		{
			if(p.u.Event_Item_List.nIndex[i])
			{
				//i+1인 인덱스의 아이템이 날아갔다
				g_pLogManager->SaveEventItemLostLog(EILT_INVALID_NAME, p.u.Event_Item_List.szChrName, i+1);
			}
		}
		return;
	}

	// 인벤의 빈칸 세기
	int blankcount = 0;
	for(int a=0; a<3; a++)
	{
		for(int b=0; b<3; b++)
		{
			for(int c=0; c<8; c++)
			{
				if( !ch->inv[a][b][c].item_no )
				{
					blankcount++;
				}
			}
		}
	}
	
	if(blankcount < 10)	// 10칸 이상인가?
	{
		::OutMessage(ch,2,13);//임시,,인벤토리 공간이 충분치 않습니다.
		// 아이템이 날라갔다. 로그 남기자.
		for(int i = 0; i < MAX_EVENT_USER_FILED; i++)
		{
			if(p.u.Event_Item_List.nIndex[i])
			{
				//i+1인 인덱스의 아이템이 날아갔다
				g_pLogManager->SaveEventItemLostLog(EILT_NOTENOUGH_INVENTORY, p.u.Event_Item_List.szChrName, i+1);
			}
		}
		return;
	}

	// 루프를 돌며지급
	for(int i = 0; i < MAX_EVENT_USER_FILED; i++)
	{
		if(p.u.Event_Item_List.item[i].item_no)
		{
			// 아이템 지급하자
			int a=0,b=0,c=0;
			if(::SearchInv(ch->inv,a,b,c))//공간이 있구나
			{
				POS pos;
				SetItemPos(INV,a,b,c,&pos);//pos셋팅 
				ITEMATTR *pItem = ::GetItemByPOS(ch,pos);//아이템 주소 받고
				(*pItem) = p.u.Event_Item_List.item[i];
				::SendServerEachItem( &pos, pItem,nUserCn);//보내기
				// DB에서 생성되어 온 아이템이니 DB에서 만들었다고 로그 남기자
				::SendItemEventLog( pItem, ch->GetServerID(), 0, SILT_MAKE_BY_DB, 1 );
			}
		}
	}
}
//> BBD 040308		데몬으로부터의 이벤트아이템 지급 응답시 콜되는 함수

int LoadRaregroup()
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;

	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)"Select * from rare_group order by no", SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{		
		int i = 0;
		retCode = SQLFetch(hStmt);
		while( retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
		{
			if(i >= MAX_RARE_GROUP) 
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return 0;
			}

			retCode = SQLGetData(hStmt, 1, SQL_INTEGER	 ,	&RareGroup[0][i].no					, 0, &cbValue);
			retCode = SQLGetData(hStmt, 2, SQL_CHAR		 ,	RareGroup[0][i].name					, 40, &cbValue);
			retCode = SQLGetData(hStmt, 3, SQL_CHAR		 ,	RareGroup[0][i].rareset[0].rare					, 40, &cbValue);
			retCode = SQLGetData(hStmt, 4, SQL_INTEGER	 ,	&RareGroup[0][i].rareset[0].rare_num			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 5, SQL_INTEGER	 ,	&RareGroup[0][i].rareset[0].rare_rate			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 6, SQL_CHAR		 ,	RareGroup[0][i].rareset[1].rare					, 40, &cbValue);
			retCode = SQLGetData(hStmt, 7, SQL_INTEGER	 ,	&RareGroup[0][i].rareset[1].rare_num			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 8, SQL_INTEGER	 ,	&RareGroup[0][i].rareset[1].rare_rate			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 9, SQL_CHAR		 ,	RareGroup[0][i].rareset[2].rare					, 40, &cbValue);
			retCode = SQLGetData(hStmt, 10, SQL_INTEGER	 ,	&RareGroup[0][i].rareset[2].rare_num			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 11, SQL_INTEGER	 ,	&RareGroup[0][i].rareset[2].rare_rate			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 12, SQL_CHAR	 ,	RareGroup[0][i].rareset[3].rare					, 40, &cbValue);
			retCode = SQLGetData(hStmt, 13, SQL_INTEGER	 ,	&RareGroup[0][i].rareset[3].rare_num			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 14, SQL_INTEGER	 ,	&RareGroup[0][i].rareset[3].rare_rate			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 15, SQL_CHAR	 ,	RareGroup[0][i].rareset[4].rare					, 40, &cbValue);
			retCode = SQLGetData(hStmt, 16, SQL_INTEGER	 ,	&RareGroup[0][i].rareset[4].rare_num			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 17, SQL_INTEGER	 ,	&RareGroup[0][i].rareset[4].rare_rate			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 18, SQL_CHAR	 ,	RareGroup[0][i].rareset[5].rare					, 40, &cbValue);
			retCode = SQLGetData(hStmt, 19, SQL_INTEGER	 ,	&RareGroup[0][i].rareset[5].rare_num			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 20, SQL_INTEGER	 ,	&RareGroup[0][i].rareset[5].rare_rate			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 21, SQL_CHAR	 ,	RareGroup[0][i].rareset[6].rare					, 40, &cbValue);
			retCode = SQLGetData(hStmt, 22, SQL_INTEGER	 ,	&RareGroup[0][i].rareset[6].rare_num			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 23, SQL_INTEGER	 ,	&RareGroup[0][i].rareset[6].rare_rate			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 24, SQL_CHAR	 ,	RareGroup[0][i].rareset[7].rare					, 40, &cbValue);
			retCode = SQLGetData(hStmt, 25, SQL_INTEGER	 ,	&RareGroup[0][i].rareset[7].rare_num			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 26, SQL_INTEGER	 ,	&RareGroup[0][i].rareset[7].rare_rate			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 27, SQL_CHAR	 ,	RareGroup[0][i].group_buffer_1			, 40, &cbValue);
			retCode = SQLGetData(hStmt, 28, SQL_CHAR	 ,	RareGroup[0][i].group_buffer_2			, 40, &cbValue);
			retCode = SQLGetData(hStmt, 29, SQL_CHAR	 ,	RareGroup[0][i].group_buffer_3			, 40, &cbValue);
			retCode = SQLGetData(hStmt, 30, SQL_INTEGER	 ,	&RareGroup[0][i].group_buffer_4		, 0, &cbValue);
			retCode = SQLGetData(hStmt, 31, SQL_INTEGER	 ,	&RareGroup[0][i].group_buffer_5		, 0, &cbValue);
			retCode = SQLGetData(hStmt, 32, SQL_INTEGER	 ,	&RareGroup[0][i].group_buffer_6		, 0, &cbValue);
			retCode = SQLGetData(hStmt, 33, SQL_INTEGER	 ,	&RareGroup[0][i].group_buffer_7		, 0, &cbValue);
			retCode = SQLGetData(hStmt, 34, SQL_INTEGER	 ,	&RareGroup[0][i].group_buffer_8		, 0, &cbValue);
			retCode = SQLGetData(hStmt, 35, SQL_INTEGER	 ,	&RareGroup[0][i].group_buffer_9		, 0, &cbValue);
			retCode = SQLGetData(hStmt, 36, SQL_INTEGER	 ,	&RareGroup[0][i].group_buffer_10		, 0, &cbValue);

			i++;

			retCode = SQLFetch(hStmt);
			if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
			{
			}
			else if( retCode == SQL_NO_DATA ) 
			{
				break;
			}
			else
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return 0;
			}
		}	
	}		
	else 
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return 0;
	}
	SQLFreeStmt(hStmt, SQL_DROP);		// 0308 YGI
	return 1;
}

void RareGroupSet()
{
	int i1 = 1,i2 = 1,	i3 = 1,	i4 = 1;
	for (;i1 < MAX_RARE_GROUP;i1++)
	{
		const int iGroupSet = RareGroup[1][i1].group_buffer_10;
		switch(iGroupSet)
		{
		case 2:
			{
				RareGroup[iGroupSet][i2] = RareGroup[1][i1];
				i2++;
			}break;
		case 3:
			{
				RareGroup[iGroupSet][i3] = RareGroup[1][i1];
				i3++;
			}break;
		case 4:
			{
				RareGroup[iGroupSet][i4] = RareGroup[1][i1];
				i4++;
			}break;
		default:
			continue;
			break;
		}
		
	}
}

int LoadItemFunctionGroup()
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;

	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)"Select * from Function_group order by no", SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{		
		int i = 0;
		retCode = SQLFetch(hStmt);
		while( retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
		{
			if(i >= MAX_RARE_GROUP) 
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return 0;
			}

			retCode = SQLGetData(hStmt, 1, SQL_INTEGER	 ,	&RareGroup[1][i].no					, 0, &cbValue);
			retCode = SQLGetData(hStmt, 2, SQL_CHAR		 ,	RareGroup[1][i].name					, 40, &cbValue);
			retCode = SQLGetData(hStmt, 3, SQL_CHAR		 ,	RareGroup[1][i].rareset[0].rare					, 40, &cbValue);
			retCode = SQLGetData(hStmt, 4, SQL_INTEGER	 ,	&RareGroup[1][i].rareset[0].rare_num			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 5, SQL_INTEGER	 ,	&RareGroup[1][i].rareset[0].rare_rate			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 6, SQL_CHAR		 ,	RareGroup[1][i].rareset[1].rare					, 40, &cbValue);
			retCode = SQLGetData(hStmt, 7, SQL_INTEGER	 ,	&RareGroup[1][i].rareset[1].rare_num			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 8, SQL_INTEGER	 ,	&RareGroup[1][i].rareset[1].rare_rate			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 9, SQL_CHAR		 ,	RareGroup[1][i].rareset[2].rare					, 40, &cbValue);
			retCode = SQLGetData(hStmt, 10, SQL_INTEGER	 ,	&RareGroup[1][i].rareset[2].rare_num			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 11, SQL_INTEGER	 ,	&RareGroup[1][i].rareset[2].rare_rate			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 12, SQL_CHAR	 ,	RareGroup[1][i].rareset[3].rare					, 40, &cbValue);
			retCode = SQLGetData(hStmt, 13, SQL_INTEGER	 ,	&RareGroup[1][i].rareset[3].rare_num			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 14, SQL_INTEGER	 ,	&RareGroup[1][i].rareset[3].rare_rate			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 15, SQL_CHAR	 ,	RareGroup[1][i].rareset[4].rare					, 40, &cbValue);
			retCode = SQLGetData(hStmt, 16, SQL_INTEGER	 ,	&RareGroup[1][i].rareset[4].rare_num			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 17, SQL_INTEGER	 ,	&RareGroup[1][i].rareset[4].rare_rate			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 18, SQL_CHAR	 ,	RareGroup[1][i].rareset[5].rare					, 40, &cbValue);
			retCode = SQLGetData(hStmt, 19, SQL_INTEGER	 ,	&RareGroup[1][i].rareset[5].rare_num			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 20, SQL_INTEGER	 ,	&RareGroup[1][i].rareset[5].rare_rate			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 21, SQL_CHAR	 ,	RareGroup[1][i].rareset[6].rare					, 40, &cbValue);
			retCode = SQLGetData(hStmt, 22, SQL_INTEGER	 ,	&RareGroup[1][i].rareset[6].rare_num			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 23, SQL_INTEGER	 ,	&RareGroup[1][i].rareset[6].rare_rate			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 24, SQL_CHAR	 ,	RareGroup[1][i].rareset[7].rare					, 40, &cbValue);
			retCode = SQLGetData(hStmt, 25, SQL_INTEGER	 ,	&RareGroup[1][i].rareset[7].rare_num			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 26, SQL_INTEGER	 ,	&RareGroup[1][i].rareset[7].rare_rate			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 27, SQL_CHAR	 ,	RareGroup[1][i].group_buffer_1			, 40, &cbValue);
			retCode = SQLGetData(hStmt, 28, SQL_CHAR	 ,	RareGroup[1][i].group_buffer_2			, 40, &cbValue);
			retCode = SQLGetData(hStmt, 29, SQL_CHAR	 ,	RareGroup[1][i].group_buffer_3			, 40, &cbValue);
			retCode = SQLGetData(hStmt, 30, SQL_INTEGER	 ,	&RareGroup[1][i].group_buffer_4		, 0, &cbValue);
			retCode = SQLGetData(hStmt, 31, SQL_INTEGER	 ,	&RareGroup[1][i].group_buffer_5		, 0, &cbValue);
			retCode = SQLGetData(hStmt, 32, SQL_INTEGER	 ,	&RareGroup[1][i].group_buffer_6		, 0, &cbValue);
			retCode = SQLGetData(hStmt, 33, SQL_INTEGER	 ,	&RareGroup[1][i].group_buffer_7		, 0, &cbValue);
			retCode = SQLGetData(hStmt, 34, SQL_INTEGER	 ,	&RareGroup[1][i].group_buffer_8		, 0, &cbValue);
			retCode = SQLGetData(hStmt, 35, SQL_INTEGER	 ,	&RareGroup[1][i].group_buffer_9		, 0, &cbValue);
			retCode = SQLGetData(hStmt, 36, SQL_INTEGER	 ,	&RareGroup[1][i].group_buffer_10		, 0, &cbValue);

			i++;

			retCode = SQLFetch(hStmt);
			if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
			{
			}
			else if( retCode == SQL_NO_DATA ) 
			{
				break;
			}
			else
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return 0;
			}
		}	
	}		
	else 
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return 0;
	}
	SQLFreeStmt(hStmt, SQL_DROP);		// 0308 YGI
	RareGroupSet();
	return 1;
}

int LoadRareProbability()
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)"Select * from rare_probability order by num", SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{		
		int i = 0;
		retCode = SQLFetch(hStmt);
		while( retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
		{
			if(i >= MAX_RARE_PROBABILIY) 
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return false;
			}
			
			retCode = SQLGetData(hStmt, 1, SQL_C_LONG,	&Rareprobability[i].num					, 0, &cbValue);
			retCode = SQLGetData(hStmt, 2, SQL_C_LONG,	&Rareprobability[i].exp_rare_suc[0]		, 0, &cbValue);
			retCode = SQLGetData(hStmt, 3, SQL_C_LONG,	&Rareprobability[i].exp_rare_suc[1]		, 0, &cbValue);
			retCode = SQLGetData(hStmt, 4, SQL_C_LONG,	&Rareprobability[i].exp_rare_suc[2]		, 0, &cbValue);
			retCode = SQLGetData(hStmt, 5, SQL_C_LONG,	&Rareprobability[i].exp_rare_suc[3]		, 0, &cbValue);
			retCode = SQLGetData(hStmt, 6, SQL_C_LONG,	&Rareprobability[i].max_rare_day		, 0, &cbValue);
			retCode = SQLGetData(hStmt, 7, SQL_C_LONG,	&Rareprobability[i].bonus_suc			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 8, SQL_C_LONG,	&Rareprobability[i].max_suc				, 0, &cbValue);
			retCode = SQLGetData(hStmt, 9, SQL_C_LONG,	&Rareprobability[i].buffer_1			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 10, SQL_C_LONG,	&Rareprobability[i].buffer_2			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 11, SQL_C_LONG,	&Rareprobability[i].buffer_3			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 12, SQL_C_LONG,	&Rareprobability[i].buffer_4			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 13, SQL_C_LONG,	&Rareprobability[i].buffer_5			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 14, SQL_C_LONG,	&Rareprobability[i].buffer_6			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 15, SQL_C_LONG,	&Rareprobability[i].buffer_7			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 16, SQL_C_LONG,	&Rareprobability[i].buffer_8			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 17, SQL_C_LONG,	&Rareprobability[i].buffer_9			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 18, SQL_C_LONG,	&Rareprobability[i].buffer_10			, 0, &cbValue);
			
			i++;

			retCode = SQLFetch(hStmt);
			if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
			{
			}
			else if( retCode == SQL_NO_DATA ) 
			{
				break;
			}
			else
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return 0;
			}
		}	
	}		
	else 
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return 0;
	}
	SQLFreeStmt(hStmt, SQL_DROP);		// 0308 YGI
	return 1;
}
/*
{
	DWORD	skillexp	:	20	;//스킬 경험치
	DWORD	makecount	:	5	;//만든 갯수
	DWORD	day 		:	8	;//저장된 날짜
	DWORD	month 		:	4	;//저장된 날짜
	DWORD	year 		:	1	;//저장된 날짜
}t_skillexp2, *LPSKILLEXP2;
typedef struct SkillExp3
{
	DWORD	skillexp	:	8	;//스킬타입
	DWORD	temp		:	2	;//스킬타입
	DWORD	year		:	9	;//년도
	DWORD	day 		:	13	;//날짜
}t_SkillExp3,*LPSKILLEXP3;
*/
int TryRareItemAbleDay(int cn,int MySkillLevel,t_skillexp2 *skillexp2)
{
	int mymax = Rareprobability[MySkillLevel].max_rare_day;
	//날짜 우선 검색	//날짜 %8 월 %8		뭔가%2
	int day		=	g_day%8;
	int month	=	g_mon%8;//0-11 까지
	/*
	원칙.
	1.오늘 날짜이면 만들 수 있다.되어야
	2.날짜가 지나면 반드시 리셋  한다.(CheckRareMakeCount)
	*/
	if(day!=skillexp2->day) {return 0;}//다른 날짜 기입 되어 있으면 튕긴다~
	else if(	month!=skillexp2->month){return 0;}//다른 달 기입 되어 있으면 튕긴다~
	else//날도 같고 달고 같고
	{
		if( mymax  <= skillexp2->makecount )//맥스에 달하는가?
		{
			OutMessage(cn,2,257);
			return 0;
		}
		else
		{
			skillexp2->makecount++;
			//만들수 있다 카운트 올라가라
			return 1;
		}
	}
	return 0;
}

int TryRareItem(const int cn, const int iSkillNo, CItem_Join join, const int iTryRare, const int iRareGruop, const int iMakeCount,const int iSuccess)//011031 lsw 
{		
	if( iTryRare  && (0 > iRareGruop || MAX_RARE_GROUP <= iRareGruop) )	return 0;

	LPCHARLIST ch = &connections[cn].chrlst;
	CItem *t ;

	int iMotherSkillNo = SkillTbl[iSkillNo].MotherSkillType;
	int TotalPercent	=	0	;
	int NowPercent		=	0	;
	int tempPercent		=	0	;
	int RareType		=   0	;
	int ItemLevel		=	join.skill1_minimum; // 경험치 플러스
	int MySkillLevel	=	ch->skillexp[iMotherSkillNo].skillexp/10000;
	int iMaxExp			=	0	;
	int iItemtakeExp	=	0	;

	t = ItemUnit( join.item_id/1000, join.item_id%1000);
	
	if(iTryRare && !t){	return 0;	}

	//자신의 레벨 보다 높으면 안되고 같으면 된다
	if(MySkillLevel < ItemLevel )	{	return 0;	}
	
	unsigned int iSuccessRate	=	ch->skillexp[iMotherSkillNo].rare+Rareprobability[MySkillLevel-ItemLevel].bonus_suc;

	const int iItemUpgradeType = (ItemMutantKind[t->GetItemKind()].upgrade_type -1);
	bool bMultiEXP = false;

	switch(t->GetItemKind())//예외적인 아이템 종류
	{
	case IK_POTION_BAG :	// BBD 040213 포션박스
	case IK_CANDLE:
	case IK_POTION:
	case IK_FOOD_MASS:
	case IK_PLATE_FOOD:
	case IK_DRINK:
	case IK_GOOD_FOOD:
	case IK_GOOD_POTION:
	case IK_GOOD_CANDLE:
		{
			bMultiEXP = true;
			if( 1 <= iItemUpgradeType && 3 >= iItemUpgradeType )
			{
				iItemtakeExp	=	Rareprobability[ItemLevel].exp_rare_suc[iItemUpgradeType];
			} 
			else
			{
				iItemtakeExp	=	Rareprobability[ItemLevel].exp_rare_suc[0];
			}
		}break;
	default:
		{
			if( iItemUpgradeType< 0 || iItemUpgradeType >3) //이거 어떻게 하니.. ㅡㅡ;; 포션 음식 양초만 오른다
			{
				return 0;//이런류의 아이템은 성공률을 주지 않는다
			}
			else
			{
				iItemtakeExp	=	Rareprobability[ItemLevel].exp_rare_suc[iItemUpgradeType];
			}
		}break;
	}	
	
	if(iTryRare && (iSuccessRate	< 0) )
	{
		ch->skillexp[iMotherSkillNo].rare = 0; //성공률이 0 이하네.. 0으로 셋팅
		return 0;	
	}

	if(iSuccessRate >1000)							{iSuccessRate=1000;}

	iMaxExp			=	Rareprobability[MySkillLevel-ItemLevel].max_suc;
	
	
	if( iTryRare )//레어 만들기 시도모드
	{
		LPSKILLEXP2 skillexp2;
		skillexp2 = (t_skillexp2*)&ch->skillexp[iMotherSkillNo-14]; 

		//랜덤 시에 0이 나오면 넘어가게끔 한다
		if(!iSuccess){return 0;}
		
		if(!TryRareItemAbleDay(cn,MySkillLevel,skillexp2))
		{
			SkillMgr.SendSkillExp(iMotherSkillNo-14, ch->skillexp[iMotherSkillNo-14], cn);
			return 0;
		}
		ch->skillexp[iMotherSkillNo].rare =0;
		SkillMgr.SendSkillExp(iMotherSkillNo-14, ch->skillexp[iMotherSkillNo-14], cn);
		if(rand()%(1001-iSuccessRate))//실패 할때 들어가는 루틴 !rand 가 아님
		{
			RareType =  0;
			SkillMgr.SendSkillExp(iMotherSkillNo, ch->skillexp[iMotherSkillNo], cn);//성공률이 부족
			return 0;//만들기 실패
		}

		for(int i = 0;  i < 8 ;i++)
		{
			TotalPercent += RareGroup[iTryRare/10][iRareGruop].rareset[i].rare_rate;
		}

		if(iTryRare && !TotalPercent) 
		{	//성공률이 부족 그룹 성공률 모자람
			return 0;	
		}//확률 모자라서 없다.
		
		NowPercent = rand()%TotalPercent;
		
		int  index = 0;
		for(; index < 8 ;index++)
		{	tempPercent += RareGroup[iTryRare/10][iRareGruop].rareset[index].rare_rate;
			if( NowPercent < tempPercent )
			{break;	}
		}
		
		RareType = RareGroup[iTryRare/10][iRareGruop].rareset[index].rare_num;//레어타입 결정

		if(RareType)
		{
			SkillMgr.SendSkillExp(iMotherSkillNo-14, ch->skillexp[iMotherSkillNo-14], cn);
		}//레어성공
	}
	else//레어 시도 안함
	{
		if( bMultiEXP) //이거 어떻게 하니.. ㅡㅡ;; 포션 음식 양초만 오른다
		{
		//	iItemtakeExp *= iMakeCount;	//갯수를 곱하기 해준다
			iItemtakeExp *= 2;	//곱하기2 로 해 준다
		}

		if( ch->skillexp[iMotherSkillNo].rare +(iItemtakeExp)> iMaxExp)
		{
				//경험치의 추가 없음
			if( ch->skillexp[iMotherSkillNo].rare > iMaxExp )//맥스 보다 높다-> 추가 없다
			{
				//맥스보다 높으면 깎지 않는다.
			}
			else//맥스와 같거나 낮다 그럼 맥스다
			{
				ch->skillexp[iMotherSkillNo].rare = iMaxExp;
			}
		}
		else
		{
			ch->skillexp[iMotherSkillNo].rare += iItemtakeExp ;
		}
	}
	SkillMgr.SendSkillExp(iMotherSkillNo, ch->skillexp[iMotherSkillNo], cn);
	return RareType;
}

int DeleteMakeItemResource( LPCHARLIST ch ,int item_no)
{
	int i,j,k;
	CItem *rit;
	ItemAttr *item;
	
	rit = ItemUnit(item_no);
	POS pos;
	if( rit )
	{
	for(i = 0;i < 3; i++ ) 
		for(j = 0;j < 3; j++ ) 
			for(k = 0; k< 8; k++ ) 
			{
				item = &ch->inv[i][j][k];
				if( item->item_no == item_no)
				{
					if( (rit->GetRbutton() == DIVIDE_ITEM))
					{
						if(item->attr[IATTR_DURATION] >0)
						{
							item->attr[IATTR_DURATION]--;
							return 1;
						}
						else
						{
							SendItemEventLog( item, ch->GetServerID(), SN_NOT_USER, SILT_RESOURCE_BY_SKILL, 4 ); //YGI acer
							DeleteItem( item );
							SetItemPos( INV, i, j, k, &pos );
							SendServerEachItem( &pos , item, ch->GetServerID());
						}
					}
					else
					{
						SendItemEventLog( item, ch->GetServerID(), SN_NOT_USER, SILT_RESOURCE_BY_SKILL, 4 ); //YGI acer
						DeleteItem( item );
						SetItemPos( INV, i, j, k, &pos );
						SendServerEachItem( &pos , item, ch->GetServerID());
						return 1;
					}
					
				}
			}
	}
	return 0;
}

int LoadItemFunction()
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;

	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)"Select * from item_function order by [no]", SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{		
		int i = 0;
		retCode = SQLFetch(hStmt);
		while( retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
		{
			if(i >= MAX_ITEM_FUNCTION) return 0;

			retCode = SQLGetData(hStmt, 1, SQL_C_LONG,	&ItemFunction[i].iNo,	0, &cbValue);
			retCode = SQLGetData(hStmt, 2, SQL_C_CHAR,	ItemFunction[i].Name,	40, &cbValue);
			retCode = SQLGetData(hStmt, 3, SQL_C_CHAR,	ItemFunction[i].NameAdd,40, &cbValue);
			retCode = SQLGetData(hStmt, 4, SQL_C_CHAR,	ItemFunction[i].Exp	,	50, &cbValue);
			retCode = SQLGetData(hStmt, 5, SQL_C_LONG,	&ItemFunction[i].ExpMark,	0, &cbValue);
			retCode = SQLGetData(hStmt, 6, SQL_C_LONG,	&ItemFunction[i].iEffectNo,	0, &cbValue);
			retCode = SQLGetData(hStmt, 7, SQL_C_LONG,	&ItemFunction[i].iUpgradeAble,	0, &cbValue);
			retCode = SQLGetData(hStmt, 8, SQL_C_LONG,	&ItemFunction[i].iScrollNo,	0, &cbValue);
			retCode = SQLGetData(hStmt, 9, SQL_C_LONG,	&ItemFunction[i].iMakeRand[0],	0, &cbValue);
			retCode = SQLGetData(hStmt, 10, SQL_C_LONG,	&ItemFunction[i].iMakeRand[1],	0, &cbValue);
			retCode = SQLGetData(hStmt, 11, SQL_C_LONG,	&ItemFunction[i].iMakeRand[2],	0, &cbValue);
			
			retCode = SQLGetData(hStmt, 12, SQL_C_LONG,	&ItemFunction[i].iGrade[0],	0, &cbValue);
			retCode = SQLGetData(hStmt, 13, SQL_C_LONG,	&ItemFunction[i].iGrade[1],	0, &cbValue);
			retCode = SQLGetData(hStmt, 14, SQL_C_LONG,	&ItemFunction[i].iGrade[2],	0, &cbValue);
			retCode = SQLGetData(hStmt, 15, SQL_C_LONG,	&ItemFunction[i].iGrade[3],	0, &cbValue);
			retCode = SQLGetData(hStmt, 16, SQL_C_LONG,	&ItemFunction[i].iGrade[4],	0, &cbValue);
			retCode = SQLGetData(hStmt, 17, SQL_C_LONG,	&ItemFunction[i].iGrade[5],	0, &cbValue);
			retCode = SQLGetData(hStmt, 18, SQL_C_LONG,	&ItemFunction[i].iGrade[6],	0, &cbValue);
			retCode = SQLGetData(hStmt, 19, SQL_C_LONG,	&ItemFunction[i].iGrade[7],	0, &cbValue);
			retCode = SQLGetData(hStmt, 20, SQL_C_LONG,	&ItemFunction[i].iGrade[8],	0, &cbValue);
			retCode = SQLGetData(hStmt, 21, SQL_C_LONG,	&ItemFunction[i].iGrade[9],	0, &cbValue);
			retCode = SQLGetData(hStmt, 22, SQL_C_LONG,	&ItemFunction[i].iGrade[10],	0, &cbValue);
			retCode = SQLGetData(hStmt, 23, SQL_C_LONG,	&ItemFunction[i].iGrade[11],	0, &cbValue);
			retCode = SQLGetData(hStmt, 24, SQL_C_LONG,	&ItemFunction[i].iGrade[12],	0, &cbValue);
			retCode = SQLGetData(hStmt, 25, SQL_C_LONG,	&ItemFunction[i].iGrade[13],	0, &cbValue);
			retCode = SQLGetData(hStmt, 26, SQL_C_LONG,	&ItemFunction[i].iGrade[14],	0, &cbValue);
			retCode = SQLGetData(hStmt, 27, SQL_C_LONG,	&ItemFunction[i].iGrade[15],	0, &cbValue);
			retCode = SQLGetData(hStmt, 28, SQL_C_LONG,	&ItemFunction[i].iGrade[16],	0, &cbValue);

			retCode = SQLGetData(hStmt, 29, SQL_C_LONG,	&ItemFunction[i].iTime[0],	0, &cbValue);
			retCode = SQLGetData(hStmt, 30, SQL_C_LONG,	&ItemFunction[i].iTime[1],	0, &cbValue);
			retCode = SQLGetData(hStmt, 31, SQL_C_LONG,	&ItemFunction[i].iTime[2],	0, &cbValue);
			retCode = SQLGetData(hStmt, 32, SQL_C_LONG,	&ItemFunction[i].iTime[3],	0, &cbValue);
			retCode = SQLGetData(hStmt, 33, SQL_C_LONG,	&ItemFunction[i].iTime[4],	0, &cbValue);
			retCode = SQLGetData(hStmt, 34, SQL_C_LONG,	&ItemFunction[i].iTime[5],	0, &cbValue);
			retCode = SQLGetData(hStmt, 35, SQL_C_LONG,	&ItemFunction[i].iTime[6],	0, &cbValue);
			retCode = SQLGetData(hStmt, 36, SQL_C_LONG,	&ItemFunction[i].iTime[7],	0, &cbValue);
			retCode = SQLGetData(hStmt, 37, SQL_C_LONG,	&ItemFunction[i].iTime[8],	0, &cbValue);
			retCode = SQLGetData(hStmt, 38, SQL_C_LONG,	&ItemFunction[i].iTime[9],	0, &cbValue);
			retCode = SQLGetData(hStmt, 39, SQL_C_LONG,	&ItemFunction[i].iTime[10],	0, &cbValue);
			retCode = SQLGetData(hStmt, 40, SQL_C_LONG,	&ItemFunction[i].iTime[11],	0, &cbValue);
			retCode = SQLGetData(hStmt, 41, SQL_C_LONG,	&ItemFunction[i].iTime[12],	0, &cbValue);
			retCode = SQLGetData(hStmt, 42, SQL_C_LONG,	&ItemFunction[i].iTime[13],	0, &cbValue);
			retCode = SQLGetData(hStmt, 43, SQL_C_LONG,	&ItemFunction[i].iTime[14],	0, &cbValue);
			retCode = SQLGetData(hStmt, 44, SQL_C_LONG,	&ItemFunction[i].iTime[15],	0, &cbValue);
			retCode = SQLGetData(hStmt, 45, SQL_C_LONG,	&ItemFunction[i].iTime[16],	0, &cbValue);

			retCode = SQLGetData(hStmt, 46, SQL_C_LONG,	&ItemFunction[i].iBuffer1,	0, &cbValue);
			retCode = SQLGetData(hStmt, 47, SQL_C_LONG,	&ItemFunction[i].iBuffer2,	0, &cbValue);
			
			i++;
			retCode = SQLFetch(hStmt);
			if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
			{
			}
			else if( retCode == SQL_NO_DATA ) 
			{
				break;
			}
			else
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return 0;
			}
		}	
	}		
	else 
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return 0;
	}
	SQLFreeStmt(hStmt, SQL_DROP);		// 0308 YGI
	return 1;
}

void Send_RareItemMakeLog(	const int cn,				const int itemno,
							const int today_count,		const int grade,
							const int mutanttype1,		const int mutanttype2,
							const int mutanttype3,		const int addeditem1,
							const int addeditem1limit,	const int addeditem2,
							const int addeditem2limit,	const int resultlimit,
							const int resultnowdur,		const int resultmaxdur,
							const int why,				const int why2,
							const int resource1limit,	const int resource2limit,
							const int resource3limit,	const int resource4limit,
							const int resource5limit,	const int resource6limit)
{
	if( cn < DRAGON_CONNECTIONS_START || cn >= DRAGON_MAX_CONNECTIONS ) return;
	if( connections[cn].chrlst.SprType == SPRITETYPE_NPC ) return;
	t_packet p;
	t_rare_item_make_log *tp;

	p.h.header.type = CMD_RARE_ITEM_MAKE_LOG;
	p.h.header.size = sizeof( t_rare_item_make_log );
	tp = &p.u.rare_item_make_log;
	memcpy( tp->maker, connections[cn].name, 20 );
	tp->map = MapInfo[MapNumber].port;

	tp->itemno			=	itemno;
	tp->today_count		=	today_count;
	tp->grade			=	grade;
	tp->mutanttype1		=	mutanttype1;
	tp->mutanttype2		=	mutanttype2;
	tp->mutanttype3		=	mutanttype3;
	tp->addeditem1		=	addeditem1;
	tp->addeditem1limit	=	addeditem1limit;
	tp->addeditem2		=	addeditem2;
	tp->addeditem2limit	=	addeditem2limit;
	tp->resultlimit		=	resultlimit;
	tp->resultnowdur	=	resultnowdur;
	tp->resultmaxdur	=	resultmaxdur;
	tp->why				=	why;
	tp->why2			=	why2;
	tp->resource1limit	=	resource1limit;
	tp->resource2limit	=	resource2limit;
	tp->resource3limit	=	resource3limit;
	tp->resource4limit	=	resource4limit;
	tp->resource5limit	=	resource5limit;
	tp->resource6limit	=	resource6limit;
	QueuePacket( connections, DB_DEMON, &p, 1 );

	return;
}

int LoadLearnItemConvetrer()
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;

	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)"Select * from LearnItemConvert", SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{		
		int i = 0;
		retCode = SQLFetch(hStmt);
		while( retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
		{
			if(i >= MAX_LEARN_ITEM_CONVERT) return 0;

			retCode = SQLGetData(hStmt, 1, SQL_INTEGER	 ,	&LIC[i].iBeforeItemNo			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 2, SQL_INTEGER	 ,	&LIC[i].iAfterItemNo			, 0, &cbValue);

			i++;
			retCode = SQLFetch(hStmt);
			if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
			{
			}
			else if( retCode == SQL_NO_DATA ) 
			{
				break;
			}
			else
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return 0;
			}
		}	
	}		
	else 
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return 0;
	}
	SQLFreeStmt(hStmt, SQL_DROP);		// 0308 YGI
	return 1;
}



bool LoadRareItemBag()
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;

	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)"Select * from RareItemBag order by [no]", SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{		
		int i = 1;
		retCode = SQLFetch(hStmt);
		while( retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
		{
			if(i >= MAX_RAREITEMBAG)
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return false;
			}
/*
	typedef struct RareItemBag
	{
		int iNo;
		int RareItemBagPercentTableNo;
		int iGradeMin;
		int iGradeMax;
		int ItemNo[21];//0 번은 없는 값
	}RareItemBag;
*/
			int column = 0;
			retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&RareItemBag[i].iNo,	0, &cbValue);
			retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&RareItemBag[i].iRareItemBagPercentTableNo,	0, &cbValue);
			retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&RareItemBag[i].iGradeMin,	0, &cbValue);
			retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&RareItemBag[i].iGradeMax,	0, &cbValue);
			
			int iTotal = 0;
			for(int count= 1; count <21; count ++)
			{
				int iDummy = 0;
				retCode = SQLGetData(hStmt,  ++column, SQL_C_LONG,	&iDummy,	0, &cbValue);
				RareItemBag[i].ItemNo[count] = iDummy;
				iTotal+=iDummy;
			}
			RareItemBag[i].ItemNo[0] = iTotal;
			i++;
			retCode = SQLFetch(hStmt);
			if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
			{
			}
			else if( retCode == SQL_NO_DATA ) 
			{
				break;
			}
			else
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return false;
			}
		}	
	}		
	else 
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return false;
	}
	SQLFreeStmt(hStmt, SQL_DROP);		// 0308 YGI
	return true;
}	
bool LoadItemControlPercent()
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;

	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)"Select * from ItemControlPercent order by [no]", SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{		
		int i = 1;
		retCode = SQLFetch(hStmt);
		while( retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
		{
			if(i >= MAX_ITEMCONTROLPERCENT)
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return false;
			}
/*
			typedef struct ItemControlPercent
			{
				int iNo		
				int	Per[8];//0번은 토탈값 1~5는 베이직 그룹 6~7은 레어 그룹
			}ItemControlPercent;
*/
			int column = 0;
			retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&ItemControlPercent[i].iNo,	0, &cbValue);
			int iTotal = 0;
			for(int count= 1; count <8; count ++)
			{
				int iDummy = 0;
				retCode = SQLGetData(hStmt,  ++column, SQL_C_LONG,	&iDummy,	0, &cbValue);
				ItemControlPercent[i].per[count] = iDummy;
				iTotal+=iDummy;
			}
			ItemControlPercent[i].per[0] = iTotal;
			i++;
			retCode = SQLFetch(hStmt);
			if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
			{
			}
			else if( retCode == SQL_NO_DATA ) 
			{
				break;
			}
			else
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return false;
			}
		}	
	}		
	else 
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return false;
	}
	SQLFreeStmt(hStmt, SQL_DROP);		// 0308 YGI
	return true;
	
}

bool LoadItemFallPercent()//밥먹고 만들자~ 
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;

	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)"Select * from ItemFallPercent order by [no]", SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{		
		int i = 1;
		retCode = SQLFetch(hStmt);
		while( retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
		{
			if(i >= MAX_ITEMFALLPERCENT) return false;
/*
			typedef struct ItemFallPercent
			{
				int iNo
				int per[21];//0 번은 토탈 값 이다
			}ItemFallPercent;
*/
			int column = 0;
			retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&ItemFallPercent[i].iNo,	0, &cbValue);
			int iTotal = 0;
			for(int count= 1; count <21; count ++)
			{
				int iDummy = 0;
				retCode = SQLGetData(hStmt,  ++column, SQL_C_LONG,	&iDummy,	0, &cbValue);
				ItemFallPercent[i].per[count] = iDummy;
				iTotal+=iDummy;
			}
			ItemFallPercent[i].per[0] = iTotal;
			i++;
			retCode = SQLFetch(hStmt);
			if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
			{
			}
			else if( retCode == SQL_NO_DATA ) 
			{
				break;
			}
			else
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return false;
			}
		}	
	}		
	else 
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return false;
	}
	SQLFreeStmt(hStmt, SQL_DROP);		// 0308 YGI
	return true;
}
bool LoadBasicItemBag()
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;

	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)"Select * from BasicItemBag order by [no]", SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{		
		int i = 1;
		retCode = SQLFetch(hStmt);
		while( retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
		{
			if(i >= MAX_BASICITEMBAG) 
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return false;
			}
/*
			typedef struct BasicItemBag
			{
				int iNo;
				int BasicItemPercentTableNo;
				int ItemNo[21];//0번은 없는값
			}BasicItemBag;
*/
			int column = 0;
			retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&BasicItemBag[i].iNo,	0, &cbValue);
			retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&BasicItemBag[i].iBasicItemPercentTableNo,	0, &cbValue);
//			int iTotal = 0;
			for(int count= 1; count <21; count ++)
			{
				int iDummy = 0;
				retCode = SQLGetData(hStmt,  ++column, SQL_C_LONG,	&iDummy,	0, &cbValue);
				BasicItemBag[i].ItemNo[count] = iDummy;
//				iTotal+=iDummy;
			}
			i++;
			retCode = SQLFetch(hStmt);
			if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
			{
			}
			else if( retCode == SQL_NO_DATA ) 
			{
				break;
			}
			else
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return false;
			}
		}	
	}		
	else 
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return false;
	}
	SQLFreeStmt(hStmt, SQL_DROP);		// 0308 YGI
	return true;
}

bool LoadItemControl()
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;

	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)"Select * from ItemControl order by [no]", SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{		
		int i = 1;
		retCode = SQLFetch(hStmt);
		while( retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
		{
			if(i >= MAX_ITEMCONTROL) 
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return false;
			}
/*
			typedef struct ItemControl
			{
				int iNo;
				int ItemControlPercentNo;
				int ItemGroup[8];//0번은 없는값 1~5는 베이직 그룹 6~7은 레어 그룹
				int	iFallItemCount;//떨어질 갯수, 즉, 반복문 호출 횟수
			}ItemControl;
*/
			int column = 0;
			retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&ItemControl[i].iNo,	0, &cbValue);
			retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&ItemControl[i].ItemControlPercentNo,	0, &cbValue);
//			int iTotal = 0;
			for(int count= 1; count <8; count ++)
			{
				int iDummy = 0;
				retCode = SQLGetData(hStmt,  ++column, SQL_C_LONG,	&iDummy,	0, &cbValue);
				ItemControl[i].ItemGroup[count] = iDummy;
//				iTotal+=iDummy;
			}
			retCode = SQLGetData(hStmt, ++column, SQL_C_LONG,	&ItemControl[i].iFallItemCount,	0, &cbValue);
			
			i++;
			retCode = SQLFetch(hStmt);
			if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
			{
			}
			else if( retCode == SQL_NO_DATA ) 
			{
				break;
			}
			else
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return false;
			}
		}	
	}		
	else 
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return false;
	}
	SQLFreeStmt(hStmt, SQL_DROP);		// 0308 YGI
	return true;
}
extern void SendGameToClientWhoSendMail( char *name );
int HandleCommand2( t_packet *p, t_connection c[], int cn ) //This function seems to handle alot of packets which are meant to be sent only by dbdemon, adding a source-check would be nice to prevent cheating - Thralas
{
	switch( p->h.header.type )
	{
	case	CMD_CONFORM_SADONIX:
		{
			if(cn == DB_DEMON)
			{
				SkillMgr.RecvCMD_CONFORM_SADONIX(*p);
			}
			else
			{
				MyLog(LOG_NORMAL, "CMD_CONFORM_SADONIX received from a non-dbdemon connection, cheat attempt?!");
			}
		}break;
	case	CMD_MOVE_TO_HOMETOWN:
		{
			if(g_MapPort != 5300)
			{
				CHARLIST *ch = CheckServerId(cn);
				if (ch)
				{
					if (ENABLE_HOMETOWN_LEVEL >= ch->GetLevel() && 0 < ch->GetLevel())
					{	//< CSD-030806
						MoveToHomeTown(ch);
					}	//> CSD-030806
				}
			}
		}break;
	case	CMD_MOVE_TO_HOMETOWN_WITH_ALIVE:
		{
			if(g_MapPort != 5300 && g_MapPort != 5830)//초보자 수련소가 아니라면 //020827 lsw
			{
				CHARLIST *ch = CheckServerId(cn);
				if (ch)
				{
					if (ENABLE_HOMETOWN_LEVEL >= ch->GetLevel() && 0 < ch->GetLevel())
					{	//< CSD-030806
						MoveToHomeTownWithAilve(ch);
					}	//> CSD-030806
				}
			}
		}break;
	case	CMD_COMFORM_RESET_ABILILTY:
		{
			RecvCMD_COMFORM_RESET_ABILITY(*p);
		}break;
	case	CMD_MERCHANT_BUY_ITEM_SEARCH_CLIENT:
		{
			Auction.RecvCMD_MERCHANT_BUY_LIST_REQUEST(cn,*p);
		}break;
	case	CMD_MERCHANT_BUY_ITEM_SEARCH_RESULT:
		{
			if(cn == DB_DEMON)
			{
				Auction.RecvCMD_MERCHANT_BUY_LIST_REQUEST_RESULT(*p);	//디비데몬꺼만 인정
			}
			else
			{
				MyLog(0,"Crack CMD_MERCHANT_BUY_ITEM_SEARCH_RESULT");
			}
		}break;
	case	CMD_MERCHANT_SELL_LIST_REQUEST_RESULT:
		{
			if(cn == DB_DEMON)
			{
				Auction.RecvCMD_MERCHANT_SELL_LIST_REQUEST_RESULT(*p);	//디비데몬꺼만 인정
			}
			else
			{
				MyLog(0,"Crack CMD_MERCHANT_SELL_LIST_REQUEST_RESULT");
			}
		}break;
	case	CMD_MERCHANT_SELL_ITEM_INFO_REQUEST://클라이언트 상인이 보내는 패킷
		{
			Auction.RecvCMD_MERCHANT_SELL_LIST_REQUEST(cn,*p);
		}break;
	case CMD_MERCHANT_ITEM_BUY	://클라이언트로 부터 받고
		{
			Auction.RecvCMD_MERCHANT_ITEM_BUY(cn,*p);
		}break;
	case CMD_MERCHANT_ITEM_BUY_RESULT:
		{
			if(cn == DB_DEMON)
			{
				Auction.RecvCMD_MERCHANT_ITEM_BUY_RESULT(*p);	//디비데몬꺼만 인정
			}
			else
			{
			}
		}break;
	case CMD_MERCHANT_ITEM_BUY_COMFORM_RESULT://디비데몬이 IsEnd를 체크 하고 응답을 주는곳
		{
			if(cn == DB_DEMON)
			{
				Auction.RecvCMD_MERCHANT_ITEM_BUY_COMFORM_RESULT(cn,*p);	//디비데몬꺼만 인정
			}
			else
			{
				MyLog(0,"Crack CMD_MERCHANT_ITEM_BUY_COMFORM_RESULT");
			}
			
		}break;
	case CMD_MERCHANT_SELL_ITEM_REGISTER://클라이언트로부터 아이템 등록 패킷을 받아냅니다.
		{
			Auction.RecvCMD_MERCHANT_SELL_ITEM_REGISTER(cn,*p);
		}break;
	case CMD_MERCHANT_SELL_ITEM_REGISTER_RESULT://판매아이템을 등록한 결과를 디비로 부터 받습니다..
		{
			if(cn == DB_DEMON)
			{
				Auction.RecvCMD_MERCHANT_SELL_ITEM_REGISTER_RESULT(*p);	//디비데몬꺼만 인정
			}
			else{MyLog(0,"Crack CMD_MERCHANT_ITEM_BUY_COMFORM_RESULT");}
		}break;
	case CMD_MERCHANT_SELL_ITEM_REGISTER_COMFORM_RESULT:
		{
			if(cn == DB_DEMON)
			{
				Auction.RecvCMD_MERCHANT_SELL_ITEM_REGISTER_COMFORM_RESULT(cn,*p);	//디비데몬꺼만 인정
			}
			else{MyLog(0,"Crack CMD_MERCHANT_SELL_ITEM_REGISTER_COMFORM_RESULT");}
		}break;
		//삭제 하기 
	case CMD_MERCHANT_SELL_ITEM_DELETE://클라이언트로 부터 판매 취소를 받는다
		{
			Auction.RecvCMD_MERCHANT_SELL_ITEM_DELETE(cn,*p);
		}break;
	case CMD_MERCHANT_SELL_ITEM_DELETE_RESULT://디비로 부터 취소 결과를 받아 클라이언트에게 보낸다 
		{
			Auction.RecvCMD_MERCHANT_SELL_ITEM_DELETE_RESULT(cn,*p);
		}break;
	case CMD_MERCHANT_SELL_ITEM_DELETE_COMFORM_RESULT://디비로 부터 판매취소한 결과를 받는다
		{
			Auction.RecvCMD_MERCHANT_SELL_ITEM_DELETE_COMFORM_RESULT(cn,*p);
		}break;
	//거래 결과 리스트 >
	case CMD_MERCHANT_RESULT_LIST_REQUEST://리스트를 요청 받았습니다.(클라이언트로 부터)
		{
			Auction.RecvCMD_MERCHANT_RESULT_LIST_REQUEST(cn,*p);
		}break;
	case CMD_MERCHANT_RESULT_LIST_REQUEST_RESULT://결과를 받았읍니다(디비로 부터)
		{
			Auction.RecvCMD_MERCHANT_RESULT_LIST_REQUEST_RESULT(*p);
		}break;
	//거래 결과 리스트 < 

	case CMD_MERCHANT_RESULT_TAKE://가져가겠답니다.
		{
			Auction.RecvCMD_MERCHANT_RESULT_TAKE(cn,*p);
		}break;
	case CMD_MERCHANT_RESULT_TAKE_RESULT://결과물을 가져가는 결과를 받습니다.(디비로 부터
		{
			if(cn == DB_DEMON)
			{
				Auction.RecvCMD_MERCHANT_RESULT_TAKE_RESULT(*p);
			}
			else{MyLog(0,"Crack CMD_MERCHANT_RESULT_TAKE_RESULT");}
		}break;
	case CMD_MERCHANT_RESULT_TAKE_COMFORM_RESULT:
		{
			if(cn == DB_DEMON)
			{
				Auction.RecvCMD_MERCHANT_RESULT_TAKE_COMFORM_RESULT(cn,*p);
			}
			else{MyLog(0,"Crack CMD_MERCHANT_RESULT_TAKE_COMFORM_RESULT");}				
		}break;
//<! BBD 040303	 머천트백업 테이블로부터 물건을 찾겠다는 메시지
	case CMD_MERCHANT_BACKUP_TAKE_REQUEST:
		{
			if(cn == DB_DEMON)
			{
				Auction.RecvCMD_MERCHANT_BACKUP_TAKE_REQUEST(cn, *p);
			}
			else
			{
				MyLog(0,"Crack CMD_MERCHANT_BACKUP_TAKE_REQUEST");
			}
		}break;
//> BBD 040303	 머천트백업 테이블로부터 물건을 찾겠다는 메시지
//<! BBD 040308		이벤트 아이템 지급 응답
	case CMD_EVENTRAREITEM_RES:
		{
			if(cn == DB_DEMON)
			{
				RecvCMD_EVENTRAREITEM_RES(cn, *p);
			}
			else
			{
				MyLog(0,"Crack CMD_EVENTRAREITEM_RES");
			}
			break;
		}
//> BBD 040308		이벤트 아이템 지급 응답
	case CMD_MERCHANT_DIRECT_EXCHANGE_LIST_REQUSET:
		{
			Auction.RecvCMD_MERCHANT_DIRECT_EXCHANGE_LIST_REQUSET(cn,*p);
		}break;
	case CMD_MERCHANT_DIRECT_EXCHANGE_LIST_REQUSET_RESULT:
		{
			Auction.RecvCMD_MERCHANT_DIRECT_EXCHANGE_LIST_REQUSET_RESULT(*p);
		}break;
	case CMD_EXCHANGE_ITEM_READY		:	
		{
			::RecvItemExchange( &p->u.kein.exchange_item_start.item, p->u.kein.exchange_item_start.you_id, cn ,EXCHANGE_TYPE_NORMAL); 
		}break;
	case CMD_EXCHANGE_OK_SIGN			:	
		{
			RecvExchangeStateSign( p->u.kein.client_other_ch_inv.other_ch, cn , true); 
		}break;
	case CMD_EXCHANGE_CANCEL_SIGN:
		{
			RecvExchangeStateSign( p->u.kein.client_other_ch_inv.other_ch, cn , false); 
		}break;
	case CMD_EXCHANGE_ITEM_EACH			:	
		{
			RecvExchangeEach( &p->u.kein.exchange_item_start.item, p->u.kein.exchange_item_start.you_id, cn ); 
		}break;
	case CMD_EXCHANGE_CANCEL			:	
		{
			CHARLIST *pCh = ::CheckServerId(cn);
			if(!pCh){break;}
			::SendExchangeCancel( pCh->GetExchangeTargetId(), cn ); 				
		}break;
	case CMD_EXCHANGE_ITEM_DEL			:	
		{
			RecvExchangeItemDel( p->u.kein.exchange_item_del.pos, p->u.kein.exchange_item_del.you_id, cn ); 
		}break;
	case CMD_SKILL_MASTER_MULTI_RARE_MAKE:
		{
			RecvCMD_SKILL_MASTER_MULTI_RARE_MAKE(cn,p);
		}break;
	case CMD_EXCHANGE_BOND_MONEY://021126 lsw
		{
			Auction.RecvCMD_EXCHANGE_BOND_MONEY(cn,*p);
		}break;
	case CMD_TRADE_MODE_CHANGE://030127 lsw
		{ 
			::RecvCMD_TRADE_MODE_CHANGE(cn,p);
		}break;
	case CMD_MAIL_REQUEST_LIST://이름으로 메일 검색
		{
			g_MailMgr.RecvReqMailList(cn,p);
		}break;
	case CMD_MAIL_LIST_FROM_DBDEMON://디비데몬으로 부터 받은 결과
		{
			g_MailMgr.RecvMailListFromDBDEMON(cn,p);
		}break;
	case CMD_MAIL_SEND:	
		{
			g_MailMgr.Recv(cn,p); 
		}break;
	case CMD_MAIL_DELETE:
		{
			g_MailMgr.RecvDelete(cn,p); 
		}break;
	case CMD_MAIL_REPAIR:
		{
			g_MailMgr.RecvRepair(cn,p); 
		}
	case CMD_MAIL_REQ_BODY:
		{
			g_MailMgr.RecvRequestBody(cn,p); 
		}break;
	case CMD_MAIL_REQ_BODY_RESULT:
		{
			g_MailMgr.RecvRequestBodyResult(cn,p); 
		}break;
	case CMD_MAIL_ALERT						:	SendGameToClientWhoSendMail( p->u.kein.who_send_mail.name ); break;
	case CMD_SEND_MAIL_OK					:
	case CMD_SEND_MAIL_FAIL					:	ReturnResultByName( p->u.kein.who_send_mail.name, p->h.header.type ); break;	// 010719 YGI
	default:
		{
			return 0;
		}break;
	}
	return 1;
}