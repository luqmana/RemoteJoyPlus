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
  * Defines common interfaces for output extensions.
  */

#ifndef __OutputExts_H_
#define __OutputExts_H_

#include <SDL.h>

struct output_ext {
	
	char *name;
	int id;
	void (*setup)(struct output_ext *);
	void (*render)(struct output_ext *, SDL_Surface);
	void (*cleanup)(struct output_ext *);

};

void register_output_ext(char* name, void (*oe_register_func)(struct output_ext *), struct output_ext *oe);

#endif // __OutputExts_H_