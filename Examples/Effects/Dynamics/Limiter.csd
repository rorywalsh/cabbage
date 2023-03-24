
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Limiter.csd
; Written by Iain McCurdy, 2016.

; A simple 'brick-wall' limiter

; Threshold -   dB threshold below above which signals will be aggressively limited
; Smoothing -   response of the amplitude follower. Higher values result in a slower, but possibly smoother, response
; Delay     -   delay applied to the input sound before it is limited (the tracked signal is always undelayed)
;               This can be useful for compensating for a limiter than is not responding fast enough to sudden dynamic transients 
; Gain      -   Make up gain. Useful for compensating for gain loss.
   
<Cabbage>
#define SLIDER_APPEARANCE textColour("black"), trackerColour("LightSlateGrey"), valueTextBox(1), fontColour("black"), popupText(0)
form caption("Limiter") size(435,105), pluginId("lmtr")
image         bounds(  0,  0,435,105), outlineThickness(6), outlineColour("white"), colour("silver")
rslider  bounds( 10, 5, 80, 90), channel("thresh"), text("Threshold [dB]"), range(-120,0,-24), $SLIDER_APPEARANCE
rslider  bounds( 90, 5, 80, 90), channel("smooth"), text("Smoothing"), range(0.01,1,0.1,0.5), $SLIDER_APPEARANCE
rslider  bounds(170, 5, 80, 90), channel("delay"), text("Delay [s]"), range(0,0.2,0,0.5), $SLIDER_APPEARANCE
rslider  bounds(250, 5, 80, 90), channel("gain"), text("Gain [dB]"), range(-48,48,0), $SLIDER_APPEARANCE
checkbox bounds(335,40,100, 20), channel("limiting"), text("Limiting"), shape("ellipse"), colour("red"), fontColour:0("black"), fontColour:1("black"), active(0)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

;sr is set by host
ksmps  =  32
nchnls =  2
0dbfs  =  1

; Author: Iain McCurdy (2016)

instr 1
 aL,aR       ins                                 ; read live audio in
 kthresh     chnget      "thresh"                ; read in widgets
 ksmooth     chnget      "smooth"                ; this is needed as an i-time variable so will have to be cast as an i variable and a reinitialisation forced
 ksmooth     init        0.1
 kthresh     =           ampdbfs(kthresh)        ; convert threshold to an amplitude value
 if changed(ksmooth)==1 then                     ; if Smoothing slider is moved...
  reinit REINIT                                  ; ... force a reinitialisation
 endif
 REINIT:                                         ; reinitialise from here
 krmsL       rms         aL, 1 / i(ksmooth)      ; scan both channels
 krmsR       rms         aR, 1 / i(ksmooth)      ; ...
 rireturn                                        ; return to performance pass from reinitialisation pass (if applicable)
 krms        max         krmsL,krmsR             ; but only used the highest rms

 kdelay      chnget      "delay"             
 if kdelay>0 then                                ; if Delay value is anything above zero ...
  aL         vdelay      aL, kdelay * 1000, 200  ; delay audio signals before limiting
  aR         vdelay      aR, kdelay * 1000, 200
 endif

 if krms>kthresh then                            ; if current RMS is above threshold; i.e. limiting required
  kfctr      =           kthresh/krms            ; derive less than '1' factor required to attenuate audio signal to limiting value
  afctr      interp      kfctr                   ; smooth changes (and interpolate from k to a)
  aL_L       =           aL * afctr              ; apply scaling factor
  aL_R       =           aR * afctr
  klimiting  =           1                       ; switch value used by GUI indicator (on)
 else
  aL_L       =           aL                      ; pass audio signals unchanged
  aL_R       =           aR                      ; ...
  klimiting  =           0                       ; switch value used by GUI indicator (off)
 endif

 kgain      chnget       "gain"                  ; make up gain control
 kgain      =            ampdb(kgain)            ; derive gain value as an amplitude factor
 aL_L       *=           kgain                   ; make up gain
 aL_R       *=           kgain

 if metro(16)==1 then                            ; peg rate if updates of limiting indicator (to save a bit of CPU)
            chnset       klimiting,"limiting"    ; send value for limiting indicator
 endif

            outs         aL_L, aL_R              ; send limited audio signals to outputs
endin                              

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>