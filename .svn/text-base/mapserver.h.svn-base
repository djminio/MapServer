#ifndef SERVER_H
#define SERVER_H

#pragma once 

#include "LowerLayers\inetwork.h"
#include "stdafx.h"

extern I4DyuchiNET*	g_pINet;
extern HANDLE hKeyEvent[5];	// 010205 KHS

bool				InitMapServer();
bool				Init();
void				__stdcall OnDisconnectServer(DWORD dwConnectionIndex);
void				EndMapServer();		

#endif
