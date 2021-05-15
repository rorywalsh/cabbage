<Cabbage>
form caption("Texteditor Example") size(380, 300), guiMode("queue"), colour(2, 145, 209) pluginId("def1")

texteditor bounds(18, 20, 341, 204) fontSize(16), channel("orcText"), scrollbars(1), wrap(1),  fontColour(124, 210, 0), colour(0, 0, 0, 100)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d --midi-key=4 --midi-velocity-amp=5
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
ksmps = 16
nchnls = 2
0dbfs = 1


; Rory Walsh 2021 
;
; License: CC0 1.0 Universal
; You can copy, modify, and distribute this file, 
; even for commercial purposes, all without asking permission. 

instr 1

    SText  = {{
/* A Cabbage texteditor is a simple text editor that can show static text, or provide a way of edit new or existing text. You can also send text back to Csound by hit Ctrl+Enter. When you do so, Csound will pick up the entire editor string.\n\nIn the this example we create a simple live-coding type environment. Each time we modify the Csound code it gets sent to Csound and compiled on the fly.\n\nAll of the Cabbage widget examples use simple texteditor widgets to show basic information, but they can also load entire text files using the 'file()' identifier.  */

;simple instrument
instr 1
    kEnv madsr .1, .4, .5, 1
    a1 oscili kEnv, p4
    outs a1, a1
endin

;schedule(1, 0, 2, 300)
    }}
    
    cabbageSet "orcText", "text", SText
    cabbageSet "scoText", "text",  "schedule(1, 0, 2, 300)"

    SText cabbageGetValue "orcText"
    if changed:k(SText) == 1 then
        event "i", 2, 0, 1
    endif
    
    SText cabbageGetValue "scoText"
    if changed:k(SText) == 1 then
        event "i", 3, 0, 1
    endif
endin

instr 2
    SText chnget "orcText"
    prints SText
    ires compilestr SText 
    print ires ; -1 as could not compile
endin

instr 3
    SText chnget "scoText"
    prints SText
    ires compilestr SText 
    print ires ; -1 as could not compile
endin
</CsInstruments>
<CsScore>
i1 0 z
</CsScore>
</CsoundSynthesizer>