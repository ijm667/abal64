#ifndef	_SINGEXPO_C
#define	_SINGEXPO_C

#include <stdio.h>
#include "sing.h"
#include "palet.h"
#include "stdpsu.h"
#include "guimouse.h"
#include "singrel.h"
#include "singpix.h"
#include "allocate.h"
#define	_SYSTEM_IMAGES
#include "sysimage.h"
#include "stdimage.h"
#include "singlang.h"
#include "singedit.h"
#include "singprod.h"
#include "singpars.h"

#include "singpal.h"
#include "singbar.h"
#include "singmenu.h"
#include "singtool.h"
#include "singtext.h"
#include "singimg.h"
#include "singdesk.h"

#include "vconf.h"
#include "visual.h"
#include "singconf.h"
#include "singexpo.h"
#include "singc.h"

#include "singappl.h"
extern	struct	application_parameters Application;
extern	struct c_configuration C_Config;

extern	char	namebuffer[];
extern	char	workbuffer[];
extern	struct	screen_control	Context;
extern	char *	font_name[];

#define	_STANDARD_RGB	system_palette


private	char *	language_instance_prefix="\0";
private	char *	language_constant_prefix="\0";
private	char *	language_hint_prefix="\0";

public	int	TargetBiosMode=3;
public	int	TargetBiosWidth=800;
public	int	TargetBiosHeight=600;
public	int	WindowCol=0;
public	int	WindowRow=0;
public	int	zWindowCol=0;
public	int	zWindowRow=0;
public	int	ZeroWindow=0;	
public	int	FixedWindows=1;

public	char *	ZeroWindowLine=(char *) 0;
public	char *	ZeroWindowColumn=(char *) 0;

public	struct window_control  ProductionWindow;

private	char *	EmptyString=AbalNulString;

private	int	variable_macros=0;
private	char 	wxbuffer[256];
private	char 	wybuffer[256];
private	char 	wwbuffer[256];
private	char 	whbuffer[256];

private	struct bios_mode {
	int	w;
	int 	h;
	} BiosDimensions[6]= {
	320,200,
	640,400,
	640,480,
	800,600,
	1024,768,
	1280,1024
	};

#ifdef	WIN32
public	char *	keyword_modext  ="modext.o";
public	char *	keyword_modexto  ="modexto.o";
#else
public	char *	keyword_modext  ="/opt/prologue/modext/modext.o";
public	char *	keyword_modexto  ="/opt/prologue/modext/modexto.o";
#endif
public	char *	keyword_include	="#include";
public	char *	keyword_use	="#use";
public	char *	keyword_user	="#user";
public	char *	keyword_this	="this";
public	char *	keyword_thisform="thisform";
public	char *	keyword_exit	="exit";
public	char *	keyword_keycode	="keycode";
public	char *	keyword_dcl 	="dcl";
public	char *	keyword_ptr 	="ptr";
public	char *	keyword_field	="field";
public	char *	keyword_oncreate="oncreate";
public	char *	keyword_onremove="onremove";
public	char *	keyword_onfocus	="onfocus";
public	char *	keyword_onshow	="onshow";
public	char *	keyword_onhide	="onhide";
public	char *	keyword_onevent	="onevent";
public	char *	keyword_focusitem ="focusitem";
public	char *	keyword_FormResize = "FormResize";
public	char *	keyword_w_BaseResize = "w_BaseResize";
public	char *	keyword_h_BaseResize = "h_BaseResize";

public	struct form_item * locate_widget( char * nptr );
public 	void	linefeed( FILE * handle );
public 	void	c_comment( FILE * handle, char * vptr , int indentation );
public	void	calculate_edit_lines( struct window_control * iptr, int * nbc, int * nbl );
public	void	calculate_form_lines( struct window_control * iptr, int * nbc, int * nbl );
public	char *	forms_modules(struct form_control * fptr,char * sptr);

public	int	SignatureItems=0;
public	char	Signature[512];


/*	-----------------------------------	*/
/*	d e t e c t _ w i d g e t _ h e l p 	*/
/*	-----------------------------------	*/
public	int	detect_widget_help(struct form_item * iptr)
{
	int	i;
	struct	form_control * fptr;

	/* no widget : no help */
	/* ------------------- */
	if (!( iptr )) return(0);

	/* no form   : no help */
	/* ------------------- */
	else if (!( fptr = iptr->parent )) return(0);

	/* not international : test slot zero only */
	/* --------------------------------------- */
	else if (!( fptr->formflags & _INTERNATIONAL )) {
		if (!( method_is_valid(iptr->document[0]) ))
			return(0);
		else	return(1);
		}

	/* not multilingual : test required language slot */
	/* ---------------------------------------------- */
	else if (!( fptr->formflags & _MULTILINGUAL )) {
		if (!( method_is_valid(iptr->document[fptr->natlang]) ))
			return(0);
		else	return(1);
		}

	/* multilingual : detect presence of help */
	/* -------------------------------------- */
	else	{
		for (i=0; i < MAXNATLANG; i++ )
			if ( method_is_valid(iptr->document[i]) )
				return(1);
		return(0);
		}
}

public	void	get_window_alignment( int * x, int *y )
{
	*x = WindowCol;
	*y = WindowRow;
	return;
}


public	void	set_zero_window_line( char * sptr, int v )
{
	if (!( ZeroWindowLine )) {
		ZeroWindowLine = allocate_string( sptr );
		zWindowRow = v;
		}
	return;
}

public	void	set_zero_window_column( char * sptr, int v )
{
	if (!( ZeroWindowColumn )) {
		ZeroWindowColumn = allocate_string( sptr );
		zWindowCol = v;
		}
	return;
}



public	int	add_interface_item(int items, struct form_item * iptr, int deftype)
{
	int		wt;
	if (!( iptr->Contents.interfaceitem ))
		return( items );
	else	{
		if (!( wt = make_widget_symbol( iptr->Contents.datatype ) ))
			wt = deftype;
		Signature[items] = (wt & 0x00FF);
		items++;
		Signature[items] = 0;
		}
	return( items );
}

struct	window_control * connect_production_window()
{
	return( & ProductionWindow );
}

public	void	exclusion_token( FILE * h, char * tptr )
{
	while ( *tptr ) {
		if ((( *tptr >= 'a' ) &&  ( *tptr <= 'z' ))
		||  (( *tptr >= 'A' ) &&  ( *tptr <= 'Z' ))
		||  (( *tptr >= '0' ) &&  ( *tptr <= '9' )))
			fprintf(h,"%c",*tptr);
		else	fprintf(h,"_");
		tptr++;
		}
	return;
}



public	void	open_file_exclusion( FILE * h, char * token )
{
	fprintf(h,"#ifndef _"); exclusion_token(h,token); linefeed(h);
	fprintf(h,"#define _"); exclusion_token(h,token); linefeed(h);
	return;
}

public	void	close_file_exclusion( FILE * h, char * token )
{
	fprintf(h,"#endif  _");  exclusion_token(h,token); linefeed(h);
	return;
}


public	void	generate_comment(FILE * h, char * mptr, int l )
{
	switch ( l ) {
		case	_GENERATE_HTML		:
		case	_GENERATE_BAL		:
		case	_GENERATE_ABAL		:
			abal_comment(h,mptr,0);
			break;
		case	_GENERATE_C		:
		case	_GENERATE_CPP		:
			c_comment(h,mptr,0);
			break;
		case	_GENERATE_JAVA		:
			break;
		case	_GENERATE_MAKE		:
			fprintf(h,"# %s ",mptr);
			linefeed(h);
		}
	return;
}

public	void	generate_comment_line(FILE * h, int n, int language)
{
	int	i;
	for (i=0; i < n; i++ ) namebuffer[i] = '-';
	namebuffer[i] = 0;
	generate_comment(h,namebuffer,language);
	return;
}

public	void	makefile_comment( FILE * h, char * mptr )
{
	generate_comment(h,mptr,_GENERATE_MAKE);
	return;
}

