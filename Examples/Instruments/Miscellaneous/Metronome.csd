; Metronome.csd
; Written by Iain McCurdy, 2013
; For the best timing accuracy, set ksmps = 1

; level 1    -    amplitude level for the main 'beat' (1st beat of the bar)
; level 2    -    amplitude level for other beats of the bar (subdivisions)
; Subdiv.    -    subdivision of the bar. i.e. 4 for 4/4, 3 for 3/4 etc.
; tone 1    -    tone of the main beat (1st beat of the bar)
; tone 2    -    tone level for other beats of the bar (subdivisions)
; duration    -    duration of each 'click'
; width        -    stereo width of each click

; to achieve a simple 1 click metronome, set level 2 to zero and subdiv to 1. 
 
; A simple Cabbage metronome

<Cabbage>
form caption("Metronome"), size(770,140), pluginId("mtro"), colour("black")
image            bounds(  0,  0,770,140), colour(160, 70,20,100), shape("rounded"), oulinecolour("white"), outlineThickness(4) 
checkbox bounds( 15, 20,130, 40), text("Start/Stop") channel("OnOff"), fontColour("White"), colour(250,250,100)  value(0)

rslider  bounds(120, 13, 65, 65), text("Level 1"),     colour(160, 70,20,100), trackerColour(250,250,200), textColour("White"), fontColour("White"), outlineColour(10,10,10), channel("level1"), range(0, 1.00, 0.4, 0.5, 0.001)
rslider  bounds(180, 13, 65, 65), text("Level 2"),     colour(160, 70,20,100), trackerColour(250,250,200), textColour("White"), fontColour("White"), outlineColour(10,10,10), channel("level2"), range(0, 1.00, 0.1, 0.5, 0.001)
rslider  bounds(240, 13, 65, 65), text("Subdiv."),     colour(160, 70,20,100), trackerColour(250,250,200), textColour("White"), fontColour("White"), outlineColour(10,10,10), channel("subdiv"), range(1, 16, 3, 1, 1)
rslider  bounds(300, 13, 65, 65), text("Tone 1"),      colour(160, 70,20,100), trackerColour(250,250,200), textColour("White"), fontColour("White"), outlineColour(10,10,10), channel("tone1"),  range(0, 4.00, 2.5)
rslider  bounds(360, 13, 65, 65), text("Tone 2"),      colour(160, 70,20,100), trackerColour(250,250,200), textColour("White"), fontColour("White"), outlineColour(10,10,10), channel("tone2"),  range(0, 4.00, 1.56)
rslider  bounds(420, 13, 65, 65), text("Duration"),    colour(160, 70,20,100), trackerColour(250,250,200), textColour("White"), fontColour("White"), outlineColour(10,10,10), channel("dur"),    range(0.003, 0.1, 0.012, 0.5, 0.00001)
rslider  bounds(480, 13, 65, 65), text("Width"),       colour(160, 70,20,100), trackerColour(250,250,200), textColour("White"), fontColour("White"), outlineColour(10,10,10), channel("width"),  range(0, 2.00, 0)
checkbox bounds(550, 18, 45, 45) channel("indicator"), fontColour("White"), colour(255, 50, 50)  value(0), shape("ellipse")
checkbox bounds(600, 18, 45, 45) channel("indicator2"), fontColour("White"), colour(100,255,100)  value(0), shape("ellipse")

hslider  bounds( 10, 75,750, 40), colour(160, 70,20), trackerColour(160, 70, 20), fontColour("White"), channel("tempo"), range(10,1000, 90, 1, 1) textBox(1)
label    bounds(325, 110, 80, 12), text("Tempo [BPM]"), fontColour("white")

; big indicators
button   bounds(655, 25, 100, 30), channel("bigButton"), text("Large LEDs")
groupbox bounds(655, 25,1030, 520), colour(160, 70,20,100), outlineThickness(0), popup(1), plant("Large LEDs"), identChannel("bigPlant")
{
checkbox bounds( 10, 10,500,500) channel("indicator"), fontColour("White"), colour(255, 50, 50)  value(0), shape("ellipse")
checkbox bounds(520, 10,500,500) channel("indicator2"), fontColour("White"), colour(100,255,100)  value(0), shape("ellipse")
}

label   bounds( 10,120, 180, 12), text("Author: Iain McCurdy |2013|"), fontColour("white")
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps         =     16    ; for maximum timing accuracy set ksmps = 1
nchnls         =     2
0dbfs        =    1    ;MAXIMUM AMPLITUDE

gistrt    ftgen    0,0,8,-2,    200,    200,    2500,    5000,    5000
giend    ftgen    0,0,8,-2,    20,    1000,    20,    20,    1000

