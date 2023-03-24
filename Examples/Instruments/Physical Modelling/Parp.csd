
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; **NOT WORKING**

; Parp.csd
; Iain McCurdy, 2015

; Based on Perry Cook's waveguide model of someone struggling to play a trombone, wgbrass.

; Please note that this is a rather temperamental opcode so some patience is needed if the instrument fails to sound.
; The values for 'Breath' (power) and 'Lip Tension' are most important in getting the instrument to sound.
; The actual values needed for these two parameters will also depend upon the note played.
; Lower notes tend to require a lower breath power and/or higher lip tension (TENSION:Offset).

; Sometimes it is necessary just to restart a note in order to get it to sound.

; In order to create a modulation of pitch and tone, tension can be randomly modulated using the controls 'Mod.Dep.' and 'Mod.Rate' 

; Some presets are provided to offer useful starting points.


<Cabbage>
#define    SLIDER_STYLE # trackerColour( 50, 50,155), fontColour("black"), textColour("black") #
#define    CHECKBOX_STYLE # colour(255,255,100) #

form caption("Parp") size(980,221), pluginId("Parp"), colour(170,170,170)

image      bounds(  2,  2,976,130), colour(220,220,220), outlineColour("black"), outlineThickness(1)
rslider    bounds(  0, 25, 90, 90), valueTextBox(1), textBox(1), text("Breath"),     channel("amp"),      range(0.001,10,1,0.5),        $SLIDER_STYLE
line       bounds( 85,  5,  2,126)
image      bounds( 80, 10,230,105), colour(0,0,0,0), plant("Tension") { 
label      bounds(  0,  0,230, 12), text("T E N S I O N"), fontColour( 70, 70, 70)
rslider    bounds(  0, 15, 90, 90), valueTextBox(1), textBox(1), text("Offset"),  channel("tens"),     range(0,5,0.75),                $SLIDER_STYLE
rslider    bounds( 70, 15, 90, 90), valueTextBox(1), textBox(1), text("Mod.Dep"),  channel("TRndDep"),  range(0,0.8,0.1,0.5,0.001),              $SLIDER_STYLE
rslider    bounds(140, 15, 90, 90), valueTextBox(1), textBox(1), text("Mod.Rate"),  channel("TRndRte"),  range(0.01,100,8,0.5,0.01),      $SLIDER_STYLE
}
line       bounds(305,  5,  2,126)

button     bounds(320, 60, 50, 20), text("Mono","Poly"), channel("MonoPoly"), value(1)
rslider    bounds(365, 25, 90, 90), valueTextBox(1), textBox(1), text("Port.Time"), channel("PortTime"),  range(0, 5, 0.9), identChannel("PortTimeID"), visible(1), $SLIDER_STYLE

rslider    bounds(435, 25, 90, 90), valueTextBox(1), textBox(1), text("Spread"),   channel("PanDep"),   range(0,1,1),                 $SLIDER_STYLE
rslider    bounds(505, 25, 90, 90), valueTextBox(1), textBox(1), text("Stereo"),   channel("Stereo"),   range(0,1,1),                 $SLIDER_STYLE

image      bounds(585,  0,227,131), colour(0,0,0,0), plant("Reverb") { 
label bounds(  0, 10,227, 12), text("R E V E R B"), fontColour( 70, 70, 70) 
line bounds(  5,  5,  2,126) 
rslider    bounds(  0, 25, 90, 90), valueTextBox(1), textBox(1), text("Mix"),      channel("RvbMix"),   range(0,1,0.5), $SLIDER_STYLE 
rslider    bounds( 70, 25, 90, 90), valueTextBox(1), textBox(1), text("Size"), channel("RvbSize"),  range(0,1,0.005,0.5,0.001),   $SLIDER_STYLE 
rslider    bounds(140, 25, 90, 90), valueTextBox(1), textBox(1), text("Damp"),     channel("RvbDamp"), range(0,1,1),                 $SLIDER_STYLE 
line       bounds(225,  5,  2,126) 
}

