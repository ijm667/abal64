/* #define	DEBUG */
/* --------------------------------------------------------------------	*/
/*									*/
/*		Project ABAL : Macro pre-processer			*/
/*									*/
/*		 Copyright (c) 1990, 1994 Amenesik / Sologic s.a.			*/
/*									*/
/*									*/
/*		File	:	TRSHADOW.C				*/
/*		Version :	2.1d					*/
/*		Date	:	15/10/1996				*/
/*									*/
/* --------------------------------------------------------------------	*/
/*	15/10/1996	:	Correction of Register Lock/Unlock	*/
/*				which manifested itself after many	*/
/*				CLASS USER FUNCTION activations.	*/
/* --------------------------------------------------------------------	*/

#ifndef	_TRSHADOW_C
#define	_TRSHADOW_C

#include <stdio.h>		/* Standard IO handling routines	*/
#include <errno.h>		/* Error return definitions		*/
#include "stdtypes.h"
#include "translat.h"
#include "trshadow.h"
#include "troption.h"
#include "trdebug.h"
#include "trerrors.h"
#include "trtables.h"
#include "trproc.h"
#include "triof.h"
#include "troutput.h"
#include "trecho.h"

#ifndef	UNIX
#else
BPTR	get_echo_text();
#endif

#define	checkmoinsun(l) 

static WORD	duplicate_type( tptr, sptr )
TCTYPPTR	tptr;
TCTYPPTR	sptr;
{
	if ((tptr != (TCTYPPTR) 0)
	&&  (sptr != (TCTYPPTR) 0)) {
		tptr->type = sptr->type;
		tptr->size = sptr->size;
		tptr->first = sptr->first;
		tptr->second = sptr->second;
		tptr->class  = sptr->class;
		return( 0 );
		}
	else	return( internal_error( 5555 ) );
}


SHADOWPTR	allocate_for_register()
{
	SHADOWPTR	rptr;

	trace("allocate_for_register()\n");

	if ((rptr = RegisterHeap) != (SHADOWPTR) 0)
		RegisterHeap = rptr->next;
	else if ((rptr = (SHADOWPTR) allocate( sizeof( struct otr_register ))) == (SHADOWPTR) 0)
		return( rptr );
	else if ((rptr->type = allocate_for_type(0,0,0,0,(VPTR) 0)) == (TCTYPPTR) 0) {
		liberate( rptr );
		return( (SHADOWPTR) 0);
		}
	rptr->prev = (SHADOWPTR) 0;
	if ((rptr->next = RegisterUsed) != (SHADOWPTR) 0)
		rptr->next->prev = rptr;
	RegisterUsed   = rptr;
	rptr->number   = 0;
	rptr->nature   = 0;
	rptr->status   = _REGISTER_DEAD;
	rptr->contents = (VPTR) 0;
	rptr->value    = 0;
	return( rptr );
}

SHADOWPTR	lock_register( sptr )
SHADOWPTR	sptr;
{
	SHADOWPTR	rptr;
	WORD		status;

	trace("lock_register()\n");

	if ( RegisterLock[ sptr->number ] != 0 ) {
		RegisterLock[ sptr->number ]++;
		return( sptr );
		}
			
	if ((rptr = RegisterHeap) != (SHADOWPTR) 0)
		RegisterHeap = rptr->next;
	else if ((rptr = (SHADOWPTR) allocate( sizeof( struct otr_register ))) == (SHADOWPTR) 0)
		return( rptr );
	else if ((rptr->type = allocate_for_type(0,0,0,0,(VPTR) 0)) == (TCTYPPTR) 0) {
		liberate( rptr );
		return( (SHADOWPTR) 0);
		}

	rptr->prev     = (SHADOWPTR) 0;
	rptr->next     = (SHADOWPTR) 0;
	rptr->nature   = sptr->nature;
	if ((status = duplicate_type( rptr->type, sptr->type )) != 0) {
		liberate_for_type( rptr->type );
		liberate( rptr );
		return( (SHADOWPTR) 0);
		}
	RegisterLock[ (	rptr->number = sptr->number ) ]++;
	rptr->status   = sptr->status;
	rptr->contents = sptr->contents;
	rptr->value    = sptr->value;
	return( rptr );
}

