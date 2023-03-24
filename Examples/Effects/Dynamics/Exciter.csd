
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Exciter.csd
; Written by Iain McCurdy, 2015.
                
; An exciter is a device that uses harmonic distortion to add a high frequency shimmer to a signal. It is a common tool in mastering.
; Csound includes an opcode called 'exciter' which is an implementation of the 'Calf' exciter plugin. 
; 'Frequency' and 'Ceiling' define the frequency range across which harmonics will be created.      
; We also have control over the amount of harmonics created and the blend between the 2nd and 3rd order harmonics. 
; The effect of these parameters is subtle and the user might find it useful to at first set 'Dry/Wet Mix' to maximum (100% wet) in order to hear the effect more clearly.
             
<Cabbage>
form caption("Exciter") size(445, 100), pluginId("exci")
image           bounds(0, 0, 445, 100), colour("LightSlateGrey"), shape("rounded"), outlineColour("white"), outlineThickness(6)
rslider  bounds( 10, 13, 75, 75), text("Frequency"), channel("freq"), range(20,20000,3000,0.5), markerColour("white"),  textColour("white"), colour(37,59,59), trackerColour("Silver")
rslider  bounds( 80, 13, 75, 75), text("Ceiling"), channel("ceil"), range(20,20000,20000,0.5), markerColour("white"), textColour("white"), colour(37,59,59), trackerColour("Silver")
rslider  bounds(150, 13, 75, 75), text("Harmonics"), channel("harms"), range(0.1,10.00,10), markerColour("white"),     textColour("white"), colour(37,59,59), trackerColour("Silver")
rslider  bounds(220, 13, 75, 75), text("Blend"), channel("blend"), range(-10,10,10,0.5,0.0001), markerColour("white"),     textColour("white"), colour(37,59,59), trackerColour("Silver")
rslider  bounds(290, 13, 75, 75), text("Mix"), channel("mix"), range(0, 1.00, 0.5), markerColour("white"),      textColour("white"), colour(37,59,59), trackerColour("Silver")
rslider  bounds(360, 13, 75, 75), text("Level"), channel("level"), range(0,20.00, 0.5,0.5), markerColour("white"),      textColour("white"), colour(37,59,59), trackerColour("Silver")
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

; sr set by host
ksmps  =  32
nchnls =  2
0dbfs  =  1

instr 1
 kporttime  linseg    0, 0.001, 0.05                      ; ramping up value used for portamento time                                                   
 kfreq      chnget    "freq"                              ; read in widgets
 kceil      chnget    "ceil"
 kharms     chnget    "harms"
 kblend     chnget    "blend"
 klevel     chnget    "level"
 kmix       chnget    "mix"
 kmix       portk     kmix, kporttime
 klevel     portk     klevel, kporttime
 
 a1,a2      ins
; a1,a2      diskin2  "ClassicalGuitar.wav",1,0,1         ; test with a sound file
 
 aE1        exciter    a1, kfreq, kceil, kharms, kblend
 aE2        exciter    a1, kfreq, kceil, kharms, kblend

 a1         ntrpol     a1, aE1, kmix                      ; dry/wet mix
 a2         ntrpol     a2, aE2, kmix
            outs       a1*klevel, a2*klevel               ; send to outputs and apply amplitude level control
endin

</CsInstruments>

<CsScore>                                              
i 1 0 [60*60*24*7]                                        ; play note for a week
</CsScore>

</CsoundSynthesizer>