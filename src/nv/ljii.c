#ifndef	_LJII_C
#define	_LJII_C

#include "lodriver.h"
#include "ljii.h"

#define	putlo(v) *(result+(rlen++))=v
#define	escapeT( v ) rlen = packethead(result,rlen,v)

int	escapeTbugfix=0;
int	MarginBytes=0;
int	PrinterReady=0;
int	RasterReady=0;
int	DotsPerInch=0;
int	RowLength=0;
char *	RasterByte=(char *) 0;
char *	RasterWork=(char *) 0;
int	PixelMask=0;
int	BitsPerPixel=0;
char *	PaletteBuffer=(char *) 0;
char	Number[32];




int	_fltused=0;
void	_cfltcvt()	{ 	return; 	}
void	_cropzeros()	{ 	return; 	}
long	_aFftol()	{	return(0L);	}

int	average( int v )
{
	int	max=0;
	int	r=0;
	char *  pptr;
	if (( v >= 0 ) && ( v <= 255)) {
		pptr = (PaletteBuffer + (v*4));
		if ((r = (*(pptr + 3) & 0x00FF)) != 0)
			return(r);
		else 	{
			if ((r = (*pptr & 0x00FF)) > max )
				max = r;
			if ((r = (*(pptr+1) & 0x00FF)) > max )
				max = r;
			if ((r = (*(pptr+2) & 0x00FF)) > max)
				max = r;
			return((*(pptr+3) = max));
			}
		}
	else	return( 0 );
}

int	packethead(char * result,int rlen, int nb )
{
	putlo( 27 ); putlo( 'T' );
	putlo( nb & 0x00FF );
	putlo( (nb >> 8) & 0x00FF );
	return(rlen);
}

int	length( char * sptr )
{
	int	l;
	for ( l=0; *(sptr+l) != 0; l++ );
	return( l );
}

int	DriverInitialise(char * palette, int dpi, int grnbcol, int grnbrow, int options)
{
	int	PaperPixels;
	int	MarginPixels;
	int	RasterPixels;
	int	RasterBytes;
	int	i;

	RasterReady = PrinterReady = 0;

	escapeTbugfix = (options & 0x1000);

	PaletteBuffer = palette;

	// Test Driver Jet Dots / Inch 
	// ---------------------------
	switch (( DotsPerInch = dpi)) {
		case	   0	: // Driver Output Inhibited
			return(0);
		case	  75 	:
		case	 100	:
		case	 150	:
		case	 300	: // Max for Driver Jet Series 2
		case	 600	:
		case	1200	: // Max for Driver Jet Series 4
			break;
		default		:
			if ( DotsPerInch < 100 )
				DotsPerInch = 75;
			else if ( DotsPerInch < 150 )
				DotsPerInch = 100;
			else if ( DotsPerInch < 300 )
				DotsPerInch = 150;
			else if ( DotsPerInch < 600 )
				DotsPerInch = 300;
			else if ( DotsPerInch < 1200 )
				DotsPerInch = 600;
			else	DotsPerInch = 1200;
			break;
		}

	switch ( DotsPerInch ) {
		case	  75 : PaperPixels =   584; break;
		case	 100 : PaperPixels =   778; break;
		case	 150 : PaperPixels =  1168; break;
		case	 300 : PaperPixels =  2336; break;
		case	 600 : PaperPixels =  4672; break;
		case	1200 : PaperPixels =  9344; break;
		default	     : return(0);
		}

	if ( PaperPixels >= (RowLength=grnbcol) ) {

		if ((BitsPerPixel = (PaperPixels/RowLength)) >= 8 ) {
			BitsPerPixel = 8;
			PixelMask    = 0x00FF;
			}
		else if ( BitsPerPixel >= 4 ) {
			BitsPerPixel = 4;
			PixelMask    = 0x00F0;
			}
		else if ( BitsPerPixel >= 2 ) {
  			BitsPerPixel = 2;
			PixelMask    = 0x00C0;
			}
		else	{
			BitsPerPixel = 1;
			PixelMask    = 0x0080;
			}
		}
	else	{
		RowLength      = PaperPixels; 
		BitsPerPixel = 1;
		PixelMask    = 0x0080;
		}

	RasterPixels = (RowLength * BitsPerPixel);
	RasterBytes =  ((RasterPixels / 8) + (RasterPixels % 8 ? 1 : 0));

	if ( PaperPixels > RasterPixels ) {
		switch ( options & 3 ) {
			case	1 :
				MarginPixels = MarginBytes = 0;
				break;

			case	2 :
				if ((MarginPixels = (PaperPixels - RasterPixels)) > 0)
					MarginBytes  = (MarginPixels / 8);
				else	MarginBytes  = 0;
				break;

			case	0 :
			case	3 :
			if ((MarginPixels = ((PaperPixels - RasterPixels) / 2)) > 0)
				MarginBytes  = (MarginPixels / 8);
			else	MarginBytes  = 0;
			}
		}
	else	MarginBytes = 0;
		

	return( ((RasterBytes + MarginBytes + 128) * BitsPerPixel) );

}


