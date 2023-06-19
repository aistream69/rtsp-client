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


#include "playerapi.h"
#include<unistd.h>
#include <sys/time.h>
#include <pthread.h>


static unsigned char url[256];

int player_callback(unsigned char * p_buf, int size, void * param)
{
	RtspPlayer *p_param = (RtspPlayer *)param;
	char *name = (char *)param;
    static int cnt = 0;
    FILE *fp = NULL;
    if(fp == NULL) {
        fp = fopen(name, "a");
        if(fp == NULL) {
            printf("fopen test.264 failed\n");
            return -1;
        }
    }
	char buf[4];
	buf[0] = 0x00;
	buf[1] = 0x00;
	buf[2] = 0x00;
	buf[3] = 0x01;
	
	//Write data
    if(fp != NULL) {
	    fwrite(buf, 1, 4, fp);
	    fwrite(p_buf, 1, size, fp);
		fclose(fp);
    }
	
    if(cnt ++ % 50 == 0) {
        printf("Recv Frames %d bytes\n", size);
    }

    return 0;
}

void usage()
{
    printf("./player url(rtsp://192.168.0.1/h264)\n");
}

static char testDoneFlag = 0;

int main(int argc, char **argv)
{
    int thread;
	pthread_t thread_t[100];
	RtspPlayer param[100];
    char playe_done_flag[100] = {0};
	char name[100][128];

    int i = 0;
    
    if (argc < 2)
    {
        usage();
        return 0;
    }

    thread = argc-1;

    printf("thread num %d\n", thread);
    for (i = 0; i < thread; i ++)
    {
		memset(&(param[i]), 0, sizeof(RtspPlayer));
	    sprintf((char *)param[i].url, "%s", (char *)argv[i+1]);
		
		sprintf(name[i],"%s",strrchr(param[i].url,'/')+1);
		param[i].cb = player_callback;
		param[i].arg = (void*)name[i];
		param[i].threadDoneFlag = 0;
		param[i].streamUsingTCP = i&1;
		RtspPlayerStart(&(param[i]));
    }
    i = 0;
    while(i<thread && getchar())
    {
		printf("stop:%s\n",param[i].url);
		RtspPlayerStop(&(param[i]));
        sleep(1);
		i++;
    }

    return 0;    
}

