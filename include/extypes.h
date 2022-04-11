/*----------------------------------------------------------------------*/
/*									*/
/*		E X E C U T E U R 	A . B . A . L			*/
/*		-------------------------------------			*/
/*									*/
/*		Copyright (c) 1987,1992 Amenesik Sologic S.A.			*/
/*									*/
/*		Module  :	EXTYPES - H				*/
/*		Version :	1.3c / 1.4a / 2.1a			*/
/*		Date 	:	04/08/1992				*/
/*									*/
/*----------------------------------------------------------------------*/

/*
 *	Executeur Type definitions
 *	--------------------------
 */

#ifndef	_EXTYPES_H
#define	_EXTYPES_H

#ifdef ABALBIG
#define	FAR far
#else
#define	FAR
#endif

#define SWORD	short
#define	VOID	void
#define CHAR	char
#define	PTR	FAR *
#define WORD	unsigned short int 
#define LONG	long
#define BYTE	unsigned char 

typedef unsigned char EXABYTE;		/* Declare TYPE 8 bit Byte	*/

#ifdef	ABAL64
typedef unsigned long long LSEEKTYPE;	/* Value for low level lseek 	*/
typedef unsigned long long EXAWORD;	/* Declare TYPE 64 bit unsigned	*/
typedef long  long int	BINAIRE;	/* Declare TYPE 64 bit signed	*/
#else
#ifdef	VERSION666
typedef unsigned long long LSEEKTYPE;	/* Value for low level lseek 	*/
typedef unsigned long  int EXAWORD;	/* Declare TYPE 32 bit unsigend */
typedef long BINAIRE;			/* Declare TYPE 32 bit signed	*/
#else
typedef unsigned long LSEEKTYPE;	/* Value for low level lseek	*/
typedef unsigned short int EXAWORD;	/* Declare TYPE 16 bit unsigned	*/
typedef short int	BINAIRE;	/* Declare TYPE 16 bit signed	*/
#endif
#endif

typedef	unsigned long ULONG;		/* unsigned long integer	*/

#define	EXALONG long

/* Types pour acces au systeme */

#ifdef DOS
#ifdef WIN32
typedef unsigned int	USINT;
#else
typedef unsigned short int USINT;
#endif
#endif

#ifdef PROLOGUE
typedef unsigned short int USINT;
#endif

#ifdef UNIX
typedef unsigned int	USINT;		/* unsigned int for Sys Calls	*/
#endif

#ifdef VMS
typedef unsigned int	USINT;		/* unsigned int for Sys Calls	*/
#endif


/* Types des pointeurs */

#ifdef ABALBIG
typedef	unsigned char FAR PTR BPTR;	/* Executeur Standard Pointers	*/
typedef	char FAR PTR CPTR;		/* Executeur BAL# Pointers	*/
typedef	long FAR PTR LPTR;		/* Executeur Long Pointers	*/
typedef	int FAR PTR I32PTR;		/* Executeur Long Pointers	*/
#ifndef VERSION666
typedef	unsigned short int FAR PTR WPTR; /* Executeur Word Pointeurs	*/
typedef	unsigned short int FAR PTR WPTR16; /* Executeur Word Pointeurs	*/
#else
typedef	unsigned long  int FAR PTR WPTR; /* Executeur Word Pointeurs	*/
typedef	unsigned short int FAR PTR WPTR16; /* Executeur Word Pointeurs	*/
#endif
typedef void FAR PTR VPTR;		/* Void General purpose pointer	*/
#else
typedef	unsigned char PTR BPTR;		/* Declare byte Pointers	*/
typedef	char PTR CPTR;			/* Signed character  Pointers	*/
typedef	long PTR LPTR;			/* Signed long int   Pointers	*/
typedef	int PTR I32PTR;			/* Signed int   Pointers	*/

#ifndef VERSION666
typedef	unsigned short int PTR WPTR; 	/* Unsigned short int Pointers	*/
typedef	unsigned short int PTR WPTR16; 	/* Unsigned short int Pointers	*/
#else
typedef	unsigned long int PTR WPTR; 	/* Unsigned short int Pointers	*/
typedef	unsigned short int PTR WPTR16; 	/* Unsigned short int Pointers	*/
#endif

#define VPTR void PTR			/* General purpose pointer cast */
#endif

#define LONGSIZE sizeof(EXALONG)	/* 4 octets ?			*/
#ifdef	VERSION666
#define	WORDSIZE ExaWordSize		/* Variable providing size	*/
#else
#define WORDSIZE sizeof(EXAWORD)	/* Machine word	 2 octets	*/
#endif
#define CHARSIZE sizeof(EXABYTE)	/* Caractere 1 octet		*/

#include "exhandle.h"

#endif	/* _EXTYPES_H */
	/* ---------- */


