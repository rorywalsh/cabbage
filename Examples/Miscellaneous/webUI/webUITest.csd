<Cabbage>
form size(800, 650), caption("Webview Examples"), pluginId("web2")
webview bounds(0, 0, 800, 650), channel("webview1"), mountPoint(".")
rslider bounds(-200, -680, 100, 100), range(0, 100, 0, 1, 1), channel("slider1")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -m0d
</CsOptions>
<CsInstruments>
nchnls = 2
0dbfs = 1
ksmps = 32


instr 1
    seed 1
    kPage cabbageGetValue "currentPage"
    printk2 kPage
    kNoteIndex init 0
    kNotes[] fillarray 60, 65, 0, 62, 0, 68, 0, 65
    aSignal randi 1, 200
    kSeqCount init 0
    kTempo = 6
    
    if kPage == 2 then ;------------------------------------------------------------ Updating slider page

        aSig oscili 1, 440
        kGain oscili 1, .25
        outs aSig*kGain, aSig*kGain
        cabbageWebSend metro(30), "webview1", "webUISlider", abs(kGain*100)
    
    elseif kPage == 3 then ;-------------------------------------------------------- Note array page
    
        cabbageWebSendArray changed:k(kPage), "webview1", "webUIDrawNoteArray", kNotes
        if metro(6) = 1 then
            if kNotes[kNoteIndex] > 0 then
                event "i", "NoteSynth", 0, 1/4, 0.3, kNotes[kNoteIndex]
                event "i", "NoteSynth", (1/kTempo)*4, 0.1, 1/4, kNotes[kNoteIndex]
            endif
            kNoteIndex = (kNoteIndex < 7 ? kNoteIndex+1 : 0)
            kSeqCount = (kNoteIndex == 0 ? kSeqCount+1 : kSeqCount) 
            if changed:k(kSeqCount) == 1 && (kSeqCount % 2 == 0) then
                kCnt = 0
                while kCnt < 8 do
                    kNotes[kCnt] = (randh:k(1000, 1000) > 0 ? 60+int(random:k(-12, 12)) : 0)
                    kCnt+=1
                od
                cabbageWebSendArray 1, "webview1", "webUIDrawNoteArray", kNotes
            endif
        endif
        
    elseif kPage == 4 then ;-------------------------------------------------------- Table update page
        
        k1 jspline 200, 0.01, .02
        k2 jspline 200, 0.02, .03
        k3 jspline 200, 0.03, .04
        k4 jspline 200, 0.04, .05
        k5 jspline 200, 0.05, .06
        
        if metro(10) == 1 then
            event "i", "UpdateTable", 0, 0.1, k1, k2, k3, k4, k5
        endif

        a1 oscili .1, k1, 1
        a2 oscili .1, k2, 1
        a3 oscili .1, k3, 1
        a4 oscili .1, k4, 1
        a5 oscili .1, k5, 1
        outall a1+a2+a3+a4+a5    
    
    elseif kPage == 5 then ;-------------------------------------------------------- Scrolling waveform page
            
        aSig vco2 abs(jspline(1, .1, 1))+.1, randh:k(200, 5)
        aSig moogladder aSig, 100+abs(jspline:k(1000, .5, .1)), .5
        outall aSig
        cabbageWebSend metro(30), "webview1", "webUIDrawScrollingWaveform", k(aSig)
        
    elseif kPage == 6 then ;-------------------------------------------------------- Sending data to Csound page
            
        aSig oscili .5, cabbageGetValue:k("slider1")*5
        outall aSig
        
    elseif kPage == 7 then ;-------------------------------------------------------- XY Controller
            
        aMod oscili .5, cabbageGetValue:k("x")*5
        aSig oscili aMod, cabbageGetValue:k("y")*5
        outall aSig
        
    endif

    ;------------------------------------------------------------ 
    
endin

instr NoteSynth
    kEnv madsr 0.01, 0, 1, .01
    aSig vco2 kEnv*p4, cpsmidinn(p5), 12 
    outs aSig, aSig
endin

instr UpdateTable
    iTable	ftgen	99, 0,   2048, 10, p4, p5, p6, p7, p8
    cabbageWebSendTable 1, "webview1", "webUIDrawTable", 99
endin

</CsInstruments>
<CsScore>
f1 0 1024 10 1 0.01 0.4 0.1 0.01
f99 0 2048 10 1 ;dummy table for display purposes
i1 0 z

</CsScore>
</CsoundSynthesizer>
