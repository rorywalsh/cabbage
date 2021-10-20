# Reserved Channels and Macros

Cabbage reserves several channels and macros. These can be used to retrieve in Csound from either the host application, or from Cabbage itself. To retrieve info from a reserved channels use a chnget opcode, i.e, `SCsdPath chnget "CSD_PATH"`. A full list of reserved channels is given below. 

Cabbage also reserved the following macros which can be used to expand system directories when defined Cabbage widgets. For example, if you want a `filebutton` to open at a user sample folder within the user's music directory, you can pass the `#USER_MUSIC_DIRECTORY` macros to the populate identifier, i.e, `populate("*.wav", "#USER_MUSIC_DIRECTORY/MySamples")`. Note these macros are only available in the Cabbage section. If you need to access these paths in the Csound orchestra, you the equivalent reserved channels.

`#USER_HOME_DIRECTORY` - retrieves the full path of the user’s home directory. 
`#USER_DESKTOP_DIRECTORY` - retrieves the full path of the user’s Desktop directory. 
`#USER_MUSIC_DIRECTORY` - retrieves the full path of the user’s Desktop directory.
`#USER_APPLICATION_DATA_DIRECTORY` - retrieves the full path of the user’s application directory. C:/Users/username/AppData on Windows, ~/Library on MacOS
`#USER_DOCUMENTS_DIRECTORY` - retrieves the full path of the user’s application directory

**CSD_PATH** This string channel will retrieve the full path to the current csd file.

**HOST_BPM** Retrieve the currents host bpm. This will be updated whenever the host BPM changes.

**IS_PLAYING** Pressing play on the host transport dialogue will cause this channel to send a 1. Hitting stop will send a 0.

**IS_RECORDING** As above only for monitoring the record status of a session.

**TIME_IN_SECONDS** Return the current time in seconds from the start of the track.

**TIME_IN_SAMPLES** Return the current time in samples from the start of the track.

**TIME_SIG_DENOM** The signature denominator, e.g. the 4 of a 3/4 time sig 

**TIME_SIG_NUM** The signature numerator, e.g. the 3 of a 3/4 time sig 

**HOST_PPQ_POS** Return the position of the start of the last bar, in pulses-per-quarter-note.

**HOST_BUFFER_SIZE** Return the size of the host buffer in samples.

**AUTOMATION** Set the automation mode, 0/1. 0, the default mode instructs Cabbage to listen to automation from a host DAW. Use this mode is you wish to automate parameters using automation envelopes and curves in your DAW. The second mode, 1, will allow the host to track channel updates if they happen in Csound. If you wish to send automation changes from your instrument, you will need to enable this mode using a `chnset`. 

**CSOUND_GESTURES** Set this to 1 if you want hosts to respond to channel changes when recording automation. 

**MOUSE_X** Returns the X coordinate of the current mouse position.

**MOUSE_Y** Returns the Y coordinate of the current mouse position

**MOUSE_DOWN_LEFT** Returns the current state of the left most mouse button; 1 if pressed, and 0 if not. 

**MOUSE_DOWN_MIDDLE** Returns the current state of the middle mouse button; 1 if pressed, and 0 if not. 

**MOUSE_DOWN_RIGHT** Returns the current state of the right mouse button; 1 if pressed, and 0 if not. 

**SCREEN_WIDTH** Returns the width of the plugin editor window

**SCREEN_HEIGHT** Returns the height of the plugin editor window

**LAST_FILE_DROPPED** This will hold the name of the last file dragged and released on the plugin window. 

**LAST_TEXT_DROPPED** This will hold the string of the last piece of text that was dragged and released on the plugin window. 

**IS_A_PLUGIN** Returns 1 if the instrument is running as an audio plugin, and 0 if the instrument is running as a standalone. 

**IS_EDITOR_OPEN** Returns 1 if the plugin editor window is currently open, or 0 if the user has closed it. 

**WINDOWS** Returns 1 if Cabbage is running on a Windows machine

**LINUX** Returns 1 if Cabbage is running on a Linux machine

**MACOS** Returns 1 if Cabbage is running on a MacOS machine

**KEY_PRESSED** Returns the ASCII key code for the key currently being pressed

**KEY_DOWN** Returns 1 a key-down event, and 0 for a key-up event.  

**KEY_MODIFIERS** Returns a string with a list of current modifiers, i.e, Shift Ctrl Alt Command

**SECONDS_SINCE_EPOCH** Return the number of seconds since the 1st of Jaunary 1970

**CURRENT_DATE_TIME** Returns the current date and time in a string, e.g., "Sun Jan 10 09:36:02 2021"

**CURRENT_WIDGET** Returns the channel string of the widget currently under the mouse - if a channel has been assigned.

**HOME_FOLDER_UID** Returns a unique ID for the users home folder.


Cabbage also set the host type, which can be retrieved using the following channels. 

**AbletonLive** Returns 1 if Cabbage is host is Ableton Live

**AdobeAudition** Returns 1 if Cabbage is host is AdobeAudition

**Ardour** Returns 1 if Cabbage is host is Adour

**Bitwig** Returns 1 if Cabbage is host is Bitwig studio

**Cubase** Returns 1 if Cabbage is host is Cubase

**FLStudio** Returns 1 if Cabbage is host is FL Studio

**GarageBand** Returns 1 if Cabbage is host is Garage band

**Logic** Returns 1 if Cabbage is host is Logic

**MainStage** Returns 1 if Cabbage is host is MainStage

**Nuendo** Returns 1 if Cabbage is host is Nuendo

**Renoise** Returns 1 if Cabbage is host is Renoise

**Reaper** Returns 1 if Cabbage is host is Reaper

**Samplitude** Returns 1 if Cabbage is host is Samplitude

**Sonar** Returns 1 if Cabbage is host is Sonar

**StudioOne** Returns 1 if Cabbage is host is Studio One

**Tracktion** Returns 1 if Cabbage is host is Tracktion

**Wavelab** Returns 1 if Cabbage is host is WaveLab

**USER_HOME_DIRECTORY** - retrieves the full path of the user’s home directory. 

**USER_DESKTOP_DIRECTORY** - retrieves the full path of the user’s Desktop directory. 

**USER_MUSIC_DIRECTORY** - retrieves the full path of the user’s Desktop directory.

**USER_APPLICATION_DATA_DIRECTORY** - retrieves the full path of the user’s application directory. C:/Users/username/AppData on Windows, ~/Library on MacOS

**USER_DOCUMENTS_DIRECTORY**, - retrieves the full path of the user’s application directory


