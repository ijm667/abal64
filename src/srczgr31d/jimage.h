//VAP HEADER
#ifndef	JIMAGE_H
#define	JIMAGE_H

#include"wdef.h"
#include "jstdpsu.h"
#include"zint.h"

struct	standard_image {
	int		rows;
	int		columns;
	STDPSUPTR	storage;
	int 		users;
	};

void  load_palette( char * filename );
void  put_palette();

VOID	initialise_images();
VOID 	zbmp_free(WORD i);
VOID	liberate_images();

struct standard_image* export_button_bitmap(WORD index);

SWORD zbmp_load(WORD entry,PTRC filename,WORD mode);
SWORD zwin_bbmp(WORD bout_type,PTRC bout_lib,WORD entry,WORD larg);// destr 79
SWORD zwin_bmp(WORD bmp_no,WORD x,WORD y,WORD larg,WORD haut);
SWORD zwin_xbmp(WORD bmp_no,WORD olarg,WORD ohaut,WORD x,WORD y,WORD larg,WORD haut);
SWORD centralize_bitmap(PTI_RECTANGLE limit_gr_rect,struct standard_image* iptr,WORD image_tx_larg,WORD image_tx_haut);
WORD  zbmp_findfreeindex();

#endif	

