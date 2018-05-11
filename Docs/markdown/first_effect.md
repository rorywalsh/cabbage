# A basic Cabbage effect
Cabbage effects are used to process incoming audio. To do this we make use of the signal input opcodes. One can use either 'ins' or 'inch'. To create a new effect click the new file button and select audio effect as shown here.

![](images/new_effect_file.gif)

After you have named the new effect Cabage will generate a very simple instrument that takes an incoming stream of audio and outputs directly, without any modification or further processing. In order to do some processing we can add some Csound code the instrument. The code presented below is for a simple reverb unit. We assign the incoming sample data to two variables, i.e., aInL and aInR. We then process the incoming signal through the reverbsc opcode. Some GUI widgets have also been added to provide users with access to various parameter. See [First Synth](first_synth.html) if you are not sure about how to add GUI widgets.  

##Example
```html
<Cabbage>
form size(280, 160), caption("Simple Reverb"), pluginID("plu1")
groupbox bounds(20, 12, 233, 112), text("groupbox")    
rslider bounds(32, 40, 68, 70), channel("size"), range(0, 1, .2, 1, 0.001), text("Size"), colour(2, 132, 0, 255), 
rslider bounds(102, 40, 68, 70), channel("fco"), range(1, 22000, 10000, 1, 0.001), text("Cut-Off"), colour(2, 132, 0, 255), 
rslider bounds(172, 40, 68, 70), channel("gain"), range(0, 1, .5, 1, 0.001), text("Gain"), colour(2, 132, 0, 255), 
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d
</CsOptions>
<CsInstruments>
sr = 44100
ksmps = 64
nchnls = 2
0dbfs=1

instr 1
	kFdBack chnget "size"
	kFco chnget "fco"
	kGain chnget "gain"
	aInL inch 1
	aInR inch 2
	aOutL, aOutR reverbsc aInL, aInR, kFdBack, kFco
	outs aOutL*kGain, aOutR*kGain
endin

</CsInstruments>  
<CsScore>
f1 0 1024 10 1
i1 0 z
</CsScore>
</CsoundSynthesizer>
```

The above instrument uses 3 rsliders to control the reverb size(feedback level), the cut-off frequency, and overall gain. The range() identifier is used with each slider to specify the min, max and starting value of the sliders. 

>If you compare the two score sections in the synth and effect instruments, you'll notice that the synth instrument doesn't use any i-statement. Instead it uses an 'f0 z'. This tells Csound to wait for incoming events until the user kills it. Because the instrument is to be controlled via MIDI we don't need to use an i-statement in the score. In the second example we use an i-statement with a long duration so that the instrument runs without stopping for a long time.     

![](images/firstEffect.png)