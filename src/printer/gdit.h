#ifndef	_GDIT_H
#define	_GDIT_H

#include	<stdio.h>
#include	<stdlib.h>
#include	"bda.h"
#include	"catcher.h"
#include	"stdgigo.h"

#define	__DRIVER_NAME	"GDIT"
#define	__DRIVER_DATE	"26/08/2008"
#define __DRIVER_MAJOR 	'1'
#define __DRIVER_MINOR 	'1'
#define __DRIVER_INDEX 	'a'

#define	PRIVATE	static
#define	PUBLIC	
#define	EXTERN	extern

BDA	desrt;				/* library description */
EXTERN BDA_INIT FAR PTR rt_init;	/* library signing */
EXTERN BDA_ARG rt_arg;			/* arguments description */

#ifdef LINT_ARGS

	VOID FAR PTR InitRelais(VOID);
PRIVATE	VOID FAR banniere(VOID);
EXTERN	VOID rt_error(EXAWORD);

#else

	VOID FAR PTR InitRelais();
PRIVATE	VOID FAR banniere();
EXTERN	VOID rt_error();

#endif /* LINT_ARGS */


	/* ----------- */
#endif	/* _GDIT_H */
	/* ----------- */







