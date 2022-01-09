// CharInfo.h: interface for the CCharInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARINFO_H__1A212137_3131_4DD3_8343_EE42343C165E__INCLUDED_)
#define AFX_CHARINFO_H__1A212137_3131_4DD3_8343_EE42343C165E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ExchangeMgr.h"
#include "ItemMgr.h"

extern int MAX_EXP_LEVEL;  // CSD-040204
extern int MAX_TAC_LEVEL;  // CSD-040204

///////////////////////////////////////////////////////////////////////////////
// 캐릭터에 관한 데이타
struct CHAR_DATA
{	
	BYTE			bAlive;					// 캐릭터의 생사여부(	REMOVE_:0 / ALIVE_:1 / DEAD_:2 / BUFE_:3)	// Check_
	int				deadcount;					// 캐릭터가 죽은후 실제 없어지기까지의 count
	BYTE			installType;
	DWORD			total_id;
	short			eventno;
	short			generationpos;		// NPC일경우 발생 위치 Table의 Index값을 기억하고 있는다.
	short			generationtype;		// NPC일겅우 발생방식( GT_SKB_FILE, GT_SCRIPT );
	char			Name[20];
	int				namenumber;			// NPC_Name_Ref[ namenumber].Female를 의미한다. 
	DWORD			Exp;
	short int		Gender;						// 0:여자 1 : 남자
	short int		Face;						
	short int		Class;						// 0:전사 1:궁수 2:도적 3:성직자 4:마법사
	short int		Spell;					// 마법사:0 / 성직자:1
	short int		fame;
	short int		timeshot; // add by rogerio
	short int       fame_pk;			// 010915 LTS //Fame_PK -> NWCharacter로 교체 DB에는 실제로 NWCharacter의 값이 들어갑니다.	// LTS AI2	//스페셜 어택 번호기억	
	NW_Character	NWCharacter;		// 010915 LTS

	short int       social_status;
	unsigned char	Tactics ;						// 선택한 전투기술 (보여주기 위함)
	unsigned char	Tactics_para;					// 실제 skill에 적용할 파라메터값을 가지고 있음...
	short int		Race;						// 종족
	short int		Str  ;				//  힘
	short int		Con  ;				//  건강
	short int		Dex  ;				//	민첩성
	short int		Wis  ;				//	지혜
	short int		Int  ;				//	지능
	short int		MoveP;				//	이동력
	short int		Char ;				//	매력
	short int		Endu ;				//  저향력
	short int		Moral;				//	사기
	short int		Luck ;				//	행운
	short int		wsps ;				//  마법력->마법 메모라이즈
	//무기, Armor, 악세사리 ,ETC 에 의한 	
	short int		Strw;				//  힘
	short int		Conw;				//  건강
	short int		Dexw;				//	민첩성
	short int		Wisw;				//	지혜
	short int		Intw;				//	지능
	short int		MovePw;				//	이동력
	short int		Charw;				//	매력
	short int		Enduw;				//  저향력
	short int		Moralw;				//	사기
	short int		Luckw;				//	행운
	short int		wspsw;

	BYTE			item_Hit_Rate;
	BYTE			item_Deffence_Rate;

	CCharRank		name_status;		// 0927 YGI
	DWORD			scan_time;			// LTS AI2 NPC SUMMON DELAY TIME
	short int		Ac;//AC( 방어력 )
	short int		Job;
	
	DWORD			Money;

	//Hp Mp 등등의 값
	int		Hp;
	int		HpMax;
	int		Mana;
	int		ManaMax;
	int		Hungry;	
	int		HungryMax;
	int		Cp;
	int		CpMax;

	float	fChangeHungry;//배고픔 수치 변화량//020214 lsw
	
	char			MapName[20];
	unsigned char	Peacests;
	int				Sight; //020425 YGI
	short int		BodyR, BodyG, BodyB;		// 몸 색깔 R.G.B
	short int		ClothR, ClothG, ClothB;		// 띠 색깔 R.G.B
	short int		Age;
	short int		nk[MAX_NATION];							// 영양가1
	
	DWORD			killmon;					// 1계열몬스터 킬링스
	DWORD			killanimal;					// 동물 킬링수
	DWORD			killpc;						// 사람,NPC 킬링수
	char			KillerName[40];
	
	BYTE Ws[200];			//	마법
	BYTE Ps[200];			//	신법
	BYTE Skill[MAX_SKILL_NO];

	t_skillexp		skillexp[MAX_SKILL_EXP];		// 011012 lsw //스킬경험치 변경

	// 내가 가지고 있는 아이템..
	ItemAttr	inv[3][3][8] ;// 인벤토리
	ItemAttr	equip[8] ;// 장착
	ItemAttr	quick[7] ;// 퀵 
	ItemAttr	handheld ;// 손에 들고 있는
	ItemAttr	bank[5][3][6];// 은행에 보관품

