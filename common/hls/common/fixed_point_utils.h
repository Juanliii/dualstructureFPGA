#ifndef STEREO_IR_FIXED_POINT_UTILS_H
#define STEREO_IR_FIXED_POINT_UTILS_H

#include "stereo_types.h"

static inline coord_q16_t project_coord_q16(int pixel_index, int center_q16, depth_t depth_mm, int focal_q16)
{
    if (focal_q16 == 0) {
        return 0;
    }

    ap_int<64> delta_q16 = ((ap_int<64>)pixel_index << 16) - (ap_int<64>)center_q16;
    ap_int<64> depth_q16 = ((ap_int<64>)depth_mm) << 16;
    ap_int<96> product = (ap_int<96>)delta_q16 * (ap_int<96>)depth_q16;
    return (coord_q16_t)(product / focal_q16);
}

static inline ap_uint<96> pack_point_q16(coord_q16_t x, coord_q16_t y, coord_q16_t z)
{
    ap_uint<96> packed = 0;
    packed.range(31, 0) = (ap_uint<32>)x;
    packed.range(63, 32) = (ap_uint<32>)y;
    packed.range(95, 64) = (ap_uint<32>)z;
    return packed;
}

#endif

