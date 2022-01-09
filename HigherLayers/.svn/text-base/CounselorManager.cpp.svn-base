// CounselorManager.cpp: implementation of the CCounselorManager class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "CounselorManager.h"

#include "Scrp_exe.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CCounselorManager::CCounselorManager()
{
	m_hash.clear();
}

CCounselorManager::~CCounselorManager()
{
	m_hash.clear();
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CCounselorManager::AddMember(WORD idUser)
{
	CHARLIST* pUser = ::CheckServerId(idUser);

	if (pUser == NULL)
	{
		MyLog(LOG_NORMAL, "CCounselorManager::AddNew : Character is NULL" );
		return;
	}

	ITOR_COUNSELOR itor = m_hash.find(pUser->Name);

	if (itor != m_hash.end())
	{
		MyLog(LOG_NORMAL, "CCounselorManager::AddNew : Duplicate %s", pUser->Name);
		return;
	}

	m_hash.insert(PAIR_COUNSELOR(pUser->Name, idUser));
}

void CCounselorManager::DelMember(WORD idUser)
{
	CHARLIST* pUser = ::CheckServerId(idUser);

	if (pUser == NULL)
	{
		MyLog(LOG_NORMAL, "CCounselorManager::Delete : Character is NULL");
		return;
	}

	ITOR_COUNSELOR itor = m_hash.find(pUser->Name);

	if (itor != m_hash.end())
	{
		m_hash.erase(itor);
	}
	else
	{
		MyLog(LOG_NORMAL, "CCounselorManager::Delete : Don't Exist %s", pUser->Name);
		return;
	}
}

void CCounselorManager::SendPacket(t_packet* pPacket)
{	//< CSD-031213
	for (ITOR_COUNSELOR i = m_hash.begin(); i != m_hash.end(); ++i)
	{
		const int idCounselor = i->second;
	
		if (connections[idCounselor].dwAgentConnectionIndex == 0)
		{
			continue;
		}

		if (connections[idCounselor].state < CONNECT_JOIN)
		{
			continue;
		}

		::QueuePacket(connections, idCounselor, pPacket, 1);
	}
}	//> CSD-031213