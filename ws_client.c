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
#include "utils.h"

int ws_http_callback(
	struct libwebsocket_context *,
	struct libwebsocket *,
	enum libwebsocket_callback_reasons,
	void *, void *, size_t);

int ws_video_callback(
	struct libwebsocket_context *,
	struct libwebsocket *,
	enum libwebsocket_callback_reasons,
	void *, void *, size_t);

int ws_control_callback(
	struct libwebsocket_context *,
	struct libwebsocket *,
	enum libwebsocket_callback_reasons,
	void *, void *, size_t);

enum protocols {
	
	PROTOCOL_HTTP = 0,
	PROTOCOL_RJ_VIDEO,
	PROTOCOL_RJ_CONTROL,

};

int sender = 0;

static struct libwebsocket_protocols ws_protocols[] = {
	
	{
		"http-only",			// Name
		ws_http_callback,		// Callback func
		0						// Per session data size
	},
	{
		"rj-video",				// Name
		ws_video_callback,		// Callback func
		0						// Per session data size
	},
	{
		"rj-control",				// Name
		ws_control_callback,		// Callback func
		0							// Per session data size
	},
	{ NULL, NULL, 0 }

};

struct libwebsocket_context *ws_context = NULL;
SDL_Thread *ws_service_thread = NULL;

