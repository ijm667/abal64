
#ifndef _vlocate_c
#define _vlocate_c
/* ------------------------------------------------------------------------- */
/* Warning : this file has been generated and should not be modified by hand */
/* ------------------------------------------------------------------------- */
/* Sing Version   : 2.2a.0.08                                                */
/* Production     : C                                                        */
/* Project        : vlocate.xml                                              */
/* Target         : vlocate.c                                                */
/* Identification : 0.0-1334659659-14025.1163                                */
/* ------------------------------------------------------------------------- */

#include <stdio.h>
#include "visual.h"
#include "singprot.h"
#define en__Auto3573   ""
#define fr__Auto3573   ""
#define it__Auto3573   ""
#define es__Auto3573   ""
#define de__Auto3573   ""
#define nl__Auto3573   ""
#define pt__Auto3573   ""
#define xx__Auto3573   ""
#define en_Auto3574   "Widget References"
#define fr_Auto3574   "R�f�rences au Widget"
#define it_Auto3574   "Widget References"
#define es_Auto3574   "Widget References"
#define de_Auto3574   "Widget References"
#define nl_Auto3574   "Widget References"
#define pt_Auto3574   "Widget References"
#define xx_Auto3574   "Widget References"
#define en_Auto3575   "Name"
#define fr_Auto3575   "Nom"
#define it_Auto3575   "Nom"
#define es_Auto3575   "Name"
#define de_Auto3575   "Name"
#define nl_Auto3575   "Name"
#define pt_Auto3575   "Name"
#define xx_Auto3575   "Ainm"
#define en_Target   ""
#define en_RefList   "Widget.Method"
#define fr_RefList   "Widget.Method"
#define it_RefList   "Widget.Method"
#define es_RefList   "Widget.Method"
#define de_RefList   "Widget.Method"
#define nl_RefList   "Widget.Method"
#define pt_RefList   "Widget.Method"
#define xx_RefList   "Widget.Method"
#define en_RefBar   ""

private struct widget_reference_locator_context {
	int	status;
	int	keycode;
	int	focus_item;
	int	focus_items;
	int	window_col;
	int	window_row;
	int	language;
	char * msg_Auto3573[8];
	char * hint_Auto3573[8];
	int	x_Auto3573;
	int	y_Auto3573;
	int	w_Auto3573;
	int	h_Auto3573;
	char * msg_Auto3574[8];
	char * hint_Auto3574[8];
	char * msg_Auto3575[8];
	char * hint_Auto3575[8];
	char * hint_Target[8];
	char	buffer_Target[256];
	char * msg_RefList[8];
	char * hint_RefList[8];
	int	value_RefList;
	int	column_RefList;
	char	buffer_RefList[32][40];
	int	formsort_RefList;	/* sort 0 rien, positif num�ro de colonne, n�gatif, num�ro de colonne tri decroissant */
	char	*libsort_RefList;	/* table interm�diaire contenant la liste des �l�ments a trier */
	int	*tabsort_RefList;	/* num�ro d'ordre des elements tri�s */
	char	*indsort_RefList;	/* pointeur table interm�diaire de tri */
	int	nelt_RefList;	/* nombre d'el�ments alou�s dans tabsort */
	char * row_RefList;
	char * hint_RefBar[8];
	int	value_RefBar;
	int	limit_RefBar;
	int	max_RefBar;
	int	ctrlbuffer;
	void * pushbuffer;
	char	signature[2];
	};
