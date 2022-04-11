#ifndef	_ldmain_c
#define	_ldmain_c

/*	------------------------------------	*/
/*			O L D			*/
/*	------------------------------------	*/
/*	ABAL Object Linker Starup Operations	*/
/*	------------------------------------	*/
/*		Iain James Marshall		*/
/*	------------------------------------	*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>		
#include "ldmain.h"
#include "ldfile.h"
#include "lderrors.h"	

#ifdef WIN32
#include "woldmess.h"	
#include "ldver.h"	
#endif

private	int analyse_options_file( BPTR aptr );
private	int add_module( BPTR filename );
        int is_target_file_busy(BPTR FileName);

struct linker	Linker;
extern BYTE		ErrorBuffer[256];	
extern int		OptIdra;		

BYTE	workbuffer[1024];

#include "ldreport.c"

EXAWORD	use_module( MODULE * rptr, MODULE * mptr );

/*	-----------------	*/
/*	 compare_strings	*/
/*	-----------------	*/
int	compare_strings( BPTR aptr, BPTR bptr )
{
	if ((!( aptr )) || (!( bptr )))
		return(0);

	while (( *aptr != 0 ) && ( *bptr != 0 ))
		if ( *(aptr++) != *(bptr++) )
			return( 0 );
	if ( *aptr != *bptr )
		return(0);
	else	return(1);
}

/*	-----------------	*/
/*	uppercase_compare	*/
/*	-----------------	*/
int	uppercase_compare( BPTR aptr, BPTR bptr )
{
	EXAWORD	a;
	EXAWORD	b;

	if ((!( aptr )) || (!( bptr )))
		return(0);

	while (1) 
	{
		a = *(aptr++);
		b = *(bptr++);
		if ((a == 0) || (b == 0))
			break;
		if (( a >= 'a' ) && ( a <= 'z' ))
			a -= ('a'-'A');
		if (( b >= 'a' ) && ( b <= 'z' ))
			b -= ('a'-'A');
		if ( a != b ) 
			return(0);
	}

	if ( a == b )
		return(1);
	else	return(0);

}

/*	------------------	*/
/*	uppercase_ncompare	*/
/*	------------------	*/
int	uppercase_ncompare( BPTR aptr, BPTR bptr, int n )
{
	int	a;
	int	b;
	int	i = 0;

	if ((!( aptr )) || (!( bptr )) || (!( n) ))
		return(0);

	while (i < n) 
	{
		a = *(aptr++);
		b = *(bptr++);
		if ((a == 0) || (b == 0))
			break;
		if (( a >= 'a' ) && ( a <= 'z' ))
			a -= ('a'-'A');
		if (( b >= 'a' ) && ( b <= 'z' ))
			b -= ('a'-'A');
		if ( a != b ) 
			return(0);
		i++;
	}

	if ( a == b )
		return(1);
	else	return(0);

}

/*	-----------------	*/
/*	 allocate_string	*/
/*	-----------------	*/
char *	allocate_string( char * vptr  )
{
	char * sptr;
	if ((sptr = (char * ) allocate( (strlen( vptr ) + 1) )) != (char *) 0)
		strcpy( sptr, vptr );
	return( sptr );
}

/*	----------	*/
/*	set_report	*/
/*	----------	*/
VOID	set_report( BPTR rname )
{
	Linker.reporting = 1;
	strcpy( Linker.OptionR, rname );
	return;
}

/*	--------------	*/
/*	reset_location	*/
/*	--------------	*/
VOID	reset_location( LOCATION * lptr )
{
	lptr->sector = 0; lptr->length = 0;
	return;
}

VOID	consume_location( LOCATION * lptr )
{
	if ( lptr ) 
	{
		Linker.sector +=  lptr->sector;
	}
	return;
}

/*	----------------------	*/
/*	consume_debug_location	*/
/*	----------------------	*/
VOID	consume_debug_location( LOCATION * lptr )
{
	if ( lptr ) 
	{
		Linker.debugsector +=  lptr->sector;
	}
	return;
}

/*	------------------	*/
/*	next_free_location	*/
/*	------------------	*/
VOID	next_free_location( LOCATION * lptr )
{
	lptr->sector = Linker.sector;
	lptr->length = 0;
	return;
}

