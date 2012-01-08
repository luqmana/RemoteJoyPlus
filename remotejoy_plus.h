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
 * Some common stuff used in the program.
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
	
	struct ScreenBuffer scr_buffers[2];
	int scr_width, scr_height;
	int psp_flags;
	int exit_flag;
	int scr_on;
	unsigned int button_state;

};

/**
 * Yes! A global struct holding ALL THE INFO!
 * Let's hope we have no race conditions...
 */
extern struct GlobalContext g_context;

#endif