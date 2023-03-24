
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; RubbedResonators.csd
; Written by Iain McCurdy, 2012. Updated 2014
; 
; This example introduces a method of 'exciting' mode filters that creates an imitation of a resonant object being bowed or rubbed. 
; The excitation signal consists of a stack of sine wave oscillators that match the number and frequencies of the mode filters being used. 
; This arrangement can cause the mode filters to resonate with great intensity so the amplitudes of the sine wave oscillators should be kept low to prevent overloading. 
; Additionally the frequency of each sine wave oscillator is individually modulated using a random 'jitter' function created using 'gaussi' (interpolating gaussian noise generator). 
; This technique results is a shifting spectral response from the filters. Adjusting the rate and amplitude of this jitter function can imitate a variety of bowing and rubbing techniques. 
; Increasing 'Jit.Dep.' while 'Jit.Rate' is kept low gives the impression of the resonator being rubbed with greater pressure. If both 'Jit.Dep.' and 'Jit.Rate' are high the impression is of the resonator begin brushed. 
; Input can also be from the live input.
; If 'Q' is low then more of the character of the impulse sound (the sine wave oscillators) will be apparent (and therefore the jitter modulation will be more apparent). If 'Q' is higher then the character of the mode filters will dominate and modulation of the impulse sound frequencies will be less apparent. A key aspect of this instrument is that once the impulse sound is removed ('Impulse Amp.' is brought to minimum) the sound produced is solely the residual resonance of the mode filters, therefore any modulation within the sine wave oscillators becomes irrelevant.
;
; Movement    -    choose between a random rubbing movement and a circular one
;
; Tuning
; ------
; Detune    -    raising this this adds an increasing amount of note-by-note fixed detuning
; Shift        -    shifts all notes (in semitones). This is intended to be used in conjunction with 'Compress'.
; Compress    -    increasingly compresses the interval between notes.

; 'Poly.Limit'    -    controls the maximum number of layers allowed. 
;             If an attempt to exceed the polyphony limit is made, the oldest held note will be removed.
;             This is useful for prevent CPU overloads.
; 
; 'Rel.Time'    -    defines the time over which a note will fade out if it has been removed on account of exceeding the polyphony limit 
; 
; For brightness (damping) control to work properly modal ratios in their function table will need to be in ascending order.
; 
; Chorus effect is disabled when 'Mix' is zero
; 
; Number of partials in the chosen algorithm is printed to the GUI for the user's information. Algorithms with high numbers of partials will demand more CPU and lower polyphony will be possible.

<Cabbage>
form caption("Rubbed Resonators"), size(640,290), pluginId("RubR"), guiRefresh(32)
image pos(0, 0),                   size(640,290), colour("Sienna"), shape("sharp"), oulinecolour("brown"), outlineThickness(4)

;EXCITATION
groupbox bounds(10, 10, 330, 90), text("Excitation"), fontColour("white"), plant("excitation"){
label    bounds(  5, 22,70,13), text("Input"), fontColour("white")
combobox bounds(  5, 35,70,18), text("Rubbing","Strike","Live","Ringer"), channel("Input"), value(1)
label    bounds(  5, 55,70,13), text("Movement"), fontColour("white")
combobox bounds(  5, 68,70,18), text("Random","Circular"), channel("Movement"), value(1)
rslider  bounds( 70, 25,60,60), text("Amp."), colour("Chocolate"), channel("ImpDB"), range(-70, 0, 0)
rslider  bounds(120, 25,60,60), text("HPF"), colour("Chocolate"), channel("HPF"), range(20, 20000, 20, 0.5)
rslider  bounds(170, 25,60,60), text("LPF"), colour("Chocolate"), channel("LPF"), range(20, 20000, 20000, 0.5)
rslider  bounds(220, 25,60,60), text("Jit. Dep."), channel("JitDep"), colour("Chocolate"), range(0, 5.00, 0.4,0.5)
rslider  bounds(270, 25,60,60), text("Jit. Rate"), channel("JitRte"), colour("Chocolate"), range(0.01,100, 3,0.5)
}

