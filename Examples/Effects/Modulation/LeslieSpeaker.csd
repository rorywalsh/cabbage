
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; LeslieSpeaker.csd
; Written by Iain McCurdy, 2016.

; An emulation of a Leslie rotating speaker as commonly used with a Hammond organ

; A Leslie speaker consists of two speaker drivers, a high frequency tweeter and a low frequency woofer, which rotate at on of two speed according to a switch.
;  (For clarification, the woofer itself does not actually spin, instead a baffle rotates around it.)

; The main interface is simply two main switches which switch between the slow and fast speeds, and a 'setup' button which accesses advanced controls.
; The default advanced settings represent typical characteristics for a real Leslie speaker.

; This .csd works as an effect upon the audio input, but it can also be tested by playing a basic built-in organ using a connected MIDI keyboard

; Setup
; -----
; The same set of parameters are provided for the woofer and the tweeter.

; Cutoff    -    the cutoff frequency of the crossover (lowpass for the woofer, highpass for the tweeter)
; Damping   -    change from one speed to the other is not instantaneous.
;                'Damping' defines the amount of mechanical resistance to changing speed.
;                Typically the woofer takes longer to change speed (higher damping).
; RPM Slow  -    speed (in revolutions per minute) when speed is 'Slow'
; RPM Fast  -    speed (in revolutions per minute) when speed is 'Fast'
; Doppler   -    amount of doppler shift (pitch shift) during rotation
; Panning   -    width of panning modulation
; Amplitude -    amount of amplitude modulation

<Cabbage>
form caption("Leslie Speaker") size(260,120), pluginId("Lsli")  style("legacy"), guiRefresh(16), colour("Black")

image     bounds(  0,  0,260,120), outlineThickness(4), outlineColour("white"), file("DarkWood.jpg");, colour(75,50,50)
image     bounds(  0,  0,260,120), outlineThickness(4), outlineColour("silver"), colour(0,0,0,0)
button    bounds( 40, 85, 60, 25), fontColour:0(80,55,55), fontColour:1(255,245,245), colour:0(75,50,50), colour:1(250,170,170), text("Slow","Slow"), channel("Slow"), latched(1), radioGroup(1), value(1)
button    bounds(100, 85, 60, 25), fontColour:0(80,55,55), fontColour:1(255,245,245), colour:0(75,50,50), colour:1(250,170,170), text("Stop","Stop"), channel("Stop"), latched(1), radioGroup(1)
button    bounds(160, 85, 60, 25), fontColour:0(80,55,55), fontColour:1(255,245,245), colour:0(75,50,50), colour:1(250,170,170), text("Fast","Fast"), channel("Fast"), latched(1), radioGroup(1)

image     bounds( 132,  7,16,16), identChannel("TweeterID"), shape("ellipse"), colour(170,150,150)
image     bounds( 110, 30,40,40), identChannel("WooferID"),  shape("ellipse"), colour(170,150,150)

button bounds(200,10,50, 18), channel("setup_Button"), text("Setup"), fontColour:0(200,200,200), fontColour:1(200,200,200), colour:0(80,55,55), colour:1(80,55,55)
groupbox bounds(360,28,340,190), plant("Setup"), outlineThickness(0), popup(1), identChannel("setupPlant"), colour("Black")
{
image     bounds(0,0,340,190), outlineThickness(4), outlineColour("silver"), colour(75,50,50), file("DarkWood.jpg")
image     bounds(0,0,170,190), outlineThickness(4), outlineColour("silver"), colour(0,0,0,0)
label     bounds(0,4,170, 14), text("Woofer"), fontColour(200,200,200)
nslider bounds(10, 20,70,40), channel("CF_Low"), text("Cutoff"),     range(20,8000,800,1,1)
nslider bounds(90, 20,70,40), channel("LF_Damp"), text("Damping"),   range(0.01,8,1.5,1,0.01)
nslider bounds(10, 60,70,40), channel("LF_Slow"), text("RPM Slow"),  range(5,1000, 40,1,1)
nslider bounds(90, 60,70,40), channel("LF_Fast"), text("RPM Fast"),  range(5,1000,342,1,1)
nslider bounds(10,100,70,40), channel("LF_Dop"), text("Doppler"),    range(0,2,0.1)
nslider bounds(90,100,70,40), channel("LF_Pan"), text("Panning"),    range(0,1,0.2,1,0.01)
nslider bounds(10,140,70,40), channel("LF_Amp"), text("Amplitude"),  range(0,1,0.3,1,0.01)

image     bounds(170,0,170,190), outlineThickness(4), outlineColour("silver"), colour(0,0,0,0)
label     bounds(170,4,170, 14), text("Tweeter"), fontColour(200,200,200)
nslider bounds(180, 20,70,40), channel("CF_High"), text("Cutoff"),    range(20,8000,800,1,1)
nslider bounds(260, 20,70,40), channel("HF_Damp"), text("Damping"),   range(0.01,8,0.5,1,0.01)
nslider bounds(180, 60,70,40), channel("HF_Slow"), text("RPM Slow"),  range(5,1000, 48,1,1)
nslider bounds(260, 60,70,40), channel("HF_Fast"), text("RPM Fast"),  range(5,1000,400,1,1)
nslider bounds(180,100,70,40), channel("HF_Dop"), text("Doppler"),    range(0,2,0.2)
nslider bounds(260,100,70,40), channel("HF_Pan"), text("Panning"),    range(0,1,0.4,1,0.01)
nslider bounds(180,140,70,40), channel("HF_Amp"), text("Amplitude"),  range(0,1,0.3,1,0.01)
}

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

