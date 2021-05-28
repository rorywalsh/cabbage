
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; GEN19.csd
; Written by Iain McCurdy, 2015

; GEN19 is an elaboration of GEN10 and GEN09 in that as well as offering control over partial number, strength and
; phase, we also have control over DC offset.

; In this example the GEN19 table is used as a 'terrain' rather than as an audio waveform.
; A small ball travels along the function as if threaded onto it. It responds to the contours of the wave 
; as if under the influence of gravity. As it travels it plays short sound pulses. The faster it moves the more 
; more frequently these pulses are generated and the louder they are, the higher it is, the higher in pitch these 
; pulses are.
; The shape of the GEN19 function is continually modulated by rspline random functions. The frequency of modulation
; for partial strength, phase and DC offset are set globally for all partials. Note that if an rspline frequency value
; is low, changes to frequency can take longer to take effect. The amplitude of modulation for the three partial 
; attributes can be set individually for each partial.

; CONTROLS
; --------
; Str.Rate    -    rate of partial strength modulation for all partials
; Phase Rate    -    rate of phase modulation for all partials
; DC Rate    -    rate of DC offset modulation for all partials
; Normalise    -    turns normalisation on or off for the GEN19 function
;            note that if normalisation is, off strength amplitudes may need to be lowered in order to keep
;            the function within the range of the gentable viewer.
; Sound On/Off    -    turns the sound on or off
; Speed        -    overall speed control for the sliding ball
; Friction    -    increasing this beyond zero adds an increasing amount of friction damping the movement of the 
;            ball. This can be useful to prevent constant movment of the ball in one direction.
; Density    -    Density of the sound impulses

; For each partial there are the following controls:
; Active    -    turns each partial on or off
; P.N.        -    partial number
; Str.        -    Amplitude of the random modulations of strength
; Phs.        -    Amplitude of the random modulations of phase
; DC        -    Amplitude of the random modulations of DC offset

             
<Cabbage>
form caption("GEN19"), size(560, 490), pluginId("gn19"), guiRefresh(32), colour(0,0,0)

gentable bounds(  0,  5, 560, 120), tableNumber(1), tableColour("silver"), identChannel("table"), outlineThickness(2), ampRange(-1,1,1), zoom(-1), tableGridColour(0,0,0,0), tablebackgroundColour(0,0,0), fill(0)
image   bounds(-50,-50, 10, 10), colour(250,150, 55), identChannel("ball"), shape("ellipse")

rslider  bounds(  5,140, 60, 60), channel("AmpRte"), text("Str.Rate"),   range(0.1, 5, 0.2,0.5,0.001)
rslider  bounds( 65,140, 60, 60), channel("PhsRte"), text("Phase Rate"), range(0.1, 5, 0.2,0.5,0.001)
rslider  bounds(125,140, 60, 60), channel("DCRte"),  text("DC Rate"),    range(0.1, 5, 0.2,0.5,0.001)
checkbox bounds(200,150,130,15), channel("Norm"), text("Normalise"), value(1)
checkbox bounds(200,170,130,15), channel("Sound"), text("Sound On/Off"), value(1)
rslider  bounds(315,140, 60, 60), channel("Speed"),  text("Speed"),    range(0.1, 16,   1,0.5,0.001)
rslider  bounds(375,140, 60, 60), channel("Friction"),  text("Friction"),    range(0,1,0.03,0.5,0.001)
rslider  bounds(435,140, 60, 60), channel("Density"),  text("Density"),    range(10,200,50,1,1)
rslider  bounds(495,140, 60, 60), channel("Level"),  text("Level"),    range(0,2,1,0.5,0.001)
line     bounds( 20,210,520,  3), colour("Grey")

