<Cabbage>
form caption("Rercord to file") size(400, 100), pluginId("FlRe")
button bounds(104, 16, 163, 40), channel("record"), text("Record to disk", "Stop recording to disk"), 
label bounds(22, 68, 336, 23) channel("label1"), fontColour("white"), text("Press to record sample to disk")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d 
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
ksmps = 16
nchnls = 2
0dbfs = 1

;instrument will be triggered by keyboard widget
instr 1
   
    kOct init 0
    kRand randi 3, 100
    if metro(1) == 1 then
        kOct = int(kRand)
    endif

    kPw1 jspline .25, .05, .1
    kPw2 jspline .25, .05, .01
    kArp oscil 1, 1, 2
    aOut1 vco2 1/2, cpsmidinn(kArp), 2, .5+kPw1
    aOut2 vco2 1/2, cpsmidinn(kArp+kOct*12), 2, .5+kPw2
    
    if kArp > 0 then
        outs (aOut1+aOut2)/2, (aOut1+aOut2)/2
    endif
    
    if changed(chnget:k("record")) == 1 then
        if chnget:k("record") == 1 then
            event "i", 1000, 0, -1
        else
            turnoff2 1000, 0, 0
        endif
    endif
    
endin

instr 1000
    prints "Starting recording\n"
    aL, aR monitor
    itim     date
    Stim     dates     itim
    Syear    strsub    Stim, 20, 24
    Smonth   strsub    Stim, 4, 7
    Sday     strsub    Stim, 8, 10
    iday     strtod    Sday
    Shor     strsub    Stim, 11, 13
    Smin     strsub    Stim, 14, 16
    Ssec     strsub    Stim, 17, 19
    Sfilname  sprintf  "%s_%s_%02d_%s_%s_%s.wav", Syear, Smonth, iday, Shor,Smin, Ssec
    fout Sfilname, 4 ,aL, aR
endin

</CsInstruments>
<CsScore>
;melody
f2 0 8 -2 60 0 60 67 56 0 67 0 
;progression
f3 0 8 -2 1 0 1 2 3 2 0 2 

;causes Csound to run for about 7000 years...
i1 0 z 1 1
</CsScore>
</CsoundSynthesizer>
