# RTL Notes

V3 RTL owns the low-level receive path:

- `rgmii_rx`
- `eth_mac_rx`
- `eth_frame_parser`
- `ipv4_parser`
- `udp_parser`
- `gvsp_parser`
- `packet_to_axi_stream`
- `sync_timing_ctrl`
- `camera_trigger_gen`
- `projector_trigger_gen`
- `trigger_regs`

These files are placeholders. Replace them with verified RTL, preferably adapted from proven Ethernet/GVSP open-source cores or vendor examples.