static int	vfh[16];
private int on_RefList_show(struct widget_reference_locator_context * _Context);
private int on_Target_getfocus(struct widget_reference_locator_context * _Context);
private int on_RefList_event(struct widget_reference_locator_context * _Context);
private int on_RefBar_event(struct widget_reference_locator_context * _Context);
private int  sort_item_RefList(struct widget_reference_locator_context * _Context,int nelts);	/* returns resolved sort item */
private void start_sort_RefList(struct widget_reference_locator_context * _Context,int nelts);	/* allocate intermediate sort table */
private void sort_RefList(struct widget_reference_locator_context * _Context,int nelts);	/* sort the intermediate table into _Context->tabsort_RefList */
private int compare_RefList(struct widget_reference_locator_context * _Context,int a,int b);	/* compare the elements a et b of _Context->libsort_RefList */
/* ------------------- */
/* resolve sorted item */
/* ------------------- */
private int  sort_item_RefList(struct widget_reference_locator_context * _Context,int nelt)	/* returns resolved sort item */
{
	if (!( nelt )) { return(nelt); }
	else {
		return( _Context->tabsort_RefList[(nelt-1)] );
	}
}

/* ------------------------------------- */
/* allocate the intermediate sort table  */
/* ------------------------------------- */
private void start_sort_RefList(struct widget_reference_locator_context * _Context,int nelts)
{
if (_Context->nelt_RefList<nelts) {
	liberate(_Context->tabsort_RefList);
	if ((_Context->tabsort_RefList=allocate(nelts*sizeof(int))) == NULL) return;
	for (_Context->nelt_RefList=0;_Context->nelt_RefList<nelts;_Context->nelt_RefList++)
		{ _Context->tabsort_RefList[_Context->nelt_RefList]=_Context->nelt_RefList+1; };	/* initialisation tableau sans tri */
	};
if ((_Context->indsort_RefList=allocate(nelts*sizeof(char))) == NULL) return;
if ((_Context->libsort_RefList=allocate(nelts*40)) != NULL)
	memset(_Context->libsort_RefList,' ',nelts*40);
}

/* ---------------------------------------------------------- */
/* sort the intermediate table into _Context->tabsort_RefList */
/* ---------------------------------------------------------- */
private void sort_RefList(struct widget_reference_locator_context * _Context,int nelts)
{
int	i,j,k;
memset(_Context->indsort_RefList,0,nelts);

for (i=0;i<nelts;i++) {
	for (k=0;(k<nelts) && (_Context->indsort_RefList[k]);k++);
	if (k<nelts-1) {
		for (j=k+1;j<nelts;j++) {
			if ((!_Context->indsort_RefList[j]) && (compare_RefList(_Context,j,k)<0)) k=j;
			};	/* for j */
		};	/* if k<nbelt-1 */
	_Context->tabsort_RefList[i]=k+1;
	_Context->indsort_RefList[k]=1;
	};	/* for i */
liberate(_Context->libsort_RefList);
liberate(_Context->indsort_RefList);
}

/* -------------------------------------------------------- */
/* compare the elements a et b of _Context->libsort_RefList */
/* -------------------------------------------------------- */
private int compare_RefList(struct widget_reference_locator_context * _Context,int a,int b) {
int i,j;

i=memicmp(&_Context->libsort_RefList[40*a],&_Context->libsort_RefList[40*b],40);	/* comparaison zone 1 */
switch (_Context->formsort_RefList) {
	case 1:	/* Widget.Method ordre normal */
		return(i);

	case -1:	/* Widget.Method ordre inverse */
		return(-i);
	default:		/* pas de tri */
		if (a<b) return(-1);
		return(1);
	}; /* fin du switch */
}


