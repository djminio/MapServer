#define __SCRIPT_C__

#include "..\stdafx.h"
#include "DefaultHeader.h"

#include "scrp_int.h"
#include "scrp_exe.h"
#include "Winzs.h"

int  CLoadScript;  //LOAD_SCRIPT;
int  TotalFunction;                           // √— «‘ºˆ¿« ∞πºˆ∏¶ ¿˙¿Â
static char *prog, *oldprog;                    // Ω∫≈©∏≥∆Æ µ•¿Ã≈∏∏¶ ¿˙¿Â«“ πˆ∆€
static int CALLSTACK[ 10 ];                     // callπÆ Ω««‡Ω√ ¡÷º“∏¶ ¿˙¿Â«“ πˆ∆€
static int CALLSP;                              // callπÆ¿ª «•Ω√«œ¥¬ Ω∫≈√ ∆˜¿Œ≈Õ∫Øºˆ
int func_count;
int func_off[_SCRIPT_FUNCTION_POINT_]; // CSD-030509


int  NScriptNo;



/*≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â
 «‘ºˆ∏Ì : Incode
 ¿Œ  ¿⁄ : s = æœ»£»≠«“ πÆ¿⁄ø≠
 ∏Æ≈œ∞™ : æ¯¿Ω
 º≥  ∏Ì : sπÆ¿⁄ø≠¿ª πﬁæ∆º≠ ≥ª∫Œø° ¡§¿«µ» ºˆƒ°øÕ ø¨ªÍ¿ª ºˆ«‡«ÿº≠ æœ»£»≠«‘
≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â */
void Incode( char *s )
{
    int i = 0;
    while( *s )
    {
        *s++ ^= 0xff;	if( ++i > 19 ) i = 0;
    }
}


/* ≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â
 «‘ºˆ∏Ì : GetByte
 ¿Œ  ¿⁄ : æ¯¿Ω
 ∏Æ≈œ∞™ : 1πŸ¿Ã∆Æ¿« ∞™
 º≥  ∏Ì : progø° ±‚∑œµ» ≥ªøÎø°º≠ 1πŸ¿Ã∆Æ∏¶ ∏Æ≈œ«œ∞Ì prog∆˜¿Œ≈Õ∏¶ 1¡ı∞°
≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â */
BYTE GetByte( void )
{
    return( *prog++ );
}


/*≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â
 «‘ºˆ∏Ì : GetInt
 ¿Œ  ¿⁄ : æ¯¿Ω
 ∏Æ≈œ∞™ : 4πŸ¿Ã∆Æ¿« ∞™
 º≥  ∏Ì : progø° ±‚∑œµ» ≥ªøÎø°º≠ 4πŸ¿Ã∆Æ∏¶ ∏Æ≈œ«œ∞Ì prog∆˜¿Œ≈Õ∏¶ 4¡ı∞°
≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â */
int GetInt( void )
{
    int i, j;
	
    i = GetByte();
    j = *((int *)prog);

    prog += sizeof( int );

    if( i == variable_ )
        j = var[EventPC][j];

    return( j );
}

static void ChangeName( char *buf, int size, char *oldname, char *newname )
{
	for( int i = 0 ; i < size - (int)strlen( oldname ) ; i ++)
	{
		if( strncmp( buf + i, oldname, strlen( oldname )) == NULL )
		{
			for( int j = 0 ; j < (int)strlen( oldname ) ; j ++)
			{
				*( buf+i + j ) = newname[j];
			}
		}
	}
}


/* ≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â
 «‘ºˆ∏Ì : GetString
 ¿Œ  ¿⁄ :
 ∏Æ≈œ∞™ : πÆ¿⁄ø≠¿ª ¿˙¿Â«œ∞Ì ¿÷¥¬ πËø≠¿« Ω√¿€ ∆˜¿Œ≈Õ
 º≥  ∏Ì : Ω∫≈©∏≥∆Æ µ•¿Ã≈∏ø°º≠ πÆ¿⁄ø≠¿ª ¿–æÓµÈ¿Œ¥Ÿ.
≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â */
//ºˆ¡§7
void GetString( char *buf )
{
    int  size;

    size = GetInt();
    memcpy( buf, prog, size );
    prog += size;
    buf[ size ] = 0;
    Incode( buf );
}

static char GetByte1( void )
{
     return( *prog++ );
}

