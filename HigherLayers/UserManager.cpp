// UserManager.cpp: implementation of the CUserManager class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "UserManager.h"

#include "Scrp_exe.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CUserManager::CUserManager()
{
	m_hash.clear();
}

CUserManager::~CUserManager()
{
	m_hash.clear();

	while (!m_queue.empty())
	{
		m_queue.pop();
	}
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CUserManager::AddMember(WORD idUser)
{
	CHARLIST* pUser = ::CheckServerId(idUser);

	if (pUser == NULL)
	{
		MyLog(LOG_NORMAL, "CUserManager::AddNew : Character is NULL" );
		return;
	}

	ITOR_USER itor = m_hash.find(pUser->Name);

	if (itor != m_hash.end())
	{
		MyLog(LOG_NORMAL, "CUserManager::AddNew : Duplicate %s", pUser->Name);
		return;
	}

	m_hash.insert(PAIR_USER(pUser->Name, idUser));
}

void CUserManager::DelMember(WORD idUser)
{
	CHARLIST* pUser = ::CheckServerId(idUser);

	if (pUser == NULL)
	{
		MyLog(LOG_NORMAL, "CUserManager::Delete : Character is NULL");
		return;
	}

	ITOR_USER itor = m_hash.find(pUser->Name);

	if (itor != m_hash.end())
	{
		m_hash.erase(itor);
	}
	else
	{
		MyLog(LOG_NORMAL, "CUserManager::Delete : Don't Exist %s", pUser->Name);
		return;
	}
}

void CUserManager::Push(MAP_MOVE_INFO infMapMove)
{
	m_queue.push(infMapMove);
}

void CUserManager::Execute()
{
	if (m_queue.empty())
	{
		return;
	}

	MAP_MOVE_INFO infMapMove = m_queue.front();
	::GotoMap(infMapMove.idUser, infMapMove.nMapPort, infMapMove.nX, infMapMove.nY);
	m_queue.pop();
}

void CUserManager::SendPacket(t_packet* pPacket)
{	//< CSD-031213
	for (ITOR_USER i = m_hash.begin(); i != m_hash.end(); ++i)
	{
		const int idUser = i->second;
	
		if (connections[idUser].dwAgentConnectionIndex == 0)
		{
			continue;
		}

		if (connections[idUser].state < CONNECT_JOIN)
		{
			continue;
		}

		::QueuePacket(connections, idUser, pPacket, 1);
	}
}	//> CSD-031213

CHARLIST* CUserManager::GetCharList(const char* pName)
{	//< CSD-031213
	ITOR_USER itFind = m_hash.find(pName);

	if (itFind != m_hash.end())
	{
		return ::CheckServerId(itFind->second);
	}

	return NULL;
}	//> CSD-031213

int CUserManager::GetServerID(const char* pName)
{	//< CSD-031213
	ITOR_USER itFind = m_hash.find(pName);

	if (itFind != m_hash.end())
	{
		return itFind->second;
	}

	return -1;
}	//> CSD-031213