
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; ModifyingWidgetAppearance.csd
; Written by Iain McCurdy, 2014
; Updated to use cabbageGet/Set opcodes Rory Walsh, 2021

; An example that demonstrates the ability to modify various attributes of a widget's appearance in realtime from within the Csound orchestra via Cabbage's own widgets.

<Cabbage>
form caption("Modifying Widget Appearance") size(800, 500), colour(0,0,0) guiMode("queue"), pluginId("TEx1")

image    bounds( 20,180, 50,11), colour(250,250,  0,255), channel("image1"), shape("square"), outlineThickness(36)
hslider  bounds(  0, 210, 450, 30), channel("HorPos"),  range(  0,450,160,1,1), textColour(white) colour(SlateGrey)
label    bounds(210,233, 35, 11), text("X Pos."), fontColour("white")
vslider  bounds(460,  0, 30, 200),  channel("VertPos"), range(  0,200,140,1,1), textColour(white) colour(SlateGrey)
label    bounds(484,100, 35, 11), text("Y Pos."), fontColour("white")
hslider  bounds( 10, 240, 390, 30), channel("SizeX"),   range(  1,450,150,1,1), textColour(white) colour(SlateGrey), text("Size X"), valueTextBox(1)
hslider  bounds( 10, 270, 390, 30), channel("SizeY"),   range(  1,450,120,1,1), textColour(white) colour(SlateGrey), text("Size Y"), valueTextBox(1)
combobox bounds(400,240, 100, 25), channel("VisibleInvisible"), value(2), text("Invisible", "Visible") fontColour(150,150,150)

groupbox   bounds( 10, 310,220,170) text("Main Object"), fontColour("white"), plant("main") 
{
combobox   bounds( 70,  30, 80, 20), channel("Shape"), text("square","circle"), value(1), channelType("string") fontColour(150,150,150)
hslider    bounds(  0,  50,220, 30), channel("Red"),     range(  0,255,250,1,1), textColour("red")   colour(red),   text("Red"),  trackerColour(red), valueTextBox(1)
hslider    bounds(  0,  80,220, 30), channel("Green"),   range(  0,255,  0,1,1), textColour("green") colour(green), text("Green"),  trackerColour(green), valueTextBox(1)
hslider    bounds(  0, 110,220, 30), channel("Blue"),    range(  0,255,  0,1,1), textColour("blue")  colour(blue),  text("Blue"),  trackerColour(blue), valueTextBox(1)
hslider    bounds(  0, 140,220, 30), channel("Alpha"),   range(  0,255,255,1,1), colour(SlateGrey), text("Alpha"), valueTextBox(1)
}

groupbox bounds(230,310,220,170) text("Line"), fontColour("white") 
{
hslider  bounds(  0,  20, 220, 30), channel("Line"),      range(0, 100, 36,1,1),  colour(SlateGrey), text("Thickness"), valueTextBox(1)
hslider  bounds(  0,  50, 220, 30), channel("LineRed"),   range(  0,255,  0,1,1), fontColour(red)   colour(red),   text("Red"),  trackerColour(red), valueTextBox(1)
hslider  bounds(  0,  80, 220, 30), channel("LineGreen"), range(  0,255,255,1,1), fontColour(green) colour(green), text("Green"),  trackerColour(green), valueTextBox(1)
hslider  bounds(  0, 110, 220, 30), channel("LineBlue"),  range(  0,255,  0,1,1), fontColour(blue)  colour(blue),  text("Blue"),  trackerColour(blue), valueTextBox(1)
hslider  bounds(  0, 140, 220, 30), channel("LineAlpha"), range(  0,255,255,1,1), colour(SlateGrey), text("Alpha"), valueTextBox(1)
}

groupbox bounds(450,320,220, 50) text("Corners"), fontColour("white") 
{
hslider  bounds(  0,  20, 220, 30), channel("cornersVal"), range(0, 300, 10,1,1), colour(SlateGrey), text("Corners"), valueTextBox(1)
}