public	int	widget_reference_locator_create(
	struct widget_reference_locator_context ** cptr,
	char * pTarget)
{

	int i;
	struct widget_reference_locator_context * _Context=(struct widget_reference_locator_context*)0;
	vfh[1] = visual_font("standard.fmf",12);
	vfh[2] = visual_font("arial2.fmf",10);
	vfh[3] = visual_font("arial3.fmf",10);
	vfh[4] = visual_font("arial4.fmf",10);
	vfh[5] = visual_font("arial5.fmf",10);
	vfh[6] = visual_font("arial6.fmf",10);
	vfh[7] = visual_font("font7.fmf",9);
	vfh[8] = visual_font("font8.fmf",9);
	vfh[9] = visual_font("font9.fmf",9);
	vfh[10] = visual_font("bmap0414.fmf",12);
	if (!(_Context = allocate( sizeof( struct widget_reference_locator_context ) ) ))
		return(27);
	else	*cptr=_Context;
	_Context->language = visual_national_language(-1);
	_Context->keycode=0;
	_Context->focus_item=1;
	_Context->focus_items=4;
	_Context->formsort_RefList=0;	/* valeur initiale du tri : aucun */
	_Context->libsort_RefList=NULL;	/* pas de table interm�diaire de tri */
	if ((_Context->tabsort_RefList=allocate(512*sizeof(int))) != NULL) {	/* taille de la table des elements */
		for (_Context->nelt_RefList=0;_Context->nelt_RefList<512;_Context->nelt_RefList++)
			{ _Context->tabsort_RefList[_Context->nelt_RefList]=_Context->nelt_RefList+1; };	/* initialisation tableau sans tri */
	} else _Context->nelt_RefList=0; /* si probl�me d'allocation */
_Context->indsort_RefList=NULL;	/* pas de pointeur de table interm�diaire */		_Context->max_RefBar=0;
		_Context->limit_RefBar=0;
		_Context->value_RefBar=0;
	if (_Context->keycode != 27 ) {
		strcpy(_Context->signature,"S");
		visual_signature(_Context->signature,_Context->signature);
		visual_transferin(_Context->buffer_Target, 255, pTarget);
		}
	for (i=0; i < 8; i++)_Context->msg_Auto3573[i]=" ";
	_Context->hint_Auto3573[0]=en__Auto3573;
	_Context->hint_Auto3573[1]=fr__Auto3573;
	_Context->hint_Auto3573[2]=it__Auto3573;
	_Context->hint_Auto3573[3]=es__Auto3573;
	_Context->hint_Auto3573[4]=de__Auto3573;
	_Context->hint_Auto3573[5]=nl__Auto3573;
	_Context->hint_Auto3573[6]=pt__Auto3573;
	_Context->hint_Auto3573[7]=xx__Auto3573;
	_Context->x_Auto3573=10;
	_Context->y_Auto3573=10;
	_Context->w_Auto3573=370;
	_Context->h_Auto3573=620;
	_Context->window_col=10;
	_Context->window_row=10;
	for (i=0; i < 8; i++)_Context->msg_Auto3574[i]=" ";
	_Context->msg_Auto3574[0]=en_Auto3574;
	_Context->msg_Auto3574[1]=fr_Auto3574;
	_Context->msg_Auto3574[2]=it_Auto3574;
	_Context->msg_Auto3574[3]=es_Auto3574;
	_Context->msg_Auto3574[4]=de_Auto3574;
	_Context->msg_Auto3574[5]=nl_Auto3574;
	_Context->msg_Auto3574[6]=pt_Auto3574;
	_Context->msg_Auto3574[7]=xx_Auto3574;
	for (i=0; i < 8; i++)_Context->msg_Auto3575[i]=" ";
	_Context->msg_Auto3575[0]=en_Auto3575;
	_Context->msg_Auto3575[1]=fr_Auto3575;
	_Context->msg_Auto3575[2]=it_Auto3575;
	_Context->msg_Auto3575[3]=es_Auto3575;
	_Context->msg_Auto3575[4]=de_Auto3575;
	_Context->msg_Auto3575[5]=nl_Auto3575;
	_Context->msg_Auto3575[6]=pt_Auto3575;
	_Context->msg_Auto3575[7]=xx_Auto3575;
	for (i=0; i < 8; i++)_Context->msg_RefList[i]=" ";
	_Context->msg_RefList[0]=en_RefList;
	_Context->msg_RefList[1]=fr_RefList;
	_Context->msg_RefList[2]=it_RefList;
	_Context->msg_RefList[3]=es_RefList;
	_Context->msg_RefList[4]=de_RefList;
	_Context->msg_RefList[5]=nl_RefList;
	_Context->msg_RefList[6]=pt_RefList;
	_Context->msg_RefList[7]=xx_RefList;
	if ( 0 > 0 ) { _Context->row_RefList=&_Context->buffer_RefList[0-1][0]; } ;
	_Context->max_RefBar=1;
	_Context->value_RefBar=1;
	_Context->limit_RefBar=1;
	_Context->value_RefBar=0;
	_Context->limit_RefBar=31;
	_Context->max_RefBar=widget_reference_count();
	_Context->ctrlbuffer=0;
	_Context->pushbuffer=visual_buffer(_Context->x_Auto3573,_Context->y_Auto3573,370+10,620+10);
	return(0);
}