/*	-------------------	*/
/*	next_debug_location	*/
/*	-------------------	*/
VOID	next_debug_location( LOCATION * lptr )
{
	lptr->sector = Linker.debugsector;
	lptr->length = 0;
	return;
}

/*	-------------	*/
/*	copy_location	*/
/*	-------------	*/
VOID	copy_location( LOCATION * lptr, LOCATION * xptr )
{
	lptr->sector = xptr->sector; lptr->length = xptr->length;
	return;
}

/*	------------	*/
/*	report_error	*/
/*	------------	*/
private	int	report_error( int e, char * mptr )
{
	BPTR	nptr;

	if (e != 0)  
	{
		if (mptr)
			sprintf(ErrorBuffer, "%u : %s", e, mptr);
		else
			sprintf(ErrorBuffer, "%u", e);
		if ( e >= 100 )
			issue_error(6, ErrorBuffer);	/* Linker error */
		else	issue_error(7, ErrorBuffer);	/* System error */

		if ( Linker.program ) 
		{
			if ( Linker.program->atname ) 
			{
				nptr = allocate_string( Linker.program->atname );
				close_tcode( Linker.program );	
				Linker.program->atname = (char *) 0;
				unlink( nptr );
				liberate( nptr );
			}

			if ( Linker.program->dbname ) 
			{
				nptr = allocate_string( Linker.program->dbname );
				close_debug( Linker.program );	
				Linker.program->dbname = (char *) 0;
				unlink( nptr );
				liberate( nptr );
			}
		}	
	}

	return(e);
}

/*	------------	*/
/*	linker_error	*/
/*	------------	*/
private	int	linker_error( int e )
{
	return( report_error(e, (char *) 0) );
}

/*	-----------------	*/	
/*	 set_targettrace	*/
/*	-----------------	*/	
private	void	set_targettrace()
{
	Linker.targettrace = 1;
	return;
}

/*	-----------------	*/
/*	 set_sourcetrace	*/
/*	-----------------	*/
private	void	set_sourcetrace()
{
	Linker.sourcetrace = 1;
	return;
}

/*	--------------	*/
/*	integer_option	*/
/*	--------------	*/
private	int	integer_option( char * aptr )
{
	int	value=0;
	if (!( aptr ))
		return( 0 );
  
	for ( 	value=0;
		*aptr != 0;
		aptr++ ) {
		if ( (*aptr >= '0' )
		&&   (*aptr <= '9' ))
			value = ((value * 10) + (*aptr - '0'));
		else	break;
		}
	return( value );
}

/*	-----------	*/
/*	set_verbose	*/
/*	-----------	*/
private	void	set_verbose(char * aptr)
{
	if (!(Linker.verbose = integer_option( aptr )))
		Linker.verbose = 1;
	return;
}

/*	-----------	*/
/*	set_warning	*/
/*	-----------	*/
private	void	set_warning(char * aptr)
{
	if (!(Linker.warning = integer_option( aptr )))
		Linker.warning = 1;
	return;
}

/*	---------	*/
/*	set_check	*/
/*	---------	*/
private	void	set_check()
{
	Linker.check = 1;
	return;
}

/*	---------	*/
/*	set_upper	*/
/*	---------	*/
private	void	set_upper()
{
	Linker.forcecase=1;
	return;
}

/*	------------	*/
/*	respect_case	*/
/*	------------	*/
private	void	respect_case()
{
	Linker.forcecase=0;
	return;
}

/*	---------	*/
/*	set_lower	*/
/*	---------	*/
private	void	set_lower()
{
	Linker.forcecase=2;
	return;
}

/*	---------	*/
/*	set_debug	*/
/*	---------	*/
private	void	set_debug()
{
	Linker.debug = 1;
	return;
}

/*	-------------------	*/
/*	set_advanced_option	*/
/*	-------------------	*/
private	int	set_advanced_option(char * aptr)
{
#ifdef ABAL33A
	if (uppercase_compare(aptr, "NOAUTOLINK")) 
	{	
		Linker.noautolink = 1;
		return 1;
	} 
	else if (uppercase_compare(aptr, "AUTOLINK")) 
	{
		Linker.autolink = 1;
		return 1;
	}
#endif
	return 0;
}

