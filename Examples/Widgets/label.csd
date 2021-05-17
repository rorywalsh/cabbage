<Cabbage>
form caption("Label Example") size(380, 500), guiMode("queue"), colour(2, 145, 209) pluginId("def1")

texteditor bounds(12, 244, 352, 228) channel("infoText"), readOnly(1), wrap(1), scrollbars(1)
label bounds(38, 6, 303, 26) channel("heading"), text("LABELS WILL BE LABELS"), fontColour(255, 255, 255, 255)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d --midi-key=4 --midi-velocity-amp=5
</CsOptions>e
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

    SText  = "A label is a simple widget that can be used to display text the screen. They can act as simple buttons sending a 0 or 1 when they are clicked. Press once for 1, press again for 0, again for 1, etc.\n\nInstrument 1 creates 16 labels in a loop, each with a channel name 'label1', 'label2', 'label3', etc. This is done dynamically using the 'cabbageCreate' opcode. A metro in instr 1 triggers the various labels to appear, and controls both position and fontColour alpha values as the labels are displayed."
    cabbageSet "infoText", "text", SText
    
    iWidgetCount init 0
    while iWidgetCount < 16 do
        SWidget sprintf "bounds(%d, %d, 200, 23), channel(\"label%d\"), text(\"Cabbage Label %d\"), fontColour(147, 210, 0, 0)", random:i(0, 330), random:i(0, 200), iWidgetCount, iWidgetCount
        cabbageCreate "label", SWidget
        iWidgetCount += 1
    od   
    
    kIndex init 1
    if metro(4) == 1 then
        event "i", "DisplayLabel", 0, 2, kIndex+1
        kIndex = (kIndex < 16 ? kIndex+1 : 0)
    endif

endin

instr DisplayLabel

    SWidgetChannel sprintf "label%d", p4
    iColour[] cabbageGet SWidgetChannel, "fontColour"
    kAlpha line 255, p3, 0    
    cabbageSet SWidgetChannel, "bounds", random:i(-100, 360), random:i(50, 200), 200, 23
    cabbageSet metro(10), SWidgetChannel, "fontColour", iColour[0], iColour[1], iColour[2], kAlpha
    
endin

                
</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
;starts instrument 1 and runs it for a week
i1 0 z
</CsScore>
</CsoundSynthesizer>
