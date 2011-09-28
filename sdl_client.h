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
  * Outputs to the screen via SDL.
  */

#ifndef __SdlClient_H_
#define __SdlClient_H_

#include "client_exts.h"

void sdl_client(struct client_ext *ce);

#endif // __SdlClient_H_