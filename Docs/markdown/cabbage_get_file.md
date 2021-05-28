# cabbageGetFile Opcodes

These opcodes will parse a full file path for useful information. 


<blockquote style="font-style:italic;border-left:10px solid #93d200;color:rgb(3, 147, 210);padding:1px;padding-left:10px;margin-top:0px;margin-bottom:1px;border-left-width:0.25rem"> Added in Cabbage v2.6.4</blockquote>

### Syntax

<pre>SFilename <b>cabbageGetFilename</b> SFile</pre>
<pre>SFileExtension <b>cabbageGetFileExtension</b> SFile</pre>
<pre>SFileNoExtension <b>cabbageGetFileNoExtension</b> SFile</pre>
<pre>SFilePath <b>cabbageGetFilePath</b> SFile</pre>


#### Initialization

* `SFile` -- a file name and path

#### Performance

* `SFilename` -- file name, no path. For example, `~/home/test/sound.wav` becomes `sound.wav`
* `SFileExtension` -- file extension, i.e, '.wav', '.png', etc. 
* `SFileNoExtension` -- file name without extension. For example, `~/home/test/sound.wav` becomes `sound`
* `SFilePath` -- file name without extension. For example, `~/home/test/sound.wav` becomes `~/home/test`


### Example

```csharp
<Cabbage>
form caption("File Opcodes") size(470, 280), guiMode("queue") colour(58, 110, 182), pluginId("MPre")
filebutton bounds(180, 12, 142, 39), channel("openFile"), corners(5) text("Open", "Open"), populate("*"), 
label bounds(5, 80, 460, 16) channel("label1"), text("File name:"), colour("white") fontColour(147, 210, 0)
label bounds(5, 100, 460, 16) channel("label2"), text("File extension:"), colour("white") fontColour(147, 210, 0)
label bounds(5, 120, 460, 16) channel("label3"), text("File without extension:"), colour("white") fontColour(147, 210, 0)
label bounds(5, 140, 460, 16) channel("label4"), text("File path:"), colour("white") fontColour(147, 210, 0)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 --midi-key-cps=4 --midi-velocity-amp=5
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1

instr 1000

    SFileToOpen, kTrigger cabbageGetValue "openFile"
    SFile1 cabbageGetFilename SFileToOpen
    cabbageSet kTrigger, "label1", sprintfk("text(\"File name: %s\")", SFile1) 
    SFile2 cabbageGetFileExtension SFileToOpen
    cabbageSet kTrigger, "label2", sprintfk("text(\"File extension: %s\")", SFile2) 
    SFile3 cabbageGetFileNoExtension SFileToOpen
    cabbageSet kTrigger, "label3", sprintfk("text(\"File without extension: %s\")", SFile3) 
    SFile4 cabbageGetFilePath SFileToOpen
    cabbageSet kTrigger, "label4", sprintfk("text(\"File path: %s\")", SFile4) 

endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
i1000 0 z
</CsScore>
</CsoundSynthesizer>
```
