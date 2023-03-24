
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Bounce.csd
; Written by Iain McCurdy, 2014

; As the xypad pointer bounces off boundaries triggers (notes) are generated.
; Code that senses barrier collisions is extracted into a UDO for easy transplantation and repetition. 

; each xypad triggers a different synthesiser instrument with a different note range and note duration

; for collisions against verticals, location from the lowest point, controls pitch
; for collisions against horizontals, location from the left-most point, controls pitch
; x position controls panning position for both horizontals and verticals

<Cabbage>
form caption("Bounce") size(610, 360), pluginId("bnce"), colour("Black")

; box 1 image    bounds(  0,  0,230,250), colour(200,100,100,200), 
shape("rounded"), ouTlinecolour("white"), outlineThickness(0) 
xypad    bounds(  0, 0,230,250), colour(200,100,100,200), channel("x1", "y1"), rangeX(0, 1.00, 0), rangeY(0, 1, 0)

; box 2
image    bounds(230,  0,270,250), colour(100,200,200,200),shape("rounded"), outlineColour("white"), line(0)
xypad    bounds(230,  0,270,250), colour(100,200,200,200), channel("x2", "y2"), rangeX(0, 1.00, 0), rangeY(0, 1, 0)

; box 3
image    bounds(  0,250,500,110), colour(100,100,200,200),shape("rounded"), outlineColour("white"), line(0)
xypad    bounds(  0,250,500,110), colour(100,100,200,200), channel("x3", "y3"), rangeX(0, 1.00, 0), rangeY(0, 1, 0)

; box 4
image    bounds(500,  0,110,360), colour(100,200,100,200),shape("rounded"), outlineColour("white"), line(0)
xypad    bounds(500,  0,110,360), colour(100,200,100,200), channel("x4", "y4"), rangeX(0, 1.00, 0), rangeY(0, 1, 0)

</Cabbage>

<CsoundSynthesizer>
<CsOptions>
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps     =     32
nchnls     =     2
0dbfs    =    1

gisine        ftgen    0,0,4096,10,1    ; sine wave
gaRvbSendL,gaRvbSendR    init    0    ; initialise reverb stereo send global audio signal

