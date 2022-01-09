#include "..\stdafx.h"
#include "DefaultHeader.h"
#include "..\HigherLayers\FileMgr.h"
#include "..\LowerLayers\mylog.h"

MAGICTABLE Magic_Ref[Num_Of_Magic];
short int nMagic;
extern HDBC hDBC;

//////////////////////////////////////////////////////////////////////////////
//
// 함수설명: 서버에서 참조할 마법테이블을 DB로부터 읽어온다.
//
// 파라미터: 없음.
//
// 리턴값: 성공(1), 실패(ret < 0)
//
//////////////////////////////////////////////////////////////////////////////

int InitEffectTbl()
{   //< CSD-020809
    char szQuerry[512];
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT * FROM effect");
	HSTMT	hStmt = NULL;
	SQLAllocStmt(hDBC, &hStmt);
	RETCODE	retCode = SQLExecDirect(hStmt, (UCHAR*)szQuerry, SQL_NTS);
	
	if (retCode != SQL_SUCCESS_WITH_INFO && retCode != SQL_SUCCESS) 
	{
		MyLog(LOG_NORMAL, "Effect : ExecDirect Error");
		SQLFreeStmt(hStmt, SQL_DROP);
		return -1;
	}
	
	SDWORD cbValue;
	TFileMgr<CEffectTbl, Num_Of_Effect> tblEffect;
	TFileMgr<CEffectTbl2, Num_Of_Effect> tblEffect2;
	
	while ((retCode = SQLFetch(hStmt)) == SQL_SUCCESS)
	{
		int temp = 0;
		SQLGetData(hStmt,  1, SQL_C_LONG, &temp, 0, &cbValue);
		CEffectTbl* pEffectTbl = tblEffect.GetBuffer(temp);
		CEffectTbl2* pEffectTbl2 = tblEffect2.GetBuffer(temp);
		pEffectTbl->effect_Num	= temp; temp = 0;
		SQLGetData(hStmt,  2, SQL_C_LONG, &temp, 0, &cbValue);pEffectTbl->pattern_Num	= temp; temp = 0;
		SQLGetData(hStmt,  3, SQL_C_LONG, &temp, 0, &cbValue);pEffectTbl->sprite_Num	= temp; temp = 0;
		SQLGetData(hStmt,  4, SQL_C_LONG, &temp, 0, &cbValue);pEffectTbl->static_Effect	= temp; temp = 0;
		SQLGetData(hStmt,  5, SQL_C_CHAR, &pEffectTbl2->sprite_Name, 30,&cbValue);
		SQLGetData(hStmt,  6, SQL_C_LONG, &temp, 0, &cbValue);pEffectTbl->sound_Num		= temp; temp = 0;
		SQLGetData(hStmt,  7, SQL_C_LONG, &temp, 0, &cbValue);pEffectTbl->sound_Volume	= temp; temp = 0;
		SQLGetData(hStmt,  8, SQL_C_LONG, &temp, 0, &cbValue);pEffectTbl->next_Effect	= temp; temp = 0;
		SQLGetData(hStmt,  9, SQL_C_LONG, &temp, 0, &cbValue);pEffectTbl->link_Frame	= temp; temp = 0;
		SQLGetData(hStmt, 10, SQL_C_LONG, &temp, 0, &cbValue);pEffectTbl->trans_Type	= temp; temp = 0;
		SQLGetData(hStmt, 11, SQL_C_LONG, &temp, 0, &cbValue);pEffectTbl->trans_Level	= temp; temp = 0;
		SQLGetData(hStmt, 12, SQL_C_LONG, &temp, 0, &cbValue);pEffectTbl->sort_Type		= temp; temp = 0;
		SQLGetData(hStmt, 13, SQL_C_LONG, &temp, 0, &cbValue);pEffectTbl->spr_Speed		= temp; temp = 0;
		SQLGetData(hStmt, 14, SQL_C_LONG, &temp, 0, &cbValue);pEffectTbl->spr_Height	= temp; temp = 0;
		SQLGetData(hStmt, 15, SQL_C_LONG, &temp, 0, &cbValue);pEffectTbl->object_Num	= temp; temp = 0;
		SQLGetData(hStmt, 16, SQL_C_LONG, &temp, 0, &cbValue);pEffectTbl->fallow		= temp; temp = 0;
		SQLGetData(hStmt, 17, SQL_C_LONG, &temp, 0, &cbValue);pEffectTbl->light			= temp; temp = 0;
		SQLGetData(hStmt, 18, SQL_C_LONG, &temp, 0, &cbValue);pEffectTbl->end_Time		= temp; temp = 0;
		SQLGetData(hStmt, 19, SQL_C_LONG, &temp, 0, &cbValue);pEffectTbl->impect_Effect_Num = temp; temp = 0;
		SQLGetData(hStmt, 20, SQL_C_LONG, &temp, 0, &cbValue);pEffectTbl->send_result		= temp; temp = 0;
		EatRearWhiteChar(pEffectTbl2->sprite_Name);
		pEffectTbl->sound_loop = pEffectTbl->sound_Num/10000;
		pEffectTbl->sound_Num = pEffectTbl->sound_Num%10000;
		pEffectTbl->sort_Type += 9;        
		tblEffect.AddNew(pEffectTbl->effect_Num);
		tblEffect2.AddNew(pEffectTbl->effect_Num);
	}
	
	SQLFreeStmt(hStmt, SQL_DROP);
	
	if (!tblEffect.Save("./output/Thunder.dat"))
	{
		return -100;
	}
	
	if (!tblEffect2.Save("./output/Lightning.dat"))
	{
		return -100;
	}
	
	return 1;
}   //> CSD-020809

