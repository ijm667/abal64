/* File generated by linbda Version 1.1b.0.06 : grasfun.def -> grasfun.c */
#ifndef __linbda_grasfun_c__
#define __linbda_grasfun_c__
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
		"\n   Abal Dynamic Library : asfun.graphique.3.1b"
		);
	printf(
		"\n   Copyright (c) 2021 Amenesik / Sologic \r\n"
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
	/* version = 1,num */
	/* Explicite Numbering */
	/* runtime = "grasfun" */
	/* 0: %winit(#,#,#,#,#) */
	/* 1: %wcreat(%? , $) */
	/* 2: %wsup(%) */
	/* 3: %wwrite(%,%,$) */
	/* 4: %wselect(%) */
	/* 5: %winv(%) */
	/* 6: %wvis(%) */
	/* 7: %wfgen(%,%) */
	/* 8: %watb(%,#,#,#) */
	/* 9: %wposcur(%,%,%) */
	/* 10:%wimpl(%) */
	/* 11:%wgel(%) */
	/* 12:%wfend() */
	/* 13:%wsetatb(%,#,#,#,%,%,%,%) */
	/* 14: %sf_init($) */
	/* 15:%sl_activ() */
	/* 16:%sl_mess(#,#,$) */
	/* 17:%sl_inv() */
	/* 18:%sl_desac() */
	/* 19:%menu_creat(#,$,.) */
	/* 20:%menu_option(#,#,$,$,$) */
	/* 21:%menu_sep(#,#) */
	/* 22:%menu_modif_opt(#,#,#,$,$,$) */
	/* 23:%menu_latt_opt(#,#,#?,#?,#?,.) */
	/* 24:%menu_matt_opt(#,#,#,#,#) */
	/* 25:%menu_blank_opt(#,#) */
	/* 26:%menu_vis(#,#,#) */
	/* 27:%menu(#,#,#?,#?,$?,.?) */
	/* 28:%menu_inv(#) */
	/* 29:%menu_sup(#) */
	/* 30:%list_creat(#,$,.) */
	/* 31:%listmul_creat(#,$,.) */
	/* 32:%list_element(#,%?,#,#,$,$) */
	/* 33:%list_read_elt(#,%,#?,#?,$?) */
	/* 34:%list_down(#,%?,#?,#?,$?) */
	/* 35:%list_up(#,%?,#?,#?,$?) */
	/* 36:%list_matt_elt(#,%,#,#,.) */
	/* 37:%list_close(#) */
	/* 38:%list_vis(#,%,%,.) */
	/* 39:%list(#,%?,%?,#?,$?,.?) */
	/* 40:%list_inv(#) */
	/* 41:%list_sup(#) */
	/* 42:%bvalid_spc($,$,#?) */
	/* 43:%bvalid(#,#?) */
	/* 44:%sf_end() */
	/* 45:%wfmod(%,%,%?,$?,#,.?) */
	/* 46:%help_activ(#,$) */
	/* 47:%help($) */
	/* 48:%help_desac() */
	/* 49:%help_def($) */
	/* 50:%wget_impl(%?) */
	/* 51:%bd_creat(%?,%,%,%,%,%,.) */
	/* 52:%bd_insert(%,%,%,%,.) */
	/* 53:%bd_use(%,%,%?,.?) */
	/* 54:%bd_inv(%) */
	/* 55:%bd_sup(%) */
	/* 56:%bd_putdata(%,$) */
	/* 57:%bd_getdata(%,$?) */
	/* 58:%wexit(%) */
	/* 59:%wrefresh() */
	/* 60:%menu_way(#?,$?) */
	/* 61:%swcreat(%?,%,%,%,%,.) */
	/* 62:%wcadre(%,#,.) */
	/* 63:%wtitre(%,#,#,#,.) */
	/* 64:%wmove(%,%,%) */
	/* 65:%listel_del(#,%) */
	/* 66:%listel_ins(#,%?,#,#,$,$) */
	/* 67:%set_tch_esc(#,$) */
	/* 68:%winfo(%,$?) */
	/* 69:%get_style(#,$?) */
	/* 70:%set_style(#,$) */
	/* 71:%wlangue(#) */
	/* 72:%asfvers($?) */
	/* 88:%who_is_there(%,%,%?,%?) */
	/* 89:%rlecomp($,$,%) */
	/* 90:%rledecomp($,$,%) */
	if (fid > function_counter) result=138; else result=(*DynamicLibrary->fonction[fid])(pptr);
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
	function_counter=90;
	if (!(DynamicLibrary = InitRelais()))
		return(139);
	else	return(0);
}

#endif  /* __linbda_grasfun_c__ */
/* End of File */
