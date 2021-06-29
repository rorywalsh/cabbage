
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Honk.csd
; Written by Iain McCurdy, 2014
; 
; Instrument based on the wgbowedbar opcode (bowed bar).
; As with many physical modelling opcodes the results can be unpredicatable sometimes resulting in silence and at other times resulting in overloading.
; I have endeavoured to protect against overloads. Controller ranges have been tuned to provide the most interesting results but extensive experimentation is still recommended.
; 
; Input (Amplitude)
; -----------------
; This controls the amplitude of vibrations caused by the bow upon the bar. Note that this is not a linear 'level' control.
; 'Amplitude'    -    this is a manual control for amplitude
; 'Velocity'    -    this controls how midi key velocity affects amplitude 
; 'Env'        -    this controls the amount of envelope influence upon amplitude. (If zero, 'Dec.Tim.' and 'Sus.Lev' will have no effect.)
; 'Att.Tim'    -    time taken to rise to a maximum value before implementing the decay stage
; 'Dec.Tim'    -    time taken to decay to the sustain level
; 'Sus.Lev'    -    the sustain level for amplitude
; 'Rel.Tim'    -    the release time for amplitude
; 
; Output
; ------
; Controls pertaining to the output of the instrument
; 'Width'    -    width of a stereo effect using slowly modulating delays. When set to zero the output (before reverb) is monophonic.
; 'Rvb.Send'    -    amount of signal sent to a reverb effect
; 'Rvb.Size'    -    room size of the reverb effect (reverbsc)
; 
; Bow Position
; ------------
; 'Position'    -    manual control of bowing position along the bar
; 'Depth'    -    depth of random modulation of bowing position
; 'Rate'    -    rate of random modulation of bowing position
; 
; Bow Pressure
; ------------
; 'Pressure'    -    manual control of bowing pressure
; 'Velocity'    -    influence of key velocity upon bow pressure. 
;             This value (range zero - 'velocity') is added to the manual pressure control (and limited to be within the range zero - 1).
;             Therefore for maximum velocity influence, set 'Velocity' to 1, 'Pressure' to zero and 'Depth' (see below) to zero.
; 'Depth'    -    depth of random modulation of bowing pressure
; 'Rate'    -    rate of random modulation of bowing pressure
; 
; System
; ------
; 'Feedback'    -    gain of filter in the model. This will affect the abiltity of the instrument to achieve and sustain resonance.
; 'Discord'    -    an integration constant used in the setup of the model. Audibly varies dischord. This control changes at i-rate therefore changing it during a note will cause discontinuities.

<Cabbage>
form caption("Honk") size(700, 440), pluginId("Honk"), colour("LightGrey")


; amplitude
image   bounds(  0,  0,440, 90), colour(50,50,100), outlineColour("LightGrey"), outlineThickness(1), shape("sharp"), plant("amplitude"){
label   bounds(  5,  5,380, 12), text("Input"), fontColour(white)
rslider bounds( 10, 20, 60, 60), text("Amplitude"),                                  channel("amp"),     range(0,      1.00, 0.85),           TrackerColour("Silver"), textColour("white"), colour("white"), outlineColour(100,100,100)
rslider bounds( 70, 20, 60, 60), text("Velocity"),                                   channel("vel"),     range(0,      1.00, 0.66),           TrackerColour("Silver"), textColour("white"), colour("white"), outlineColour(100,100,100)
rslider bounds(130, 20, 60, 60), text("Env."),                                       channel("AEnv"),    range(0,      1.00, 0),              TrackerColour("Silver"), textColour("white"), colour("white"), outlineColour(100,100,100)
rslider bounds(190, 20, 60, 60), text("Att.Tim."),                                   channel("AAttTim"), range(0,      2.00, 0.1, 0.5),       TrackerColour("Silver"), textColour("white"), colour("white"), outlineColour(100,100,100)
rslider bounds(250, 20, 60, 60), text("Dec.Tim."),                                   channel("ADecTim"), range(0.001,  5.00, 0.1, 0.5),       TrackerColour("Silver"), textColour("white"), colour("white"), outlineColour(100,100,100)
rslider bounds(310, 20, 60, 60), text("Sus.Lev."),                                   channel("ASusLev"), range(0,      1.00, 1),              TrackerColour("Silver"), textColour("white"), colour("white"), outlineColour(100,100,100)
rslider bounds(370, 20, 60, 60), text("Rel.Tim."),                                   channel("ARelTim"), range(0,      2.00, 0.1,0.5,0.001),  TrackerColour("Silver"), textColour("white"), colour("white"), outlineColour(100,100,100)
}

