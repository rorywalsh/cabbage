# Bundling files

You may need to bundle a set of files with your plugins during the exporting process. For example, images, sound files, include files, etc, used by your Cabbage/Csound code will need to be available to the plugin when it is opened in a DAW. The best approach is to keep all these files in a single folder, and use the `bundle()` identifier to bundle them when exporting your plugin. 

The `bundle()` identifier takes a list of filename, or directories, and copies them to the same folder as the exported plugin file. On Windows and Linux, this will be the directory that the exported .dll/.so file resides in. On OSX it will be to the plugin bundle itself. Please us relative paths in all your Cabbage instrument to avoid any conflicts when exporting. 

