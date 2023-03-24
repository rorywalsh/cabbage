
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; BufferFreeze.csd
; Written by Iain McCurdy, 2014, updated 2016

; Created using the mincer opcode and circular writing into a function table

; Pointer     - a long horizontal slider that controls the read pointer position (relates to the waveform above) when 'Freeze' is active.
; Freeze      - freeze the buffer
; Input Gain  - gain applied to the dry signal (NB. dry signal muted anyway while freezing)
; Freeze      - level of the frozen buffer sound (when 'Freeze' is activated)
; FFT Size    - FFT size used in the freezing process. Use smaller values for rhythmic material, larger values for sustained, harmonic material
; Ptr. Rand.  - amount of gaussian random jitter of the pointer position when in 'Freeze' mode (rate is preset at k-rate)
; Transpose   - transposition of frozen audio
; Trans.Rand. - amount of gaussian random jitter of transposition of frozen audio (rate is preset at k-rate)
; 
; 
; 
;

<Cabbage>
form caption("Buffer Freeze"), size(610, 250), pluginId("BfFr"), colour(50,50,75), guiMode("queue")
gentable   bounds(  5,  5,600,120), tableNumber(1), tableColour("LightBlue"), fill(0), ampRange(-1,1,1), channel("displayTable"), zoom(-1)

hslider    bounds(  0,120, 610, 25), channel("ptr"),    range(0, 1.00, 0.9), trackerColour("LightBlue")
label      bounds(205,143,200,  12), text("Pointer"), fontColour("white")

image      bounds(604,  5,  3,120), alpha(0.5), channel("Scrubber")
;image      bounds(  5,  5,  3,120), alpha(0.5)  

checkbox   bounds( 15,180, 80, 30), channel("freeze"), text("Freeze"), fontColour:0("white"), fontColour:1("white"), colour("LightBlue")

rslider    bounds(100,170, 70, 70), channel("InGain"), text("Input Gain"), textColour("white"), range(0, 8.00, 1,0.5), trackerColour("LightBlue")
rslider    bounds(170,170, 70, 70), channel("DryGain"), text("Dry Gain"), textColour("white"), range(0, 8.00, 1,0.5), trackerColour("LightBlue")
rslider    bounds(240,170, 70, 70), channel("FreezeGain"), text("Freeze Gain"), textColour("white"), range(0, 8.00, 1), trackerColour("LightBlue")

label      bounds(325,175, 60,13), text("FFT Size"), fontColour("white")
combobox   bounds(325,190, 60,20), text("128","256","512","1024","2048","4096","8192"), channel("FFTSize"), value(4), fontColour(255,255,255)

rslider    bounds(395,170, 70, 70), channel("PtrRand"), text("Ptr.Rand."), textColour("white"), range(0, 1.00, 0), trackerColour("LightBlue")
rslider    bounds(465,170, 70, 70), channel("Transpose"), text("Transpose"), textColour("white"), range(-24, 24.00, 0), trackerColour("LightBlue")
rslider    bounds(535,170, 70, 70), channel("TransRand"), text("Trans.Rand."), textColour("white"), range(0, 24.00, 0), trackerColour("LightBlue")

</Cabbage>
                    
<CsoundSynthesizer>

<CsOptions>   
-dm0 -n
</CsOptions>

<CsInstruments>

; sr set by host
ksmps   =    16   ; NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls  =    2    ; NUMBER OF CHANNELS
0dbfs   =    1    ; MAXIMUM AMPLITUDE
             
giDispBuffer   ftgen    1,0, -600,-2, 0    ; define live audio buffer table for display. It makes sense if this corresponds to the display size in pixels.
giAudBuffer    ftgen    2,0, 2^18,10, 0    ; define live audio buffer table

instr    1
    ; READ LIVE INPUT
    aL,aR       ins

    ; RAMP-UP FUNCTION FOR PORTAMENTO TIME
    kporttime   linseg    0,0.001,0.01
    
    ; READ IN WIDGETS
    kfreeze     cabbageGetValue    "freeze"
    kInGain     cabbageGetValue    "InGain"
    kDryGain    cabbageGetValue    "DryGain"
    kFreezeGain cabbageGetValue    "FreezeGain"
    kptr        cabbageGetValue    "ptr"
    kptr        portk              kptr, kporttime
    kFFTSize    cabbageGetValue    "FFTSize"
    kFFTSize    init               4 ; (an init zero causes a crash)

;    koffset     init      0    
    
    ; SCALE AUDIO INPUT ACCORDING TO GUI WIDGET
    aL          *=        kInGain
    aR          *=        kInGain

    ; GENERATE FUNCTION FOR RANDOM POINTER OFFSET
    kPtrRand   gauss      cabbageGetValue:k("PtrRand")
               cabbageSet changed:k(kptr,kPtrRand), "Scrubber", "bounds", 5 + (kptr+kPtrRand) * 599, 5, 3,  120
    
    ; FREEZE CODE IS FREEZE BUTTON *NOT* BEEN PRESSED
    if kfreeze==0 then                                            ; if writing to table mode...
    ; write audio to table
    aptr        phasor    sr/ftlen(giAudBuffer)                
                tablew    aL,aptr,giAudBuffer,1

    koffset     downsamp  aptr                                    ; amount of offset added to freeze read pointer (NB. audio buffer does not scroll)
    
    if metro(sr*ftlen(giDispBuffer)/ftlen(giAudBuffer))==1 then   ; update according to size of display table and size of audio buffer
                tablew    k(aL),ftlen(giDispBuffer),giDispBuffer  ; write current audio sample value (as a downsampled krate value) to table
         
     ; SHUNT ENTIRE TABLE CONTENTS ONE STEP TO THE RIGHT IN ORDER TO DISPLAY A CONTINUOUSLY SCROLLING FUNCTION TABLE
     kcount     =         0
     loop:
     kval       table     kcount+1,giDispBuffer                   ; READ VALUE FROM TABLE
                tablew    kval,kcount,giDispBuffer                ; WRITE TO TABLE LOCATION IMMEDIATELY TO THE LEFT
                loop_lt   kcount,1,ftlen(giDispBuffer),loop       ; CONDITIONALLY LOOP BACK TO LABEL
                cabbageSet k(1), "displayTable", "tableNumber", 1 ; UPDATE TABLE DISPLAY ON COMPLETION OF THE SHUNT
    endif
                outs      aL*kDryGain, aR*kDryGain
     
    else                                                          ; OTHERWISE WE MUST BE IN FREEZE/WRITE MODE

     kptr       wrap      kptr+koffset+kPtrRand,0,1               ; NORMALISE POINTER (LIMIT BETWEEN ZERO AND 1)
     kPitch     =         semitone( cabbageGetValue:k("Transpose") + gauss:k(cabbageGetValue:k("TransRand")) )
     ; UPDATE FFT WINDOW SIZE
     if changed(kFFTSize)==1 then
      reinit UPDATE
     endif
     UPDATE:
     iFFTSize   =         2^(i(kFFTSize)+6)
     ; GENERATE AUDIO USING MINCER OPCODE
     asig       mincer    a(kptr*(ftlen(giAudBuffer)/sr)), kFreezeGain, kPitch, giAudBuffer, 0, iFFTSize
     rireturn 
     
     ; SEND MINCER AUDIO TO OUTPUTS
                outs      asig, asig
    endif
endin

</CsInstruments>

<CsScore>
i 1 0 z
</CsScore>

</CsoundSynthesizer>