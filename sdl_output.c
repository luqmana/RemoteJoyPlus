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

#include "fast_events.h"
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

	if (event.type == SDL_VIDEORESIZE) {

		SDL_FreeSurface(screen);
		screen = SDL_SetVideoMode(event.resize.w, event.resize.h, 0, SDL_HWSURFACE);
		g_context.scr_width = event.resize.w;
		g_context.scr_height = event.resize.h;

	} else if ((event.type == SDL_KEYDOWN) || (event.type == SDL_KEYUP)) {
		
		SDL_KeyboardEvent *key = (SDL_KeyboardEvent *) &event;

		if (key->keysym.sym == SDLK_ESCAPE) {
			
			SDL_Event event;

			event.type = SDL_QUIT;
			event.user.code = 0;
			event.user.data1 = event.user.data2 = NULL;

			FE_PushEvent(&event);

		} else if (key->keysym.sym == SDLK_F8) {
			
			if (event.type == SDL_KEYDOWN)
				SDL_WM_ToggleFullScreen(screen);

		} else if (key->keysym.sym == SDLK_F5) {
			
			if (event.type == SDL_KEYDOWN) {
				
				if (g_context.scr_on) {
					
					rj_send_event(TYPE_SCREEN_CMD, 0);
					g_context.scr_on = 0;

				} else {
					
					rj_send_event(TYPE_SCREEN_CMD, SCREEN_CMD_ACTIVE | g_context.psp_flags);
					g_context.scr_on = 1;

				}

			}

		}

	}

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