int	DriverConnect( char * bptr, char * wptr, char * result )
{
	int	rlen=0;
	int	i=0;

	RasterByte = bptr;
	RasterWork = wptr;

	// Reset Left Margin Space to WHITE
	// --------------------------------
	if ( MarginBytes )
		for ( i=0; i < MarginBytes; i++ )
			*(RasterByte + i) = 0;


	// Reset DriverJet Printer
	// ----------------------
	escapeT( 2 ); putlo( 27 ); putlo('E');

	PrinterReady=1;

	return( rlen );
}

int	DriverStartRaster(char * result)
{
	int	rlen=0;
	int	i;

	char 	number[16];

	if (!( RasterReady )) {

		// Establish DriverJet Dots per Inch
		// --------------------------------
		sprintf(number,"%u",DotsPerInch);

		escapeT( (4 + length( number )) );
			putlo( 27 ); 
			putlo('*');
			putlo('t');
			for ( i=0; number[i] != 0; i++ )
				putlo(number[i]);
			putlo('R');

		// Ensure start at Left margin
		// ---------------------------
		escapeT( 5 ); 
	 	putlo( 27 ); putlo('*'); putlo('r'); putlo('0'); putlo('A'); 

		// Ensure Printing in Logical X direction
		// --------------------------------------
		escapeT( 5 ); 
		putlo( 27 ); putlo('*'); putlo('r'); putlo('0'); putlo('F'); 

		// Ensure Opaque
		// -------------
		escapeT( 5 ); 
		putlo( 27 ); putlo('*'); putlo('r'); putlo('1'); putlo('N');
		
		RasterReady = 1;
		}
	return(rlen);
}


int	DriverEndRaster(char * result)
{
	int	rlen=0;
	if ( RasterReady ) {
		// End Graphics
		escapeT( 4 ); 
		putlo( 27 ); 	putlo('*'); 
		putlo('r'); 	putlo('C');
		RasterReady=0;
		}
	return(rlen);
}

int	DriverLiberate(char * result)
{
	int	rlen=0;

	if ( RasterReady )
		rlen = DriverEndRaster(result);

	// Reset DriverJet Printer

	escapeT( 2 ); 
		putlo( 27 ); 
		putlo('E');

	RasterWork = (char *) 0;
	RasterByte = (char *) 0;
	return(rlen);
}

int	optimiser(int notwhite,char * result,int rlen)
{
	int	bpp=BitsPerPixel;
	int	o=0;
	int	i=0;
	int	n=0;
	int	a=0;
	int	c=0;
	int	l=0;
	int	d=0;
	int	x=0;
	int	f=0;

	char *  rptr=RasterByte;
	char *  wptr=RasterWork;
	char *	nptr=Number;

	while ( i < notwhite ) {

		// Detect string of identical bytes
		// --------------------------------
		for (c=*(rptr+i),n=0,a=(i+1); a < notwhite; a++ ) {
			if ( *(rptr + a) != c )
				break;
			else	n++;
			}
		if ( n ) {
			n++;
			// N identical bytes found
			// -----------------------
			while ( n >= 128 ) {
				*(wptr +o) = ((128 - 1) * -1); o++; n-=128;
				}

			if ( n ) {
				*(wptr+o) = ((n - 1) * -1); o++;		
				}
			
			}
		else	{
			// --------------------------------
			// No match found
			// --------------------------------
			// Detect string of different bytes
			// --------------------------------
			for (n=0,a=(i+1); a < notwhite; a++ ) {
				if ( *(rptr + a) != *(rptr + i) ) {
					i++;
					n++;
					}
				else	break;
				}		

			if ( a == notwhite )
				n++;
			else	a = i;

			while ( n >= 128 ) {
				*(wptr +o) = 127; o++; n-=128;
				}

			if ( n ) {
				*(wptr+o) = (n-1); o++;		
				}
			}
		i = a;
		}

	// Totalise all bytes to be sent
	// -----------------------------
	for (n=0,a=0; a < o; a++ ) {
		if ( *(wptr+a) >= 0 )
			n += (*(wptr+a) + 2);
		else	n += 2;
		}
	
	if (!(f = n))
		n++;

	sprintf(nptr,"%u",n);
	l = length( nptr );

	for (a=0; a < bpp; a++ ) {

		if ( f ) 
			escapeT(6+l+n);
		else	escapeT(4+l+n);

		putlo(27); putlo('*'); putlo('b');

		if ( f ) {
			putlo('2');putlo('m');
			}

		for ( i=0; *(nptr+i) != 0; i++ )
			putlo(*(nptr+i));
		putlo('W');

		if (!( f ))
			putlo(f);

		else for (d=0,i=0; i < o; i++ ) {
			putlo((c = *(wptr+i)));
			if ( c >= 0 ) {
				c++;
				for ( x=0;x < c; x++ ) {
					putlo( *(rptr + d));
					d++;
					}
				}
			else	{
				putlo( *(rptr + d) );
				d += ((c * -1) + 1);
				}
			}
		}

	return(rlen);
}

