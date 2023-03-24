
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; GEN33.csd
; Written by Iain McCurdy, 2014
; 
; GEN33 generates a harmonic waveform based on data stored in another table. 
; The data for each partial in the second table is stored in the form: Strength, Partial Number / Frequency, Phase, for each partial.
; Therefore data for 10 partials would be stored using 30 items of data. GEN02 is normally used for this purpose.
; 
; In this example the user can select from a number of data sets of partial using a combobox.
; The number of partials contained within the chosen data set is displayed below the combobox.
; 
; When the GEN33 table is created the user can choose to limit the number of partials used.
; 
; In this example the GEN33 table can be auditioned and interacted with manually using the controls in the first groupbox
; or it can be played from the MIDI keyboard in which case key velocity is mapped to the number of partials used by GEN33
; thereby implementing a kind of velocity to brightness mechanism.


<Cabbage>
form caption("GEN33"), size(520, 320), pluginId("gn33")
gentable bounds(  0,  0, 520, 120), identChannel("table1"), tableNumber(1), tableColour("yellow"), zoom(-1), , fill(0), ampRange(1,-1,1)


groupbox bounds(0, 120,370,120), text("Manual Control"), plant("Manual"), fontColour("white"){
checkbox bounds( 10, 30,110, 20), channel("ManualOnOff"),  value(0), text("Test On/Off")
label    bounds( 10, 50,140, 11), text("Source")
combobox bounds( 10, 61,140, 20), channel("source"), value(1), text("Tubular Bell","Chladni Plates","Tibetan Bowl 180mm","Tibetan Bowl 152mm","Tibetan Bowl 140mm","Small Handbell","Spinel Sphere","Clarinet")

label    bounds( 10, 84,100, 13), text("Maximum Harms:"), fontColour("white"), align(left)
nslider  bounds(110, 81, 40, 20), text(""), fontColour(white), channel("NumPartials"), range(0, 100, 0,1,1)

rslider  bounds(150, 30, 80, 80), channel("nh"), text("Num.Harms."), valueTextBox(1), textBox(1), range(1,25,25,1,1), TrackerColour("yellow"), colour(200,200,200)
rslider  bounds(220, 30, 80, 80), channel("amp"), text("Amp."), valueTextBox(1), textBox(1), range(0,1.000,0.1,0.5), TrackerColour("yellow"), colour(200,200,200)
rslider  bounds(290, 30, 80, 80), channel("frq"), text("Freq."), valueTextBox(1), textBox(1), range(5,1000.000,520,0.5), TrackerColour("yellow"), colour(200,200,200)
}

groupbox bounds(370,120,150,120), text("MIDI [Keyboard]"), plant("MIDI"), fontColour("white"){
rslider  bounds(  0, 30, 80, 80), channel("DecTim"), text("Dec.Time"), valueTextBox(1), textBox(1), range(0.1,15.00,8,0.5), TrackerColour("yellow"), colour(200,200,200)
rslider  bounds( 70, 30, 80, 80), channel("RelTim"), text("Rel.Tim"), valueTextBox(1),  textBox(1), range(0.1,15.00,8,0.5), TrackerColour("yellow"), colour(200,200,200)
}

keyboard bounds(  0,240,520, 80)
</Cabbage>
                    
<CsoundSynthesizer>

<CsOptions>   
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps         =     32    ; NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls         =     2    ; NUMBER OF CHANNELS (1=MONO)
0dbfs        =    1    ; MAXIMUM AMPLITUDE
    massign    0,3    ; assign midi notes on all channels to instrument 3

; partial frequency data         strength    frequency    phase
; tubular bell
gidata1        ftgen    0,0,-10*3,-2,    1,        272,        0,\ 
                    1,        538,        0,\ 
                    1,        874,        0,\ 
                    1,        1281,        0,\ 
                    1,        1755,        0,\ 
                    1,        2264,        0,\ 
                    1,        2813,        0,\ 
                    1,        3389,        0,\ 
                    1,        4822,        0,\ 
                    1,        5255,        0

