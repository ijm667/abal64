/* --------------------------------------------------------------------	*/
/*									*/
/*		Project ABAL : Macro pre-processer			*/
/*									*/
/*		 Copyright (c) 1990, 1994 Amenesik / Sologic s.a.	*/
/*									*/
/*									*/
/*		File	:	TRSYMBOL.C				*/
/*		Version :	2.1d					*/
/*		Date	:	08/12/1994				*/
/*									*/
/* --------------------------------------------------------------------	*/
#ifndef	_TRSYMBOL_C
#define	_TRSYMBOL_C

#define	DEBUG

#include <stdio.h>		/* Standard IO handling routines	*/
#include <errno.h>		/* Error return definitions		*/
#include <string.h>

#include "stdtypes.h"
#include "translat.h"
#include "trparser.h"
#include "trsymbol.h"
#include "troption.h"
#include "trdebug.h"
#include "trsyntax.h"
#include "trecho.h"

#ifndef	UNIX
#else
TCVARPTR	temp_variable();
#endif

static	SYMBOLPTR	FreeSymbols=(SYMBOLPTR) 0;
static	WORD		SymbolModule=0;
static	SYMBOLPTR	TerminalSymbol=(SYMBOLPTR) 0;
static	BYTE		SymbolToken[65];
static	LONG		IsSymbol=0L;

/*	--------------	*/
/*	symbol_compare	*/
/*	--------------	*/
WORD	symbol_compare( aptr, bptr )
BPTR	aptr;
BPTR	bptr;
{
	while (( *aptr ) && ( *bptr ))
		if ( *(aptr++) != *(bptr++) )
			return( 0 );

	if ( *aptr != *bptr )
		return( 0 );
	else	return( 1 );
}

/*	-------------------	*/
/*	allocate_for_string	*/
/*	-------------------	*/
BPTR	allocate_for_string( token )
BPTR	token;
{
	BPTR	nptr;
	trace("allocate_for_string("); 
	if ((nptr = (BPTR) allocate( (strlen( token ) + 1) )) != (BPTR) 0)
		strcpy( nptr, token );
	return( nptr );
}

/*	------------------	*/
/*	allocate_for_token	*/
/*	------------------	*/
BPTR	allocate_for_token( token )
BPTR	token;
{
	BPTR	nptr=(BPTR) 0;
	WORD	l;
	WORD	i;
	trace("allocate_for_token("); 
	if ((l = strlen(token)) != 0) 
	{
		if ( l > OtrOption.TokenSize )
			l = OtrOption.TokenSize;		
		if ((nptr = (BPTR) allocate( (l + 1) )) != (BPTR) 0) 
		{
			for ( i = 0; i < l; i++ )
				*(nptr+i) = *(token+i);
			*(nptr+i) = 0;
		}
	}
	return( nptr );
}

/*	--------------	*/
/*	release_symbol	*/
/*	--------------	*/
VOID	release_symbol( sptr )
SYMBOLPTR	sptr;
{
 	if ( sptr != (SYMBOLPTR) 0) 
	{
		trace("liberate_symbol("); trace(sptr->name); trace(")\n");
		if ( sptr->name != (BPTR) 0 )
			liberate( sptr->name );
		/* ------------------------------------------- */
		/* Adds the element to the head of FreeSymbols */
		/* ------------------------------------------- */
		sptr->next = FreeSymbols;
		FreeSymbols = sptr;
	}
	return;
}

/*	---------------------	*/
/*	calculate_symbol_hash	*/
/*	---------------------	*/
WORD	calculate_symbol_hash( nptr , hash )
BPTR	nptr;
WORD	hash;
{
	WORD	result=0;
	while ( *nptr )
		result += *(nptr++);
	return((result % hash));
}

