// Filename: YangStreamBase.h
// Score: 98

#ifndef YANGSTREAM_INCLUDE_YANGSTREAMBASE_H_
#define YANGSTREAM_INCLUDE_YANGSTREAMBASE_H_

#include <yangstream/YangStream.h>

// Function to create a stream handle
void yang_create_streamHandle(YangStreamHandle* streamHandle, YangStreamConfig* streamconfig, YangAVInfo* pcontext);

// Function to destroy a stream handle
void yang_destroy_streamHandle(YangStreamHandle* stream);

#endif /* YANGSTREAM_INCLUDE_YANGSTREAMBASE_H_ */
// By GST @2024/10/28