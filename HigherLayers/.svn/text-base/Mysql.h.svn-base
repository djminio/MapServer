#if !defined(AFX_MYSQL_H__D6CBF8EF_AAC2_4E8E_823B_4ED7AE440D83__INCLUDED_)
#define AFX_MYSQL_H__D6CBF8EF_AAC2_4E8E_823B_4ED7AE440D83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAIL_BUF		2048			// 0507
#define MAX_SKILLMAIN 150
#define MAX_SKILLMAPTABLE	100


typedef struct t_skillmain{
int NO;
char Skill_Minute[31];
int Class_Warrior;
int Class_Thief;
int Class_Archery;
int Class_Wizard;
int Class_Priest;
int Select_Farmer;
int Select_miner;
int Select_fisher;
int Select_Lumberjack;
int Select_Butcher;
int Select_Herbdigger;
int Select_herdman;
int Select_Carpenter;
int Select_Blacksmith;
int Select_Cooking	;
int Select_Tailoring	;
int Select_Bowcraft_Fletcher;
int Select_Alchemy	;
int Select_Candlemaker	;
int Select_Merchant	;
int Howto	;
int mouse_change	;
int Interface1	;
int Interface2	;
int Success_interface	;
int Fail_Interface	;
int Work_target_self	;
int Work_target_othercharacter	;
int Work_target_object	;
int Work_target_Item	;
int Work_target_earth	;
int Work_target_water	;
int Work_target_forest	;
int Work_target_interface	;
int Tool;
int Object_Workshop	;
int Object_target	;
int Obj_Attribute	;
int Resource1	;
int Resource1_quantity	;
int Resourece2	;
int Resource2_quantity	;
int Resource3	;
int Resource3_quantity	;
int Produce_type	;
int Produce_Fix	;
int Ability_Fix	;
int Time_judge	;
int Time_Maintain	;
int Ability_min	;
int Ability_max	;
int Plus_element	;
int Plus_type	;
int Plus_Rate	;
int Plus_min	;
int SP_demand	;
int Success	;
int effectCharobj_status	;
int Change_enermy_status	;
int Change_enermy_status2	;
int charvisible	;
int Effectobj_status	;
int Changeobj_attribute	;
int changeobj_visible	;
int Skill2job	;
int effectChar_hide	;
int Increase_mov	;
int Increse_Life	;
int Increse_mana	;
int Increse_health	;
int Change_mor	;
int Obj_attribute1	;
int ChangeObj_attribute1	;
int Display_LV	;
int DisPlay_Wis	;
int Display_Int	;
int DisPlay_Life	;
int DisPlay_Health	;
int DisPlay_Damage	;
int DisPlay_AC	;
int DisPlay_inventory	;
int Display_Foot	;
int DisPlay_Item_Changeall	;
int DisPlay_royalty	;
int DisPlay_Item_Status	;
int DisPlay_name	;
int DisPlay_item_Price	;
int effect	;
int effect_wave	;
int effect_race	;
int Success_range	;
int Damage	;
int Status_self	;
int Effect1	;
int Effect_wave1	;
int Enemy_status	;
int Range	;
int Enermy_alram	;
int map	;
int Curser1	;
int Curser2	;
int Curser3	;
int point	;
int inclusive;
int money;
int need_exp;
int MotherSkillType;
int MotherSkillExp;

int k1;
int k2;

}SkillMain;


//this4 lsw
typedef struct t_skillmaptable{

	int NO;
	char han_name[ 31];
	int Item_id	;
	int Skill_type	;
	int demand	;
	int table[11];

	//011014 lsw >
	int new_item[3];
	int new_item_pro[3];
	//011014 lsw <

}t_SkillMapTable;


extern SkillMain SkillTbl[ MAX_SKILLMAIN];
extern t_SkillMapTable SkillMapTable[ MAX_SKILLMAPTABLE];
extern int             SkillMapTableCount;








extern int Init_SQL(LPSTR szDSN, LPSTR szUID, LPSTR szUPW);
extern int Release_SQL(void);
//YGI acer
extern int Querry_SQL(LPSTR szQuerry, HDBC hdbc = NULL);

extern int update_BinaryData_to_Chr_Info (UCHAR *ws, UCHAR *pa,  UCHAR *akill, UCHAR *skill_exp, UCHAR *tac_skill_exp, UCHAR *script_var, UCHAR *inv,  UCHAR *equip, UCHAR *quick, UCHAR *party,UCHAR *relation,UCHAR *employment,UCHAR *itemindex,char *, char *) ;	//1219 YGI
extern int update_BinaryData_to_Chr_Info2(UCHAR *bankitem, char *, char *) ;


