
#ifndef _vsave_c
#define _vsave_c
/* ------------------------------------------------------------------------- */
/* Warning : this file has been generated and should not be modified by hand */
/* ------------------------------------------------------------------------- */
/* Sing Version   : 1.1a.0.03  */
/* Production     : C  */
/* Project        : vsave.sng  */
/* Target         : vsave.c  */
/* Identification : 0.0-1065019112-3524.3523 */
/* ------------------------------------------------------------------------- */

#include <stdio.h>
#include "visual.h"

private struct accept_form_save_context {
	int	keycode;
	int	focus_item;
	int	focus_items;
	int	window_col;
	int	window_row;
	char	buffer_FileName[71];
	int	ctrlbuffer;
	void * pushbuffer;
	char	signature[2];
	} * _Context;

private	int	accept_form_save_create(
	char * pFileName)
{

	if (!(_Context = allocate( sizeof( struct accept_form_save_context ) ) ))
		return(27);
	_Context->keycode=0;
	_Context->focus_item=1;
	_Context->focus_items=5;
	if (_Context->keycode != 27 ) {
		strcpy(_Context->signature,"S");
		visual_signature(_Context->signature,_Context->signature);
		visual_transferin(_Context->buffer_FileName, 70, pFileName);
		}
	_Context->window_col=0;
	_Context->window_row=0;
	_Context->ctrlbuffer=0;
	_Context->pushbuffer=visual_buffer(100,210,600+10,180+10);
	return(0);
}

private	int	accept_form_save_hide()
{
	if ((_Context->pushbuffer != (void *) 0)
	&& (_Context->ctrlbuffer != 0)) {
		(void)visual_buffer_put(_Context->pushbuffer,100,210);
		_Context->ctrlbuffer=0;
		}

	return(0);
}

private	int	accept_form_save_remove(
	char * pFileName)
{

	int	result=_Context->keycode;
	if (_Context->keycode != 27 ) {
		strcpy(_Context->signature,"S");
		visual_signature(_Context->signature,_Context->signature);
		visual_transferout(_Context->buffer_FileName, 70, pFileName);
		}
	if (_Context->pushbuffer != (void *) 0)
		_Context->pushbuffer = visual_drop(_Context->pushbuffer);
	_Context=liberate(_Context);

	return(result);
}

private	int	accept_form_save_show()
{
	if ((_Context->pushbuffer != (void *) 0)
	&& (!(_Context->ctrlbuffer))) {
		(void)visual_buffer_get(_Context->pushbuffer,100,210);
		_Context->ctrlbuffer=1;
		}
	visual_freeze();
	visual_window(100,210,600,180,2,"Sing : Version 1.0a",19,0x403);
	visual_image(103,232,592,154,"skin.gif",4);
	visual_text(120,250,560,20,2,34,0,"Name of Form to Save",20,258);
	visual_frame(120,270,560+2,16+2,5);
	visual_text(120+1,270+1,560,16,1,34,0,_Context->buffer_FileName,70,0);
	visual_button(120,330,110,30,2,34,0,"&Accept",7,0);
	visual_button(570,330,110,30,2,34,0,"&Cancel",7,0);
	visual_button(460,330,100,30,2,27,24,"&Select",7,0);
	visual_image(322,318,50,50,"formicon.gif",0);
	visual_thaw(100,210,600,180);

	return(0);
}
private int on_Accept_event() {
	return(13);
	return(-1);
}
private int on_Cancel_event() {
	return(27);
	return(-1);
}
private int on_Select_event() {
	use_file_selector(_Context->buffer_FileName,WIDGET_FILE_SNG);
	accept_form_save_show();
	return(-1);
}


private	int	accept_form_save_event()
{
	int	mx,my,mt,mb;
	if (!((mt = visual_event(1)) & 0x3031)) return(-1); 
	mx = visual_event(7);
	my = visual_event(6);
	if ((mx >= 679 )  
	&&  (my >= 214 )  
	&&  (mx <= 695 )  
	&&  (my <= 230 )) {
		return(1);	/* auto2 */

		}
	if ((mx >= 663 )  
	&&  (my >= 214 )  
	&&  (mx <= 679 )  
	&&  (my <= 230 )) {
		if ( visual_event(1) & _MIMO_UP ) {
			visual_help("vsave.htm");
			};
		return(-1);	/* auto2 */

		}
	if (( mx >= 120 ) 
	&&  ( my >= 270 ) 
	&&  ( mx <= 680 ) 
	&&  ( my <= 286 )) {
		return(2); /* FileName */
		}
	if (( mx >= 120 ) 
	&&  ( my >= 330 ) 
	&&  ( mx <= 230 ) 
	&&  ( my <= 360 )) {
		return(3); /* Accept */
		}
	if (( mx >= 570 ) 
	&&  ( my >= 330 ) 
	&&  ( mx <= 680 ) 
	&&  ( my <= 360 )) {
		return(4); /* Cancel */
		}
	if (( mx >= 460 ) 
	&&  ( my >= 330 ) 
	&&  ( mx <= 560 ) 
	&&  ( my <= 360 )) {
		return(5); /* Select */
		}

	return(-1);
}


