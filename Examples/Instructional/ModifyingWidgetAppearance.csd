; ModifyingWidgetAppearance.csd
; Written by Iain McCurdy, 2014

; An example that demonstrates the ability to modify various attributes of a widget's appearance in realtime from within the Csound orchestra via Cabbage's own widgets.

<Cabbage>
form caption("Modifying Widget Appearance") size(800, 500), pluginID("TEx1"), guirefresh(16)
image bounds( 20,180, 50,11), colour(250,250,  0,255), shape("sharp"), outlinecolour("white"), outlinethickness(2), identchannel("image1"), rotate(0, 0, 0)
hslider bounds(  0, 210, 450, 30), channel("HorPos"),  range(  0,450, 20,1,1), fontcolour(white) colour(SlateGrey)
label      bounds(210,233, 35, 11), text("X Pos."), fontcolour("white")
vslider bounds(460,  0, 30, 200),  channel("VertPos"), range(  0,200,20,1,1), fontcolour(white) colour(SlateGrey)
label      bounds(484,100, 35, 11), text("Y Pos."), fontcolour("white")
hslider bounds( 10, 240, 390, 30), channel("SizeX"),   range(  1,450, 50,1,1), fontcolour(white) colour(SlateGrey), text("Size X")
hslider bounds( 10, 270, 390, 30), channel("SizeY"),   range(  1,450, 11,1,1), fontcolour(white) colour(SlateGrey), text("Size Y")
combobox bounds(400,240, 100, 25), channel("VisibleInvisible"), value(2), text("Invisible", "Visible")

groupbox   bounds( 10, 310,220,170) text("Main Object"), plant("main") {
combobox   bounds( 70,  30, 80, 20), channel("Shape"), items("rounded", "sharp", "ellipse"), value(1), channeltype("string")
hslider    bounds(  0,  50,220, 30), channel("Red"),     range(  0,255,250,1,1), fontcolour(red)   colour(red),   text("Red"),  trackercolour(red)
hslider    bounds(  0,  80,220, 30), channel("Green"),   range(  0,255,250,1,1), fontcolour(green) colour(green), text("Green"),  trackercolour(green)
hslider    bounds(  0, 110,220, 30), channel("Blue"),    range(  0,255,  0,1,1), fontcolour(blue)  colour(blue),  text("Blue"),  trackercolour(blue)
hslider    bounds(  0, 140,220, 30), channel("Alpha"),   range(  0,255,255,1,1), fontcolour(white) colour(SlateGrey), text("Alpha")
}

groupbox bounds(230,310,220,170) text("Line"), plant("line") {
hslider  bounds(  0,  20, 220, 30), channel("Line"), range(0, 100, 0,1,1), fontcolour(white) colour(SlateGrey), text("Thick")
hslider  bounds(  0,  50, 220, 30), channel("LineRed"),     range(  0,255,250,1,1), fontcolour(red)   colour(red),   text("Red"),  trackercolour(red)
hslider  bounds(  0,  80, 220, 30), channel("LineGreen"),   range(  0,255,250,1,1), fontcolour(green) colour(green), text("Green"),  trackercolour(green)
hslider  bounds(  0, 110, 220, 30), channel("LineBlue"),    range(  0,255,  0,1,1), fontcolour(blue)  colour(blue),  text("Blue"),  trackercolour(blue)
hslider  bounds(  0, 140, 220, 30), channel("LineAlpha"),   range(  0,255,255,1,1), fontcolour(white) colour(SlateGrey), text("Alpha")
}

groupbox bounds(450,370,220,110) text("Rotation"), plant("rotatation") {
hslider  bounds(  0,  20, 220, 30), channel("radians"), range(0, 6.283, 0,1,0.001), fontcolour(white) colour(SlateGrey), text("Radians")
hslider  bounds(  0,  50, 220, 30), channel("pivotx"),  range(0, 1, 0,1,0.001),     fontcolour(white) colour(SlateGrey), text("Pivot X")
hslider  bounds(  0,  80, 220, 30), channel("pivoty"),  range(0, 1, 0,1,0.001),     fontcolour(white) colour(SlateGrey), text("Pivot Y")
}