; output
image   bounds(440,  0,260, 90), colour(50,50,100), outlineColour("LightGrey"), outlineThickness(1), shape("sharp"), plant("output"){
label   bounds(  5,  5,250, 12), text("Output"), fontColour(white)
rslider bounds( 10, 20, 60, 60), text("Width"),                                      channel("width"),   range(0,      1.00, 0.3), TrackerColour("Silver"), textColour("white"), colour("white"), outlineColour(100,100,100)
rslider bounds( 70, 20, 60, 60), text("Rvb.Send"),                                   channel("RvbSend"), range(0,      1.00, 0.2), TrackerColour("Silver"), textColour("white"), colour("white"), outlineColour(100,100,100)
rslider bounds(130, 20, 60, 60), text("Rvb.Size"),                                   channel("RvbSize"), range(0.5,    0.99, 0.6), TrackerColour("Silver"), textColour("white"), colour("white"), outlineColour(100,100,100)
rslider bounds(190, 20, 60, 60), text("Level"),                                      channel("OutGain"), range(0,     10.00, 0.5,0.5), TrackerColour("Silver"), textColour("white"), colour("white"), outlineColour(100,100,100)
}

; bow position
image   bounds(  0, 90,700, 90), colour(50,50,100), outlineColour("LightGrey"), outlineThickness(1), shape("sharp"), plant("bowposition"){
label   bounds(  5,  5,690, 12), text("Bowing Position"), fontColour(white)
hslider bounds( 10, 30,555, 30), text("Manual"),                                     channel("pos"),      range(0,      1.00, 0.7), TrackerColour("Silver"), textColour("white"), colour("white")
line    bounds(580,  8,100, 2), colour("grey")
label   bounds(595,  5, 72, 10), text("MODULATION"), colour(50,50,100), fontColour("white")
rslider bounds(570, 20, 60, 60), text("Depth"),                                      channel("PosModDep"),range(0,     0.50, 0.25), TrackerColour("Silver"), textColour("white"), colour("white"), outlineColour(100,100,100)
rslider bounds(630, 20, 60, 60), text("Rate"),                                       channel("PosModRte"),range(0.01, 50.00, 3,0.5,0.01),    TrackerColour("Silver"), textColour("white"), colour("white"), outlineColour(100,100,100)
}

; bow pressure
image   bounds(  0,180,700, 90), colour(50,50,100), outlineColour("LightGrey"), outlineThickness(1), shape("sharp"), plant("bowpressure"){
label   bounds(  5,  5,690, 12), text("Bowing Pressure"), fontColour(white)
hslider bounds( 10, 30,495, 30), text("Manual"),                                     channel("bowpres"),   range(0,     1, 0.45), TrackerColour("Silver"), textColour("white"), colour("white")
rslider bounds(510, 20, 60, 60), text("Velocity"),                                   channel("BowPresVel"),range(0,     1.00, 0), TrackerColour("Silver"), textColour("white"), colour("white"), outlineColour(100,100,100)
line    bounds(580,  8,100, 2), colour("grey")
label   bounds(595,  5, 72, 10), text("MODULATION"), colour(50,50,100), fontColour("white")
rslider bounds(570, 20, 60, 60), text("Depth"),                                      channel("BowPresDep"),range(0,     0.50, 0.25), TrackerColour("Silver"), textColour("white"), colour("white"), outlineColour(100,100,100)
rslider bounds(630, 20, 60, 60), text("Rate"),                                       channel("BowPresRte"),range(0.01, 50.00, 3.5,0.5,0.01),  TrackerColour("Silver"), textColour("white"), colour("white"), outlineColour(100,100,100)
}

