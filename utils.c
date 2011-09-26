/**
 * RemoteJoy+
 * 
 * This is based on RemoteJoy by James F. (TyRaNiD).
 * It also exports the PSP screen over WebSockets
 * to be accessible to web clients.
 *
 * Licensed under the BSD license.
 * 
 * Copyright (c) 2006 James F. <tyranid@gmail.com>
 * Copyright (c) 2011 Luqman A. <laden@csclub.uwaterloo.ca>
 */

 /**
  * Some utils
  */

#include "remotejoy_plus.h"
#include "utils.h"

#include <ctype.h>
#include <string.h>

#if defined BUILD_BIGENDIAN || defined _BIG_ENDIAN

uint16_t swap16(uint16_t i) {
	
	uint8_t* p = (uint8_t*) &i;
	uint16_t ret;
	
	ret = (p[1] << 8) | p[0];
	
	return ret;

}

uint32_t swap32(uint32_t i) {
	
	uint8_t* p = (uint8_t*) &i;
	uint32_t ret;
	
	ret = (p[3] << 24) | (p[2] << 16) | (p[1] << 8) | p[0];
	
	return ret;

}

uint64_t swap64(uint64_t i) {
	
	uint8_t* p = (uint8_t*) &i;
	uint64_t ret;
	
	ret = (uint64_t) p[0] | ((uint64_t) p[1] << 8) | ((uint64_t) p[2] << 16) | ((uint64_t) p[3] << 24) 
		| ((uint64_t) p[4] << 32) | ((uint64_t) p[5] << 40) | ((uint64_t) p[6] << 48) | ((uint64_t) p[7] << 56);
	
	return ret;

}

#endif // defined BUILD_BIGENDIAN || defined _BIG_ENDIAN

void remove_wsp(char* buf) {
	
	int len = strlen(buf);
	int i = 0;

	while (isspace(buf[i]))
		i++;

	if (i > 0) {
		
		len -= i;
		memmove(buf, &buf[i], len + 1);

	}

	if (len <= 0)
		return;

	i = len - 1;

	while(isspace(buf[i]))
		buf[i--] = 0;

}

SDL_Surface* create_surface(void *buf, int mode) {
	
	unsigned int rmask, bmask, gmask, amask;
	int currw, currh;
	int bpp;

	currw = g_context.scr_width;
	currh = g_context.scr_height;

	switch (mode) {
		
		case 3:

			rmask = LE32(0x000000FF);
			gmask = LE32(0x0000FF00);
			bmask = LE32(0x00FF0000);
			amask = 0;
			bpp = 32;

			break;

		case 2:

			rmask = LE16(0x000F);
			gmask = LE16(0x00F0);
			bmask = LE16(0x0F00);
			amask = 0;
			bpp = 16;

			break;

		case 1:
			
			rmask = LE16(0x1F);
			gmask = LE16(0x1F << 5);
			bmask = LE16(0x1F << 10);
			amask = 0;
			bpp = 16;

			break;

		case 0:
			
			rmask = LE16(0x1F);
			gmask = LE16(0x3F << 5);
			bmask = LE16(0x1F << 11);
			amask = 0;
			bpp = 16;

			break;

		default:

			return NULL;
			
	}

	return SDL_CreateRGBSurfaceFrom(buf, currw, currh, 
									bpp, currw * (bpp / 8),
									rmask, gmask,
									bmask, amask);

}