;RESONATORS
groupbox bounds(340, 10,290, 90), text("Resonators"), fontColour("white"), plant("resonators"){
label    bounds( 15, 21,140, 12), text("Instrument"), fontColour("white")
combobox bounds( 15, 36,140, 25), channel("sound"), value(21), text("Single", "Dahina", "Banyan", "Xylophone", "Tibetan Bowl 180mm", "Spinel Sphere", "Pot Lid", "Red Cedar Wood Plate", "Tubular Bell", "Redwood Wood Plate", "Douglas Fir Wood Plate", "Uniform Wooden Bar", "Uniform Aluminium Bar", "Vibraphone 1", "Vibraphone 2", "Chladni Plates", "Tibetan Bowl 152mm", "Tibetan Bowl 140mm", "Wine Glass", "Small Handbell", "Albert Clock Bell", "Wood Block","Harmonic 10","Harmonic 20","Harmonic 30","Harmonic Odd 10","Harmonic Odd 20","Anvil")
label    bounds( 11, 65,120, 12), text("Number of Partials:"), fontColour("white")
nslider  bounds(126, 62, 29, 20), channel("npartials"), range(1, 10000, 6, 1, 1)
rslider  bounds(160, 25, 60, 60), text("Q"), colour("orange"), channel("Q"), range(1, 10000, 2000, 0.5)
rslider  bounds(220, 25, 60, 60), text("Bright"), colour("orange"), channel("bright"), range(-4.00, 4, 0)
}

;TUNING                                        
groupbox bounds( 10,100,160, 90), text("Tuning"), fontColour("white"), plant("tuning"){
rslider  bounds(  0, 25, 60, 60), text("Detune"), colour("orange"), channel("detune"), range(0, 5, 0)
rslider  bounds( 50, 25, 60, 60), text("Shift"), colour("orange"), channel("shift"), range(0, 127, 0)
rslider  bounds(100, 25, 60, 60), text("Compress"), colour("orange"), channel("compress"), range(0, 1, 0)
}

;POLYPHONY                                        
groupbox bounds(170,100,220, 90), text("Polyphony"), fontColour("white"), plant("polyphony"){
button   bounds(  8, 25, 84, 20), text("Polyphonic","Monophonic"), channel("monopoly"), value(0)
hslider  bounds(  5, 43, 95, 38), colour("chocolate"), channel("GlissTime"), range(0.005,20, 0.1, 0.5, 0.0001)
label    bounds(  5, 74, 95, 12), text("Gliss Time")
rslider  bounds( 95, 25, 60, 60), text("Poly.Limit"), channel("PolyLimit"), range(0, 20, 5,1,1), colour("chocolate")
rslider  bounds(155, 25, 60, 60), text("Rel.Time"), channel("RelTim"), range(0.01, 5,0.2,0.5,0.01), colour("chocolate")
}

;CHORUS
groupbox bounds(390,100,160, 90), text("Chorus"), fontColour("white"), plant("chorus"){
rslider  bounds(  0, 25, 60, 60), text("Mix"), channel("ChoMix"), range(0, 1.00, 0.5), colour("yellow")
rslider  bounds( 50, 25, 60, 60), text("Depth"), channel("ChoDep"), range(0, 0.1, 0.01,0.5, 0.001), colour("yellow")
rslider  bounds(100, 25, 60, 60), text("Rate"), channel("ChoRte"), range(0, 20, 0.96, 0.5), colour("yellow")
}                                                            

;OUTPUT
groupbox bounds(550,100, 80, 90), text("Output"), fontColour("white"), plant("output"){
rslider  bounds( 10, 25, 60, 60), text("Level"), channel("OutLev"), range(0, 1.00, 0.25), colour("GoldenRod")
}

keyboard bounds(10, 195, 620, 85)

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

massign    0,2    ;MIDI DATA DIRECTED TO INSTRUMENT 2

;Author: Iain McCurdy (2012)

;FUNCTION TABLES STORING MODAL FREQUENCY RATIOS------------------------------------------------------------------------------------------------------------------------------------------------
;single
girtos1        ftgen    0,0,2, -2,    1,1

;dahina
girtos2        ftgen    0,0,-6,-2,    1,    2.89,    4.95,    6.99,    8.01,    9.02

;banyan
girtos3        ftgen    0,0,-6,-2,    1,     2.0,     3.01,     4.01,     4.69,     5.63

