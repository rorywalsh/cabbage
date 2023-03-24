
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; ConvolutionReverb.csd
; Written by Iain McCurdy, 2012.
; 
; ------------------
; You are encouraged to experiment with different impulse files.
; You can try this one to start with: http://www.iainmccurdy.org/CsoundRealtimeExamples/SourceMaterials/Stairwell.wav
; You can find some here: http://www.openairlib.net/
; NOTE THAT ZERO LATENCY (or close to zero) CONV0LUTION WILL NOT BE POSSIBLE.
; 
; Instructions
; ------------
; 'Size Ratio' compresses the duration of the impulse file (provided that 'compressed' is chosen for 'resize' mode).
; 'Curve' defines the shape of an envelope applied to compressed impulse files using GEN16
;     zero        =    straight line decaying
;     positive vals.     =    increasingly convex decaying
;     negative vals    =    increasingly concave decaying
; Increase 'iplen' (must be a power of 2) if experiencing performance problems, or reduce it to reduce latency.
; 'Delay OS' fine tunes the delay applied to the dry signal. Normally this should be zero but adjust it to modify how the dry signal lines up with the convoluted signal.

<Cabbage>
form caption("Convolution Reverb") size(505,230), pluginId("Conv")
image bounds(0, 0, 505,230), colour(135, 30, 30,220), shape("rounded"), outlineColour(255,255,150), outlineThickness(4) 
label   bounds( 29,  10, 55, 12),       text("direction:"), fontColour(250,250,250)
button  bounds( 10,  22, 90, 20),       text("forward","backward"),                            channel("FwdBwd"), value(0), fontColour(250,250,250)
label   bounds( 33,  43, 45, 12),       text("resize:"), fontColour(250,250,250)
button  bounds( 10,  55, 90, 20),       text("normal","compressed"),                           channel("resize"), value(0), fontColour(250,250,250)
rslider bounds(105, 11, 70, 70),        text("Size Ratio"),     colour(115, 10, 10), trackerColour(255,255,150), outlineColour( 75, 35,  0), textColour(250,250,250),  channel("CompRat"),     range(0, 1.00, 1)
rslider bounds(170, 11, 70, 70),        text("Curve"),          colour(115, 10, 10), trackerColour(255,255,150), outlineColour( 75, 35,  0), textColour(250,250,250),  channel("Curve"),       range(-8.00, 8.00, 0)
rslider bounds(235, 11, 70, 70),        text("In Skip"),        colour(115, 10, 10), trackerColour(255,255,150), outlineColour( 75, 35,  0), textColour(250,250,250),  channel("skipsamples"), range(0, 1.00, 0)
rslider bounds(300, 11, 70, 70),        text("Del.OS."),        colour(115, 10, 10), trackerColour(255,255,150), outlineColour( 75, 35,  0), textColour(250,250,250),  channel("DelayOS"),     range(-1.00, 1.00, 0)
rslider bounds(365, 11, 70, 70),        text("Mix"),            colour(115, 10, 10), trackerColour(255,255,150), outlineColour( 75, 35,  0), textColour(250,250,250),  channel("mix"),         range(0, 1.00, 0.25)
rslider bounds(430, 11, 70, 70),        text("Level"),          colour(115, 10, 10), trackerColour(255,255,150), outlineColour( 75, 35,  0), textColour(250,250,250),  channel("level"),       range(0, 1.00, 0.4)

filebutton bounds( 10, 90, 90, 25), text("Open File","Open File"), fontColour("white") channel("filename"), shape("ellipse")
soundfiler bounds(110, 90,380,125), channel("beg","len"), identChannel("ImpulseFile"),  colour(0, 255, 255, 255), fontColour(160, 160, 160, 255)
label      bounds(114, 92,300, 16), text(""), align(left), colour(0,0,0,0), fontColour(255,255,255,150), identChannel("stringbox")
label      bounds(114, 92,350, 16), text("First open a short stereo impulse sound file..."), align(left), fontColour(255,255,255,150), visible(1), identChannel("InstructionID")
}
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n
</CsOptions>

<CsInstruments>

; sr set by host
ksmps         =     32    ;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls         =     2    ;NUMBER OF CHANNELS (2=STEREO)
0dbfs        =    1

;Author: Iain McCurdy (2012)

;giImpulse    ftgen    1,0,0,1,"D:/Csound/SSDIR/stairwell.wav",0,0,0    ; load stereo file
giImpulse    ftgen    1,0,2,-2,0
giDisplay    ftgen    2,0,ftlen(giImpulse),2,0                        ; display table table
tableicopy 2, 1
gkReady    init    0

; reverse function table UDO
opcode    tab_reverse,i,i
ifn             xin
iTabLen         =               ftlen(ifn)
iTableRev       ftgen           ifn + 100,0,-iTabLen,-2, 0
icount          =               0
loop:
ival            table           iTabLen - icount - 1, ifn
                tableiw         ival, icount, iTableRev
                loop_lt         icount, 1, iTabLen, loop
                xout            iTableRev