	short int	Item[MAX_LEARN_ITEM];		// 만들수 있는 아이템		1220
	short int	ItemMax;					// 현재 만들수 있는 아이템 최대치 1220
	int			itemweight;					// 소환한 주인의 ID			// LTS AI2

	//  Client을 위한 Data
	short int		SprNo;
	short int		mutant;  // 0810 KHS
	short int		npc_index; // NPC index concerned by Parameter 'index'
	short int		SprType;	
	short int		ElementType; // 원천적인 속성을 의미한다. (예) 미노: 불(fire)		
	BYTE			accessory[4];
	
	short int		mantle;		// 011018 KHS 망또
	short int		jjing;      // 011030 KHS JJING

	DIRECTION		Direction;
	DWORD			IDWhoImAttacked;	// 나를 친놈이 누구냐..
	short int		nCurrentAction;						
	short int       ox, oy;		// 자신이 태어난장소..
	short int		X, Y;		// 현재 좌표( Dot 단위 )
	short int		MovePathCount;
	short int		MoveLength;
	unsigned char	MoveType;
	short int		OldMoveSx,MoveSx;	// 현재좌표(타일단뒤 )
	short int		OldMoveSy,MoveSy;
	unsigned char   MoveDirTbl[MAX_DIR_TABLE];
	short int		MoveGox;			// 목적지의 좌표( Dot 단위 )
	short int		MoveGoy;
	short int		Tox, Toy;			// 타격을 받으면 밀린다.( Dot단위 )
	short int		detect_x, detect_y;
	int				path[ MAX_SHORTPATH][2];		// PathBuild()를 위한 변수..
	int				pathcount;
	// --------------  NPC Pattern---------- NPC공격을 위해..-
	short 			patterntype;
	short 			oldpatterntype;
	int				targetid;
	int				bossid;
	short			attackcount;		// NPC가 사용한다. 지금은사용하지 않는다. 
	DWORD 			aitimedelay;
	DWORD			aidelayhowmuch;
	int				tx, ty, how;// 이동에 관련된사항.
	int				attacked;
	bool			notcomeinside; // 집안에 들어갈수 없다. 
	//  ------------------------------------- NPC가 죽으면 얻을수 있는 것들...
	short int		Get_meat;
	char			Meat_amount;
	short int		Get_skin;
	char			Skin_amount;
	short int		Get_Wool;
	char			Wool_amount;
	short int		Get_beewax;
	char			BeeWax_amount;
	char			viewtype;
	short int		openhouse;
	short int		tame;				// tame수치... 
	short int		tamingable;
	short int		tamingcountdown;
	BYTE			tame_cmd;
	char			HostName[ 20]; // 주인의 이름...
	short int		HostId;
	char			TargetName[ 31]; // 주인이 정한 공격자의 이름.
	short			pkid;		//
	short			pkcount;	// 이 수치가 3이상이면 PK를 한것이다 ! 
	DWORD			cast_pk;	// BroadCast된 시간.
	//------------------------   서버에서만 사용하는 para...
	DWORD			WalkTime;
	DWORD			desttime;
	short int		cur_skillno; // 현재 실행되기 위해 시전중인 Skill...
	short int		cur_skillx;
	short int		cur_skilly;
	short int		cur_skilltargettype;
	short int		cur_skilltarget_id;
	short int		cur_skillKey;//030102 lsw Skill Key
	POS				cur_skillPOS;
	char			cur_skillTargetName[31];//020531 lsw
	DWORD			exetime_skill; // 실행될 Skill의 시간.

	short int		MaxWizardSpell ;
	short int		MaxPriestSpell ;

	short int		Weight, MaxWeight ;

	DWORD			tac_skillEXP[13] ;
	char			cur_tac_skill;	// 현재 어떤공격이 이류어졌을때 Setting되며 전투기술 Index가 들어간다. ( TACTICS_Crapple....과 같은 )

	DWORD			LastTotal;
	DWORD			LastLoan;
	DWORD			LastLoan_time;
//	DWORD			box_id;							// 현재 열려 있는 박스의 고유 ID
	int				Defence;						// 방어력
	
	// Chatting관련 변수 ----------------
	char			chatname[ 31];
	char			chattype;
	short int		whisperid;
	
	// Party / Relation / Employment.
	CharacterParty party[MAX_PARTY_MEMBER];			
	CharacterParty relation[3];				
	CharacterParty employment[6];				
	
