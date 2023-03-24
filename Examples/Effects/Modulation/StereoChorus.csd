
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; StereoChorus.csd
; Written by Iain McCurdy, 2012.

; Two LFO modes are offered, sines and rsplines.
; 

; Dereg. (deregulate) adds a random modulation to both LFO rate and depth in both LFO modes

<Cabbage>
form caption("Stereo Chorus") size(595, 100), pluginId("scho")
image                 bounds(0, 0, 595, 100), colour("DarkSlateGrey"), shape("rounded"), outlineColour("white"), outlineThickness(6)
label    bounds( 15, 22, 75, 12), text("LFO Type:"), fontColour("white")
combobox bounds( 15, 35, 75, 20), text("Sine","RSpline"), channel("type"), textColour("white"), colour( 7,29,29), fontColour("white"), value(1)
rslider  bounds( 90, 13, 75, 75), text("Rate"), channel("rate"), range(0.001, 7, 0.5,0.5),  textColour("white"), colour(37,59,59), trackerColour("Silver")
rslider  bounds(160, 13, 75, 75), text("Dereg"), channel("dereg"), range(0, 4, 0,0.5,0.01), textColour("white"), colour(37,59,59), trackerColour("Silver")
rslider  bounds(230, 13, 75, 75), text("Depth"), channel("depth"), range(0, 1.00, 0.2),     textColour("white"), colour(37,59,59), trackerColour("Silver")
rslider  bounds(300, 13, 75, 75), text("Offset"), channel("offset"), range(0.0001,0.1,0.001,0.5,0.0001),     textColour("white"), colour(37,59,59), trackerColour("Silver")
rslider  bounds(370, 13, 75, 75), text("Width"), channel("width"), range(0, 1.00, 0.75),    textColour("white"), colour(37,59,59), trackerColour("Silver")
rslider  bounds(440, 13, 75, 75), text("Mix"), channel("mix"), range(0, 1.00, 0.5),      textColour("white"), colour(37,59,59), trackerColour("Silver")
rslider  bounds(510, 13, 75, 75), text("Level"), channel("level"), range(0, 1.00, 1),      textColour("white"), colour(37,59,59), trackerColour("Silver")
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>
;sr set by host
ksmps = 32
nchnls = 2
0dbfs = 1

;Author: Iain McCurdy (2012)
;http://iainmccurdy.org/csound.html

opcode    StChorus,aa,aakkakk
    ainL,ainR,krate,kdepth,aoffset,kwidth,kmix    xin            ;READ IN INPUT ARGUMENTS
    ilfoshape    ftgentmp    0, 0, 131072, 19, 1, 0.5, 0,  0.5    ;POSITIVE DOMAIN ONLY SINE WAVE
    kporttime    linseg    0,0.001,0.02                    ;RAMPING UP PORTAMENTO VARIABLE
    kChoDepth    portk    kdepth*0.01, kporttime                ;SMOOTH VARIABLE CHANGES WITH PORTK
    aChoDepth    interp    kChoDepth                    ;INTERPOLATE TO CREATE A-RATE VERSION OF K-RATE VARIABLE
    amodL         osciliktp     krate, ilfoshape, 0            ;LEFT CHANNEL LFO
    amodR         osciliktp     krate, ilfoshape, kwidth*0.5        ;THE PHASE OF THE RIGHT CHANNEL LFO IS ADJUSTABLE
    amodL        =        (amodL*aChoDepth)+aoffset            ;RESCALE AND OFFSET LFO (LEFT CHANNEL)
    amodR        =        (amodR*aChoDepth)+aoffset            ;RESCALE AND OFFSET LFO (RIGHT CHANNEL)
    aChoL        vdelay    ainL, amodL*1000, 1.2*1000            ;CREATE VARYING DELAYED / CHORUSED SIGNAL (LEFT CHANNEL) 
    aChoR        vdelay    ainR, amodR*1000, 1.2*1000            ;CREATE VARYING DELAYED / CHORUSED SIGNAL (RIGHT CHANNEL)
    aoutL        ntrpol     ainL*0.6, aChoL*0.6, kmix            ;MIX DRY AND WET SIGNAL (LEFT CHANNEL)
    aoutR        ntrpol     ainR*0.6, aChoR*0.6, kmix            ;MIX DRY AND WET SIGNAL (RIGHT CHANNEL)
            xout    aoutL,aoutR                    ;SEND AUDIO BACK TO CALLER INSTRUMENT
