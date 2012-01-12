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
  * Defines common interfaces for in/output client extensions.
  */

#ifndef __ClientExts_H_
#define __ClientExts_H_

#include "remotejoy_plus.h"

struct client_ext {
	
	char *name;
	int id;
	void (*setup)(struct client_ext *, int, char **);
	void (*handle_event)(struct client_ext *, SDL_Event);
	void (*render)(struct client_ext *, struct ScreenBuffer *);
	void (*cleanup)(struct client_ext *);

};

void register_client_ext(char* name, void (*ce_register_func)(struct client_ext *), struct client_ext *ce);

void forward_button_input(int type, unsigned int keymap);

#endif // __ClientExts_H_
