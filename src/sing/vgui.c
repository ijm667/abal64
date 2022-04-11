/* File generated by /home/abal64/bin/linbda Version 2.1a.0.03 : /home/abal64/src/pixel/visual.def -> /home/abal64/src/pixel/visual.c */
#ifndef __linbda__home_abal64_src_pixel_visual_c__
#define __linbda__home_abal64_src_pixel_visual_c__
#include <stdio.h>
#include <setjmp.h>
#include "ldfbda.h"

private BDA * DynamicLibrary;
	public  LPBDA_INIT	rt_init;
	public  BDA_INIT	v_rt_init;
	public  BDA_ARG rt_arg;

/* ABAL Runtime Error Trap */
private	jmp_buf rt_error_context;
public  VOID rt_error(EXAWORD evalue) { 
	longjmp(rt_error_context,evalue);
	return;
}

private int function_counter=0;
/* ABAL Word Conversions */
public EXAWORD GetAbalInt8(char FAR PTR bptr)
{
	return(*bptr);
}

public EXAWORD GetAbalInt16(BYTE FAR PTR wptr)
{
	return(((*wptr<< 8)|*(wptr+1)));
}

public EXAWORD GetAbalInt32(BYTE FAR PTR wptr)
{
	int length=4;
	EXAWORD result=0;
	while ( length-- ) result = ((result << 8) | (*(wptr++) & 0x00FF));
	return(result);
}

public EXAWORD GetAbalInt64(BYTE FAR PTR wptr)
{
	int length=8;
	EXAWORD result=0;
	while ( length-- ) result = ((result << 8) | (*(wptr++) & 0x00FF));
	return(result);
}

public void PutAbalInt8(char FAR * wptr, EXAWORD v)
{
	*wptr = (v & 0x00FF);
	return;
}
public void PutAbalInt16(BYTE FAR * wptr, EXAWORD v)
{
	int length=2;
	while ( length-- ) { *(wptr +length) = (v & 0x00FF); v >>= 8; }
	return;
}

public void PutAbalInt32(BYTE FAR * wptr, EXAWORD v)
{
	int length=4;
	while ( length-- ) { *(wptr +length) = (v & 0x00FF); v >>= 8; }
	return;
}

public void PutAbalInt64(BYTE FAR * wptr, EXAWORD v)
{
	int length=8;
	while ( length-- ) { *(wptr +length) = (v & 0x00FF); v >>= 8; }
	return;
}

#define LdfParaWord(n) (tptr[n]==1 ? GetAbalInt16(pptr[n]) : (tptr[n]==0 ? GetAbalInt8(pptr[n]) : (tptr[n]==5 ? GetAbalInt32(pptr[n]): GetAbalInt64(pptr[n]) )))
#define LdfParaByte(n) (tptr[n]==1 ? (GetAbalInt16(pptr[n])&0x00FF) : (tptr[n]==0 ? GetAbalInt8(pptr[n]):(tptr[n]==5?(GetAbalInt32(pptr[n])&0x00FF):(GetAbalInt64(pptr[n])&0x00FF))))

private BYTE PTR function_names[256];
private char * trace_name=(char*)0;
private FILE * trace_handle=(FILE*)0;
extern long time(long*);

/* Trace Activation */
private void start_trace()
{
	trace_name = getenv("VISUAL_TRACE");
	if ( trace_name != (char *) 0 ) {
		trace_handle=fopen(trace_name,"w");
		}
	return;
}

/* Trace Write */
private void write_trace(char * mptr)
{
	if ( trace_handle != (FILE*)0) {
		fprintf(trace_handle,mptr);
		fprintf(trace_handle,"\r\n");
		}
	return;
}

/* Trace Result */
private void trace_result(EXAWORD result)
{
	if ( trace_handle != (FILE*)0) {
		fprintf(trace_handle,"RETURN: %u \r\n",result);
		}
	return;
}

