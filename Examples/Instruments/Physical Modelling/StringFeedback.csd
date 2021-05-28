
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; StringFeedback.csd - an emulation of a feeding back vibrating string
; Written by Iain McCurdy, 2013
; ------------------
; 
; String
; ---------
; Feedback -   signal that is fed back into the vibrating string model. If a particular note doesn't sound, boost this!
; Drive    -   overdrive level (on each individual string, so no interference sidebands will be produced when chords are played, see 'Gain' in the 'Output' section for this effect)
; Sustain  -   sustain of the string vibrations. Inverse of damping.
; Bright   -   brightness of the signal (lowpass filtering). Uses keyboard tracking, value stated is a ratio of the fundemental of the note played.
; Cut      -   remove lower partials of the signal (highpass filtering). Uses keyboard tracking, value stated is a ratio of the fundemental of the note played.
; 
; Movement
; ---------
; Movement is a rough imitation of the effect of moving the vibrating string with respect to the location of the speaker. This is simply a modulating very short delay.
; Speed     -   speed of modulation (created using a random spline)
; 
; Pick-up
; -------
; Settings pertaining to the location of the pick-up along the length of the vibrating string
; Position  -   manual positioning of the pick-up. 0 or 1 = either end. 0.5 = half-way along the string's length.
; Auto      -   if this is active, manual positioning is ignored and the pick-up moves around randomly
; Speed     -   speed of random movement of the pick-up
; 
; Reverb
; ------
; Distance  -   Distance of the listener from the speaker (basically a dry/wet signal crossfader)
; Room      -   Room size
; 
; Output
; ------
; FB.Rel.   -   release time for the feedback signal
; Str.Rel.  -   release time for the string output signal
; Gain      -   gain of a global overdrive applied to a mix of all playing notes. This will create the characteristic 'growling' distortion effect when, in particular, 4ths and 5ths are played.
; Level     -   Level control (pre reverb so reverberation will always die away natuarally)

; Whammy    -   this is am emulation of the whammy bar on an electric guitar. 
;               The whammy control has a different range on different notes so intervals become warped as the whammy control is moved.
;               When released it springs back to unison.

<Cabbage>
form caption("String Feedback"), size(885, 215), pluginId("fbck"), colour(0,0,0), guiRefresh(4)

groupbox bounds(  0,  0, 290, 90), text("String"), fontColour(195,126, 0), plant("String") {
rslider bounds(  5, 25, 60, 60), text("Feedback"),  colour(195,126,  0), FontColour(195,126,  0), channel("fback"),   range(0, 8, 0.8,0.5)
rslider bounds( 60, 25, 60, 60), text("Drive"),   colour(195,126,  0), FontColour(195,126,  0), channel("drive"),   range(0, 1, 0.25)
rslider bounds(115, 25, 60, 60), text("Sustain"), colour(195,126,  0), FontColour(195,126,  0), channel("sustain"),   range(0.0001, 1, 1, 0.5, 0.0001)
rslider bounds(170, 25, 60, 60), text("Bright"),  colour(195,126,  0), FontColour(195,126,  0), channel("LPF"),   range(1, 32, 16)
rslider bounds(225, 25, 60, 60), text("Cut"),     colour(195,126,  0), FontColour(195,126,  0), channel("HPF"),   range(0, 32, 1)
}

groupbox bounds(290,  0,  70, 90), text("Movement"), fontColour(195,126, 0), plant("Movement") {
rslider  bounds(  5, 25, 60, 60), text("Speed"),   colour(195,126,  0), FontColour(195,126,  0), channel("speed"),   range(0.001,1, 0.1,0.5,0.0001)
}

groupbox bounds(360,  0,170, 90), text("Pick-up"), fontColour(195,126, 0), plant("Pickup") {
rslider bounds(  5, 25, 60, 60), text("Position"),colour(195,126,  0), FontColour(195,126,  0), channel("PickPos"),   range(0, 1, 0.1)
checkbox bounds( 65, 25, 70, 15), text("Auto") channel("auto"), FontColour(195,126,  0), colour("orange")
rslider bounds(105, 25, 60, 60), text("Speed"),colour(195,126,  0), FontColour(195,126,  0), channel("PickPosSpeed"),   range(0.001, 8, 1, 0.5,0.001)
}

groupbox bounds(530,  0,120, 90), text("Reverb"), fontColour(195,126, 0), plant("Reverb") {
rslider bounds(  5, 25, 60, 60), text("Distance"),colour(195,126,  0), FontColour(195,126,  0), channel("distance"),   range(0, 1, 0.1)
rslider bounds( 60, 25, 60, 60), text("Room"),    colour(195,126,  0), FontColour(195,126,  0), channel("room"),   range(0.5,0.99, 0.85)
}

