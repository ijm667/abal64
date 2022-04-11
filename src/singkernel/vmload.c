
#ifndef _vmload_c
#define _vmload_c
/* ------------------------------------------------------------------------- */
/* Warning : this file has been generated and should not be modified by hand */
/* ------------------------------------------------------------------------- */
/* Sing Version   : 1.1a.0.45  */
/* Production     : C  */
/* Project        : vmload.xml  */
/* Target         : vmload.c  */
/* Identification : 0.0-1100085262-2982.2981 */
/* ------------------------------------------------------------------------- */

#include <stdio.h>
#include "visual.h"
#include "singprot.h"
#define en_auto703   "Sing : Version 1.0a"
#define fr_auto703   "Sing : Version 1.0a"
#define en_Auto1152   "skin.gif"
#define fr_Auto1152   "skin.gif"
#define en_auto706   "Name of Application to Load"
#define fr_auto706   "Nom du Projet � Charger"
#define it_auto706   ""
#define es_auto706   ""
#define de_auto706   ""
#define nl_auto706   ""
#define pt_auto706   ""
#define xx_auto706   ""
#define en_Accept   "&Accept"
#define fr_Accept   "&Valider"
#define en_Select   "&Select"
#define fr_Select   "&S�lection"
#define en_auto711   "fileicon.gif"
#define fr_auto711   "fileicon.gif"
#define it_auto711   ""
#define es_auto711   ""
#define de_auto711   ""
#define nl_auto711   ""
#define pt_auto711   ""
#define xx_auto711   ""
#define en_Cancel   "&Cancel"
#define fr_Cancel   "&Annuler"
static int	vfh[10];

private struct accept_modelfilename_context {
	int	keycode;
	int	focus_item;
	int	focus_items;
	int	window_col;
	int	window_row;
	int	language;
	char * msg_auto703[8];
	int	x_auto703;
	int	y_auto703;
	int	w_auto703;
	int	h_auto703;
	char * msg_Auto1152[8];
	char * msg_auto706[8];
	char	buffer_FileName[70];
	char * msg_Accept[8];
	int	trigger_Accept;
	char * msg_Select[8];
	int	trigger_Select;
	char * msg_auto711[8];
	char * msg_Cancel[8];
	int	trigger_Cancel;
	int	ctrlbuffer;
	void * pushbuffer;
	char	signature[2];
	};

public	int	accept_modelfilename_create(
	struct accept_modelfilename_context ** cptr,
	char * pFileName)
{

	int i;
	struct accept_modelfilename_context * _Context=(struct accept_modelfilename_context*)0;
	vfh[1] = visual_font("standard.fmf",12);
	vfh[2] = visual_font("arial2.fmf",10);
	vfh[3] = visual_font("arial3.fmf",10);
	vfh[4] = visual_font("arial4.fmf",10);
	vfh[5] = visual_font("arial5.fmf",10);
	vfh[6] = visual_font("ufjt6.fmf",9);
	vfh[7] = visual_font("font7.fmf",9);
	vfh[8] = visual_font("font8.fmf",9);
	vfh[9] = visual_font("font10.fmf",10);
	if (!(_Context = allocate( sizeof( struct accept_modelfilename_context ) ) ))
		return(27);
	else	*cptr=_Context;
	_Context->language = visual_national_language(-1);
	_Context->keycode=0;
	_Context->focus_item=1;
	_Context->focus_items=5;
	if (_Context->keycode != 27 ) {
		strcpy(_Context->signature,"S");
		visual_signature(_Context->signature,_Context->signature);
		visual_transferin(_Context->buffer_FileName, 69, pFileName);
		}
	for (i=0; i < 8; i++)_Context->msg_auto703[i]=" ";
	_Context->msg_auto703[0]=en_auto703;
	_Context->msg_auto703[1]=fr_auto703;
	_Context->x_auto703=105;
	_Context->y_auto703=195;
	_Context->w_auto703=590;
	_Context->h_auto703=210;
	_Context->window_col=0;
	_Context->window_row=0;
	for (i=0; i < 8; i++)_Context->msg_Auto1152[i]=" ";
	_Context->msg_Auto1152[0]=en_Auto1152;
	_Context->msg_Auto1152[1]=fr_Auto1152;
	for (i=0; i < 8; i++)_Context->msg_auto706[i]=" ";
	_Context->msg_auto706[0]=en_auto706;
	_Context->msg_auto706[1]=fr_auto706;
	_Context->msg_auto706[2]=it_auto706;
	_Context->msg_auto706[3]=es_auto706;
	_Context->msg_auto706[4]=de_auto706;
	_Context->msg_auto706[5]=nl_auto706;
	_Context->msg_auto706[6]=pt_auto706;
	_Context->msg_auto706[7]=xx_auto706;
	for (i=0; i < 8; i++)_Context->msg_Accept[i]=" ";
	_Context->msg_Accept[0]=en_Accept;
	_Context->msg_Accept[1]=fr_Accept;
	for (i=0; i < 8; i++)_Context->msg_Select[i]=" ";
	_Context->msg_Select[0]=en_Select;
	_Context->msg_Select[1]=fr_Select;
	for (i=0; i < 8; i++)_Context->msg_auto711[i]=" ";
	_Context->msg_auto711[0]=en_auto711;
	_Context->msg_auto711[1]=fr_auto711;
	_Context->msg_auto711[2]=it_auto711;
	_Context->msg_auto711[3]=es_auto711;
	_Context->msg_auto711[4]=de_auto711;
	_Context->msg_auto711[5]=nl_auto711;
	_Context->msg_auto711[6]=pt_auto711;
	_Context->msg_auto711[7]=xx_auto711;
	for (i=0; i < 8; i++)_Context->msg_Cancel[i]=" ";
	_Context->msg_Cancel[0]=en_Cancel;
	_Context->msg_Cancel[1]=fr_Cancel;
	_Context->ctrlbuffer=0;
	_Context->pushbuffer=visual_buffer(_Context->x_auto703,_Context->y_auto703,590+10,210+10);
	return(0);
}

