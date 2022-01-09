// GuildManager.h: interface for the CGuildManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDMANAGER_H__74D06DB6_566C_4A3C_911A_66886B1C9E24__INCLUDED_)
#define AFX_GUILDMANAGER_H__74D06DB6_566C_4A3C_911A_66886B1C9E24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////
// 길드정보에 관련된 클래스
class CGuildInfo
{
	enum
	{
		MAX_GUILD_NAME = 20,
		MAX_GUILD_DEGREE = 5,
		MAX_GUILD_EXPLANIN = 1024
	};

	typedef list<int>             LIST_MEMBER;
	typedef LIST_MEMBER::iterator ITOR_MEMBER;

public:
	static CGuildInfo* CreateGuildInfo()
	{
		return new CGuildInfo;
	}

private:
	CGuildInfo();

public:
	~CGuildInfo();

public :
	BYTE on;				// 등록된 길드 인지 확인
	BYTE first;			// 예비 길드 인가?
	BYTE active;			// 사용하고 있는 길드인지의 여부
	BYTE make;	
	BYTE mark_num;	// 바뀐 이미지 값
	BYTE info_num;
	BYTE nation;

	char guild_name[MAX_GUILD_NAME];
	char guild_degree[MAX_GUILD_DEGREE][MAX_GUILD_NAME];
	char guild_explain[MAX_GUILD_EXPLANIN];

public:
	void AddNew(DWORD idMember);
	void Delete(DWORD idMember);

	void SendPacket(t_packet* pPacket);

	bool IsExist(DWORD idMember) const;

private:
	LIST_MEMBER m_ltMember;
};
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
class CGuildManager : public TSingleton<CGuildManager>
{
public:
	typedef map<short, CGuildInfo*> HASH_GUILD;
	typedef HASH_GUILD::iterator    ITOR_GUILD;
	typedef HASH_GUILD::value_type  PAIR_GUILD;	

	CGuildManager();
	virtual ~CGuildManager();

public:
	bool AddGuild(int nCode, CGuildInfo* pGuild);
	bool DelGuild(int nCode);
	
	bool AddMember(int nCode, DWORD idMember);
	bool DelMember(int nCode, DWORD idMember);

	bool SendPacket(int nCode, t_packet* pPacket);

public:
	HASH_GUILD& GetGuildSet()
	{
		return m_hash;
	}

	CGuildInfo* GetGuildInfo(int nCode)
	{
		if (m_hash.find(nCode) != m_hash.end())
		{
			return m_hash[nCode];
		}

		return NULL;
	}

	bool IsExist(int nCode) const
	{
		return (m_hash.find(nCode) != m_hash.end()) ? true:false;
	}

private:
	HASH_GUILD m_hash;
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_GUILDMANAGER_H__74D06DB6_566C_4A3C_911A_66886B1C9E24__INCLUDED_)