endop

; compress function table UDO
opcode    tab_compress,i,iii
ifn,iCompRat,iCurve    xin
iTabLen         =               ftlen(ifn)
iTabLenComp     =               int(ftlen(ifn) * iCompRat)
iTableComp      ftgen           ifn + 200, 0, -iTabLenComp, -2, 0
iAmpScaleTab    ftgen           ifn + 300, 0, -iTabLenComp, -16, 1, iTabLenComp, iCurve, 0
icount          =               0
loop:
ival            table           icount, ifn
iAmpScale       table           icount, iAmpScaleTab
                tableiw         ival*iAmpScale,icount,iTableComp
                loop_lt         icount,1,iTabLenComp,loop
                xout            iTableComp
endop

; compress reverse function table UDO
opcode    tab_compress_rev,i,iii
ifn,iCompRat,iCurve    xin
iTabLen         =               nsamp(ifn)
iTabLenComp     =               int(nsamp(ifn)*iCompRat)
iTableComp      ftgen           ifn+400,0,-iTabLenComp,-2, 0
iAmpScaleTab    ftgen           ifn+500,0,-iTabLenComp,-16, 1,iTabLenComp,iCurve,0
icount          =               0
loop:
ival            table           icount, ifn
iAmpScale       table           icount, iAmpScaleTab
                tableiw         ival*iAmpScale, iTabLenComp-icount-1,iTableComp
                loop_lt         icount,1,iTabLenComp,loop
                xout            iTableComp
endop

opcode FileNameFromPath,S,S            ; Extract a file name (as a string) from a full path (also as a string)
 Ssrc        xin                       ; Read in the file path string
 icnt        strlen  Ssrc              ; Get the length of the file path string
 LOOP:                                 ; Loop back to here when checking for a backslash
 iasc        strchar Ssrc, icnt        ; Read ascii value of current letter for checking
 if iasc==92 igoto   ESCAPE            ; If it is a backslash, escape from loop
 loop_gt    icnt,1,0,LOOP              ; Loop back and decrement counter which is also used as an index into the string
 ESCAPE:                               ; Escape point once the backslash has been found
 Sname       strsub  Ssrc, icnt+1, -1  ; Create a new string of just the file name
             xout    Sname             ; Send it back to the caller instrument
endop

instr    1
    gSfilepath    chnget    "filename"
    kNewFileTrg    changed    gSfilepath  ; if a new file is loaded generate a trigger
    if kNewFileTrg==1 then                ; if a new file has been loaded...
     event    "i",99,0,0                  ; call instrument to update sample storage function table 
    endif
    
    if trigger:k(gkReady,0.5,0)==1 then   ; when a file is loaded for the first time do this conditional branch...
     event    "i",2,0,3600*24*7           ; start the convolution instrument
    endif
endin

