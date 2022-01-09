// MailMgr.h: interface for the CMailMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAILMGR_H__3DCF6E55_A881_498E_9E4E_A342F8E62D81__INCLUDED_)
#define AFX_MAILMGR_H__3DCF6E55_A881_498E_9E4E_A342F8E62D81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



enum eMAIL_VIEW_TYPE
{//인터페이스의 nWillDo에도 사용 됩니다.
	MVT_RECV_LIST,//받은편지함
	MVT_DELETE_LIST,//지운 편지함
};

class CMailMgr  
{
	enum eVALUE
	{
		LIST_LINE_GAB	= 24,
	};

	enum eMAIL_TYPE
	{
		NORMAL_MAIL	= 1,
		GUILD_MAIL	= 2,
		GAMEMASTER_MAIL	= 3,
		
		NORMAL_MAIL_REPLY		= 101,
		GAMEMASTER_MAIL_REPLY	= 103,
	};

public:
	CMailMgr();
	virtual ~CMailMgr();

public:
	bool Recv(const int cn,t_packet *p);//보내기
	void RecvDelete(const int cn,t_packet *p);//지우기(리스트 번호를 보냅니다.)
	void RecvRepair(const int cn,t_packet *p);//복원하기(리스트 번호를 보냅니다.)

	void RecvRequestBody(const int cn,t_packet *p);//해당 메일의 내용 받기
	void RecvBody(const int cn,t_packet *p);

	void SaveMailToLocal(const int cn,t_packet *p);//선택된 메일 저장 하기.
	void RecvRequestBodyResult(const int cn,t_packet *p);
public:
	void SelectAll();//모두 선택하기
	void SendReqOtherPage(const int cn,t_packet *p);//다른 페이지 요청
	void RecvReqMailList(const int cn,t_packet *p);//리스트 요청을 받음
	
	void RecvMailListFromDBDEMON(const int cn,t_packet *p);//데몬으로 부터 리스트를 전해 받음 
	
	void SendReqMailList(const int cn,t_packet *p);//메일 리스트 요청
};

extern CMailMgr g_MailMgr;
#endif // !defined(AFX_MAILMGR_H__3DCF6E55_A881_498E_9E4E_A342F8E62D81__INCLUDED_)