;xylophone
girtos4        ftgen    0,0,-6,-2,    1,     3.932,     9.538,    16.688,    24.566,    31.147

;tibetan bowl (180mm)
girtos5        ftgen    0,0,-7,-2,    1,     2.77828,    5.18099,     8.16289,    11.66063,    15.63801,    19.99

;spinel sphere with diameter of 3.6675mm
girtos6        ftgen    0,0,-24,-2,    1,    1.026513174725,    1.4224916858532,    1.4478690202098,    1.4661959580455,    1.499452545408,    1.7891839345101,    1.8768994627782,    1.9645945254541,    1.9786543873113,    2.0334612432847,    2.1452852391916,    2.1561524686621,    2.2533435661294,    2.2905090816065,    2.3331798413917,    2.4567715528268,    2.4925556408289,    2.5661806088514,    2.6055768738808,    2.6692760296751,    2.7140956766436,    2.7543617293425,    2.7710411870043 

;pot lid
girtos7        ftgen    0,0,-6,-2,    1,     3.2,     6.23,     6.27,     9.92,     14.15

;red cedar wood plate
girtos8        ftgen    0,0,-4,-2,    1,     1.47,     2.09,     2.56

;tubular bell
girtos9        ftgen    0,0,-10,-2,    272/437,    538/437,    874/437,    1281/437,    1755/437,    2264/437,    2813/437,    3389/437,    4822/437,    5255/437

;redwood wood plate
girtos10    ftgen    0,0,-4,-2,    1, 1.47, 2.11, 2.57

;douglas fir wood plate
girtos11    ftgen    0,0,-4,-2,    1, 1.42, 2.11, 2.47

;uniform wooden bar
girtos12     ftgen     0,0,-6,-2,      1, 2.572, 4.644, 6.984, 9.723, 12

;uniform aluminum bar
girtos13    ftgen    0,0,-6,-2,    1, 2.756, 5.423, 8.988, 13.448, 18.680

;vibraphone 1
girtos14    ftgen    0,0,-6,-2,    1, 3.984, 10.668, 17.979, 23.679, 33.642

;vibraphone 2
girtos15    ftgen    0,0,-6,-2,    1, 3.997, 9.469, 15.566, 20.863, 29.440

;Chladni plates
girtos16    ftgen    0,0,-5,-2,    1, 1.72581, 5.80645, 7.41935, 13.91935

;tibetan bowl (152 mm)
girtos17    ftgen    0,0,-7,-2,    1, 2.66242, 4.83757, 7.51592, 10.64012, 14.21019, 18.14027

;tibetan bowl (140 mm)
girtos18    ftgen    0,0,-5,-2,    1, 2.76515, 5.12121, 7.80681, 10.78409

;wine glass
girtos19    ftgen    0,0,-5,-2,    1, 2.32, 4.25, 6.63, 9.38

;small handbell
girtos20    ftgen    0,0,-22,-2,    1, 1.0019054878049, 1.7936737804878, 1.8009908536585, 2.5201981707317, 2.5224085365854, 2.9907012195122, 2.9940548780488, 3.7855182926829, 3.8061737804878, 4.5689024390244, 4.5754573170732, 5.0296493902439, 5.0455030487805, 6.0759908536585, 5.9094512195122, 6.4124237804878, 6.4430640243902, 7.0826219512195, 7.0923780487805, 7.3188262195122, 7.5551829268293 

;albert clock bell belfast
;girtos21    ftgen    0,0,-22,-2,     2.043260,1.482916,1.000000,3.328848,4.761811,1.477056,0.612007,2.661295,1.002793,4.023776,0.254139,2.043916,4.032463,2.659438,4.775560,5.500494,3.331014,0.809697,2.391301, 0.254098,1.901476,2.366563    ;,0.614968,2.046543,1.814887,3.130744,2.484426,0.558874,0.801697,0.070870,3.617036,2.782656
girtos21    ftgen    0,0,-22,-2,     0.254098,0.254139,0.612007,0.809697,1.000000,1.002793,1.477056,1.482916,1.901476,2.043260,2.043916,3.331014,2.366563,2.391301,2.659438,2.661295,3.328848,4.023776,4.032463,4.761811,4.775560,5.500494

