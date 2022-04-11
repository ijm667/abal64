static int code_lang = 0;	/* code langue				*/

/*
 *	tables de conversion minuscule/majuscule
 *	----------------------------------------
 */

static BYTE maj_to_min[] = {
	0x87, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 	/* 0x80 */
	0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x84, 0x86,
	0x82, 0x91, 0x91, 0x93, 0x94, 0x95, 0x96, 0x97, 	/* 0x90 */
	0x98, 0x94, 0x81, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
	0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa4, 0xa6, 0xa7, 	/* 0xa0 */
	0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
	0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0x85, 0xa0, 0x83, 	/* 0xb0 */
	0xce, 0x8a, 0x88, 0x89, 0x8d, 0xa1, 0x8c, 0xbf,
	0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0x8b, 0x95, 	/* 0xc0 */
	0xa2, 0x93, 0xcf, 0x97, 0xa3, 0x96, 0xce, 0xcf,
};

static BYTE min_to_maj[][2] = {
	0x80, 0x80, 0x9a, 'U',  0x90, 'E',  0xb7, 'A',          /* 0x80 */
	0x8e, 'A',  0xb5, 'A',  0x8f, 'A',  0x80, 'C',
	0xba, 'E',  0xbb, 'E',  0xb9, 'E',  0xc6, 'I',
	0xbe, 'I',  0xbc, 'I',  0x8e, 0x8e, 0x8f, 0x8f,
	0x90, 0x90, 0x92, 0x92, 0x92, 0x92, 0xc9, 'O',          /* 0x90 */
	0x99, 'O',  0xc7, 'O',  0xcd, 'U',  0xcb, 'U',
	'Y',  'Y',  0x99, 0x99, 0x9a, 0x9a, 0x9b, 0x9b,
	0x9c, 0x9c, 0x9d, 0x9d, 0x9e, 0x9e, 0x9f, 0x9f,
	0xb6, 'A',  0xbd, 'I',  0xc8, 'O',  0xcc, 'U',          /* 0xa0 */
	0xa5, 'N',  0xa5, 0xa5, 'A',  'A',  'O',  'O',
	0xa8, 0xa8, 0xa9, 0xa9, 0xaa, 0xaa, 0xab, 0xab,
	0xac, 0xac, 0xad, 0xad, 0xae, 0xae, 0xaf, 0xaf,
	0xb0, 0xb0, 0xb1, 0xb1, 0xb2, 0xb2, 0xb3, 0xb3, 	/* 0xb0 */
	0xb4, 0xb4, 0xb5, 0xb5, 0xb6, 0xb6, 0xb7, 0xb7,
	0xb8, 0xb8, 0xb9, 0xb9, 0xba, 0xba, 0xbb, 0xbb,
	0xbc, 0xbc, 0xbd, 0xbd, 0xbe, 0xbe, 0xbf, 0xbf,
	0xc0, 0xc0, 0xc1, 0xc1, 0xc2, 0xc2, 0xc3, 0xc3, 	/* 0xc0 */
	0xc4, 0xc4, 0xc5, 0xc5, 0xc6, 0xc6, 0xc7, 0xc7,
	0xc8, 0xc8, 0xc9, 0xc9, 0xca, 0xca, 0xcb, 0xcb,
	0xcc, 0xcc, 0xcd, 0xcd, 0xb8, 'A',  0xca, 'O',
};


WORD	get_language_code()	{	return( code_lang );	}
VOID	set_language_code(int v)
{
	code_lang=v;		
}

WORD	ix_case_conversion( int c, int conv )
{
	c &= 0xff;
	if (conv == UPPER_CASE) {
		if (c >= 'a' && c <= 'z')
			c -= 0x20;
		else if (c >= 0x80 && c <= 0xcf)
			c = min_to_maj[c - 0x80][(code_lang > 1) ? 0 : 1];
	} else if (conv == LOWER_CASE) {
		if (c >= 'A' && c <= 'Z')
			c += 0x20;
		else if (c >= 0x80 && c <= 0xcf)
			c = maj_to_min[c - 0x80];
	}
	return (c);
}