/*	----------	*/
/*	set_target	*/
/*	----------	*/
private	void	set_target(BPTR nptr)
{
	strcpy(Linker.OptionD,nptr);
	Linker.targetname = Linker.OptionD;
	return;
}

/*	--------	*/
/*	solution	*/
/*	--------	*/
void	solution( BPTR nptr, MODULE * mptr )
{
	int	i;
	char	progname[16];

	for ( i=0; i < 15; i++ )
		progname[i] = mptr->progname[i];
	progname[15] = '\0';

	sprintf(ErrorBuffer, "%s in ""%s""", nptr, progname);
	issue_message(8, ErrorBuffer);

	return;
}

#include "ldtcode.c"
#include "ldbuff.c"
#include "ldhead.c"
#include "ldfile.c"
#include "ldreloc.c"
#include "ldname.c"
#include "ldxref.c" 
#include "ldcons.c"
#include "ldvarb.c"
#include "ldproc.c"
#include "ldsegm.c"
#include "ldlibr.c"
#include "ldlibmod.c"
#include "ldmodule.c"

/*	--------------	*/
/*	locate_program	*/
/*	--------------	*/
private	MODULE * locate_program()
{
	MODULE * mptr;

	for (	mptr=Linker.modules.root; mptr != (MODULE *) 0; mptr = mptr->next )
		if ( is_tcode_program( mptr ) )
			break;

	if (!( mptr ))
		return( mptr );

	else	
	{
		if (!( mptr->prev )) 
		{
			if (!( Linker.modules.root = mptr->next ))
				Linker.modules.last = Linker.modules.root;
			else	mptr->next->prev = (MODULE *) 0;
		}
		else  if (( mptr->prev->next = mptr->next ) != (MODULE *) 0)
			mptr->next->prev = mptr->prev;
		else	Linker.modules.last = mptr->prev;

		return( mptr );
	}
}

/*	----------------	*/
/*	resolve_external	*/
/*	----------------	*/
private	int resolve_external( MODULE * program )
{
	XPROCEDURE	*	pptr;
	XSEGMENT	*	sptr;
	FIELD		*	fptr;
	EXAWORD			status;

	/* --------------------------- */
	/* resolve external procedures */
	/* --------------------------- */
	for (	pptr=program->xprocedures.root;
		pptr != (XPROCEDURE *) 0;
		pptr = pptr->next ) 
	{
		if (!( pptr->state )) 
		{
			if (!( pptr->relocate ))
				return((pptr->state=1));
			else if (!( resolve_procedure( pptr ) ))
				return(report_error(102,pptr->name));
			else if (!( pptr->host ))
				return(0);
			else if ((status = use_module( program, pptr->host )) != 0)
				return(report_error(103,pptr->name));
			else if ((status = relocate_external_procedure( pptr )) != 0)
				return(report_error(104,pptr->name));
			else	return((pptr->state=1));
		}
	}
	/* ------------------------- */
	/* resolve external segments */
	/* ------------------------- */
	for (	sptr=program->xsegments.root;
		sptr != (XSEGMENT *) 0;
		sptr = sptr->next )  
	{
		if (!( sptr->state )) 
		{
			if ((!( sptr->relocate )) && ( sptr->number == Linker.minusone ))
				return((sptr->state=1));
			else if (!( resolve_segment( sptr ) ))
				return(report_error(105,sptr->name));
			else if (!( sptr->host ))
				return(0);
			else if ((status = use_module( program, sptr->host )) != 0)
				return(report_error(106,sptr->name));
			else if ((status = relocate_external_segment( sptr )) != 0)
				return(report_error(107,sptr->name));
			else	return((sptr->state=1));
		}
	}

	/* ----------------------- */
	/* resolve external fields */
	/* ----------------------- */
	for (	fptr=program->fields.root;
		fptr != (FIELD *) 0;
		fptr = fptr->next )  
	{
		if (!( fptr->state )) 
		{
			if (!( resolve_field( fptr ) ))
				return(linker_error(100));
			else if (!( fptr->host ))
				return(0);
			else if ((status = use_modules( program, fptr )) != 0)
				return(report_error(101,fptr->name));
			else	return((fptr->state=1));
		}
	}

	return(0);
}

