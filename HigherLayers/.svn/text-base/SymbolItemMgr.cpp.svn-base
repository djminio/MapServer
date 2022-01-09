// SymbolItemMgr.cpp: implementation of the CSymbolItemMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "SymbolItemMgr.h"
#include "RareEffectMgr.h"
#include "Scrp_exe.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSymbolItemMgr g_CSymbolMgr;

CSymbolItemMgr::CSymbolItemMgr(HDBC hDragonDB, HDBC hChrLogDB)
{
	Init(hDragonDB,hChrLogDB);
}
CSymbolItemMgr::CSymbolItemMgr()
{
	Init();	
}
void CSymbolItemMgr::Init(HDBC hDragonDB, HDBC hChrLogDB)
{
	InitDBHandle(hDragonDB,hChrLogDB);
	m_vtSymbolItemList.clear();
	m_vtSymbolItemList.reserve(MAX_SYMBOL_ITEM); // 정의된 심볼아이템의 갯수만큼만 할당해준다.
	m_nAttrMaxCount = 0;
}

CSymbolItemMgr::~CSymbolItemMgr()
{
	DestroyAllObject();
}


void CSymbolItemMgr::InitDBHandle(HDBC hDragonDB, HDBC hChrLogDB)
{
	m_hDragonDB = hDragonDB;
	m_hChrLogDB = hChrLogDB;
	m_nAttrMaxCount = 0;
}

bool CSymbolItemMgr::LoadTable()
{
	if(m_hDragonDB == NULL)return false;
	
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
	char		szQuerry[512];
	
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT * FROM Item_Symbol");
	SQLAllocStmt(m_hDragonDB, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);

	if (!SQLOK(retCode))
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return false;
	}

//< Item_Symbol 테이블 로드
	for(int nItemCount = 0;nItemCount < MAX_SYMBOL_ITEM; nItemCount++)
	{
		retCode = SQLFetch(hStmt);
	
		if(!SQLOK(retCode))
		{
			SQLFreeStmt(hStmt, SQL_DROP);
			return false;
		}

		SYMBOL_ITEM	Item; memset(&Item,0,sizeof(SYMBOL_ITEM));

		int nFieldCount;
		for( nFieldCount= 0;nFieldCount < 8 ; nFieldCount++)
		{
			switch(nFieldCount + 1)
			{
			case 3:
				retCode = SQLGetData(hStmt,nFieldCount+1,SQL_C_CHAR,Item.strName,MAX_NAME_LENGTH,&cbValue);
				break;
			case 4:
				retCode = SQLGetData(hStmt,nFieldCount+1,SQL_C_CHAR,Item.strHanName,MAX_NAME_LENGTH,&cbValue);
				break;
			case 5:
				retCode = SQLGetData(hStmt,nFieldCount+1,SQL_C_LONG,&Item.nFirst_Class,0,&cbValue);
				break;
			case 6:
				retCode = SQLGetData(hStmt,nFieldCount+1,SQL_C_LONG,&Item.nSecond_Class,0,&cbValue);
				break;
			}

			if (!SQLOK(retCode))
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return false;
			}

		}
			

		int nAttrIndex = 0;
		for(;nFieldCount < 8 + MAX_APPLY_ATTR;++nFieldCount,++nAttrIndex)
		{
			retCode = SQLGetData(hStmt,nFieldCount+1,SQL_C_LONG,&(Item.anAplly_Attr[nAttrIndex]),0,&cbValue);

			if (!SQLOK(retCode))
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return false;
			}
			
			if(Item.anAplly_Attr[nAttrIndex] > 0) //속성필드에 값이 있으면.
			{
				m_nAttrMaxCount = __max(m_nAttrMaxCount,nAttrIndex+1);
			}
		}

		m_vtSymbolItemList.push_back(Item);
	}

	SQLFreeStmt(hStmt, SQL_DROP);
//> Item_Symbol 테이블 로드	끝.


