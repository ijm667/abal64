/*----------------------------------------------------------------------*/
/*									*/
/*		E X E C U T E U R 	A . B . A . L			*/
/*		-------------------------------------			*/
/*									*/
/*		Copyright (c) 1987,2012 Amenesik / Sologic			*/
/*									*/
/*		Module  :	EXD.C  					*/
/*		Version	:	1.4b /2.1a				*/
/*		Date 	:	16/01/1997				*/
/*											*/
/*----------------------------------------------------------------------*/
/* 	Include file for : EXA-C, EXADB-C				*/
/* -------------------------------------------------------------------- */

/* Mises a jour :
 * ------------
 * 16/03/12 : (#BG) Ajout variable globale gdesc.
 */

/* 03/01/97: Pour agf */
#include "pubagf.h"

#define	EXDATASEG

EXABYTE	inbuf[FNMAX];		/* T-Code Filename buffer 	*/	

BINAIRE bufdex;			/* Buffer Index 		*/
BINAIRE kdex;			/* Keyboard buffer index	*/
BINAIRE maxbuf;			/* Buffer limit value 		*/

EXAWORD	len_glob;
EXAWORD	nom_glob;
EXAWORD	var_glob;
EXAWORD	var_locl;

EXAWORD MaxOverlayCode;		/* Maximum length of a segment (octets) */
EXAWORD	MaxIpcSem;		/* Maximum IPC Semaphores		*/

EXAWORD	ExFlgSgn;		/* # Convert 0 => Signed 1 = UnSigned   */ 
EXAWORD	diezfile;		/* Temporary file counter		*/
EXAWORD	segstop;		/* Current max code length		*/

EXAWORD	err_typ=VARB_INT16;
EXAWORD	err_len=0;
EXAWORD err_ptr=(BPTR) 0;

#ifndef PROLOGUE
#ifndef DOS
EXAWORD	idflg,idbal;		/* Process management variables		*/
#endif
#endif

EXAWORD	TcodeTraceControl=0;
EX_FILE_HANDLE TcodeFileHandle;	/* Numero logique du fichier		*/

#ifdef WIN32
BPTR				TcodeFileName;	/* #BG 17/10/01 Nom du fichier */
BPTR				TcodeTradName;	/* #BG 28/04/08 Nom du traduit */
#endif

/*		Variables d'access fichiers			*/
/*		---------------------------			*/

struct fichdt PTR AssignTable;	/* Pointeur du table ASSIGN	*/

/*		Gestion des fonctions Executeur			*/
/*		-------------------------------			*/

EXAWORD	(*smach [16]) ();	/* Pointers to CLFn Integer functions	*/ 

#include	"exmvirt.h"

/*		Gestion des erreurs d'execution				*/
/*		-------------------------------				*/

ERRORPTR	GlobalErrorTrap;	/* Pointer to Error structure	*/

#ifdef WIN32
EXAWORD addtrap;
EXAWORD ValidPtr;
#endif

/*		Gestion des Segments et Procedures			*/
/*		----------------------------------			*/

EXAWORD	cur_seg;			/* Current Segment number 	*/
EXAWORD	procid,procmaxid;		/* Procedure ID and Limit	*/
EXAWORD	encryptiontime;

/*	ABAL Work pointers	*/
/*	------------------	*/

BPTR lineptr;					/* New Line pointer for Resume	  */
BPTR oldlineptr;
BPTR GlobalDataTable;			/* Pointeur vers Descrip Var Glob */
BPTR LocalDataTable;			/* Pointeur vers Descrip Var Locl */
BPTR GlobalDataSegment;			/* Pointeur vers stockage Vars Gl */
BPTR LocalDataSegment;			/* Pointeur de stockage Var Local */
BPTR TcodeByteStack;			/* Pointeur Descripteurs Tempory  */
BPTR TcodeConstants;			/* Pointeur CONSTANTES		  */
BPTR GlobalConstants;			/* Global Constants Base Pointer  */
	
/*	Segment and Procedure table Structures	*/
/*	--------------------------------------	*/
struct segmform	PTR TcodeSegmentTable;	/* Pointeur SEGMENTS		  */
struct procform PTR TcodeProcTable;	/* Pointeur Procedures		  */

BPTR proc_tabl;			/* Descripteur variables proc	*/
BPTR proc_data;			/* Memoire des variables proc	*/

BPTR	TcodeSegment;			/* T-code segment Base (Static) */
BPTR	fdex;				/* T-code Arg value pointer 	*/

EXAWORD	adesc,amode;			/* Arguament management		*/
BPTR 	adex;				/* Argument descriptor Pointer	*/

