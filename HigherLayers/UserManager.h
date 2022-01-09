// UserManager.h: interface for the CUserManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERMANAGER_H__7F7661A1_937C_4887_8BD6_FB3F1A3391B4__INCLUDED_)
#define AFX_USERMANAGER_H__7F7661A1_937C_4887_8BD6_FB3F1A3391B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
	Added by Tya : case insensitive string.
  **/
struct ignorecase_traits : public std::char_traits<char>
{
	// return whether c1 and c2 are equal
	static bool eq(const char& c1, const char& c2)
	{
		return toupper(c1)==toupper(c2);
	}
	// return whether c1 is less than c2
	static bool lt(const char& c1, const char& c2)
	{
		return toupper(c1)<toupper(c2);
	}
	// compare up to n characters of s1 and s2
	static int compare(const char* s1, const char* s2,size_t n)
	{
		for (int i=0; i<n; ++i)
		{
			if (!eq(s1[i],s2[i]))
				return lt(s1[i],s2[i])?-1:1;
		}
		return 0;
	}
	// search c in s
	static const char* find(const char* s, size_t n,const char& c)
	{
		for (int i=0; i<n; ++i)
		{
			if (eq(s[i],c))
				return &(s[i]);
		}
		return 0;
	}
};

typedef std::basic_string<char,ignorecase_traits> icstring;
inline std::ostream& operator << (std::ostream& strm, const icstring& s)
{
    return strm << std::string(s.data(),s.length());
}


///////////////////////////////////////////////////////////////////////////////
// 立加磊 包府 努贰胶
class CUserManager : public TSingleton<CUserManager>
{
public:
	typedef map<icstring, WORD> HASH_USER;
	typedef HASH_USER::iterator ITOR_USER;
	typedef HASH_USER::value_type PAIR_USER;
	typedef queue<MAP_MOVE_INFO> QUEUE_MAP;

public:
	CUserManager();
	virtual ~CUserManager();

public:
	void AddMember(WORD idUser);
	void DelMember(WORD idUser);

	void Push(MAP_MOVE_INFO infMapMove);
	void Execute();

	void SendPacket(t_packet* pPacket);

	CHARLIST* GetCharList(const char* pName);
	int GetServerID(const char* pName);

public:
	HASH_USER& GetUserSet()
	{
		return m_hash;
	}

	int GetSize() const 
	{ 
		return m_hash.size(); 
	}

	bool IsExist(const char* pName) const
	{
		return (m_hash.find(pName) != m_hash.end()) ? true:false;
	}

private:
	HASH_USER m_hash;
	QUEUE_MAP m_queue;
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_USERMANAGER_H__7F7661A1_937C_4887_8BD6_FB3F1A3391B4__INCLUDED_)