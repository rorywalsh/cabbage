<a name="tablebackgroundcolour"></a>**tablebackgroundcolour("colour")** This sets the global background colour. Any CSS or HTML colour string can be passed to this identifier. The colour identifier can also be passed an RBG, or RGBA value. All channel values must be between 0 and 255. For instance colour(0, 0, 255) will create a blue, while colour(0, 255, 0, 255) will create a green with an alpha channel set to full. 

>The default value for the background colour is rgb(10, 10, 10). If you are superimposing tables on top of each other only one can be seen at any time when using the default background colour. If you wish to show all tables at the same time use a colour with an alpha value. Tables  with no alpha channels are drawn faster and use less CPU.  

<!--UPDATE WIDGET_IN_CSOUND
    SIdent sprintf "tablebackgroundcolour:0(%d, %d, %d) ", rnd(255), rnd(2550, rnd(255)
    SIdentifier strcat SIdentifier, SIdent
-->