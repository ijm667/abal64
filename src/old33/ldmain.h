#ifndef	_ldmain_h
#define	_ldmain_h
#define	ALSMID "old"

#ifdef	WIN32
#include <malloc.h>
#define	allocate( n ) malloc( n )
#define	liberate( p ) free( p )	
#else	/* WIN32 */
#include "allocate.h"
#endif

#define	private	
#define	public	

#define	BYTE	unsigned char
#define	VOID	void
#define	XPTR	*
#define	BPTR	BYTE XPTR
#define	VPTR	VOID XPTR
#define	LONG	unsigned long
#define	SHORT	unsigned short

#if defined(UNIX) || defined(WIN32)
#define	EXAWORD	LONG
#else
#define	EXAWORD	SHORT
#endif

#define	ItemList(a,b) struct { a * root; a * last; } b

#define	OFFSET_TIMESTAMP	0x00F0

#define	WORD		EXAWORD
#define	TCTEXTPTR	BPTR
#define	BLOCKSIZE	256
#define	MOINS_UN	Linker.minusone
#define	MOINS_UN16	0x0000FFFF
#define	MOINS_UN32	0xFFFFFFFFL
#define	OVERFLOW16	0xFFFF0000

#define	VRAI		1
#define	FAUX		0
#define	_DIEZEMEM 		2048
#define	_DIEZEFILES		  64
#define	_DIEZESTACK		 256
#define	_DIEZEHEAP		1024


#define	WORD16SIZE	sizeof( short int )
#define	WORD32SIZE	sizeof( long  int )

#define	ISLOCAL2	0x8000
#define	TOGLOBAL2	0x7FFF
#define	ISLOCAL3	0x00000001L
#define	TOGLOBAL3	0xFFFFFFFEL
#define	TCPREFIX	18

#define	ATR_HOLE	0x0010		/* Atr table hole bug bit	*/
#define	PRIMARY 	0x0040		/* Variable has first dim	*/
#define	SECONDARY	0x0020		/* Variable has second dim	*/
#define	STRINGTYPE 	0x0003		/* String type separator 	*/
#define	INTEGERTYPE 	0x0001		/* Integer type limit    	*/
#define	SPECIALE	0xFF00		/* Long offset variable types	*/
#define	ISDYNAMIC	0x8000		/* Indicates a special type	*/
#define	ISETENDU 	0x4000		/* Extended memory type    	*/
#define	ISVIRTUAL	0x3F00		/* Virtual memory upper limit	*/
#define	ISMEMBER  	0x2000		/* Indicates a member type	*/
#define	ISPOINTER	0x1000		/* Indicates a pointer type	*/
#define	ISNORMAL	0x0000		/* Its not special		*/
#define	ISARG		0x0008		/* marks an ARG			*/
#define	BASIC		0x0007		/* Integer / string bits	*/
	
#define	STRICT_TYPE	(ISPOINTER | PRIMARY | SECONDARY | ISETENDU | BASIC)

#define	TCODE_DEBUG	0x0001
#define	TCODE_MODULE 	0x0003
#define	TCODE_OPTIMISED	0x0040

#define	LK_MAX_PATH	512

#define	MODULE		struct module
#define	SEGMENT		struct segment
#define	XSEGMENT	struct xsegment
#define	PROCEDURE	struct procedure
#define	XPROCEDURE	struct xprocedure
#define	VARIABLE	struct variable
#define	LOCATION	struct location
#define	REFERENCE	struct reference 
#define	FIELD		struct field
#define	RELOCATION	struct relocation 
#define	COMPONENT	struct component
#define	LIBRARY		struct library
#define	NAMES		struct modulenames
#define	SOLUTION	struct solution
#define	DATALABEL	struct datalabel

DATALABEL	{
	DATALABEL *	next;
	DATALABEL *	prev;
	WORD		value;
	WORD		extra;
	BPTR		name;
	WORD		type;
	};

SOLUTION	{
	SOLUTION	*	prev;
	SOLUTION	*	next;
	VARIABLE	*	variable;
	MODULE		*	module;
	};

NAMES		{
	NAMES *	next;
	NAMES *	prev;
	BPTR	name;
	EXAWORD	number;
	EXAWORD	alternative;
	};


COMPONENT	{
	COMPONENT * next;
	COMPONENT * prev;
	MODULE	  * module;
	};

LIBRARY		{
	LIBRARY	*	next;
	LIBRARY	*	prev;
	BPTR		name;
	EXAWORD		number;
	EXAWORD		options;
	EXAWORD		functions;
	MODULE	*	host;
	};

LOCATION	{
	EXAWORD	sector;
	EXAWORD	length;
	};

RELOCATION	{
	RELOCATION * 	next;
	LOCATION	detail;
	};	

#include "ldxref.h"
#include "ldvarb.h"
#include "ldproc.h"
#include "ldsegm.h"

