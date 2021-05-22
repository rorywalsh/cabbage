<a name="file"><h3 style="padding-top: 40px; margin-top: 40px;"></h3></a>
_____________________________
**file("filename")** This identifier is used across a number of widgets. <br>
<br>`combobox` and `listbox` - will load lines from a file as selectable items. 
<br>`gentable` and `soundfiler` - will load a sound file to display
<br>`image` - will load an iamge to display (PNG/JPG/SVG) 
<br>`infobutton` - sets the file to display when the button is clicked This can be a local html file, or a URL. 
<br>`texteditor` - sets the file to load, should be a text file.   

If a full file path is not given, file() will search in the current directory, i.e., the directory that contains the csd file that is open. It is best to keep all files in the same directory as your csd file. If you wish to keep them in a separate folder use relative paths, i.e, `../../folder`. 

>Avoid absolute path at all costs. Failure to do so will most likely problems with your instruments as soon as you share them. 
