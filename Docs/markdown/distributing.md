# Distributing Cabbage plugins
Anyone using Cabbage to develop instruments will have Csound installed on their machines, but people using their plugins might not. In order to avoid the need for users to pre-install Csound you can simply distribute Csound with your plugins. The best way to do this is to create an installer for your instrument that installs Csound along with all of your Cabbage files. There are several easy to use package builders for both OSX and Windows that can be used to create simple all in one installers.  

Alternatively you can instruct your users to install Csound on their host machines themselves before running your plugins.

>If you need to bundle files with your plugins, you may use the bundle() identifier. See [here](bundle.html) section and start there. ) 