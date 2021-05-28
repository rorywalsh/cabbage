
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Reverse.csd
; Iain McCurdy [2012]
; 
; Buffers audio for reversed (and forward) playback.
; 
; INSTRUCTIONS
; ------------
; Time L      --    length of the left channel delay buffer 
; Time R      --    length of the right channel delay buffer
; Spread        --    stereo spread of the two channel. 1 = hard left and right
; Mix        --    dry/wet mix
; Level        --    output level
; Reverse     --    (switch) activate reversed buffer
; Forward    --    (switch) activate forward buffer
; Link L&R    --    (switch) pair the left and right Time controls (functions differently if 'Time Mod' is activated)
; Time Mod    --    (switch) if this switch is on the delay times for both channels will modulate randomly between 'Time L' and 'Time R'. If 'Link L&R' is active this modulation is done in tandem for both channels.
; Pan Mod    --    (switch) if this switch is on the panning modulates randomly



<Cabbage>
form caption("Reverse") size(455, 95), pluginId("rvrs")
image           bounds(0, 0, 455, 95), colour("darkslategrey"), shape("rounded"), outlineColour("white"), outlineThickness(4)
rslider  bounds( 10, 10,  75, 75), text("Time L"),    channel("timeL"),   range(0.010, 4, 1, 0.5,0.001),colour(37,59,59)   textColour(255,255,200), trackerColour(lightblue)
rslider  bounds( 80, 10,  75, 75), text("Time R"),    channel("timeR"),   range(0.010, 4, 1, 0.5,0.001),colour(37,59,59)   textColour(255,255,200), trackerColour(lightblue)
rslider  bounds(150, 10, 75, 75),  text("Spread"),    channel("spread"),  range(0, 1.00, 1),            colour(37,59,59)   textColour(255,255,200), trackerColour(lightblue)
rslider  bounds(220, 10, 75, 75),  text("Mix"),       channel("mix"),     range(0, 1.00, 1),            colour(37,59,59)   textColour(255,255,200), trackerColour(lightblue)
rslider  bounds(290, 10, 75, 75),  text("Level"),     channel("level"),   range(0, 1.00, 1, 0.5),       colour(37,59,59)   textColour(255,255,200), trackerColour(lightblue)
checkbox bounds(370, 12, 100, 12), text("Reverse"),   channel("reverse"),  value(1),                    colour(255,255, 50), fontColour:0(255,255,200), fontColour:1(255,255,200)
checkbox bounds(370, 27, 100, 12), text("Forward"),   channel("forward"),  value(0),                    colour(255,255, 50), fontColour:0(255,255,200), fontColour:1(255,255,200)
checkbox bounds(370, 42, 100, 12), text("Link L&R"),  channel("link"),     value(0),                    colour(255,255, 50), fontColour:0(255,255,200), fontColour:1(255,255,200)
checkbox bounds(370, 57, 100, 12), text("Time Mod."), channel("TMod"),    value(0),                     colour(255,255, 50), fontColour:0(255,255,200), fontColour:1(255,255,200)
checkbox bounds(370, 72, 100, 12), text("Pan Mod."),  channel("PMod"),    value(0),                     colour(255,255, 50), fontColour:0(255,255,200), fontColour:1(255,255,200)
}
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-d -n
</CsOptions>
<CsInstruments>

; sr set by host
ksmps = 32
nchnls = 2
0dbfs = 1

;Author: Iain McCurdy (2012)
;http://iainmccurdy.org/csound.html

