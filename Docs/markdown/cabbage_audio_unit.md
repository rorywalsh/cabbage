# Cabbage Audio Units

The Audio Unit interface for Cabbage differs slightly from the VST implementation due to differences in the underlying SDKs. After installing Cabbage for OSX, users need to add a CabbageAUEffect, or CabbageAUSynth to their Garageband, Logic or Ardour projects. 
The installer places the two different Cabbage AudioUnit's into the expected OSX location so they should be immediately accessible in your DAW. When you first load an instance of a Cabbage AudioUnit, a generic plugin interface will open. Users can then click and browse for a Cabbage .csd file to load.

![AudioUnit](images/CabbageAU1.png) 

Once a file has been loaded, the AudioUnit will display the full plugin interface and users can interact with the plugin in the same way you would any other AudioUnit. 

![AudioUnit](images/CabbageAU2.png) 

When a project is saved, the name of the .csd file will also be saved with the session data. This insures that the Cabbage AudioUnit plugin will automatically load the associated instrument file the next time the project is opened. Examples can be found a folder called 'CabbageAudioUnit' in your ~/Music directory. 


> As of the time of writing, plugin parameter settings are not saved with the session. Therefore automation should be used if you wish to store the current setting of a particular parameter. 
