#include "..\stdafx.h"
#include "DefaultHeader.h"

#include "../LowerLayers/mylog.h" //!!
#include "CItem.h"
#include "WeatherControl.h"
#include "Map.h"
#include "Path.h"
#include "OP_Magic.h"
#include "SealStoneGameServer.h"
#include "SealStone.h"
#include "scrp_exe.h"
#include "teambattle.h"
#include "QuestFunction.h"	// 021022 kyo 
#include "LogManager.h"
#include "AIManager.h"
#include "ArenaManager.h"
#include "RegenManager.h"
#include "UserManager.h"
#include "Chatting.h" // For counselor lms chat
#include "eventmgr.h" //For Exp Events - Eleval 22/07/09

extern void CountNationDeath(CHARLIST* pCaster,CHARLIST* pTarget);
extern void CheckLocalWarDeath(CHARLIST* pCaster,CHARLIST* pTarget);	// LTS LOCALWAR
extern void CheckEventLocalWarDeath(CHARLIST* Attacker,CHARLIST* Defencer);		// 020115 LTS
extern bool isNationWarfieldServer();
extern bool IsWar();
extern void UpdateGuardStoneStatus(LPCHARLIST pCaster,LPCHARLIST pTarget);
extern bool isNewWarfieldServer();
extern void CountNewWarfieldDeath(LPCHARLIST Attacker,LPCHARLIST Defencer);
extern void SendBBS2( char *text, short int cn );

extern void CheckIfQuestItem( CHARLIST *pCaster, CHARLIST *pTarget); //Eleval


// 030506 YGI
extern char g_LogoutPath[80];
extern char g_ChangeLogPath[80];

// tactics skill definition
#define FIST	0
#define SWORD	1
#define AXE		2
#define MACE	3
#define STAFF	4
#define SPEAR	5
#define THR_SW	6
#define BOW		7
#define TWO_SW	8
#define TWO_AX	9
#define SHIELD	10
#define FENCING	11


#define PLAYER	0
#define NPC		1

#define MELEE			0	// 단거리.
#define RANGE_BOW		1	// 장거리..
#define RANGE_HURL		2	// 중거리.
#define RANGE_DEFAULT	3	// 단거리..
#define MAGIC			4	// 마법.

//Define added by Eleval 25/05/09 for the log with the Monster Bug
#define WG_DEBUG
//End of it

extern NPCName_by_Gender	NPC_Name_Ref[Num_Of_NPC_Name];
extern NPCLev_to_Exp		NPC_Lev_Ref[Num_Of_NPC_Lev] ;
extern NPC_Generation		NPC_Gen_Ref[Num_Of_NPC_Generation] ;

// 010129 
extern NPC_Generation		NPC_Gen_Ref[Num_Of_NPC_Generation];
extern MAGICTABLE Magic_Ref[] ;

extern int GetEnemyEnemy( CHARLIST *a, CHARLIST *d );
extern bool isLevelUp(CHARLIST *chr);
extern void getItemIndex(int, int &, int &) ;


extern void SendServerKillPc( short int cn );			//
///////////////////// 0811 yjs 추가 ////////////////////////

extern bool IsWar();	// 010904 LTS								// 국가전 상황에서 전쟁이 아니면 성문 수호석 미스
extern bool CanDestroyTarget(CHARLIST* pCaster,CHARLIST* pTarget);  // 010904 LTS	// 방어자의 성문 수호석 미스
extern void CheckBossAndDeActiveGroupNo(CHARLIST* Npc);				// LTS AI2

void rollDice(int base_hd, int &dice_res);
int  getDistance(CHARLIST *a, CHARLIST *d);
int  calcClassWeaponCorrection( CHARLIST * a, int itemtype );	// 클래스에 맞는 Item을 사용했을경우 20%증가한다. 
short int getWeatherCorrection( void );
int  calcRangeCorrection(CHARLIST *a, CHARLIST *d );
int  AttackMapper( int a_id, int d_id,  int *damage_val);
void SendNK( int id, int nk );
void SendNK( int id, int nk3, int nk4, int nk6 );
inline int  IsHeNK(  LPCHARLIST ch, int nation  );
inline int	OutNK( LPCHARLIST ch, int nation );
void		InNK( LPCHARLIST ch, int nation, short int add_nk );
inline bool IsWarfare( int anation, int bnation );
int			GetCaseOfNK( int cur_nation, int a_nation, int d_nation, int a_nk, int d_nk ) ;
void		CheckNK( int a_id, int d_id, int type  );
extern void CheckDragonLordWarTeam(CHARLIST *a,CHARLIST *d);			// LTS DRAGONLORD



// 020428 YGI acer
extern int SaveLogDefault( char *file_name, char *str, int is_first );
extern void SaveCharacterBin( char *filename, CHARLIST *ch );

///////////////////////////////////////////////////////////////////////////////////////
///
///		User Function Definitions....
///

inline int ran( int r )
{
	if( r == 0 ) return 0;
	return rand()%r;
}

void rollDice(int base_hd, int &dice_res)		// 0712 YGI
{	
	int dice_type, dice_repeat;
	int i ;
	
	int plus = 0;
	if( base_hd > 1000000 ) 
	{
		plus = base_hd % 1000;
		base_hd /= 1000;
	}
	
	dice_type	= base_hd % 1000 ;
	dice_repeat	= base_hd / 1000 ;
	
	dice_res= 0 ;
	for(i= 0 ; i < dice_repeat ; i++) {
		dice_res+= (ran(dice_type) + 1);
	}
	dice_res += plus;
}	

int getDistance(CHARLIST *a, CHARLIST *d)
{
	double dist ;
	dist= pow((float)(a->X - d->X), (float)2) + pow((float)(a->Y - d->Y), (float)2) ;
	dist= sqrt(dist) ;
	
	//return -1 ;
	return (int)dist / TILE_SIZE ;
}	

int calcClassWeaponCorrection( CHARLIST * a, int itemtype )	
{				
	int item_no= a->equip[ itemtype].item_no ;
	int type, no;
				
	getItemIndex( item_no, type, no );
	if( item_no <= 0 ) return 80;
				
	CItem *ii = ItemUnit( type, no );
	if( ii == NULL ) return 80;
	else		
	{			
		if ( itemtype == WT_WEAPON || itemtype == WT_SHIELD ) 
		{		
			switch( a->Class )
			{	
			case  WARRIOR	:	return CWeapon[no].Class_Warrior * 20 + 80;
			case  THIEF		:	return CWeapon[no].Class_Archer  * 20 + 80;
			case  ARCHER	:	return CWeapon[no].Class_Thief   * 20 + 80;
			case  WIZARD	:	return CWeapon[no].Class_Wizard  * 20 + 80;
			case  PRIEST	:	return CWeapon[no].Class_Cleric  * 20 + 80;
			default : return 80;
			}	
		}		
	}				
				
	return 80;
}					

static bool	bAttackChangeCondition_slow_i = false;
static bool	bAttackChangeCondition_slow_you = false;
static bool	bAttackChangeCondition_daze_i  =false;
static bool	bAttackChangeCondition_daze_you  =false;
// 클래스-무기별 보정: 1.전사(단거리:100/중거리:100/장거리:90) 2.도적(단거리:90/중거리:100/장거리:100) 3.궁수(단거리:90/중거리:100/장거리:100)
//                     4.마법사(단거리:100/중거리:100/장거리:100) 5.성직자(단거리:100/중거리:100/장거리:100)
static	int rate_by_class[5][3] = { { 100, 90, 100 }, { 90, 100, 100}, { 90, 100, 100}, { 100, 100, 100 }, { 100, 100, 100 } };

extern ItemAttr *GetItemByPOS( int cn, POS item ); // itemtable.cpp

void SendItemAttrDurationChange( t_connection c[], int cn, int pos )
{	
	t_packet p;
	
	ItemAttr *i = &c[cn].chrlst.equip[pos];
	if( i == NULL ) return;
	if( i->item_no == 0 ) return;
	
	p.h.header.type = CMD_ITEM_DURATION_CHANGE;
	{
		p.u.item_duration_change.pos = pos;
		p.u.item_duration_change.dur = LOWORD( i->attr[ IATTR_DURATION] );
	}
	p.h.header.size = sizeof( t_item_duration_change );
	
	QueuePacket( c, cn, &p, 1 );
}	


extern void GetItemDuration( ItemAttr item, WORD &d_curr, WORD &d_max );	// 아이템 내구력 뽑아 오기
// 020428 YGI acer
extern int BreakItemByDuration( ItemAttr *item, CHARLIST *ch, POS pos );

/*	무기나 방어구의 마노도 계산..
// type : 0 공격도구.
//        1 방어구 

  
	d_curr = LOWORD( item.attr[IATTR_DURATION] );
	d_max = HIWORD( item.attr[IATTR_DURATION] );
	
	  좋은 아이템은  1000저도를 기준한다. 
	  
		계산은 1000을 가지고 하지만 Display는 /10을 하여 표시한다. 
		
*/
int getEquipRandomPos( void )//내구도 깎을때 포지션을 받아오는 부분
{	
	const int pos = rand()%100;
	
	if( pos < 25 )  {return	WT_ARMOR;}
	if( pos < 50 )  {return  WT_SHIELD;}
	if( pos < 75 )  {return  WT_HELMET;}
	if( pos < 100 ) {return  WT_SHOES;}
	
	return WT_ARMOR; 
}	

// 장거리 무기의 종류에 따른 사정거리의 변화와 공격력의 가감..
////////////////////// 0804 lkh 추가 ////////////////////////
int calcRangeCorrection(CHARLIST *a, CHARLIST *d )
{	
	int range_corr= 100;
	int min = 0, mid = 0, max = 0;
	
	const int dist= getDistance(a, d) ;
	if(dist < 2 )			return 100;		// 상호 거리가 64미만이면..
	
	const int ino = a->equip[ WT_WEAPON].item_no;		// Item이 없다.
	if( ino == 0 )			return 100;
	
	CItem *ii = ItemUnit( ino/1000, ino%1000 );	// Item이 없다.
	if( ii == NULL )		return 100;
	
	
	// 무기의 사정거리..
	/*
	int range = ii->Getrange();
	if( range < 2 )			return 100;
	*/
	
	switch( ii->GetItemKind() ) 
	{
	case IK_HURL: // general throwing weapon
		min = 3;
		mid = 4;
		max = 5;
		break ;
		
	case IK_LONG_BOW:	// 010724 LSW
	case IK_DUAL_BOW://030102 lsw Dual BOW
		min = 13;
		mid = 19;
		max = 22;
		break ;
		
	case IK_SHORT_BOW : // short BOW
		min = 4;
		mid = 8;
		max = 12;
		break ;
		
	case IK_TWO_ARROW :	//화살.석궁..
	case IK_DUAL_ARROW:
		min = 5;
		mid = 10;
		max = 15;
		break ;
		
	default :
		min = 3;
		mid = 5;
		max = 7;
		break;
	}
	
	if		( dist < min )		range_corr= 100;
	else if	( dist < mid )		range_corr=  90;
	else if	( dist < max )		range_corr=  85;
	
	return 	range_corr;
}	

