// Implementation of "DummySink":
#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "DummySink.h"
// Even though we're not going to be doing anything with the incoming data, we still need to receive it.
// Define the size of the buffer that we'll use:

DummySink* DummySink::createNew(UsageEnvironment& env, sink_player_data_cb cb, void *arg,MediaSubsession& subsession, char const* streamId, unsigned buffersize) {
  return new DummySink(env, cb, arg, subsession, streamId, buffersize);
}

DummySink::DummySink(UsageEnvironment& env, sink_player_data_cb cb, void *arg,
					MediaSubsession& subsession, char const* streamId, unsigned size)
  : MediaSink(env),callback(cb),userArg(arg),fSubsession(subsession) {
  if(size == 0)
    fBuffersize = DUMMY_SINK_RECEIVE_BUFFER_SIZE;
  else
    fBuffersize = size;
  printf("fBuffersize=%d\n",fBuffersize);

  char buf[4] = {0x00, 0x00, 0x00, 0x01};
  fStreamId = strDup(streamId);
  fReceiveBuffer = new u_int8_t[fBuffersize + 4];
  memcpy(fReceiveBuffer, buf, 4);
  this->callback = cb;
  this->userArg = arg;

}

DummySink::~DummySink() {
  delete[] fReceiveBuffer;
  delete[] fStreamId;
}

void DummySink::afterGettingFrame(void* clientData, unsigned frameSize, unsigned numTruncatedBytes,
				  struct timeval presentationTime, unsigned durationInMicroseconds) {
  DummySink* sink = (DummySink*)clientData;
  sink->afterGettingFrame(frameSize, numTruncatedBytes, presentationTime, durationInMicroseconds);
}

// If you don't want to see debugging output for each received frame, then comment out the following line:
//#define DEBUG_PRINT_EACH_RECEIVED_FRAME 1

void DummySink::afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes,
				  struct timeval presentationTime, unsigned /*durationInMicroseconds*/) {
  // We've just received a frame of data.  (Optionally) print out information about it:
  //if(callback)
  callback(fReceiveBuffer, frameSize + 4, userArg);
  // Then continue, to request the next frame of data:
  continuePlaying();
}

Boolean DummySink::continuePlaying() {
  if (fSource == NULL) return False; // sanity check (should not happen)

  // Request the next frame of data from our input source.  "afterGettingFrame()" will get called later, when it arrives:
  fSource->getNextFrame(fReceiveBuffer + 4, fBuffersize,
                        afterGettingFrame, this,
                        onSourceClosure, this);
  return True;
}
