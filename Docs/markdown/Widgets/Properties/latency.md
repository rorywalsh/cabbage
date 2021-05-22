<a name="latency"><h3 style="padding-top: 40px; margin-top: 40px;"></h3></a>
_____________________________
**latency(val)** Sets the plugin delay compensation in samples. Defaults to ksmps. Setting latency to -1 forces Csound to run at ksmps=1, but does not result in a one sample delay. In this mode, Cabbage performs ‘in-place’ processing. Note this -1 latency is only useful for realtime processing and in almost all cases, it is more efficient to use the host’s PDC.  