instr    2    ;CONVOLUTION REVERB INSTRUMENT
    chnset    "visible(0)","InstructionID"        ; hide the instruction

    kFwdBwd      chnget    "FwdBwd"
    kresize      chnget    "resize"
    kmix         chnget    "mix"
    klevel       chnget    "level"
    kCompRat     chnget    "CompRat"
    kCurve       chnget    "Curve"
    kskipsamples chnget    "skipsamples"
    kDelayOS     chnget    "DelayOS"
    kCompRat     init      1              ; IF THIS IS LEFT UNINITIALISED A CRASH WILL OCCUR! 

    
    ainL,ainR    ins                ;READ STEREO AUDIO INPUT
    ;ainL,ainR    diskin2    "808loop.wav",1,0,1    ;USE A SOUND FILE FOR TESTING
    ainMix       sum       ainL,ainR
    
    ;CREATE REVERSED TABLES
    irev    tab_reverse    giImpulse
        
    kSwitchStr   changed   gSfilepath
    kSwitchStr   delayk    kSwitchStr,1
    kSwitch      changed   kskipsamples, kFwdBwd, kDelayOS, kCompRat, kCurve, kresize    ;GENERATE A MOMENTARY '1' PULSE IN OUTPUT 'kSwitch' IF ANY OF THE SCANNED INPUT VARIABLES CHANGE. (OUTPUT 'kSwitch' IS NORMALLY ZERO)
    kSwitch      +=        kSwitchStr
    if    kSwitch=1    then             ;IF I-RATE VARIABLE IS CHANGED...
        reinit    UPDATE                ;BEGIN A REINITIALISATION PASS IN ORDER TO EFFECT THIS CHANGE. BEGIN THIS PASS AT LABEL ENTITLED 'UPDATE' AND CONTINUE UNTIL rireturn OPCODE 
    endif                               ;END OF CONDITIONAL BRANCHING
    UPDATE:                             ;LABEL
    
    ;CREATE COMPRESSED TABLES
    icomp    tab_compress    giImpulse,i(kCompRat),i(kCurve)
        
    ;CREATE COMPRESSED REVERSED TABLES
    icomprev    tab_compress_rev    giImpulse,i(kCompRat),i(kCurve)
        
    iplen        =    1024                            ;BUFFER LENGTH (INCREASE IF EXPERIENCING PERFORMANCE PROBLEMS, REDUCE IN ORDER TO REDUCE LATENCY)
    itab         =    giImpulse                       ;DERIVE FUNCTION TABLE NUMBER OF CHOSEN TABLE FOR IMPULSE FILE
    iirlen       =    nsamp(itab)*0.5                 ;DERIVE THE LENGTH OF THE IMPULSE RESPONSE IN SAMPLES. DIVIDE BY 2 AS TABLE IS STEREO.
    iskipsamples =    nsamp(itab)*0.5*i(kskipsamples) ;DERIVE INSKIP INTO IMPULSE FILE. DIVIDE BY 2 (MULTIPLY BY 0.5) AS ALL IMPULSE FILES ARE STEREO
    
        ;FORWARDS REVERB
    if kFwdBwd==0&&kresize==0 then
     aL,aR    ftconv    ainMix, itab, iplen,iskipsamples, iirlen ;CONVOLUTE INPUT SOUND
     adelL    delay    ainL, abs((iplen/sr)+i(kDelayOS))         ;DELAY THE INPUT SOUND ACCORDING TO THE BUFFER SIZE
     adelR    delay    ainR, abs((iplen/sr)+i(kDelayOS))         ;DELAY THE INPUT SOUND ACCORDING TO THE BUFFER SIZE
        
        ;BACKWARDS REVERB
    elseif kFwdBwd==1&&kresize==0 then
     aL,aR    ftconv    ainMix, irev, iplen, iskipsamples, iirlen                      ;CONVOLUTE INPUT SOUND
     adelL    delay    ainL,abs((iplen/sr)+(iirlen/sr)-(iskipsamples/sr)+i(kDelayOS))  ;DELAY THE INPUT SOUND ACCORDING TO THE BUFFER SIZE AND THE DURATION OF THE IMPULSE FILE
     adelR    delay    ainR,abs((iplen/sr)+(iirlen/sr)-(iskipsamples/sr)+i(kDelayOS))  ;DELAY THE INPUT SOUND ACCORDING TO THE BUFFER SIZE AND THE DURATION OF THE IMPULSE FILE
        
        ;FORWARDS COMPRESSED
    elseif kFwdBwd==0&&kresize==1 then
     aL,aR    ftconv   ainMix, icomp, iplen,iskipsamples, iirlen*i(kCompRat)  ;CONVOLUTE INPUT SOUND
     adelL    delay    ainL, abs((iplen/sr)+i(kDelayOS))                      ;DELAY THE INPUT SOUND ACCORDING TO THE BUFFER SIZE
     adelR    delay    ainR, abs((iplen/sr)+i(kDelayOS))                      ;DELAY THE INPUT SOUND ACCORDING TO THE BUFFER SIZE
        
        ;BACKWARDS COMPRESSED
    elseif kFwdBwd==1&&kresize==1 then
     aL,aR    ftconv    ainMix, icomprev, iplen, iskipsamples, iirlen*i(kCompRat)        ;CONVOLUTE INPUT SOUND
     adelL    delay    ainL,abs((iplen/sr)+((iirlen*i(kCompRat))/sr)-(iskipsamples/sr)+i(kDelayOS))    ;DELAY THE INPUT SOUND ACCORDING TO THE BUFFER SIZE AND THE DURATION OF THE IMPULSE FILE
     adelR    delay    ainR,abs((iplen/sr)+((iirlen*i(kCompRat))/sr)-(iskipsamples/sr)+i(kDelayOS))    ;DELAY THE INPUT SOUND ACCORDING TO THE BUFFER SIZE AND THE DURATION OF THE IMPULSE FILE
    endif
                 
        rireturn
        
    ; CREATE A DRY/WET MIX
    aMixL    ntrpol    adelL,aL*0.1,kmix
    aMixR    ntrpol    adelR,aR*0.1,kmix
            outs    aMixL*klevel,aMixR*klevel

endin




instr    99    ; load sound file
     giImpulse    ftgen    1,0,0,1,gSfilepath,0,0,0    ; load stereo file
     gkReady     init    1                    ; if no string has yet been loaded giReady will be zero
     Smessage sprintfk "file(%s)", gSfilepath            ; print file to viewer
     chnset Smessage, "filer1"    

     Smessage sprintfk "file(%s)", gSfilepath            ; print sound file image to fileplayer
     chnset Smessage, "ImpulseFile"

    Sname    FileNameFromPath    gSfilepath                ; Call UDO to extract file name from the full path
    Smessage sprintfk "text(%s)",Sname
    chnset Smessage, "stringbox"
endin


;instr    99    ; load sound file
; Sname FileNameFromPath    gSfilepath                ; Call UDO to extract file name from the full path
; Smessage sprintfk "text(%s)",Sname
; chnset Smessage, "stringbox"
;endin
        
</CsInstruments>

<CsScore>
i 1 0 [3600*7*24] 
</CsScore>

</CsoundSynthesizer>
