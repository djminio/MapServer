// ResistState.cpp: implementation of the CResistState class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "ResistState.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CResistState::CResistState()
{
	::memset(m_naBasicResist,0,sizeof(m_naBasicResist));
	::memset(m_naExtentionResist,0,sizeof(m_naExtentionResist));
}

///////////////////////////////////////////////////////////////////////////////
// Public 
///////////////////////////////////////////////////////////////////////////////
void CResistState::IncBasicResist(const eRESIST_TYPE RTType , short nVolume)
{
	short &nrResist = m_naBasicResist[RTType];
	nrResist += nVolume;
	if (nrResist > MAX){ nrResist = MAX; }
}

void CResistState::DecBasicResist(const eRESIST_TYPE RTType , short nVolume)
{
	short &nrResist = m_naBasicResist[RTType];
	nrResist -= nVolume;
	if (nrResist < MIN) { nrResist = MIN; }
}

void CResistState::SetBasicResist(const eRESIST_TYPE RTType , short nVolume)
{ 
	short &nrResist = m_naBasicResist[RTType];
	nrResist = nVolume; 
	if (nrResist > MAX){ nrResist = MAX; }
	if (nrResist < MIN) { nrResist = MIN; }
}

void CResistState::IncExtResist(const eRESIST_TYPE RTType , short nVolume)
{
	short &nrResist = m_naExtentionResist[RTType];
	nrResist += nVolume;
	if (nrResist > MAX){ nrResist = MAX; }
}

void CResistState::DecExtResist(const eRESIST_TYPE RTType , short nVolume)
{
	short &nrResist = m_naExtentionResist[RTType];
	nrResist -= nVolume;
	if (nrResist < MIN) { nrResist = MIN; }
}

void CResistState::SetExtResist(const eRESIST_TYPE RTType , short nVolume)
{ 
	short &nrResist = m_naExtentionResist[RTType];
	nrResist = nVolume; 
	if (nrResist > MAX){ nrResist = MAX; }
	if (nrResist < MIN) { nrResist = MIN; }
}

short CResistState::GetExtResist(const eRESIST_TYPE RTType)const
{
	return m_naExtentionResist[RTType];
}
