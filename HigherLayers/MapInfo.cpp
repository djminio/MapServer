// MapInfo.cpp: implementation of the CMapInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdAfx.h"
#include "MapInfo.h"

t_MapInfo MapInfo[MAX_MAP_];

t_MapInfo* GetMapInfoByMapName(const char *szMapName)
{	//< CSD-030506
	for (int i = 0;  i< MAX_MAP_; ++i)
	{
		if (!_stricmp(MapInfo[i].mapfile, szMapName))
		{
			return &MapInfo[i];
		}
	}

	return NULL;
}	//> CSD-030506

t_MapInfo* GetMapInfoByMapPort(int nPort)
{	//< CSD-030506
	for (int i = 0;  i< MAX_MAP_; ++i)
	{
		if (MapInfo[i].port == nPort)
		{
			return &MapInfo[i];
		}
	}

	return NULL;
}	//> CSD-030506