#ifdef ABALSQL
EXAWORD	gldesc;	/*#BG 16/03/12*/
#endif

/*	Overlay management variables	*/
/*	----------------------------	*/
#include "exovl.h"

#ifdef	UNIX
#define	INCL_WORK
#endif

#ifdef	VMS
#define	INCL_WORK
#endif

#ifdef	WIN32
#define	INCL_WORK
#endif

#ifdef INCL_WORK

/* -------------------------------------------------------------- */
/* These variables are declared in EXBCD.ASM for PROLOGUE and DOS */
/* versions with assemble BCD arithmetic. For UNIX and VMS they   */
/* need to be declared here for the 'C' BCD arithmetic.		  */
/* -------------------------------------------------------------- */

	BPTR 	workptr1;
	EXAWORD	worklen1;
	EXAWORD	worktyp1;

	BPTR	workptr2;
	EXAWORD	worklen2;
	EXAWORD	worktyp2;

	BPTR	workptr3;
	EXAWORD	worklen3;
	EXAWORD	worktyp3;

	/* --------- */
#endif	/* INCL_WORK */
	/* --------- */

	BINAIRE	workval1;
	BINAIRE	workval2;
	BINAIRE	workval3;


EXAWORD	lreg[64];	/* length  registers  	*/
BPTR  	preg[64];	/* Pointer registers	*/	

/*	Variable de travail - Controle d'ecriture des Variables F = M	*/
/*	-------------------------------------------------------------	*/

BPTR 	workptrw;
EXAWORD	worklenw,worktypw,workeepw,extwrite;
BINAIRE	workvalw;
BPTR 	descptrw;
BPTR 	descptr;

#ifndef LINEAR_MEM
BPTR	keepvptr;
EXAWORD	keepvlen;
EXALONG	keepvofs;
#endif

/*	-------------------------------------------------		*/
/*	V I R T U E L   M E M O R Y   M A N A G E M E N T		*/
/*	-------------------------------------------------		*/

BPTR 	fieldex;		/* Pointer to Virtuel memory Buffer	*/
EXAWORD	fieldlen;		/* Length of Virtuel memory buffer	*/
EX_FILE_HANDLE fieldlog;	/* Current Virtuel memory File Handle	*/
EXALONG	fieldpos;		/* Current Virtuel memory file offset	*/

struct	extvform PTR extv_base; /* Pointer to external variable 	*/


#ifdef	ABAL14
EXAWORD	DataImageSector;	/* Sector of Global Data Image		*/
EXAWORD	DataImageLength;	/* Length of Global Data Image		*/
#endif

EXAWORD	ExportTableSector;	/* Sector of Procedure Export Table 	*/
EXAWORD	ExportTableLength;	/* Length of Procedure Export Table 	*/
BPTR	ExportTable;		/* Procedure Export Table		*/

#include "exdmath.c"		/* BCD Work Variables			*/

LSEEKTYPE hugeint;		/* Conversion Controller		*/

#ifdef DEBUGGER
EXAWORD	piege;			/* Debug Break points			*/
EXAWORD trace;			/* Debug Trace Flag declaration		*/
#endif

BPTR 	GlobalNamesTable;	/* Variables names pointer		*/
BPTR 	LocalNamesTable;	/* Variables locales (noms)		*/

EXAWORD	wwtyp;
EXAWORD	wwlen;

EXAWORD	datamax;
EXAWORD	datadex;
EXAWORD	datapnt;
EXAWORD	SegmentCount;

BPTR 	datawork;
BPTR	segdata;

/*	----------------------------------	*/
/*	PRINT/ASK Format Control Variables	*/
/*	----------------------------------	*/
#ifndef	ABAL_RTL

/*	Donnees des Entrees / Sorties Console Caracteres		*/
/*	------------------------------------------------		*/
#ifdef WIN32
EXABYTE	askbuf[ASKBUFMAX+16];	/* Buffer for ASK function 		*/
#else
EXABYTE	askbuf[ASKBUFMAX+16];	/* Buffer for ASK function 		*/
#endif
EXAWORD	LastJumpKey;	/* Key Value causing last debranchement */
EXAWORD	insflg;			/* Insert mode flag			*/
EXAWORD	askjmp[256];	/* Matrix of jump adresses   		*/
EXAWORD	asktst[256];	/* Keys that cause jumps     		*/
EXAWORD	jmpdex;			/* Number to test	     		*/
EXAWORD	askmask;		/* Ask mask for Input control 		*/
EXAWORD 	askfmt;		/* Current Format byte 			*/
EXAWORD	askmust;		/* Nombre de caracteres obligatoire 	*/
BINAIRE	askl;			/* Line of Ask Start	*/
BINAIRE	askc;			/* Column of Ask Start 	*/
EXAWORD	abufdex;		/* Current Ask buffer index		*/
EXAWORD	maxask;			/* Current Ask Buffer limit		*/
EXAWORD	askvirtkbdchar;	/* #BG 21/04/20 ASK VIRTUAL KBD */
EXABYTE	sepdec;
EXABYTE	defmil;			/* Global Separateurs 		*/