public	void	production_banner( FILE * h,int language, char * modelname, char * target )
{
	char * lptr;
	char * getpatternversion();
	long   time(long *);
	char * sptr="Warning : this file has been generated and should not be modified by hand";
	generate_comment_line(h,strlen(sptr),language);
	generate_comment(h,sptr,language);
	generate_comment_line(h,strlen(sptr),language);
	if (language==_GENERATE_C) {
		memset(namebuffer,' ',strlen(sptr));
		namebuffer[strlen(sptr)]=0;
		};
	sprintf(namebuffer,"Sing Version   : %s ",getpatternversion());
	if (language==_GENERATE_C)
		namebuffer[strlen(namebuffer)]=' ';
	generate_comment(h,namebuffer,language);
	if (language==_GENERATE_C) {
		memset(namebuffer,' ',strlen(sptr));
		namebuffer[strlen(sptr)]=0;
		};
	switch ( language ) {
		case	_GENERATE_BAL		: lptr = "ABAL";   	break;
		case	_GENERATE_ABAL		: lptr = "ABAL++"; 	break;
		case	_GENERATE_C		: lptr = "C";	 	break;
		case	_GENERATE_CPP		: lptr = "C++"; 	break;
		case	_GENERATE_HTML		: lptr = "HTML"; 	break;
		case	_GENERATE_JAVA		: lptr = "JAVA"; 	break;
		case	_GENERATE_MAKE		: lptr = "MAKE";	break;
		default				: lptr = "WHAT";	break;
		}
	sprintf(namebuffer,"Production     : %s ",lptr );
	if (language==_GENERATE_C)
		namebuffer[strlen(namebuffer)]=' ';
	generate_comment(h,namebuffer,language);
	if (language==_GENERATE_C) {
		memset(namebuffer,' ',strlen(sptr));
		namebuffer[strlen(sptr)]=0;
		};
	sprintf(namebuffer,"Project        : %s ",modelname);
	if (language==_GENERATE_C)
		namebuffer[strlen(namebuffer)]=' ';
	generate_comment(h,namebuffer,language);
	if (language==_GENERATE_C) {
		memset(namebuffer,' ',strlen(sptr));
		namebuffer[strlen(sptr)]=0;
		};
	sprintf(namebuffer,"Target         : %s ",target);
	if (language==_GENERATE_C)
		namebuffer[strlen(namebuffer)]=' ';
	generate_comment(h,namebuffer,language);
	if (language==_GENERATE_C) {
		memset(namebuffer,' ',strlen(sptr));
		namebuffer[strlen(sptr)]=0;
		}
#ifdef	UNIX
	sprintf(namebuffer,"Identification : %u.%u-%lu-%u.%u",getgid(),getuid(),time((long*)0),getpid(),getppid());
#else
	sprintf(namebuffer,"Identification : 0.0-%lu-0.0",time((long*)0));
#endif
	if (language==_GENERATE_C)
		namebuffer[strlen(namebuffer)]=' ';
	generate_comment(h,namebuffer,language);
	generate_comment_line(h,strlen(sptr),language);
	linefeed(h);
	return;
}

public	void	calculate_window_alignment( struct form_item * iptr )
{
	int	px;
	int	py;

	if ( ZeroWindow )
		return;
	else if (!( iptr ))
		return;

	switch ( TargetBiosMode ) {
		case	0 : px =   320; py =  200; break;
		case	1 : px =   640; py =  400; break;
		case	2 : px =   640; py =  480; break;
		case	3 : px =   800; py =  600; break;
		case	4 : px =  1024; py =  768; break;
		case	5 : px =  1280; py = 1024; break;
		case	7 : 
			TargetBiosWidth  = px = iptr->Contents.w; 
			TargetBiosHeight = py = iptr->Contents.h; 
			break;
		default	  : px =   800; py =  600; break;
		}

	/* Using Alignment */
	WindowCol = iptr->Contents.x;
	WindowRow = iptr->Contents.y;
	switch (( iptr->Contents.align & 0x000F )) {
		case	_A_LEFT	:
			WindowCol = 0;
			WindowRow = iptr->Contents.y;
			break;
		case	_A_RIGHT	:
			WindowCol = (px - iptr->Contents.w);
			WindowRow = iptr->Contents.y;
			break;
		case	_A_CENTER	:
			WindowCol = ((px - iptr->Contents.w)/2);;
			WindowRow = ((py - iptr->Contents.h)/2);;
			break;
		case	_A_NONE	:
		default		:
			WindowCol = iptr->Contents.x;
			WindowRow = iptr->Contents.y;
			break;
		}
	switch (( iptr->Contents.align & 0x00F0 )) {
		case	_A_TOP	:
			WindowRow = 0;
			break;
		case	_A_BOTTOM:
			WindowRow = (py - iptr->Contents.h);
			break;
		case	_A_MIDDLE:
			WindowRow = ((py - iptr->Contents.h)/2);
			break;
		}
	WindowCol -= iptr->Contents.x;
	WindowRow -= iptr->Contents.y;
	return;
}


public	void	clear_window_alignment()
{
	WindowCol = 0;
	WindowRow = 0;
	return;
}


/*	General Source Production Methods 	*/
/*	---------------------------------	*/

private	void	file_symbol(FILE * h, char * token )
{
	fprintf(h,"_");
	for ( ; *token != 0; token++) {
		switch ( *token ) {
			case	' '	:
			case	'\r'	:
			case	'\n'	:
			case	'\t'	:
			case	'/'	:
			case	':'	:
			case	'.'	:
			case	'-'	:
			case	'\\'	:
				fprintf(h,"_");
				continue;
			default		:
				fprintf(h,"%c",*token);
				continue;
			}
		}
	return;
}
		
public 	void	linefeed( FILE * handle )
{
#ifdef	UNIX
	fprintf(handle,"\n");
#else
	fprintf(handle,"\n");
#endif
	return;
}

public 	void	file_exclusion(FILE * h, char * token )
{
	linefeed(h); fprintf(h,"#ifndef "); file_symbol(h,token);
	linefeed(h); fprintf(h,"#define "); file_symbol(h,token);
	linefeed(h);
	return;
}

private	char *	remove_space(char * sptr)
{
	while (1) {
		switch ( *sptr) {
			case	0	:
				return((char *) 0);
			case	' ' 	:
			case	'\r'	:
			case	'\n'	:
			case	'\t'	:
			case	','	:
			case	';'	:
				sptr++;
				continue;
			default		:
				return( sptr );
			}
		}
}

private	char *	seek_space(char * sptr)
{
	while (1) {
		switch ( *sptr) {
			case	0	:
			case	' ' 	:
			case	'\r'	:
			case	'\n'	:
			case	'\t'	:
			case	','	:
			case	';'	:
				return( sptr );
			default		:
				sptr++;
				continue;
			}
		}
}

public	void	standard_dependance( FILE * h, char * sptr,  char * mptr )
{
	char	*	xptr;
	char		c;
	if (!( sptr ))
		return;

	while ( *sptr != 0 ) {
		if (!( sptr = remove_space( sptr ) ))
			break;
		else if (!( xptr = seek_space( sptr ) ))
			break;
		c = *xptr;
		*xptr = 0;
		fprintf(h,"%s %c%s%c",mptr,0x0022,sptr,0x0022);
		linefeed(h);
		*xptr = c;
		sptr  = xptr;
		}
	return;
}

public 	void	close_exclusion(FILE * h, char * token )
{
	linefeed(h); fprintf(h,"#endif /* "); file_symbol(h,token); fprintf(h," */");
	linefeed(h);
	return;
}

public 	void	indent( FILE * handle, int indentation )
{
	int	i;
	for (i=0; i < indentation; i++ )
		fprintf(handle,"\t");
	return;
}

public	void	quoted_string(FILE * handle, char * sptr )
{
	if (!( sptr ))
		sptr = EmptyString;
	else if (!( *sptr ))
		sptr = EmptyString;
	fprintf(handle,"%c%s%c",0x0022,sptr,0x0022);
	return;
}

public 	void	quoted_filename(FILE * handle, char * sptr )
{

	if (!( sptr ))
		sptr = EmptyString;
	fprintf(handle,"%c",0x0022);
	while ( *sptr ) {
		if ( *sptr == 0x005C )
			/* double the backslash ! */
			fprintf(handle,"%c",0x005C);
		fprintf(handle,"%c",*(sptr++));
		}
	fprintf(handle,"%c",0x0022);
	return;
}