/*	------------	*/
/*	show_library	*/
/*	------------	*/
private	void	show_library( LIBRARY * lptr )
{
	sprintf(ErrorBuffer, "Dll  %s", lptr->name);
	issue_message(0, ErrorBuffer);
	return;
}

/*	--------------	*/
/*	show_procedure	*/
/*	--------------	*/
private	void	show_procedure( PROCEDURE * pptr )
{
	sprintf(ErrorBuffer, "Proc %s() in %s at (%llu,%llu)",
		pptr->name,
		pptr->host->rootname,
		pptr->tcode.sector,
		pptr->tcode.length);
	issue_message(0, ErrorBuffer);
	return;
}

/*	------------	*/
/*	show_segment	*/
/*	------------	*/
private	void	show_segment( SEGMENT * sptr )
{
	if ( sptr->name ) 
	{
		sprintf(ErrorBuffer, "Segment %s in %s at (%llu,%llu)",
			sptr->name,
			sptr->host->rootname,
			sptr->tcode.sector,
			sptr->tcode.length);
		issue_message(0, ErrorBuffer);
	}
	else	
	{
		sprintf(ErrorBuffer, "Segment %llu in %s at (%llu,%llu)",
			sptr->identity,
			sptr->host->rootname,
			sptr->tcode.sector,
			sptr->tcode.length);
		issue_message(0, ErrorBuffer);
	}
	return;
}

/*	-----------	*/
/*	show_module	*/
/*	-----------	*/
private	void	show_module(BPTR message, MODULE * mptr )
{
	int	i;
	char	progname[16];

	for ( i=0; i < 15; i++ )
		progname[i] = mptr->progname[i];
	progname[15] = '\0';

	sprintf(ErrorBuffer, "%s : ""%s""", message, progname);
	issue_message(8, ErrorBuffer);
}	

/*	------------	*/
/*	show_program	*/
/*	------------	*/
private	void	show_program( MODULE * mptr )
{
	show_module("Program",mptr);
	return;
}

/*	--------------	*/
/*	show_component	*/
/*	--------------	*/
private	void	show_component( MODULE * mptr )
{
	show_module("\tModule ",mptr);
	return;
}

