<a name="popupText"><h3 style="padding-top: 40px; margin-top: 40px;"></h3></a>
**popupText("text")** Sets the text that will appear in the popup text bubble when a user hovers over the widget. This can be used to override the defaul text which displays the channel name and the current value. Passing a 0 will disable all popupText notices completely. 

<!--UPDATE WIDGET_IN_CSOUND
    SIdent sprintf "popupText(\"Popup Text %d\") ", rnd(1000)
    SIdentifier strcat SIdentifier, SIdent
-->