#Cabbage Syntax

Cabbage widgets can loosly be split into two groups, interactive widgets and non-interactive widgets. Each interactive widget, such as a slider, a button, etc., will be accessible in a plugin host. The non-interactive widgets, such as group boxes and images, can only be controlled manually, or via Csound. The name that appears beside each native slider in the plugin host will be the assigned channel name for that widget. 

The syntax for each widget follows a pretty simple pattern. The first thing to delcare is the widet type which is done simply by placing a widget name at the start of a line of code in the Cabbage section of the source file.

```csharp
<Cabbage>
(...)
rslider
(...)
</Cabbage>
```

Each Cabbage widget has a corresponding set of identifiers that can be set to control its appearance and behaviour. These identifiers are placed on the same line of code as the widget name. For example, the following code creates an rslider, sets its size to 100px's by 100px's and positions it 10px's down from the top of the screen, and 10px's in from the left. It also sets the range of the slider. 

```csharp
<Cabbage>
(...)
rslider bounds(10, 10, 100, 100), range(0, 1, .5)
(...)
</Cabbage>
```
Not all widgets support the same identifiers however. For example, a groupbox will never need to have a channel assigned to it because it's a static control. Likewise buttons don't need to use the range() identifier. Parameters presented in the following reference manual within quotation marks represent string values, while those without represent floating point decimals, or integer values. Please use the menu on the left to navigate through the various types of widget's supported be Cabbage. 

> bounds() must be set for every widget. All other identifiers will be given default values if none are set. 
> Only ; type comments can be used in the Cabbage section of code, /**/ will not work. 