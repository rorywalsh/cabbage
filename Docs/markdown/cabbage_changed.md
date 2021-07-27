# cabbageChanged Opcodes

This opcode takes an array of channel names and listens for a change. It reports a trigger value alongside the name of the channel that changed. The channels can hold strings or numbers. 


<blockquote style="font-style:italic;border-left:10px solid #93d200;color:rgb(3, 147, 210);padding:1px;padding-left:10px;margin-top:0px;margin-bottom:1px;border-left-width:0.25rem"> Added in Cabbage v2.7.12</blockquote>

### Syntax

<pre>SChannel, kTrig <b>cabbageChanged</b> SChannels[]</pre>


#### Initialization

* `SChannels[]` -- an array of all channel you wish to monitor for changes

#### Performance

* `SChannel` -- The channel was most recently changed
* `kTrig` -- will output a trigger value of 1 whenever a channel changes. 

### Example

```csharp
<Cabbage>
form caption("Cabbage Changed") size(430, 290) pluginId("tl01") guiMode("queue")
rslider bounds(10, 10, 100, 100), channel("slider1")
rslider bounds(110, 10, 100, 100), channel("slider2")
rslider bounds(210, 10, 100, 100), channel("slider3")
rslider bounds(310, 10, 100, 100), channel("slider4")
combobox bounds(20, 118, 80, 20) channel("combo1"), channelType("string")
combobox bounds(120, 118, 80, 20) channel("combo2"), channelType("string")
combobox bounds(220, 118, 80, 20) channel("combo3"), channelType("string")
combobox bounds(320, 118, 80, 20) channel("combo4"), channelType("string")
label bounds(8, 158, 412, 21) channel("label1"), align("left"), fontColour(0, 0, 0, 255) text("Most recently changed widget:")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -m0d -+rtmidi=NULL 
</CsOptions>
<CsInstruments>
ksmps   = 32  

instr 1

    SWidgetChannels[] cabbageGetWidgetChannels
    SUpdatedChannel, kTrig cabbageChanged SWidgetChannels
    cabbageSet kTrig, "label1", sprintfk("text(\"Last updated widget: %s\n\")", SUpdatedChannel)

endin

</CsInstruments>
<CsScore>
i1 0 z
</CsScore>
</CsoundSynthesizer>
```
