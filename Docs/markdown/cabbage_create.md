# cabbageCreate

This opcode can be used to create widgets directly within a Csound orchestra.  

### Syntax

<pre><b>cabbageCreate</b> SWidgetType, SWidgetIdentifiers</pre>

#### Initialization

* `SWidgetType` -- widget type name, i.e, rslider, combobox, checkbox, etc
* `SWidgetIdentifiers` -- widget identifiers, passed a single string. If you are using quotation marks, don't forget to escape the inner ones. You can also avoid having to do so by wrapping your identifier code in {{ }}. See Csound string type documentation for more details. 



### Example

![Cabbage Create](cabbageCreate.gif)

This example generated a gris of 128 checkbox widgets. It then randomly assigns colours to each one and generates some simple tones.  

```csharp
<Cabbage>
form caption("What have I done!!") size(820, 500), guiMode("queue") colour(0, 0, 0), pluginId("def1")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d --midi-key-cps=4 --midi-velocity-amp=5
</CsOptions>
<CsInstruments>
ksmps = 32
nchnls = 2
0dbfs = 1

instr 1
    iX, iY init 0
    iWidgetCount init 0
    while iY < 8 do
        while iX < 16 do
            SWidget sprintf "bounds(%d, %d, 50, 50), channel(\"check%d\"), colour:1(%d, %d, %d)", 10+iX*50, 10+iY*50, iWidgetCount, rnd(255), rnd(255), rnd(255)
            cabbageCreate "checkbox", SWidget
            iWidgetCount += 1
            iX += 1
        od
        iX = 0
        iY += 1
    od
    
    if metro(20) == 1 then
        kOnOff  = (random:k(0, 100) > 50 ? 0 : 1)
        kBox = random:k(36, 88)
        SWidgetChannel sprintfk "check%d", kBox
        cabbageSetValue SWidgetChannel, kOnOff
        event "i", "Synth", 0, 2, int(kBox), kOnOff
    endif
    
endin

instr Synth
    aEnv expon 1, p3, 0.01
    aOut oscil aEnv, cpsmidinn(p4)
    outs aOut*p5, aOut*p5
endin
</CsInstruments>
<CsScore>
f1 0 4096 10 1 .5 .25 .17
i1 0 z
</CsScore>
</CsoundSynthesizer>
```

> Added in Cabbage v2.5.44