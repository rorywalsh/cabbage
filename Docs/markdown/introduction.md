#Using Cabbage

Cabbage is a software for prototyping and developing audio instruments with the Csound audio synthesis language. Instrument development and prototyping is carried out with the main Cabbage IDE. Users write and compile Csound code in a code editor. If one wishes one can also create a graphical frontend, although this is no longer a requirement for Cabbage. Any Csound file can be run with Cabbage, regardless of whether or not it has a graphical interface. Cabbage is designed for realtime processing in mind. It is possible to use Cabbage to run Csound in the more traditional score-driven way, but your success may vary. 

Cabbage is a 'host' application. It treats each and every Csound instruments as a unique audio plugin, which gets added to a digital audio graph (DAG) once it is compiled. The graph can be opened and edited at any point during a performance. If one wishes to use one of their Csound instruments in another audio plugin host, such as Reaper, Live, Bitwig, Ardour, QTractor, etc, they can export the instrument through the 'Export instrument' option.  

##Using these docs
The following pages will take you through the basic using of Cabbage; from navigating features of the main development environment to exporting VST plugins. If you wish to start making sounds straight away, navigate to the [First synth](first_synth.html) page and start there. If you wish to learn more about Cabbage and its features, visit the [Using Cabbage](../using_cabbage.html) page. If you are curious about Csound and wish to learn more visit the [Beginners](../beginners.html) section.  

A reference section for all the available GUI widgets is available from the mune on the right. This reference manual can be launched directly from the Cabbage code editor.  
![Button](images/screens/ConvolutionReverbScreen.png "Convolution Reverb plugins included with Cabbage examples")
