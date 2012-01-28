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
 * This interface handles the actual communication
 * with the PSP via RemoteJoy.
 */

#include "fast_events.h"
#include "remotejoy_plus.h"
#include "rj_interface.h"
#include "utils.h"

#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <SDL.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#ifndef SOL_TCP
	#define SOL_TCP IPPROTO_TCP
#endif

static int rj_sock = -1;
static SDL_Thread *rj_reader_thread = NULL;

int rj_reader_handler(void *p) {
	
	int err = 0;
	int frame = 0;
	fd_set saveset, readset;
	int count;
	int sock = *(int *) p;
	struct JoyScrHeader head;

	FD_ZERO(&saveset);
	FD_SET(sock, &saveset);

	while (!err) {
		
		readset = saveset;

		count = select(FD_SETSIZE, &readset, NULL, NULL, NULL);

		if (count > 0) {
			
			int ret;
			int mode;
			int size;

			if (FD_ISSET(sock, &readset)) {
				
				ret = read(sock, &head, sizeof(head));

				// Confirm the magic!
				if ((ret != sizeof(head)) || (LE32(head.magic) != JOY_MAGIC)) {
					
					fprintf(stderr, "Error in socket %d magic %08X.\n", ret, head.magic);

					break;

				}
				
				mode = LE32(head.mode);
				size = LE32(head.size);
				g_context.scr_buffers[frame].head.mode = mode;
				g_context.scr_buffers[frame].head.size = size;

				if (mode < 0) {
					
					SDL_Event event;
					event.type = SDL_USEREVENT;
					event.user.code = EVENT_ENABLE_SCREEN;
					event.user.data1 = NULL;
					event.user.data2 = NULL;

					FE_PushEvent(&event);

				} else if (mode > 3) {
					
					// Something about flushing should go here...

				} else {
					
					int loc = 0;

					while (1) {
						
						readset = saveset;

						count = select(FD_SETSIZE, &readset, NULL, NULL, NULL);

						if (count > 0) {
							
							ret = read(sock, &(g_context.scr_buffers[frame].buf[loc]), size - loc);

							if (ret < 0) {
								
								if (errno != EINTR) {
									
									perror("read");
									err = 1;
									break;

								}

							} else if (ret == 0) {
								
								fprintf(stderr, "EOF\n");
								break;

							}

							loc += ret;
							if (loc == size)
								break;

						} else if (count < 0) {
							
							if (errno != EINTR) {
								
								perror("select");
								err = 1;
								break;

							}

						}

					}

					if (!err) {
						
						SDL_Event event;
						event.type = SDL_USEREVENT;
						event.user.code = (frame) ? EVENT_RENDER_FRAME_2 : EVENT_RENDER_FRAME_1;
						event.user.data1 = NULL;
						event.user.data2 = NULL;

						FE_PushEvent(&event);

					}

				}

			}

		} else if (count < 0) {
			
			if (errno != EINTR) {
				
				perror("select");
				err = 1;

			}

		}

	}

	return 0;

}

int rj_interface_init() {
	
	struct sockaddr_in name;
	int flag;

	rj_sock = socket(PF_INET, SOCK_STREAM, 0);

	if (rj_sock < 0) {
		
		perror("socket");
		return -1;

	}

	struct hostent *hostinfo;

	name.sin_family = AF_INET;
	name.sin_port = htons(DEFAULT_RJ_PORT);
	hostinfo = gethostbyname(DEFAULT_RJ_IP);

	if (hostinfo == NULL) {
		
		fprintf(stderr, "Unknown host %s.\n", DEFAULT_RJ_IP);
		return -1;

	}

	name.sin_addr = *(struct in_addr *) hostinfo->h_addr;

	if (connect(rj_sock, (struct sockaddr *) &name, sizeof(name)) < 0) {
		
		perror("connect");
		close(rj_sock);
		return -1;

	}

	// Disable Nagle's algorithm
	setsockopt(rj_sock, SOL_TCP, TCP_NODELAY, &flag, sizeof(int));

	rj_send_event(TYPE_SCREEN_CMD, SCREEN_CMD_ACTIVE | g_context.psp_flags);

	return 0;

}

int rj_reader_start() {
	
	rj_reader_thread = SDL_CreateThread(rj_reader_handler, (void *) &rj_sock);

	if (rj_reader_thread == NULL) {

		fprintf(stderr, "%s\n", SDL_GetError());
		return -1;

	}

	return 0;

}

int rj_send_event(int type, unsigned int value) {

	struct JoyEvent event;

	if (rj_sock < 0)
		return -1;

	// Swap endianness for the PSP
	event.magic = LE32(JOY_MAGIC);
	event.type = LE32(type);
	event.value = LE32(value);

	const void *buf = (void *) &event;

	int written = 0;
	int len = sizeof(event);

	while (written < len) {
		
		int ret = write(rj_sock, buf + written, len - written);

		if (ret < 0) {
			
			if (errno != EINTR) {
				
				perror("write");
				written = -1;

				break;

			}

		} else
			written += ret;

	}

	if (written != len) {
		
		fprintf(stderr, "Could not write out data to socket.\n");
		return -1;

	}
	
	return 0;

}

void rj_interface_cleanup() {
	
	if (rj_reader_thread)
		SDL_KillThread(rj_reader_thread);

	if (rj_sock >= 0)
		close(rj_sock);

}
