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

#ifndef __RJInterface_H_
#define __RJInterface_H_

#include <SDL_thread.h>

#define DEFAULT_RJ_IP				"localhost"
#define DEFAULT_RJ_PORT				10004

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

extern int rj_reader_sock;
extern SDL_Thread *rj_reader_thread;

int rj_interface_init();

int rj_reader_start();

int rj_send_event(int type, unsigned int value);

void rj_interface_cleanup();

#endif // __RJInterface_H_