/*	-------------	*/
/*	build_program	*/
/*	-------------	*/
private	int	build_program( MODULE * mptr )
{
	COMPONENT	*	cptr;
	LIBRARY  	*	lptr;
	PROCEDURE	*	pptr;
	SEGMENT		*	sptr;
	FIELD		*	fptr;
	EXAWORD			status;

	report_build_program( mptr );

	if ( Linker.verbose & 1 )  
	{
		show_program( mptr );
		for (	cptr=mptr->components.root;
			cptr != (COMPONENT*)0;
			cptr = cptr->next ) 
			show_component( cptr->module );
	}

#ifdef ABAL64
#ifdef ABAL32DBG
	Linker.sector = 4; Linker.debugsector = 4;
#else
	Linker.sector = 2; Linker.debugsector = 2;
#endif
#else
#ifdef ABAL32DBG
	Linker.sector = 2; Linker.debugsector = 2;
#else
	Linker.sector = 1; Linker.debugsector = 1;
#endif
#endif

	/* --------- */
	/* Filenames */
	/* --------- */
	flush_filenames( mptr );
#ifdef ABAL32DBG
	flush_modnames( mptr );
#endif

	/* --------- */
	/* Libraries */
	/* --------- */
	if ( start_library_table(mptr) != 0 ) 
	{

		for (	lptr=mptr->libraries.root;
			lptr != (LIBRARY*)0;
			lptr = lptr->next ) 
		{
			if ( Linker.verbose & 1 ) 
				show_library( lptr );
			output_library( mptr, lptr );
		}
		close_library_table(mptr);
	}

	/* --------- */
	/* Constants */
	/* --------- */
	start_constant_table(mptr);

	/* --------------------------------------------- */
	/* we just show the collection of pieces for now */
	/* --------------------------------------------- */
	for (	cptr=mptr->components.root;
		cptr != (COMPONENT*)0;
		cptr = cptr->next )
		if ((status = output_constants( mptr, cptr->module )) != 0)
			return(status);

	close_constant_table(mptr);

	/* --------- */
	/* Variables */
	/* --------- */
	start_variable_names_table(mptr);
	start_variable_table(mptr);

	/* --------------------------------------------- */
	/* we just show the collection of pieces for now */
	/* --------------------------------------------- */
	for (	cptr=mptr->components.root;
		cptr != (COMPONENT*)0;
		cptr = cptr->next )
		if ((status = output_variables( mptr, cptr->module )) != 0)
			return(status);

	close_variable_table(mptr);
	flush_variable_names_table(mptr);

#ifdef ABAL32DBG
#ifdef DUPLICATED_TABLE
	flush_new_variable_names_table(mptr);
#endif
	flush_variable_host_table(mptr);
#endif

	/* ------------------------ */
	/* relocate external fields */
	/* ------------------------ */
	for (	fptr=mptr->fields.root;
		fptr != (FIELD *) 0;
		fptr = fptr->next )  
	{
		if (!( fptr->state ))
			continue;
		else if ((status = relocate_extern_field( fptr )) != 0)
			return(report_error(108,fptr->name));
	}

	/* ---------- */
	/* Procedures */
	/* ---------- */
	start_procedure_table(mptr);

	for (	pptr=mptr->procedures.root;
		pptr != (PROCEDURE*)0;
		pptr = pptr->next ) 
	{
		if ( Linker.verbose & 1 ) 
			show_procedure( pptr );
		if ((status = output_procedure( mptr, pptr )) != 0)
			return(report_error(status,fptr->name) );
	}

	close_procedure_table(mptr);
	flush_procedure_names_table(mptr);

#ifdef ABAL32DBG
#ifdef DUPLICATED_TABLE
	flush_new_procedure_names_table(mptr);
#endif
	flush_procedure_host_table(mptr);
#endif

	/* -------- */
	/* Segments */
	/* -------- */
	start_segment_table(mptr);

	for (	sptr=mptr->segments.root;
		sptr != (SEGMENT*)0;
		sptr = sptr->next ) 
	{
		if ( Linker.verbose & 1 ) 
			show_segment( sptr );
		if ((status = output_segment( mptr, sptr )) != 0)
			return(report_error(status,fptr->name) );
	}

	close_segment_table(mptr);
	flush_segment_names_table(mptr);

#ifdef ABAL32DBG
#ifdef DUPLICATED_TABLE
	flush_new_segment_names_table(mptr);
#endif
	flush_segment_host_table(mptr);
#endif

	/* ------------ */
	/* End: headers */
	/* ------------ */
	flush_tcode( mptr );

	return(0);
}

/*	------------	*/
/*	perform_link	*/
/*	------------	*/
private	int perform_link()
{
	MODULE * mptr;
	BPTR	 nptr;
	WORD	status;
	int	i;

	if ( Linker.check )
		return(0);

	else if ((mptr = locate_program()) != (MODULE *) 0) 
	{
		report_component_table();

		if (!( nptr = Linker.targetname )) 
		{
			if (!(nptr = mptr->rootname))
				return( linker_error( 109 ) );
		}
		else	Linker.targetname = (BPTR) 0;
	
		if (!( Linker.program = allocate_module( nptr ) ))
			return( report_error( 110, nptr ) );
		else if ( mptr->wordsize == 2 ) 
			tcode_version_two( Linker.program );
		else if ( mptr->wordsize == 4 )
		{
			if ( Linker.version != 3 )
				tcode_version_two( Linker.program );
			else	tcode_version_three( Linker.program );
		}
		else
		{
			switch ( Linker.version )
			{
			case	2	:
				tcode_version_two( Linker.program );
				break;
			case	3	:
				tcode_version_three( Linker.program );
				break;
			case	4	:
			default		:
				tcode_version_four( Linker.program );
				break;
			}
		}
	
		for (i=0; i < 16; i++ ) { Linker.program->progname[i] = mptr->progname[i]; }

		if (( status = create_tcode( Linker.program )) != 0)
			return( report_error( 111, nptr ) );

		else if (( status = create_debug( Linker.program )) != 0)
			return( report_error( 112, nptr ) );

		else if ((status = use_module( Linker.program, mptr )) != 0)
			return( report_error( 113, mptr->rootname ) );
		else if ((status = calculate_max_segment( Linker.program )) != 0)
			return( report_error( 205, Linker.program->rootname ) );

		Linker.program->options = mptr->options;

		while ((status = resolve_external( Linker.program )) == 1 );

		if (status != 0)
			return( report_error( 114, nptr ));

		if ((status = build_program( Linker.program )) != 0 ) 
			return( report_error( 115, nptr ) );

		else if ((status = close_tcode( Linker.program )) != 0)
			return( report_error( 116, nptr) );
		else	
		{
			Linker.program = drop_module( Linker.program );
			return( 0 );
		}
	}
	else 	return( linker_error( 117 ) );
}

