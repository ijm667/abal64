#ifndef	_JPEGFUNC_C
#define	_JPEGFUNC_C

#ifdef		TRACE_ALSM_LBSM
#undef		TRACE_ALSM_LBSM
#endif

#include "catcher.h"
#include "jpegflag.h"

static	unsigned char waitbuffer[16];
	

void	llwait( int delay )
{
	unsigned char far * descriptor = waitbuffer;
	*(descriptor+0) = 0x080;
	*(descriptor+1) = (delay & 0x00FF);
	*(descriptor+2) = ((delay>>8) & 0x00FF);
	*(descriptor+3) = 0;
	*(descriptor+4) = 0;
	*(descriptor+5) = 0;
	*(descriptor+6) = 0;
	*(descriptor+7) = 0;

_asm	{
	push	ds			
	push	bx
	push	ax
	lds	bx,descriptor
	mov	ah,42h	
	int	110
	lds	bx,descriptor
	mov	ah,31h	
	int	110
	pop	ax
	pop	bx
	pop	ds			
	};
	return;
}

extern 	void	jinit_max_to_use( long );
extern	char	JerrorBuffer[];
extern	int	JerrorFlag;

PRIVATE	JPEGPTR JpegHeap[10];

// #define	malloc(a) pralloc(a)
// #define	free(a) pree(a)

VOID PTR malloc( WORD );
VOID     free( VOID PTR );

WORD	verbose=0;
WORD	JpegColours=1;

char *	PlayCtrl="\0=PlayCtrl";

void	playnote( hertz, length )
{
	if ( *PlayCtrl != 0 ) {
		speaker(hertz);
		llwait(length);
		speaker(0);
		}
	return;
}

PRIVATE	JPEGPTR	LiberateJpeg( JPEGPTR	jptr )
{
	if ( jptr ) {
		jpeg_destroy_decompress(&jptr->cinfo);

/* IJM : correction 1.1b : liberation of filename */
		if (jptr->filename) {
			free( jptr->filename );
			jptr->filename = (char *) 0;
			}
/* IJM */
		if ( jptr->handle != (FILE *) 0 ) {
			fclose( jptr->handle );
			jptr->handle = (FILE *) 0;
			}
			
		free( jptr );
		}
	return((JPEGPTR) 0);
}


PRIVATE	JPEGPTR	AllocateJpeg( BYTE FAR PTR filename, WORD namelen )
{
	JPEGPTR	jptr;
	WORD	i;
	if ((jptr = (JPEGPTR) malloc( sizeof( JPEGLOADER ) )) != (JPEGPTR) 0) {
		jptr->options= 0;
		jptr->handle = (FILE *) 0;
		jptr->row_stride = 0;
		jptr->cinfo.err = jpeg_std_error(&jptr->pub);
		(void) jpeg_create_decompress(&jptr->cinfo);
		if ((jptr->filename = (BYTE PTR) malloc( namelen+1 )) == (BYTE PTR) 0)
			jptr = LiberateJpeg( jptr );
		else	{
			for (i=0; i < namelen; i++ ) 
				*(jptr->filename + i) = *(filename + i);
			*(jptr->filename + i) = 0;
			}
		}
	return( jptr );
}

PUBLIC WORD JpegInitialise( WORD options )
{
	WORD	i;
	WORD	x=0;
	if ((x = catch_exception()) == 0) {
		JpegColours = options;
		for (i=0; i < 10; i++)
			JpegHeap[i] = (JPEGPTR) 0;
		}
	return(x);
}

PUBLIC WORD JpegClose(WORD handle)
{
	JPEGPTR	jptr=(JPEGPTR) 0;
	WORD	x=0;
	WORD	state=0;

	if (( handle >= 0) && ( handle < 10 )) {

		if ((x = catch_exception()) == 0) {
			if ((jptr = JpegHeap[handle]) != (JPEGPTR) 0) {
				if ( jptr->handle != (FILE *) 0 ) {
					state=1;
					(void) jpeg_finish_decompress(&jptr->cinfo);
					state=2;
					fclose( jptr->handle );
					jptr->handle = (FILE *) 0;
					}
				state=3;
				jptr = LiberateJpeg( jptr );
				state=4;
				set_palette_pointer( (char *) 0 );
				}
			}
		else	{
			if ( jptr ) {
				if ( state < 2 ) {
					fclose( jptr->handle );
					jptr->handle = (FILE *) 0;
					}
				if ( state < 4 )
					jptr = LiberateJpeg( jptr );
				}
			}
		JpegHeap[handle] = (JPEGPTR) 0;
		}

	return(x);
}