;wood block
girtos22    ftgen    0,0,4,-2,    915/915,1540/915,1863/915,3112/915

;harmonic 10
girtos23    ftgen    0,0,-10,-2,    1,2,3,4,5,6,7,8,9,10

;harmonic 20
girtos24    ftgen    0,0,-20,-2,    1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20

;harmonic 30
girtos25    ftgen    0,0,-30,-2,    1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30

;harmonic odd 10
girtos26    ftgen    0,0,-10,-2,    1,3,5,7,9,11,13,15,17,19

;harmonic odd 20
girtos27    ftgen    0,0,-20,-2,    1,3,5,7,9,11,13,15,17,19,21,23,25,27,29,31,33,35,37,39

;anvil
girtos28    ftgen    0,0,-16,-2,    3690/5122,    3790/5122,    4710/5122,    5122/5122,    5295/5122,    5843/5122,    6595/5122,    7570/5122,    8590/5122,    9770/5122,    11370/5122,    11915/5122,    12700/5122,    14575/5122,    16780/5122,    17103/5122

/*
1    3690    -17
2    3790    -21
3    4710    -20
4    5122    -23
5    5295    -15
6    5843    -18
7    6595    -21
8    7570    -18
9    8590    -22
10    9770    -17
11    11370    -20
12    11915    -30
13    12700    -28
14    14575    -30
15    16780    -35                                   
16    17103    -34
*/

seed    0            ;random number generators seeded from the system clock
gasend    init    0        ;initialise the global audio send variable
gisine    ftgen    0,0,4096,10,1    ;a sine wave
gidetuning    ftgen    0,0,128,21,6,1            ; random array used for fixing unique detune values for each note
gihanning        ftgen    0,0,4097,20,2
;table that stores indicators for each active note. 1=active 0=inactive index_location=note_number
giNoteActive    ftgen    0,0,128,2,0                                                                                                                                                             
;--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

gkbright    init    0
gasend        init    0
gkactive     init     0    ; total number of active notes
gkPolyLimit    init    5

;--UDOS--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
opcode    scale_i,i,iii
 ival,imax,imin    xin
 ival    =    (ival * (imax-imin)) + imin
    xout    ival
endop

opcode    SineVoice,a,kkkkiiii
    kfreq,kMovement,kjitdep,kjitrte,icount,invoices,irtos,isfn    xin
    amix    =    0                                ;initialise audio mix variable (mixture of all sine wave oscillators)
    if kMovement==1 then
     kjit    gaussi    kjitdep, 1, kjitrte                        ;jitter function (semitone deviation)
    else
     kjit    oscil    kjitdep,kjitrte*randomi:k(0.8,1.2,1,1),-1,rnd(1)
    endif
    irto    table    icount-1,irtos                            ;read the frequency ratio for this oscillator from the appropriate ratios table
    a1    poscil    1,a(semitone(kjit)*kfreq*irto),isfn                ;create the oscillator
    if icount<invoices then                                ;if not all required oscillators have been created yet... 
     amix    SineVoice    kfreq,kMovement,kjitdep,kjitrte,icount+1,invoices,irtos,isfn    ;call the udo again, with the incremented counter
    endif
    xout    amix + a1                                ;send mix of all oscillators back the the udo output
endop

opcode    ModeVoice,a,akkkiii                        ;mode udo (k-rate base frequency) - used for non-midi mode 
    ain,kfreq,kq,kdamp,icount,inmodes,irtos  xin            ;name input variables
    amix    =    0                        ;initialise audio mix variable (mixture of all mode filters)
    asig    =    0                        ;initialise audio for the mode filter in this iteration of the UDO. Only needed if only 1 mode is present.
    irto    table    icount-1,irtos                    ;read frequency ratio for current mode according to iteration counter
    kThisFreq    =    kfreq * irto                ;derive modal frequency from product of ratio and base frequency

    ; legato smearing
    /*
    kporttime    linseg    0,0.001,1
    kThisFreq    portk    kThisFreq,kporttime*rspline:k(0.5,2,0.3,0.7)
    */
    
    if sr/kThisFreq>=$M_PI then
     asig    mode    ain, a(kThisFreq), kq                ;create mode signal
     if kdamp>0 then
      kamp    =    ((inmodes-icount+1)^(-kdamp))*(kdamp+1)
     else
      kamp    =    icount^kdamp
     endif
     aamp    interp    kamp
     asig    =    asig*aamp
     if    icount < inmodes    then                        ;if all the modes in this current instrument have not yet been created...
      amix        ModeVoice    ain, kfreq,kq,kdamp,icount+1, inmodes,irtos    ;...call modemodule udo again with incremented counter
     endif                                        ;end of conditional branching
    endif
    xout    amix+asig                                ;send all audio back to caller instrument