label      bounds(820, 46, 75, 12), text("Preset"), fontColour("black")
combobox   bounds(820, 60, 75, 20), text("Preset 1","Preset 2","Preset 3","Preset 4","Preset 5"),    channel("Preset"), value(1)
rslider    bounds(885, 25, 90, 90), valueTextBox(1), textBox(1), text("Level"),    channel("outamp"),   range(0.001,100,3,0.5,0.001), $SLIDER_STYLE

keyboard   bounds(  2,134,976, 85)

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps = 16
nchnls = 2
0dbfs = 1

massign    0,2

opcode    lineto2,k,kk
 kinput,ktime    xin
 ktrig    changed    kinput,ktime                ; reset trigger
 if ktrig==1 then                    ; if new note has been received or if portamento time has been changed...
  reinit RESTART
 endif
 RESTART:                        ; restart 'linseg' envelope
 if i(ktime)==0 then                    ; 'linseg' fails if duration is zero...
  koutput    =    i(kinput)            ; ...in which case output simply equals input
 else
  koutput    linseg    i(koutput),i(ktime),i(kinput)    ; linseg envelope from old value to new value
 endif
 rireturn
         xout    koutput
endop

; FUNCTION TABLES MAP NOTE NUMBERS TO BREATH POWER (AMPSCL) AND OUTPUT LEVEL IN AN ATTEMPT TO PROVIDE A MORE PLAYABLE INSTRUMENT
giAmpScl    ftgen    0,0,128,-25, 0,0.001, 30,0.01, 36,0.04, 48,0.1, 60,0.2, 72,0.4, 84,1.5, 96,4, 108,10, 128,20
giLevScl    ftgen    0,0,128,-25, 0,10, 30,6, 36,4, 48,1.6, 60,1, 72,0.6, 84,0.15, 96,0.05, 108,0.02, 128,0.01

; EACH NOTE PLAYED MAPS TO A DIFFERENT (RANDOM BUT REPEATABLE) PANNING LOCATION
giPanDist    ftgen    0,0,128,21,1,1
gasendL,gasendR    init    0            ; REVERB SEND VARIABLE

instr    1
 kPortTime    linseg    0,0.001,0.05
 /* READ IN WIDGETS */
 kamp        chnget    "amp"
 gkamp        portk    kamp,kPortTime        ; SMOOTH CHANGES
 gktens       chnget    "tens"     
 gkoutamp     chnget    "outamp"     
 gkTRndDep    chnget    "TRndDep"    
 gkTRndRte    chnget    "TRndRte"    
 gkPanDep    chnget    "PanDep"
 kStereo    chnget    "Stereo"
 gkStereo    portk    kStereo,kPortTime    ; SMOOTH CHANGES
 gkRvbMix    chnget    "RvbMix"
 gkRvbSize    chnget    "RvbSize"
 gkRvbDamp    chnget    "RvbDamp"
 gkMonoPoly    chnget    "MonoPoly"
 gkPortTime    chnget    "PortTime"        ; PORTAMENTO TIME IN MONOPHONIC LEGATO MODE 
endin

    givfn    ftgenonce    0, 0, 256, 10, 1    ; VIBRATO WAVEFORM (A SINE WAVE) - (VIBRATO IS NOT USED)


instr    2                    ; RESPONDS TO MIDI NOTES PLAYED
 inum    notnum                    ; READ IN NOTE NUMBER
 gknum    =    inum                ; ASSIGN TO K-RATE GLOBAL VARIABLE FOR NOTE NUMBER (NEEDED IN MONOPHONIC LEGATO MODE)
 iactive    active    3            ; SENSE NUMBER OF ACTIVE NOTES IN SOUNDING INSTRUMENT
 if i(gkMonoPoly)==0&&iactive==0 then        ; IF MONOPHONIC/LEGATO MODE AND THIS IS THE FIRST NOTE PLAYED...
  event_i    "i",3,0,-1            ; START SOUNDING INSTRUMENT WITH A HELD NOTE
 elseif i(gkMonoPoly)==1 then            ; OTHERWISE IF POLYPHONIC MODE
  aL,aR    subinstr    3, inum            ; CALL SOUNDING INSTRUMENT AS A SUBINSTRUMENT
 endif                        ; END OF CONDITIONAL BRANCHING
