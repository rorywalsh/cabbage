<a name="blacknotecolour"></a>
**blacknotecolour("colour")** This sets the colour of the black notes of the keyboard. Any CSS or HTML colour string can be passed to this identifier. The colour identifier can also be passed an RBG, or RGBA value. All channel values must be between 0 and 255. For instance colour(0, 0, 255) will create a blue, while colour(0, 255, 0, 255) will create a green with an alpha channel set to full.  

<!--UPDATE WIDGET_IN_CSOUND
    SIdent sprintf "blacknotecolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
    SIdentifier strcat SIdentifier, SIdent
-->