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
  * Common interfaces for in/output client extensions.
  */

#include "client_exts.h"
#include "rj_interface.h"
#include "remotejoy_plus.h"

int ce_ids = 0;

void register_client_ext(char* name, void (*ce_register_func)(struct client_ext *), struct client_ext *ce) {
	
	ce->name = name;
	ce->id = ++ce_ids;
	ce_register_func(ce);

}

void forward_button_input(int type, unsigned int keymap) {
	
	if (type == TYPE_BUTTON_DOWN)
		g_context.button_state |= keymap;
	else if (type == TYPE_BUTTON_UP)
		g_context.button_state &= ~keymap;
	else
		return;

	rj_send_event(type, keymap);

}