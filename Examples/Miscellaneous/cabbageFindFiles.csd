<Cabbage>
form caption("Find Files") size(400, 300), guiMode("queue") pluginId("def1")
filebutton bounds(8, 208, 384, 43) channel("browseButton"), corners(5), mode("directory") file("/Users/walshr/sourcecode/cabbage/Examples/Miscellaneous")
listbox bounds(8, 10, 383, 192) channel("fileBrowser"), colour(147, 210, 0), highlightColour("black"), channelType("string")
label bounds(35, 265, 335, 16) channel("label1"), text("Please select a file"), colour("white"), fontColour(3, 147, 210)
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
    SFiles[] cabbageFindFiles kTrigger, SDirectory, "files"
    cabbageSet kTrigger, "fileBrowser", sprintfk("populate(\"*\", \"%s\")", SDirectory)    
    SSelectedItem, kFileSelected cabbageGetValue "fileBrowser"
    SSelectedFile strcatk cabbageGetFilename(SSelectedItem), cabbageGetFileExtension(SSelectedItem)
    cabbageSet kFileSelected, "label1", "text", cabbageGetFilename(SSelectedItem)
endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
;starts instrument 1 and runs it for a week
i1 0 [60*60*24*7] 
</CsScore>
</CsoundSynthesizer>
