<a name="text_button"></a>**text("offCaption", "onCaption")**
The text identifier must be passed at least one string argument. This string will be the one that will appear on the button. If you pass two strings to text() the button will toggle between the two string each time it is pushed. 

<!--UPDATE WIDGET_IN_CSOUND
    SIdent sprintf "text(\"TextOff %f\", \"TextOn %f\") ", rnd(100), rnd(100)
    SIdentifier strcat SIdentifier, SIdent
-->