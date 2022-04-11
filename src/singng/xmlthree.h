#ifndef	_singx3d_h
#define	_singx3d_h

/*	------------------------------------------------------------------------------------------	*/
/* 	Document Type Definition or Declaration for Visual Forms Ressource Files generated by Sing 	*/
/*	------------------------------------------------------------------------------------------	*/

static	char *	XmlElement[] = {
	(char *) "THREED",
	(char *) "NAME",
	(char *) "VECTOR",
	(char *) "SIZES",
	(char *) "POSITION",
	(char *) "FOCUS",
	(char *) "DIRECTION",
	(char *) "MOVEMENT",
	(char *) "TIMER",
	(char *) "STEP",
	(char *) "OPTION",
	(char *) "IMAGE",
	(char *) "FILL",
	(char *) "DOCUMENT",
	(char *) "DOCNAME",
	(char *) "OBJECT",
	(char *) "TOPLEFT",
	(char *) "BOTTOMRIGHT",
	(char *) "NATURE",
	(char *) "XLR",
	(char *) "YUL",
	(char *) "ZFB",
	(char *) "SHL",
	(char *) "FORM",
	(char *) "TEXT",
	(char *) "PRODUCTION",
	(char *) "TARGET",
	(char *) "LANGUAGE",
	(char *) "PROJECT",
	(char *) "MAKEFILE",
	(char *) "CONFIGURE",
	(char *) "HTML-PRODUCTION",
	(char *) "TITLE",
	(char *) "SCREEN",
	(char *) "PRINT",
	(char *) "LOGO",
	(char *) "FOOTER",
	(char *) "PATH",
	(char *) "SEPARATE",
	(char *) "REPORT",
	(char *) "NATIONAL",

	/* Tags for database file description management */
	/* --------------------------------------------- */
	(char *) "SQFILE",
	(char *) "SIFILE",
	(char *) "MCFILE",
	(char *) "DBFILE",
	(char *) "VMEMORY",
	(char *) "INDEX",
	(char *) "RECORD",
	(char *) "CLASS",
	(char *) "SIZE",
	(char *) "MEMBER",
	(char *) "FILE",
	(char *) "TYPE",
	(char *) "DATABASE",
	(char *) "BASE",
	(char *) "ICONS",
	(char *) "PALETTE",

	/* Tags for the TreeViewer Configuration */
	/* ------------------------------------- */
	(char *) "TREEVIEW",
	(char *) "FONT",
	(char *) "ZONE",
	(char *) "SPACING",
	(char *) "SOURCE",
	(char *) "PACKAGE",
	(char *) "IDENTITY",
	(char *) "WIDTH",

	/* Template management */
	/* ------------------- */
	(char *) "TEMPLATE",
	(char *) "ROLLBACK",

	(char *) "METRIC",
	(char *) "SCALE",
	(char *) "NATLANG",
	(char *) "MODULES",
	(char *) "USESTYLE",
	(char *) "FORMSFOLDER",
	(char *) "SOURCESFOLDER",
	(char *) "BINARIESFOLDER",

	(char *) "GRIDLOCK",
	(char *) "WIDGET",
	(char *) "VALUE",
	(char *) "FIELD",
	(char *) "FILLER",
	(char *) "TESTDATA",
	(char *) "COMPONENTS",
	(char *) "LOCKON",
	(char *) "GRIDTYPE",
	(char *) "EXTENSION",
	(char *) "VIEW",

	/* ---------------------------------------------------------- */
	/* extension for abal 3.2a.0.11 : database help text handling */
	/* ---------------------------------------------------------- */

	(char *) "COMMENT",
	(char *) "TOOL",     
	(char *) "ENGLISH",    
	(char *) "FRANCAIS",   
	(char *) "ITALIANO",   
	(char *) "ESPAGNOL",   
	(char *) "DEUTSCH",    
	(char *) "NEDERLANDS", 
	(char *) "PORTUGUES",  
	(char *) "OTHER",
	(char *) "X",
	(char *) "Y",
	(char *) "LINK",

	/* new atributs for the DATABASE tag */
	/* --------------------------------- */
	(char *) "HYPERVIEW",
	(char *) "FRACTION",
	(char *) "HELP",
	(char *) "TRACING",
	(char *) "COLOUR",
	(char *) "PADDING",
	(char *) "MARGIN",
	(char *) "DEPTH",
	(char *) "XPADDING",
	(char *) "YPADDING",
	(char *) "JOINTURE",
	(char *) "STYLE"
	};


