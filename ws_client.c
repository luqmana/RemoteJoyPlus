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
  * Outputs to a browser via WebSockets
  */

#include "ws_client.h"

#include <libwebsockets.h>
#include <SDL_thread.h>
#include <unistd.h>

int ws_http_callback(
	struct libwebsocket_context *,
	struct libwebsocket *,
	enum libwebsocket_callback_reasons,
	void *, void *, size_t);

int ws_videoframe_callback(
	struct libwebsocket_context *,
	struct libwebsocket *,
	enum libwebsocket_callback_reasons,
	void *, void *, size_t);

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
	{
		"video-frame",			// Name
		ws_videoframe_callback,	// Callback func
		0						// Per session data size
	},
	{ NULL, NULL, 0 }

};

struct libwebsocket_context *ws_context = NULL;
SDL_Thread *ws_service_thread = NULL;

int ws_service_handler(void* p) {
	
	while (1) {
		
		libwebsocket_service(ws_context, 50);

	}

	return 0;

}

int ws_http_callback(
	struct libwebsocket_context *context,
	struct libwebsocket *wsi,
	enum libwebsocket_callback_reasons reason,
	void *user, void *in, size_t len) {

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

int ws_videoframe_callback(
	struct libwebsocket_context *context,
	struct libwebsocket *wsi,
	enum libwebsocket_callback_reasons reason,
	void *user, void *in, size_t len) {

	switch (reason) {
		
		case LWS_CALLBACK_ESTABLISHED:

			printf("videoframe_callback: LWS_CALLBACK_ESTABLISHED\n");

			break;

		case LWS_CALLBACK_BROADCAST:

			printf("len is [%d]\n", (int)len);

			//int n = libwebsocket_write(wsi, p)

			break;

		case LWS_CALLBACK_RECEIVE:

			break;

		case LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION:

			// return non-zero here to kill connection

			break;

		default:
			break;

	}

	return 0;

}

void ws_client_setup(struct client_ext *ce) {
	
	printf("Hi! From %s [id: %d]. \n", ce->name, ce->id);

	int opts = 0;

	ws_context = libwebsocket_create_context(
					DEFAULT_WS_PORT, DEFAULT_WS_IP,
					ws_protocols,
					libwebsocket_internal_extensions,
					NULL, NULL, -1, -1, opts);

	if (ws_context == NULL) {
		
		fprintf(stderr, "WebSockets Client: Unable to initialize libwebsockets.\n");

		return;

	}

	ws_service_thread = SDL_CreateThread(ws_service_handler, NULL);

	if (ws_service_thread == NULL) {
		
		fprintf(stderr, "WebSockets Client: Unable to create ws service thread [%s].\n", SDL_GetError());

		libwebsocket_context_destroy(ws_context);

		ws_context = NULL;

		return;
		
	}

}

void ws_client_cleanup(struct client_ext *ce) {
	
	if (ws_context == NULL)
		return;

	libwebsocket_context_destroy(ws_context);

}

void ws_client_handle_event(struct client_ext *ce, SDL_Event event) {
	
	if (ws_context == NULL)
		return;

}

void ws_client_render(struct client_ext *ce, struct ScreenBuffer *sbuf) {
	
	if (ws_context == NULL)
		return;

	libwebsockets_broadcast(&ws_protocols[PROTOCOL_VIDEOFRAME], NULL, sbuf->head.mode);

}

void ws_client(struct client_ext *ce) {
	
	ce->setup = &ws_client_setup;
	ce->handle_event = &ws_client_handle_event;
	ce->render = &ws_client_render;
	ce->cleanup = &ws_client_cleanup;

}