/*	-------------------	*/
/*	allocate_for_symbol	*/
/*	-------------------	*/
SYMBOLPTR	allocate_for_symbol()
{
	SYMBOLPTR	sptr;
	trace("allocate_for_symbol(");
	if ((sptr = FreeSymbols) != (SYMBOLPTR) 0)
		FreeSymbols = sptr->next;
	else if ((sptr = (SYMBOLPTR) allocate( sizeof( struct otr_symbol ) )) == (SYMBOLPTR) 0)
		return( sptr );

	sptr->hash = 0;
	sptr->name = (BPTR) 0;
	sptr->type = 0;
	sptr->module	= 0;
	sptr->owner = SymbolOwner;
	sptr->contents = (VPTR) 0;
	sptr->next = (SYMBOLPTR) 0;

	return( sptr );
}

/*	------------------	*/
/*	initialise_symbols	*/
/*	------------------	*/
VOID		initialise_symbols()
{
	WORD	i;
	for ( i = 0; i < SYMBHASH2; i++ )
		SymbolHash[i] = (SYMBOLPTR) 0;

	SymbolOwner = 0;
	return;
}

/*	----------------	*/
/*	liberate_symbols	*/
/*	----------------	*/
VOID		liberate_symbols()
{
	WORD		i;
	SYMBOLPTR	sptr;
	for ( i = 0; i < SYMBHASH2; i++ ) 
	{
		while ((sptr = SymbolHash[i]) != (SYMBOLPTR) 0) 
		{
			SymbolHash[i] = sptr->next;
			release_symbol( sptr );
		}
	}
	while ((sptr = FreeSymbols) != (SYMBOLPTR) 0) 
	{
		FreeSymbols = sptr->next;
		liberate( sptr );
	}
	return;
}

/*	----------------	*/
/*	new_symbol_owner	*/
/*	----------------	*/
VOID	new_symbol_owner()
{
	SymbolOwner++;
	return;
}

/*	-------------------	*/
/*	verify_symbol_owner	*/
/*	-------------------	*/
WORD	verify_symbol_owner( sptr )
SYMBOLPTR	sptr;
{
	if ( sptr->owner == SymbolOwner )
		return( VRAI );
	else	return( FAUX );
}

/*	------------------------------------------	*/
/*		liberate_local_symbols			*/
/*	------------------------------------------	*/
/*	If a local symbol is after a global symbol	*/
/*	in a list, same hash, it must be released	*/
/*	to avoid a re-declaration error in an 		*/
/*	other procedure or segment code block.		*/
/*	------------------------------------------	*/
VOID	liberate_local_symbols()
{
	WORD		i;
	SYMBOLPTR	sptr;
	SYMBOLPTR	sptr_prev;
	SYMBOLPTR	sptr_next;

	for ( i = 0; i < SYMBHASH2; i++ ) 
	{
		sptr = SymbolHash[i];
		sptr_prev = (SYMBOLPTR)0;
		while (sptr != (SYMBOLPTR)0) 
		{
			if (sptr->type & SYMBOL_LOCAL) 
			{
				sptr_next = sptr->next;
				release_symbol( sptr );
				if (sptr_prev)
					sptr_prev->next = sptr = sptr_next;
				else	SymbolHash[i] = sptr = sptr_next;
			} 
			else 	
			{
				sptr_prev = sptr;
				sptr = sptr->next;
			}
		}
	}
	return;
}

/*	------------------------------------------	*/
/*		   drop_local_symbols			*/
/*	------------------------------------------	*/
/*	If a local symbol is after a global symbol	*/
/*	in a list, same hash, it must be released	*/
/*	to avoid a re-declaration error in an 		*/
/*	other procedure or segment code block.		*/
/*	------------------------------------------	*/
VOID		drop_local_symbols()
{
	WORD		i;
	SYMBOLPTR	sptr;
	SYMBOLPTR	sptr_prev;
	SYMBOLPTR	sptr_next;

	for ( i = 0; i < SYMBHASH2; i++ ) 
	{
		sptr = SymbolHash[i];
		sptr_prev = (SYMBOLPTR)0;
		while (sptr != (SYMBOLPTR)0) 
		{
			if ((sptr->type & SYMBOL_LOCAL) 
			&& (sptr->owner == SymbolOwner)) 
			{
				sptr_next = sptr->next;
				release_symbol( sptr );
				if (sptr_prev)
					sptr_prev->next = sptr = sptr_next;
				else	SymbolHash[i] = sptr = sptr_next;
				}
			else 	
			{
				sptr_prev = sptr;
				sptr = sptr->next;
			}
		}
	}
	return;
}