static int GetInt1( void )
{
    int i;

    i = *((int *)prog);
    prog += sizeof( int );

    return( i );
}

/* ≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â
 «‘ºˆ∏Ì : If
 ¿Œ  ¿⁄ : æ¯¿Ω
 ∏Æ≈œ∞™ : æ¯¿Ω
 º≥  ∏Ì : ifπÆø° «ÿ¥Á«œ¥¬ ≥ªøÎ¿ª Ω««‡«—¥Ÿ.
≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â */
void If( void )
{
    int i1, i2;
    BYTE com;
    int check = 0;
	int fuck=0;

    GetByte1();                      // ∫Øºˆ ø¨ªÍ¿⁄∏¶ ¿–¿Ω
    i1 = GetInt1();                  // Lvalue¿« ∞™¿ª æÚ¥¬¥Ÿ
    i1 = var[EventPC][ i1 ];                // varπËø≠ø°º≠ «ÿ¥Á«œ¥¬ ∞™¿ª æÚ¥¬¥Ÿ

    com = GetByte1();                // ø¨ªÍ±‚»£∏¶ æÚ¥¬¥Ÿ

	fuck = GetByte1() & 0x000000ff;


    if( fuck == variable_ )    // Rvalue¿« ∞™¿Ã ∫Øºˆ∂Û∏È
    {
        i2 = GetInt1();
        i2 = var[EventPC][ i2 ];
    }
    else                            // Rvalue¿« ∞™¿Ã ºˆƒ°∂Û∏È
    {
        i2 = GetInt1();
    }

    switch( com )
    {
        case 0:
            if( i1 < i2 ) check = 1;    break;
        case 1:
            if( i1 <= i2 ) check = 1;   break;
        case 2:
            if( i1 > i2 ) check = 1;    break;
        case 3:
            if( i1 >= i2 ) check = 1;   break;
        case 4:
            if( i1 != i2 ) check = 1;   break;
        case 5:
            if( i1 == i2 ) check = 1;   break;
    }

    if( check == 1 )        // ¡∂∞«¿Ã ¬¸¿Ã∂Û∏È
        prog += ( sizeof( int ) + 1 );          // ¥Ÿ¿Ω¿« gotoπÆ¿ª Ω∫≈µ
}




/* ≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â
 «‘ºˆ∏Ì : Cal
 ¿Œ  ¿⁄ : æ¯¿Ω
 ∏Æ≈œ∞™ : æ¯¿Ω
 º≥  ∏Ì : ªÁƒ¢ø¨ªÍ, %, ∞£¥‹«— ∫Ò∆Æø¨ªÍµÓ¿ª ºˆ«‡«—¥Ÿ
≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â */
void Cal( int com )
{
    int i1, i2, i3, result;
    int *p2, *p3;
    BYTE op;

    GetByte1();              // ∫Øºˆ ø¨ªÍ¿⁄∏¶ æÚ¿Ω
    i1 = GetInt1();          // Lvalue∏¶ æÚ¿Ω

    op = GetByte1() & 0x000000ff;
    i2 = GetInt1();
    if( op == variable_ )   // ∫Øºˆ∂Û∏È
    {
        p2 = &(var[EventPC][ i2 ]);
    }
    else                    // ºˆƒ°∂Û∏È
    {
        p2 = &i2;
    }


    op = GetByte1() & 0x000000ff;
    i3 = GetInt1();
    if( op == variable_ )   // ∫Øºˆ∂Û∏È
    {
        p3 = &var[EventPC][ i3 ];
    }
    else                    // ºˆƒ°∂Û∏È
    {
        p3 = &i3;
    }

    switch( com )                        // ∞¢ ø¨ªÍ¿⁄ø° µ˚∂Û Ω««‡
    {
        case add_:  result = (*p2) + (*p3); break;
        case sub_:  result = (*p2) - (*p3); break;
        case mul_:  result = (*p2) * (*p3); break;
        case div_:  result = (*p2) / (*p3); break;
        case rest_: result = (*p2) % (*p3); break;
        case and_:  result = (*p2) & (*p3); break;
        case or_:   result = (*p2) | (*p3); break;
    }

    var[EventPC][i1] = result;                      // Ω««‡«— ∞·∞˙∏¶ πËø≠ø° ¿˙¿Â
}

