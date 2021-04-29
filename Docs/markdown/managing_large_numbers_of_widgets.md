# Managing large numbers of widgets

It can seem like a lot of work at times to deal with large numbers of widgets. Endless lines of Cabbage code can be difficult to maintain. Take for example a grid of 8x16 checkboxes. Writing 128 individual lines of Cabbage code for each checkbox seems a little tedious. It is much simpler to generate the widgets using a while loop in Csound. 

```csharp
<Cabbage>
form caption("That's a lot of checkboxes!") size(820, 440), guiMode("queue") colour(0, 0, 0), pluginId("def1")
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
            SWidget sprintf "checkbox bounds(%d, %d, 50, 50), channel(\"check%d\"), colour:1(%d, %d, %d)", 10+iX*50, 10+iY*50, iWidgetCount, rnd(255), rnd(255), rnd(255)
            cabbageCreate SWidget
            iWidgetCount += 1
            iX += 1
        od
        iX = 0
        iY += 1
    od
endin

</CsInstruments>
<CsScore>
i1 0 z
</CsScore>
</CsoundSynthesizer>
```

The above code will arrange a set of checkboxes in a grid. The channels of each box will be as follows:

["check1  ", "check2  ", "check3  ", "check4  ", "check5  ", "check6  ", "check7  ", "check8  "]
["check9  ", "check10 ", "check11 ", "check12 ", "check13 ", "check14 ", "check15 ", "check16 "]
["check17 ", "check18 ", "check19 ", "check20 ", "check21 ", "check22 ", "check23 ", "check24 "]
["check25 ", "check26 ", "check27 ", "check28 ", "check29 ", "check30 ", "check31 ", "check32 "]
["check33 ", "check34 ", "check35 ", "check36 ", "check37 ", "check38 ", "check39 ", "check40 "]
["check41 ", "check42 ", "check43 ", "check44 ", "check45 ", "check46 ", "check47 ", "check48 "]
["check49 ", "check50 ", "check51 ", "check52 ", "check53 ", "check54 ", "check55 ", "check56 "]
["check57 ", "check58 ", "check59 ", "check60 ", "check61 ", "check62 ", "check63 ", "check64 "]
["check65 ", "check66 ", "check67 ", "check68 ", "check69 ", "check70 ", "check71 ", "check72 "]
["check73 ", "check74 ", "check75 ", "check76 ", "check77 ", "check78 ", "check79 ", "check80 "]
["check81 ", "check82 ", "check83 ", "check84 ", "check85 ", "check86 ", "check87 ", "check88 "]
["check89 ", "check90 ", "check91 ", "check92 ", "check93 ", "check94 ", "check95 ", "check96 "]
["check97 ", "check98 ", "check99 ", "check100", "check101", "check102", "check103", "check104"]
["check105", "check106", "check107", "check108", "check109", "check110", "check111", "check112"]
["check113", "check114", "check115", "check116", "check117", "check118", "check119", "check120"]
["check121", "check122", "check123", "check124", "check125", "check126", "check127", "check128"]

If we want to step through each checkbox and query its value, we can do so using a k-rate while loop. 

```csharp
kIndex = 0
kColumn init 0
kRow = 0 

if metro(2)==1 then        
	while kRow<8 do
		kVal cabbageGetValue sprintfk("check%d", kRow*16+kColumn)
		if kVal == 1 then
			//if checkbox is one, do something
		endif  
		kRow = kRow+1            
	od
	kColumn = kColumn<15 ? kColumn+1 : 0		
endif     
```

The code above will cause Csound to read through the values of a column. The column number increments twice a second. Finally, we can add a simple synthesiser to the code. This will get triggered whenever a checkbox is on. In this way, one can generate a grid-style step sequencer with very little code. Here is the entire instrument. 

```csharp
<Cabbage>
form caption("That's a lof of checkboxes!") size(820, 420), guiMode("queue") colour(0, 0, 0), pluginId("def1")
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
            SWidget sprintf "checkbox bounds(%d, %d, 50, 50), channel(\"check%d\"), colour:1(%d, %d, %d)", 10+iX*50, 10+iY*50, iWidgetCount, rnd(255), rnd(255), rnd(255)
            cabbageCreate SWidget
            iWidgetCount += 1
            iX += 1
        od
        iX = 0
        iY += 1
    od
    
    kIndex = 0
    kColumn init 0
    kRow = 0 
    
    if metro(2)==1 then        
        while kRow<8 do
            kVal cabbageGetValue sprintfk("check%d", kRow*16+kColumn)
            if kVal == 1 then
                event "i", "Synth", 0, 1, ((8-kRow)+10)*10, .3
            endif  
            kRow = kRow+1            
        od
        kColumn = kColumn<15 ? kColumn+1 : 0		
    endif     
endin

instr Synth
    aEnv expon 1, p3, 0.01
    aOut oscil aEnv, p4
    outs aOut*p5, aOut*p5
endin
</CsInstruments>
<CsScore>
f1 0 4096 10 1 .5 .25 .17
i1 0 z
</CsScore>
</CsoundSynthesizer>
```
