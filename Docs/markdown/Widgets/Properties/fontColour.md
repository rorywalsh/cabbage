<a name="fontColour"><h3 style="padding-top: 40px; margin-top: 40px;"></h3></a>
_____________________________
**fontColour("colour")** Sets the colour of the font. In the case of slider this sets the colour of the font in the value textBox if it is shown. Any CSS or HTML colour string can be passed to this identifier. This identifier can also be passed an RBG, or RGBA value. All channel values must be between 0 and 255. For instance colour(0, 0, 255) will create a blue, while colour(0, 255, 0, 255) will create a green with an alpha channel set to full. A single integer value between 0 and 255 will set a grayscale colour. 

<!--UPDATE WIDGET_IN_CSOUND
    SIdent sprintf "fontColour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
    SIdentifier strcat SIdentifier, SIdent  
-->