	/// up_date용
	char		party_str[6][31];
	char		relation_str[3][31];
	char		employment_str[6][31];
	int			partyrequestflag;
	BYTE		Status;				// 캐릭터의 휴식 상태 0은 nomal		//0108
	DWORD		timetoupdate;
	char		updatable; // CSD-TW-030620
	int			balancingtest;
	DWORD		attackedblock;  // 맵에 들어오면  0810 KHS							
	AREA		area_tbl[MAX_AREA_BUFFER];							
	int			ctrlpc;			// NPC일경우 나를 제어하는 NPC의 ID, 없을 경우 -1를 가지고 있다.
	int			ctrlnpc[MAX_PC_CONTROL_NPC];		
	DWORD		SpeedUp;			//  현재 속도의 1/2만큼 더 빨라짐
	char		believe_god;		// 내가 믿는 신
	short int	faith;				// 신앙심
	char		preach_name[20];	// 전도한 사람의 이름
	short int	god_cast_level;		// 신전 주문 학습 단계	// 0410	YGI	// sql 저장 필요
	bool		in_temple;			// 신전안에 있다		// 0410	YGI	// sql 저장 필요 없음
	int			is_god_meeting;		// 현재 예배중이다		// 0410	YGI	// sql 저장 필요 없음
	DWORD		god_meeting_start_time;	// 예배 시작한 시간	// 0410	YGI	// sql 저장 필요 없음
	bool		i_go_year_meeting;		// 연중 예배에 갔다.// 0410	YGI	// sql 저장 필요 없음
	bool		i_go_month_meeting;		// 월중 예배에 갔다.// 0410	YGI	// sql 저장 필요 없음
	bool		i_go_day_meeting;		// 일일 예배에 갔다.// 0410	YGI	// sql 저장 필요 없음
	bool		use_supper;				// 음식 제공을 받는 중이ㅏ.		// 0601
	int			much_supper;			// 제공 받는 음식의 양			// 0601
	DWORD		can_memorize_time;		// 가능한 시간	// 저장할 필요 없음  0414 YGI
	short int	startposition;
	short int	startmap;
	//////////////////////////////////  결투 시스템...
	char        dualname[20];
	char        dualstatus;	// 0: No Dual    1: 신청한상태..   2: 대결상태.
	DWORD		dualvictory;	
	DWORD		dualfail;	
	DWORD		dualdraw;	
	/////////////////// DB Update없이 Packet으로 타 게임서버에 전송.
	char		change_map_confirm[9];						// YGI 0521
	int			now_changing_map;	// 현재 맵 전환 중이다	// YGI 0521
	POINT		memory_Position;			//타운포탈했을때 이동할 맵 좌표(타일 좌표)
	int			homemap;
	int         homex, homey;
	BOOL		fight_flag;	//1:1 대결 상태체크.
	int			fight_id;	//전투할 놈..
	DWORD		first_ResultMagicTime;		//일반 대상마법인 경우 1초에 1번 이내로 제한하기 위한 시간기억
	DWORD		magic_delay;
	int			curr_fight_map;		// 0907 YGI
	int			dec_duration;		// 장비의 Duration이 닮는정도를 Check한다. 
	short int	vote_num;			// 마지막 까지 선거한 날짜
	int         sealstone;	// 결계석이 맞으면 이수치가 올라가며. 적당히 올라갈때마다. 클라이언트로 보내준다. 
	int			view_job;		// 보여주기 직업		//001028 YGI
  
	// Added by chan78 at 2000/11/27
	short int		MapChange_MoveGox;
	short int		MapChange_MoveGoy;
	short int		MapChange_X;
	short int		MapChange_Y;
	short int		MapChange_MovePathCount;
	short int		MapChange_MoveLength;
	short int		MapChange_MoveSx;
	short int		MapChange_MoveSy;

	DWORD			Last_MapChangeTime;
	
	// 010406 YGI
	DWORD			LadderScore;		// 콜로세스 래더 점수
	WINLOSEPOINT	WinLoseScore;		// 콜로세스 승패
	bool			refresh_ladder;		// 래더 랭킹 디비에 저장할 필요가 있는가? // 010511 YGI

	DWORD	dwSkillTime;//010703 lsw
	DWORD	dwSkillMakeitemTime;//010703 lsw
 
	int iPhysicalTotalDamage;
	int iPhysicalRandomDamage;
	
	//유령이나 트랩을 보는 시간
	bool	bCanViewGhost;
	DWORD	dwCanViewGhostTime;
	
	bool	bCanViewTrap;
	DWORD	dwCanViewTrapTime;
	
	bool	bCanViewBomb;
	DWORD	dwCanViewBombTime;

	short	nSkillSadCount;
	short	nSkillBlessLeaf;
	
	//회복 시간
	DWORD	dwHungryRecoverTime;
	DWORD	dwHpRecoverTime;
	DWORD	dwManaRecoverTime;
	DWORD	dwDivineRecoverTime;
	DWORD	dwCpRecoverTime;
	
