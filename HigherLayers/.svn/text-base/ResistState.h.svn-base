// ResistState.h: interface for the CResistState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESISTSTATE_H__65B83583_1513_45FC_BFED_15B04DDE37F7__INCLUDED_)
#define AFX_RESISTSTATE_H__65B83583_1513_45FC_BFED_15B04DDE37F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////
// 저항 상태에 대한 클래스
class CResistState
{
  enum {MIN = 0, MAX = 500};

public:
	CResistState();

public:
	void IncBasicResist(const eRESIST_TYPE RTType, short nVolume);
	void DecBasicResist(const eRESIST_TYPE RTType, short nVolume);
	void SetBasicResist(const eRESIST_TYPE RTType, short nVolume);
	short GetBasicResist(const eRESIST_TYPE RTType)const { return m_naBasicResist[RTType]; }

	void IncExtResist(const eRESIST_TYPE RTType, short nVolume);
	void DecExtResist(const eRESIST_TYPE RTType, short nVolume);
	void SetExtResist(const eRESIST_TYPE RTType, short nVolume);
	short GetExtResist(const eRESIST_TYPE RTType)const;

private:
	short m_naBasicResist[MAX_RESIST_TYPE];//DB에 기록 되는 베이스 마법 방어력 값
	short m_naExtentionResist[MAX_RESIST_TYPE];//정적으로 아이템에 의해서 올라간 마법방어력 값(저장하지 않는 값)
};

//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_RESISTSTATE_H__65B83583_1513_45FC_BFED_15B04DDE37F7__INCLUDED_)
