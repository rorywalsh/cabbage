<a name="meterColour"><h3 style="padding-top: 40px; margin-top: 40px;"></h3></a>
**meterColour:N("colour")**Sets the colours of the meter. Use `meterColour:0("colour")` to set the first colour. Any further colours with cause a gradient fill to be created. 

<!--UPDATE WIDGET_IN_CSOUND
    SIdent sprintf "meterColour:0(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
    SIdentifier strcat SIdentifier, SIdent  
--->