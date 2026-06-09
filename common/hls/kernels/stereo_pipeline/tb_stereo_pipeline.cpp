#include "stereo_pipeline.h"

#include "../../common/camera_params.h"
#include "../../common/hls_config.h"

#include <iostream>

static axis_pixel_t make_pixel(unsigned char value, bool sof, bool eol)
{
    axis_pixel_t word;
    word.data = value;
    word.keep = -1;
    word.strb = -1;
    word.user = sof ? 1 : 0;
    word.last = eol ? 1 : 0;
    word.id = 0;
    word.dest = 0;
    return word;
}

int main()
{
    const int width = 8;
    const int height = 4;

    hls::stream<axis_pixel_t> left_stream;
    hls::stream<axis_pixel_t> right_stream;
    hls::stream<axis_depth_t> depth_stream;
    hls::stream<axis_point_t> pointcloud_stream;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            bool sof = (x == 0 && y == 0);
            bool eol = (x == width - 1);
            left_stream.write(make_pixel((unsigned char)(x + y), sof, eol));
            right_stream.write(make_pixel((unsigned char)(x + y + 1), sof, eol));
        }
    }

    stereo_pipeline(
        left_stream,
        right_stream,
        depth_stream,
        pointcloud_stream,
        width,
        height,
        0,
        64,
        DEFAULT_FX_Q16,
        DEFAULT_FY_Q16,
        4 * STEREO_Q16_ONE,
        2 * STEREO_Q16_ONE,
        DEFAULT_BASELINE_MM_Q16
    );

    int errors = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            axis_depth_t depth_word = depth_stream.read();
            axis_point_t point_word = pointcloud_stream.read();

            if (depth_word.data != STEREO_DEFAULT_DEPTH_MM) {
                ++errors;
            }
            if (depth_word.last != (x == width - 1)) {
                ++errors;
            }
            if (point_word.last != (x == width - 1)) {
                ++errors;
            }

            ap_uint<96> packed = point_word.data;
            ap_int<32> z_q16 = (ap_int<32>)packed.range(95, 64);
            if (z_q16 != (STEREO_DEFAULT_DEPTH_MM << STEREO_Q16_SHIFT)) {
                ++errors;
            }
        }
    }

    if (errors != 0) {
        std::cout << "FAIL: " << errors << " mismatches" << std::endl;
        return 1;
    }

    std::cout << "PASS: stereo_pipeline placeholder csim" << std::endl;
    return 0;
}

