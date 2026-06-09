# RTL Notes

V2 RTL should stay limited to AXI stream adapters, trigger timing, and optional GPIO/interrupt glue.

GigE Vision packet acquisition is still in PS Linux for V2.

Trigger timing belongs in PL:

- frame-period counter;
- left/right camera trigger generators;
- laser/projector enable generator;
- exposure-window marker;
- AXI-Lite control/status registers;
- frame and error/status counters.
