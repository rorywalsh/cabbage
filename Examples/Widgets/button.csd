<Cabbage>
form caption("Button Example") size(380, 200), guiMode("queue"), colour(220, 220, 220), pluginId("def1")

button bounds(16, 12, 117, 40) channel("trigger"), text("Start Synth", "Stop Synth"), corners(5)
button bounds(146, 12, 80, 40) channel("mute"), text("Mute", "Unmute"), corners(5), value(1)
button bounds(242, 12, 117, 40) channel("toggleFreq"), text("Toggle Freq"), corners(5)

texteditor bounds(17, 69, 341, 108) channel("infoText"), readOnly(1), wrap(1), scrollbars(1)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d 
</CsOptions>e
<CsInstruments>
; Initialize the global variables. 
sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1

instr 1

SText  = "This instrument shows an example of how buttons can be used in Cabbage. Button will send a 0 or a 1 when they are pressed. Typically you simply test if they have been pressed and do somthing accordingly.\n\n In this example, each time the \"Start Synth\" button is pressed Csound will either enable or disable the Synth instrument. The other two button show how the instrument can be controlled in real time" 
cabbageSet "infoText", "text", SText

kVal, kTrig cabbageGet "trigger"

if kTrig == 1 then
    if kVal == 1 then
        event "i", "Synth", 0, 3600
    else
        turnoff2 nstrnum("Synth"), 0, 0
    endif
endif

endin

instr Synth
    prints "Starting Synth"
    kMute cabbageGet "mute"
    a1 oscili .5*kMute, 300*(cabbageGet:k("toggleFreq")+1)
    outs a1, a1
endin

                

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
;starts instrument 1 and runs it for a week
i1 0 z
</CsScore>
</CsoundSynthesizer>