int addEXP(CHARLIST *a, CHARLIST *d, int criteria, int damage )
{	//< CSD-030509
#ifdef LMS_MAPSERVER
	return 0;
#endif
	if (g_pArenaManager->IsColossusArena())
	{
		return 0;
	}
	//> CSD-030509 
	if (a == NULL || d == NULL)         return 0;
	if (a->IsNpc())                     return 0;
	if (a->IsPlayer() && d->IsPlayer()) return 0;
			
	int exp;
	float base_point;
	DWORD partyexp;
	int tdamage;
	
	int t,i;																
	int partexpflag = 0;											
	bool bCanGetExpPcPc = false;
	
	damage = abs(damage);
	
	switch (d->Race)
	{ // 경험치를 주지 않는 NPC
    case GUARD:
    case SEALSTONE: return 0; 
	}
	
	if( d->SprNo == 96 ) return 0;
	if (d->IsTamedNpc())  return 0;	// 태밍된 애한테서는 경치를 얻을수 없다.
	
#ifdef _NATION_APPLY_
	if( d->SprType != SPRITETYPE_NPC ) 
		if( a->name_status.nation != d->name_status.nation ) bCanGetExpPcPc = true;	// 0927 YGI
#endif
		
		switch(criteria)					
		{
		case HIT_FAILED : return 0;											
		case HIT_AND_DEAD:
		case HIT_AND_NOTDEAD:	
			{
				if (bCanGetExpPcPc == false)
					if (a->IsPlayer() && d->IsPlayer())
					{// PK한 상태이다. 
						return 0;
					}		
					
					if( d ) // 경험치를 얻을수 없는 NPC들..					
						if( d->SprType == SPRITETYPE_NPC )
						{
							switch( d->SprNo )
							{
							case 92 :	// 기둥..
							case 93 :	// 기둥..
							case 94 :	// 과녁..
							case 95 :	// 허수아비.
							case 96 :	// 허수아비.
								{
									if (a->GetLevel() > ENABLE_DUMMY_LEVEL)
									{	//< CSD-030806
										return 0;
									}	//> CSD-030806
								}
							}
							// NPC의 종족이 인간이면...경치없음..
							if( d->Race == 1 ) return 0;
						}
						
						const int nTotal = (d->IsPlayer()) ? NPC_Lev_Ref[d->GetLevel()].nTrainingExp:
															 NPC_Gen_Ref[d->npc_index].nEpMax; // CSD-030806 
						if (a->GetLevel() == d->GetLevel())
						{	//< CSD-030806 
							t = 1;
						}	//> CSD-030806 
						
						if (a->GetLevel() < d->GetLevel())
						{	//< CSD-030806 
							t = 1;
							if( NPC_Lev_Ref[a->GetLevel()].nGreatRate )
							{
								t = (d->GetLevel() - a->GetLevel())/NPC_Lev_Ref[a->GetLevel()].nGreatRate;
							}
							else
							{
								SaveCharacterBin( "error_level_check", a);
							}
						}	//> CSD-030806 
						
						if (a->GetLevel() > d->GetLevel())
						{	//< CSD-030806 
							t = 1;
							if( NPC_Lev_Ref[a->GetLevel()].nLessRate )
							{
								t = (a->GetLevel() - d->GetLevel())/NPC_Lev_Ref[a->GetLevel()].nLessRate;
							}
							else
							{
								SaveCharacterBin( "error_level_check", a);
							}
						}	//> CSD-030806 
						
						base_point = (t < 1) ? nTotal:nTotal/t;
						break;
			}
		case HIT_GHOSTRESURRECTION:
			{
				base_point = damage;
				break;
			}
		}
		
		
		// base_point를 이용해 경치의 량을 계산한다. 
		switch(criteria)
		{
		case HIT_FAILED:		exp = 0; partexpflag = 0;	break;
		case HIT_AND_DEAD:	
			{
				exp = base_point*0.4;
				partexpflag = 1; // 파티와 나눈다.	
				
				if (d)
				{
					switch (d->SprNo) 	
					{ // 더미를 공격할때에는 파티를 해도 경치를 나누지 않는다. 
					case 92:
					case 93:
					case 94:
					case 95:
					case 96: partexpflag = 0;	break;
					}		
				}
				
				break; 
			}
		case HIT_AND_NOTDEAD:
			{
				if (damage > d->HpMax)
				{
					exp =	base_point;
				}
				else
				{
					tdamage = damage*100/ATTACKPOWER_PRO_;
					exp =	base_point*tdamage/d->HpMax;
				}
				
				partexpflag = 1; // 파티와 나눈다.
				break; 
			}
		case HIT_AND_ALIVE:	        
			{
				exp = (DWORD)(base_point*0.02);	
				partexpflag = 1; // 파티와 나눈다.  
				break; 
			}
		case HIT_GHOSTRESURRECTION:	
			{
				exp = (DWORD)base_point;  
				partexpflag = 0;	 
				break;
			}
		default :  
			{
				return 0;
			}
		}
		
		if( bCanGetExpPcPc == true )
		{	
			exp /= GET_EXP_PC_AND_PC_;	// PC간에는 경험치를 조금만 준다. 
		}	
		
		if( exp == 0 ) return 0; 	
		
		if (a->GetLevel() < 5)
		{	//< CSD-030806
			exp += 2; // 최저 2이상이 나오게 한다.   이런수치는 어차피 없지만..
		}	//> CSD-030806
		
		CHARLIST *ttt[6] ={0,};
		int tc = 0;

		if( partexpflag == 1  ) // 경험치를 분배하는경우는, 파티가 있는지 찾는다. 
		{
			AREA *aa = connections[a->GetServerID()].area_tbl;

			for( i = 0; i < MAX_AREA_BUFFER; i ++)
			{			
				if( aa[i].bAlive == TRUE )
				{		
					switch( aa[i].Type )
					{						
					case 1:
						{
							CHARLIST *tt = ::CheckServerId(aa[i].ID);

							if (tt != NULL && !tt->IsDead())
							{
								if( BoxAndDotCrash( a->MoveSx - 15,  a->MoveSy - 15, 30, 30, tt->MoveSx, tt->MoveSy ) )
								{	
									for (int i = 0; i < MAX_PARTY_MEMBER; i++)
									{
										if (a->party[i].On && !_stricmp(a->party[i].Name, tt->Name))
										{   // 파티원이지 여부 검사
											if (abs(a->GetLevel() - tt->GetLevel()) <= ENABLE_PARTY_LEVEL)
											{   //< CSD-030806 : 레벨차이가 35이하이면 경험치 분배
												goto YES_FIND_PARTY_;
											}	//> CSD-030806
										}
									}
									continue;
	YES_FIND_PARTY_:// 파티가 있으면  tc != 0 이 된다. 
									ttt[tc] = tt; tc++;
								}		
							}

							break;
						}
					default:	continue;
					}		
				}		
			}	
		}

		/****NAO PRECISA MUDAR NADA AQUI, SO MUDAR NA TABELA npc_lv2exp */
		/********************************************/
		/*********** exp pill 2, the return *********/
		int FoundPill;
		FoundPill = SearchItemByInv(10301,a);
		/*if(FoundPill != 1) SearchItemByInv(10311,a); //Eleval 19/08/09 - Added other pills
		if(FoundPill != 1) SearchItemByInv(10312,a); //Eleval 19/08/09 - Added other pills
		if(FoundPill != 1) SearchItemByInv(10313,a); //Eleval 19/08/09 - Added other pills*/
		//MyLog(LOG_NORMAL,"Procura a pill na bag");
		if( FoundPill==1 ){
			//MyLog(LOG_NORMAL,"Achou a pill na bag");
			exp = exp*2;			
		}

		if(g_EventExp.IsEvent()) //Eleval 22/07/09 - For Exp Events
		{
			exp *= g_EventExp.multiplier;
		}
		/********************************************/

		a->ChangeUpAddExp(exp); // CSD-TW-030624		
		isLevelUp(a);

		//-------------- Exp Pills - Coded by Rogerio ------------------
		/*if (type_exp < 3){
			exp = exp/valExpAdd;
		}*/
		if( FoundPill==1 ){
			exp = exp/2;			
		}
		int expparty;
		//---------------End of Exp Pills - Coded by Rogerio ------------

		if( tc )// 인원수에 따른 보정치를 적용한다. 
		{
			switch( tc )
			{						  	
			case 1:	partyexp = exp*65/100; break; 
			case 2:	partyexp = exp*60/100; break;
			case 3:	partyexp = exp*55/100; break;
			case 4:	partyexp = exp*50/100; break;
			case 5:	partyexp = exp*45/100; break;
			case 6:	partyexp = exp*40/100; break;
			default: goto END_;    
			}	

			expparty = partyexp; // Add by rogerio
			for( i = 0 ; i < tc ; i ++)	
			{	
				if (ttt[i]->GetLevel() < 81){ // Add by rogerio
					expparty = partyexp*5;
				}else{					
					if (ttt[i]->GetLevel() < 120){
						expparty = partyexp*2;
					}			
				}

				switch (ttt[i]->party_select)
				{	//< CSD-TW-030624
				case 0:
				case PS_EXP:
					{ 
						ttt[i]->ChangeUpAddExp(expparty);
						isLevelUp(ttt[i]);
						break;
					} 
				case PS_TACTIC:
					{ 
						ttt[i]->ChangeUpTacExp(5, expparty*3);
						break;
					} 
				case PS_MAGIC_TAC:
					{
						ttt[i]->ChangeUpTacExp(3, expparty*3);
						break;
					} 
				} //> CSD-TW-030624

				expparty = partyexp;
			}
		}
		
END_:		
	if( FoundPill==1 ){ // Add by rogerio
		exp = exp*2;
	}
	

	return exp;
}			
//1018 zhh

extern void MakeSealStoneItem(CHARLIST* ach,CHARLIST* dch);	// 011213 LTS
extern void AddNPCControlData(int cn,int NPC_ID);		// LTS NPC BUG
extern void RemoveNPCControlData(int cn,int NPC_ID);	// LTS NPC BUG
extern void CheckNPCControlData(int cn,int NPC_ID);	// LTS NPC BUG
// YGI acer 3
extern int g_LocalWarBegin;			// 011215 LTS

void RemoveFromTamedList(CHARLIST *d)
{
	if(d == NULL) return;

	CHARLIST *ch = CheckServerId(d->GetMaster());

	if(ch != NULL)
	if(!ch->TamedMonster.empty())
	for(int i = 0; i < ch->TamedMonster.size() ; i++)
	{
		if(ch->TamedMonster.at(i) == d->GetServerID())
			ch->TamedMonster.at(i) = -1;
		break;
	}
}