public 	int	accept_modelfilename_hide(struct accept_modelfilename_context * _Context)
{
	int	result;
	if (!( _Context )) { return(0); }
	result=_Context->keycode;
	if ((_Context->pushbuffer != (void *) 0)
	&& (_Context->ctrlbuffer != 0)) {
		(void)visual_buffer_put(_Context->pushbuffer,_Context->x_auto703,_Context->y_auto703);
		_Context->ctrlbuffer=0;
		}

	return(result);
}

public	int	accept_modelfilename_remove(
	struct accept_modelfilename_context * _Context,
	char * pFileName)
{

	int	result;
	if (!( _Context )) { return(0); }
	result=_Context->keycode;
	if (_Context->keycode != 27 ) {
		strcpy(_Context->signature,"S");
		visual_signature(_Context->signature,_Context->signature);
		visual_transferout(_Context->buffer_FileName, 69, pFileName);
		}
	if (_Context->pushbuffer != (void *) 0)
		_Context->pushbuffer = visual_drop(_Context->pushbuffer);
	_Context=liberate(_Context);

	return(result);
}

public	int	accept_modelfilename_show(struct accept_modelfilename_context * _Context)
{
	_Context->language = visual_national_language(-1);
	if ((_Context->pushbuffer != (void *) 0)
	&& (!(_Context->ctrlbuffer))) {
		(void)visual_buffer_get(_Context->pushbuffer,_Context->x_auto703,_Context->y_auto703);
		_Context->ctrlbuffer=1;
		}
	visual_freeze();
	visual_window(_Context->x_auto703,_Context->y_auto703,590,210,vfh[2],_Context->msg_auto703[_Context->language],strlen(_Context->msg_auto703[_Context->language]),0x407);
	visual_image(_Context->x_auto703+3,_Context->y_auto703+22,582,184,_Context->msg_Auto1152[_Context->language],4);
	visual_text(_Context->x_auto703+20,_Context->y_auto703+50,546,16,vfh[2],27,0,_Context->msg_auto706[_Context->language],strlen(_Context->msg_auto706[_Context->language]),258);
	visual_frame(_Context->x_auto703+20,_Context->y_auto703+70,552+2,16+2,5);
	visual_text(_Context->x_auto703+20+1,_Context->y_auto703+70+1,552,16,vfh[1],26,0,_Context->buffer_FileName,69,0);
	_Context->trigger_Accept=visual_trigger_code(_Context->msg_Accept[_Context->language],strlen(_Context->msg_Accept[_Context->language]));
	visual_button(_Context->x_auto703+20,_Context->y_auto703+160,110,30,vfh[2],26,0,_Context->msg_Accept[_Context->language],strlen(_Context->msg_Accept[_Context->language]),0);
	_Context->trigger_Select=visual_trigger_code(_Context->msg_Select[_Context->language],strlen(_Context->msg_Select[_Context->language]));
	visual_button(_Context->x_auto703+350,_Context->y_auto703+160,100,30,vfh[2],27,24,_Context->msg_Select[_Context->language],strlen(_Context->msg_Select[_Context->language]),0);
	visual_image(_Context->x_auto703+222,_Context->y_auto703+140,50,50,_Context->msg_auto711[_Context->language],0);
	_Context->trigger_Cancel=visual_trigger_code(_Context->msg_Cancel[_Context->language],strlen(_Context->msg_Cancel[_Context->language]));
	visual_button(_Context->x_auto703+460,_Context->y_auto703+160,110,30,vfh[2],26,0,_Context->msg_Cancel[_Context->language],strlen(_Context->msg_Cancel[_Context->language]),0);
	visual_thaw(_Context->x_auto703,_Context->y_auto703,590,210);
	visual_thaw(_Context->x_auto703,_Context->y_auto703,590,210);

	return(0);
}

