#ifndef	_ldxref_h
#define	_ldxref_h

#define	NUL_REFERENCE 0x00FF
#define	INT_REFERENCE	0
#define	EXT_REFERENCE	1
#define	REF_PROCEDURE	0
#define	REF_SEGMENT	2	
#define	REF_CONSTANT	4
#define	REF_VARIABLE	8
#define	REF_LIBRARY 	16
#define	GDS_OFFSET	32
#define	XDS_OFFSET	64


#define	INT_PROCEDURE	(REF_PROCEDURE	| INT_REFERENCE)
#define	EXT_PROCEDURE	(REF_PROCEDURE	| EXT_REFERENCE)
#define	INT_SEGMENT	(REF_SEGMENT	| INT_REFERENCE)
#define	EXT_SEGMENT	(REF_SEGMENT	| EXT_REFERENCE)
#define	INT_CONSTANT	(REF_CONSTANT	| INT_REFERENCE)
#define	EXT_CONSTANT	(REF_CONSTANT	| EXT_REFERENCE)
#define	INT_VARIABLE	(REF_VARIABLE	| INT_REFERENCE)
#define	EXT_VARIABLE	(REF_VARIABLE	| EXT_REFERENCE)
#define	INT_LIBRARY	(REF_LIBRARY	| INT_REFERENCE)
#define	EXT_LIBRARY	(REF_LIBRARY	| EXT_REFERENCE)

REFERENCE	{
	REFERENCE *	next;
	REFERENCE *	prev;
	BYTE		type;
	EXAWORD		value;
	EXAWORD		number;
	LOCATION	patch;
	};

#endif	/* _ldxref_h */

