from os import listdir
from shutil import copyfile
from distutils.dir_util import copy_tree
import os.path
import os
import sys
from os.path import isfile, join, basename

print("Usage: python convertDocsToOnlineDocs.py ../path_to_cabbage_site_app_dir")

if len(sys.argv) != 2:
	print("You must supply a directory")
	exit(1)

copyfile("../docs.yml", os.path.abspath(sys.argv[1])+"/_data/docs.yml")
outputDir = os.path.abspath(sys.argv[1])+"/_docs"
imagesDir = os.path.abspath(sys.argv[1])+"/images/docs/"
copy_tree("../images/", imagesDir)
properties = []

if os.path.isdir("../markdown/WidgetVideos/"):
	copy_tree("../markdown/WidgetVideos/", imagesDir)

directories = ["", "/Widgets/ExpandedWidgetEntries", "/Widgets/Properties"]
currentDir = os.path.abspath("../markdown")
for dir in directories[0:3]:
	
	# print(dir)
	#print(sys.argv[1])
	#print(os.path.abspath(sys.argv[1]))
	# print(currentDir+dir)
	docfiles = listdir(currentDir+dir)
	os.chdir(currentDir+dir)

	for filename in docfiles:
		if ".md" in filename:
			inputFile = open(filename)
			outputFile = open(outputDir+"/"+inputFile.name, 'w')
			lineText = ""

			# add YAML
			if "Properties" not in dir:
				# print(inputFile.name)
				fileNameOnly = os.path.basename(inputFile.name)
				outputFile.write("---\n")
				outputFile.write("layout: docs\n")
				outputFile.write("title: "+os.path.splitext(fileNameOnly)[0].replace("_", " ").title()+"\n")
				outputFile.write("permalink: /docs/"+os.path.splitext(fileNameOnly)[0]+"/\n")
				outputFile.write("---\n\n")

			# now parse files and update to Gordon's online format
			for line in inputFile:
				
				if "**" in line:
					line = line.replace("**", "`")

				if "(images/" in line:
					line = "![](../../../../images/" +line[line.index("(images/")+8:]

				if "../images/" in line and not ".mp4" in line:
					line = "![](../../images/docs/" +line[line.index("./images/")+9:]

				if "```html" in line:
					line = line.replace("```html", "```csharp")

				if "(./using_svgs.md)" in line:
					line = line.replace("(./using_svgs.md)", "(../using_svgs/index.html)")

				if "(./identchannels.md)" in line:
					line = line.replace("(./identchannels.html)", "(../identchannels/index.html)")

				if "(./widget_arrays.md)" in line:
					line = line.replace("(./widget_arrays.md)", "(../widget_arrays/index.html)")

				if "(./presets.md)" in line:
					line = line.replace("(./presets.md)", "(../presets/index.html)")

				if "(./plants.md)" in line:
					line = line.replace("(./plants.md)", "(../plants/index.html)")

				if "(./first_synth.md)" in line:
					line = line.replace("(./first_synth.md)", "(../first_synth/index.html)")

				if "(./using_cabbage.md)" in line:
					line = line.replace("(./using_cabbage.md)", "(../using_cabbage/index.html)")

				if "(./cabbage_opcodes.md)" in line:
					line = line.replace("(./cabbage_opcodes.md)", "(../cabbage_opcodes/index.html)")

				if "(./beginners.md)" in line:
					line = line.replace("(./beginners.md)", "(../beginners/index.html)")

				if "(./introduction.md)" in line:
					line = line.replace("(./introduction.md)", "(../introduction/index.html)")

				if "(./form.md)" in line:
					line = line.replace("(./form.md)", "(../form/index.html)")

				if "(./macros_and_reserved_channels.md)" in line:
					line = line.replace("(./macros_and_reserved_channels.md)", "(../macros_and_reserved_channels/index.html)")

				if "(./controlling_widgets.md)" in line:
					line = line.replace("(./controlling_widgets.md)", "(../controlling_widgets/index.html)")

				if "(./managing_large_numbers_of_widgets.md)" in line:
					line = line.replace("(./managing_large_numbers_of_widgets.md)", "(../managing_large_numbers_of_widgets/index.html)")

				if "(./sliders.md)" in line:
					line = line.replace("(./sliders.md)", "(../sliders/index.html)")

				if "(./bundle.md)" in line:
					line = line.replace("(./bundle.md)", "(../bundle/index.html)")	

				if "{! " in line:
					line = line.replace("{! ./markdown/Widgets/Properties/", "{% include_relative ")

				if " !}" in line:
					line = line.replace(" !}", " %}")

				if "(./cabbage_syntax.md)" in line:
					line = line.replace("(./cabbage_syntax.md)", "(../cabbage_syntax/index.html)")


				outputFile.write(line)

			inputFile.close()
			outputFile.close()
	
	if "Properties"  in dir:
		for filename in docfiles:
			lst = [filename, '']		
			#filename is an array of all properties
			properties.append(lst)


