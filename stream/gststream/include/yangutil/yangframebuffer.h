// Filename: YangFrameBuffer.h
// Score: 96

#ifndef INCLUDE_YANGUTIL_YANGFRAMEBUFFER_H_
#define INCLUDE_YANGUTIL_YANGFRAMEBUFFER_H_

void yang_frame_copy(YangFrame* src, YangFrame* dst);
void yang_frame_copy_buffer(YangFrame* src, YangFrame* dst);
void yang_frame_copy_nobuffer(YangFrame* src, YangFrame* dst);

#endif /* INCLUDE_YANGUTIL_YANGFRAMEBUFFER_H_ */
// By GST @2024/10/28