endin

instr    3                    ; SOUNDING INSTRUMENT
 if active:k(2)==0 then                ; IF ALL MIDI NOTES (INSTR 2 RESPONDS TO MIDI) ARE RELEASED...
  turnoff                    ; TURN THIS INSTRUMENT OFF (ALLOW RELEASE SEGMENTS TO COMPLETE)
 endif                        ; END OF CONDITIONAL BRANCHING
 kPortTime    linseg    0,0.001,1        ; PORTAMENTO FUNCTION RAMPS UP RAPIDLY FROM ZERO TO A HELD VALUE OF '1'

 if release:k()==1 kgoto SKIP            ; IF NOTE IS IN RELEASE STAGE DO NOT READ IN ANY NEW NOTE VALUES
 if i(gkMonoPoly)==0 then            ; IF MONOPHONIC/LEGATO MODE...
  knum        lineto2    gknum,kPortTime*gkPortTime    ; ...GLIDE TO NEW NOTE VALUES (NOTE NUMBER IS DEFINED BY NEWEST MIDI NOTE PLAYED)
 elseif i(gkMonoPoly)==1 then            ; OTHERWISE (POLYPHONIC MODE)
  knum        init    p4            ; NOTE IS JUST DEFINED BY p4 VALUE SENT BY THE SUBINSTR OPCODE IN INSTR 2
 endif                        ; END OF CONDITIONAL BRANCHING
 SKIP:                        ; JUMP TO HERE IF IN RELEASE STAGE
 
 ktensrnd    rspline    -gkTRndDep, gkTRndDep, gkTRndRte, gkTRndRte*3    ; RANDOM SPLINE FUNCTION USED TO MODULATE LIP TENSION PARAMETER
 
 ktensenv    transeg    -0.2,.02,4,0
 ktensrnd    +=    ktensenv
 
 kAmpScl    tablei    knum,giAmpScl        ; READ AMP (BREATH POWER) SCALING VALUE ACCORDING NOTE NUMBER
 asig        wgbrass gkamp*kAmpScl, cpsmidinn(knum), gktens+ktensrnd, 0.01, 0, 0, givfn, 1    ; GENERATE A WGBRASS TONE
 kLevScl    tablei    knum,giLevScl        ; READ OUTPUT LEVEL SCALING VALUE ACCORDING NOTE NUMBER
 asig        *=    kLevScl            ; APPLY LEVEL SCALING VALUE
 
 kPan        tablei    knum,giPanDist        ; READ IN PAN POSITION FOR THE NOTE PLAYED FROM RANDOM DISTRIBUTION TABLE
 aL,aR        pan2    asig,(kPan*gkPanDep) + (0.5*(1-gkPanDep))     ; CREATE STEREO (PANNED) SIGNALS
 
 if gkStereo>0 then
  kRndDelL    rspline        0, 0.04*gkStereo, 0.1, 0.2    ; SPLINIC RANDOM DELAY TIME (LEFT CHANNEL)
  kRndDelR    rspline        0, 0.04*gkStereo, 0.1, 0.2    ; SPLINIC RANDOM DELAY TIME (RIGHT CHANNEL)
  kRndDelL    limit        kRndDelL, 0, 0.04        ; CONSTRAIN POSSIBLE OUT OF RANGE VALUE
  kRndDelR    limit        kRndDelR, 0, 0.04
  aRndDelL    interp        kRndDelL            ; CONVERT TO A-RATE WITH LINER INTERPOLATION
  aRndDelR    interp        kRndDelR
  aL        vdelay        aL, 0.1+(aRndDelL*1000), 0.1 + 40    ; CREATE SHORT DELAY VERSIONS OF BOTH CHANNELS WITH DIFFERENT DELAY TIMES
  aR        vdelay        aR, 0.1+(aRndDelR*1000), 0.1 + 40
 endif
     
 kenv        linsegr    1,0.1,0                    ; ANTI-CLICK ENVELOPE
 aL        *=    kenv                    ; APPLY ENVELOPE
 aR        *=    kenv
 gasendL    +=    aL                    ; MIX IN BOTH CHANNELS TO SEND SIGNAL FOR INSTR 99 (REVERB AND OUTPUT)
 gasendR    +=    aR

