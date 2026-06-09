#ifndef STEREO_IR_HLS_STREAM_UTILS_H
#define STEREO_IR_HLS_STREAM_UTILS_H

#include "stereo_types.h"

static inline void set_depth_sidebands(axis_depth_t &word, bool sof, bool eol)
{
    word.keep = -1;
    word.strb = -1;
    word.user = sof ? 1 : 0;
    word.last = eol ? 1 : 0;
    word.id = 0;
    word.dest = 0;
}

static inline void set_point_sidebands(axis_point_t &word, bool sof, bool eol)
{
    word.keep = -1;
    word.strb = -1;
    word.user = sof ? 1 : 0;
    word.last = eol ? 1 : 0;
    word.id = 0;
    word.dest = 0;
}

#endif

