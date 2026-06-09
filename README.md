# Stereo IR FPGA Projects

This workspace contains three FPGA project skeletons for the AXU5EVB-E Zynq UltraScale+ MPSoC board and stereo infrared depth processing.

## Projects

| Project | Input path | Capture owner | PL/HLS role |
| --- | --- | --- | --- |
| `projects/v1_usb_ps_hls` | USB stereo/USB depth reference camera | PS Linux | DMA into shared HLS stereo pipeline |
| `projects/v2_gige_ps_hls` | Two MV-CS013-60GN GigE IR cameras | PS Linux | DMA into shared HLS stereo pipeline |
| `projects/v3_gige_pl_hls` | Two MV-CS013-60GN GigE IR cameras | PL Ethernet/GVSP | Direct AXI4-Stream into shared HLS stereo pipeline |

Default image format for the stereo pipeline is `1224 x 1024`, `Mono8`, `30 fps`, left and right streams.

## Shared Hardware Assumptions

- Board: AXU5EVB-E with XCZU5EV-2SFVC784I.
- Toolchain: Vivado/Vitis/Vitis HLS 2021.2.
- PS memory: 4 GB DDR4.
- PL memory: 1 GB DDR4.
- Useful board I/O: 4 x USB3.0 host, 1 x PS Gigabit Ethernet, 1 x PL Gigabit Ethernet, DisplayPort, UART, SD card.
- V2/V3 camera: MV-CS013-60GN, 1224 x 1024 Mono8, up to 59.2 fps, GigE Vision 2.0 / GenICam.
- V1 USB reference: Orbbec Gemini 335 USB 3.0 can be used as a USB depth/stereo reference path, while the FPGA HLS path keeps the common 1224 x 1024 Mono8 contract for this project.

## Layout

```text
stereo_ir_fpga/
├── common/
│   ├── hls/                  # shared Vitis HLS stereo_pipeline IP
│   └── software/             # shared Linux register/control headers
├── docs/
│   └── references.md         # source document summary
└── projects/
    ├── v1_usb_ps_hls/
    ├── v2_gige_ps_hls/
    └── v3_gige_pl_hls/
```

Start HLS C simulation from the shared IP:

```bash
cd stereo_ir_fpga/common/hls/kernels/stereo_pipeline
vitis_hls -f run_hls.tcl
```