; system
image   bounds(  0,270,700, 90), colour(50,50,100), outlineColour("LightGrey"), outlineThickness(1), shape("sharp"), plant("system"){
label   bounds(  5,  5,690, 12), text("System"), fontColour(white)
hslider bounds( 10, 20,675, 30), text("F.back"),                                   channel("gain"),    range(0,      1.00, 0.95), TrackerColour("Silver"), textColour("white"), colour("white")
hslider bounds( 10, 50,675, 30), text("Discord"),                                    channel("const"),   range(0,   1.50, 0.97), TrackerColour("Silver"), textColour("white"), colour("white")
}

keyboard pos( 0, 360), size(700, 80)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps     =     32
nchnls     =     2
;0dbfs    =    1

gasendL,gasendR    init    0

/* MAPPING TO SCALE AMPLITUDES ACCORDING TO BOW PRESSURE (HIGHER BOW PRESSURES DEMAND AMPLITUDE ATTENUATIONS) */
giPresScale    ftgen    0, 0, 128*4, 16, 1, 78*4, 0, 1, 50*4, -1, 0.01

/* UDO TO CONTAIN EXPLOSIONS IN AMPLITUDE */
opcode    AmpLimit,a,aiii
 asig,imax,iatt,irel    xin
 krms        init    0
 kGainCtrl    init    1
 if krms>imax then
  kGainCtrl    limit    kGainCtrl - iatt, 0, 1
 else
  kGainCtrl    limit    kGainCtrl + irel, 0, 1
 endif
 asig    =    asig * kGainCtrl
 krms        rms    asig
    xout    asig
endop


