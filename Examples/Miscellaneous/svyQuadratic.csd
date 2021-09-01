<Cabbage>
form caption("Quads") size(600, 300), colour(0, 0, 0),  guiMode("queue") pluginId("def1")
image bounds(0, 0, 600, 300), channel("gridImage"), colour(150,150,150) 
image bounds(0, 0, 600, 300), channel("quadEnv1"), colour(0,0,0,0), mouseInteraction(0) 
image bounds(0, 0, 600, 300), channel("quadEnv2"), colour(0,0,0,0), mouseInteraction(0) 
image bounds(0, 0, 600, 300), channel("quadEnv3"), colour(0,0,0,0), mouseInteraction(0) 
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

opcode rgbString, S,iii
    iR, iG, iB xin
    SColourString sprintf {{rgb(%d, %d, %d)}}, iR, iG, iB
    xout SColourString
endop

opcode rgb, i[],iii
iR,iG,iB xin
iColour[] fillarray iR, iG, iB
xout iColour
endop

opcode svgQuad, 0,Sii[]i[]
     
    SChannel, iHandleNumber, iFillColour[], iHandleColour[]  xin
    kMouseX chnget "MOUSE_X"
    kMouseY chnget "MOUSE_Y"
    kMouseDown chnget "MOUSE_DOWN_LEFT"
    
    SHandle sprintf "%s_handle", SChannel
    SHandleLabel sprintf "%s_handleLabel", SChannel
    SHandleWidth sprintf "%s_width", SChannel
    SHandleLine sprintf "%s_line", SChannel
    iBounds[] cabbageGet SChannel, "bounds"
    kWidthBounds[] cabbageGet SHandleWidth, "bounds"
    kHandleBounds[] cabbageGet SHandle, "bounds"
    SCurrentWidget, kTrig cabbageGet "CURRENT_WIDGET"
    kCurrentWidth init 40
    
    if kMouseDown == 1 && strcmpk(SCurrentWidget, SHandleWidth) == 0 then
        cabbageSet 1, SHandleWidth, "bounds", kMouseX-5, kHandleBounds[1], 12, 12
        cabbageSet 1, SHandleLine, "bounds", kMouseX+5, kHandleBounds[1]+2, (kHandleBounds[0]-kMouseX)+5, 4
        kCurrentWidth = kHandleBounds[0]-kMouseX
    endif
    if kMouseDown == 1 && strcmpk(SCurrentWidget, SHandle) == 0 then
        cabbageSet 1, SHandle, "bounds", kMouseX-10, kMouseY-10, 30, 30
        cabbageSet 1, SHandleLabel, "bounds", kMouseX-7, kMouseY-7.5, 25, 25
        cabbageSet 1, SHandleWidth, "bounds", kMouseX-kCurrentWidth, kMouseY-12, 15, 15
        cabbageSet 1, SHandleLine, "bounds", kMouseX-kCurrentWidth, kMouseY-8, kCurrentWidth+5, 4
    endif

    kQX = kHandleBounds[0]+10
    kQY = iBounds[3]/2
    cabbageSet kMouseDown, SChannel, "svgElement", sprintfk({{
    <path d="M 0 %d L %d %d Q %d %d %d %d Q %d %d %d %d L %d %d" style="stroke:rgb(0,0,0);fill:rgb(%d, %d, %d);stroke-width:4;fill-opacity:.2"/>
    }}, iBounds[3]/2, kQX-kCurrentWidth-100, iBounds[3]/2, kQX-kCurrentWidth, kQY, kHandleBounds[0]+15, kHandleBounds[1]+10, kQX+kCurrentWidth, kQY, kQX+kCurrentWidth+100, iBounds[3]/2, iBounds[2], iBounds[3]/2, iFillColour[0], iFillColour[1], iFillColour[2])
        
    
    cabbageCreate "image", sprintf({{bounds(%d, %d, 54, 4), colour(0,0,0), channel("%s"), parent("%s")}}, iBounds[0]+iBounds[2]/2-45, iBounds[1]+iBounds[3]/2+2, SHandleLine, SChannel) 
    cabbageCreate "image", sprintf({{bounds(%d, %d, 30, 30), outlineThickness(2), outlineColour(0,0,0), colour(%d, %d, %d), channel("%s"), shape("ellipse"), parent("%s")}}, iBounds[0]+iBounds[2]/2-10, iBounds[1]+iBounds[3]/2, iHandleColour[0], iHandleColour[1], iHandleColour[2], SHandle, SChannel) 
    cabbageCreate "label", sprintf({{bounds(%d, %d, 25, 25), text("%d"), fontColour(0, 0, 0), mouseInteraction(0), channel("%s")}},iBounds[0]+iBounds[2]/2-7, iBounds[1]+iBounds[3]/2+2.5,  iHandleNumber, SHandleLabel) 
    cabbageCreate "image", sprintf({{bounds(%d, %d, 15, 15), outlineThickness(1), outlineColour(0,0,0), colour(0, 55, 180), channel("%s"), shape("ellipse"), parent("%s")}}, iBounds[0]+iBounds[2]/2-50, iBounds[1]+iBounds[3]/2-2.5, SHandleWidth, SChannel) 

endop

opcode svgGrid, 0, Siii[]
    SChannel, iNumRows, iNumCols, iColour[] xin
    iBounds[] cabbageGet SChannel, "bounds" 
    iCnt init 0
    SPath init ""
    
    while iCnt < iNumRows do
        SPath strcat SPath, sprintf({{
        <line x1="%d" y1="%d" x2="%d" y2="%d" style="stroke:rgb(%d, %d, %d);stroke-width:.3" />
        }}, 0, (iBounds[3]/iNumRows)*iCnt, iBounds[2], (iBounds[3]/iNumRows)*iCnt, iColour[0], iColour[1], iColour[2])  
        iCnt +=1
    od
    
    iCnt = 0
    while iCnt < iNumCols do
        SPath strcat SPath, sprintf({{
        <line x1="%d" y1="%d" x2="%d" y2="%d" style="stroke:rgb(%d, %d, %d);stroke-width:.3" />
        }}, (iBounds[2]/iNumCols)*iCnt, 0, (iBounds[2]/iNumCols)*iCnt, iBounds[3], iColour[0], iColour[1], iColour[2])  
        iCnt +=1
    od
    
    cabbageSet SChannel, "svgElement", SPath
endop

instr 1
    
    svgGrid "gridImage", 20, 20, rgb(250, 255, 250)
    svgQuad "quadEnv1", 1, rgb(100, 0, 0), rgb(200, 100, 100)
    svgQuad "quadEnv2", 2, rgb(0, 100, 0), rgb(100, 200, 100)
    svgQuad "quadEnv3", 3, rgb(0, 0, 100), rgb(100, 100, 200)

endin

</CsInstruments>
<CsScore>
i1 0 [60*60*24*7] 
</CsScore>
</CsoundSynthesizer>
