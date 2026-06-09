# V1 USB PS HLS FPGA Project

V1 proves the basic FPGA acceleration path:

```text
USB stereo/reference camera
-> PS Linux capture
-> PS DDR buffers
-> AXI DMA / AXI4-Stream
-> shared HLS stereo_pipeline IP
-> depth stream + pointcloud stream
-> PS result readback
```

## Project Assumptions

- Board: AXU5EVB-E, XCZU5EV-2SFVC784I.
- Tools: Vivado/Vitis/Vitis HLS 2021.2.
- Input target passed to HLS: `1224 x 1024 Mono8 @ 30 fps x 2`.
- USB reference device can be an Orbbec Gemini 335 or another USB stereo/IR source. Its native USB modes must be converted in PS software to the common HLS input contract.
- HLS IP is shared from `../../common/hls`.

## Directory Map

```text
v1_usb_ps_hls/
├── configs/project.yaml
├── hls/README.md
├── vivado/
├── vitis/
├── software/
└── rtl/
```

## First Build Steps

Run HLS C simulation:

```bash
cd ../../common/hls
vitis_hls -f scripts/run_all_csims.tcl
```

Export the shared IP:

```bash
cd ../../common/hls
vitis_hls -f scripts/export_all_ips.tcl
```

The Vivado and Vitis scripts are scaffolds for 2021.2. After creating a board-specific block design and exporting an XSA:

```bash
cd vivado
vivado -mode batch -source create_project.tcl
vivado -mode batch -source build_bitstream.tcl
vivado -mode batch -source export_xsa.tcl

cd ../vitis
xsct scripts/create_platform.tcl
xsct scripts/create_app.tcl
xsct scripts/build_app.tcl
```

## Open Source Code Candidates

- Linux capture path: V4L2 examples and libusb-style capture utilities.
- Buffer/control path: Xilinx AXI DMA Linux examples and Xilinx generated HLS driver headers.
- Image validation: OpenCV on PS or host PC for raw/depth visualization.

Keep the first acceptance target small: consume two input streams and verify constant depth plus Q16.16 point cloud output before adding real stereo matching.