button   bounds( 10,225, 50, 20), channel("On1"), text("Active"), value(1), fontColour:0(50,50,50), fontColour:1(255,255,0)
combobox bounds( 10,250, 50, 20), channel("PN1"), text(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16), value(1)
label    bounds( 15,270, 40, 12), text("P.N.")
rslider  bounds(  5,290, 60, 60), channel("Amp1"), text("Str."),   range(0, 1, 0.5,1,0.001)
rslider  bounds(  5,355, 60, 60), channel("Phs1"), text("Phs."),   range(0,360,180,1,0.001)
rslider  bounds(  5,420, 60, 60),  channel("DC1"),  text("DC."),   range(0, 1, 0.5,1,0.001)

button   bounds( 80,225, 50, 20), channel("On2"), text("Active"), value(1), fontColour:0(50,50,30), fontColour:1(255,255,0)
combobox bounds( 80,250, 50, 20), channel("PN2"), text(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16), value(2)
label    bounds( 85,270, 40, 12), text("P.N.")
rslider  bounds( 75,290, 60, 60), channel("Amp2"), text("Str."),   range(0, 1, 0.5,1,0.001)
rslider  bounds( 75,355, 60, 60), channel("Phs2"), text("Phs."),   range(0,360,180,1,0.001)
rslider  bounds( 75,420, 60, 60),  channel("DC2"),  text("DC."),   range(0, 1, 0.5,1,0.001)

button   bounds(150,225, 50, 20), channel("On3"), text("Active"), value(1), fontColour:0(50,50,30), fontColour:1(255,255,0)
combobox bounds(150,250, 50, 20), channel("PN3"), text(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16), value(3)
label    bounds(155,270, 40, 12), text("P.N.")
rslider  bounds(145,290, 60, 60), channel("Amp3"), text("Str."),   range(0, 1, 0.5,1,0.001)
rslider  bounds(145,355, 60, 60), channel("Phs3"), text("Phs."),   range(0,360,180,1,0.001)
rslider  bounds(145,420, 60, 60),  channel("DC3"),  text("DC."),   range(0, 1, 0.5,1,0.001)

button   bounds(220,225, 50, 20), channel("On4"), text("Active"), value(1), fontColour:0(50,50,30), fontColour:1(255,255,0)
combobox bounds(220,250, 50, 20), channel("PN4"), text(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16), value(4)
label    bounds(225,270, 40, 12), text("P.N.")
rslider  bounds(215,290, 60, 60), channel("Amp4"), text("Str."),   range(0, 1, 0.5,1,0.001)
rslider  bounds(215,355, 60, 60), channel("Phs4"), text("Phs."),   range(0,360,180,1,0.001)
rslider  bounds(215,420, 60, 60),  channel("DC4"),  text("DC."),   range(0, 1, 0.5,1,0.001)

button   bounds(290,225, 50, 20), channel("On5"), text("Active"), value(0), fontColour:0(50,50,30), fontColour:1(255,255,0)
combobox bounds(290,250, 50, 20), channel("PN5"), text(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16), value(5)
label    bounds(295,270, 40, 12), text("P.N.")
rslider  bounds(285,290, 60, 60), channel("Amp5"), text("Str."),   range(0, 1, 0.5,1,0.001)
rslider  bounds(285,355, 60, 60), channel("Phs5"), text("Phs."),   range(0,360,180,1,0.001)
rslider  bounds(285,420, 60, 60),  channel("DC5"),  text("DC."),   range(0, 1, 0.5,1,0.001)

button   bounds(360,225, 50, 20), channel("On6"), text("Active"), value(0), fontColour:0(50,50,30), fontColour:1(255,255,0)
combobox bounds(360,250, 50, 20), channel("PN6"), text(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16), value(6)
label    bounds(365,270, 40, 12), text("P.N.")
rslider  bounds(355,290, 60, 60), channel("Amp6"), text("Str."),   range(0, 1, 0.5,1,0.001)
rslider  bounds(355,355, 60, 60), channel("Phs6"), text("Phs."),   range(0,360,180,1,0.001)
rslider  bounds(355,420, 60, 60),  channel("DC6"),  text("DC."),   range(0, 1, 0.5,1,0.001)

