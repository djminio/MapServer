
#include "..\stdafx.h"
#include "DefaultHeader.h"
#include "CItem.h"
#include "Map.h"
#include "OP_Magic.h"
#include "ChrLog.h"
#include "teambattle.h"
#include "ArenaManager.h"

#define NUM_OF_RACE	9
#define NUM_OF_KIND 8

extern NPCLev_to_Exp		NPC_Lev_Ref[] ;
extern NPC_Generation		NPC_Gen_Ref[];	// 010129 KHS
extern CItem_Weapon			CWeapon[] ;
extern inline int IsNKNation( LPCHARLIST ch );
extern inline int IsHeNK(  LPCHARLIST ch , int nation );

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///		User Functions Declaration...
///

int InitPartyMemberServerToServer( CHARLIST *ch );		//  이름만 디비에 저장하고 이름만 꺼내온다.
int EndsetPartyMenberForUpdate( CHARLIST *ch );
int SetMySkill( CHARLIST *ch );		// 일단 기본적으로 알아야하는 스킬 셋



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///		User Functions.
///

void SendLevelUpMsgAndPoint( CHARLIST *chr )		// 1217
{
	t_packet p;
	p.h.header.type = CMD_LEVELUP_POINT;
	p.h.header.size = sizeof(t_server_levelup_point);
	p.u.server_levelup_point.idServer = chr->GetServerID();
	p.u.server_levelup_point.nPoint	= chr->GetReservedPoint();
	p.u.server_levelup_point.nLevel = chr->GetLevel(); // CSD-030806
	p.u.server_levelup_point.nLife = chr->Hp;
	chr->GetStepInfo(p.u.server_levelup_point.aStepInfo, sizeof(p.u.server_levelup_point.aStepInfo));
	QueuePacket(connections, chr->GetServerID(), &p, 1);
	CastMe2Other( chr->GetServerID(), &p );
}

void RecvLevelUpPoint(int cn, t_client_levelup_point *p)
{		
	t_packet packet;
	
	CHARLIST *ch = &connections[cn].chrlst;
	if (!ch) return;
	int t = 0;
	int i;
	
	for( i = 0 ; i < 11 ; i ++)	t += p->aPoint[i];
	
	if (t == ch->GetReservedPoint())
	{	
		ch->Str		+= p->aPoint[0];				//  힘
		ch->Dex		+= p->aPoint[1];				//  건강
		ch->Con		+= p->aPoint[2];				//	민첩성
		ch->Wis		+= p->aPoint[3];				//	지혜
		ch->Int		+= p->aPoint[4];				//	지능
		ch->Char	+= p->aPoint[5];				//	이동력
		ch->MoveP	+= p->aPoint[6];				//	매력
		ch->Endu	+= p->aPoint[7];				//  저향력
		ch->Moral	+= p->aPoint[8];				//	체력
		ch->wsps	+= p->aPoint[9];				//	사기
		ch->Luck	+= p->aPoint[10];			  //	행운
		
		packet.h.header.type = CMD_LEVELUP_POINT_OK;
		packet.h.header.size = 0;
		QueuePacket(connections, cn, &packet, 1);
	}													
	else										
	{										
		packet.h.header.type = CMD_LEVELUP_POINT_NO;
		packet.h.header.size = 0;
		QueuePacket(connections, cn, &packet, 1);
	}															
}	

void DropItem(CHARLIST* pTarget)
{	//< CSD-031210
	for (int i = 0; i < 5; i++)
	{	
		const int t1 = rand()%3;
		const int t2 = rand()%3;
		const int t3 = rand()%8;
		ItemAttr* pItem = &pTarget->inv[t1][t2][t3];
		ItemAttr	SaveItem = *pItem;

		if (pItem->item_no)
		{
			CItem* i = ItemUnit(pItem->item_no);
			if (!i) continue;
			if (IsNoExchange(pItem)) continue;

			POS pos;
			SetItemPos(INV, t1, t2, t3 , &pos);

			if (SendDeleteItem(pItem, &pos, pTarget))
			{	// 020428 YGI acer
				AddItemList(SaveItem.item_no, SaveItem.attr, 0, 
							pTarget->X + (rand()%64) + 16, 
							pTarget->Y + (rand()%64) + 16, 
							0, 0, 0, 0, 0, 0);
				SendItemEventLog(&SaveItem,
								 pTarget->GetServerID(), 
								 SN_NOT_USER, 
								 SILT_DROP_BY_PK, 
								 1);
			}
		}
	}
}	//> CSD-031210

