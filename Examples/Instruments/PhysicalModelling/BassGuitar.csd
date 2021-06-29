
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; BassGuitar.csd
; Written by Iain McCurdy, 2011

; Physical model of a plucked string with a pickup point. 
; Model is created from first principles in order to implement some improvements over the existing Csound opcodes for plucked strings.
; A bandpass filter is employed within the delay buffer used to implement the pluck which facilitates filtering to specific harmonics. 
; Conventional damping effects are still possible when cutoff frequency ratio = 1.

<Cabbage>
form caption("Bass Guitar"), size(660, 200), pluginId("basg")

#define RSliderStyle # colour(150,205,150), textColour("LightGreen"), outlineColour(0,0,0), trackerColour(yellow)#

image    bounds(  0,  0,660,200),  colour("DarkGreen"), shape("sharp"), outlineColour("white"), outlineThickness(3)
rslider  bounds( 10, 10, 60, 60),  text("Sustain"),      channel("feedback"), range(0.9,1, 0.999, 2, 0.001), $RSliderStyle
rslider  bounds( 70, 10, 60, 60),  text("Filt. Ratio"), channel("FiltRatio"), range(0.5, 32, 1, 0.5), $RSliderStyle
rslider  bounds(130, 10, 60, 60),  text("B.width"),     channel("bw"), range(1, 32, 16), $RSliderStyle
rslider  bounds(190, 10, 60, 60),  text("Att"),         channel("att"), range(0, 3, 0.16,0.5), $RSliderStyle
checkbox bounds(260, 20, 30, 30),  text("Legato")       channel("legato"), fontColour("white"), colour("yellow"),  value(1)
label    bounds(252, 54, 43, 12),  align("centre"), text("Legato"), fontColour("LightGreen")
rslider  bounds(300, 10, 60, 60),  text("Leg.Speed"),        channel("LegSpeed"),  range(0.01,1,0.05,0.5), $RSliderStyle
rslider  bounds(360, 10, 60, 60),  text("Vib.Depth"),        channel("VibDep"),    range(0, 1, 0.25, 0.75, 0.001), $RSliderStyle
rslider  bounds(420, 10, 60, 60),  text("Vib.Rate"),         channel("VibRte"),    range(0.5, 20, 3, 0.5), $RSliderStyle
rslider  bounds(480, 10, 60, 60),  text("Level"),            channel("level"),     range(0, 1, 0.7), $RSliderStyle
rslider  bounds(540, 10, 60, 60),  text("Pluck Pos."),        channel("Plk"),     range(0.01, 0.99, 0.1), $RSliderStyle
checkbox bounds(610, 20, 30, 30),  text("Inharm")       channel("Inharm"), fontColour("white"), colour("yellow"),  value(0)
label    bounds(600, 54, 50, 12),  align("centre"), text("Inharm."), fontColour("LightGreen")
hslider  bounds( 15, 70,635, 40),  text("Pickup Position"), channel("PickupPos"), range(0.01, 0.99, 0.1), $RSliderStyle
keyboard bounds(  5,115,650, 80)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps         =     64
nchnls         =     2
0dbfs        =    1    ;MAXIMUM AMPLITUDE
massign    0,2

gkNoteTrig    init    0
gkactive    init    0    ; Will contain number of active instances of instr 3 when legato mode is chosen. NB. notes in release stage will not be regarded as active. 


opcode PluckedElectricString, a, iiaakaaki
iPlk,iFund,aFund,aPickup,kRefl,acutoff,abw,kInharm,iVel xin
setksmps 1
aPlk    linseg   0, (1/iFund)*iPlk, 1, (1/iFund)*(1-iPlk), 0
aPlk    butlp    aPlk, cpsoct((iVel*7)+6)
aBuf    delayr   4
aTap1   deltapi  aPickup/aFund
aTap2   deltapi  1/aFund
;aTap2   tone     aTap2, (sr*0.5) * (1-kDamp)
if kInharm==1 then
 aTap2,aR2 hilbert  aTap2
endif
abw     limit    acutoff * abw, 0.001, 10000    ; limit bandwidth values to prevent explosion
aTap2   butbp    aTap2, acutoff, abw            ; bandpass filter (nb. within delay buffer)


        delayw   aPlk + (aTap2 * kRefl)
        xout     (aTap2 - aTap1)
endop

