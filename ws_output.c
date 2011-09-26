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

#include <libwebsockets.h>
#include <SDL_thread.h>
#include <unistd.h>

int ws_http_callback(struct libwebsocket_context* context,
				  	 struct libwebsocket* wsi,
				  	 enum libwebsocket_callback_reasons reason,
				  	 void* user, void* in, size_t len);

enum protocols {
	
	PROTOCOL_HTTP = 0,
	PROTOCOL_VIDEOFRAME

};

static struct libwebsocket_protocols ws_protocols[] = {
	
	{
		"http-only",			// Name
		ws_http_callback,		// Callback func
		0						// Per session data size
	},
	{ NULL, NULL, 0 }

};

struct libwebsocket_context *ws_context = NULL;
SDL_Thread *ws_service_thread = NULL;

int ws_service_handler(void* p) {
	
	while (1) {
		
		usleep(100);

		//libwebsockets_broadcast(&protocols[PROTOCOL_VIDEOFRAME], &buf[LWS_SEND_BUFFER_PRE_PADDING], 1);

		libwebsocket_service(ws_context, 50);

	}

	return 0;

}

int ws_http_callback(struct libwebsocket_context* context,
				  	 struct libwebsocket* wsi,
				  	 enum libwebsocket_callback_reasons reason,
				  	 void* user, void* in, size_t len) {

	char client_name[128];
	char client_ip[128];

	switch(reason) {
		
		case LWS_CALLBACK_HTTP:

			printf("Serving HTTP URI %s.\n", (char*) in);

			if (libwebsockets_serve_http_file(wsi, "index.html", "text/html")) {
				
				fprintf(stderr, "Failed to send HTTP file.\n");

			}

			break;

		/*
			Callback for confirming to continue with client IP.
			Appears in protocol 0 callback since no WebSocket
			protocol was agreed upon at that time.
			You can just ignore this if you won't filter on
			client IP since the default unhandled callback
			return is 0 meaning let the connection continue.
		*/
		case LWS_CALLBACK_FILTER_NETWORK_CONNECTION:

			libwebsockets_get_peer_addresses((int)(long) user, client_name, sizeof(client_name), client_ip, sizeof(client_ip));

			printf("Received network connect from %s (%s).\n", client_name, client_ip);

			// If we return a non-zero from here,
			// we kill the connection

			break;

		default:
			break;

	}

	return 0;

}

void ws_output_setup(struct output_ext *oe) {
	
	printf("Hi! From %s [id: %d]. \n", oe->name, oe->id);

	ws_context = libwebsocket_create_context(DEFAULT_WS_PORT, DEFAULT_WS_IP, ws_protocols, libwebsocket_internal_extensions, NULL, NULL, -1, -1, 0);

	if (ws_context == NULL) {
		
		fprintf(stderr, "Unable to initialize libwebsockets.\n");

		return;

	}

	ws_service_thread = SDL_CreateThread(ws_service_handler, NULL);

	if (ws_service_thread == NULL) {
		
		fprintf(stderr, "Unable to create ws service thread [%s].\n", SDL_GetError());

		libwebsocket_context_destroy(ws_context);

		ws_context = NULL;

		return;
		
	}

}

void ws_output_handle_event(struct output_ext *oe, SDL_Event event) {
	
	if (ws_context == NULL)
		return;

}

void ws_output_render(struct output_ext *oe, SDL_Surface surf) {
	
	if (ws_context == NULL)
		return;

}

void ws_output_cleanup(struct output_ext *oe) {
	
	if (ws_context == NULL)
		return;

fprintf(stderr, "CLEANUP!\n");

	if (ws_service_thread)
		SDL_KillThread(ws_service_thread);

	libwebsocket_context_destroy(ws_context);

}

void ws_output(struct output_ext *oe) {
	
	oe->setup = &ws_output_setup;
	oe->handle_event = &ws_output_handle_event;
	oe->render = &ws_output_render;
	oe->cleanup = &ws_output_cleanup;

}