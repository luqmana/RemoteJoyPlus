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
  * Main entry point.
  * Prepares everything.
  */

#include <SDL.h>
#include <stdio.h>

#include "fast_events.h"
#include "output_exts.h"
#include "rj_reader.h"
#include "sdl_output.h"
#include "ws_output.h"

int main(int argc, char **argv) {
 	
	printf("RemoteJoy+ for PSP (c) 2011 Luqman A.\n");
 	printf("Built: %s %s\n", __DATE__, __TIME__);

 	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTTHREAD) != 0) {
 		
 		fprintf(stderr, "Could not initialise SDL [%s].\n", SDL_GetError());
 		return 1;

 	}

 	if (FE_Init() != 0) {
 		
 		fprintf(stderr, "Could not initialize SDL fast events system [%s].\n", FE_GetError());
 		return 1;

 	}

 	if (rj_reader_init() != 0) {
 		
 		fprintf(stderr, "Could not initiate remotejoy reader system.\n");
 		return 1;

 	}

 	// Our list of outputs
 	struct output_ext oe_list[2];

 	// Register some output now
 	register_output_ext("SDL Output", &sdl_output, &oe_list[0]);
 	register_output_ext("WebSocket Output", &ws_output, &oe_list[1]);

 	// Let the outputs setup what they need to
 	for (int r = 0; r < 2; r++)
 		oe_list[r].setup(&oe_list[r]);

 	rj_reader_start();

 	// Let the outputs cleanup
 	for (int r = 0; r < 2; r++)
 		oe_list[r].cleanup(&oe_list[r]);

 	FE_Quit();
 	SDL_Quit();

	return 0;

}