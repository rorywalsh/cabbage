<Cabbage>
form caption("Button Example") size(400, 300), colour(220, 220, 220), pluginID("def1")
label bounds(8, 6, 368, 20), text("Basic Usage"), fontcolour("black")

hslider bounds(8, 38, 369, 50), channel("gain"), text("Gain") range(0, 1, 0, 1, 0.001) fontcolour(91, 46, 46, 255) textcolour(29, 29, 29, 255)

groupbox bounds(8, 110, 380, 177), text("Randomly Updated Identifiers")
rslider bounds(70, 140, 41, 119) channel("rsliderChannel"), identchannel("widgetIdent"), range(0, 1, 0, 1, 0.001) colour(40, 138, 113, 255) fontcolour(6, 78, 100, 255) fontcolour(6, 78, 100, 255) text("TextOff 98.865305", "TextOn 74.482468") value(40) fontcolour(6, 78, 100, 255) fontcolour(6, 78, 100, 255) fontcolour(6, 78, 100, 255) fontcolour(6, 78, 100, 255) valuetextbox(1) fontcolour(173, 6, 77, 255)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d 
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1

seed 0 
;basic usage
instr 1
    aTone oscili chnget:k("gain"), 300
    outs aTone, aTone    
    
    kRand randi 100, 100, 2
    ;update value of moving slider
    if metro(1) ==  1 then

    endif
endin

;WIDGET_ADVANCED_USAGE

                instr 2
                    kUpdate init 0
                    iCnt init 0
                    if metro(1) == 1 then
                        event "i", "ChangeAttributes", 0, 1, kUpdate
                        kUpdate = (kUpdate < 6 ? kUpdate+1 : 0)
                    endif
                endin

                instr ChangeAttributes
                    SIdentifier init ""
    SIdent sprintf "pos(%d, 140) ", 50 + rnd(100)
    SIdentifier strcat SIdentifier, SIdent
    SIdent sprintf "value(%f) ", rnd(1)
    SIdentifier strcat SIdentifier, SIdent
    SIdent sprintf "size(%d, %d) ", abs(rnd(200))+40, abs(rnd(100)+50)
    SIdentifier strcat SIdentifier, SIdent
    SIdent sprintf "colour:0(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
    SIdentifier strcat SIdentifier, SIdent
    SIdent sprintf "colour:1(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
    SIdentifier strcat SIdentifier, SIdent
    SIdent sprintf "fontcolour:0(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
    SIdentifier strcat SIdentifier, SIdent
    SIdent sprintf "fontcolour:1(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
    SIdentifier strcat SIdentifier, SIdent   
    SIdent sprintf "text(\"TextOff %f\", \"TextOn %f\") ", rnd(100), rnd(100)
    SIdentifier strcat SIdentifier, SIdent

                    chnset SIdentifier, "widgetIdent"     
                   ; prints SIdentifier         
                endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
;starts instrument 1 and runs it for a week
i1 0 z
i2 0 z
</CsScore>
</CsoundSynthesizer>