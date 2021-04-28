<a name="filmstrip"><h3 style="padding-top: 40px; margin-top: 40px;"></h3></a>
**filmstrip("filename", numFrames, "orientation")** The filmstrip identifier allow users to use a filmstrip style .png to skin sliders. `filename` is the name of the .png file to be used. `numFrames` is the number of frames in the film strip image. `removeA` and `removeB` will reScale the underlying slider object, which is invisible due to the skin laid on top of it. For example, in the case of a hslider, removeA(.1) will remove .1 of the overall length of the slider from the left side, removeB(.1) will remove .1 of the overall length from the right. This allows one to reScale sliders according to where their skin images start and end.

For a vslider removeA and removeB refer to the top and bottom of the slider. See the KnobManSlider.csd file in the Misc. examples for more details. This idetnfier only works with vertical framed images.

>Try to avoid full path names at all costs. They will work fine on a local machine, but will not be valid on another machine.