public 	void	font_parameter(FILE * handle, struct form_item * iptr )
{
	if (!( iptr->parent ))
		fprintf(handle,",%u",iptr->Contents.font);
	else	{
		switch (iptr->parent->language) {
			case	_GENERATE_C		:
			case	_GENERATE_CPP		:
			case	_GENERATE_JAVA		:
				fprintf(handle,",");
				logical_c_font( handle, iptr );
				break;
			case	_GENERATE_HTML		:
			case	_GENERATE_BAL		:
			case	_GENERATE_ABAL		:
				fprintf(handle,",");
				logical_abal_font(handle,iptr->Contents.font);
				break;

			}
		}
}

public	int	widget_foreground( struct form_item * iptr )
{
	return( iptr->Contents.colour & 0x00FF );
}

public	int	widget_background( struct form_item * iptr )
{
	return( (iptr->Contents.colour & 0xFF00) >> 8) ;
}

public 	void	colour_parameter(FILE * handle, struct form_item * iptr )
{
	fprintf(handle,",%u,%u",
		widget_foreground(iptr),
		widget_background(iptr));
	return;
}

public 	void	zoned_colour_parameter(
			FILE * handle, 
			struct form_item * iptr,
			char *	expression )
{
	switch ( iptr->Contents.align & 0x0003 ) {
		case	0	:	/* WHITE  */
			fprintf(handle,",%u,%u",
				widget_foreground(iptr),
				standard_white);
			break;
			
		case	1	:	/* ZONEW  */
			fprintf(handle,",%u,(%u-%s)",
				widget_foreground(iptr),
				standard_white,expression);
			break;
			
		case	2	:	/* COLOUR */
			fprintf(handle,",%u,%u",
				widget_foreground(iptr),
				widget_background(iptr));
			break;
		case	3	:	/* ZONEC */
			fprintf(handle,",%u,(%u-%s)",
				widget_foreground(iptr),
				widget_background(iptr),expression);
			break;
			
		}

	return;
}

public 	void	align_parameter(FILE * handle, struct form_item * iptr )
{
	fprintf(handle,",%u",iptr->Contents.align);
	return;
}

public 	void	length_parameter(FILE * handle, int l )
{
	fprintf(handle,",%u",l);
	return;
}

public	void	string_length( FILE * h, char * sptr)
{
	if (!( sptr ))
		fprintf(h,"0");
	else	fprintf(h,"%u",strlen(sptr));
}

public 	void	variable_value(FILE * handle, char * sptr )
{
	if ( variable_macros == _GENERATE_BAL )
		abal_macro_conversion(handle,sptr,(char *) 0);
	else if ( variable_macros == _GENERATE_C )
		c_macro_conversion(handle,sptr,(char *) 0);
	else if ( variable_macros == _GENERATE_CPP )
		cpp_macro_conversion(handle,sptr,(char *) 0);
	else 	fprintf(handle,"%s",sptr);
	return;
}

public 	void	variable_length(FILE * handle, char * sptr )
{
	if ( variable_macros == _GENERATE_BAL ) {
		fprintf(handle,"Len(");
		abal_macro_conversion(handle,sptr,(char *) 0);
		fprintf(handle,")");
		}
	else if ( variable_macros == _GENERATE_C ) {
		fprintf(handle,"strlen(");
		c_macro_conversion(handle,sptr,(char *) 0);
		fprintf(handle,")");
		}
	else if ( variable_macros == _GENERATE_CPP ) {
		fprintf(handle,"strlen(");
		cpp_macro_conversion(handle,sptr,(char *) 0);
		fprintf(handle,")");
		}
	else 	fprintf(handle,"%s",strlen(sptr));
	return;
}

public 	void	variable_lendol(FILE * handle, char * sptr )
{
	if ( variable_macros == _GENERATE_BAL ) {
		fprintf(handle,"Len$(");
		abal_macro_conversion(handle,sptr,(char *) 0);
		fprintf(handle,")");
		}
	else if ( variable_macros == _GENERATE_C ) {
		fprintf(handle,"strlen(");
		c_macro_conversion(handle,sptr,(char *) 0);
		fprintf(handle,")");
		}
	else if ( variable_macros == _GENERATE_CPP ) {
		fprintf(handle,"strlen(");
		cpp_macro_conversion(handle,sptr,(char *) 0);
		fprintf(handle,")");
		}
	else 	fprintf(handle,"%s",strlen(sptr));
	return;
}

public 	void	string_value(FILE * handle, char * sptr )

{
	if (!( sptr ))
		quoted_string(handle,EmptyString);
	else if ( *(sptr) == _UNQUOTED_STRING ) {
		variable_value(handle,(sptr+1));
		}
	else if (!( *sptr ))
		quoted_string(handle,EmptyString);
	else 	quoted_string(handle,sptr);
	return;
}

public 	void	format_string_value(FILE * handle, char * sptr )

{
	if (!( sptr ))
		quoted_string(handle,"(E)");
	else if ( *(sptr) == _UNQUOTED_STRING ) {
		variable_value(handle,(sptr+1));
		}
	else if (!( *sptr ))
		quoted_string(handle,"(E)");
	else if ( *sptr == '(' )
		quoted_string(handle,sptr);
	else	{
		if ( *sptr == ',' )
			sptr++;
		fprintf(handle,"%c(%s)%c",0x0022,sptr,0x0022);
		}
	return;
}

public 	void	format_value(FILE * handle, char * sptr )

{
	if (!( sptr ))
		fprintf(handle,"(E)");
	else if ( *(sptr) == _UNQUOTED_STRING ) {
		variable_value(handle,(sptr+1));
		}
	else if (!( *sptr ))
		fprintf(handle,"(E)");
	else if ( *sptr == '(' )
		fprintf(handle,"%s",sptr);
	else	{
		if ( *sptr == ',' )
			sptr++;
		fprintf(handle,"(%s)",sptr);
		}
	return;
}

public	char *	payload_value( struct form_item * iptr )
{
	struct	form_control * fptr;
	if (!( fptr = iptr->parent ))
		return( iptr->Contents.payload );
	else if (!( fptr->formflags & _INTERNATIONAL ))
		return( iptr->Contents.payload );
	else	return( iptr->Contents.messages[fptr->natlang] );
}


public	char *	hint_value( struct form_item * iptr )
{
	struct	form_control * fptr;
	if (!( fptr = iptr->parent ))
		return( iptr->Contents.hint );
	else if (!( fptr->formflags & _INTERNATIONAL ))
		return( iptr->Contents.hint );
	else	return( iptr->Contents.hmessages[fptr->natlang] );
}


public 	void	string_parameter(FILE * handle, char * sptr )

{
	fprintf(handle,",");
	string_value( handle, sptr );
}

public 	void	constant_value(FILE * handle, struct form_item * iptr )
{
	char *	sptr;
	struct form_control * fptr;
	if (!( sptr = iptr->Contents.name ))
		string_value( handle, iptr->Contents.payload );
	else if (!( sptr = payload_value( iptr ) ))
		string_value( handle, sptr );
	else if ( *sptr == _UNQUOTED_STRING )
		string_value( handle, sptr );
	else	{
		if (((fptr = iptr->parent) != (struct form_control*) 0) 
		&&  (fptr->formflags & _FORM_CONSTANTS))
			string_value( handle, sptr );
		else	{
			if ((iptr->Contents.style & _FRAME_TYPE) == _RADIO_FRAME)
				fprintf(handle,"%s%u_%s",
					language_constant_prefix,
					iptr->Contents.radio,iptr->Contents.name);		
			else	fprintf(handle,"%s_%s",
					language_constant_prefix,
					iptr->Contents.name);		
			if (((fptr = iptr->parent) != (struct form_control*) 0) 
			&&  (fptr->formflags & _MULTILINGUAL)) {
				fprintf(handle,"(NationalLanguage)");
				}
			}
		}

	return;
}

public 	void	constant_parameter(FILE * handle, struct form_item * iptr )
{
	fprintf(handle,",");
	constant_value( handle, iptr );
	return;
}

