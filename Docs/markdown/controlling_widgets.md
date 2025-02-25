# Controlling Widgets from Csound
All widget identifiers can be updated in real-time if a channel has been assigned using the channel identifier. Once a channel has been set up, any of the widget's identifiers be accessed or updated from Csound using the `cabbageGet`, or `cabbageSet` (see [Cabbage opcodes](./cabbage_opcodes.md)). For example, if one sets up a checkbox with a channel called "checkbox1", its bounds can be controlled from Csound.

```csharp
cabbageSet metro(10) "checkbox1", "bounds", 10, 10, 100, 100
```

k-rate variables can be ued to control the position of a checkbox in real-time. In this example random values are used to create new bounds 5 times a second. **metro(5)**

```csharp
<Cabbage>
form size(600, 200), caption("Random Checkbox"), guiMode("queue"), pluginID("plu1")
checkbox bounds(0, 0, 50, 50), corners(10), channel("checkbox1"), colour:0("lime")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d
</CsOptions>
<CsInstruments>
ksmps = 64
nchnls = 2
0dbfs=1
  
instr 1
	kXPos randi 600, 1, 2
	kYPos randi 200, 1, 2
	cabbageSet metro(5), "checkbox1", "bounds", abs(kXPos), abs(kYPos), 70, 30
endin

</CsInstruments>  
<CsScore> 
i1 0 1000
</CsScore>
</CsoundSynthesizer> 
```
The above code creates this type of effect.

![Random Movement](images/random_combo.gif)

Channels can also be used with [Plants](./plants.md). In the following example, 3 plants, containing 4 hsliders each are placed on top of each other. One has visible() set to 1, while the others have `visible()` set to 0. When the instrument first launches, the users sees only one of the slider plants. As the user clicks on the button marked toggle, they can show and hide the other plants. 

![Random Movement](images/toggling_plants.gif)

```csharp
<Cabbage>
form size(400, 200), caption("Toggling Plants"), guiMode("queue") pluginId("plu1"), guiRefresh(10)
button bounds(12, 12, 60, 25), channel("but1"), text("Toggle", "Toggle")

groupbox bounds(80, 10, 200, 160), text("White Sliders"), visible(0), channel("sliders1"), plant("GUIabst_1"){
hslider bounds(0, 30, 200, 30), channel("hslider1"), range(0, 100, 55), colour("white")
hslider bounds(0, 60, 200, 30), channel("hslider2"), range(0, 100, 23), colour("white")
hslider bounds(0, 90, 200, 30), channel("hslider3"), range(0, 100, 45), colour("white")
hslider bounds(0, 120, 200, 30), channel("hslider4"), range(0, 100, 56), colour("white")
}

groupbox bounds(80, 10, 200, 160), text("Red Sliders"), visible(0), channel("sliders2"), plant("GUIabst_2"){
hslider bounds(0, 30, 200, 30), channel("hslider5"), range(0, 100, 25), colour("red")
hslider bounds(0, 60, 200, 30), channel("hslider6"), range(0, 100, 63), colour("red")
hslider bounds(0, 90, 200, 30), channel("hslider7"), range(0, 100, 15), colour("red")
hslider bounds(0, 120, 200, 30), channel("hslider8"), range(0, 100, 16), colour("red")
}

groupbox bounds(80, 10, 200, 160), text("Yellow Sliders"), visible(1), channel("sliders3"), plant("GUIabst_3"){
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
	
    cabbageSet kTrig, "sliders1", "visible", kCnt==1 ? 1 : 0
    cabbageSet kTrig, "sliders2", "visible", kCnt==2 ? 1 : 0
    cabbageSet kTrig, "sliders3", "visible", kCnt==3 ? 1 : 0
		
	if kTrig==1 then
	    kCnt = (kCnt>2 ? 1 : kCnt+1)
	endif
endin


</CsInstruments>  
<CsScore>
f1 0 2048 10 1 
i1 0 1000
</CsScore>
</CsoundSynthesizer>
```

While Cabbage comes with several fun animation examples, please keep in mind that it is not a framework for developing animations! The more widgets you manipulate, and the speed at which you modify them at will inevitably cause a drain on the CPU. The key here is to only update your widget's when it is absolutely required.

> The above code example can be found in the 'Miscellaneous' example file menu in Cabbage. 