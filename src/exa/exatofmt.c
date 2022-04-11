#ifndef	_exatofmt_c
#define	_exatofmt_c

/*
 *	A T O F M T ( fptr , flen , nptr , nlen )
 *	-----------------------------------------
 *	Generates 	an   ABAL format code   string 
 *			from ABAL Ascii  format string
 *
 */

EXAWORD	atofmt(fptr,flen,nptr,nlen)
BPTR	fptr;			/* String of Ascii Format	*/
EXAWORD	flen;			/* Length of string		*/
BPTR	nptr;			/* Format storage zone		*/
EXAWORD	nlen;			/* Length of storage zone	*/

{
	EXAWORD	xtrl,x,y;
	BPTR	wptr;
	BPTR	hptr;
	EXABYTE	c;

/* Duplicate Result Pointer */
/* ------------------------ */
wptr = nptr;

/* Establish Format Start Code */
/* --------------------------- */
*(wptr++) = (EXABYTE) FMT_DEB; xtrl = 0; nlen--;

/* Initialise Format V control */
/* --------------------------- */
fbcdv = 0; fbcdc = 0; fbcde = 0; 

/* Enter Symbolic Format string Analysis Loop */
/* ------------------------------------------ */

do 	{ 

	/* Collect UPPERCASE character from SYMBOLIC string */
	/* ------------------------------------------------ */
	c = *(fptr++); flen--; c = toupper(c);

	/* Select Symbolic Function representation */
	/* --------------------------------------- */
	switch ( c )	{

#ifdef	ABAL14
		/* Inhibition of insignificant zeros for FORMAT Z */
		/* ---------------------------------------------- */
		case 	'I'	: *(wptr++) = FMT_INH; nlen--; continue;
#endif
		/* Declaration of DYNAMIC value for next element */
		/* --------------------------------------------- */
		case	'?'	: *(wptr++) = FMT_DYN; nlen--; continue;

		/* String cutting functions */
		/* ------------------------ */
		case	'$'	: /* Check for DISPLAY till CHARACTER */
				  /* -------------------------------- */ 
				  if ( *fptr == '(' ) {
					*(wptr++) = FMT_DCH;
				  	fptr++; flen--; /* Avaler le parenthese */
				  	*(wptr++) = *(fptr++); flen--;
				  	fptr++; flen--; /* Avaler le parenthese */
					nlen -= 2;
					}
				else 	{
					/* Display usable portion (LEN$) */
					/* ----------------------------- */
					*(wptr++) = FMT_DOL; nlen--; 
					}
				continue;

		/* White space is always ignored between Function BLOCKS */
		/* ----------------------------------------------------- */
		case	' '	: 
		case	'\t'	: while ((flen > 0 ) && ((*fptr == '\t') || (*fptr == ' '))) { fptr++; flen--; }
				  continue;

		/* Power displacement +- power of 10 */
		/* --------------------------------- */
		case	'V'	: fbcdc |= 1;
				  continue;

		/* Display Constant text string */
		/* ---------------------------- */
		case	'"'	: *(wptr++) = (EXABYTE) FMT_PR;
				  hptr = wptr; wptr++; x = 0;
				  while (( *fptr != '"' ) && ( flen > 0 )) {
					*(wptr++) = *(fptr++);
					x++; flen--;
					}
				  if ( flen == 0 ) { 
					err_val = ABAL_FORMAT; 
					return(0); 
					}
				  *hptr = (EXABYTE) x; fptr++; flen--;
				  nlen -= ( x + 2 );
				  continue;

		/* Format BLOCK separator */
		/* ---------------------- */
		case	','	: TerminateFormatV();
				  *(wptr++) = FMT_SEP; nlen--; continue;

		/* Output justification operators */
		/* ------------------------------ */
		case	'L'	: *(wptr++) = FMT_JL;  nlen--; continue;
		case	'R'	: *(wptr++) = FMT_JR;  nlen--; continue;
		
		/* Insignificant ZERO substitution character */
		/* ----------------------------------------- */ 
		case	'*'	: /* Test for EXPLICITE character */
				  /* ---------------------------- */
				  if ( *fptr == '(' ) {
					*(wptr++) = FMT_ZCH;
				  	fptr++; flen--; /* Avaler le parenthese */
				  	*(wptr++) = *(fptr++); flen--;
				  	fptr++; flen--; /* Avaler le parenthese */
					nlen -= 2;
					}
				else 	{
					/* IMPLICITE default character */
					/* --------------------------- */ 
					*(wptr++) = FMT_ZER; nlen--; 
					}
				continue;

		/* Format libre : USE DEFAULT FORMAT type */
		/* -------------------------------------- */
		case	'E'	: *(wptr++) = FMT_STR; nlen--; continue;

		/* Do not fill to right in ASK with space */
		/* -------------------------------------- */
		case	'F'	: *(wptr++) = FMT_CUT; nlen--; continue;

		/* Numeric number grouping Separator */
		/* --------------------------------- */
		case	'S'	: /* Check for EXPLICTE count or separator */
				  /* ------------------------------------- */
				  if ( *fptr == '(' ) {

					/* Scan past left bracket */
					/* ---------------------- */
					fptr++; flen--;

					/* Test for None Numeric Character */
					/* ------------------------------- */
					if ((*fptr < '0') || ( *fptr > '9')) {

						/* Establish Separator Type */
						/* ------------------------ */
						*(wptr++) = FMT_MCH; nlen--;
					  	*(wptr++) = *(fptr++); flen--;
						nlen--;
						}
#ifdef	ABAL14
					/* Check for second arguament */
					/* -------------------------- */
					if ((*fptr >= '0' ) && ( *fptr <= '9')) {
						*(wptr++) = FMT_MNB; nlen--;
						*(wptr++) = (*(fptr++) - '0');
						nlen--;
						}
#endif
					/* Remove Right Hand Bracket */
					/* ------------------------- */
				  	fptr++; flen--; 
					}
				else 	{
					/* Default TYPE and POSITION */
					/* ------------------------- */
					*(wptr++) = FMT_MIL; 
					nlen--; 
					}
			  	continue;

		/* Optional START of FORMAT symbol */
		/* ------------------------------- */
		case	'('	: continue;

		/* Optional END   of FORMAT symbol */
		/* ------------------------------- */
		case	')'	: TerminateFormatV();
				  *(wptr++) = FMT_FIN; xtrl = 1; nlen--; 
				  continue;
#ifdef	ABAL14
		/* N to Z Decimals Convertor */
		/* ------------------------- */
		case 	'O'	: x = FMT_O; break;	/* Format N or Z  */

		/* HEXADECIMAL format flag */
		/* ----------------------- */
		case	'H'	: *(wptr++) = FMT_H;	/* Hexa Format    */
				  nlen--; continue;	/* -------------- */
#endif
		/* Standard character filter symbols */
		/* --------------------------------- */
		case	'K'	: x = FMT_K; break;
#ifdef	ABAL21
		case	'Q'	: x = FMT_Q; break;
#endif
		case	'D'	: x = FMT_D; break;
		case	'A'	: x = FMT_A; break;
		case	'N'	: x = FMT_N; break;
		case	'Z'	: x = FMT_Z; break;
		case	'C'	: x = FMT_C; break;
		case	'B'	: x = FMT_B; break;
		case	'W'	: x = FMT_W; break;
		case	'U'	: x = FMT_U; break;
		case	'.'	: 
#ifdef	ABAL21
			if ( *fptr == '(' ) {

				/* Scan past left bracket */
				/* ---------------------- */
				fptr++; flen--;
				*(wptr++) = FMT_PCH; nlen--;
				*(wptr++) = *(fptr++); nlen--;
				fptr++; flen--;

				}
			else	{
#endif
				*(wptr++) = FMT_P; nlen--; 
				fbcdc |= 2;
#ifdef	ABAL21
				}
#endif
			continue;

		/* Linefeed Symbol */
		/* --------------- */
		case	'/'	: x = FMT_LF;  break;

		/* Select Screen attribute symbol */
		/* ------------------------------ */ 
		case	'\\'	: x = FMT_ATB; break;

		/* Tabulation symbol */
		/* ----------------- */
		case	'T'	: x = FMT_TAB; break;

		/* Space Symbol */
		/* ------------ */
		case	'X'	: x = FMT_ESP; break;

		/* Sign Symbols */
		/* ------------ */
		case	'+'	: x = FMT_PNG; break;
		case	'-'	: x = FMT_NEG; break;

		/* Incorrect format Symbol */
		/* ----------------------- */
		default		: err_val = ABAL_FORMAT; return(0);
		}

	/* Save Binary Format Code Value */
	/* ----------------------------- */
	*(wptr++) = (EXABYTE) x; nlen--; y = 1;

	/* Avoid leading spaces between filter and count : (Z 2) etc */
	/* --------------------------------------------------------- */
	while (( flen != 0 ) && ( *fptr == ' ')) {
		fptr++; flen--;
		}

	/* Scan and count identical format Symbols 'NNNNN' etc */
	/* --------------------------------------------------- */
	while ((flen != 0) && ( toupper((*fptr)) == c )) { 
		y++; fptr++; flen--;
		}

	/* Watch out for end of format error */
	/* --------------------------------- */
	if ( flen > 0 ) {

		/* Remove leading Zeros (Dit710) */
		/* ----------------------------- */
		if ( x != FMT_ATB )
			while ((flen > 0) && (*fptr == '0')) {
				fptr++; flen--;
				} 

		if (flen > 0 ) {

			/* Check for and Add in Numerical Suffix */
			/* ------------------------------------- */
			if (( *fptr >= '0' ) && ( *(fptr) <= '9' )) {
				y--; x = 0;
				while ((flen > 0) && ( *fptr >= '0' ) && ( *(fptr) <= '9' )) { 
					x = ((x * 10) + ( *(fptr++) - '0' ));
					flen--;
					}
				y += x;
				}
			}
		}

	/* Check for Format Size errors */
	/* ---------------------------- */
	if ( y > 256 ) { err_val = ABAL_FORMAT; return(0); }
	if ( y == 256) { y = 0; }

	/* Adjust any Format V Power displacements */
	/* --------------------------------------- */
	if (( fbcdc & 1 ) == 0) { fbcdv += y; }
	if (( fbcdc & 2 ) == 0) { fbcde += y; }

	/* Store the Filter Operation Count Value */
	/* -------------------------------------- */
	*(wptr++) = (EXABYTE) y; nlen--;
	}

/* Until : END OF SYMBOLIC or END OF RESULT ZONE */
/* --------------------------------------------- */
while ((flen > 0 ) && ( xtrl == 0 ) && ( nlen > 0));

/* Check for Result zone full */
/* -------------------------- */
if ((xtrl == 0) && ( nlen == 0)) {

	/* Scan till end of Symbolic Format string */
	/* --------------------------------------- */
	while ( flen > 0 ) {
		switch ( *(fptr++) ) {
			case ' ' :
			case ')' :
			case 0   : flen--; continue;
			default	 : /* Result buffer Too Small */
				   /* ----------------------- */
				   err_val = ABAL_FORMAT; 
				   return(0);
			}
		}
	}


TerminateFormatV();


/* Padd to end of Result with END_OF_FORMAT */
/* ---------------------------------------- */
if ( (flen == 0) && ( nlen > 0 ) ) { *wptr = (EXABYTE) FMT_FIN; nlen--; }
while ( nlen > 0 ) { *(wptr++) = FMT_FIN; nlen--; }

return(1);
}

#endif	/* _exatofmt_c */
	/* ----------- */