MODULE	{

	MODULE 	* 	next;
	MODULE 	* 	prev;

	BPTR		rootname;
	EXAWORD		unique;
	BYTE		progname[16];

	int		whatisit;

//	List of added component modules
//	-------------------------------
	ItemList( COMPONENT, components );


/*	The program or Module	*/
/*	---------------------	*/
	BPTR		 atname;
	FILE	       * athandle;
	BYTE	 	 atheader[BLOCKSIZE];

/*	The symbolic infos	*/
/*	------------------	*/
	BPTR		 dbname;
	FILE	       * dbhandle;
	BYTE	 	 dbheader[BLOCKSIZE];

//	IO Byte/Word buffering mechanisms
//	---------------------------------
	FILE	       *bhandle;
	BYTE	 	buffer[BLOCKSIZE];
	EXAWORD		bindex;
	EXAWORD		bsector;
	LOCATION *	boutput;
	REFERENCE *	overspill;

	BYTE		spillzone[16];


//	Tcode interpretation control
//	----------------------------
	EXAWORD		minusone;
	EXAWORD		wordsize;
	EXAWORD		toglobal;
	EXAWORD		islocal;

	EXAWORD		argvariable;
	EXAWORD		diezeheap;
	EXAWORD		diezefiles;
	EXAWORD		diezemem; 
	EXAWORD		diezestack;
	EXAWORD		maxlds;
	EXAWORD		maxldt;
	EXAWORD		CryptCode;
	EXAWORD		options;

	LOCATION	extended;

	LOCATION	modnames;

	ItemList( NAMES, names );

	LOCATION	filenames;

	ItemList( NAMES, filename );
	ItemList( DATALABEL, labels );
	ItemList( REFERENCE, relocations );

#ifdef ABAL33A
// Implicit Library modules management
// -----------------------------------
	LOCATION	libmodtable;
	BPTR		lmt;
#endif

//	The Five Tcode Entites 
//	----------------------
//	Dynamic Library Control
//	-----------------------
	EXAWORD		LdtAdjust;
	LOCATION	librtable;
	EXAWORD		nblibr;
	ItemList( LIBRARY, libraries );

//	Constants Manager
//	-----------------
	EXAWORD		GctAdjust;
	EXAWORD		GctLimit;
	LOCATION	constable;
	LOCATION	consnames;
	EXAWORD		nbcons;
	BPTR		gct;

//	Variable  Management
//	--------------------
	EXAWORD		GlobalVariables;	/* Cumul of global items  */
	EXAWORD		GlobalAllocation;	/* cumul of global memory */
	LOCATION	varbtable;
	LOCATION	varbnames;
	EXAWORD		nbvarb;
	EXAWORD		GdsAdjust;		/* adjustment rellocate */
	EXAWORD		GdsLimit;		/* size of this module	*/
	EXAWORD		GdtAdjust;
	EXAWORD		GdtLimit;
	LONG   		EtdAdjust;
	LONG   		EtdLimit;
	BPTR		gvt;

	ItemList( VARIABLE, variables );

//	External variables (what for)
	LOCATION	xvarbtable;
	LOCATION	xvarbnames;
	EXAWORD		xnbvarb;
	BPTR		xvt;

	ItemList( VARIABLE, xvariables );

//	External fields
//	---------------
	LOCATION	fieldtable;
	EXAWORD		nbfields;

	ItemList( FIELD, fields );

//	Procedure Management
//	--------------------
	EXAWORD		PdtLimit;
	EXAWORD		PdtAdjust;
	LOCATION	proctable;
	LOCATION	procnames;
	EXAWORD		nbproc;
	BPTR		gpt;

	ItemList( PROCEDURE, procedures );
	ItemList(XPROCEDURE,xprocedures );

//	Segment Manager
//	---------------
	EXAWORD		NewSegment;
	EXAWORD		SdtLimit;
	EXAWORD		SdtAdjust;
	LOCATION	segmtable;
	LOCATION	segmnames;
	EXAWORD		nbsegm;
	BPTR		gst;

	ItemList( SEGMENT, segments );
	ItemList(XSEGMENT,xsegments );

	};

#define	MAX_INCLUDE 10

struct	linker	{
	EXAWORD		includes;
	BPTR		inclusion[MAX_INCLUDE];
	EXAWORD		check;
	EXAWORD		exitcode;
	EXAWORD		overspill;
	EXAWORD		reporting;
	FILE	*	rhandle;
	BYTE		OptionR[256];
	BYTE		OptionD[256];
	EXAWORD		minusone;
	EXAWORD		wordsize;
	EXAWORD		sector;
	EXAWORD		debugsector;
	EXAWORD		counter;
	EXAWORD		targettrace;
	EXAWORD		sourcetrace;
	EXAWORD		version;
	int		forcecase;
	int		verbose;
	int		warning;
	int		debug;
	int		noautolink;
	BPTR		targetname;
	MODULE * 	program;
	MODULE	*	atsource;
	MODULE	*	dbsource;
	ItemList( MODULE, modules );
	};

#endif	/* _ldmain_h */