public 	int	widget_reference_locator_hide(struct widget_reference_locator_context * _Context)
{
	int	result;
	if (!( _Context )) { return(0); }
	result=_Context->keycode;
	if ((_Context->pushbuffer != (void *) 0)
	&& (_Context->ctrlbuffer != 0)) {
		(void)visual_buffer_put(_Context->pushbuffer,_Context->x_Auto3573,_Context->y_Auto3573);
		_Context->ctrlbuffer=0;
		}

	return(result);
}

public	int	widget_reference_locator_remove(
	struct widget_reference_locator_context * _Context,
	char * pTarget)
{

	int	result;
	if (!( _Context )) { return(0); }
	result=_Context->keycode;
	if (_Context->keycode != 27 ) {
		strcpy(_Context->signature,"S");
		visual_signature(_Context->signature,_Context->signature);
		visual_transferout(_Context->buffer_Target, 255, pTarget);
		}
	if (_Context->pushbuffer != (void *) 0)
		_Context->pushbuffer = visual_drop(_Context->pushbuffer);
	_Context=liberate(_Context);

	return(result);
}
private int on_RefList_show(struct widget_reference_locator_context * _Context) {
	int i;
	for(i=1;i<=31;i++){
	if ( i > 0 ) { _Context->row_RefList=&_Context->buffer_RefList[i-1][0]; } ;
	get_widget_reference(i+_Context->value_RefBar,(_Context->row_RefList+0),40);
	}
		visual_table(_Context->x_Auto3573+10,_Context->y_Auto3573+80,330,530,vfh[1],0,28,_Context->msg_RefList[_Context->language],_Context->buffer_RefList,0x4401,"($)");
;
	return(-1);
}

public	int	widget_reference_locator_show(struct widget_reference_locator_context * _Context)
{
	_Context->language = visual_national_language(-1);
	if ((_Context->pushbuffer != (void *) 0)
	&& (!(_Context->ctrlbuffer))) {
		(void)visual_buffer_get(_Context->pushbuffer,_Context->x_Auto3573,_Context->y_Auto3573);
		_Context->ctrlbuffer=1;
		}
	visual_freeze();
	if (((_Context->status = visual_styled_element(_Context->x_Auto3573,_Context->y_Auto3573,370,620,GetWidgetStyle(0),GetWindowVersion(),strlen(GetWindowVersion()))) != 0)
) {
		visual_window(_Context->x_Auto3573,_Context->y_Auto3573,370,620,vfh[2],GetWindowVersion(),strlen(GetWindowVersion()),0x407);
		}
	visual_text(_Context->x_Auto3573+10,_Context->y_Auto3573+30,322,20,vfh[3],16,0,_Context->msg_Auto3574[_Context->language],strlen(_Context->msg_Auto3574[_Context->language]),1282);
	visual_text(_Context->x_Auto3573+10,_Context->y_Auto3573+60,42,16,vfh[2],16,0,_Context->msg_Auto3575[_Context->language],strlen(_Context->msg_Auto3575[_Context->language]),258);
	visual_frame(_Context->x_Auto3573+48,_Context->y_Auto3573+60,288+2,16+2,5);
	visual_text(_Context->x_Auto3573+48+1,_Context->y_Auto3573+60+1,288,16,vfh[1],0,28,_Context->buffer_Target,255,0);
	(void) on_RefList_show(_Context);
	visual_scrollbar(_Context->x_Auto3573+340,_Context->y_Auto3573+80,20,530,vfh[1],0,28,_Context->value_RefBar,_Context->limit_RefBar,_Context->max_RefBar,259);
	visual_thaw(_Context->x_Auto3573,_Context->y_Auto3573,370,620);

	return(0);
}