PUBLIC WORD JpegLiberate()
{
	WORD	i;
	WORD	x;
	for (i=0; i < 10; i++)
		(void) JpegClose( i );
	return(0);
}

//	-------------------------------------------------------
//	Precise information concerning the JPEG options please
//	consult the header file JPEGFLAG.H (IJM 03/12/1999)
//	and for even further information the USAGE.DOC and
//	DJPEG.C of the JPEG library
//	-------------------------------------------------------

PUBLIC WORD JpegOpen(BYTE FAR PTR filename,WORD namelen,WORD options, BYTE FAR PTR palette )
{
	JPEGPTR		jptr=(JPEGPTR) 0;
	WORD		handle;
	int		v,n,i;

	WORD	x;

	if ((jptr = AllocateJpeg(filename,namelen)) == (JPEGPTR) 0)
		return(-1);

	else if ((x = catch_exception()) != 0 ) {
		playnote(100,4);
		playnote(200,4);
		jptr = LiberateJpeg(jptr);
		return(-1);
		}

	for (handle=0; handle < 10; handle++)
		if (!( JpegHeap[handle] ))
			break;

	if ( handle >= 10 ) {
		jptr = LiberateJpeg(jptr);
		return(-1);
		}

	else if ((jptr->handle = fopen( jptr->filename, "rb" )) != (FILE *) 0) {

		jpeg_stdio_src(&jptr->cinfo, jptr->handle);

		if ( jpeg_read_header(&jptr->cinfo,1) != 1) {
			playnote(200,4);
			playnote(100,4);
			jptr = LiberateJpeg(jptr);
			return(-1);
			}

		set_palette_pointer( palette );

		// Is Quantizing to be inhibited ?
		// -------------------------------
		if (!( options & _NVJPEG_NOQUANT )) {

			// Must 32 Colours be protected
			// ----------------------------
			if ( options & _NVJPEG_PROTECT )

				// Protect 32 Basic Colours
				jptr->cinfo.desired_number_of_colors = (256-32);

			else	// Use all 256 colours
				// -------------------
				jptr->cinfo.desired_number_of_colors = 256;

			jptr->cinfo.quantize_colors = TRUE;

			}


		if ( options & _NVJPEG_ONEPASS )
			jptr->cinfo.two_pass_quantize = FALSE;

		if ( options & _NVJPEG_NOSMOOTH )
			jptr->cinfo.do_fancy_upsampling = FALSE;

		if ( options & _NVJPEG_DCTIFAST )
			jptr->cinfo.dct_method = JDCT_IFAST;
		else	jptr->cinfo.dct_method = JDCT_ISLOW;

		if ( options & _NVJPEG_FSDITHER )
			jptr->cinfo.dither_mode = JDITHER_FS;
		else if ( options & _NVJPEG_ORDERED )
			jptr->cinfo.dither_mode = JDITHER_ORDERED;
		else	jptr->cinfo.dither_mode = JDITHER_NONE;


		if ( jpeg_start_decompress(&jptr->cinfo) != 1) {
			playnote(400,4);
			playnote(200,4);
			playnote(300,4);
			jptr = LiberateJpeg(jptr);
			return(-1);
			}

		jptr->row_stride = jptr->cinfo.output_width * jptr->cinfo.output_components;

		jptr->buffer = (*jptr->cinfo.mem->alloc_sarray)
			((j_common_ptr) &jptr->cinfo, JPOOL_IMAGE, jptr->row_stride, 1);

		// This will be affected by the QUANTIZING flags 
		if ( jptr->cinfo.output_components == 1 ) {

			if ( jptr->cinfo.out_color_space != JCS_GRAYSCALE ) {
				n = jptr->cinfo.actual_number_of_colors;
				if ( n > 256 ) n=256;
				if ( options & _NVJPEG_PROTECT ) {
					for ( 	v=0; v < n; v++ ) {
						if (( i = vga_application(v,
							(jptr->cinfo.colormap[0][v] & 0x00FF),
							(jptr->cinfo.colormap[1][v] & 0x00FF),
							(jptr->cinfo.colormap[2][v] & 0x00FF))) != -1 )
							jptr->palette[v] = i;
						else	break;
						}
					}
				else	{
					for ( 	v=0; v < n; v++ )
						jptr->palette[v] = 
							vga_approximation(
								(jptr->cinfo.colormap[0][v] & 0x00FF),
								(jptr->cinfo.colormap[1][v] & 0x00FF),
								(jptr->cinfo.colormap[2][v] & 0x00FF));
					}
				}
			else	{
				for ( 	v=0; v < 256; v++ )
					jptr->palette[v] =  vga_approximation( v,v,v );

				}

			}

		JpegHeap[handle] = jptr;
		return(handle);
		}
	else	{
		jptr = LiberateJpeg(jptr);
		return(-1);
		}
}


