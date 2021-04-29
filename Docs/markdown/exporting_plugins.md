# Exporting your instruments as audio plugins

Cabbage gives users the option of exporting instruments as:

* VST 
* AudioUnits
* FMOD module
* Rack modules
* Standalone applications

Once you have created your instruments, you will need to export them as plugins from the 'File' menu in the Cabbage IDE. When you export an instrument Cabbage will create a plugin binary with a .so, .vst, or .dll file extension, depending on your OS. Cabbage will also make a copy of your current text file and place it into the same directory as the plugin file. The newly created text file and plugin file will have the same file name (with different file extensions). 

![Exporting](images/exporting_plugin.gif)

It's important that you keep these files together. Failure to do so will cause problems as the Cabbage plugin will not be able to find the corresponding Csound text file. Once you have your plugin created, you will need to add the plugin directory to your host. On Linux you can simply make sure that all plugins reside in ~/.vst. Most hosts on MacOS and Windows let you specify unique plugin directories. The best strategy here is to keep all your Cabbage plugins in the same directory. That way you only need to set the plugin directory once. For more details on setting plugin directories consult the documentation for your host software. 

>In order to make changes to your newly exported plugin you only need to edit the associated .csd file. You don't need to keep exporting the plugin each time. For instance, on Windows you might have a plugin called "SavageCabbage.dll". If you wish to make some changes to this plugin you only have to edit the corresponding "SavageCabbage.csd" file. In order to see the changes in your plugin host you will need to delete and reinstate the plugin from the track. Once you remove and add the plugin it will show the new changes. 


### Exporting your instruments as VCV Rack modules

Cabbage also provides experimental support for the creation of VCV Rack modules. The steps are the same to those outlined in the section above, but there are some caveats you should be aware of (see below). 

![Run instrument](images/vcvRackExample.gif) 

The simplest thing to do to get started with VCV Rack module is to create a new VCV Rack file from the 'New' dialogue window. When you export your file Cabbage will bundle all the VCV Rack files together into a single folder. This folder must be placed in the [Rack plugins-v1 folder](https://vcvrack.com/manual/FAQ.html#where-is-the-rack-user-folder). Once it has been created you can start editing the .csd file from there directly to save having to export each time you wish to update your module. 

## The following caveats apply to all Cabbage VCV Rack modules:

* Plugin width MUST be 380. If you make your instrument bigger or smaller than this it won't fit in the rack.

* Of the standard set of Cabbage widgets, only rslider, label, groupbox, checkbox, button, combobox and image widgets are supported (with reduced functionality). Only the most basic identifiers are supported, so please keep things simple and keep testing your modules to make sure you are not using unsupported identifiers. Also note that the combobox is more like a multi-switch than a traditional combobox.  

* CabbageRack provides 3 VCV Rack specific widgets, `cvInput` and `cvOutput`, (see below) and `light`. The `light` widget can be triggered from a Csound instrument to give users visual feedback their modules.

* Unlike the other audio plugin interfaces for Cabbage, CabbageRack does not use the `in` and `out` family of opcodes. It uses audio-rate channels instead. Each `cvInput` and `cvOutput` widget is linked to a set of a-rate `chnget` and `chnset` opcodes. The reason for this is allow maximum configurability of inputs and outputs.

* Slider skew values are not yet supported. 

* The slider increment identifier can be set to a number equal or greater than one to allow snapping to integer values, but it will only snap to multiple of 1.

* Modules are named by their folder, which should contain a .csd file of the same nam. No whitespaces can be used. Use _ or - instead.

* Colours are RGBA only, there is no support for CSS colour names.

* This interface uses a much simpler parser than the one used in Cabbage plugins, spaces between identifiers and opening brackets will cause issues. 

* This interface draws all GUI widgets using an entirely different library than the native Cabbage app, so expect some things to look slightly different between this interface and an exported Cabbage audio plugins. 

* Only the light widget can be updated from Csound.

* VCV Rack auto-saves sessions. If you make some changes to your Cabbage module and reload, VCV Rack may try to load settings for controls that no longer exist. This might cause VCV Rack to crash. If so, you will need to edit or delete the auto-saved file.


