<a name="autoUpdate"><h3 style="padding-top: 40px; margin-top: 40px;"></h3></a>
**autoUpdate()** When added to the form declaration, this identifier will causes Cabbage to start a timer that periodically checks for updates to the underlying source code. When it sees the .csd file has changed, for example through an external editor, it will automatically update the instrument. Note that adding extra widgets will require you to re-instantiation your instrument in the DAW, as widgets are tied to plugin parameters, which can't be dynamically modified.