;sr set by host
ksmps     =     32
nchnls     =     2
0dbfs    =    1
massign    0,1

;Author: Iain McCurdy (2016)

opcode    DopplerSpin,aa,akkkkki
    asig,kfreq,kAmpDepth,kAmpPhase,kPanDepth,kDopDep,ishape    xin
    
    isine        ftgen    0,0,1024,10,1
    aAmp         osciliktp kfreq, ishape, kAmpPhase        ; AN LFO DEFINES A VARIABLE USED TO MODULATE AMPLITUDE (NOTE: VARIABLE PHASE). THIS MODELS THE LOCATION WITHIN THE CIRCLE FROM WHICH WE ARE LISTENING. NO AMPLITUDE MODULATION REPRESENT REPRESENTS OUR LISTENING POSITION BEING EXACTLY CENTRAL, MAXIMUM MODULATION REPRESENTS US BEING NEAR TO THE EDGE OF A LARGE CIRCLE. THE CONTROL OVER PHASE OF THIS LFO REPRESENTS WHICH EDGE WE ARE CLOSER TO.
    aAmp        =    (aAmp * 0.5 * kAmpDepth) + 0.5        ; RESCALE AND OFFSET AMPLITUDE MODULATION LFO
    
    aPan        oscili    (kPanDepth * 0.5), kfreq, ishape, 0.75    ; AN LFO DEFINES A VARIABLE FOR PANNING CONTROL - I.E. WHETHER SOUND IS CURRENTLY TO OUT LEFT OR TO OUT RIGHT. NOTE THAT PHASE IS 0.75 AND THEREFORE 0.75 RADIANS (OR 270 DEGREES OUT OF PHASE) WITH THE DELAY MODULATION 
    aPan        =    aPan + 0.5                ; OFFSET PANNING LFO

    iMaxDelay    =    4                    ; DEFINE A VARIABLE THAT WILL BE USE FOR 'MAXIMUM DELAY TIME' (BUFFER LENGTH)
    aDelTim        oscili    kDopDep, kfreq, ishape, 0        ; AN LFO DEFINES A VARIABLE FOR DELAY TIME (NOTE PHASE AT ZERO)
    aDelTim        =    aDelTim + kDopDep            ; DELAY TIME  VARIABLE 'aDelay' IS OFFSET TO STAY WITHIN THE POSITIVE DOMAIN    
        
    aDelTap        vdelay    asig*aAmp, aDelTim, iMaxDelay;, 16
    
    aL, aR    pan2     aDelTap, aPan, 1    ;APPLY PANNING TO SIGNAL OUTPUT FROM DELAY USING pan2 OPCODE.

        xout    aL,aR    
endop


gisine        ftgen    0,0,4096,10,1                                                    ; sine wave
gitri    ftgen    0,0,1024,-7,0,512,1,512,0                                            ; triangle wave

giSrc        ftgen    0,0,4096,10,1,0.5,0.4,0.01,0.01,0.01,0.01,0,0.01                ; waveform of a single tonewheel (not quite a sine wave)
;giAllOut    ftgen    0,0,4096,31, giSrc, 1,1,0, 2,1,0, 3,1,0, 4,1,0, 8,1,0    ; composite of several tonewheels
giAllOut    ftgen    0,0,4096,9, 1,1,0, 2,1,0, 3,1,0, 4,1,0, 5,.1,0, 6,.1,0, 7,.1,0, 8,1,0    ; composite of several tonewheels

instr    1    ; a basic MIDI triggered organ sound
 aEnv        linsegr    0,0.005,1,0.01,0                    ; anti-click envelope
 aMix        poscil    0.5*aEnv,cpsmidi(),giAllOut            ; basic organ sound
 chnmix        aMix,"send"                                    ; mix into send 'channel'
endin

