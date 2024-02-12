<Cabbage>
form caption("Untitled") size(400, 300), guiMode("queue") pluginId("def1")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
ksmps = 32
nchnls = 2
0dbfs = 1


instr 1
    ;create profile with label 'p1' and test block 'oscil'
    cabbageProfilerStart "P1", "oscil"
    a1 oscil 1, 100
    ;stop block 'oscil' test
    kTime1 cabbageProfilerStop "P1", "oscil"


    ;add a new block 'pvs'
    cabbageProfilerStart "P1", "pvs"
    ifftsize  = 1024
    ioverlap  = ifftsize / 4
    iwinsize  = ifftsize
    iwinshape = 1						
    ain       vco2 1, 1
    fftin     pvsanal ain, ifftsize, ioverlap, iwinsize, iwinshape
    fftblur   pvsblur fftin, 2, 4	
    aout      pvsynth fftblur					
    ;stop block 'oscil' test
    kTime2 cabbageProfilerStop "p1", "pvs"

endin

instr 2
    cabbageProfilerStart "P1", "vco2"
    a1 vco2 1, 100
    kTime1 cabbageProfilerStop "P1", "vco2"


    cabbageProfilerStart "P1", "pvs2"
    ifftsize  = 1024
    ioverlap  = ifftsize / 4
    iwinsize  = ifftsize
    iwinshape = 1							;von-Hann window
    ain       vco2 1, 1
    fftin     pvsanal ain, ifftsize, ioverlap, iwinsize, iwinshape	;fft-analysis of the audio-signal
    fftblur   pvscale fftin, 2					;scale
    aout      pvsynth fftblur					;resynthesis
    kTime2 cabbageProfilerStop "P1", "pvs2"

    ;print data from profiler 'P1'
    cabbageProfilerPrint "P1", metro(1)

endin
</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
;starts instrument 1 and runs it for a week
i1 0 [60*60*24*7] 
i2 0 [60*60*24*7] 
</CsScore>
</CsoundSynthesizer>
