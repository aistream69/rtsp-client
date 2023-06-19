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

 
#ifndef __DUMMY_SINK_H__
#define __DUMMY_SINK_H__

#include "MediaSink.hh"

// Define a data sink (a subclass of "MediaSink") to receive the data for each subsession (i.e., each audio or video 'substream').
// In practice, this might be a class (or a chain of classes) that decodes and then renders the incoming audio or video.
// Or it might be a "FileSink", for outputting the received data into a file (as is done by the "openRTSP" application).
// In this example code, however, we define a simple 'dummy' sink that receives incoming data, but does nothing with it.
#define DUMMY_SINK_RECEIVE_BUFFER_SIZE 800000//100000

typedef int (* sink_player_data_cb) (unsigned char * p_buf, int size, void * param);

class DummySink: public MediaSink {
public:
  static DummySink* createNew(UsageEnvironment& env,sink_player_data_cb cb, void *arg,
			      MediaSubsession& subsession, // identifies the kind of data that's being received
			      char const* streamId = NULL,unsigned buffersize = DUMMY_SINK_RECEIVE_BUFFER_SIZE); // identifies the stream itself (optional)

private:
  DummySink(UsageEnvironment& env,sink_player_data_cb cb, void *arg, MediaSubsession& subsession, char const* streamId, unsigned buffersize);
    // called only by "createNew()"
  virtual ~DummySink();

  static void afterGettingFrame(void* clientData, unsigned frameSize,
                                unsigned numTruncatedBytes,
				struct timeval presentationTime,
                                unsigned durationInMicroseconds);
  void afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes,
			 struct timeval presentationTime, unsigned durationInMicroseconds);

private:
  // redefined virtual functions:
  virtual Boolean continuePlaying();

private:
  sink_player_data_cb callback;
  void *              userArg;
  u_int8_t* fReceiveBuffer;
  unsigned int     fBuffersize;
  MediaSubsession& fSubsession;
  char* fStreamId;
};

#endif /* __PLAYER_SINK_H__ */