groupbox bounds(450,370,220,110) text("Rotation"), fontColour("white") 
{
hslider  bounds(  0,  20, 220, 30), channel("radians"), range(0, 6.283, 0,1,0.001),  colour(SlateGrey), text("Radians")
hslider  bounds(  0,  50, 220, 30), channel("pivotx"),  range(0, 1, 0.5,1,0.01),     colour(SlateGrey), text("Pivot X"), valueTextBox(1)
hslider  bounds(  0,  80, 220, 30), channel("pivoty"),  range(0, 1, 0.5,1,0.01),     colour(SlateGrey), text("Pivot Y"), valueTextBox(1)
}

texteditor bounds(520,  5, 269, 315), readOnly(1), wrap(1) colour("black"), fontColour("white") channel("textInfo")
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
SText = {{Widget attributes are set using 'identifiers' such as text(), bounds() etc. Identifiers can be updated from the Csound orchestra using the cabbageSet opcode. For this to work you must assign a channel name to each widget you wish to control in real time. 

In this example various attributes of a 'image' widget can be modified using the on-screen sliders comboboxes and buttons. For the sake of efficiency widgets are only sent updates when the sliders are moved. We can detect changes in the sliders using triggers generated by the cabbageGetValue opcode, which will retrieve the value of the slider.
}}

   cabbageSet "textInfo", "text", SText


  ; Change size and position (bounds)
  kHorPos		chnget	"HorPos"
  kVertPos	    chnget	"VertPos"
  kSizeX		chnget	"SizeX"
  kSizeY		chnget	"SizeY"
  
  if changed(int(kHorPos))==1||changed(int(kVertPos))==1||changed(int(kSizeX))==1||changed(int(kSizeY))==1 then
    cabbageSet 1, "image1", "bounds", kHorPos, 200-kVertPos, kSizeX, kSizeY
  endif
  
  ; Change shape
  SShape, kShapeTrig	cabbageGet	"Shape"
  printf SShape, kShapeTrig
  cabbageSet kShapeTrig, "image1", "shape", SShape

  ; Visibility
  kVisibleInvisible, kVisChanged cabbageGet "VisibleInvisible"
  cabbageSet kVisChanged, "image1", "visible", kVisibleInvisible-1
  
  ; Change line thickness
  kLine, kLineChanged cabbageGet "Line"
  cabbageSet kLineChanged, "image1", "outlineThickness", kLine

  ; Change corner size
  kcornersVal, kcornersValChanged cabbageGet "cornersVal"
  cabbageSet kcornersValChanged, "image1", "corners", kcornersVal
 
  
  ; Change line colour
  kLineRed	chnget	"LineRed"
  kLineGreen	chnget	"LineGreen"
  kLineBlue	chnget	"LineBlue"
  kLineAlpha	chnget	"LineAlpha" 
  
  if changed(int(kLineRed))==1||changed(int(kLineGreen))==1||changed(int(kLineBlue))==1||changed(int(kLineAlpha))==1 then
   cabbageSet 1, "image1", "outlineColour", kLineRed, kLineGreen, kLineBlue, kLineAlpha
  endif
  
  ; Change colour
  kRed		chnget	"Red"
  kGreen		chnget	"Green"
  kBlue		chnget	"Blue"
  kAlpha		chnget	"Alpha" 
  
  if changed(int(kRed))==1||changed(int(kGreen))==1||changed(int(kBlue))==1||changed(int(kAlpha))==1 then
   cabbageSet 1, "image1", "colour", kRed, kGreen, kBlue, kAlpha
  endif
  
  ; Change rotation
  kradians		chnget	"radians"
  kpivotx		chnget	"pivotx"
  kpivoty		chnget	"pivoty"
  if changed(kradians,kpivotx,kpivoty)==1 then
   cabbageSet 1, "image1", "rotate", kradians, kpivotx*kSizeX, kpivoty*kSizeY
  endif


endin
	
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>