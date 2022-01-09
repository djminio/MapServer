#define MemAlloc(x, y)				__Alloc( (char **)(&##x), (y), __FILE__, __LINE__ )
#define MemFree( x ) MemFree_Sub( (char **)(&##x), __FILE__, __LINE__ )

#ifndef __HONG_SUB_H__
#define __HONG_SUB_H__

extern void Debug( char *s, ... );
extern bool BoxAndDotCrash(int dx, int dy, int dxl, int dyl, int x, int y);
extern void MemFree_Sub( char **buf, char *file, int line  );
extern void __Alloc( char **s, int size, const char *file, int line );
extern char* EatRearWhiteChar( char* pStr );
extern int CheckIntersect(int p1x, int p1y, int p2x, int p2y, int p3x, int p3y, int p4x, int p4y);
extern int CheckCurFrame( void );
extern int YesOrNo( char *s, char *title );

extern int ViewCheckRoutine( int t );
extern void Error ( char *mssg, ... );
extern void JustMsg( char *s, ... );

#endif
