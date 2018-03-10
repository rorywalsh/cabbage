<Cabbage>
form caption("SVG Example") size(530, 480), colour("black"), pluginid("SMo1")
 groupbox bounds(122, 4, 376, 135), text("")
 groupbox bounds(120, 152, 379, 277), text(""), imgfile("custom_groupbox.svg"), identchannel("groupbox")
rslider bounds(18, 90, 87, 85) channel("Waveshape1") imgfile("Slider", "rslider.svg") imgfile("background", "rslider_background.svg"), range(0, 5, 0, 1, 1) trackercolour(255, 165, 0, 255) trackerthickness(.5),
 rslider bounds(18, 184, 90, 90), channel("Waveshape2"), imgfile("slider", "rslider.svg"), imgfile("background", "rslider_background.svg"), range(0, 1, 0), trackercolour("orange"), trackerthickness(0.4), textbox(1)
rslider bounds(18, 280, 90, 90) channel("Waveshape3") imgfile("Slider", "rslider.svg") imgfile("background", "rslider_background.svg"), range(0, 5, 0, 1, 1) trackercolour(255, 165, 0, 255) trackerthickness(.5),
 hslider bounds(150, 34, 323, 27), channel("hslider1"), imgfile("slider", "hslider.svg"), imgfile("background", "hslider_background.svg"), text("Param1"), range(0, 1, .5), trackercolour("orange"), textbox(1), gradient(0), trackerthickness(.2)
 hslider bounds(150, 64, 322, 27), channel("hslider2"), imgfile("slider", "hslider.svg"), imgfile("background", "hslider_background.svg"), text("Param2"), range(0, 1, .75), trackercolour("orange"), textbox(1), gradient(0), trackerthickness(.2)
 hslider bounds(150, 94, 322, 27), channel("hslider3"), imgfile("slider", "hslider.svg"), imgfile("background", "hslider_background.svg"), text("Param3"), range(0, 1, .25), trackercolour("orange"), textbox(1), gradient(0), trackerthickness(.2)
 vslider bounds(148, 192, 50, 220), channel("vslider1"), imgfile("slider", "vslider.svg"), imgfile("background", "vslider_background.svg"), trackercolour("darkorange"), textbox(1), range(0, 1, .4), text("vP.1"), trackerthickness(.125), gradient(0)
 vslider bounds(202, 192, 50, 220), channel("vslider2"), imgfile("slider", "vslider.svg"), imgfile("background", "vslider_background.svg"), trackercolour("darkorange"), textbox(1), range(0, 1, .24), text("vP.1"), trackerthickness(.125), gradient(0)
 vslider bounds(256, 192, 50, 220), channel("vslider3"), imgfile("slider", "vslider.svg"), imgfile("background", "vslider_background.svg"), trackercolour("darkorange"), textbox(1), range(0, 1, .64), text("vP.1"), trackerthickness(.125), gradient(0)
 vslider bounds(310, 192, 50, 220), channel("vslider4"), imgfile("slider", "vslider.svg"), imgfile("background", "vslider_background.svg"), trackercolour("darkorange"), textbox(1), range(0, 1, .34), text("vP.1"), trackerthickness(.125), gradient(0)
 vslider bounds(364, 192, 50, 220), channel("vslider5"), imgfile("slider", "vslider.svg"), imgfile("background", "vslider_background.svg"), trackercolour("darkorange"), textbox(1), range(0, 1, .14), text("vP.1"), trackerthickness(.125), gradient(0)
 vslider bounds(418, 192, 50, 220), channel("vslider6"), imgfile("slider", "vslider.svg"), imgfile("background", "vslider_background.svg"), trackercolour("darkorange"), textbox(1), range(0, 1, .4), text("vP.1"), trackerthickness(.125), gradient(0)
 button bounds(20, 6, 85, 35), channel("but1"), text("Disabled", "Enabled"), fontcolour:0("orange")
 button bounds(20, 46, 85, 35), channel("but2"), text("Disabled", "Enabled"), fontcolour:0("orange")
button bounds(20, 379, 85, 50) channel("but3") identchannel("svgIdent") fontcolour:0(255, 165, 0, 255) text(""), imgfile("On", "led_on.svg") imgfile("off", "led_off.svg"),
</Cabbage>  
<CsoundSynthesizer>
<CsOptions>
-d -n
</CsOptions>
<CsInstruments>
sr 		= 	44100	;SAMPLE RATE
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