public 	void	constant_hint_value(FILE * handle, struct form_item * iptr )
{
	char *	sptr;
	struct form_control * fptr;
	if (!( sptr = iptr->Contents.name ))
		string_value( handle, iptr->Contents.hint );
	else if (!( sptr = hint_value( iptr ) ))
		string_value( handle, sptr );
	else if ( *sptr == _UNQUOTED_STRING )
		string_value( handle, sptr );
	else	{
		if (((fptr = iptr->parent) != (struct form_control*) 0) 
		&&  (fptr->formflags & _FORM_CONSTANTS))
			string_value( handle, sptr );
		else	{
			if ((iptr->Contents.style & _FRAME_TYPE) == _RADIO_FRAME)
				fprintf(handle,"%s%u_%s",
					language_hint_prefix,
					iptr->Contents.radio,iptr->Contents.name);		
			else	fprintf(handle,"%s_%s",
					language_hint_prefix,
					iptr->Contents.name);		
			if (((fptr = iptr->parent) != (struct form_control*) 0) 
			&&  (fptr->formflags & _MULTILINGUAL)) {
				fprintf(handle,"(NationalLanguage)");
				}
			}
		}

	return;
}

public 	void	constant_hint_parameter(FILE * handle, struct form_item * iptr )
{
	fprintf(handle,",");
	constant_hint_value( handle, iptr );
	return;
}

public	void	generic_length_parameter( FILE * h, char * sptr )
{
	if ( *sptr == _UNQUOTED_STRING )
		sptr++;
	switch ( Context.Form->language ) {
		case	_GENERATE_C	:
		case	_GENERATE_CPP	:
			fprintf(h,",strlen(%s)",sptr);
			break;
		case	_GENERATE_HTML	:
		case	_GENERATE_BAL	:
		case	_GENERATE_ABAL	:
			fprintf(h,",Len$(%s)",sptr);
			break;
		default			:
			length_parameter(h,strlen(sptr));
		}
	return;
}


public 	void	string_length_parameter(FILE * h, struct form_item * iptr )
{
	char	*	sptr;
	if (!(sptr = iptr->Contents.payload ))
		length_parameter(h, iptr->Contents.size );
	else if ( *(sptr++) != _UNQUOTED_STRING )
		length_parameter(h, iptr->Contents.size );
	else 	generic_length_parameter(h,sptr);
	return;
}

public	void	trigger_parameter( FILE * h, struct form_item * iptr )
{
	char	*	sptr;
	int	j;
	int	i;
	int	t;
	int	c;
	struct form_control * fptr;
	if (((fptr = iptr->parent) != (struct form_control *) 0)
	&&  ((fptr->formflags & _INTERNATIONAL) != 0)) {
		if (!( sptr = iptr->Contents.messages[fptr->natlang] ))
			sptr = " ";
		}
	else if (!( sptr = iptr->Contents.payload ))
		sptr = " ";


	if ( *sptr == _UNQUOTED_STRING ) {
		string_parameter( h, sptr );
		generic_length_parameter( h, sptr );
		}
	else if (!(t = iptr->Contents.trigger)) {
		string_parameter( h, sptr );
		length_parameter( h, strlen(sptr) );
		}

	else	{
		/* scan the string to see if explicite trigger exists */
		/* -------------------------------------------------- */
		for (i=0; *(sptr+i); i++ )
			if (*(sptr+i) == _HOTKEY_TRIGGER)
				break;

		string_parameter( h, sptr );
		length_parameter(h,strlen(sptr));

		}
}

public 	void	filename_parameter(FILE * handle, char * sptr )
{
	fprintf(handle,",");
	if (!( sptr ))
		quoted_filename(handle,EmptyString);
	else if ( *(sptr) == _UNQUOTED_STRING )
		variable_value(handle,(sptr+1));
	else if (!( *sptr ))
		quoted_filename(handle,EmptyString);
	else 	quoted_filename(handle,sptr);
	return;
}

public 	void	buffer_value(FILE * handle, char * sptr )
{
	if (!( sptr ))
		quoted_filename(handle,EmptyString);
	else if ( *(sptr) == _UNQUOTED_STRING )
		fprintf(handle,"%s",(sptr +1));
	else if (!( *sptr ))
		quoted_filename(handle,EmptyString);
	else 	quoted_filename(handle,sptr);
	return;
}

public 	void	buffer_parameter(FILE * handle, char * sptr )
{
	fprintf(handle,",");
	buffer_value( handle, sptr );
	return;
}

/*
 *	The four following functions perform return of the widget
 *	position or dimension in the form of a string which may
 *	contain the integer value or the variable name depending
 *	on the mode option and the state of the CREATE EVENT 
 *		FOCUS==SIZES
 *	flag.
 *
 *	When mode is set to TRUE the initial integer constant value
 *	will always be returned. when mode is false/0 then either
 *	the name or the value will be returned respecting the option.
 *
 */

public	char * 	widget_fh_dimension( struct form_item * iptr )
{
	int	fh=16;
	if ((!( iptr->Contents.h )) 
	||  (!( fh = guifontsize(iptr->Contents.font ) )))
		sprintf(whbuffer,"%u",16);
	else	sprintf(whbuffer,"%u",(fh >> 8));
	return( whbuffer );
}

public	char * 	widget_w_dimension( struct form_item * iptr, int mode )
{
	if ((( iptr->Contents.events->xo_create & _EVENT_IS_FOCUS ) || (is_xy_modif(iptr)))
	&& (!( mode ))) {
		if (( iptr->Contents.style & _FRAME_TYPE) == _RADIO_FRAME)
			sprintf(wwbuffer,"%sw%u_%s",language_instance_prefix,iptr->Contents.radio,iptr->Contents.name);
		else	sprintf(wwbuffer,"%sw_%s",language_instance_prefix,iptr->Contents.name);
		}
	else if (is_w_modif(iptr))
		sprintf(wwbuffer,"(%u*_wref)/%d",iptr->Contents.w,w_base_resize(iptr));
	else	sprintf(wwbuffer,"%u",iptr->Contents.w);
	return( wwbuffer );
}

public	char * 	widget_h_dimension( struct form_item * iptr, int mode )
{
	if ((( iptr->Contents.events->xo_create & _EVENT_IS_FOCUS ) || (is_xy_modif(iptr)))
	&& (!( mode ))) {
		if (( iptr->Contents.style & _FRAME_TYPE) == _RADIO_FRAME)
			sprintf(whbuffer,"%sh%u_%s",language_instance_prefix,iptr->Contents.radio,iptr->Contents.name);
		else	sprintf(whbuffer,"%sh_%s",language_instance_prefix,iptr->Contents.name);
		}
	else if (is_h_modif(iptr))
		sprintf(whbuffer,"(%u*_href)/%d",iptr->Contents.h,h_base_resize(iptr));
	else	sprintf(whbuffer,"%u",iptr->Contents.h);
	return( whbuffer );
}

public	char * 	widget_x_position( struct form_item * iptr, int mode )
{
	if ((( iptr->Contents.events->xo_create & _EVENT_IS_FOCUS ) || (is_xy_modif(iptr)))
	&& (!( mode ))) {
		if (( iptr->Contents.style & _FRAME_TYPE) == _RADIO_FRAME)
			sprintf(wxbuffer,"%sx%u_%s",language_instance_prefix,iptr->Contents.radio,iptr->Contents.name);
		else	sprintf(wxbuffer,"%sx_%s",language_instance_prefix,iptr->Contents.name);
		}
	else if ((( iptr->Contents.events->xo_create & _EVENT_IS_FOCUS ) || (is_xy_modif(iptr))) && ( mode & 2 )) {
		if (( iptr->Contents.style & _FRAME_TYPE) == _RADIO_FRAME)
			sprintf(wxbuffer,"%sx%u_%s",language_instance_prefix,iptr->Contents.radio,iptr->Contents.name);
		else	sprintf(wxbuffer,"%sx_%s",language_instance_prefix,iptr->Contents.name);
		}
	else if ( ZeroWindowColumn ) {
		if (!( iptr->Contents.x - zWindowCol ))
			sprintf(wxbuffer,"%s%s",language_instance_prefix,ZeroWindowColumn);
		else if (is_xy_modif(iptr))
			sprintf(wxbuffer,"%s%s+((%u*_wref)/%d)",language_instance_prefix,ZeroWindowColumn,iptr->Contents.x-zWindowCol,w_base_resize(iptr));
		else	sprintf(wxbuffer,"%s%s+%u",language_instance_prefix,ZeroWindowColumn,iptr->Contents.x-zWindowCol);
		}
	else	{
		switch ( ZeroWindow ) {
			case	0	:
				if (is_xy_modif(iptr))
					sprintf(wxbuffer,"(%u*_wref)/%d",(iptr->Contents.x + WindowCol),w_base_resize(iptr));
				else	sprintf(wxbuffer,"%u",(iptr->Contents.x + WindowCol));
				break;

			case	1	:
				if (is_xy_modif(iptr))
					sprintf(wxbuffer,"(%u*_wref)/%d",(iptr->Contents.x - WindowCol),w_base_resize(iptr));
				else	sprintf(wxbuffer,"%u",(iptr->Contents.x - WindowCol));
				break;
			default		:
				sprintf(wxbuffer,"%u",0);
			}
		}
	return( wxbuffer );
}
		
