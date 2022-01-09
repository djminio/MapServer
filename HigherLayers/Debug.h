extern void DebugInfo(HRESULT Ret, char *Str, int Line, int Mode);

#define _DEBUG_DD(exp)    { { HRESULT rval = exp; if (rval != DD_OK) { DebugInfo(rval, __FILE__, __LINE__, 0); return FALSE; } } }
#define _DEBUG_DS(exp)    { { HRESULT rval = exp; if (rval != DS_OK) { DebugInfo(rval, __FILE__, __LINE__, 1); return FALSE; } } }



	extern int debug_SavePacketExeTimeIng( void );
	extern void debug_SavePacketExeTime( void );
	extern void debug_StopSavePacketExeTime( void );
	extern void SavePacketStatus( void );