instr    1        ; WGBOWEDBAR INSTRUMENT
    kporttime    linseg    0,0.001,0.05                            ; portamento time used to smooth value changes from widgets

    gkamp        chnget    "amp"
    gkvel        chnget    "vel"

    /* BOW POSITION */
    gkpos        chnget    "pos"                                ; bow position
    kpos        portk    gkpos, kporttime
    gkPosModDep    chnget    "PosModDep"
    gkPosModRte    chnget    "PosModRte"
    if gkPosModDep>0 then                                    ; if modulation depth greater than zero...
     kPosMod    rspline    -gkPosModDep,gkPosModDep,gkPosModRte*0.5,gkPosModRte
     kpos        limit    kpos + kPosMod, 0, 1
    endif

    /* BOW PRESSURE */    
    gkbowpres    chnget    "bowpres"
    kbowpres    portk    gkbowpres, kporttime
    gkBowPresDep    chnget    "BowPresDep"
    gkBowPresRte    chnget    "BowPresRte"
    if gkBowPresDep>0 then                                    ; if modulation depth greater than zero...
     kBowPresMod    rspline    -gkBowPresDep,gkBowPresDep,gkBowPresRte*0.5,gkBowPresRte
     kbowpres    limit    kbowpres + kBowPresMod, 0, 1
    endif
    iBowPresVel    chnget    "BowPresVel"                            ; velocity influence of
    iVelPres    ampmidi    iBowPresVel                            ; velocity to bow pressure
    kbowpres    limit    kbowpres + iVelPres, 0, 1                    ; limit to prevent out of range values that could result from the combination of manual, random and velocity control of bow pressure
    kBowPresScale    tablei    kbowpres,giPresScale,1                        ; read pressure amplitude scale value from table (n.b. higher bow pressure values demand amplitude scaling)
    kBowPresScale    portk    kBowPresScale, kporttime                    ; smooth changes in bow pressure amplitude scaling
    kbowpres    scale    kbowpres, 0.2, 20.00                        ; remap bow pressure value from 0 - 1 to 0.2 - 20

    gkAEnv        chnget    "AEnv"                                ; amount of envelope influence. If this is zero, 'Att.Tim.', 'Dec.Tim' 'Sus.Lev." and 'Rel.Tim.' will have no influence
    gkAAttTim    chnget    "AAttTim"
    gkADecTim    chnget    "ADecTim"
    gkASusLev    chnget    "ASusLev"
    gkARelTim    chnget    "ARelTim"

    gkRvbSend    chnget    "RvbSend"
    gkRvbSize    chnget    "RvbSize"
    gkwidth        chnget    "width"

    gkamp        portk    gkamp, kporttime
    
    /* AMPLITUDE ENVELOPE */
    if i(gkAAttTim)>0 then                                ; if zero attack time, skip attack stage in envelope
     kAmpEnv    linsegr    0,i(gkAAttTim),1,i(gkADecTim),i(gkASusLev),i(gkARelTim),0
    else
     kAmpEnv    linsegr    1,i(gkADecTim),i(gkASusLev),i(gkARelTim),0        ; with attack stage
    endif
    kAmpEnv        ntrpol    1,kAmpEnv,gkAEnv                    ; mix of envelope influence: 0=no_envelope_influence 1=full_envelope_influence
    aAmpEnv        interp    kAmpEnv
    gkamp    =    gkamp * aAmpEnv                            ; scale manual amplitude control with envelope
    
    icps    cpsmidi                ; READ MIDI PITCH VALUES - THIS VALUE CAN BE MAPPED TO GRAIN DENSITY AND/OR PITCH DEPENDING ON THE SETTING OF THE MIDI MAPPING SWITCHES
    ivel    veloc        0,1        ; READ NOTE VELOCITY. WILL BE WITHIN THE RANGE 0 AND 1
    iamp    ampmidi    i(gkvel)*(-4)
    gkamp    scale    gkamp,0,4
    kamp     limit    iamp + gkamp, 0,10

    gkgain        chnget    "gain"                            ; called 'feedback' in the GUI
    gkconst        chnget    "const"                            ; called 'Discord' in the GUI
    kSwitch        changed        gkconst
    if    kSwitch=1    then        ;IF I-RATE VARIABLE CHANGE TRIGGER IS '1'...
        reinit    UPDATE            ;BEGIN A REINITIALISATION PASS FROM LABEL 'UPDATE'
    endif
    UPDATE:    
    abowedbar          wgbowedbar     kamp, icps, kpos, kbowpres, gkgain, i(gkconst), 0, 0, 20
    rireturn    ;RETURN FROM REINITIALISATION PASS TO PERFORMANCE TIME PASSES    
        
    /* OUTPUT LEVEL CONTROL */
    gkOutGain    chnget    "OutGain"                            ; called 'Level' in the GUI. Controls output level.
    aenv    linsegr    1, i(gkARelTim), 0        ;ANTI-CLICK ENVELOPE
    abowedbar    =    abowedbar * gkOutGain * aenv * kBowPresScale

    /* CALL THE AmpLimit UDO */
    ;                INPUT     | AMP_LIMIT | ATTACK | RELEASE
    abowedbar    AmpLimit    abowedbar,     0.9*0dbfs,      0.01,    0.01

    /* STEREO WIDENING */
    if gkwidth>0 then
     kRndDelL        rspline        0, 0.01*gkwidth, 0.4, 0.8
     kRndDelR        rspline        0, 0.01*gkwidth, 0.4, 0.8
     kRndDelL        limit        kRndDelL, 0, 0.01
     kRndDelR        limit        kRndDelR, 0, 0.01
     aRndDelL        interp        kRndDelL
     aRndDelR        interp        kRndDelR
     aL            vdelay        abowedbar, 0.1+(aRndDelL*1000), 0.1 + 10
     aR            vdelay        abowedbar, 0.1+(aRndDelR*1000), 0.1 + 10
         
     gasendL        =    gasendL + (aL * gkRvbSend)
     gasendR        =    gasendR + (aR * gkRvbSend)
             outs         aL, aR
    else     ; WIDTH CONTROL IS ZERO (NO STEREO EFFECT)
     gasendL        =    gasendL + (abowedbar * gkRvbSend)
     gasendR        =    gasendR + (abowedbar * gkRvbSend)
         
     outs         abowedbar, abowedbar
    endif
endin

instr    99
    aL,aR    reverbsc    gasendL,gasendR,gkRvbSize,4000
        outs        aL,aR
        clear        gasendL,gasendR
endin
</CsInstruments>

<CsScore>
i 1 0 0.1    ; INITIALISES THE ALGORITHM (OTHERWISE THE FIRST NOTE PLAYED DOESN'T SOUND)
i 99 0 3600    ; REVERB
</CsScore>

</CsoundSynthesizer>