/*	-----------------	*/
/*	set_symbol_module	*/
/*	-----------------	*/
VOID 	set_symbol_module( modunum )
WORD	modunum;
{
	SymbolModule = modunum;
}

/*	---------	*/
/*	is_symbol	*/
/*	---------	*/
SYMBOLPTR	is_symbol( tptr )
BPTR		tptr;
{
	SYMBOLPTR	sptr;
	WORD		h=0;

	trace("is_symbol("); trace(tptr); trace(") : ");

	/* ------------------------ */
	/* Select hashed list entry */
	/* ------------------------ */
	if ((sptr = SymbolHash[(h=calculate_symbol_hash( tptr, SYMBHASH2 ))]) != (SYMBOLPTR) 0) 
	{
		h = keyword_hash( tptr );
		do	
		{
			if (( sptr->hash == h ) || ( sptr->hash == MOINS_UN)) 
			{
				if (!( symbol_compare( sptr->name, tptr ) ))
					continue;
				else if ((sptr->module != MOINS_UN) 
				     &&  (SymbolModule != sptr->module))
					continue;

				else	break;
			}
		}
		while ((sptr = sptr->next) != (SYMBOLPTR) 0);
	}
	return( sptr );
}

/*	----------------------	*/
/*	get_alternative_symbol	*/
/*	----------------------	*/
SYMBOLPTR	get_alternative_symbol( SYMBOLPTR xptr )
{
	SYMBOLPTR	sptr;
	for (	sptr = xptr->next;
		sptr != (SYMBOLPTR) 0;
		sptr = sptr->next ) 
	{
		if ( sptr->hash != xptr->hash )
			continue;
		else if (!( symbol_compare( sptr->name, xptr->name ) ))
			continue;
		else if ((sptr->module != MOINS_UN) 
		     &&   (SymbolModule != sptr->module))
			continue;
		else	break;
	}

	if ( SymbolModule )
		if ( sptr )
			TerminalSymbol = sptr;
	return( sptr );
}

/*	----------	*/
/*	add_symbol	*/
/*	----------	*/
SYMBOLPTR	add_symbol( token, type )
BPTR		token;
WORD		type;
{
	SYMBOLPTR	sptr;
	WORD		h;
	if ((sptr = allocate_for_symbol()) != (SYMBOLPTR) 0) 
	{
		if ((sptr->name = (BPTR) allocate_for_token( token )) != (BPTR) 0) 
		{
			trace("add_symbol("); trace(token); trace(")\n");
			if ( Context->CurrentXFT )
				sptr->module = Context->CurrentXFT->module;
			sptr->type = type;
			sptr->hash = keyword_hash( sptr->name );
			sptr->next = SymbolHash[(h = calculate_symbol_hash( sptr->name, SYMBHASH2 ))];
			SymbolHash[h] = sptr;
		}
		else	
		{
			release_symbol( sptr );
			sptr = (SYMBOLPTR) 0;
		}
	}
	return( sptr );
}

/*	-------------------------	*/
/*	 consume_terminal_symbol	*/
/*	-------------------------	*/
VOID	consume_terminal_symbol()
{
	TerminalSymbol = (SYMBOLPTR) 0;
}

/*	-------------------	*/
/*	reset_symbol_module	*/
/*	-------------------	*/
VOID	reset_symbol_module()
{
	consume_terminal_symbol();
	SymbolModule = 0;
	return;
}

/*	-------------------	*/
/*	get_terminal_symbol	*/
/*	-------------------	*/
SYMBOLPTR	get_terminal_symbol()
{
	SYMBOLPTR	sptr;
	TCMODPTR	mptr;

	if (!( sptr = TerminalSymbol )) 
	{
		while (1) 
		{
			if (!( sptr = is_symbol( TokenBuffer ) )) 
				break;
			else if ((sptr->type & SYMBOL_TYPE ) != SYMBOL_MODULE)
				break;
			else if (!( mptr = sptr->contents ))
				break;
			else if (!( is_period() ))
				break;
			else if (!( get_token() ))
				break;
			else	SymbolModule = mptr->number;
		}
		if ( SymbolModule )
			TerminalSymbol = sptr;
	}
	return( sptr );
}