endop                                            ;end of udo

opcode    sspline,k,Kiii
    kdur,istart,iend,icurve    xin                                        ;READ IN INPUT ARGUMENTS
    imid    =    istart+((iend-istart)/2)                                ;SPLINE MID POINT VALUE
    isspline    ftgentmp    0,0,4096,-16,istart,4096*0.5,icurve,imid,(4096/2)-1,-icurve,iend    ;GENERATE 'S' SPLINE
    kspd    =    i(kdur)/kdur                                        ;POINTER SPEED AS A RATIO (WITH REFERENCE TO THE ORIGINAL DURATION)
    kptr    init    0                                            ;POINTER INITIAL VALUE    
    kout    tablei    kptr,isspline                                        ;READ VALUE FROM TABLE
    kptr    limit    kptr+((ftlen(isspline)/(i(kdur)*kr))*kspd), 0, ftlen(isspline)-1            ;INCREMENT THE POINTER BY THE REQUIRED NUMBER OF TABLE POINTS IN ONE CONTROL CYCLE AND LIMIT IT BETWEEN FIRST AND LAST TABLE POINT - FINAL VALUE WILL BE HELD IF POINTER ATTEMPTS TO EXCEED TABLE DURATION
        xout    kout                                            ;SEND VALUE BACK TO CALLER INSTRUMENT
endop
;--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

instr    1    ;read in widgets (always on)
    gkInput     chnget    "Input"
    gkMovement  chnget    "Movement"
    gkQ         chnget    "Q"            ;read in widgets
    kImpDB      chnget    "ImpDB"
    gkHPF       chnget    "HPF"
    gkLPF       chnget    "LPF"
    gkJitDep    chnget    "JitDep"
    gkJitRte    chnget    "JitRte"
    gkbright    chnget    "bright"
    gkdetune    chnget    "detune"
    gkdetune    portk    gkdetune,0.1
    gkshift     chnget    "shift"
    gkshift     portk    gkshift,0.1
    gkcompress  chnget    "compress"    
    gkcompress  portk    gkcompress,0.1
    gkmonopoly  chnget    "monopoly"
    gkGlissTime chnget    "GlissTime"
    gksound     chnget    "sound"
    gkChoMix    chnget    "ChoMix"
    gkChoRte    chnget    "ChoRte"
    gkChoDep    chnget    "ChoDep"
    gkOutLev    chnget    "OutLev"
    gkOutLev    port    gkOutLev,0.05
    gkImpAmp    =    ampdbfs(kImpDB)        ;convert from dB value to amp value
    gkbright    port    gkbright,0.02        ;smooth changes using portamento
    ktrig       changed    gksound                ;if sound combobox changes...
    if ktrig==1 then                
     event    "i",5,0,0.001                ;...call instrument 5 to update 'N.Partials' number box
    endif
endin

instr    2    ;MIDI TRIGGERED INSTRUMENT
    gkNoteTrig    init    1    ;at the beginning of a new note set note trigger flag to '1'    
    inum    notnum
    gknum    =    inum
    ivel    ampmidi    1                        ;read in midi note velocity as a value within the range 0 to 1
    gkvel    init    ivel
    inum    notnum                            ;read in midi note number as an integer (used for create a table of active notes flags)
    krelease    release                        ;note release flag (0 = note held, 1 = note released)
        tablew    1-krelease,inum,giNoteActive            ;write note release flag to table at the location corresponding to note number
    iactive    active    p1+1
        
    if iactive==0||i(gkmonopoly)==0 then    ; if polyphonic mode or if this is the first in a series of notes in monophonic mode...
     event_i    "i",p1+1,0,3600,inum        ; call instrument 3 with the appropriate p-fields
    endif
