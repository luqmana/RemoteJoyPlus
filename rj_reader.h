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

#ifndef __RJReader_H_
#define __RJReader_H_

#include <SDL_thread.h>

#define DEFAULT_RJ_IP				"localhost"
#define DEFAULT_RJ_PORT				10004

#define PSP_SCREEN_W 				480
#define PSP_SCREEN_H 				272

#define EVENT_ENABLE_SCREEN			1
#define EVENT_RENDER_FRAME_1		2
#define EVENT_RENDER_FRAME_2		3
#define EVENT_DISABLE_SCREEN		4

#define JOY_MAGIC 					0x909ACCEF

#define TYPE_BUTTON_DOWN 			1
#define TYPE_BUTTON_UP   			2
#define TYPE_ANALOG_Y    			3
#define TYPE_ANALOG_X    			4
#define TYPE_SCREEN_CMD  			5

/* Screen commands */
#define SCREEN_CMD_ACTIVE 			1
#define SCREEN_CMD_HSIZE  			2
#define SCREEN_CMD_FULLCOLOR  		4
#define SCREEN_CMD_DROPRATE(x) 		((x)<<24)
#define SCREEN_CMD_GETDROPRATE(x) 	((x)>>24)

struct JoyEvent {

	unsigned int magic;
	int type;
	unsigned int value;

} __attribute__((packed));

struct JoyScrHeader {

	unsigned int magic;
	int mode; /* 0-3 */
	int size;
	int ref;

} __attribute__((packed));

struct ScreenBuffer {

	unsigned char buf[PSP_SCREEN_W * PSP_SCREEN_H * 4];
	struct JoyScrHeader head;

};

extern int rj_reader_sock;
extern SDL_Thread *rj_reader_thread;

int rj_reader_init();
void rj_reader_start();

#endif // __RJReader_H_