	DWORD	dwCheckAccelTime;
	DWORD	dwSendedAccelTime;

	//밝기 조절
	short int nDayLightControl;

	DWORD		dwDayLightControlCustomTime;
	short int	nDayLightControlCustom;//020610 lsw
		
	RareMain					SquadRare;
	DYNAMICRAREEFFECT			DynamicRare;
	STATICRAREEFFECT			StaticRare;		//착용 아이템
  
	// 011108 YGI
	BYTE	select_rune;			// 겜블시 선택된 룬 번호
 	int 	JoinLocalWar;			// 국지전에 참여 했음 하지 않았음	// LTS LOCALWAR		// LTS AI2 MONSTER 그룹번호 // CSD-030528
	char	GainedFame;				// 국지전에서 얻은 명성			// LTS LOCALWAR			// LTS AI2 몬스터 보스번호
	char	CurrentPhase;			// 국지전에서 적국가의 맵에서 한단계씩 들어갈때
	char	ChairNum;				// 의자번호						// LTS SITDOWN BUG

	char	reporter_mode;
	char	party_select;	// 파티시 올라가는 경험치 결정
	char	ox_select;		// ox event 에서 선택한 값

	char			EventLocalWarJoin;			// 020115 LTS			// 다음이벤트에 쓰기위해 
	Event_Join		EventJoin;					// 020115 LTS
	int				EventPoint;					// 020115 LTS

	char			HorseNo;					// LTS HORSERIDER		// LTS NEW AI		// 리커버리 카운트 
	char			RiderNo;					// LTS HORSERIDER
	char			HorseGrade;					// LTS HORSERIDER
	char			RiderGrade;					// LTS HORSERIDER

	char			pk_on_off;		// YGI 020418
	char			QuestMode;					// LTS DRAGON MODIFY
	char			DragonLordWarTeam;			// LTS DRAGONLORD		// 0 : 없음 1: BLUE 2: RED

	//021030 lsw 교환 변경
	K_ITEM			give[MAX_EXCHANGE_ITEM];		// 내가 줘야 하는 아이템
	K_ITEM			recv[MAX_EXCHANGE_ITEM];		// 내가 받아야 하는 아이템
	short int		m_iExchangeTargetId;		// 거래 대상
	char			m_szExchangeTargetName[21];		// 거래 대상 이름
	int				m_iExchangeType;		// 거래하고 있는지 없는지...
	int				m_iExchangeState;		// 거래하고 있는지 없는지...
	bool			m_bExchangeMode;		// 거래하고 있는지 없는지...
	int				m_nUserAge;				// 030929 kyo // 태국에서만 사용하는 유저 나이
	DWORD			m_dwLastMoveTime;	// finito 21/08/07 last time that a CMD_MOVE packet sent (should be 300ms or more)
	bool			m_bIsDuelAllow;		// finito 25/08/07 are duel requests allowed
	int				hack_timestart; // Add by rogerio
	int				hack_timeend; // Add by rogerio
	int				hack_counthits; // Add by rogerio
	std::vector<int>	TamedMonster; //Eleval 21/06/09 - To keep a track of the monsters the player tamed
	int				s_quest_floor;	//Eleval 10/08/09 - To keep a track of the floor the player was in when he DCed

	//Char's option by Eleval
	bool hat_is_hidden;
	//End of it
};

///////////////////////////////////////////////////////////////////////////////
// 획득 포인터에 관한 클래스
class CGainPoint
{
public:
	CGainPoint() : m_nPoint(0)
	{
	}
	
public:
	void Clear() { m_nPoint = 0; }
	
	int GetPoint() const { return m_nPoint; }
	void SetPoint(int nPoint) { m_nPoint = nPoint; }
	void Increase(int nPoint = 1) { m_nPoint += nPoint; }
	void Decrease(int nPoint = 1) { m_nPoint -= nPoint; }
	
	bool IsEnough(int nPoint) const
	{ 
		return (m_nPoint >= nPoint) ? true:false;
	}
	
public:
	int m_nPoint;
};
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// 상태 포인터에 관련된 클래스
class CStatePoint
{
public:
	CStatePoint() :	m_nMaxPoint(0), m_nCurPoint(0)
	{
	
	}

public:
	int Increase(int nPoint, int nPercent);
	int Decrease(int nPoint, int nPercent);

public:
	void SetMaximumPoint(int nPoint)
	{
		m_nMaxPoint = nPoint;
	}

	int GetMaximumPoint() const
	{
		return m_nMaxPoint;
	}

	void SetCurrentPoint(int nPoint)
	{
		m_nCurPoint = nPoint;
	}

