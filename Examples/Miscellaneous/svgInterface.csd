<Cabbage>
form caption("SVG GUI") size(560, 300), colour(60, 60, 60),  guiMode("queue") pluginId("def1")
rslider bounds(10, 30, 100, 100) channel("slider1")  range(0, 100, 10, 1, 0.001), alpha(0)
rslider bounds(120, 30, 100, 100) channel("slider2")  range(0, 100, 20, 1, 0.001), alpha(0)
rslider bounds(230, 30, 100, 100) channel("slider3")  range(0, 100, 30, 1, 0.001), alpha(0)
rslider bounds(340, 30, 100, 100) channel("slider4")  range(0, 100, 40, 1, 0.001), alpha(0)
rslider bounds(450, 30, 100, 100) channel("slider5")  range(0, 100, 40, 1, 0.001), alpha(0)


button bounds(10, 138, 100, 40) channel("button1"), alpha(0), text("Start", "Stop")
button bounds(120, 138, 100, 40) channel("button2"), alpha(0), text("Start", "Stop")
button bounds(230, 138, 100, 40) channel("button3"), alpha(0), text("Start", "Stop")
button bounds(340, 138, 100, 40) channel("button4"), alpha(0), text("Start", "Stop")
button bounds(450, 138, 100, 40) channel("button5"), alpha(0), text("Start", "Stop")
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

;UDO for generating SVG-like rgb string
opcode rgbString, S,iii
    iR, iG, iB xin
    SColourString sprintf {{rgb(%d, %d, %d)}}, iR, iG, iB
    xout SColourString
endop

;UDO which palces an image on top of a slider, and then uses the slider to rotate the image
opcode svgSlider, k,Siii
    SSliderChannel, iR, iG, iB xin
    kInit init 1
    iBounds[] cabbageGet SSliderChannel, "bounds"
    iRange[] cabbageGet SSliderChannel, "range"
    SImageChannel sprintf "%s_image", SSliderChannel    
    cabbageCreate "image", sprintf({{bounds(%d, %d, %d, %d) colour(0, 0, 0, 0), mouseInteraction(0) channel("%s")}}, iBounds[0], iBounds[1], iBounds[2], iBounds[3], SImageChannel)
    kSliderValue, kTrig cabbageGetValue SSliderChannel
    kPos scale kSliderValue, -50, 230, iRange[0], iRange[1]
    iHalfSizePos = iBounds[2]/2
    cabbageSet kTrig + kInit, SImageChannel, "svgElement", sprintfk({{
        <g transform="rotate(%d, %d, %d)">
        <circle style="fill:%s;stroke:#000000;stroke-width:3;stroke-dashoffset:0" cx="%d" cy="%d" r="%d" />
        <circle style="fill:%s;stroke:#000000;stroke-width:3;stroke-dashoffset:0" cx="%d" cy="%d" r="%d" />
        <rect style="fill:%s;stroke:#000000;stroke-width:3;stroke-dashoffset:0" width="%d" height="%d" x="%d" y="%d" />
        </>
    }}, kPos, iHalfSizePos, iHalfSizePos, rgbString(iR*.65, iG*.65, iB*.65), iHalfSizePos, iHalfSizePos, iHalfSizePos*.9, rgbString(iR*.45, iG*.45, iB*.45), iHalfSizePos, iHalfSizePos, iHalfSizePos*.5, rgbString(iR, iG, iB), iHalfSizePos*.65, iHalfSizePos*.3, 0, iHalfSizePos*.85)
    kInit = 0    
    xout kSliderValue
endop

;UDO which draws an image on top of a button
opcode svgButton, k, Siii
    SButtonChannel, iR, iG, iB xin
    SImageChannel sprintf "%s_image", SButtonChannel 
    SLabelChannel sprintf "%s_label", SButtonChannel  
    SText[] cabbageGet SButtonChannel, "text"
    kValue, kTrig cabbageGetValue SButtonChannel
    kInit init 1
    iBounds[] cabbageGet SButtonChannel, "bounds"    
    cabbageCreate "image", sprintf({{bounds(%d, %d, %d, %d) colour(0, 0, 0, 0), mouseInteraction(0) channel("%s")}}, iBounds[0], iBounds[1], iBounds[2], iBounds[3], SImageChannel)
    cabbageCreate "label", sprintf({{bounds(%d, %d, %d, %d) mouseInteraction(0) fontColour(250, 250, 250) channel("%s"), fontSize(20), text("%s")}}, iBounds[0], iBounds[1], iBounds[2], iBounds[3], SLabelChannel, SText[chnget:i(SButtonChannel)])
    cabbageSet kTrig + kInit, SImageChannel, "svgElement", sprintfk({{
        <rect x="1" y="1" width="%d" height="%d" rx="%d" style="fill:%s;stroke:%s;stroke-width:3;stroke-dashoffset:0"/>
    }}, iBounds[2]-2, iBounds[3]-2, iBounds[3]*.2, rgbString(iR*.5, iG*.5, iB*.5), rgbString(0, 0, 0))
    cabbageSet kTrig, SLabelChannel, "text", SText[kValue]    
    xout kValue
endop



instr 1
    
    kSlider1 svgSlider "slider1", 200, 55, 140
    kSlider2 svgSlider "slider2", 160, 155, 40
    kSlider3 svgSlider "slider3", 30, 155, 180
    kSlider4 svgSlider "slider4", 200, 255, 140
    kSlider5 svgSlider "slider5", 100, 55, 240
    
    kButton1 svgButton "button1", 200, 55, 140
    kButton2 svgButton "button2", 160, 155, 40
    kButton3 svgButton "button3", 30, 155, 180
    kButton4 svgButton "button4", 200, 255, 140
    kButton5 svgButton "button5", 100, 55, 240    
        
    cabbageSetValue "slider1", abs(jspline:k(100, 1, 3)), kButton1
    cabbageSetValue "slider2", abs(jspline:k(100, 1, 3)), kButton2
    cabbageSetValue "slider3", abs(jspline:k(100, 1, 3)), kButton3
    cabbageSetValue "slider4", abs(jspline:k(100, 1, 3)), kButton4
    cabbageSetValue "slider5", abs(jspline:k(100, 1, 3)), kButton5    

endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
;starts instrument 1 and runs it for a week
i1 0 [60*60*24*7] 
</CsScore>
</CsoundSynthesizer>