extern bool IsActiveDugeon();

void ExpDown(CHARLIST* pCaster, CHARLIST* pTarget, bool bDrop)
{	//< CSD-030930
#ifdef LMS_MAPSERVER
	return;
#endif
	if (g_pArenaManager->IsColossusArena()){return;}
	if (IsActiveDugeon()){return;}
	if (pTarget == NULL){return;}

	if (pTarget->IsPlayer())
	{
		if (pCaster == NULL)
		{	// 공격자가 없이 죽은 경우
			if (IsHeNK(pTarget, MapInfo[MapNumber].nation))
			{	// NK인 경우에 죽었을 경우 
				if (bDrop)
				{	
					DropItem(pTarget);
				}

				if (!pTarget->DecExperience(true))
				{
					const int nKind = pTarget->GetTacticsKind() - TACTICS_Crapple;
					pTarget->DecTacticExp(nKind, true);
				}
				
				return;
			}

			pTarget->DecExperience(false);
			return;
		}
		
		if (IsManToManFight(pCaster, pTarget))
		{	
			return;
		}	

		if (IsHeNK(pTarget, MapInfo[MapNumber].nation))
		{	// NK인 경우에 죽었을 경우 
			if (bDrop)
			{	
				DropItem(pTarget);
			}

			if (!pTarget->DecExperience(true))// decreases exp nk
			{
				const int nKind = pTarget->GetTacticsKind() - TACTICS_Crapple;
				pTarget->DecTacticExp(nKind, true);
			}

			return;
		}

		if (pCaster->IsNpc() && !pCaster->IsTamedNpc())
		{
			pTarget->DecExperience(false);
			return;	
		}
	}
}	//> CSD-030930

bool isLevelUp(CHARLIST *chr)		
{	//< CSD-020809
	if (chr && chr->LevelUpAbility())
	{
		SendLevelUpMsgAndPoint(chr);
		SendLog2ChrLogDB(connections, chr->GetServerID(), CHR_LOG_LEVELUP_);
		::SaveLevelUp(chr);
		return true;
	}
	
	return false;
}	//> CSD-020809	

