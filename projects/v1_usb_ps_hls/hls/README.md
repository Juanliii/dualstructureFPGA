# HLS Usage

This project uses the shared `stereo_pipeline` HLS IP in `../../common/hls`.

For V1, the PS captures USB frames, arranges left/right Mono8 buffers, and sends them into PL through DMA. No V1-specific HLS algorithm fork is planned at this stage.

