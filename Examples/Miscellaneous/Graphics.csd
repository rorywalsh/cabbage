<Cabbage>
form caption("Untitled") size(400, 300), colour(58, 110, 182), pluginId("def1")
graphics bounds(10, 10, 380, 280)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d --opcode-lib="/Users/walshr/sourcecode/gcOpcodes/Builds/MacOSX/build/Debug/gcOpcodes.dylib"
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
ksmps = 32
nchnls = 2
0dbfs = 1

;gi1 gcGetContext 1

instr 1
k1 randh 180, 10, 2
k2 randh 80, 30, 2

;k1 gcSetColour 0, 1, 0, 0
;k2 gcFillAll 0, 1, 0, .4

;k3 gcFillEllipse k1, k2, 10, 10

endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
;starts instrument 1 and runs it for a week
i1 0 z
</CsScore>
</CsoundSynthesizer>
