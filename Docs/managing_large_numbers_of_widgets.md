# Managing large numbers of widgets

It can seem like a lot of work at times to deal with large numbers of widgets, but with a little thought and consideration things can be made a lot easier. The previous instrument (see [WidgetArrays](./widget_arrays.md)) will serve as a good starting point to explore this topic. 

The [WidgetArrays](./widget_arrays.md) example dynamically created and positioned 100 check box widgets to the screen. Let's say we turn the matrix of check boxes into a simple step sequencer. To do so we will need to check the value of each check box in each column at a set rate. The long way to do this would be to test the state of each check box manually on every k-cycle. The code for that might look like this.

```csharp
instr 1
k1 chnget "checkbox1"
k2 chnget "checkbox2"
k3 chnget "checkbox3"
k4 chnget "checkbox4"
k5 chnget "checkbox5"
k6 chnget "checkbox6"
k7 chnget "checkbox7"
k8 chnget "checkbox8"
k9 chnget "checkbox9"
k10 chnget "checkbox10"
//todo add variables for the next 90 checkboxes...
(...)
```
The above code will provide the values of the first 10 check boxes. We would need to then add 90 more calls to **chnget** to get the values of the remaining check boxes. That's obviously a very long way of doing things. 

If, however, the channel names are consistent and use numbers then it is easy to use an array to hold the value of each widget. Because we used a widget array to create the widgets, we know that each channel will have a number appended to it that goes from 1 to the number of widgets. We can easily use a **sprintfk** opcode to generate our channels names while we run through a while loop. 

```csharp
(...)
kSteps[] init 100
kIndex = 0
while kIndex<100 do
	kSteps[kIndex] chnget sprintfk("step%d", kIndex+1)
	kIndex = kIndex+1
od 
(...)
```
In the code presented above, the array **kSteps[]** will be filled with the value of each channel, ranging from the channel named **step1** to **step100**. The values of each channel are written to each index of the **kSteps[]** on every k-cycle. If a user presses one of the on-screen check boxes, the kSteps array will update. 

Following this we need to check the values of each index of the kSteps array. If any of them are set to 1 we can trigger a note. Of course we will need to deal with the check boxes on a column by column basis. 

Introducing a metro opcode to count each column will provide us with a step counter. We then need to check each check box in each column, one after another in time with the metro opcode. Something like this will work nicely:

```csharp
kColumn init 0
kRow = 0
if metro(1)==1 then
	kColumn = kColumn<9 ? kColumn+1 : 0
	while kRow<100 do
			if kSteps[kRow+kColumn]==1 then
				event "i", 1000, 0, 1,  (kRow+10)*10
			endif  
		kRow = kRow+10
	od		
endif 
```   
kColumn will increment by one every second. When it does, we check the corresponding vertical column of check boxes for ones that are on. If they are on, we trigger instrument 1000 to play. We pass **(kRow+10)*10** as the frequency for each instance of instr 1000 that is triggered. In this case instrument 1000 is a very simple synthesier, but it could be as complex an instrument as you wish to make it. 

Provided below is the complete source code for a very simple step sequencer. Although it use 100 on-screen widgets, An impressively small amount of code is needed in order to create something quite musical.    


```csharp
<Cabbage>
form size(260, 290), caption(""), colour(55, 110, 185) pluginID("plu1")
checkbox bounds(-100, -100, 25, 25), text(""), widgetarray("step", 100), value(0)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d --midi-key=4
</CsOptions>
<CsInstruments>
sr = 44100
ksmps = 64
nchnls = 2
0dbfs=1

instr 2
kSteps[] init 100
kIndex = 0
kColumn init 0
kRow = 0

while kIndex<100 do
	kSteps[kIndex] chnget sprintfk("step%d", kIndex+1)
	kIndex = kIndex+1
od

if metro(2)==1 then
	kColumn = kColumn<9 ? kColumn+1 : 0
	while kRow<100 do
			if kSteps[kRow+kColumn]==1 then
				event "i", 1000, 0, 1,  (kRow+10)*10
			endif  
		kRow = kRow+10
	od		
endif            
endin

instr 1000
a1 expon .2, p3, .0001
a2 oscili a1, p4
outs a2, a2
endin

instr 1
iCnt init 0
iCntRows init 0
kMetro metro 1
until iCnt > 100 do
    S1 sprintf "pos(%d, %d)", iCnt%10*25+5, iCntRows*25
    S2 sprintf "step_ident%d", iCnt+1
    chnset S1, S2
    iCnt=iCnt+1
    iCntRows = (iCnt%10==0 ? iCntRows+1 : iCntRows)
enduntil
endin


</CsInstruments>  
<CsScore> 
i1 0 1
i2 1 1000
</CsScore>
</CsoundSynthesizer>
```

> Note that `chnget` can be used with strings that are updated at k-rate, `chnset` cannot. This issue is addressed above by calling `chnset` in `instr 1` in an i-rate loop. This limitation will be removed in Csound version 6.10.
