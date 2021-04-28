# Distributing Cabbage plugins
Anyone using Cabbage to develop instruments will have Csound installed on their machines, but people using their plugins might not. In order to avoid the need for users to pre-install Csound you can simply distribute Csound with your plugins. The easiest way to do this is to create an installer for your instrument that installs Csound with all of your Cabbage files. There are several easy to use package builders for both OSX and Windows that can be used to create simple all in one installers. An optional install of Csound is the best option here as it gives users with Csound already installed a chance to skip reinstallation each time they install a new Cabbage plugin.   

Alternatively you can instruct your users to install Csound on their host machines themselves before running your plugins.

If you wish to provide custom Csound builds with your packages you must be careful to do so in a way that doesn't violate Csound's LGPL license. On OSX you can simply add the Csound framework to your plugin bundle and relink your plugin binaries to the Csound library. On Windows you will need to build your own Csound library with a unique name, and then build Cabbage while linking to that new library. Note the custom Csound library name must still have Csound in its name so as not to violate the terms of the LGPL. If you need more information about this please ask on the [Cababge forums](http://forum.cabbageaudio.com).   

>If you need to bundle files with your plugins, you may use the bundle() identifier. See the form widget [here](./form.md) for more details. 


