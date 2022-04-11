#ifndef	_ldreport_c
#define	_ldreport_c

/*	------------------------------------	*/
/*			O L D			*/
/*	------------------------------------	*/
/*	Linker Statistics and HTML Reporting 	*/
/*	------------------------------------	*/
/*		Iain James Marshall		*/
/*	------------------------------------	*/

struct	linker_statistics	
{
	EXAWORD	glob_proc;
	EXAWORD	glob_segm;
	EXAWORD	glob_varb;
	EXAWORD	glob_data;
	EXAWORD	glob_size;
	EXAWORD	glob_cons;

	EXAWORD	proc_code;
	EXAWORD	proc_data;
	EXAWORD	proc_varb;
	EXAWORD	proc_size;
	EXAWORD	proc_cons;

	EXAWORD	segm_code;
	EXAWORD	segm_data;
	EXAWORD	segm_varb;
	EXAWORD	segm_size;
	EXAWORD	segm_cons;

	EXAWORD	extended;

	EXAWORD	rowcount;

} 
LinkStat = 
{ 
	0,0,0,0,0,0,	/* global */
	0,0,0,0,0,	/* proc */
	0,0,0,0,0,	/* segm */
	0, 0
};

VOID	report_section( BPTR sptr );

/*	-----------	*/
/*	report_putl	*/
/*	-----------	*/
void	report_putl( BPTR sptr )
{
	if ( Linker.rhandle )
		fprintf(Linker.rhandle,"%s\n",sptr);
	return;
}

/*	-----------	*/
/*	report_puts	*/
/*	-----------	*/
void	report_puts( BPTR sptr )
{
	if ( Linker.rhandle )
		fprintf(Linker.rhandle,"%s",sptr);
	return;
}

/*	-------------	*/
/*	report_puthex	*/
/*	-------------	*/
void	report_puthex( EXAWORD v )
{
	char	buffer[64];
	int	prefix=0;
	if ( Linker.rhandle )
	{
		if ( v & 0xFFFF0000) 
			report_puts("<b>");
		sprintf(buffer,"%llx",v);
		prefix = (8 - strlen(buffer));
		while ( prefix-- )
			fprintf(Linker.rhandle,"_");
		fprintf(Linker.rhandle,"%s",buffer);
		if ( v & 0xFFFF0000) 
			report_puts("</b>");
	}
	return;
}

/*	-------------	*/
/*	report_putdec	*/
/*	-------------	*/
void	report_putdec( EXAWORD v )
{
	if ( Linker.rhandle )
		fprintf(Linker.rhandle,"%llu",v);
	return;
}

/*	------------	*/
/*	start_report	*/
/*	------------	*/
private	int	start_report()
{
	if (!( Linker.reporting )) 
		return(0);
	if (!( Linker.rhandle   )) 
	{
		if ((Linker.rhandle = fopen( Linker.OptionR,"w")) == (FILE *) 0)
			return(( Linker.reporting = 0));
		else	
		{
			report_puts("<html><head><title>ABAL++Linker Report");
			report_putl("</title>");
			report_putl("<link href=style.css rel=stylesheet>");
			report_putl("</head><body><devi align=center>");
			report_puts("<p><h1 align=center>ABAL++Linker Report");
			report_putl("</h1><p>");
		}
	}
	return( 1 );
}

/*	------------	*/
/*	check_report	*/
/*	------------	*/
private	int	check_report()
{
	if (!( Linker.reporting )) 
		return(0);
	if (!( Linker.rhandle   )) 
		return( start_report() );
	else	return(1);
}

/*	------------	*/
/*	close_report	*/
/*	------------	*/
VOID	close_report()
{
	if (!( Linker.reporting )) 
		return;
	else if (!( Linker.rhandle ))
		return;
	else	
	{
		report_putl("</div></body></html>");
		fclose( Linker.rhandle );
		Linker.rhandle = (FILE *) 0;
		return;
	}
	return;
}

/*	--------------	*/
/*	report_section	*/
/*	--------------	*/
VOID	report_section( BPTR sptr )
{
	if (!( check_report()   )) return;
	report_puts("<p><h2 align=center>");
	report_puts( sptr );
	report_putl("</h2><p>");
	return;

}

/*	-----------------	*/
/*	report_open_table	*/
/*	-----------------	*/
VOID	report_open_table()
{
	report_putl("<p><div align=center><table border=1 align=center width='80%%'>");
	LinkStat.rowcount=0;
	return;
}

/*	-----------------	*/
/*	 report_open_row	*/
/*	-----------------	*/
VOID	report_open_row()
{
	if (!( LinkStat.rowcount & 1))
		report_puts("<tr class=EVEN>");
	else	report_puts("<tr class=ODD >");
	LinkStat.rowcount = ((LinkStat.rowcount+1) & 1);
	return;
}

/*	------------------	*/
/*	report_close_table	*/
/*	------------------	*/
VOID	report_close_table()
{
	if (!( check_report()   )) return;
	report_putl("</table></div>");
	return;
}