	int GetCurrentPoint() const
	{
		return m_nCurPoint;
	}

private:
	int	m_nMaxPoint;
	int	m_nCurPoint;
};
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// 캐릭터 정보에 관한 클래스
class CBaseArena;
class CArenaTeam;
class CCharInfo : public CHAR_DATA
{
public:
	CCharInfo();
	
public:
	void InitCombat();
	void IncAbility(BYTE nType);
	void DecAbility(BYTE nType);
	
	void ResetAbility(BYTE nCombat);
	void DivideAbility(BYTE nNext);
	void ConvertTacticsLevel(BYTE nKind);
	void Message(BYTE nType, BYTE nKind, WORD nNumber);
	
	bool IncExperience(int nExp); // CSD-030930
	bool DecExperience(bool bNK); // CSD-030930

	bool IncTacticExp(BYTE nKind, int Exp); // CSD-030930
	bool DecTacticExp(BYTE nKind, bool bNK); // CSD-030930

	int GetWeaponSkill();
	int GetAbility(BYTE nType) const;
	int GetNpcAttack() const;
	int GetNpcDefence() const;
	int GetNpcMagic() const;
	int GetNpcDefence(BYTE nType) const;
	int GetNpcStrikePiercing() const; // CSD-021024
	int GetNpcMagicPiercing() const;  // CSD-021024
	int GetNpcStunAvoid() const;      // CSD-021024
	int GetNpcSlowAvoid() const;      // CSD-021024
	int GetNpcPoisonAvoid() const;    // CSD-021024
	int GetNpcBlindAvoid() const;     // CSD-021024
	int GetNpcDrainAvoid() const;     // CSD-021024
	int GetNpcFreezeAvoid() const;    // CSD-021024
	int GetMovePower(BYTE nType) const;
	int GetDistance(int nX, int nY) const;
	int GetRiseFall(int nX, int nY) const;	
	int GetWeaponKind() const;
	int GetTacticsKind() const;
	int GetCombatValue(int nIndex) const;
	int GetPureLife() const;
	int GetExtendAc() const;
	int GetItemEffectNumber() const; // CSD-031007
	
	void GetStepInfo(char* pData, int nSize);
	void SetStepInfo(char* pData, int nCount);
	
	int CorrectCombatAttack(int nCombat, int nResult) const;
	int CorrectCombatDefense(int nCombat, int nResult) const;
	int CorrectItemEffect() const; // CSD-031007
	
	bool LevelUpAbility();
	bool LevelUpTactics(int nKind);
	bool LevelUpCombat(int nKind, int nPoint);

	bool ChangeUpAddExp(int nExp); // CSD-TW-030624
	bool ChangeUpAddExpBonus(int nExp); // CSD-TW-030624 
	bool ChangeUpTacExp(int nType, int nExp); // CSD-TW-030624

	void GenerateNpcName();
	void AutoAttribute();
	void SetAttribute(int nRace);
	
	bool IsShield() const;
	bool IsArrow() const;
	bool IsLimit(int nPoint = 0) const;
	bool IsEnableCombat(BYTE nCombat) const;
	
	void Rare_Squad();
	void Rare_Eat();
	void Rare_Inv();
	void Rare_Quick();
	void Rare_Equip();
	void CalcRareEffect(int iFlag);
	bool IsLimitedAge( const int nLimitedAge){ return ( m_nUserAge > nLimitedAge ? false:true); }; // 030929 kyo //태국에서 사용하는 유저나이 제한
	
public:
	inline int IncLife(int nVolume, int nMax);
	inline int DecLife(int nVolume, int nMin);
	inline int IncMana(int nVolume, int nMax);
	inline int DecMana(int nVolume, int nMin);
	inline int IncHungry(int nVolume, int nMax);
	inline int DecHungry(int nVolume, int nMin);
	inline int IncCombat(int nVolume, int nMax);
	inline int DecCombat(int nVolume, int nMin);
	
	short GetServerID() const { return m_idServer; } // CSD-HK-030830
	int GetLevel() const { return m_nLevel; } // CSD-030806
	int GetGuildCode() const { return m_nGuildCode; } // CSD-030806
	int GetReservedPoint() const { return m_gnAbility.GetPoint(); }
	int GetCombatPoint() const { return m_gnCombat.GetPoint(); }
	int GetDualClass() const { return m_nDualCls; }
	int GetClassStep() const { return m_nClsStep; }
	int GetExperienceStep() const { return m_nExpStep; }
	int GetTacticsStep(int nIndex) const { return m_aTacStep[nIndex]; }
	int GetNK(int nNation) const { return nk[nNation]; }
	int GetCombatLevel(int nIndex) const { return Skill[nIndex - 100]; }
	int GetWanted() const { return m_bWanted; }
	CBaseArena* GetJoinArenaGame() const { return m_pArenaGame; }  // CSD-030509
	CArenaTeam* GetJoinArenaTeam() const { return m_pArenaTeam; }  // CSD-030509
	