//////////////////// 0812 lkh 수정 //////////////////////
void killCharacter( CHARLIST *a,CHARLIST *d )
{	// 공격자는 NULL일 수도 있음
	//	if( a == NULL ) return;
	if( d == NULL ) return;
	if( d->GetServerID() == LOGIN_SERVER_CONNECT || d->GetServerID() == DB_DEMON ) return;
	
	if( d->bAlive >= DEAD_ ) return;
	
	d->bAlive		= DEAD_;	
//Log added by Eleval 25/05/09 to find and fix that bug with Dark Bears and Cyclops.
//This map server should only be used for West Glade and Lethe's Reign, thanks.
//The WG_DEBUG is defined up there and can be disabled if you need to build this MapServer for an other map
/*#ifdef WG_DEBUG
	if(!d->IsPlayer())
	{
		MyLog(0, "A monster died ! (%d, %d) (Spr: %d)", d->X / 32, d->Y / 32, d->SprNo);
		if(a == NULL)
		{
			MyLog(0, "The killer is a NULL value !!!");
		}
		else
		{
			MyLog(0, "The killer is %s", a->Name);
		}
	}
#endif*/
//End of the log
	
	if( d->SprType != SPRITETYPE_NPC )
	{	
		d->deadcount = DEAD_PC_COUNT_;				
	}	
	else						
	{	//< CSD-040407
		switch (d->Race)
		{						
		case HUMAN:
			{
				d->deadcount = DEAD_PC_COUNT_;
				break;
			}
		case ANIMAL:
			{
				d->deadcount = DEAD_NPC_ANIMAL_COUNT_;
				break;
			}
		case SEALSTONE:
			{
				d->deadcount = DEAD_SEALSTONE_COUNT_;
				break;
			}
		default:
			{	// 도축이 불가능한 경우는 리젠시간을 줄임
				d->deadcount =  (d->Meat_amount > 0) ? DEAD_NPC_MONSTER_COUNT_:DEAD_NPC_HUNT_MONSTER_COUNT_;
				break;
			}
		}						
	}	//> CSD-040407
	
	d->SetState(CON_DEATH);

	if(d->IsTamedNpc())
	{
		RemoveFromTamedList(d);
	}
	
	if (d->IsNpc() && !d->IsTamedNpc())
	{	
		switch (d->generationtype)
		{	//< CSD-030417
		case GT_EVENT_MONSTER:
			{
				g_pAIManager->CheckDeath(d); // LTS DRAGON MODIFY
				CheckBossAndDeActiveGroupNo(d);		// LTS AI2
				break;
			}
		case GT_HUNT_MONSTER:
			{
				const int nGroup = d->JoinLocalWar;
				CRegenInfo* pRegen = g_pRegenManager->GetRegenInfo(nGroup);

				if (pRegen != NULL)
				{
					switch (d->GainedFame)
					{
					case 0: 
					case 2: pRegen->DelMember(d); break;
					case 1: pRegen->DelMaster(d); break;
					}

					pRegen->CalcRegenTime();
				}
								
				if (g_pArenaManager->IsColossusArena())
				{	//< CSD-030522
					if (a != NULL && a->IsPlayer())
					{	
						CArenaTeam* pTeam = a->GetJoinArenaTeam();

						if (pTeam == NULL)
						{
							break;
						}
							
						pTeam->IncKillCount();

						CBaseArena* pGame = a->GetJoinArenaGame();

						if (pGame == NULL)
						{
							break;
						}

						const int nTeam = pTeam->GetTeamNumber();
						const int nCount = pTeam->GetKillCount();
						pGame->SendKillCount(nTeam, nCount);
					}	
				}	//> CSD-030522
				break;
			}	
		}	//> CSD-030417

		
		if( d->Race == SEALSTONE ) 
		{
			if (a->IsPlayer()) a->GainedFame+=2;					// 결계석을 죽인 사람에게 얻을 명성치를 +2해준다.
			Send_CMD_SEALSTONE_STATUS( d, 0 );
		} 
		else
		{
			if (d->Race==GUARDSTONE)						// 전쟁터 계열
			{
				UpdateGuardStoneStatus(a,d);						
			}
			SkillMgr.FallItemWhenNPCDie(d);
		}
		
		if (!d->ChairNum)							// 서버 AI가 아닌 NPC가 죽으면 바로 컨트롤 데이터를 해제한다.		// LTS DRAGON MODIFY
		{
			int cn=d->ctrlpc;
			RemoveNPCControlData(cn,d->GetServerID());	// NPC AI 컨트롤 데이터를 제거한다.
		}
		//1020 zhh		//특정한 조건을 갖추었을때 특정한 아이템 생산
	}	
	
	if (d->IsPlayer())
	{	
		ExpDown(a, d, true); // CSD-030314
		// 전쟁 중인 경우 적군을 죽인 수 중가
		if (isNationWarfieldServer() && IsWar())
		{
			CountNationDeath(a, d);
		}
		if (isNewWarfieldServer() && IsWar())
		{
			CountNewWarfieldDeath(a,d);
		}
#ifdef LMS_MAPSERVER
		if (g_pArenaManager->IsColossusArena())
		{
			CArenaTeam* pTeam = d->GetJoinArenaTeam();

			if (pTeam != NULL)
			{
				CBaseArena* pGame = d->GetJoinArenaGame();

				if (pGame != NULL)
				{
					if (pGame->GetArenaType() == AT_LMS)
					{
						char temp[TEXT_LENGTH];
						sprintf_s(temp, sizeof(temp), "LMS: %s has been deafted!", d->Name);
						string chatData(temp);
						SendCounselorChat(chatData, CHATTYPE_LMS);
						pGame->SendMemberDefeat(d);
					}
				}
			}
		}
#endif
		CheckLocalWarDeath(a,d);			// LTS LOCALWAR
		CheckEventLocalWarDeath(a,d);		// 020115 LTS
		CheckDragonLordWarTeam(a,d);		// LTS DRAGONLORD
		
		// 020428 YGI acer 3
		if( a )
		{
			if( a->IsPlayer() || a->IsTamedNpc() )	// 테밍에 성공해도 소환해도 IsTamedNpc() 는 true 이다.
			{	// 누군가를 죽였다면 기록에 남기자...
				CHARLIST *ch = 0;
				if( a->IsPlayer() )
				{
					ch = a;
				}
				else
				{ 
					if (a->IsTamedNpc())
					{
						// 020620 YGI
						CHARLIST *master = GetCharListPtr(a->GetMaster() );
						if( master && master->IsPlayer() ) ch = master;
					}
				}
				
				if( ch )
				{
					bool bRecord = true;
					
					if (ch->fight_flag == 1 &&	d->fight_flag == 1 && ch->fight_id == d->GetServerID())
					{
						bRecord = false;
					}
					
					if (isNationWarfieldServer() &&	IsWar()) 
					{
						bRecord = false;
					}
					
					if (IsArenaFight(ch))
					{	//< CSD-030506
						bRecord = false;
					}	//> CSD-030506

					if (g_LocalWarBegin &&	ch->JoinLocalWar &&	d->JoinLocalWar)
					{
						bRecord = false;
					}
					
					if( bRecord )
					{
						char pk_msg[2048] = {0,};
						sprintf_s( pk_msg, sizeof(pk_msg), "pk : %s(%3d,%3d) [%s](NK:%d,%d,%d)->[%s](NK:%d,%d,%d) ( %02d.%02d %02d:%02d:%02d ) ",
							MapName, ch->MoveSx, ch->MoveSy, ch->Name,OutNK(ch,3), OutNK(ch, 4), OutNK(ch, 6), 
							d->Name,OutNK(d,3), OutNK(d, 4), OutNK(d, 6), g_mon+1, g_day,g_hour, g_min, g_sec );
						// 030506 YGI
						string strPath;

						if (g_pLogManager->GetLogPath(strPath))
						{	//< CSD-TW-030701
							char temp_path[80];
							sprintf_s( temp_path, sizeof(temp_path), "%s/pk.txt", strPath.c_str());
							SaveLogDefault(temp_path, pk_msg, 0);
						}	//> CSD-TW-030701
					}
				}
			}
		}

	}

	
	d->Hp = 0;
	d->Mana = 0;
	d->Hungry= 0;
	d->MoveLength = d->MovePathCount = 0;				
	d->cur_skillno = 0;  // 망약 기술을 시전중이었으면....
	
	
	//021030 YGI
	extern void CheckEventWhenKillMonster( CHARLIST *pKiller, CHARLIST *pNpc );
	if( d->IsNpc() )
	{
		CheckEventWhenKillMonster( a, d );
		CheckIfQuestItem( a, d ); //Eleval
	}
	
	
	if( a == NULL ) return;
	if( a->GetServerID() == LOGIN_SERVER_CONNECT || a->GetServerID() == DB_DEMON ) return;
	
	// 021023 kyo npc죽인 카운터를 세고 있을때 
	if (a && d->IsNpc())
	{
		//캐릭터가 죽였거나 캐릭터의 소환수가 죽였거나.
		CHARLIST *ch = a;
		if (!ch) return;

		if (a->IsSummonMonster())
		{
			const WORD idMaster = a->GetMaster();
			ch  = GetCharListPtr(idMaster);
		}
		
		g_QuestInMap.RunScriptCounter( ch->GetServerID(), ch->Name, d->npc_index, SCRIPT_COUNTER_TYPE__NPCKILL );
		g_QuestInMap.GiveRequitalAfterKillNpc( ch, d ); // 021106 kyo
	}

	// 누가 죽였는지 적어 둔다. 
	if (a->IsSummonMonster())
	{
		char szBuffer[40];
		const WORD idMaster = a->GetMaster();
		// 020620 YGI
		LPCHARLIST pMaster = GetCharListPtr(idMaster);
		if( pMaster )
		{
			sprintf_s(szBuffer, sizeof(szBuffer), "%s(%s)", a->Name, pMaster->Name);
			strcpy_s(d->KillerName, sizeof(d->KillerName), szBuffer);
		}
	}
	else
	{
		strcpy_s(d->KillerName, sizeof(d->KillerName), a->Name);
	}

	
	switch (NPC_Gen_Ref[d->SprNo].nNpcRace)
	{	//< CSD-031013					
	case HUMAN:
		{
			if (a->name_status.nation == d->name_status.nation)
			{	
				++a->killpc;
				SendServerKillPc(a->GetServerID());
			}

			break;	
		}
	case ANIMAL:
		{
			++a->killanimal;
			break;		
		}
	case NORM_MON:
		{
			if (a->SprType  != SPRITETYPE_NPC)
			{
				++a->killmon;
				SendServerKillMon(a->GetServerID()); //0209 YGI
			}
			
			break;		
		}
	}	//> CSD-031013

	// 소환몬이 적을 공격한 후 상대방을 죽었다면 주인을 따라감
	if (a && a->IsSummonMonster() && a->ctrlpc < 10000)
	{ 
		const WORD idMaster = a->GetMaster();
		LPCHARLIST pMaster = GetCharListPtr(idMaster);
		
		if (pMaster != NULL)
		{
			pMaster->SetMaster(idMaster);
			pMaster->m_xSummon.Command(FollowSummon);
		}
	} 

	// 소환몬이 죽었다면 주인의 리스트에서 제거
	if (d && d->IsSummonMonster())
	{ 
		const WORD idMaster = d->GetMaster();
		LPCHARLIST pMaster = GetCharListPtr(idMaster);
		
		if (pMaster != NULL)
		{
			pMaster->m_xSummon.Decrease(d->GetServerID());
		}
	} 
	
	if( d && d->fight_flag )
	{
		EndManToManFight( a, d );
	}
}								

#define PAI   3.141592

BOOL ChangeDirection( DIRECTION* direction, int sx, int sy, int dx, int dy )
//int ReturnDirection( float tx1, float ty1, float tx2, float ty2 )
{			
	double t;
	
	if( dy == sy && sx == dx ) return 0;
	
	t = atan2( (float)(dy - sy) , (float)(dx - sx) );
	if(    PAI*7/8 < t && t <=  PAI + 1   )	{*direction = DIRECTION_LEFT;		return 1; }
	if(   -PAI -1    <=t && t <= -PAI*7/8 )	{*direction = DIRECTION_LEFT;		return 1; }
	
	if(   -PAI*7/8 < t && t <= -PAI*5/8 )	{*direction = DIRECTION_LEFTUP;		return 1; }
	if(   -PAI*5/8 < t && t <= -PAI*3/8 )	{*direction = DIRECTION_UP;			return 1; }
	if(   -PAI*3/8 < t && t <= -PAI  /8 )	{*direction = DIRECTION_RIGHTUP;	return 1; }
	if(   -PAI  /8 < t && t <=  PAI  /8 )	{*direction = DIRECTION_RIGHT;		return 1; }
	if(    PAI  /8 < t && t <=  PAI*3/8 )	{*direction = DIRECTION_RIGHTDOWN; 	return 1; }
	if(    PAI*3/8 < t && t <=  PAI*5/8 )	{*direction = DIRECTION_DOWN; 		return 1; }
	if(    PAI*5/8 < t && t <=  PAI*7/8 )	{*direction = DIRECTION_LEFTDOWN;	return 1; }
	
	return 0;
}		




///////////////////////// 0525 lkh 수정 ///////////////////////////////
bool	YouCanBackdraw( CHARLIST *a, CHARLIST *d, int how )
{		
	DIRECTION dir;
	int x, y;
	int	t_X=0, t_Y=0;
	
	ChangeDirection( &dir, d->MoveSx, d->MoveSy, a->MoveSx, a->MoveSy );
	
	x = d->MoveSx;
	y = d->MoveSy;
	
	switch( dir )
	{	
	case DIRECTION_UP		:				t_Y=how;		break;
	case DIRECTION_RIGHTUP	:	t_X=-how;	t_Y=how;		break;
	case DIRECTION_LEFTUP	:	t_X=how;	t_Y=how;		break;
	case DIRECTION_RIGHT	:	t_X=-how;      				break;
	case DIRECTION_LEFT		:	t_X=how;      				break;	
	case DIRECTION_RIGHTDOWN:	t_X=-how;	t_Y=-how;		break;
	case DIRECTION_LEFTDOWN	:	t_X=how;	t_Y=-how;		break;
	case DIRECTION_DOWN		:				t_Y=-how;		break;
	}	
	x += t_X;
	y += t_Y;
	
	if( FreeTile( d, x, y, x, y ) )			//실제 갈수 있는 지점인가?
	{	
		d->MoveLength = d->MovePathCount = 0;
		d->MoveSx	= x;
		d->MoveSy	= y;
		d->X		= x*TILE_SIZE;
		d->Y		= y*TILE_SIZE;
		d->Tox		= t_X;
		d->Toy		= t_Y;
		
		if( d->SprType == SPRITETYPE_NPC )
		{
			SetArea( MOVE_NPC_AREA, d->GetServerID());
		}
		else
		{
			SetArea( MOVE_PC_AREA, d->GetServerID());
		}
		
		return true;
	}	
	
	d->Tox = 0;
	d->Toy = 0;
	return false;
}

