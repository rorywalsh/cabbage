<Cabbage>
form caption("SVG Example") size(530, 480), colour("black"), pluginId("SVGE")
 groupbox bounds(122, 4, 376, 135), text("")
 groupbox bounds(120, 152, 379, 277), text(""), imgFile("custom_groupbox.svg"), identChannel("groupbox")
rslider bounds(18, 90, 87, 85) channel("Waveshape1") imgFile("Slider", "rslider.svg") imgFile("background", "rslider_background.svg"), range(0, 5, 0, 1, 1) trackerColour(255, 165, 0, 255) trackerThickness(.5),
 rslider bounds(18, 184, 90, 90), channel("Waveshape2"), imgFile("slider", "rslider.svg"), imgFile("background", "rslider_background.svg"), range(0, 1, 0), trackerColour("orange"), trackerThickness(0.4), textBox(1)
rslider bounds(18, 280, 90, 90) channel("Waveshape3") imgFile("Slider", "rslider.svg") imgFile("background", "rslider_background.svg"), range(0, 5, 0, 1, 1) trackerColour(255, 165, 0, 255) trackerThickness(.5),
 hslider bounds(150, 34, 323, 27), channel("hslider1"), imgFile("slider", "hslider.svg"), imgFile("background", "hslider_background.svg"), text("Param1"), range(0, 1, .5), trackerColour("orange"), textBox(1), gradient(0), trackerThickness(.2)
 hslider bounds(150, 64, 322, 27), channel("hslider2"), imgFile("slider", "hslider.svg"), imgFile("background", "hslider_background.svg"), text("Param2"), range(0, 1, .75), trackerColour("orange"), textBox(1), gradient(0), trackerThickness(.2)
 hslider bounds(150, 94, 322, 27), channel("hslider3"), imgFile("slider", "hslider.svg"), imgFile("background", "hslider_background.svg"), text("Param3"), range(0, 1, .25), trackerColour("orange"), textBox(1), gradient(0), trackerThickness(.2)
 vslider bounds(148, 192, 50, 220), channel("vslider1"), imgFile("slider", "vslider.svg"), imgFile("background", "vslider_background.svg"), trackerColour("darkorange"), textBox(1), range(0, 1, .4), text("vP.1"), trackerThickness(.125), gradient(0)
 vslider bounds(202, 192, 50, 220), channel("vslider2"), imgFile("slider", "vslider.svg"), imgFile("background", "vslider_background.svg"), trackerColour("darkorange"), textBox(1), range(0, 1, .24), text("vP.1"), trackerThickness(.125), gradient(0)
 vslider bounds(256, 192, 50, 220), channel("vslider3"), imgFile("slider", "vslider.svg"), imgFile("background", "vslider_background.svg"), trackerColour("darkorange"), textBox(1), range(0, 1, .64), text("vP.1"), trackerThickness(.125), gradient(0)
 vslider bounds(310, 192, 50, 220), channel("vslider4"), imgFile("slider", "vslider.svg"), imgFile("background", "vslider_background.svg"), trackerColour("darkorange"), textBox(1), range(0, 1, .34), text("vP.1"), trackerThickness(.125), gradient(0)
 vslider bounds(364, 192, 50, 220), channel("vslider5"), imgFile("slider", "vslider.svg"), imgFile("background", "vslider_background.svg"), trackerColour("darkorange"), textBox(1), range(0, 1, .14), text("vP.1"), trackerThickness(.125), gradient(0)
 vslider bounds(418, 192, 50, 220), channel("vslider6"), imgFile("slider", "vslider.svg"), imgFile("background", "vslider_background.svg"), trackerColour("darkorange"), textBox(1), range(0, 1, .4), text("vP.1"), trackerThickness(.125), gradient(0)
 button bounds(20, 6, 85, 35), channel("but1"), text("Disabled", "Enabled"), fontColour:0("orange")
 button bounds(20, 46, 85, 35), channel("but2"), text("Disabled", "Enabled"), fontColour:0("orange")
button bounds(20, 379, 85, 50) channel("but3") identChannel("svgIdent") fontColour:0(255, 165, 0, 255) text(""), imgFile("On", "led_on.svg") imgFile("off", "led_off.svg"),
</Cabbage>  
<CsoundSynthesizer>
<CsOptions>
-d -n
</CsOptions>
<CsInstruments>
;sr is set by the host
ksmps 		= 	32	;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls 		= 	2	;NUMBER OF CHANNELS (2=STEREO)
0dbfs		=	1

instr	1
if metro(1)==1 then
	kRand random 0, 10000
	chnset sprintfk("svgdebug(%d)", kRand), "svgIdent"
	chnset sprintfk("svgdebug(%d)", kRand), "groupbox"	
endif
endin
		
</CsInstruments>
<CsScore>
i 1 0 [3600*24*7]
</CsScore>
</CsoundSynthesizer>