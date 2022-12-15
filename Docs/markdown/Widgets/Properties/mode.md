<a name="mode"><h3 style="padding-top: 40px; margin-top: 40px;"></h3></a>
_____________________________
**mode("mode")** 

Can be used with a `filebutton`, `texteditor` and a combobox. Valid modes are:

`filebutton`:
* *file*, Default. Will open a browser to select a file
* *save*, Will open a browser to select an existing file, or write the name of a new file. Note Cabbage will not create this new file, you will need to do that yourself in Csound. 
* *directory*, Will open the browser to select a folder/directory
* *preset*, Will stop the file browser from opening, and instead write a preset file to disk with a default file name. See [Presets](./presets.md) 
* *named preset*, Will display a message box with a text editor so that users can name their presets. See [Presets](./presets.md) 
* *remove preset*, Will remove the currently select preset from the snapshot XML file. See [Presets](./presets.md) 

`texteditor`:
* *multline*, Defaults to "single". Use "multi" to enable multiline.

`combobox`:
* *resize*, use `mode("resize")` to make the combobox act as a UI resizer.  