;Chladni plates     
gidata2        ftgen    0,0,-5*3,-2,    1,        62,        0,\
                    1,        107,            0,\
                    1,        360,            0,\
                    1,        460,            0,\
                    1,        863,            0

;tibetan bowl (180mm)     
gidata3        ftgen    0,0,-7*3,-2,    1,        221,        0,\
                    1,        614,            0,\
                    1,        1145,           0,\
                    1,        1804,           0,\
                    1,        2577,           0,\
                    1,        3456,           0,\
                    1,        4419,           0

;tibetan bowl (152 mm)
gidata4        ftgen    0,0,-7*3,-2,    1,        314,        0,\
                    1,        836,            0,\
                    1,        1519,           0,\
                    1,        2360,           0,\
                    1,        3341,           0,\
                    1,        4462,           0,\
                    1,        5696,           0

;tibetan bowl (140 mm)     
gidata5        ftgen    0,0,-5*3,-2,    1,        528,        0,\
                    1,        1460,           0,\
                    1,        2704,           0,\
                    1,        4122,           0,\
                    1,        5694,           0

;small handbell
gidata6        ftgen    0,0,-22*3,-2,    1,        1312.0,        0,\ 
                    1,        1314.5,         0,\
                    1,        2353.3,         0,\
                    1,        2362.9,         0,\
                    1,        3306.5,         0,\
                    1,        3309.4,         0,\
                    1,        3923.8,         0,\
                    1,        3928.2,         0,\
                    1,        4966.6,         0,\
                    1,        4993.7,         0,\
                    1,        5994.4,         0,\
                    1,        6003.0,         0,\
                    1,        6598.9,         0,\
                    1,        6619.7,         0,\
                    1,        7971.7,         0,\
                    1,        7753.2,         0,\
                    1,        8413.1,         0,\
                    1,        8453.3,         0,\
                    1,        9292.4,         0,\
                    1,        9305.2,         0,\
                    1,        9602.3,         0,\
                    1,        9912.4,         0

;spinel sphere
gidata7        ftgen    0,0,-25*3,-2,    1,        977.25,        0,\
                    1,        1003.16,        0,\
                    1,        1390.13,        0,\
                    1,        1414.93,        0,\
                    1,        1432.84,        0,\
                    1,        1465.34,        0,\
                    1,        1748.48,        0,\
                    1,        1834.20,        0,\
                    1,        1919.90,        0,\
                    1,        1933.64,        0,\
                    1,        1987.20,        0,\
                    1,        2096.48,        0,\
                    1,        2107.10,        0,\
                    1,        2202.08,        0,\
                    1,        2238.40,        0,\
                    1,        2280.10,        0,\
                    1,        2290.53,        0,\
                    1,        2400.88,        0,\
                    1,        2435.85,        0,\
                    1,        2507.80,        0,\
                    1,        2546.30,        0,\
                    1,        2608.55,        0,\
                    1,        2652.35,        0,\
                    1,        2691.70,        0,\
                    1,        2708.00,        0
            
;clarinet
gidata8        ftgen    0,0,-24*3,-2,        0.753504,    100,    rnd(1),\
                        0.011446,    200,    rnd(1),\
                        0.501720,    300,    rnd(1),\
                        0.055791,    400,    rnd(1),\
                        0.241768,    500,    rnd(1),\
                        0.037889,    600,    rnd(1),\
                        0.027203,    700,    rnd(1),\
                        0.062793,    800,    rnd(1),\
                        0.102494,    900,    rnd(1),\
                        0.030506,    1000,    rnd(1),\
                        0.042453,    1100,    rnd(1),\
                        0.050202,    1200,    rnd(1),\
                        0.031077,    1300,    rnd(1),\
                        0.027208,    1400,    rnd(1),\
                        0.020897,    1500,    rnd(1),\
                        0.022205,    1600,    rnd(1),\
                        0.012154,    1700,    rnd(1),\
                        0.008160,    1800,    rnd(1),\
                        0.004886,    1900,    rnd(1),\
                        0.005076,    2000,    rnd(1),\
                        0.003805,    2100,    rnd(1),\
                        0.001322,    2200,    rnd(1),\
                        0.001149,    2300,    rnd(1),\
                        0.001450,    2300,    rnd(1),\
                        0.001450,    2400,    rnd(1)


