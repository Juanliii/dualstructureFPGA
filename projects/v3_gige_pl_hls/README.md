# V3 GigE PL HLS FPGA Project

V3 is the low-latency target where camera Ethernet packets enter PL and PS only configures/monitors the system:

```text
2 x MV-CS013-60GN GigE IR cameras
-> PL RGMII / Ethernet MAC RX
-> IPv4 / UDP / GVSP parser
-> packet-to-pixel stream
-> shared HLS stereo_pipeline IP
-> depth stream + pointcloud stream
-> PS status/control
```

V3 should also move camera synchronization and infrared laser/speckle projector timing into PL:

```text
PL trigger_controller
-> left_camera_trigger
-> right_camera_trigger
-> laser/projector_enable
-> exposure_window / frame_id
```

## Project Assumptions

- Board: AXU5EVB-E, XCZU5EV-2SFVC784I.
- Tools: Vivado/Vitis/Vitis HLS 2021.2.
- PL Ethernet path uses the AXU5EVB-E PL Gigabit Ethernet interface.
- Camera format: `1224 x 1024 Mono8 @ 30 fps`.
- HLS IP is shared from `../../common/hls`.
- RTL owns Ethernet MAC RX, frame parsing, UDP/GVSP parsing, packet order checking, and frame reconstruction.
- RTL also owns deterministic camera trigger timing and laser/projector enable timing.

## PL Synchronization Scope

The trigger controller should be part of the V3 PL timing domain:

- generate 30 fps or configurable frame periods;
- drive left/right camera trigger pulses with configurable skew;
- drive laser/projector enable with configurable lead/lag and on-time;
- expose exposure-window and frame-id timing to the receive pipeline;
- count triggers, frames, dropped packets, malformed packets, and frame mismatches;
- expose AXI-Lite control/status registers to PS.

Typical timing:

```text
t0: laser/projector enable asserted
t0 + laser_lead_us: left/right camera trigger asserted
t0 + laser_lead_us + exposure_us: exposure window ends
t0 + laser_on_us: laser/projector enable deasserted
t0 + frame_period_us: next frame starts
```

The FPGA should not directly drive unknown camera or laser electrical loads. Add the required level shifting, isolation, MOSFET/driver stage, polarity control, and safety interlock. Laser/projector output should default to off after reset.

## First Build Steps

```bash
cd ../../common/hls
vitis_hls -f scripts/run_all_csims.tcl
vitis_hls -f scripts/export_all_ips.tcl
```

Then build the V3 Vivado design after replacing the RTL placeholders with working Ethernet/GVSP modules.

## Open Source Code Candidates

- Ethernet MAC/RGMII references: LiteEth, verilog-ethernet, or vendor example MACs.
- Packet parsing style: open-source UDP/IP FPGA examples, adapted carefully to AXI4-Stream.
- PS control: Xilinx generated HLS drivers, AXI-Lite register access examples.

Do not start V3 by hand-writing stereo matching RTL. Keep stereo/depth/point cloud in HLS and spend RTL effort on deterministic packet capture, stream reconstruction, and synchronized camera/projector timing.