	void SetServerID(short idServer) { m_idServer = idServer; } // CSD-HK-030829
	void SetLevel(int nLevel) { m_nLevel = nLevel; } // CSD-030806
	void SetGuildCode(int nGuildCode) { m_nGuildCode = nGuildCode; } // CSD-030806
	void SetReservedPoint(int nPoint) { m_gnAbility.SetPoint(nPoint); }
	void SetCombatPoint(int nPoint) { m_gnCombat.SetPoint(nPoint); }
	void SetDualClass(int nDual) { m_nDualCls = __int8(nDual); }
	void SetClassStep(int nStep) { m_nClsStep = __int8(nStep); }
	void SetExperienceStep(int nStep) { m_nExpStep = __int8(nStep); }
	void SetTacticsStep(int nIndex, int nStep) { m_aTacStep[nIndex] = __int8(nStep); }
	void SetCombatLevel(int nIndex, int nLevel) { Skill[nIndex - 100] = nLevel; }
	void SetWanted(bool bWanted) { if (IsPlayer())  m_bWanted = bWanted; }
	void SetJoinArenaGame(CBaseArena* pGame) { m_pArenaGame = pGame; }  // CSD-030509
	void SetJoinArenaTeam(CArenaTeam* pTeam) { m_pArenaTeam = pTeam; }  // CSD-030509
	
	void IncReservedPoint(int nPoint = 1) { m_gnAbility.Increase(nPoint); }
	void DecReservedPoint(int nPoint = 1) { m_gnAbility.Decrease(nPoint); }
	void IncCombatPoint(int nPoint = 1) { m_gnCombat.Increase(nPoint); }
	void DecCombatPoint(int nPoint = 1) { m_gnCombat.Decrease(nPoint); }
	
	bool IsDead() const { return (Hp <= 0) ? true:false; }
	bool IsHungry() const { return (Hungry <= 0) ? true:false; }
	bool IsFullLife() const {return (Hp >= HpMax) ? true:false; }
	bool IsFullMana() const {return (Mana >= ManaMax) ? true:false; }
	bool IsFullHungry() const {return (Hungry >= HungryMax) ? true:false; }
	bool IsFullCombat() const {return (Cp >= CpMax) ? true:false; } // CSD-030506
	bool IsPlayer() const { return (SprType < SPRITETYPE_NPC) ? true:false; }
	bool IsNpc() const { return (SprType > SPRITETYPE_CHARACTER) ? true:false; }
	bool IsDual() const { return m_nClsStep > 0; }
	bool IsUltraNK(int nNation) const { return (nk[nNation] >= ULTRA_NK) ? true:false; }
	bool IsJoinArenaGame() const { return (m_pArenaGame == NULL) ? false:true; } // CSD-030509
	bool IsJoinArenaTeam() const { return (m_pArenaTeam == NULL) ? false:true; } // CSD-030509
	bool IsLevelUp()            const { return (GetLevel() < MAX_EXP_LEVEL) ? true:false; }
	bool IsLevelUp(int nLevel)  const { return (nLevel < MAX_TAC_LEVEL) ? true:false; }
	
	bool IsIgnoreNormalLevel()     const { return m_bsAttr[IGNORE_NORMAL_LEVEL]; }
	bool IsIgnorePhysicalAttack()  const { return m_bsAttr[IGNORE_STRIKE_ATTACK]; }
	bool IsIgnorePhysicalDefence() const { return m_bsAttr[IGNORE_STRIKE_DEFENSE]; }
	bool IsIgnoreMagicAmplify()    const { return m_bsAttr[IGNORE_MAGIC_AMPLIFY]; }
	bool IsIgnoreMagicResist()     const { return m_bsAttr[IGNORE_MAGIC_RESIST]; }
    
	void SendCharInfoBasic(BYTE nKind, DWORD dwData) const; // CSD-TW-030620
	void SendCharInfoTactic(BYTE nKind) const; // CSD-TW-030620
	//021030 lsw 교환 변경
	int  GetExchangeTargetId()const{return m_iExchangeTargetId;}//거래 대상을 받는다
	const char*  GetExchangeTargetName()const{return &m_szExchangeTargetName[0];}//거래 대상을 받는다
	void SetExchangeTarget(const int iTradeTarget,const char* szTargetName)
	{
		m_iExchangeTargetId = iTradeTarget;
		strcpy_s(m_szExchangeTargetName, sizeof(m_szExchangeTargetName),szTargetName);
	}//거래 대상을 설정 한다
	
	int  GetExchangeType()const{return m_iExchangeType;}//거래 타입을 받는다
	void SetExchangeType(const int iTradeType){m_iExchangeType = iTradeType;}//거래 타입을 설정한다
	
