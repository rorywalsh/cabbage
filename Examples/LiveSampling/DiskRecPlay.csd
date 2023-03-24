
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; DiskRecPlay.csd
; Written by Iain McCurdy, 2012


<Cabbage>
form caption("Disk Rec/Play") size(300, 210), pluginId("dkrp") colour("Black")
groupbox bounds(10,   0, 70, 80), text("Record")
checkbox channel("Record"), bounds(20,25,50,50), value(0), shape("square"), colour("red")
groupbox bounds(80,   0, 70, 80), text("Pause")
checkbox channel("Pause"), bounds(90,25,50,50), value(0), shape("square"), colour("Blue")
groupbox bounds(150,   0, 70, 80), text("Play Loop")
checkbox channel("PlayLoop"), bounds(160,25,50,50), value(0), shape("square")
groupbox bounds(220,   0, 70, 80), text("Play Once")
checkbox channel("PlayOnce"), bounds(230,25,50,50), value(0), shape("square"), colour("yellow")
hslider bounds(10,  90, 280,20), channel("Speed"), range(-4, 4, 1), text("Speed")
hslider bounds(10, 120, 280,20), channel("InSkip"), range(0, 1, 0), text("In Skip")
hslider bounds(10, 150, 280,20), channel("InGain"), range(0, 1, 1), text("Input Gain")
hslider bounds(10, 180, 280,20), channel("OutGain"), range(0, 1, 1), text("Output Gain")
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>
;Author: Iain McCurdy (2012)

<CsInstruments>

; sr set by host
ksmps    =    32
nchnls   =    2
0dbfs    =    1
gkrecdur    init    0



instr    1
 SFile       =         "/Recording.wav"
 SPath       chnget    "CSD_PATH"
 gSFilePath  strcat    SPath,SFile
 
 gkRecord    chnget    "Record"                       ; READ IN CABBAGE WIDGET CHANNELS
 gkPause     chnget    "Pause"
 gkPlayLoop  chnget    "PlayLoop"
 gkPlayOnce  chnget    "PlayOnce"
 gkSpeed     chnget    "Speed"
 gkInSkip    chnget    "InSkip"
 gkInGain    chnget    "InGain"
 gkOutGain   chnget    "OutGain"

 kswitch    changed    gkRecord,gkPlayOnce,gkPlayLoop ; IF EITHER 'PLAY ONCE', 'PLAY LOOP' OR 'RECORD' ARE CHANGED, GENERATE A MOMENTARY '1'
 if kswitch==1 then                                   ; IF SWITCH CHANGE TRIGGER IS '1'
  reinit RESTART                                      ; BEGIN A REINITIALISATION PASS
 endif                                                ; END OF CONDITIONAL BRANCH
 RESTART:                                             ; A LABEL. BEGIN REINITIALISATION PASS FROM HERE

 if i(gkRecord)=1 then        ; IF RECORD BUTTON IS ON...
     turnon 4                 ; TURN ON RECORD INSTRUMENT
 elseif i(gkRecord)=0 then    ; OR ELSE IF RECORD BUTTON IS OFF...
     turnoff2 4,0,0           ; TURN OFF RECORD INSTRUMENT
 endif                        ; END OF THIS CONDITIONAL BRANCH

 if i(gkPlayLoop)=1 then      ; IF 'PLAY LOOP' BUTTON IS ON...
     turnon 2                 ; TURN ON PLAY LOOP INSTRUMENT
 endif                        ; END OF THIS CONDITIONAL BRANCH

 if i(gkPlayOnce)=1 then      ; IF 'PLAY ONCE' BUTTON IS ON...
     turnon 3                 ; TURN ON PLAY ONCE INSTRUMENT
 endif                        ; END OF THIS CONDITIONAL BRANCH
endin
        
instr    2                    ; PLAYBACK LOOPED INSTRUMENT
 if    gkPlayLoop=0    then   ; IF PLAY LOOPED BUTTON IS DEACTIVATED THEN...
     turnoff                  ; ...TURNOFF THIS INSTRUMENT
 endif                        ; END OF CONDITIONAL BRANCH
 ifilelen     filelen    gSFilePath
 iInSkip      =          i(gkInSkip)*ifilelen
 kporttime    linseg     0,0.001,0.02
 kSpeed       portk      gkSpeed,kporttime
 if gkPause!=1 then                                     ; IF PAUSE BUTTON IS NOT ACTIVATED... 
    aL,aR     diskin2    gSFilePath,kSpeed,iInSkip,1    ; PLAY AUDIO FROM FILE
              outs       aL*gkOutGain,aR*gkOutGain      ; SEND AUDIO TO OUTPUT
 endif                                                  ; END OF CONDITIONAL BRANCH
endin

instr    3    ;PLAYBACK ONCE INSTRUMENT
 if gkPlayOnce=0 then       ; IF PLAY ONCE BUTTON IS DEACTIVATED THEN...
     turnoff                ; ...TURNOFF THIS INSTRUMENT
 endif                      ; END OF CONDITIONAL BRANCH
 ifilelen     filelen    gSFilePath
 iInSkip      =          i(gkInSkip)*ifilelen
 kporttime    linseg     0,0.001,0.02
 kSpeed       portk      gkSpeed,kporttime
 if gkPause!=1 then                                        ; IF PAUSE BUTTON IS NOT ACTIVATED... 
   aL,aR      diskin2    gSFilePath,kSpeed,iInSkip,0   ; PLAY AUDIO FROM FILE
              outs       aL*gkOutGain,aR*gkOutGain               ; SEND AUDIO TO OUTPUT
   kplaydur    line      0,1,1                               ; CREATE A RISING VALUE USED AS A TIMER
   if    kplaydur>=gkrecdur    then                        ; IF END OF RECORDING IS REACHED...
     koff     =          0
              chnset     koff,"PlayOnce"
     turnoff                ; - TURN OFF THIS INSTRUMENT IMMEDIATELY.
   endif                    ; END OF CONDITIONAL BRANCH
 endif                      ; END OF CONDITIONAL BRANCH
 krelease     release
 if krelease==1 then
              chnset     1-krelease,"PlayOnce"
 endif
endin

instr    4    ;RECORD
 if    gkPause!=1    then                                            ; IF PAUSE BUTTON IS NOT ACTIVATED...
   ainL,ainR     ins                                                 ; READ AUDIO FROM STEREO INPUT
                 fout    gSFilePath, 6, ainL*gkInGain, ainR*gkInGain ; WRITE MONO AUDIO TO A 32 BIT HEADERLESS FILE (TYPE:6)
   gkrecdur      line    0,1,1                                       ; GLOBAL VARIABLE USED TO REGISTER THE DURATION OF THE CURRENTLY RECORDED FILE
 endif                                                               ; END OF CONDITIONAL BRANCHING    
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>