private	int	accept_form_save_focus()
{
	int retcode=0;

	while( visual_kbhit() ) (void) visual_getch();

	while(1) {
		switch (_Context->focus_item) {
			case	0x2 :
				/* FileName */
				_Context->keycode = visual_edit(120+1,270+1,560,16,1,_Context->buffer_FileName,70);
				break;
			case	0x3 :
				/* Accept */
				visual_button(120,330,110,30,2,34,0,"&Accept",7,2);
				_Context->keycode = visual_getch();
				visual_button(120,330,110,30,2,34,0,"&Accept",7,0);
				break;
			case	0x4 :
				/* Cancel */
				visual_button(570,330,110,30,2,34,0,"&Cancel",7,2);
				_Context->keycode = visual_getch();
				visual_button(570,330,110,30,2,34,0,"&Cancel",7,0);
				break;
			case	0x5 :
				/* Select */
				visual_button(460,330,100,30,2,27,24,"&Select",7,2);
				_Context->keycode = visual_getch();
				visual_button(460,330,100,30,2,27,24,"&Select",7,0);
				break;

			default :
				_Context->keycode=5;
			}
retry:
		switch (_Context->keycode) {
		case	0xc :
			_Context->focus_item=1;
			accept_form_save_show();
			continue;
		case	0x2 :
			_Context->focus_item=5;
			accept_form_save_show();
			continue;
			case	0x100 :
				if (visual_event(1) & _MIMO_ALT) { _Context->keycode=visual_event(2); goto retry; }
				if ((retcode=accept_form_save_event()) == -1)
					continue;
				switch ((_Context->focus_item = retcode)) {
					case	0x1 :
						/* auto2 */
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
							visual_button(120,330,110,30,2,34,0,"&Accept",7,8);
							}
						else if (visual_event(1) & _MIMO_UP) {
							visual_button(120,330,110,30,2,34,0,"&Accept",7,0);
							}
						if ( visual_event(1) &  0x20 ) {
						if ((_Context->keycode = on_Accept_event()) != -1) break;

							}
						continue;
					case	0x4 :
						/* Cancel */
						if (visual_event(1) & _MIMO_DOWN) {
							visual_button(570,330,110,30,2,34,0,"&Cancel",7,8);
							}
						else if (visual_event(1) & _MIMO_UP) {
							visual_button(570,330,110,30,2,34,0,"&Cancel",7,0);
							}
						if ( visual_event(1) &  0x20 ) {
						if ((_Context->keycode = on_Cancel_event()) != -1) break;

							}
						continue;
					case	0x5 :
						/* Select */
						if (visual_event(1) & _MIMO_DOWN) {
							visual_button(460,330,100,30,2,27,24,"&Select",7,8);
							}
						else if (visual_event(1) & _MIMO_UP) {
							visual_button(460,330,100,30,2,27,24,"&Select",7,0);
							}
						if ( visual_event(1) &  0x66 ) {
						if ((_Context->keycode = on_Select_event()) != -1) break;

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
		case 0x841 : 
		case 0x861 : 
		/* Accept */
		visual_button(120,330,110,30,2,34,0,"&Accept",7,8);
		visual_button(120,330,110,30,2,34,0,"&Accept",7,0);
		_Context->focus_item=3;
		if ((_Context->keycode = on_Accept_event()) != -1) break;
		continue;
		case 0x843 : 
		case 0x863 : 
		/* Cancel */
		visual_button(570,330,110,30,2,34,0,"&Cancel",7,8);
		visual_button(570,330,110,30,2,34,0,"&Cancel",7,0);
		_Context->focus_item=4;
		if ((_Context->keycode = on_Cancel_event()) != -1) break;
		continue;
		case 0x853 : 
		case 0x873 : 
		/* Select */
		visual_button(460,330,100,30,2,27,24,"&Select",7,8);
		visual_button(460,330,100,30,2,27,24,"&Select",7,0);
		_Context->focus_item=5;
		if ((_Context->keycode = on_Select_event()) != -1) break;
		continue;
		default :
			continue;

			}
		break;
		}


	while( visual_kbhit() ) (void) visual_getch();
	return(0);

}

public	int	accept_form_save(
	char * pFileName)
{
	int	status=0;
	status = accept_form_save_create(
	 pFileName);
	if ( status != 0) return(status);
	status = accept_form_save_show();
		enter_modal();
	status = accept_form_save_focus();
		leave_modal();
	status = accept_form_save_hide();
	status = accept_form_save_remove(
	 pFileName);

	while( visual_kbhit() ) (void) visual_getch();
	return(status);
}
/* End of File */

#endif /* _vsave_c */