/*	----------	*/
/*	add_module	*/
/*	----------	*/
private	int add_module( BPTR filename )
{
	int	i, status = 27;
	MODULE * mptr;
	char	fullpathname[LK_MAX_PATH];

	strcpy(fullpathname, filename);

	/* Search the module in the current directory */
	if (!(mptr = allocate_module( filename )))
		return( report_error(110,filename) );

	if ((status = open_tcode( mptr )) != 0) 
	{
		drop_module( mptr );

		/* Search the module in include paths */
		for(i = 0; i < Linker.includes; i++) 
		{
			sprintf(fullpathname, "%s%s", Linker.inclusion[i], filename);
			if (!(mptr = allocate_module( fullpathname )))
				return( report_error(110,fullpathname) );
			if ((status = open_tcode( mptr )) != 0)
				drop_module( mptr );
			else	break;
		}
		if (status != 0)
			return( report_error( 118,fullpathname));
	}

	if ((status = open_debug( mptr )) != 0) 
	{
		drop_module( mptr );
		return( report_error( 119,filename));
	}

	if ((mptr->prev = Linker.modules.last) != (MODULE*) 0)
		Linker.modules.last->next = mptr;
	else	Linker.modules.root = mptr;

	Linker.modules.last = mptr;
	if ((!( status = unlock_headers ( mptr )))
	&&  (!( status = load_module_names ( mptr )))
#ifdef ABAL33A
	&&  (!( status = load_implicite_modules ( mptr )))
#endif
	&&  (!( status = load_filenames ( mptr )))
	&&  (!( status = load_libraries ( mptr )))
	&&  (!( status = load_constants ( mptr )))
	&&  (!( status = load_variables ( mptr )))
	&&  (!( status = load_external_variables ( mptr )))
	&&  (!( status = load_external_fields ( mptr )))
	&&  (!( status = load_procedures ( mptr )))
	&&  (!( status = load_external_procedures ( mptr )))
	&&  (!( status = load_segments ( mptr )))
	&&  (!( status = load_external_segments ( mptr )))
	&&  (!( status = close_debug ( mptr )))
	&&  (!( status = close_tcode ( mptr )))) 
	{
		if  (( Linker.check ) || ( Linker.sourcetrace )) 
		{
			if ((mptr->atheader[0x013] & TCODE_MODULE) == TCODE_MODULE)
				sprintf(ErrorBuffer, "module  : %s , tcode version : %c.%c", mptr->rootname, mptr->atheader[0], mptr->atheader[1]);
			else	sprintf(ErrorBuffer, "program : %s , tcode version : %c.%c", mptr->rootname, mptr->atheader[0], mptr->atheader[1]);
			issue_message(0, ErrorBuffer);
		}
		return( 0 );
	}
	else	return( report_error( 120, filename ) );
}

