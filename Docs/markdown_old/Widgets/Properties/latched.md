<a name="latched"><h3 style="padding-top: 40px; margin-top: 40px;"></h3></a>
**latched(val)**
With latched set to 0 the button will send a message to Csound on the initial mouse press, and once again as soon as the user releases the button. Therefore the button will toggle between 0 and 1 on each click rather than needing two separate mouse clicks. 

<!--UPDATE WIDGET_IN_CSOUND
    SIdent sprintf "latched(%d) ", rnd(100) < 70 ? 1 : 0
    SIdentifier strcat SIdentifier, SIdent
-->