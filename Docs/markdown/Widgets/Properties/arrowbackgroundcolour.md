<a name="arrowbackgroundcolour"><h3 style="padding-top: 40px; margin-top: 40px;"></h3></a>
**arrowbackgroundcolour("colour")** This sets the background colour of the arrows by the left and right hand side of the keys in the keyboard widgets. Any CSS or HTML colour string can be passed to this identifier. The colour identifier can also be passed an RBG, or RGBA value. All channel values must be between 0 and 255. For instance colour(0, 0, 255) will create a blue, while colour(0, 255, 0, 255) will create a green with an alpha channel set to full.  

<!--UPDATE WIDGET_IN_CSOUND
    SIdent sprintf "arrowbackgroundcolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
    SIdentifier strcat SIdentifier, SIdent
-->