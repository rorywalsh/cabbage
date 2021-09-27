# Exporting your instruments as VCV Rack modules

Cabbage provides experimental support for the creation of VCV Rack modules. The steps are the same as those outlined in the previous section, but there are some caveats you should be aware of (see below). 

![Run instrument](images/vcvRackExample.gif) 

The simplest thing to get started with a VCV Rack module is to create a new VCV Rack file from the 'New' dialogue window. Cabbage will bundle all the VCV Rack files into a single folder when you export your file. You must place this folder in the [Rack plugins-v1 folder](https://vcvrack.com/manual/FAQ.html#where-is-the-rack-user-folder). From there, you can start editing the .csd file directly to save having to export each time you wish to update your module. 

## Caveats that apply to Cabbage VCV Rack modules

* Plugin height MUST be 380. If you make your instrument taller or shorter than this, it won't fit in the rack.

* Of the standard set of Cabbage widgets, only `rslider`, `label`, `groupbox`, `checkbox`, `button`, `combobox`, and `image` widgets are supported (with reduced functionality). Only the most basic identifiers are supported, so please keep things simple and keep testing your modules to make sure you are not using unsupported identifiers. Also, note that the `combobox` is more like a multi-switch than a traditional `combobox`.  

* CabbageRack provides 3 VCV Rack specific widgets, `cvInput` and `cvOutput`, (see below) and `light`. A Csound instrument can trigger the `light` widget to give users visual feedback on their modules.

* Unlike the other audio plugin interfaces for Cabbage, CabbageRack does not use the `in` and `out` family of opcodes. It uses audio-rate channels instead. Each `cvInput` and `cvOutput` widget is linked to a set of a-rate `chnget` and `chnset` opcodes. The reason for this is to allow maximum configurability of inputs and outputs.

* Slider skew values are not yet supported. 

* The slider increment identifier can be set to a number equal or greater than one to allow snapping to integer values, but it will only snap to multiples of 1.

* Modules names come from their folder, which should contain a .csd file of the same name. Module names can contain no whitespaces. Use _ or - instead.

* Colours are RGBA only, and there is no support for CSS colour names.

* This interface uses a much simpler parser than the one used in Cabbage plugins, spaces between identifiers and opening brackets will cause issues. 

* This interface draws all GUI widgets using an entirely different library than the native Cabbage app, so expect some things to look slightly different between this interface and an exported Cabbage audio plugins. 

* Only the light widget can be updated from Csound.

* VCV Rack auto-saves sessions. If you make some changes to your Cabbage module and reload, VCV Rack may try to load settings for controls that no longer exist. The missing settings might cause VCV Rack to crash. If so, you will need to edit or delete the auto-saved file.
