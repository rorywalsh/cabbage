<Cabbage>
form caption("SVG FFT") size(600, 300), colour(60, 60, 60),  guiMode("queue") pluginId("def1")
image bounds(0, 0, 600, 300), channel("fftGrid"), colour(0,0,0,0) 
image bounds(0, 0, 600, 300), channel("fftImage1"), colour(0,0,0,0) 
image bounds(0, 0, 600, 300), channel("fftImage2"), colour(0,0,0,0) 
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

opcode svgFFT, 0,Sk[]iiii
    SChannel, kData[], iUpdateRate, iR, iG, iB xin
    iBounds[] cabbageGet SChannel, "bounds"
    kIndex = 0 
    kLen lenarray kData
    
    if metro(iUpdateRate) == 1 then
    
        SPath strcpyk sprintfk("<path d=\"M0 %d", iBounds[3])      
          
        while kIndex < lenarray(kData)/2 do
            kAmp scale kData[kIndex], 0, iBounds[3]*.8, 0, .5
            SPath strcatk SPath, sprintfk("L%d %d ", kIndex/kLen * 1200, iBounds[3]-kAmp)  
            kIndex += 2
        od
        
        SPath strcatk SPath, sprintfk("L%d %d L0 %d\" style=\"fill:%s;stroke:white;stroke-width:0;\"/>", iBounds[2], iBounds[3], iBounds[3], rgbString(iR, iG, iB))
        cabbageSet 1, SChannel, "svgElement", SPath
        
    endif
endop

opcode svgGrid, 0, Sii
    SChannel, iNumRows, iNumCols xin
    iBounds[] cabbageGet SChannel, "bounds" 
    iCnt init 0
    SPath init ""
    
    while iCnt < iNumRows do
        SPath strcat SPath, sprintf({{
        <line x1="%d" y1="%d" x2="%d" y2="%d" style="stroke:rgb(100,100,100);stroke-width:1" />
        }}, 0, (iBounds[3]/iNumRows)*iCnt, iBounds[2], (iBounds[3]/iNumRows)*iCnt)  
        iCnt +=1
    od
    
    iCnt = 0
    while iCnt < iNumCols do
        SPath strcat SPath, sprintf({{
        <line x1="%d" y1="%d" x2="%d" y2="%d" style="stroke:rgb(100,100,100);stroke-width:1" />
        }}, (iBounds[2]/iNumCols)*iCnt, 0, (iBounds[2]/iNumCols)*iCnt, iBounds[3])  
        iCnt +=1
    od
    
    cabbageSet SChannel, "svgElement", SPath
endop

instr 1
   
   svgGrid "fftGrid", 20, 10
   kData1[] init 1026
   kData2[] init 1026
   aNoise randi 1, sr/2
   aSig butterbp aNoise, abs(jspline:k(10000, .1, 1)), abs(jspline:k(1000, .4, 2))
   aSig balance aSig, aNoise
   fSig1   pvsanal aSig, 1024,256,1024, 1
   fSig2   pvsanal aSig*.5, 1024,256,1024, 1
   kFrame1  pvs2array kData1, fSig1
   kFrame2  pvs2array kData2, fSig2
   svgFFT "fftImage1", kData1, 10, 147, 200, 60 
   svgFFT "fftImage2", kData2, 10, 147, 250, 160

endin

</CsInstruments>
<CsScore>
i1 0 [60*60*24*7] 
</CsScore>
</CsoundSynthesizer>