//  to에게 from의 Data를 보낸다..
void UserAddBasicData( t_connection c[], int from, int to, t_server_user_add *p )
{
	CHARLIST *ch = &c[ from].chrlst;
	if (!ch) return;
	
	p->namelen = strlen( ch->Name );
	memcpy(p->name, ch->Name, NM_LENGTH);
	
	p->server_id  = from;
	
	p->spr_no		= ch->SprNo;
	p->spell		= ch->Spell;
	p->face			= ch->Face;
	
	p->body_r		= ch->BodyR>>3;
	p->body_g		= ch->BodyG>>3;
	p->body_b		= ch->BodyB>>3;
	p->cloth_r		= ch->ClothR>>3;
	p->cloth_g		= ch->ClothG>>3;
	p->cloth_b		= ch->ClothB>>3;
	
	p->equip0		= ch->accessory[0];
	p->equip1		= ch->accessory[1];
	p->equip2		= ch->accessory[2];
	p->equip3		= ch->accessory[3];
	p->mantle		= ch->mantle;   // 011018 KHS 망또 
	p->jjing        = ((RareMain*)&(ch->equip[ WT_ARMOR].attr[ IATTR_RARE_MAIN]))->grade; // 011030 KHS JJING
	
	p->peacests		= ch->Peacests;
	
	p->bAlive		= ch->bAlive;
	
	p->x			= ch->MoveSx;
	p->y			= ch->MoveSy;
	
	p->viewtype		= ch->viewtype;
	
	p->lv			= ch->GetLevel(); // CSD-030806
	p->class_type	= ch->Class;
	
	// 1014 YGI -----------------------------------
	p->nation		= ch->name_status.nation;
	p->guild_master	= ch->name_status.guild_master;
	p->king			= ch->name_status.king;
	p->guild_code	= ch->GetGuildCode(); // CSD-030324
	p->counselor	= ch->name_status.counselor;
	//---------------------------------------------
	
	p->nk3		= IsHeNK( ch, N_VYSEUS );
	p->nk4		= IsHeNK( ch, N_ZYPERN );
	p->nk6		= IsHeNK( ch, N_YLLSE );
	
	p->SquadNo			=	ch->NWCharacter.SquadNo;				// 011019 LTS
	p->isCommander		=	ch->NWCharacter.isCommander;			// 010904 LTS
	p->isSquadLeader	=	ch->NWCharacter.isSquadLeader;			// LTS CHAR
	p->YL_JoinNation	=	ch->NWCharacter.YL_JoinNation;
	
	p->nDualCls = ch->GetDualClass();
	p->nClsStep = ch->GetClassStep();
	p->idWeaponItem = ch->equip[WT_WEAPON].item_no; // CSD-031001
	
	p->reporter = ch->name_status.reporter;
	p->nCurrentAction	=	ch->nCurrentAction;
	p->ChairNum			=	ch->ChairNum; //앉기 추후 말타기 춤추기등..			// LTS SITDOWN BUG
	p->JoinLocalWar		=	ch->JoinLocalWar;						// 011214 LTS
	
	p->HorseNo			=	ch->HorseNo;							// LTS HORSERIDER
	p->RiderNo			=	ch->RiderNo;							// LTS HORSERIDER
	p->HorseGrade		=	ch->HorseGrade;							// LTS HORSERIDER
	p->RiderGrade		=	ch->RiderGrade;							// LTS HORSERIDER
	p->DragonLordWarTeam = ch->DragonLordWarTeam;					// LTS DRAGONLORD

	p->MantleDyeColour = ch->equip[WT_NECK].attr[IATTR_RESERVED1]; // Finito mantle dye

	//Char's options by Eleval
	p->hat_is_hidden = ch->hat_is_hidden; //Eleval 18/08/09
	//End of it
}	

void PutPacketCharDB( t_connection c[], int cn, t_packet *packet )
{	
	CHARLIST *ch = &c[cn].chrlst;
	if (!ch) return;
	
	t_server_user_db_data* p = &packet->u.server_user_db_data;
	p->lev = ch->GetLevel(); // CSD-030806
	p->exp = ch->Exp;
	
	p->Attr[0] = ch->Gender;
	p->Attr[1] = ch->Face;
	p->Attr[2] = ch->GetGuildCode(); // CSD-030324
	p->Attr[3] = ch->Class;
	p->Attr[4] = ch->Spell;
	p->Attr[5] = ch->Tactics;
	
	p->Avility[STR]	= ch->Str;		// 1118
	p->Avility[CON]	= ch->Con;
	p->Avility[DEX]	= ch->Dex;
	p->Avility[WIS]	= ch->Wis;
	p->Avility[INT_] = ch->Int;
	p->Avility[MOVP] = ch->MoveP;
	p->Avility[CHA]	= ch->Char;
	p->Avility[ENDU] = ch->Endu;
	p->Avility[MOR]	= ch->Moral;
	p->Avility[LUCK] = ch->Luck;
	p->Avility[WSPS] = ch->wsps;
	
	p->money = ch->Money;
	p->nLife = ch->Hp;
	p->nMaxHp = ch->HpMax;
	p->nMana = ch->Mana;
	p->nMaxMp = ch->ManaMax;
	p->nHungry = ch->Hungry;
	p->nMaxHungry = ch->HungryMax;
	p->condition = ch->GetState();
	p->sprno = ch->SprNo;
	p->x = ch->X;
	p->y = ch->Y;
	
	p->acc_equip1 = ch->accessory[0];
	p->acc_equip2 = ch->accessory[1];
	p->acc_equip3 = ch->accessory[2];
	p->acc_equip4 = ch->accessory[3];
							
	ch->MoveSx = ch->X/TILE_SIZE;
	ch->MoveSy = ch->Y/TILE_SIZE;
	
	memcpy(p->mapname, ch->MapName, NM_LENGTH);

	p->sight = ch->Sight;
	
	p->bodyr = (unsigned char)ch->BodyR;
	p->bodyg = (unsigned char)ch->BodyG;
	p->bodyb = (unsigned char)ch->BodyB;
	
	p->clothr = (unsigned char)ch->ClothR;
	p->clothg = (unsigned char)ch->ClothG;
	p->clothb = (unsigned char)ch->ClothB;
	
	p->Age = (unsigned char)ch->Age;			// 1118	
	p->openhouse = ch->openhouse;
	p->reserved_point = ch->GetReservedPoint();				// 1221
	p->viewtype	= ch->viewtype;
	
	p->social_status = ch->social_status;
	memcpy(&p->nation, &ch->name_status, sizeof(DWORD));	// 1004 YGI
	
	p->fame = ch->fame;
	p->NWCharacter = ch->NWCharacter;					// 011019 LTS
	p->nk3 = IsHeNK(ch, N_VYSEUS);
	p->nk4 = IsHeNK(ch, N_ZYPERN);
	p->nk6 = IsHeNK(ch, N_YLLSE);
	ch->GetStepInfo(p->aStepInfo, sizeof(p->aStepInfo));
}	

