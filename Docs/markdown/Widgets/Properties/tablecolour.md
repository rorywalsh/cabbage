<a name="tablecolour"></a>**tablecolour:N("colour1")** Sets the colour of the table's waveform, where N is the table number, starting at 0 for the first table. If you leave this identifier out default colours will be chosen for the tables. If the table size is equal or less than two, the colours passed to tablecolour() will form a gradient fill or three colours.  
>The tablecolour() identifier will not work with RGBA/RGB values. You must instead use a valid CSS colour name.

<!--UPDATE WIDGET_IN_CSOUND
    SIdent sprintf "tablecolour:0(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
    SIdentifier strcat SIdentifier, SIdent  
-->