#define	ELEMENT_THREED		0x0000
#define	ELEMENT_NAME		0x0001
#define	ELEMENT_VECTOR		0x0002
#define	ELEMENT_SIZES		0x0003
#define	ELEMENT_POSITION	0x0004
#define	ELEMENT_FOCUS		0x0005
#define	ELEMENT_DIRECTION	0x0006
#define	ELEMENT_MOVEMENT	0x0007
#define	ELEMENT_TIMER		0x0008
#define	ELEMENT_STEP		0x0009
#define	ELEMENT_OPTION		0x000A
#define	ELEMENT_IMAGE		0x000B
#define	ELEMENT_FILL		0x000C
#define	ELEMENT_DOCUMENT	0x000D
#define	ELEMENT_DOCNAME 	0x000E
#define	ELEMENT_OBJECT		0x000F
#define	ELEMENT_TOPLEFT		0x0010
#define	ELEMENT_BOTTOMRIGHT	0x0011
#define	ELEMENT_NATURE		0x0012
#define	ELEMENT_XLR		0x0013
#define	ELEMENT_YUL		0x0014
#define	ELEMENT_ZFB		0x0015
#define	ELEMENT_SHL		0x0016
#define	ELEMENT_FORM		0x0017
#define	ELEMENT_TEXT		0x0018
#define ELEMENT_PRODUCTION	0x0019
#define ELEMENT_TARGET		0x001A
#define ELEMENT_LANGUAGE	0x001B
#define ELEMENT_PROJECT		0x001C
#define ELEMENT_MAKEFILE	0x001D
#define ELEMENT_CONFIGURE	0x001E
#define ELEMENT_HTML_PRODUCTION	0x001F
#define ELEMENT_TITLE		0x0020
#define ELEMENT_SCREEN		0x0021
#define ELEMENT_PRINT		0x0022
#define ELEMENT_LOGO		0x0023
#define ELEMENT_FOOTER		0x0024
#define ELEMENT_PATH		0x0025
#define ELEMENT_SEPARATE	0x0026
#define ELEMENT_REPORT		0x0027
#define ELEMENT_NATIONAL	0x0028
#define	DATABASE_SQFILE		0x0029		
#define	DATABASE_SIFILE		0x002A		
#define	DATABASE_MCFILE		0X002B
#define	DATABASE_DBFILE		0x002C		
#define	DATABASE_VMEMORY	0x002D
#define	DATABASE_INDEX		0x002E
#define	DATABASE_RECORD		0x002F
#define	DATABASE_CLASS		0x0030
#define	DATABASE_SIZE		0x0031
#define	DATABASE_MEMBER		0x0032
#define	DATABASE_FILE		0x0033
#define	DATABASE_TYPE		0x0034
#define ELEMENT_DATABASE	0x0035
#define ELEMENT_BASE		0x0036
#define ELEMENT_ICONS		0x0037
#define ELEMENT_PALETTE 	0x0038
#define ELEMENT_TREEVIEW	0x0039
#define ELEMENT_FONT		0x003A
#define ELEMENT_ZONE		0x003B
#define ELEMENT_SPACING		0x003C
#define ELEMENT_SOURCE		0x003D
#define ELEMENT_PACKAGE		0x003E
#define ELEMENT_IDENTITY	0x003F
#define ELEMENT_WIDTH   	0x0040
#define ELEMENT_TEMPLATE	0x0041
#define ELEMENT_ROLLBACK	0x0042
#define ELEMENT_METRIC  	0x0043
#define ELEMENT_SCALE   	0x0044
#define ELEMENT_NATLANG 	0x0045
#define ELEMENT_MODULES 	0x0046
#define ELEMENT_USESTYLE   	0x0047
#define ELEMENT_FORMULAIRES	0x0048
#define ELEMENT_REPSOURCES	0x0049
#define ELEMENT_BINAIRES	0x004A
#define ELEMENT_GRIDLOCK	0x004B
#define ELEMENT_WIDGET		0x004C
#define ELEMENT_VALUE   	0x004D
#define DATABASE_FIELD   	0x004E
#define DATABASE_FILLER   	0x004F
#define ELEMENT_TESTDATA	0x0050
#define ELEMENT_COMPONENTS	0x0051
#define ELEMENT_LOCKON		0x0052
#define ELEMENT_GRIDTYPE	0x0053
#define ELEMENT_EXTENSION	0x0054
#define ELEMENT_VIEW		0x0055

/* ---------------------------------------------------------- */
/* extension for abal 3.2a.0.11 : database help text handling */
/* ---------------------------------------------------------- */

#define	DATABASE_COMMENT	0x0056
#define	DATABASE_TOOL		0x0057       
#define	DATABASE_ENGLISH	0x0058    
#define	DATABASE_FRANCAIS	0x0059   
#define	DATABASE_ITALIANO	0x005A   
#define	DATABASE_ESPAGNOL	0x005B   
#define	DATABASE_DEUTSCH	0x005C    
#define	DATABASE_NEDERLANDS	0x005D 
#define	DATABASE_PORTUGUES	0x005E  
#define	DATABASE_OTHER		0x005F      
#define	DATABASE_X		0x0060    
#define	DATABASE_Y		0x0061    
#define	DATABASE_LINK		0x0062

/* extension for new database model editor */
/* --------------------------------------- */
#define	DATABASE_HYPERVIEW	0x0063
#define	DATABASE_FRACTION	0x0064
#define	DATABASE_HELP		0x0065
#define	DATABASE_TRACING	0x0066
#define	DATABASE_COLOUR		0x0067
#define	DATABASE_PADDING	0x0068
#define	DATABASE_MARGIN		0x0069
#define	DATABASE_DEPTH		0x006A
#define	DATABASE_XPADDING	0x006B
#define	DATABASE_YPADDING	0x006C
#define	DATABASE_JOINTURE	0x006D

#define	ELEMENT_STYLE		0x006E	

#define MAX_ELEMENT      	0x006F

#endif	/* _singx3d_h */
	/* ---------- */

