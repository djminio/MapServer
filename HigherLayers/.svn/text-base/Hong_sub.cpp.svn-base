#define __HONG_SUB_H__

#include "Hong_Sub.h"
#include "..\stdafx.h"
#include "DefaultHeader.h"
#include <mmsystem.h>

#include "RajaSystem.h"

bool BoxAndDotCrash(int dx, int dy, int dxl, int dyl, int x, int y)
{	
  return (dx <= x && x <= dx + dxl  &&  dy <= y && y <= dy+dyl) ? true:false;
}	

static int IMemUseCount;
static DWORD ITotalUseMemory;

/*────────────────────────────────────────
함수명 : void FreeMem( char **buf )
설  명 : *buf의 값이 NULL이 아닌경우에만 free한다.
인  자 :
리  턴 :
────────────────────────────────────────*/
void MemFree_Sub( char **buf, char *file, int line  )
{
	if( *buf == NULL ) return; 

    IMemUseCount--;

	if( *buf != NULL ) GlobalFree( *buf );
	*buf = NULL;
}



/*────────────────────────────────────────
함수명 : void __Alloc( char **s, int size, const char *file, int line )
설  명 :
인  자 :
리  턴 :
────────────────────────────────────────*/
void __Alloc( char **s, int size, const char *file, int line )
{	
	if( *s )	
	{
		//Error( "*s 에 뭔가가 있다. " );
		MessageBox( NULL, "*s 에 뭔가가 있다. ", "에러", MB_OK );
	}
	
    *s = (char *)GlobalAlloc( GMEM_FIXED, size );

	ITotalUseMemory += size;

    if( !(*s) )
	{
		char temp[ FILENAME_MAX];
		sprintf_s( temp, sizeof(temp), "%s FILE %d LINE --> %d size alloc error.\n", file, line, size );
		MessageBox( NULL, temp, "에러", MB_OK );
		exit(0);
	}
	memset( *s, 0, size );

    IMemUseCount++;
}	
	
	



char* EatRearWhiteChar( char* pStr )
{	
	char*	szWhite = " \t\n\r";
	char*	pRear;
	
	pRear = pStr + strlen( pStr ) - 1;
	if ( pRear )
	{
		while ( pStr <= pRear )
		{
			if ( strchr( szWhite, *pRear ) )
			{
				*pRear-- = 0;
			}
			else
			{
				break;
			}
		}
	}
	
	return	pStr;
}	


static inline void swap( int  *a, int *b )
{
	int t;

	t = *a;
	*a = *b;
	*b = t;	
}			

int CheckIntersect(int p1x, int p1y, int p2x, int p2y, int p3x, int p3y, int p4x, int p4y)
{			
    long m;	
    float x;
    if (p1x > p2x)
	{		
		swap(&p1x, &p2x);
		swap(&p1y, &p2y);
	}		
    if (p3x > p4x)
	{		
		swap(&p3x, &p4x);
		swap(&p3y, &p4y);
	}		
    m = (p4x - p3x)*(p2y - p1y) - (p4y - p3y)*(p2x - p1x);
			
    x = ((float)(p3y - p1y)*(p2x - p1x)*(p4x - p3x)
	 + (float)p1x*(p2y - p1y)*(p4x - p3x)
	 - (float)p3x*(p4y - p3y)*(p2x - p1x)) / (float)m;
    if (p1x <= x && x <= p2x && p3x <= x && x <= p4x) return 1;
    else return -1;
}			
			
			




int CheckCurFrame( void )
{	
	static int TimeCheckCount;
	static int TickCount, TickFlag;
	static int currentfps;

	if( TickFlag == 0 )	{		TickCount = timeGetTime();		TickFlag = 1;	}
	TimeCheckCount++;
	if(  timeGetTime() - TickCount >=1000 ) { 	TickFlag = 0; currentfps = TimeCheckCount; TimeCheckCount= 0; }

	return currentfps;
}	
	
	
	
int YesOrNo( char *s, char *title )
{	
	DWORD size = strlen(s);
	DWORD size2 = strlen(title);
	return MessageBox( NULL, s, title, MB_YESNO | MB_ICONQUESTION );
}	
	
	
	
int ViewCheckRoutine( int t )
{	
	static int TimeCheckCount;
	static int TickCount, OldTickCount, TickFlag;
	static int currentfps;
	
	if( t == 0 ) TickFlag = 0;
	if( TickFlag == 0 )	{	OldTickCount = TickCount = timeGetTime();	TickFlag = 1; 	}
	else
	{
		OldTickCount= TickCount;
		TickCount	= timeGetTime();
	}
	char temp[ FILENAME_MAX];
	sprintf_s( temp, sizeof(temp), "%d : %d \n", t, TickCount - OldTickCount );
	OutputDebugString( temp );

//	if( TickCount - OldTickCount )
//	if ( t == 9999 )
//	{
//		FILE *fp = fopen( "000_check_time.txt", "at+" );
//		if( fp ) 
//		{
//			fprintf( fp, temp );
//			fclose(fp);
//		}
//	}
	
	return TickCount - OldTickCount;
}						
	
	
	
void Error ( char *mssg, ... )
{	
	char caTemp[ FILENAME_MAX];
	va_list va;
	HWND hwnd;
	
	va_start( va, mssg );
	vsprintf_s( caTemp, sizeof(caTemp), mssg, va );
	va_end( va );
	
	hwnd = GetActiveWindow();
	
	if( MessageBox( hwnd, caTemp, "Error", MB_YESNO ) == IDOK )	goto END_;
	
	_fcloseall();
	PostQuitMessage(0); 

END_:

	return;
}


void JustMsg( char *s, ... )
{	
    char temp[ FILENAME_MAX];
    va_list arg;
    va_start( arg, s );
    vsprintf_s( temp, sizeof(temp), s, arg );
    va_end( arg );
	HWND hwnd = GetActiveWindow();
	
	MessageBox( hwnd, temp, "Information", MB_OK | MB_ICONINFORMATION );
}	
	
	
	
	
	
	
