// EventTreasureBoxMgr.h: interface for the CEventTreasureBoxMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVENTTREASUREBOXMGR_H__B5661CEB_1F05_4766_89B5_FFB98C5D1F85__INCLUDED_)
#define AFX_EVENTTREASUREBOXMGR_H__B5661CEB_1F05_4766_89B5_FFB98C5D1F85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//<soto-030709
typedef struct tag_DROPITEM
{
	int nItem;
	int nItem_Rate;
}DROPITEM_;

typedef struct tag_DROPITEM_INFO
{
	int	nSadOnyx;
	int nLeafOfBessed;
	int	nCoins;
	int nGoldCoin;
	int nGrade_Min;
	int nGrade_Max;
	DROPITEM_ stItem[6];
	DROPITEM_ stRareItem[4];
}DROPITEM_INFO;

typedef vector<DROPITEM_INFO> VT_MAKE_DROPITEM_INFO_LIST;

class CEventTreasureBoxMgr  
{
private:
	CEventTreasureBoxMgr();
	static bool m_bEvent;
	static CEventTreasureBoxMgr* m_pClass;
	VT_MAKE_DROPITEM_INFO_LIST	m_vtMake_DropItem_Info_List;
public:	
	void CreateRareItemRandomPosition(const int nItemNumber, const int nCreateCount,const int nGradeMin,const int nGradeMax,const int Xpos,const int Ypos, const int nRange);
	void CreateItemRandomPosition(const int nItemNumber,const int nDuration,const int  nCreateCount,const int Xpos,const int Ypos,const int  nRange);
	int CreatTreasure(const int nBoxItemId, int cn);
	bool IsTreasureBox(const int nItemKind);
	bool LoadTable(HDBC hDragonDB);
	static CEventTreasureBoxMgr* GetClass(){return m_pClass;}

	static enum
	{
		ITEM_TREASURE_BOX_KIND = 146//보물 상자이다.//KIND로 체크한다.
	};

	static void Create()
	{
		if(m_bEvent && (NULL == m_pClass)) //중복 생성은 되어선 안된다.
		{
			m_pClass = new CEventTreasureBoxMgr;
		}
	}

	static void Destory()
	{
		if(m_pClass)
		{
			delete m_pClass;
			m_pClass = NULL;
		}
	}	
	
	virtual ~CEventTreasureBoxMgr();
};

inline CEventTreasureBoxMgr* TreasureBoxMgr(){return CEventTreasureBoxMgr::GetClass();}

#endif // !defined(AFX_EVENTTREASUREBOXMGR_H__B5661CEB_1F05_4766_89B5_FFB98C5D1F85__INCLUDED_)
