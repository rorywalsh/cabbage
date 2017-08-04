**svgpath("filepath")** Sets the path for any SVG files to be used for drawing widgets. Using this identifier will to save yourself from having to set the svgfile() identifier for each of the widgets. In order to use this identifier you will need to name your SVGs as follows:

- rslider.svg: used to draw the inner circle, and moving part of a rotary slider.
- hslider.svg: used to draw the moving thumb of a vertical slider. 
- vslider.svg: used to draw the moving thumb of a horizontal slider.
- vslider_background.svg: used to draw the background of a vertical slider.
- hslider_background.svg: used to draw the background of a horizontal slider. 
- groupbox.svg: used to draw a custom groupbox widget. 
- buttonon.svg: used to draw a button when on. 
- buttonoff.svg: used to draw a button when off. 

> Theming your instruments can also be done using images, but the advantage of using SVGs is that can they will scale without loss of resolution. You should exercise some caution when using text. If the font you choose if not available on the target platform, it will not be drawn. Also note that unlike shapes and lines, text does not scale well. For more information please see [Using SVGs](./using_svgs.md)

