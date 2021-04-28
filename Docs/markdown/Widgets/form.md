# Form

Form creates the main application window. pluginId() is the only required identifier. The default values for size are 600x300. 

<big></pre>
form WIDGET_SYNTAX
</pre></big>

### Specific Identifiers
{! ./markdown/Widgets/Properties/caption.md !} 

{! ./markdown/Widgets/Properties/size.md !} 

{! ./markdown/Widgets/Properties/pluginId.md !} 

{! ./markdown/Widgets/Properties/guiRefresh.md !}     

{! ./markdown/Widgets/Properties/guiMode.md !}   

{! ./markdown/Widgets/Properties/import.md !}  

{! ./markdown/Widgets/Properties/bundle.md !}  

{! ./markdown/Widgets/Properties/titleBarColour.md !}  

{! ./markdown/Widgets/Properties/style.md !}  

{! ./markdown/Widgets/Properties/sideChain.md !}  

{! ./markdown/Widgets/Properties/typeface.md !}  

{! ./markdown/Widgets/Properties/latency.md !} 

{! ./markdown/Widgets/Properties/autoUpdate.md !}  

{! ./markdown/Widgets/Properties/sideChain.md !}  

### Common Identifiers

{! ./markdown/Widgets/Properties/colour.md !}   

{! ./markdown/Widgets/Properties/fontColour.md !}  

<!--(End of identifiers)/-->
![](../images/formExample.png)

##Example
<!--(Widget Example)/-->
```csharp
<Cabbage>
form size(400, 500), caption("Form Example"), pluginID("plu1"), colour(39, 40, 34)
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

;basic usage
instr 1 
endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
;starts instrument 1 and runs it for a week
i1 0 z
</CsScore>
</CsoundSynthesizer>
```
<!--(End Widget Example)/-->
