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
  * Outputs to a browser via WebSockets
  */

#include "ws_output.h"

void ws_output_setup(struct output_ext *oe) {
	
	printf("Hi! From %s [%d]. \n", oe->name, oe->id);

}

void ws_output_render(struct output_ext *oe, SDL_Surface surf) {
	


}

void ws_output_cleanup(struct output_ext *oe) {
	


}

void ws_output(struct output_ext *oe) {
	
	oe->setup = &ws_output_setup;
	oe->render = &ws_output_render;
	oe->cleanup = &ws_output_cleanup;

}