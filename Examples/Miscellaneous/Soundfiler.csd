<Cabbage>
form caption("Soundfiler") size(430, 240), colour(58, 110, 182), pluginid("Sflr")
 
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

;===================================================================================
; LoadSoundFile UDO
; usage:
;
;   LoadSoundfile SFileButtonChannel,SFileButtonIdentChannel, STableNumberChannel
;
;   SFileButtonChannel: channel given to filebutton that is used to browse for soundfile
;   SSoundfilerIdentChannel: soundfiler ident channel 
;   STableNumberChannel: channel use to access table number that was asigned to soundfile
;
;===================================================================================
opcode	LoadSoundfile,0,SSi	                                                ; load sound file into a table
    SFileButtonChannel,SFileButtonIdentChannel, iTableNumber xin                                       

    if changed:k(chnget:S(SFileButtonChannel))==1 then 
		SFilePath chnget SFileButtonChannel
        reinit RELOAD_FILE_TO_TABLE

        RELOAD_FILE_TO_TABLE:
    
        if filevalid(SFilePath) ==1 then
            itableOsc	ftgen	iTableNumber,0,0,1,SFilePath,0,0,0		    ; load sound file into a GEN 01 function table
            SMessage sprintfk "file(%s)", SFilePath           
            chnset SMessage, SFileButtonIdentChannel                        ; show file in soundfiler              
        else
            printks "Filename is not valid \n", 0
        endif
        rireturn
    endif 

endop

;=================================================================================
; Instrument 1 is always on and waiting for users to browse for files, 
; or press play/stop once a file has been loaded
;=================================================================================
instr 1
    Sfilepath	chnget	"filebutton1"
    kCombo chnget "presetsCombo"
    kComboChanged changed kCombo
    kFileChanged changed Sfilepath 

    if kComboChanged == 1 || kFileChanged == 1 then		; if user has requested a preset or new file...
        ;event	"i", "LoadSoundFile", 0, 0				                        ; call instrument to update sample storage function table 
        LoadSoundfile "filebutton1", "soundfiler1", 101
    endif  
 
    kPlayButtonTrigger chnget "playbutton1"
    if changed(kPlayButtonTrigger) == 1 then
        if kPlayButtonTrigger == 1 then
            event "i", "PlaySample", 0, 1, 101
        else
            turnoff2 "PlaySample", 0, 0
        endif
    endif
endin

;===================================================================
; Simple function table player
;===================================================================
instr PlaySample
    prints "Playing back sample"
    iChans ftchnls p4 
    p3 = ftlen(p4)/sr
    if iChans == 1 then
        prints "This example only reads stereo files"
    else
        a1, a2 loscil 1, 1, p4, 1
        outs a1, a2
    endif
endin


</CsInstruments>
<CsScore>
;causes instrument 1 to run for about 7000 years...
i1 0 z
</CsScore>
</CsoundSynthesizer>