endin

instr    99                            ; REVERB AND OUTPUT
    aL,aR        freeverb    gasendL,gasendR,gkRvbSize,gkRvbDamp    ; FREEVERBED SIGNALS
    aL        ntrpol        gasendL/2,aL,gkRvbMix    ; MIX DRY/WET
    aR        ntrpol        gasendR/2,aR,gkRvbMix
            outs        aL*gkoutamp, aR*gkoutamp    ; SEND DRY/WET MIX TO OUTPUTS
            clear        gasendL,gasendR            ; CLEAR AUDIO SEND SIGNALS
endin

instr    100
 kPreset    chnget    "Preset"
 
 #define PRESET(amp'tens'TRndDep'TRndRte'MonoPoly'PortTime'PanDep'Stereo'RvbMix'RvbSize'RvbDamp'outamp)
 #
 chnset    $amp,           "amp"
 chnset    $tens,        "tens"    
 chnset    $TRndDep,    "TRndDep"    
 chnset    $TRndRte,    "TRndRte"    
 chnset    $MonoPoly,      "MonoPoly"    
 chnset    $PortTime,      "PortTime"    
 chnset    $PanDep,    "PanDep"    
 chnset    $Stereo,    "Stereo"    
 chnset    $RvbMix,    "RvbMix"    
 chnset    $RvbSize,    "RvbSize"    
 chnset    $RvbDamp,    "RvbDamp"    
 chnset    $outamp,    "outamp"    
 #

 if changed(kPreset)==1 then
  reinit SendPreset
  SendPreset:
  if i(kPreset)==1 then
   ;       amp 'tens  'TRndDep'TRndRte'MonoPoly'PortTime'PanDep'Stereo'RvbMix'RvbSize'RvbDamp'outamp)
   $PRESET(1   '0.75  '0.1    '8      '1       '0.9     '1     '1     '0.5   '0.005  '1      '3)     
  elseif i(kPreset)==2 then
   $PRESET(0.54'0.64  '0.0    '0.1    '1       '0.8     '0.5   '0.27  '0.5   '0.005  '1      '3)
  elseif i(kPreset)==3 then
   $PRESET(0.79'0.64  '0.065  '1.82   '1       '0.7     '1     '0.3   '0.5   '0.005  '1      '3)
  elseif i(kPreset)==4 then
   $PRESET(0.46'2.70  '0.131  '32     '1       '0.6     '0.75  '0.5   '0.5   '0.005  '1      '3)
  elseif i(kPreset)==5 then
   $PRESET(0.77'0.63  '0.065  '1.82   '0       '1.7     '1     '0.22  '1     '0.64   '1      '3)
  endif
 endif
endin

</CsInstruments>  

<CsScore>
i 1   0 [3600*24*7]    ; READ IN WIDGETS
i 3 0.1 3 60        ; INITIAL DUMMY NOTE - NEEDED TO PREVENT FIRST PLAYED NOTE FROM BEING SILENT. THIS IS A BIT MYSTERIOUS.
i 99  0 [3600*24*7]    ; REVERB AND OUTPUT
i 100 0 [3600*24*7]    ; PRESETS
</CsScore>

</CsoundSynthesizer>