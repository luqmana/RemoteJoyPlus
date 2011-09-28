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
  * Outputs to the screen via SDL.
  */

#include "fast_events.h"
#include "remotejoy_plus.h"
#include "sdl_client.h"

SDL_Surface* screen = NULL;

void sdl_client_setup(struct client_ext *ce) {
	
	printf("Hi! From %s [id: %d]. \n", ce->name, ce->id);

	if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
		
		fprintf(stderr, "SDL Client: Unable to initiate video sub system [%s].\n", SDL_GetError());
		return;

	}

	screen = SDL_SetVideoMode(g_context.scr_width, g_context.scr_height, 0, SDL_HWSURFACE);

	if (screen == NULL) {

		fprintf(stderr, "SDL Client: Unable to create screen surface [%s].\n", SDL_GetError());
		return;

	}

	//SDL_ShowCursor(SDL_DISABLE);

}

void sdl_client_handle_event(struct client_ext *ce, SDL_Event event) {
	
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
				
				SDL_Event kevent;
				kevent.type = SDL_USEREVENT;
				kevent.user.data1 = NULL;
				kevent.user.data2 = NULL;

				if (g_context.scr_on) {

					kevent.user.code = EVENT_DISABLE_SCREEN;
					FE_PushEvent(&kevent);

				} else {
					
					kevent.user.code = EVENT_ENABLE_SCREEN;
					FE_PushEvent(&kevent);

				}

			}

		}

	}

}

void sdl_client_render(struct client_ext *ce, SDL_Surface surf) {
	
	if (screen == NULL)
		return;

	SDL_Surface* s = &surf;

	SDL_BlitSurface(s, NULL, screen, NULL);
	SDL_UpdateRect(screen, 0, 0, g_context.scr_width, g_context.scr_height);

}

void sdl_client_cleanup(struct client_ext *ce) {
	
	if (screen != NULL) {
		
		SDL_FreeSurface(screen);

	}

	if (SDL_WasInit(SDL_INIT_VIDEO) != 0)
		SDL_QuitSubSystem(SDL_INIT_VIDEO);

}

void sdl_client(struct client_ext *ce) {
	
	ce->setup = &sdl_client_setup;
	ce->handle_event = &sdl_client_handle_event;
	ce->render = &sdl_client_render;
	ce->cleanup = &sdl_client_cleanup;

}