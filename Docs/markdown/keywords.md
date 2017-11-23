# Keywords

Keywords are special reserved words that have a unique function and meaning. The two most commonly used keywords in the Csound language are **instr** and **endin**. These two keywords define an instrument block which contain instructions on how an instrument functions. Each instrument must be given a unique name or number which follows the **instr** keyword.

<pre><code data-language="javascript">
instr 1
;to stuff
endin

instr DoStuff
;do stuff
endin
</code></pre>

Other common keywords are found in the **CsInstruments** header section. The header section appears before any instrument block and sets up vital information about things such as sampling rates, sr, the number of audio channels to use, nchnls, and decibels relative to full scale, 0dbfs. 

```csharp
sr = 44100
nchnls = 2
0dbfs = 1

instr 1
;to stuff
endin
```

As most plugins deal with audio on a normalised scale of -1 to 1 it's good practice to leave **0dbfs** set 1. Cabbage instruments use 2 channels by default. If you change **nchnls** to a number greater than 2 you will need to make sure your version of Cabbage supports it.   