# Your first Cabbage instrument
The following section shows a very simple Csound/Cabage instrument. It is assumed that the user has some prior knowledge of Csound. If not it may be best to jump to the [Beginners](beginners.html) section and start there. 

## A basic Cabbage synthesiser

To create a new instrument hit the new file button as shown in the following image.

![New file](images/new_file.gif)

When Cabbage creates the file it will generate some basic code that can be used as a template which you can modify to suit your needs. This instrument, shown below, uses the MIDI interop command line flags to pipe MIDI data directly to p-fields in instrument 1. In this case all MIDI pitch data is being written directly to a special variable named p4 while all MIDI amplitude data is written to variable called p5. You can read more about this in the [Beginners](beginners.html) section. MIDI data being sent on channel 1 will cause instrument 1 to play. Data being sent on channel 2 will cause instrument 2 to play(if one was defined), and so on. If you wish to assign an instrument to a unique MIDI channel you can use the 'massign' opcode. If you don't have a keyboard you can still send MIDI data to an instrument by using the 'leyboard' widget.  

##Example
```html
<Cabbage>
form caption("Untitled") size(400, 300), colour(58, 110, 182), pluginID("def1")
keyboard bounds(8, 158, 381, 95)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d --midi-key-cps=4 --midi-velocity-amp=5
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1

;instrument will be triggered by keyboard widget
instr 1
kEnv madsr .1, .2, .6, .4
aOut vco2 p5, p4
outs aOut*kEnv, aOut*kEnv
endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
</CsScore>
</CsoundSynthesizer>

```

##Running your instrument
Each Csound instrument you have open in Cabbage will have a unique code editor and Csound output console shown with it. To run an instrument simply hit the 'Play' button in the file tab controls (as shown below), or go to the Tools menu and hit Bulid Instrument. 

![Run instrument](images/first_synth.gif)

