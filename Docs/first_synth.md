# Your first Cabbage instrument
The following section shows a very simple Csound/Cabage instrument. It is assumed that the user has some prior knowledge of Csound. If not it may be best to jump to the Beginners section and start there. 

## A basic Cabbage synthesiser

Code to create the most basic of Cabbage synthesisers is presented below. This instrument uses the MIDI interop command line flags to pipe MIDI data directly to p-fields in instrument 1. In this case all MIDI pitch data is being written directly to a special variable named p4 while all MIDI amplitude data is written to variable called p5. MIDI data being sent on channel 1 will cause instrument 1 to play. Data being sent on channel 2 will cause instrument 2 to play(if one was defined), and so on. If you wish to assign an instrument to a unique MIDI channel you can use the 'massign' opcode. 

##Example
```html
<Cabbage>
form size(380, 160), caption("Simple synth"), pluginID("plu1")
keyboard bounds(12, 6, 360, 100)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 --midi-key-cps=4 --midi-velocity-amp=5
</CsOptions>
<CsInstruments>
sr = 44100
ksmps = 64
nchnls = 2
0dbfs=1

instr 1
a1 oscili p5, p4, 1
outs a1, a1
endin

</CsInstruments>  
<CsScore>
f1 0 1024 10 1
f0 3600
</CsScore>
</CsoundSynthesizer>
```

![](images/firstSynth.png)