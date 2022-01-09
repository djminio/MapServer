#ifndef	__PATH_H__
#define	__PATH_H__


typedef struct	tagSPNODE
{
	int					f, h, g;
	int					x, y;
	int					number;
	struct tagSPNODE	*lpParent;
	struct tagSPNODE	*lpChild[ 8 ];
	struct tagSPNODE	*lpNext;
} SP_NODE, *LPSP_NODE;

typedef struct	tagSPSTACK
{
	struct tagSPNODE	*lpNode;
	struct tagSPSTACK	*lpNext;
} SP_STACK, *LPSP_STACK;

#ifdef __cplusplus
extern "C"
{
#endif

	extern BOOL			PathBuild( CHARLIST *ch, int dx, int dy );
	extern void			PathDestroy( void );
	extern LPSP_NODE	PathFind( CHARLIST *ch,int sx, int sy, int dx, int dy );

	////////////////// 0730 yjs 수정 디폴트 인자값을 넣음 //////////////////////////
	extern int			FreeTile( LPCHARLIST c, int sx, int sy, int x, int y, BOOL	notCheck_My=0, LPCHARLIST attacker=NULL );
	////////////////////////////////////////////////////////////////////////////////

	#ifdef __cplusplus
}
#endif

#endif	// __PATH_H__











