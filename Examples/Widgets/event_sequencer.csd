<Cabbage>
form caption("String Sequencer") size(400, 400), pluginID("def1")
stringsequencer bounds(10, 10, 300, 320), channels("step", "track1", "track2", "track3", "track4"), active(0), identChannel("trackerIdent"), textColour(200, 200, 200), highlightColour(60, 60, 60) outlineColour(80,80,80), bpm(180), fontColour("white") backgroundColour(20, 20, 20) showStepNumbers(4), numberOfSteps(16)
rslider bounds(314, 10, 70, 70) channel("bpm") range(10, 300, 180, 1, 0.001) text("BPM") 
button bounds(314, 82, 70, 27) channel("startStop") text("Start", "Stop") 
rslider bounds(316, 112, 70, 70) channel("position") range(0, 15, 0, 1, 1) 
</Cabbage>
