
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Clavinet.csd - an emulation of the Hohner Clavinet D6
; -----------------------------------------------------
; 
; Pickup Selection
; ----------------
; CA - neck pickup only (pickup A)
; CB - bridge pickup only (pickup B)
; DA - both pickups in phase
; DB - bridge pickup (pickup B) 180 degrees out of phase w.r.t the neck pickup 
; 
; Setup
; -----
; Pick A - position of the first pickup (range 0 - 1). Typically referred to as the 'neck' pickup
; Pick B - position of the second pickup (range 0 - 1). Typically referred to as the 'bridge' pickup
;  (NB. the relevant pickup has to be 'active' according to the CD AB switches in order for you to be able to here changes to it pickup position.)
; Mod.A - the pickup position can also modulated by an LFO. This control controls the depth of this modulation
; Mod.B - Depth of this modulation for the second pickup
; Rate A - Rate of modulation for the first pickup
; Rate B - Rate of modulation for the second pickup
; Env.A - The pickup position can be modulated by a simple single segment envelope. This is typically used to add an particualar attack nuance to the sound. This control controls the initial amplitude of this envelope (can be positive or negative). The envelope always ends at zero.
; Env.B - Envelope amplitude for the second pickup.
; Time A - Duration of the envelope segment for the first pickup
; Time B - Duration of the envelope segment for the second pickup
; Pluck - location of the 'pluck' (range 0 - 1). Should be close to zero for a typical clavinet behaviour
; Damp - string vibration damping that is applied while a note is being held. String damping is also dependent upon the note played using a 'key tracking' function table 'girefl'. In general lower notes should experience greater damping.
; HPF - this controls the cutoff frequency of a highpass filter that the signal passes through. 
;       The value supplied here is multiplied by the fundemental frequency of the note played to derive the cutoff frequency.
;       Therefore if a value of '2' is supplied the cutoff will always by twice the fundemental, i.e. the frequency of the first harmonic/second partial.
;       This control can be used to remove the emphasis of the fundemental partial.
;       If this control is at its minimum setting the filter is bypassed.
; LPF - behaves in a similar way to HPF but in respect to a lowpass filter. This control, if reduced, can be used to soften the brightness of a note's attack.
;       If this control is at its maximum setting the filter is bypassed.
; Inharm - applies inharmonicity to the spectrum produced by repluck. Negative values compress the spectrum, positive values expand the spectrum and zero leaves the spectrum unaffected. 
; 
; Release
; -------
;  * Poorly maintained clavinets exhibit a release 'pluck' as the rubber pad which strikes and frets is lifted from the string when the key is released. The is on account of the rubber pad wearing out.
; Ampl - amplitude of the release 'pluck'
; Tone - a tone control applied to the release 'pluck' (this will also affect amplitude)
; D.Time - time it takes for full damping to be applied (this control has a subtle effect)
; Damp - amount of damping that is applied to string vibrations whenever a key is
; released (this amount of damping is achieved after 'D.Time' seconds). Note that the 'Damp'
; control in the 'Setup' pop-up panel also has an impact upon the release damping.
; A.Time - amplitude release time. If this is too short, vibration damping may not be heard and the above controls may not appear to do anything 
; 
; Filters
; -------
;  The D6 Clavinet design incorporates four bandpass filters which can be switched in or out of the circuit.
;  Emphasising four different frequency bands from high to low they are named 'brilliant', 'treble', 'medium' and 'soft'
;  Cutoff frequencies (fco), levels and Q values for each of the four filters can be adjusted.
;  Obviously the relevant filter has to be activated in order to be able to hear changes made to its settings.
;  The four filters are in a parallel arrangement as opposed to being in series.

<Cabbage>
form caption("Clavinet"), size(550, 170), pluginId("clav"), colour(228,156,28), guiRefresh(64)

#define RSliderStyle textColour(black), outlineColour(50,50,50)

image bounds(0,0,550,170), colour(228,156,28), shape("sharp")

; filter switches
checkbox bounds( 10,  5, 70, 15), text("Brilliant") channel("brilliant"), fontColour:0(50,50,50), fontColour:1(50,50,50), colour("lime")
checkbox bounds( 10, 25, 70, 15), text("Treble") channel("treble"), fontColour:0(50,50,50), fontColour:1(50,50,50), colour("lime"), value(1)
checkbox bounds( 10, 45, 70, 15), text("Medium") channel("medium"), fontColour:0(50,50,50), fontColour:1(50,50,50), colour("lime")
checkbox bounds( 10, 65, 70, 15), text("Soft") channel("soft"), fontColour:0(50,50,50), fontColour:1(50,50,50), colour("lime")