inline int IsNKNation(LPCHARLIST ch)
{		
	switch (MapInfo[MapNumber].nation)
	{	
	case N_VYSEUS:
	case N_YLLSE:
	case N_ZYPERN:
		{
			return MapInfo[ MapNumber].nation;
		}
	default:
		{
			switch( ch->name_status.nation )
			{
			case N_VYSEUS:
			case N_YLLSE:
			case N_ZYPERN:
				{
					return ch->name_status.nation;
				}
			}

			break;
		}
	}	
	
	return 0;
}		

inline int IsHeNK( LPCHARLIST ch, int nation )
{	// 현재 맵에 대해서 PK수치가 있다.
	switch (nation)
	{
	case N_VYSEUS:
	case N_ZYPERN:
	case N_YLLSE:
		{
			return ch->nk[ nation];
		} 
	}

	return 0;
}	

void CheckElapsedTime_NK_Down( void )
{	//< CSD-CN-031213
	static DWORD time = 0;
	
	if( g_curr_time - time > 1000  )
	{
		time = g_curr_time;
	}	
	else 
	{
		return;
	}	

	CUserManager::HASH_USER mpUser = g_pUserManager->GetUserSet();

	for (CUserManager::ITOR_USER i = mpUser.begin(); i != mpUser.end(); ++i)
	{
		const int idUser = i->second;
		CHARLIST* pUser = CheckServerId(idUser);
		
		if (pUser != NULL)
		{
			if (pUser->nk[N_VYSEUS] > 0)
			{
				InNK(pUser, N_VYSEUS, -1);
			}

			if (pUser->nk[N_ZYPERN] > 0)
			{
				InNK(pUser, N_ZYPERN, -1);
			}

			if (pUser->nk[N_YLLSE ] > 0)
			{
				InNK(pUser, N_YLLSE , -1);
			}
		}
	}
}	//> CSD-CN-031213

#define MAX_NK_		30000
#define MAX_ULTRA_NK_	100
#define PK_START_LIMIT_		5
static int nCaseOfNK[3][3] = {{0,1,1},{ 4,3,3},{4,2,2}};

inline int OutNK( LPCHARLIST ch, int nation )
{	
	switch( nation )
	{
	case N_VYSEUS :	
	case N_ZYPERN : 
	case N_YLLSE :  return ch->nk[ nation];  
		
	default : return ch->nk[ ch->name_status.nation ];	
	}

	return 0;
}	

void InNK( LPCHARLIST ch, int nation, short int add_nk )
{	
	if( add_nk == 0 ) return;
	if( ch->SprType == SPRITETYPE_NPC ) return;
	
	short int oldnk;
	short int *ch_nk;
	switch( nation )
	{
	case N_VYSEUS	:
	case N_ZYPERN	:
	case N_YLLSE	:   oldnk = ch->nk[ nation];
		ch_nk = &ch->nk[ nation];  break;
	default			:  return;
	}
	*ch_nk += add_nk;
	
	if( *ch_nk > MAX_NK_ ) *ch_nk = MAX_NK_;
	if( *ch_nk < 0       ) *ch_nk = 0;
	
	if( *ch_nk == 0 )
	{
		switch( nation )
		{
		case N_VYSEUS :	ch->name_status.ultra_nk3 = 0;	break;
		case N_ZYPERN : ch->name_status.ultra_nk4 = 0;	break;
		case N_YLLSE :  ch->name_status.ultra_nk6 = 0;	break;
		default : return;
		}
	}
	else if( *ch_nk >= MAX_ULTRA_NK_ )
	{
		switch( nation )
		{
		case N_VYSEUS :	ch->name_status.ultra_nk3 = 1;	break;
		case N_ZYPERN : ch->name_status.ultra_nk4 = 1;	break;
		case N_YLLSE :  ch->name_status.ultra_nk6 = 1;	break;
		default : return;
		}
	}
	
	SendNK( ch->GetServerID(), ch->nk[ N_VYSEUS], ch->nk[ N_ZYPERN], ch->nk[ N_YLLSE] );
}	


// 각 국가가 교전상태이면 true를 리턴한다. 
inline bool IsWarfare( int anation, int bnation )
{
	if( anation < 0 || anation >= MAX_NATION )	return false;
	if( bnation < 0 || bnation >= MAX_NATION )	return false;
	
	switch( NationRelationStatus[ anation][ bnation] )
	{
	case NATION_RELATION_ENEMY_	: return true;
	default : return false;
	}
	
	return false;
}


int GetCaseOfNK( int cur_nation, int a_nation, int d_nation, int a_nk, int d_nk ) 
{
	bool warfare;
	int step2flag = 0, step3flag = 0;
	
	warfare = IsWarfare( cur_nation, d_nation );
	
	//step 2
	if( warfare )
	{
		step2flag = 0;
	}
	else if( d_nk )
	{
		step2flag = 2;
	}
	else
	{
		step2flag = 1;
	}
	
	//step3
	warfare = IsWarfare( cur_nation, a_nation );
	
	if( warfare )
	{
		step3flag = 0;
	}
	else if( d_nk )
	{	
		step3flag = 2;
	}	
	else
	{	
		step3flag = 1;
	}	
	
	int tcase = nCaseOfNK[ step2flag][ step3flag];
	
	return tcase;
}

#define NK_TYPE_STEAL_		0
#define NK_TYPE_KILL_		1
#define NK_TYPE_NO_KILL_	2

static int NationNkNum[5][3][3] = 
{	
	{{0, 0, 0},	{ 0, 0, 0},	{0, 0, 0}},
	{{0, 0, 0},	{ 0, -5,10},{0, 0, 0}},
	{{0, 0, 0},	{ 0, 0, 0},	{0, 0, 0}},
	{{0, 5, 5},	{ 0,20,20}, {0, 1, 1}},
	{{0, 0, 0},	{-5, 0,10},	{0, 0, 0}}
};

extern int g_isLocalWarServer;
extern int g_LocalWarBegin;

void CheckNK( int a_id, int d_id, int type )
{	//< CSD-030509
#ifdef LMS_MAPSERVER
	return;
#endif
	switch (MapInfo[MapNumber].forrookie)
	{
	case 0:
	case 3: return;
	}

	LPCHARLIST attacker = GetCharListPtr(a_id);
	
	if (attacker == NULL) 
	{
		return;
	}

	LPCHARLIST defender = GetCharListPtr(d_id);

	if (defender == NULL)
	{
		return;
	}
	// 현상범인 경우
/*	if (defender->GetWanted()) 
	{
		return;
	} finito 25/08/07 disabled temp until exploit fixed*/
	// 국지전인 경우
	if (g_LocalWarBegin && g_isLocalWarServer)
	{
		if (attacker->JoinLocalWar && defender->JoinLocalWar)
		{	
			return;
		}
	}

	if (g_pArenaManager->IsColossusArena())
	{
		if (attacker->IsJoinArenaGame() && defender->IsJoinArenaGame())
		{
			if (attacker->IsJoinArenaTeam() && defender->IsJoinArenaTeam())
			{
				return;
			}
		}
	}
	
	switch (type)
	{
	case NK_TYPE_KILL_:
	case NK_TYPE_NO_KILL_:
		{
			if( IsManToManFight( attacker, defender ) ) return;
			break;
		}
	case NK_TYPE_STEAL_:
		break;
	}
	
	int down = 0;	
	short int ano = 0, dno = 0;
	int a_nk, d_nk;	
	int cur_nation = MapInfo[ MapNumber].nation;
	int a_nation, d_nation;		
	
	a_nation =	attacker->name_status.nation;
	d_nation =	defender->name_status.nation;
	
	// Monster는 칠수 없다.. 
	if( d_nation == N_NONSTER ) return;
	if( defender->SprType == SPRITETYPE_NPC && type == NK_TYPE_NO_KILL_ ) return;  // 010613 KHS
	
	// 지금 위치한 맵에 대한 NK수치값..
	a_nk = OutNK( attacker, cur_nation );
	d_nk = OutNK( defender, cur_nation );
	int tcase = GetCaseOfNK( cur_nation, a_nation, d_nation, a_nk, d_nk );
	int *t = NationNkNum[tcase][ type];
	
	InNK( attacker, a_nation, t[0] );
	InNK( attacker, cur_nation, t[1] );
	
	if( tcase == 3)	
	{
		if( cur_nation != d_nation)	InNK( attacker, d_nation, t[2] ); // 적국과 적군의나라가 일치하면 두번 PK가 올라가지 않게 한다.
	}
	else 
	{
		InNK( attacker, d_nation, t[2] );	
	}
	// 그맵에 죽었으니... 그맵에 대한 NK수치는 떨어짐
	if (d_nk && type == NK_TYPE_KILL_)	
	{
		InNK(defender, cur_nation, -10);
	}	
}	//> CSD-030509

//------------------------------------------------------------------------------

// 배틍을 할수 있는 지역인가 
// 배틍을 할수 상태인가를 판다...
// 0810 KHS
bool CanBattleArea( LPCHARLIST a, LPCHARLIST  d)
{
	if (a->MoveSx < 0 || d->MoveSx < 0)  return false;
	if (a->MoveSy < 0 || d->MoveSy < 0)  return false; 
	if (a->MoveSx >= g_lpMapFile->wWidth || d->MoveSx >= g_lpMapFile->wWidth)   return false;
	if (a->MoveSy >= g_lpMapFile->wHeight || d->MoveSy >= g_lpMapFile->wHeight)  return false;

	// 0810 KHS  CMD_MOVE 패킷이 들어오는곳에 d->attackedblock = 0;으로 Setting한다. 
	if( d->attackedblock ) return false; // 최초에 게임에 들어오면 이동하기전까지는 공격을 받지 않는다. 
	
	
	if( a->Hp <= 0 ) return false; //  죽은자는 공격할수 없다. 
	if( d->Hp <= 0 ) return false; //  죽은자를 또 공격할순 없다. 
	
	// 극악PK // 1012 KHS
	//	if( d->name_status.ultra_pk == 1 ) return true;
	//	if( a->name_status.ultra_pk == 1 ) return false; //  국악 PK는 공격할수 없다.  마법에도 있나  Check 해줘...
	
	if( d->IsCounselor() && d->name_status.santa_power) return false;
	if( d->IsReporter() ) return false;//Finito 
	if( d->accessory[ WT_WEAPON] == 114 ) return false; // 산타 옷을 입었다. 
	// 030106 kyo 위치이동
	if (a->Race != GUARD) 
	{	//< CSD-030226 : 경비원이 아니라면 경우
		if (TileMap[a->MoveSx][a->MoveSy].attr_no_battle) 
		{
			return false;  // 맵에 no battle 속성이 있으면 공격할수 없다. 
		}
		
		if (TileMap[d->MoveSx][d->MoveSy].attr_no_battle) 
		{
			return false;  // 맵에 no battle 속성이 있으면 공격할수 없다. 
		}
	}	//> CSD-030226
	
#ifdef _NATION_APPLY_
	if( a->name_status.nation != d->name_status.nation ) return true;	// 0927 YGI
																		/*	else
																		{
																		if( d->SprType == SPRITETYPE_NPC )
																		if( d->Race == HUMAN )
																		{
																		// 같은 국가 사람npc는 공격을 받지 않게 한다. 
																		return false;
																		}
																		}
	*/
	
#endif
	
	if (a->Race == GUARD)	return true;
	if (IsHeNK(d, MapInfo[MapNumber].nation) > 5)	return true;
	if (g_block_attack)	return false; // 공격할수 없다. 많약 Event때문에 공격할수 없는 상태로 설정을 바꾸었다면 ..
	if (a->dwDontAttack > 0)	return false; //만약 공격불가 상태라면
	if (a->Hungry <= 0)	return false;
	
	return true;
}



