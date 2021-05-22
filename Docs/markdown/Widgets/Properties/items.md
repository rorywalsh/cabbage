<a name="items"><h3 style="padding-top: 40px; margin-top: 40px;"></h3></a>
_____________________________
**items("item1", "item2", ...)** list of items that will populate the combo box. Each item has a corresponding index value. The first item when selected will send a 1, the second item a 2, the third a 3 etc. If this identifier is left out default values of "Item 1", "Item  2", "Item  3", "Item 4" and "Item 5" will be used.  

If you wish to use submenus in your combox box, you can use 'items:' followed by a number or a string. The first string you pass to `item:..()` will be the sub menu heading. For example, the following will create two sub menus called Strings and Woodwind:

`combobox bounds(10, 10, 100, 50), items:1("Strings", "Violin", "Viola", "Cello", "Contrabass"), items:2("Woodwind", "Clarinet", "Oboe", "Bassoon")`

The index of each item is always determined by where is appear in the widget declaration.  

<!--UPDATE WIDGET_IN_CSOUND
    SIdent sprintf "items(\"Text%d\", \"Text%d\", \"Text%d\", \"Text%d\") ", rnd(100), rnd(100), rnd(100), rnd(100)
    SIdentifier strcat SIdentifier, SIdent
-->