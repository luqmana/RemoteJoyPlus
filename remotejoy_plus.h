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

#ifndef __RemoteJoyPlus_H_
#define __RemoteJoyPlus_H_

#include "rj_interface.h"

#include <SDL.h>

#define PSP_SCREEN_W 				480
#define PSP_SCREEN_H 				272

struct ScreenBuffer {

	unsigned char buf[PSP_SCREEN_W * PSP_SCREEN_H * 4];
	struct JoyScrHeader head;

};

struct GlobalContext {
	
	SDL_Surface* sur_buffers[2];
	struct ScreenBuffer scr_buffers[2];
	int scr_mode[2];
	int scr_width, scr_height;
	int psp_flags;
	int exit_flag;
	int scr_on;

};

extern struct GlobalContext g_context;

#endif