VOID	unlock_register( sptr )
SHADOWPTR	sptr;
{
	trace("unlock_register()\n");
#ifndef	_BEFORE_15101996
	if ( RegisterLock[sptr->number] != 0 ) {
#endif
		RegisterLock[ sptr->number ]--;

		if ( RegisterLock[ sptr->number ] == 0 ) {
			sptr->next = RegisterHeap;
			RegisterHeap = sptr;
			}
#ifndef	_BEFORE_15101996
		}
#endif
	return;
}

VOID	initialise_shadower()
{
	WORD	r;
	RegisterHeap = (SHADOWPTR) 0;
	RegisterUsed = (SHADOWPTR) 0;
	BaseRegister = 1;
	Register     = 1;
	for ( r = 0; r < MAX_REGISTERS; r++ ) {
		Registers[r] = (SHADOWPTR) 0;
		RegisterLock[r] = 0;
		}
	return;
}

SHADOWNODE	release_shadow_list( sptr )
SHADOWNODE	sptr;
{
	if ( sptr != (SHADOWNODE) 0 ) {
		if (( sptr->nature & _SHADOW_MASK ) == _SHADOW_CONV )
			sptr->lptr = (VPTR) release_shadow_list( sptr->lptr );
		else if (( sptr->nature & _SHADOW_MASK ) == _SHADOW_TEXT )
			liberate( sptr->lptr );
		sptr->lptr = (VPTR) ShadowFree;
		ShadowFree = sptr;
		}
	return((SHADOWNODE) 0);
}

VOID	reset_shadower()
{
	SHADOWPTR	rptr;
	SHADOWPTR	tptr;
	WORD		r;

/*	sysprintf("entry_reset_shadower(BaseRegister = %u,Register = %u)\n",BaseRegister,Register); */

	for ( r = BaseRegister; r < Register; r++ )
		if (!( RegisterLock[r] ))
			Registers[r] = (SHADOWPTR) 0;

	if ((rptr = RegisterUsed) != (SHADOWPTR) 0) {
		while (1) {
			if (( rptr->number >= BaseRegister )
			&&  (!( RegisterLock[ rptr->number ] ))) {
				if ( rptr->operand != (SHADOWNODE) 0) {
					checkmoinsun((rptr->operand = release_shadow_list( rptr->operand )));
					}
				if ((tptr = rptr->next) != (SHADOWPTR) 0 )
					rptr->next->prev = rptr->prev;
				if ( rptr->prev != (SHADOWPTR) 0 )
					rptr->prev->next = rptr->next;
				else	RegisterUsed     = rptr->next;

				rptr->next = RegisterHeap;
				RegisterHeap = rptr;

				if ((rptr = tptr) == (SHADOWPTR) 0)
					break;
				}
			else if ((rptr = rptr->next) == (SHADOWPTR) 0)
				break;
			}
		}

	Register = BaseRegister;
/*	sysprintf("exit_reset_shadower(BaseRegister = %u,Register = %u)\n",BaseRegister,Register); */
	return;
}

VOID	terminate_shadower()
{
	SHADOWPTR	rptr;
	reset_shadower();
	while (( rptr = RegisterHeap) != (SHADOWPTR) 0) {
		RegisterHeap = rptr->next;
		liberate_for_type( rptr->type );
		liberate( rptr );
		}
	return;
}

SHADOWPTR	allocate_new_register()
{
	SHADOWPTR	sptr;
	trace("allocate_new_register()\n");
	if ((sptr = allocate_for_register()) != (SHADOWPTR) 0) {
		while ( 1 ) {
			if ( Register < MAX_REGISTERS ) {
				if (!( RegisterLock[ Register ] )) {
					sptr->number = Register++;
					Registers[sptr->number] = sptr;
					break;
					}
				else	Register++;
				}
			else	{
				(void) syntax_error( 233 );
				return((SHADOWPTR) 0);
				}
			}
		}
/*	sysprintf("allocated_new_register(%u)\n",sptr->number);	*/
	return( sptr );
}