void SendItemInventory( int count, t_connection c[], int cn )		//1218
{
	int i = count /3;
	int j = count %3;
	
	t_packet packet;
	packet.h.header.type = CMD_CHR_ITEM_INFO_0;
	packet.h.header.size = sizeof(t_chr_item_info0 );
	
	packet.u.chr_item_info0.ar0 = i;
	packet.u.chr_item_info0.ar1 = j;
	memcpy( (unsigned char *)packet.u.chr_item_info0.inv, 
		(unsigned char *)c[ cn].chrlst.inv[i][j],	sizeof( ItemAttr ) * 8 );
	
	QueuePacket( c, cn, &packet, 1);
}

// 8개..
void SendItemEquip( t_connection c[], int cn )
{
	t_packet packet;
	
	packet.h.header.type = CMD_CHR_ITEM_INFO_1;
	{
		memcpy( (unsigned char *)packet.u.chr_item_info1.equip, 
			(unsigned char *)c[ cn].chrlst.equip,	sizeof( ItemAttr ) * 8 );
	}
	packet.h.header.size = sizeof(t_chr_item_info1 );
	QueuePacket( c, cn, &packet, 1);
}

void SendItemQuick( t_connection c[], int cn )
{
	t_packet packet;
	
	packet.h.header.type = CMD_CHR_ITEM_INFO_2;
	{
		memcpy( (unsigned char *)packet.u.chr_item_info2.quick, 
			(unsigned char *)c[ cn].chrlst.quick,	sizeof( ItemAttr ) * 6 );
	}
	packet.h.header.size = sizeof(t_chr_item_info2 );
	QueuePacket( c, cn, &packet, 1);
}

void SendCharInfoMagic( t_connection c[], int cn )
{
	t_packet packet;
	
	packet.h.header.type = CMD_CHAR_INFO_MAGIC;
	{
		if( c[cn].chrlst.Spell == 0 ) // Wizard Spell..
			memcpy( packet.u.char_info_magic.magic, c[ cn].chrlst.Ws, 200 );
		else 
			memcpy( packet.u.char_info_magic.magic, c[ cn].chrlst.Ps, 200 );
	}
	packet.h.header.size = sizeof( t_char_info_magic );
	QueuePacket( c, cn, &packet, 1);
}

void SendCharInfoSkill( t_connection c[], int cn )
{
	t_packet packet;
	
	packet.h.header.type = CMD_CHAR_INFO_SKILL;
	{
		memcpy( packet.u.char_info_skill.skill, c[ cn].chrlst.Skill, 200 );
	}
	packet.h.header.size = sizeof( t_char_info_skill);
	QueuePacket( c, cn, &packet, 1);
}