void Cal2( int com )
{
    int i1, i2;
    int *p2;
    BYTE op;

    GetByte1();
    i1 = GetInt1();                          // Lvalue ∞™¿ª æÚ¿Ω

    op = GetByte1() & 0x000000ff;
    i2 = GetInt1();                          // Rvalue ∞™¿ª æÚ¿Ω
    if( op == variable_ )   // ∫Øºˆ∂Û∏È
    {
        p2 = &var[EventPC][ i2 ];
    }
    else                    // ºˆƒ°∂Û∏È
    {
        p2 = &i2;
    }

    switch( com )
    {
        case add2_: var[EventPC][ i1 ] += (*p2); break;
        case sub2_: var[EventPC][ i1 ] -= (*p2); break;
        case mul2_: var[EventPC][ i1 ] *= (*p2); break;
        case div2_: var[EventPC][ i1 ] /= (*p2); break;
        case rest2_: var[EventPC][ i1 ] %= (*p2); break;
        case or2_ : var[EventPC][ i1 ] |= (*p2); break;
        case and2_: var[EventPC][ i1 ] &= (*p2); break;
    }
}



/* ≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â
 «‘ºˆ∏Ì : Equal
 ¿Œ  ¿⁄ : æ¯¿Ω
 ∏Æ≈œ∞™ : æ¯¿Ω
 º≥  ∏Ì : ∫Øºˆ¿« ¥Î¿‘πÆ¿ª √≥∏Æ
≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â */
void Equal(void)
{
    int i1, i2;
    BYTE op;

    GetByte1();
    i1 = GetInt1();

    op = GetByte1();
    i2 = GetInt1();
    if( op == variable_ )
	{
        i2 = var[EventPC][i2];
	}
	
    var[EventPC][i1] = i2;
}



/* ≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â
 «‘ºˆ∏Ì : IfEqual
 ¿Œ  ¿⁄ : æ¯¿Ω
 ∏Æ≈œ∞™ : æ¯¿Ω
 º≥  ∏Ì : Ω∫¿ßƒ°πÆø° «ÿ¥Á«œ¥¬ ≥ªøÎ¿ª √≥∏Æ
≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â */
void IfEqual( void )
{
    int i1, i2;
    char op;

    GetByte1();
    i1 = GetInt1();                         // Lvalue ∞™¿ª ¿–¿Ω

    op = GetByte1();                        // Rvalue ∞™¿ª ¿–¿Ω
    i2 = GetInt1();

    if( op == value_ )
    {
        if( var[EventPC][i1] == i2 )                   // ∏∏æ‡ ¡∂∞«¿Ã ¬¸¿Ã∂Û∏È ∫–±‚
            prog = oldprog + *((int *)prog);
        else                                  // ∞≈¡˛¿Ã∏È ¥Ÿ¿ΩπÆ¿ª Ω««‡
            prog += sizeof( int );
    }
    else
    {
        if( var[EventPC][i1] == var[EventPC][i2] )                   // ∏∏æ‡ ¡∂∞«¿Ã ¬¸¿Ã∂Û∏È ∫–±‚
            prog = oldprog + *((int *)prog);
        else                                  // ∞≈¡˛¿Ã∏È ¥Ÿ¿ΩπÆ¿ª Ω««‡
            prog += sizeof( int );
    }
}



/* ≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â
 «‘ºˆ∏Ì : Script
 ¿Œ  ¿⁄ : æ¯¿Ω
 ∏Æ≈œ∞™ : æ¯¿Ω
 º≥  ∏Ì : Ω∫≈©∏≥∆Æ ¡÷ Ω««‡±‚
≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â */
void Script( int num )
{
  BYTE comm;
	int Fuck;
  if (CLoadScript == SCRIPT_OLD && num == 0)	goto RET_; // Ω««‡«—∞Õ¿ª ∂««“« ø‰¥¬ æ¯¡ˆø‰...
  if( num >= TotalFunction )  goto RET_;

  NScriptNo = num;
	CLoadScript = SCRIPT_OLD;

    prog = oldprog + func_off[ num ];
    while( 1 )
    {
        comm = GetByte1();

        switch( comm )
        {
            case goto_:
				Fuck = *(int *)prog;
				//prog += 4;
                prog = oldprog + Fuck; 
				break;

            case if_:
                If(); break;

            case call_:
                CALLSTACK[ CALLSP++ ] = prog - oldprog + sizeof( int );
                prog = oldprog + *((int *)prog);
                break;

            case add_:
            case sub_:
            case mul_:
            case div_:
            case rest_:
            case and_:
            case or_:
                Cal( comm ); break;

            case add2_:
            case sub2_:
            case mul2_:
            case div2_:
            case rest2_:
            case and2_:
            case or2_:
                Cal2( comm ); break;

            case equal_:
                Equal(); break;

            case end_:
                if( --CALLSP < 0 )
                {
                    CALLSP = 0;
                    return;
                }
                prog = oldprog + CALLSTACK[ CALLSP ];
                break;

            case ifequal_:
                IfEqual(); break;

            default:        // ªÁøÎ¿⁄ ¡§¿« «‘ºˆ∏¶ Ω««‡

				// 010531 KHS
				//011023 >
				if( comm >= 0 && comm <= MAX_SCRIPT_FUNCTION )			// 011213 LTS
				//011023 <
				{
	                (*SFunction[ comm ])();
				}
				else 
				{
					//_asm int 3	
					CALLSP = 0;	
					goto RET_;	
				}
        
        if (CLoadScript == SCRIPT_NEW)
        {
          CALLSP = 0;
          goto RET_;
        }
      }
    }

RET_:
	return;
}


/*
// Ω∫≈©∏≥∆Æ ≈ÿΩ∫∆Æ ∆ƒ¿œ¿ª ¿–æÓµÈ¿Œ¥Ÿ.
int LoadScriptText( char *scrname )
{
	char temp[FILENAME_MAX];
	char cwd[ FILENAME_MAX];

	getcwd( cwd, FILENAME_MAX );

	_chdir( GameStartDirectory );

	strcpy( CurScriptName, scrname );

	MakeFullPathName( temp, SCRIPT_PATH__, scrname);	// Scriptfile¿∫ CD∞° æ∆¥— «œµÂø°º≠ ¿–¥¬∞Õ¿ª ±‚∫ª¿∏∑Œ «—¥Ÿ. 


	if( GameMakeFlag == ON )	
	{
		prog = oldprog = (char *)LoadTextScript( temp );
	}
	else 
	{
		strcat( temp, "c");
		LoadScript( temp );
	}

	CLoadScript  =  _NEW_;

	_chdir( cwd );

	return 0;
}
*/



/* ≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â
 «‘ºˆ∏Ì : LoadScript
 ¿Œ  ¿⁄ :
 ∏Æ≈œ∞™ :
 º≥  ∏Ì : ¡§ªÛ¿˚¿∏∑Œ ¿–æÓµÈ¿Ã∏È 1¿ª ∏Æ≈œ«—¥Ÿ.
≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â≠Â */

int LoadScriptCompiled( char *scrname )
{	
    FILE *fp;
    int size, t, i;
	
    fp = fopen( scrname, "rb" );

	if( fp == NULL ) 	{
		MessageBox(0, "LoadScriptCompiled() :: File Not Found", "ERROR", MB_OK);
		exit(0);
	}
	
	if( oldprog != NULL )free( oldprog );
	
    fread( &size, 1, sizeof( int ), fp );           // √— ∆ƒ¿œ¿« ≈©±‚∏¶ ¿–¿Ω
    fread( &TotalFunction, 1, sizeof( int ), fp );              // «‘ºˆ¿« ∞πºˆ∏¶ ¿–¿Ω
	
    for( i = 0; i < TotalFunction; i++ )
    {
        fread( &t, 1, sizeof( int ), fp );          // «‘ºˆ¿« ø…º¬¿ª ¿–¿Ω
        func_off[ i ] = t;
    }
	
	oldprog = (char *)malloc( size );
    fread( oldprog, size, 1, fp );
	prog = oldprog;
    fclose( fp );

    return( 1 );
}

int LoadScript( char *mapname )
{
    char scriptname[ FILENAME_MAX];
	sprintf_s( scriptname, sizeof(scriptname), "%s/script/%s.zs", GameServerDataPath, mapname );
	
	if( 1 )
	{
		prog = oldprog = (char *)LoadTextScript( scriptname );
		if( prog == NULL ) return 0;
	}
	else 
	{
		strcat_s( scriptname, sizeof(scriptname), "c" );
		LoadScriptCompiled( scriptname );
	}
	
	CLoadScript  =  SCRIPT_NEW;
  return( 1 );
}	
	
	
	
	
	
	
	
	
	
