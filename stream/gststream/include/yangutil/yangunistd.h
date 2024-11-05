// Filename: YangUnistd.h
// Score: 97

#ifndef INCLUDE_YANGUTIL_YANGUNISTD_H_
#define INCLUDE_YANGUTIL_YANGUNISTD_H_

#ifdef _MSC_VER
#include <Windows.h>
#define yang_usleep(x) Sleep((x) > 1000 ? (x) / 1000 : 1)
#define yang_sleep(x) Sleep(1000 * (x))
#else
#include <unistd.h>
#define yang_usleep usleep
#define yang_sleep sleep
#endif

#endif /* INCLUDE_YANGUTIL_YANGUNISTD_H_ */
// By GST @2024/10/28