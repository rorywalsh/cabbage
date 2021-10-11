<Cabbage>
form caption("SVG Table") size(600, 350), guiMode("queue") pluginId("def1")
image bounds(0, 0, 600, 300) channel("table1"), colour(40,40,40,0)
button bounds(8, 304, 80, 40) channel("draw"), text("Draw Table")
hslider bounds(92, 296, 257, 39) text("Resolution") textColour("white"), channel("resolution") range(0, .05, .01, 1, 0.001)
label bounds(96, 328, 251, 12) channel("label1"), align("left") fontColour(255, 255, 255, 255), text("Number of points drawn: 4325")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
ksmps = 32
nchnls = 2
0dbfs = 1

instr 1
    kDraw, kTrig cabbageGetValue "draw"
    kRes, kResTrig cabbageGetValue "resolution"
    cabbageSet kResTrig, "label1", sprintfk("text(\"Number of points drawn: %d\")", 131072*chnget:k("resolution"))
    if kTrig == 1 then           
        event "i", "UpdateTable", 0, 1
    endif
    
endin

instr UpdateTable
    gi1 ftgen 1, 0, 131072, 10, rnd(1), rnd(1), rnd(1), rnd(1), rnd(1), rnd(1), rnd(1), rnd(1) 
    iCnt init 0
    iAmp init 1
    iSampleIncr = 131072/(131072*chnget:i("resolution"))

    iBounds[] cabbageGet "table1", "bounds"
    iLength = ftlen(1)
    print 131072*chnget:i("resolution")
    SPath init ""
    SPath strcat SPath, sprintfk("<path d=\"M0 %d ", iBounds[3]/2)   
               
    while iCnt < iLength do
        iAmp = (table:i(iCnt, 1)+1)*.5*iBounds[3]*.99
        SPath strcat SPath, sprintf("L%d %d ", iCnt/iLength * iBounds[2], iBounds[3]-iAmp)     
        iCnt += iSampleIncr
    od        
    
    SPath strcat SPath, sprintf({{
        L%d %d" style="fill:none;stroke:lime;stroke-width:2;stroke-linejoin:round;"/>
        }}, iBounds[2], iBounds[3]/2)

    cabbageSet "table1", "svgElement", SPath
    
endin

</CsInstruments>
<CsScore>
f1 0 131072 10 1 .5 .2 1 .2 .4 .5 .3 .4
i1 0 [60*60*24*7] 
</CsScore>
</CsoundSynthesizer>