PUBLIC WORD JpegGetError(BYTE FAR PTR pixels,WORD buflen)
{
	int	i=0;
	if ( JerrorFlag ) {
		for (	i=0;
			i < buflen;
			i++ ) {
			if (!( *(pixels + i) = JerrorBuffer[i] ))
				break;
			}
		*(pixels+i) = 0;
		}
	return(i);
}

PUBLIC WORD JpegGetRow(WORD handle,BYTE FAR PTR pixels,WORD buflen)
{
	JPEGPTR	jptr;
	BYTE     PTR	bptr;
	BYTE FAR PTR	pptr;
	int	i,r,g,b,p,n,v;

	WORD	x;
	if ((x = catch_exception()) != 0 ) {
		return(-1);
		}

	if ((jptr = JpegHeap[handle]) != (JPEGPTR) 0) {
		if (jptr->cinfo.output_scanline < jptr->cinfo.output_height) {
			(void) jpeg_read_scanlines(&jptr->cinfo, jptr->buffer, 1);
			bptr=(char *) jptr->buffer[0]; 
			pptr = pixels;
			// Watch out for Grey Scale
			if ( jptr->cinfo.out_color_space == JCS_GRAYSCALE ) {
				for (i=0; i < jptr->row_stride; i++ ) {
					n = (*(bptr+i) & 0x00FF);
					*(pptr + i) = jptr->palette[n];
					}
				}
			else	{
				i=0; 
				n = jptr->cinfo.output_components;

				while (i < jptr->row_stride ) {
					if ( n == 1 ) {
						v = *( bptr + (i++));
						*(pptr++) = jptr->palette[(v & 0x00FF)];
						}
					else if ( n == 3 ) {
						r = *( bptr + (i++));
						g = *( bptr + (i++));
						b = *( bptr + (i++));
						*(pptr++) = vga_approximation( r, g, b);
						}
					else	break;
					}
				}
			return(jptr->cinfo.output_width);
			}
		}
	return(-1);
}

PUBLIC WORD JpegGetWidth(WORD handle)
{
	JPEGPTR	jptr;
	WORD	x;
	if ((x = catch_exception()) != 0 ) {
		return(0);
		}
	else if ((jptr = JpegHeap[handle]) != (JPEGPTR) 0)
		return(jptr->cinfo.output_width);
	else	return(0);
}

PUBLIC WORD JpegGetHeight(WORD handle)
{
	JPEGPTR	jptr;
	WORD	x;
	if ((x = catch_exception()) != 0 ) {
		return(0);
		}
	else if ((jptr = JpegHeap[handle]) != (JPEGPTR) 0)
		return(jptr->cinfo.output_height);
	else	return(0);
}

PUBLIC VOID JpegSetMemory( WORD kilos )
{
	LONG	k=kilos;
	jinit_max_to_use( kilos );
	return;
}

#endif	/* _JPEGFUNC_H */
	/* ----------- */