void SendCharInfotac_skillEXP( t_connection c[], int cn )
{
	t_packet packet;
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;
	
	packet.h.header.type = CMD_CHAR_INFO_TAC_SKILL_EXP;
	{
		memcpy( packet.u.char_info_tac_skillexp.tac_skillEXP, ch->tac_skillEXP, 13 * sizeof( DWORD ) );
		ch->GetStepInfo( packet.u.char_info_tac_skillexp.aStepInfo, 20 );
	}
	packet.h.header.size = sizeof( t_char_info_tac_skillexp );
	QueuePacket( c, cn, &packet, 1);
}

extern void SendCMD_LOCALWAR_STARTUP_STATUS(CHARLIST* ch); // 011215 LTS
// 011101 YGI
void CheckCharacterWhenJoinGame( CHARLIST *ch )
{
	CheckInitSkill( ch );
	InitPartyMemberServerToServer( &connections[ch->GetServerID()].chrlst );
	SendCMD_LOCALWAR_STARTUP_STATUS(ch);				// 011215 LTS
	return;
}

int InitPartyMemberServerToServer( CHARLIST *ch )		//  이름만 디비에 저장하고 이름만 꺼내온다.		// 0620 YGI
{
	int i;
	for( i=0; i<MAX_PARTY_MEMBER; i++ )
	{
		if( *ch->party_str[i] ) 
		{
			strcpy_s( ch->party[i].Name, sizeof(ch->party[i].Name), ch->party_str[i] );
			ch->party[i].On = true;
		}
	}
	for( i=0; i<3; i++ )
	{
		if( *ch->relation_str[i] )
		{
			strcpy_s( ch->relation[i].Name, sizeof(ch->relation[i].Name), ch->relation_str[i] );
			ch->relation[i].On = true;
		}
	}
	for( i=0; i<6; i++ )
	{
		if( *ch->employment_str[i] )
		{
			strcpy_s( ch->employment[i].Name, sizeof(ch->employment[i].Name), ch->employment_str[i] );
			ch->employment[i].On = true;
		}
	}
	return 1;
}

int EndsetPartyMenberForUpdate( CHARLIST *ch )
{
	int i;
	for( i=0; i<MAX_PARTY_MEMBER; i++ )
	{
		strcpy_s( ch->party_str[i], sizeof(ch->party_str[i]) ,ch->party[i].Name );
	}
	for( i=0; i<3; i++ )
	{
		strcpy_s( ch->relation_str[i], sizeof(ch->relation_str[i]), ch->relation[i].Name );
	}
	for( i=0; i<6; i++ )
	{
		strcpy_s( ch->employment_str[i], sizeof(ch->employment_str[i]), ch->employment[i].Name );
	}
	return 1;
}






//######################## login server 용 ##########################

void SendItemIndex( t_connection c[], int cn )		// 배운 아이템 보내주기
{
	CHARLIST *ch = &c[cn].chrlst;
	if (!ch) return;
	t_packet packet;
	packet.h.header.type = CMD_ITEM_INDEX;
	{
		memcpy( packet.u.kein.server_item_index.item, ch->Item, c[cn].chrlst.ItemMax  * sizeof( short int ) );
	}
	packet.h.header.size = sizeof(k_server_item_index);
	QueuePacket(c, cn, &packet, 1);
}

void SendEmployment( t_connection c[], int cn )		// employment 보내주기 //1229
{
	return;
}

