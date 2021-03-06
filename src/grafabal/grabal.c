/* File generated by /usr/share/abal4/bin/linbda Version 3.3a.0.01 : /home/Dev/abal32/src/grafabal/grafabal.def -> /home/Dev/abal32/src/grafabal/grafabal.c */
#ifndef __linbda__home_Dev_abal32_src_grafabal_grafabal_c__
#define __linbda__home_Dev_abal32_src_grafabal_grafabal_c__
#include <stdio.h>
#include <setjmp.h>
#include "ldfbda.h"

static BDA * DynamicLibrary;
	LPBDA_INIT	rt_init;
	BDA_ARG	rt_arg;

/* ABAL Runtime Error Trap */
static jmp_buf rt_error_context;
VOID rt_error(EXAWORD evalue) { 
	longjmp(rt_error_context,evalue);
	return;
}

static int function_counter=0;
/* ABAL Word Convertor */
static EXAWORD LdfAbalByte(char FAR PTR bptr)
{
	return(*bptr);
}

static EXAWORD LdfAbalWord(BYTE FAR PTR wptr)
{
	return(((*wptr<< 8) | *(wptr+1)));
}

static EXAWORD LdfAbalLong(BYTE FAR PTR wptr)
{
	return((((*wptr)<< 24) | (*(wptr+1) << 16)  | (*(wptr+2) << 8)| *(wptr+3)));
}

#define LdfParaWord(n) ( tptr[n] == 1 ? LdfAbalWord(pptr[n]) : ( tptr[n] == 0 ?  LdfAbalByte(pptr[n]) : LdfAbalLong(pptr[n])) )
#define LdfParaByte(n) ( tptr[n] == 1 ? (LdfAbalWord(pptr[n])&0x00FF) : ( tptr[n] == 0 ?  LdfAbalByte(pptr[n]) : (LdfAbalLong(pptr[n])&0x00FF) ) )

static BYTE PTR function_names[256];
static char * trace_name=(char*)0;
static FILE * trace_handle=(FILE*)0;
extern long time(long*);

/* Trace Activation */
static void start_trace()
{
	trace_name = getenv("TRACE_GRAFABAL");
	if ( trace_name != (char *) 0 ) {
		trace_handle=fopen(trace_name,"w");
		}
	return;
}

/* Trace Write */
static void write_trace(char * mptr)
{
	if ( trace_handle != (FILE*)0) {
		fprintf(trace_handle,mptr);
		fprintf(trace_handle,"\r\n");
		}
	return;
}

/* Trace Result */
static void trace_result(EXAWORD result)
{
	if ( trace_handle != (FILE*)0) {
		fprintf(trace_handle,"RETURN: %u \r\n",result);
		}
	return;
}

/* Trace Call */
static void trace_call(
	EXAWORD fid,
	EXAWORD argc,
	BYTE FAR tptr[],
	VOID FAR PTR FAR pptr[],
	EXAWORD FAR wptr[])
{
	char * bptr;
	int	i=0;
	int	j=0;
	if ( trace_handle != (FILE*)0) {
			fprintf(trace_handle,"[%lu] ",time((long*)0));
		if ( function_names[fid] != (char *) 0) {
			fprintf(trace_handle,"(%s)",function_names[fid]);
			}
		else	{
			fprintf(trace_handle,"(call %u)",fid);
			}
		for (i=0; i < argc; i++) {
			switch(tptr[i]) {
				case 0x0080 : 
					fprintf(trace_handle,"\t(*b=%x)\r\n",*((bptr=pptr[i])));
					break;
				case 0  : 
					fprintf(trace_handle,"\t(b=%x)\r\n",*((bptr=pptr[i])));
					break;
				case 0x0081 : 
					fprintf(trace_handle,"\t(*w=%x)\r\n",LdfAbalWord(pptr[i]));
					break;
				case 1  : 
					fprintf(trace_handle,"\t(w=%x)\r\n",LdfAbalWord(pptr[i]));
					break;
				case 2  : 
				case 3  : 
				case 0x0082 : 
				case 0x0083 : 
					fprintf(trace_handle,"\t(*=");
					for (bptr=pptr[i],j=0;j<wptr[i];j++) {
						fprintf(trace_handle,"%x ",(*(bptr+j)&0x00FF));
						if ((j % 20) == 19) {
							fprintf(trace_handle,"\r\n");
							}
						}
					fprintf(trace_handle,")\r\n");
				};
			}
		fprintf(trace_handle,"\r\n");
		}
	return;
}

