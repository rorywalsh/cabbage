# Macros and Reserved Channels
Cabbage macros can be used to define a range of different identifiers, and provide a way of achieving a consistent look and feel across your instrument's widgets, without having to hard-code each and every identifier. To create a macro one must use the `#define` keyword. Macros can appear anywhere after a widget has been declared. Their use is normally reserved for entire identifier strings as in the examples below. You may however use them to form part of an identifier's parameters, but they resultant identifer will not be available for updating through the use of the GUI editor. 

The following code will create a macro called `SLIDER1` that will define several attributes of a slider widget:

```csharp
#define SLIDER1 colour("red"), fontColour("yellow"), outlineColour("red"), range(0, 1000, 500)
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
Macros defined in the Cabbage section of your code can also be accessed in the Csound section using the same syntax. For example, the code below will cause Csound to print the value of the SLIDER one macro when instrument 1 is first started

```csharp
<Cabbage>
form caption("Test") size(350, 200), colour(58, 110, 182), guiMode("queue"), pluginId("sfi1")
rslider bounds(20, 8, 117, 121) range(0, 1, 0, 1, 0.001), channel("gain1"), text("Gain")
#define SLIDER1 colour("red"), fontColour("yellow"), outlineColour("red"), range(0, 1000, 500)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d 
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
ksmps = 32
nchnls = 2
0dbfs = 1

instr 1
    printf_i "%s \n", 1, $SLIDER1 
endin

</CsInstruments>
<CsScore>
i1 0 z
</CsScore>
</CsoundSynthesizer>
```

> Note that macros passed from the Cabbage section to the Csound orchestra are string macros. Therefore you will need to convert to a number if you wish to pass numbered macros between Cabbage and the Csound orchestra.   