opcode    Reverse, a, aKkkk            ;nb. CAPITAL K CREATE A K-RATE VARIABLE THAT HAS A USEFUL VALUE ALSO AT I-TIME
    ain,ktime,kreverse,kforward,klink    xin            ;READ IN INPUT ARGUMENTS
    ;four windowing envelopes. An appropriate one will be chosen based on the reversed chunk duration
    ienv1    ftgenonce            0, 0, 131072, 7, 0, 1024,           1, 131072-(1024*2),           1,  1024,       0    ;for longest chunk times
    ienv2    ftgenonce            0, 0, 131072, 7, 0, 4096,           1, 131072-(4096*2),           1,  4096,       0
    ienv3    ftgenonce            0, 0, 131072, 7, 0,16384,           1, 131072-(16384*2),          1, 16384,       0
    ienv4    ftgenonce            0, 0, 131072, 7, 0,32768,           1, 131072-(32768*2),          1, 32768,       0    ;for shortest chunk times

    atime    interp    ktime            ;INTERPOLATE TO CREATE A-RATE VERSION OF K-TIME
        
    iratio    =    octave(1)
    
    ktrig    trigger    klink,0.5,0        ;if 'Link L&R' is turned on restart delay time phasors to ensure sync between the two channels
    if ktrig=1 then
     reinit    RESTART_PHASOR
    endif
    RESTART_PHASOR:
    aptr    phasor    (2/ktime)        ;CREATE A MOVING PHASOR THAT WITH BE USED TO TAP THE DELAY BUFFER
    rireturn
    if ktime<0.2 then            ;IF CHUNK TIME IS LESS THAN 0.2... (VERY SHORT) 
     aenv    table3    aptr,ienv4,1        ;CREATE AMPLITUDE ENVELOPE
    elseif ktime<0.4 then
     aenv    table3    aptr,ienv3,1
    elseif ktime<2 then
     aenv    table3    aptr,ienv2,1
    else                    ;other longest bracket of delay times
     aenv    table3    aptr,ienv1,1
    endif
    aptr    =    aptr*atime        ;SCALE PHASOR ACCORDING TO THE LENGTH OF THE DELAY TIME CHOSEN BY THE USER
 
     abuffer    delayr    4 ;+ 0.01        ;CREATE A DELAY BUFFER
    abwd    deltap3    aptr            ;READ AUDIO FROM A TAP WITHIN THE DELAY BUFFER
    afwd    deltap3    atime            ;FORWARD DELAY
        delayw    ain            ;WRITE AUDIO INTO DELAY BUFFER
    
    ;rireturn                ;RETURN FROM REINITIALISATION PASS
    xout    (abwd*aenv*kreverse)+(afwd*kforward)    ;SEND AUDIO BACK TO CALLER INSTRUMENT. APPLY AMPLITUDE ENVELOPE TO PREVENT CLICKS.
endop


instr 1
ktimeL   chnget "timeL"
ktimeR   chnget "timeR"
kspread  chnget "spread"
kmix     chnget "mix"
klevel   chnget "level"
kreverse chnget "reverse"
kforward chnget "forward"
kTMod    chnget "TMod"
kPMod    chnget "PMod"

/* LINK */
klink chnget "link"            ; if 'Link L&R' is selected
if klink=1&&kTMod=0 then        
 ktrigL    changed    ktimeL,klink
 ktrigR    changed    ktimeR
 if ktrigL=1 then
  chnset    ktimeL,"timeR"
 elseif ktrigR=1 then
  chnset    ktimeR,"timeL"
 endif
endif

a1,a2    ins

if kTMod=1 then                        ; if time modulation is selected....
 if klink=0 then                    ; and if 'link L&R' is off...
  ktime1    rspline    ktimeL,ktimeR,0.2,1        ; generate delay time value: random spline between ktimeL and ktimeR 
  ktime2    rspline    ktimeL,ktimeR,0.2,1
  ktimeL    limit    ktime1,0.01,4            ; assign to delay time variable and limit to prevent out of range values (possible with rspline)
  ktimeR    limit    ktime2,0.01,4
 else
  ktime        rspline    ktimeL,ktimeR,0.2,1        
  ktimeL    limit    ktime,0.01,4
  ktimeR    =    ktimeL                ; right channel delay the same as left
 endif
endif

arev1    Reverse    a1,ktimeL,kreverse,kforward,klink        ; call UDO
arev2    Reverse    a2,ktimeR,kreverse,kforward,klink

if kPMod=1 then                        ; if panning modulation is on...
 kpan    rspline    0,1,0.2,1                ; pan position generated as a random spline
 ap1    =    (arev1*kpan)     + (arev2*(1-kpan))    ; create new left channel
 ap2    =    (arev1*(1-kpan)) + (arev2*kpan)        ; create new right channel
 arev1    =    ap1                    ; reassign left channel to new left channel
 arev2    =    ap2                     ; reassign right channel to new right channel
endif

a1    ntrpol    a1,arev1,kmix            ; dry/wet mix
a2    ntrpol    a2,arev2,kmix
a1    =    a1 * klevel            ; apply level control
a2    =    a2 * klevel
kspread    scale    kspread,1,0.5             ; reScale from range 0 - 1 to 0.5 - 1
aL    sum    a1*kspread,a2*(1-kspread)    ; create stereo mix according to Spread control
aR    sum    a2*kspread,a1*(1-kspread)    ; create stereo mix according to Spread control
    outs    aL,aR
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>