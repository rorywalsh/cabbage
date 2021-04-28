# GenTable

Gentable displays the contents of a function table, multiple tables, or files. Function tables that have their identifier active() set to one, and use a negative GEN05, GEN07 or GEN02 can be edited by clicking and dragging on the table. Only tables of size less or equal to 16384 points can be manually edited. If you wish to save any tables that you create you can use one of Csound's built-in table saving opcode "ftsave". If you need only display the contents of a static sound file from disk, use soundfiler as it will be fast and use less CPU. If on the other hand you wish to create some user editable envelopes, or display FFT or live waveform data use table. 


<big></pre>
gentable [amprange](#amprange)(min, max, tablenumber, quantise), 
[bounds](#bounds)(x, y, width, height), 
[tablenumber](#tablenumber)(val), 
[tablecolour:N](#tablecolour)("colour1"), 
[tablebackgroundcolour](#tablebackgroundcolour)("colour"), 
[tablegridcolour](#tablegridcolour)("colour"), 
[file](#file_gentable)("filename"), 
[scrubberposition](#scrubberposition_gentable)(val, tableNum), 
[samplerange](#samplerange)(min, max), 
[samplerange](#samplerange)(min, max), 
[outlinethickness](#outlinethickness)(value), 
[identchannel](#identchannel)("channel"), 
[alpha](#alpha)(val), 
[visible](#visible)(val), 
[rotate](#rotate)(radians, pivotx, pivoty), 
[rotate](#rotate)(radians, pivotx, pivoty), 
[tofront](#tofront)(), 
[active](#active)(val), 

</pre></big>

### Specific Identifiers

{! ./markdown/Widgets/Properties/amprange.md !} 

{! ./markdown/Widgets/Properties/bounds.md !} 

{! ./markdown/Widgets/Properties/tablenumber.md !} 

{! ./markdown/Widgets/Properties/tablecolour.md !} 

{! ./markdown/Widgets/Properties/tablebackgroundcolour.md !} 

{! ./markdown/Widgets/Properties/tablegridcolour.md !} 

{! ./markdown/Widgets/Properties/file_gentable.md !} 

{! ./markdown/Widgets/Properties/scrubberposition_gentable.md !} 

{! ./markdown/Widgets/Properties/samplerange.md !} 

{! ./markdown/Widgets/Properties/samplerange.md !} 

{! ./markdown/Widgets/Properties/outlinethickness.md !}  With regards to gentable, this sets the thickness of the waveform's outline. If you are drawing spectrograms setting this to 0 will speed up the process and reduce CPU drain. 

### Common Identifiers

{! ./markdown/Widgets/Properties/identchannel.md !} 

{! ./markdown/Widgets/Properties/alpha.md !} 

{! ./markdown/Widgets/Properties/visible.md !} 

{! ./markdown/Widgets/Properties/rotate.md !} 

{! ./markdown/Widgets/Properties/rotate.md !} 

{! ./markdown/Widgets/Properties/tofront.md !} 

{! ./markdown/Widgets/Properties/active.md !} 
<!--(End of identifiers)/-->

>If you need to redraw tables quickly, make sure they don't use negative GEN routines as it will seriously slow down redrawing. 

![](../images/gentable.gif)

##Example
<!--(Widget Example)/-->
```csharp
<Cabbage>
form caption("Gentable Example") size(400, 300), colour(220, 220, 220), pluginID("def1")
label bounds(8, 6, 368, 20), text("Basic Usage"), fontcolour("black")
gentable bounds(8, 30, 380, 70), tablenumber(1), tablegridcolour(0, 0, 0, 255), fill(0)
groupbox bounds(8, 110, 380, 177), text("Randomly Updated Identifiers")
gentable bounds(70, 140, 141, 119), tablenumber(1) identchannel("widgetIdent"),  
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d 
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1

giWave ftgen 1, 0, 4096, 10, 1, 1, 1, 1 
seed 0 
;basic usage
instr 1
    aTone oscili chnget:k("gain"), 300
    outs aTone, aTone    
endin

;WIDGET_ADVANCED_USAGE

instr 2
    if metro(1) == 1 then
        event "i", "ChangeAttributes", 0, 1
    endif
endin

instr ChangeAttributes
    SIdentifier init ""
	SIdent sprintf "pos(%d, 140) ", 100 + rnd(100)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "size(%d, %d) ", abs(rnd(200))+40, abs(rnd(100))+50
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "tablecolour:0(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent  
	SIdent sprintf "tablebackgroundcolour:0(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "tablegridcolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent  
	SIdent sprintf "outlinethickness(%f) ", rnd(100)/50
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "alpha(%f) ", 50 + rnd(50)/50
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "visible(%d) ", (rnd(100) > 80 ? 0 : 1)
	SIdentifier strcat SIdentifier, SIdent
    ;send identifier string to Cabbage
    chnset SIdentifier, "widgetIdent"           
endin
                

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
;starts instrument 1 and runs it for a week
i1 0 z
i2 0 z
</CsScore>
</CsoundSynthesizer>
```
<!--(End Widget Example)/-->
