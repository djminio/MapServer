#if !defined(AFX_OP_MAGIC_H__9E46C0B2_FD86_4EBD_B107_B9D8B67B0669__INCLUDED_)
#define AFX_OP_MAGIC_H__9E46C0B2_FD86_4EBD_B107_B9D8B67B0669__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define		SET_LOSTSIGHT	1			//시력상실
#define		LIGHTNESS		2			//빛에 의한 어둠해소
#define		SET_CUREBLIND	3			//시력회복 처리
#define		SET_INFORMATION	4			//성직자-바른 깨닭음(상대 정보를 얻어옴)

////////////////////////// lkh 0725 수정 ///////////////////////////
#define		TRAP_TIME		60			//마법 덫 유지시간 : 60000밀리초->1분

enum eT_TYPE
{
	PT_PHASING			        =1,
	PT_TOWNPORTAL		        =2,
	PT_TELEPORT			        =3,
	PT_MULTIPORT		        =4,
	PT_MEMORIZINGPORTAL	        =5,
	PT_ABSOL_MOVE_BY_COUNSELLER =6,
	PT_DODGE                    =7,
};

typedef struct tagMAGICTRAP
{
	short int		trap_X;
	short int		trap_Y;
	short int		trap_Type;
	DWORD	Time;

	struct	tagMAGICTRAP *prev;
	struct	tagMAGICTRAP *next;
}	MAGICTRAP, *LPMAGICTRAP;

extern MAGICTRAP	g_MagicTrap;
extern LPMAGICTRAP	g_lpMagicTrap[8][8];
extern int			g_Condition_Table[8][8];

/////////////////////////// 0320 lkh 추가 //////////////////////////////
extern void	LoadConditionTable(void);
extern void InitTrap();
extern BOOL	InsertTrap(short int tile_X, short int tile_Y, short int trap_Type, DWORD continue_Time);
extern void	AddTrap( LPMAGICTRAP *Header, LPMAGICTRAP lpMagicTrap_add );
extern void	DeleteTrap( LPMAGICTRAP *Header, LPMAGICTRAP lpTrap_Delete);
extern LPMAGICTRAP	FindTrap( LPMAGICTRAP *Header, short int tile_X, short int tile_Y );
extern void			CheckTimeTrap(void);
extern void			SendExposeTrap(CHARLIST* target, short int tile_X, short int tile_Y, short int trap_Type);
extern void			SendCMD_MAGICTRAP_EXPOSE( t_server_magictrap_expose *p);
extern void			RecvHexaEditing(int cn, t_client_editing_table *p);

extern void	SendCMD_MAGIC_POSITION( t_server_magic_position *p);
extern void AddCasterEXP(CHARLIST *attacker, CHARLIST *defender, int result, int magicType, int magic_Num=0, int damage=0);
extern void NPCLostSight(CHARLIST *target, int Time);
extern void SendViewType(CHARLIST *target, int view_Type, int continue_Time);
extern void SendViewTypeChange(int, int);
extern void SendCharacterCondition(CHARLIST*, int);
extern void SendMagicCommand(CHARLIST* lpChar, short int slot1 = 0, short int slot2 = 0, short int slot3 = 0);
extern void CheckCharacterCondition(CHARLIST* pTarget);
extern void CheckTransparency(CHARLIST* pTarget, bool bAuto = false);
extern bool IsMovable(int& rX, int& rY);
extern void TransArea(BYTE nType, CHARLIST* pTarget, int nX, int nY);
extern void TransMap(CHARLIST* pTarget);
extern bool SummonMonster(CHARLIST* pMaster, int nCreate, int nX, int nY);
extern bool FollowSummon(WORD idFollowing, WORD idMaster);
extern void KillMonster(WORD wNumber);
extern bool SearchTrap(CHARLIST* pCaster, int nX, int nY);
extern int getEquipRandomPos( void );

#endif // !defined(AFX_OP_MAGIC_H__9E46C0B2_FD86_4EBD_B107_B9D8B67B0669__INCLUDED_)