inline int BojungDamage( LPCHARLIST d, int damage )
{
	return damage;
}
//............................
void SendYouDie( int cn )
{
	t_packet p;
	
	p.h.header.type = CMD_YOU_DIE;
	p.u.you_die.id		= cn;
	p.u.you_die.t		= 10;
	p.h.header.size = sizeof( t_you_die );
	
	QueuePacket( connections, cn, &p, 1 );
	CastMe2Other( cn, &p );     
}

void RecvDebugReqCharInfo( int cn, int target )
{
	t_packet p;
	LPCHARLIST n;
	
	p.h.header.type = CMD_DEBUG_REQ_CHAR_INFO;
	p.u.debug_server_req_char_info.id = target;
	
	if( target >= 10000 )
	{
		n = &NPCList[target-10000];
	}
	else 
	{
		n = &connections[ target].chrlst;
	}
	
	p.u.debug_server_req_char_info.id = target;
	
	p.u.debug_server_req_char_info.lv = n->GetLevel(); // CSD-030806
	p.u.debug_server_req_char_info.exp   = n->Exp; 
	p.u.debug_server_req_char_info.ac    = n->Ac;
	p.u.debug_server_req_char_info.Str   = n->Str;					//  힘
	p.u.debug_server_req_char_info.Con   = n->Con;				//  건강
	p.u.debug_server_req_char_info.Dex   = n->Dex;				//	민첩성
	p.u.debug_server_req_char_info.Wis   = n->Wis;				//	지혜
	p.u.debug_server_req_char_info.Int   = n->Int;				//	지능
	p.u.debug_server_req_char_info.MoveP = n->MoveP;				//	이동력
	p.u.debug_server_req_char_info.Char  = n->Char;				//	매력
	p.u.debug_server_req_char_info.Endu  = n->Endu;				//  저향력
	p.u.debug_server_req_char_info.Moral = n->Moral;				//	사기
	p.u.debug_server_req_char_info.Luck  = n->Luck;				//	행운
	p.u.debug_server_req_char_info.wsps  = n->wsps;				//  마법력->마법 메모라이즈
	p.u.debug_server_req_char_info.Hp    = n->Hp;    	
	p.u.debug_server_req_char_info.HpMax = n->HpMax;
	p.u.debug_server_req_char_info.Mp    = n->Mana;     
	p.u.debug_server_req_char_info.MpMax = n->ManaMax;
	p.u.debug_server_req_char_info.Hungry= n->Hungry; 
	p.u.debug_server_req_char_info.HungryMax = n->HungryMax;
	p.u.debug_server_req_char_info.event_no = n->eventno;			// 0131 YGI
	
	p.h.header.size = sizeof( t_debug_server_req_char_info );
	QueuePacket( connections, cn, &p, 1 );
}

//acer7
char skill_name42[][30] = 
{
	"SPRITUALITY",
		"POISONING",
		"ENTRAPMENT",
		"SCROLLING",
		"CUNNING",
		"BOMBPLAY",
		"STEALING",
		"FARMING",
		"MINING",
		"FISHING",
		"CUTDOWN",
		"BUTCHERING",
		"HERBDIGGING",
		"HERDING",
		"CARPENTRY",
		"BLACKSMITHY",
		"COOKING",
		"TAILORING",
		"BOWCRAFTING",
		"ALCHEMING",
		"CANDLEMAKING",
		"REPARING",
		"REUTILIZING",
		"MUSICIANSHIP",
		"DETECT",
		"HIDE",
		"CAMPING",
		"TAIMING",
		"COMMUNICATION",
		"ANIMAL_LORE",
		"ARMS_LORE",
		"EVALUATE_INTELIGENCE",
		"EVALUATE_WISDOM",
		"ANATOMY_JUDGEMENT",
		"ITEM_IDENTIFICATION",
		"TASTE_IDENTIFICATION",
		"APPRAISAL",
		"HEALING",
		"JUMP",
		"RIDING",
};


// acer7
void SaveBallancingSkill( LPCHARLIST a, int skillno )
{							
	FILE *fp;																							
	char temp[ FILENAME_MAX];
	
	a->balancingtest --;
	if( a->balancingtest > 0 )	return;
	a->balancingtest = 50;
	
	sprintf_s( temp, sizeof(temp), "./skill_log/%s.txt", a->Name );
	
	fp = fopen( temp, "at+");
	if(fp == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, a->Name);
		sprintf_s( temp, sizeof(temp), "./skill_log/%s.txt", file);

		fp = fopen( temp, "at+");
	}
	if( fp == NULL )
	{
		sprintf_s( temp, sizeof(temp), "./skill_log/ID_%s.txt", connections[a->GetServerID()].id );
		fp = fopen( temp, "at+" );
		if( fp == NULL ) return;
	}
	
	int mother = SkillTbl[skillno].MotherSkillType;
	fprintf( fp, "skill< %s[%d(%d)](lv%d:exp%d) >\n", skill_name42[skillno], skillno, mother, a->skillexp[mother].skillexp/10000, a->skillexp[mother].skillexp%10000 );
	fprintf( fp, "---%s(%3d,%3d) %02d.%02d %02d:%02d:%02d ---\n",  MapName, a->MoveSx, a->MoveSy, g_mon+1, g_day,g_hour, g_min, g_sec );
	fprintf( fp, "WORKER(Lv:%3d/Exp:%9d/ladder:%3d/fame:%3d)\n", a->GetLevel(), a->Exp, a->LadderScore, a->fame ); // CSD-030806
	for(int iCount = 0; iCount<45; iCount++)//스킬 경험치
	{
		if( a->skillexp[iCount].skillexp/10000 )
		{
			fprintf(fp,"SkillExp%d	Exp	%d/%d	RareRate	%d		month	%d	day	%d	makecount	%d\n",
				iCount,a->skillexp[iCount].skillexp/10000, a->skillexp[iCount].skillexp%10000,
				a->skillexp[iCount].rare,
				((LPSKILLEXP2)(&a->skillexp[iCount]))->month,
				((LPSKILLEXP2)(&a->skillexp[iCount]))->day,
				((LPSKILLEXP2)(&a->skillexp[iCount]))->makecount) ;
		}
	}
	fprintf( fp, "\n" );
	fclose(fp);
}	

void SaveBallancing2( LPCHARLIST a, LPCHARLIST d, int magicno, int damage, int mana, int attack, int attacktype, int tac  )
{   //< CSD-030324
	if(a->IsNpc()){return;}
	if( a->balancingtest/20  == (a->balancingtest-1)/20)//20 번에 한번
	{
		return;
	}
	
	char temp[ FILENAME_MAX] = {0,};
	::sprintf_s( temp, sizeof(temp), "./SimpleLog/%s.txt", a->Name );
	FILE *fp = fopen( temp, "at+");

	if(fp == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, a->Name);
		sprintf_s( temp, sizeof(temp), "./SimpleLog/%s.txt", file);

		fp = fopen( temp, "at+");
	}

	if( NULL == fp )
	{
		sprintf_s( temp, sizeof(temp), "./SimpleLog/%s.txt", connections[a->GetServerID()].id );
		fp = fopen( temp, "at+" );
		if( fp == NULL ) return;
		fprintf( fp, " %s \n", a->Name );
	}
	
	if( magicno )
	{
		if (magicno >= 215)
		{
			fprintf( fp, "Combat Skill< %3d:%30s >  --> %20s   \n", magicno, Magic_Ref[magicno].Han_Name, d->Name );
		}
		else
		{
			fprintf( fp, "Magic< %3d:%30s >  --> %20s   \n", magicno, Magic_Ref[magicno].Han_Name, d->Name );
		}
		
		fprintf( fp, "---%s(%3d,%3d) %02d.%02d %02d:%02d:%02d ---\n",  MapName, a->MoveSx, a->MoveSy, g_mon+1, g_day,g_hour, g_min, g_sec );
		
		fprintf( fp, " Attack(Lv:%3d/Exp:%9d/ladder:%3d/fame:%3d)\n", a->GetLevel(), a->Exp, a->LadderScore, a->fame); // CSD-030806
		fprintf( fp, " Defence(Lv:%3d/Exp:%9d)\n", d->GetLevel(), d->Exp); // CSD-030806

		fprintf( fp, " Hp %4d/%4d ->  Hp %4d/%4d \n", a->Hp, a->HpMax, d->Hp, d->HpMax );
		fprintf( fp, " Mana %4d/%4d -> Mana %4d/%4d \n", a->Mana, a->ManaMax, d->Mana, d->ManaMax );  
		fprintf( fp, " Hungry %4d/%4d -> Hngy %4d/%4d \n", a->Hungry, a->HungryMax, d->Hungry, d->HungryMax );
		fprintf( fp, " Damage : %4d        \n", damage );
		fprintf( fp, " Used Mana: %4d        \n",   mana );

		switch(a->Spell)
		{
		case WIZARD_SPELL:
			{
				::fprintf(fp, " Magery               Tactics(%3d:%d)\n", 
					a->Skill[TACTICS_Magery], a->tac_skillEXP[11]);
			}break;
		case PRIEST_SPELL:
			{
				::fprintf(fp, " Orison               Tactics(%3d:%d)\n", 
					a->Skill[TACTICS_Orison], a->tac_skillEXP[12]);
			}break;
		}
	}	
	else
	{
		fprintf( fp, "Weapon-Attack< %d:%s >  --> %s\n", attacktype, magicno, Magic_Ref[magicno].Han_Name,  d->Name  );
		fprintf( fp, "---%s(%3d,%3d) %02d.%02d %02d:%02d:%02d ---\n",  MapName, a->MoveSx, a->MoveSy, g_mon+1, g_day,g_hour, g_min, g_sec );
		
		fprintf(fp, " Attack(Lv:%3d/Exp:%9d/ladder:%3d/fame:%3d)\n", a->GetLevel(), a->Exp, a->LadderScore, a->fame); // CSD-030806
		fprintf(fp, " Defence(Lv:%3d/Exp:%9d)\n", d->GetLevel(), d->Exp); // CSD-030806
				
		const int ino = a->equip[ WT_WEAPON].item_no;
		CItem *ii = ItemUnit(ino);
		
		int itemkind = IK_NONE;

		if( ii )	{itemkind = ii->GetItemKind();}
		else		{itemkind = IK_NONE;}
		
		char *at = NULL;

		switch( itemkind )
		{			
			//	장거리 무기....
		case IK_DUAL_BOW:		at = "DUAL_BOW";	break;//030102 lsw Dual BOW
		case IK_SHORT_BOW	:	at = "SHORT_BOW";	break;
		case IK_TWO_ARROW	:	at = "TWO_ARROW";	break;
		case IK_DUAL_ARROW	:	at = "DUAL_ARROW";	break;//030102 lsw
		case IK_LONG_BOW	:	at = "LONG_BOW";	break;
		case IK_ARROW_POINT	:	at = "ARROW_POINT";	break;
		case IK_FEATHER		:	at = "FEATHER";	break;
			// 중장거리.. 
		case IK_HURL		:	at = "THROWING WEAPON"; break;
			//	단거리무기..
		default             :   at = "MELEE WEAPON";    break;
		}			
		
		char temp[ FILENAME_MAX];
		
		if( ii ) 
		{
			strcpy_s( temp, sizeof(temp), ii->GetHanName() );
		}
		else 
		{
			strcpy_s( temp, sizeof(temp), " NO WEAPON " );
		}
		fprintf( fp, " Hp %4d/%4d ->  Hp %4d/%4d \n", a->Hp, a->HpMax, d->Hp, d->HpMax );
		fprintf( fp, " Mana %4d/%4d ->Mana %4d/%4d \n", a->Mana, a->ManaMax, d->Mana, d->ManaMax );
		fprintf( fp, " Hungry %4d/%4d ->Hngy %4d/%4d \n", a->Hungry, a->HungryMax, d->Hungry, d->HungryMax );
		fprintf( fp, " Ac  %3d    ->       %3d   \n", 	a->Ac, d->Ac );
		fprintf( fp, " Damage  :    %4d     \n",   damage );
		fprintf( fp, " WEAPON :  %s( %d )  \n",    temp, a->equip[ WT_WEAPON].item_no );
		fprintf( fp, " AttackType :    %s      \n",    at );
	}
