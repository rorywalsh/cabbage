<Cabbage>
form caption("That's a lof of checkboxes!") size(820, 420), guiMode("queue"), pluginId("def1")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d --midi-key-cps=4 --midi-velocity-amp=5
</CsOptions>
<CsInstruments>
ksmps = 32
nchnls = 2
0dbfs = 1

instr 1
    iX, iY init 0
    iWidgetCount init 0
    while iY < 8 do
        while iX < 16 do
            SWidget sprintf "bounds(%d, %d, 50, 50), channel(\"check%d\"), colour:1(%d, %d, %d)", 10+iX*50, 10+iY*50, iWidgetCount, rnd(255), rnd(255), rnd(255)
            cabbageCreate "checkbox", SWidget
            iWidgetCount += 1
            iX += 1
        od
        iX = 0
        iY += 1
    od
    
    kIndex = 0
    kColumn init 0
    kRow = 0 
    
    if metro(2)==1 then        
        while kRow<8 do
            kVal cabbageGetValue sprintfk("check%d", kRow*16+kColumn)
            if kVal == 1 then
                event "i", "Synth", 0, 1, ((8-kRow)+10)*10, .3
            endif  
            kRow = kRow+1            
        od
        kColumn = kColumn<15 ? kColumn+1 : 0		
    endif     
endin

instr Synth
    aEnv expon 1, p3, 0.01
    aOut oscil aEnv, p4
    outs aOut*p5, aOut*p5
endin
</CsInstruments>
<CsScore>
f1 0 4096 10 1 .5 .25 .17
i1 0 z
</CsScore>
</CsoundSynthesizer>