void SendRelation( t_connection c[], int cn )		// 사제 관계보내주기		//1229
{
	return;
}
void SendParty( t_connection c[], int cn )			// 파티원 보내주기	//0213 YGI
{
	CHARLIST *ch = &c[cn].chrlst;
	if (!ch) return;
	t_packet packet;
	
	memset( &packet, 0, sizeof( t_packet ) );
	packet.h.header.type = CMD_PARTY;
	{
		for( int i=0; i<MAX_PARTY_MEMBER; i++ )
		{
			if( ch->party[i].On ) 
			{
				packet.u.kein.server_party.Level[i]	= ch->party[i].Level;
				packet.u.kein.server_party.Gender[i]= ch->party[i].Gender;
				packet.u.kein.server_party.Face[i]	= ch->party[i].Face;
				packet.u.kein.server_party.Str[i]	= ch->party[i].Str;
				packet.u.kein.server_party.Class[i]	= ch->party[i].Class;
				memcpy( packet.u.kein.server_party.name[i], ch->party[i].Name, sizeof( char ) *31 );
			}
		}
	}
	packet.h.header.size = sizeof(k_server_party );
	QueuePacket(c, cn, &packet, 1);
}



//011012 lsw >
void makemyjobexpsetting( int skillno , CHARLIST *ch )
{
	int kind = SkillTbl[skillno].MotherSkillType;
	if( ch->skillexp[kind].skillexp < 50000 ) ch->skillexp[kind].skillexp = 50000;
}
//011012 lsw <


/// 0212 YGI
int SetMySkill( CHARLIST *ch )		// 일단 기본적으로 알아야하는 스킬 셋
{
	int i;
	switch( ch->Class ) 
	{
	case WARRIOR :	for(  i=0; i<MAX_SKILLMAIN;i++ )	
						if( SkillTbl[i].Class_Warrior == 1 )
							ch->Skill[i] = 1;
						break;
	case THIEF :	for(  i=0; i<MAX_SKILLMAIN;i++ )
						if( SkillTbl[i].Class_Thief == 1 )
							ch->Skill[i] = 1;
						break;
	case ARCHER :	for(  i=0; i<MAX_SKILLMAIN;i++ )
						if( SkillTbl[i].Class_Archery == 1 )
							ch->Skill[i] = 1;
						break;
	case WIZARD :	for(  i=0; i<MAX_SKILLMAIN;i++ )
						if( SkillTbl[i].Class_Wizard == 1 )
							ch->Skill[i] = 1;
						break;
	case PRIEST :	for(  i=0; i<MAX_SKILLMAIN;i++ )
						if( SkillTbl[i].Class_Priest == 1 )
							ch->Skill[i] = 1;
						break;
	}
	
	switch( ch->Job )
	{
	case J_FAMER		:	for(  i=0; i<MAX_SKILLMAIN;i++ ) if( SkillTbl[i].Select_Farmer		== 1 )		{ ch->Skill[i] = 1; makemyjobexpsetting( i, ch ); }break;
	case J_MINNER		:	for(  i=0; i<MAX_SKILLMAIN;i++ ) if( SkillTbl[i].Select_miner		== 1 )		{ ch->Skill[i] = 1; makemyjobexpsetting( i, ch ); }break;
	case J_FISHER		:	for(  i=0; i<MAX_SKILLMAIN;i++ ) if( SkillTbl[i].Select_fisher		== 1 )		{ ch->Skill[i] = 1; makemyjobexpsetting( i, ch ); }break;
	case J_LUMBERJACK	:	for(  i=0; i<MAX_SKILLMAIN;i++ ) if( SkillTbl[i].Select_Lumberjack	== 1 )		{ ch->Skill[i] = 1; makemyjobexpsetting( i, ch ); }break;
	case J_BUTCHER		:	for(  i=0; i<MAX_SKILLMAIN;i++ ) if( SkillTbl[i].Select_Butcher		== 1 )		{ ch->Skill[i] = 1; makemyjobexpsetting( i, ch ); }break;
	case J_HERB_DIGGER	:	for(  i=0; i<MAX_SKILLMAIN;i++ ) if( SkillTbl[i].Select_Herbdigger	== 1 )		{ ch->Skill[i] = 1; makemyjobexpsetting( i, ch ); }break;
	case J_HERDSMAN		:	for(  i=0; i<MAX_SKILLMAIN;i++ ) if( SkillTbl[i].Select_herdman		== 1 )		{ ch->Skill[i] = 1; makemyjobexpsetting( i, ch ); }break;
	case J_CARPENTER	:	for(  i=0; i<MAX_SKILLMAIN;i++ ) if( SkillTbl[i].Select_Carpenter	== 1 )		{ ch->Skill[i] = 1; makemyjobexpsetting( i, ch ); }break;
	case J_BLACKSMITH	:	for(  i=0; i<MAX_SKILLMAIN;i++ ) if( SkillTbl[i].Select_Blacksmith	== 1 )		{ ch->Skill[i] = 1; makemyjobexpsetting( i, ch ); }break;
	case J_COOKER		:	for(  i=0; i<MAX_SKILLMAIN;i++ ) if( SkillTbl[i].Select_Cooking		== 1 )		{ ch->Skill[i] = 1; makemyjobexpsetting( i, ch ); }break;
	case J_TAILOR		:	for(  i=0; i<MAX_SKILLMAIN;i++ ) if( SkillTbl[i].Select_Tailoring	== 1 )		{ ch->Skill[i] = 1; makemyjobexpsetting( i, ch ); }break;
	case J_BOWCRAFT		:	for(  i=0; i<MAX_SKILLMAIN;i++ ) if( SkillTbl[i].Select_Bowcraft_Fletcher == 1 ){ ch->Skill[i] = 1; makemyjobexpsetting( i, ch ); }break;
	case J_ALCHEMIST	:	for(  i=0; i<MAX_SKILLMAIN;i++ ) if( SkillTbl[i].Select_Alchemy		== 1 )		{ ch->Skill[i] = 1; makemyjobexpsetting( i, ch ); }break;
	case J_CANDLEMAKER	:	for(  i=0; i<MAX_SKILLMAIN;i++ ) if( SkillTbl[i].Select_Candlemaker	== 1 )		{ ch->Skill[i] = 1; makemyjobexpsetting( i, ch ); }break;
	case J_MERCHANT		:	for(  i=0; i<MAX_SKILLMAIN;i++ ) if( SkillTbl[i].Select_Merchant	== 1 )		{ ch->Skill[i] = 1; makemyjobexpsetting( i, ch ); }break;
	}
	return 1;
}

