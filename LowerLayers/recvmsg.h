#ifndef	__RECV__MSG_H
#define	__RECV__MSG_H

#include "..\stdafx.h"

void __stdcall ReceivedMsgFromServer(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void __stdcall OnAcceptServer( DWORD dwConnectionIndex );
void __stdcall OnAcceptUser( DWORD dwConnectionIndex );
void __stdcall OnDisconnectServer( DWORD dwConnectionIndex );
void __stdcall ReceivedMsgFromUser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

#endif 