; pickup configuration switches
checkbox bounds( 80,  5, 70, 15), text("C/D") channel("C_D"), fontColour:0(50,50,50), fontColour:1(50,50,50), colour("lime"), value(1)
checkbox bounds( 80, 25, 70, 15), text("A/B") channel("A_B"), fontColour:0(50,50,50), fontColour:1(50,50,50), colour("lime"), value(1)

; controls pertaining to the setup of the instrument (pickup positions, pluck position, damping etc.) accessed in a pop-up panel
button   bounds(140,  5, 100, 30), channel("setup_Button"), text("Setup")
groupbox bounds(140, 25, 232, 305),  colour(228,156,28), plant("Setup"), outlineThickness(0), popup(1), identChannel("setupPlant")
{
image   bounds(  0,  0,172, 305),  colour(228,156,28)
rslider bounds(  5,  8, 55, 55), text("Pick A"), colour(195,126,  0), channel("pickA"),   range(0, 1.00, 0.25, 1, 0.001), textColour(black),  $RSliderStyle
rslider bounds( 55,  8, 55, 55), text("Pick B"), colour(195,126,  0), channel("pickB"),   range(0, 1.00, 0.07, 1, 0.001), textColour(black),  $RSliderStyle
rslider bounds(  5, 68, 55, 55), text("Mod A"),  colour(225,156, 28), channel("ModA"),   range(0,0.50, 0.03, 0.5, 0.001), textColour(black), $RSliderStyle
rslider bounds( 55, 68, 55, 55), text("Mod B"),  colour(225,156, 28), channel("ModB"),   range(0,0.50, 0.03, 0.5, 0.001), textColour(black), $RSliderStyle
rslider bounds(  5,128, 55, 55), text("Rate A"), colour(255,186, 58), channel("RateA"),   range(0,14.00, 0.2, 0.5, 0.001), textColour(black), $RSliderStyle
rslider bounds( 55,128, 55, 55), text("Rate B"), colour(255,186, 58), channel("RateB"),   range(0,14.00, 0.15, 0.5, 0.001), textColour(black), $RSliderStyle
rslider bounds(  5,188, 55, 55), text("Env.A"),  colour(255,216, 88), channel("EnvA"),   range(-1, 1.00, 0), textColour(black), $RSliderStyle

rslider bounds( 55,188, 55, 55), text("Env.B"),  colour(255,216, 88), channel("EnvB"),   range(-1, 1.00, 0), textColour(black), $RSliderStyle
rslider bounds(  5,248, 55, 55), text("Time A"), colour(255,246,118), channel("TimeA"),   range(0, 1.00, 0.05, 0.5, 0.001), textColour(black), $RSliderStyle
rslider bounds( 55,248, 55, 55), text("Time B"), colour(255,246,118), channel("TimeB"),   range(0, 1.00, 0.05, 0.5, 0.001), textColour(black), $RSliderStyle
line bounds(110, 25,  1, 260), colour( 50, 50, 50)
rslider bounds(112,  8, 55, 55), text("Pluck"),  colour(195,126,  0), channel("plk"),     range(0, 1.00, 0.005, 0.5, 0.0001), textColour(black), $RSliderStyle
rslider bounds(112, 68, 55, 55), text("Damp"),   colour(225,156, 28), channel("SusDamp"), range(-1.00, 1.00, 0), textColour(black), $RSliderStyle
rslider bounds(112,128, 55, 55), text("HPF"),    colour(255,186, 58), channel("HPF"),     range(0,16.0, 0), textColour(black), $RSliderStyle
rslider bounds(112,188, 55, 55), text("LPF"),    colour(255,216, 88), channel("LPF"),     range(1.0,72, 72, 0.375, 0.0001), textColour(black), $RSliderStyle
line bounds(170, 25,  1, 260), colour( 50, 50, 50)
rslider bounds(172,  8, 55, 55), text("Inharm."),colour(255,216, 88), channel("inharm"),  range(-0.99,0.99, 0), textColour(black), $RSliderStyle
rslider bounds(172, 68, 55, 55), text("Mix"), colour(255,216, 88), channel("FSMix"),  range(0, 1, 1), textColour(black), $RSliderStyle
rslider bounds(172,128, 55, 55), text("Decay"), colour(255,216, 88), channel("FSEnvTime"),  range(0, 1, 0), textColour(black), $RSliderStyle
rslider bounds(172,188, 55, 55), text("Feedback"), colour(255,216, 88), channel("FS_FB_Amt"),  range(0, 0.6, 0), textColour(black), $RSliderStyle
button  bounds(174,248, 51, 20), text("Key Follow","Key Follow"), fontColour:0(100,100,100), fontColour:1(255,255,170), value(0), latched(1), channel("FSKeyFollow") 
}