gisine    ftgen    0,0,4096,10,1    ; sine wave (used by the audio oscillator)
gasend    init    0        ; global variable used to send audio to the 'stereo' instrument

gibar    ftgen    0,0,64,-2,1    ; bar structure basically a '1' followed by 63 zeroes, '1' indicating the first neat of a bar and zero indicating any other beat

instr    1
 kOnOff        chnget    "OnOff"
 kdur        chnget    "dur"
 ksubdiv    chnget    "subdiv"
 ktone1    chnget    "tone1"
 ktone2    chnget    "tone2"
 klevel1    chnget    "level1"
 klevel2    chnget    "level2"
 kstrt    tablei    ktone1,gistrt
 kend    tablei    ktone1,giend
 kstrt2    tablei    ktone2,gistrt
 kend2    tablei    ktone2,giend

 ; restart metronome if subdiv changes to ensure timing accuracy
 ktrig    changed    ksubdiv,kOnOff
 if ktrig==1 then
  reinit RESTART
 endif
 RESTART:
 
 kbeat    init    0    ; initialise beat position (0 = first beat of the bar)

 if kOnOff==1 then
  gktempo    chnget    "tempo"
  kmetro    metro     gktempo/60            ; bar metronome

  if kmetro=1 then                    ; if a beat is occuring...
   ktype    table    kbeat,gibar    ;1=bar 0=beat    ; read if this is a first beat of a bar or not (1=first beat, 0=any other)
   if ktype=1 then                    ; if first beat of a bar...
    event "i",2,0,kdur,kstrt,kend,klevel1,1        ; ... call metronome click as if 1st beat (p7 = 1 = flash indicator)
   else                            ; otherwise (must be aby other beat of a bar)...
    event "i",2,0,kdur,kstrt2,kend2,klevel2,0        ; ...call metronome click as if any other beat of a bar (p7 = 0 = don't flash indicator)
   endif
   kbeat wrap kbeat+1, 0, ksubdiv            ; increment beat position but wrap around to zero if final beat of a bar has already been reacxhed
  endif
 endif
endin

instr    2    ; produce metronome sound (this instrument will be called repeatedly)
    iwidth    chnget    "width"
    acps    transeg    p4,p3,-7,p5        ; pitch of oscillator (a quickly decaying exponential curve) start='tone 1', end='tone 2'
    aenv    transeg    1,p3,-7,0        ; amplitude of oscillator (a quickly decaying exponential curve)
    asig    oscili    p6,acps,gisine    ; audio oscillator
    amod    oscili    3,200,gisine        ; modulation signal
    asig    =    asig*amod        ; ring modulate original audio signal
    asig    butbp    asig*10,100,100        ; bandpass filter the sound to attenuate lows and highs
    asig    =    asig*aenv        ; apply envelope
        outs    asig,asig        ; send audio to outputs
    gasend    =    gasend+asig*iwidth    ; send a bit of the audio to the stereo-ifying instrument
    event_i    "i",5-p7,0,0.08        ; flash indicator (call instrument 4 for 0.1 duration)
endin

instr    3    ; 'stereo'ify the sound
    aL    delay    gasend,    rnd(0.01)    ; slightly delay audio by a random amount to create left channel audio
    aR    delay    gasend,    rnd(0.01)    ; slightly delay audio by a random amount to create right channel audio
    aL    butlp    aL,cpsoct(rnd(10))    ; filter left channel (random cutoff frequency)
    aR    butlp    aR,cpsoct(rnd(10))    ; filter right channel (random cutoff frequency)
        outs    aL,aR            ; send audio to outputs
    gasend    =    0            ; clear global send variable
endin

instr    4    ; flash red indicator
    gkrelease    release            ; krelease=0 while note is active, =1 when note ends (final k-cycle)
    chnset    1-gkrelease,"indicator"        ; turn indicator on or off
endin

instr    5    ; flash green indicator
    gkrelease    release            ; krelease=0 while note is active, =1 when note ends (final k-cycle)
    chnset    1-gkrelease,"indicator2"        ; turn indicator on or off
endin


instr 1000                ; launches plant popups

klaunch    init    0

#define LAUNCH_PLANT(name)
#
kpressed    chnget "$name.Button"
if changed(kpressed)==1 then
  Smsg sprintfk "visible(%d), pos(1, 19)", klaunch
  chnset Smsg, "$name.Plant"
endif
#

$LAUNCH_PLANT(big)

klaunch    =    1

endin


</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]
i 3 0 [60*60*24*7]
i 1000 0 [60*60*24*7]
</CsScore>
</CsoundSynthesizer>