endin

instr    3                ;impulse and modal resonators instrument
    inum        =    p4                    ;note number
    
    /* POLYPHONY CONTROL */
    iPolyLimit    chnget    "PolyLimit"
    if iPolyLimit>0 then              
     gkactive init i(gkactive) + 1    ; INCREMENT NOTE COUNTER
     if gkactive>iPolyLimit then    ; IF POLYPHONY IS EXCEEDED (THROUGH THE ADDITION OF NEW NOTE)
      turnoff                        ; REMOVE THIS NOTE
     endif
     krel release                    ; IF NOTE HELD = 0, IF NOTE RELEASED = 1
     ktrig trigger krel,0.5,0        ; WHEN RELEASE FLAG CROSSES 0.5 UPWARDS, I.E. NOTE HAS BEEN RELEASED...    
     if ktrig==1 then        
      gkactive = gkactive - 1        ; ...DECREMENT ACTIVE NOTES COUNTER
     endif                                                                                                                                                                                                
    endif

    ktrig    changed    gksound
    if ktrig==1 then
     reinit RESTART_INSTRUMENT                                                                                                                                                         
    endif
    RESTART_INSTRUMENT:
    isound    init    i(gksound)

    if gkmonopoly==1 then
    /* monophonic */
     kNoteTrig    changed    gknum                    ;...GENERATE A TRIGGER IS A NEW NOTE NUMBER IS GENERATED (FROM INSTR. 1)
     gkNoteTrig    =    0
     gkOldNum    init    p4                        ;OLD NOTE NUMBER = FIRST NOTE NUMBER UPON INITIAL NOTE BEING PLAYED 
     if kNoteTrig==1 then                        ;IF A NEW (LEGATO) NOTE HAS BEEN PRESSED
      reinit    S_CURVE                            ;BEGIN A REINITIALISATION PASS FROM LABEL
     endif                                        ;END OF CONDITIONAL BRANCH
     S_CURVE:                                    ;A LABEL. REINITIALISATION BEGINS FROM HERE.
     knum    sspline    gkGlissTime,i(gkOldNum),i(gknum),1                     ; CALL sspline UDO (FIXED PORTAMENTO TIME)
     rireturn                                    ;RETURN FROM INITIALISATION PASS
     gkOldNum    =    knum                        ;SET OLD NUMBER CURRENT NUMBER     
     ;kporttime    linseg    0,0.001,1
     ;knum    portk    gknum,kporttime*gkGlissTime                                                                  
     kactive        active    p1-1
     kactive        limit    kactive,0,1    
    else
    /* polyphonic */
     knum        =    p4                            ;base note number read from p4    
     kactive        table    inum,giNoteActive            ;check whether the midi key corresponding to this note is being held or has been released (1 = held, 0 = released). this value will be used to control whether the impulse scound should be active or not.
    endif
    
    /*detuning*/
    idtn        table    inum,gidetuning
    kdtn        =        idtn * gkdetune

    knum2        =        (knum * (1-gkcompress)) + gkshift     ; shifted and compressed version  of knum
    
    icount        =    1                        ;counter to count iterations for recursive udos for sine oscillators and mode filters (starts at zero)
    irtos        =    girtos1 + isound - 1    ;derive actual function table number for ratios according to 'sound' chosen
    invoices    =    ftlen(irtos)            ;derive the number of voices needed for the 'sound' chosen
    kactivePort    port    kactive,0.05        ;smooth note active on/off switching
    ktrig changed gkNoteTrig                ;if a new note is started...
    if ktrig==1 then                        
     reinit RESTART_ENVELOPE                    ;...reinitialise from the given label
    endif
    RESTART_ENVELOPE:                        ;if a new note is trigger reinitialisation from here occurs to start a new impulse buid-up envelope based on new velocity value
    ivel    init    i(gkvel)
    kenv        linseg    0,(1-ivel)+0.01,ivel            ;create an amplitude envelope for the impulse sound. impulse sound will be velocity sensitive and the attack time of this envelope will be proportionate to the note velocity also. 
    rireturn

    ktrig    changed    gksound                    ; to fix bug where sound would not update in mono mode
    if ktrig==1 then
     reinit RESTART_INSTRUMENT2
    endif
    RESTART_INSTRUMENT2:

    ;; impulse
    if gkInput==1 then
     aSineMix    SineVoice    cpsmidinn(knum2)*semitone(kdtn),gkMovement,gkJitDep,gkJitRte,icount,invoices,irtos,gisine    ; call sine oscillator udo (it will be recursively recalled within the udo the appropriate number of times according to invoices)
     aSineMix    =    aSineMix*0.002*kactivePort*kenv*gkImpAmp                        ; scale the mixture of sines audio signal
     aSineMix    buthp    aSineMix,gkHPF
     aInput        butlp    aSineMix,gkLPF     
    elseif gkInput==2 then                    ; strike impulse
     itim    scale_i    ivel^0.125,0.001,0.03
     aInput        tablei    line:a(0,itim,1),gihanning,1
     aInput        *=        ivel
     aInput        buthp    aInput,gkHPF
     aInput        butlp    aInput,gkLPF     
    elseif gkInput==3 then                    ; live
     a1,a2        ins
     aInput    =    (a1+a2)*kactivePort*gkImpAmp          
     aInput        buthp    aInput,gkHPF
     aInput        butlp    aInput,gkLPF     
    else ; ringer
     aInput        gausstrig  1, 12, 0.1
     aInput        *=         linsegr:k(1,0.3,0)
    endif
    amodes        ModeVoice    aInput,cpsmidinn(knum2)*semitone(kdtn),gkQ,gkbright,icount,invoices,irtos    ; call sine oscillator udo (it will be recursively recalled within the udo the appropriate number of times according to invoices)
    amodes        =        (amodes*0.2*gkOutLev)/invoices                                ; scale the amplitude of the sound according to the number of modes in the chosen algorithm

    /* POLYPHONY LIMIT ENVELOPE */
    if iPolyLimit>0 then
     iRelTim    chnget    "RelTim"
     aRelEnv    linsegr    1,iRelTim,0
     amodes    =    amodes * aRelEnv
    endif

    /* TRACK OUTPUT AMPLITUDE. REMOVE SILENT NOTES */
    krms        rms        amodes                                ;track the amplitude of the sound as an rms value
    ithreshold    =    0.00001
    if krms<ithreshold&&kactive=0 then            ;if sound has died away and key is inactive...
     turnoff                        ;turn instrument off
    endif
    ;xtratim    0.1                        ;this is needed to prevent initialisation crashes, I'm not sure why 

            outs    amodes,amodes            ;send mode filters output mixture to output and reScale according to the number of modes used
    gasend    =    gasend+amodes*gkChoMix
    gkNoteTrig    =    0                ;reset new-note trigger (in case it was '1')
