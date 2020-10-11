# Plants

This section deals with advanced features of plants and assumes the reader already has prior knowledge of [plants](./plants.md) in Cabbage.

Reusable and modular plants can be defined in a special plant XML file that should contain you Cabbage plant code, and a set of Csound UDOs to interact with the plant. To import the plant xml you must use the import() identifier when declaring your form. import() can be passed any number of import files. For example:

```
<Cabbage>
form caption("RadioThings") size(440, 180), colour(58, 110, 210), import("plant.xml")
radioValueButtonGroup bounds(18, 18, 258, 110), channel("radioGroup"), namespace("rw")
</Cabbage>
```

The plant xml file, which can take any file extension, should have the following structure:

``` xml
<?xml version="1.0" encoding="UTF-8"?>
<plant>
	<namespace> 
	</namespace>
	<name> 
	</name>
	<cabbagecode>
	</cabbagecode>
	<cabbagecodescript>
	</cabbagecodescript>
	<csoundcode>
	</csoundcode>
	<help>
	</help>
</plant> 
```

### namespace
A unique string should be added to the **namespace** section. This will ensure that plants with the same name, but from different authors can be loaded into a .csd file. Note the namespace name is case sensitive.

### name
The **name** section should be passed the name of the plant. As with the case of the namespace attribute, this is always case sensitive. 

### cabbagecode
The **cabbagecode** section should provide code for a single plant. It should adhere to the same structure as a standard Cabbage plant, i.e, it should contain opening and closing curly brackets spread over at least 2 lines of code. Each widgets should have a channel attached to it in the usual fashion. When Cabbage imports these widgets into a .csd, it will pre-pend a channel string to the channels defined in your plant. This ensure that each instance of a plant will have unique channel names. you can create an instance of a custom plant in the same way you create any widget in Cabbage. But you must import the plant xml, as described above, before trying to create one. The following code will import a plnt xml, and create an instance of that plant.

```
<Cabbage>
form caption("RadioThings") size(440, 180), colour(58, 110, 210), import("plant.xml")
radioValueButtonGroup bounds(18, 18, 258, 110), channel("radioGroup"), namespace("rw")
</Cabbage>
```
Each widget in the imported plant will have "radioGroup" prepended to its channel name. This is required to ensure that each channel has a unique name, which is required when you instantiate more than one of the same plant. 


### cabbagecodescript
Javascript code can be added to the *cabbagecodescript*. This script can be used to generate Cabbage code that will then be added to your .csd file. This can be extremely useful if you are creating plants with lots of widgets. Use the Cabbage.print() method to write any JS strings as cabbagecode that will be read by Cabbage when the instrument loads. The code below is an example taken from the GridSequencer.xml file in the Cabbage/instructional examples folder.

``` javascript
var channelNumber = 1;
Cabbage.print("image bounds(10, 10, 360, 178) colour(0, 0, 0, 0){");

for(var y = 0 ; y < 8 ; y++)
{    
  for (var x = 0 ; x < 16 ; x++)
  {
  if(y==0)
    Cabbage.print("image bounds("+((x*22))+", 0, 23, 176), colour(100, 100, 100), identchannel(\"scrubberIdent"+(x+1)+"\")");

  Cabbage.print("checkbox bounds("+x*22+","+y*22+", 20, 20), colour(\"red\"), channel(\"gridChannel"+channelNumber+"\")");
  channelNumber++;
  }
}
Cabbage.print("}");
```

Calls to Cabbage.print() are made first to declare an image widget to be used as the main plant container. Then the same method is called in a loop to create an 8 x 16 matrix of checkbox widgets. Finally, a closing curly bracket is added to complete the plant. 

### csoundcode

The **csoundcode** section is where you declare your custom user defined opcodes. These UDOs can be used to interact with your plant code. It is good practice to use the namespace defined in the **namespace** section as a pre or post-fix to your UDO name. Every UDO you create needs to accept a string parameter that can be used to construct channel names that match those created by Cabbage. See below for a full example of how all this works. 

### info
The **info** section should be filled with information about how to use your custom widget and associated UDOs. 


## A Complete example

The following example demonstrates the creation and use of a simple ADSR widget. The example in full can be seen in the Instructional examples that ship with Cabbage. 

The following is a simple abstraction of an ADSR in Cabbage code. 
```
<cabbagecode>
groupbox bounds(0, 0, 330, 122) text("ADSR") {
    rslider bounds(12, 32, 70, 70) channel("attack") range(0, 1, 0.01, 1, 0.001) text("Att.") 
    rslider bounds(90, 32, 70, 70) channel("decay") range(0, 1, 0.2, 1, 0.001) text("Dec") 
    rslider bounds(164, 32, 70, 70) channel("sustain") range(0, 1, .6, 1, 0.001) text("Sus.") 
    rslider bounds(240, 32, 70, 70) channel("release") range(0, 1, 0.8, 1, 0.001) text("Rel.")
} 
</cabbagecode>
```
A corresponding UDO might look like this:

```
<csoundcode>
opcode cab_adsr,a,S
    SChan xin
    iCnt init 0
    iAtt chnget sprintf("%sattack", SChan)
    iDec chnget sprintf("%sdecay", SChan)
    iSus chnget sprintf("%ssustain", SChan)
    iRel chnget sprintf("%srelease", SChan)
    aEnv madsr iAtt, iDec, iSus, iRel
    xout aEnv		
endop
</csoundcode>
```

Note the use of the `sprintf` opcode. its used to construct unique channel names so that the UDO can query the correct channels from the corresponding Cabbage code. Below is the full example. Note how the plant file is imported during the `form` declaration. The plant itself is called with its own `bounds()`, and unique `channel()` identifiers. This unique channel is then used in the corresponding call to the `cab_adsr` opcode. 


```
<Cabbage>
form caption("Custom ADSR plant") size(320, 300), colour(18, 60, 82), import("adsr.plant"), pluginID("def1")
adsr bounds(10, 10, 300, 120), channel("adsr"), namespace("cab")
keyboard bounds(8, 158, 300, 95)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d --midi-key-cps=4 --midi-velocity-amp=5
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1

;instrument will be triggered by keyboard widget
instr 1
aEnv cab_adsr "adsr"
aOut vco2 p5, p4
outs aOut*aEnv, aOut*aEnv
endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
</CsScore>
</CsoundSynthesizer>
```