groupbox bounds(650,  0,235, 90), text("Output"), fontColour(195,126, 0), plant("Output") {
rslider bounds(  5, 25, 60, 60), text("FB.Rel."),  colour(195,126,  0), FontColour(195,126,  0), channel("rel"),     range(0.01, 8, 0.01, 0.5)
rslider bounds( 60, 25, 60, 60), text("Str.Rel."), colour(195,126,  0), FontColour(195,126,  0), channel("StrRel"),  range(0.1, 15, 5, 0.5)
rslider bounds(115, 25, 60, 60), text("Gain"),     colour(195,126,  0), FontColour(195,126,  0), channel("Gain"),    range(1, 10, 1)
rslider bounds(170, 25, 60, 60), text("Level"),    colour(195,126,  0), FontColour(195,126,  0), channel("level"),   range(0, 1, 0.5)
}

keyboard bounds(0,  95,885,80)
hslider  bounds(0,180,888,30), text("Whammy"), range(-48,48,0), channel("Bend")

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>


<CsInstruments>

; sr set by host
ksmps       =    64
nchnls       =    2
0dbfs      =   1

massign      0,1      ; assign all midi to channel 1
      seed   0   ; random number generators seeded from system clock

gasendL, gasendR, garvbL, garvbR   init   0                    ; initialise global variables to zero
giscal    ftgen   0, 0, 1024, -16, 1, 1024, 4, 8               ; amplitude scaling used in 'overdrive' mechanism
gifbscl   ftgen   0, 0, 128, -27, 0, 1, 36, 1, 86, 3, 127, 3   ; Feedback scaling function table. Higher notes need higher levels of feedback.

opcode StringPickUp, a, aiiikkk
setksmps 1
aIn, iPlk, iAmp, iFund, kPickup, kFB, kBend   xin
aPlk    linseg   0, (1/iFund)*iPlk, 1, (1/iFund)*(1-iPlk), 0
aPlk    *=       iAmp
iMinFrq =        0.05
iBndScl =        (cpsmidinn(48) / iFund) ^ 0.5                  ; n.b. whammy bend does not affect all notes equally  
kFund   limit    iFund * semitone(kBend*iBndScl), iMinFrq, 15000
aBuf    delayr   1/iMinFrq
aTap1   deltapi  a(kPickup/kFund)
aTap2   deltapi  a(1/kFund)
aTap2   tone     aTap2, (sr*0.5) * 0.3
aIn     tone     aIn, (sr*0.5) * 0.3
        delayw   aPlk + aIn + (aTap2 * kFB)
        xout     aTap2 - aTap1
endop

instr   1
kMOUSE_DOWN_LEFT chnget "MOUSE_DOWN_LEFT"

kporttime   linseg   0,0.001,0.05        ; used to smooth GUI control movement
iplk        =   0.1                      ; pluck position (largely irrelevant)
iamp        =   1                        ; amplitude
icps        cpsmidi                      ; midi note played (in cycles-per-second)
inum        notnum                       ; midi note number
ifbscl      table     inum, gifbscl      ; Feedback scaling. Higher notes need higher levels of feedback. This scaling function is defined earlier in a function table.
ksustain    chnget    "sustain"          ; string sustain (inverse of damping). Higher values = more sustain
krefl       =         1-ksustain         ; string damping (derived from sustain value). Higher values = more damping. Range 0 - 1
adel        init      0                  ; feedback signal from the modulating delay (initial value for the first pass) 

; pick-up position
kauto           chnget   "auto"                                       ; 'auto' checkbox button.
if(kauto==0)    then                                                  ; if 'auto' button is off pick-up position is set manually
 kpick          chnget   "PickPos"                                    ; 'Position' GUI knob
 kpick          portk    kpick,kporttime                              ; smooth changes made to the value to prevent glitches
else                                                                  ; other 'Auto' pick-up position modulation mode is used
 kPickposSpeed  chnget   "PickPosSpeed"                               ; pick-up position modulation 'Speed' GUI knob
 kpick          rspline  0.01, 0.99, kPickposSpeed, kPickposSpeed*2   ; pick-up position created using a random spline method
 kpick          limit    kpick,0.01,0.99                              ; contain possible values. (N.b. rspline will generate values beyond its given limits.)
endif

irel            chnget   "rel"                     ; feedback signal release time
aInRel          linsegr  1, 0.001, 1, irel, 0      ; feedback signal release envelope
adust           dust     0.1, 1                    ; generate a sparse crackle signal. This is used to help keep the string vibrating.

kfback          chnget   "fback"                   ; 'Feedback' value from GUI knob
;asig           repluck  iplk, iamp, icps, kpick, krefl,( (adel*kfback*ifbscl) + ( (garvbL+garvbR) * 0.1) + adust) * aInRel   ; vibrating string model
 
kBend           chnget   "Bend"

; Reset pitch bend when mouse is released
if trigger:k(kMOUSE_DOWN_LEFT,0.5,1)==1 then
 chnset k(0), "Bend"
endif
kBend           portk        kBend, kporttime     ; smooth changes
asig            StringPickUp ((adel*kfback*ifbscl) + ( (garvbL+garvbR) * 0.1) + adust) * aInRel, iplk, iamp, icps, kpick, (1-krefl), kBend

