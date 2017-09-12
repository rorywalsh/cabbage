# Controlling Widgets from Csound
All widget identifiers can be updated in real-time if an identifier channel has been set up using an identchannel() identifier. Once an identifier channel has been set up, one can update a widget's identifier using a chnset opcode in Csound, with the appropriate ident-channel string. For example, if one sets up a checkbox with an identifier channel "checkboxIdent", its position can be controlled using Csound and a chnset opcode.

```csharp
chnset "bounds(10, 10, 100, 100)", "checkboxIdent"
```

> Note that only one identChannel string can be sent in a single k-cycle. This is important. If two **chnset** opcodes are used in the same instrument to send an identifier string to a widget, the second will overwrite the first. If you wish to update several attributes of a widget place them in the same string or concatenate each attribute into a single string.   

To control the position of a checkbox in real-time, one can use the **sprintfk** opcode to dynamically create an identifier string that can be passed to the widget. In this example random values are passed to sprintfk to create a new bounds() strings 5 times a second. **metro(5)**

```csharp
<Cabbage>
form size(600, 200), caption("Rnadom Checkbox"), pluginID("plu1")
checkbox bounds(0, 0, 50, 50), identchannel("checkboxIdent")
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
	kXPos randi 600, 1, 2
	kYPos randi 200, 1, 2
	if metro(5)==1 then
		SIdentifier sprintfk "bounds(%d, %d, 50, 50)", abs(kXPos), abs(kYPos)
		chnset SIdentifier, "checkboxIdent"
	endif
endin
</CsInstruments>  
<CsScore> 
i1 0 1000
i2 0 1000
</CsScore>
</CsoundSynthesizer> 
```
The above code creates this type of effect.

![Random Movement](images/random_combo.gif)

Identifier channels can also be used with [Plants](./plants.md). In the following example, 3 plants, containing 4 hsliders each are placed on top of each other. One has visible() set to 1, while the others have visible() set to 0. When the instrument first launches, the users sees only one of the slider plants. As the user clicks on the button marked toggle, they can show and hide the other plants. 

```csharp
<Cabbage>
form size(400, 200), caption("Toggling Plants"), pluginID("plu1"), guirefresh(10)
button bounds(12, 12, 60, 25), channel("but1"), text("Toggle", "Toggle")

groupbox bounds(80, 10, 200, 160), text("White Sliders"), visible(0), identchannel("sliders1"), plant("GUIabst_1"){
hslider bounds(0, 30, 200, 30), channel("hslider1"), range(0, 100, 55), colour("white")
hslider bounds(0, 60, 200, 30), channel("hslider2"), range(0, 100, 23), colour("white")
hslider bounds(0, 90, 200, 30), channel("hslider3"), range(0, 100, 45), colour("white")
hslider bounds(0, 120, 200, 30), channel("hslider4"), range(0, 100, 56), colour("white")
}

groupbox bounds(80, 10, 200, 160), text("Red Sliders"), visible(0), identchannel("sliders2"), plant("GUIabst_2"){
hslider bounds(0, 30, 200, 30), channel("hslider5"), range(0, 100, 25), colour("red")
hslider bounds(0, 60, 200, 30), channel("hslider6"), range(0, 100, 63), colour("red")
hslider bounds(0, 90, 200, 30), channel("hslider7"), range(0, 100, 15), colour("red")
hslider bounds(0, 120, 200, 30), channel("hslider8"), range(0, 100, 16), colour("red")
}

groupbox bounds(80, 10, 200, 160), text("Yellow Sliders"), visible(1), identchannel("sliders3"), plant("GUIabst_3"){
hslider bounds(0, 30, 200, 30), channel("hslider9"), range(0, 100, 5), colour("yellow")
hslider bounds(0, 60, 200, 30), channel("hslider10"), range(0, 100, 63), colour("yellow")
hslider bounds(0, 90, 200, 30), channel("hslider11"), range(0, 100, 25), colour("yellow")
hslider bounds(0, 120, 200, 30), channel("hslider12"), range(0, 100, 96), colour("yellow")
}

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
	kCnt init 1
	kTrig changed chnget:k("but1") 

	if kTrig==1 then
		Smessage sprintfk "visible(%d)", kCnt==1 ? 1 : 0
		chnset Smessage, "sliders1"
		Smessage sprintfk "visible(%d)", kCnt==2 ? 1 : 0
		chnset Smessage, "sliders2"
		Smessage sprintfk "visible(%d)", kCnt==3 ? 1 : 0
		chnset Smessage, "sliders3"
		kCnt = (kCnt>2 ? 1 : kCnt+1)
		printk2 kCnt
	endif
	
endin


</CsInstruments>  
<CsScore>
f1 0 2048 10 1 
i1 0 1000
</CsScore>
</CsoundSynthesizer>
```

While Cabbage comes with several fun animation examples, please keep in mind that it is not a framework for developing animations! The more widgets you manipulate, and the speed at which you modify them at will inevitably cause a drain on the CPU. The key here is to only call chnset when it is absolutely required.

> The above code examples can be found in the 'Instructional' that ship with Cabbage. 