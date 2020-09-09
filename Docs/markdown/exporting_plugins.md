# Exporting your instruments as audio plugins

Cabbage gives users the option of exporting instruments for use in VST and AU hosts such as Reaper, Live, Logic, etc. Once you have created your instruments, you will need to export them as plugins from the 'File' menu in the Cabbage IDE. When you export an instrument Cabbage will create a plugin binary with a .so, .vst, or .dll file extension, depending on your OS. Cabbage will also make a copy of your current text file and place it into the same directory as the plugin file. The newly created text file and plugin file will have the same file name (with different file extensions). 

![Exporting](images/exporting_plugin.gif)

It's important that you keep these files together. Failure to do so will cause problems as the Cabbage plugin will not be able to find the corresponding Csound text file. Once you have your plugin created, you will need to add the plugin directory to your host. On Linux you can simply make sure that all plugins reside in ~/.vst. Most hosts on MacOS and Windows let you specify unique plugin directories. The best strategy here is to keep all your Cabbage plugins in the same directory. That way you only need to set the plugin directory once. For more details on setting plugin directories consult the documentation for your host software. 

>In order to make changes to your newly exported plugin you only need to edit the associated .csd file. You don't need to keep exporting the plugin each time. For instance, on Windows you might have a plugin called "SavageCabbage.dll". If you wish to make some changes to this plugin you only have to edit the corresponding "SavageCabbage.csd" file. In order to see the changes in your plugin host you will need to delete and reinstate the plugin from the track. Once you remove and add the plugin it will show the new changes. 

## Differences between VST and AU

The AU and VST interfaces work in much the same way. MacOS looks for AUs in `~/Library/Audio/Plug-Ins/Components`, or `/Library/Audio/Plug-Ins/Components`. If your AUs plugins don't get reconsigned after exporting, run `auval -a` from the terminal. If you see an `error retrieving Component Version: -50` it is likely that your host is blocking your plugin from opening the Csound library. To fix this you need to modify the Csound library so that it uses an absolute path. To do this, open the terminal window and cd to `/Library/Frameworks/CsoundLib64.framework/Versions/6.0/`. Then run the following command:

`otool -L CsoundLib64`

You might see output like this:

```
CsoundLib64:
	CsoundLib64.framework/CsoundLib64 (compatibility version 6.0.0, current version 0.0.0)
	@loader_path/../../libs/libsndfile.1.dylib (compatibility version 2.0.0, current version 2.28.0)
	/usr/lib/libSystem.B.dylib (compatibility version 1.0.0, current version 1281.0.0)
	/usr/lib/libcurl.4.dylib (compatibility version 7.0.0, current version 9.0.0)
	/System/Library/Frameworks/Accelerate.framework/Versions/A/Accelerate (compatibility version 1.0.0, current version 4.0.0)
```

When your AU plugins tries to load, it will be sandboxed in a separate process, and won't be able to find CsoundLib64. To fix this, you need to run the following command:

`sudo install_name_tool -id /Library/Frameworks/CsoundLib64.framework/CsoundLib64 CsoundLib64`

The output from `otool` should now look like this:

```
CsoundLib64:
	/Library/Frameworks/CsoundLib64.framework/CsoundLib6 (compatibility version 6.0.0, current version 0.0.0)
	@loader_path/../../libs/libsndfile.1.dylib (compatibility version 2.0.0, current version 2.28.0)
	/usr/lib/libSystem.B.dylib (compatibility version 1.0.0, current version 1281.0.0)
	/usr/lib/libcurl.4.dylib (compatibility version 7.0.0, current version 9.0.0)
	/System/Library/Frameworks/Accelerate.framework/Versions/A/Accelerate (compatibility version 1.0.0, current version 4.0.0)
```

>Note that you may need to restart your system before the plugins will be seen. MacOS is pretty pedantic about registering new AUs. You may also need to clear your AudioUnit cache.  