; controls pertaining to the release stage of a note accessed in a pop-up panel
button bounds(250,   5, 100, 30), channel("release_Button"), text("Release")
groupbox bounds(250, 25, 265, 65),  colour(228,156,28), plant("Release"), outlineThickness(0), popup(1), identChannel("releasePlant")
{
image   bounds(  0,  0,265, 65),  colour(228,156,28)
rslider bounds(  5,  8, 55, 55), text("Ampl."),  colour(255,186,58), channel("RelAmpl"),  range(0, 20.00, 2), textColour(black), $RSliderStyle
rslider bounds( 55,  8, 55, 55), text("Tone"),   colour(255,186,58), channel("RelTone"),  range(100, 8000, 1000), textColour(black), $RSliderStyle  
rslider bounds(105,  8, 55, 55), text("D.Time"), colour(255,186,58), channel("RelDTime"), range(0.001, 1, 0.01, 0.5, 0.001), textColour(black), $RSliderStyle
rslider bounds(155,  8, 55, 55), text("Damp"),   colour(255,186,58), channel("RelDamp"),  range(0.10, 0.99, 0.95), textColour(black), $RSliderStyle
rslider bounds(205,  8, 55, 55), text("A.Time"), colour(255,186,58), channel("RelATime"), range(0.01,20, 0.05, 0.5, 0.01), textColour(black), $RSliderStyle
}

; controls pertaining to the setup of the instrument's bandpass filters accessed in a pop-up panel
button bounds(360,  5, 100, 30), channel("filters_Button"), text("Filters")
groupbox bounds(360, 25, 230, 195),  colour(228,156,28), plant("Filters"), outlineThickness(0), popup(1), identChannel("filtersPlant")
{
image    bounds(  0,  0,230,195),  colour(228,156,28)
label    bounds( 10,  5, 45, 12), text("Brilliant"), colour(228,156,28), fontColour(50,50,50)
label    bounds( 64,  5, 45, 12), text("Treble"),    colour(228,156,28), fontColour(50,50,50)
label    bounds(121,  5, 45, 12), text("Medium"),    colour(228,156,28), fontColour(50,50,50)
label    bounds(174,  5, 45, 12), text("Soft"),      colour(228,156,28), fontColour(50,50,50)
checkbox bounds( 26, 18, 10, 10), channel("brilliant"), colour("lime"), value(0), shape("ellipse"), active(0)
checkbox bounds( 81, 18, 10, 10), channel("treble"),    colour("lime"), value(0), shape("ellipse"), active(0)
checkbox bounds(136, 18, 10, 10), channel("medium"),    colour("lime"), value(0), shape("ellipse"), active(0)
checkbox bounds(191, 18, 10, 10), channel("soft"),      colour("lime"), value(0), shape("ellipse"), active(0)
rslider  bounds(  5, 30, 55, 55), channel("fco1"),  text("fco.1"),  colour(235,166,38),  range(50,5000,2500,0.5), textColour(black), $RSliderStyle
rslider  bounds( 60, 30, 55, 55), channel("fco2"),  text("fco.2"),  colour(235,166,38),  range(50,5000,1000,0.5), textColour(black), $RSliderStyle
rslider  bounds(115, 30, 55, 55), channel("fco3"),  text("fco.3"),  colour(235,166,38),  range(50,5000, 500,0.5), textColour(black), $RSliderStyle
rslider  bounds(170, 30, 55, 55), channel("fco4"),  text("fco.4"),  colour(235,166,38),  range(50,5000, 200,0.5), textColour(black), $RSliderStyle
rslider  bounds(  5, 85, 55, 55), channel("gain1"), text("Gain 1"), colour(255,216,88),  range(0.50,30,  3,0.5), textColour(black), $RSliderStyle
rslider  bounds( 60, 85, 55, 55), channel("gain2"), text("Gain 2"), colour(255,216,88),  range(0.50,30,2.3,0.5), textColour(black), $RSliderStyle
rslider  bounds(115, 85, 55, 55), channel("gain3"), text("Gain 3"), colour(255,216,88),  range(0.50,30,1.3,0.5), textColour(black), $RSliderStyle
rslider bounds(170, 85, 55, 55), channel("gain4"),  text("Gain 4"), colour(255,216,88),  range(0.50,30,1.4,0.5), textColour(black), $RSliderStyle
rslider bounds(  5,140, 55, 55), channel("Q1"),     text("Q 1"),    colour(255,255,138), range(0.50,30,1.25,0.5), textColour(black), $RSliderStyle
rslider bounds( 60,140, 55, 55), channel("Q2"),     text("Q 2"),    colour(255,255,138), range(0.50,30,1   ,0.5), textColour(black), $RSliderStyle
rslider bounds(115,140, 55, 55), channel("Q3"),     text("Q 3"),    colour(255,255,138), range(0.50,30,1   ,0.5), textColour(black), $RSliderStyle
rslider bounds(170,140, 55, 55), channel("Q4"),     text("Q 4"),    colour(255,255,138), range(0.50,30,2   ,0.5), textColour(black), $RSliderStyle
line bounds( 59, 25,  1, 150), colour(50,50,50)
line bounds(114, 25,  1, 150), colour(50,50,50)
line bounds(169, 25,  1, 150), colour(50,50,50)
}

