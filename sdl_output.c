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

#include "remotejoy_plus.h"
#include "sdl_output.h"

SDL_Surface* screen = NULL;

void sdl_output_setup(struct output_ext *oe) {
	
	printf("Hi! From %s [id: %d]. \n", oe->name, oe->id);

	screen = SDL_SetVideoMode(g_context.scr_width, g_context.scr_height, 0, SDL_HWSURFACE);

	if (screen == NULL) {

		fprintf(stderr, "SDL Output: Unable to create screen surface [%s].\n", SDL_GetError());
		return;

	}

	SDL_ShowCursor(SDL_DISABLE);

}

void sdl_output_handle_event(struct output_ext *oe, SDL_Event event) {
	
	if (screen == NULL)
		return;

}

void sdl_output_render(struct output_ext *oe, SDL_Surface surf) {
	
	if (screen == NULL)
		return;

	SDL_Surface* s = &surf;

	SDL_BlitSurface(s, NULL, screen, NULL);
	SDL_UpdateRect(screen, 0, 0, g_context.scr_width, g_context.scr_height);

}

void sdl_output_cleanup(struct output_ext *oe) {
	


}

void sdl_output(struct output_ext *oe) {
	
	oe->setup = &sdl_output_setup;
	oe->handle_event = &sdl_output_handle_event;
	oe->render = &sdl_output_render;
	oe->cleanup = &sdl_output_cleanup;

}