/*	--------------	*/
/*	trace_variable	*/
/*	--------------	*/
WORD	trace_variable( sptr )
SYMBOLPTR	sptr;
{
	TCVARPTR	vptr;
	trace("trace_variable()\n");
	if  ((sptr->type & SYMBOL_TYPE) == SYMBOL_VARB) 
	{
		trace("symbol is variable\n");
		if  ((vptr = (TCVARPTR) sptr->contents) != (TCVARPTR) 0) 
		{
			trace("contenu ok\n");
			if  (vptr->type != (TCTYPPTR) 0)
				trace("type ok\n");
			else	trace("type null\n");
		}
		else	trace("contenu null\n");
	}
	else	trace("symbol not variable\n");
	trace("fin_trace_variable()\n");
	return(1);
}

/*	------------	*/
/*	get_variable	*/
/*	------------	*/
TCVARPTR	get_variable(WORD use_alternative)
{
	SYMBOLPTR	sptr=(SYMBOLPTR) 0;
	TCVARPTR	vptr=(TCVARPTR) 0;

	for (	sptr = get_terminal_symbol();
		sptr != (SYMBOLPTR) 0;
		sptr = get_alternative_symbol(sptr)) 
	{
		switch ( (sptr->type & SYMBOL_TYPE) ) 
		{
		case	SYMBOL_VARB	:
			if (((vptr = (TCVARPTR) sptr->contents) != (TCVARPTR) 0)
			&&  (vptr->type != (TCTYPPTR) 0)
			&&  ((vptr = temp_variable( vptr )) != (TCVARPTR) 0)) 
			{
				reset_symbol_module();
				return( vptr );
			}
			else	return( (TCVARPTR) 0 );
		default			:
			if ( use_alternative )
				continue;
			else	return( (TCVARPTR) 0 );
		}
	}
	return( (TCVARPTR) 0 );
}

/*	-----------	*/
/*	echo_symbol	*/
/*	-----------	*/
WORD	echo_symbol( sptr )
SYMBOLPTR	sptr;
{
	sysprintf("trace_symbol( %s , %x )\n",sptr->name,sptr->type);
	return( 1 );
}

/*	------------------	*/
/*	get_local_variable	*/
/*	------------------	*/
TCVARPTR	get_local_variable()
{
	SYMBOLPTR	sptr=(SYMBOLPTR) 0;
	TCVARPTR	vptr=(TCVARPTR) 0;

	if ((( sptr = get_terminal_symbol()) != (SYMBOLPTR) 0)
	&&  ((sptr->type & (SYMBOL_TYPE | SYMBOL_LOCAL))
		== (SYMBOL_VARB | SYMBOL_LOCAL))
	&&  (sptr->owner == SymbolOwner)
	&&  ((vptr = (TCVARPTR) sptr->contents) != (TCVARPTR) 0)
	&&  (vptr->type != (TCTYPPTR) 0)
	&&  ((vptr = temp_variable( vptr )) != (TCVARPTR) 0)) 
	{
		reset_symbol_module();
		return( vptr );
	}
	else	return( (TCVARPTR) 0 );
}

/*	---------	*/
/*	is_object	*/
/*	---------	*/
TCVARPTR 	is_object()
{
	SYMBOLPTR	sptr;
	TCVARPTR	vptr;
	trace("is_object()");
	if ((( sptr = get_terminal_symbol()) != (SYMBOLPTR) 0)
	&&  ((sptr->type & SYMBOL_TYPE) == OBJECT_VARB)
	&&  ((vptr = (TCVARPTR) sptr->contents) != (TCVARPTR) 0)
	&&  (vptr->type != (TCTYPPTR) 0)
	&&  ((vptr = temp_variable( vptr )) != (TCVARPTR) 0)) 
	{
		reset_symbol_module();
		return( vptr );
	}
	else	return( (TCVARPTR) 0 );
}

