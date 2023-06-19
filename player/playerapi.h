/**********
This library is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the
Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version. (See <http://www.gnu.org/copyleft/lesser.html>.)

This library is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
**********/
/* Copyright (C) 2011 Zack Xue <zackxue@163.com> */

#ifndef __RTSP_PLAYER_API__
#define __RTSP_PLAYER_API__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (* PlayerDataCb) (unsigned char *buf, int size, void *arg);

typedef struct {
    void            *playhandle;
    unsigned char   url[256];
    int             streamUsingTCP;
    PlayerDataCb    cb;
	char            threadDoneFlag;
	short           startPort;
    unsigned int    buffersize;
    void            *arg;
} RtspPlayer;

int RtspPlayerStart(RtspPlayer* param);
int RtspPlayerStop(RtspPlayer* param);

#ifdef __cplusplus
};
#endif
#endif
