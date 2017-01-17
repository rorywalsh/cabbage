<Cabbage>
form caption("SVG Example") size(530, 480), colour("black"), pluginID("SMo1"), svgpath(".")
 groupbox bounds(122, 4, 376, 135), text("")
 groupbox bounds(120, 152, 379, 277), text(""), svgfile("custom_groupbox.svg"), identchannel("groupbox")
 rslider bounds(18, 90, 90, 90), channel("Waveshape"), range(0, 5, 0, 1, 1), trackercolour("orange"), text("rSlider"), trackercolour(0,0,0,0), trackerthickness(.5)
 rslider bounds(18, 184, 90, 79), channel("rslider"), range(0, 1, 0), trackercolour("darkorange"), trackerthickness(0.4), textbox(1)
 hslider bounds(150, 34, 323, 27), channel("vslider1"), text("Param1"), range(0, 1, .5), trackercolour("orange"), textbox(1), gradient(0), trackerthickness(.2)
 hslider bounds(150, 64, 322, 27), channel("vslider2"), text("Param2"), range(0, 1, .75), trackercolour("orange"), textbox(1), gradient(0), trackerthickness(.2)
 hslider bounds(150, 94, 322, 27), channel("vslider3"), text("Param3"), range(0, 1, .25), trackercolour("orange"), textbox(1), gradient(0), trackerthickness(.2)
 vslider bounds(148, 192, 50, 220), channel("vslider1"), trackercolour("darkorange"), textbox(1), range(0, 1, .4), text("vP.1"), trackerthickness(.125), gradient(0)
 vslider bounds(202, 192, 50, 220), channel("vslider2"), trackercolour("darkorange"), textbox(1), range(0, 1, .24), text("vP.1"), trackerthickness(.125), gradient(0)
 vslider bounds(256, 192, 50, 220), channel("vslider3"), trackercolour("darkorange"), textbox(1), range(0, 1, .64), text("vP.1"), trackerthickness(.125), gradient(0)
 vslider bounds(310, 192, 50, 220), channel("vslider4"), trackercolour("darkorange"), textbox(1), range(0, 1, .34), text("vP.1"), trackerthickness(.125), gradient(0)
 vslider bounds(364, 192, 50, 220), channel("vslider5"), trackercolour("darkorange"), textbox(1), range(0, 1, .14), text("vP.1"), trackerthickness(.125), gradient(0)
 vslider bounds(418, 192, 50, 220), channel("vslider6"), trackercolour("darkorange"), textbox(1), range(0, 1, .4), text("vP.1"), trackerthickness(.125), gradient(0)
 button bounds(20, 6, 85, 35), channel("but1"), text("Disabled", "Enabled"), fontcolour:0("orange")
 button bounds(20, 46, 85, 35), channel("but2"), text("Disabled", "Enabled"), fontcolour:0("orange")
 rslider bounds(18, 264, 90, 90), channel("Waveshape"), range(0, 5, 0, 1, 1), trackercolour("orange"), text("rSlider"), trackercolour(0,0,0,0), trackerthickness(.5)
 button bounds(20, 358, 85, 76), channel("but3"), text(""), identchannel("svgIdent"), fontcolour:0("orange"), svgfile("buttonoff", "led_off.svg"), svgfile("buttonon", "led_on.svg"), 
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