endin

instr    4    ;Chorus effect
    if gkChoMix==0 goto SKIP_CHORUS
    kporttime    linseg    0,0.001,1
    kporttime    =    kporttime/gkChoRte
    kdlt1    randomi    ksmps/sr,gkChoDep,gkChoRte,1
    kdlt1    portk    kdlt1,kporttime
    adlt1    interp    kdlt1
    acho1    vdelay    gasend,adlt1*1000,1*1000
    
    kdlt2    randomi    ksmps/sr,gkChoDep,gkChoRte,1
    kdlt2    portk    kdlt2,kporttime
    adlt2    interp    kdlt2
    acho2    vdelay    gasend,adlt2*1000,1*1000
    
    kpan1    randomi    0,1,gkChoRte,1
    kpan2    randomi    0,1,gkChoRte,1
    a1L,a1R    pan2    acho1,kpan1
    a2L,a2R    pan2    acho2,kpan2
    achoL    =    a1L+a2L
    achoR    =    a1R+a2R
        outs    achoL, achoR
        clear    gasend
    SKIP_CHORUS:
endin

instr    5    ;update 'Number of Partials' number box
    invoices    =    ftlen(girtos1+i(gksound)-1)    ;derive the number of voices needed for the 'sound' chosen
    knvoices    init    invoices            ;create krate version of invoices
    chnset        knvoices,"npartials"            ;send value to number box
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]
i 4 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>