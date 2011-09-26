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
#include "remotejoy_plus.h"
#include "rj_interface.h"
#include "sdl_output.h"
#include "utils.h"
#include "ws_output.h"

struct GlobalContext g_context;

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

 	if (rj_interface_init() != 0) {
 		
 		fprintf(stderr, "Could not initiate remotejoy system.\n");
 		return 1;

 	}

 	// Our list of outputs
 	struct output_ext oe_list[2];

 	// Register some output now
 	register_output_ext("SDL Output", &sdl_output, &oe_list[0]);
 	register_output_ext("WebSocket Output", &ws_output, &oe_list[1]);

 	g_context.psp_flags = SCREEN_CMD_FULLCOLOR;
 	g_context.exit_flag = 0;
 	g_context.scr_mode[0] = g_context.scr_mode[1] = 3;
 	g_context.scr_width = PSP_SCREEN_W;
 	g_context.scr_height = PSP_SCREEN_H;

 	g_context.sur_buffers[0] = create_surface(g_context.scr_buffers[0].buf, g_context.scr_mode[0]);
 	g_context.sur_buffers[1] = create_surface(g_context.scr_buffers[1].buf, g_context.scr_mode[1]);

 	if (g_context.sur_buffers[0] == NULL || g_context.sur_buffers[1] == NULL) {
 		
 		fprintf(stderr, "Unable to create surface buffers [%s].\n", SDL_GetError());
 		return 1;

 	}

 	// Let the outputs setup what they need to
 	for (int r = 0; r < 2; r++)
 		oe_list[r].setup(&oe_list[r]);

 	if (rj_reader_start() != 0) {
 		
 		fprintf(stderr, "Could not start remotejoy reader system.\n");
 		return 1;

 	}

 	while (!g_context.exit_flag) {
 		
 		SDL_Event event;

 		if (!FE_WaitEvent(&event))
 			break;

 		if (event.type == SDL_USEREVENT) {
 			
 			switch (event.user.code) {
 				
 				case EVENT_ENABLE_SCREEN:

 					rj_send_event(TYPE_SCREEN_CMD, SCREEN_CMD_ACTIVE | g_context.psp_flags);

 					break;

 				case EVENT_DISABLE_SCREEN:

 					rj_send_event(TYPE_SCREEN_CMD, 0);

 					break;

 				case EVENT_RENDER_FRAME_1:

 					if (g_context.scr_buffers[0].head.mode != g_context.scr_mode[0]) {
 						
 						SDL_FreeSurface(g_context.sur_buffers[0]);
 						g_context.sur_buffers[0] = create_surface(g_context.scr_buffers[0].buf, g_context.scr_buffers[0].head.mode);
 						g_context.scr_mode[0] = g_context.scr_buffers[0].head.mode;

 					}

 					// Render time!
				 	for (int r = 0; r < 2; r++)
				 		oe_list[r].render(&oe_list[r], *g_context.sur_buffers[0]);

 					break;

 				case EVENT_RENDER_FRAME_2:

 					if (g_context.scr_buffers[1].head.mode != g_context.scr_mode[1]) {
 						
 						SDL_FreeSurface(g_context.sur_buffers[1]);
 						g_context.sur_buffers[1] = create_surface(g_context.scr_buffers[1].buf, g_context.scr_buffers[1].head.mode);
 						g_context.scr_mode[1] = g_context.scr_buffers[1].head.mode;

 					}

 					// Render time!
				 	for (int r = 0; r < 2; r++)
				 		oe_list[r].render(&oe_list[r], *g_context.sur_buffers[1]);

 					break;

 				default:

 					break;

 			}

 		}

 		if (event.type == SDL_QUIT)
 			break;

 		// Let the outputs handl events if they want
	 	for (int r = 0; r < 2; r++)
	 		oe_list[r].handle_event(&oe_list[r], event);

 	}

 	// Let the outputs cleanup
 	for (int r = 0; r < 2; r++)
 		oe_list[r].cleanup(&oe_list[r]);

 	if (g_context.sur_buffers[0]) {
 		
 		SDL_FreeSurface(g_context.sur_buffers[0]);
 		g_context.sur_buffers[0] = NULL;

 	}

 	if (g_context.sur_buffers[1]) {
 		
 		SDL_FreeSurface(g_context.sur_buffers[1]);
 		g_context.sur_buffers[1] = NULL;

 	}

 	rj_interface_cleanup();

 	FE_Quit();
 	SDL_Quit();

	return 0;

}