//	
	const int iTacKind = a->GetTacticsKind();// == SKILL_UNKNOWN 이면 기록 하면 안되지.
	switch(iTacKind)
	{
	case TACTICS_Crapple						:
		{
			::fprintf(fp, " Crapple              Tactics(%3d:%d)\n", 
				a->Skill[iTacKind], a->tac_skillEXP[0]);
		}break;
	case TACTICS_swordmanship					:
		{
			::fprintf(fp, " Swordmanship         Tactics(%3d:%d)\n", 
				a->Skill[iTacKind], a->tac_skillEXP[1]);
		}break;
	case TACTICS_Archery						:
		{
			::fprintf(fp, " Archery              Tactics(%3d:%d)\n", 
				a->Skill[iTacKind], a->tac_skillEXP[2]);
		}break;
	case TACTICS_Fencing						:
		{
			::fprintf(fp, " Fencing              Tactics(%3d:%d)\n", 
				a->Skill[iTacKind], a->tac_skillEXP[3]);
		}break;
	case TACTICS_Mace_fighting					:
		{
			::fprintf(fp, " Mace Fighting        Tactics(%3d:%d)\n", 
				a->Skill[iTacKind], a->tac_skillEXP[4]);
		}break;
	case TACTICS_Pierce							:
		{
			::fprintf(fp, " Pierce               Tactics(%3d:%d)\n", 
				a->Skill[iTacKind], a->tac_skillEXP[5]);
		}break;
	case TACTICS_Whirl							:
		{
			::fprintf(fp, " Whirl                Tactics(%3d:%d)\n", 
				a->Skill[iTacKind], a->tac_skillEXP[6]);
		}break;
	case TACTICS_Hurl							:
		{
			::fprintf(fp, " Hurl                 Tactics(%3d:%d)\n", 
				a->Skill[iTacKind], a->tac_skillEXP[7]);
		}break;
	case TACTICS_Parrying						:
		{
			::fprintf(fp, " Parrying             Tactics(%3d:%d)\n", 
				a->Skill[iTacKind], a->tac_skillEXP[8]);
		}break;
	case TACTICS_Double_Swordmanship			:
		{
			::fprintf(fp, " Double Swordmanship  Tactics(%3d:%d)\n", 
				a->Skill[iTacKind], a->tac_skillEXP[9]);
		}break;
	case TACTICS_Double_MaceFighting			:
		{
			::fprintf(fp, " Double Mace Fighting Tactics(%3d:%d)\n", 
				a->Skill[iTacKind], a->tac_skillEXP[10]);
		}break;
	case TACTICS_Magery							:
		{
			::fprintf(fp, " Magery               Tactics(%3d:%d)\n", 
				a->Skill[iTacKind], a->tac_skillEXP[11]);
		}break;
	case TACTICS_Orison							:
		{
			::fprintf(fp, " Orison               Tactics(%3d:%d)\n", 
				a->Skill[iTacKind], a->tac_skillEXP[12]);
		}break;
	default:
		{
			::fprintf(fp, " TACTIC UNKNOWN \n");
		}break;
	}
	::fprintf(fp, "\n");
	fclose(fp);
}

void SaveBallancing( LPCHARLIST a, LPCHARLIST d, int magicno, int damage, int mana, int attack, int attacktype, int tac  )
{   //< CSD-TW-030622
	if (a->IsNpc())
	{
		return;
	}

	a->balancingtest --;
	if( a->balancingtest > 0 )
	{
		::SaveBallancing2( a, d, magicno, damage, mana, attack, attacktype, tac  );
		return;
	}
	a->balancingtest = 100;
	
	string strPath;

	if (!g_pLogManager->GetLogPath(LT_BATTLE, strPath))
	{
		return;
	}
	//< CSD-040224
	FILE* fp = fopen(VA("%s/%s.txt", strPath.c_str(), a->Name), "at+");

	if(fp == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, a->Name);
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (fp == NULL)
	{
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), connections[a->GetServerID()].id), "at+");

		if (fp == NULL)
		{
			return;
		}
	}
	//> CSD-040224
	if( magicno )
	{
		if (magicno >= 215)
		{
			fprintf( fp, "Combat Skill< %3d:%30s >  --> %20s   \n", magicno, Magic_Ref[magicno].Han_Name, d->Name );
		}
		else
		{
			fprintf( fp, "Magic< %3d:%30s >  --> %20s   \n", magicno, Magic_Ref[magicno].Han_Name, d->Name );
		}
		
		fprintf( fp, "---%s(%3d,%3d) %02d.%02d %02d:%02d:%02d ---\n",  MapName, a->MoveSx, a->MoveSy, g_mon+1, g_day,g_hour, g_min, g_sec );
		
		fprintf(fp, " Attack(Lv:%3d/Exp:%9d/ladder:%3d/fame:%3d)\n", a->GetLevel(), a->Exp, a->LadderScore, a->fame); // CSD-030806
		fprintf(fp, " Defence(Lv:%3d/Exp:%9d)\n", d->GetLevel(), d->Exp); // CSD-030806
		fprintf( fp, " Reserved Point %3d -> Reserved Point %3d\n", a->GetReservedPoint(), d->GetReservedPoint());
		fprintf( fp, " Str   %3d    ->  Str   %3d                Hp %4d/%4d ->  Hp %4d/%4d \n", a->Str, d->Str, a->Hp, a->HpMax, d->Hp, d->HpMax );
		fprintf( fp, " Con   %3d    ->  Con   %3d              Mana %4d/%4d ->Mana %4d/%4d \n", a->Con, d->Con, a->Mana, a->ManaMax, d->Mana, d->ManaMax );  
		fprintf( fp, " Dex   %3d    ->  Dex   %3d            Hungry %4d/%4d ->Hngy %4d/%4d \n", a->Dex, d->Dex, a->Hungry, a->HungryMax, d->Hungry, d->HungryMax );
		fprintf( fp, " Wis   %3d    ->  Wis   %3d                                   \n", 	a->Wis, d->Wis );
		fprintf( fp, " Int   %3d    ->  Int   %3d                 Damage : %4d        \n", 	a->Int, d->Int, damage );
		fprintf( fp, " MoveP %3d    ->  MoveP %3d             Used Mana: %4d        \n",    a->MoveP, d->MoveP, mana );
		fprintf( fp, " Char  %3d    ->  Char  %3d                                   \n",    a->Char , d->Char );
		fprintf( fp, " Endu  %3d    ->  Endu  %3d                                   \n",    a->Endu , d->Endu ); 
		fprintf( fp, " Moral %3d    ->  Moral %3d                                   \n",    a->Moral, d->Moral); 
		fprintf( fp, " Luck  %3d    ->  Luck  %3d                                   \n",    a->Luck , d->Luck ); 
		fprintf( fp, " wsps  %3d    ->  wsps  %3d                                   \n",    a->wsps , d->wsps ); 
		
		fprintf( fp, "방어자의 마법저항치 \n" );
		fprintf(fp, "Resist Poison  %3d\n", d->GetBasicResist(RT_POISON));//030227 lsw
		fprintf(fp, "Resist Curse   %3d\n", d->GetBasicResist(RT_CURSE));
		fprintf(fp, "Resist Fire    %3d\n", d->GetBasicResist(RT_FIRE));
		fprintf(fp, "Resist Ice     %3d\n", d->GetBasicResist(RT_ICE));
		fprintf(fp, "Resist Elect   %3d\n", d->GetBasicResist(RT_ELECT));
		fprintf(fp, "Resist Holy    %3d\n", d->GetBasicResist(RT_HOLY));
	}	
	else
	{	
		fprintf( fp, "Weapon-Attack< %d:%s >  --> %s\n", attacktype, magicno, Magic_Ref[magicno].Han_Name,  d->Name  );
		fprintf( fp, "---%s(%3d,%3d) %02d.%02d %02d:%02d:%02d ---\n",  MapName, a->MoveSx, a->MoveSy, g_mon+1, g_day,g_hour, g_min, g_sec );
		
		fprintf(fp, " Attack(Lv:%3d/Exp:%9d/ladder:%3d/fame:%3d)\n", a->GetLevel(), a->Exp, a->LadderScore, a->fame); // CSD-030806
		fprintf(fp, " Defence(Lv:%3d/Exp:%9d)\n", d->GetLevel(), d->Exp); // CSD-030806
		
		int itemkind;
		char *at;
		const int ino = a->equip[ WT_WEAPON].item_no;
		CItem *ii = ItemUnit( ino/1000, ino%1000);
		
		if( ii )	itemkind = ii->GetItemKind();
		else		itemkind = IK_NONE;
		
		switch( itemkind )
		{			
			//	장거리 무기....
		case IK_DUAL_BOW:		at = "DUAL_BOW";	break;//030102 lsw Dual BOW
		case IK_SHORT_BOW	:	at = "SHORT_BOW";	break;
		case IK_TWO_ARROW	:	at = "TWO_ARROW";	break;
		case IK_DUAL_ARROW	:	at = "DUAL_ARROW";	break;//030102 lsw
		case IK_LONG_BOW	:	at = "LONG_BOW";	break;
		case IK_ARROW_POINT	:	at = "ARROW_POINT";	break;
		case IK_FEATHER		:	at = "FEATHER";	break;
			// 중장거리.. 
		case IK_HURL		:	at = "THROWING WEAPON"; break;
			//	단거리무기..
		default             :   at = "MELEE WEAPON";    break;
		}			
		
		char temp[ FILENAME_MAX];
		
		if( ii ) 
		{
			strcpy_s( temp, sizeof(temp), ii->GetHanName() );
		}
		else 
		{
			strcpy_s( temp, sizeof(temp), " NO WEAPON " );
		}
		
		fprintf( fp, " Reserved Point %3d -> Reserved Point %3d\n", a->GetReservedPoint(), d->GetReservedPoint());
		fprintf( fp, " Str   %3d	->  Str   %3d                Hp %4d/%4d ->  Hp %4d/%4d \n", a->Str, d->Str, a->Hp, a->HpMax, d->Hp, d->HpMax );
		fprintf( fp, " Con   %3d	->  Con   %3d              Mana %4d/%4d ->Mana %4d/%4d \n", a->Con, d->Con, a->Mana, a->ManaMax, d->Mana, d->ManaMax );
		fprintf( fp, " Dex   %3d	->  Dex   %3d            Hungry %4d/%4d ->Hngy %4d/%4d \n", a->Dex, d->Dex, a->Hungry, a->HungryMax, d->Hungry, d->HungryMax );
		fprintf( fp, " Wis   %3d	->  Wis   %3d                Ac  %3d    ->       %3d   \n", 	a->Wis, d->Wis, a->Ac, d->Ac );
		fprintf( fp, " Int   %3d	->  Int   %3d                                   \n",    a->Int, d->Int);
		fprintf( fp, " MoveP %3d	->  MoveP %3d              Damage  :    %4d     \n",    a->MoveP, d->MoveP, damage );
		fprintf( fp, " Char  %3d	->  Char  %3d             WEAPON :  %s( %d )  \n",    a->Char , d->Char,  temp, a->equip[ WT_WEAPON].item_no );
		fprintf( fp, " Endu  %3d	->  Endu  %3d           AttackType :    %s      \n",    a->Endu , d->Endu,  at );
		fprintf( fp, " Moral %3d	->  Moral %3d                                   \n",    a->Moral, d->Moral );
		fprintf( fp, " Luck  %3d	->  Luck  %3d                                   \n",    a->Luck , d->Luck );
		fprintf( fp, " wsps  %3d	->  wsps  %3d                                   \n",    a->wsps , d->wsps );
	}
	
	fprintf(fp, " [Tactic Info]\n");
	fprintf(fp, " Crapple              Tactics(%3d:%d)\n", a->Skill[TACTICS_Crapple], a->tac_skillEXP[0]);
	fprintf(fp, " Swordmanship         Tactics(%3d:%d)\n", a->Skill[TACTICS_swordmanship], a->tac_skillEXP[1]);
	fprintf(fp, " Archery              Tactics(%3d:%d)\n", a->Skill[TACTICS_Archery], a->tac_skillEXP[2]);
	fprintf(fp, " Fencing              Tactics(%3d:%d)\n", a->Skill[TACTICS_Fencing], a->tac_skillEXP[3]);
	fprintf(fp, " Mace Fighting        Tactics(%3d:%d)\n", a->Skill[TACTICS_Mace_fighting], a->tac_skillEXP[4]);
	fprintf(fp, " Pierce               Tactics(%3d:%d)\n", a->Skill[TACTICS_Pierce], a->tac_skillEXP[5]);
	fprintf(fp, " Whirl                Tactics(%3d:%d)\n", a->Skill[TACTICS_Whirl], a->tac_skillEXP[6]);
	fprintf(fp, " Hurl                 Tactics(%3d:%d)\n", a->Skill[TACTICS_Hurl], a->tac_skillEXP[7]);
	fprintf(fp, " Parrying             Tactics(%3d:%d)\n", a->Skill[TACTICS_Parrying], a->tac_skillEXP[8]);
	fprintf(fp, " Double Swordmanship  Tactics(%3d:%d)\n", a->Skill[TACTICS_Double_Swordmanship], a->tac_skillEXP[9]);
	fprintf(fp, " Double Mace Fighting Tactics(%3d:%d)\n", a->Skill[TACTICS_Double_MaceFighting], a->tac_skillEXP[10]);
	fprintf(fp, " Magery               Tactics(%3d:%d)\n", a->Skill[TACTICS_Magery], a->tac_skillEXP[11]);
	fprintf(fp, " Orison               Tactics(%3d:%d)\n", a->Skill[TACTICS_Orison], a->tac_skillEXP[12]);

	fprintf(fp, " [NK Info]\n");
	fprintf(fp, "N_NEUTRAL  = %d	N_NONSTER	= %d	N_VYSEUS = %d\n", a->nk[N_NEUTRAL], a->nk[N_NONSTER], a->nk[N_VYSEUS]);
	fprintf(fp, "N_ZYPERN	= %d	N_HEGEMONIA	= %d	N_YLLSE	 =%d\n", a->nk[N_ZYPERN	], a->nk[N_HEGEMONIA], a->nk[N_YLLSE]);
	
	fprintf(fp, " [Party Info]\n");
	
	for (int i = 0; i < MAX_PARTY_MEMBER; ++i)
	{
		if (a->party[i].On && a->party[i].Name)
		{ 
			fprintf(fp, " [%d]%20s%s", i, a->party[i].Name, (i==2)?"\n":"  ");
		}
	}

	fprintf(fp, "\n");
	fclose(fp);
}	//> CSD-TW-030622

