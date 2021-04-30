# Plants

Cabbage plants are simply groups of widgets that are bound to a parent widget. All widgets contained within a plant have top and left positions which are relative the the top left position of the parent. Resizing the plant will in turn resize all the widgets contained within. As of Cabbage version 2, plants can now be placed within plants, but note that dynamically resizing plants within plants using the GUI editor may lead to unpredictable behaviour.  

Plants are created by enclosing a group of widgets within curly brackets. In the code below an image is set up as a plant, or parent widget. The plant has three child sliders, which all use relative coordinates. For example, the first checkbox will appear 20 pixels from the rightmost edge of the image, and 4 pixels from the topmost edge. 

```csharp
image bounds(6, 0, 300, 200), colour(255, 0, 0, 255)
{
checkbox bounds(20, 4, 121, 38), channel("mute1"), text("Mute Channel 1")
checkbox bounds(20, 44, 119, 38), channel("mute2"), text("Mute Channel 2")
combobox bounds(20, 100, 155, 30), channel("waveform"), value(2), text("Sine", "Square", "Sawtooth") 
}
``` 

The major advantage in using plants is that you can easily move and resize all widgets in one action. If you wish to dynamically change the position or visibility of a plant at run-time, you must give the image or groupbox a channel. Then you can call `cabbageSet` to update it. In the code provided below, a button is used to toggle the visibility of several plants. 

![New file](images/toggling_plants.gif)

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

> Note: Plants have to be created manually. Opening brackets must appear on either the same line as the plant widget, or the following line. Plants cannot be declared inline, meaning they must be spread over a number of lines. 