/* Trace Closure */
static void close_trace()
{
	if ( trace_handle != (FILE*)0) {
		fclose(trace_handle);
		trace_handle=(FILE*)0;
		}
	return;
}

/* Library Information */
EXAWORD LdfBdaInformation(void * vptr)
{
	printf(
		"\n   Abal Dynamic Library : GRAFABAL32.Abal.3.3a.0.01"
		);
	printf(
		"\n   Copyright (c) Amenesik / Sologic \n\n"
		);
	return(0);
}

/* Library Destructor */
EXAWORD LdfBdaTerminate()
{
	write_trace("LdfBdaTerminate()");
	close_trace();
	if (DynamicLibrary->fin != (VOID (FAR PTR)()) 0)
		 (*DynamicLibrary->fin)();
	return(0);
}

/* Library Call Through */
EXAWORD LdfBdaExecute(
	EXAWORD fid,
	EXAWORD argc,
	BYTE FAR tptr[],
	VOID FAR PTR FAR pptr[],
	EXAWORD FAR wptr[],
	EXAWORD FAR * eptr)
{
	EXAWORD	result=0;
	rt_arg.c=argc; rt_arg.v=pptr;
	rt_arg.l=wptr; rt_arg.t=tptr;
	if ((result=setjmp(rt_error_context))!=0) {
		if ( eptr != (EXAWORD FAR *) 0) *eptr = result;
		return( result );
		}
	if ( trace_handle != (FILE*)0) {
	trace_call(fid,argc,tptr,pptr,wptr);
		}
	/* Version = 1 */
	/* Runtime = "grafabal" */
	switch ( fid ) {
		case	0 : /* %g_init() */
			result=(*DynamicLibrary->fonction[0])(); break;
		case	1 : /* %g_assign(#, $, #, #) */
			result=(*DynamicLibrary->fonction[1])(
				LdfParaByte(0),
				pptr[1],
				LdfParaByte(2),
				LdfParaByte(3)); break;
		case	2 : /* %g_open(#) */
			result=(*DynamicLibrary->fonction[2])(
				LdfParaByte(0)); break;
		case	3 : /* %g_close(#) */
			result=(*DynamicLibrary->fonction[3])(
				LdfParaByte(0)); break;
		case	4 : /* %g_window(#, .) */
			result=(*DynamicLibrary->fonction[4])(
				LdfParaByte(0)); break;
		case	5 : /* %g_viewport(#, .) */
			result=(*DynamicLibrary->fonction[5])(
				LdfParaByte(0)); break;
		case	6 : /* %g_points(#, .) */
			result=(*DynamicLibrary->fonction[6])(
				LdfParaByte(0)); break;
		case	7 : /* %g_lines(#, .) */
			result=(*DynamicLibrary->fonction[7])(
				LdfParaByte(0)); break;
		case	8 : /* %g_circle(#, .) */
			result=(*DynamicLibrary->fonction[8])(
				LdfParaByte(0)); break;
		case	9 : /* %g_arc(#, .) */
			result=(*DynamicLibrary->fonction[9])(
				LdfParaByte(0)); break;
		case	10 : /* %g_point(#, %, %) */
			result=(*DynamicLibrary->fonction[10])(
				LdfParaByte(0),
				LdfParaWord(1),
				LdfParaWord(2)); break;
		case	11 : /* %g_line(#, %, %, %, %) */
			result=(*DynamicLibrary->fonction[11])(
				LdfParaByte(0),
				LdfParaWord(1),
				LdfParaWord(2),
				LdfParaWord(3),
				LdfParaWord(4)); break;
		case	12 : /* %g_paint(#, .) */
			result=(*DynamicLibrary->fonction[12])(
				LdfParaByte(0)); break;
		case	13 : /* %g_atpen(#, #, #, #) */
			result=(*DynamicLibrary->fonction[13])(
				LdfParaByte(0),
				LdfParaByte(1),
				LdfParaByte(2),
				LdfParaByte(3)); break;
		case	14 : /* %g_defpaint(#, .) */
			result=(*DynamicLibrary->fonction[14])(
				LdfParaByte(0)); break;
		case	15 : /* %g_box(#, .) */
			result=(*DynamicLibrary->fonction[15])(
				LdfParaByte(0)); break;
		case	16 : /* %g_pie(#, .) */
			result=(*DynamicLibrary->fonction[16])(
				LdfParaByte(0)); break;
		case	17 : /* %g_display(#, %, %, $, .) */
			result=(*DynamicLibrary->fonction[17])(
				LdfParaByte(0),
				LdfParaWord(1),
				LdfParaWord(2),
				pptr[3]); break;
		case	18 : /* %g_textpen(#, #, #) */
			result=(*DynamicLibrary->fonction[18])(
				LdfParaByte(0),
				LdfParaByte(1),
				LdfParaByte(2)); break;
		case	19 : /* %g_usefont(#, $, #?) */
			result=(*DynamicLibrary->fonction[19])(
				LdfParaByte(0),
				pptr[1],
				pptr[2]); break;
		case	20 : /* %g_freefont(#, #) */
			result=(*DynamicLibrary->fonction[20])(
				LdfParaByte(0),
				LdfParaByte(1)); break;
		case	21 : /* %g_palet(#, .) */
			result=(*DynamicLibrary->fonction[21])(
				LdfParaByte(0)); break;
		case	22 : /* %g_clear(#, .) */
			result=(*DynamicLibrary->fonction[22])(
				LdfParaByte(0)); break;
		case	23 : /* %g_grconf(#, .) */
			result=(*DynamicLibrary->fonction[23])(
				LdfParaByte(0)); break;
		case	24 : /* %g_conf(#, .) */
			result=(*DynamicLibrary->fonction[24])(
				LdfParaByte(0)); break;
		case	25 : /* %g_pread(#, %, %, %?) */
			result=(*DynamicLibrary->fonction[25])(
				LdfParaByte(0),
				LdfParaWord(1),
				LdfParaWord(2),
				pptr[3]); break;
		case	26 : /* %g_atb(#, .) */
			result=(*DynamicLibrary->fonction[26])(
				LdfParaByte(0)); break;
		case	27 : /* %open_graphic() */
			result=(*DynamicLibrary->fonction[27])(); break;
		case	28 : /* %set_dimension(%, %, .) */
			result=(*DynamicLibrary->fonction[28])(
				LdfParaWord(0),
				LdfParaWord(1)); break;
		case	29 : /* %set_defaultparam() */
			result=(*DynamicLibrary->fonction[29])(); break;
		case	30 : /* %set_charttype(%, %) */
			result=(*DynamicLibrary->fonction[30])(
				LdfParaWord(0),
				LdfParaWord(1)); break;
		case	31 : /* %load_piestate(%, .) */
			result=(*DynamicLibrary->fonction[31])(
				LdfParaWord(0)); break;
		case	32 : /* %load_data(%, .) */
			result=(*DynamicLibrary->fonction[32])(
				LdfParaWord(0)); break;
		case	33 : /* %load_label(%, .) */
			result=(*DynamicLibrary->fonction[33])(
				LdfParaWord(0)); break;
		case	34 : /* %load_legend(%, .) */
			result=(*DynamicLibrary->fonction[34])(
				LdfParaWord(0)); break;
		case	35 : /* %set_maintitle($, .) */
			result=(*DynamicLibrary->fonction[35])(
				pptr[0]); break;
		case	36 : /* %set_subtitle($, .) */
			result=(*DynamicLibrary->fonction[36])(
				pptr[0]); break;
		case	37 : /* %set_xytitles($, $, .) */
			result=(*DynamicLibrary->fonction[37])(
				pptr[0],
				pptr[1]); break;
		case	38 : /* %set_param(%, .) */
			result=(*DynamicLibrary->fonction[38])(
				LdfParaWord(0)); break;
		case	39 : /* %draw_chart() */
			result=(*DynamicLibrary->fonction[39])(); break;
		case	40 : /* %unload_chart() */
			result=(*DynamicLibrary->fonction[40])(); break;
		case	41 : /* %close_graphic() */
			result=(*DynamicLibrary->fonction[41])(); break;
		case	42 : /* %clear_graphic(#) */
			result=(*DynamicLibrary->fonction[42])(
				LdfParaByte(0)); break;
		case	43 : /* %create_screen(%?, %, .) */
			result=(*DynamicLibrary->fonction[43])(
				pptr[0],
				LdfParaWord(1)); break;
		case	44 : /* %use_screen(%) */
			result=(*DynamicLibrary->fonction[44])(
				LdfParaWord(0)); break;
		case	45 : /* %load_prndrv($) */
			result=(*DynamicLibrary->fonction[45])(
				pptr[0]); break;
		case	46 : /* %print_chart(%, %, .) */
			result=(*DynamicLibrary->fonction[46])(
				LdfParaWord(0),
				LdfParaWord(1)); break;
		case	47 : /* %unload_prndrv() */
			result=(*DynamicLibrary->fonction[47])(); break;
		case	48 : /* %delete_screen(%) */
			result=(*DynamicLibrary->fonction[48])(
				LdfParaWord(0)); break;
		case	49 : /* %g_end() */
			result=(*DynamicLibrary->fonction[49])(); break;
		default	: result=138;
		break;
		}
	/* End */
	if ( trace_handle != (FILE*)0) {
		trace_result( result );
	trace_call(fid,argc,tptr,pptr,wptr);
		}
	return(result);
}

