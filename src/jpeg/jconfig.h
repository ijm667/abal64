/* jconfig.h --- generated by ckconfig.c */
/* see jconfig.doc for explanations */

#define NO_GETENV
#define HAVE_PROTOTYPES
#define HAVE_UNSIGNED_CHAR
#define HAVE_UNSIGNED_SHORT
/* #define void char */
/* #define const */
#undef CHAR_IS_UNSIGNED
#undef HAVE_STDDEF_H
#undef HAVE_STDLIB_H
#undef NEED_BSD_STRINGS
#undef NEED_SYS_TYPES_H
#undef NEED_FAR_POINTERS
#undef NEED_SHORT_EXTERNAL_NAMES
#undef INCOMPLETE_TYPES_BROKEN

#ifdef JPEG_INTERNALS

#undef RIGHT_SHIFT_IS_UNSIGNED

#endif /* JPEG_INTERNALS */

#ifdef JPEG_CJPEG_DJPEG

#define BMP_SUPPORTED		/* BMP image file format */
#define GIF_SUPPORTED		/* GIF image file format */
#define PPM_SUPPORTED		/* PBMPLUS PPM/PGM image file format */
#undef RLE_SUPPORTED		/* Utah RLE image file format */
#define TARGA_SUPPORTED		/* Targa image file format */

#undef TWO_FILE_COMMANDLINE	/* You may need this on non-Unix systems */
#undef NEED_SIGNAL_CATCHER	/* Define this if you use jmemname.c */
#undef DONT_USE_B_MODE
/* #define PROGRESS_REPORT */	/* optional */
#define MAX_ALLOC_CHUNK 32000L
#define ALIGN_TYPE char

#endif /* JPEG_CJPEG_DJPEG */

