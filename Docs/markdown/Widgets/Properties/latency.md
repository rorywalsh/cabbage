<a name="latency"><h3 style="padding-top: 40px; margin-top: 40px;"></h3></a>
_____________________________
**latency(val)** Sets the plugin delay compensation in samples. Defaults to ksmps. Setting latency to -1 forces Csound to run at ksmps=1, but does not result in a one sample delay. In this mode, Cabbage performs ‘in-place’ processing. Note this -1 latency is only useful for realtime processing and in almost all cases it is more efficient to use the host’s PDC. Latency can be updated dynamically. The code shown below updates the plugin latency in realtime with a slider that jumps in integer increments from 1 to 8. Updating a plugin's PDC in real time may not be supported by some plugin hosts.   

```csharp
instr UpdateLatency
    kLatencySliderVal, kLatencyTrig cabbageGetValue "latencySlider"
    cabbageSet kLatencyTrig, "form", "latency", pow(2, kLatencySliderVal)
    printf "Setting latency to %d\n", kLatencyTrig, pow(2, kLatencySliderVal)
endin
```
