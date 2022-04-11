/*
 *	Bibliotheques Statiques ABAL
 *	----------------------------
 *
 *	Copyright (c) 2021 Amenesik / Sologic
 *
 *	bsa.c - code de demarrage d'une bibliotheque statique
 *
 *	Mises a jour :
 *	------------
 *
 */


#include <stdio.h>
#include <setjmp.h>
#include <sys/types.h>
#include "asfcico.h"
#include "sfundef.h"

/* #include "bda.h" */

#if MSDOS || PRL
#include <conio.h>
#else
#include <stdio.h>
#endif

#include "bda.h"



/*
 *	contexte de retour brutal a l'executeur
 *	---------------------------------------
 */

static jmp_buf Jump;


/*
 *	declaration des structures de la BDA
 *	------------------------------------
 */

static BDA_INIT PTR rt_init = GN(BDA_INIT);
static BDA PTR descripteur;
extern WORD err_val;
#ifdef	KEEP_THIS_SHIT
void FAR *InitRelais(char);
#else
void FAR * InitRelais();
#endif
static VOID rt_error(WORD);

/*
 *	definitions
 *	-----------
 */

#define FC_INIT		0
#define FC_KILL		1
#define FC_CALL		2
#define	BDA_SCRATCH	138
#define BDA_UNKNOWN	139

#ifdef	KEEP_THIS_SHIT

/*
 *	Point d'entree general dans une bibliotheque statique
 *	-----------------------------------------------------
 *
 *	E/ WORD bsa_xx(fonction,desinit,nofct,a);
 *
 *		WORD fonction	 = fonction a effectuer
 *		BDA_INIT *desinit= descripteur signature
 *		WORD nofct	 = numero de fonction de la BSA
 *		VOID *a[]        = arguments
 *
 * retour:	code de retour de la fonction,
 *		err_val != 0 en cas d'erreur.
 */
WORD bsa_mf(fonction,desinit,nofct,a)
WORD fonction;
BDA_INIT PTR desinit;
WORD nofct;
VOID PTR a[];
{ WORD status;

  /* Prepare une condition de sortie exceptionnelle */
  status = setjmp(Jump);
  if (status != 0) {
	err_val = status;
	return(BDA_ERREUR);
  }

  switch(fonction) {
	/* fonction d'initialisation */
	case FC_INIT : rt_init = desinit;
		       if (rt_init != GN(BDA_INIT))
				rt_init->ordinaire.signature = BDA_ORDINAIRE;
		       descripteur = (BDA PTR) InitRelais(0);
		       return(BDA_OK);

	/* fonction de fin */
	case FC_KILL : if (descripteur->fin != GN(VOID))
				(*descripteur->fin)();
		       return(BDA_OK);

	/* appel d'une fonction de la bibliotheque */
	case FC_CALL : if ((nofct >= descripteur->nombre) ||
			   (descripteur->fonction[nofct] == GN(VOID)))
				rt_error(BDA_UNKNOWN);
		       return((*descripteur->fonction[nofct])(a));

	/* fonction inconnue */
	default : rt_error(BDA_SCRATCH);
  }
}


#endif /* KEEP_THIS_SHIT */

/* rt_error - retour brutal a l'executeur
 * --------------------------------------
 *
 * appel:	VOID rt_error(code);
 *		WORD code = code erreur a retourner a l'executeur
 *
 * retour:	rien
 *
 */
static VOID rt_error(code)
WORD code;
{
	longjmp(Jump, code);
}

/* -------------- Fin de Fichier : bsamf.c ---------------------------- */

/*
 	Bibliotheque Dynamique Multi-Fenetrage
 *	--------------------------------------
 *
 *	Copyright (c) 2021 Amenesik / Sologic
 *
 *	asfun.c - fonctions entete
 *
 *	$Header: /home/Produits/Abal3/asfun/oldbdamf.c,v 1.2 2006/05/18 16:00:12 jamie Exp $
 *
 ***************************************************
 *	MAJ llb		18.12.90
 *	    cr		18.03.91
 *	    llb		14.08.91	Boites dialogue
 *	    llb		11.10.91	wexit,wrefresh	
 *	    llb		16.10.91	menu_way
 *	    llb		21.01.92	swcreat
 *	    llb		03.02.92	wcadre,wtitre
 *	    llb		18.02.92	wmove
 *	    eds		23.04.92	set_tch_esc
 *	    llb		13.05.92	winfo...( Version 1.3)
 *	    eds		01.06.92	fct mouse( Version 1.4)
 * 	    llb	        01.04.93	get/set_style
 *	    llb	        07.04.93	wlangue
 *	    llb	        06.05.93	asfvers
 *	    llb	        27.09.93	read_event()
 *	    llb	        26.11.93	Relay_lo = Run_Mi (Mouse ABAL 2.1) 
 * Version  1.4d
 *	    llb		17.12.93	adapt TOP WINDOW	
 * Version  1.4e
 *	    llb 	21.02.94	Corr clear ecran sur initrelay
 *			27.09.94	fnc SCROLL_BAR accessibles en dosext
 */

#define	VERS	'1'
#define	INDICE	'4'
#define	RELEASE	'e'



#define		NB_WFONC	89 	/* nbre fonctions ASFUN */

/* Variable necessaire a CICO : 03/12/91 */

unsigned short int sicico=0;


/* *	Definitions evolutives
 *	----------------------
 */

#ifndef MENTION_PROVISOIRE
#if M_I8086 || M_I186 || M_I286 || __TURBOC__
#define MENTION_PROVISOIRE	"\n\r   PROVISOIRE ("__DATE__" "__TIME__")"
#else
#define MENTION_PROVISOIRE	"\n\r   PROVISOIRE"
#endif /* M_I8086 || M_I186 || M_I286 || __TURBOC__ */
#endif /* MENTION_PROVISOIRE */

#define VERSION 		"  Version 1.4e"
#define COPYRIGHT		"   Copyright (c) 2021 Amenesik / Sologic "

#ifdef PRL
#define prologue		/* adaptation "bda.h" 1.4a */
#endif


/*
 *	Inclusions
 *	----------
 */

#include "sfundef.h"
#include "bda.h"

#if MSDOS || PRL || dosext
#include <conio.h>
#else
#include <stdio.h>
#endif


/*
 *	Fonctions externes
 *	------------------
 */

#if PRL

extern void far Run_Co(WORD);
extern WORD far Run_Info(WORD);
extern void far Run_Mes(BYTE far *,WORD);
extern void far Run_Str(BYTE far *);
extern WORD Run_Mi(BYTE far *);

#else

extern void initerm();		/* initialisation CICO		*/
extern void finterm();		/* terminaison CICO		*/
extern WORD x_getch();
extern WORD x_kbhit();
extern void x_clear();
extern WORD prfo();
extern WORD lcfo();
extern WORD Run_Conf();
extern WORD Run_Co();
extern WORD Run_Lin();
extern WORD Run_Col();
extern void Run_Mes();
extern void _co_start();
extern void _co_flush();
extern void _lo();

#endif /* PRL */


/*
 *	Variables externes
 *	------------------
 */

#if !PRL

extern WORD prndest;

#endif /* !PRL */


/*
 *	Variables utilisees
 *	-------------------
 */

static BDA Descripteur;		/* descripteur de bibliotheque	*/
extern BDA_INIT FAR * rt_init;


extern GLOBAL BDA_ARG rt_arg;




#if PRL

int _mem_res = 1;		/* chargement resident		*/
int _stkmin = 4096;		/* minimum reserve a la pile	*/
int _stklen = 40280;		/* taille pile + tas		*/

#endif /* PRL */


