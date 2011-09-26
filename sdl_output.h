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
  * Outputs to the screen via SDL
  */

#ifndef __SdlOutput_H_
#define __SdlOutput_H_

#include "output_exts.h"

void sdl_output(struct output_ext *oe);

#endif // __SdlOutput_H_