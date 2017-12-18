<a name="popuptext"></a>
**popuptext("text")** Sets the text that will appear in the popup text bubble when a user hovers over the widget. This can be used to override the defaul text which displays the channel name and the current value. Passing a 0 will disable all popuptext notices completely. 

<!--UPDATE WIDGET_IN_CSOUND
    SIdent sprintf "popuptext(\"Popup Text %d\") ", rnd(1000)
    SIdentifier strcat SIdentifier, SIdent
-->