button   bounds(430,225, 50, 20), channel("On7"), text("Active"), value(0), fontColour:0(50,50,30), fontColour:1(255,255,0)
combobox bounds(430,250, 50, 20), channel("PN7"), text(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16), value(7)
label    bounds(435,270, 40, 12), text("P.N.")
rslider  bounds(425,290, 60, 60), channel("Amp7"), text("Str."),   range(0, 1, 0.5,1,0.001)
rslider  bounds(425,355, 60, 60), channel("Phs7"), text("Phs."),   range(0,360,180,1,0.001)
rslider  bounds(425,420, 60, 60),  channel("DC7"),  text("DC."),   range(0, 1, 0.5,1,0.001)

button   bounds(500,225, 50, 20), channel("On8"), text("Active"), value(0), fontColour:0(50,50,30), fontColour:1(255,255,0)
combobox bounds(500,250, 50, 20), channel("PN8"), text(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16), value(8)
label    bounds(505,270, 40, 12), text("P.N.")
rslider  bounds(495,290, 60, 60), channel("Amp8"), text("Str."),   range(0, 1, 0.5,1,0.001)
rslider  bounds(495,355, 60, 60), channel("Phs8"), text("Phs."),   range(0,360,180,1,0.001)
rslider  bounds(495,420, 60, 60),  channel("DC8"),  text("DC."),   range(0, 1, 0.5,1,0.001)

label bounds(0,480,100,10), text("Iain McCurdy |2015|")
</Cabbage>
                    
<CsoundSynthesizer>

<CsOptions>   
-dm0 -n
</CsOptions>

<CsInstruments>

; sr set by host
ksmps         =     32    ;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls         =     2    ;NUMBER OF CHANNELS (1=MONO)
0dbfs        =    1    ;MAXIMUM AMPLITUDE

seed    0    ; seed random number generators for the system clock
opcode    scale_i,i,iii
 ival,imax,imin    xin
 ival    =    (ival * (imax-imin)) + imin
    xout    ival
endop

