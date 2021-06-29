
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; BarModel.csd
; Original physical model code by Stefan Bilbao, ported to Csound by John ffitch
; Example written by Iain McCurdy, 2015
; 
; Uses barmodel physical model of a struck bar
; Note that some tuning anomalies may exist. These are due to rounding errors in the algorithm and are largely unresolvable.
; On account of this instrument being based upon a fairly complex physical modelling algorithm, it will be unreasonable 
; to expect massive polyphony in realtime. For this reason a polyphony control mechanism is implemented to provide a means 
; to prevent underruns in realtime performance.

; CONTROLS:
; ---------
; Boundary Conditions    -    this combobox selection determines the nature of how the bar is fixed at either end 
;                 (clamped/pivoting/free)
;                 For simplification and safety, only the valid options are offered.
;                 Nonetheless care must be taken is if 'Clamped-Free' is chosen: if 'Decay' and 'Damping' 
;                 are high in combination with this choice this can lead to the fundemental building in 
;                 amplitude dangerously.
;                               The boundary conditions in general affect how stretched out the partials become, the partials
;                               are most compressed with clamped-free, next is clamped-pivoted, next clamped-clamped and finally
;                               the partials are most stretched out if pivoted-pivoted is chosen. It is also interesting to
;                               note that with a clamped-free bar model, an additional partial beneath the fundemental
;                               emerges somewhere around E3 and above. This sub-partial is roughly 2 octaves + a minor 6th
;                               below the fundemental.
;                When 'Clamped-Free' is selected the fundemental partial is unaffected by 'Damping'. 
; Keyboard Scaling  -    when active, damping, attack time, decay time and release times are influenced by the note played 
;                           with higher notes attacking and decaying quicker, and therefore reflecting real-world physics.
; Decay            -    This is the time it will take for the amplitude to decay by 30dB from its initial level.
; HOLD (button)        -    If this is active, the value for decay will be a large number and the value of the slider will be ignored. 
;                This feature can be used to create 'sustained' notes but bear in mind that a note's ability
;                to sustain will also be dependent upon the value of 'Damping'. For a sustaining timbre, 
;                'Damping' should be zero.
; Release        -    Amplitude release time once a note is released. (This can be over-ridden if polyphony limit is exceeded.
; Damping        -    High frequency damping. Larger value imply increased damping. A value of zero implies no damping.
; Polyphony        -    Polyphony limit. If this is exceeded (including notes in their release stage) 
;                 then the oldest note will be removed quickly. Amplitudes will be ramped to prevent clicks
;                 upon note removal. If zero, this mechanism is bypassed and polyphony is not limited.
; Width            -    Width of strike. Higher notes may demand a higher value for 'Width'. 
;                Higher values for Width will also result in a 'darker' tone being produced.
;                On account of the amplitude level changes that will result from changing this parameter, it should be used in conjunction with the 'Level' control.
; Attack:Time        -    Duration of the attack envelope. If zero, this envelope is bypassed.
; Attack:Amount        -    Amount of attack envelope to be applied. Basically the initial value of the attack envelope before ramping up to '1'.
;                 This envelope affects both amplitude and tone (LPF) 
; Presets        -    Some hard-wired presets to help the user get orientated. 
; Reverb:Mix        -    Dry/Wet reverb mix (100% dry - 100% wet). If mix is zero the reverb is bypassed to improve efficiency and to remove the need for an on/off switch. 
; Reverb:Size        -    Reverb size
; Level            -    Output amplitude level
; 
; Velocity Mappings: how note velocity maps to various parameters. 
; Velocity is on the x-axis, low velocitys (left hand side of the graph) being on the left side of the graph.
; Click and drag on the graphs. These should normally be single straight line graphs.
; ----------------------------------------------------------------------------------------------------------------------------------------------------
; Strike Position    -    how velocity maps to strike position across the length of the bar
;                 Note that for some Boundary Condition setting it may be more appropriate to limit the range of this graph 
;                 from zero to 0.5 (or from 0.5 to 1).
;                 For a fixed strike position, design this graph as horizontal line.
; Tone            -    velocity to tone (LPF)
; Amplitude        -    velocity to amplitude
; 
; Modulation: modulation of the scanning position of the bar. This produces spectral variation.
; ---------------------------------------------------------------------------------------------
; Mix    -    mix between an unmodulated and a modulated barmodel.
;         Note that if this value is anything other than zero or '1' then each note played will activate 2 instances 
;         of the barmodel opcode, effectively doubling the CPU draw.
; Rate    -    Rate of modulation. This is a fixed value (in hertz) but the rate of modulation can also be influenced 
;         by an envelope and splinic random variation.
; x 100 (button)    -    If this button is active the value of the 'Rate' slider will be multiplied by 100. 
;                Note that very fast scanning position modulations will result in quantisation artefacts being produced.
; Envelope    -    An envelope which influences modulation rate. It will be multiplied to the value of 'Rate'.
; Duration    -    The time it will take for the above mentioned envelope to complete.
; Random:Amount    -    Amount of random splinic modulation. This function will be added to 'Rate' and 'Envelope'.
; Random:Rate    -    Rate of random splinic modulation.
; Panning    -    Width of panning modulation. The shape of this modulation will follow that of the scanning modulation.
;            To remove all modulation from the sound produced bring this slider to to zero as well as the 'Mix' slider.


<Cabbage>
form caption("Bar Model"), size(955,340), pluginId("BaMo"), colour(0,0,0), guiRefresh(128)
label     bounds( 10, 15,150, 15), text("Boundary Conditions")
combobox  bounds( 10, 32,150, 20), channel("bc"), value(2), text("Clamped-Clamped","Clamped-Pivoted","Pivoted-Pivoted","Clamped-Free")
checkbox  bounds( 10, 70,150, 15), channel("KybdScal"), text("Keyboard Scaling") value(0)
rslider   bounds(160,  5, 80, 80), text("Decay"), channel("T30"), valueTextBox(1), textBox(1), range(0.01,30,5.5,0.5,0.01), trackerColour("yellow")
button    bounds(175, 90, 50, 15), text("HOLD","HOLD"), channel("T30_Hold"), fontColour:0(30,30,30), fontColour:1(255,255,100)
rslider   bounds(220,  5, 80, 80), text("Release"), channel("rel"), valueTextBox(1), textBox(1), range(0.01,30,5.5,0.5,0.01), trackerColour("yellow")
rslider   bounds(280,  5, 80, 80), text("Damping"), channel("b"), valueTextBox(1), textBox(1), range(0, 9.999,0.4,0.5,0.001), trackerColour("yellow")
rslider   bounds(350,  5, 80, 80), text("Polyphony"), channel("PolyLimit"), valueTextBox(1), textBox(1), range(0, 20,5,1,1), trackerColour("yellow")
;rslider   bounds(420,  5, 80, 80), text(""), channel(""), valueTextBox(1), range(0, 1,0.5,1,0.001), trackerColour("yellow")
rslider   bounds(420,  5, 80, 80), text("Width"), channel("wid"), valueTextBox(1), textBox(1), range(0.01, 1,0.5,1,0.01), trackerColour("yellow")
;line      bounds(505, 15, 90,  1)
image     bounds(505, 15, 90,  1), colour(30,30,30), identChannel("AttackLine")
label     bounds(525,  9, 50, 12), text("Attack"), colour(0,0,0), fontColour(30,30,30), identChannel("AttackLabel")
rslider   bounds(490, 25, 61, 61), text("Time"), channel("AttTim"), range(0,3,0,0.5,0.001), trackerColour("yellow")
rslider   bounds(550, 25, 61, 61), text("Amount"), channel("AttAmt"), range(0, 1, 0,1,0.001), trackerColour("yellow")
label     bounds(615, 15,150, 15), text("Presets")
combobox  bounds(615, 32,140, 20), channel("preset"), value(1), text("Xylobar","Scan Envelope","Soft Sustain","Rhodes","Good Vibes","Bad Vibes","Hard Beater","Stacking Plates","Bowed Plank","Interference","Ride Cymbal","Tubular Bell", "Discord Bar")
;line      bounds(775, 15, 90,  1)
image     bounds(775, 15, 90,  1), colour(30,30,30), identChannel("ReverbLine")
label     bounds(795,  9, 50, 12), text("Reverb"), colour(0,0,0), fontColour(30,30,30), identChannel("ReverbLabel")
rslider   bounds(760, 25, 61, 61), text("Mix"), channel("RvbMix"), range(0, 1, 0,1,0.01), trackerColour("yellow")
rslider   bounds(820, 25, 61, 61), text("Size"), channel("RvbSize"), range(0.5,0.99,0.8,1,0.01), trackerColour("yellow")
rslider   bounds(870,  5, 80, 80), text("Level"), channel("OutAmp"), valueTextBox(1), textBox(1), range(0,9.999,0.8,0.5,0.001), trackerColour("yellow")

groupbox  bounds(  5,115,400,140), text("Velocity Mappings"), colour(0,0,0,60), shape("sharp"), plant("VelocityMappings"){
label     bounds(  5, 25,120, 10), text("Strike Position")
gentable  bounds(  5, 37,120, 90), tableNumber(2), identChannel(table2), ampRange(0,1,2), zoom(-1), tableColour(turquoise), active(1), fill(0)
label     bounds(135, 25,120, 10), text("Tone")
gentable  bounds(135, 37,120, 90), tableNumber(3), identChannel(table3), ampRange(0,1,3), zoom(-1), tableColour(turquoise), active(1), fill(0)
label     bounds(265, 25,120, 10), text("Amplitude")
gentable  bounds(265, 37,120, 90), tableNumber(4), identChannel(table4), ampRange(0.01,1,4), zoom(-1), tableColour(turquoise), active(1), fill(0)
}

; MODULATION
groupbox  bounds(410,115,540,140), text("Modulation"), colour(0,0,0,60), shape("sharp"), plant("modulation"){
rslider   bounds(  5, 25, 80, 80), text("Mix"), channel("ScanMix"), valueTextBox(1), textBox(1), range(0, 1, 0,1,0.001), trackerColour("yellow")
rslider   bounds( 65, 25, 80, 80), text("Rate"), channel("scan"), valueTextBox(1), textBox(1), range(0, 1, 0.1,0.5,0.001), trackerColour("yellow")
button    bounds( 80,110, 50, 15), text("x 100","x 100"), channel("scan_x100"), fontColour:0(30,30,30), fontColour:1(255,255,100)
label     bounds(150, 25,120, 10), text("Envelope")
gentable  bounds(150, 37,120, 90), tableNumber(5), identChannel(table5), ampRange(0.01,2,-1), zoom(-1), tableColour(yellow), active(1), fill(0)
rslider   bounds(280, 25, 80, 80), text("Duration"), channel("ModEnvDur"), valueTextBox(1), textBox(1), range(0.1, 10, 1,0.5,0.01), trackerColour("yellow")
line      bounds(365, 35, 90,  1)
label     bounds(385, 29, 50, 12), text("Random"), colour(0,0,0)
rslider   bounds(350, 45, 61, 61), text("Amount"), channel("RndModAmt"), range(0, 1, 0,1,0.001), trackerColour("yellow")
rslider   bounds(410, 45, 61, 61), text("Rate"), channel("RndModRte"), range(0.01,10,0.1,0.5,0.001), trackerColour("yellow")
rslider   bounds(460, 25, 80, 80), text("Panning"), channel("PanDepth"), valueTextBox(1), textBox(1), range(0, 0.5, 0,1,0.001), trackerColour("yellow")
label bounds(440, 120,98,10),text("Iain McCurdy 2015")
}

keyboard bounds(4, 254, 945, 80) mouseoeverkeycolour(255, 255, 0, 128) 

;;TUNING
;image     bounds(  0,350,955,170), plant("tuning"), colour(0,0,0,0) {
;nslider bounds(  0, 15,160, 70), caption("Note Number"), channel("note"), range(0,128,0,1,1)
;nslider bounds(260, 15,160, 70), caption("Output"), channel("output"), range(0,8000,0,1,0.001)
;button    bounds(160,  0,100, 15), text("ZERO","ZERO"), channel("ZeroOffset"), fontColour:0(30,30,30), fontColour:1(255,255,100), latched(0)
;nslider bounds(160, 15,100, 70), caption("Offset"), channel("offset"), range(-100,100,0,1,0.01)
;checkbox  bounds(425, 15,100, 20), text("Test Tone"), channel("TestTone"), value(0)
;hslider   bounds(425, 40,120, 15), channel("TestVol"), range(0, 0.2, 0.05, 0.5,0.001), trackerColour("yellow")
;csoundoutput bounds(545,0,400,170)
;}
</Cabbage>
                    
<CsoundSynthesizer>

<CsOptions>   
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

;sr is set by the host
ksmps         =     4
nchnls         =     2
0dbfs        =    1

massign    0,2

; MAPPING FUNCTIONS: NOTE->STIFFNESS (C="CLAMPED" P="PIVOTING" F="FREE")
giCC    ftgen    0, 0, 128, -17, 0,5.887,            17,5.887,    18,6.236,    19,6.593,    20,7.01,    21,7.421,    22,7.8,        23,8.223,    24,8.773,    25,9.282,    26,9.736,    27,10.4013,    28,10.993,    29,11.52,    30,12.298,    31,12.795,    32,13.77,    33,14.57,    34,15.27,    35,16.297,    36,17.142,    37,18.21,    38,19.2,    39,20.41,    40,21.53,    41,22.710,    42,24.11,    43,25.382,    44,26.938,    45,28.385,    46,30.04,    47,31.937,    48,33.182,    49,35.13,    50,37.574,    51,39.418,    52,42.06,    53,43.962,    54,46.598,    55,49.301,    56,52.274,    57,55.16,    58,58.325,    59,61.089,    60,64.91,    61,68.63,    62,72.491,    63,76.2975,    64,80.792,    65,84.786,    66,90.089,    67,95.632,    68,99.023,    69,104.895,    70,110.928,    71,116.307,    72,122.731,    73,130.473,    74,137.982,    75,145.78,    76,153.124,    77,161.277,    78,171.217,    79,180.8015,    80,186.41,    81,198.914,    82,210.724,    83,220.499,    84,230.52,    85,244.06,    86,258.55,    87,271.361,    88,280.53,    89,297.311,    90,318.36,    91,333.471,    92,344.54,    93,358.38,    94,379.586,    95,401.92,    96,425.373,    97,449.656,    98,450,        99,473.94,    100,505.217,    101,534.021,    102,565.833,    103,599.109,    104,612.474,    105,620.586,    106,655,    107,692.93,    108,732.94,    109,775.11,    110,819.52,    111,866.25,    112,881.99,    113,882.901,    114,921.2,    115,972.4,    116,1025.96,    117,1081.92,    118,1140.28,    119,1201,    120,1264.03
giCP    ftgen    0, 0, 128, -17, 0,8.198,    16,8.198,    17,8.738,    18,9.26,    19,9.6,        20,10.37,    21,10.8889,    22,11.606,    23,12.18,    24,13.006,    25,13.681,    26,14.44,    27,15.27,    28,16.08,    29,17.026,    30,18.111,    31,19.103,    32,20.264,    33,21.4893,    34,22.68,    35,24.08,    36,25.43,    37,27.02,    38,28.5,    39,30.19,    40,32.08,    41,34.19,    42,35.98,    43,38.18,    44,40.3,    45,42.73,    46,45.32,    47,47.82,    48,50.6,    49,53.56,    50,56.6,    51,59.97,    52,63.4,    53,67.16,    54,71.11,    55,75.34,    56,79.550,    57,84.11,    58,88.870,    59,94.176,    60,99.42,    61,104.96,    62,111.51,    63,117.550,    64,124.382,    65,131.000,    66,138.704,    67,147.070,    68,154.617,    69,163.690,    70,173.640,    71,182.280,    72,193.050,    73,204.560,    74,216.790,    75,227.133,    76,240.660,    77,254.910,    78,270.080,    79,282.358,    80,299.040,    81,316.880,    82,335.660,    83,349.750,    84,370.420,    85,392.452,    86,415.640,    87,440.386,    88,456.274,    89,483.374,    90,511.980,    91,542.431,    92,574.379,    93,608.550,    94,626.050,    95,662.920,    96,702.160,    97,743.397,    98,787.520,    99,838.16,    100,881.300,    101,899.350,    102,958.290,    103,1010.900,    104,1067.740,    105,1126.700,    106,1198.630,    107,1267.480,    108,1340.940,    109,1378.15,    110,1440,    111,1521.86,    112,1608.16,    113,1608.16,    114,1794,    115,1893.7,    116,1998,    117,2107,    118,2220.7,    119,2338.9
giPP    ftgen    0, 0, 128, -17, 0,21.3,                                                                23,21.3,    24,22.15,    25,23.1,    26,24.1259,    27,25.2,    28,26.35,    29,27.85,    30,29.5,    31,31.5,    32,33.4,    33,35.00,    34,37.5,    35,38.86,    36,41.87,    37,44.46,    38,47.15,    39,49.89,    40,52.550,    41,55.690,    42,58.950,    43,62.580,    44,66.310,    45,70.350,    46,74.280,    47,78.560,    48,83.590,    49,88.57,    50,93.83,    51,99.45,    52,105.37,    53,111.63,    54,118.35,    55,125.4,    56,132.95,    57,140.86,    58,149.24,    59,158.3,    60,167.7,    61,177.7,    62,188.5,    63,199.7,    64,211.6,    65,224.6,    66,237.94,    67,252.1,    68,267.06,    69,283.7,    70,300.56,    71,318.5,    72,337.35,    73,358.9,    74,380.14,    75,402.73,    76,426.7,    77,454.8,    78,481.8,    79,510.4,    80,540.72,    81,572.9,    82,606.9,    83,649.4,    84,687.94,    85,728.77,    86,772.01,    87,817.8,    88,866.3,    89,934.92,    90,990.31,    91,1049,    92,1111.05,    93,1176.78,    94,1246.35,    95,1319.99,    96,1455.78,    97,1541.65,    98,1632.488,    99,1728.6,    100,1830.17,    101,1937.6,    102,2051.15,    103,2171.14,    104,2297.9,    105,2735.71,    106,2894.65,    107,3062.34,    108,3239.16,    109,3425.51,    110,3621.76,    111,3828.29,    112,4045.44,    113,4273.54,    114,4512.87,    115,4763.67,    116,5026.14,    117,5300.32
giCF    ftgen    0, 0, 128, -17, 0,7.37,                                        20,7.37,    21,7.7,        22,8.2,        23,8.67,    24,9.2,        25,9.83,    26,10.3,    27,11,        28,11.5,    29,12.19,    30,12.92,    31,13.703,    32,14.547,    33,15.392,    34,16.368,    35,17.28,    36,18.211,    37,19.6,    38,20.8,    39,21.88,    40,23.24,    41,24.67,    42,26.111,    43,27.41,    44,29.6,    45,31.34,    46,33.2,    47,35.181,    48,37.27,    49,39.484,    50,41.828,    51,44.315,    52,46.945,    53,49.737,    54,52.69,    55,55.815,    56,59.133,    57,62.64,    58,66.365,    59,70.30,    60,74.48,    61,78.89,    62,83.58,    63,88.528,    64,93.792,    65,99.34,    66,105.245,    67,111.501,    68,118.087,    69,125.108,    70,132.486,    71,140.362,    72,148.704,    73,157.46,    74,166.81,    75,176.727,    76,187.098,    77,198.22,    78,209.991,    79,222.27,    80,235.47,    81,249.454,    82,264.266,    83,279.634,    84,296.24,    85,313.812,    86,332.43,    87,351.63,    88,372.48,    89,394.56,    90,417.93,    91,442.69,    92,468.04,    93,495.72,    94,525.03,    95,556.05,    96,588.88,    97,622.53,    98,659.21,    99,698.01,    100,739.04,    101,782.414,    102,828.27,    103,876.72,    104,928.98,    105,983.1,    106,1040.21,    107,1100.47,    108,1164.01,    109,1230.98,    110,1308.040,    111,1380.110,    112,1467.360,    113,1562.151,    114,1661.6,    115,1753.94,    116,1850.58,    117,1951.524,    118,2056.79,    119,2166.31,    120,2280,    121,2397.64
; NOTE RANGES FOR THE FOUR BOUNDARY CONDITIONS OPTIONS
giNoteRanges[]    fillarray     17,120, 16,119, 23,117, 20,121

; POLYPHONY CONTROL
gkActiveNotes    init    0        ; active notes (notes in release stage are disregarded)
giNoteTable    ftgen    0,0,128,-2,0    ; note table indicating on/off status. Note number denoted by table index.
giPolyLimit    init    5

giVelFunc    ftgen    0,0,128,-16,.1,128,-1,1            ; map note number to barmodel strike-velocity parameter
giWidFunc    ftgen    0,0,128,-16,0.7,128,8,8            ; map note number to barmodel strike-width parameter to compensate for loss of power in higher notes
giWidVelScal    ftgen    0,0,128,-16,30000,128,-4,1000        ; scaling of strike velocity according to strike width value. Strike velocity needs to be boosted if strike width is low.
; GUI GEN TABLES
giVelToPos    ftgen    2,0,128,-7,0.5,128,0            ; map MIDI velocity to barmodel strike position parameter
giVelToTone    ftgen    3,0,128,-7,0.5,128,1            ; map MIDI velocity to tone (LPF)
giVelToAmp    ftgen    4,0,128,-5,0.01,128,1            ; map MIDI velocity to amplitude
giModEnv    ftgen    5,0,512,-5,1,512,1            ; modulation rate envelope

giInterruptTime    init    0.2                    ; time it will take for amplitude to ramp down if a note is stopped on account of a polyphony limit violation

gisine        ftgen    0,0,4096,10,10

gasendL,gasendR    init    0

gkNoteTable[]    init    128

; UDO for an i-rate version of the 'scale' opcode
opcode    scale_i,i,iii
 ival,imax,imin    xin
 ival    =    (ival * (imax-imin)) + imin
    xout    ival
endop

; barmodel demands a high kr for high quality results whenever kscan>0, therefore it is embedded in a UDO which can dictate its own kr
opcode barmodel_ksmps,a,iiiikiiii
 ibcL, ibcR, iK, ib, kscan, iT30, ipos, iStrk, iwid    xin
 setksmps    1        ; experiment with this value. Powers of 2 closer to 1 will result in higher quality results at the expense of CPU
 asig     barmodel     ibcL, ibcR, iK, ib,     kscan, iT30, ipos, iStrk, iwid 
     xout        asig
endop

opcode pan2_ksmps,aa,aa
 asig,apan    xin
 setksmps    1        ; experiment with this value. Powers of 2 closer to 1 will result in higher quality results at the expense of CPU
 kpan    downsamp    apan
 aL,aR    pan2    asig,kpan
    xout    aL,aR
endop


instr    1    ; always on instrument. Read in widgets that will be used as global variables.
 kporttime    linseg    0,0.001,0.05
 gkscan        chnget    "scan"        
 kscan_x100    chnget    "scan_x100"
 if kscan_x100==1 then
  gkscan    *=    100
 endif
 gkScanMix    chnget    "ScanMix" 
 gkOutAmp    chnget    "OutAmp"
 gkOutAmp    portk    gkOutAmp,kporttime
 gkRndModAmt    chnget    "RndModAmt"
 gkRndModRte    chnget    "RndModRte"
 gkPanDepth    chnget    "PanDepth"
 gkRvbMix    chnget    "RvbMix"
 gkRvbSize    chnget    "RvbSize"
 gkDry        limit    (1-gkRvbMix)*2,0,1
 gkWet        limit    gkRvbMix*2,0,1
 gkKybdScal chnget  "KybdScal"
 
; ; TUNING
; ; TEST TONE
; gknote    init    60                        ; (used for diagnostics and tuning)
; gktrans    chnget    "trans"                        ; (used for diagnostics and tuning)
; kTestTone    chnget    "TestTone"                    ; (used for diagnostics and tuning)
; if kTestTone==1 then                            ; (used for diagnostics and tuning)
;  kTestVol    chnget    "TestVol"                    ; (used for diagnostics and tuning)
;  atest    poscil    kTestVol,cpsmidinn(gknote)*octave(gktrans),-1    ; (used for diagnostics and tuning)
;      outs    atest,atest                        ; (used for diagnostics and tuning)
; endif                                    ; (used for diagnostics and tuning)
; 
; ; MIDI CONTROL OF OFFSET
; kup    ctrl7    1,1,0,10                        ; (used for diagnostics and tuning)
; kdn    ctrl7    1,2,0,10                        ; (used for diagnostics and tuning)
; kincr    =    0.0005                             ; (used for diagnostics and tuning)
; gkoffset    chnget    "offset"                    ; (used for diagnostics and tuning)
; if kup>0 then                                ; (used for diagnostics and tuning)
;  chnset    gkoffset+(kincr*kup),"offset"                ; (used for diagnostics and tuning)
; elseif kdn>0 then                            ; (used for diagnostics and tuning)
;  chnset    gkoffset-(kincr*kdn),"offset"                ; (used for diagnostics and tuning)
; endif                                    ; (used for diagnostics and tuning)
; 
; ; ZERO OFFSET
; kZeroOffset    chnget    "ZeroOffset"                    ; (used for diagnostics and tuning)
; ktrig    trigger    kZeroOffset,0.5,0                    ; (used for diagnostics and tuning)
; ktrig2    changed    gknote                        ; (used for diagnostics and tuning)
; if ktrig==1||ktrig2==1 then                        ; (used for diagnostics and tuning)
;  kzero        =    0                    ; (used for diagnostics and tuning)
;  chnset    kzero,"offset"                        ; (used for diagnostics and tuning)
; endif                                    ; (used for diagnostics and tuning)
 
endin



instr    2    ; receive midi note ons and offs
 inote    notnum                                ; midi note number
 ivel    veloc    0,1                            ; key velocity (0 to 1)
 ibc        chnget    "bc"                        ; Boundary Conditions selection (1,2,3 or 4)
 iNoteMin    =    giNoteRanges[(ibc*2)-2]                ; minimum note number which has a corresponding mapping value for this boundary condition algorithm
 iNoteMax    =    giNoteRanges[(ibc*2)-1]                ; maximum note number which has a corresponding mapping value for this boundary condition algorithm
 if inote>=iNoteMin && inote<=iNoteMax then
      event_i    "i",3+(inote*0.001),0,3600,inote,ivel
      ;tableiw    1,inote,giNoteTable                    ; add note to the active notes table
    gkNoteTable[inote]    =    1
  giPolyLimit    chnget    "PolyLimit"
  
  kactive    active    p1            ; scan active instances of this instrument
  if kactive>giPolyLimit&&giPolyLimit>0 then    ; if polyphony limit has been exceeded...
   tablew    0,inote,giNoteTable        ; remove this note number from the notes table
   turnoff2    3,1,1                ; ...turn off the oldest note and allow it to release (interrupt release)
   turnoff2    p1,1,0                ; turn off the oldest version of this instrument immediately
  endif
  
  krel    release                    ; scan for this note's MIDI release. 0 = active, 1 = released
  if (krel==1) then                ; If this note has been released...
   ;tablew    0,inote,giNoteTable        ; remove this note number from the notes table
    gkNoteTable[inote]    =    0
  endif
  
  if giPolyLimit>0 then                ; bypass polyphony control if its value is zero
   gkActiveNotes    init    i(gkActiveNotes) + 1    ; increment active notes counter at note start
   if gkActiveNotes>giPolyLimit then        ; if the addition of this new note results in exceeding the polyphony limit...
    turnoff2    3,1,1                ; ...turn off the oldest note and allow it to release (interrupt release)
    gkActiveNotes    limit    gkActiveNotes - 1,0,127    ; decrement active notes counter
   endif
  else
   gkActiveNotes    =    0            ; if no polyphony limit is being used (slider is set to zero) then leave ActiveNotes at zero 
  endif
 endif
 
endin



instr    3    ; Sound producing instrument. Triggered from instrument 2.
 inote    =    p4
 ivel    =    p5
 ;kactive    table    inote,giNoteTable    ; Read active value for this note from the table 1 = midi note down, 0 = midi note released
 kactive    =    gkNoteTable[p4]    
; ; TUNING
; ; REINIT
; kmetro    metro    8            ; (used for diagnostics and tuning)
; if kmetro==1 then                ; (used for diagnostics and tuning)
;  kchanged    changed    gkoffset        ; (used for diagnostics and tuning)
;  if kchanged==1 then                ; (used for diagnostics and tuning)
;   reinit RESTART                ; (used for diagnostics and tuning)
;  endif                    ; (used for diagnostics and tuning)
; RESTART:                    ; (used for diagnostics and tuning)
; endif                        ; (used for diagnostics and tuning)
;     chnset    inote,"note"            ; (used for diagnostics and tuning)
; gknote    init    inote            ; (used for diagnostics and tuning)

 iDurRatio  =       i(gkKybdScal) == 1 ? cpsmidinn(60)/cpsmidinn(inote) : 1     ; turn on or off keyboard scaling
 
 ib            chnget    "b"                ; damping
 ib         /=      iDurRatio
 irel        chnget    "rel"            ; release time
 iT30        chnget    "T30"            ; 30dB decay time
 iT30       *=      iDurRatio
 iT30_Hold    chnget    "T30_Hold"        ; over-ride decay slider value (HOLD button)
 iT30        =    iT30_Hold == 1 ? 3600 : iT30    ; if 'HOLD' button is active set T30 value to 3600 and ignore slider value
 ibc        chnget    "bc"            ; boundary conditions selection (1,2,3 or 4)
 iwid        chnget    "wid"            ; strike width
 ipos        tablei    ivel,giVelToPos,1    ; map midi key velocity to strike position parameter
 itone        tablei    ivel,giVelToTone,1    ; map midi key velocity to LPF cutoff frequency
 itone        scale_i    itone,14,4        ; reScale 0 - 1 to 4 - 14 (oct format)
 iStrk        tablei    ivel,giVelToAmp,1    ; map midi key velocity to strike velocity parameter

 ; STIFFNESS / PITCH
 if ibc==1 then        ; if Clamped-Clamped
  ibcL    =    1
  ibcR    =    1 
  itab    =    giCC
 elseif ibc==2 then    ; if Clamped-Pivoting
  ibcL    =    1
  ibcR    =    2
  itab    =    giCP
 elseif ibc==3 then    ; if Pivoting-Pivoting
  ibcL    =    2
  ibcR    =    2
  itab    =    giPP
 elseif ibc==4 then    ; if Clamped-Free
  ibcL    =    1
  ibcR    =    3
  itab    =    giCF  
 endif
 iK    table    inote,itab        ; read note to stiffness mapping from the appropriate function table
 ;ioffset    chnget    "offset"    ; (used for diagnostics and tuning)
 ;iK    =    iK + ioffset        ; (used for diagnostics and tuning)
 chnset    iK,"output"
 
 ; VELOCITY SCALING (ACCORDING TO NOTE PLAYED). Lower notes need to have an attenuated velocity value
 iVelScal    table    inote,giVelFunc

 ; STRIKE WIDTH SCALING (ACCORDING TO NOTE PLAYED). Higher notes need to have an increased strike width value
 iWidScal    table    inote,giWidFunc
 iwid        =    iwid * iWidScal

 ; SCALE STRIKE VELOCITY ACCORDING TO STRIKE WIDTH 
 ivel    table    iwid,giWidVelScal,1
 
 ; MODULATION ENVELOPE
 iModEnvDur    chnget    "ModEnvDur"    ; envelope duration
 kndx    linseg    0,iModEnvDur,1        ; envelope reading index
 kModEnv    tablei    kndx,giModEnv,1    ; read function
 kscan    =    gkscan * (kModEnv-0.01)    ; multiply 'Rate' slider and 'Envelope'
 kporttime    linseg    0,0.05,0.1    ; portamento time function
 kscan    portk    kscan, kporttime    ; apply portamento smoothing (without this a click can sometimes result if scanning rate changes abruptly)
 
 ; RANDOM MODULATION
 kRandScan    rspline    0,gkRndModAmt,gkRndModRte,gkRndModRte*2    ; splinic random modulation
 kscan    limit    kscan + kRandScan,0.01,10000            ; added to the modulation rate value so far (and limited)
 
 ; BARMODELS
 if gkScanMix==0 then                                            ; mix = 0
  asig     barmodel_ksmps     ibcL, ibcR, iK, ib,     0, iT30, ipos, ivel*iVelScal*iStrk, iwid         ; only one barmodel
 elseif gkScanMix>0 && (gkscan>0 || gkRndModAmt>0) then                            ; 0 < mix < 1
  asig     barmodel_ksmps     ibcL, ibcR, iK, ib,     0, iT30, ipos, ivel*iVelScal*iStrk, iwid         ; mix two barmodels
  asig2     barmodel_ksmps     ibcL, ibcR, iK, ib, kscan, iT30, ipos, ivel*iVelScal*iStrk, iwid    
  asig    ntrpol        asig, asig2, gkScanMix
 elseif gkScanMix==1 then                                        ; mix = 1
  asig     barmodel_ksmps     ibcL, ibcR, iK, ib, kscan, iT30, ipos, ivel*iVelScal*iStrk, iwid         ; only one barmodel
 endif
 
 ; AMPLITUDE ATTACK ENVELOPE
 iAttAmt    chnget    "AttAmt"        ; attack amount
 iAttTim    chnget    "AttTim"        ; attack time
 aatt    init    1
 if iAttTim>0 then                ; only create attack envelope if 'Time' is greater than zero 
  aatt    linseg        1-iAttAmt,iAttTim*iDurRatio,1
 endif
 asig    =    asig * aatt * gkOutAmp

 ; AMPLITUDE RELEASE ENVELOPE
 arel    init    1
 if kactive==0 then            ; if the midi key that started this note has been released...
  arel    transeg    1,irel*iDurRatio,-4,0        ; create a release envelope
  asig    =    asig * arel        ; apply envelope
 endif
 kOffTrig    trigger    kactive,0.5,1    ; sense when note key has been released
 kOffTrig    delayk    kOffTrig,irel    ; kill note trigger delayed by normal release time to allow release segment to complete
 if kOffTrig==1 then            ; when kill note trigger is finally received...
  turnoff                ; turn note off
  if giPolyLimit>0 then            ; ...if greater than zero
   gkActiveNotes    limit    gkActiveNotes - 1,0,127    ; decrement active notes counter
  endif
 endif

 ; POLYPHONY INTERRUPT
 aInterrupt    linsegr    1,giInterruptTime,0        ; if the note is forced to interrupt (due to polyphony limit violation) then this envelope will be triggered
 asig    =    asig * aInterrupt

 ; TONE CONTROL (LPF)
 aToneEnv    =    aatt*aInterrupt*arel        ; cutoff frequency will follow attack, release and interrupt envelopes
 ahp,alp,abp,abr statevar asig, cpsoct(itone) * aToneEnv, 1
 asig    =    alp
 
 ; PANNING
 ;kpan    oscili    gkPanDepth,kscan*2,gisine        ; panning follows scanning function (frequency doubled)
 ;aL,aR    pan2    asig, 0.5 + kpan
 apan    oscili    gkPanDepth,kscan*2,gisine        ; panning follows scanning function (frequency doubled)
 aL,aR    pan2_ksmps    asig, 0.5 + apan
 
 if gkRvbMix>0 then
  gasendL    +=    aL * gkWet
  gasendR    +=    aR * gkWet
 endif
 
     outs        aL*gkDry,aR*gkDry
endin



instr    80    ; reverb
 if gkRvbMix>0 then        ; if mix is zero, bypass reverb for efficiency
  aL,aR    reverbsc    gasendL,gasendR,gkRvbSize,4000
    outs        aL,aR
     clear        gasendL,gasendR
 endif

 ; TURN LABELS ON OR OFF
 kOnTrig    trigger    gkRvbMix,0.005,0
 kOffTrig    trigger    gkRvbMix,0.005,1
 if kOnTrig==1 then
  chnset    "fontColour(170,170,170)","ReverbLabel"
  chnset    "colour(170,170,170)","ReverbLine"
 elseif kOffTrig==1 then
  chnset    "fontColour(30,30,30)","ReverbLabel" 
  chnset    "colour(30,30,30)","ReverbLine" 
 endif

 gkAttTim    chnget    "AttTim"
 kOnTrig    trigger    gkAttTim,0.0005,0
 kOffTrig    trigger    gkAttTim,0.0005,1
 if kOnTrig==1 then
  chnset    "fontColour(170,170,170)","AttackLabel"
  chnset    "colour(170,170,170)","AttackLine"
 elseif kOffTrig==1 then
  chnset    "fontColour(30,30,30)","AttackLabel" 
  chnset    "colour(30,30,30)","AttackLine" 
 endif

endin


instr    99
 kpreset    chnget    "preset"
 kpreset    init    1
 if changed(kpreset)==1 then
  reinit SET_PRESET
  SET_PRESET:
  ipreset    init    i(kpreset)
  if ipreset==1 then    ; xylobar
   giVelToPos    ftgen    2,0,128,-7,0.5,128,0            ; map MIDI velocity to barmodel strike position parameter
           chnset    "tableNumber(2)","table2"
   giVelToTone    ftgen    3,0,128,-7,0.5,128,1            ; map MIDI velocity to tone (LPF)
           chnset    "tableNumber(3)","table3"
   giVelToAmp    ftgen    4,0,128,-5,0.01,128,1            ; map MIDI velocity to amplitude
           chnset    "tableNumber(4)","table4"
   giModEnv    ftgen    5,0,512,-5,1,512,1            ; modulation rate envelope
           chnset    "tableNumber(5)","table5"
        chnset    2, "bc"
        chnset    5.5, "T30"
        chnset    0, "T30_Hold"
        chnset    5.5, "rel"
           chnset    0.4, "b"
        chnset    5, "PolyLimit"
        chnset    0.5, "wid"
        chnset  0, "AttTim"
        chnset  0, "AttAmt"
        chnset    0, "RvbMix"
        chnset    0.8, "RvbSize"
        chnset    0.8, "OutAmp"
           chnset    0,"ScanMix"
            chnset    0.1,"scan"
          chnset    0,"scan_x100"
        chnset    1, "ModEnvDur"
        chnset    0, "RndModAmt"
        chnset    0.1, "RndModRte"
        chnset    0, "PanDepth"

  elseif ipreset==2 then    ; Scan Envelope
   giVelToPos    ftgen    2,0,128,-7,0,128,0.5            ; map MIDI velocity to barmodel strike position parameter
           chnset    "tableNumber(2)","table2"
   giVelToTone    ftgen    3,0,128,-7,0.5,128,1            ; map MIDI velocity to tone (LPF)
           chnset    "tableNumber(3)","table3"
   giVelToAmp    ftgen    4,0,128,-5,0.5,128,1            ; map MIDI velocity to amplitude
           chnset    "tableNumber(4)","table4"
   giModEnv    ftgen    5,0,512,-5,2,512,0.01            ; modulation rate envelope
           chnset    "tableNumber(5)","table5"
        chnset    1, "bc"
        chnset    7.4, "T30"
        chnset    0, "T30_Hold"
        chnset    0.32, "rel"
           chnset    0.006, "b"
        chnset    2, "PolyLimit"
        chnset    0.5, "wid"
        chnset  0, "AttTim"
        chnset  0, "AttAmt"
        chnset    0, "RvbMix"
        chnset    0.8, "RvbSize"
           chnset    0.2, "OutAmp"
        chnset    1,"ScanMix"
           chnset    0.033,"scan"           
           chnset    1,"scan_x100"
        chnset    0.3, "ModEnvDur"
        chnset    0, "ModRndAmt"
        chnset    0, "ModRndRte"
        chnset    0, "PanDepth"

  elseif ipreset==3 then    ; Soft Sustain
   giVelToPos    ftgen    2,0,128,-7,0.9,128,0.9            ; map MIDI velocity to barmodel strike position parameter
           chnset    "tableNumber(2)","table2"
   giVelToTone    ftgen    3,0,128,-7,0.5,128,0.9            ; map MIDI velocity to tone (LPF)
           chnset    "tableNumber(3)","table3"
   giVelToAmp    ftgen    4,0,128,-5,0.5,128,1            ; map MIDI velocity to amplitude
           chnset    "tableNumber(4)","table4"
   giModEnv    ftgen    5,0,512,-5,1,512,1            ; modulation rate envelope
           chnset    "tableNumber(5)","table5"
        chnset    3, "bc"
        chnset    7.4, "T30"
        chnset    1, "T30_Hold"
        chnset    2.50, "rel"
           chnset    0.000, "b"
        chnset    5, "PolyLimit"
        chnset    0.5, "wid"
        chnset  1, "AttTim"
        chnset  1, "AttAmt"
        chnset    0.4, "RvbMix"
        chnset    0.8, "RvbSize"
        chnset    0.218, "OutAmp"
           chnset    0.5,"ScanMix"
           chnset    0.113,"scan"
           chnset    0,"scan_x100"                     
        chnset    0.2, "RndModAmt"
        chnset    0.4, "RndModRte"
        chnset    0.5, "PanDepth"
        
  elseif ipreset==4 then    ; Rhodes
   giVelToPos    ftgen    2,0,128,-7,0.5,128,0.9            ; map MIDI velocity to barmodel strike position parameter
           chnset    "tableNumber(2)","table2"
   giVelToTone    ftgen    3,0,128,-7,0.5,128,0.7            ; map MIDI velocity to tone (LPF)
           chnset    "tableNumber(3)","table3"
   giVelToAmp    ftgen    4,0,128,-5,0.1,128,1            ; map MIDI velocity to amplitude
           chnset    "tableNumber(4)","table4"
   giModEnv    ftgen    5,0,512,-5,1,512,1            ; modulation rate envelope
           chnset    "tableNumber(5)","table5"
        chnset    3, "bc"
        chnset    11.6, "T30"
        chnset    0, "T30_Hold"
        chnset    0.08, "rel"
           chnset    0.038, "b"
        chnset    5, "PolyLimit"
        chnset    0.98 ,"wid"
        chnset  0, "AttTim"
        chnset  0, "AttAmt"
        chnset    0, "RvbMix"
        chnset    0.8, "RvbSize"
        chnset    0.4, "OutAmp"
           chnset    0,"ScanMix"
           chnset    0.014,"scan"
           chnset    1,"scan_x100"                     
        chnset    0, "RndModAmt"
        chnset    0.1, "RndModRte"
        chnset    0.420, "PanDepth"

  elseif ipreset==5 then    ; Good Vibes
   giVelToPos    ftgen    2,0,128,-7,0.4,128,0.5            ; map MIDI velocity to barmodel strike position parameter
           chnset    "tableNumber(2)","table2"
   giVelToTone    ftgen    3,0,128,-7,0.5,128,0.7            ; map MIDI velocity to tone (LPF)
           chnset    "tableNumber(3)","table3"
   giVelToAmp    ftgen    4,0,128,-5,0.5,128,1            ; map MIDI velocity to amplitude
           chnset    "tableNumber(4)","table4"
   giModEnv    ftgen    5,0,512,-5,1,512,0.8            ; modulation rate envelope
           chnset    "tableNumber(5)","table5"
        chnset    3, "bc"
        chnset    16.6, "T30"
        chnset    0, "T30_Hold"
        chnset    0.3, "rel"
           chnset    0.038, "b"
        chnset    5, "PolyLimit"
        chnset    0.54 ,"wid"
        chnset  0.005, "AttTim"
        chnset  1, "AttAmt"
        chnset    0, "RvbMix"
        chnset    0.8, "RvbSize"
        chnset    0.323, "OutAmp"
           chnset    0.548,"ScanMix"
           chnset    0.014,"scan"
           chnset    1,"scan_x100"                     
        chnset    8.84, "ModEnvDur"
        chnset    0, "RndModAmt"
        chnset    0.1, "RndModRte"
        chnset    0.420, "PanDepth"

  elseif ipreset==6 then    ; Bad Vibes
   giVelToPos    ftgen    2,0,128,-7,0,128,0.5            ; map MIDI velocity to barmodel strike position parameter
           chnset    "tableNumber(2)","table2"
   giVelToTone    ftgen    3,0,128,-7,1,128,1            ; map MIDI velocity to tone (LPF)
           chnset    "tableNumber(3)","table3"
   giVelToAmp    ftgen    4,0,128,-5,0.5,128,1            ; map MIDI velocity to amplitude
           chnset    "tableNumber(4)","table4"
   giModEnv    ftgen    5,0,512,-5,1,512,0.8            ; modulation rate envelope
           chnset    "tableNumber(5)","table5"
        chnset    4, "bc"
        chnset    16.6, "T30"
        chnset    1, "T30_Hold"
        chnset    0.08, "rel"
           chnset    0.038, "b"
        chnset    5, "PolyLimit"
        chnset    0.54 ,"wid"
        chnset  0.012, "AttTim"
        chnset  1, "AttAmt"
        chnset    0.21, "RvbMix"
        chnset    0.65, "RvbSize"
        chnset    0.058, "OutAmp"
           chnset    1,"ScanMix"
           chnset    0.007,"scan"
           chnset    1,"scan_x100"                     
        chnset    8.84, "ModEnvDur"
        chnset    0.272, "RndModAmt"
        chnset    1.313, "RndModRte"
        chnset    0.420, "PanDepth"

  elseif ipreset==7 then    ; Hard Beater
   giVelToPos    ftgen    2,0,128,-7,0.3,128,0            ; map MIDI velocity to barmodel strike position parameter
           chnset    "tableNumber(2)","table2"
   giVelToTone    ftgen    3,0,128,-7,0.5,128,1            ; map MIDI velocity to tone (LPF)
           chnset    "tableNumber(3)","table3"
   giVelToAmp    ftgen    4,0,128,-5,0.01,128,1            ; map MIDI velocity to amplitude
           chnset    "tableNumber(4)","table4"
   giModEnv    ftgen    5,0,512,-5,1,512,1            ; modulation rate envelope
           chnset    "tableNumber(5)","table5"
        chnset    1, "bc"
        chnset    3.0, "T30"
        chnset    0, "T30_Hold"
        chnset    3.00, "rel"
           chnset    0.365, "b"
        chnset    4, "PolyLimit"
        chnset    0.47 ,"wid"
        chnset  0, "AttTim"
        chnset  0, "AttAmt"
        chnset    0, "RvbMix"
        chnset    0.25, "RvbSize"
        chnset    1.8, "OutAmp"
           chnset    0,"ScanMix"
           chnset    0.01,"scan"
           chnset    0,"scan_x100"                     
        chnset    0.1, "ModEnvDur"
        chnset    0.0, "RndModAmt"
        chnset    0.01, "RndModRte"
        chnset    0.0, "PanDepth"

  elseif ipreset==8 then    ; Stacking Plates
   giVelToPos    ftgen    2,0,128,-7,0.3,128,0            ; map MIDI velocity to barmodel strike position parameter
           chnset    "tableNumber(2)","table2"
   giVelToTone    ftgen    3,0,128,-7,1,128,1            ; map MIDI velocity to tone (LPF)
           chnset    "tableNumber(3)","table3"
   giVelToAmp    ftgen    4,0,128,-5,0.01,128,1            ; map MIDI velocity to amplitude
           chnset    "tableNumber(4)","table4"
   giModEnv    ftgen    5,0,512,-5,1,512,1            ; modulation rate envelope
           chnset    "tableNumber(5)","table5"
        chnset    4, "bc"
        chnset    0.11, "T30"
        chnset    0, "T30_Hold"
        chnset    0.11, "rel"
           chnset    0.269, "b"
        chnset    0, "PolyLimit"
        chnset    0.26 ,"wid"
        chnset  0, "AttTim"
        chnset  0, "AttAmt"
        chnset    0, "RvbMix"
        chnset    0.8, "RvbSize"
        chnset    2.5, "OutAmp"
           chnset    0,"ScanMix"
           chnset    0.01,"scan"
           chnset    0,"scan_x100"                     
        chnset    0.1, "ModEnvDur"
        chnset    0.0, "RndModAmt"
        chnset    0.01, "RndModRte"
        chnset    0.0, "PanDepth"

  elseif ipreset==9 then    ; Bowed Plank
   giVelToPos    ftgen    2,0,128,-7,0.3,128,0            ; map MIDI velocity to barmodel strike position parameter
           chnset    "tableNumber(2)","table2"
   giVelToTone    ftgen    3,0,128,-7,1,128,1            ; map MIDI velocity to tone (LPF)
           chnset    "tableNumber(3)","table3"
   giVelToAmp    ftgen    4,0,128,-5,0.01,128,1            ; map MIDI velocity to amplitude
           chnset    "tableNumber(4)","table4"
   giModEnv    ftgen    5,0,512,-5,1,512,1            ; modulation rate envelope
           chnset    "tableNumber(5)","table5"
        chnset    2, "bc"
        chnset    6, "T30"
        chnset    0, "T30_Hold"
        chnset    6, "rel"
           chnset    0.144, "b"
        chnset    5, "PolyLimit"
        chnset    0.47 ,"wid"
        chnset  0.19, "AttTim"
        chnset  1, "AttAmt"
        chnset    0, "RvbMix"
        chnset    0.8, "RvbSize"
        chnset    2.1, "OutAmp"
           chnset    0,"ScanMix"
           chnset    0.01,"scan"
           chnset    0,"scan_x100"                     
        chnset    0.1, "ModEnvDur"
        chnset    0.0, "RndModAmt"
        chnset    0.01, "RndModRte"
        chnset    0.0, "PanDepth"

  elseif ipreset==10 then    ; Interference
   giVelToPos    ftgen    2,0,128,-7,0.33,128,0.1            ; map MIDI velocity to barmodel strike position parameter
           chnset    "tableNumber(2)","table2"
   giVelToTone    ftgen    3,0,128,-7,0.6,128,1            ; map MIDI velocity to tone (LPF)
           chnset    "tableNumber(3)","table3"
   giVelToAmp    ftgen    4,0,128,-5,0.1,128,1            ; map MIDI velocity to amplitude
           chnset    "tableNumber(4)","table4"
   giModEnv    ftgen    5,0,512,-5,1,512,0.2            ; modulation rate envelope
           chnset    "tableNumber(5)","table5"
        chnset    1, "bc"
        chnset    2.73, "T30"
        chnset    0, "T30_Hold"
        chnset    0.37, "rel"
           chnset    0.011, "b"
        chnset    2, "PolyLimit"
        chnset    0.29 ,"wid"
        chnset  0, "AttTim"
        chnset  0, "AttAmt"
        chnset    0, "RvbMix"
        chnset    0.8, "RvbSize"
        chnset    0.5, "OutAmp"
           chnset    1,"ScanMix"
           chnset    0.318,"scan"
           chnset    1,"scan_x100"                     
        chnset    2.73, "ModEnvDur"
        chnset    0.0, "RndModAmt"
        chnset    0.01, "RndModRte"
        chnset    0.0, "PanDepth"

  elseif ipreset==11 then    ; Ride Cymbal
   giVelToPos    ftgen    2,0,128,-7,0.33,128,0.1            ; map MIDI velocity to barmodel strike position parameter
           chnset    "tableNumber(2)","table2"
   giVelToTone    ftgen    3,0,128,-7,0,128,1            ; map MIDI velocity to tone (LPF)
           chnset    "tableNumber(3)","table3"
   giVelToAmp    ftgen    4,0,128,-5,0.1,128,1            ; map MIDI velocity to amplitude
           chnset    "tableNumber(4)","table4"
   giModEnv    ftgen    5,0,512,-5,1,512,1            ; modulation rate envelope
           chnset    "tableNumber(5)","table5"
        chnset    4, "bc"
        chnset    18.12, "T30"
        chnset    0, "T30_Hold"
        chnset    0.28, "rel"
           chnset    0.028, "b"
        chnset    3, "PolyLimit"
        chnset    0.26 ,"wid"
        chnset  0, "AttTim"
        chnset  0, "AttAmt"
        chnset    0.25, "RvbMix"
        chnset    0.8, "RvbSize"
        chnset    0.3, "OutAmp"
           chnset    1,"ScanMix"
           chnset    1,"scan"
           chnset    1,"scan_x100"                     
        chnset    10, "ModEnvDur"
        chnset    0.0, "RndModAmt"
        chnset    0.01, "RndModRte"
        chnset    0.0, "PanDepth"

  elseif ipreset==12 then    ; Tubular Bell
   giVelToPos    ftgen    2,0,128,-7,0.33,128,0.1            ; map MIDI velocity to barmodel strike position parameter
           chnset    "tableNumber(2)","table2"
   giVelToTone    ftgen    3,0,128,-7,0,128,0.8            ; map MIDI velocity to tone (LPF)
           chnset    "tableNumber(3)","table3"
   giVelToAmp    ftgen    4,0,128,-5,0.1,128,1            ; map MIDI velocity to amplitude
           chnset    "tableNumber(4)","table4"
   giModEnv    ftgen    5,0,512,-5,1,512,0.8            ; modulation rate envelope
           chnset    "tableNumber(5)","table5"
        chnset    4, "bc"
        chnset    30, "T30"
        chnset    0, "T30_Hold"
        chnset    30, "rel"
           chnset    0.001, "b"
        chnset    1, "PolyLimit"
        chnset    0.14 ,"wid"
        chnset  0, "AttTim"
        chnset  0, "AttAmt"
        chnset    0.25, "RvbMix"
        chnset    0.65, "RvbSize"
        chnset    0.5, "OutAmp"
           chnset    0.352,"ScanMix"
           chnset    0.178,"scan"
           chnset    0,"scan_x100"                     
        chnset    10, "ModEnvDur"
        chnset    0.0, "RndModAmt"
        chnset    0.01, "RndModRte"
        chnset    0.15, "PanDepth"

  elseif ipreset==13 then    ; Discord Bar
   giVelToPos    ftgen    2,0,128,-7,0.33,128,0.1            ; map MIDI velocity to barmodel strike position parameter
           chnset    "tableNumber(2)","table2"
   giVelToTone    ftgen    3,0,128,-7,0,128,0.8            ; map MIDI velocity to tone (LPF)
           chnset    "tableNumber(3)","table3"
   giVelToAmp    ftgen    4,0,128,-5,0.1,128,1            ; map MIDI velocity to amplitude
           chnset    "tableNumber(4)","table4"
   giModEnv    ftgen    5,0,512,-5,1,512,1            ; modulation rate envelope
           chnset    "tableNumber(5)","table5"
        chnset    4, "bc"
        chnset    2.95, "T30"
        chnset    0, "T30_Hold"
        chnset    0.28, "rel"
           chnset    1.274, "b"
        chnset    1, "PolyLimit"
        chnset    0.26 ,"wid"
        chnset  0, "AttTim"
        chnset  0, "AttAmt"
        chnset    0.25, "RvbMix"
        chnset    0.8, "RvbSize"
        chnset    0.437, "OutAmp"
           chnset    1,"ScanMix"
           chnset    0.425,"scan"
           chnset    1,"scan_x100"                     
        chnset    10, "ModEnvDur"
        chnset    0.0, "RndModAmt"
        chnset    0.01, "RndModRte"
        chnset    0, "PanDepth"

  endif
 endif 
endin

</CsInstruments>

<CsScore>
i 1  0 [3600*24*7]    ; read in widgets
i 80 0 [3600*24*7]    ; reverb
i 99 0 [3600*24*7]    ; presets
</CsScore>

</CsoundSynthesizer>
