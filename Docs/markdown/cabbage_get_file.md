# cabbageFindFile

This opcode will search a given directory for files, folder, or files and folders. If you wish to use this opcode at k-rate use the version with the kTriggerFind input parameter. 

### Syntax

<pre>SFiles[] <b>cabbageFindFiles</b> SLocation [, SType, SExtension]</pre>
<pre>SFiles[] <b>cabbageFindFiles</b> kTriggerFind, SLocation [, SType, SExtension]</pre>

#### Initialization

* `SLocation` -- the directory you wish to search
* `kTriggerFind` -- will trigger a search each time this is 1. 
* `SType` (optional) -- what you want to search for, `"files"`, `"directories"`, `"fileAndDirectories"`
* `SExtension` (optional) -- a file extension in the form of `"*.wav"`. This can be a `;` delimited list, i.e, `"*.wav;*.ogg"`

#### Performance

* `SFiles[]` -- a string array of files found


### Example

```csharp
<Cabbage>
form caption("Find Files") size(400, 300), guiMode("queue") pluginId("def1")
filebutton bounds(112, 4, 173, 43) channel("browseButton"), corners(5), mode("directory")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d 
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
ksmps = 32
nchnls = 2
0dbfs = 1


instr 1
    SDirectory, kTrigger cabbageGetValue "browseButton"
    SFiles[] cabbageFindFiles kTrigger, SDirectory, "directories", "*"
    printarray SFiles, kTrigger
endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
;starts instrument 1 and runs it for a week
i1 0 [60*60*24*7] 
</CsScore>
</CsoundSynthesizer>

```

> Added in Cabbage v2.6.4