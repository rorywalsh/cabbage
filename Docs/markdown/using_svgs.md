# Using SVGs and PNGs

Cabbage widgets can be drawn using Scalable Vector Graphics such as the .svg files created using software such as Inkscape, or Adobe Illustrator. Doing so can give you further control over how your instrument looks, and can help you create unique and original themes for your instruments. While SVGs may not provide the same level of detail as PNG files, they are scalable without any loss of data. This reduces the need to provide several copies of a single image at various sizes and resolutions. This also means your SVGs can easily be used on a range of instruments without having to be modified.  

> Note that the SVG parser does not support the entire SVG specification. Basic shapes/lines are rendered perfectly. Text is rendered, but you may need to tweak the dimensions. Careful that your text is not classified as 'flowtext'. Flowtext will not render. Filters are not yet supported.  

The **imgFile()** identifier lets you attach a unique SVG file to widgets. 

##Example
The example below show how **imgFile()** identifiers can be used. 

```csharp
<Cabbage>
form caption("SVG Example") size(530, 480), colour("black"), pluginID("SMo1")
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
```

![](images/svgExample.gif)
