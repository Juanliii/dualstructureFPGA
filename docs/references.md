# Reference Notes

The scaffold was created from these local documents:

- `双目红外.md`
- `AXU5EVB-E开发板用户手册.pdf`
- `MV-CS013-60GN(1).pdf`
- `奥比中光335.pdf`

## Extracted Constraints

AXU5EVB-E:

- Zynq UltraScale+ MPSoC, XCZU5EV-2SFVC784I.
- PS + PL architecture with four Cortex-A53 cores.
- 4 GB PS DDR4 and 1 GB PL DDR4.
- 4 x USB3.0 host ports.
- 2 x Gigabit Ethernet ports, one attached to PS and one attached to PL.
- PL Ethernet uses RGMII at Gigabit rate.

MV-CS013-60GN:

- 1224 x 1024 resolution.
- Mono8/10/12 pixel formats.
- 59.2 fps at 1224 x 1024 Mono8.
- Gigabit Ethernet, GigE Vision 2.0, GenICam.
- Line0 opto-isolated input, Line1 opto-isolated output, Line2 configurable I/O.
- 9-24 VDC or PoE power.

Gemini 335 USB reference:

- USB 3.0 product family reference.
- Useful as a V1 USB acquisition/reference device.
- Its native modes are documented separately from the common FPGA stereo contract, so V1 software must adapt captured frames into the HLS input format before DMA.

