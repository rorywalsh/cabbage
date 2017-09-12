#Constants and variables.

Constants are easy to spot in Csound code. They appear as fixed numbers. Their values cannot change at any stage during performance, or between performances. Variables on the other hand are temporary memory slots that can be used to store data. The three simplest and most common types of variables in Csound are *i*, *k* and *a*-rate variables. These types of variables are very easy to spot in Csound because they will always begin with an i, k or a.

```csharp
instr 1
iAmp = 1
kFreq = 440
aSignal = 0
endin
```

Variables can be given any name so long as they start with an i, k or a. But what do the i, k and a mean?

When Csound starts it begins looping very quickly through the code of each of its instruments. For the sake of simplicity we can say that it does this sampling rate times a second. So by default, it will read and process each instrument's code 44100 times a second. Each time it runs through the instrument code it will update its variables. How quickly it updates each of its variables is determined by the variable type.

a-rate variables are updated each time SR times a second. k-rate variables are updated less often, and i-rate variables are only updated once just as an instrument starts. Why the different update rates? Performance. Each and every update to a variable forces the PC to compute something. And each time it does it uses some of its CPU. By limiting the update rate of different variables users can improve the performance
of their instruments and avoid unwanted audio dropouts.

### p variables

There also exists *p* variables which are special i-rate variables. The unique thing about *p* variables is their values can be passed to an instrument from either the Csound score section, or from MIDI data. Cabbage uses p-variables quite a lot when sending MIDI data to an instrument. We'll seemore on these variables in the section on developing a simple synthesiser. 

> The general rule when it comes to choosing variable types in Csound is to use a-rate variables for anything that is producing an audio signal or is being used to modulate audio rate paraemeters. k-rate variables can be used for controlling parameters such as automation controls, and i-rate variables are used to set values when an instrument starts.   