/* Library Constructor */

EXAWORD LdfBdaInitialise(
	BDA_INIT FAR PTR descriptor,
	EXAWORD dlength)
{
	int	i=0;
	rt_init = descriptor;
	function_counter=50;
	function_names[0] = "%g_init()";
	function_names[1] = "%g_assign(#, $, #, #)";
	function_names[2] = "%g_open(#)";
	function_names[3] = "%g_close(#)";
	function_names[4] = "%g_window(#, .)";
	function_names[5] = "%g_viewport(#, .)";
	function_names[6] = "%g_points(#, .)";
	function_names[7] = "%g_lines(#, .)";
	function_names[8] = "%g_circle(#, .)";
	function_names[9] = "%g_arc(#, .)";
	function_names[10] = "%g_point(#, %, %)";
	function_names[11] = "%g_line(#, %, %, %, %)";
	function_names[12] = "%g_paint(#, .)";
	function_names[13] = "%g_atpen(#, #, #, #)";
	function_names[14] = "%g_defpaint(#, .)";
	function_names[15] = "%g_box(#, .)";
	function_names[16] = "%g_pie(#, .)";
	function_names[17] = "%g_display(#, %, %, $, .)";
	function_names[18] = "%g_textpen(#, #, #)";
	function_names[19] = "%g_usefont(#, $, #?)";
	function_names[20] = "%g_freefont(#, #)";
	function_names[21] = "%g_palet(#, .)";
	function_names[22] = "%g_clear(#, .)";
	function_names[23] = "%g_grconf(#, .)";
	function_names[24] = "%g_conf(#, .)";
	function_names[25] = "%g_pread(#, %, %, %?)";
	function_names[26] = "%g_atb(#, .)";
	function_names[27] = "%open_graphic()";
	function_names[28] = "%set_dimension(%, %, .)";
	function_names[29] = "%set_defaultparam()";
	function_names[30] = "%set_charttype(%, %)";
	function_names[31] = "%load_piestate(%, .)";
	function_names[32] = "%load_data(%, .)";
	function_names[33] = "%load_label(%, .)";
	function_names[34] = "%load_legend(%, .)";
	function_names[35] = "%set_maintitle($, .)";
	function_names[36] = "%set_subtitle($, .)";
	function_names[37] = "%set_xytitles($, $, .)";
	function_names[38] = "%set_param(%, .)";
	function_names[39] = "%draw_chart()";
	function_names[40] = "%unload_chart()";
	function_names[41] = "%close_graphic()";
	function_names[42] = "%clear_graphic(#)";
	function_names[43] = "%create_screen(%?, %, .)";
	function_names[44] = "%use_screen(%)";
	function_names[45] = "%load_prndrv($)";
	function_names[46] = "%print_chart(%, %, .)";
	function_names[47] = "%unload_prndrv()";
	function_names[48] = "%delete_screen(%)";
	function_names[49] = "%g_end()";
	start_trace();
	write_trace("LdfBdaInitialise()");
	if (!(DynamicLibrary = InitRelais()))
		return(139);
	else	return(0);
}

#endif  /* __linbda__home_Dev_abal32_src_grafabal_grafabal_c__ */
/* End of File */
