; HRTF3D.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form caption("HRTF 3D"), size(365, 275), pluginId("HRTF")
image pos(0, 0), size(365, 236), colour(50, 20,  0), shape("rounded"), outlineColour("white"), outlineThickness(4) 
;xypad bounds(6,  6, 250, 224), channel("Az","Elev"), text("X=Azimuth : Y=Elev."), rangeX(-180,  180,   0), rangeY(-40, 90, 0)
xypad bounds(6,  6, 250, 224), channel("Az","Elev"), text("X=Azimuth : Y=Elev."), rangeX(0,  1,   0.5), rangeY(0, 1, 0.5)
nslider bounds( 40,195, 55,20), channel("AzD"), range(-180,180,0,1,1)
nslider bounds(165,195, 55,20), channel("ElevD"), range(-40,90,0,1,1)
checkbox bounds(265, 6, 80, 12), channel("TestSound"), FontColour("white"),  value(1), text("Test Sound"), fontColour:0("white"), fontColour:1("white"), colour(yellow)

combobox bounds(265, 20, 90,25), channel("type"), value(2), text("hrtfmove", "hrtfmove2")
label    bounds(265,  50, 90, 15), text("L/R In"), fontColour("white")
hslider  bounds(265,  65, 90, 20), channel("L_R_In"), range(0, 1.00, 0)
label    bounds(265,110, 90, 15), text("Overlap"), fontColour("white")
combobox bounds(265,125, 90, 20), channel("overlap"), value(3), text("1", "2", "4", "8", "16", "32", "64")
label    bounds(265, 175, 90, 15), text("Radius"), fontColour("white")
hslider  bounds(265, 190, 90, 20), channel("radius"), range(1, 30, 9)
image    bounds(5,245, 215, 20), colour(100,100,100,80), plant("credit"), outlineThickness(0){
label    bounds(0.03, 0.1, .95, .7), text("Author: Iain McCurdy |2012|"), fontColour("white")}
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

; sr set by host
ksmps    =     16
nchnls   =     2
0dbfs    =     1

;Author: Iain McCurdy (2012)

instr    1
kAz        chnget        "Az"            ;READ WIDGETS...
kAz        scale         kAz, 180, -180
           chnset        kAz, "AzD"
kElev      chnget        "Elev"
kElev      scale         kElev, 90, -40
           chnset        kElev, "ElevD"
ktype      chnget        "type"
kL_R_In    chnget        "L_R_In"
koverlap   chnget        "overlap"
kradius    chnget        "radius"

kTestSound chnget        "TestSound"
if kTestSound=1 then                                ; generate a test sound...
 aL        pinkish        0.3                       ; generate some pink noise
 koct      rspline        8,12,0.3,1                ; randomly moving pitch (oct format)
 aL        reson          aL,cpsoct(koct),cpsoct(koct)*0.2,1    ; reson filter it
 aR        =              aL                        ; rig channel same as the left
else
 aL,aR     ins                    ;READ STEREO AUDIO INPUT
; audio input
 aL, aR       diskin2  "/Users/imccurdy/Documents/SourceSounds/Amen.wav",1,0,1
endif
asrc       ntrpol         aL,aR,kL_R_In
kporttime  linseg         0,0.001,0.02              ; PORTAMENTO TIME RISES UP QUICKLY FROM ZERO TO A HELD VALUE
;kAz       portk          kAz,kporttime             ; SMOOTH RT CHANGES OF AZIMUTH
;kElev     portk          kElev,kporttime           ; SMOOTH RT CHANGES OF ELEVATION
kswitch    changed        koverlap,kradius          ; IF 'OVERLAP' OR 'RADIUS' CHANGE, GENERATE A TRIGGER
if kswitch==1 then                                  ; IF A CHANGE TRIGGER HAS BEEN GENERATED...
 reinit    update                                   ; REINITIALISE FROM LABEL 'update'
endif                                               ; END OF CONDITIONAL BRANCH
update:                                             ; A LABEL. REINITIALISATION BEGINS FROM HERE
ioverlap    =             2 ^ (i(koverlap) - 1)     ; DERIVE OVERLAP VALUE (1,2,4,8 etc.) FROM COMBOBOX INDEX VALUES (1,2,3,4 etc.)
if ktype==1 then                                    ; IF 'hrtfmove' SELECTED USING COMBOBOX...
 aleft, arig hrtfmove     asrc, kAz, kElev, "hrtf-44100-left.dat", "hrtf-44100-right.dat", ioverlap, i(kradius) ;, isr]       ; APPLY hrtfmove2 OPCODE TO AUDIO SOURCE FROM INSTR 1
else                                                ; IF 'hrtfmove2' SELECTED USING COMBOBOX...
 aleft, arig hrtfmove2    asrc, kAz, kElev, "hrtf-44100-left.dat","hrtf-44100-right.dat",ioverlap, i(kradius);, isr]       ; APPLY hrtfmove2 OPCODE TO AUDIO SOURCE FROM INSTR 1
endif                                               ; END OF CONDITIONAL BRANCH
rireturn
           outs           aleft, arig               ; SEND AUDIO TO OUTPUTS
endin
        
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>