/*	-----------	*/
/*	is_variable	*/
/*	-----------	*/
TCVARPTR	is_variable( use_alternative)
WORD		use_alternative;
{
	TCVARPTR	vptr=(TCVARPTR) 0;

	trace("is_variable()\n");
	if ((vptr = get_variable(use_alternative)) != (TCVARPTR) 0)
		return(collect_dimensions(vptr));
	else	return( vptr );
}

/*	-----------------	*/
/*	is_local_variable	*/
/*	-----------------	*/
TCVARPTR	is_local_variable()
{
	TCVARPTR	vptr=(TCVARPTR) 0;

	if ((vptr = get_local_variable()) != (TCVARPTR) 0) 
	{
		if ( is_period() ) 
		{
			unget_byte();
			return( vptr );
		}
		else	return(collect_dimensions(vptr));
	}
	else	return( vptr );
}

/*	------------	*/
/*	is_procedure	*/
/*	------------	*/
TCPRCPTR	is_procedure()
{
	SYMBOLPTR	sptr=(SYMBOLPTR) 0;
	TCPRCPTR	pptr=(TCPRCPTR) 0;

	trace("is_procedure()\n");
	for (	sptr = get_terminal_symbol();
		sptr != (SYMBOLPTR) 0;
		sptr = get_alternative_symbol( sptr )) 
	{
		if  (((sptr->type & SYMBOL_TYPE) == SYMBOL_PROC)
		&&   ((pptr = (TCPRCPTR) sptr->contents) != (TCPRCPTR) 0)) 
		{
			reset_symbol_module();
			return( pptr );
		}
		else if (!( OtrOption.DuplicateSymbols ))
			break;
	}
	return( (TCPRCPTR) 0 );
}

/*	-----------	*/
/*	is_constant	*/
/*	-----------	*/
TCCONPTR	is_constant()
{
	SYMBOLPTR	sptr=(SYMBOLPTR) 0;
	TCCONPTR	vptr=(TCCONPTR) 0;

	if (((sptr = is_symbol( TokenBuffer )) != (SYMBOLPTR) 0)
	&&  ((sptr->type & SYMBOL_TYPE) == SYMBOL_CONST)
	&&  ((vptr = (TCCONPTR) sptr->contents) != (TCCONPTR) 0)
	&&  (vptr->type != (TCTYPPTR) 0)) 
	{
		reset_symbol_module();
		return( vptr );
	}
	else	return( (TCCONPTR) 0 );
}

/*	-----------------	*/
/*	 is_dll_function	*/
/*	-----------------	*/
TCDLFPTR	is_dll_function()
{
	SYMBOLPTR	sptr=(SYMBOLPTR) 0;
	TCDLFPTR	fptr=(TCDLFPTR) 0;

	if (((sptr = is_symbol( TokenBuffer )) != (SYMBOLPTR) 0)
	&&  ((sptr->type & SYMBOL_TYPE) == SYMBOL_USER)
	&&  ((fptr = (TCDLFPTR) sptr->contents) != (TCDLFPTR) 0)) 
	{
		reset_symbol_module();
		return( fptr );
	}
	else	return( (TCDLFPTR) 0 );
}

/*	------------------------	*/
/*	resolve_symbolic_integer	*/
/*	------------------------	*/
WORD	resolve_symbolic_integer( token )
BPTR	token;
{
	SYMBOLPTR	sptr=(SYMBOLPTR) 0;
	TCCONPTR	vptr=(TCCONPTR) 0;
	WORD		r;
	BYTE		tokencopy[64];
	for (r=0; r < OtrOption.TokenSize; r++ )
		if (!(tokencopy[r] = *(token+r)))
			break;
	tokencopy[r]=0;
	if (((sptr = is_symbol( tokencopy )) != (SYMBOLPTR) 0)
	&&  ((sptr->type & SYMBOL_TYPE) == SYMBOL_CONST)
	&&  ((vptr = (TCCONPTR) sptr->contents) != (TCCONPTR) 0)
	&&  (vptr->type != (TCTYPPTR) 0)) 
	{
		reset_symbol_module();
		return( vptr->identity );
	}
	else	return( base_member_constant( token ) );
}

	/* ----------- */
#endif	/* _TRSYMBOL_C */
	/* ----------- */