/*	----------	*/
/*	liberation	*/
/*	----------	*/
private	void	liberation()
{
	MODULE * mptr;
	int	i;
	if ( Linker.atsource ) 
	{
		close_tcode( Linker.atsource );
		Linker.atsource = (MODULE*)0;
	}
	if ( Linker.dbsource ) 
	{
		close_debug( Linker.dbsource );
		Linker.dbsource = (MODULE*)0;
	}
	while ((mptr = Linker.modules.root) != (MODULE *) 0) 
	{
		Linker.modules.root = mptr->next;
		mptr = drop_module( mptr );
	}
	if ((mptr = Linker.program ) != (MODULE*)0)
		mptr = drop_module( mptr );

	if ( Linker.rhandle  != (FILE *) 0)
		close_report();

	if ( Linker.overspill )
	{
		if ( Linker.targettrace ) 
		{
			sprintf(ErrorBuffer, "over spill count : %llu", Linker.overspill);
			issue_message(5, ErrorBuffer);
		}
	}

	for (i=0; i < Linker.includes; i++ )
		if ( Linker.inclusion[i] )
			liberate( Linker.inclusion[i] );

	Linker.includes = 0;
	return;
}

/*	--------------	*/
/*	initialisation	*/
/*	--------------	*/
private	int initialisation()
{
	respect_case();
	if ( sizeof(EXAWORD) > 2 ) 
	{
#ifdef old64
		Linker.version = 4; 
		Linker.wordsize= 8;
		Linker.minusone= MOINS_UN64;
#else
		Linker.version = 3; 
		Linker.wordsize= 4;
		Linker.minusone= MOINS_UN32;
#endif
	}
	else	
	{
		Linker.version = 2;
		Linker.wordsize= 2;
		Linker.minusone= MOINS_UN16;
	}
	Linker.warning = 0;
	Linker.check = 0;
	Linker.exitcode = 0;
	Linker.targettrace = 0;
	Linker.verbose = 0;
	Linker.sourcetrace = 0;
	Linker.debug	= 0;
	Linker.autolink = 0;
	Linker.noautolink = 0;
	Linker.targetname= (BPTR) 0;
	Linker.atsource=
	Linker.dbsource=
	Linker.program = (MODULE *) 0;
	Linker.OptionD[0] = 
	Linker.OptionR[0] =  0;
	Linker.reporting=0;
	Linker.rhandle = (FILE *) 0;
	Linker.modules.root = Linker.modules.last = (MODULE *) 0;
	Linker.overspill=0;
	return(0);
}

/*	----------------	*/
/*	add_include_path	*/
/*	----------------	*/
private	int	add_include_path(char * sptr)
{
	int	status = 0;

	if ( Linker.includes < MAX_INCLUDE ) 
	{
		if (!( Linker.inclusion[Linker.includes] = allocate_string( sptr )))
			status = 27;
		else	Linker.includes++;
	}
	else	status = 30;
	return status;
}

/*	--------------	*/
/*	analyse_option	*/
/*	--------------	*/
public	int	analyse_option( BPTR aptr )
{
	if ( Linker.verbose & 1 ) 
	{
		sprintf(ErrorBuffer, "option : %s",aptr);
		issue_message(0, ErrorBuffer);
	}
	while ( *aptr ) 
	{
		switch ( *(aptr++) ) 
		{
		case 'H'	:
		case 'h'	:
			return(0);

		case 'I'	:
		case 'i'	:
			return( add_include_path(aptr) );

		case 'f'	:
		case 'F'	:
			return( analyse_options_file( aptr ) );

		case 'z'	:
		case 'Z'	:
			switch ( *(aptr++) ) 
			{
			case	't'	:
			case	'T'	:
				set_targettrace();
				continue;
			case	's'	:
			case	'S'	:
				set_sourcetrace();
				continue;
			}
			break;

		case 'a'	:
		case 'A'	:
			break;

		case 'p'	:
		case 'P'	:
			set_check();
			continue;

		case 'v'	:
		case 'V'	:
			set_verbose(aptr);
			break;

		case 'w'	:
		case 'W'	:
			set_warning(aptr);
			break;

		case 'd'	:
		case 'D'	:
			set_target( aptr );
			break;

		case 'r'	:
		case 'R'	:
			set_report( aptr );
			break;

		case 'c'	:
			respect_case();
			continue;

		case 'm'	:
		case 'M'	:
			set_debug();
			continue;

		case '-'	:	
			if (set_advanced_option(aptr))
				break;

		default		:
			return( report_error(121,(aptr-1)) );
		}
		break;
	}
	return( 0 );
}

