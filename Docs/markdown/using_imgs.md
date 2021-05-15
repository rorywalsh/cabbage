# Using Images with widgets

Cabbage widgets can be use custom graphics such as PNGs, JPEGs or SVGs. Using custom graphics can help you create unique and original themes for your instruments. While SVGs may not provide the same level of detail as a high-res PNG, they are scalable without any loss of data. This reduces the need to provide several copies of a single image at various sizes and resolutions. This also means your SVGs can easily be used on a range of instruments without having to be modified. Filmstrips (see below) must be in PNG format.  

> Note that the SVG parser does not support the entire SVG specification. Basic shapes/lines are rendered perfectly. Text is rendered so long as you convert it to a path before savings your SVG. SVG Filters are not yet supported.  

The **imgFile()** identifier lets you attach a unique image file to a widget. Many widgets take multiple `imgFile()` identifiers. For example, buttons take an `imgFile()` for the on image, and the off image. 

##Example
The example below show how **imgFile()** identifiers can be used. 

```csharp
<Cabbage>
form caption("SVG Example") size(530, 480), colour("black"), pluginID("SMo1")
 groupbox bounds(122, 4, 376, 135), text("")
 groupbox bounds(120, 152, 379, 277), text(""), imgFile("custom_groupbox.svg")
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
button bounds(20, 379, 85, 50) channel("but3") fontColour:0(255, 165, 0, 255) text(""), imgFile("On", "led_on.svg") imgFile("off", "led_off.svg"),
</Cabbage>  
```

![](images/svgExample.gif)

## Filmstrip images

Filmstrip images are typically used for widgets that have many states, such as a slider. In many ways they are like sprite-sheets; they contain X amount of individual frames. These frames that are called up according to the state the widget is in. When a slider is at its minimum value, frame 1 will show. When the slider is at max value the last frame will show. To add a filmstrip to a slider use the filmstrip() identifier. 

**filmstrip("filename", numFrames)** 

If you do not set the correct number of frames, you slider's image is likely to jump around the place as you move it. For move information on the filmstrip() identifier, see the manual entry for [Slider](./sliders.md).

![](images/filmstrips.png)

```csharp
<Cabbage>
form caption("Filmstrips") size(380, 140), pluginId("def1")
groupbox bounds(6, 4, 365, 120) text("Filmstrip Sliders")
rslider bounds(18, 30, 80, 80), channel("gain1"), range(-60, 100, 15, .5, 0.01), text("Gain"), filmstrip("rSliderFilmStrip.png", 111, "vertical"), valueTextBox(1) 
rslider bounds(104, 30, 80, 80), channel("gain3"), range(0, 1, 0.48, 1, 0.01), text("Gain"), filmstrip("rSliderFilmStrip.png", 111)
rslider bounds(190, 30, 80, 80), channel("gain5"), range(0, 1, 0.48, 1, 0.01), filmstrip("rSliderFilmStrip.png", 111)
rslider bounds(276, 30, 80, 80), channel("gain7"), range(0, 1, 0.48, 1, 0.01) filmstrip("rSliderFilmStrip.png", 111), valueTextBox(1) 
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d 
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
ksmps = 32
nchnls = 2
0dbfs = 1

;this instrument uses the following filmstrip image
;https://www.g200kg.com/en/webknobman/gallery.php?m=p&p=1659

instr 1

endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
;starts instrument 1 and runs it for a week

</CsScore>
</CsoundSynthesizer>

```


