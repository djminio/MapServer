// UserManager.h: interface for the CUserManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef COUNSELOR_MANAGER_H
#define COUNSELOR_MANAGER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
	Added by Tya : case insensitive string.
  **/
struct counselor_ignorecase_traits : public std::char_traits<char>
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

typedef std::basic_string<char,counselor_ignorecase_traits> cicstring;
inline std::ostream& operator << (std::ostream& strm, const cicstring& s)
{
    return strm << std::string(s.data(),s.length());
}

///////////////////////////////////////////////////////////////////////////////
// 立加磊 包府 努贰胶
class CCounselorManager : public TSingleton<CCounselorManager>
{
public:
	typedef map<cicstring, WORD> HASH_COUNSELOR;
	typedef HASH_COUNSELOR::iterator ITOR_COUNSELOR;
	typedef HASH_COUNSELOR::value_type PAIR_COUNSELOR;

public:
	CCounselorManager();
	virtual ~CCounselorManager();

public:
	void AddMember(WORD idUser);
	void DelMember(WORD idUser);

	void SendPacket(t_packet* pPacket);

public:
	HASH_COUNSELOR& GetCounselorSet()
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
	HASH_COUNSELOR m_hash;
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // COUNSELOR_MANAGER_H