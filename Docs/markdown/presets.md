# Presets
Presets can be saved a restored at any stage using a combination of a filebutton and a combobox. In your preset combobox you should specify a file tyoe of "\*.snaps" in the populate() identifier. The filebutton should also use a populate identifier, also set to filtype of "\*.snaps". So your combobox and file button code might look like this

```csharp
combobox bounds(178, 286, 100, 25), channel("combobox"), populate("*.snaps")
filebutton bounds(116, 286, 60, 25), channel("but1"), text("Save"), populate("*.snaps")
```

Each time a user presets the 'Save' button a file browser dialogue with open, allowing the user to name and save the preset files to disk. If you wish to have the newly saved preset accessible thought the associated combo box you will need to save the files in the same directory as specified in the combox box populate identifier. For ease of use it's usually best to work from the current directory.  

If you wish to quickly write presets to disk, you can add a "mode("snapshot")" identifier to the filebutton. This will disable to file browser dialogue and instead write automatically named files to disk. The new filename will be the csd filename, with an underscore and number appended to it. For instance, if the filename is CabbageStew.csd, the preset files will be named CabbageStew_0.csd, CabbageStew_1.csd, CabbageStew_2.csd, etc. The snapshot mode provides is a very quick way of saving presets to disk.  