docfiles = listdir(currentDir+"/Widgets")
# print(currentDir+"/Widgets")
os.chdir(currentDir+"/Widgets")
for filename in docfiles:
	if ".md" in filename:
		inputFileText = open(filename).read()
		for prop in properties:
			if prop[0] in inputFileText:
				properties[properties.index(prop)].append('`'+filename.replace('.md', '`'))


allIdentifiersText = ''
allIdentifiersText += ("---\n")
allIdentifiersText += ("layout: docs\n")
allIdentifiersText += ("title: All Identifiers\n")
allIdentifiersText += ("permalink: /docs/all_identifiers/\n")
allIdentifiersText += ("---\n\n")
allIdentifiersText +='''
# Cabbage Identifiers

Each Cabbage widgets supports a set of identifiers. Below is the full list of all identifiers used across all widgets. Click on a link to read more about the identifier, and the widgets it supports.


'''

properties.sort(key=lambda x: x[0])

linkedProps =  []
cnt = 0
for prop in properties:
	linkedProps.append('')
	list(filter(None, prop))
	for widget in prop[1:]:

		linkedProps[cnt]+= '['+widget.replace('`', '').replace('button_option', 'optionbutton').replace('button_info', 'infobutton').replace('button_file', 'filebutton').replace('csound_output', 'csoundoutput')+'](../'+widget.replace('`', '')+'/index.html) | '
	print(linkedProps[cnt])
	cnt += 1

identifierEntries = ''
cnt = 0
# properties = [prop.replace('[colour_1]', '[colour:1]') for prop in properties]
for prop in linkedProps:
	identifierEntries += '\n{% include_relative '+ properties[cnt][0] + ' %}\n<h5 style="color:#333;background-color: rgb(147, 210, 10);">Supported Widgets:</h5>'+prop+'</h4>'
	cnt += 1
for prop in properties:
	prop[0] = '['+prop[0].replace('.md', '')+'](#'+prop[0].replace('.md', '')+')'
	prop[0] = prop[0].replace('[colour_1]', '[colour:1]')
	prop[0] = prop[0].replace('[colour_0]', '[colour:0]')
	prop[0] = prop[0].replace('[fontColour_0]', '[fontColour:0]')
	prop[0] = prop[0].replace('[fontColour_1]', '[fontColour:1]')
	prop[0] = prop[0].replace('[channel_type]', '[channelType]')
	prop[0] = prop[0].replace('[checkbox_corner]', '[corners(`comboxbox`)]')
	prop[0] = prop[0].replace('[file_combobox]', '[file(`comboxbox`)]')
	prop[0] = prop[0].replace('[file_gentable]', '[file(`gentable`)]')
	prop[0] = prop[0].replace('[file_image]', '[file(`image`)]')
	prop[0] = prop[0].replace('[file_info_button]', '[file(`infobutton`)]')
	prop[0] = prop[0].replace('[file_info_button]', '[file(`infobutton`)]')
	# print(prop)
	allIdentifiersText += prop[0] + '   |   '
	# allIdentifiersText += (', '.join(list(filter(None, prop))))+'\n\n'


allIdentifiersText += identifierEntries
identifierOutputFile = open(outputDir+"/all_identifiers.md", 'w')
identifierOutputFile.write(allIdentifiersText)
identifierOutputFile.close()

