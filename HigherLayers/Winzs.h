

//#define MAKE_EXE_FILE	

#ifndef __WINZS_H__
#define __WINZS_H__


extern void				InitTextScript( void );
extern void				EndTextScript( void );
extern unsigned char   *LoadTextScript( char *name );
extern void				NotePad( char *name );
extern int				LoadData( char *szdat );


#else


void			InitTextScript( void );
void			EndTextScript( void );
unsigned char  *LoadTextScript( char *name );
void			NotePad( char *name );
int				LoadData( char *szdat );

#endif


