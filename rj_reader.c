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
  * Reads data from the PSP via RemoteJoy/PSPLink
  */

#include "rj_reader.h"
#include "utils.h"

#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#ifndef SOL_TCP
	#define SOL_TCP IPPROTO_TCP
#endif

int rj_reader_sock = -1;
SDL_Thread *rj_reader_thread = NULL;

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

				//if ((ret != sizeof(head)) ||)

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

int rj_reader_init() {
	
	struct sockaddr_in name;
	int flag;

	rj_reader_sock = socket(PF_INET, SOCK_STREAM, 0);

	if (rj_reader_sock < 0) {
		
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

	if (connect(rj_reader_sock, (struct sockaddr *) &name, sizeof(name)) < 0) {
		
		perror("connect");
		close(rj_reader_sock);
		return -1;

	}

	// Disable Nagle's algorithm
	setsockopt(rj_reader_sock, SOL_TCP, TCP_NODELAY, &flag, sizeof(int));

	rj_reader_thread = SDL_CreateThread(rj_reader_handler, (void *) &rj_reader_sock);

	if (rj_reader_thread == NULL)
		return -1;

	return 0;

}

void rj_reader_start() {
	


}