	int  GetExchangeState()const{return m_iExchangeState;}//거래 상태를 받는다
	void SetExchangeState(const int iStateType){m_iExchangeState = iStateType;}//거래 상태를 설정한다
	
	void SetTradeMode(const bool bIsTradeOn){m_bExchangeMode = bIsTradeOn;}//거래 상태를 설정한다
	bool IsTradeOn()const{return !m_bExchangeMode;}//Trade On Off 값을 리턴한다. m_bExchangeMode --> false 일경우 Trade On 이다
	
	void ClearExchangeInfo()//거래 정보를 클리어 시킴
	{
		::memset( give, 0, sizeof( K_ITEM ) * MAX_EXCHANGE_ITEM );
		::memset( recv, 0, sizeof( K_ITEM ) * MAX_EXCHANGE_ITEM );
		m_iExchangeTargetId =0; 
		m_iExchangeType = EXCHANGE_TYPE_NO; 
		m_iExchangeState = EXCHANGE_STATE_READY;
		::strcpy_s(m_szExchangeTargetName, sizeof(m_szExchangeTargetName),"");
	}

	bool IsCounselor() const { return (name_status.counselor) ? true:false; }
	bool IsReporter() const { return (name_status.reporter) ? true:false; }
	bool IsPayedUser() const { return (BT_FREE != GetBillingType()) ? true:false; } //이 유저는 돈을 낸 유저인가?

	short GetBillingType()const{return m_nBillingType;}
	void SetBillingType(const short wValue){m_nBillingType = wValue;}

	DWORD GetBankMoney() const 
	{	//< CSD-030723
		return m_dwBankMoney;
	}	//> CSD-030723
	
	void SetBankMoney(const DWORD dwMoney, const eBankMoneyChangeType BMCT)
	{	//< CSD-030723
		::SaveChangeBankMoney(GetServerID(), GetBankMoney(), dwMoney, BMCT);
		
		m_dwBankMoney = dwMoney;
	}	//> CSD-030723
			
	bool IncBankMoney(const DWORD dwMoney, const eBankMoneyChangeType BMCT, const bool bSend = true)
	{	//< CSD-030723
		if (DWORD(MAX_BANK_MONEY - m_dwBankMoney) >= dwMoney) //ULONG_MAX
		{
			SetBankMoney(m_dwBankMoney + dwMoney, BMCT);
			
			if (bSend)
			{	//은행돈 갱신
				SendCharInfoBasic(BANKMONEY, GetBankMoney());
			}

			return true;
		}

		return false;
	}	//> CSD-030723

	bool DecBankMoney(const DWORD dwMoney, const eBankMoneyChangeType BMCT, const bool bSend = true)
	{	//< CSD-030723
		if (m_dwBankMoney >= dwMoney) //꺼낼 돈보다 있는 돈이 많으면
		{	//은행돈 갱신
			SetBankMoney(m_dwBankMoney - dwMoney, BMCT);
			
			if(bSend)
			{
				SendCharInfoBasic(BANKMONEY, GetBankMoney());
			}
			
			return true;
		}

		return false;
	}	//> CSD-030723

	bool IsEventRecv() { return m_bEventRecv; } ;			// BBD 040308
	void SetEventRecv(bool val) { m_bEventRecv = val; } ;	// BBD 040308
	
protected:
	int GetExtStr() const;
	int GetExtCon() const;
	int GetExtDex() const;
	int GetExtWis() const;
	int GetExtInt() const;
	int GetExtMove() const;
	int GetExtChar() const;
	int GetExtEndu() const;
	int GetExtMor() const;
	int GetExtLuck() const;
	int GetExtWs() const;
	int GetExtPs() const;
	
	int GetRemainStr() const;
	int GetRemainCon() const;
	int GetRemainDex() const;
	int GetRemainWis() const;
	int GetRemainInt() const;
	int GetRemainMove() const;
	int GetRemainChar() const;
	int GetRemainEndu() const;
	int GetRemainMor() const;
	int GetRemainLuck() const;
	int GetRemainWs() const;
	int GetRemainPs() const;
	
	int CalcNewLife(BYTE nCombat) const;
	int CalcNewMana(BYTE nCombat) const;
	int CalcNewHungry(BYTE nCombat) const;
	int CalcNewCombat() const;
	int CalcNewAc() const;
	
	int CalcWeaponHit() const;
	int CalcCriticalHit(BYTE nActive, BYTE nPassive) const;
	int CalcTacticExp(BYTE nKind, DWORD dwExp) const;
	