private int Target_help()
{
	switch (visual_national_language(-1)) {
		case 0 : 
			{
			char * mptr[2];
			mptr[0] = "This edit field provides the name of the widget for which the list ";
			mptr[1]="of references is to be located.   ";
			return(visual_online_help(mptr,2));
			}
			break;
		default : return(0);
		}
	return(0);
}

private int RefList_help()
{
	switch (visual_national_language(-1)) {
		case 0 : 
			{
			char * mptr[5];
			mptr[0] = "This control displays the actual list of references to the target ";
			mptr[1]="widget. These may be from widget methods or from forms methods of ";
			mptr[2]="any of the forms of the current project. The Widget Properties dialog box, of the corresponding reference may be ";
			mptr[3]="activated by clicking with the LEFT button of the mouse on the required widget reference expression. ";
			mptr[4]="  ";
			return(visual_online_help(mptr,5));
			}
			break;
		default : return(0);
		}
	return(0);
}

private int RefBar_help()
{
	switch (visual_national_language(-1)) {
		case 0 : 
			{
			char * mptr[2];
			mptr[0] = "The scroll bar may be used to scroll amongst the list of references ";
			mptr[1]="if this exceeds the visual page.   ";
			return(visual_online_help(mptr,2));
			}
			break;
		default : return(0);
		}
	return(0);
}

private int Auto3573action(struct widget_reference_locator_context * _Context) {
	int x=visual_event(7);
	int y=visual_event(6);
	do {
		do {

			while(visual_getch() != 256);
			}
		while( visual_kbhit() );
		visual_freeze();
			widget_reference_locator_hide(_Context);

		_Context->x_Auto3573 += (visual_event(7)-x);
		x=visual_event(7);
		if (_Context->x_Auto3573 < 0) { _Context->x_Auto3573=0; }
		_Context->y_Auto3573 += (visual_event(6)-y);
		y=visual_event(6);
		if (_Context->y_Auto3573 < 0) { _Context->y_Auto3573=0; }
			widget_reference_locator_show(_Context);

		visual_thaw(_Context->x_Auto3573,_Context->y_Auto3573,370,620);
		}
	while ((visual_event(1) & _MIMO_UP) != _MIMO_UP);
	return(-1);
}
private int on_RefList_event(struct widget_reference_locator_context * _Context) {
	int l;
	l=_Context->value_RefList;
	if(l>0){
		widget_reference_locator_hide(_Context);
;
	if ( l > 0 ) { _Context->row_RefList=&_Context->buffer_RefList[l-1][0]; } ;
	status_message_length(2,(_Context->row_RefList+0),40);
	if(visual_event(2)!=_MIMO_RIGHT)
	inspect_widget_reference(_Context->value_RefBar+l);
	else search_widget_reference(_Context->value_RefBar+l,_Context->buffer_Target,255);
		widget_reference_locator_show(_Context);
;
	}
	return(-1);
}
private int on_RefBar_event(struct widget_reference_locator_context * _Context) {
	on_RefList_show(_Context);
	return(-1);
}