/*	------------	*/
/*	option_token	*/
/*	------------	*/
private	int	option_token( FILE * h, int first )
{
	int	i;
	int	c;

	if (( workbuffer[(i=0)] = first) != 0)
		i++;
	for (; i < 1024; i ++ ) 
	{
		if ((c = fgetc(h)) > 0) 
		{
			switch ( c ) 
			{
			case	0	:
			case	' '	:
			case	'\t'	:
			case	'\r'	:
			case	'\n'	:
				break;
			default		:
				workbuffer[i] = c;
				continue;
			}
		}
		break;
	}
	workbuffer[i] = 0;
	return(i);
}

/*	--------------------	*/
/*	analyse_options_file	*/
/*	--------------------	*/
private	int	analyse_options_file( BPTR aptr )
{
	FILE	*	handle;
	int		status;
	int		c;

	if (!(handle = fopen( aptr,"r"))) 
		return( report_error(122,aptr));

	while ((c = fgetc(handle))> 0) 
	{
		switch (c) 
		{
		case	' '	:
		case	'\t'	:
		case	'\r'	:
		case	'\n'	:
			continue;
		case	'-'	:
			if (!( option_token( handle,0 ) ))
				break;
			else if ((status = analyse_option( workbuffer )) != 0)
				break;
			else	continue;
		default		:
			if (!( option_token( handle, c ) ))
				break;
			else if ((status = add_module( workbuffer )) != 0)
				break;
			else	continue;

		}
		break;
	}
	fclose(handle);
	return(status); 
}

/*	---------	*/
/*	operation	*/
/*	---------	*/
public	int	operation( int argc, BPTR argv[] )
{
	int	status;
	int	argi;
	BPTR	aptr;
	for (argi=1; argi < argc; argi++ ) 
	{
		if (!( aptr = argv[argi] ))
			break;
		else if ( *aptr == '-' ) 
		{
			if ((status = analyse_option((aptr+1))) != 0)
				return( status );
		}
		else if ((status = add_module( aptr )) != 0)
			return( status );
	}

#ifdef ABAL33A
	if (!Linker.noautolink)
		if ((status = add_implicite_modules()) != 0)
			return( status );
#endif

	return( perform_link() );
}

/*	------	*/
/*	banner	*/
/*	------	*/
private	int	banner()
{
		/* Functions Banner_xxx() implemented in bancode.h and banner.h. */
#ifdef	WIN32
	BYTE Buf[256];

	/* Product name */
	x_cprintf(Banner_GetProductName(Buf));

	/* Version */
	x_cprintf("\n");
	x_cprintf(Banner_GetVersionID(Buf, Banner_GetVersionStatus()));

	/* Build */
	x_cprintf("\n");
	x_cprintf(Banner_GetBuild(Buf));
	x_cprintf("\n");

	/* Copyright */
	x_cprintf(Banner_GetCopyright(Buf));
	x_cprintf("\n");

#else
	/* UNIX */

	printf("\n   ABAL Object Linker : Version 5.0.1 ");
	printf(" -16/32");
#ifdef	old64
	printf("/64");
#endif
	printf("\n   Copyright (c) 2022 Amenesik / Sologic. \n");
	printf("\n");

	printf("\nOptions: ");
	printf("\n       -i<path>     : set module inclusion path ");
	printf("\n       -f<file>     : specify option file ");
	printf("\n       -z<[s|t]>    : set source or target trace ");
	printf("\n       -a           : aliasing of constants ");
	printf("\n       -p           : activate checking ");
	printf("\n       -v           : activate verbose messages ");
	printf("\n       -w           : activate warnings ");
	printf("\n       -r name.html : activate HTML reporting ");
	printf("\n       -d name      : set output target name ");
	printf("\n       -c           : respect case ");
	printf("\n       -m           : generate debug information \n");
	printf("\n");

#endif /* WIN32 */

	return(0);
}


/*	----	*/
/*	main 	*/
/*	----	*/
#ifndef WIN32
public	int	main( int argc, BPTR argv[] )
#else
public	int	majn( int argc, BPTR argv[] )
#endif
{
	int	status;

	if ( argc==1 )
		return((status = banner()));
	else if ((status = initialisation()) != 0) 
		return( status );
	else	
	{
		status = operation(argc,argv);
		liberation();
		return( status );
	}
}

#endif	/* _ldmain_c */

