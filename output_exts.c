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
  * Common interfaces for output extensions.
  */

#include "output_exts.h"

int oe_ids = 0;

void register_output_ext(char* name, void (*oe_register_func)(struct output_ext *), struct output_ext *oe) {
	
	oe->name = name;
	oe->id = ++oe_ids;
	oe_register_func(oe);

}