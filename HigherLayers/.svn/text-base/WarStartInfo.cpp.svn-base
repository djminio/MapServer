// WarStartInfo.cpp: implementation of the CWarStartInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "WarStartInfo.h"
#include "UserManager.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CWarStartInfo::CWarStartInfo()
{
	m_vtTime.clear();
	m_dwRemainTime = 0;
	m_nIndex = -1;
	m_bActive = false;
}

CWarStartInfo::~CWarStartInfo()
{
	for_each(m_vtTime.begin(), m_vtTime.end(), TDelete());
	m_vtTime.clear();
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CWarStartInfo::Register(START_UP_INFO* pTime)
{
	m_vtTime.push_back(pTime);
}

void CWarStartInfo::Execute()
{
	if (m_bActive)
	{
		++m_dwRemainTime;

		if (IsCheckOut(m_nIndex))
		{
			m_bActive = false;
		}
	}
	else
	{
		for (int i = 0; i < m_vtTime.size(); ++i)
		{
			if (IsCheckIn(i))
			{
				m_nIndex = i;
				m_bActive = true;
				return;
			}
		}

		m_dwRemainTime = 0;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Protected Method
///////////////////////////////////////////////////////////////////////////////

bool CWarStartInfo::IsCheckIn(int nIndex) const
{
	if (g_wday != m_vtTime[nIndex]->nApplyDay)
	{
		return false;
	}
	
	if (g_hour != m_vtTime[nIndex]->nStartHour)
	{
		int HourDifference = m_vtTime[nIndex]->nStartHour - g_hour;
		if (HourDifference >0 && HourDifference <= 5 && g_min == 0)
		{
			// finito 050707 send amount of time until local war begin to servers and then clients
			t_packet packet;
			packet.h.header.type=CMD_LOCALWAR_BEGIN_TIME;
			packet.u.LocalWar.LocalWarBeginTime.nHour = HourDifference;
			packet.h.header.size=sizeof(t_LocalWarBeginTime);
			g_pUserManager->SendPacket(&packet); // send to clients of this map
			SendPacket2Maps(&packet); // send packet to other maps which will then send to the clients
			
		}
		return false;
	}
	
	if (g_min != m_vtTime[nIndex]->nStartMinute)
	{
		return false;
	}

	return true;
}

bool CWarStartInfo::IsCheckOut(int nIndex) const
{
	if (g_hour != m_vtTime[nIndex]->nEndHour)
	{
		return false;
	}
	
	if (g_min != m_vtTime[nIndex]->nEndMinute)
	{
		return false;
	}

	return true;
}