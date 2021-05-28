# cabbageSet

Sets a widget's attribute identifier. Identifiers can be updated by a single string argument, or by passing multiple arguments that comprise the identifiers. If used at perf-time you must pass a trigger signal as the first argument. The i-rate version of this opcode does not need a trigger argument. Multiple identifers can also be updated by passing entire identifier strings   

### Syntax

<pre><b>cabbageSet</b> kTrig, SChannel, SIdentifier, XArgs</pre>
<pre><b>cabbageSet</b> kTrig, SChannel, SIdentifierString</pre>
<pre><b>cabbageSet</b> SChannel, SIdentifierString</pre>

#### Initialization

* `SChannel` -- widget channel name
* `SIdentifier` -- a widget identifier you wish to query. This should be passed without any parenthesis.
* `SIdentifierString` -- the new identifier value you wish to set `SIdentifier` to.

#### Performance

* `kTrig` -- an optional trigger. Updates will only be sent when this is equal to 1

>The versions of <b>cabbageSet</b> that only take two input strings is used to set identifiers at i-rate.

### Example

```csharp
<Cabbage>
form caption("Test") size(350, 200), colour(58, 110, 182), guiMode("queue"), pluginId("sfi1")
rslider bounds(20, 8, 117, 121) range(0, 1, 0, 1, 0.001), channel("gain1"), text("Gain")
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
    cabbageSet metro(10), "gain1", "colour", random:k(100, 255), random:k(100, 255), random:k(100, 255)
endin

</CsInstruments>
<CsScore>
i1 0 z
</CsScore>
</CsoundSynthesizer>
```

> Added in Cabbage v2.5.44