public	char *	widget_y_position( struct form_item * iptr, int mode )
{
	if ((( iptr->Contents.events->xo_create & _EVENT_IS_FOCUS ) || (is_xy_modif(iptr)))
	&& (!( mode ))) {
		if (( iptr->Contents.style & _FRAME_TYPE) == _RADIO_FRAME)
			sprintf(wybuffer,"%sy%u_%s",language_instance_prefix,iptr->Contents.radio,iptr->Contents.name);
		else	sprintf(wybuffer,"%sy_%s",language_instance_prefix,iptr->Contents.name);
		}
	else if ((( iptr->Contents.events->xo_create & _EVENT_IS_FOCUS ) || (is_xy_modif(iptr)))
	     && ( mode & 2 )) {
		if (( iptr->Contents.style & _FRAME_TYPE) == _RADIO_FRAME)
			sprintf(wybuffer,"%sy%u_%s",language_instance_prefix,iptr->Contents.radio,iptr->Contents.name);
		else	sprintf(wybuffer,"%sy_%s",language_instance_prefix,iptr->Contents.name);
		}
	else if ( ZeroWindowLine ) {
		if (!( iptr->Contents.y - zWindowRow ))
			sprintf(wybuffer,"%s%s",language_instance_prefix,ZeroWindowLine);
		else if (is_xy_modif(iptr))
			sprintf(wybuffer,"%s%s+((%u*_href)/%d)",language_instance_prefix,ZeroWindowLine,iptr->Contents.y-zWindowRow,h_base_resize(iptr));
		else	sprintf(wybuffer,"%s%s+%u",language_instance_prefix,ZeroWindowLine,iptr->Contents.y-zWindowRow);
		}
	else	{
		switch ( ZeroWindow ) {
			case	0	:
				if (is_xy_modif(iptr))
					sprintf(wybuffer,"(%u*_href)/%d",(iptr->Contents.y + WindowRow),h_base_resize(iptr));
				else	sprintf(wybuffer,"%u",(iptr->Contents.y + WindowRow));
				break;
			case	1	:
				if (is_xy_modif(iptr))
					sprintf(wybuffer,"(%u*_href)/%d",(iptr->Contents.y - WindowRow),h_base_resize(iptr));
				else	sprintf(wybuffer,"%u",(iptr->Contents.y - WindowRow));
				break;
			default		:
				sprintf(wybuffer,"%u",0);
			}
		}
	return( wybuffer );
}
		
public	void	widget_position( FILE * h, struct form_item * iptr, int ax, int ay )
{
	if ( ax != 0 )
		fprintf(h,"%s+%u",widget_x_position(iptr,0),ax);
	else	fprintf(h,"%s",widget_x_position(iptr,0));
	if ( ay != 0 )
		fprintf(h,",%s+%u",widget_y_position(iptr,0),ay);
	else	fprintf(h,",%s",widget_y_position(iptr,0));
	return;
}

public	void	widget_bottom( FILE * h, struct form_item * iptr, int ax, int ay )
{
	fprintf(h,",%u,%u",
		(iptr->Contents.w+ax+WindowCol),
		(iptr->Contents.h+ay+WindowRow));
	return;
}

public	void	widget_dimensions( FILE * h, struct form_item * iptr, int ax, int ay )
{
	if ( ax != 0 )
		fprintf(h,",%s+%u",widget_w_dimension(iptr,0),ax);
	else	fprintf(h,",%s",widget_w_dimension(iptr,0));
	if ( ay != 0 )
		fprintf(h,",%s+%u",widget_h_dimension(iptr,0),ay);
	else	fprintf(h,",%s",widget_h_dimension(iptr,0));
	return;
}

public	void	widget_font_dimensions( FILE * h, struct form_item * iptr, int ax, int ay )
{
	if ( ax != 0 )
		fprintf(h,",%s+%u",widget_w_dimension(iptr,0),ax);
	else	fprintf(h,",%s",widget_w_dimension(iptr,0));
	if ( ay != 0 )
		fprintf(h,",%s+%u",widget_fh_dimension(iptr),ay);
	else	fprintf(h,",%s",widget_fh_dimension(iptr));
	return;
}

public	struct form_item * locate_radio_method( struct form_control * fptr, char * nptr, int method )
{
	struct	form_item * iptr;
	if (!( fptr ))
		return((struct form_item *) 0);
	else if (!( nptr ))
		return((struct form_item *) 0);
	for (	iptr=fptr->first;
		iptr != (struct form_item *) 0;
		iptr = iptr->next ) {
		if (!( iptr->Contents.name ))
			continue;
		else if ( strcmp( iptr->Contents.name, nptr ) )
			continue;
		else	{
			switch ( method ) {
				case	1 : 	if (!( method_is_valid( iptr->Contents.events->on_create )))
							continue;
						else	return( iptr );
				case	2 : 	if (!( method_is_valid( iptr->Contents.events->on_show   )))
							continue;
						else	return( iptr );
				case	3 : 	if (!( method_is_valid( iptr->Contents.events->on_get_focus )))
							continue;
						else	return( iptr );
				case	4 : 	if (!( method_is_valid( iptr->Contents.events->on_event  )))
							continue;
						else	return( iptr );
				case	5 : 	if (!( method_is_valid( iptr->Contents.events->on_lose_focus )))
							continue;
						else	return( iptr );
				case	6 : 	if (!( method_is_valid( iptr->Contents.events->on_hide )))
							continue;
						else	return( iptr );
				case	7 : 	if (!( method_is_valid( iptr->Contents.events->on_remove )))
							continue;
						else	return( iptr );
				}
			}
		}
	return((struct form_item *) 0);
}


public	struct form_item * locate_radio_create( struct form_control * fptr, char * nptr )
{
	return( locate_radio_method( fptr,nptr,1) );
}

public	struct form_item * locate_radio_show( struct form_control * fptr, char * nptr )
{
	return( locate_radio_method( fptr,nptr,2) );
}

public	struct form_item * locate_radio_get_focus( struct form_control * fptr, char * nptr )
{
	return( locate_radio_method( fptr,nptr,3) );
}

public	struct form_item * locate_radio_event( struct form_control * fptr, char * nptr )
{
	return( locate_radio_method( fptr,nptr,4) );
}

public	struct form_item * locate_radio_lose_focus( struct form_control * fptr, char * nptr )
{
	return( locate_radio_method( fptr,nptr,5) );
}

public	struct form_item * locate_radio_hide( struct form_control * fptr, char * nptr )
{
	return( locate_radio_method( fptr,nptr,6) );
}

public	struct form_item * locate_radio_remove( struct form_control * fptr, char * nptr )
{
	return( locate_radio_method( fptr,nptr,7) );
}

public	void	calculate_edit_size( struct window_control * iptr )
{
	int	nc;
	int	nl;
	int	fw;
	int	fh;
	int	flen=0;
	char *	sptr;
	if (!( iptr->font ))
		return;

	switch ( iptr->style & _FRAME_TYPE ) {
		case	_LINE_FRAME	:
		case	_GRAPH_FRAME	:
			return;
		case	_EDIT_FRAME	:
		case	_FORM_FRAME	:
			break;
		default			:
			if ((sptr = payload_value(iptr)) != (char *) 0) {
				iptr->size = strlen( sptr );
				}
			else	iptr->size = 0;
			return;
		}

	/* FORM and EDIT frames calculate their buffer size to fit their visible zone */
	/* -------------------------------------------------------------------------- */
	if (((fw  = Context.FontTable[iptr->font].width ) != 0)
	&&  ((fh  = Context.FontTable[iptr->font].height) != 0)) {
		if (!(nc = (iptr->w  / fw)))
			nc = 1;
		if (!(nl = (iptr->h / fh)))
			nl = 1;
		if ((iptr->style & _FRAME_TYPE) == _FORM_FRAME)
			if ( method_is_valid(iptr->payload ) ) 
				if ( nl > 0 )
					nl--;
		iptr->size = (nc * nl );
		}

	/* Edit frames may declare a larger buffer than their visible size by their format component */
	/* ----------------------------------------------------------------------------------------- */
	if (( iptr->style & _FRAME_TYPE ) == _EDIT_FRAME ) 
		if ( method_is_valid( iptr->format ) )
			if ((flen = visual_format_length( iptr->format )) > iptr->size )
				iptr->size = flen;

	return;
}