VOID	release_base_register( sptr )
SHADOWPTR	sptr;
{
	trace("release_base_register()\n");
	sptr->next = RegisterUsed;
	RegisterUsed = sptr;
	BaseRegister--;
	return;
}

SHADOWPTR protect_base_register( sptr )
SHADOWPTR	sptr;
{
	SHADOWPTR	rptr;
	trace("protect_base_register()\n");
	if ((rptr = allocate_for_register()) != (SHADOWPTR) 0) {
		if ( duplicate_type( rptr->type, sptr->type ) != 0)
			return((SHADOWPTR) 0);
		RegisterUsed   = rptr->next;
		rptr->next     = (SHADOWPTR) 0;
		rptr->prev     = (SHADOWPTR) 0;
		RegisterUsed->prev = (SHADOWPTR) 0;
		rptr->number   = BaseRegister;
		rptr->nature   = sptr->nature;
		rptr->contents = sptr->contents;
		rptr->value    = sptr->value;
		Registers[BaseRegister++] = rptr;
		if ( sptr->number != rptr->number )
			generate_copy_register( rptr->number, sptr->number );
		}
	return( rptr );
}

SHADOWPTR	shadow_integer_register( sptr, value )
SHADOWPTR	sptr;
WORD		value;
{
	trace("shadow_integer_register()\n");
	if ( sptr != (SHADOWPTR) 0) {
		sptr->type->type = 1;
		sptr->type->size = 2;
		sptr->nature = _REGISTER_INT;
		sptr->value  = value;
		}
	return(sptr);
}

SHADOWPTR	shadow_constant_register( sptr, cptr )
SHADOWPTR	sptr;
TCCONPTR	cptr;
{
	trace("shadow_constant_register()\n");
	if ( sptr != (SHADOWPTR) 0) {
		if (( cptr->type->type & 3 ) > 1) {
			sptr->nature   = _REGISTER_CONST;
			if (duplicate_type( sptr->type, cptr->type ) != 0)
				return((SHADOWPTR) 0);
			sptr->contents = (VPTR)cptr;
			sptr->value    = cptr->identity;
			}
		else	return(shadow_integer_register( sptr, cptr->identity));
		}
	return(sptr);
}

TCVARPTR	register_variable( sptr )
SHADOWPTR	sptr;
{
	if (( sptr != (SHADOWPTR) 0)
	&&  ( sptr->nature == _REGISTER_VARB ))
		return( (TCVARPTR) sptr->contents );
	else	return( (TCVARPTR) 0 );
}

TCTYPPTR	register_alias( sptr )
SHADOWPTR	sptr;
{
	if (( sptr != (SHADOWPTR) 0)
	&&  ( sptr->nature == _REGISTER_ALIAS))
		return( sptr->type );
	else	return( (TCTYPPTR) 0 );
}

SHADOWPTR	shadow_alias_register( sptr, tptr )
SHADOWPTR	sptr;
TCTYPPTR	tptr;
{
	trace("shadow_alias_register()\n");
	if ( sptr != (SHADOWPTR) 0) {
		sptr->nature   = _REGISTER_ALIAS;
		if (duplicate_type( sptr->type, tptr ) != 0)
			return((SHADOWPTR) 0);
		sptr->contents = (VPTR) 0;
		sptr->value    = MOINS_UN;
		}
	return(sptr);
}

SHADOWPTR	shadow_variable_register( sptr, vptr )
SHADOWPTR	sptr;
TCVARPTR	vptr;
{
	trace("shadow_variable_register()\n");
	if ( sptr != (SHADOWPTR) 0) {
		sptr->nature   = _REGISTER_VARB;
		if (duplicate_type( sptr->type, vptr->type ) != 0)
			return((SHADOWPTR) 0);
		sptr->contents = (VPTR)vptr;
		sptr->value    = vptr->identity;
		}
	return(sptr);
}