instr    99    ; a Leslie speaker effect
 kSlow    chnget    "Slow"                                    ; read 'Slow' button widget
 kStop    chnget    "Stop"                                    ; read 'Stop' button widget
 kFast    chnget    "Fast"                                    ; read 'Faset' button widget
 
 if kSlow==1 then
  kSpeed    =    0
 elseif kStop==1 then
  kSpeed    =    1
 else
  kSpeed    =    2
 endif
                                                         ; 0=slow 1=stop 2=fast
 aMix        chnget    "send"                                ; read in organ sound from instr 1

 aL,aR        ins                                            ; read live audio in
 aMix        +=    aL+aR                                    ; mix both live audio in channels into the organ sound
 
 kCF_High    chnget    "CF_High"                            ;crossover cutoff frequencies
 kCF_Low    chnget    "CF_Low"
 
 kHF_Slow    =    chnget:k("HF_Slow")/60                    ; tweeter chorale (slow) speed
 kHF_Fast    =    chnget:k("HF_Fast")/60                    ; tweeter tremolo (fast) speed

 kLF_Slow    =    chnget:k("LF_Slow")/60                    ; tweeter chorale (slow) speed
 kLF_Fast    =    chnget:k("LF_Fast")/60                    ; woofer tremolo (fast) speed
 
 kHF_Damp    chnget    "HF_Damp"                            ; tweeter speed change time
 kLF_Damp    chnget    "LF_Damp"                            ; woofer speed change time
 
 kHF_Trem    chnget    "HF_Trem"                            ; tremolo modulation depth
 kLF_Trem    chnget    "LF_Trem"
 
 kHF_Dop    chnget    "HF_Dop"                            ; doppler modulation depth
 kLF_Dop    chnget    "LF_Dop"
 
 kHF_Pan    chnget    "HF_Pan"                            ; panning modulation depth
 kLF_Pan    chnget    "HF_Pan"
 
 kHF_Amp    chnget    "HF_Amp"                            ; amplitude modulation depth
 kLF_Amp    chnget    "LF_Amp"

 aLF        butlp    aMix,kCF_Low                        ; woofer audio signal
 aHF        buthp    aMix,kCF_High                        ; tweeter audio signal

 if kSpeed==0 then                                        ; if speed is slow
  kLF_Speed    =    kLF_Slow                                ; set woofer speed to slow speed as defined in the set-up pop-up
  kHF_Speed    =    kHF_Slow                                ; set tweeter speed to slow speed as defined in the set-up pop-up
 elseif kSpeed==2 then
  kLF_Speed    =    kLF_Fast                                ; set woofer speed to slow speed as defined in the set-up pop-up
  kHF_Speed    =    kHF_Fast                                ; set tweeter speed to slow speed as defined in the set-up pop-up
 else
  kLF_Speed    =    0                                        ; set woofer speed to slow speed as defined in the set-up pop-up
  kHF_Speed    =    0                                        ; set tweeter speed to slow speed as defined in the set-up pop-up
 endif
 
 kportramp    linseg    0,0.01,1                            ; portamento time ramps up quickly from zero
 kLF_Speed    portk    kLF_Speed,kLF_Damp                    ; damp speed change from 
 kHF_Speed    portk    kHF_Speed,kHF_Damp

 aHF_L,aHF_R    DopplerSpin    aHF,kHF_Speed,kHF_Amp,0.5,kHF_Pan,kHF_Dop,gisine    ; call UDO
 aLF_L,aLF_R    DopplerSpin    aLF,kLF_Speed,kLF_Amp,0.5,kLF_Pan,kLF_Dop,gisine
 
        outs    aHF_L,aHF_R
         outs    aLF_L,aLF_R

 ; GRAPHICAL ROTATING SPEAKERS 
 kPhsTweeter    poscil    1,kHF_Speed,gitri                
 kPhsWoofer    poscil    1,kLF_Speed,gitri
 ktrig    metro    16
 ;, colour(170,150,150)
 if ktrig==1 then
  Smsg    sprintfk    "bounds(%d,10,%d,16), colour(%d,%d,%d)",122+(8*(1-kPhsTweeter)),kPhsTweeter*16, kPhsTweeter*120+50, kPhsTweeter*100+50, kPhsTweeter*100+50
      chnset        Smsg,"TweeterID"
  Smsg    sprintfk    "bounds(%d,33,%d,40), colour(%d,%d,%d)",110+(20*(1-kPhsWoofer)),kPhsWoofer*40, kPhsWoofer*120+50, kPhsWoofer*100+50, kPhsWoofer*100+50
      chnset        Smsg,"WooferID"
 endif 
         
         chnclear    "send"                                 ; clear audio send channel (organ)
endin


instr 1000                ; launches plant popups
 klaunch    init        0
 #define LAUNCH_PLANT(name)
 #
 kpressed    chnget         "$name._Button"
 if changed(kpressed)==1 then
   Smsg     sprintfk     "visible(%d), pos(1, 19)", klaunch
           chnset         Smsg, "$name.Plant"
 endif
 #
 $LAUNCH_PLANT(setup)
 klaunch    =        1
endin


</CsInstruments>

<CsScore>
i 99   0 [3600*24*7]
i 1000 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>