private int on_RefBar_action(struct widget_reference_locator_context * _Context) {
	int status;
	int aty=visual_event(6);
	int toy=visual_event(6);
	if ( visual_event(1) !=_MIMO_DOWN) { return(0); }
	if ( visual_event(6) < (_Context->y_Auto3573+80+20) ) {
		do { while( visual_getch() != 256); } while ( visual_event(1)  != _MIMO_UP );
		_Context->value_RefBar -= 1;
		if (_Context->value_RefBar < 0) 
			_Context->value_RefBar = 0;
		(void) on_RefBar_event(_Context);
	visual_scrollbar(_Context->x_Auto3573+340,_Context->y_Auto3573+80,20,530,vfh[1],0,28,_Context->value_RefBar,_Context->limit_RefBar,_Context->max_RefBar,259);
		return(-1);
		}
	if ( visual_event(6) > (_Context->y_Auto3573+80+530-20) ) {
		do { while( visual_getch() != 256); } while ( visual_event(1)  != _MIMO_UP );
		_Context->value_RefBar += 1;
		if (!(_Context->max_RefBar))
			_Context->value_RefBar = 0;
		else if ((_Context->max_RefBar < _Context->limit_RefBar) 
		&& (_Context->value_RefBar >= _Context->max_RefBar)) 
			_Context->value_RefBar = (_Context->max_RefBar-1);
		else if (_Context->value_RefBar > (_Context->max_RefBar-_Context->limit_RefBar)) 
			_Context->value_RefBar = (_Context->max_RefBar-_Context->limit_RefBar);
		(void) on_RefBar_event(_Context);
	visual_scrollbar(_Context->x_Auto3573+340,_Context->y_Auto3573+80,20,530,vfh[1],0,28,_Context->value_RefBar,_Context->limit_RefBar,_Context->max_RefBar,259);
		return(-1);
		}
	visual_scrollbar(_Context->x_Auto3573+340,_Context->y_Auto3573+80,20,530,vfh[1],0,28,_Context->value_RefBar,_Context->limit_RefBar,_Context->max_RefBar,259);

	do {
		while(visual_getch() != 256);
		switch( visual_event(2) ) {
		case _MIMO_LEFT  :
		case _MIMO_RIGHT :
		aty = _Context->value_RefBar;
		_Context->value_RefBar = (((visual_event(6) - (_Context->y_Auto3573+80+20)) * _Context->max_RefBar) / (530 - (2 * 20)));
		if (_Context->value_RefBar < 0) 
			_Context->value_RefBar = 0;
		else if (_Context->value_RefBar > (_Context->max_RefBar-_Context->limit_RefBar)) 
			_Context->value_RefBar = (_Context->max_RefBar-_Context->limit_RefBar);
		if (_Context->value_RefBar != aty) {
		(void) on_RefBar_event(_Context);
	visual_scrollbar(_Context->x_Auto3573+340,_Context->y_Auto3573+80,20,530,vfh[1],0,28,_Context->value_RefBar,_Context->limit_RefBar,_Context->max_RefBar,259);
			}
			}
		}
	while (visual_event(1) != _MIMO_UP);
	switch( visual_event(2) ) {
		case _MIMO_WHEELUP   :
			_Context->value_RefBar -= (_Context->limit_RefBar/2);
			if (_Context->value_RefBar < 0) 
				_Context->value_RefBar = 0;
		(void) on_RefBar_event(_Context);
	visual_scrollbar(_Context->x_Auto3573+340,_Context->y_Auto3573+80,20,530,vfh[1],0,28,_Context->value_RefBar,_Context->limit_RefBar,_Context->max_RefBar,259);
			break;
		case _MIMO_WHEELDOWN :
			_Context->value_RefBar += (_Context->limit_RefBar/2);
			if (!(_Context->max_RefBar))
				_Context->value_RefBar = 0;
			else if ((_Context->max_RefBar < _Context->limit_RefBar) 
			&& (_Context->value_RefBar >= _Context->max_RefBar)) 
				_Context->value_RefBar = (_Context->max_RefBar-1);
			else if (_Context->value_RefBar > (_Context->max_RefBar-_Context->limit_RefBar)) 
				_Context->value_RefBar = (_Context->max_RefBar-_Context->limit_RefBar);
		(void) on_RefBar_event(_Context);
	visual_scrollbar(_Context->x_Auto3573+340,_Context->y_Auto3573+80,20,530,vfh[1],0,28,_Context->value_RefBar,_Context->limit_RefBar,_Context->max_RefBar,259);
			break;
		}
	visual_scrollbar(_Context->x_Auto3573+340,_Context->y_Auto3573+80,20,530,vfh[1],0,28,_Context->value_RefBar,_Context->limit_RefBar,_Context->max_RefBar,259);

	return(1);
}


