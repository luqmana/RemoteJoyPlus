/**
 * RemoteJoy+
 * 
 * This is based on RemoteJoy by James F. (TyRaNiD).
 *
 * It provides an easy framework which allows building
 * client extension which can connect to the PSP receiving
 * the output and forwarding input even simultaneously.
 *
 * Licensed under the BSD license.
 * 
 * Copyright (c) 2006 James F. <tyranid@gmail.com>
 * Copyright (c) 2011 Luqman A. <laden@csclub.uwaterloo.ca>
 */

 /**
  * Some utils
  */

#ifndef __Utils_H__
#define __Utils_H__

#include <SDL.h>

#if defined BUILD_BIGENDIAN || defined _BIG_ENDIAN

uint16_t swap16(uint16_t i);
uint32_t swap32(uint32_t i);
uint64_t swap64(uint64_t i);

#define LE16(x) swap16(x)
#define LE32(x) swap32(x)
#define LE64(x) swap64(x)

#else

#define LE16(x) (x)
#define LE32(x) (x)
#define LE64(x) (x)

#endif // defined BUILD_BIGENDIAN || defined _BIG_ENDIAN

void remove_wsp(char* buf);

SDL_Surface* create_surface(void *buf, int mode);

#endif // __Utils_H__ //