// 0224 YGI
void SendCreateAbility( short int cn )
{
	CHARLIST *ch = &connections[cn].chrlst;
	if( !ch ) return;
	
	t_packet p;
	p.h.header.type = CMD_CREATE_ABILITY;
	{
	/*
	for( int i=STR; i<= LUCK; i++ )
	{
	plus[i] = rand() % 4;
	}
	p.u.kein.start_create_ability.str  = plus[STR];
	p.u.kein.start_create_ability.dex  = plus[DEX];
	p.u.kein.start_create_ability.con  = plus[CON];
	p.u.kein.start_create_ability.wis  = plus[WIS];
	p.u.kein.start_create_ability.inte = plus[INT_];
	p.u.kein.start_create_ability.cha  = plus[CHA];
	p.u.kein.start_create_ability.movp = plus[MOVP];
	p.u.kein.start_create_ability.endu = plus[ENDU];
	p.u.kein.start_create_ability.mor  = plus[MOR];
	p.u.kein.start_create_ability.wsps = plus[WSPS];
	p.u.kein.start_create_ability.luck = plus[LUCK];
		*/
	}
	p.h.header.size = 0  ;
	QueuePacket(connections, cn, &p, 1);
}




// 0410
int GetDiceAbility( int DiceNumber )		// 다이스 돌리기
{
	int dice_count, dice_max;
	dice_count = DiceNumber / 1000;
	dice_max = DiceNumber % 1000;
	
	int ret = 0;
	for( int i=0; i<dice_count ; i++ )
	{
		ret += rand()%dice_max + 1;
	}
	return ret;
}

