<Cabbage>
form caption("Soundfiler") size(430, 240), colour(58, 110, 182), pluginID("def1")
 
combobox bounds(324, 188, 100, 25), channel("presetsCombo"), populate("*.snaps")
filebutton bounds(262, 188, 60, 25) value(0) text("Save", "Save") mode("snapshot") 

soundfiler bounds(6, 4, 417, 178) identchannel("soundfiler1") tablenumber(-1) 
filebutton bounds(8, 188, 81, 27) channel("filebutton1") value(0) text("Open file", "Open file") 
button bounds(90, 188, 81, 27) channel("playbutton1") text("Play file", "Stop file") 
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 --midi-key-cps=4 --midi-velocity-amp=5
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1

gSfilepath init ""     ;create a global filename

instr 1
    gSfilepath	chnget	"filebutton1"
    kCombo chnget "presetsCombo"
    kComboChanged changed kCombo
    kFileChanged changed gSfilepath 

    if kComboChanged == 1 || kFileChanged == 1 then		; if user has requested a preset or new file...
        event	"i", "LoadSoundFile", 0, 0				                        ; call instrument to update sample storage function table 
    endif  
 
    kPlayButtonTrigger chnget "playbutton1"
    if changed(kPlayButtonTrigger) == 1 then
        if kPlayButtonTrigger == 1 then
            event "i", "PlaySample", 0, 1
        else
            turnoff2 "PlaySample", 0, 0
        endif
    endif
endin


instr	LoadSoundFile	; load sound file
    prints "Loading Soundfile into soundfiler\n"
    if filevalid(gSfilepath) == 1 then
        giTable	ftgen	901,0,0,1,gSfilepath,0,0,0		; load sound file into a GEN 01 function table 
        giChans	filenchnls	gSfilepath			; derive the number of channels (mono=1,stereo=2) in the sound file
        giReady 	=	1					; if no string has yet been loaded giReady will be zero
        gkTabLen	init		ftlen(giTable)/giChans		; table length in sample frames
        Smessage sprintfk "file(\"%s\")", gSfilepath			; print sound file to viewer
        chnset Smessage, "soundfiler1"	
    endif
endin

instr PlaySample
    prints "Playing back sample"
    p3 = ftlen(giTable)/sr
    if giChans == 1 then
        a1 loscil 1, 1, giTable, 1
        outs a1, a1
    else
        a1, a2 loscil 1, 1, giTable, 1
        outs a1, a2
    endif
endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
i1 0 z
</CsScore>
</CsoundSynthesizer>