void SaveVendorLog(LPCHARLIST pTarget)
{	//< CSD-TW-030622
	string strPath;

	if (!g_pLogManager->GetLogPath(LT_VENDOR, strPath))
	{
		return;
	}
	//< CSD-040224
	FILE* fp = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");

	if(fp == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, pTarget->Name);
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (fp == NULL)
	{
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pTarget->GetServerID()].id), "at+");

		if (fp == NULL)
		{
			return;
		}
	}
	//> CSD-040224
	
	fprintf(fp, " %04d.%02d.%02d %02d:%02d:%02d - %s	%d x %d", g_year, g_mon + 1, g_day, g_hour, g_min, g_sec, pTarget->MapName, pTarget->MoveSx, pTarget->MoveSy); // CSD-040202
	fprintf(fp, "\n");
	fclose(fp);
}	//> CSD-TW-030622

void SaveMantleDye(LPCHARLIST pTarget, int itemno, int limit, int colour)
{	//< CSD-TW-030622
	string strPath;

	if (!g_pLogManager->GetLogPath(LT_MANTLEDYE, strPath))
	{
		return;
	}
	//< CSD-040224
	FILE* fp = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");

	if(fp == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, pTarget->Name);
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (fp == NULL)
	{
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pTarget->GetServerID()].id), "at+");

		if (fp == NULL)
		{
			return;
		}
	}
	//> CSD-040224
	
	fprintf(fp, "%04d.%02d.%02d %02d:%02d:%02d - itemno:%d limit:%d colour:%d map:%s x:%d y:%d", g_year, g_mon + 1, g_day, g_hour, g_min, g_sec, itemno, limit, colour, pTarget->MapName, pTarget->MoveSx, pTarget->MoveSy); // CSD-040202
	fprintf(fp, "\n");
	fclose(fp);
}	//> CSD-TW-030622

void SaveNationTransfer(LPCHARLIST c)
{
	string strPath;
	if (!g_pLogManager->GetLogPath(LT_NATIONTRANSFER, strPath)) return;

	FILE* fp = fopen(VA("%s/%s.txt", strPath.c_str(), c->Name), "at+");

	if(fp == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, c->Name);
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (fp == NULL)
	{
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), connections[c->GetServerID()].id), "at+");
		if (fp == NULL) return;
	}

	fprintf(fp, "%04d.%02d.%02d %02d:%02d:%02d - map:%s x:%d y:%d", 
		g_year, g_mon + 1, g_day, g_hour, g_min, g_sec, c->MapName, c->MoveSx, c->MoveSy);
	fprintf(fp, "\n");
	fclose(fp);
}

void SaveSexChange(LPCHARLIST c)
{
	string strPath;
	if (!g_pLogManager->GetLogPath(LT_SEXCHANGE, strPath)) return;

	FILE* fp = fopen(VA("%s/%s.txt", strPath.c_str(), c->Name), "at+");

	if(fp == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, c->Name);
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (fp == NULL)
	{
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), connections[c->GetServerID()].id), "at+");
		if (fp == NULL) return;
	}

	fprintf(fp, "%04d.%02d.%02d %02d:%02d:%02d - map:%s x:%d y:%d", 
		g_year, g_mon + 1, g_day, g_hour, g_min, g_sec, c->MapName, c->MoveSx, c->MoveSy);
	fprintf(fp, "\n");
	fclose(fp);
}