/*	--------------------	*/
/*	report_library_table	*/
/*	--------------------	*/
VOID	report_library_table()
{
	if (!( check_report()   )) return;
	report_section("Dynamic Library Table");
	report_open_table();
	report_open_row();
	report_putl("<td>Name<td>Number");
	return;
}

/*	----------------	*/
/*	 report_library 	*/
/*	----------------	*/
VOID	report_library( LIBRARY * lptr )
{
	if (!( check_report()   )) return;
	report_open_row();
	report_puts("<td>"); report_putl(lptr->name);
	report_puts("<td>");	 report_putdec(lptr->number );
	report_putl(" ");
	return;	
}

/*	----------------------	*/
/*	report_procedure_table	*/
/*	----------------------	*/
VOID	report_procedure_table()
{
	if (!( check_report()   )) return;
	report_section("Procedure Table");
	report_open_table();
	report_open_row();
	report_putl("<td>Name<td>Identity<td>Module<td>Identity<td>Code<td>Data<td>Variables<td>Memory<td>Constants");
	return;
}

/*	----------------------	*/
/*	report_component_table	*/
/*	----------------------	*/
VOID	report_component_table()
{
	if (!( check_report()   )) return;
	report_section("Component Table");
	report_open_table();
	report_open_row();
#ifdef	ABAL64
	report_putl("<td>Nature<td>Identity<td>Procedures<td>Segments<td>Libraries<td>Data<td>Variables<td>Memory<td>Constants<td>External");
#else
	report_putl("<td>Nature<td>Identity<td>Procedures<td>Segments<td>Libraries<td>Data<td>Variables<td>Memory<td>Constants<td>Extended");
#endif
	return;
}

/*	----------------	*/
/*	report_component	*/
/*	----------------	*/
VOID	report_component( MODULE * mptr, WORD type )
{
	VARIABLE *	vptr;
	WORD		xitems=0;

	if (!( check_report()   )) return;

	report_open_row();
	report_puts("<td>");

	if (!( type ))
		report_puts("MODULE");
	else	report_puts("PROGRAM");

	report_puts("<td>"); report_putl( mptr->rootname );
	report_puts("<td>"); 
	if (!( type )) 
	{
		report_putdec( mptr->proctable.length / (8*mptr->wordsize) );
		LinkStat.glob_proc += (mptr->proctable.length / (8*mptr->wordsize));
	}
	else	report_putdec( LinkStat.glob_proc );

	report_puts("<td>"); 
	if (!( type )) 
	{
		report_putdec( mptr->segmtable.length / (12*mptr->wordsize) );
		LinkStat.glob_segm += (mptr->segmtable.length / (12*mptr->wordsize));
	}
	else	report_putdec( LinkStat.glob_segm );

	report_puts("<td>"); report_putdec( mptr->nblibr );

	report_puts("<td>"); 
	if (!( type )) 
	{
		report_puthex( mptr->varbtable.length  );
		LinkStat.glob_data += mptr->varbtable.length;
	}
	else	report_puthex( LinkStat.glob_data );
		
	report_puts("<td>"); 
	if (!( type )) 
	{
		report_putdec( mptr->nbvarb );
		LinkStat.glob_varb += mptr->nbvarb;
	}
	else	report_putdec( LinkStat.glob_varb );
		
	report_puts("<td>"); 
	if (!( type )) 
	{
		report_puthex( mptr->GdsLimit );
		LinkStat.glob_size += mptr->GdsLimit;
	}
	else	report_puthex( LinkStat.glob_size );
		
	report_puts("<td>"); 
	if (!( type )) 
	{
		report_puthex( mptr->constable.length );
		LinkStat.glob_cons += mptr->constable.length;
	}
	else	report_puthex( LinkStat.glob_cons );

	report_puts("<td>"); 
#ifdef	ABAL64
	if ((vptr = mptr->xvariables.root) != (VARIABLE *) 0)
	{
		xitems=0;
		while (	vptr )
		{
			xitems++;
			vptr = vptr->next;
		}
		report_putdec( xitems );
		LinkStat.extended + xitems;
	}
	else	report_puts("&nbsp;");
#else
	if (!( type )) 
	{
		report_puthex( mptr->EtdLimit );
		LinkStat.extended += mptr->EtdLimit;
	}
	else	report_puthex( LinkStat.extended );
#endif		
	report_putl(" ");
	return;
}

/*	--------------------	*/
/*	report_segment_table	*/
/*	--------------------	*/
VOID	report_segment_table()
{
	if (!( check_report()   )) return;
	report_section("Segment Table");
	report_open_table();
	report_open_row();
	report_putl("<td>Name<td>Number<td>Offset<td>Module<td>Offset<td>Code<td>Data<td>Variables<td>Memory<td>Constants");
	return;
}

/*	-------------------	*/
/*	report_put_location	*/
/*	-------------------	*/
VOID	report_put_location( LOCATION * lptr )
{
	BYTE	buffer[64];
	sprintf(buffer,"%llu,%llu",lptr->sector,lptr->length);
	report_puts( buffer );
	return;
}

