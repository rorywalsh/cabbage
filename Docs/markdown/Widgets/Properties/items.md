<a name="items"></a>
**items("item1", "item2", ...)** list of items that will populate the combo box. Each item has a corresponding index value. The first item when selected will send a 1, the second item a 2, the third a 3 etc. If this identifier is left out default values of "Item 1", "Item  2", "Item  3", "Item 4" and "Item 5" will be used.  

<!--UPDATE WIDGET_IN_CSOUND
    SIdent sprintf "items(\"Text%d\", \"Text%d\", \"Text%d\", "Text%d\") ", rnd(100), rnd(100), rnd(100), rnd(100)
    SIdentifier strcat SIdentifier, SIdent
-->