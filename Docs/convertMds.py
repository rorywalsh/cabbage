import markdown
import include
import os, sys
import shutil

if os.path.exists('CabbageDocs') == False:
	os.mkdir("CabbageDocs")
if os.path.exists('CabbageDocs/Widgets') == False:
	os.mkdir("CabbageDocs/Widgets")
if os.path.exists('CabbageDocs/images') == False:
	shutil.copytree("images", "CabbageDocs/images")



def createHtmlFiles( file ):
	mkin = open(file)
	md = markdown.Markdown(extensions = ['include','fenced_code'], output_format="html5")
	gen_html = md.convert(mkin.read())

	fullHtmlOutput ="""
	<!doctype html>
	<html>
	<head>
	<meta charset="utf-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
	<meta name="viewport" content="width=device-width, initial-scale=1">
	<title>Cabbage user Manual</title>
	<link rel="stylesheet" href="http://netdna.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css">
	<link href='http://fonts.googleapis.com/css?family=Roboto+Condensed' rel='stylesheet' type='text/css'>
	<link href='../../Styles/menu.css' rel='stylesheet' type='text/css'>
	<link href='../Styles/menu.css' rel='stylesheet' type='text/css'>
	<link href='./Styles/menu.css' rel='stylesheet' type='text/css'>
	</head>
	<body>
	<input type="checkbox" id="menu-toggle" />
	<label for="menu-toggle" class="menu-icon"><i class="fa fa-bars"></i></label>
	<div class="content-container">
	</div>
	<div class="content">"""

	fullHtmlOutput+=gen_html +"""
	  </div>
	</div>
	<div class="slideout-sidebar">
	<ul>
	<li><a href="index.html">Cabbage User Manual</a></li>
	<li><a href="csound.html">Beginners (Csound)</a><ul>
	<li><a href="file_structure_and_syntax.html">File Structure and Syntax</a></li>
	<li><a href="keywords.html">Keywords</a></li>
	<li><a href="constants_and_variables.html">Constants and Variables</a></li>
	<li><a href="opcodes.html">Opcodes</a></li>
	<li><a href="operators_and_comments.html">Operators and Comments</a></li>
	</ul>
	</li>
	<li><a href="introduction.html">Using Cabbage</a><ul>
	<li><a href="using_cabbage.html">Introduction</a></li>
	<li><a href="cabbage_syntax.html">Cabbage Syntax</a></li>
	<li><a href="beginner_synth.html">A simple Synthesiser</a></li>
	<li><a href="first_effect.html">A simple Effect</a></li>
	<li><a href="audio_graph.html">The Cabbage patcher</a></li>
	<li><a href="exporting.html">Exporting plugins</a></li>
	<li><a href="cabbage_in_host.html">Using plugins in 3rd Party software</a></li>
	<li><a href="distributing.html">Distributing instruments</a></li>
	</ul>
	</li>
	<li><a href="features.html">Features</a><ul>
	<li><a href="plants.html">Plants</a></li>
	<li><a href="identchannels.html">Controlling widgets with Csound</a></li>
	<li><a href="presets.html">Presets</a></li>
	<li><a href="widget_arrays.html">Widget arrays</a></li>
	<li><a href="macros_and_reserved_channels.html">Reserved channels and Macros</a></li>
	<li><a href="managing_large_numbers_of_widgets.html">Managing large numbers of Widgets</a></li>
	<li><a href="using_svgs.html">Using SVGs</a></li>
	<li><a href="soundfiles.html">Working with sound files</a></li>
	<li><a href="host_info.html">Getting host information into Csound</a></li>
	</ul>
	</li>
	<li><a href="cabbage_syntax.html">Widget Reference</a><ul>
	<li><a href="./Widgets/button.html">Button</a></li>
	<li><a href="./Widgets/button_file.html">Button(File)</a></li>
	<li><a href="./Widgets/button_info.html">Button(Info)</a></li>
	<li><a href="./Widgets/checkbox.html">Checkbox</a></li>
	<li><a href="./Widgets/combobox.html">Combobox</a></li>
	<li><a href="./Widgets/csound_output.html">Csound Output</a></li>
	<li><a href="./Widgets/encoder.html">Endless encoder</a></li>
	<li><a href="./Widgets/gentable.html">Gentable</a></li>
	<li><a href="./Widgets/form.html">Form</a></li>
	<li><a href="./Widgets/groupbox.html">Groupbox</a></li>
	<li><a href="./Widgets/hrange.html">Hrange</a></li>
	<li><a href="./Widgets/image.html">Image</a></li>
	<li><a href="./Widgets/keyboard.html">Keyboard</a></li>
	<li><a href="./Widgets/label.html">Labels</a></li>
	<li><a href="./Widgets/listbox.html">Listbox</a></li>
	<li><a href="./Widgets/meter.html">Meteres</a></li>
	<li><a href="./Widgets/numberbox.html">Numberbox</a></li>
	<li><a href="./Widgets/signaldisplay.html">SignalDisplay</a></li>
	<li><a href="./Widgets/sliders.html">Sliders</a></li>
	<li><a href="./Widgets/soundfiler.html">Soundfiler</a></li>
	<li><a href="./Widgets/textbox.html">Textbox</a></li>
	<li><a href="./Widgets/texteditor.html">Texteditor</a></li>
	<li><a href="./Widgets/vrange.html">Vrange</a></li>
	<li><a href="./Widgets/xypad.html">XY Pad</a></li>
	</ul>
	</li>
	<li><a href="add_new_widgets.html">Adding new Widgets</a><ul>
	<li><a href="new_widgets_part_1.html">Part 1</a></li>
	<li><a href="new_widgets_part_2.html">Part 2</a></li>
	<li><a href="new_widgets_part_3.html">Part 3</a></li>
	</ul>
	</li>
	</ul>

	</div>
	</body>
	</html>
	"""

	filename = os.path.splitext(os.path.basename(file))[0]+".html"
	print "Converting: " + file + " to " + filename
	if "Widgets" in file:
		output_file = open("./CabbageDocs/Widgets/"+filename, 'w')
	else:
		output_file = open("./CabbageDocs/"+filename, 'w')

	output_file.write(''.join(fullHtmlOutput))
	output_file.close()


#call createHtmlFiles() on main docs
for file in os.listdir("./markdown"):
   	if file.endswith(".md"):
   		createHtmlFiles("./markdown/"+file)

#call createHtmlFiles() for widgets 
for file in os.listdir("./markdown/Widgets"):
    if file.endswith(".md"):
    	createHtmlFiles("./markdown/Widgets/"+file)



		

# end