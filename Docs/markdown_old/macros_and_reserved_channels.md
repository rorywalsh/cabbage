# Macros and Reserved Channels
Cabbage macros can be used to define a range of different identifiers, and provide a way of achieving a consistent look and feel across your instrument's widgets, without having to hard-code each and every identifier. To create a macro one must use the `#define` keyword. Macros can appear anywhere after a widget has been declared. Their use is normally reserved for entire identifier strings as in the examples below. You may however use them to form part of an identifier's parameters, but they resultant identifer will not be available for updating through the use of the GUI editor. 

The following code will create a macro called `SLIDER1` that will define several attributes of a slider widget:

```csharp
#define SLIDER1 colour("red"), fontcolour("yellow"), outlinecolour("red"), range(0, 1000, 500)
```
The macro can then be used anywhere in your Cabbage code section by placing it on an appropriate line of Cabbage code. For example, the following code uses the above macro with an rslider.

```csharp
rslider bounds(39, 12, 50, 50), channel("rslider"), $SLIDER1
```
You can override any identifiers contained in a macro by placing them after the macro name. The following code will override the colour identifier from the macro with a new colour.

```csharp
rslider bounds(39, 12, 50, 50), channel("rslider"), $SLIDER1, colour("purple")
```

You can use as many macros as you wish in your Cabbage code. 

### Accessing Cabbage macros in Csound code
Macros defined in the Cabbage section of your code can also be accessed in the Csound section using the same syntax. For example, a macro named SLIDER1 is defined in the Cabbage section as:  

```csharp
#define SLIDER1 colour("red"), fontcolour("yellow"), outlinecolour("red"), range(0, 1000, 500)
``` 

It can then accessed in the Csound orchestra like this:

```csharp
printf_i "%s \n", 1, "$SLIDER1" 
```

> Note that macros passed from the Cabbage section to the Csound orchestra are string macros. Therefore you will need to convert to a number if you wish to pass numbered macros between Cabbage and the Csound orchestra.   


### Cabbage Csound Macros 
The following are Csound macros that are set by Cabbage when it first loads. These macros are accessed in Csound in the very same way that regular instrument macros are. Note however that each of these macros returns a string value. Therefore you may need to convert them to a number using the strtod opcode.  

**IS_A_PLUGIN** Defined if the instrument is running as a plugin

**SCREEN_WIDTH** Provides the instrument width in pixels as a string value. Use strtod to convert from string to number. On Android devices this will return the actual screen width. This can be useful if you wish to move widgets around in realtime using [Identifier channels](./identchannels.html).  

**SCREEN_HEIGHT** Provides the instrument height in pixels as a string value. Use strtod to convert from string to number. On Android devices this will return the actual screen height. This can be useful if you wish to move widgets around in realtime using [Identifier channels](./identchannels.html).  

## Reserved Channels

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

**AUTOMATION** Set the automation mode, 0/1. 0, the default mode instructs Cabbage to listen to automation from a host DAW. Use this mode is you wish to automate parameters using automation envelopes and curves in your DAW. The second mode, 1, will allow the host to track channel updates if they happen in Csound. If you wish to send automation changes from your instrument, you will need to enable this mode using a `chnset`. 

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




