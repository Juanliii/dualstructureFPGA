# RTL Notes

V1 should keep RTL small:

- AXI-Lite register glue if the generated HLS driver is not enough.
- AXI4-Stream adapters around DMA.
- Reset/clock synchronizers.

Stereo depth logic remains in Vitis HLS.

