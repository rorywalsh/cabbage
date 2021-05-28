
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; InharmonicAdditiveSynth.csd
; Written by Iain McCurdy, 2012.
; 
; This instrument takes information about modal frequency values for a sound (in the form of a list of values in a GEN 02 
; function table and transfers it into a GEN 09 function table for efficient playback by a single oscillator opcode.
; GEN 09 demands partial numbers to be integers (if we want ot avoid unwanted artefacts in the sound) so all modal 
; frequencies are multiplied by a large number before being rounded to integers  for the GEN 09 table in order to reduce 
; the loss of precision. There is a trade off in that we need this multiplier value to be large to minimise quantisation
; but if it is very large we need to use a higher quality oscillator opcode (more CPU drain) and/or a larger function table
; size for the GEN 09 tables (longer load time when the example is started).
; 
; If the size chosen for the GEN 09 tables is too small, aliasing artefacts will result when it is played back.
; 
; Amplitude Envelope
; ------------------
; Att.    -    Attack time in seconds
; Dec.    -    Decay time in seconds
; Sus.    -    Sustain level
; Rel.    -    Release time
; Level    -    Output amplitude level
; 
; Instrument
; ----------
; Select a GEN09 function table corresponding to a instrument or object
; 
; Filter Envelope (Lowpass)
; -------------------------
; Amount    -    Amount of influence of the envelope (in octaves)
; Dec.    -    Decay time in seconds
; Sus.    -    Sustain level
; Rel.    -    Release time in seconds
; Offset    -    Filter offset value (in octaves)
; 
; Chorus
; ------
; Mix    -    Dry/Wet mix of the chorus effect
; Depth    -    Depth of the modulations of the chorus effect
; Rate    -    Rate of modulation


<Cabbage>
form caption("Inharmonic Synth"), size(445, 285), pluginId("InSy")
image pos(0, 0),                  size(445, 285), colour("black"), shape("rounded"), outlineColour("brown"), line(4)

groupbox bounds(275,10,160, 90), text("Instrument"), plant("instrument"){
combobox channel("Instr"), bounds( 10, 40,140, 25), value(5), text("Bass Guitar", "Dahina", "Banyan", "Xylophone", "Tibetan Bowl 180mm", "Spinel Sphere", "Pot Lid", "Red Cedar Wood Plate", "Tubular Bell", "Redwood Wood Plate", "Douglas Fir Wood Plate", "Uniform Wooden Bar", "Uniform Aluminium Bar", "Vibraphone 1", "Vibraphone 2", "Chladni Plate", "Tibetan Bowl 152mm", "Tibetan Bowl 140mm", "Wine Glass", "Small Handbell", "Albert Clock Bell", "Wood Block","Anvil")
}

groupbox bounds(10, 10,260, 90), text("Amplitude Envelope"), plant("ampenv"){
rslider bounds(  0, 25, 60, 60), text("Att."),  channel("AAtt"), colour("red"), trackerColour("red"), range(0.0001,1.00,0.0001,0.25,0.0001)
rslider bounds( 50, 25, 60, 60), text("Dec."),  channel("ADec"), colour("red"), trackerColour("red"), range(0.001,30.00,7,0.5)
rslider bounds(100, 25, 60, 60), text("Sus."),   channel("ASus"), colour("red"), trackerColour("red"), range(0, 1.00, 0)
rslider bounds(150, 25, 60, 60), text("Rel."),   channel("ARel"), colour("red"), trackerColour("red"), range(0.01, 30.00, 7,0.5)
rslider bounds(200, 25, 60, 60), text("Level"), channel("Lev"),  colour("maroon"), trackerColour("maroon"), range(0, 5.00, 0.1,0.5)
}

groupbox bounds(10,105,260, 90), text("Filter Envelope"), plant("filterenv"){
rslider bounds(  0, 25, 60, 60), text("Amount"),  colour("cornflowerblue"), trackerColour("cornflowerblue"), channel("FAmt"), range(0, 10.0, 8)
rslider bounds( 50, 25, 60, 60), text("Dec."),   colour("cornflowerblue"), trackerColour("cornflowerblue"), channel("FDec"), range(0.001,30.00,7,0.5)
rslider bounds(100, 25, 60, 60), text("Sus."),   colour("cornflowerblue"), trackerColour("cornflowerblue"), channel("FSus"), range(0,1.00,0)
rslider bounds(150, 25, 60, 60), text("Rel."),   colour("cornflowerblue"), trackerColour("cornflowerblue"), channel("FRel"), range(0.1,30.00,7,0.5)
rslider bounds(200, 25, 60, 60), text("Offset"),  colour("cornflowerblue"), trackerColour("cornflowerblue"), channel("F_OS"), range(-4,10.0, 0)
}

groupbox bounds(275,105, 160, 90), text("Chorus"), plant("cho"){
rslider  bounds(  0, 25,60,60), text("Mix"),   channel("ChoMix"), trackerColour("white"), range(0, 1.00, 0.5)
rslider  bounds( 50, 25,60,60), text("Depth"), channel("ChoDep"), trackerColour("white"), range(0, 0.1, 0.01,0.5, 0.001)
rslider  bounds(100, 25,60,60), text("Rate"),  channel("ChoRte"), trackerColour("white"), range(0, 20.00, 0.96, 0.5)
}

keyboard pos(10, 200), size(425, 80)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps         =     32
nchnls         =     2
0dbfs        =    1    ;MAXIMUM AMPLITUDE

massign    0,2                    ;MIDI NOTES TO INSTRUMENT 2
giTableSize    =    131073            ;FUNCTION TABLE SIZE - THIS NEEDS TO BE FAIRLY LARGE IN ORDER TO ACCOMMODATE ACCURATE REPRESENTATION OF UPPER PARTIALS. IF THIS IS TOO LOW ALIASING WILL OCCUR WHEN HIGHER NOTES ARE PLAYED
giRtosScale    =    1000            ;SCALING FACTOR APPLIED TO RATIOS WRITTEN TO GEN09 FUNCTION TABLE. THIS IS BASICALLY A PRECISION CONTROL ON HOW MUCH PARTIAL FREQUENCIES WILL BE QUANTISED (ROUNDED UP OR DOWN) BEFORE BEING WRITTEN TO THE GEN 09 FUNCTION TABLE

;Author: Iain McCurdy (2012)

;FUNCTION TABLES STORING MODAL FREQUENCY RATIOS===================================================================================================================================
;plucked string
girtos1        ftgen    0,0,-20, -2, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20
;dahina
girtos2        ftgen    0,0,-6,-2, 1, 2.89, 4.95, 6.99, 8.01, 9.02
;banyan
girtos3        ftgen    0,0,-6,-2, 1, 2.0, 3.01, 4.01, 4.69, 5.63
;xylophone
girtos4        ftgen    0,0,-6,-2, 1, 3.932, 9.538, 16.688, 24.566, 31.147
;tibetan bowl (180mm)
girtos5        ftgen    0,0,-7,-2, 1, 2.77828, 5.18099, 8.16289, 11.66063, 15.63801, 19.99
;spinel sphere with diameter of 3.6675mm
girtos6        ftgen    0,0,-24,-2, 1, 1.026513174725, 1.4224916858532, 1.4478690202098, 1.4661959580455, 1.499452545408, 1.7891839345101, 1.8768994627782, 1.9645945254541, 1.9786543873113, 2.0334612432847, 2.1452852391916, 2.1561524686621, 2.2533435661294, 2.2905090816065, 2.3331798413917, 2.4567715528268, 2.4925556408289, 2.5661806088514, 2.6055768738808, 2.6692760296751, 2.7140956766436, 2.7543617293425, 2.7710411870043 
;pot lid
girtos7        ftgen    0,0,-6,-2, 1, 3.2, 6.23, 6.27, 9.92, 14.15
;red cedar wood plate
girtos8        ftgen    0,0,-4,-2, 1, 1.47, 2.09, 2.56
;tubular bell
girtos9        ftgen    0,0,-10,-2, 272/437, 538/437, 874/437, 1281/437, 1755/437, 2264/437, 2813/437, 3389/437, 4822/437, 5255/437
;redwood wood plate
girtos10    ftgen    0,0,-4,-2, 1, 1.47, 2.11, 2.57
;douglas fir wood plate
girtos11    ftgen    0,0,-4,-2, 1, 1.42, 2.11, 2.47
;uniform wooden bar
girtos12    ftgen    0,0,-6,-2, 1, 2.572, 4.644, 6.984, 9.723, 12
;uniform aluminum bar
girtos13    ftgen    0,0,-6,-2, 1, 2.756, 5.423, 8.988, 13.448, 18.680
;vibraphone 1
girtos14    ftgen    0,0,-6,-2, 1, 3.984, 10.668, 17.979, 23.679, 33.642
;vibraphone 2
girtos15    ftgen    0,0,-6,-2, 1, 3.997, 9.469, 15.566, 20.863, 29.440
;Chalandi plates
girtos16    ftgen    0,0,-5,-2, 1, 1.72581, 5.80645, 7.41935, 13.91935
;tibetan bowl (152 mm)
girtos17    ftgen    0,0,-7,-2, 1, 2.66242, 4.83757, 7.51592, 10.64012, 14.21019, 18.14027
;tibetan bowl (140 mm)    
girtos18    ftgen    0,0,-5,-2, 1, 2.76515, 5.12121, 7.80681, 10.78409
;wine glass
girtos19    ftgen    0,0,-5,-2, 1, 2.32, 4.25, 6.63, 9.38
;small handbell
girtos20    ftgen    0,0,-22,-2, 1, 1.0019054878049, 1.7936737804878, 1.8009908536585, 2.5201981707317, 2.5224085365854, 2.9907012195122, 2.9940548780488, 3.7855182926829, 3.8061737804878, 4.5689024390244, 4.5754573170732, 5.0296493902439, 5.0455030487805, 6.0759908536585, 5.9094512195122, 6.4124237804878, 6.4430640243902, 7.0826219512195, 7.0923780487805, 7.3188262195122, 7.5551829268293 
;albert clock bell belfast
girtos21    ftgen    0,0,-22,-2, 2.043260,1.482916,1.000000,3.328848,4.761811,1.477056,0.612007,2.661295,1.002793,4.023776,0.254139,2.043916,4.032463,2.659438,4.775560,5.500494,3.331014,0.809697,2.391301, 0.254098,1.901476,2.366563    ;,0.614968,2.046543,1.814887,3.130744,2.484426,0.558874,0.801697,0.070870,3.617036,2.782656
;wood block
girtos22    ftgen    0,0,4,-2,    915/915,1540/915,1863/915,3112/915
;anvil
girtos23    ftgen    0,0,16,-2,    0.783,0.805,1,1.087,1.124,1.241,1.400,1.607,1.824,2.074,2.414,2.530,2.696,3.094,3.562,3.631
;=================================================================================================================================================================================

;=================================================================================================================================================================================
;GEN09 FUNCTION TABLE VERSIONS OF THE MODAL FREQUENCY TABLES
;THESE AREN'T ACTUALLY NEEDED AS THE TABLES ARE CALCULATED IN INSTRUMENT 2 BUT IF INSTRUMENT 2 IS OMITTED THESE TABLES WILL BE NEEDED
;NOTE THAT PARTIAL NUMBER HAVE BEEN MULTIPLIED BY giRtosScale AND THAT THE FREQUENCY OF ANY OSCILLATOR THAT USES THESE TABLES WILL HAVE TO BE DIVIDED BY THE SAME NUMBER 
giwave1        ftgen    0, 0, giTableSize, 9, 1000,1.000,0,     2000,0.667,0,     3000,0.444,0,     4000,0.296,0,     5000,0.198,0,     6000,0.132,0,     7000,0.088,0,     8000,0.059,0,     9000,0.039,0,     10000,0.026,0,     11000,0.017,0,     12000,0.012,0,     13000,0.008,0,     14000,0.005,0,     15000,0.003,0,     16000,0.002,0,     17000,0.002,0,     18000,0.001,0,     19000,0.001,0,     20000,0.000,0,     0,0.000,0,     0,0.000,0
giwave2        ftgen    0, 0, giTableSize, 9, 1000,1.000,0,     2890,0.500,0,     4950,0.250,0,     6990,0.125,0,     8010,0.062,0,     9020,0.031,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0
giwave3        ftgen    0, 0, giTableSize, 9, 1000,1.000,0,     2000,0.667,0,     3010,0.444,0,     4010,0.296,0,     4690,0.198,0,     5630,0.132,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0
giwave4        ftgen    0, 0, giTableSize, 9, 1000,1.000,0,     3932,0.500,0,     9538,0.250,0,     16688,0.125,0,     24566,0.062,0,     31147,0.031,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0
giwave5        ftgen    0, 0, giTableSize, 9, 1000,1.000,0,     2778,0.500,0,     5181,0.250,0,     8163,0.125,0,     11661,0.062,0,     15638,0.031,0,     19990,0.016,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0
giwave6        ftgen    0, 0, giTableSize, 9, 1000,1.000,0,     1027,1.000,0,     1422,1.000,0,     1448,1.000,0,     1466,1.000,0,     1499,1.000,0,     1789,1.000,0,     1877,1.000,0,     1965,1.000,0,     1979,1.000,0,     2033,1.000,0,     2145,1.000,0,     2156,1.000,0,     2253,1.000,0,     2291,1.000,0,     2333,1.000,0,     2457,1.000,0,     2493,1.000,0,     2566,1.000,0,     2606,1.000,0,     2669,1.000,0,     2714,1.000,0
giwave7        ftgen    0, 0, giTableSize, 9, 1000,1.000,0,     3200,0.833,0,     6230,0.694,0,     6270,0.579,0,     9920,0.482,0,     14150,0.402,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0
giwave8        ftgen    0, 0, giTableSize, 9, 1000,1.000,0,     1470,0.667,0,     2090,0.444,0,     2560,0.296,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0
giwave9        ftgen    0, 0, giTableSize, 9, 622,1.000,0,     1231,1.000,0,     2000,1.000,0,     2931,1.000,0,     4016,1.000,0,     5181,1.000,0,     6437,1.000,0,     7755,1.000,0,     11034,1.000,0,     12025,1.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0
giwave10    ftgen    0, 0, giTableSize, 9, 1000,1.000,0,     1470,1.000,0,     2110,1.000,0,     2570,1.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0
giwave11    ftgen    0, 0, giTableSize, 9, 1000,1.000,0,     1420,1.000,0,     2110,1.000,0,     2470,1.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0
giwave12    ftgen    0, 0, giTableSize, 9, 1000,1.000,0,     2572,0.667,0,     4644,0.444,0,     6984,0.296,0,     9723,0.198,0,     0,0.132,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0
giwave13    ftgen    0, 0, giTableSize, 9, 1000,1.000,0,     2756,1.000,0,     5423,1.000,0,     8988,1.000,0,     13448,1.000,0,     18680,1.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0
giwave14    ftgen    0, 0, giTableSize, 9, 1000,1.000,0,     3984,0.500,0,     10668,0.250,0,     17979,0.125,0,     23679,0.062,0,     33642,0.031,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0
giwave15    ftgen    0, 0, giTableSize, 9, 1000,1.000,0,     3997,0.500,0,     9469,0.250,0,     15566,0.125,0,     20863,0.062,0,     29440,0.031,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0
giwave16    ftgen    0, 0, giTableSize, 9, 1000,1.000,0,     1726,0.667,0,     5806,0.444,0,     7419,0.296,0,     13919,0.198,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0
giwave17    ftgen    0, 0, giTableSize, 9, 1000,1.000,0,     2662,0.500,0,     4838,0.250,0,     7516,0.125,0,     10640,0.062,0,     14210,0.031,0,     18140,0.016,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0
giwave18    ftgen    0, 0, giTableSize, 9, 1000,1.000,0,     2765,0.500,0,     5121,0.250,0,     7807,0.125,0,     10784,0.062,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0
giwave19    ftgen    0, 0, giTableSize, 9, 1000,1.000,0,     2320,1.000,0,     4250,1.000,0,     6630,1.000,0,     9380,1.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0
giwave20    ftgen    0, 0, giTableSize, 9, 1000,1.000,0,     1002,0.833,0,     1794,0.694,0,     1801,0.579,0,     2520,0.482,0,     2522,0.402,0,     2991,0.335,0,     2994,0.279,0,     3786,0.233,0,     3806,0.194,0,     4569,0.162,0,     4575,0.135,0,     5030,0.112,0,     5046,0.093,0,     6076,0.078,0,     5909,0.065,0,     6412,0.054,0,     6443,0.045,0,     7083,0.038,0,     7092,0.031,0,     7319,0.026,0,     7555,0.022,0
giwave21    ftgen    0, 0, giTableSize, 9, 2043,1.000,0,     1483,0.909,0,     1000,0.826,0,     3329,0.751,0,     4762,0.683,0,     1477,0.621,0,     612,0.564,0,     2661,0.513,0,     1003,0.467,0,     4024,0.424,0,     254,0.386,0,     2044,0.350,0,     4032,0.319,0,     2659,0.290,0,     4776,0.263,0,     5500,0.239,0,     3331,0.218,0,     810,0.198,0,     2391,0.180,0,     254,0.164,0,     1901,0.149,0,     2367,0.135,0
giwave22    ftgen    0, 0, giTableSize, 9, 1000,1.000,0,     1683,0.909,0,     2036,0.826,0,     3401,0.751,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0,     0,0.000,0
;anvil
giwaves23    ftgen    0, 0, giTableSize, 9, 720,1.413,0, 739,0.891,0, 919,1.000,0, 1000,0.708,0, 1033,1.778,0, 1140,1.259,0, 1287,0.891,0, 1477,1.259,0, 1677,0.794,0, 1907,1.413,0, 2219,1.000,0, 2326,0.316,0, 2479,0.398,0, 2845,0.316,0, 3276,0.178,0, 3339,0.200,0
;=================================================================================================================================================================================

gisine        ftgen    0,0,4096,10,1                ;A SINE WAVE
                                ;FREQUENCY OF AUDIO OSCILLATOR WILL BE DIVIDED BY THIS VALUE
                                ;TOO LOW A VALUE HERE WILL RESULT IN QUANTISATION OF PARTIAL FREQUENCIES
                                ;TOO HIGH A VALUE WILL RESULT IN HIGHER PARTIALS BEING POORLY REPRESENTED IN THE TABLE, LEADING TO ALIASING 
gilfoshape    ftgen    0, 0, 4096, 19, 0.5, 1, 180, 1        ;U-SHAPE PARABOLA (USED BY THE CHORUS EFFECT)
gidurscal    ftgen    0, 0, 128, -16, 3, 128, -4, 0.2        ;A FUNCTION USED TO RESCALE ENVELOPE DURATIONS ACCORDING TO NOTE PLAYED
givelscal    ftgen    0, 0, 128, -16, 0, 128, -4, 1        ;A FUNCTION USED TO REMAP MIDI VELOCITY VALUES - THE CURVATURE OF THIS FUNCTION CAN BE CHANGED TO TAKE ACCOUNT OF THE VARYING VELOCITY RESPONSE OF DIFFERENT MIDI KEYBOARDS
gasend        init    0

instr    CreateGEN09Tables    ;DERIVE GEN09 FUNCTION TABLE WAVEFORMS FROM THE GEN02 FUNCTION TABLES OF MODAL FREQUENCY RATIOS
;A MACRO IS DEFINED THAT READS A SINGLE MODAL FREQUENCY RATIO FROM A TABLE 

#define    PARTIAL(WaveNum'PartNum)
    #
    if $PartNum<=inratios then                ;IF COUNTER IS WITHIN THE LIMITS OF THE NUMBER OF DEFINED MODAL FREQUENCY RATIOS...
     irto$PartNum    table    $PartNum-1,girtos$WaveNum    ;...READ A VALUE FROM THE TABLE
     irto$PartNum    =    round(irto$PartNum*giRtosScale)    ;MULTIPLY RATIO BY giRtosScale THEN INTEGERISE BYT ROUND TO THE NEAREST WHOLE NUMBER
     iamp$PartNum    =    iamp/(iampscal^($PartNum-1))    ;DERIVE AMPLITUDE VALUE FOR THIS PARTIAL
    else                            ;IF COUNTER IS BEYOND THE LIMITS OF THE NUMBER OF DEFINED MODAL FREQUENCY RATIOS...
     irto$PartNum    =    0                ;SET RATIO TO ZERO
     iamp$PartNum    =    0                ;SET PARTIAL STRENGTH TO ZERO
    endif
    #

;A MACRO IS DEFINED THAT CREATES A GEN09 TABLE FROM MODAL FREQUENCY DATA 
#define    WAVEFORM(WaveNum'AmpScal)
    #
    inratios    =    ftlen(girtos$WaveNum)
    iampscal    =    $AmpScal            ;AMPLITUDE SCALING 
    iamp        =    1                ;PARTIAL STRENGTH OF FIRST PARTIAL
    $PARTIAL($WaveNum'1)                        ;MACRO EXPANDED FOR EACH PARTIAL...
    $PARTIAL($WaveNum'2)
    $PARTIAL($WaveNum'3)
    $PARTIAL($WaveNum'4)
    $PARTIAL($WaveNum'5)
    $PARTIAL($WaveNum'6)
    $PARTIAL($WaveNum'7)
    $PARTIAL($WaveNum'8)
    $PARTIAL($WaveNum'9)
    $PARTIAL($WaveNum'10)
    $PARTIAL($WaveNum'11)
    $PARTIAL($WaveNum'12)                          
    $PARTIAL($WaveNum'13)
    $PARTIAL($WaveNum'14)
    $PARTIAL($WaveNum'15)
    $PARTIAL($WaveNum'16)
    $PARTIAL($WaveNum'17)
    $PARTIAL($WaveNum'18)                                                                                                                                                         
    $PARTIAL($WaveNum'19)
    $PARTIAL($WaveNum'20)
    $PARTIAL($WaveNum'21)
    $PARTIAL($WaveNum'22)                                                                                                                                                            
    ;GENERATE A GEN09 FUNCTION TABLE (ALL PHASES ARE SET TO ZERO)
    giwave$WaveNum    ftgen    0,0,giTableSize,9, irto1,iamp1,0, irto2,iamp2,0, irto3,iamp3,0, irto4,iamp4,0, irto5,iamp5,0, irto6,iamp6,0, irto7,iamp7,0, irto8,iamp8,0, irto9,iamp9,0, irto10,iamp10,0, irto11,iamp11,0, irto12,iamp12,0, irto13,iamp13,0, irto14,iamp14,0, irto15,iamp15,0, irto16,iamp16,0, irto17,iamp17,0, irto18,iamp18,0, irto19,iamp19,0, irto20,iamp20,0, irto21,iamp21,0, irto22,iamp22,0     

    ;UNCOMMENT THE FOLLOWING LINE IF YOU WISH TO WRITE GENERATED TABLES TO A TEXT FILE FOR LATER USE
    ;fprints "GEN09InharmonicTables.txt", "giwave$WaveNum%tftgen%t0, 0, giTableSize, 9, %d,%5.3f,0,%t %d,%5.3f,0,%t %d,%5.3f,0,%t %d,%5.3f,0,%t %d,%5.3f,0,%t %d,%5.3f,0,%t %d,%5.3f,0,%t %d,%5.3f,0,%t %d,%5.3f,0,%t %d,%5.3f,0,%t %d,%5.3f,0,%t %d,%5.3f,0,%t %d,%5.3f,0,%t %d,%5.3f,0,%t %d,%5.3f,0,%t %d,%5.3f,0,%t %d,%5.3f,0,%t %d,%5.3f,0,%t %d,%5.3f,0,%t %d,%5.3f,0,%t %d,%5.3f,0,%t %d,%5.3f,0%n", irto1,iamp1, irto2,iamp2, irto3,iamp3, irto4,iamp4, irto5,iamp5, irto6,iamp6, irto7,iamp7, irto8,iamp8, irto9,iamp9, irto10,iamp10, irto11,iamp11, irto12,iamp12, irto13,iamp13, irto14,iamp14, irto15,iamp15, irto16,iamp16, irto17,iamp17, irto18,iamp18, irto19,iamp19, irto20,iamp20, irto21,iamp21, irto22,iamp22
    #    
    ;EXPAND MACRO FOR EACH WAVEFORM TO BE CREATED
    ;ARG1 = A COUNTER, SHOULD CORRESPOND TO NUMBER APPENDED TO 'girtos' FOR THE TABLE HANDLE OF THE REQUIRED MODAL FREQUENCIES TABLE
    ;ARG2 = PARTIAL STRENGTHS SCALING: 1=FLAT >1=HIGHER PARTIALS ARE INCREASINGLY ATTENUATED
    $WAVEFORM(1' 1.5)
    $WAVEFORM(2' 2)                                                                                  
    $WAVEFORM(3' 1.5)
    $WAVEFORM(4' 2)
    $WAVEFORM(5' 2)
    $WAVEFORM(6' 1)
    $WAVEFORM(7' 1.2)
    $WAVEFORM(8' 1.5)
    $WAVEFORM(9' 1)          
    $WAVEFORM(10' 1)
    $WAVEFORM(11' 1)
    $WAVEFORM(12' 1.5)                                       
    $WAVEFORM(13' 1)
    $WAVEFORM(14' 2)
    $WAVEFORM(15' 2)    
    $WAVEFORM(16' 1.5)
    $WAVEFORM(17' 2)
    $WAVEFORM(18' 2)
    $WAVEFORM(19' 1)
    $WAVEFORM(20' 1.2)
    $WAVEFORM(21' 1.1)
    $WAVEFORM(22' 1.1)
endin


instr    1    ; Read in widgets
    gkInstr    chnget    "Instr"
    gkAAtt    chnget    "AAtt"
    gkADec    chnget    "ADec"
    gkASus    chnget    "ASus"
    gkARel    chnget    "ARel"
    gkLev    chnget    "Lev"
    gkFDec    chnget    "FDec"
    gkFSus    chnget    "FSus"
    gkFRel    chnget    "FRel"
    gkFAmt    chnget    "FAmt"
    gkF_OS    chnget    "F_OS"
endin

instr    2    ;SOUND GENERATING INSTRUMENT

    icps    cpsmidi                        ;READ CPS VALUE IN FROM MIDI KEYBOARD
    inum    notnum                        ;READ MIDI NOTE NUMBER
    ivel    veloc    0,1                    ;READ MIDI VELOCITY
    ivel    table    ivel,givelscal,1            ;REMAP MIDI VELOCITY - SEE TABLE givelscal ABOVE
    iscale    table    inum,gidurscal                ;DURATIONAL RESCALING ACCORDING TO NOTE PLAYED - HIGHER NOTE WILL DECAY QUICKER IN REFLECTION OF REAL-WORLD CHARACTERISTICS
    
    iAAtt    =        i(gkAAtt)*iscale
    iADec    =        i(gkADec)*iscale
    iARel    =        i(gkARel)*iscale
    kenv    linsegr        0,iAAtt,1,iADec,i(gkASus),iARel,0    ;AMPLITUDE ENVELOPE WITH MIDI SENSING RELEASE SEGEMENT
    kenv    expcurve    kenv,64
    aenv    interp        kenv
    asig    poscil3        gkLev*ivel*aenv*0.8,icps/giRtosScale,giwave1+i(gkInstr)-1                ;AUDIO OSCILLATOR poscil3 USED FOR IMPROVED FIDELITY OVER OTHER OSCILLATORS. FREQUENCY IS SCALED DOWN ACCORIND TO THE VALUE OF giRtosScale DEFINED IN THE HEADER
    
    iFDec    =        i(gkFDec)*iscale
    iFSus    =        i(gkFSus)
    iFRel    =        i(gkFRel)*iscale
    iFRel    =        iFRel > iARel ? iARel : iFRel
    kcf_env    linsegr        1,iFDec,iFSus,iFRel,0
    kcf_env    expcurve    kcf_env,8    
    kcf_oct    limit    octcps(icps*0.25) + gkF_OS + (gkFAmt*kcf_env*ivel), 4, 14
    ;asig    clfilt        asig, cpsoct(kcf_oct), 0, 2                    ;LOW PASS FILTER THE SOUND (SUBTRACTIVE SYNTHESIS)
    asig    tone        asig, cpsoct(kcf_oct)                        ;LOW PASS FILTER THE SOUND (SUBTRACTIVE SYNTHESIS)
    gasend    =    gasend+asig

endin

instr    3    ; Chorus Effect
    gkChoMix    chnget    "ChoMix"
    gkChoRte    chnget    "ChoRte"
    gkChoDep    chnget    "ChoDep"

    if gkChoMix==0 kgoto SKIP_CHORUS
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
        outs    achoL*gkChoMix, achoR*gkChoMix
    SKIP_CHORUS:

        outs        gasend, gasend    ;SEND UN-CHORUSSED AUDIO TO THE OUTPUTS
        clear    gasend
endin



instr    1000    ; Preset (change sliders when a new instrument is selected)
 ktrig    changed    gkInstr
; gkInstr    init    5
; ktrig        init    1
 if ktrig==1 then
  reinit SetPreset
  SetPreset:
  #define PRESET(N'AAtt'ADec'ASus'ARel'FAmt'FDec'FSus'FRel'F_OS'ChoMix'ChoDep'ChoRte)
  #
  if i(gkInstr)==$N then
   chnset    $AAtt, "AAtt"  
   chnset    $ADec, "ADec"  
   chnset    $ASus, "ASus"  
   chnset    $ARel, "ARel"  
   chnset    $FAmt, "FAmt"  
   chnset    $FDec, "FDec"  
   chnset    $FSus, "FSus"  
   chnset    $FRel, "FRel"  
   chnset    $F_OS, "F_OS"  
   chnset    $ChoMix, "ChoMix"  
   chnset    $ChoDep, "ChoDep"  
   chnset    $ChoRte, "ChoRte"
  endif
  #
  ;      (N 'AAtt  'ADec'ASus'ARel'FAmt'FDec'FSus'FRel'F_OS'ChoMix'ChoDep'ChoRte)
  $PRESET(1 '0.0001'16.0'0   '0.60'4.8 '2.01'0   '0.60'1   '0.5   '0.01  '0.96)    ; plucked string
  $PRESET(2 '0.0001'1.70'0   '1.70'2.02'0.27'0   '0.27'0   '0.5   '0.01  '0.96)    ; dahina
  $PRESET(3 '0.0001'0.45'0   '0.45'0.7 '0.45'0   '0.45'0   '0.5   '0.01  '0.96)    ; banyan
  $PRESET(4 '0.0050'1.90'0   '1.90'3.5 '1.27'0   '1.27'-2  '0.5   '0.01  '0.96)    ; xylophone
  $PRESET(5 '0.0001'15.0'0   '15.0'8   '3.00'0   '3.00'0.3 '0.5   '0.065 '0.13)    ; tibetan bowl (180mm)
  $PRESET(6 '0.0001'0.26'0   '0.26'8   '0.05'0   '0.05'-4  '0.5   '0.01  '0.96)    ; spinel sphere with diameter of 3.6675mm
  $PRESET(7 '0.0001'7   '0   '7   '7.6 '7   '0   '7   '-4  '0.5   '0.01  '0.96)    ; pot lid
  $PRESET(8 '0.0001'0.70'0   '0.70'8   '0.47'0   '0.47'-4  '0.5   '0.01  '0.96)    ; red cedar wood plate
  $PRESET(9 '0.0001'15.0'0   '15.0'8   '6.00'0   '6.00'2   '0.5   '0.01  '0.96)    ; tubular bell
  $PRESET(10'0.0001'0.70'0   '0.70'8   '0.47'0   '0.47'-4  '0.5   '0.01  '0.96)    ; redwood wood plate
  $PRESET(11'0.0001'0.70'0   '0.70'8   '0.47'0   '0.47'-4  '0.5   '0.01  '0.96)    ; douglas fir wood plate
  $PRESET(12'0.0001'0.70'0   '0.70'8   '0.47'0   '0.47'-4  '0.5   '0.01  '0.96)    ; uniform wooden bar
  $PRESET(13'0.0001'15.0'0   '15.0'8   '6.00'0   '6.00'2   '0.5   '0.065 '0.13)    ; uniform aluminum bar
  $PRESET(14'0.01  '15.0'0   '15.0'8.6 '6.80'0   '6.80'-4  '0.5   '0.065 '0.13)    ; vibraphone 1
  $PRESET(15'0.01  '15.0'0   '15.0'8.6 '6.80'0   '6.80'-4  '0.5   '0.065 '0.13)    ; vibraphone 2
  $PRESET(16'0.0001'3.2 '0   '3.2 '5.6 '2.90'0   '2.90'-4  '0.5   '0.01  '0.96)    ; Chladni plate
  $PRESET(17'0.0001'15.0'0   '15.0'8   '3.00'0   '3.00'0.3 '0.5   '0.065 '0.13)    ; tibetan bowl (152 mm)
  $PRESET(18'0.0001'15.0'0   '15.0'8   '3.00'0   '3.00'0.3 '0.5   '0.065 '0.13)    ; tibetan bowl (140 mm)
  $PRESET(19'0.0001'7   '0   '7   '5.6 '7   '0   '7   '-4  '0.5   '0.01  '0.96)    ; wine glass
  $PRESET(20'0.0001'15  '0   '15  '5.6 '12  '0   '12  '-4  '0.5   '0.01  '0.96)    ; small handbell
  $PRESET(21'0.0001'15.0'0   '15.0'8   '6.00'0   '6.00'2   '0.5   '0.01  '0.96)    ; albert clock bell belfast
  $PRESET(22'0.0001'0.26'0   '0.26'8   '0.05'0   '0.05'-4  '0.5   '0.01  '0.96)    ; wood block
  rireturn
 endif
endin


</CsInstruments>

<CsScore>
;i "CreateGEN09Tables" 0 0
i 1 0 [60*60*24*7]
i 3 0 [60*60*24*7]
i 1000 0.1 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>