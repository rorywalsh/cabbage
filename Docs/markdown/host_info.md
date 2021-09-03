# Getting host information into Csound

There will be times when your plugin will need to ask the host for certain information. Like when a track starts playing for example, or what the current tempo is. Cabbage uses reserved channels to retrieve this information. The reserved channel relating to host information are:

**HOST_BPM** Retrieve the currents host bpm. This will be updated whenever the host BPM changes.

**IS_PLAYING** Pressing play on the host transport controls will cause this channel to send a 1. Hitting stop will send a 0.

**IS_RECORDING** As above only for monitoring the record status of a session.

**TIME_IN_SECONDS** Return the current time in seconds from the start of the track.

**TIME_IN_SAMPLES** Return the current time in samples from the start of the track.

**TIME_SIG_DENOM** The signature denominator, e.g. the 4 of a 3/4 time sig 

**TIME_SIG_NUM** The signature numerator, e.g. the 3 of a 3/4 time sig 

**HOST_PPQ_POS** Return the position of the start of the last bar, in pulses-per-quarter-note. 

All the host information can be found using the `chnget` opcode. 

```csharp
kBPM chnget "HOST_BPM"
kIsPlaying chnget "IS_PLAYING"
kIsRecording chnget "IS_RECORDING"
kTimeInSeconds chnget "TIME_IN_SECONDS"
kPPQPosition chnget "HOST_PPQ_POS"
kTimeInSamples chnget "TIME_IN_SAMPLES"
kTimeSigNum chnget "TIME_SIG_NUM"
kTimeSigDenom chnget "TIME_SIG_DENOM"
```
Csound will update these values on each k-cycle. In most case this is every 32 samples. If you need a more immediate update lower ksmps, but note this may cause knock on affects on other parts of your instrument. 

Note that you need to be careful when querying these values. Some host will not broadcast this information until it starts playing. This can lead to a division by 0 errors. If you wish to use the host BPM to set say a delay time, you should wrap it in a `divz` opcode, i.e, 

`kDelay = divz(60, kBPM, 1)`



The [HostInfo.csd](https://github.com/rorywalsh/cabbage/blob/master/Examples/Instructional/HostInfo.csd) file in the Cabbage examples shows all the current information that can be retrieved in Csound. 

![Host info](images/host_info.gif)

