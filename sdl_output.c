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
  * Outputs to the screen via SDL
  */

#include "sdl_output.h"

void sdl_output_setup(struct output_ext *oe) {
	
	printf("Hi! From %s [%d]. \n", oe->name, oe->id);

}

void sdl_output_render(struct output_ext *oe, SDL_Surface surf) {
	


}

void sdl_output_cleanup(struct output_ext *oe) {
	


}

void sdl_output(struct output_ext *oe) {
	
	oe->setup = &sdl_output_setup;
	oe->render = &sdl_output_render;
	oe->cleanup = &sdl_output_cleanup;

}