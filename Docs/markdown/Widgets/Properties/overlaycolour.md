<a name="overlaycolour"></a>**overlaycolour("colour")** This sets the colour of the meter overlay. The meter overlay draws a rectangle on top of the meter's gradient fill to give the impression that the meter is moving up and down. This colour shouldn't have any transparency. Any CSS or HTML colour string can be passed to this identifier. The colour identifier can also be passed an RBG, or RGBA value. All channel values must be between 0 and 255. For instance colour(0, 0, 255) will create a blue, while colour(0, 255, 0, 255) will create a green with an alpha channel set to full.  

<!--UPDATE WIDGET_IN_CSOUND
    SIdent sprintf "overlaycolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
    SIdentifier strcat SIdentifier, SIdent  
--->