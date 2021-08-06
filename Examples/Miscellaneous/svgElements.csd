<Cabbage>
form caption("SVG Elements") size(340, 360), guiMode("queue") pluginId("def1")

;first image acts as bounding box - next images as handles
image bounds(20, 20, 300, 120) channel("envelope1"), colour(0,0,0), outlineColour(40, 40, 40), outlineThickness(4)
image bounds(15, 135, 10, 10) channel("env1BottomLeft") colour("lime"), shape("ellipse")
image bounds(80, 20, 10, 10) channel("env1Handle") colour("lime"), shape("ellipse")
image bounds(315, 135, 10, 10) channel("env1BottomRight") colour("lime"), shape("ellipse")

;first image acts as bounding box - next as handle for curve
image bounds(20, 180, 300, 120) channel("envelope2"), colour(50,50,50), outlineColour(40, 40, 40) outlineThickness(1)
image bounds(80, 290, 10, 10) channel("env2Handle") colour("lime"), shape("ellipse")

</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d 
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
ksmps = 32
nchnls = 2
0dbfs = 1

;constraint handles within bounds boxes
opcode ConstrainPos, k,ki[]Sii
    kValue, iBounds[], SPos, iLimit1, iLimit2 xin
    kReturnVal init 0
    if strcmpk(SPos, "x") == 0 then
        kReturnVal = limit:k(kValue, iBounds[0]+iLimit1, iBounds[0]+iBounds[2]-iLimit2)
    elseif strcmpk(SPos, "y") == 0 then
        kReturnVal = limit:k(kValue, iBounds[1]+iLimit1, iBounds[1]+iBounds[3]-iLimit2)
    endif
    xout kReturnVal
endop

;creates the first SVG - a simple triangle 
opcode CreateSVG1, 0, kSkk
    kTrig, SEnv, kX, kY xin
    iBounds[] cabbageGet SEnv, "bounds"
    
    cabbageSet kTrig, SEnv, "svgElement", sprintfk({{<path d="M 0 %d L %d %d L %d %d" stroke="white" fill="rgba(50, 50, 50)"/>}}, iBounds[3], kX, kY, iBounds[2], iBounds[3])
endop

;create the second SVG - a parametric curve type thingymabob
opcode CreateSVG2, 0, kSkk
    kTrig, SEnv, kX, kY xin
    iBounds[] cabbageGet SEnv, "bounds"
    cabbageSet kTrig, SEnv, "svgElement", sprintfk(\
    {{<path d="M 0 60 L 0 60 L %d 60 C %d 60 %d 60 %d %d C %d 60 %d 60 %d 60 L %d 60 L %d 120 L 0 %d" stroke="rgb(200, 200, 200)" stroke-width="2" fill="rgba(0,0,0)"/>}}, kX-80, kX, kX, kX, iBounds[3]-(iBounds[2]-(kY)), kX, kX, kX+80, iBounds[2]+1, iBounds[2]+1, iBounds[2])
endop

instr 1
    iEnv1Bounds[] cabbageGet "envelope1", "bounds"
    iEnv2Bounds[] cabbageGet "envelope2", "bounds"
    kPosX cabbageGet "MOUSE_X"
    kPosY cabbageGet "MOUSE_Y"
    kMouseDown, kMouseDown_Trig cabbageGet "MOUSE_DOWN_LEFT"
    SChannel, kTrig cabbageGet "CURRENT_WIDGET"
    kInit init 1

    cabbageSet (kMouseDown == 1 && strcmpk(SChannel, "env1Handle") == 0) ? 1 : 0, "env1Handle", "bounds", ConstrainPos(kPosX, iEnv1Bounds, "x", 0, 10), ConstrainPos(kPosY, iEnv1Bounds, "y", 0, 10), 10, 10
    cabbageSet (kMouseDown == 1 && strcmpk(SChannel, "env2Handle") == 0) ? 1 : 0, "env2Handle", "bounds", ConstrainPos(kPosX, iEnv2Bounds, "x", 40, 50), ConstrainPos(kPosY, iEnv2Bounds, "y", 0, 10), 10, 10

    if kInit == 1 then
        CreateSVG1 1, "envelope1", 65, 5
        CreateSVG2 1, "envelope2", 65, 300
    else
        CreateSVG1 (kMouseDown == 1 && strcmpk(SChannel, "env1Handle") == 0) ? 1 : 0, "envelope1", ConstrainPos(kPosX, iEnv1Bounds, "x", 0, 10)-15, ConstrainPos(kPosY, iEnv1Bounds, "y", 0, 10)-15
        CreateSVG2 (kMouseDown == 1 && strcmpk(SChannel, "env2Handle") == 0) ? 1 : 0, "envelope2", ConstrainPos(kPosX, iEnv2Bounds, "x", 40, 50)-15, ConstrainPos(kPosY, iEnv2Bounds, "y", 0, 0)
    endif
    
    kInit = 0
endin


</CsInstruments>
<CsScore>
f1 0 1024 7 0 
i1 0 [60*60*24*7] 
</CsScore>
</CsoundSynthesizer>