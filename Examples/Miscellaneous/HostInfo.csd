<Cabbage>
form size(440, 320), caption("Host Info"), pluginId("HtIn"), guiRefresh(10)
label bounds(5, 10, 430, 30), text("BPM:"), align("left"), identChannel("BPMLabel"), fontColour("white"), colour(147, 210, 0) 
label bounds(5, 48, 430, 30), text("Playing:"), align("left"), identChannel("IsPlayingLabel"), fontColour("white"), colour(147, 210, 0) 
label bounds(5, 86, 430, 30), text("Recording:"), align("left"), identChannel("IsRecordingLabel"), fontColour("white"), colour(147, 210, 0) 
label bounds(5, 124, 430, 30), text("Time in Seconds:"), align("left"), identChannel("TimeInSecondsLabel"), fontColour("white"), colour(147, 210, 0) 
label bounds(5, 164, 430, 30), text("Time in Samples:"), align("left"), identChannel("TimeInSamplesLabel"), fontColour("white"), colour(147, 210, 0) 
label bounds(5, 202, 430, 30), text("Time Signature:"), align("left"), identChannel("TimeSignatureLabel"), fontColour("white"), colour(147, 210, 0) 
label bounds(5, 240, 430, 30), text("Pulses per quater note:"), align("left"), identChannel("PPQNLabel"), fontColour("white"), colour(147, 210, 0) 

</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d
</CsOptions>
<CsInstruments>
;sr is set by the host
ksmps = 64
nchnls = 2
0dbfs=1
;host info Demo. Rory.

instr 1
;get information from here using reserved channels
kBPM chnget "HOST_BPM"
kIsPlaying chnget "IS_PLAYING"
kIsRecording chnget "IS_RECORDING"
kTimeInSeconds chnget "TIME_IN_SECONDS"
kPPQPosition chnget "HOST_PPQ_POS"
kTimeInSamples chnget "TIME_IN_SAMPLES"
kTimeSigNum chnget "TIME_SIG_NUM"
kTimeSigDenom chnget "TIME_SIG_DENOM"

;print current BPM
if changed:k(kBPM)==1 then
	SMess sprintfk "text(\"BPM: %d\")", kBPM
	chnset SMess, "BPMLabel"
endif
;print is host currently playing
if changed:k(kIsPlaying)==1 then
	SMess sprintfk "text(\"Playing: %d\")", kIsPlaying
	chnset SMess, "IsPlayingLabel"
endif
;print is host currently recording
if changed:k(kIsRecording)==1 then
	SMess sprintfk "text(\"Recording: %d\")", kIsRecording
	chnset SMess, "IsRecordingLabel"
endif
;print current time in seconds of performance
if changed:k(kTimeInSeconds)==1 then
	SMess sprintfk "text(\"Time in Seconds: %d\")", kTimeInSeconds
	chnset SMess, "TimeInSecondsLabel"
endif
;print current time in samples of performance
if changed:k(kTimeInSeconds)==1 then
	SMess sprintfk "text(\"Time in Samples: %d\")", kTimeInSamples
	chnset SMess, "TimeInSamplesLabel"
endif
;current time signature
if changed:k(kTimeSigNum)==1 || changed:k(kTimeSigDenom)==1 then
	SMess sprintfk "text(\"Time Signature: %d/%d\")", kTimeSigNum, kTimeSigDenom
	chnset SMess, "TimeSignatureLabel"
endif
;print current pulse-per-quater note
if changed:k(kPPQPosition)==1 then
	SMess sprintfk "text(\"Pulse per quater note: %d\")", kPPQPosition
	chnset SMess, "PPQNLabel"
endif

endin

</CsInstruments>  
<CsScore>
f1 0 1024 10 1
i1 0 3600
</CsScore>
</CsoundSynthesizer>