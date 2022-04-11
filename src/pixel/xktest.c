#include <stdio.h>
#include <X11/keysymdef.h>
#include <X11/keysym.h>

unsigned char macrovalue[256] = {

	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
	0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
	0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
	0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
	0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
	0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
	0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
	0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
	0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
	0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0xB0,

	' ', ' ', 0x2C, 0x9F, 0x22, 0xF7, 0xC5, 0xD8,
	0x5E, 0x25, 0x53, 0x3C, 0x99, ' ', ' ', ' ',

	' ', 0x60, 0x27, 0x22, 0x22, 0xF8, 0x2D, 0xC4,
	0x7E, 0x54, 0x73, 0x3E, 0x99, ' ', ' ', 0x98,

	' ', 0xAD, 0x9B, 0x9C, 0x24, 0x9D, 0x7C, 0xD2,
	0x22, 0x03, 0x46, 0xAE, 0x21, 0xF7, 0x12, 0x4D,
	0xF8, 0xF1, 0xFD, 0x33, 0x2F, 0xE6, 0xD2, 0xF9,
	0xF5, 0x31, 0x4D, 0xAF, 0xAC, 0xAB, 0xF0, 0xA8,
	0x41, 0x41, 0x41, 0x41, 0x8E, 0x8F, 0x92, 0x80,
	0x45, 0x90, 0x45, 0x45, 0x49, 0x49, 0x49, 0x49,
	0xE4, 0xA5, 0x4F, 0x4F, 0x4F, 0x4F, 0x99, 0x78,
	0xE9, 0x55, 0x55, 0x55, 0x9A, 0x59, 0xE2, 0xE1,
	0x85, 0xA0, 0x83, 0x61, 0x84, 0x86, 0x91, 0x87,
	0x8A, 0x82, 0x88, 0x89, 0x8D, 0xA1, 0x8C, 0x8B,
	0xE4, 0xA4, 0x95, 0xA2, 0x93, 0x6F, 0x94, 0xF6,
	0xED, 0x97, 0xA3, 0x96, 0x81, 0x79, 0xE2, 0x98
	};


int	main()
{
	printf("Ntilde : D1 = %x ->: %x \n",XK_Ntilde,macrovalue[XK_Ntilde]);
	printf("ntilde : F1 = %x ->: %x \n",XK_ntilde,macrovalue[XK_ntilde]);

	return(0);
}

