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