// Filename: YangBuffer.h
// Score: 95

#ifndef INCLUDE_YANGUTIL_BUFFER_YANGCBUFFER_H_
#define INCLUDE_YANGUTIL_BUFFER_YANGCBUFFER_H_

#include <yangutil/yangtype.h>

// Buffer structure
typedef struct {
    char* head;  // current position at bytes
    char* data;  // the bytes data for buffer to read or write
    int32_t size;  // the total number of bytes
} YangBuffer;

// Initialize buffer
void yang_init_buffer(YangBuffer* buf, char* b, int32_t nn);

// Destroy buffer
void yang_destroy_buffer(YangBuffer* buf);

// Get current position in buffer
int32_t yang_buffer_pos(YangBuffer* buf);

// Get left bytes in buffer, total size minus the current pos
int32_t yang_buffer_left(YangBuffer* buf);

// Check if buffer is empty
int32_t yang_buffer_empty(YangBuffer* buf);

// Check if buffer can supply required size of bytes
int32_t yang_buffer_require(YangBuffer* buf, int32_t required_size);

// Skip bytes in buffer
void yang_buffer_skip(YangBuffer* buf, int32_t size);

// Write 1 byte char to buffer
void yang_write_1bytes(YangBuffer* buf, char value);

// Write 2 bytes int32_t to buffer
void yang_write_2bytes(YangBuffer* buf, int16_t value);
void yang_write_le2bytes(YangBuffer* buf, int16_t value);

// Write 4 bytes int32_t to buffer
void yang_write_4bytes(YangBuffer* buf, int32_t value);
void yang_write_le4bytes(YangBuffer* buf, int32_t value);

// Write 3 bytes int32_t to buffer
void yang_write_3bytes(YangBuffer* buf, int32_t value);
void yang_write_le3bytes(YangBuffer* buf, int32_t value);

// Write 8 bytes int32_t to buffer
void yang_write_8bytes(YangBuffer* buf, int64_t value);
void yang_write_le8bytes(YangBuffer* buf, int64_t value);

// Write bytes to buffer
void yang_write_bytes(YangBuffer* buf, char* data, int32_t size);
void yang_write_cstring(YangBuffer* buf, char* data);

// Read 1 byte char from buffer
char yang_read_1bytes(YangBuffer* buf);

// Read 2 bytes int32_t from buffer
int16_t yang_read_2bytes(YangBuffer* buf);
int16_t yang_read_le2bytes(YangBuffer* buf);

// Read 3 bytes int32_t from buffer
int32_t yang_read_3bytes(YangBuffer* buf);
int32_t yang_read_le3bytes(YangBuffer* buf);

// Read 4 bytes int32_t from buffer
int32_t yang_read_4bytes(YangBuffer* buf);
int32_t yang_read_le4bytes(YangBuffer* buf);

// Read 8 bytes int32_t from buffer
int64_t yang_read_8bytes(YangBuffer* buf);
int64_t yang_read_le8bytes(YangBuffer* buf);

// Read bytes from buffer
void yang_read_bytes(YangBuffer* buf, char* data, int32_t size);

// Read 2 bytes from char buffer
uint16_t yang_readchar_2bytes(char* buf);

// Read 4 bytes from char buffer
uint32_t yang_readchar_4bytes(char* buf);

#endif /* INCLUDE_YANGUTIL_BUFFER_YANGCBUFFER_H_ */
// By GST @2024/10/28