; UDO that senses changes of direction (and therefore barrier collisions) of the xypad widget
opcode    XYPadEdgeTriggers,kkkk,kk
 kx,ky    xin                    ; read in input arguments
 kOldx    init    0                ; initialise variables that hold the *previous* values of x and y positions (for comparison with current x and y positions)
 kOldy    init    0
 kHorDir    init    0            ; set initial (arbitrary) state for horizontal direction (0=left to right, 1=right to left)
 kVertDir    init    0            ; set initial (arbitrary) state for vertical direction (0=bottom to top, 1=top to bottom)
 ktrig1    =    0                ; reset all triggers to zero (this is needed because not all 'trigger' opcodes are read on each k-cycle and some trigger outputs could otherwise 'stick' at '1' and not return to zero 
 ktrig2    =    0
 ktrig3    =    0
 ktrig4    =    0

 ; LEFT AND RIGHT BARRIERS 
 if kHorDir==0 then                ; if horizontal direction is left to right...  
  ktrig1    trigger    kx,kOldx,1        ; if current x position is less than previous x position - i.e. right edge barrier has been bounced against - generate a trigger
  if ktrig1==1 then                ; if bounce against right edge has been detected...
   kHorDir    =    1            ; change direction
  endif
 elseif kHorDir==1 then                ; if horizontal direction is right to left...
  ktrig2    trigger    kx,kOldx,0        ; if current x position is greater than previous x position - i.e. left edge barrier has been bounced against - generate a trigger
  if ktrig2==1 then                ; if bounce against left edge has been detected...
   kHorDir    =    0            ; change direction
  endif
 endif 
 kOldx    =    kx                ; previous x position equals current x position (for the next k pass)

 ; UPPER AND LOWER BARRIERS 
 if kVertDir==0 then                ; if vertical direction is bottom to top...
  ktrig3    trigger    ky,kOldy,1        ; if current y position is less than previous y position - i.e. top edge barrier has been bounced against - generate a trigger
  if ktrig3==1 then                ; if bounce against top edge has been detected...
   kVertDir    =    1            ; change direction
  endif
 elseif kVertDir==1 then            ; if vertical direction is top to bottom...
  ktrig4    trigger    ky,kOldy,0        ; if current y position is greater than previous y position - i.e. bottom edge barrier has been bounced against - generate a trigger
  if ktrig4==1 then                ; if bounce against bottom edge has been detected...
   kVertDir    =    0            ; change direction
  endif
 endif 
 kOldy    =    ky                ; previous y position equals current y position (for the next k pass)
 xout    ktrig1,ktrig2,ktrig3,ktrig4        ; send triggers back to caller instrument
endop




instr    1
 ; box 1
 kx    chnget    "x1"                        ; read in x and y position for this box
 ky    chnget    "y1"
 kMOUSE_DOWN_LEFT    chnget    "MOUSE_DOWN_LEFT"
 kMOUSE_DOWN_RIGHT    chnget    "MOUSE_DOWN_RIGHT"
 
 kClickSum    =    kMOUSE_DOWN_LEFT+kMOUSE_DOWN_RIGHT
 kClickSum    =    1-kClickSum
 
 ktrig1,ktrig2,ktrig3,ktrig4    XYPadEdgeTriggers    kx,ky    ; call UDO that senses barrier collisions
 schedkwhen    (ktrig1+ktrig2+ktrig3+ktrig4)*kClickSum,0,0,101,0,2.5,ky,kx              ; trigger instruments


 ; box 2
 kx    chnget    "x2"                        ; read in x and y position for this box
 ky    chnget    "y2"
 ktrig1,ktrig2,ktrig3,ktrig4    XYPadEdgeTriggers    kx,ky    ; call UDO that senses barrier collisions    
 schedkwhen    (ktrig1+ktrig2+ktrig3+ktrig4)*kClickSum,0,0,102,0,1.5,ky,kx             ; trigger instruments

 ; box 3
 kx    chnget    "x3"                        ; read in x and y position for this box
 ky    chnget    "y3"
 ktrig1,ktrig2,ktrig3,ktrig4    XYPadEdgeTriggers    kx,ky    ; call UDO that senses barrier collisions
 schedkwhen    (ktrig1+ktrig2+ktrig3+ktrig4)*kClickSum,0,0,103,0,0.1,ky,kx             ; trigger instruments 

 ; box 4
 kx    chnget    "x4"                        ; read in x and y position for this box
 ky    chnget    "y4"
 ktrig1,ktrig2,ktrig3,ktrig4    XYPadEdgeTriggers    kx,ky    ; call UDO that senses barrier collisions
 schedkwhen    (ktrig1+ktrig2+ktrig3+ktrig4)*kClickSum,0,0,104,0,0.2,ky,kx              ; trigger instruments

endin

; SCALE FOR REFLECTION DEPENDENT UPON MIDI NOTE NUMBER (LESS DAMPING FOR HIGHER NOTES)
giScal1    ftgen    0,0,128, -27,  0, 0.9, 24, 0.9, 36, 0.85, 48, 0.75, 60, 0.65, 72, 0.35, 84, 0.001, 96, 0.001, 127        ; longer sustaining notes / less damping
giScal2    ftgen    0,0,128, -27,  0, 0.983, 24, 0.983, 36, 0.971, 48, 0.939, 60, 0.855, 72, 0.747, 84, 0.364, 96, 0.001, 127    ; shorter sustaining notes / more damping

instr    101    ; longer harmonic pluck sound
 inum    =    48 + int(24 * p4)
 aEnv    linsegr    0, 0.005, 1, p3-0.105, 1, 0.1, 0        ; amplitude envelope
 iPlk    random    0.1, 0.3                    ; point at which to pluck the string
 iDtn    random    -0.05, 0.05                    ; random detune
 irefl    table    inum, giScal1                    ; read reflection value from giScal table according to note number  
 aSig    wgpluck2  0.58, 0.5, cpsmidinn(inum+iDtn), iPlk, irefl    ; generate Karplus-Strong plucked string audio 
 kcf    expon    cpsoct(rnd(6)+6),p3,50                ; filter cutoff frequency envelope
 aSig    clfilt    aSig, kcf, 0, 2                    ; butterworth lowpass filter    
 aL,aR pan2      aSig * aEnv, p5                ; random panning   
       outs      aL, aR                        ; send audio to outputs
 gaRvbSendL    =    gaRvbSendL + aL*0.1
 gaRvbSendR    =    gaRvbSendR + aR*0.1
endin


opcode    FreqShifter,a,aki                    ; frequency shifter
    ain,kfshift,ifn    xin                    ;READ IN INPUT ARGUMENTS
    areal, aimag hilbert ain                ;HILBERT OPCODE OUTPUTS TWO PHASE SHIFTED SIGNALS, EACH 90 OUT OF PHASE WITH EACH OTHER
    asin     oscili       1,    kfshift,     ifn,          0
    acos     oscili       1,    kfshift,     ifn,          0.25    
    ;RING MODULATE EACH SIGNAL USING THE QUADRATURE OSCILLATORS AS MODULATORS
    amod1    =        areal * acos
    amod2    =        aimag * asin    
    ;UPSHIFTING OUTPUT
    aFS    = (amod1 - amod2)
        xout    aFS                ;SEND AUDIO BACK TO CALLER INSTRUMENT
endop

instr    102    ; longer inharmonic pluck sound
 inum        =        60 + int(24 * p4)
 aEnv        linsegr        0, 0.005, 1, p3-0.105, 1, 0.1, 0 ; amplitude envelope
 iPlk        random        0.1, 0.3                         ; point at which to pluck the string
 iDtn        random        -0.05, 0.05                      ; random detune
 irefl        table        inum, giScal1
 aSig        wgpluck2    0.58, 0.5, cpsmidinn(inum+iDtn), iPlk, irefl   
 kcf        expon        cpsoct(rnd(6)+6),p3,50
 aSig        clfilt        aSig, kcf, 0, 2                    ; butterworth lowpass filter    
 aSig        FreqShifter    aSig,44,gisine
 aL,aR        pan2          aSig * aEnv, p5        ; random panning   
         outs          aL, aR
 gaRvbSendL    =        gaRvbSendL + aL*0.2
 gaRvbSendR    =        gaRvbSendR + aR*0.2
endin

instr    103    ; short sine impulse - no dry signal, only reverb
 inum        =        84 + int(18 * p4)
 aEnv        linsegr        0, 0.005, 1, p3-0.105, 1, 0.1, 0 ; amplitude envelope
 aSig        poscil        0.2,cpsmidinn(inum),gisine

 aL,aR        pan2          aSig * aEnv, p5        ; random panning   
;         outs          aL, aR
 gaRvbSendL    =        gaRvbSendL + aL
 gaRvbSendR    =        gaRvbSendR + aR
endin

instr    104    ; short harmonic pluck sound - wide picth range
 inum        =        48 + int(48 * p4)
 aEnv          linsegr        0, 0.005, 1, p3-0.105, 1, 0.1, 0 ; amplitude envelope
 iPlk          random        0.1, 0.3                         ; point at which to pluck the string
 iDtn          random        -0.05, 0.05                      ; random detune
 irefl        table        inum, giScal2
 aSig          wgpluck2      0.58, 0.5, cpsmidinn(inum+iDtn), iPlk, irefl        
 kcf        expon        cpsoct(rnd(6)+6),p3,50
 aSig        clfilt        aSig, kcf, 0, 2                    ; butterworth lowpass filter    
 aL,aR         pan2          aSig * aEnv, rnd(0.5)+0.5        ; random panning   
              outs          aL, aR
 gaRvbSendL    =        gaRvbSendL + aL*0.3
 gaRvbSendR    =        gaRvbSendR + aR*0.3
endin


instr    201    ; reverb instrument
    aL,aR    reverbsc    gaRvbSendL,gaRvbSendR,0.85,7000
        outs        aL,aR
        clear        gaRvbSendL,gaRvbSendR
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]    ; sense collisions with barriers
i 201 0 [3600*24*7]    ; reverb instrument
</CsScore>

</CsoundSynthesizer>