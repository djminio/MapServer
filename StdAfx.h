// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once

#ifndef WINVER
#define WINVER 0x0A00
#endif

#pragma warning(disable: 4996)

#include <afx.h>
#include <ole2.h>
#include <initguid.h>
#include <windows.h>
#include <winsock2.h>
// 공유 라이브러리
#pragma comment (lib, "../Library/HSEL.lib")
#include "../Shared/HSEL.h"
#pragma comment (lib, "../Library/Shared.lib")
#include "../Shared/Shared.h"

#define rand()	rand_()		// 020707 YGI

extern int rand_();

//#define BUFF_BUG_CHECKS // Finito 24/08/07 enable buff bug checking. Checks if tiem difference 
						// is 12 hours an takes  away 12 hours so that server time is kept at the proper time

#include "HigherLayers/DefaultHeader.h"

#include "GameSystem.h"


