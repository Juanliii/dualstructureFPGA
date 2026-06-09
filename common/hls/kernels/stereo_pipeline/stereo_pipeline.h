#ifndef STEREO_IR_STEREO_PIPELINE_H
#define STEREO_IR_STEREO_PIPELINE_H

#include "../../common/stereo_types.h"

void stereo_pipeline(
    hls::stream<axis_pixel_t> &left_stream,
    hls::stream<axis_pixel_t> &right_stream,
    hls::stream<axis_depth_t> &depth_stream,
    hls::stream<axis_point_t> &pointcloud_stream,
    int width,
    int height,
    int min_disp,
    int max_disp,
    int fx_fixed,
    int fy_fixed,
    int cx_fixed,
    int cy_fixed,
    int baseline_fixed
);

#endif