//< Item_Symbol_AbilityX 테이블들 로드
	for(int i = 0;i < m_nAttrMaxCount;i++)//속성의 쓰인 속성테이블의 갯수 만큼 테이블이 필요하다.
	{
		sprintf_s(szQuerry, sizeof(szQuerry), "SELECT * FROM Item_Symbol_Ability%d",i+1);
		SQLAllocStmt(m_hDragonDB, &hStmt);
		retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
		if (!SQLOK(retCode))
		{
			SQLFreeStmt(hStmt, SQL_DROP);
			return false;
		}

		SYMBOL_VALUE_LIST	ValueList; ValueList.clear();
				
		for(int nLevelCount = 0;nLevelCount < MAX_SYMBOL_LEVEL;nLevelCount++)
		{
			retCode = SQLFetch(hStmt);
			if (!SQLOK(retCode))
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return false;
			}

			SYMBOL_VALUE	Value; memset(&Value,0,sizeof(Value));
			for(int nFieldCount = 0;nFieldCount < MAX_SYMBOL_ITEM;nFieldCount++)
			{
				retCode = SQLGetData(hStmt,nFieldCount+1,SQL_C_LONG,&(Value.anAttrValue[nFieldCount]),0,&cbValue);
				if (!SQLOK(retCode))
				{
					SQLFreeStmt(hStmt, SQL_DROP);
					return false;
				}
			}
			
			ValueList.push_back(Value);
		}

		m_vtSymbolApplyAttrList.push_back(ValueList);
		
		SQLFreeStmt(hStmt, SQL_DROP);
	}
//> Item_Symbol_AbilityX 테이블들 로드 끝.
	
	return true;
}

void CSymbolItemMgr::DestroyAllObject()
{
	m_vtSymbolItemList.clear();	
	m_vtSymbolApplyAttrList.clear();
}

bool CSymbolItemMgr::ExChangeSymbol(CHARLIST *pCharactor,int nFirstClass,int nSecondClass ,int nDualGrade)
{
	if(pCharactor->quick[5].item_no && nDualGrade == 1) //아이템을 처음 얻었다.
	{
		for(int i = 0;i < m_nAttrMaxCount;i++)
		{
			int nAttr = GetRareNum(nFirstClass,nSecondClass,i);

			if(nAttr < 0)// Error이다.
			{
				return false;
			}

			pCharactor->quick[6].attr[i] = nAttr;
		}
	}
	else // 아이템 업그레이드 이다.
	{
				
	}	

	return true;
}

int CSymbolItemMgr::GetItemNumber(int n1StClass, int n2StClass)
{
	int nItemCount = m_vtSymbolItemList.size();
	
	if(nItemCount)
	{
		for(int i = 0; i < nItemCount ;i++)
		{
			if(m_vtSymbolItemList[i].nFirst_Class == n1StClass+1 && m_vtSymbolItemList[i].nSecond_Class == n2StClass+1)
			{
				return i+1;
			}
		}
	}
	return 0;
}
int CSymbolItemMgr::GetRareNum(int nFirstClass, int nSecondClass, int nRareIndex)
{
	int nItemCount = m_vtSymbolItemList.size();
	
	if(nItemCount && nRareIndex < m_nAttrMaxCount)
	{
		for(int i = 0; i < nItemCount ;i++)
		{
			if(m_vtSymbolItemList[i].nFirst_Class == nFirstClass+1 && m_vtSymbolItemList[i].nSecond_Class == nSecondClass+1)
			{
				return m_vtSymbolItemList[i].anAplly_Attr[nRareIndex];
			}
		}
	}

	return 0;
}

int CSymbolItemMgr::AddSymbolEffect(CHARLIST *pChar)
{
	int nItemNumber = pChar->quick[5].item_no;
	int	nDualGrade	= pChar->quick[5].attr[0];	//아이템의 등급이다.
	int	n1StClass	= pChar->Class;
	int	n2StClass	= pChar->GetDualClass();
	
	if(!nItemNumber || !nDualGrade)return 0;

	for(int i = 0;i < MAX_APPLY_ATTR;i++)
	{
		int nKind = GetRareNum(n1StClass,n2StClass,i);
		if(!nKind)
		{	
//			::MyLog(0,"NAME : %s SymbolNum
			return 0;
		}		
		if(nKind == 107) continue;//19번 심벌에 마법 속성 추가 문제. 수정.//soto-030520


		if(i >= m_vtSymbolApplyAttrList.size() || nDualGrade > m_vtSymbolApplyAttrList[i].size()|| nItemNumber > MAX_SYMBOL_ITEM)
		{
			::MyLog(0,"심벌 AddSymbolEffect()!!!!ERROR!!!!   NAME : %s --> AttrNum: %d, nDualGrade: %d, ItemNumber: %d",i,nDualGrade,nItemNumber);
			return 0;
		}

		int nValue = m_vtSymbolApplyAttrList[i][nDualGrade-1].anAttrValue[nItemNumber-1];
		
		if(!nValue)continue;
		RareEM.ApplyStaticRareEffect(pChar->StaticRare,nKind,nValue,true);
	}

	return 1;	
}



