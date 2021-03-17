# Reserved Channels
Cabbage reserves several channels which are used to send information to Csound from either the host application, or from Cabbage itself. To retrieve info from any of these channels just use a chnget opcode.

**CSD_PATH** This string channel will retrieve the full path to the current csd file.

**HOST_BPM** Retrieve the currents host bpm. This will be updated whenever the host BPM changes.

**IS_PLAYING** Pressing play on the host transport dialogue will cause this channel to send a 1. Hitting stop will send a 0.

**IS_RECORDING** As above only for monitoring the record status of a session.

**TIME_IN_SECONDS** Return the current time in seconds from the start of the track.

**TIME_IN_SAMPLES** Return the current time in samples from the start of the track.

**TIME_SIG_DENOM** The signature denominator, e.g. the 4 of a 3/4 time sig 

**TIME_SIG_NUM** The signature numerator, e.g. the 3 of a 3/4 time sig 

**HOST_PPQ_POS** Return the position of the start of the last bar, in pulses-per-quarter-note.

**MOUSE_X** Returns the X coordinate of the current mouse position.

**MOUSE_Y** Returns the Y coordinate of the current mouse position

**MOUSE_DOWN_LEFT** Returns the current state of the left most mouse button; 1 if pressed, and 0 if not. 

**MOUSE_DOWN_MIDDLE** Returns the current state of the middle mouse button; 1 if pressed, and 0 if not. 

**MOUSE_DOWN_RIGHT** Returns the current state of the right mouse button; 1 if pressed, and 0 if not. 

**IS_A_PLUGIN** Returns 1 if the instrument is running as an audio plugin, and 0 if the instrument is running as a standalone. 

**LINUX** Returns 1 if Cabbage is running on a Linux OS. 

**Mac** Returns 1 if Cabbage is running on a Mac OS. 

**Windows** Returns 1 Cabbage is running on a Windows OS. 

**SECONDS_SINCE_EPOCH** Return the number of seconds since the 1st of Jaunary 1970

**CURRENT_DATE_TIME** Returns the current date and time in a string, e.g., "Sun Jan 10 09:36:02 2021" 

**CURRENT_WIDGET** Returns the channel string of the widget currently under the mouse - if a channel has been assigned.