private int auto703action(struct accept_modelfilename_context * _Context) {
	int x=visual_event(7);
	int y=visual_event(6);
	do {
		do {

			while(visual_getch() != 256);
			}
		while( visual_kbhit() );
		visual_freeze();
			accept_modelfilename_hide(_Context);

		_Context->x_auto703 += (visual_event(7)-x);
		x=visual_event(7);
		if (_Context->x_auto703 < 0) { _Context->x_auto703=0; }
		_Context->y_auto703 += (visual_event(6)-y);
		y=visual_event(6);
		if (_Context->y_auto703 < 0) { _Context->y_auto703=0; }
			accept_modelfilename_show(_Context);

		visual_thaw(_Context->x_auto703,_Context->y_auto703,590,210);
		}
	while (visual_event(1) != _MIMO_UP);
	return(-1);
}
private int on_Accept_event(struct accept_modelfilename_context * _Context) {
	return(13);
	return(-1);
}
private int on_Select_event(struct accept_modelfilename_context * _Context) {
	use_file_selector(_Context->buffer_FileName,WIDGET_FILE_XML);
		accept_modelfilename_show(_Context);
;
	return(-1);
}
private int on_Cancel_event(struct accept_modelfilename_context * _Context) {
	return(27);
	return(-1);
}


public	int	accept_modelfilename_event(
struct accept_modelfilename_context * _Context){
	int	mx,my,mt,mb;
	if (visual_event(1) & _MIMO_ALT) {
		if (((mb = (visual_event(2) & 0x00FF)) >= 'a') && (mb <= 'z')) mb -= ('a' - 'A');
		if (_Context->trigger_Accept == mb ) return(3);
		if (_Context->trigger_Select == mb ) return(4);
		if (_Context->trigger_Cancel == mb ) return(5);
		return(-1);
		}
	if (!((mt = visual_event(1)) & 0x3031)) return(-1); 
	mx = visual_event(7);
	my = visual_event(6);
	switch (mt) {
	case _MIMO_UP :
	if ((mx >= (_Context->x_auto703+569) )  
	&&  (my >= (_Context->y_auto703+4) )  
	&&  (mx <= (_Context->x_auto703+586) )  
	&&  (my <= (_Context->y_auto703+20) )) {
		return(1);	/* auto703 */

		}
	if ((mx >= (_Context->x_auto703+553) )  
	&&  (my >= (_Context->y_auto703+4) )  
	&&  (mx <= (_Context->x_auto703+570) )  
	&&  (my <= (_Context->y_auto703+20) )) {
				if ( visual_event(1) & _MIMO_UP ) {
			visual_help("vmload.htm");
			};
		return(-1);	/* auto703 */

		}
	if ((mx >= (_Context->x_auto703+537) )  
	&&  (my >= (_Context->y_auto703+4) )  
	&&  (mx <= (_Context->x_auto703+554) )  
	&&  (my <= (_Context->y_auto703+20) )) {
				if (visual_event(2) ==_MIMO_LEFT) {
			_Context->language++; _Context->language %=8;
			}
		else {
		if ( _Context->language > 0 ) { _Context->language--; _Context->language %=8; }
		else { _Context->language=7; }
			}
		(void) visual_national_language(_Context->language);
		accept_modelfilename_show(_Context);
		return(-1);	/* auto703 */

		}
	case _MIMO_DOWN :
		if ((mx >= (_Context->x_auto703+4) )  
		&&  (my >= (_Context->y_auto703+4) )  
		&&  (mx <= (_Context->x_auto703+522) )  
		&&  (my <= (_Context->y_auto703+20) )) {
			return( auto703action(_Context) );
			}
		}
	if (( mx >= (_Context->x_auto703+20) ) 
	&&  ( my >= (_Context->y_auto703+70) ) 
	&&  ( mx <= (_Context->x_auto703+20+552) ) 
	&&  ( my <= (_Context->y_auto703+70+16))) {
		return(2); /* FileName */
		}
	if (( mx >= (_Context->x_auto703+20) ) 
	&&  ( my >= (_Context->y_auto703+160) ) 
	&&  ( mx <= (_Context->x_auto703+20+110) ) 
	&&  ( my <= (_Context->y_auto703+160+30))) {
		return(3); /* Accept */
		}
	if (( mx >= (_Context->x_auto703+350) ) 
	&&  ( my >= (_Context->y_auto703+160) ) 
	&&  ( mx <= (_Context->x_auto703+350+100) ) 
	&&  ( my <= (_Context->y_auto703+160+30))) {
		return(4); /* Select */
		}
	if (( mx >= (_Context->x_auto703+460) ) 
	&&  ( my >= (_Context->y_auto703+160) ) 
	&&  ( mx <= (_Context->x_auto703+460+110) ) 
	&&  ( my <= (_Context->y_auto703+160+30))) {
		return(5); /* Cancel */
		}

