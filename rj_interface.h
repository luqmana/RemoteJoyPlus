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
#define EVENT_TOGGLE_SCREEN			5
#define EVENT_TOGGLE_FULLCOLOUR		6

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

enum PspCtrlButtons {
	/** Select button. */
	PSP_CTRL_SELECT     = 0x000001,
	/** Start button. */
	PSP_CTRL_START      = 0x000008,
	/** Up D-Pad button. */
	PSP_CTRL_UP         = 0x000010,
	/** Right D-Pad button. */
	PSP_CTRL_RIGHT      = 0x000020,
	/** Down D-Pad button. */
	PSP_CTRL_DOWN      	= 0x000040,
	/** Left D-Pad button. */
	PSP_CTRL_LEFT      	= 0x000080,
	/** Left trigger. */
	PSP_CTRL_LTRIGGER   = 0x000100,
	/** Right trigger. */
	PSP_CTRL_RTRIGGER   = 0x000200,
	/** Triangle button. */
	PSP_CTRL_TRIANGLE   = 0x001000,
	/** Circle button. */
	PSP_CTRL_CIRCLE     = 0x002000,
	/** Cross button. */
	PSP_CTRL_CROSS      = 0x004000,
	/** Square button. */
	PSP_CTRL_SQUARE     = 0x008000,
	/** Home button. */
	PSP_CTRL_HOME       = 0x010000,
	/** Music Note button. */
	PSP_CTRL_NOTE       = 0x800000,
	/** Screen button. */
	PSP_CTRL_SCREEN     = 0x400000,
	/** Volume up button. */
	PSP_CTRL_VOLUP      = 0x100000,
	/** Volume down button. */
	PSP_CTRL_VOLDOWN    = 0x200000
};

enum PspButtons {
	PSP_BUTTON_CROSS = 0,
	PSP_BUTTON_CIRCLE = 1,
	PSP_BUTTON_TRIANGLE = 2,
	PSP_BUTTON_SQUARE = 3,
	PSP_BUTTON_LTRIGGER = 4,
	PSP_BUTTON_RTRIGGER = 5,
	PSP_BUTTON_START = 6,
	PSP_BUTTON_SELECT = 7,
	PSP_BUTTON_UP = 8,
	PSP_BUTTON_DOWN = 9,
	PSP_BUTTON_LEFT = 10,
	PSP_BUTTON_RIGHT = 11,
	PSP_BUTTON_HOME  = 12,
	PSP_BUTTON_NOTE  = 13,
	PSP_BUTTON_SCREEN = 14,
	PSP_BUTTON_VOLUP  = 15,
	PSP_BUTTON_VOLDOWN = 16,
	PSP_BUTTON_MAX   = 17
};

extern int rj_reader_sock;
extern SDL_Thread *rj_reader_thread;

int rj_interface_init();

int rj_reader_start();

int rj_send_event(int type, unsigned int value);

void rj_interface_cleanup();

#endif // __RJInterface_H_