public	char *	resolve_widget_payload( struct window_control * iptr )
{
	if (!( iptr ))
		return( (char *) 0);
	else if ( iptr->payload )
		return( iptr->payload );
	else 	return( iptr->messages[0] );
}

public	void	calculate_edit_lines( struct window_control * iptr, int * nbc, int * nbl )
{
	int	nc;
	int	nl;
	int	fw;
	int	fh;

	if (!( iptr->font ))
		return;

	if (((fw  = Context.FontTable[iptr->font].width  ) != 0)
	&&  (( fh  = Context.FontTable[iptr->font].height) != 0)) {
		if (!(nc = (iptr->w  / fw)))
			nc = 1;
		if (!(nl = (iptr->h / fh)))
			nl = 1;
		if ((iptr->style & _FRAME_TYPE) == _FORM_FRAME)
			if ( method_is_valid(resolve_widget_payload(iptr)) ) 
				if ( nl > 0 )
					nl--;
		*nbc = nc;
		*nbl = nl;
		}

	return;
}

public	void	calculate_form_lines( struct window_control * iptr, int * nbc, int * nbl )
{
	int	nc;
	int	nl;
	int	fw;
	int	fh;
	int	xa=4;
	int	ya=4;

	if (!( iptr->font ))
		return;

	if (((fw  = Context.FontTable[iptr->font].width  ) != 0)
	&&  ((fh  = Context.FontTable[iptr->font].height ) != 0)) {
		if (!(nc = ((iptr->w-xa)  / fw)))
			nc = 1;
		if (!(nl = ((iptr->h-ya) / fh)))
			nl = 1;
		if ((iptr->style & _FRAME_TYPE) == _FORM_FRAME)
			if ( method_is_valid(resolve_widget_payload(iptr)) ) 
				if ( nl > 0 )
					nl--;
		*nbc = nc;
		*nbl = nl;
		}
	return;
}

#include "singpars.c"

private	int	java_production( char ** aptr, char ** bptr )
{
	return(27 );
}

private	char	*	language_extension( int language )
{
	switch (language) {
		case	_GENERATE_C		:
			return((char *)C_EXTENSION);
		case	_GENERATE_CPP		:
			return((char *)CPP_EXTENSION);
		case	_GENERATE_HTML		:
			return((char *)HTM_EXTENSION);
		case	_GENERATE_ABAL		:
		case	_GENERATE_BAL		:
			return((char *)(Context.BalOrAbal ? S_EXTENSION : AS_EXTENSION));
		case	_GENERATE_JAVA		:
			return((char *)JAV_EXTENSION);
		case	_GENERATE_COBOL		:
			return((char *)COB_EXTENSION);
		case	_GENERATE_MAKE		:
			return((char *)MF_EXTENSION);
		default				:
			return((char *) 0);
		}
}

private	char *	join_strings( char * sptr, char * tptr)
{
	char *	rptr;
	if (!( sptr ))
		return( sptr );
	else if (!( tptr ))
		return( sptr );
	else if (!( rptr = allocate( strlen(sptr) + strlen( tptr ) + 8 ) ))
		return( sptr );
	else	{
		sprintf( rptr, "%s %s",sptr,tptr);		
		liberate( sptr );
		return( rptr );
		}
}

private	void	post_production(
		struct form_control * fptr,
		int	language,
		char *	prodname,
		int	automatic )
{
	char *	modules;
	char *	abal_modules(char *, char*);
	if ( fptr->construction )
		modules = allocate_string( fptr->construction );
	else	modules = (char *) 0;
	switch ( language ) {
		case	_GENERATE_JAVA		:
		case	_GENERATE_C		:
		case	_GENERATE_CPP		:
			break;
		case	_GENERATE_HTML		:
			/* ------------------------ */
			/* translation of web agent */
			/* ------------------------ */
			prodname = enforce_extension(prodname,".as");

		case	_GENERATE_BAL		:
			/* ---------------------------------------------------- */
			/* 	   DO NOT EXCLUDE this FOR WINDOWS		*/
			/* !!!!!  these two need to be found for windows !!!!!  */
			/* abal source beautifier sources in the amake project  */
			/* ---------------------------------------------------- */
			if (!( confgetenv("NOASB") )) { 
				asb_initialise();
				asb_operation(prodname);
				}
#ifdef ABALCPP
			if (!( translate_abal(0,prodname,automatic,NULL) ))
#else
			if (!( translate_abal(0,prodname,automatic) ))
#endif
				break;
			else if (!( fptr->isprogram ))
				break;
			else if (!(modules = abal_modules((char *) 0, forms_modules(fptr,modules) ) )) {
				if ( fptr->modext ) {
					if (!( fptr->abal.flags & 16 ))
						link_abal(fptr->prodname,keyword_modext,automatic,0);
					else	link_abal(fptr->prodname,keyword_modexto,automatic,0);
					}
				break;
				}
			else if (!( fptr->modext )) {
				link_abal(fptr->prodname,modules,automatic,0);
				break;
				}
			else	{
				if (!( fptr->abal.flags & 16 ))
					modules = join_strings(modules,keyword_modext);
				else	modules = join_strings(modules,keyword_modexto);
				link_abal(fptr->prodname,modules,automatic,0);
				break;
				}

		case	_GENERATE_ABAL		:
#ifdef ABALCPP
			if (!( translate_abal(1,prodname,automatic,NULL) ))
#else
			if (!( translate_abal(1,prodname,automatic) ))
#endif
				break;
			else if (!( fptr->isprogram ))
				break;
			else if (!(modules = abal_modules((char *) 0, forms_modules(fptr,modules) ) )) {
				if ( fptr->modext ) {
					if (!( fptr->abal.flags & 16 ))
						link_abal(fptr->prodname,keyword_modext,automatic,1);
					else	link_abal(fptr->prodname,keyword_modexto,automatic,1);
					}
				break;
				}
			else if (!( fptr->modext )) {
				link_abal(fptr->prodname,modules,automatic,1);
				break;
				}
			else	{
				if (!( fptr->abal.flags & 16 ))
					modules = join_strings(modules,keyword_modext);
				else	modules = join_strings(modules,keyword_modexto);
				link_abal(fptr->prodname,modules,automatic,1);
				break;
				}
			break;
		case	_GENERATE_COBOL		:
#ifdef ABALCPP
			if (!( translate_abal(2,prodname,automatic,NULL) ))
#else
			if (!( translate_abal(2,prodname,automatic) ))
#endif
				break;
			else if (!(modules = abal_modules((char *) 0, forms_modules(fptr,modules) ) ))
				break;
			else if (!( fptr->isprogram ))
				break;
			else	{
				link_abal(prodname,modules,automatic,0);
				break;
				}
			break;
		}
	if ( modules )
		liberate( modules );
	if ( prodname )
		liberate( prodname );
	return;

}

#ifdef	UNIX
extern	int	errno;
#endif

#include "singprod.c"

public	struct form_item * locate_widget( char * nptr )
{
	struct form_item * iptr;
	for (	iptr=Context.Form->first;
		iptr != (struct form_item *) 0;
		iptr = iptr->next ) {
		if (!( iptr->Contents.name ))
			continue;
		else if ( string_compare( iptr->Contents.name, nptr ) )
			break;
		}
	return( iptr );
}

public	int	program_nature(int isp, int iso)
{
	if ( isp ) {
		if (!( iso ))	return( _NATURE_PROGRAM );
		else		return( _NATURE_OVERLAY );
		}
	else	{
		if (!( iso ))	return( _NATURE_INCLUDE );
		else		return( _NATURE_MODULE  );
		}
}

