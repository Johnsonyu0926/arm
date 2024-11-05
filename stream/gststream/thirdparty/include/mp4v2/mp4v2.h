// Filename: mp4v2.h
// Score: 100

#ifndef MP4V2_MP4V2_H
#define MP4V2_MP4V2_H

/*****************************************************************************/

#include <mp4v2/platform.h>
#include <mp4v2/project.h>

/*****************************************************************************/

/* exploit C++ ability of default values for function parameters */
#if defined( DEFAULT )
#   define __MP4V2_SAVE_DEFAULT DEFAULT
#endif
#undef DEFAULT
#if defined( __cplusplus )
#   define DEFAULT(x) =x
#else
#   define DEFAULT(x)
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/

#include <mp4v2/general.h>
#include <mp4v2/file.h>
#include <mp4v2/file_prop.h>
#include <mp4v2/track.h>
#include <mp4v2/track_prop.h>
#include <mp4v2/sample.h>
#include <mp4v2/chapter.h>
#include <mp4v2/itmf_generic.h>
#include <mp4v2/itmf_tags.h>
#include <mp4v2/streaming.h>
#include <mp4v2/isma.h>

/*****************************************************************************/

/* restore macro DEFAULT to state prior to mp4v2 headers */
#undef DEFAULT
#if defined( __MP4V2_SAVE_DEFAULT )
#   define DEFAULT __MP4V2_SAVE_DEFAULT
#endif

#ifdef __cplusplus
} // extern "C"
#endif

/*****************************************************************************/

#endif /* MP4V2_MP4V2_H */

// By GST @Date