rslider  bounds(470,  5, 65, 65), text("Level"), colour(255,186,58), channel("level"), range(0, 8.00, 0.5, 0.5), textColour(black), $RSliderStyle

; presets
image      bounds(250, 45,100, 37), colour(228,156, 28), outlineColour( 50, 50, 50), outlineThickness(1), shape("sharp"),plant("presets"){
label      bounds( 10,  2, 80, 11), text("PRESETS"), fontColour( 50, 50, 50)
combobox   bounds( 10, 15, 80, 18), channel("preset"), value(1), text("Clavinet 1", "Clavinet 2", "Clavinet 3", "Clavinet 4", "Clavinet 5", "Clavinet 6", "Clavinet 7", "Clavinet 8", "Clavinet 9", "Clavinet 10")
}

keyboard bounds(  5, 85,540, 80)
label bounds(440, 73,  110, 11), text("Iain McCurdy |2013|"), fontColour(50,50,50)

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps         =     16
nchnls         =     2
0dbfs        =    1
massign    0,2

; table used to map of note played to bridge relection
girefl    ftgen    0,0,128,-27, 0,0.4, 36,0.4, 72,0.01, 127,0.01
gisine    ftgen    0,0,4096,10,1
gitri    ftgen    0,0,4096,7,0,1024,1,2046,-1,1024,0

opcode    FreqShifter,a,aki
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


instr    1    ; presets
 kval1    =    0.5
 kval2    =    1
 kpreset    chnget    "preset"
 ktrig        changed    kpreset
 if ktrig==1 then
  reinit    PRESET
  turnoff2    2,0,0
  PRESET:
  
  #define    SEND_SETTINGS(N'brilliant'treble'medium'soft'C_D'A_B'pickA'pickB'ModA'ModB'RateA'RateB'EnvA'EnvB'TimeA'TimeB'plk'SusDamp'HPF'LPF'inharm'RelAmpl'RelTone'RelDTime'RelDamp'RelATime'brilliant'treble'medium'soft'fco1'fco2'fco3'fco4'gain1'gain2'gain3'gain4'Q1'Q2'Q3'Q4'level)#
  if i(kpreset)==$N then
   chnset    $brilliant    ,"brilliant"
   chnset    $treble        ,"treble"
   chnset    $medium        ,"medium"
   chnset    $soft        ,"soft"
   chnset    $C_D        ,"C_D"
   chnset    $A_B        ,"A_B"
   chnset    $pickA        ,"pickA"
   chnset    $pickB        ,"pickB"
   chnset    $ModA        ,"ModA"
   chnset    $ModB        ,"ModB"
   chnset    $RateA        ,"RateA"
   chnset    $RateB        ,"RateB"
   chnset    $EnvA        ,"EnvA"
   chnset    $EnvB        ,"EnvB"
   chnset    $TimeA        ,"TimeA"
   chnset    $TimeB        ,"TimeB"
   chnset    $plk        ,"plk"
   chnset    $SusDamp    ,"SusDamp"
   chnset    $HPF        ,"HPF"
   chnset    $LPF        ,"LPF"
   chnset    $inharm        ,"inharm"   
   chnset    $RelAmpl    ,"RelAmpl"
   chnset    $RelTone    ,"RelTone"
   chnset    $RelDTime    ,"RelDTime"
   chnset    $RelDamp    ,"RelDamp"
   chnset    $RelATime    ,"RelATime"
   chnset    $brilliant    ,"brilliant"
   chnset    $treble        ,"treble"
   chnset    $medium        ,"medium"
   chnset    $soft        ,"soft"
   chnset    $fco1        ,"fco1"
   chnset    $fco2        ,"fco2"
   chnset    $fco3        ,"fco3"
   chnset    $fco4        ,"fco4"
   chnset    $gain1        ,"gain1"
   chnset    $gain2        ,"gain2"
   chnset    $gain3        ,"gain3"
   chnset    $gain4        ,"gain4"
   chnset    $Q1        ,"Q1"
   chnset    $Q2        ,"Q2"
   chnset    $Q3        ,"Q3"
   chnset    $Q4        ,"Q4"
   chnset    $level        ,"level"
  endif#