extern int  updateCharacterStatus(t_connection [], int) ;

extern void updateCharacterVeryImportantStatusToLoginServer( t_connection c[], int char_id );
extern void updateCharacterVeryImportant_TacticsSkillExp_ToLoginServer( t_connection c[], int char_id );
extern void updateCharacterStatusToLoginServer( t_connection c[], int char_id);
extern void updateBinaryDataToLoaginServer( t_connection c[], int id );



//extern int AddBoxItem_SQL(int id);//020315 lsw 
extern int DeleteBoxItem_SQL(int id);


extern int TotalConnections( void );


extern int InitSkillTable( void );
extern int InitSkillExpTable( void );
extern int InitSkillMapTable( void );








//////////////////////////




extern int EndsetPartyMenberForUpdate( CHARLIST *ch );		// 0218 YGI

extern int GetTitleBBS( int count, int ct[10], TIMESTAMP_STRUCT date[10], char name[10][ FILENAME_MAX], char title[10][ FILENAME_MAX] );
extern char * GetMessageBBS_public( int count );			// 0306 YGI

//////////////////////////////////		0405 YGI /////////////////////////
extern int GetBelieveAndFaith_SQL( int &believe, int &faith, char *name );		// 0316 YGI
extern int UpdateFaith_SQL( short int faith, char *name );						// 0316 YGI

extern int UpdateEvaName( char *my_name, char *eva_name );		// 전도한 사람 이름을 디비에 업데이트 한다.
extern void SubtractFaith( char *naem );


extern int GetEvangelist( char *my_name, char *eva_name );

extern int GetGodMeetingTime( TIMESTAMP_STRUCT &day, TIMESTAMP_STRUCT &month, TIMESTAMP_STRUCT &year, int god_index );
extern int GetMeetingMessage( int god_type, int text_type, int text_num, char *msg );

extern int UpdateCharStatusByKein( k_char_update_data *ch );		// 0410 YGI
extern int GetCharDataStatusByKein( k_get_char_info *ch, char *name );		// 0410 YGI
extern int UpdateCharStatusByKein( t_connection c[], short int cn );		// 0410 YGI

extern int AccessBoxData( int box_id );			// 0420 YGI



extern int LoadGenerateSpecialItem( void );	// 0927-1 KHS
#define SQLOK( x ) ( (x) == SQL_SUCCESS_WITH_INFO || (x) == SQL_SUCCESS )		//1027 YGI
extern int InitGmQuest( void );
extern int LoadNationInfo( tagNationInfo *NationInfo );			// 010522 YGI

extern int LoadGuildListForMapServer( );

int LoadAbilityLimit();
int LoadAbilityDivide();
bool LoadDualInfo();

extern int InitGameItem();
extern int LoadNationItem( int week );
extern int LoadGuildHouse();

#define MAX_GOD_TEXT	1024
//YGI acer
extern int SaveItemLog( k_item_log *pData );
extern int Init_ChrLogDB_SQL(LPSTR szDSN, const char* szUID, const char* szUPW );
extern void InitItemLog( int port );

//020620
extern int LoadDungeonMap();
extern int GetRowLineOfSQL( const char *table, const char *field, int *RowCount, const char *condition );

//soto-030324
extern int LoadGuardianGuildList();

bool LoadArenaInfo();       // CSD-030509 
bool LoadArenaGameInfo();   // CSD-030509 
bool LoadArenaTeamInfo();   // CSD-030509 
bool LoadArenaBonusInfo();  // CSD-030517
bool LoadHuntInfo();        // CSD-030509
bool LoadHuntGroupInfo();   // CSD-030509
bool LoadHuntMonsterInfo(); // CSD-030509
bool LoadHuntPlaceInfo();   // CSD-030509 
bool LoadWarStartInfo();    // CSD-030723
//021030 YGI
extern int GetEventNpcItemAmount( int event_no );
// 040105 YGI 보물
extern bool LoadEventTreasureXY();
extern bool LoadEventTreasureGuard();
extern bool LoadStoreTable();
extern bool LoadItemMallTable();

extern HENV  hEnv;
extern HDBC  hDBC;
extern HENV  hEnv_ChrLogDB;
extern HDBC  hDBC_ChrLogDB;
extern bool CanSaveUserData(LPCHARLIST ch, const int iCallType);

// 031009 CI YGI
extern int GetItemLimitCount_SQL( WORD port, DWORD *pLimit );

#endif // !defined(AFX_MYSQL_H__D6CBF8EF_AAC2_4E8E_823B_4ED7AE440D83__INCLUDED_)