// 0410 YGI
void SendThrowDice( char type, short int cn )
{
	if( type < 0 || type > 5 ) return;
	
	CHARLIST *ch = &connections[cn].chrlst;
	if( !ch ) return;
	
	ch->Class = type;
	int nAbility[LUCK+1] = {0,};
	
	int num_to_rate[2] = { 1003, 1002 };
	int	class_ability[5][5] = { 
								{ STR,	CON }, 
								{ DEX,	MOVP},
								{ DEX,	CON },
								{ INT_,	WSPS},
								{ WIS,	WSPS} 
	};
	
	for( int i=0; i<2; i++ )
	{
		int ability = GetDiceAbility( num_to_rate[i] );		
		nAbility[ class_ability[ch->Class][i] ] = ability;
	}
	
	t_packet p;
	memset(&p, 0, sizeof(t_packet));
	p.h.header.type = CMD_THROW_DICE;
	{
		p.u.kein.start_throw_dice.ability1 = nAbility[ class_ability[ch->Class][0] ];
		p.u.kein.start_throw_dice.ability2 = nAbility[ class_ability[ch->Class][1] ];
	}
	p.h.header.size = sizeof( k_start_throw_dice );
	QueuePacket(connections, cn, &p, 1);
	
	
	int basic_ability[5][11] = 
	{
		{ 50, 10, 30, 10, 10, 10, 10, 10, 10, 10, 10 },
		{ 10, 40, 20, 10, 10, 10, 20, 10, 10, 10, 20 },
		{ 10, 50, 20, 10, 10, 10, 20, 10, 10, 10, 10 },
		{ 10, 10, 15, 10, 40, 10, 10, 20, 10, 25, 10 },
		{ 10, 10, 20, 35, 10, 10, 10, 20, 10, 25, 10 },
	};
	
	ch->Str		= basic_ability[type][STR];			// 0229 YGI
	ch->Con		= basic_ability[type][CON];
	ch->Dex		= basic_ability[type][DEX];
	ch->Wis		= basic_ability[type][WIS];
	ch->Int		= basic_ability[type][INT_];
	ch->MoveP	= basic_ability[type][MOVP];
	ch->Char	= basic_ability[type][CHA];
	ch->Endu	= basic_ability[type][ENDU];
	ch->Moral	= basic_ability[type][MOR];
	ch->Luck	= basic_ability[type][LUCK];
	ch->wsps	= basic_ability[type][WSPS];
	
	ch->Str		+= nAbility[STR];			// 0229 YGI
	ch->Con		+= nAbility[CON];
	ch->Dex		+= nAbility[DEX];
	ch->Wis		+= nAbility[WIS];
	ch->Int		+= nAbility[INT_];
	ch->MoveP	+= nAbility[MOVP];
	ch->Char	+= nAbility[CHA];
	ch->Endu	+= nAbility[ENDU];
	ch->Moral	+= nAbility[MOR];
	ch->Luck	+= nAbility[LUCK];
	ch->wsps	+= nAbility[WSPS];
}

///////////////////////////////////////////////////////////////////////////////
//		
//		시간에 의한 캐릭터의 배고픔 수치 줄이기	//###1213_2 수정
//		
///////////////////////////////////////////////////////////////////////////////
void HungryMuchAdd( CHARLIST *ch, const int iType)//020214 lsw
{		
	double iValue	= 0	;		
	if( ch->bAlive != ALIVE_ ) //죽었기 때문에
	{
		ch->Hungry = 0;	
		return;
	}	
	
	switch(iType)
	{
	case HT_NORMAL:      iValue = HT_NORMAL_CHANGE_VALUE;      break;
	case HT_WALK:        iValue = HT_WALK_CHANGE_VALUE;        break;			
	case HT_RUN:         iValue = HT_RUN_CHANGE_VALUE;         break;
	case HT_BATTLE_WALK: iValue = HT_BATTLE_WALK_CHANGE_VALUE; break;
	case HT_BATTLE_RUN:	 iValue = HT_BATTLE_RUN_CHANGE_VALUE;	 break;
	case HT_ATTACK:      iValue = HT_ATTACK_CHANGE_VALUE;  		 break;
	case HT_MAGIC:       iValue = HT_MAGIC_CHANGE_VALUE; 		   break;
	case HT_SKILL:       iValue = HT_SKILL_CHANGE_VALUE; 		   break;
	case HT_COMBAT:      iValue = 5; 		                       break;
	default:                 
		{	
			return;
		}break;
	}
	ch->fChangeHungry += (float)iValue;
}