void SaveLevelUp(LPCHARLIST pTarget)
{	//< CSD-TW-030622
	string strPath;

	if (!g_pLogManager->GetLogPath(LT_LEVEL_UP, strPath))
	{
		return;
	}
	//< CSD-040224
	FILE* fp = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");

	if(fp == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, pTarget->Name);
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (fp == NULL)
	{
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pTarget->GetServerID()].id), "at+");

		if (fp == NULL)
		{
			return;
		}
	}
	//> CSD-040224
	fprintf(fp, " %04d.%02d.%02d %02d:%02d:%02d - %s	%d x %d	\n ", g_year, g_mon + 1, g_day, g_hour, g_min, g_sec, pTarget->MapName, pTarget->MoveSx, pTarget->MoveSy); // CSD-040202

	fprintf(fp, " [Ability Info]\n");
	fprintf(fp, " Level %3d     Exp   %3d     Step  %3d\n", pTarget->GetLevel(), pTarget->Exp, pTarget->GetExperienceStep()); // CSD-030806
	fprintf(fp, " Class %3d     Step  %3d     Dual  %3d\n", pTarget->Class, pTarget->GetClassStep(), pTarget->GetDualClass());
	fprintf(fp, " Str   %3d     Con   %3d     Dex   %3d\n", pTarget->Str, pTarget->Con, pTarget->Dex);
	fprintf(fp, " Wis   %3d     Int   %3d     MoveP %3d\n", pTarget->Wis, pTarget->Int, pTarget->MoveP);
	fprintf(fp, " Char  %3d     Endu  %3d     Moral %3d\n", pTarget->Char, pTarget->Endu, pTarget->Moral);
	fprintf(fp, " Luck  %3d     wsps  %3d     Reserved Point %3d\n", pTarget->Luck, pTarget->wsps, pTarget->GetReservedPoint());
	
	fprintf(fp, " [Tactic Info]\n");
	fprintf(fp, " Crapple              Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Crapple], pTarget->tac_skillEXP[0]);
	fprintf(fp, " Swordmanship         Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_swordmanship], pTarget->tac_skillEXP[1]);
	fprintf(fp, " Archery              Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Archery], pTarget->tac_skillEXP[2]);
	fprintf(fp, " Fencing              Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Fencing], pTarget->tac_skillEXP[3]);
	fprintf(fp, " Mace Fighting        Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Mace_fighting], pTarget->tac_skillEXP[4]);
	fprintf(fp, " Pierce               Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Pierce], pTarget->tac_skillEXP[5]);
	fprintf(fp, " Whirl                Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Whirl], pTarget->tac_skillEXP[6]);
	fprintf(fp, " Hurl                 Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Hurl], pTarget->tac_skillEXP[7]);
	fprintf(fp, " Parrying             Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Parrying], pTarget->tac_skillEXP[8]);
	fprintf(fp, " Double Swordmanship  Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Double_Swordmanship], pTarget->tac_skillEXP[9]);
	fprintf(fp, " Double Mace Fighting Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Double_MaceFighting], pTarget->tac_skillEXP[10]);
	fprintf(fp, " Magery               Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Magery], pTarget->tac_skillEXP[11]);
	fprintf(fp, " Orison               Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Orison], pTarget->tac_skillEXP[12]);
	
	fprintf(fp, " [Resist Info]\n");
	fprintf(fp, " Resist Poison  %3d\n", pTarget->GetBasicResist(RT_POISON));//030227 lsw
	fprintf(fp, " Resist Curse   %3d\n", pTarget->GetBasicResist(RT_CURSE));
	fprintf(fp, " Resist Fire    %3d\n", pTarget->GetBasicResist(RT_FIRE));
	fprintf(fp, " Resist Ice     %3d\n", pTarget->GetBasicResist(RT_ICE));
	fprintf(fp, " Resist Elect   %3d\n", pTarget->GetBasicResist(RT_ELECT));
	fprintf(fp, " Resist Holy    %3d\n", pTarget->GetBasicResist(RT_HOLY));
	
	fprintf(fp, " [Combat Skill Info]\n");
	fprintf(fp, " Lightning Boom    %3d  Thunder Blow      %3d\n", pTarget->GetCombatLevel(LIGHTNING_BOOM), pTarget->GetCombatLevel(THUNDER_BLOW));
	fprintf(fp, " Lightning Shock   %3d  Thunder Strike    %3d\n", pTarget->GetCombatLevel(LIGHTNING_SHOCK), pTarget->GetCombatLevel(THUNDER_STRIKE));
	fprintf(fp, " Lightning Shield  %3d  Lightning Extreme %3d\n", pTarget->GetCombatLevel(LIGHTNING_SHIELD), pTarget->GetCombatLevel(LIGHTNING_EXTREME));
	fprintf(fp, " Blood Earth       %3d  Ground Attack     %3d\n", pTarget->GetCombatLevel(BLOOD_EARTH), pTarget->GetCombatLevel(GROUND_ATTACK));
	fprintf(fp, " Blood Will        %3d  Stone Armor       %3d\n", pTarget->GetCombatLevel(BLOOD_WILL), pTarget->GetCombatLevel(STONE_ARMOR));
	fprintf(fp, " Simunian Bosom    %3d  Earth Extreme     %3d\n", pTarget->GetCombatLevel(SIMUNIAN_BOSOM), pTarget->GetCombatLevel(EARTH_EXTREME));
	fprintf(fp, " Sword N'Roses     %3d  Multiple Fire     %3d\n", pTarget->GetCombatLevel(SWORD_N_ROSES), pTarget->GetCombatLevel(MULTIPLE_FIRE));
	fprintf(fp, " Fleet Missile     %3d  Converting Armor  %3d\n", pTarget->GetCombatLevel(FLEET_MISSILE), pTarget->GetCombatLevel(CONVERTING_ARMOR));
	fprintf(fp, " Burning Bloody	%3d  Fire Extreme      %3d\n", pTarget->GetCombatLevel(BURNING_BLOODY), pTarget->GetCombatLevel(FIRE_EXTREME)); // 030415 kyo 
	fprintf(fp, " Poisoning Nova    %3d  Cherroy Shade     %3d\n", pTarget->GetCombatLevel(POISONING_NOVA), pTarget->GetCombatLevel(CHERROY_SHADE));
	fprintf(fp, " Dark Blade        %3d  Tybern Gift       %3d\n", pTarget->GetCombatLevel(DARK_BLADE), pTarget->GetCombatLevel(TYBERN_GIFT));
	fprintf(fp, " AcuquireDark      %3d  Dark Extreme      %3d\n", pTarget->GetCombatLevel(ACUQUIRE_DARK), pTarget->GetCombatLevel(DARK_EXTREME)); // 030415 kyo 
	fprintf(fp, " Icing Blast       %3d  Critical Hiding   %3d\n", pTarget->GetCombatLevel(ICING_BLAST), pTarget->GetCombatLevel(CRITICAL_HIDING));
	fprintf(fp, " Shark Missile     %3d  Horn Of Iceberg   %3d\n", pTarget->GetCombatLevel(SHARK_MISSILE), pTarget->GetCombatLevel(HORN_OF_ICEBERG));
	fprintf(fp, " Ice Shield        %3d  Ice Extreme       %3d\n", pTarget->GetCombatLevel(ICE_SHIELD), pTarget->GetCombatLevel(ICE_EXTREME));
	fprintf(fp, " Charging          %3d  Double Attack     %3d\n", pTarget->GetCombatLevel(CHARGING), pTarget->GetCombatLevel(DOUBLE_ATTACK));
	fprintf(fp, " Whilwind          %3d  Twister           %3d\n", pTarget->GetCombatLevel(WHILWIND), pTarget->GetCombatLevel(TWISTER));
	fprintf(fp, " Gust              %3d  Wind Extreme      %3d\n", pTarget->GetCombatLevel(GUST), pTarget->GetCombatLevel(WIND_EXTREME));
	fprintf(fp, " Combat Skill Point %3d \n", pTarget->GetCombatPoint());
	
	fprintf(fp, " [NK Info]\n");
	fprintf(fp, " N_NEUTRAL %d	N_NONSTER   %d	N_VYSEUS %d\n", pTarget->nk[N_NEUTRAL], pTarget->nk[N_NONSTER], pTarget->nk[N_VYSEUS]);
	fprintf(fp, " N_ZYPERN	%d	N_HEGEMONIA	%d	N_YLLSE  %d\n", pTarget->nk[N_ZYPERN], pTarget->nk[N_HEGEMONIA], pTarget->nk[N_YLLSE]);
	
	fprintf(fp, " [Party Info]\n");
	for (int i = 0; i < MAX_PARTY_MEMBER; ++i)
	{
		if (pTarget->party[i].On && pTarget->party[i].Name)
		{ 
			fprintf(fp, " [%d]%20s%s", i, pTarget->party[i].Name, (i==2)?"\n":"  ");
		}
	}
	
	fprintf(fp, "\n");
	fclose(fp);
}	//> CSD-TW-030622

void SaveTacticUp(int nKind, int nLevel, int EventPC) //Eleval 15/08/09 - Saves when a player's TT are lvl up
{
	CHARLIST *pTarget = CheckServerId(EventPC);
	string strPath;
	if (!g_pLogManager->GetLogPath(LT_TACTICUP, strPath)) return;

	FILE* fp = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");
	
	if(fp == NULL) //Eleval 16/08/09 - For chars with special chars (like *)
	{
		char file[FILENAME_MAX];
		RewritePathWithSpecialCharacters(file, pTarget->Name);
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), file), "at+");
	}

	if (fp == NULL)
	{
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pTarget->GetServerID()].id), "at+");
		if (fp == NULL) return;
	}

	fprintf(fp, " %04d.%02d.%02d %02d:%02d:%02d - %s	%d x %d	\n ", g_year, g_mon + 1, g_day, g_hour, g_min, g_sec, pTarget->MapName, pTarget->MoveSx, pTarget->MoveSy); // CSD-040202

	fprintf(fp, " [Tactic Info Level Up]\n");
	switch(nKind)
	{
	case 0:
		fprintf(fp, " Crapple              Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Crapple], pTarget->tac_skillEXP[0]);
		break;
	case 1:
		fprintf(fp, " Swordmanship         Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_swordmanship], pTarget->tac_skillEXP[1]);
		break;
	case 2:
		fprintf(fp, " Archery              Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Archery], pTarget->tac_skillEXP[2]);
		break;
	case 3:
		fprintf(fp, " Fencing              Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Fencing], pTarget->tac_skillEXP[3]);
		break;
	case 4:
		fprintf(fp, " Mace Fighting        Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Mace_fighting], pTarget->tac_skillEXP[4]);
		break;
	case 5:
		fprintf(fp, " Pierce               Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Pierce], pTarget->tac_skillEXP[5]);
		break;
	case 6:
		fprintf(fp, " Whirl                Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Whirl], pTarget->tac_skillEXP[6]);
		break;
	case 7:
		fprintf(fp, " Hurl                 Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Hurl], pTarget->tac_skillEXP[7]);
		break;
	case 8:
		fprintf(fp, " Parrying             Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Parrying], pTarget->tac_skillEXP[8]);
		break;
	case 9:
		fprintf(fp, " Double Swordmanship  Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Double_Swordmanship], pTarget->tac_skillEXP[9]);
		break;
	case 10:
		fprintf(fp, " Double Mace Fighting Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Double_MaceFighting], pTarget->tac_skillEXP[10]);
		break;
	case 11:
		fprintf(fp, " Magery               Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Magery], pTarget->tac_skillEXP[11]);
		break;
	case 12:
		fprintf(fp, " Orison               Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Orison], pTarget->tac_skillEXP[12]);
		break;
	default:
		fprintf(fp, " Unknown %d           Tactics(Lv.)\n", nKind, nLevel);
		break;
	}

	fprintf(fp, "\n");
	fclose(fp);
}
//----------------------------------------------------------------------------
// 010502 YGI
// 주문을 읇었을 경우
// acer5
void Recv_CMD_PLEASE_GIVE_LIFE(int cn, int alive_word_type)
{	
	LPCHARLIST ch = CheckServerId( cn );
	if (ch == NULL) return; // finito null check
	bool alive_flag = false;
	
	if( ch->viewtype != VIEWTYPE_GHOST_ ) return; //유령타입도 아니면서..
	if( g_stop_please_give_life ) return;		// 신전의 기능을 막을수 있다. ( 되살아니는 )..
	if( ch->reporter_mode ) return;		// 기자모드일경우엔 해당없다.		// 011128 YGI
	
	int ret = g_AliveMap.CheckAlive( ch->MoveSx, ch->MoveSy, MapNumber, alive_word_type );
	if( ret < 0 ) return;		// 살아 날수 있는 곳이 아니다.
	
	if( ret == 100 )		// 살려 준다.
	{
		SkillMgr.CharacterToAlive(ch);// 살려준다.//020501 lsw
	}
	else		// 살아나는 곳이긴 하지만 주문이 틀렸다.
	{
		char ret_alive_word_type = ret;
		SendPacketDefault( CMD_ALIVE_FAIL_RESULT, (char*)&ret_alive_word_type, sizeof( char), cn );
	}
	return;
}

void Recv_CMD_PLEASE_GIVE_LIFE_GM(int cn, int id)
{	
	LPCHARLIST ch = CheckServerId( cn );
	if (ch == NULL) return; // finito null check
	if (!ch->IsCounselor()) return;
	
	LPCHARLIST target = CheckServerId(id);
	if (target == NULL) return;

	CHARLIST* pTarget = NULL;
	CHARLIST *pUser = CheckServerId( cn );
	if( !pUser ) return;
	if( !pUser->IsCounselor() ) return;
	
	SkillMgr.CharacterToAlive(target, 1);// 살려준다.//020501 lsw
	CrackMgr.SendHackingUser( cn, USE_GM_TOOL, "CMD_GIVE_LIFE" );
	return;
}

///////////////////////// 0726 lkh 추가 ///////////////////////////
void	TameNpcAI( LPCHARLIST master )			//주인에게 속해 있는 테밍NPC의 행동을 제어
{
	LPCHARLIST	tame_Char=NULL, target	= NULL;
	int			distance, distance_Min	= 1000;
	
	for( int i = NPC_LIST_START ; i < MAX_NPC_LIST ; i ++ )	//태밍한 NPC를 찾는다.
	{
		if (NPCList[i].bAlive == ALIVE_ && NPCList[i].IsTamedNpc())
			if( NPCList[i].HostName[0] )
				if( !strcmp(NPCList[i].HostName, master->Name ) )
				{
					tame_Char = &NPCList[i];
					break;
				}
	}
	if(!tame_Char)	return;						//주인이 테밍한 NPC가 없는 경우
	
	for( int j = NPC_LIST_START ; j < MAX_NPC_LIST ; j ++ )	//공격할 대상을 찾는다.
	{
		if( NPCList[j].bAlive == ALIVE_ && tame_Char!=&NPCList[j] )
		{
			distance = sqrt( pow( (float)(master->X-NPCList[j].X),(float)2 ) + pow( (float)(master->Y-NPCList[j].Y),(float)2 ) );
			if(distance<distance_Min)
			{
				distance_Min = distance;
				target = &NPCList[j];
			}
		}
	}
	if(!target)		return;						//공격할 대상이 없다.
	
	if( target->SprType == SPRITETYPE_NPC ) 
	{
		tame_Char->targetid = target->GetServerID() + 10000;
		SendNPC_Parameter( tame_Char->GetServerID(), NPC_PARAMETER_TARGETID, tame_Char->targetid );
	}
	else 
	{
		tame_Char->targetid = target->GetServerID();
		SendNPC_Parameter( tame_Char->GetServerID(), NPC_PARAMETER_TARGETID, tame_Char->targetid );
	}
	
	tame_Char->tame_cmd = LC_TAME_ATTACK;
	SendNPC_Parameter( tame_Char->GetServerID(), NPC_PARAMETER_TAME_CMD, LC_TAME_ATTACK );
	strcpy_s( tame_Char->TargetName, sizeof(tame_Char->TargetName), target->Name );
	SendTameResult( master->GetServerID(), 5 );	//TAME_ATTACK
	SendNPC_StringParameter( tame_Char->GetServerID(), NPC_PARAMETER_TARGETNAME, tame_Char->TargetName );
}
// PK를 할수 있는 Flag를 보낸다.
//void SendStartGoodBad( int id )
void SendNK( int id, int nk )
{			
	t_packet p;
	
	if( id >= 10000 ) return;
	
	p.h.header.type = CMD_START_GOODBAD;
	p.u.start_goodbad.id		= id;
	p.u.start_goodbad.nk3       = nk;
	p.h.header.size = sizeof( t_start_goodbad );
	
	QueuePacket( connections, id, &p, 1 );
	CastMe2Other( id, &p );
}			

void SendNK( int id, int nk3, int nk4, int nk6 )
{			
	t_packet p;
	
	if( id >= 10000 ) return;
	
	p.h.header.type = CMD_START_GOODBAD;
	p.u.start_goodbad.id		= id;
	p.u.start_goodbad.nk3        = nk3;
	p.u.start_goodbad.nk4        = nk4;
	p.u.start_goodbad.nk6        = nk6;
	p.h.header.size = sizeof( t_start_goodbad );
	
	QueuePacket( connections, id, &p, 1 );
	CastMe2Other( id, &p );
}

void SaveCharacterBin( char *filename, CHARLIST *ch )
{
	FILE *fp;
	char full_name[MAX_PATH];
	sprintf_s( full_name, sizeof(full_name), "%d_%d_%d_%d_%s.bin", g_mon+1, g_day, g_hour, g_min, filename );
	fp = fopen( full_name, "wb" );
	if( !fp ) return;
	
	fwrite( ch, sizeof(CHARLIST), 1, fp );
	fclose( fp );
}