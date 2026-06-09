# Shared HLS IP

This directory contains the first Vitis HLS 2021.2 implementation step: a minimal `stereo_pipeline` IP with the complete AXI4-Stream and AXI-Lite interface.

The current kernel is intentionally a placeholder:

- consumes left and right Mono8 input streams;
- emits constant `1000 mm` depth for every pixel;
- emits Q16.16 point cloud coordinates using `X=(u-cx)*Z/fx`, `Y=(v-cy)*Z/fy`, `Z=depth`;
- sets output `TLAST` at end of each image row.

Build from the kernel directory:

```bash
cd kernels/stereo_pipeline
vitis_hls -f run_hls.tcl
```

Or from `common/hls`:

```bash
vitis_hls -f scripts/run_all_csims.tcl
vitis_hls -f scripts/export_all_ips.tcl
```

