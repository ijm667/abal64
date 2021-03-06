#include <stdio.h>

#define BYTE unsigned char
#define	BPTR BYTE *
#define WORD unsigned short
#define	WPTR WORD *

WORD	prndest=1;
WORD	prnfid=0;
WORD	prnleng=0;
BPTR	prnptr=(BPTR) 0;
WORD	err_val=0;
WORD	prndex=0;

extern	BPTR	x_capacity(WORD);
extern	BPTR	x_table(WORD);

int	main(int argc,char * argv[])
{
	BYTE	buffer[32];
	FILE	*	h;
	int	i;
	int	n;
	int	v;
	char	*	cptr;
	char	*	xptr;
	char	*	evarb=getenv("CICO");
	initerm();
	x_clear();
	if ( argc == 1 ) {
		x_cprintf("\r\n   ABAL CICO Source File Generator : Version 4.1.3");
//		x_cprintf("\r\n   Provisoire du 07/11/2001 ");
		x_cprintf("\r\n   Copyright (c) 2019 A.P.I. \r\n");
		}
	else	{
		if ((h = fopen( argv[1], "w" )) != (FILE *) 0) {
			fprintf(h,"/* file generated by %s */\n",argv[0]);
			fprintf(h,"/* CICO=%s */\n",(evarb != (char *) 0 ? evarb : "<null>" ));
			fprintf(h,"#define COULEUR\n");
			fprintf(h,"char *fichier=%c",0x0022);
			for ( cptr=evarb; *cptr != 0; cptr++ ) {
				if ( *cptr == ':' )
					break;
				else 	fprintf(h,"%c",*cptr);
				}
			fprintf(h,"%c;\n",0x0022);
			fprintf(h,"char *nom =%c",0x0022);
			if ((cptr = x_table(0)) != (char *) 0) {
				for ( cptr=evarb; *cptr != 0; cptr++ ) {
					if ( *cptr == ':' )
						break;
					else 	fprintf(h,"%c",*cptr);
					}
				}
			fprintf(h,"%c;\n",0x0022);
			fprintf(h,"int tab_nb[] = {\n");
			fprintf(h,"\t%u,\n",conf(1));
			fprintf(h,"\t%u,\n",conf(2));
			fprintf(h,"\t8,\n");
			fprintf(h,"\t-1\n");
			fprintf(h,"};\n");

			fprintf(h,"char *tab_str[] = {\n");
			for (i=0; x_capacity(i) != (BPTR) 0; i++ ) {
				fprintf(h,"\t%c",0x0022);
				for (	cptr = x_capacity(i);
					*cptr != 0;
					cptr++ )
					if ( *cptr > ' ' )
						fprintf(h,"%c",(*cptr & 0x00FF));
					else	{
						sprintf(buffer,"\\%03.1o",(*cptr & 0x00FF));
						for ( xptr=buffer; *xptr != 0; xptr++ )
							if ( *xptr == ' ' )
								*xptr = '0';
						fprintf(h,"%s",buffer);
						}
				fprintf(h,"%c,\n",0x0022);
				}
			fprintf(h,"\t(char *) 0\n};\n");
			if ((cptr = x_table(1)) != (char *) 0) {
				fprintf(h,"char  tab_in[256] = {");
				for (n=0,i=0; i < 256; i++ ) {
					if (!( n ))
						fprintf(h,"\n\t");
					n++;
					if ( n ==8 )
						n = 0;
					v = (*(cptr + i) & 0x00FF);
					if (( v == 0x0027 )
					||  ( v == 0x005C )
					||  ( v >  127    )
					||  ( v <= 32     ))
						fprintf(h,"0x%x",v);
					else 	fprintf(h,"'%c'",v);
					if ( i < 255 )
						fprintf(h,",");
					}
				fprintf(h,"\n};\n");
				}
			if ((cptr = x_table(2)) != (char *) 0) {
				fprintf(h,"char  tab_out[256] = {");
				for (n=0,i=0; i < 256; i++ ) {
					if (!( n ))
						fprintf(h,"\n\t");
					n++;
					if ( n ==8 )
						n = 0;
					v = (*(cptr + i) & 0x00FF);
					if (( v == 0x0027 )
					||  ( v == 0x005C )
					||  ( v >  127    )
					||  ( v <= 32     ))
						fprintf(h,"0x%x",v);
					else 	fprintf(h,"'%c'",v);
					if ( i < 255 )
						fprintf(h,",");
					}
				fprintf(h,"\n};\n");
				}

			fprintf(h,"/* end of file */\n");
			fclose(h);
			}
		else	{
			x_cprintf("\r\n** error creating output file ");
			x_cprintf( argv[1] );
			x_cprintf("\r\n");
			}
		}

	finterm();
	return;
}

