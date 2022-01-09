
#include "..\stdafx.h"
#include "Cinfo.h"

CWndInfo WndInfo;

//------------------------------------------------------------------------------------------
CWndInfo::CWndInfo()
{
  DWORD dwVersion;
  DWORD dwBuild;
  DWORD dwWindowMajorVersion, dwWindowMinorVersion;

    Width     = GetDeviceCaps(GetDC(NULL), HORZRES);
    Height    = GetDeviceCaps(GetDC(NULL), VERTRES);
    ColorBits = (GetDeviceCaps(GetDC(NULL), PLANES) * GetDeviceCaps(GetDC(NULL), BITSPIXEL));

    dwVersion = GetVersion();
	  dwWindowMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
	  dwWindowMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));

	  if(dwVersion < 0x80000000)  // NT
	  {
	    OS = 2;
	    dwBuild = (DWORD)(HIWORD(dwVersion));
	  }
    else
	  if(dwWindowMajorVersion < 4)  // Win32S
	  {
	    OS = 0;
	    dwBuild = (DWORD)(HIWORD(dwVersion) & ~0x8000);
	  }
    else
	  {
	    OS = 1;
      dwBuild = 0;  // Win95
	  }
};

//------------------------------------------------------------------------------------------
CWndInfo::~CWndInfo()
{
};

//------------------------------------------------------------------------------------------
int CWndInfo::GetWidth(void)
{
    return(Width);
};

//------------------------------------------------------------------------------------------
int CWndInfo::GetHeight(void)
{
    return(Height);
};

//------------------------------------------------------------------------------------------
int CWndInfo::GetColorBits(void)
{
    return(ColorBits);
};

//------------------------------------------------------------------------------------------
int CWndInfo::GetOS(void)
{
    return(OS);
};

//------------------------------------------------------------------------------------------
