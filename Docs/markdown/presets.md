# Presets
Presets can be saved, restored and deleted at any stage using a combination of a filebuttons and a combobox. In your preset combobox you should specify a file type of "\*.snaps" in the `populate()` identifier. If you wish to save a preset to disk, you will need to use a `filebutton` with its mode set to either 'snapshot' or 'named snapshot'. Setting it to 'named snapshot' will cause a dialogue box to appear with a text editor to let users specify the name of the new preset. With 'snapshot' mode set Cababge will write a preset with a generic name to disk. This can be useful for quickly saving presets. Note that the preset file can easily be modified manually later. The preset data is stored as a simple JSON object. The code below shows how to set up a basic preset system with generic names. 

```csharp
combobox bounds(178, 286, 100, 25), populate("*.snaps")
filebutton bounds(116, 286, 60, 25), text("Save"), mode("preset")
```

The preset file, which will be updated each time a user hit the 'Save' button will be written to the same directory as the current .csd file. It will also have the same name as the current .csd file, albeit with a ".snaps" extension. You can also set the name of the custom preset file. Instead of passing "\*.snaps" as the file type, pass the full file name, i.e, "myPresets.snaps". Don't pass a full path here. Note that the same preset filename should be passed to all `filebutton` and `combobox` widgets involved in the management of the presets. 

> If you set a channel for the preset combobox it will contain the most current preset selected.  

Below is a simple example showing 'named snapshots'. It also features a 'Remove' `filebutton`. It has its mode set to 'remove preset'. 

![](images/presets_named.gif)

```csharp
<Cabbage>
form caption("Presets") size(370, 280), colour(58, 110, 182), pluginId("MPre")
keyboard bounds(10, 90, 345, 95)
rslider bounds(12, 8, 85, 79), channel("att"), range(0, 1, 0.01), text("Att.")
rslider bounds(98, 8, 85, 79), channel("dec"), range(0, 1, 0.4), text("Dec.")
rslider bounds(184, 8, 85, 79), channel("sus"), range(0, 1, 0.7), text("Sus.")
rslider bounds(270, 8, 85, 79), channel("rel"), range(0, 1, 0.8), text("Rel.")
combobox bounds(74, 190, 100, 25), populate("*.snaps"), channelType("string")
filebutton bounds(12, 190, 60, 25), text("Save"), populate("*.snaps", "test"), mode("named preset")
filebutton bounds(12, 220, 60, 25), text("Remove"), populate("*.snaps", "test"), mode("remove preset")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 --midi-key-cps=4 --midi-velocity-amp=5
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
;sr is set by the host
ksmps = 32
nchnls = 2
0dbfs = 1

;instrument will be triggered by keyboard widget
instr 1

endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
i1 0 z
</CsScore>
</CsoundSynthesizer>
```