#ifdef	ABAL14
EXAWORD	AtbContext;		/* Current screen atribut		*/
EXAWORD	LastFore;		/* Current foreground colour		*/
EXAWORD	LastBack;		/* Current back ground colour		*/
EXAWORD	askcolour;		/* Current ASK colour			*/
EXAWORD	askatrib;		/* Current ASK atributs			*/
#endif
EXAWORD	NationalLanguage;
EXAWORD	dynamat[DYNASIZE];	/* Dynamic fmt store values	*/
EXAWORD	dynadex;		/* Current element		*/
EXAWORD	dynamax;		/* Current Stroe limit		*/

BPTR 	fmtptr;
BPTR	fmtbase;		/* Pointer to current format string */
EXAWORD	fmtflg;
EXAWORD	oblig;
EXAWORD	fmtchk;
EXAWORD	fmtgod[FMTSIZE];
EXAWORD	fmthld[FMTSIZE];
EXAWORD	fmtcpt[FMTSIZE];
EXAWORD	fmtdex;
EXAWORD	fmtmax;
EXAWORD	fmtlen;
EXAWORD	fmthex;
EXAWORD	fmtmilcount;
EXABYTE	fmtmil;

EXAWORD	fbcdc;
EXAWORD	fbcde;
EXAWORD	fbcdf;
BINAIRE	fbcdv;
EXABYTE	fbcdp;
EXABYTE	fbcdm;
EXABYTE	fbcdd;
EXABYTE	fbcdxe;
EXABYTE	fbcdxf;
EXABYTE	zpadn;
#endif

EXALONG rnd_var;		/* Random generator	*/
EXABYTE	ymtabl[12];		/* Month table DATE(4)	*/
EXAWORD	chain;			/* Chain file handle	*/
EXAWORD	loadgo;			/* Loadgo file handle	*/
BPTR 	chptr;			/* Chain Arg pointer	*/

#include	"exdngf.c"	/* SI Data Declarations	*/

/*
 *	Gestion des Variables ETENDUS
 *	-----------------------------
 */

EXAWORD	bigvlen;		/* Longeur de la variable		*/
EXALONG	bigvofs;		/* Offset a partir de la base 		*/
BPTR 	bigbase;		/* Base effective			*/

#ifdef LINEAR_MEM
BPTR   ExtendedMemory;	/* Base Segment Value			*/
#endif

/*
 *	ON TIMER etc Management variables
 *	---------------------------------
 */

			/* -------------------------------------------- */
EXAWORD	timersem;	/* Semaphore d'etat de le horloge 		*/
EXAWORD	timeradr;	/* Adresse de debranchement			*/
EXAWORD	timercpt;	/* Delai programmee de le horloge		*/
EXABYTE	descarto[7];	/* Descriptor Pour ARTO 			*/
			/* -------------------------------------------- */

				/* -------------------------------------------- */
				/* allocates maximum possible storge required	*/
				/* -------------------------------------------- */
EXABYTE	timerproc[ABALPTRSIZE];	/* Resolved procedure pointer for timer call	*/
				/* -------------------------------------------- */

/*
 *	EVENT management
 *	----------------
 */

				/* -------------------------------------------- */
EXAWORD		eventadr;	/* Adresse de le bloc conditionelle de controle */
				/* de l'evenement et traitement eventuelle	*/
				/* -------------------------------------------- */

EXAWORD		EventChannel;

#ifdef	ABAL21
EXAWORD		HardEvent=0;
EXAWORD		EventSignal=0;
unsigned long	EventTime=0L;
#endif

				/* -------------------------------------------- */
				/* allocates maximum possible storge required	*/
				/* -------------------------------------------- */
EXABYTE	eventproc[ABALPTRSIZE];	/* Resolved procedure pointer for event call	*/
				/* -------------------------------------------- */

#ifdef VMS_CHARSET
EXAWORD	VmsCico;	/* Vms Console mode VMS or PROLOGUE		*/
#endif	

