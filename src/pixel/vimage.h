#ifndef	_vimage_h
#define	_vimage_h

#define	_DEFAULT_SCALE_FACTOR	10
#define	_DEFAULT_ROUND_FACTOR	0

#include "stdpsu.h"

struct	standard_image {
	int		rows;
	int		columns;
	STDPSUPTR	storage;
	int 		users;
	char *		palette;
	int		format;
	char *		name;
	};

struct standard_image * image_loader( char * );
struct standard_image * resolve_image(char *,int);
struct standard_image * resolve_image_by_number(int);
char 		      * resolve_imagename_by_number(int);
struct standard_image * drop_image( struct standard_image * );
			
#endif	/* _vimage_h */