int	DriverPrintRow(char * result, char * rowbuffer)
{
	int	rlen=0;
	int	t;
	int	a;
	int	b;
	int	c;
	int	i;
	int	v;
	int	p;
	int	n;
	int	rl=RowLength;
	char *	rb=RasterByte;
	int	pm=PixelMask;
	int	bpp=BitsPerPixel;
	int	notwhite=0;
	char *	nptr=Number;

	if (!( PrinterReady ))
		return(0);
	else if (!( RasterReady ))
		rlen = DriverStartRaster(result);

	// Preparation of Raster Row
	// -------------------------
	for (notwhite=0,v=0,i=pm,b=MarginBytes,c=0; c < rl; c++ ) {

		// Average and Invert Luminosity (0 -> 255 getting darker)
		// -------------------------------------------------------
		
		a = average(( *(rowbuffer + c) & 0x00FF));

		a = ((a ^ 0x00FF) & 0x00FF);

		switch ( bpp ) {
			case	8	:
				v = a; 
				break;
			case	4	:
				if ( i == pm )
					a &= pm;
				else	a = ((a & pm) >> 4);
				v |= a;
				break;
			case	2	:
				switch ( a & pm ) {
					case	0x00C0	:
						v |= i; break;
					case	0x0080	:
						v |= (i & 0x5555); break;
					case	0x0040	:
						v |= (i & 0xAAAA); break;
					}
				break;

			case	1	:
				if ( a & pm )
					v |= i;
				break;
			}
		i >>= bpp; 

		if (!( i )) {
			i = pm;
			if (( escapeTbugfix )
			&&  ( v == 0x001A   )) 
				v = 0x002A;
			if (( *(rb + (b++)) = v ) != 0)
				notwhite = b;
			v=0;
			}
		}

	if ( i != pm ) {
		if (( escapeTbugfix )
		&&  ( v == 0x001A   )) 
			v = 0x002A;
		if (( *(rb + (b++)) = v ) != 0)
			notwhite = b;
		}

	// Send Rows of Raster Data
	// ------------------------
	if (!( notwhite ))
		notwhite=1;

	// Normal non-optimised heavy weight
	// ---------------------------------
	sprintf(nptr,"%u",notwhite);
	v = length( nptr );

	while (((t = (4+v+notwhite)) & 0x00FF) == 0x001A) {
		if (!( escapeTbugfix ))
			break;
		else	{
			notwhite++;		
			sprintf(nptr,"%u",notwhite);
			v = length( nptr );
			}
		}

	for (n=0; n < bpp; n++ ) {
		escapeT(t);
		putlo(27); putlo('*'); putlo('b');

		for ( i=0; *(nptr+i) != 0; i++ )
			putlo(*(nptr+i));
		putlo('W');
		for (i=0; i < notwhite; i++ )
			putlo(*(rb+i));
		}

	return(rlen);
}

int	DriverColours()	{	return(0); 	}


#include "lodriver.c"



	/* ----------- */
#endif	/* _DRIVERABAL_C */
	/* ----------- */



