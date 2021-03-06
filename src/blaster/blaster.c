/* File generated by /opt/prologue/bin/linbda Version 1.1b.0.06 : /home/Dev/abal32/src/blaster/blaster.def -> /home/Dev/abal32/src/blaster/blaster.c */
#ifndef __linbda__home_Dev_abal32_src_blaster_blaster_c__
#define __linbda__home_Dev_abal32_src_blaster_blaster_c__
#include <stdio.h>
#include <setjmp.h>
#include "ldfbda.h"

static BDA * DynamicLibrary;
	LPBDA_INIT	rt_init;
	BDA_INIT	v_rt_init;
	BDA_ARG rt_arg;

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
/* Library Information */
EXAWORD LdfBdaInformation(void * vptr)
{
	printf(
		"\n   Abal Dynamic Library : BLASTER32.abal3.1a.0.20"
		);
	printf(
		"\n   Copyright (c) 2004 Amenesik / Sologic \n\n"
		);
	return(0);
}

/* Library Destructor */
EXAWORD LdfBdaTerminate()
{
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
	/* version = 1 */
	/* runtime = "blaster" */
	switch ( fid ) {
		case	0 : /* % sbinit(%,%,%) */
			result=(*DynamicLibrary->fonction[0])(
				LdfParaWord(0),
				LdfParaWord(1),
				LdfParaWord(2)); break;
		case	1 : /* % sbliber() */
			result=(*DynamicLibrary->fonction[1])(); break;
		case	2 : /* % sbputb(%) */
			result=(*DynamicLibrary->fonction[2])(
				LdfParaWord(0)); break;
		case	3 : /* % sbputz(%,$) */
			result=(*DynamicLibrary->fonction[3])(
				LdfParaWord(0),
				pptr[1]); break;
		case	4 : /* % sbsend(%,$) */
			result=(*DynamicLibrary->fonction[4])(
				LdfParaWord(0),
				pptr[1]); break;
		case	5 : /* % sbstatus()  */
			result=(*DynamicLibrary->fonction[5])(); break;
		case	6 : /* % sbreset() */
			result=(*DynamicLibrary->fonction[6])(); break;
		case	7 : /* % sbtempo(%,%,%) */
			result=(*DynamicLibrary->fonction[7])(
				LdfParaWord(0),
				LdfParaWord(1),
				LdfParaWord(2)); break;
		case	8 : /* % sbrecv(%,$?) */
			result=(*DynamicLibrary->fonction[8])(
				LdfParaWord(0),
				pptr[1]); break;
		case	9 : /* % sbgetz(%,$?) */
			result=(*DynamicLibrary->fonction[9])(
				LdfParaWord(0),
				pptr[1]); break;
		case	10 : /* % sbgetb() */
			result=(*DynamicLibrary->fonction[10])(); break;
		case	11 : /* % sbsound(%) */
			result=(*DynamicLibrary->fonction[11])(
				LdfParaWord(0)); break;
		case	12 : /* % sbdmastate() */
			result=(*DynamicLibrary->fonction[12])(); break;
		case	13 : /* % sbalsm($?,%) */
			result=(*DynamicLibrary->fonction[13])(
				pptr[0],
				LdfParaWord(1)); break;
		case	14 : /* % sblbsm($?)  */
			result=(*DynamicLibrary->fonction[14])(
				pptr[0]); break;
		case	15 : /* % sbgetmix(%) */
			result=(*DynamicLibrary->fonction[15])(
				LdfParaWord(0)); break;
		case	16 : /* % sbputmix(%,%) */
			result=(*DynamicLibrary->fonction[16])(
				LdfParaWord(0),
				LdfParaWord(1)); break;
		case	17 : /* % sbputopl(%,%) */
			result=(*DynamicLibrary->fonction[17])(
				LdfParaWord(0),
				LdfParaWord(1)); break;
		case	18 : /* % sboplnote(%,%,%,%) */
			result=(*DynamicLibrary->fonction[18])(
				LdfParaWord(0),
				LdfParaWord(1),
				LdfParaWord(2),
				LdfParaWord(3)); break;
		case	19 : /* % sboplmode(%,%) */
			result=(*DynamicLibrary->fonction[19])(
				LdfParaWord(0),
				LdfParaWord(1)); break;
		case	20 : /* % sboplctrl(%) */
			result=(*DynamicLibrary->fonction[20])(
				LdfParaWord(0)); break;
		case	21 : /* % sbwavefile($,%,%) */
			result=(*DynamicLibrary->fonction[21])(
				pptr[0],
				LdfParaWord(1),
				LdfParaWord(2)); break;
		case	22 : /* % sbrecord($,%,%,%,%) */
			result=(*DynamicLibrary->fonction[22])(
				pptr[0],
				LdfParaWord(1),
				LdfParaWord(2),
				LdfParaWord(3),
				LdfParaWord(4)); break;
		case	23 : /* % sbtrack(%,$,%,%,%,%) */
			result=(*DynamicLibrary->fonction[23])(
				LdfParaWord(0),
				pptr[1],
				LdfParaWord(2),
				LdfParaWord(3),
				LdfParaWord(4),
				LdfParaWord(5)); break;
		case	24 : /* % sbplay(%,%) */
			result=(*DynamicLibrary->fonction[24])(
				LdfParaWord(0),
				LdfParaWord(1)); break;
		case	25 : /* % sbcontrol(%,%,%,%) */
			result=(*DynamicLibrary->fonction[25])(
				LdfParaWord(0),
				LdfParaWord(1),
				LdfParaWord(2),
				LdfParaWord(3)); break;
		default	: result=138;
		break;
		}
	/* end */
	return(result);
}

/* Library Constructor */

EXAWORD LdfBdaInitialise(
	BDA_INIT FAR PTR descriptor,
	EXAWORD dlength)
{
	int	i=0;
	rt_init = descriptor;
	function_counter=26;
	if (!(DynamicLibrary = InitRelais()))
		return(139);
	else	return(0);
}

#endif  /* __linbda__home_Dev_abal32_src_blaster_blaster_c__ */
/* End of File */
