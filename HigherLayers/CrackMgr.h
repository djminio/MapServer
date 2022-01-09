// CrackMgr.h: interface for the CCrackMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRACKMGR_H__9A9C3C94_89C2_4C08_8C9E_12ADDEA0459F__INCLUDED_)
#define AFX_CRACKMGR_H__9A9C3C94_89C2_4C08_8C9E_12ADDEA0459F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const	int HACKING_EFFECTTABLE_	=	20001;
const	int HACKING_011106_			=	20002;
const	int HACKING_CONDITIONTABLE_	=	20003;
const	int HACKING_MOVEPATTERN_	=	20004;
const	int HACKING_ANITABLE_		=	20005;
const	int HACKING_ATTACKRANGE_	=	20006;
const	int HACKING_ACCELATOR_		=	20007;
const	int HACKING_FILTER_			=	20008;
//acer5
#define USE_GM_TOOL				10001


class CCrackMgr  
{
public:
	CCrackMgr();
	virtual ~CCrackMgr();
	void AutoCheckAccelator(CHARLIST *ch);
	void ComformAccelator( const int cn, const t_packet *packet );
	void SendHackingUser( const int cn, const int type, const char *cause );
	void RecvAccelType(int cn,t_packet *p); // CSD-030808
};
extern CCrackMgr CrackMgr;
#endif // !defined(AFX_CRACKMGR_H__9A9C3C94_89C2_4C08_8C9E_12ADDEA0459F__INCLUDED_)
