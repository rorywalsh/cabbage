# cabbageGet

Gets the widget identifier value, for example the current colour, size, position, etc. You can add an optional trigger output argument. This will send a trigger signal of 1 whenever the value has changed. 

### Syntax

<pre>iIdent <b>cabbageGet</b> SChannel, SIdentifier</pre>
<pre>kIdent <b>cabbageGet</b> SChannel, SIdentifier</pre>
<pre>SIdent <b>cabbageGet</b> SChannel, SIdentifier</pre>
<pre>kIdent [, kTrig] <b>cabbageGet</b> SChannel, SIdentifier</pre>
<pre>SIdent [, kTrig] <b>cabbageGet</b> SChannel, SIdentifier</pre>

<pre>iIdent[] <b>cabbageGet</b> SChannel, SIdentifier</pre>
<pre>kIdent[] <b>cabbageGet</b> SChannel, SIdentifier</pre>
<pre>SIdent[] <b>cabbageGet</b> SChannel, SIdentifier</pre>
<pre>kIdent[] [, kTrig] <b>cabbageGet</b> SChannel, SIdentifier</pre>
<pre>SIdent[] [, kTrig] <b>cabbageGet</b> SChannel, SIdentifier</pre>

<pre>kIdent <b>cabbageGet</b> SChannel</pre>
<pre>SIdent <b>cabbageGet</b> SChannel</pre>
<pre>kIdent [, kTrig] <b>cabbageGet</b> SChannel</pre>
<pre>SIdent [, kTrig] <b>cabbageGet</b> SChannel</pre>

>The versions of <b>cabbageGet</b> that only take a single channel string are typically used to query [reserved channels](./macros_and_reserved_channels.md). 

#### Initialization

* `SChannel` -- widget's channel name
* `SIdentifier` -- widget's identifier you wish to query. This should be passed without any parenthesis. 

#### Performance

* `kIdent/iIdent` -- the value of a widget's numeric identifier, i.e, `visible()`, `velocity()`, `corners()` etc. 
* `kIdent[]/iIdent[]` -- an array that holds the value of a widget's numeric identifier in case where more that one parameter is passed, , i.e, `bounds()`, `range()`, `colour()` etc. 
* `SIdent` -- the value of a widget's string identifier, i.e, `shape()`, `text()`, `channel()` etc. 
* `SIdent[]` -- an array that holds the value of a widget's string identifier in case where more that one parameter is passed, , i.e, `text()` in buttons, `channel()` for xypads, range sliders, etc. 
* `kTrig` -- an optional trigger output. Will be 1 for a single k-cycle whenever the widget value has changed. Otherwise it will remain 0.  



### Example

```csharp
<Cabbage>
form caption("Test") size(350, 200), colour(58, 110, 182), guiMode("queue"), pluginId("sfi1")
rslider bounds(20, 8, 60, 60) range(0, 1, 0, 1, 0.001), colour(255, 0, 0), channel("gain"), text("Gain")
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
    kColour[] cabbageGet "gain", "colour"
    printarray kColour, metro(1)
endin

</CsInstruments>
<CsScore>
i1 0 z
</CsScore>
</CsoundSynthesizer>
```

> Added in Cabbage v2.5.44