<Cabbage>
form size(410, 330), caption("Text Editor"), pluginid("TxEd")
texteditor bounds(12, 34, 387, 20), channel("texteditor"), text("Type here...."), color(20, 20, 20), fontcolour(0, 0, 0, 255), 
csoundoutput bounds(12, 60, 387, 149)
label bounds(12, 8, 387, 20), text("Enter some text and hit enter"), align("centre"),  colour(60, 0, 0), fontcolour(255, 255, 255, 255)
label bounds(12, 208, 387, 20), text("Pressing the up/down keys when the"), align("centre"), visible(0) colour(60, 0, 0), identchannel("label2")
label bounds(12, 230, 387, 20), text("editor is focused toggles through the"), align("centre"), visible(0), colour(60, 0, 0), identchannel("label3")
label bounds(12, 252, 387, 20), text("previous entries."), align("centre"),  colour(60, 0, 0), visible(0), identchannel("label4")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 
</CsOptions>
<CsInstruments>
;sr is set by the host
ksmps = 64
nchnls = 2
0dbfs = 1
;Rory Walsh, 2014

;this demo shows how the texeditor widget can be used to
;send data to Csound while it's running. Hitting enter will
;cause a string message ot be sent to Csound on a named channel.

instr 1
kShowExtraText init 1
prints "The text you type into the texteditor will be output here:\n"
SText chnget "texteditor"
if changed(SText)==1 then 
	SMessage sprintfk "%s\n", SText
	printks SMessage, 1
	if kShowExtraText==0 then
		chnset "visible(1)", "label1"
		chnset "visible(1)", "label2"
		chnset "visible(1)", "label3"
		chnset "visible(1)", "label4"
	endif
	kShowExtraText=0
endif

endin

</CsInstruments>  
<CsScore>
i1 0 3600
</CsScore>
</CsoundSynthesizer>