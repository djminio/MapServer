// GuildManager.cpp: implementation of the CGuildManager class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "GuildManager.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CGuildInfo::CGuildInfo()
{ 
	on = 0;
	first = 0;
	active = 0;
	make = 0;
	mark_num = 0;
	info_num = 0;
	nation = 0;
	memset(guild_name, 0, MAX_GUILD_NAME);
	memset(guild_degree, 0, MAX_GUILD_DEGREE*MAX_GUILD_NAME);
	memset(guild_explain, 0, MAX_GUILD_EXPLANIN);

	m_ltMember.clear();
}

CGuildInfo::~CGuildInfo()
{
	m_ltMember.clear();
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CGuildInfo::AddNew(DWORD idMember)
{
	m_ltMember.push_back(idMember);
}
	
void CGuildInfo::Delete(DWORD idMember)
{
	m_ltMember.remove(idMember);
}

void CGuildInfo::SendPacket(t_packet* pPacket)
{
	for (ITOR_MEMBER i = m_ltMember.begin(); i != m_ltMember.end(); ++i)
	{
		if(::CheckServerId((*i)))
		{
			::QueuePacket(connections, (*i), pPacket, 1);
		}
	}
}

bool CGuildInfo::IsExist(DWORD idMember) const
{
	if (find(m_ltMember.begin(), m_ltMember.end(), idMember) != m_ltMember.end())
	{
		return true;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CGuildManager::CGuildManager()
{
	m_hash.clear();
}

CGuildManager::~CGuildManager()
{
	for (ITOR_GUILD i = m_hash.begin(); i != m_hash.end(); ++i)
	{
		delete i->second;	
	}

	m_hash.clear();
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

bool CGuildManager::AddGuild(int nCode, CGuildInfo* pGuild)
{
	ITOR_GUILD itor = m_hash.find(nCode);

	if (itor != m_hash.end())
	{
		MyLog(LOG_NORMAL, "CGuildManager::AddGuild : exist guild code");
		MyLog(LOG_NORMAL, ">>> guild code : %d", nCode);
		return false;
	}

	m_hash.insert(PAIR_GUILD(nCode, pGuild));
	return true;
}

bool CGuildManager::DelGuild(int nCode)
{
	ITOR_GUILD itor = m_hash.find(nCode);

	if (itor != m_hash.end())
	{
		CGuildInfo* pTemp = itor->second;
		// 
		m_hash.erase(itor);

		if (pTemp != NULL)
		{
			delete pTemp;
		}

		return true;
	}

	MyLog(LOG_NORMAL, "CGuildManager::DelGuild : don't exist guild code");
	MyLog(LOG_NORMAL, ">>> guild code : %d", nCode);
	return false;
}
	
bool CGuildManager::AddMember(int nCode, DWORD idMember)
{
	if (!IsExist(nCode))
	{
		MyLog(LOG_NORMAL, "CGuildManager::AddMember : don't exist guild code");
		MyLog(LOG_NORMAL, ">>> guild code : %d", nCode);
		return false;
	}
		
	if (m_hash[nCode]->IsExist(idMember))
	{
		MyLog(LOG_NORMAL, "CGuildManager::AddMember : exist user_id");
		MyLog(LOG_NORMAL, ">>> guild code : %d, user_id : %d", nCode, idMember);
		return false;
	}

	m_hash[nCode]->AddNew(idMember);
	return true;
}

bool CGuildManager::DelMember(int nCode, DWORD idMember)
{
	if (!IsExist(nCode))
	{
		MyLog(LOG_NORMAL, "CGuildManager::DelMember : don't exist guild code");
		MyLog(LOG_NORMAL, ">>> guild code : %d", nCode);
		return false;
	}
		
	if (!m_hash[nCode]->IsExist(idMember))
	{
		return false;
	}

	m_hash[nCode]->Delete(idMember);
	return true;
}

bool CGuildManager::SendPacket(int nCode, t_packet* pPacket)
{
	if (!IsExist(nCode))
	{
		MyLog(LOG_NORMAL, "CGuildManager::SendPacket : don't exist guild code");
		MyLog(LOG_NORMAL, ">>> guild code : %d", nCode);
		return false;
	}

	m_hash[nCode]->SendPacket(pPacket);	
	return true;
}