	void AddPhysicalTacExp1(int nKind, int nExp); // CSD-TW-030624
	void AddPhysicalTacExp2(int nKind, int nExp); // CSD-TW-030624
	void AddPhysicalTacExp3(int nKind, int nExp); // CSD-TW-030624
	void AddMagicTacExp1(int nKind, int nExp); // CSD-TW-030624
	void AddMagicTacExp2(int nKind, int nExp); // CSD-TW-030624
	
private:
	bool IsLimitExp(int nLevel);
	bool IsLimitTac(int nKind);
	
protected:
	enum
	{
		IGNORE_NORMAL_LEVEL = 0,
		IGNORE_STRIKE_ATTACK = 1,
		IGNORE_STRIKE_DEFENSE = 2,
		IGNORE_MAGIC_AMPLIFY = 3,
		IGNORE_MAGIC_RESIST = 4,
		IGNORE_MAX = 5
	};
	
	bitset<IGNORE_MAX> m_bsAttr; // 속성값
	
protected:
	mutable __int8 m_nCriticalCount;   // CSD-031007 : 크리티칼 히트를 위한 카운트
	mutable __int8 m_nItemEffectCount; // CSD-031007 : 아이템 효과를 위한 카운트

private:
	short	m_idServer;          // CSD-HK-030829
	int     m_nLevel;			 // CSD-030806
	int		m_nGuildCode;		 // CSD-030806
	__int8  m_nDualCls;          // 듀얼 클래스 종류
	__int8  m_nClsStep;          // 클래스 단계
	__int8  m_nExpStep;          // 경험치 단계
	__int8  m_aTacStep[NO_TAC];  // 택틱 경험치 단계
	
	bool	m_bEventRecv;		// BBD 040308	이벤트 아이템 중복 지급요청을 막는 플래그
private:
	CGainPoint  m_gnAbility;     // 올릴 수 있는 어빌러티 포인터
	CGainPoint  m_gnCombat;      // 올릴 수 있는 전투스킬 포인터
	bool	    m_bWanted;     // 현상범 여부
	DWORD	    m_dwBankMoney;	//은행돈
	short	    m_nBillingType;	//과금 타입
	CBaseArena* m_pArenaGame; // CSD-030509
	CArenaTeam* m_pArenaTeam; // CSD-030509
	vector<int> m_nHunterListIds;
};
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// 인라인 함수
inline int CCharInfo::IncLife(int nVolume, int nMax = 100) 
{ // Life 증가
	const int nOld = Hp;
	const int nLimit = HpMax*nMax/100;
	Hp += nVolume; 
	if (Hp > nLimit)  Hp = nLimit;
	return Hp - nOld;
}

inline int CCharInfo::DecLife(int nVolume, int nMin = 0) 
{ // Life 감소
	const int nOld = Hp;
	const int nLimit = HpMax*nMin/100;
	Hp -= nVolume; 
	if (Hp < nLimit)  Hp = nLimit;
	return nOld - Hp;
}
// 마법력에 관련된 Method
inline int CCharInfo::IncMana(int nVolume, int nMax = 100)
{ // Mana 증가
	const int nOld = Mana;
	const int nLimit = ManaMax*nMax/100;
	Mana += nVolume; 
	if (Mana > nLimit)  Mana = nLimit;
	return Mana - nOld;
}

inline int CCharInfo::DecMana(int nVolume, int nMin = 0) 
{ // Mana 감소
	const int nOld = Mana;
	const int nLimit = ManaMax*nMin/100;
	Mana -= nVolume; 
	if (Mana < nLimit)  Mana = nLimit;
	return nOld - Mana;
}

inline int CCharInfo::IncHungry(int nVolume, int nMax = 100) 
{ // 배고픔정도 증가
	const int nOld = Hungry;
	const int nLimit = HungryMax*nMax/100;
	Hungry += nVolume; 
	if (Hungry > nLimit)  Hungry = nLimit;
	return Hungry - nOld;
}

inline int CCharInfo::DecHungry(int nVolume, int nMin = 0)
{ // 배고픔정도 감소
	const int nOld = Hungry;
	const int nLimit = HungryMax*nMin/100;
	Hungry -= nVolume; 
	if (Hungry < nLimit)  Hungry = nLimit;
	return nOld - Hungry;
}

inline int CCharInfo::IncCombat(int nVolume, int nMax = 100) 
{ // 전투스킬 사용가능량 증가
	const int nOld = Cp;
	const int nLimit = CpMax*nMax/100;
	Cp += nVolume; 
	if (Cp > nLimit)  Cp = nLimit;
	return Cp - nOld;
}

inline int CCharInfo::DecCombat(int nVolume, int nMin = 0)
{ // 전투스킬 사용가능량 감소
	const int nOld = Cp;
	const int nLimit = CpMax*nMin/100;
	Cp -= nVolume; 
	if (Cp < nLimit)  Cp = nLimit;
	return nOld - Cp;
}
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_CHARINFO_H__1A212137_3131_4DD3_8343_EE42343C165E__INCLUDED_)
