#ifndef STEREO_IR_STEREO_TYPES_H
#define STEREO_IR_STEREO_TYPES_H

#include <ap_axi_sdata.h>
#include <ap_int.h>
#include <hls_stream.h>

typedef ap_axiu<8, 1, 1, 1> axis_pixel_t;
typedef ap_axiu<16, 1, 1, 1> axis_depth_t;
typedef ap_axiu<96, 1, 1, 1> axis_point_t;

typedef ap_uint<8> pixel_t;
typedef ap_uint<16> depth_t;
typedef ap_int<32> coord_q16_t;

#endif