public	int	widget_reference_locator_event(
struct widget_reference_locator_context * _Context){
	int	mx,my,mt,mb;
	if (visual_event(1) & _MIMO_ALT) {
		if (((mb = (visual_event(2) & 0x00FF)) >= 'a') && (mb <= 'z')) mb -= ('a' - 'A');
		return(-1);
		}
	if (!((mt = visual_event(1)) & 0x3030)) return(-1); 
	mx = visual_event(7);
	my = visual_event(6);
	switch (mt) {
	case _MIMO_UP :
	if ((mx >= (_Context->x_Auto3573+349) )  
	&&  (my >= (_Context->y_Auto3573+4) )  
	&&  (mx <= (_Context->x_Auto3573+366) )  
	&&  (my <= (_Context->y_Auto3573+20) )) {
		return(1);	/* Auto3573 */

		}
	if ((mx >= (_Context->x_Auto3573+333) )  
	&&  (my >= (_Context->y_Auto3573+4) )  
	&&  (mx <= (_Context->x_Auto3573+350) )  
	&&  (my <= (_Context->y_Auto3573+20) )) {
				if ( visual_event(1) & _MIMO_UP ) {
			visual_help("vlocate.htm");
			};
		return(-1);	/* Auto3573 */

		}
	if ((mx >= (_Context->x_Auto3573+317) )  
	&&  (my >= (_Context->y_Auto3573+4) )  
	&&  (mx <= (_Context->x_Auto3573+334) )  
	&&  (my <= (_Context->y_Auto3573+20) )) {
				if (visual_event(2) ==_MIMO_LEFT) {
			_Context->language++; _Context->language %=8;
			}
		else {
		if ( _Context->language > 0 ) { _Context->language--; _Context->language %=8; }
		else { _Context->language=7; }
			}
		(void) visual_national_language(_Context->language);
		widget_reference_locator_show(_Context);
		return(-1);	/* Auto3573 */

		}
	case _MIMO_DOWN :
		if ((mx >= (_Context->x_Auto3573+4) )  
		&&  (my >= (_Context->y_Auto3573+4) )  
		&&  (mx <= (_Context->x_Auto3573+302) )  
		&&  (my <= (_Context->y_Auto3573+20) )) {
			return( Auto3573action(_Context) );
			}
		}
	if (( mx >= (_Context->x_Auto3573+48) ) 
	&&  ( my >= (_Context->y_Auto3573+60) ) 
	&&  ( mx <= (_Context->x_Auto3573+48+288) ) 
	&&  ( my <= (_Context->y_Auto3573+60+16))) {
		return(2); /* Target */
		}
	if (( mx >= (_Context->x_Auto3573+10) ) 
	&&  ( my >= (_Context->y_Auto3573+80) ) 
	&&  ( mx <= (_Context->x_Auto3573+10+330) ) 
	&&  ( my <= (_Context->y_Auto3573+80+530))) {
		if ((_Context->value_RefList = ((visual_event(6) - (_Context->y_Auto3573+80)) / 16)) < 1)
			_Context->value_RefList=0;
else if (_Context->value_RefList > 31 )
			_Context->value_RefList=31;
		if ( _Context->value_RefList > 0 ) { _Context->row_RefList = &_Context->buffer_RefList[(_Context->value_RefList-1)][0]; } 
		_Context->column_RefList = (visual_event(7) - (_Context->x_Auto3573+10));
		if ( _Context->column_RefList < 320 ) {
			_Context->column_RefList = 1;
			}
		if ((!(_Context->value_RefList)) && (visual_event(1)==_MIMO_DOWN)) {	/* si clic sur titre */
			if (_Context->formsort_RefList==_Context->column_RefList) /* si colonne choisie concerne d�j� le tri */
				_Context->formsort_RefList=-_Context->column_RefList;	/* alors tri d�croissant */
			else
				_Context->formsort_RefList=_Context->column_RefList; };	/* sinon tri croissant */
		return(3); /* RefList */
		}
	if (( mx >= (_Context->x_Auto3573+340) ) 
	&&  ( my >= (_Context->y_Auto3573+80) ) 
	&&  ( mx <= (_Context->x_Auto3573+340+20) ) 
	&&  ( my <= (_Context->y_Auto3573+80+530))) {
		return(4); /* RefBar */
		}

