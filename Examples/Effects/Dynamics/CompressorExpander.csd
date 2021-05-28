
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Compressor/Expander.csd
; Written by Iain McCurdy, 2015.

; A dynamics processor
; Read point on the transfer function is indicated by a moving dot.

; Threshold -   threshold point at which the main dynamics processing curve begins
; Compress  -   degree of dynamic compression of the dynamic area beneath the threshold
; Curve     -   curve of the main dynamics processing segment
; Ceiling   -   dynamic ceiling. Shifts main dynamic curve to the left in the transfer function.
; Attack    -   Attack time of the dynamic tracker. Increase to smooth out dynamic changes carried out.
; Release   -   Release time of the dynamic tracker. Increase to smooth out dynamic changes carried out.
; Smoothing -   A lowpass filter is applied to the output gain function. increasing 'Smoothing' will smooth changes in dynamics.
;           This is in addition to the attack and release parameters used in the follow2 opcode but may produce nicer smoothing.
; Pre-gain  -   Boost the input signal before dynamic tracking takes place. 
;           This does not affect the output level but merely forces reading further across the transfer function - this is confirmed by observing the dot on the graph. 
; Sensitivity   -   Adjusts the range of dynamic change the effect will apply (without having to adjust the transfer function.  
; Mix       -   Dry/wet mix. This should normally be set at maximum '1' but switching back to '0' (dry) may be informative in auditioning changes.  
; Level     -   Output level

<Cabbage>
form caption("Compressor/Expander") size(890, 100), pluginId("CoEx"), guiRefresh(16)
image                       bounds(0, 0, 890, 100), colour("tan"), shape("rounded"), outlineColour("white"), outlineThickness(6)
rslider  bounds( 10, 13, 75, 75), text("Threshold"), channel("thresh"), range(0,1,0.143,0.5,0.001), markerColour("white"),     textColour("black"), colour(37,59,59), trackerColour("Silver")
rslider  bounds( 80, 13, 75, 75), text("Compress"),  channel("comp"),   range(0,1,0.25), markerColour("white"),               textColour("black"), colour(37,59,59), trackerColour("Silver")
rslider  bounds(150, 13, 75, 75), text("Curve"),     channel("curve"),  range(-20,20,2.6), markerColour("white"),             textColour("black"), colour(37,59,59), trackerColour("Silver")
rslider  bounds(220, 13, 75, 75), text("Ceiling"),   channel("ceil"),   range(0,1,0), markerColour("white"),                 textColour("black"), colour(37,59,59), trackerColour("Silver")
gentable bounds(300, 10, 80, 80), tableNumber(1), tableColour("silver"), fill(0), identChannel("table"), ampRange(0,1,-1), zoom(-1), tableGridColour(100,100,100,100), tableBackgroundColour("white"), outlineThickness(2)
image    bounds(0, 0, 6, 6), colour("Red"), shape("ellipse"), outlineThickness(0), identChannel("Dot")
rslider  bounds(385, 13, 75, 75), text("Attack"),    channel("att"),    range(0.01,1,0.3,0.5,0.01), markerColour("white"),   textColour("black"), colour(37,59,59), trackerColour("Silver")
rslider  bounds(455, 13, 75, 75), text("Release"),   channel("rel"),    range(0.01,1,0.3,0.5,0.01), markerColour("white"),   textColour("black"), colour(37,59,59), trackerColour("Silver")
rslider  bounds(525, 13, 75, 75), text("Smoothing"), channel("smooth"), range(0.01,1,0.1,0.5,0.001), markerColour("white"),  textColour("black"), colour(37,59,59), trackerColour("Silver")
rslider  bounds(595, 13, 75, 75), text("Pre-gain"),  channel("PreGain"),range(0.5,10,2.4,0.5,0.001), markerColour("white"),  textColour("black"), colour(37,59,59), trackerColour("Silver")
rslider  bounds(665, 13, 75, 75), text("Sensitivity"),channel("sense"), range(1,10,3), markerColour("white"),               textColour("black"), colour(37,59,59), trackerColour("Silver")
rslider  bounds(735, 13, 75, 75), text("Mix"),       channel("mix"),    range(0,1,1), markerColour("white"),                 textColour("black"), colour(37,59,59), trackerColour("Silver")
rslider  bounds(805, 13, 75, 75), text("Level"),     channel("lev"),    range(0,5,0.5,0.5,0.001), markerColour("white"),     textColour("black"), colour(37,59,59), trackerColour("Silver")

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n
</CsOptions>

<CsInstruments>
;sr is set by the host
ksmps  =  32
nchnls =  2
0dbfs  =  1


instr 1
 kthresh    chnget     "thresh"
 kcomp      chnget     "comp"
 kcurve     chnget     "curve"
 kceil      chnget     "ceil"
 katt       chnget     "att"
 krel       chnget     "rel"
 ksmooth    chnget     "smooth"
 kPreGain   chnget     "PreGain"
 ksense     chnget     "sense" 
 kmix       chnget     "mix"
 klev       chnget     "lev"
 
 kpeg       metro      2                                                         ; maximum rate at which table will be rebuilt upon receipt of changes to Threshold, Compress, Curve or Ceiling
 if kpeg==1 then                                                       
  if changed(kthresh,kcomp,kcurve,kceil)==1 then
   reinit REBUILD_TABLE
  endif
 endif
 REBUILD_TABLE:
 icurve     =          i(kcurve)
 ithresh    =          i(kthresh)
 iTabLen    =          4096
 ithreshi   =          int(iTabLen * ithresh)                                    ; threshold as a table point x value
 irem       =          iTabLen - ithreshi                                        ; remainder table points
 iceil      =          i(kceil)
 irem1      =          int(irem * (1 - iceil))
 irem2      =          irem - irem1
 icomp      =          i(kcomp)
 giTransferFunc ftgen 1, 0, iTabLen, 16, 0, ithreshi, 0, ithresh*icomp, irem1, icurve, 1, irem2, 0, 1
            chnset     "tableNumber(1)","table"
 
 aL,aR      ins
 ;aL,aR      diskin2    "ClassicalGuitar.wav", 1, 0, 1
 
 aFollow    follow2    (aL + aR) * kPreGain, katt, krel
 ;aFollow    pow        aFollow, 0.5
 kRMS       downsamp   aFollow

 /* apply transfer function */
 aMap       table      aFollow, giTransferFunc, 1                                ; read transfer function value
 aDiff      =          1 + (aMap - aFollow)                                      ; divergence from flat dynamic response 
 aDiff      pow        aDiff, ksense                                             ; raising to a power greater than 1 increases range of change: greater attenuation, greater amplification
 aDiff      tone       aDiff, 1/ksmooth                                          ; smoothing. This is in addition to the attack and release parameters used in the follow2 opcode but may produce nicer smoothing. Reduce this value for greater smoothing, increase it for less.
 aDynL      =          aL * aDiff                                                ; apply dynamic change
 aDynR      =          aR * aDiff
 aMixL      ntrpol     aL, aDynL, kmix                                           ; dry / wet mix
 aMixR      ntrpol     aR, aDynR, kmix
            outs       aMixL * klev, aMixR * klev                                ; send audio to outputs

 /* DISPLAY */
 if metro(ksmps*0.5)==1 then
  kx        limit      kRMS,0,1                                                  ; limit x position
  ky        table      kx,giTransferFunc,1                                       ; derice y position (0 to 1)
  Smsg      sprintfk   "pos(%d,%d)", 297 + int(kx * 80), 7 + (80 - int(ky * 80)) ; create message string
            chnset     Smsg,"Dot"                                                ; send message string to gentable widget
 endif

endin

</CsInstruments>

<CsScore>                                              
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>