/* Trace Call */
private void trace_call(
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
					fprintf(trace_handle,"\t(*w=%x)\r\n",GetAbalInt16(pptr[i]));
					break;
				case 1  : 
					fprintf(trace_handle,"\t(w=%x)\r\n",GetAbalInt16(pptr[i]));
					break;
				case 0x0085 : 
					fprintf(trace_handle,"\t(*w=%x)\r\n",GetAbalInt32(pptr[i]));
					break;
				case 5  : 
					fprintf(trace_handle,"\t(w=%x)\r\n",GetAbalInt32(pptr[i]));
					break;
				case 0x0086 : 
					fprintf(trace_handle,"\t(*w=%x)\r\n",GetAbalInt64(pptr[i]));
					break;
				case 6  : 
					fprintf(trace_handle,"\t(w=%x)\r\n",GetAbalInt64(pptr[i]));
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
private void close_trace()
{
	if ( trace_handle != (FILE*)0) {
		fclose(trace_handle);
		trace_handle=(FILE*)0;
		}
	return;
}

/* Library Information */
public EXAWORD LdfBdaInformation(void * vptr)
{
	printf(
		"\n   Abal Dynamic Library : Visual64.Abal.3.3u.0.01"
		);
	printf(
		"\n   Copyright (c) Amenesik / Sologic \n\n"
		);
	return(0);
}

/* Library Destructor */
public EXAWORD LdfBdaTerminate()
{
	write_trace("LdfBdaTerminate()");
	close_trace();
	if (DynamicLibrary->fin != (VOID (FAR PTR)()) 0)
		 (*DynamicLibrary->fin)();
	return(0);
}

/* Library Call Through */
public EXAWORD LdfBdaExecute(
	EXAWORD fid,
	EXAWORD argc,
	BYTE FAR tptr[],
	VOID FAR PTR FAR pptr[],
	EXAWORD FAR wptr[],
	int FAR * eptr)
{
	EXAWORD	result=0;
	rt_arg.c=argc; rt_arg.v=pptr;
	rt_arg.l=wptr; rt_arg.t=tptr;
	if ((result=setjmp(rt_error_context))!=0) {
		if ( eptr != (int FAR *) 0) *eptr = result;
		return( result );
		}
	if ( trace_handle != (FILE*)0) {
	trace_call(fid,argc,tptr,pptr,wptr);
		}
	/* Version = 1 */
	/* Runtime = "visual" */
	switch ( fid ) {
		case	0 : /* % VisualZone(%,%,%,%,%) */
			result=(*DynamicLibrary->fonction[0])(
				LdfParaWord(0),
				LdfParaWord(1),
				LdfParaWord(2),
				LdfParaWord(3),
				LdfParaWord(4)); break;
		case	1 : /* % VisualLine(%,%,%,%,%,%) */
			result=(*DynamicLibrary->fonction[1])(
				LdfParaWord(0),
				LdfParaWord(1),
				LdfParaWord(2),
				LdfParaWord(3),
				LdfParaWord(4),
				LdfParaWord(5)); break;
		case	2 : /* % VisualFont($,%) */
			result=(*DynamicLibrary->fonction[2])(
				pptr[0],
				LdfParaWord(1)); break;
		case	3 : /* % VisualText(%,%,%,%, %, %,%, $,%,%) */
			result=(*DynamicLibrary->fonction[3])(
				LdfParaWord(0),
				LdfParaWord(1),
				LdfParaWord(2),
				LdfParaWord(3),
				LdfParaWord(4),
				LdfParaWord(5),
				LdfParaWord(6),
				pptr[7],
				LdfParaWord(8),
				LdfParaWord(9)); break;
		case	4 : /* % VisualEdit(%,%,%,%,%,$?,%,%) */
			result=(*DynamicLibrary->fonction[4])(
				LdfParaWord(0),
				LdfParaWord(1),
				LdfParaWord(2),
				LdfParaWord(3),
				LdfParaWord(4),
				pptr[5],
				LdfParaWord(6),
				LdfParaWord(7)); break;
		case	5 : /* % VisualWindow(%,%,%,%,%,$,%,%) */
			result=(*DynamicLibrary->fonction[5])(
				LdfParaWord(0),
				LdfParaWord(1),
				LdfParaWord(2),
				LdfParaWord(3),
				LdfParaWord(4),
				pptr[5],
				LdfParaWord(6),
				LdfParaWord(7)); break;
		case	6 : /* % VisualButton(%,%,%,%, %, %,%, $,%,%) */
			result=(*DynamicLibrary->fonction[6])(
				LdfParaWord(0),
				LdfParaWord(1),
				LdfParaWord(2),
				LdfParaWord(3),
				LdfParaWord(4),
				LdfParaWord(5),
				LdfParaWord(6),
				pptr[7],
				LdfParaWord(8),
				LdfParaWord(9)); break;
		case	7 : /* % VisualCheck (%,%,%,%, %, %,%, $,%,%) */
			result=(*DynamicLibrary->fonction[7])(
				LdfParaWord(0),
				LdfParaWord(1),
				LdfParaWord(2),
				LdfParaWord(3),
				LdfParaWord(4),
				LdfParaWord(5),
				LdfParaWord(6),
				pptr[7],
				LdfParaWord(8),
				LdfParaWord(9)); break;
		case	8 : /* % VisualRadio (%,%,%,%, %, %,%, $,%, %,%) */
			result=(*DynamicLibrary->fonction[8])(
				LdfParaWord(0),
				LdfParaWord(1),
				LdfParaWord(2),
				LdfParaWord(3),
				LdfParaWord(4),
				LdfParaWord(5),
				LdfParaWord(6),
				pptr[7],
				LdfParaWord(8),
				LdfParaWord(9),
				LdfParaWord(10)); break;
		case	9 : /* % VisualImage(%,%,%,%,$,%,%) */
			result=(*DynamicLibrary->fonction[9])(
				LdfParaWord(0),
				LdfParaWord(1),
				LdfParaWord(2),
				LdfParaWord(3),
				pptr[4],
				LdfParaWord(5),
				LdfParaWord(6)); break;
		case	10 : /* % VisualTabPage(%,%,%,%,%,$,%,%,%) */
			result=(*DynamicLibrary->fonction[10])(
				LdfParaWord(0),
				LdfParaWord(1),
				LdfParaWord(2),
				LdfParaWord(3),
				LdfParaWord(4),
				pptr[5],
				LdfParaWord(6),
				LdfParaWord(7),
				LdfParaWord(8)); break;
		case	11 : /* % VisualSelect(%,%,%,%, %, %,%, $,%, %?,%) */
			result=(*DynamicLibrary->fonction[11])(
				LdfParaWord(0),
				LdfParaWord(1),
				LdfParaWord(2),
				LdfParaWord(3),
				LdfParaWord(4),
				LdfParaWord(5),
				LdfParaWord(6),
				pptr[7],
				LdfParaWord(8),
				pptr[9],
				LdfParaWord(10)); break;
		case	12 : /* % VisualScroll(%,%,%,%, %, %,%, %,%,%, %) */
			result=(*DynamicLibrary->fonction[12])(
				LdfParaWord(0),
				LdfParaWord(1),
				LdfParaWord(2),
				LdfParaWord(3),
				LdfParaWord(4),
				LdfParaWord(5),
				LdfParaWord(6),
				LdfParaWord(7),
				LdfParaWord(8),
				LdfParaWord(9),
				LdfParaWord(10)); break;
		case	13 : /* % VisualPalette($,%) */
			result=(*DynamicLibrary->fonction[13])(
				pptr[0],
				LdfParaWord(1)); break;
		case	14 : /* % VisualInitialise(%) */
			result=(*DynamicLibrary->fonction[14])(
				LdfParaWord(0)); break;
		case	15 : /* % VisualLiberate() */
			result=(*DynamicLibrary->fonction[15])(); break;
		case	16 : /* % VisualFreeze() */
			result=(*DynamicLibrary->fonction[16])(); break;
		case	17 : /* % VisualThaw(%,%,%,%) */
			result=(*DynamicLibrary->fonction[17])(
				LdfParaWord(0),
				LdfParaWord(1),
				LdfParaWord(2),
				LdfParaWord(3)); break;
		case	18 : /* % VisualEvent($?) */
			result=(*DynamicLibrary->fonction[18])(
				pptr[0]); break;
		case	19 : /* % VisualFill(%,%,%,%,%,%) */
			result=(*DynamicLibrary->fonction[19])(
				LdfParaWord(0),
				LdfParaWord(1),
				LdfParaWord(2),
				LdfParaWord(3),
				LdfParaWord(4),
				LdfParaWord(5)); break;
		case	20 : /* % VisualKey(%) */
			result=(*DynamicLibrary->fonction[20])(
				LdfParaWord(0)); break;
		case	21 : /* % VisualControl(%,$,%) */
			result=(*DynamicLibrary->fonction[21])(
				LdfParaWord(0),
				pptr[1],
				LdfParaWord(2)); break;
		case	22 : /* % VisualProgress(%,%,%,%, %, %,%, %,%, %) */
			result=(*DynamicLibrary->fonction[22])(
				LdfParaWord(0),
				LdfParaWord(1),
				LdfParaWord(2),
				LdfParaWord(3),
				LdfParaWord(4),
				LdfParaWord(5),
				LdfParaWord(6),
				LdfParaWord(7),
				LdfParaWord(8),
				LdfParaWord(9)); break;
		case	23 : /* % VisualTable(%,%,%,%, %, %,%, $,%, $,%, %, $,%) */
			result=(*DynamicLibrary->fonction[23])(
				LdfParaWord(0),
				LdfParaWord(1),
				LdfParaWord(2),
				LdfParaWord(3),
				LdfParaWord(4),
				LdfParaWord(5),
				LdfParaWord(6),
				pptr[7],
				LdfParaWord(8),
				pptr[9],
				LdfParaWord(10),
				LdfParaWord(11),
				pptr[12],
				LdfParaWord(13)); break;
		case	24 : /* % VisualColour(%,$?,%,%) */
			result=(*DynamicLibrary->fonction[24])(
				LdfParaWord(0),
				pptr[1],
				LdfParaWord(2),
				LdfParaWord(3)); break;
		case	25 : /* % VisualBuffer(%,%) */
			result=(*DynamicLibrary->fonction[25])(
				LdfParaWord(0),
				LdfParaWord(1)); break;
		case	26 : /* % VisualPage(%) */
			result=(*DynamicLibrary->fonction[26])(
				LdfParaWord(0)); break;
		case	27 : /* % VisualGet(%,%,%) */
			result=(*DynamicLibrary->fonction[27])(
				LdfParaWord(0),
				LdfParaWord(1),
				LdfParaWord(2)); break;
		case	28 : /* % VisualGetRow(%,$?,%) */
			result=(*DynamicLibrary->fonction[28])(
				LdfParaWord(0),
				pptr[1],
				LdfParaWord(2)); break;
		case	29 : /* % VisualPutRow(%,$,%) */
			result=(*DynamicLibrary->fonction[29])(
				LdfParaWord(0),
				pptr[1],
				LdfParaWord(2)); break;
		case	30 : /* % VisualPut(%,%,%) */
			result=(*DynamicLibrary->fonction[30])(
				LdfParaWord(0),
				LdfParaWord(1),
				LdfParaWord(2)); break;
		case	31 : /* % VisualDrop(%) */
			result=(*DynamicLibrary->fonction[31])(
				LdfParaWord(0)); break;
		case	32 : /* % VisualTransform(%,%,%,%) */
			result=(*DynamicLibrary->fonction[32])(
				LdfParaWord(0),
				LdfParaWord(1),
				LdfParaWord(2),
				LdfParaWord(3)); break;
		case	33 : /* % VisualPutZone(%,%,%,%,$) */
			result=(*DynamicLibrary->fonction[33])(
				LdfParaWord(0),
				LdfParaWord(1),
				LdfParaWord(2),
				LdfParaWord(3),
				pptr[4]); break;
		case	34 : /* % VisualSwitch(%,%,%,%, %, %,%, $,%,%) */
			result=(*DynamicLibrary->fonction[34])(
				LdfParaWord(0),
				LdfParaWord(1),
				LdfParaWord(2),
				LdfParaWord(3),
				LdfParaWord(4),
				LdfParaWord(5),
				LdfParaWord(6),
				pptr[7],
				LdfParaWord(8),
				LdfParaWord(9)); break;
		case	35 : /* % VisualGraph(%,%,%,%, %, %,%, $,%,%) */
			result=(*DynamicLibrary->fonction[35])(
				LdfParaWord(0),
				LdfParaWord(1),
				LdfParaWord(2),
				LdfParaWord(3),
				LdfParaWord(4),
				LdfParaWord(5),
				LdfParaWord(6),
				pptr[7],
				LdfParaWord(8),
				LdfParaWord(9)); break;
		case	36 : /* % VisualActivate(%,%,%) */
			result=(*DynamicLibrary->fonction[36])(
				LdfParaWord(0),
				LdfParaWord(1),
				LdfParaWord(2)); break;
		case	37 : /* % VisualViewPort(%,%,%,%,%) */
			result=(*DynamicLibrary->fonction[37])(
				LdfParaWord(0),
				LdfParaWord(1),
				LdfParaWord(2),
				LdfParaWord(3),
				LdfParaWord(4)); break;
		case	38 : /* % VisualStyle(%,%,%,%,$,%,$,%) */
			result=(*DynamicLibrary->fonction[38])(
				LdfParaWord(0),
				LdfParaWord(1),
				LdfParaWord(2),
				LdfParaWord(3),
				pptr[4],
				LdfParaWord(5),
				pptr[6],
				LdfParaWord(7)); break;
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

public EXAWORD LdfBdaInitialise(
	BDA_INIT FAR PTR descriptor,
	EXAWORD dlength)
{
	int	i=0;
	rt_init = descriptor;
	function_counter=39;
	function_names[0] = "% VisualZone(%,%,%,%,%)";
	function_names[1] = "% VisualLine(%,%,%,%,%,%)";
	function_names[2] = "% VisualFont($,%)";
	function_names[3] = "% VisualText(%,%,%,%, %, %,%, $,%,%)";
	function_names[4] = "% VisualEdit(%,%,%,%,%,$?,%,%)";
	function_names[5] = "% VisualWindow(%,%,%,%,%,$,%,%)";
	function_names[6] = "% VisualButton(%,%,%,%, %, %,%, $,%,%)";
	function_names[7] = "% VisualCheck (%,%,%,%, %, %,%, $,%,%)";
	function_names[8] = "% VisualRadio (%,%,%,%, %, %,%, $,%, %,%)";
	function_names[9] = "% VisualImage(%,%,%,%,$,%,%)";
	function_names[10] = "% VisualTabPage(%,%,%,%,%,$,%,%,%)";
	function_names[11] = "% VisualSelect(%,%,%,%, %, %,%, $,%, %?,%)";
	function_names[12] = "% VisualScroll(%,%,%,%, %, %,%, %,%,%, %)";
	function_names[13] = "% VisualPalette($,%)";
	function_names[14] = "% VisualInitialise(%)";
	function_names[15] = "% VisualLiberate()";
	function_names[16] = "% VisualFreeze()";
	function_names[17] = "% VisualThaw(%,%,%,%)";
	function_names[18] = "% VisualEvent($?)";
	function_names[19] = "% VisualFill(%,%,%,%,%,%)";
	function_names[20] = "% VisualKey(%)";
	function_names[21] = "% VisualControl(%,$,%)";
	function_names[22] = "% VisualProgress(%,%,%,%, %, %,%, %,%, %)";
	function_names[23] = "% VisualTable(%,%,%,%, %, %,%, $,%, $,%, %, $,%)";
	function_names[24] = "% VisualColour(%,$?,%,%)";
	function_names[25] = "% VisualBuffer(%,%)";
	function_names[26] = "% VisualPage(%)";
	function_names[27] = "% VisualGet(%,%,%)";
	function_names[28] = "% VisualGetRow(%,$?,%)";
	function_names[29] = "% VisualPutRow(%,$,%)";
	function_names[30] = "% VisualPut(%,%,%)";
	function_names[31] = "% VisualDrop(%)";
	function_names[32] = "% VisualTransform(%,%,%,%)";
	function_names[33] = "% VisualPutZone(%,%,%,%,$)";
	function_names[34] = "% VisualSwitch(%,%,%,%, %, %,%, $,%,%)";
	function_names[35] = "% VisualGraph(%,%,%,%, %, %,%, $,%,%)";
	function_names[36] = "% VisualActivate(%,%,%)";
	function_names[37] = "% VisualViewPort(%,%,%,%,%)";
	function_names[38] = "% VisualStyle(%,%,%,%,$,%,$,%)";
	start_trace();
	write_trace("LdfBdaInitialise()");
	if (!(DynamicLibrary = InitRelais()))
		return(139);
	else	return(0);
}

#endif  /* __linbda__home_abal64_src_pixel_visual_c__ */
/* End of File */