#if MSDOS || PRL || dosext	/* table des messages		*/
static char *message[] = {
	"*STX*2ASFUN   13a00",
#else
static char message[][42] = {
	"*STX*3ASFUN   11b00\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",
#endif /* MSDOS || PRL */
	"   Gestionnaire multifenetres et Services",
	"                                         ",
	"*ETX*",
};

static char *provisoire = MENTION_PROVISOIRE;
static char *version = VERSION;
static char *copyright = COPYRIGHT;


/*
 *	Declarations des fonctions
 *	--------------------------
 */

static void FAR Banniere();
static void FAR FinRelais();
static void AfficheChaine();
static void NouvelleLigne();
static char *ChercheMessage();
WORD   asfvers();

/*
 *	Fonctions entetes pour Unix ou VMS
 *	----------------------------------
 */

#if unix || M_XENIX || vms || dosext

WORD PASCAL FAR _winit(a)
void *a[];
{
#ifdef CPYFO
	WORD ret;
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
	ret =  winit(*((BYTE *)a[0] + o), *((BYTE *)a[1] + o), *((BYTE *)a[2] + o), *((BYTE *)a[3] + o), *((BYTE *)a[4] + o));
        cpyfo();			/* recopie des touches fonctions dans asfun.c */
	return(ret);
#else
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
	return (winit(*((BYTE *)a[0] + o), *((BYTE *)a[1] + o), *((BYTE *)a[2] + o), *((BYTE *)a[3] + o), *((BYTE *)a[4] + o)));
#endif /*CPYFO */
}

WORD PASCAL FAR _wcreat(a)
void *a[];
{
	return (wcreat((WORD *)a[0], (BYTE *)a[1]));
}

WORD PASCAL FAR _wsup(a)
void *a[];
{
#ifdef AIX_VERS
#ifdef SPARC
WORD w;
	memcpy(&w,a[0],sizeof(WORD));
	return (wsup(w));
#else
	return (wsup(*(WORD *)a[0]));
#endif
#else  /* AIX_VERS */
	return (wsup(*(WORD *)a[0]));
#endif /* AIX_VERS */
}

WORD PASCAL FAR _wwrite(a)
void *a[];
{
#ifdef AIX_VERS
#ifdef SPARC
WORD w0,w1;
	memcpy(&w0,a[0],sizeof(WORD));
	memcpy(&w1,a[1],sizeof(WORD));
	return (wwrite(w0, w1, (BYTE *)a[2]));
#else
	return (wwrite(*(WORD *)a[0], *(WORD *)a[1], (BYTE *)a[2]));
#endif
#else  /* AIX_VERS */
	return (wwrite(*(WORD *)a[0], *(WORD *)a[1], (BYTE *)a[2]));
#endif /* AIX_VERS */
}

WORD PASCAL FAR _wselect(a)
void *a[];
{
#ifdef AIX_VERS
#ifdef SPARC
WORD w;
	memcpy(&w,a[0],sizeof(WORD));
	return (wselect(w));
#else
	return (wselect(*(WORD *)a[0]));
#endif
#else  /* AIX_VERS */
	return (wselect(*(WORD *)a[0]));
#endif /* AIX_VERS */
}

WORD PASCAL FAR _winv(a)
void *a[];
{
#ifdef AIX_VERS
#ifdef SPARC
WORD w;
	memcpy(&w,a[0],sizeof(WORD));
	return (winv(w));
#else
	return (winv(*(WORD *)a[0]));
#endif
#else  /* AIX_VERS */
	return (winv(*(WORD *)a[0]));
#endif /* AIX_VERS */
}

WORD PASCAL FAR _wvis(a)
void *a[];
{
#ifdef AIX_VERS
#ifdef SPARC
WORD w;
	memcpy(&w,a[0],sizeof(WORD));
	return (wvis(w));
#else
	return (wvis(*(WORD *)a[0]));
#endif
#else  /* AIX_VERS */
	return (wvis(*(WORD *)a[0]));
#endif /* AIX_VERS */
}

WORD PASCAL FAR _wfgen(a)
void *a[];
{
#ifdef AIX_VERS
#ifdef SPARC
WORD w0;
WORD w1;
	memcpy(&w0,a[0],sizeof(WORD));
	memcpy(&w1,a[1],sizeof(WORD));
	return (wfgen(w0,w1));
#else
	return (wfgen(*(WORD *)a[0], *(WORD *)a[1]));
#endif
#else  /* AIX_VERS */
	return (wfgen(*(WORD *)a[0], *(WORD *)a[1]));
#endif /* AIX_VERS */
}

WORD PASCAL FAR _watb(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
#ifdef AIX_VERS
#ifdef SPARC
	WORD w;
	memcpy(&w,a[0],sizeof(WORD));
	return (watb(w, *((BYTE *)a[1] + o), *((BYTE *)a[2] + o), *((BYTE *)a[3] + o)));
#else
	return (watb(*(WORD *)a[0], *((BYTE *)a[1] + o), *((BYTE *)a[2] + o), *((BYTE *)a[3] + o)));
#endif 
#else  /* AIX_VERS */
	return (watb(*(WORD *)a[0], *((BYTE *)a[1] + o), *((BYTE *)a[2] + o), *((BYTE *)a[3] + o)));
#endif /* AIX_VERS */
}

WORD PASCAL FAR _wposcur(a)
void *a[];
{
#ifdef AIX_VERS
#ifdef SPARC
WORD w0,w1,w2;
	memcpy(&w0,a[0],sizeof(WORD));
	memcpy(&w1,a[1],sizeof(WORD));
	memcpy(&w2,a[2],sizeof(WORD));
	return (wposcur(w0, w1, w2));
#else
	return (wposcur(*(WORD *)a[0], *(WORD *)a[1], *(WORD *)a[2]));
#endif
#else  /* AIX_VERS */
	return (wposcur(*(WORD *)a[0], *(WORD *)a[1], *(WORD *)a[2]));
#endif /* AIX_VERS */
}

WORD PASCAL FAR _wimpl(a)
void *a[];
{
#ifdef AIX_VERS
#ifdef SPARC
WORD w;
	memcpy(&w,a[0],sizeof(WORD));
	return (wimpl(w));
#else
	return (wimpl(*(WORD *)a[0]));
#endif
#else  /* AIX_VERS */
	return (wimpl(*(WORD *)a[0]));
#endif /* AIX_VERS */
}

WORD PASCAL FAR _wgel(a)
void *a[];
{
#ifdef AIX_VERS
#ifdef SPARC
WORD w;
	memcpy(&w,a[0],sizeof(WORD));
	return (wgel(w));
#else
	return (wgel(*(WORD *)a[0]));
#endif
#else  /* AIX_VERS */
	return (wgel(*(WORD *)a[0]));
#endif /* AIX_VERS */
}

WORD PASCAL FAR _wfend(a)
void *a[];
{
	return (wend());
}

WORD PASCAL FAR _wsetatb(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
#ifdef AIX_VERS
#ifdef SPARC
	WORD w0,w4,w5,w6,w7;

	memcpy(&w0,a[0],sizeof(WORD));
	memcpy(&w4,a[4],sizeof(WORD));
	memcpy(&w5,a[5],sizeof(WORD));
	memcpy(&w6,a[6],sizeof(WORD));
	memcpy(&w7,a[7],sizeof(WORD));
	return (wsetatb(w0, *((BYTE *)a[1] + o), *((BYTE *)a[2] + o), *((BYTE *)a[3] + o), w4,w5,w6,w7));
#else
	return (wsetatb(*(WORD *)a[0], *((BYTE *)a[1] + o), *((BYTE *)a[2] + o), *((BYTE *)a[3] + o), *(WORD *)a[4], *(WORD *)a[5], *(WORD *)a[6], *(WORD *)a[7]));
#endif
#else  /* AIX_VERS */
	return (wsetatb(*(WORD *)a[0], *((BYTE *)a[1] + o), *((BYTE *)a[2] + o), *((BYTE *)a[3] + o), *(WORD *)a[4], *(WORD *)a[5], *(WORD *)a[6], *(WORD *)a[7]));
#endif /* AIX_VERS */
}

WORD PASCAL FAR _wgetimpl(a)
void *a[];
{
#ifdef AIX_VERS
#ifdef SPARC
WORD w;
WORD ret;

	memcpy(&w,a[0],sizeof(WORD));
	ret = wgetimpl(&w);
	memcpy(a[0],&w,sizeof(WORD));
	return (ret);
#else
	return (wgetimpl((WORD *)a[0]));
#endif
#else  /* AIX_VERS */
	return (wgetimpl((WORD *)a[0]));
#endif /* AIX_VERS */
}

WORD PASCAL FAR _sf_init(a)
void *a[];
{
#ifdef CPYFO
	WORD ret;
	ret = sfinit((BYTE *)a[0]);
	cpyfo();
	return(ret);
#else
	return (sfinit((BYTE *)a[0]));
#endif /* CPYFO */
}

WORD PASCAL FAR _lsacti(a)
void *a[];
{
	return (lsacti());
}

WORD PASCAL FAR _lsmess(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
	return (lsmess(*((BYTE *)a[0] + o), *((BYTE *)a[1] + o), (BYTE *)a[2]));
}

WORD PASCAL FAR _lsclear(a)
void *a[];
{
	return (lsclear());
}

WORD PASCAL FAR _lsdesac(a)
void *a[];
{
	return (lsdesac());
}

WORD PASCAL FAR _menu_cre(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
	return (md_creat(*((BYTE *)a[0] + o), (BYTE *)a[1]));
}

WORD PASCAL FAR _menu_opt(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
	return (mo_creat(*((BYTE *)a[0] + o), *((BYTE *)a[1] + o), (BYTE *)a[2], (BYTE *)a[3], (BYTE *)a[4]));
}

WORD PASCAL FAR _menu_sep(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
	return (mo_sep(*((BYTE *)a[0] + o), *((BYTE *)a[1] + o)));
}

WORD PASCAL FAR _menu_mod(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
	return (mo_modif(*((BYTE *)a[0] + o), *((BYTE *)a[1] + o), *((BYTE *)a[2] + o), (BYTE *)a[3], (BYTE *)a[4], (BYTE *)a[5]));
}

WORD PASCAL FAR _menu_lat(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
	return (mo_latt(*((BYTE *)a[0] + o), *((BYTE *)a[1] + o), (BYTE *)a[2], (BYTE *)a[3], (BYTE *)a[4]));
}

WORD PASCAL FAR _menu_mat(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
	return (mo_matt(*((BYTE *)a[0] + o), *((BYTE *)a[1] + o), *((BYTE *)a[2] + o), *((BYTE *)a[3] + o), *((BYTE *)a[4] + o)));
}

WORD PASCAL FAR _menu_ann(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
	return (mo_clear(*((BYTE *)a[0] + o), *((BYTE *)a[1] + o)));
}

WORD PASCAL FAR _menu_aff(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
	return (md_aff(*((BYTE *)a[0] + o), *((BYTE *)a[1] + o), *((BYTE *)a[2] + o)));
}

WORD PASCAL FAR _menu_cho(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
	return (md_opt(*((BYTE *)a[0] + o), *((BYTE *)a[1] + o), (BYTE *)a[2], (BYTE *)a[3], (BYTE *)a[4], (BYTE *)a[5]));
}

WORD PASCAL FAR _menu_eff(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
	return (md_clear(*((BYTE *)a[0] + o)));
}

WORD PASCAL FAR _menu_sup(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
	return (md_sup(*((BYTE *)a[0] + o)));
}

WORD PASCAL FAR _list_cre(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
	return (ld_creat(*((BYTE *)a[0] + o), (BYTE *)a[1]));
}

WORD PASCAL FAR _lismul_c(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
	return (lm_creat(*((BYTE *)a[0] + o), (BYTE *)a[1]));
}

WORD PASCAL FAR _list_ele(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
#ifdef AIX_VERS
#ifdef SPARC
	WORD ret;
	WORD w1;
	memcpy(&w1,a[1],sizeof(WORD));
	ret = le_creat(*((BYTE *)a[0] + o), (WORD *)&w1, *((BYTE *)a[2] + o), *((BYTE *)a[3] + o), (BYTE *)a[4], (BYTE *)a[5]);
	memcpy(a[1],&w1,sizeof(WORD));
	return(ret);
#else
	return (le_creat(*((BYTE *)a[0] + o), (WORD *)a[1], *((BYTE *)a[2] + o), *((BYTE *)a[3] + o), (BYTE *)a[4], (BYTE *)a[5]));
#endif
#else /* AIX_VERS */
	return (le_creat(*((BYTE *)a[0] + o), (WORD *)a[1], *((BYTE *)a[2] + o), *((BYTE *)a[3] + o), (BYTE *)a[4], (BYTE *)a[5]));
#endif /* AIX_VERS */
}

WORD PASCAL FAR _list_lec(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
#ifdef AIX_VERS
#ifdef SPARC
	WORD w1;
	memcpy(&w1,a[1],sizeof(WORD));
	return (le_lect(*((BYTE *)a[0] + o), w1, (BYTE *)a[2], (BYTE *)a[3], (BYTE *)a[4]));
#else
	return (le_lect(*((BYTE *)a[0] + o), *(WORD *)a[1], (BYTE *)a[2], (BYTE *)a[3], (BYTE *)a[4]));
#endif
#else /* AIX_VERS */
	return (le_lect(*((BYTE *)a[0] + o), *(WORD *)a[1], (BYTE *)a[2], (BYTE *)a[3], (BYTE *)a[4]));
#endif /* AIX_VERS */
}

WORD PASCAL FAR _list_sui(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
#ifdef AIX_VERS
#ifdef SPARC
	WORD ret;
	WORD w1;
	memcpy(&w1,a[1],sizeof(WORD));
	ret = le_down(*((BYTE *)a[0] + o), (WORD *)&w1, (BYTE *)a[2], (BYTE *)a[3], (BYTE *)a[4]);
	memcpy(a[1],&w1,sizeof(WORD));
	return(ret);
#else
	return (le_down(*((BYTE *)a[0] + o), (WORD *)a[1], (BYTE *)a[2], (BYTE *)a[3], (BYTE *)a[4]));
#endif
#else  /* AIX_VERS */
	return (le_down(*((BYTE *)a[0] + o), (WORD *)a[1], (BYTE *)a[2], (BYTE *)a[3], (BYTE *)a[4]));
#endif /* AIX_VERS */
}

WORD PASCAL FAR _list_pre(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
#ifdef AIX_VERS
#ifdef SPARC
	WORD ret;
	WORD w1;
	memcpy(&w1,a[1],sizeof(WORD));
	ret = le_up(*((BYTE *)a[0] + o), (WORD *)&w1, (BYTE *)a[2], (BYTE *)a[3], (BYTE *)a[4]);
	memcpy(a[1],&w1,sizeof(WORD));
	return(ret);
#else
	return (le_up(*((BYTE *)a[0] + o), (WORD *)a[1], (BYTE *)a[2], (BYTE *)a[3], (BYTE *)a[4]));
#endif
#else  /* AIX_VERS */
	return (le_up(*((BYTE *)a[0] + o), (WORD *)a[1], (BYTE *)a[2], (BYTE *)a[3], (BYTE *)a[4]));
#endif /* AIX_VERS */
}

WORD PASCAL FAR _list_mat(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
#ifdef AIX_VERS
#ifdef SPARC
	WORD w1;
	memcpy(&w1,a[1],sizeof(WORD));
	return (le_matt(*((BYTE *)a[0] + o), w1, *((BYTE *)a[2] + o), *((BYTE *)a[3] + o), (BYTE*)a[4], (BYTE*)a[5]));
#else
	return (le_matt(*((BYTE *)a[0] + o), *(WORD *)a[1], *((BYTE *)a[2] + o), *((BYTE *)a[3] + o), (BYTE*)a[4], (BYTE*)a[5]));
#endif
#else  /* AIX_VERS */
	return (le_matt(*((BYTE *)a[0] + o), *(WORD *)a[1], *((BYTE *)a[2] + o), *((BYTE *)a[3] + o), (BYTE*)a[4], (BYTE*)a[5]));
#endif /* AIX_VERS */
}

WORD PASCAL FAR _list_fer(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
	return (ld_close(*((BYTE *)a[0] + o)));
}

WORD PASCAL FAR _list_aff(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
#ifdef AIX_VERS
#ifdef SPARC
	WORD w1;
	WORD w2;

	memcpy(&w1,a[1],sizeof(WORD));
	memcpy(&w2,a[2],sizeof(WORD));

	return (ld_aff(*((BYTE *)a[0] + o), w1,w2));
#else
	return (ld_aff(*((BYTE *)a[0] + o), *(WORD *)a[1], *(WORD *)a[2]));
#endif
#else  /* AIX_VERS */
	return (ld_aff(*((BYTE *)a[0] + o), *(WORD *)a[1], *(WORD *)a[2]));
#endif /* AIX_VERS */
}

WORD PASCAL FAR _list_cho(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
#ifdef AIX_VERS
#ifdef SPARC
	WORD w1, w2;
	WORD ret;
	memcpy(&w1,a[1],sizeof(WORD));
	memcpy(&w2,a[2],sizeof(WORD));
	ret = ld_choix(*((BYTE *)a[0] + o), &w1, &w2, (BYTE *)a[3], (BYTE *)a[4]);
	memcpy(a[1],&w1,sizeof(WORD));
	memcpy(a[2],&w2,sizeof(WORD));
	return (ret);
#else
	return (ld_choix(*((BYTE *)a[0] + o), (WORD *)a[1], (WORD *)a[2], (BYTE *)a[3], (BYTE *)a[4]));
#endif
#else  /* AIX_VERS */
	return (ld_choix(*((BYTE *)a[0] + o), (WORD *)a[1], (WORD *)a[2], (BYTE *)a[3], (BYTE *)a[4]));
#endif /* AIX_VERS */
}

WORD PASCAL FAR _list_eff(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
	return (ld_clear(*((BYTE *)a[0] + o)));
}

WORD PASCAL FAR _list_sup(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
	return (ld_sup(*((BYTE *)a[0] + o)));
}

WORD PASCAL FAR _bvalid_s(a)
void *a[];
{
	return (bv_spc((BYTE *)a[0], (BYTE *)a[1], (BYTE *)a[2]));
}

WORD PASCAL FAR _bvalid(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
	return (bv_std(*((BYTE *)a[0] + o), (BYTE *)a[1]));
}

WORD PASCAL FAR _sf_end(a)
void *a[];
{
	return (sfend());
}

WORD PASCAL FAR _wfmod(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
#ifdef AIX_VERS
#ifdef SPARC
	WORD w0;
	WORD w1;
	WORD w2;
	WORD ret;
	memcpy(&w0,a[0],sizeof(WORD));
	memcpy(&w1,a[1],sizeof(WORD));
	memcpy(&w2,a[2],sizeof(WORD));
	ret = sftxt_mod(w0,w1, &w2, (BYTE *)a[3], *((BYTE *)a[4] + o), (BYTE *)a[5]);
	memcpy(a[2],&w2,sizeof(WORD));
	return(ret);
#else
	return (sftxt_mod(*(WORD *)a[0], *(WORD *)a[1], (WORD *)a[2], (BYTE *)a[3], *((BYTE *)a[4] + o), (BYTE *)a[5]));
#endif
#else  /* AIX_VERS */
	return (sftxt_mod(*(WORD *)a[0], *(WORD *)a[1], (WORD *)a[2], (BYTE *)a[3], *((BYTE *)a[4] + o), (BYTE *)a[5]));
#endif /* AIX_VERS */
}

WORD PASCAL FAR _aid_acti(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
	return (sa_activ(*((BYTE *)a[0] + o), (BYTE *)a[1]));
}

WORD PASCAL FAR _aid(a)
void *a[];
{
	return (sa_aid((BYTE *)a[0]));
}

WORD PASCAL FAR _aid_desa(a)
void *a[];
{
	return (sa_desac());
}

WORD PASCAL FAR _aid_def(a)
void *a[];
{
	return (sa_aiddef((BYTE *)a[0]));
}

/* 1.3 */


WORD PASCAL FAR _bd_creat(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;

	return (bd_creat());
}

WORD PASCAL FAR _bd_insert(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
#ifdef AIX_VERS
#ifdef SPARC1
	WORD w0;
	WORD w1;
	WORD w2;
	WORD ret;

	memcpy(&w0,a[0],sizeof(WORD));
	memcpy(&w1,a[1],sizeof(WORD));
	memcpy(&w2,a[2],sizeof(WORD));
	ret = bd_insert(w0,w1,w2, (BYTE *)a[3], *((BYTE *)a[4] + o), (BYTE *)a[5]);
	return(ret);
#else
        return(bd_insert());
#endif
#else  /* AIX_VERS */
        return(bd_insert());
#endif /* AIX_VERS */
}

WORD PASCAL FAR _bd_use(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
	return (bd_use());
}

WORD PASCAL FAR _bd_close(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
	return (bd_close());
}

WORD PASCAL FAR _bd_sup(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
	return (bd_sup());
}

WORD PASCAL FAR _bd_putdata(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
	return (bd_putdata());
}

WORD PASCAL FAR _bd_getdata(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
	return (bd_getdata());
}

WORD PASCAL FAR _wexit(a)
void *a[];
{
#ifdef AIX_VERS
#ifdef SPARC
	WORD w0;
	memcpy(&w0,a[0],sizeof(WORD));
	return (wexit(w0));
#else
	return (wexit(*(WORD *)a[0]));
#endif
#else  /* AIX_VERS */
	return (wexit(*(WORD *)a[0]));
#endif /* AIX_VERS */
}

WORD PASCAL FAR _wrefresh(a)
void *a[];
{
	return (wrefresh());
}

WORD PASCAL FAR _md_way(a)
void *a[];
{
	return (md_way((BYTE *)a[0], (BYTE *)a[1]));
}

WORD PASCAL FAR _swcreat(a)
void *a[];
{
#ifdef AIX_VERS
#ifdef SPARC
	WORD w0,w1,w2,w3,w4;
	WORD ret;
	memcpy(&w0,a[0],sizeof(WORD));
	memcpy(&w1,a[1],sizeof(WORD));
	memcpy(&w2,a[2],sizeof(WORD));
	memcpy(&w3,a[3],sizeof(WORD));
	memcpy(&w4,a[4],sizeof(WORD));

	ret = swcreat(&w0, w1, w2, w3, w4);

	memcpy(a[0],&w0,sizeof(WORD));
	return(ret);

#else
	return (swcreat((WORD *)a[0], *(WORD *)a[1], *(WORD *)a[2], *(WORD *)a[3], *(WORD *)a[4]));
#endif
#else  /* AIX_VERS */
	return (swcreat((WORD *)a[0], *(WORD *)a[1], *(WORD *)a[2], *(WORD *)a[3], *(WORD *)a[4]));
#endif /* AIX_VERS */
}

WORD PASCAL FAR _wcadre(a)
void *a[];
{
#ifdef AIX_VERS
#ifdef SPARC
WORD w;
	memcpy(&w,a[0],sizeof(WORD));
	return (wcadre(w, *(BYTE *)a[1]));
#else
	return (wcadre(*(WORD *)a[0], *(BYTE *)a[1]));
#endif
#else  /* AIX_VERS */
	return (wcadre(*(WORD *)a[0], *(BYTE *)a[1]));
#endif /* AIX_VERS */
}

WORD PASCAL FAR _wtitre(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
#ifdef AIX_VERS
#ifdef SPARC
	WORD w0;
	memcpy(&w0,a[0],sizeof(WORD));
	return (wtitre(w0, *((BYTE *)a[1] + o), *((BYTE *)a[2] + o), *((BYTE *)a[3] + o)));
#else
	return (wtitre(*(WORD *)a[0], *((BYTE *)a[1] + o), *((BYTE *)a[2] + o), *((BYTE *)a[3] + o)));
#endif
#else  /* AIX_VERS */
	return (wtitre(*(WORD *)a[0], *((BYTE *)a[1] + o), *((BYTE *)a[2] + o), *((BYTE *)a[3] + o)));
#endif /* AIX_VERS */
}

WORD PASCAL FAR _wmove(a)
void *a[];
{
#ifdef AIX_VERS
#ifdef SPARC
	WORD w0,w1,w2;
	memcpy(&w0,a[0],sizeof(WORD));
	memcpy(&w1,a[1],sizeof(WORD));
	memcpy(&w2,a[2],sizeof(WORD));
	return (wmove(w0, w1, w2));
#else
	return (wmove(*(WORD *)a[0], *(WORD *)a[1], *(WORD *)a[2]));
#endif
#else  /* AIX_VERS */
	return (wmove(*(WORD *)a[0], *(WORD *)a[1], *(WORD *)a[2]));
#endif /* AIX_VERS */
}

WORD PASCAL FAR _le_delete(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
#ifdef AIX_VERS
#ifdef SPARC
	WORD w1;
	memcpy(&w1,a[1],sizeof(WORD));
	return (le_delete(*((BYTE *)a[0] + o), w1));
#else
	return (le_delete(*((BYTE *)a[0] + o), *(WORD *)a[1]));
#endif
#else  /* AIX_VERS */
	return (le_delete(*((BYTE *)a[0] + o), *(WORD *)a[1]));
#endif /* AIX_VERS */
}

WORD PASCAL FAR _le_insert(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
#ifdef AIX_VERS
#ifdef SPARC
	WORD w1;
	WORD ret;
	memcpy(&w1,a[1],sizeof(WORD));
	ret = le_insert(*((BYTE *)a[0] + o), &w1, *((BYTE *)a[2] + o), *((BYTE*)a[3]+o), (BYTE *)a[4], (BYTE *)a[5]);
	memcpy(a[1],&w1,sizeof(WORD));
	return(ret);
#else
	return (le_insert(*((BYTE *)a[0] + o), (WORD *)a[1], *((BYTE *)a[2] + o), *((BYTE*)a[3]+o), (BYTE *)a[4], (BYTE *)a[5]));
#endif
#else  /* AIX_VERS */
	return (le_insert(*((BYTE *)a[0] + o), (WORD *)a[1], *((BYTE *)a[2] + o), *((BYTE*)a[3]+o), (BYTE *)a[4], (BYTE *)a[5]));
#endif /* AIX_VERS */
}

WORD PASCAL FAR _sfset_tch_esc(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
	return (sfset_tch_esc( *((BYTE *)a[0]+o), (BYTE *)a[1]));
}

WORD PASCAL FAR _wfinfo(a)
void *a[];
{
#ifdef AIX_VERS
#ifdef SPARC
	WORD w0;
	memcpy(&w0,a[0],sizeof(WORD));
	return (wfinfo(w0, (BYTE *)a[1]));
#else
	return (wfinfo(*(WORD *)a[0], (BYTE *)a[1]));
#endif
#else  /* AIX_VERS */
	return (wfinfo(*(WORD *)a[0], (BYTE *)a[1]));
#endif /* AIX_VERS */
}

WORD PASCAL FAR _get_style(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
	return (get_style(*((BYTE *)a[0]+o), (BYTE *)a[1]));
}

WORD PASCAL FAR _set_style(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
	return (set_style(*((BYTE *)a[0]+o), (BYTE *)a[1]));
}

WORD PASCAL FAR _wlangue(a)
void *a[];
{
	WORD m = 0x0100;
	int o = *(BYTE *)&m;
	return (wlangue(*((BYTE *)a[0]+o)));
}

WORD PASCAL FAR _asfvers(a)
void *a[];
{
	return(asfvers( (BYTE *)a[0]));
}

WORD PASCAL FAR _who_is_there(a)
void *a[];
{
#ifdef AIX_VERS
#ifdef SPARC
	WORD w0, w1, w2, w3;
	WORD ret;
	memcpy(&w0,a[0],sizeof(WORD));
	memcpy(&w1,a[1],sizeof(WORD));
	memcpy(&w2,a[2],sizeof(WORD));
	memcpy(&w3,a[3],sizeof(WORD));
	ret = who_is_there(w0, w1, &w2, &w3);
	memcpy(a[2],&w2,sizeof(WORD));
	memcpy(a[3],&w3,sizeof(WORD));
	return(ret);
#else
	return (who_is_there(*(WORD *)a[0], *(WORD *)a[1], (WORD *)a[2], (WORD *)a[3]));
#endif
#else  /* AIX_VERS */
	return (who_is_there(*(WORD *)a[0], *(WORD *)a[1], (WORD *)a[2], (WORD *)a[3]));
#endif /* AIX_VERS */
}


WORD PASCAL FAR _get_event(a)
void *a[];
{
	return (get_event( (BYTE *)a[0]));
}


WORD PASCAL FAR _put_event(a)
void *a[];
{
	return (put_event( (BYTE *)a[0]));
}
WORD PASCAL FAR _add_scroll_bar(a)
void *a[];
{
#ifdef AIX_VERS
#ifdef SPARC
	WORD w0;
	memcpy(&w0,a[0],sizeof(WORD));
	return (add_scroll_bar(w0, *(BYTE *)a[1]));
#else
	return (add_scroll_bar(*(WORD *)a[0], *(BYTE *)a[1]));
#endif
#else  /* AIX_VERS */
	return (add_scroll_bar(*(WORD *)a[0], *(BYTE *)a[1]));
#endif /* AIX_VERS */
}
WORD PASCAL FAR _del_scroll_bar(a)
void *a[];
{
#ifdef AIX_VERS
#ifdef SPARC
	WORD w0;
	memcpy(&w0,a[0],sizeof(WORD));
	return (del_scroll_bar(w0, *(BYTE *)a[1]));
#else
	return (del_scroll_bar(*(WORD *)a[0], *(BYTE *)a[1]));
#endif
#else  /* AIX_VERS */
	return (del_scroll_bar(*(WORD *)a[0], *(BYTE *)a[1]));
#endif /* AIX_VERS */
}
WORD PASCAL FAR _update_scroll_bar(a)
void *a[];
{
#ifdef AIX_VERS
#ifdef SPARC
	WORD w0, w2, w3;
	memcpy(&w0,a[0],sizeof(WORD));
	memcpy(&w2,a[2],sizeof(WORD));
	memcpy(&w3,a[3],sizeof(WORD));
	return (update_scroll_bar(w0, *(BYTE *)a[1], w2, w3));
#else
	return (update_scroll_bar(*(WORD *)a[0], *(BYTE *)a[1], *(WORD *)a[2], *(WORD *)a[3]));
#endif
#else  /* AIX_VERS */
	return (update_scroll_bar(*(WORD *)a[0], *(BYTE *)a[1], *(WORD *)a[2], *(WORD *)a[3]));
#endif /* AIX_VERS */
}
WORD PASCAL FAR _get_scroll_bar_region(a)
void *a[];
{
#ifdef AIX_VERS
#ifdef SPARC
	WORD w0, w2, w3, w4, w5;
	memcpy(&w0,a[0],sizeof(WORD));
	memcpy(&w2,a[2],sizeof(WORD));
	memcpy(&w3,a[3],sizeof(WORD));
	memcpy(&w4,a[4],sizeof(WORD));
	memcpy(&w5,a[5],sizeof(WORD));
	return (get_scroll_bar_region(w0, *(BYTE *)a[1], w2, w3, w4, w5));
#else
	return (get_scroll_bar_region(*(WORD *)a[0], *(BYTE *)a[1], *(WORD *)a[2], *(WORD *)a[3], *(WORD *)a[4], *(WORD *)a[5]));
#endif
#else  /* AIX_VERS */
	return (get_scroll_bar_region(*(WORD *)a[0], *(BYTE *)a[1], *(WORD *)a[2], *(WORD *)a[3], *(WORD *)a[4], *(WORD *)a[5]));
#endif /* AIX_VERS */
}
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/


WORD FAR __co_flush()
{
/*	return (_co_flush()); */ /* _co_flush return void */
	_co_flush();
	return(0);
}

WORD FAR __co_start()
{
	/*	return (_co_start()); */ /* _co_start return void */
	_co_start();
	return(0);
}

WORD FAR _x_getch()
{
	return (x_getch());
}

WORD FAR _x_kbhit()
{
	return (x_kbhit());
}

WORD FAR _prfo(a)
void *a[];
{
#ifdef AIX_VERS
#ifdef SPARC
	WORD w0, w2;
	memcpy(&w0,a[0],sizeof(WORD));
	memcpy(&w2,a[2],sizeof(WORD));
	return (prfo(w0, (BYTE *)a[1], w2));
#else
	return (prfo(*(WORD *)a[0], (BYTE *)a[1], *(WORD *)a[2]));
#endif
#else  /* AIX_VERS */
	return (prfo(*(WORD *)a[0], (BYTE *)a[1], *(WORD *)a[2]));
#endif /* AIX_VERS */
}

WORD FAR _lcfo(a)
void *a[];
{
#ifdef AIX_VERS
#ifdef SPARC
	WORD w0, w2;
	memcpy(&w0,a[0],sizeof(WORD));
	memcpy(&w2,a[2],sizeof(WORD));
	return (lcfo(w0, (BYTE *)a[1], w2));
#else
	return (lcfo(*(WORD *)a[0], (BYTE *)a[1], *(WORD *)a[2]));
#endif
#else /* AIX_VERS */
	return (lcfo(*(WORD *)a[0], (BYTE *)a[1], *(WORD *)a[2]));
#endif /* AIX_VERS */
}

WORD FAR _Run_Conf(a)
void *a[];
{
#ifdef AIX_VERS
#ifdef SPARC
	WORD w0;
	memcpy(&w0,a[0],sizeof(WORD));
	return (Run_Conf(w0));
#else
	return (Run_Conf(*(WORD *)a[0]));
#endif
#else /* AIX_VERS */
	return (Run_Conf(*(WORD *)a[0]));
#endif /* AIX_VERS */
}

void FAR _Run_Co(a)
void *a[];
{
#ifdef AIX_VERS
#ifdef SPARC
	WORD w0;
	memcpy(&w0,a[0],sizeof(WORD));
	Run_Co(w0);
#else
	Run_Co(*(WORD *)a[0]);
#endif
#else /* AIX_VERS */
	Run_Co(*(WORD *)a[0]);
#endif /* AIX_VERS */
}

void FAR _Run_Mes(a)
void *a[];
{
#ifdef AIX_VERS
#ifdef SPARC
	WORD w1;
	memcpy(&w1,a[1],sizeof(WORD));
	Run_Mes((BYTE *)a[0], w1);
#else
	Run_Mes((BYTE *)a[0], *(WORD *)a[1]);
#endif
#else  /* AIX_VERS */
	Run_Mes((BYTE *)a[0], *(WORD *)a[1]);
#endif /* AIX_VERS */
}

void FAR __lo(a)
void *a[];
{
#ifdef AIX_VERS
#ifdef SPARC
	WORD w0;
	memcpy(&w0,a[0],sizeof(WORD));
	_lo(w0);
#else
	_lo(*(WORD *)a[0]);
#endif
#else  /* AIX_VERS */
	_lo(*(WORD *)a[0]);
#endif /* AIX_VERS */
}

WORD FAR _Run_Lin(a)
void *a[];
{
	return(Run_Lin());
}

WORD FAR _Run_Col(a)
void *a[];
{
	return(Run_Col());
}

#endif /* unix || M_XENIX || vms || dosext */




/* InitRelais - initialisation bibliotheque
 * ----------------------------------------
 *
 * appel:	void FAR *InitRelais(origine);
 *		char origine = origine de l'appel
 *			       0 = appel par InitRunTime()
 *			       1 = appel depuis l'interpreteur de commandes
 *
 * retour:	pointeur sur le descripteur de bibliotheque
 *
 */
#ifdef	KEEP_THIS_SHIT
void FAR *InitRelais(origine)
char origine;
#else
void FAR * InitRelais()
#endif
{
#ifdef KEEP_THIS_SHIT
#if !PRL
	if (origine == 0) {
		initerm();
		prndest = 1;
/*		x_clear();		corr 21.02.94 */
	}
#endif /* !PRL */
#endif
	Descripteur.banniere = (void (FAR *)())Banniere;
#ifdef	KEEP_THIS_SHIT
	Descripteur.fin = (origine == 0) ? (void (FAR *)())FinRelais : (void (FAR *)())0;
#else
	Descripteur.fin = (void (FAR *)())FinRelais;
#endif

	Descripteur.nombre = NB_WFONC+12;

#if unix || M_XENIX || vms || dosext

	Descripteur.fonction[0] = (WORD (FAR *)())_winit;
	Descripteur.fonction[1] = (WORD (FAR *)())_wcreat;
	Descripteur.fonction[2] = (WORD (FAR *)())_wsup;
	Descripteur.fonction[3] = (WORD (FAR *)())_wwrite;
	Descripteur.fonction[4] = (WORD (FAR *)())_wselect;
	Descripteur.fonction[5] = (WORD (FAR *)())_winv;
	Descripteur.fonction[6] = (WORD (FAR *)())_wvis;
	Descripteur.fonction[7] = (WORD (FAR *)())_wfgen;
	Descripteur.fonction[8] = (WORD (FAR *)())_watb;
	Descripteur.fonction[9] = (WORD (FAR *)())_wposcur;
	Descripteur.fonction[10] = (WORD (FAR *)())_wimpl;
	Descripteur.fonction[11] = (WORD (FAR *)())_wgel;
	Descripteur.fonction[12] = (WORD (FAR *)())_wfend;
	Descripteur.fonction[13] = (WORD (FAR *)())_wsetatb;
	Descripteur.fonction[14] = (WORD (FAR *)())_sf_init;
	Descripteur.fonction[15] = (WORD (FAR *)())_lsacti;
	Descripteur.fonction[16] = (WORD (FAR *)())_lsmess;
	Descripteur.fonction[17] = (WORD (FAR *)())_lsclear;
	Descripteur.fonction[18] = (WORD (FAR *)())_lsdesac;
	Descripteur.fonction[19] = (WORD (FAR *)())_menu_cre;
	Descripteur.fonction[20] = (WORD (FAR *)())_menu_opt;
	Descripteur.fonction[21] = (WORD (FAR *)())_menu_sep;
	Descripteur.fonction[22] = (WORD (FAR *)())_menu_mod;
	Descripteur.fonction[23] = (WORD (FAR *)())_menu_lat;
	Descripteur.fonction[24] = (WORD (FAR *)())_menu_mat;
	Descripteur.fonction[25] = (WORD (FAR *)())_menu_ann;
	Descripteur.fonction[26] = (WORD (FAR *)())_menu_aff;
	Descripteur.fonction[27] = (WORD (FAR *)())_menu_cho;
	Descripteur.fonction[28] = (WORD (FAR *)())_menu_eff;
	Descripteur.fonction[29] = (WORD (FAR *)())_menu_sup;
	Descripteur.fonction[30] = (WORD (FAR *)())_list_cre;
	Descripteur.fonction[31] = (WORD (FAR *)())_lismul_c;
	Descripteur.fonction[32] = (WORD (FAR *)())_list_ele;
	Descripteur.fonction[33] = (WORD (FAR *)())_list_lec;
	Descripteur.fonction[34] = (WORD (FAR *)())_list_sui;
	Descripteur.fonction[35] = (WORD (FAR *)())_list_pre;
	Descripteur.fonction[36] = (WORD (FAR *)())_list_mat;
	Descripteur.fonction[37] = (WORD (FAR *)())_list_fer;
	Descripteur.fonction[38] = (WORD (FAR *)())_list_aff;
	Descripteur.fonction[39] = (WORD (FAR *)())_list_cho;
	Descripteur.fonction[40] = (WORD (FAR *)())_list_eff;
	Descripteur.fonction[41] = (WORD (FAR *)())_list_sup;
	Descripteur.fonction[42] = (WORD (FAR *)())_bvalid_s;
	Descripteur.fonction[43] = (WORD (FAR *)())_bvalid;
	Descripteur.fonction[44] = (WORD (FAR *)())_sf_end;
	Descripteur.fonction[45] = (WORD (FAR *)())_wfmod;
	Descripteur.fonction[46] = (WORD (FAR *)())_aid_acti;
	Descripteur.fonction[47] = (WORD (FAR *)())_aid;
	Descripteur.fonction[48] = (WORD (FAR *)())_aid_desa;
	Descripteur.fonction[49] = (WORD (FAR *)())_aid_def;
	Descripteur.fonction[50] = (WORD (FAR *)())_wgetimpl;
/* 1.3 */

	Descripteur.fonction[51] = (WORD (FAR *)())_bd_creat;
	Descripteur.fonction[52] = (WORD (FAR *)())_bd_insert;
	Descripteur.fonction[53] = (WORD (FAR *)())_bd_use;
	Descripteur.fonction[54] = (WORD (FAR *)())_bd_close;
	Descripteur.fonction[55] = (WORD (FAR *)())_bd_sup;
	Descripteur.fonction[56] = (WORD (FAR *)())_bd_putdata;
	Descripteur.fonction[57] = (WORD (FAR *)())_bd_getdata;
	Descripteur.fonction[58] = (WORD (FAR *)())_wexit;
	Descripteur.fonction[59] = (WORD (FAR *)())_wrefresh;
	Descripteur.fonction[60] = (WORD (FAR *)())_md_way;
	Descripteur.fonction[61] = (WORD (FAR *)())_swcreat;
	Descripteur.fonction[62] = (WORD (FAR *)())_wcadre;
	Descripteur.fonction[63] = (WORD (FAR *)())_wtitre;
	Descripteur.fonction[64] = (WORD (FAR *)())_wmove;
	Descripteur.fonction[65] = (WORD (FAR *)())_le_delete;
	Descripteur.fonction[66] = (WORD (FAR *)())_le_insert;
	Descripteur.fonction[67] = (WORD (FAR *)())_sfset_tch_esc;
	Descripteur.fonction[68] = (WORD (FAR *)())_wfinfo;


/********************* 1.4 *****/

	Descripteur.fonction[69] = (WORD (FAR *)())_get_style;
	Descripteur.fonction[70] = (WORD (FAR *)())_set_style;
	Descripteur.fonction[71] = (WORD (FAR *)())_wlangue;
	Descripteur.fonction[72] = (WORD (FAR *)())_asfvers;
/*
	Descripteur.fonction[73] = (WORD (FAR *)())set_mouse_position;
	Descripteur.fonction[74] = (WORD (FAR *)())mouse_on;
	Descripteur.fonction[75] = (WORD (FAR *)())mouse_off;
	Descripteur.fonction[76] = (WORD (FAR *)())put_item_mouse_list;
	Descripteur.fonction[77] = (WORD (FAR *)())del_item_mouse_list;
	Descripteur.fonction[78] = (WORD (FAR *)())get_item_mouse_list;
	Descripteur.fonction[79] = (WORD (FAR *)())wait_event;
	Descripteur.fonction[80] = (WORD (FAR *)())flush_event;
	Descripteur.fonction[81] = (WORD (FAR *)())_get_event;
*/
	Descripteur.fonction[82] = (WORD (FAR *)())_put_event;
	Descripteur.fonction[83] = (WORD (FAR *)())_add_scroll_bar;
	Descripteur.fonction[84] = (WORD (FAR *)())_del_scroll_bar;
	Descripteur.fonction[85] = (WORD (FAR *)())_update_scroll_bar;
	Descripteur.fonction[86] = (WORD (FAR *)())_get_scroll_bar_region;
/*
	Descripteur.fonction[87] = (WORD (FAR *)())read_event;
*/
	Descripteur.fonction[88] = (WORD (FAR *)())_who_is_there;

/****************/

	Descripteur.fonction[NB_WFONC+1] = (WORD (FAR *)())_x_getch;
	Descripteur.fonction[NB_WFONC+2] = (WORD (FAR *)())_x_kbhit;
	Descripteur.fonction[NB_WFONC+3] = (WORD (FAR *)())_prfo;
	Descripteur.fonction[NB_WFONC+4] = (WORD (FAR *)())_lcfo;
	Descripteur.fonction[NB_WFONC+5] = (WORD (FAR *)())_Run_Conf;
	Descripteur.fonction[NB_WFONC+6] = (WORD (FAR *)())_Run_Co;
	Descripteur.fonction[NB_WFONC+7] = (WORD (FAR *)())_Run_Str;
	Descripteur.fonction[NB_WFONC+8] = (WORD (FAR *)())_Run_Lin;
	Descripteur.fonction[NB_WFONC+9] = (WORD (FAR *)())_Run_Col;
	Descripteur.fonction[NB_WFONC+10] = (WORD (FAR *)())__co_start;
	Descripteur.fonction[NB_WFONC+11] = (WORD (FAR *)())__co_flush;
	Descripteur.fonction[NB_WFONC+12] = (WORD (FAR *)())__lo;

#else
	Descripteur.fonction[0] = (WORD (FAR *)())winit;
	Descripteur.fonction[1] = (WORD (FAR *)())wcreat;
	Descripteur.fonction[2] = (WORD (FAR *)())wsup;
	Descripteur.fonction[3] = (WORD (FAR *)())wwrite;
	Descripteur.fonction[4] = (WORD (FAR *)())wselect;
	Descripteur.fonction[5] = (WORD (FAR *)())winv;
	Descripteur.fonction[6] = (WORD (FAR *)())wvis;
	Descripteur.fonction[7] = (WORD (FAR *)())wfgen;
	Descripteur.fonction[8] = (WORD (FAR *)())watb;
	Descripteur.fonction[9] = (WORD (FAR *)())wposcur;
	Descripteur.fonction[10] = (WORD (FAR *)())wimpl;
	Descripteur.fonction[11] = (WORD (FAR *)())wgel;
	Descripteur.fonction[12] = (WORD (FAR *)())wend;
	Descripteur.fonction[13] = (WORD (FAR *)())wsetatb;
	Descripteur.fonction[14] = (WORD (FAR *)())sfinit;
	Descripteur.fonction[15] = (WORD (FAR *)())lsacti;
	Descripteur.fonction[16] = (WORD (FAR *)())lsmess;
	Descripteur.fonction[17] = (WORD (FAR *)())lsclear;
	Descripteur.fonction[18] = (WORD (FAR *)())lsdesac;
	Descripteur.fonction[19] = (WORD (FAR *)())md_creat;
	Descripteur.fonction[20] = (WORD (FAR *)())mo_creat;
	Descripteur.fonction[21] = (WORD (FAR *)())mo_sep;
	Descripteur.fonction[22] = (WORD (FAR *)())mo_modif;
	Descripteur.fonction[23] = (WORD (FAR *)())mo_latt;
	Descripteur.fonction[24] = (WORD (FAR *)())mo_matt;
	Descripteur.fonction[25] = (WORD (FAR *)())mo_clear;
	Descripteur.fonction[26] = (WORD (FAR *)())md_aff;
	Descripteur.fonction[27] = (WORD (FAR *)())md_opt;
	Descripteur.fonction[28] = (WORD (FAR *)())md_clear;
	Descripteur.fonction[29] = (WORD (FAR *)())md_sup;
	Descripteur.fonction[30] = (WORD (FAR *)())ld_creat;
	Descripteur.fonction[31] = (WORD (FAR *)())lm_creat;
	Descripteur.fonction[32] = (WORD (FAR *)())le_creat;
	Descripteur.fonction[33] = (WORD (FAR *)())le_lect;
	Descripteur.fonction[34] = (WORD (FAR *)())le_down;
	Descripteur.fonction[35] = (WORD (FAR *)())le_up;
	Descripteur.fonction[36] = (WORD (FAR *)())le_matt;
	Descripteur.fonction[37] = (WORD (FAR *)())ld_close;
	Descripteur.fonction[38] = (WORD (FAR *)())ld_aff;
	Descripteur.fonction[39] = (WORD (FAR *)())ld_choix;
	Descripteur.fonction[40] = (WORD (FAR *)())ld_clear;
	Descripteur.fonction[41] = (WORD (FAR *)())ld_sup;
	Descripteur.fonction[42] = (WORD (FAR *)())bv_spc;
	Descripteur.fonction[43] = (WORD (FAR *)())bv_std;
	Descripteur.fonction[44] = (WORD (FAR *)())sfend;
	Descripteur.fonction[45] = (WORD (FAR *)())sftxt_mod;
	Descripteur.fonction[46] = (WORD (FAR *)())sa_activ;
	Descripteur.fonction[47] = (WORD (FAR *)())sa_aid;
	Descripteur.fonction[48] = (WORD (FAR *)())sa_desac;
	Descripteur.fonction[49] = (WORD (FAR *)())sa_aiddef;
	Descripteur.fonction[50] = (WORD (FAR *)())wgetimpl;
/* 1.3 */

	Descripteur.fonction[51] = (WORD (FAR *)())bd_creat;
	Descripteur.fonction[52] = (WORD (FAR *)())bd_insert;
	Descripteur.fonction[53] = (WORD (FAR *)())bd_use;
	Descripteur.fonction[54] = (WORD (FAR *)())bd_close;
	Descripteur.fonction[55] = (WORD (FAR *)())bd_sup;
	Descripteur.fonction[56] = (WORD (FAR *)())bd_putdata;
	Descripteur.fonction[57] = (WORD (FAR *)())bd_getdata;
	Descripteur.fonction[58] = (WORD (FAR *)())wexit;
	Descripteur.fonction[59] = (WORD (FAR *)())wrefresh;
	Descripteur.fonction[60] = (WORD (FAR *)())md_way;
	Descripteur.fonction[61] = (WORD (FAR *)())swcreat;
	Descripteur.fonction[62] = (WORD (FAR *)())wcadre;
	Descripteur.fonction[63] = (WORD (FAR *)())wtitre;
	Descripteur.fonction[64] = (WORD (FAR *)())wmove;
	Descripteur.fonction[65] = (WORD (FAR *)())le_delete;
	Descripteur.fonction[66] = (WORD (FAR *)())le_insert;
	Descripteur.fonction[67] = (WORD (FAR *)())sfset_tch_esc;
	Descripteur.fonction[68] = (WORD (FAR *)())wfinfo;

/* 1.4 */	

	Descripteur.fonction[69] = (WORD (FAR *)())get_style;
	Descripteur.fonction[70] = (WORD (FAR *)())set_style;
	Descripteur.fonction[71] = (WORD (FAR *)())wlangue;
	Descripteur.fonction[72] = (WORD (FAR *)())asfvers;


	Descripteur.fonction[73] = (WORD (FAR *)())set_mouse_position;
	Descripteur.fonction[74] = (WORD (FAR *)())mouse_on;
	Descripteur.fonction[75] = (WORD (FAR *)())mouse_off;
	Descripteur.fonction[76] = (WORD (FAR *)())put_item_mouse_list;
	Descripteur.fonction[77] = (WORD (FAR *)())del_item_mouse_list;
	Descripteur.fonction[78] = (WORD (FAR *)())get_item_mouse_list;
	Descripteur.fonction[79] = (WORD (FAR *)())wait_event;
	Descripteur.fonction[80] = (WORD (FAR *)())flush_event;
	Descripteur.fonction[81] = (WORD (FAR *)())get_event;
	Descripteur.fonction[82] = (WORD (FAR *)())put_event;
	Descripteur.fonction[83] = (WORD (FAR *)())add_scroll_bar;
	Descripteur.fonction[84] = (WORD (FAR *)())del_scroll_bar;
	Descripteur.fonction[85] = (WORD (FAR *)())update_scroll_bar;
	Descripteur.fonction[86] = (WORD (FAR *)())get_scroll_bar_region;
	Descripteur.fonction[87] = (WORD (FAR *)())read_event;
	Descripteur.fonction[88] = (WORD (FAR *)())who_is_there;



#if !PRL
	Descripteur.fonction[NB_WFONC+1] = (WORD (FAR *)())x_getch;
	Descripteur.fonction[NB_WFONC+2] = (WORD (FAR *)())x_kbhit;
	Descripteur.fonction[NB_WFONC+3] = (WORD (FAR *)())prfo;
	Descripteur.fonction[NB_WFONC+4] = (WORD (FAR *)())lcfo;
	Descripteur.fonction[NB_WFONC+5] = (WORD (FAR *)())Run_Conf;
	Descripteur.fonction[NB_WFONC+6] = (WORD (FAR *)())Run_Co;
	Descripteur.fonction[NB_WFONC+7] = (WORD (FAR *)())Run_Str;
	Descripteur.fonction[NB_WFONC+8] = (WORD (FAR *)())Run_Lin;
	Descripteur.fonction[NB_WFONC+9] = (WORD (FAR *)())Run_Col;
	Descripteur.fonction[NB_WFONC+10] = (WORD (FAR *)())_co_start;
	Descripteur.fonction[NB_WFONC+11] = (WORD (FAR *)())_co_flush;
	Descripteur.fonction[NB_WFONC+12] = (WORD (FAR *)())_lo;
#endif /* !PRL */
#endif /* unix || M_XENIX || vms */
#ifdef	KEEP_THIS_SHIT
	if (origine == 0) {
#endif
		rt_init->cico.signature = BDA_ASFUN;

#if PRL
		rt_init->cico.Relay_Co = (void far *)Run_Co;
		rt_init->cico.Relay_Info = (void far *)Run_Info;
		rt_init->cico.Relay_Mes = (void far *)Run_Str;
/*llb 21/10/93 
		rt_init->cico.capacite = 0x0007;
*/
		rt_init->cico.capacite = 0x000F;	
		rt_init->cico.Relay_Lo = (WORD far *)Run_Mi;

#else
		rt_init->cico.fonction[CICO_CI]     = NB_WFONC+1;
		rt_init->cico.fonction[CICO_TPCI]   = NB_WFONC+2;
		rt_init->cico.fonction[CICO_PRFO]   = NB_WFONC+3;
		rt_init->cico.fonction[CICO_LCFO]   = NB_WFONC+4;
		rt_init->cico.fonction[CICO_CONF]   = NB_WFONC+5;
		rt_init->cico.fonction[CICO_CO]     = NB_WFONC+6;
		rt_init->cico.fonction[CICO_MES]    = NB_WFONC+7;
		rt_init->cico.fonction[CICO_LIGNE]  = NB_WFONC+8;
		rt_init->cico.fonction[CICO_COLONNE]= NB_WFONC+9;
		rt_init->cico.fonction[CICO_START]  = NB_WFONC+10;
		rt_init->cico.fonction[CICO_FLUSH]  = NB_WFONC+11;
		rt_init->cico.fonction[CICO_LO]     = NB_WFONC+12;
#endif /* PRL */
#ifdef	KEEP_THIS_SHIT
		}
#endif

	return((void FAR *)&Descripteur);
}


/* FinRelais - terminaison de la bibliotheque
 * ------------------------------------------
 *
 * appel:	static void FinRelais(void);
 *
 * retour:	rien
 *
 */
static void FAR FinRelais()
{
	sfend();
#if !PRL
	finterm(0);
#if unix
	PurgeLox();
#endif /* unix */
#endif /* !PRL */
}


/* banniere - banniere de la bibliotheque
 * --------------------------------------
 *
 * appel:	static void banniere(void);
 *
 * retour:	rien
 *
 */
static void FAR Banniere()
{
	NouvelleLigne();
	AfficheChaine(ChercheMessage(1));
	AfficheChaine(version);
#if PRL3
	AfficheChaine("-P3");
#endif
#if PROV
	AfficheChaine(provisoire);
#endif
	NouvelleLigne();
	AfficheChaine(copyright);
	NouvelleLigne();
}


/* AfficheChaine - affichage d'une chaine sur l'ecran
 * --------------------------------------------------
 *
 * appel:	static void AfficheChaine(chaine);
 *		char chaine = chaine a afficher
 *
 * retour:	rien
 *
 */
static void AfficheChaine(chaine)
char *chaine;
{
#if MSDOS || PRL || dosext
	cputs(chaine);
#else
	fputs(chaine, stdout);
#endif
}


/* NouvellLigne - passage a la ligne suivante sur l'ecran
 * ------------------------------------------------------
 *
 * appel:	static void NouvelleLigne(void);
 *
 * retour:	rien
 *
 */
static void NouvelleLigne()
{
#if MSDOS || PRL || dosext
	cputs("\r\n");
#else
	putchar('\n');
#endif
}


/* ChercheMessage - recherche d'un message dans la table
 * -----------------------------------------------------
 *
 * appel:	static void ChercheMessage(numero);
 *		int numero = numero de message
 *
 * retour:	pointeur sur le message
 *
 */
static char *ChercheMessage(numero)
int numero;
{
	char *p;

	p = message[0];
	while (numero) {
		while (*p++);
		while (!*p++);
		numero--;
	}
	return (--p);
}


WORD asfvers( BPTR chaine)
{
	if(rt_arg.c != 1)
	    return(RT_err_arg);
	if(rt_arg.t[0] != (TYPE_CHAINE | MODIFIABLE))
	    return(RT_err_typ);

	chaine[0] = VERS;
	chaine[1] = INDICE;
	chaine[2] = RELEASE;
	return(0);
}


/* fin de fichier */

