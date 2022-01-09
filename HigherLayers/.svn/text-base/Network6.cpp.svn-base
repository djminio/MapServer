#include "..\stdafx.h"
#include "DefaultHeader.h"
#include "Sealstone.h"
#include "..\\lowerlayers\\servertable.h"
#include "..\\lowerlayers\\servertable2.h"
#include "scrp_exe.h"
extern CServerTable* g_pServerTable;
void SendProxyWantChangeDBDEMON()
{
//	g_pServerTable->SendToProxyServer( (char*)pPacket, dwLength );
	g_pServerTable->RequestToSetDBDemon();
}

void MoveToHomeTown(CHARLIST *Caster)
{
	switch(Caster->name_status.nation)
	{
	case NW_BY :	{MapMove(Caster->GetServerID(),"MA-IN",245,115);	}break;
	case NW_ZY :	{MapMove(Caster->GetServerID(),"RENES_C",210,175);	}break;
	case NW_YL :	{MapMove(Caster->GetServerID(),"BARANTAN",325,98);	}break;
	default:		break;					
	}
}

void MoveToHomeTownWithAilve(CHARLIST *Caster)
{
	SkillMgr.CharacterToAlive(Caster);
	MoveToHomeTown(Caster);
}

extern DWORD GetAbilityRedistributionMoney(t_connection c[], int cn);		// 020115 LTS

void SendCMD_COMFORM_RESET_ABILITY(CHARLIST *ch)//020820 lsw//020901 lsw
{
	if(!ch) {return;}

	for(int i = 0; i < 8;i++)
	{
		if(ch->equip[i].item_no)//장착 된게 있다면//020701 lsw
		{
			return;
		}
	}

	const int cn = ch->GetServerID();
		 
	const DWORD tempAbilityRedistributionMoney=GetAbilityRedistributionMoney(connections,cn);
	if (ch->GetBankMoney()>=tempAbilityRedistributionMoney)	// 크거나 같다면  //돈검사
	{
		//캐릭터 이름을 날린다
		t_packet p;
		p.h.header.type=CMD_COMFORM_RESET_ABILILTY;				
		p.h.header.size=sizeof(t_ComformResetAbililty);
		memcpy(p.u.ComformResetAbililty.szName,ch->Name,20);
		p.u.ComformResetAbililty.szName[20]= 0;
		p.u.ComformResetAbililty.cn = ch->GetServerID();
		QueuePacket(connections,DB_DEMON,&p,1);
	}
	else//돈없잖아
	{
		t_packet packet;
		packet.h.header.type=CMD_CHECK_ABILITY_CHANGE_MONEY;				
		packet.h.header.size=sizeof(t_CommonDataC);
		packet.u.NationWar.CommonDataC.Data=0;	// 실패
		QueuePacket(connections,cn,&packet,1);
	}	
}

void RecvCMD_COMFORM_RESET_ABILITY(t_packet &packet)//020820 lsw//020901 lsw
{
	CHARLIST *ch = CheckServerId(packet.u.ComformResetAbililty.cn);

	if(!ch) {return;}

	if(!packet.u.ComformResetAbililty.iIsAble)//020827 lsw
	{
		OutMessage(ch,2,710);
		return;
	}

	if(strcmp(ch->Name, packet.u.ComformResetAbililty.szName))
	{
		{return;}
	}
	DWORD tempAbilityRedistributionMoney;
	packet.h.header.type=CMD_CHECK_ABILITY_CHANGE_MONEY;				
	packet.h.header.size=sizeof(t_CommonDataC);
	for(int i = 0; i < 8;i++)
	{
		if(ch->equip[i].item_no)//장착 된게 있다면//020701 lsw
		{
			return;
		}
	}

	const int cn = ch->GetServerID();
	tempAbilityRedistributionMoney=GetAbilityRedistributionMoney(connections,cn);
	
	if (ch->DecBankMoney(tempAbilityRedistributionMoney, BMCT_RESET_ABILITY)) // 돈을 없엔다
	{	//< CSD-030723
		packet.u.NationWar.CommonDataC.Data=1;	// 성공
		RecvReqResetAbility(cn);									// 어빌재분배 루틴을 호출한다.
	}	//> CSD-030723
	else
	{
		packet.u.NationWar.CommonDataC.Data=0;	// 실패
	}
	QueuePacket(connections,cn,&packet,1);
}



void RecvAskSkillPoint(t_packet *packet, short int cn )
{
	CHARLIST *ch = CheckServerId( cn ); 
	if (ch == NULL) return; // finito null check
	t_packet p;
	
	p.h.header.type = CMD_NOW_SKILL_POINT;
	p.h.header.size = sizeof(t_NowSkillPoint);

	p.u.NowSkillPoint.item.item_no	=	SADONIX_NO;
	p.u.NowSkillPoint.iNowPoint		=	ch->nSkillSadCount;
			
	::QueuePacket(connections,ch->GetServerID(),&p,1);

	p.h.header.type = CMD_NOW_SKILL_POINT;
	p.h.header.size = sizeof(t_NowSkillPoint);

	p.u.NowSkillPoint.item.item_no	=	BLESSLEAF_NO;
	p.u.NowSkillPoint.iNowPoint		=	ch->nSkillBlessLeaf;
			
	::QueuePacket(connections,ch->GetServerID(),&p,1);
}