	return(-1);
}


public	int	accept_modelfilename_focus(struct accept_modelfilename_context * _Context)
{
	int retcode=0;

	while( visual_kbhit() ) (void) visual_getch();

	while(1) {
		switch (_Context->focus_item) {
			case	0x2 :
				/* FileName */
				_Context->keycode = visual_edit(_Context->x_auto703+20+1,_Context->y_auto703+70+1,552,16,vfh[1],_Context->buffer_FileName,69);
			break;
			case	0x3 :
				/* Accept */
				visual_button(_Context->x_auto703+20,_Context->y_auto703+160,110,30,vfh[2],26,0,_Context->msg_Accept[_Context->language],strlen(_Context->msg_Accept[_Context->language]),2);
				_Context->keycode = visual_getch();
				visual_button(_Context->x_auto703+20,_Context->y_auto703+160,110,30,vfh[2],26,0,_Context->msg_Accept[_Context->language],strlen(_Context->msg_Accept[_Context->language]),0);
				break;
			case	0x4 :
				/* Select */
				visual_button(_Context->x_auto703+350,_Context->y_auto703+160,100,30,vfh[2],27,24,_Context->msg_Select[_Context->language],strlen(_Context->msg_Select[_Context->language]),2);
				_Context->keycode = visual_getch();
				visual_button(_Context->x_auto703+350,_Context->y_auto703+160,100,30,vfh[2],27,24,_Context->msg_Select[_Context->language],strlen(_Context->msg_Select[_Context->language]),0);
				break;
			case	0x5 :
				/* Cancel */
				visual_button(_Context->x_auto703+460,_Context->y_auto703+160,110,30,vfh[2],26,0,_Context->msg_Cancel[_Context->language],strlen(_Context->msg_Cancel[_Context->language]),2);
				_Context->keycode = visual_getch();
				visual_button(_Context->x_auto703+460,_Context->y_auto703+160,110,30,vfh[2],26,0,_Context->msg_Cancel[_Context->language],strlen(_Context->msg_Cancel[_Context->language]),0);
				break;

			default :
				_Context->keycode=5;
			}
retry:
		switch (_Context->keycode) {
		case	0xc :
			_Context->focus_item=1;
			accept_modelfilename_show(_Context);
			continue;
		case	0x2 :
			_Context->focus_item=5;
			accept_modelfilename_show(_Context);
			continue;
			case	0x100 :
				if ((retcode=accept_modelfilename_event(_Context)) == -1)
					continue;
				switch ((_Context->focus_item = retcode)) {
					case	0x1 :
						/* auto703 */
						if (visual_event(1) & _MIMO_UP) {
							_Context->keycode=27;
							break;
						} else {
							continue;
							}
					case	0x2 :
						/* FileName */
						continue;
					case	0x3 :
						/* Accept */
						if (visual_event(1) & _MIMO_DOWN) {
							visual_button(_Context->x_auto703+20,_Context->y_auto703+160,110,30,vfh[2],26,0,_Context->msg_Accept[_Context->language],strlen(_Context->msg_Accept[_Context->language]),2);
							}
						else if (visual_event(1) & _MIMO_UP) {
							visual_button(_Context->x_auto703+20,_Context->y_auto703+160,110,30,vfh[2],26,0,_Context->msg_Accept[_Context->language],strlen(_Context->msg_Accept[_Context->language]),0);
							}
						if ( visual_event(1) &  0x20 ) {
						if ((_Context->keycode = on_Accept_event(_Context)) != -1) break;

							}
						continue;
					case	0x4 :
						/* Select */
						if (visual_event(1) & _MIMO_DOWN) {
							visual_button(_Context->x_auto703+350,_Context->y_auto703+160,100,30,vfh[2],27,24,_Context->msg_Select[_Context->language],strlen(_Context->msg_Select[_Context->language]),2);
							}
						else if (visual_event(1) & _MIMO_UP) {
							visual_button(_Context->x_auto703+350,_Context->y_auto703+160,100,30,vfh[2],27,24,_Context->msg_Select[_Context->language],strlen(_Context->msg_Select[_Context->language]),0);
							}
						if ( visual_event(1) &  0x20 ) {
						if ((_Context->keycode = on_Select_event(_Context)) != -1) break;

							}
						continue;
					case	0x5 :
						/* Cancel */
						if (visual_event(1) & _MIMO_DOWN) {
							visual_button(_Context->x_auto703+460,_Context->y_auto703+160,110,30,vfh[2],26,0,_Context->msg_Cancel[_Context->language],strlen(_Context->msg_Cancel[_Context->language]),2);
							}
						else if (visual_event(1) & _MIMO_UP) {
							visual_button(_Context->x_auto703+460,_Context->y_auto703+160,110,30,vfh[2],26,0,_Context->msg_Cancel[_Context->language],strlen(_Context->msg_Cancel[_Context->language]),0);
							}
						if ( visual_event(1) &  0x20 ) {
						if ((_Context->keycode = on_Cancel_event(_Context)) != -1) break;

							}
						continue;
					case	0xffffffff :
					default :
						_Context->focus_item=1;
					continue;
					}
				break;
			case	0x1b :
			case	0xd :
				break;
			case	0x9 :
			case	0x5 :
			if ( _Context->focus_item < _Context->focus_items ) {
				_Context->focus_item++;			} else {
				_Context->focus_item=1;
			}
			continue;
			case	0xb :
			if ( _Context->focus_item > 1 ) {
				_Context->focus_item--;
			} else {
				_Context->focus_item=5;
			}
			continue;
		default :
			continue;

			}
		break;
		}


	while( visual_kbhit() ) (void) visual_getch();
	return(0);

}

public	int	accept_modelfilename(
	char * pFileName)
{
	int	status=0;
	struct accept_modelfilename_context * _Context=(struct accept_modelfilename_context*) 0;
	status = accept_modelfilename_create(
	&_Context,
	 pFileName);
	if ( status != 0) return(status);
	status = accept_modelfilename_show(_Context);
		enter_modal();
	status = accept_modelfilename_focus(_Context);
		leave_modal();
	status = accept_modelfilename_hide(_Context);
	status = accept_modelfilename_remove(
	_Context,
	 pFileName);

	while( visual_kbhit() ) (void) visual_getch();
	return(status);
}
/* End of File */

#endif /* _vmload_c */
