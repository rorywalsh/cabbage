<Cabbage>
form caption("Combobox Sample Player") size(400, 100), colour(58, 110, 182), pluginid("def1")
combobox bounds(6, 8, 80, 20) channel("combo1") channeltype("string") populate("*.wav", "/home/rory/Downloads/AKWF_bw_perfectwaves/")
button bounds(90, 8, 80, 40) channel("button1") text("Play", "Stop") 
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d 
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1


instr 1
kButton chnget "button1"
if changed:k(kButton) == 1 then
    if kButton == 1 then
        event "i", "PlaySample", 0, -1
    else
        turnoff2 "PlaySample", 0, 0
    endif
endif
endin

instr PlaySample
    Sfilename chnget "combo1"
    iNumChannels filenchnls Sfilename

    if iNumChannels == 1 then
        a1 diskin2 Sfilename, 1, 0, 1
        a2 = a1
    elseif iNumChannels == 2 then
        a1, a2 diskin2 Sfilename, 1, 0, 1
    endif
endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
;starts instrument 1 and runs it for a week
i1 0 [60*60*24*7] 
</CsScore>
</CsoundSynthesizer>
