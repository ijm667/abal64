/* File generated by /home/abal64/bin/linbda Version 2.1a.0.03 : /home/abal64/src/bdanet/bdanet.def -> /home/abal64/src/bdanet/bdanet.c */
#ifndef __linbda__home_abal64_src_bdanet_bdanet_c__
#define __linbda__home_abal64_src_bdanet_bdanet_c__
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
/* Library Information */
public EXAWORD LdfBdaInformation(void * vptr)
{
	printf(
		"\n   Abal Dynamic Library : Bd@Net64.Abal.3.3u.0.01"
		);
	printf(
		"\n   Copyright (c) Amenesik / Sologic \n\n"
		);
	return(0);
}

/* Library Destructor */
public EXAWORD LdfBdaTerminate()
{
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
	/* Version = 1 */
	/* Runtime = "bdanet" */
	switch ( fid ) {
		case	0 : /* % HttpInitialise($,%) */
			result=(*DynamicLibrary->fonction[0])(
				pptr[0],
				LdfParaWord(1)); break;
		case	1 : /* % HttpLiberate() */
			result=(*DynamicLibrary->fonction[1])(); break;
		case	2 : /* % HttpOpenServer(%,%) */
			result=(*DynamicLibrary->fonction[2])(
				LdfParaWord(0),
				LdfParaWord(1)); break;
		case	3 : /* % HttpOpenClient($,%,%) */
			result=(*DynamicLibrary->fonction[3])(
				pptr[0],
				LdfParaWord(1),
				LdfParaWord(2)); break;
		case	4 : /* % HttpClose(%,%) */
			result=(*DynamicLibrary->fonction[4])(
				LdfParaWord(0),
				LdfParaWord(1)); break;
		case	5 : /* % HttpAccept(%,%) */
			result=(*DynamicLibrary->fonction[5])(
				LdfParaWord(0),
				LdfParaWord(1)); break;
		case	6 : /* % HttpCommand(%,$,%) */
			result=(*DynamicLibrary->fonction[6])(
				LdfParaWord(0),
				pptr[1],
				LdfParaWord(2)); break;
		case	7 : /* % HttpGet(%,$?,%) */
			result=(*DynamicLibrary->fonction[7])(
				LdfParaWord(0),
				pptr[1],
				LdfParaWord(2)); break;
		case	8 : /* % HttpPut(%,$,%) */
			result=(*DynamicLibrary->fonction[8])(
				LdfParaWord(0),
				pptr[1],
				LdfParaWord(2)); break;
		case	9 : /* % HttpGetLine(%,$?,%) */
			result=(*DynamicLibrary->fonction[9])(
				LdfParaWord(0),
				pptr[1],
				LdfParaWord(2)); break;
		case	10 : /* % HttpPutLine(%,$,%) */
			result=(*DynamicLibrary->fonction[10])(
				LdfParaWord(0),
				pptr[1],
				LdfParaWord(2)); break;
		case	11 : /* % HttpAddress(%,$?,%) */
			result=(*DynamicLibrary->fonction[11])(
				LdfParaWord(0),
				pptr[1],
				LdfParaWord(2)); break;
		case	12 : /* % HttpSetConversion(%) */
			result=(*DynamicLibrary->fonction[12])(
				LdfParaWord(0)); break;
		case	13 : /* % HttpTranscode(%) */
			result=(*DynamicLibrary->fonction[13])(
				LdfParaWord(0)); break;
		case	14 : /* % HttpSetHost(%,$,%) */
			result=(*DynamicLibrary->fonction[14])(
				LdfParaWord(0),
				pptr[1],
				LdfParaWord(2)); break;
		case	15 : /* % HttpSetType($,%) */
			result=(*DynamicLibrary->fonction[15])(
				pptr[0],
				LdfParaWord(1)); break;
		case	16 : /* % HttpPost(%,$,%) */
			result=(*DynamicLibrary->fonction[16])(
				LdfParaWord(0),
				pptr[1],
				LdfParaWord(2)); break;
		case	17 : /* % Reserved() */
			result=(*DynamicLibrary->fonction[17])(); break;
		case	18 : /* % HttpAuthorise($,%) */
			result=(*DynamicLibrary->fonction[18])(
				pptr[0],
				LdfParaWord(1)); break;
		case	19 : /* % HttpProxyAuthorise($,%) */
			result=(*DynamicLibrary->fonction[19])(
				pptr[0],
				LdfParaWord(1)); break;
		case	20 : /* % HttpCryptage(%,$,%,%) */
			result=(*DynamicLibrary->fonction[20])(
				LdfParaWord(0),
				pptr[1],
				LdfParaWord(2),
				LdfParaWord(3)); break;
		case	21 : /* % HttpGetError() */
			result=(*DynamicLibrary->fonction[21])(); break;
		case	22 : /* % HttpGetVersion() */
			result=(*DynamicLibrary->fonction[22])(); break;
		case	23 : /* % HttpEncodeBase64($?,$,%) */
			result=(*DynamicLibrary->fonction[23])(
				pptr[0],
				pptr[1],
				LdfParaWord(2)); break;
		case	24 : /* % HttpDecodeBase64($?,$,%) */
			result=(*DynamicLibrary->fonction[24])(
				pptr[0],
				pptr[1],
				LdfParaWord(2)); break;
		case	25 : /* % HttpResponse(%,%,$,%) */
			result=(*DynamicLibrary->fonction[25])(
				LdfParaWord(0),
				LdfParaWord(1),
				pptr[2],
				LdfParaWord(3)); break;
		case	26 : /* % HttpFile(%,$,%,%) */
			result=(*DynamicLibrary->fonction[26])(
				LdfParaWord(0),
				pptr[1],
				LdfParaWord(2),
				LdfParaWord(3)); break;
		case	27 : /* % HttpSelect(%,%) */
			result=(*DynamicLibrary->fonction[27])(
				LdfParaWord(0),
				LdfParaWord(1)); break;
		case	28 : /* % HttpConfigure($,%) */
			result=(*DynamicLibrary->fonction[28])(
				pptr[0],
				LdfParaWord(1)); break;
		case	29 : /* % HttpLoadTable($,%) */
			result=(*DynamicLibrary->fonction[29])(
				pptr[0],
				LdfParaWord(1)); break;
		case	30 : /* % HttpReadTable($,%) */
			result=(*DynamicLibrary->fonction[30])(
				pptr[0],
				LdfParaWord(1)); break;
		case	31 : /* % HttpGetFile(%,$,%,%,%) */
			result=(*DynamicLibrary->fonction[31])(
				LdfParaWord(0),
				pptr[1],
				LdfParaWord(2),
				LdfParaWord(3),
				LdfParaWord(4)); break;
		case	32 : /* % HttpSize($,%,%,$?) */
			result=(*DynamicLibrary->fonction[32])(
				pptr[0],
				LdfParaWord(1),
				LdfParaWord(2),
				pptr[3]); break;
		case	33 : /* % HttpLength(%,$,%) */
			result=(*DynamicLibrary->fonction[33])(
				LdfParaWord(0),
				pptr[1],
				LdfParaWord(2)); break;
		case	34 : /* % HttpHeader(%,$,%) */
			result=(*DynamicLibrary->fonction[34])(
				LdfParaWord(0),
				pptr[1],
				LdfParaWord(2)); break;
		case	35 : /* % HttpDuplicate(%) */
			result=(*DynamicLibrary->fonction[35])(
				LdfParaWord(0)); break;
		case	36 : /* % HttpSecurity(%,$,$,$,$) */
			result=(*DynamicLibrary->fonction[36])(
				LdfParaWord(0),
				pptr[1],
				pptr[2],
				pptr[3],
				pptr[4]); break;
		default	: result=138;
		break;
		}
	/* End */
	return(result);
}

/* Library Constructor */

public EXAWORD LdfBdaInitialise(
	BDA_INIT FAR PTR descriptor,
	EXAWORD dlength)
{
	int	i=0;
	rt_init = descriptor;
	function_counter=37;
	if (!(DynamicLibrary = InitRelais()))
		return(139);
	else	return(0);
}

#endif  /* __linbda__home_abal64_src_bdanet_bdanet_c__ */
/* End of File */
