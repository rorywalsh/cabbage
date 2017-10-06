# File Structure and Syntax

All Cabbage source files are made up of several sections which are defined in XML type tags. The most important sections are the **Cabbage**, **CsInstruments**, and **CsScore** sections. The **cabbage** section comprises of Cabbage syntax which defines how an instrument will look. Information on Cabbage syntax can be found [here](cabbage_syntax.md). All other sections will contain Csound code only. The **CsInstruments** and **CsScore** sections determine how an instrument will sound, and when it will start start. All Csound code is case sensitive. That means upper-case letters are not the same as lower-case letters. Presented below is the typical document structures for a single Cabbage file. 

```csharp
<Cabbage>
//cabbage code used to create widgets
</Cabbage>
<CsoundSynthesizer>
//all code relating to Csound should be encapsulated between 
<CsoundSynthesizer> and </CsoundSynthesizer>
<CsOptions>
//this sections tell Csound how to interact with various devices and hardware
</CsOptions>
<CsInstruments>
//this section contains instrument definitions
</CsInstruments>  
<CsScore>
//this section tells Csound when and how to perform instruments defined
//in the CsInstruments section above. 
</CsScore>
</CsoundSynthesizer>
```

Csound code can be described in terms of its syntax and grammar. Each section uses a slightly different syntax. For instance, the syntax used in the **CsInstruments** section is not the same as the simplified syntax used in the **CsScore** section. While the score section is made up of different statements, the CsInstruments syntax is made up of 6 different components: 

- keywords 
- variables
- constants
- opcodes
- operators 
- comments

Let's consider each one in turn.  