endop


giPow3  ftgen 1,0,2048,10,1            ; table for storage of functions
iftlen =       ftlen(giPow3)            ; length of the function table
icount    =    0                ; reset counter. (Moves for each index of the function table)
loop3:                        ; loop beginning                                            
ix    =    ((icount/iftlen) * 2) -1        ; shift x range to -1 to 1                                  
iy    =    ix ^ 3                          ; apply formula to derive y                                 
    tableiw iy,icount,giPow3                ; write y value to table                                    
loop_lt,icount,1,iftlen,loop3                   ; loop back and increment counter if we are not yet finished

opcode    StChorusRspline,aa,aakkkkkk
    ainL,ainR,krate,kdereg,kdepth,koffset,kwidth,kmix    xin        ;READ IN INPUT ARGUMENTS
    kporttime    linseg    0,0.001,0.02                    ;RAMPING UP PORTAMENTO VARIABLE
    kChoDepth    portk    kdepth*0.01, kporttime                ;SMOOTH VARIABLE CHANGES WITH PORTK    
    kmod1        rspline    koffset,kChoDepth, krate*4+0.01, ((krate*4*kdereg)+0.01)
    kmod2        rspline    kChoDepth,koffset, krate*4+0.01, ((krate*4*kdereg)+0.01)
    kmod1        limit    kmod1,0.0001,1.2
    kmod2        limit    kmod2,0.0001,1.2
    amod1        interp    kmod1    
    amod2        interp    kmod2
    aCho1        vdelay    ainL, amod1*1000, 1.2*1000            ;CREATE VARYING DELAYED / CHORUSED SIGNAL (LEFT CHANNEL) 
    aCho2        vdelay    ainR, amod2*1000, 1.2*1000            ;CREATE VARYING DELAYED / CHORUSED SIGNAL (RIGHT CHANNEL)
    kpan        rspline    0,1,krate,2*krate*kdereg            ;PANNING
    kpan        limit    kpan,0,1
    apan        interp    kpan
    aChoL        =    (aCho1*apan)+(aCho2*(1-apan))
    aChoR        =    (aCho2*apan)+(aCho1*(1-apan))
    aChoL        ntrpol    aChoL,aCho1,kwidth                ;WIDTH PROCESSING BETWEEN AUTO-PANNED AND HARD-PANNED
    aChoR        ntrpol    aChoR,aCho2,kwidth
    aoutL        ntrpol     ainL*0.6, aChoL*0.6, kmix            ;MIX DRY AND WET SIGNAL (LEFT CHANNEL)
    aoutR        ntrpol     ainR*0.6, aChoR*0.6, kmix            ;MIX DRY AND WET SIGNAL (RIGHT CHANNEL)
            xout    aoutL,aoutR                    ;SEND AUDIO BACK TO CALLER INSTRUMENT
endop

instr 1
 kporttime    linseg    0,0.001,0.05                                                     
 krate chnget "rate"
 kdereg chnget "dereg"
 kdepth chnget "depth"
 koffset chnget "offset"
 kwidth chnget "width"
 klevel chnget "level"
 kmix      chnget "mix"
 ktype    chnget    "type"
 ktype    init    1
 kmix    portk    kmix,kporttime
 klevel    portk    klevel,kporttime
 koffset    portk    koffset,kporttime*0.5
 aoffset    interp    koffset
 
 a1,a2    ins
 ;a1    pinkish    0.1
 ;a2    pinkish    0.1
 ;a1    vco2    0.1,50,4,0.5
 ;a2    vco2    0.1,50,4,0.5
 ;a1    poscil    0.1,100
 ;a2    poscil    0.1,100
 
 
 if ktype==1 then
  kdereg    rspline    -kdereg, kdereg, 0.1, 0.5
  ktrem    rspline    0,-1,0.1,0.5
  ktrem    pow    2,ktrem
  a1,a2    StChorus    a1,a2,krate*octave(kdereg),kdepth*ktrem,aoffset,kwidth,kmix
 else
  a1,a2    StChorusRspline    a1,a2,krate,kdereg,kdepth,koffset,kwidth,kmix
 endif
 a1    =    a1 * klevel
 a2    =    a2 * klevel
     outs    a1,a2
endin

</CsInstruments>

<CsScore>                                              
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>                                                  