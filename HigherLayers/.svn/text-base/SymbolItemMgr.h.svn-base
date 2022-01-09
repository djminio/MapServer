// SymbolItemMgr.h: interface for the CSymbolItemMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYMBOLITEMMGR_H__AA6CB90B_B5FE_47E9_BFDA_249D23E92322__INCLUDED_)
#define AFX_SYMBOLITEMMGR_H__AA6CB90B_B5FE_47E9_BFDA_249D23E92322__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
using namespace std;

#define MAX_APPLY_ATTR		5
#define MAX_SYMBOL_LEVEL	5
#define MAX_SYMBOL_ITEM		23	// 현재의 심볼 아이템은 23개이다.
#define MAX_NAME_LENGTH		64
#define MAX_NAME_BUFFER		64+1



typedef struct tag_SYMBOL_ITEM{
	char	strName[MAX_NAME_BUFFER];
	char	strHanName[MAX_NAME_BUFFER];
	int		nFirst_Class;
	int		nSecond_Class;
	int		anAplly_Attr[MAX_APPLY_ATTR]; //해당 되는 Attr에는 레어번호가 들어간다.
}SYMBOL_ITEM;


typedef vector<SYMBOL_ITEM>	SYMBOL_LIST; // 현재 23개다.
typedef vector<SYMBOL_ITEM>::iterator	ITOR_SYMBOL_LIST;

typedef struct tag_SYMBOL_VALUE{
	int		anAttrValue[MAX_SYMBOL_ITEM];
}SYMBOL_VALUE;
typedef vector<SYMBOL_VALUE>	SYMBOL_VALUE_LIST; //현재 5개의 레벨로 설정되어 있다.


typedef vector<SYMBOL_VALUE_LIST> SYMBOL_APPLY_ATTR_LIST;//현재 5개의 속성으로 되어있다.


enum{
	SYMBOL_RARENUM0 = 0,
	SYMBOL_RARENUM1 = 1,
	SYMBOL_RARENUM2 = 2,
	SYMBOL_RARENUM3 = 3,
	SYMBOL_RARENUM4 = 4,
	SYMBOL_RARENUM5 = 5
};



class CSymbolItemMgr
{
private:
	SYMBOL_LIST				m_vtSymbolItemList;
	SYMBOL_APPLY_ATTR_LIST	m_vtSymbolApplyAttrList;

	int						m_nAttrMaxCount;// 한 아이템에 포함되는 속성의 최대 개수.
	HDBC					m_hDragonDB;
	HDBC					m_hChrLogDB;


	int GetRareNum(int nFirstClass, int nSecondClass, int nRareIndex);
public:
	int GetMagicAttr(CHARLIST *pChar);
	void UpGradeSymbol(CHARLIST *pChar);
	void UpGradeSymbol(CHARLIST* pChar,int nGrade);
	void MakeSymbolItem(CHARLIST*	pChar,int nGrade=1);
	int GetSymbolGrade(CHARLIST* pChar);
	bool DoHaveSymbol(CHARLIST* pChar);
	int GetItemNumber(int n1StClass,int n2StClass);
	int AddSymbolEffect(CHARLIST *pChar);
	
	bool ExChangeSymbol(CHARLIST *pCharactor,int nFirstClass,int nSecondClass ,int nDualGrade);
	void Init(HDBC hDragonDB = NULL, HDBC hChrLogDB = NULL);
	void DestroyAllObject();
	bool LoadTable();
	void InitDBHandle(HDBC hDragonDB = NULL,HDBC hChrLogDB = NULL);
	CSymbolItemMgr(HDBC hDragonDB, HDBC hChrLogDB);
	CSymbolItemMgr();
	virtual ~CSymbolItemMgr();
};

extern CSymbolItemMgr g_CSymbolMgr;

#endif // !defined(AFX_SYMBOLITEMMGR_H__AA6CB90B_B5FE_47E9_BFDA_249D23E92322__INCLUDED_)
