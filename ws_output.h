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

#ifndef __WSOutput_H_
#define __WSOutput_H_

#include "output_exts.h"

#define DEFAULT_WS_IP			"localhost"
#define DEFAULT_WS_PORT			7681

void ws_output(struct output_ext *oe);

#endif // __WSOutput_H_