#ifndef	_TRTABLES_H
#define _TRTABLES_H


#ifndef	UNIX
WORD		not_legal_digit( BPTR, WORD );
WORD		copy_token( BPTR, WORD );
WORD		atobcd( BPTR, WORD, BPTR, WORD );
HUGE		LongProduct( WORD, WORD, WORD );
LONG		ShortMul(  WORD, WORD );
LONG		LongMul( LONG, WORD );
LONG		FieldIndent( TCTYPPTR, WORD, WORD );
WORD		field_equals_m( VOID );
WORD		field_equals_file( WORD );
WORD		field_equals_f( VOID );
WORD		field_equals_g( VOID );
WORD		field_equals_v( TCVARPTR, LONG );
WORD		field_equals_l( VOID );
VOID		drop_global_labels( VOID );
WORD		check_data_label( BPTR );
WORD		endloc_options( WORD );
WORD		copy_field( FIELDPTR, FIELDPTR );
WORD		common_field( FIELDPTR, FIELDPTR );
FIELDNODEPTR	allocate_for_field_node( VOID );
WORD		field_equals_s( VOID );
TCVDTPTR	allocate_for_vdt( VOID );
WORD		liberate_external_field( XTFIELDPTR );
XTLOCALPTR	allocate_external_local( VOID );
WORD		add_external_local( XTFIELDPTR );
WORD		add_external_field( WORD, WORD, BPTR, LONG);
WORD		field_equals_e( VOID );
WORD		field_equals_r( VOID );
WORD		release_fields( VOID );
WORD		tcode_program_identity( VOID );
WORD		initialise_field( FIELDPTR );
WORD		set_global_scope( VOID );
WORD		set_local_scope( VOID );
WORD		build_token( BPTR, BPTR );
WORD		copy_token( BPTR, WORD );
LONG		collect_field_indent( TCTYPPTR );
WORD		redefine_base_variable( TCVARPTR );
TCVARPTR	variable_symbol( BPTR );
WORD		redefine_variable( VOID );
WORD		dcl_filler( WORD );
TCCONPTR	allocate_for_constant( VOID );
#ifndef	otr32
TCCONPTR	redirect_local_constant( TCCONPTR );
#endif
WORD		add_to_variable_table( TCVDTPTR, TCVARPTR, WORD );
TCBLKPTR	allocate_for_block( VOID );
VOID		liberate_for_segment( TCSEGPTR );
TCSEGPTR	allocate_for_segment( VOID );
TCTYPPTR	allocate_for_type( WORD, WORD, WORD, WORD, VPTR );
VOID		liberate_for_type( TCTYPPTR );
WORD		resolve_real_host( TCVDTPTR, WORD );
WORD		resolve_real_identity( TCVARPTR );
WORD		mark_variable( TCVARPTR );
TCVARPTR	allocate_for_variable( VOID );
VOID		release_variables( VOID );
TCVARPTR	lock_used_variables( VOID );
VOID		unlock_used_variables( TCVARPTR );
TCVARPTR	temp_variable( TCVARPTR );
VOID		initialise_variables( VOID);
VOID		liberate_variables( VOID );
TCTYPPTR	copy_type( TCTYPPTR );
TCVARPTR	protect_variable( TCVARPTR );
TCVARPTR	copy_variable( TCVARPTR );
WORD		trdtoi( BPTR );
WORD		trhtoi( BPTR );
WORD		tratoi( BPTR );
WORD		store_to_constant( TCCONPTR, BPTR );
WORD		add_data_item( BPTR );
VOID		calculate_constant_hash( TCCONPTR );
WORD		compare_constant_value(TCCONPTR, TCCONPTR);
WORD		alias_constant( TCCONPTR );
WORD		add_to_constant_list( TCCONPTR );
WORD		add_constant( BPTR, WORD, BPTR );
TCCONPTR	build_explicite_constant( WORD, BPTR );
TCCONPTR	build_implicite_constant( WORD );
WORD		add_external_variable( XTFIELDPTR, TCVARPTR );
WORD		declare_variable( TCVARPTR );
TCVARPTR	next_master_object( TCVARPTR, WORD );
TCVARPTR	locate_master_object( WORD );
VOID		update_field_information( TCVARPTR );
WORD		variable_nature( TCVARPTR );
WORD		dcl_variable( BPTR, WORD, WORD, WORD, WORD, WORD, WORD, WORD, WORD );
TCVARPTR	dcl_object_alias( TCTYPPTR, WORD );
WORD		add_variable( BPTR, WORD, WORD, WORD, WORD, WORD, WORD, WORD );
VOID		liberate_vdt( TCVDTPTR );
VOID		flush_vdt_members( TCVDTPTR, WORD );
WORD		flush_local_data( VOID );
WORD		flush_variable_rellocation( VOID );
WORD		flush_global_labels( VOID );
WORD		flush_global_data( VOID );
WORD		flush_constant_rellocation( VOID );
WORD		flush_library_rellocation( VOID );
VOID		liberate_for_constant( TCCONPTR );
WORD		flush_global_const( VOID );
WORD		flush_local_const( VOID );
TCMODPTR	locate_module( BPTR );
WORD		resolve_module_number( BPTR, WORD);
TCPRCPTR	add_procedure( TCPRCPTR, WORD, WORD );
WORD		establish_field_offset( TCVARPTR );
WORD		declare_procedure_parameters( TCVARPTR );
WORD		reset_data_control( VOID );
VOID		flush_data_control( VOID );
WORD		start_procedure( TCPRCPTR, WORD );
TCSEGPTR	locate_internal_segment( BPTR );
XTSEGPTR	locate_external_segment( BPTR );
VOID		convert_external_segment( XTSEGPTR, WORD );
TCSEGPTR	build_segment( WORD, BPTR, WORD );
WORD		start_segment( TCSEGPTR, WORD, WORD );
WORD		add_segment( WORD, WORD, BPTR, WORD );
WORD		implicite_segment_zero( VOID );
RELOCPTR	allocate_for_rellocation( VOID );
WORD		add_constant_rellocation( TCCONPTR, WORD, WORD );
WORD		add_variable_rellocation( TCVARPTR, WORD, WORD );
WORD		add_library_rellocation( TCDLBPTR, WORD, WORD );
XREFPTR	build_cross_reference( WORD, WORD, WORD, WORD );
WORD		add_procedure_crossref( WORD, WORD, WORD, WORD );
WORD		add_segment_crossref( WORD, WORD, WORD, WORD );
WORD		rellocate_external_segment( XTSEGPTR );
WORD		rellocate_external_procedure( TCPRCPTR );
WORD		resolve_external_segment( BPTR );
WORD		resolve_internal_segment( BPTR );
XTSEGPTR	build_external_segment( BPTR, WORD, BPTR, WORD, WORD );
WORD		add_external_segment( BPTR, WORD, BPTR, WORD, WORD );
WORD		explicite_named_segment( BPTR );
VOID		update_external_locals( WORD, WORD );
WORD		translate_stop( VOID );
WORD		segmental_closure( WORD );
WORD		terminate_segment( WORD, BPTR, WORD );
WORD		procedure_closure( TCPRCPTR );
WORD		start_data_control( VOID );
WORD		terminate_procedure( VOID  );
WORD		flush_libraries( VOID );
WORD		flush_procedures( VOID );
VOID		liberate_external_segment( XTSEGPTR );
WORD		flush_external_variables( VOID );
WORD		count_used_field_members( XTFIELDPTR );
WORD		flush_external_field_members( XTFIELDPTR );
WORD		flush_external_fields( VOID );
WORD		flush_external_locals( VOID );
WORD		flush_external_procedures( VOID );
WORD		flush_external_segments( VOID );
WORD		flush_segments( VOID );
#ifdef ABAL33A
WORD		flush_library_modules( VOID );
#endif
WORD		flush_modules( VOID );
BPTR		context_filename( VOID );
WORD		start_tcode( VOID );
WORD		tcode_garbage_collector( VOID );
WORD		terminate_tcode( VOID );
VOID		set_operation_pointer(VOID * );
VOID		pop_operation_pointer( VOID );
WORD		push_operation_pointer( VOID );
WORD		previous_operation_pointer( VOID );
WORD		flush_operation_pointers( VOID );
WORD		initialise_tcode_context( BPTR, BPTR );
WORD		liberate_tcode_context( VOID );
WORD		terminate_tcode_context( VOID );
WORD		terminate_tcode_production( VOID );
VOID		panic_message( BPTR, BPTR, WORD );
WORD		initialise_source_translation( BPTR );
WORD		initialise_remote_translation( BPTR );
VOID		implicite_return( VOID );
WORD		generate_named_ldgoseg( BPTR );
WORD		event_call_procedure( TCPRCPTR, WORD );
WORD		event_call_variable( TCVARPTR, WORD );

#else
HUGE		LongProduct();
TCTYPPTR	copy_type();
TCVARPTR	lock_used_variables();
VOID		unlock_used_variables();
TCVARPTR	copy_variable();
VOID		set_operation_pointer();
#ifndef	otr32
TCCONPTR	redirect_local_constant();
#endif
#endif

#endif	/* _TRTABLES_H */
	/* ----------- */