;                          brilliant  treble  medium  soft  C_D   A_B  pickA          pickB          ModA           ModB            RateA        RateB         EnvA  EnvB  TimeA          TimeB          plk             SusDamp          HPF           LPF          inharm        RelAmpl        RelTone        RelDTime       RelDamp        RelATime       brilliant treble  medium  soft   fco1   fco2   fco3   fco4    gain1   gain2        gain3        gain4        Q1       Q2      Q3    Q4    level
  $SEND_SETTINGS(1         '0         '1      '0      '0    '1    '1   '0.25          '0.0700000003  '0.0300000012  '0.0300000012  '0.200000003  '0.150000006  '0    '0    '0.0500000007  '0.0500000007  '0.00499999989  '0               '0           '72           '0            '2             '1000          '0.0100000007  '0.949999988   '0.049999997  '0         '1      '0      '0     '2500  '1000  '500   '200    '3      '2.29999995  '1.29999995  '1.39999998  '1.25    '1      '1    '2    '0.5         )
  $SEND_SETTINGS(2         '0         '0      '1      '0    '1    '0   '0.938000023   '0.0500000007  '0.0300000012  '0.0300000012  '0.200000003  '0.150000006  '0    '0    '0.0500000007  '0.0500000007  '0.165399998    '0.50999999      '0           '39.6390991   '0            '0             '1000          '0.0100000007  '0.359999985   '4.01999998   '0         '0      '1      '0     '2500  '1000  '500   '200    '3      '2.29999995  '1.29999995  '1.39999998  '1.25    '1      '1    '2    '0.925000072 )
  $SEND_SETTINGS(3         '0         '0      '1      '0    '1    '0   '0.120000005   '0.252000004   '0.0300000012  '0.0300000012  '0.200000003  '0.150000006  '0    '0    '0.0500000007  '0.0500000007  '0.00079999998  '0.0799999759    '2.04999995  '8.57209969   '-0.50999999  '2.15999985    '2358.80005    '0.0650000051  '0.300000012   '8            '0         '0      '1      '0     '2500  '1000  '500   '200    '3      '2.29999995  '1.29999995  '1.39999998  '1.25    '1      '1    '2    '1.21700001  )  
  $SEND_SETTINGS(4         '0         '1      '0      '0    '1    '0   '0.530000031   '0.462000012   '0.0300000012  '0.0300000012  '0.200000003  '0.150000006  '0    '0    '0.0500000007  '0.0500000007  '0.269099981    '-1              '0           '25.2099991   '0            '2             '1000          '0.0100000007  '0.879999995   '0.049999997  '0         '1      '0      '0     '2500  '1000  '500   '200    '3      '2.29999995  '1.29999995  '1.39999998  '1.25    '1      '1    '2    '0.5         )
  $SEND_SETTINGS(5         '0         '1      '0      '1    '0    '1   '0.398000032   '0.522000015   '0.0300000012  '0.0300000012  '0.200000003  '0.150000006  '0    '0    '0.0500000007  '0.0500000007  '0.285899997    '0.0999999791    '0           '31.4514999   '0            '0             '1000          '0.0100000007  '0.579999983   '3.91999984   '0         '1      '0      '1     '2500  '1000  '500   '200    '3      '2.29999995  '1.29999995  '1.39999998  '1.25    '1      '1    '2    '0.461000025 )
  $SEND_SETTINGS(6         '0         '1      '0      '0    '1    '1   '0.0940000042  '0.398000032   '0.0300000012  '0.0300000012  '0.200000003  '0.150000006  '0    '0    '0.0500000007  '0.0500000007  '0.128700003    '0.299999982     '0           '72           '0            '0             '1000          '0.0100000007  '0.280000001   '1.39999998   '0         '1      '0      '0     '2500  '1000  '500   '200    '3      '2.29999995  '1.29999995  '1.39999998  '1.25    '1      '1    '2    '0.5         )
  $SEND_SETTINGS(7         '1         '1      '0      '0    '1    '0   '0.00200000009 '0.950000048   '0.0300000012  '0.0300000012  '0.200000003  '0.150000006  '0    '0    '0.0500000007  '0.0500000007  '0.128700003    '0.299999982     '3.46000004  '24.0676003   '0            '0             '1000          '0.0100000007  '0.280000001   '3.32999992   '1         '1      '0      '0     '2500  '1000  '500   '200    '3      '2.29999995  '1.29999995  '1.39999998  '1.25    '1      '1    '2    '4.26300001  )
  $SEND_SETTINGS(8         '1         '1      '0      '0    '1    '0   '0.584000051   '0.450000018   '0.0300000012  '0.0300000012  '0.200000003  '0.150000006  '0    '0    '0.0500000007  '0.0500000007  '0.23709999     '0.459999979     '1.46999991  '16.7103996   '0            '0             '1000          '0.0100000007  '0.140000001   '3.58999991   '1         '1      '0      '0     '2500  '1000  '500   '200    '3      '2.29999995  '1.29999995  '1.39999998  '1.25    '1      '1    '2    '0.720000029 )
  $SEND_SETTINGS(9         '1         '0      '0      '0    '0    '0   '0.25          '0.0700000003  '0.0300000012  '0.0300000012  '0.200000003  '0.150000006  '0    '0    '0.0500000007  '0.0500000007  '0.00499999989  '0               '0           '25.5985985   '0            '2             '1000          '0.0100000007  '0.949999988   '0.049999997  '1         '0      '0      '0     '2500  '1000  '500   '200    '3      '2.29999995  '1.29999995  '1.39999998  '1.25    '1      '1    '2    '1.92100012  )
  $SEND_SETTINGS(10        '1         '0      '0      '0    '1    '0   '0.25          '0.0700000003  '0.0300000012  '0.0300000012  '0.200000003  '0.150000006  '0    '0    '0.0500000007  '0.0500000007  '0.00499999989  '0               '0           '25.5985985   '0            '2             '1000          '0.0100000007  '0.949999988   '0.049999997  '1         '0      '0      '0     '2500  '1000  '500   '200    '3      '2.29999995  '1.29999995  '1.39999998  '1.25    '1      '1    '2    '1.92100012  )  
  rireturn
 endif
endin

giActiveNotes    ftgen    0,0,128,-2,0

instr    2    ; RESPOND TO MIDI NOTES
 inum        notnum
         event_i    "i",3+(inum*0.001),0,-1,cpsmidi(),inum,veloc(0,1)
         tablew    1-release(),inum,giActiveNotes
endin

instr    3
 ibrilliant    chnget    "brilliant"    ; filter switch
 itreble    chnget    "treble"    ; filter switch
 imedium    chnget    "medium"    ; filter switch
 isoft        chnget    "soft"        ; filter switch
 iC_D        chnget    "C_D"        ; single/dual pickup selector
 iA_B        chnget    "A_B"        ; neck/bridge pickup selector
 gklevel    chnget    "level"        ; output volume control

 icps        =    p4        ; cps read from midi
 inum        =    p5        ; note number read from midi
 ivel        =    p6        ; velocity read from midi

 kActive    table    inum,giActiveNotes    ; activity of the key that triggered this note
 if kActive==0 then                ; if the key has been released...
  turnoff                    ; turnoff this instrument immediately
 endif

 kactive    active    p1,0,1
 kIntrpt    init    0
 if kactive>4 then
  kIntrpt    =    1
  turnoff
 endif
 kIntrptTrig    trigger    kIntrpt,0.5,0

 ; pluck position is an i-rate variable so a mechanism is created to re-initialise the instrument if it is changed in realtime
 kplk    chnget    "plk"            ; pluck position
 ktrig    changed    kplk            ; if GUI knob changes, generate a trigger
 if ktrig==1 then            ; if triggered...
  reinit    UPDATE            ; reinitialise...
 endif                
 UPDATE:                ; ...from here (until the end of the instrument)
 iplk    =    i(kplk)            ; cast k-rate pluck position to i-rate variable
 
 ; pickup positions
 kpickA    chnget    "pickA"            ; neck pickup position
 kpickB    chnget    "pickB"            ; bridge pickup position
 
 ; string vibration damping / reflectivity
 irefl        table        inum,girefl            ; read reflectivity for this note from function table (lower notes will experience more damping)
 iRelDTime    chnget        "RelDTime"            ; release damping time: time for damping state to be established
 iRelDamp    chnget        "RelDamp"            ; amount of damping upon release
 krefl        linsegr        irefl,iRelDTime,iRelDamp    ; upon note release vibrations are damped quickly and dramatically
 kSusDamp    chnget        "SusDamp"            ; user controlled modulation of string vibration damping
 krefl        limit        krefl+kSusDamp,0.001,0.999    ; consolidate reflection/damping value and limit possible values to prevent out of range values
 iamp        =        ivel*0.3
 kenv        expseg        1, 0.01,0.001,1,0.001        ; create an amplitude envelope for the noise burst
 axcite        pinkish        kenv-0.001            ; create a pink noise signal
 axcite        tone        axcite,icps*2
 krelease    release                        ; sense release of note
 iRelAmpl    chnget        "RelAmpl"            ; release pluck amplitude
 iRelTone    chnget        "RelTone"            ; tone of the release pluck
 if krelease==1 then                        ; if key is released...
  axcite    mpulse        iRelAmpl,0            ; audio impulse is created as an implementation of the release pluck impulse 
  axcite    butlp        axcite,iRelTone            ; lowpass filter the impulse
 endif
 
 kModA        chnget        "ModA"                ; pickup position modulation depth
 kModB        chnget        "ModB"
 kRateA        chnget        "RateA"                ; pickup position modulation rate
 kRateB        chnget        "RateB"
 
 iEnvA        chnget        "EnvA"                ; pickup position envelope modulation depth
 iEnvB        chnget        "EnvB"
 iTimeA        chnget        "TimeA"                ; pickup position envelope modulation time
 iTimeB        chnget        "TimeB"
 
 kenv1        transeg        iEnvA,iTimeA,-4,0        ; envelope applied to the pickup position (pickup A)
 kenv2        transeg        iEnvB,iTimeB,-4,0        ; envelope applied to the pickup position (pickup B)
 kmod1        oscili        kModA,kRateA,gitri        ; lfo used to modulate pickup position (pickup A)
 kmod2        oscili        kModB,kRateB,gitri        ; lfo used to modulate pickup position (pickup B)
 kpickA        limit        kpickA + kmod1 + kenv1, 0, 1    ; consolidate pickup position between fixed value, lfo and envelope. Also protect against out of range values. (pickup A)
 kpickB        limit        kpickB + kmod2 + kenv2, 0, 1    ; consolidate pickup position between fixed value, lfo and envelope. Also protect against out of range values. (pickup B)
 
 iinharm        chnget        "inharm"                ; inharmonicity of the tone. negative values compress the spectrum, positive values expand it. Compensation will be applied to the fundemental so that it will remain consistent.
 iFShift        =            icps*iinharm            ; amount of frequency shift (in CPS)
 iFSMix         chnget  "FSMix"
 iFSKeyFollow   chnget  "FSKeyFollow"
 iFSEnvTime     chnget  "FSEnvTime"
 iFS_FB_Amt     chnget  "FS_FB_Amt"
 
 ;a1         repluck     iplk, iamp, icps-iFShift, kpickA, krefl, axcite    ; employ plucked string model (pickup A)
 ;a2         repluck     iplk, iamp, icps-iFShift, kpickB, krefl, axcite    ; employ plucked string model (pickup B)
 a1         repluck     iplk, iamp, icps, kpickA, krefl, axcite    ; employ plucked string model (pickup A)
 a2         repluck     iplk, iamp, icps, kpickB, krefl, axcite    ; employ plucked string model (pickup B)
 
 if(iinharm!=0) then                        ; only apply frequency shifting if inharmonicity is anything other than zero
  aFS1,aFS2    init    0
  iFShift   =   iFSKeyFollow == 1 ? iFShift*(cpsoct(4)/icps) : iFShift
  if iFSEnvTime>0 then
   kFSEnv       transeg 1, (1-iFSEnvTime)*3, -2, 0
   aFS1        FreqShifter    a1+aFS1*iFS_FB_Amt,iFShift*kFSEnv,gisine        ; call frequency shifting UDO
   aFS2        FreqShifter    a2+aFS2*iFS_FB_Amt,iFShift*kFSEnv,gisine
  else
   aFS1        FreqShifter    a1+aFS1*iFS_FB_Amt,iFShift,gisine        ; call frequency shifting UDO
   aFS2        FreqShifter    a2+aFS2*iFS_FB_Amt,iFShift,gisine
  endif
  
  a1        ntrpol          a1, aFS1, iFSMix
  a2        ntrpol          a2, aFS2, iFSMix
 endif
 
 if(iC_D==0&&iA_B==0) then    ;CA - neck pickup only
  asig    =    a1
 elseif(iC_D==0&&iA_B==1) then    ;CB - bridge pickup only
  asig    =    a2
 elseif(iC_D==1&&iA_B==0) then    ;DA - both pickups in phase
  asig    =    a1 + a2
 elseif(iC_D==1&&iA_B==1) then    ;DB - both pickups. Pickup B 180 degrees out of phase
  asig    =    a1 - a2
 endif
 
 ; velocity to brightness
 icf        =        (ivel*6)+8            ; cutoff frequency in 'oct' format
 asig        butlp        asig,cpsoct(icf)        ; lowpass filter audio signal
 
 ; filter switches
 kfco1        chnget        "fco1"    ; filter cutoffs
 kfco2        chnget        "fco2"
 kfco3        chnget        "fco3"
 kfco4        chnget        "fco4"
 kQ1        chnget        "Q1"    ; filter q's
 kQ2        chnget        "Q2"
 kQ3        chnget        "Q3"
 kQ4        chnget        "Q4"
 kgain1        chnget        "gain1"    ; filter gains
 kgain2        chnget        "gain2"
 kgain3        chnget        "gain3"
 kgain4        chnget        "gain4"
  
 ; filters
 amix    =    0    ; audio mix of filter outputs (reset to silence upon each iteration)
 if(ibrilliant==1) then ; if 'brilliant' selected...
  afilt        rbjeq         asig, kfco1, 1, kQ1, 1, 4        ; bandpass filter sound
  amix        =        amix + (afilt * kgain1)            ; add a scaled amount of filtered signal to filter mix signal
 endif
 if(itreble==1) then ; if 'treble' selected...
  afilt        rbjeq         asig, kfco2, 1, kQ2, 1, 4        ; bandpass filter sound                                      
  amix        =        amix + (afilt * kgain2)                 ; add a scaled amount of filtered signal to filter mix signal
 endif    
 if(imedium==1) then ; if 'medium' selected...
  afilt        rbjeq         asig, kfco3, 1, kQ3, 1, 4        ; bandpass filter sound                                      
  amix        =        amix + (afilt * kgain3)                 ; add a scaled amount of filtered signal to filter mix signal
 endif
 if(isoft==1) then ; if 'soft' selected...
  afilt        rbjeq         asig, kfco4, 1, kQ4, 1, 4        ; bandpass filter sound                                      
  amix        =        amix + (afilt * kgain4)                 ; add a scaled amount of filtered signal to filter mix signal
 endif
 if((ibrilliant + itreble + imedium + isoft)==0) then    ; if no filters selected...
  amix    =    asig                    ; ...just send unfiltered audio to filter section output
 endif
 
 ; key following highpass filter
 kHPF        chnget        "HPF"                    ; highpass filter multiple
 if kHPF>0 then                                ; if HPF control is greater than 1...
  kcf        limit        icps*kHPF,20,sr/2            ; prevent out of range values which would cause the filter to 'blow up'
  amix        buthp        amix,kcf                ; filter tone relative to note fundemental
 endif
 
 ; key following lowpass filter
 kLPF        chnget        "LPF"                    ; lowpass filter multiple
 if kLPF<72 then                            ; if LPF control is greater than 1...
  kcf        limit        icps*kLPF,20,sr/2            ; prevent out of range values which would cause the filter to 'blow up'
  amix        butlp        amix,kcf                ; filter tone relative to note fundemental
 endif
 
 ; release envelope
 iRelATime    chnget        "RelATime"                ; amplitude release time
 aenv        transegr    1, iRelATime, -4, 0        ; amplitude release envelope (senses key release before entering final/only segment)
 
 ; output signal
 aout        =        amix * aenv * gklevel            ; apply release envelope and level control
         outs        aout, aout                ; send audio to outputs
endin



instr 1000                ; launches plant popups
 klaunch    init        0
 #define LAUNCH_PLANT(name)
 #
 kpressed    chnget         "$name._Button"
 if changed(kpressed)==1 then
   Smsg     sprintfk     "visible(%d), pos(1, 19)", klaunch
           chnset         Smsg, "$name.Plant"
 endif
 #
 $LAUNCH_PLANT(setup)
 $LAUNCH_PLANT(release)
 $LAUNCH_PLANT(filters)
 klaunch    =        1
endin

</CsInstruments>

<CsScore>
i 1    0 [60*60*24*7]    ; presets
i 1000 0 [60*60*24*7]    ; plant pop-ups
</CsScore>

</CsoundSynthesizer>