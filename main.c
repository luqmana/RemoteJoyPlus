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
  * Main entry point.
  * Prepares everything.
  */

#include <SDL.h>
#include <stdio.h>

#include "fast_events.h"
#include "client_exts.h"
#include "remotejoy_plus.h"
#include "rj_interface.h"
#include "utils.h"

#include "sdl_client.h"
#include "ws_client.h"

/**
 * Yes! A global struct holding ALL THE INFO!
 * Let's hope we have no race conditions...
 */
struct GlobalContext g_context;

int main(int argc, char **argv) {
 	
	printf("RemoteJoy+ for PSP (c) 2011 Luqman A.\n");
 	printf("Built: %s %s\n", __DATE__, __TIME__);

 	g_context.psp_flags = SCREEN_CMD_FULLCOLOR;
 	g_context.exit_flag = 0;
 	g_context.scr_width = PSP_SCREEN_W;
 	g_context.scr_height = PSP_SCREEN_H;
 	g_context.scr_on = 1;
 	g_context.button_state = 0;

 	if (SDL_Init(SDL_INIT_EVENTTHREAD) != 0) {
 		
 		fprintf(stderr, "Could not initialise SDL [%s].\n", SDL_GetError());
 		return 1;

 	}

 	if (FE_Init() != 0) {
 		
 		fprintf(stderr, "Could not initialize SDL fast events system [%s].\n", FE_GetError());
 		return 1;

 	}

 	if (rj_interface_init() != 0) {
 		
 		fprintf(stderr, "Could not initiate remotejoy system.\n");
 		return 1;

 	}

 	// Our list of outputs
 	int num_client_exts = 2;
 	struct client_ext ce_list[num_client_exts];

 	// Register some output now
 	register_client_ext("SDL Client", &sdl_client, &ce_list[0]);
 	register_client_ext("WebSockets Client", &ws_client, &ce_list[1]);

 	// Let the clients setup what they need to
 	for (int r = 0; r < num_client_exts; r++)
 		ce_list[r].setup(&ce_list[r]);

 	if (rj_reader_start() != 0) {
 		
 		fprintf(stderr, "Could not start remotejoy reader system.\n");
 		return 1;

 	}

 	rj_send_event(TYPE_SCREEN_CMD, SCREEN_CMD_ACTIVE | g_context.psp_flags);

 	// Begin main loop
 	while (!g_context.exit_flag) {
 		
 		SDL_Event event;

 		// This call just blocks until we've received some event
 		// It'll break outta the loop and thus exit the program
 		// if it errors out for some reason
 		if (!FE_WaitEvent(&event))
 			break;

 		if (event.type == SDL_USEREVENT) {
 			
 			switch (event.user.code) {

 				case EVENT_TOGGLE_SCREEN:

 					if (g_context.scr_on) {
 						
 						event.user.code = EVENT_DISABLE_SCREEN;
 						FE_PushEvent(&event);

 					} else {
 						
 						event.user.code = EVENT_ENABLE_SCREEN;
 						FE_PushEvent(&event);

 					}

 					break;

 				case EVENT_TOGGLE_FULLCOLOUR:

 					g_context.psp_flags ^= SCREEN_CMD_FULLCOLOR;
 					rj_send_event(TYPE_SCREEN_CMD, SCREEN_CMD_ACTIVE | g_context.psp_flags);
 					g_context.scr_on = 1;

 					break;
 				
 				case EVENT_ENABLE_SCREEN:

 					rj_send_event(TYPE_SCREEN_CMD, SCREEN_CMD_ACTIVE | g_context.psp_flags);
 					g_context.scr_on = 1;

 					break;

 				case EVENT_DISABLE_SCREEN:

 					rj_send_event(TYPE_SCREEN_CMD, 0);
 					g_context.scr_on = 0;

 					break;

 				case EVENT_RENDER_FRAME_1:

 					if (!g_context.scr_on)
 						break;

 					// Render time!
 					for (int r = 0; r < num_client_exts; r++)
 						ce_list[r].render(&ce_list[r], &g_context.scr_buffers[0]);

 					break;

 				case EVENT_RENDER_FRAME_2:

 					if (!g_context.scr_on)
 						break;

 					// Render time!
 					for (int r = 0; r < num_client_exts; r++)
 						ce_list[r].render(&ce_list[r], &g_context.scr_buffers[1]);

 					break;

 				default:

 					break;

 			}

 		}

 		if (event.type == SDL_QUIT)
 			break;

 		// Let the clients handle events if they want
	 	for (int r = 0; r < num_client_exts; r++)
	 		ce_list[r].handle_event(&ce_list[r], event);

 	}

 	// Let the clients cleanup
 	for (int r = 0; r < num_client_exts; r++)
 		ce_list[r].cleanup(&ce_list[r]);

 	rj_interface_cleanup();

 	FE_Quit();
 	SDL_Quit();

	return 0;

}
