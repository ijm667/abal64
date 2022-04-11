#include <stdio.h>

extern	int	pushfile();
extern	int	popfile();
extern	int	filzone(int a, int b , int c , int d , int e );
extern	int	initialisepixel(int mode);
extern	int	liberaetpixel();

int	autoflag=0;

static	int wipearea( int mode , int dath )
{

	switch (( mode & 0x00FF )) {
		case	0 : filzone(0,0,320,200,dath); break;
		case	1 : filzone(0,0,640,400,dath); break;
		case	2 : filzone(0,0,640,480,dath); break;
		case	3 : filzone(0,0,800,600,dath); break;
		case	4 : filzone(0,0,1024,786,dath); break;
		case	5 : filzone(0,0,1200,1024,dath); break;
		default	  : return(0);
		}
	return(1);
}

int	prlsysci()
{
	int	c=0;
	if ( autoflag )
		return( 13 );
	_asm	{
		mov	ah,010H
		int	110
		xor	ah,ah
		mov	c,ax
		}
	return(c);
}

int	main(int argc, char * argv[] )
{
	int	status;
	int	mode;
	int	argi;
	char * aptr;
	if ( argc > 1 ) {
		for ( argi = 1; argi < argc; argi++ ) {

			if ((aptr = argv[argi]) != (char *) 0) {
				mode = 0;
				if ( *aptr == '-' ) {
					autoflag=1;
					continue;
					}
				else	{
					while (( *aptr >= '0' ) && ( *aptr <= '9'))
						mode = ((mode * 10)  + ( *(aptr++) - '0' ));
					}
				}
			else	mode = 0;
			
			(void) initialisepixel( mode );
			if (!( wipearea( mode, 77 ) ))
				break;
			else if ((status = pushfile()) != 0)
				break;
			else if ( prlsysci() == 27 ) 
				break;
			else if (!( wipearea( mode, 145 ) ))
				break;
			else if ( prlsysci() == 27 ) 
				break;
			else if ((status = popfile()) != 0)
				break;
			else if ( prlsysci() == 27 ) 
				break;
			else	liberatepixel();
			}
		liberatepixel();
		}
	else	{
		printf("\n   PushPopFileGraphicTester : Version 1.1a ");
		printf("\n   Provisoire du auhjourdhui ");
		printf("\n   Copyright (me) 1066 and all that\n");
		}
	return(0);
}