int InitMagicTbl()
{
	char szQuerry[512];
	sprintf_s(szQuerry, sizeof(szQuerry), "SELECT * FROM magic");
	HSTMT	hStmt = NULL;
	SQLAllocStmt(hDBC, &hStmt);
	RETCODE	retCode = SQLExecDirect(hStmt, (UCHAR*)szQuerry, SQL_NTS);
	
	if (retCode != SQL_SUCCESS_WITH_INFO && retCode != SQL_SUCCESS) 
	{
		MyLog(LOG_NORMAL, "Magic : ExecDirect Error");
		SQLFreeStmt(hStmt, SQL_DROP);
		return -1;
	}
	
	SDWORD cbValue;
	MAGICTABLE tblTemp;
	TFileMgr<CMagicTbl, Num_Of_Magic> tblMagic;
	
	while ((retCode = SQLFetch(hStmt)) == SQL_SUCCESS)
	{
		memset(&tblTemp, 0, sizeof(MAGICTABLE));
		SQLGetData(hStmt,  1, SQL_C_LONG, &tblTemp.order_Type, 0, &cbValue);       
		SQLGetData(hStmt,  2, SQL_C_CHAR, &tblTemp.Han_Name, 30, &cbValue);
		SQLGetData(hStmt,  4, SQL_C_LONG, &tblTemp.serial_Num, 0, &cbValue);       
		SQLGetData(hStmt,  5, SQL_C_LONG, &tblTemp.basic_Level, 0, &cbValue);      
		SQLGetData(hStmt,  6, SQL_C_LONG, &tblTemp.point_MS, 0, &cbValue);         
		SQLGetData(hStmt,  7, SQL_C_LONG, &tblTemp.before_Magic, 0, &cbValue);     
		SQLGetData(hStmt,  8, SQL_C_LONG, &tblTemp.basic_magery, 0, &cbValue);     
		SQLGetData(hStmt,  9, SQL_C_LONG, &tblTemp.cast_Time, 0, &cbValue);        
		SQLGetData(hStmt, 10, SQL_C_LONG, &tblTemp.cast_Continue, 0, &cbValue);    
		SQLGetData(hStmt, 11, SQL_C_LONG, &tblTemp.continue_Time, 0, &cbValue);    
		SQLGetData(hStmt, 12, SQL_C_LONG, &tblTemp.avail_Type, 0, &cbValue);       
		SQLGetData(hStmt, 13, SQL_C_LONG, &tblTemp.avail_Range, 0, &cbValue);      
		SQLGetData(hStmt, 14, SQL_C_LONG, &tblTemp.exhaust_MP, 0, &cbValue);       
		SQLGetData(hStmt, 15, SQL_C_LONG, &tblTemp.exhaust_MS, 0, &cbValue);       
		SQLGetData(hStmt, 16, SQL_C_LONG, &tblTemp.require_IT, 0, &cbValue);       
		SQLGetData(hStmt, 17, SQL_C_LONG, &tblTemp.require_WP, 0, &cbValue);      
		SQLGetData(hStmt, 18, SQL_C_LONG, &tblTemp.nHDPoint, 0, &cbValue); // CSD-030314
		SQLGetData(hStmt, 19, SQL_C_LONG, &tblTemp.nHDLimit, 0, &cbValue); // CSD-030314
		SQLGetData(hStmt, 20, SQL_C_LONG, &tblTemp.point_WS, 0, &cbValue);         
		SQLGetData(hStmt, 22, SQL_C_LONG, &tblTemp.nDualStep, 0, &cbValue); // 030415 kyo 
		SQLGetData(hStmt, 26, SQL_C_LONG, &tblTemp.invalid_TargetIT, 0, &cbValue); 
		SQLGetData(hStmt, 31, SQL_C_LONG, &tblTemp.nClass, 0, &cbValue);
		SQLGetData(hStmt, 32, SQL_C_LONG, &tblTemp.nTactics, 0, &cbValue);
		SQLGetData(hStmt, 33, SQL_C_LONG, &tblTemp.nAddition, 0, &cbValue);
		SQLGetData(hStmt, 34, SQL_C_LONG, &tblTemp.aLevel[1], 0, &cbValue); 
		SQLGetData(hStmt, 35, SQL_C_LONG, &tblTemp.aRandom[1], 0, &cbValue); 
		SQLGetData(hStmt, 36, SQL_C_LONG, &tblTemp.aLevel[2], 0, &cbValue); 
		SQLGetData(hStmt, 37, SQL_C_LONG, &tblTemp.aRandom[2], 0, &cbValue); 
		SQLGetData(hStmt, 38, SQL_C_LONG, &tblTemp.aLevel[3], 0, &cbValue); 
		SQLGetData(hStmt, 39, SQL_C_LONG, &tblTemp.aRandom[3], 0, &cbValue); 
		SQLGetData(hStmt, 46, SQL_C_LONG, &tblTemp.Spell_Type, 0, &cbValue);       
		SQLGetData(hStmt, 47, SQL_C_LONG, &tblTemp.Appoint_Type, 0, &cbValue);     
		SQLGetData(hStmt, 48, SQL_C_LONG, &tblTemp.Excute_Type,	0, &cbValue);      
		SQLGetData(hStmt, 49, SQL_C_LONG, &tblTemp.magic_Type, 0, &cbValue);       
		SQLGetData(hStmt, 50, SQL_C_LONG, &tblTemp.Price, 0, &cbValue);            
		SQLGetData(hStmt, 51, SQL_C_LONG, &tblTemp.Exp, 0, &cbValue);              
		SQLGetData(hStmt, 52, SQL_C_LONG, &tblTemp.Learnable,	0, &cbValue);        
		SQLGetData(hStmt, 53, SQL_C_LONG, &tblTemp.nSummonMax, 0, &cbValue);   // CSD-030314
		SQLGetData(hStmt, 54, SQL_C_LONG, &tblTemp.nCombatCount, 0, &cbValue); // CSD-030314
		SQLGetData(hStmt, 55, SQL_C_LONG, &tblTemp.nResist,	0, &cbValue);          
		SQLGetData(hStmt, 56, SQL_C_LONG, &tblTemp.nRiseFall,	0, &cbValue);        
		Magic_Ref[tblTemp.serial_Num] = tblTemp;
		//
		CMagicTbl* pMagicTbl = tblMagic.GetBuffer(tblTemp.serial_Num);
		pMagicTbl->order_Type = tblTemp.order_Type;
		pMagicTbl->serial_Num = tblTemp.serial_Num;
		pMagicTbl->basic_Level = tblTemp.basic_Level;
		pMagicTbl->WS = tblTemp.point_MS;
		pMagicTbl->before_Magic = tblTemp.before_Magic;
		pMagicTbl->basic_Magery = tblTemp.basic_magery;
		pMagicTbl->cast_Time = tblTemp.cast_Time;
		pMagicTbl->cast_Continue = tblTemp.cast_Continue;
		pMagicTbl->continue_Time = tblTemp.continue_Time;
		pMagicTbl->avail_Type = tblTemp.avail_Type;
		pMagicTbl->avail_Range = tblTemp.avail_Range;
		pMagicTbl->exhaust_MP = tblTemp.exhaust_MP;
		pMagicTbl->exhaust_WS = tblTemp.exhaust_MS;	
		pMagicTbl->require_IT = tblTemp.require_IT;
		pMagicTbl->require_WP = tblTemp.require_WP;
		pMagicTbl->point_WS = tblTemp.point_WS;		
		pMagicTbl->nDualStep = tblTemp.nDualStep;		// 030520 kyo
		pMagicTbl->invalid_TargetIT = tblTemp.invalid_TargetIT;
		pMagicTbl->appoint_Type = tblTemp.Appoint_Type;
		pMagicTbl->spell_Type = tblTemp.Spell_Type;
		pMagicTbl->excute_Type = tblTemp.Excute_Type;
		pMagicTbl->magic_Type = tblTemp.magic_Type;
		pMagicTbl->Price = tblTemp.Price;
		pMagicTbl->Exp = tblTemp.Exp;
		pMagicTbl->Learnable = tblTemp.Learnable;
		pMagicTbl->nSummonMax = tblTemp.nSummonMax;       // CSD-030314
		pMagicTbl->nCombatCount = tblTemp.nCombatCount; // CSD-030314
		pMagicTbl->nResist = tblTemp.nResist;
		pMagicTbl->nRiseFall = tblTemp.nRiseFall;
		tblMagic.AddNew(pMagicTbl->serial_Num);
	}
	
	SQLFreeStmt(hStmt, SQL_DROP);
	return (tblMagic.Save("./output/Thunder.ab")) ? 1:-1;
}