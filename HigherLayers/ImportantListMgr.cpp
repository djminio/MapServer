// ImportantListMgr.cpp: implementation of the CImportantListMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "../stdafx.h"
#include "ImportantListMgr.h"
#include "CItem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CImportantListMgr *g_pILMgr = NULL;
CImportantListMgr::CImportantListMgr()
{
	Clear();
}

CImportantListMgr::~CImportantListMgr()
{
	Clear();
}

void CImportantListMgr::Clear()
{
	m_mapItemList.clear();
} 

bool CImportantListMgr::Init(const eInitType InitType, const char *filename)
{
	FILE *fp = ::fopen( filename,"rt" );
	if( !fp ) 
	{
		::MyLog(0,"CImportantListMgr::Init Not Exist File '%s'",filename);
		return 0;
	}

	char temp[255] ={0,};
	while(::fgets( temp, 255, fp ))
	{
		char *token = ::strtok(temp,"\t\n\r");	
		if( token == NULL ){continue;}

		const int iItemNo = ::atoi( token );
		
		if( !iItemNo ){continue;}

		CItem *t = ::ItemUnit( iItemNo );
		if( !t ) 
		{
			return 0;
		}
		
		int iLimitNo = 0;
		
		switch(InitType)
		{
		case IL_INIT_ITEM_NO_WITH_LIMIT:
			{
				token = ::strtok(NULL,"\t\n\r");//if With Limit
				if( token == NULL ){continue;}
				iLimitNo = ::atoi( token );

				m_mapItemList.insert(HASH::value_type(iItemNo, iLimitNo));
			}break;
		case IL_INIT_JUST_ITEM_NO:
		default:
			{
				m_mapItemList.insert(HASH::value_type(iItemNo, iItemNo));
			}break;
		}
	}
	::fclose( fp );
	return 1;
}