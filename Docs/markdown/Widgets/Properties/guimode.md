<a name="guimode"><h3 style="padding-top: 40px; margin-top: 40px;"></h3></a>
**guimode("val")** Sets how widgets will be updated. `guimode("polling")` (default if `guimode()` is not specified), will cause each widget's channel to be polled for updates at k-boundaries. The update rate is set using the `guirefresh()` identifier. If you are using a large number of widgets this can get quite slow, especially if the widgets also contains identifier channels defined with `identchannel()`.

Using guimode("queue") will improve performance, especially if you are using a large number of widgets. `identchannel()` identifiers are not supported when `guimode` is set to "queue". In this case you can use the cabbageSet/Set opcodes to set and get data from a widget. More details can be found here.  