WORD	is_shadow_integer( sptr )
SHADOWPTR	sptr;
{
	if (!( sptr->type ))
		return( FAUX );
	else if (( sptr->type->type & _TYPE_BASIC) < _TYPE_NUMERIC)
		return(VRAI);
	else if (( sptr->type->type & _TYPE_BASIC) == _TYPE_INT32 )
		return( VRAI );
	else	return(FAUX);
}

WORD	is_shadow_string( sptr )
SHADOWPTR	sptr;
{
	if (!( sptr->type ))
		return( FAUX );
	else if (( sptr->type->type & _TYPE_BASIC ) == _TYPE_STRING)
		return(VRAI);
	else	return(FAUX);
}

WORD	is_shadow_numeric( sptr )
SHADOWPTR	sptr;
{
	if (!( sptr->type ))
		return( FAUX );
	else if (( sptr->type->type & _TYPE_BASIC ) == _TYPE_NUMERIC)
		return(VRAI);
	else	return(FAUX);
}

WORD	register_type( sptr )
SHADOWPTR	sptr;
{
	if ( sptr != (SHADOWPTR) 0) {
		if (!( sptr->type ))
			return(MOINS_UN );
		else	return( sptr->type->type & 0x0017 );
		}
	else	return( MOINS_UN );
}

WORD	verify_register_type( sptr, type )
SHADOWPTR	sptr;
TCTYPPTR	type;
{
	if (( sptr != (SHADOWPTR) 0 )
	&&  ( sptr->type != (TCTYPPTR) 0))
		return( compare_types( sptr->type, type ) );
	else	return( FAUX );
}

SHADOWNODE	allocate_shadow_node()
{
	SHADOWNODE	xptr;
	if ((xptr = ShadowFree) != (SHADOWNODE) 0)
		ShadowFree = (SHADOWNODE)ShadowFree->lptr;
	else if ((xptr = (SHADOWNODE) allocate( sizeof( struct otr_shadow_node ) )) == (SHADOWNODE) 0)
		return( xptr );

	xptr->lptr   = (VPTR) 0;
	xptr->rptr   = (VPTR) 0;
	xptr->nature = MOINS_UN;

	return( xptr );
}

VOID		set_shadow_operands( sptr, operation, lptr, rptr )
SHADOWPTR	sptr;
WORD		operation;
VPTR	lptr;
VPTR	rptr;
{
	SHADOWNODE	xptr;

	if (( xptr = allocate_shadow_node()) != (SHADOWNODE) 0) {
		if ( sptr->operand != (SHADOWNODE) 0 )
			checkmoinsun((sptr->operand = release_shadow_list( sptr->operand )));
		xptr->lptr      = lptr;
		xptr->rptr      = rptr;
		xptr->nature    = operation;
		sptr->operand   = xptr;
		}
	return;
}	

VOID		set_shadow_operand( sptr, operation, lptr )
SHADOWPTR	sptr;
WORD		operation;
VPTR		lptr;
{
	SHADOWNODE	xptr;

	if (( xptr = allocate_shadow_node()) != (SHADOWNODE) 0) {
		if ( sptr->operand != (SHADOWNODE) 0 )
			checkmoinsun((sptr->operand = release_shadow_list( sptr->operand )));
		xptr->lptr      = lptr;
		xptr->nature    = operation;
		sptr->operand   = xptr;
		}
	return;
}	

VOID	set_shadow_variable( sptr, vptr )
SHADOWPTR	sptr;
VPTR		vptr;
{
/*	sysprintf("set_shadow_variable()\n");	*/
	if ((sptr != (SHADOWPTR) 0)
	&&  (vptr != (VPTR) 0))
		set_shadow_operand( sptr, (_SHADOW_RESULT | _REGISTER_VARB ), vptr );
	return;
}

