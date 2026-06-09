#include "stereo_pipeline.h"

#include "../../common/camera_params.h"
#include "../../common/fixed_point_utils.h"
#include "../../common/hls_config.h"
#include "../../common/hls_stream_utils.h"

static int sanitize_dimension(int value, int fallback, int limit)
{
    if (value <= 0) {
        return fallback;
    }
    if (value > limit) {
        return limit;
    }
    return value;
}

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
)
{
#pragma HLS INTERFACE axis port=left_stream
#pragma HLS INTERFACE axis port=right_stream
#pragma HLS INTERFACE axis port=depth_stream
#pragma HLS INTERFACE axis port=pointcloud_stream
#pragma HLS INTERFACE s_axilite port=width bundle=CTRL
#pragma HLS INTERFACE s_axilite port=height bundle=CTRL
#pragma HLS INTERFACE s_axilite port=min_disp bundle=CTRL
#pragma HLS INTERFACE s_axilite port=max_disp bundle=CTRL
#pragma HLS INTERFACE s_axilite port=fx_fixed bundle=CTRL
#pragma HLS INTERFACE s_axilite port=fy_fixed bundle=CTRL
#pragma HLS INTERFACE s_axilite port=cx_fixed bundle=CTRL
#pragma HLS INTERFACE s_axilite port=cy_fixed bundle=CTRL
#pragma HLS INTERFACE s_axilite port=baseline_fixed bundle=CTRL
#pragma HLS INTERFACE s_axilite port=return bundle=CTRL

    (void)min_disp;
    (void)max_disp;
    (void)baseline_fixed;

    const int cols = sanitize_dimension(width, STEREO_DEFAULT_WIDTH, STEREO_MAX_WIDTH);
    const int rows = sanitize_dimension(height, STEREO_DEFAULT_HEIGHT, STEREO_MAX_HEIGHT);
    const int fx = (fx_fixed == 0) ? DEFAULT_FX_Q16 : fx_fixed;
    const int fy = (fy_fixed == 0) ? DEFAULT_FY_Q16 : fy_fixed;
    const int cx = (cx_fixed == 0) ? DEFAULT_CX_Q16 : cx_fixed;
    const int cy = (cy_fixed == 0) ? DEFAULT_CY_Q16 : cy_fixed;
    const depth_t depth_mm = STEREO_DEFAULT_DEPTH_MM;
    const coord_q16_t z_q16 = ((coord_q16_t)depth_mm) << STEREO_Q16_SHIFT;

row_loop:
    for (int y = 0; y < rows; ++y) {
    col_loop:
        for (int x = 0; x < cols; ++x) {
#pragma HLS PIPELINE II=1
            axis_pixel_t left_word = left_stream.read();
            axis_pixel_t right_word = right_stream.read();
            (void)left_word;
            (void)right_word;

            const bool sof = (x == 0 && y == 0);
            const bool eol = (x == cols - 1);

            axis_depth_t depth_word;
            depth_word.data = depth_mm;
            set_depth_sidebands(depth_word, sof, eol);
            depth_stream.write(depth_word);

            axis_point_t point_word;
            coord_q16_t x_q16 = project_coord_q16(x, cx, depth_mm, fx);
            coord_q16_t y_q16 = project_coord_q16(y, cy, depth_mm, fy);
            point_word.data = pack_point_q16(x_q16, y_q16, z_q16);
            set_point_sidebands(point_word, sof, eol);
            pointcloud_stream.write(point_word);
        }
    }
}