bool CSymbolItemMgr::DoHaveSymbol(CHARLIST *pChar)
{
	return (pChar->quick[5].item_no > 0);
}



int CSymbolItemMgr::GetSymbolGrade(CHARLIST *pChar)
{
	return (int)pChar->quick[5].attr[0];
}

void CSymbolItemMgr::MakeSymbolItem(CHARLIST*	pChar,int nGrade)
{
	int nItemNumber = this->GetItemNumber(pChar->Class,pChar->GetDualClass());
	
	if(nItemNumber)
	{
		//서버에서의 만들기.
		pChar->quick[5].item_no = nItemNumber;
		pChar->quick[5].attr[0] = nGrade;
		
		//클라이언트로 만들었다는 메세지를 보낸다.
		t_packet	packet;memset(&packet,0,sizeof(t_packet));
		packet.h.header.type = CMD_SYMBOL_MAKE;
		packet.h.header.size = sizeof(t_SYMBOL_ITEM_MSG);
		packet.u.Symbol_Item.m_nGrade = nGrade;
		packet.u.Symbol_Item.m_nItemNumber = nItemNumber;

		QueuePacket(connections,pChar->GetServerID(),&packet,1);
		Send_RareItemMakeLog(pChar->GetServerID(),nItemNumber,-1,nGrade,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
	}
}

//수동 업그레이드
void CSymbolItemMgr::UpGradeSymbol(CHARLIST *pChar, int nGrade)
{
	int nItemNumber = pChar->quick[5].item_no;
	
	if(nItemNumber)
	{
		//서버에서의 업그레이드..
		pChar->quick[5].item_no = nItemNumber;
		pChar->quick[5].attr[0] = nGrade;
		
		//클라이언트로 업그레이드했다는 메세지를 보낸다.
		t_packet	packet;memset(&packet,0,sizeof(t_packet));
		packet.h.header.type = CMD_SYMBOL_UPGRADE;
		packet.h.header.size = sizeof(t_SYMBOL_ITEM_MSG);
		packet.u.Symbol_Item.m_nGrade = nGrade;
		packet.u.Symbol_Item.m_nItemNumber = nItemNumber;

		QueuePacket(connections,pChar->GetServerID(),&packet,1);
		Send_RareItemMakeLog(pChar->GetServerID(),nItemNumber,-2,nGrade,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
	}
}


//자동 업그레이드.
void CSymbolItemMgr::UpGradeSymbol(CHARLIST *pChar)
{
	int nGrade		= pChar->quick[5].attr[0]+1;
	UpGradeSymbol(pChar,nGrade);
}

//이곳에서는..Rare속성에 없는 속성.을 적용시킬때 쓰인다.단하나만.
//인자 설명 ( pChar : 해당 캐릭터, i : 심볼에 달린 속성 몇번째 속성 인가. i+1번째 속성.
int CSymbolItemMgr::GetMagicAttr(CHARLIST *pChar)
{
	int nItemNumber = pChar->quick[5].item_no;
	int	nDualGrade	= pChar->quick[5].attr[0];	//아이템의 등급이다.
	int	n1StClass	= pChar->Class;
	int	n2StClass	= pChar->GetDualClass();

	if(!nItemNumber || !nDualGrade)return 0;
	
	int nValue = 0;

	for(int i = 0;i < MAX_APPLY_ATTR;i++)
	{
		int nKind = GetRareNum(n1StClass,n2StClass,i);
		
		switch(nKind)
		{
		case 107: 
			{
				nValue = m_vtSymbolApplyAttrList[i][nDualGrade-1].anAttrValue[nItemNumber-1];
				return nValue;
			}
		}
	}

	return 0;
}