#include "singcpp.c"
#include "singhtml.c"
#include "singjava.c"

public	char *	headername( char * sptr, char * xptr )
{
	int	l;
	int	i;
	int	n;
	if (!( l = strlen( sptr ) ))
		return((char *) 0 );

	for ( i=(l-1); i > 0; i-- )
		if ( *(sptr+i) == '.' )
			break;

	for (n=0; n < i; n++ )
		namebuffer[n] = *(sptr+n);
	for (i=0; *(xptr+i) != 0; i++ )
		namebuffer[n+i] = *(xptr+i);
	namebuffer[n+i] = 0;
	return( allocate_string( namebuffer ) );
}

private	char	*	prefix_production_name( int language, char * nptr )
{
	char	*	source_name = (char *) 0;
	switch ( language ) {
		case	_GENERATE_C	:
		case	_GENERATE_CPP	:
		case	_GENERATE_BAL	:
		case	_GENERATE_ABAL	:
		case	_GENERATE_COBOL	:
		case	_GENERATE_JAVA	:
			return((source_name = use_temporary_directory( nptr )));
		case	_GENERATE_HTML	:
			return((source_name = use_product_directory( nptr )));
		}
	return((char *) 0);
}

public 	int	export_Form( struct form_control * fptr, int automatic )
{
	int		restriction=1;
	int		contextual=1;
	int		sessionimage=0;
	char	*	nullptr=(char *) 0;
	FILE	*	h;
	char	*	xptr=(char *) 0;
	char	*	nptr;
	int		maxx;
	int		maxy;
	int		whoops=0;
	int		truecolour=0;
	int		autotrad=1;
	int		pnature=0;
	int		usediezemem=0;
	int		usediezefiles=0;
	int		usepragmafile=0;
	int		usediezestack=0;
	int		usediezeheap=0;
	int		usetokensize=0;
	int		uselabelsize=0;
	int		userespectcase=0;
	int		useoptimisetables=0;
	int		webmode=0;
	int		webopt=1;
	int		iType=0;
	int		iQuality=0;
	int		iCookies=1;
	int		metrics=get_document_metric();
	int		points=get_document_scale();
	char	*source_name,*app_sources;

	struct window_control  Window;
	char *	headname=(char *) 0;

	ZeroWindow = 0; TargetBiosMode = 3;

	if (!( Context.Form = fptr ))
		return(0);

	else if (!( xptr = language_extension( fptr->language )))
		return(0);


	/* Clear these now */
	/* --------------- */
	if ( ZeroWindowLine ) {
		liberate( ZeroWindowLine );
		ZeroWindowLine = (char *) 0;
		}

	if ( ZeroWindowColumn ) {
		liberate( ZeroWindowColumn );
		ZeroWindowColumn = (char *) 0;
		}


	if ( fptr->isdynamic )
		FixedWindows = 0;
	else	FixedWindows = 1;


	/* Perform Calculation of Focus,and Page Items */
	/* ------------------------------------------- */
	prepare_Form( fptr );

	if (!(nptr = fptr->prodname )) {
		sprintf(namebuffer,"%s%s","new",xptr);
		if (!(fptr->prodname = allocate_string( namebuffer ) ))
			return(27);
		else	nptr = fptr->prodname;
		}


	form_Dimensions( &ProductionWindow, fptr );

	maxx = (ProductionWindow.x+ProductionWindow.w);
	maxy = (ProductionWindow.y+ProductionWindow.h);

	while (1) {
		if (( maxx > BiosDimensions[TargetBiosMode].w )
		||  ( maxy > BiosDimensions[TargetBiosMode].h )) {
			if ( TargetBiosMode < 5 )
				TargetBiosMode++;
			else	{
				whoops=1;
				break;
				}
			}
		else	break;
		}
#ifdef	ABAL32A
	if ( fptr->weboptions & 1 )
		webmode=1;
	else	webmode=0;
	if ( fptr->weboptions & 2 )
		webopt=1;
	else	webopt=0;
	if ( fptr->weboptions & 4 )
		restriction=1;
	else	restriction=0;
	if ( fptr->weboptions & 8 )
		sessionimage=1;
	else	sessionimage=0;
	if ( fptr->weboptions & 16 )
		iType=1;
	else	iType=0;
	if ( fptr->weboptions & 32 )
		iQuality=1;
	else	iQuality=0;
	if ( fptr->weboptions & 64 )
		iCookies=1;
	else	iCookies=0;
	if ( fptr->weboptions & 128 )
		contextual=1;
	else	contextual=0;
#endif
	if (!( automatic & _INHIBIT_PROPERTIES )) {

		switch ( fptr->language ) {
#ifdef	ABAL33A
			case	_GENERATE_CPP		:
				if (fptr->isoverlay)
					fptr->isprogram=1-fptr->isprogram;
				if ( fptr->abal.flags & 256 )
					truecolour=1;
				else	truecolour=0;

				if ( accept_cpp_production( 
						&fptr->prodname, 
						&fptr->identifier, 
						&fptr->isprogram, 
						&fptr->isoverlay, 
						&truecolour,
						&fptr->dependance,
						&TargetBiosMode,
						&fptr->OptionsCompile,
						&fptr->OptionsLinkage) == 27 ) {
					if ((fptr->language == _GENERATE_CPP) && (fptr->isoverlay))
						fptr->isprogram=1-fptr->isprogram;
					return(0);
					}
				else	{
					if( truecolour )
						fptr->abal.flags |= 256;
					else	fptr->abal.flags &= ~256;
					if (fptr->isoverlay) 
						fptr->isprogram=1-fptr->isprogram;
					break;
					}
#endif
			case	_GENERATE_C		:
				if ( fptr->abal.flags & 256 )
					truecolour=1;
				else	truecolour=0;
				if ( accept_c_production( 
						&fptr->prodname, 
						&fptr->identifier, 
						&fptr->isprogram, 
						&fptr->isoverlay, 
						&truecolour,
						&fptr->dependance,
						&TargetBiosMode ) == 27 ) {
					if ((fptr->language == _GENERATE_CPP) && (fptr->isoverlay))
						fptr->isprogram=1-fptr->isprogram;
					return(0);
					}
				else	{
					if( truecolour )
						fptr->abal.flags |= 256;
					else	fptr->abal.flags &= ~256;
					break;
					}

#ifdef	ABAL32A
			case	_GENERATE_HTML		:
				if ( accept_html_production( 
						&fptr->prodname, 
						&fptr->identifier, 
						&fptr->stylesheet, 
						/* &webmode, */ 	/* html, agent or other */
						&metrics, 	/* document metric type	*/
						&points, 	/* basic text pointsize	*/
						&webopt, 	/* generate agent	*/
						&autotrad, 	/* hide/show secondary	*/
						&restriction, 	/* window restriction	*/
						&contextual, 	/* window restriction	*/
						&sessionimage, 	/* active session image	*/
						&iQuality,	/* image quality	*/
						&iCookies	/* require cookies	*/
						) == 27 ) {
					draw_console();
					return(0);
					}
				if ( autotrad )
					automatic = _INHIBIT_PROPERTIES;
				webmode = 0;
				if ( webmode )
					fptr->weboptions |= 1;
				else	fptr->weboptions &= ~1;
				if ( webopt )
					fptr->weboptions |= 2;
				else	fptr->weboptions &= ~2;
				if ( restriction )
					fptr->weboptions |= 4;
				else	fptr->weboptions &= ~4;
				if ( sessionimage )
					fptr->weboptions |= 8;
				else	fptr->weboptions &= ~8;
				if ( iType )
					fptr->weboptions |= 16;
				else	fptr->weboptions &= ~16;
				if ( iQuality )
					fptr->weboptions |= 32;
				else	fptr->weboptions &= ~32;
				if ( iCookies )
					fptr->weboptions |= 64;
				else	fptr->weboptions &= ~64;
				if ( contextual )
					fptr->weboptions |= 128;
				else	fptr->weboptions &= ~128;
				break;
#endif	
			case	_GENERATE_BAL		:
			case	_GENERATE_ABAL		:
			case	_GENERATE_COBOL		:
				if ((autotrad = abal_production_parameters(fptr)) == 27) {
					draw_console();
					return(0);
					}
				else if ( whoops )
					whoops=0;
				if ( autotrad )
					automatic = _INHIBIT_PROPERTIES;
				break;

#ifdef	ABAL33A
			case	_GENERATE_JAVA		:
				if ( java_production( &fptr->prodname, &fptr->identifier ) == 27 ) {
					draw_console();
					return(0);
					}
				else	break;
#endif
			default				:
				return(55);
			}
		status_message(fptr->prodname,3);
		}


	if ( fptr->abal.flags & 1024 )
		TargetBiosMode = 7;

	if ( fptr->formflags & _INTERNATIONAL ) { store_payloads(fptr); }

	renumerate_form( fptr );

	switch ( fptr->language ) {

#ifdef	ABAL33A
		case	_GENERATE_CPP		:

			EmptyString=CNulString;
			if (!( headname = headername( fptr->prodname, H_EXTENSION ) ))
				break;
			else if (!( h = open_production_target(headname,0))) 
				break;
			file_exclusion(h, headname );
			export_cpp_form( h, (char *) 0 );
			close_exclusion(h, headname );
			close_production_target(h,headname);
			break;
#endif
		default				:
			headname = (char *) 0;
		}

	if (!( source_name = prefix_production_name( (variable_macros=fptr->language), fptr->prodname ) ))
		return(0);

	if (( h = open_production_target(source_name,0)) != (FILE *) 0) {

		if (!( automatic & _INHIBIT_STATUS )) 
			x_event(35);

		if (!( automatic & _INHIBIT_STATUS )) 
			status_message(fptr->prodname,3);

		switch ((variable_macros=fptr->language)) {
			case	_GENERATE_C		:
				language_instance_prefix="_Context->";
				language_constant_prefix="_Context->msg";
				language_hint_prefix="_Context->hint";
				EmptyString=CNulString;
				file_exclusion(h, fptr->prodname );
				switch ( fptr->formflags & _FORM_TYPE_FLAGS ) {
					case	_FORM_TYPE_DIALOG	:
					case	_FORM_TYPE_EVENT 	:
						export_c_form( h );
						break;
					case	_FORM_TYPE_QUALITY	:
					case	_FORM_TYPE_PAGE  	:
						WindowCol = ProductionWindow.x;
						WindowRow = ProductionWindow.y;
						ZeroWindow = 1;
						export_c_page( h );
						ZeroWindow = 0;
						break;
					}
				close_exclusion(h, fptr->prodname );
				break;

#ifdef	ABAL33A
			case	_GENERATE_CPP		:
				language_instance_prefix="\0";
				language_constant_prefix="_";
				language_hint_prefix="__";
				EmptyString=CNulString;
				file_exclusion(h, fptr->prodname );
				switch ( fptr->formflags & _FORM_TYPE_FLAGS ) {
					case	_FORM_TYPE_DIALOG	:
					case	_FORM_TYPE_EVENT 	:
						export_cpp_form( h, headname );
						break;
					case	_FORM_TYPE_QUALITY	:
					case	_FORM_TYPE_PAGE  	:
						ZeroWindow = 1;
						WindowCol = ProductionWindow.x;
						WindowRow = ProductionWindow.y;
						export_cpp_page( h, headname );
						ZeroWindow = 0;
						break;
					}
				close_exclusion(h, fptr->prodname );
				if ( headname )
					headname = liberate( headname );
				break;

#endif

#ifdef	ABAL32A
			case	_GENERATE_HTML		:
				variable_macros=_GENERATE_BAL;
				language_instance_prefix="\0";
				language_constant_prefix="msg";
				language_hint_prefix="hint";
				EmptyString=HtmlNulString;
				if ( sessionimage )
					restriction |= 2;
				else	restriction &= ~2;
				if ( contextual )
					restriction |= 4;
				else	restriction &= ~4;
				switch ( webmode ) {
					case	1 : 
						(void) document_Form( h, Context.Form,automatic,1);
						break;
					case	0 :
						if (!( fptr->weboptions & 16 )) {
							/* Jpeg Image */
							iQuality = get_jpeg_quality();
							if (!( fptr->weboptions & 32 ))
								set_jpeg_quality( 90  );
							else	set_jpeg_quality( 100 );
							}
						autotrad = export_html_image_form(h,webopt,automatic,restriction,metrics,points );
						if (!( fptr->weboptions & 16 )) {
							set_jpeg_quality( iQuality );
							}
						break;
					case	2 :
						autotrad = export_html_form(h,webopt,automatic,restriction,metrics,points);
						break;
					}
				break;
#endif
			case	_GENERATE_BAL		:
				language_instance_prefix="\0";
				language_constant_prefix="msg";
				language_hint_prefix="hint";
				EmptyString=AbalNulString;
				switch ( fptr->formflags & _FORM_TYPE_FLAGS ) {
					case	_FORM_TYPE_DIALOG	:
					case	_FORM_TYPE_EVENT 	:
						export_abal_form( h );
						break;
					case	_FORM_TYPE_QUALITY	:
					case	_FORM_TYPE_PAGE  	:
						ZeroWindow = 1;
						WindowCol = ProductionWindow.x;
						WindowRow = ProductionWindow.y;
						export_abal_page( h );
						ZeroWindow = 0;
						break;
					}
				break;

			case	_GENERATE_ABAL		:
				language_instance_prefix="\0";
				language_constant_prefix="_";
				language_hint_prefix="__";
				EmptyString=AbalNulString;
				switch ( fptr->formflags & _FORM_TYPE_FLAGS ) {
					case	_FORM_TYPE_DIALOG	:
					case	_FORM_TYPE_EVENT 	:
						export_aplus_form( h );
						break;
					case	_FORM_TYPE_QUALITY	:
					case	_FORM_TYPE_PAGE  	:
						ZeroWindow = 1;
						WindowCol = ProductionWindow.x;
						WindowRow = ProductionWindow.y;
						export_aplus_page( h );
						ZeroWindow = 0;
						break;
					}
				break;

			case	_GENERATE_COBOL		:
				switch ( fptr->formflags & _FORM_TYPE_FLAGS ) {
					case	_FORM_TYPE_DIALOG	:
					case	_FORM_TYPE_EVENT 	:
						export_cobol_form( h );
						break;
					case	_FORM_TYPE_QUALITY	:
					case	_FORM_TYPE_PAGE  	:
						export_cobol_page( h );
						break;
					}

#ifdef	ABAL33A
			case	_GENERATE_JAVA		:
				language_instance_prefix="\0";
				EmptyString=JavaNulString;
				export_java_form( h ); break;
#endif
			}


		close_production_target(h,source_name);

		if (fptr->language==_GENERATE_CPP)
			{
			if (fptr->isprogram)
				{
				compile_cpp(source_name);
				if (!fptr->isoverlay) link_cpp(source_name);
				}
			else 
				{
				if (fptr->isoverlay)	/* faire une DLL */	
					{
					compile_dll_cpp(source_name);
					link_dll_cpp(source_name);
					}
				}
			}

		if ( fptr->formflags & _INTERNATIONAL ) { reload_payloads(fptr); }

		if ( autotrad != 27 ) {
			/* correct the source path for temporary file */
			/* ------------------------------------------ */
			if ( fptr->language == 	_GENERATE_HTML	) {
				liberate( source_name );
				if (!( source_name = prefix_production_name( _GENERATE_ABAL, fptr->prodname ) ))
					return(0);
				}
			post_production(fptr, fptr->language, allocate_string(source_name),  automatic );
			}

		liberate(source_name);

		if (!( automatic & _INHIBIT_REFRESH ))
			draw_console();

		if (!( automatic & _INHIBIT_STATUS )) 
			x_event(34);

		return(1);
		}
	else 	{
		sprintf(namebuffer,"error:%s.c",nptr);

		if (!( automatic & _INHIBIT_STATUS )) 
			status_message(namebuffer,3);
		else	printf("%s\n",namebuffer);

		if ( fptr->formflags & _INTERNATIONAL ) { reload_payloads(fptr); }
		if (!( automatic & _INHIBIT_REFRESH ))
			draw_console();
		return(0);
		}
}

public	void	initialise_production()
{
	initialise_c_production();
	initialise_abal_production();
	initialise_production_control();
	return;
}

public	void	release_production()
{
	release_production_control();
	release_abal_production();
	release_c_production();
	return;
}

#include "singtrig.c"

#endif	/* _SINGEXPO_C */

