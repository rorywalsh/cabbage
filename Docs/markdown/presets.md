# Presets
Presets can be saved a restored at any stage using a combination of a filebutton and a combobox. In your preset combobox you should specify a file type of "\*.snaps" in the populate() identifier. The filebutton should have its mode identifer set to 'snapshot'. Your combobox and filebutton code might look like this:

```csharp
combobox bounds(178, 286, 100, 25), channeltype("string"), channel("comboChannel"), populate("*.snaps")
filebutton bounds(116, 286, 60, 25), channel("but1"), text("Save"), mode("snapshot")
```

Each time a user presets the 'Save' button a new preset will be written. The new filename will be the same as the current csd filename, but with a .snaps extension. For instance, if the filename is CabbageStew.csd, the preset files will be named CabbageStew.snaps. Cabbage will generate generically numbered preset names, although they can be changed by opening the .snaps file in a text editor and editing the names by hand.    

>To have a host recall the preset name you must specify channeltype("string") to the preset combobox - and you must also provide channel.