instr    1    ;read in widgets - this instrument runs constantly during performance
    gkfeedback    chnget    "feedback"
    gkFiltRatio    chnget    "FiltRatio"
    gkbw        chnget    "bw"
    gkatt        chnget    "att"
    gklegato    chnget    "legato"
    gkLegSpeed    chnget    "LegSpeed"
    gkVibDep    chnget    "VibDep"
    gkVibRte    chnget    "VibRte"
    gklevel        chnget    "level"
    gkPickupPos    chnget    "PickupPos"
    gkInharm    chnget    "Inharm"
endin

instr    2    ;triggered via MIDI
    gkNoteTrig    init    1    ;at the beginning of a new note set note trigger flag to '1'
    icps        cpsmidi        ;read in midi note pitch in cycles per second
    givel        veloc    0,1    ;read in midi note velocity

    gkcps    =    icps        ;update a global krate variable for note pitch

    if i(gklegato)==0 then        ;if we are *not* in legato mode...
     inum    notnum                        ; read midi note number (0 - 127)
         event_i    "i",p1+1+(inum*0.001),0,-1,icps        ; call sound producing instr
     krel    release                        ; release flag (1 when note is released, 0 otherwise)
     if krel==1 then                    ; when note is released...
      turnoff2    p1+1+(inum*0.001),4,1            ; turn off the called instrument
     endif                            ; end of conditional
    else                ;otherwise... (i.e. legato mode)
     iactive    =    i(gkactive)            ;number of active notes of instr 3 (notes in release are disregarded)
     if iactive==0 then                    ;...if no notes are active
      event_i    "i",p1+1,0,-1,icps                ;...start a new held note
     endif
    endif
endin

instr    3
    kporttime    linseg    0,0.001,1        ;portamento time function rises quickly from zero to a held value
    kporttime    =    kporttime*gkLegSpeed    ;scale portamento time function with value from GUI knob widget
    iPlk        chnget   "Plk"

    if i(gklegato)==1 then                ;if we are in legato mode...
     krel    release                    ;sense when  note has been released
     gkactive    =    1-krel            ;if note is in release, gkactive=0, otherwise =1
     kcps    portk    gkcps,kporttime            ;apply portamento smooth to changes in note pitch (this will only have an effect in 'legato' mode)
     acps    interp    kcps                ;create a a-rate version of pitch (cycles per second)
     kcutoff        limit    gkcps*gkFiltRatio,20,20000    ;cutoff frequency of the bandpass filter will be relative to the pitch of the note. Limit it to prevent out of range values that would cause filter expolosion.
     kactive    active    p1-1            ;...check number of active midi notes (previous instrument)
     if kactive==0 then                ;if no midi notes are active...
      turnoff                    ;... turn this instrument off
     endif
    else                        ;otherwise... (polyphonic / non-legato mode)
     acps    =    p4                 ;pitch equal to the original note pitch
     kcutoff        limit    p4*gkFiltRatio,20,20000    ;cutoff frequency of the bandpass filter will be relative to the pitch of the note. Limit it to prevent out of range values that would cause filter expolosion.
    endif
            
    ;In legato mode modulations are reinitialised
    if gkNoteTrig==1&&gklegato==1 then
     reinit    RESTART_ENVELOPE
    endif
    RESTART_ENVELOPE:
    krise    linseg    0,0.3,0,1.5,1            ;build-up envelope - modulations do not begin immediately
    rireturn
    arise    interp    krise                ;interpolation prevents discontinuities (clicks) when oscili lfos are reinitialised
    avib    oscil    0.8*arise*gkVibDep,gkVibRte    ;vibrato
    acps    =    acps*semitone(avib)
    atrm    oscil    0.8*arise*gkVibDep,gkVibRte    ;tremolo

    gkPickupPos    portk    gkPickupPos,kporttime        ;smooth changes made to pickup position slider

    ares         PluckedElectricString  iPlk, p4, acps, a(gkPickupPos), gkfeedback, a(kcutoff), a(gkbw), gkInharm, givel
        
    aenv        linsegr    0.7,0.05,0            ;amplitude envelope
    
    if i(gkatt)>0 then                    ;if attack time is anything greater than zero call the lowpass filter envelope
    
    acfenv    expseg    p4/ (i(gkatt)*5), i(gkatt), sr/2, 1, sr/2
    ares    tone    ares, acfenv
    endif
    
    ares        =    ares*aenv*(1+atrm)*gklevel    ;scale amplitude of audio signal with envelope, tremolo and level knob widget
            outs    ares,ares
    gkNoteTrig    =    0                ;reset new-note trigger (in case it was '1')
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]    ;instrument that reads in widget data
</CsScore>

</CsoundSynthesizer>