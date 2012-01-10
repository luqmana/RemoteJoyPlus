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
#include "utils.h"

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

	SDL_ShowCursor(SDL_DISABLE);

}

void sdl_client_cleanup(struct client_ext *ce) {
	
	if (screen != NULL) {
		
		SDL_FreeSurface(screen);

	}

	if (SDL_WasInit(SDL_INIT_VIDEO) != 0)
		SDL_QuitSubSystem(SDL_INIT_VIDEO);

}

void sdl_client_handle_event(struct client_ext *ce, SDL_Event event) {
	
	if (screen == NULL)
		return;

	if (event.type == SDL_USEREVENT) {
		
		if (event.user.code == EVENT_DISABLE_SCREEN) {
			
			// Let's also black it out
			SDL_FillRect(screen, NULL, 0x000000);
			SDL_UpdateRect(screen, 0, 0, g_context.scr_width, g_context.scr_height);

		}

	} else if (event.type == SDL_VIDEORESIZE) {

		// Free old screen and setup with new size

		SDL_FreeSurface(screen);
		screen = SDL_SetVideoMode(event.resize.w, event.resize.h, 0, SDL_HWSURFACE);
		g_context.scr_width = event.resize.w;
		g_context.scr_height = event.resize.h;

	} else if ((event.type == SDL_KEYDOWN) || (event.type == SDL_KEYUP)) {
		
		SDL_KeyboardEvent *key = (SDL_KeyboardEvent *) &event;

		if (key->keysym.sym == SDLK_ESCAPE) {
			
			// Quit!
			// ToDo: Quit only the SDL client?
			
			SDL_Event event;

			event.type = SDL_QUIT;
			event.user.code = 0;
			event.user.data1 = event.user.data2 = NULL;

			FE_PushEvent(&event);

		} else if (key->keysym.sym == SDLK_F3) {

			// Toggle full colour mode
			if (event.type == SDL_KEYDOWN) {
				
				SDL_Event kevent;
				kevent.type = SDL_USEREVENT;
				kevent.user.code = EVENT_TOGGLE_FULLCOLOUR;
				kevent.user.data1 = NULL;
				kevent.user.data2 = NULL;
				FE_PushEvent(&kevent);


			}

		} else if (key->keysym.sym == SDLK_F5) {

			// Toggle Screen State
			
			if (event.type == SDL_KEYDOWN) {
				
				SDL_Event kevent;
				kevent.type = SDL_USEREVENT;
				kevent.user.code = EVENT_TOGGLE_SCREEN;
				kevent.user.data1 = NULL;
				kevent.user.data2 = NULL;
				FE_PushEvent(&kevent);

			}

		} else if (key->keysym.sym == SDLK_F11) {
			
			// Make the screen fullscreen!
			
			if (event.type == SDL_KEYDOWN)
				SDL_WM_ToggleFullScreen(screen);

		}
		
		// Handle user input
		// and forward to psp
		unsigned int keymap = 0;
		switch (key->keysym.sym) {
			
			case SDLK_LEFT:
				keymap = PSP_CTRL_LEFT;
				break;
			case SDLK_RIGHT:
				keymap = PSP_CTRL_RIGHT;
				break;
			case SDLK_UP:
				keymap = PSP_CTRL_UP;
				break;
			case SDLK_DOWN:
				keymap = PSP_CTRL_DOWN;
				break;
			default:
				break;

		}

		if (event.type == SDL_KEYDOWN)
			forward_button_input(TYPE_BUTTON_DOWN, keymap);
		else if (event.type == SDL_KEYUP)
			forward_button_input(TYPE_BUTTON_UP, keymap);
			
	}

}

void sdl_client_render(struct client_ext *ce, struct ScreenBuffer *sbuf) {
	
	if (screen == NULL)
		return;

	SDL_Surface *surf = create_surface(sbuf->buf, sbuf->head.mode);

	SDL_BlitSurface(surf, NULL, screen, NULL);
	SDL_UpdateRect(screen, 0, 0, g_context.scr_width, g_context.scr_height);

	SDL_FreeSurface(surf);

}

void sdl_client(struct client_ext *ce) {
	
	ce->setup = &sdl_client_setup;
	ce->handle_event = &sdl_client_handle_event;
	ce->render = &sdl_client_render;
	ce->cleanup = &sdl_client_cleanup;

}
