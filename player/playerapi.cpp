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

#include "MediaSink.hh"
#include "playerapi.h"
#include "BasicTaskSchedulerEpoll.h"
#include "playerEnv.h"
#include "rtspClient.h"
#include "DummySink.h"
#include <pthread.h>

#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"

void* player_run(void* param);

// Forward function definitions:
class player
{
public:
    player(RtspPlayer* p_param);
    virtual ~player();
private:
	RtspPlayer* p_play_param;
	pthread_t      play_pid;
};

player::player(RtspPlayer* p_param)
        : p_play_param(p_param),play_pid(0){
	pthread_create(&play_pid, NULL, player_run, (void*)p_play_param);
}

player::~player() {
	printf("stop %s begin\n",p_play_param->url);
	pthread_join(play_pid,NULL);
	printf("stop %s finish\n",p_play_param->url);
}

int RtspPlayerStart(RtspPlayer* p_param)
{
	if(NULL == p_param) {
		return -1;
	}
	p_param->threadDoneFlag = 0;
	player *phandle = new player(p_param);
	p_param->playhandle = (void *)phandle;
	return 0;
}

int  RtspPlayerStop(RtspPlayer *p_param)
{
	p_param->threadDoneFlag = ~0;
	player *phandle = (player *)(p_param->playhandle);
	delete phandle;
	p_param->playhandle = NULL;
	return 0;
}

void* player_run(void* param) 
{
  // Begin by setting up our usage environment:  
  RtspPlayer *p_param = (RtspPlayer *)param;
  TaskScheduler* scheduler = BasicTaskScheduler::createNew();
  if(NULL == scheduler || NULL == p_param) {
	printf("NULL point\n");
	return NULL;
  }
  UsageEnvironment* env = BasicUsageEnvironment::createNew(*scheduler);
  // Begin by creating a "RTSPClient" object.  Note that there is a separate "RTSPClient" object for each stream that we wish
  // to receive (even if more than stream uses the same "rtsp://" URL).
  ourRTSPClient* rtspClient = ourRTSPClient::createNew(*env, p_param, RTSP_CLIENT_VERBOSITY_LEVEL, "liveVideoStream");
  if (rtspClient == NULL) {
    printf( "Failed to create a RTSP client for URL %s :%s\n" ,p_param->url,  env->getResultMsg() );
	env->reclaim();env = NULL;
  	delete scheduler;scheduler = NULL;
    return NULL;
  }

  openURL((RTSPClient *)rtspClient);

  // All subsequent activity takes place within the event loop:
  env->taskScheduler().doEventLoop(&(p_param->threadDoneFlag));
    // This function call does not return, unless, at some point in time, "eventLoopWatchVariable" gets set to something non-zero.
  shutdownStream(rtspClient);
  env->reclaim();env = NULL;
  delete scheduler;scheduler = NULL;
  
  printf("=========play thread run over=========\n");
  return NULL;
}