/*	----------------------	*/
/*	close_procedure_report	*/
/*	----------------------	*/
VOID	close_procedure_report()
{
	if (!( check_report()   )) return;
	report_open_row();
	report_puts("<td colspan=4>Procedure Totals");
	report_puts("<td>");
	report_puthex(LinkStat.proc_code );
	report_puts("<td>");
	report_puthex(LinkStat.proc_data );
	report_puts("<td>");
	report_puthex(LinkStat.proc_varb );
	report_puts("<td>");
	report_puthex(LinkStat.proc_size );
	report_puts("<td>");
	report_puthex(LinkStat.proc_cons );
	report_close_table();
	return;
}

/*	----------------	*/
/*	report_procedure	*/
/*	----------------	*/
VOID	report_procedure( PROCEDURE * pptr )
{
	if (!( check_report()   )) return;
	report_open_row();
	report_puts("<td>");
	report_puts( pptr->name );
	report_puts("<td>");
	report_puthex(pptr->identity+pptr->original->PdtAdjust);
	report_puts("<td>");
	report_puts( pptr->original->rootname );
	report_puts("<td>");
	report_puthex(pptr->identity);
	report_puts("<td>");
	report_puthex(pptr->tcode.length); 
	report_puts("<td>");
	report_puthex(pptr->lvt.length);
	report_puts("<td>");
	report_puthex(pptr->stats.sector);
	report_puts("<td>");
	report_puthex(pptr->stats.length);
	report_puts("<td>");
	report_puthex(pptr->lct.length);
	report_putl("\0");
	LinkStat.proc_code += pptr->tcode.length;
	LinkStat.proc_data += pptr->lvt.length;
	LinkStat.proc_varb += pptr->stats.sector;
	LinkStat.proc_size += pptr->stats.length;
	LinkStat.proc_cons += pptr->lct.length;
	return;
}

/*	--------------------	*/
/*	close_segment_report	*/
/*	--------------------	*/
VOID	close_segment_report()
{
	if (!( check_report()   )) return;
	report_open_row();
	report_puts("<td colspan=5>Segment Totals");
	report_puts("<td>");
	report_puthex(LinkStat.segm_code );
	report_puts("<td>");
	report_puthex(LinkStat.segm_data );
	report_puts("<td>");
	report_puthex(LinkStat.segm_varb );
	report_puts("<td>");
	report_puthex(LinkStat.segm_size );
	report_puts("<td>");
	report_puthex(LinkStat.segm_cons );
	report_open_row();
	report_puts("<td colspan=5>Combined Totals");
	report_puts("<td>");
	report_puthex(LinkStat.segm_code + LinkStat.proc_code );
	report_puts("<td>");
	report_puthex(LinkStat.segm_data + LinkStat.proc_data );
	report_puts("<td>");
	report_puthex(LinkStat.segm_varb + LinkStat.proc_varb );
	report_puts("<td>");
	report_puthex(LinkStat.segm_size + LinkStat.proc_size );
	report_puts("<td>");
	report_puthex(LinkStat.segm_cons + LinkStat.proc_cons );
	report_close_table();
	return;
}

/*	----------------	*/
/*	 report_segment		*/
/*	----------------	*/
VOID	report_segment( SEGMENT * sptr , EXAWORD offset )
{
	if (!( check_report()   )) return;
	report_open_row();
	report_puts("<td>");
	report_puts( sptr->name );
	report_puts("<td>");
	report_putdec(sptr->identity);
	report_puts("<td>");
	report_puthex(offset);
	report_puts("<td>");
	report_puts( sptr->original->rootname );
	report_puts("<td>");
	report_puthex(sptr->offset);
	report_puts("<td>");
	report_puthex(sptr->tcode.length);
	report_puts("<td>");
	report_puthex(sptr->stats.sector);
	report_puts("<td>");
	report_puthex(sptr->stats.length);
	report_puts("<td>");
	report_puthex(sptr->lvt.length);
	report_puts("<td>");
	report_puthex(sptr->lct.length);
	report_putl("\0");
	LinkStat.segm_code += sptr->tcode.length;
	LinkStat.segm_data += sptr->lvt.length;
	LinkStat.segm_varb += sptr->stats.sector;
	LinkStat.segm_size += sptr->stats.length;
	LinkStat.segm_cons += sptr->lct.length;
	return;
}

/*	-----------------	*/
/*	report_add_module	*/
/*	-----------------	*/
VOID	report_add_module( BPTR nptr )
{
	if (!( check_report()   )) return;
	report_open_table();
	report_open_row();
	report_puts("<td>MODULE<td>");
	report_puts(nptr);
	report_close_table();
	return;
}

/*	--------------------	*/
/*	report_build_program	*/
/*	--------------------	*/
VOID	report_build_program( MODULE * mptr )
{
	if (!( check_report()   )) return;
	report_component( mptr, 1 );
	report_close_table();
	return;
}

/*	-----------------	*/
/*	report_use_module	*/
/*	-----------------	*/
VOID	report_use_module( MODULE * mptr )
{
	if (!( check_report()   )) return;
	report_component( mptr, 0 );
	return;
}

#endif	/* _ldreport_c */

