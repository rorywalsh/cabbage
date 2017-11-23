# Plants

This section deals with advanced features of using sol-called plants and assumes the reader already has prior knowledge of [plants](./plants.md) in Cabbage.

Reusable and modular plants can be defined in a special plant .xml file that should contain you Cabbage plant code, and a set of Csound UDOs to interact with the plant. The xml files should have the following structure:

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

###namespace###
A unique string should be added to the **namespace** section. This will ensure that plants with the same name, but from different authors can be loaded into a .csd file. Note the namespace name is case sensitive.

###name###
The **name** section should be passed the name of the plant. As with the case of the namespace attribute, this is always case sensitive. 

###cabbagecode###
The **cabbagecode** section should provide code for a single plant. It should adhere to the same structure as a standard Cabbage plant, i.e, it should contain opening and closing curly brackets spread over at least 2 lines of code. Each widgets should have a channel attached to it in the usual fashion.  

###cabbagecodescript###
Javascript code can be added to the *cabbagecodescript*. This script can be used to generate Cabbage code that will then be added to yur .csd file. This can be extremely useful if you are creating plants with lots of widgets. Use the Cabbage.print() method to write any JS strings as cabbagecode that will be read by Cabbage when the instrument loads. The code below is an example taken from the GridSequencer.xml file in the Cabbage/instructional examples folder.

``` javascript
var channelNumber = 1;
Cabbage.print("image bounds(10, 10, 360, 178) colour(0, 0, 0, 0){");

for(var y = 0 ; y &lt; 8 ; y++)
{    
  for (var x = 0 ; x &lt; 16 ; x++)
  {
  if(y==0)
    Cabbage.print("image bounds("+((x*22))+", 0, 23, 176), colour(100, 100, 100), identchannel(\"scrubberIdent"+(x+1)+"\")");

  Cabbage.print("checkbox bounds("+x*22+","+y*22+", 20, 20), colour(\"red\"), channel(\"gridChannel"+channelNumber+"\")");
  channelNumber++;
  }
}
Cabbage.print("}");
```

Calls to Cabbage.print() are made first to declare an image widget to be used as the main plant container. Then the same method is called in a loop to create an 8 x 16 matrix of checkbox widgets. Finally, a closing curly bracket is added to complete the plant. Note that less than and greater than signs are not valid in xml. so one must use a character reference instead. you can learn about character references [here](https://en.wikipedia.org/wiki/List_of_XML_and_HTML_character_entity_references)

###csoundcode###

The **csoundcode** section is where you 



For more advanced uses of plants, see [Custom plant imports](./custom_plant_import.md)
![Plants](images/plants.gif)