VOID	set_shadow_register( sptr, tptr )
SHADOWPTR	sptr;
SHADOWPTR	tptr;
{
/*	sysprintf("set_shadow_register()\n");	*/

	if (sptr != (SHADOWPTR) 0) {
		if ( sptr->operand != (SHADOWNODE) 0 )
			checkmoinsun((sptr->operand = release_shadow_list( sptr->operand )));
		}

	if (tptr != (SHADOWPTR) 0) {
		checkmoinsun((sptr->operand = tptr->operand));
		tptr->operand = (SHADOWNODE) 0;
		}

	return;
}

VOID	set_shadow_constant( sptr, vptr )
SHADOWPTR	sptr;
VPTR		vptr;
{
/*	sysprintf("set_shadow_variable()\n");	*/
	if ((sptr != (SHADOWPTR) 0)
	&&  (vptr != (VPTR) 0))
		set_shadow_operand( sptr, (_SHADOW_RESULT | _REGISTER_CONST), vptr );
	return;
}

VOID		set_shadow_text( sptr )
SHADOWPTR	sptr;
{
	BPTR	tptr;

	if ((tptr = get_echo_text()) != (BPTR) 0) {
/*		sysprintf("EchoText(%s)\n",tptr);	*/
		set_shadow_operand( sptr, _SHADOW_TEXT, tptr );
		}
	return;
}

VOID		set_shadow_result(sptr,rptr )
SHADOWPTR	sptr;
VPTR     	rptr;
{
	set_shadow_operand( sptr, _SHADOW_RESULT, rptr );
	return;
}

VOID	set_shadow_increment( sptr )
SHADOWPTR	sptr;
{
	SHADOWNODE	xptr;
	if (( xptr = allocate_shadow_node()) != (SHADOWNODE) 0) {
		if ((xptr->lptr = (VPTR)sptr->operand) != (VPTR) 0)
			xptr->nature = _SHADOW_INC;
		else if ((xptr->lptr = sptr->contents) != (VPTR) 0)
			xptr->nature = (_SHADOW_INC | _SHADOW_RESULT | sptr->nature );
		else	xptr->nature = MOINS_UN;
		checkmoinsun((sptr->operand = xptr));
		}
	return;


}

VOID	set_shadow_decrement( sptr )
SHADOWPTR	sptr;
{
	SHADOWNODE	xptr;
	if (( xptr = allocate_shadow_node()) != (SHADOWNODE) 0) {
		if ((xptr->lptr = (VPTR)sptr->operand) != (VPTR) 0)
			xptr->nature = _SHADOW_DEC;
		else if ((xptr->lptr = sptr->contents) != (VPTR) 0)
			xptr->nature = (_SHADOW_DEC | _SHADOW_RESULT | sptr->nature );
		else	xptr->nature = MOINS_UN;
		checkmoinsun((	sptr->operand = xptr ));
		}
	return;


}
VOID	set_shadow_negation( sptr )
SHADOWPTR	sptr;
{
	SHADOWNODE	xptr;
	if (( xptr = allocate_shadow_node()) != (SHADOWNODE) 0) {
		if ((xptr->lptr = (VPTR)sptr->operand) != (VPTR) 0)
			xptr->nature = _SHADOW_NEG;
		else if ((xptr->lptr = sptr->contents) != (VPTR) 0)
			xptr->nature = (_SHADOW_NEG | _SHADOW_RESULT | sptr->nature );
		else	xptr->nature = MOINS_UN;
		checkmoinsun((sptr->operand = xptr));
		}
	return;


}


VOID		set_shadow_conv( sptr, type )
SHADOWPTR	sptr;
WORD		type;
{
	SHADOWNODE	xptr;
	if (( xptr = allocate_shadow_node()) != (SHADOWNODE) 0) {
		if ((xptr->lptr = (VPTR)sptr->operand) != (VPTR) 0)
			xptr->nature = (_SHADOW_CONV | type);
		else if ((xptr->lptr = sptr->contents) != (VPTR) 0)
			xptr->nature = (_SHADOW_CONV | _SHADOW_RESULT | type);
		else	xptr->nature = MOINS_UN;
		checkmoinsun((	sptr->operand = xptr ));
		}
	return;
}