int ws_service_handler(void* p) {
	
	while (1) {
		
		usleep(20);

		libwebsocket_service(ws_context, 20);

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

void ws_do_input(char *rec) {

	char t = rec[0];
	char *k = &rec[1];

	unsigned int keymap = 0;

	if (strcmp(k, "LEFT") == 0)
		keymap = PSP_CTRL_LEFT;
	else if (strcmp(k, "UP") == 0)
		keymap = PSP_CTRL_UP;
	else if (strcmp(k, "RIGHT") == 0)
		keymap = PSP_CTRL_RIGHT;
	else if (strcmp(k, "DOWN") == 0)
		keymap = PSP_CTRL_DOWN;
	else if (strcmp(k, "CROSS") == 0)
		keymap = PSP_CTRL_CROSS;
	else if (strcmp(k, "CIRCLE") == 0)
		keymap = PSP_CTRL_CIRCLE;
	else if (strcmp(k, "SQUARE") == 0)
		keymap = PSP_CTRL_SQUARE;
	else if (strcmp(k, "TRIANGLE") == 0)
		keymap = PSP_CTRL_TRIANGLE;
	else if (strcmp(k, "LTRIGGER") == 0)
		keymap = PSP_CTRL_LTRIGGER;
	else if (strcmp(k, "RTRIGGER") == 0)
		keymap = PSP_CTRL_RTRIGGER;
	else if (strcmp(k, "START") == 0)
		keymap = PSP_CTRL_START;
	else if (strcmp(k, "SELECT") == 0)
		keymap = PSP_CTRL_SELECT;
	else if (strcmp(k, "SCREEN") == 0)
		keymap = PSP_CTRL_SCREEN;
	else if (strcmp(k, "NOTE") == 0)
		keymap = PSP_CTRL_NOTE;
	else if (strcmp(k, "VOLUP") == 0)
		keymap = PSP_CTRL_VOLUP;
	else if (strcmp(k, "VOLDOWN") == 0)
		keymap = PSP_CTRL_VOLDOWN;
	else if (strcmp(k, "HOME") == 0)
		keymap = PSP_CTRL_HOME;
	else
		return;

	if (t == 'd')
		forward_button_input(TYPE_BUTTON_DOWN, keymap);
	else if (t == 'u')
		forward_button_input(TYPE_BUTTON_UP, keymap);

}

int ws_video_callback(
	struct libwebsocket_context *context,
	struct libwebsocket *wsi,
	enum libwebsocket_callback_reasons reason,
	void *user, void *in, size_t len) {

	switch (reason) {
		
		case LWS_CALLBACK_ESTABLISHED:

			break;

		case LWS_CALLBACK_BROADCAST:{

			int n = libwebsocket_write(wsi, (unsigned char *)in, len, LWS_WRITE_BINARY);

			if (n < 0) {
				
				fprintf(stderr, "Error writing video frame.\n");
				return 1;

			}
			//libwebsocket_close_and_free_session(context, wsi, LWS_CLOSE_STATUS_NORMAL);

			}break;

		case LWS_CALLBACK_RECEIVE:
		case LWS_CALLBACK_CLIENT_RECEIVE:

			

			break;

		case LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION:

			// return non-zero here to kill connection

			break;

		default:
			break;

	}

	return 0;

}

int ws_control_callback(
	struct libwebsocket_context *context,
	struct libwebsocket *wsi,
	enum libwebsocket_callback_reasons reason,
	void *user, void *in, size_t len) {

	switch (reason) {
		
		case LWS_CALLBACK_ESTABLISHED:

			break;

		case LWS_CALLBACK_BROADCAST:{

			int n = libwebsocket_write(wsi, (unsigned char *)in, len, LWS_WRITE_BINARY);

			if (n < 0) {
				
				fprintf(stderr, "Error control info.\n");
				return 1;

			}
			//libwebsocket_close_and_free_session(context, wsi, LWS_CLOSE_STATUS_NORMAL);

			}break;

		case LWS_CALLBACK_RECEIVE:
		case LWS_CALLBACK_CLIENT_RECEIVE:

			((char *)in)[len] = '\0';

			char *rec = ((char *)in);
			
			if (rec[0] == 'd' || rec[0] == 'u')
				ws_do_input(rec);

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

	ws_context = NULL;

}

void ws_client_handle_event(struct client_ext *ce, SDL_Event event) {
	
	if (ws_context == NULL)
		return;

	if (event.type == SDL_VIDEORESIZE) {
		
		unsigned char buf[LWS_SEND_BUFFER_PRE_PADDING + 6 + LWS_SEND_BUFFER_POST_PADDING];
		memcpy(&buf[LWS_SEND_BUFFER_PRE_PADDING], "resize", 6);

		libwebsockets_broadcast(&ws_protocols[PROTOCOL_RJ_CONTROL], &buf[LWS_SEND_BUFFER_PRE_PADDING], 6);

	}

}

void ws_client_render(struct client_ext *ce, struct ScreenBuffer *sbuf) {
	
	if (ws_context == NULL)
		return;
	
	if (sender == 15)
		sender = 0;

	if (sender != 0) {
		
		sender++;
		return;

	}

	sender++;
	
	// convert to a surface to handle variable modes
	SDL_Surface *r = create_surface(sbuf->buf, sbuf->head.mode);
	SDL_Surface *s = SDL_DisplayFormat(r);

	SDL_RWops *rw;
	char b[PSP_SCREEN_W * PSP_SCREEN_H * 4];
	rw = SDL_RWFromMem(b, sizeof(b));
	SDL_SaveBMP_RW(s, rw, 0);

	// now to get the size of the bmp
	int sz;
	memcpy(&sz, b + 2, 4);

	unsigned char buf[LWS_SEND_BUFFER_PRE_PADDING + sz + LWS_SEND_BUFFER_POST_PADDING];
	memcpy(&buf[LWS_SEND_BUFFER_PRE_PADDING], b, sz);

	libwebsockets_broadcast(&ws_protocols[PROTOCOL_RJ_VIDEO], &buf[LWS_SEND_BUFFER_PRE_PADDING], sz);

	SDL_FreeSurface(s);
	SDL_FreeSurface(r);/*

	unsigned char buf[LWS_SEND_BUFFER_PRE_PADDING + sbuf->head.size + LWS_SEND_BUFFER_POST_PADDING];
	memcpy(&buf[LWS_SEND_BUFFER_PRE_PADDING], sbuf->buf, sbuf->head.size);

	libwebsockets_broadcast(&ws_protocols[PROTOCOL_RJ_VIDEO], &buf[LWS_SEND_BUFFER_PRE_PADDING], sbuf->head.size);*/

}

void ws_client(struct client_ext *ce) {
	
	ce->setup = &ws_client_setup;
	ce->handle_event = &ws_client_handle_event;
	ce->render = &ws_client_render;
	ce->cleanup = &ws_client_cleanup;

}