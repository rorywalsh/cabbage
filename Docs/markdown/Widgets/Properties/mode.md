<a name="mode"><h3 style="padding-top: 40px; margin-top: 40px;"></h3></a>
**mode("mode")** Set how the button will behave when pressed. Valid modes are:

* *file*, Default. Will open a browser to select a file
* *save*, Will open a browser to select an existing file, or write the name of a new file. Note Cabbage will not create this new file, you will need to do that yourself in Csound. 
* *directory*, Will open the browser to select a folder/directory
* *snapshot*, Will stop the file browser from opening, and instead write a preset file to disk with a default file name. See [Presets](./presets.md) 