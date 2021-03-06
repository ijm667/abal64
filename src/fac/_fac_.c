/* File generated by WinBda Version 1.1b.0.03 : \Abal31_Dev\src\bda\fac\sources\_fac_.def -> \Abal31_Dev\src\bda\fac\sources\_fac_.c */
#ifndef __linbda__Abal31_Dev_src_bda_fac_sources__fac__c__
#define __linbda__Abal31_Dev_src_bda_fac_sources__fac__c__
#include <stdio.h>
#include <setjmp.h>
#include "bda.h"

EXAWORD	LdfBdaInitialise(BDA_INIT FAR *, EXAWORD);
EXAWORD	LdfBdaExecute(EXAWORD, EXAWORD, BYTE FAR [], VOID FAR * FAR [], EXAWORD FAR [], EXAWORD FAR *);
EXAWORD	LdfBdaTerminate();

static LPBDA_DESC	rt_desc;
LPBDA_INIT       	rt_init;
BDA_ARG          	rt_arg;

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

#define LdfParaWord(n) ( tptr[n] == 1 ? LdfAbalWord(pptr[n]) : LdfAbalByte(pptr[n]) )
#define LdfParaByte(n) ( tptr[n] == 1 ? (LdfAbalWord(pptr[n])&0x00FF) : LdfAbalByte(pptr[n]) )
/* Library Destructor */
EXAWORD LdfBdaTerminate()
{
	if (prt_desc->fin != (VOID (FAR PTR)()) 0)
		 (*prt_desc->fin)();
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
	/* Version = 1 */
	/* Runtime = "fac" */
	/* % StrmOpen(%,$,%) */
	/* % StrmClose(%) */
	/* % StrmFlush(%) */
	/* % StrmGetError(%) */
	/* % StrmClearError(%) */
	/* % StrmReadByte(%,#?) */
	/* % StrmWriteByte(%,#) */
	/* % StrmReadBuf(%,$?,%,%?) */
	/* % StrmWriteBuf(%,$,%,%?) */
	/* % StrmFindByte(%,#,%) */
	/* % StrmFindBuf(%,$,%,%) */
	/* % StrmFindByteInColl(%,$,%,#?,%) */
	/* % StrmSeek(%,%,%) */
	/* % StrmSeek_BCD(%,*,%) */
	/* % StrmTell(%,%?) */
	/* % StrmTell_BCD(%,*?) */
	/* % StrmFileSize(%,%?) */
	/* % StrmFileSize_BCD(%,*?) */
	/* % StrmEof(%) */
	/* % StrmLnAnalyse(%) */
	/* % StrmLnRead(%,$?,%,%?) */
	/* % StrmLnAppend(%,$,%,%?) */
	/* % StrmLnSeek(%,%,%) */
	/* % StrmLnSeek_BCD(%,*,%) */
	/* % StrmLnTell(%,%?) */
	/* % StrmLnTell_BCD(%,*?) */
	/* % StrmLnFileSize(%,%?) */
	/* % StrmLnFileSize_BCD(%,*?) */
	/* % StrmLnEof(%) */
	/* % StrmLnLen(%,%?) */
	/* % StrmLnLen_BCD(%,*?) */
	/* % StrmLnSetTab(%,%) */
	/* % StrmLnGetTab(%,%?) */
	/* % StrmLnFileFormat(%,%?,%?) */
	/* % StrmLnInfoLine(%,%?,%?) */
	/* % StrmLnInfoLine_BCD(%,*?,*?) */
	/* % StrmLnIsLast(%) */
	/* % StrmLnMaxLen(%,%?) */
	/* % StrmLnMaxLen_BCD(%,*?) */
	/* % FileGetVersion($,$?,%,%?) */
	/* % FileCopy($,$,%) */
	/* % FileCopyExt($,$,%,%,%) */
	/* % FileCopyExt_BCD($,$,%,*,*) */
	/* % FileComp($,$,%,%?) */
	/* % FileComp_BCD($,$,%,*?) */
	/* % FileSize($,%,%?) */
	/* % FileSize_BCD($,%,*?) */
	/* % SysMkDir($,%) */
	/* % SysRmDir($) */
	/* % SysNgfUpd($) */
	/* % StrmLnSetPad(%,%) */
	/* % StrmLnGetPad(%,%?) */
	/* % StrmGetFree(%?) */
	/* % StrmReadChar(%,$?) */
	/* % StrmWriteChar(%,$)  */
	/* % StrmLnReadSec(%,$?,%,%?) */
	/* % StrmLnReadKey(%,$?,%,%?,$?,%,%?) */
	/* % StrmLnFindSec(%,$,%,%) */
	/* % StrmLnFindKey(%,$,%,$?,%,%?,%) */
	/* % FileIniFind($,$,%,$,%,$,%,%?) */
	/* % SysGetAbalEnv($,$,%,%?)  */
	/* % StrmFindChar(%,$,%) */
	/* % StrmLnSetFormFeed(%,%) */
	/* % StrmLnGetFormFeed(%,%?) */
	/* % StrmFindCharInColl(%,$,%,$?,%) */
	if (fid > function_counter) result=138; else result=(*prt_desc->fonction[fid])(pptr);
	/* end */
	return(result);
}

/* Library Constructor */

EXAWORD LdfBdaInitialise(
	BDA_INIT FAR PTR descriptor,
	EXAWORD dlength)
{
	int	i=0;
	prt_init = descriptor;
	function_counter=65;
	if (!(prt_desc = InitRelais()))
		return(139);
	else	return(0);
}

#endif  /* __linbda_\Abal31_Dev\src\bda\fac\sources\_fac__c__ */
/* End of File */