; overdrive
kDrive          chnget       "drive"              ; overdrive value from GUI knob
ktrig           changed      kDrive               ; if 'Drive' knob is moved generate a trigger. Reinitialisation will be needed to refresh the 'clip' opcode.
if(ktrig==1) then                                 ; if 'Drive' knob has been moved...
 reinit   UPDATE                                  ; reinitialise from 'UPDATE' label
endif               
UPDATE:                                           ; a label. Reinitialise from here.
iClipLev        =            1-(i(kDrive)*0.9)    ; clip level remapped from 0 to 1 remapped to 1 to 0.1.
asig            clip         asig,0,iClipLev      ; clip the audio signal
igain           table        i(kDrive),giscal,1   ; get amplitude compensation value according to the current 'Drive' setting
asig            =            asig * igain         ; compensate for gain loss
rireturn

; filtering
kLPF            chnget       "LPF"                   ; Lowpass filter ('Bright') GUI knob. This is a ratio of the fundemental frequency so the final cutoff frequency will depend on the note played (keyboard tracking).
kLPF            portk        kLPF, kporttime         ; smooth changes
kLPF            limit        icps * kLPF, 20, sr/2   ; derive cycles-per-second filter cutoff value (and protect against out of range values which would cause the filter to 'blow up')
asig            butlp        asig, kLPF              ; lowpass filter the signal
kHPF            chnget       "HPF"                   ; same as the above but with a highpass filter. (Highpass filter called 'Cut' in GUI.)
kHPF            portk        kHPF, kporttime
kHPF            limit        icps * kHPF, 1, sr/2
asig            buthp        asig, kHPF

; amplitude enveloping
iStrRel         chnget       "StrRel"
aAmpEnv         expsegr      0.001, 0.1, 1, irel+iStrRel, 0.001  ; amplitude envelope (extend note duration a little beyond the when the feedback signal envelope ends)
asig            =            asig*aAmpEnv                        ; apply amplitude envelope

; delay (this controls how different harmonics are accentuated in the feedback process)
kspeed          chnget       "speed"                             ; delay time modulation speed
iDelMin         =            ksmps/sr                            ; delay minimum...
iDelMax         =            0.01                                ; ...and maximum. These could maybe be brought out as GUI controls
kdeltim         rspline      iDelMin, iDelMax, kspeed, kspeed*2  ; random spline for delay time (k-rate)
kdeltim         limit        kdeltim, iDelMin, iDelMax           ; limit value (n.b. rspline will generate values beyond its given minimum and maximum) 
adeltim         interp       kdeltim                             ; interpolate to create a-rate verion for delay time (a-rate will give smoother results)
adel            vdelay       asig, adeltim*1000, 1000            ; create modulating delay signal

; stereo spatialisation - this gives the dry (unreverberated) signal some stereo width 
kdelL           rspline      0.001, 0.01, 0.05, 0.1              ; left channel spatialising delay
kdelR           rspline      0.001, 0.01, 0.05, 0.1              ; right channel spatialising delay
kdelL           limit        kdelL,0.001, 0.01                   ; limit left channel delay times (n.b. rspline will generate values beyond its given minimum and maximum)
kdelR           limit        kdelR,0.001, 0.01                   ; limit right channel delay times (n.b. rspline will generate values beyond its given minimum and maximum)
adelL           interp       kdelL                               ; create a-rate of left channel delay time version using interpolation
adelR           interp       kdelR                               ; create a-rate of right channel delay time version using interpolation
aL              vdelay       adel,adelL*1000,10                  ; delay signal left channel
aR              vdelay       adel,adelR*1000,10                  ; delay signal right channel 

; create reverb send signals (stereo)
klevel          chnget       "level"            ; level control (pre reverb input therefore allows reverb to die away naturally)
gasendL         =            gasendL+(aL*klevel*5)      ; left channel reverb send signal accumulated with audio from this note
gasendR         =            gasendR+(aR*klevel*5)      ; right channel reverb send signal accumulated with audio from this note
endin

instr   Reverb
 iComp          ftgen        0, 0, 1024, -16, 1, 1024, -6, 0.6
 kGain          chnget       "Gain"
 if kGain>0     then
  kComp         table        ((kGain-1)/9), iComp, 1
  gasendL       =            tone:a(tanh(gasendL*kGain) * kComp, 4000)
  gasendR       =            tone:a(tanh(gasendR*kGain) * kComp, 4000)
  
 endif
 
 kroom           chnget     "room"                                 ; room size
 garvbL, garvbR  reverbsc   gasendL, gasendR, kroom, 7000          ; apply reverb
 iDryLev         =          0.1                                    ; dry signal level
 kRvbLev         chnget     "distance"                             ; reverb signal level
                 outs      (garvbL * kRvbLev * 0.2) + (gasendL * iDryLev * (1 - kRvbLev)), (garvbR * kRvbLev * 0.2) + (gasendR * iDryLev * (1 - kRvbLev))
                 clear     gasendL, gasendR
endin


</CsInstruments>

<CsScore>
i "Reverb" 0 3600
</CsScore>

</CsoundSynthesizer>