giTabSize    =    (2^15) +1    ; function table size of the GEN33 table

instr    1    ; manual instrument
 gknh        chnget    "nh"            ; read in widgets
 gkamp        chnget    "amp"
 gkfrq        chnget    "frq"
 gkManualOnOff    chnget    "ManualOnOff"
 gksource    chnget    "source"
 gksource    init    1
 
 kporttime    linseg    0,0.001,0.05
 gkfrq        portk    gkfrq, kporttime
 gkamp        portk    gkamp, kporttime
 
 ktrig        changed    gknh,gkManualOnOff,gksource    ; if listed widgets are changed generate a trigger

 if ktrig==1 then                ; if a trigger has been generated...
  reinit UPDATE_WAVEFORM            ; ...begin a reinitialisation pass from the label UPDATE_WAVEFORM
 endif
 UPDATE_WAVEFORM:                ; begin reinitialisation from here
  gisource    =    gidata1+i(gksource)-1    ; derive number of source function table from combo box and number of first table in the list 
  iNumPartials    =    ftlen(gisource)/3
          chnset    iNumPartials,"NumPartials"    ; write number of partials for this algorithm to number box
  giwave    ftgen    1,0, giTabSize,-33,gisource,i(gknh),1,-1  ; create a GEN33 function table / waveform from the source function table data and make use of the user defined number of partials
 rireturn
 
 
 if ktrig==1 then                ; if a widget for 'Num.Harms' or 'Manual On/Off' has been changed...
   chnset    "tableNumber(1)", "table1"    ; ...update table display    
 endif
 
 if gkManualOnOff==1 then            ; if 'Manual On/Off' button is on...
  aL        oscil3    gkamp,gkfrq/400,giwave    ; create an audio oscillator using the GEN33 waveform
  aR        oscil3    gkamp,gkfrq/400,giwave,0.13    ; create an audio oscillator using the GEN33 waveform
         outs    aL,aR        
 endif
 
endin


instr    3    ; midi / keyboard instrument
 iDecTim    chnget    "DecTim"
 iRelTim    chnget    "RelTim"
 
 iNumH    ampmidi    (ftlen(gisource)/3)+1            ; midi velocity mapped to a value between zero (min velocity) and the number of partials in the data table (max velocity)
 
     chnset    int(iNumH),"nh"
 ivel    ampmidi    1                    ; midi velocity mapped to a value in the range zero to 1 
 iwave    ftgentmp    0,0, giTabSize,-33,gisource,iNumH,1,-1    ; create a local and temporary waveform using GEN33 (overlapping notes may need different tables)
     tableicopy    giwave, iwave            ; copy local table to global table at i-time
 
 ktrig    init    1                    ; only update table during the first k period of a note (to conserve resources)
 if  ktrig==1 then
  chnset    "tableNumber(1)", "table1"        ; update table display    
 endif
 ktrig    =    0    

 ; CREATE SOME SOUND
 icps    cpsmidi                        ; midi note number
 aL    oscil3    gkamp,icps/400,iwave    ; create an audio oscilator
 aR    oscil3    gkamp,icps/400,iwave,0.13; create an audio oscilator
 acf    expsegr    10000,iDecTim,   50,iRelTim,50        ; filter cutoff envelope (percussive decay)
 aenv    expsegr    1,    iDecTim,0.001,iRelTim,0.001    ; amplitude envelope
 aL    butlp    aL,acf                    ; filter audio
 aR    butlp    aR,acf                    ; filter audio
 aL    =    aL * (aenv-0.001) * ivel        ; scale amplitude of audio using envelope and midi velocity
 aR    =    aR * (aenv-0.001) * ivel        ; scale amplitude of audio using envelope and midi velocity
     outs    aL, aR
 
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
;f 0 3600
</CsScore>

</CsoundSynthesizer>
