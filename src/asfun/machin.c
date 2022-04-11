/* File generated by linbda : asfun.def -> asfun.c */
#ifndef __linbda_asfun_c__
#define __linbda_asfun_c__
#include "ldfbda.h"

#define	_DEBUG_ASFUN_BDA

static BDA * DynamicLibrary;
	BDA_INIT	v_rt_init;
	BDA_INIT *	rt_init=&v_rt_init;
	BDA_ARG rt_arg;
/*
	VOID rt_error(EXAWORD evalue) { return; }
 */
static int function_counter=0;

/* ABAL Word Convertor */
static EXAWORD LdfAbalWord(BYTE FAR PTR wptr)
{
	return(((*wptr<< 8) | *(wptr+1)));
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
	int * eptr)
{
	rt_arg.c=argc; rt_arg.v=pptr;
	rt_arg.l=wptr; rt_arg.t=tptr;
	/* version = 1,num */
	/* Explicite Numbering */
	/* runtime = "asfun" */
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
	/* 90:%rledcomp($,$,%) */
	if (fid > function_counter) return(138); else return((*DynamicLibrary->fonction[fid])(pptr));
	/* end */
	return(0);
}

EXAWORD LdfBdaInitialise(
	BDA_INIT FAR PTR descriptor,
	EXAWORD dlength)
{
	rt_init = descriptor;

	function_counter=90;
	DynamicLibrary = InitRelais();
	if (!( DynamicLibrary ))
		return(139);
	else	return(0);
}


#endif  /* __linbda_asfun_c__ */
/* End of File */