SHADOWPTR	set_shadow_type( sptr, type )
SHADOWPTR	sptr;
WORD		type;
{
	trace("set_shadow_type()\n");
	if ( sptr != (SHADOWPTR) 0) {
		sptr->type->type = type;
		sptr->nature     = _REGISTER_EXPR;
		}
	return( sptr );
}

WORD	push_live_registers()
{
	WORD		r;
	SHADOWPTR	sptr;
	trace("DEB push_live_registers()\n");
	for ( r = 1; r < Register; r++ ) {
		if ((sptr = Registers[r]) != (SHADOWPTR) 0) {
			if ( sptr->status == _REGISTER_LIVE ) {
				generate_push( sptr->number );
				sptr->status = _REGISTER_PUSH;
				}
			}
		}
	trace("FIN push_live()\n");
	return(0);
}

WORD	pop_live_registers()
{
	WORD		r;
	SHADOWPTR	sptr;
	trace("DEB push_live_registers()\n");
	for ( r = Register; r > 1; r-- ) {
		if (((sptr = Registers[r]) != (SHADOWPTR) 0)
		&&  ( sptr->status == _REGISTER_PUSH )) {
			generate_pop( sptr->number );
			sptr->status = _REGISTER_LIVE;
			}
		}
	trace("FIN push_live()\n");
	return(0);
}

WORD	pop_live_register( sptr )
SHADOWPTR	sptr;
{
	trace("pop_live_register()\n");
	if ((sptr != (SHADOWPTR) 0)
	&&  (sptr->status == _REGISTER_PUSH)) {
		generate_pop( sptr->number );
		sptr->status = _REGISTER_LIVE;
		}
	return(0);
}

WORD	mark_live_register( sptr )
SHADOWPTR	sptr;
{
	trace("mark_live_register()\n");
	if (sptr != (SHADOWPTR) 0) {
		pop_live_register( sptr );
		sptr->status = _REGISTER_LIVE;
		}
	else	trace("invalid register ptr ! \n");
	return(0);
}

WORD	mark_dead_register( sptr )
SHADOWPTR	sptr;
{
	WORD	r;
/*	sysprintf("mark_dead_register(");	*/
	if (sptr != (SHADOWPTR) 0) {
/*		sysprintf(" %u )\n",sptr->number);	*/
		pop_live_register(sptr);
		sptr->status = _REGISTER_DEAD;
		r = sptr->number;
		if ((sptr = Registers[r]) != (SHADOWPTR) 0)
			sptr->status = _REGISTER_DEAD;
		}
/*	else	sysprintf("\t * * invalid register ptr * * )\n");	*/
	return(0);
}

WORD	mark_live_noregister( nreg )
WORD	nreg;
{
	SHADOWPTR	sptr;
	trace("mark_live_noregister()\n");
	if ((sptr = Registers[nreg]) != (SHADOWPTR) 0)
		mark_live_register( sptr );
	else	trace("invalid register ptr ! \n");
	return(0);
}

WORD	mark_dead_noregister( nreg )
WORD	nreg;
{
	SHADOWPTR	sptr;
	trace("mark_dead_noregister()\n");
	if ((sptr = Registers[nreg]) != (SHADOWPTR) 0)
		mark_dead_register( sptr );
	else	trace("invalid register ptr ! \n");
	return(0);
}

WORD	lock_used_registers()
{
	WORD	current=BaseRegister;
	trace("lock_used_registers()\n");
	BaseRegister = Register;
	return( current );
}

VOID    unlock_used_registers( previous )
WORD	previous;
{
	trace("unlock_used_registers()\n");
	Register     = BaseRegister;
	BaseRegister = previous;
	return;
}



#endif	/* _TRSHADOW_C */
	/* ----------- */