	return(-1);
}
private int on_Target_getfocus(struct widget_reference_locator_context * _Context) {
	return(visual_getch());
	return(-1);
}


public	int	widget_reference_locator_focus(struct widget_reference_locator_context * _Context)
{
	int retcode=0;

	while( visual_kbhit() ) (void) visual_getch();

	while(1) {
		switch (_Context->focus_item) {
			case	0x2 :
				/* Target */
				_Context->keycode = on_Target_getfocus(_Context);
			break;
			case	0x4 :
				/* RefBar */
				_Context->keycode=visual_getch();
				break;

			default :
				_Context->keycode=5;
			}
retry:
		switch (_Context->keycode) {
		case	0xc :
			_Context->focus_item=1;
			widget_reference_locator_show(_Context);
			continue;
		case	0x2 :
			_Context->focus_item=4;
			widget_reference_locator_show(_Context);
			continue;
		case	0x100 :
			if ((retcode=widget_reference_locator_event(_Context)) == -1)
				continue;
			if (( visual_event(1) == _MIMO_DOWN ) && ( visual_event(2) == _MIMO_RIGHT)) {
				switch ((_Context->focus_item = retcode)) {
					case	0x2 :
						if (Target_help(_Context) != 0) { continue; }
						else { break; }
					case	0x3 :
						if (RefList_help(_Context) != 0) { continue; }
						else { break; }
					case	0x4 :
						if (RefBar_help(_Context) != 0) { continue; }
						else { break; }
					}
				}
			switch ((_Context->focus_item = retcode)) {
				case	0x1 :
					/* Auto3573 */
					if (visual_event(1) & _MIMO_UP) {
						_Context->keycode=27;
						break;
					} else {
						continue;
						}
				case	0x2 :
					/* Target */
					continue;
				case	0x3 :
					/* RefList */
					if ( visual_event(1) &  0x10 ) {
					if ((_Context->keycode = on_RefList_event(_Context)) != -1) break;

						}
					continue;
				case	0x4 :
					/* RefBar */
					(void) on_RefBar_action(_Context);
					continue;
				case	0xffffffff :
				default :
					_Context->focus_item=1;
				continue;
				}
				break;
		case	0x1b :
			break;
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
				_Context->focus_item=4;
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

public	int	widget_reference_locator(
	char * pTarget)
{
	int	status=0;
	struct widget_reference_locator_context * _Context=(struct widget_reference_locator_context*) 0;
	status = widget_reference_locator_create(
	&_Context,
	 pTarget);
	if ( status != 0) return(status);
	status = widget_reference_locator_show(_Context);
		enter_modal();
	status = widget_reference_locator_focus(_Context);
		leave_modal();
	status = widget_reference_locator_hide(_Context);
	status = widget_reference_locator_remove(
	_Context,
	 pTarget);

	while( visual_kbhit() ) (void) visual_getch();
	return(status);
}
/* End of File */

#endif /* _vlocate_c */