instr    1
    kPhsrSpd    init    0.1        ; initial phasor speed
    kSpeed        chnget    "Speed"        ; speed
    kphasor    phasor    kPhsrSpd        ; phasor that moves the 'ball'
    kAmpMod    chnget    "AmpRte"        ; random partial strength modulation frequency
    kPhsMod    chnget    "PhsRte"        ; random phase modulation frequency
    kDCMod    chnget    "DCRte"            ; random DC offset modulation frequency
    kNorm    chnget    "Norm"            ; normalise on/off
    kSound    chnget    "Sound"            ; sound on/off
    kFriction    chnget    "Friction"    ; friction
    kDensity    chnget    "Density"    ; sound pulse density

    ; define a macro to read in and process attributes for each partial
    #define    MOD_PARTIAL(N)
    #
    kOn$N    chnget    "On$N"                        ; partial on/off (active switch)
    kPN$N    chnget    "PN$N"                        ; partial number
    kamp    chnget    "Amp$N"                        ; amount of strength modulation
    kphs    chnget    "Phs$N"                        ; amount of phase modulation
    kDC    chnget    "DC$N"                        ; amount of DC offset modulation
    kamp$N    rspline        0,kamp,    kAmpMod,kAmpMod        ; random partial strength function
    kamp$N    limit        kamp$N,-1,1                ; limit it (rspline can otherwise exceed its given limits)
    kphs$N    rspline        -kphs,kphs,    kPhsMod,kPhsMod        ; random phase function
    kDC$N    rspline        -kDC,kDC,      kDCMod, kDCMod        ; random DC offset function
    kDC$N    limit        kDC$N,-1,1                ; limit it (rspline can otherwise exceed its given limits)
    #
    ; expand macro for each partial required
    $MOD_PARTIAL(1)
    $MOD_PARTIAL(2)
    $MOD_PARTIAL(3)
    $MOD_PARTIAL(4)
    $MOD_PARTIAL(5)
    $MOD_PARTIAL(6)
    $MOD_PARTIAL(7)
    $MOD_PARTIAL(8)
    
    kmetro    metro    16            ; a metronome trigger used to restrict the rate of updates
    if kmetro==1 then            ; if trigger is generated...
     reinit REBUILD_TABLE            ; ... do a reinit in order to rebuild the table
    endif
    REBUILD_TABLE:
     iNorm    =    (i(kNorm) * 2) -1    ; Normalise value: 0 to 1 mapped to -1 to 1
     ; create the new updated table
     gifun    ftgen    1,0,512,19*iNorm,    i(kPN1),i(kamp1)*i(kOn1),i(kphs1),i(kDC1)*i(kOn1),    \
                          i(kPN2),i(kamp2)*i(kOn2),i(kphs2),i(kDC2)*i(kOn2),    \
                          i(kPN3),i(kamp3)*i(kOn3),i(kphs3),i(kDC3)*i(kOn3),    \
                          i(kPN4),i(kamp4)*i(kOn4),i(kphs4),i(kDC4)*i(kOn4),    \
                          i(kPN5),i(kamp5)*i(kOn5),i(kphs5),i(kDC5)*i(kOn5),    \
                          i(kPN6),i(kamp6)*i(kOn6),i(kphs6),i(kDC6)*i(kOn6),    \
                          i(kPN7),i(kamp7)*i(kOn7),i(kphs7),i(kDC7)*i(kOn7),    \
                          i(kPN8),i(kamp8)*i(kOn8),i(kphs8),i(kDC8)*i(kOn8)
    rireturn                ; out of reinit
    if kmetro==1 then            ; if trigger was '1' (i.e. and table was updated...
     chnset    "tableNumber(1)", "table"    ; update the table 'view'
     kndx    =    ftlen(gifun)*kphasor    ; convert phasor (0 to 1) to raw index (0 to 512(table size))
     kval    table    kphasor,gifun,1        ; read value from table (used to calculate ball graphical height
     k1    table    kndx-1,gifun        ; read the value before the widget location
     k2    table    kndx+1,gifun        ; read the value after the widget location
     kDlta    =    k2 - k1                    ; difference between the next table value and the previous table value to the ball location. This indicates slope (magnitude) and direction (sign)
     kPhsrSpd    =    kPhsrSpd - (kDlta*kSpeed)    ; acceleration/deceleration due to gravity 
     kPhsrSpd    =    kPhsrSpd * (1-(0.1*kFriction*(kSpeed^0.25)))    ; friction
     
     ; move the 'ball' widget (account for ball size, n.b. widget location refers to the top left hand corner)
     Smsg    sprintfk    "pos(%d,%d)", (kphasor*560)-5, 60+(120*-(kval*0.5))
         chnset        Smsg,"ball"
    endif
    
    ktrig    metro    abs(kPhsrSpd)*kDensity    ; trigger for sound pulses. Density depends on ball speed (regardless of direction (abs)) and Density slider
    schedkwhen    ktrig*kSound,0,0,2,0,0.3,kval,abs(kPhsrSpd),kphasor    ; trigger a sound pulse and send it table value (height), widget speed and y position as p4,p5 and p6

endin

instr    2    ; sound pulse
    ispd    limit    p5,0.05,0.4        ; limit speed values. Otherwise when the ball move very fast the sound sound gets too loud.
    asig    mpulse    0.2*(ispd^2),0        ; generate a click
    icfoct    scale_i    (p4*0.5)+1,10,5        ; derive a pitch value (in oct) from ball height
    icfoct    limit    icfoct,4,12        ; convert to CPS (and limit - use of unnormalised GEN19 can result in tables that extend way beyond the graphical limits)
    iLevel    chnget    "Level"
    asig    reson    asig*100*iLevel,cpsoct(icfoct),cpsoct(icfoct)*0.05,2    ; filter the click to produce a note
    aL,aR    pan2    asig,p6        ; pan the click
    aL    delay    aL,0.01*(1-p6)        ; spatialise
    aR    delay    aR,0.01*p6        ;
        outs    aL,aR
endin


</CsInstruments>

<CsScore>
i 1 0 3600
</CsScore>

</CsoundSynthesizer>