image bounds(525,10,265,310), colour( 10, 10, 15), shape("sharp"), outlinecolour("white"), line(0), plant("instructions"){
label      bounds(  5, 5, 255, 12),  fontcolour(white), align(left), text("It is possible to modify various aspects of a ")
label      bounds(  5,20, 255, 12),  fontcolour(white), align(left), text("widget's appearance during Csound  ")
label      bounds(  5,35, 255, 12),  fontcolour(white), align(left), text("performance. This is achieved through the ")
label      bounds(  5,50, 255, 12),  fontcolour(white), align(left), text("use of widget's 'identchannel' - a name given")
label      bounds(  5,65, 255, 12),  fontcolour(white), align(left), text("to the widget to identify it from within the ")
label      bounds(  5,80, 255, 12),  fontcolour(white), align(left), text("Csound orchestra. The widget is sent an ")
label      bounds(  5,95, 255, 12),  fontcolour(white), align(left), text("instruction as a string variable  ")
label      bounds(  5,110, 255, 12), fontcolour(white), align(left), text("corresponding to the change in appearance")
label      bounds(  5,125, 255, 12), fontcolour(white), align(left), text("we desire. This message is sent using the)
label      bounds(  5,140, 255, 12), fontcolour(white), align(left), text("'chnset' opcode and the channel used is the ")
label      bounds(  5,155, 255, 12), fontcolour(white), align(left), text("'identchannel' name we defined previously ")
label      bounds(  5,170, 255, 12), fontcolour(white), align(left), text("for the widget.")
label      bounds(  5,190, 255, 12), fontcolour(white), align(left), text("In this example various attributes of an")
label      bounds(  5,205, 255, 12), fontcolour(white), align(left), text("'image' widget can be modified using the  ")
label      bounds(  5,220, 255, 12), fontcolour(white), align(left), text("on-screen sliders comboboxes and buttons.")
label      bounds(  5,235, 255, 12), fontcolour(white), align(left), text("For the sake of efficiency widgets are only")
label      bounds(  5,250, 255, 12), fontcolour(white), align(left), text("sent messages when the sliders are moved.")
label      bounds(  5,265, 255, 12), fontcolour(white), align(left), text("We can detect changes in the sliders using")
label      bounds(  5,280, 255, 12), fontcolour(white), align(left), text("triggers generated by the 'changed' opcode.")
}

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

 if metro(20)==1 then
  ; Change size and position (bounds)
  kHorPos		chnget	"HorPos"
  kVertPos	chnget	"VertPos"
  kSizeX		chnget	"SizeX"
  kSizeY		chnget	"SizeY"
  if changed(int(kHorPos))==1||changed(int(kVertPos))==1||changed(int(kSizeX))==1||changed(int(kSizeY))==1 then
    Smessage sprintfk "bounds(%d,%d,%d,%d)", kHorPos, 200-kVertPos, kSizeX, kSizeY
    chnset Smessage, "image1"
  endif
  
  ; Change shape
  SShape	chnget	"Shape"
  if changed(SShape)==1 then
   Smessage	sprintfk	"shape(%s)", SShape
   chnset	Smessage, "image1"
  endif

  ; Visibility
  kVisibleInvisible	chnget	"VisibleInvisible"
  if changed(kVisibleInvisible)==1 then
   Smessage	sprintfk	"visible(%d)", kVisibleInvisible-1
   chnset	Smessage, "image1"
  endif
  
  ; Change line thickness
  kLine	chnget	"Line"
  if changed(kLine)==1 then
    Smessage	sprintfk	"outlinethickness(%d)",kLine
    chnset Smessage, "image1"
  endif
  
  ; Change line colour
  kLineRed	chnget	"LineRed"
  kLineGreen	chnget	"LineGreen"
  kLineBlue	chnget	"LineBlue"
  kLineAlpha	chnget	"LineAlpha" 
  if changed(int(kLineRed))==1||changed(int(kLineGreen))==1||changed(int(kLineBlue))==1||changed(int(kLineAlpha))==1 then
   Smessage sprintfk "outlinecolour(%d,%d,%d,%d)", kLineRed, kLineGreen, kLineBlue, kLineAlpha
   chnset Smessage, "image1"
  endif
  
  ; Change colour
  kRed		chnget	"Red"
  kGreen		chnget	"Green"
  kBlue		chnget	"Blue"
  kAlpha		chnget	"Alpha" 
  if changed(int(kRed))==1||changed(int(kGreen))==1||changed(int(kBlue))==1||changed(int(kAlpha))==1 then
   Smessage sprintfk "colour(%d,%d,%d,%d)", kRed, kGreen, kBlue, kAlpha
   chnset Smessage, "image1"
  endif
  
  ; Change rotation
  kradians		chnget	"radians"
  kpivotx		chnget	"pivotx"
  kpivoty		chnget	"pivoty"
  if changed(kradians)==1||changed(kpivotx)==1||changed(kpivoty)==1 then
   Smessage sprintfk "rotate(%f,%d,%d)", kradians, kpivotx*kSizeX, kpivoty*kSizeY
   chnset Smessage, "image1"
  endif
 endif

endin
	
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>