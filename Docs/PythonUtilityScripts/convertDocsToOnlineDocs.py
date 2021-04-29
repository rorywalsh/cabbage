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

if os.path.isdir("../markdown/Widgets/ExpandedWidgetEntries/gifs/"):
	copy_tree("../markdown/Widgets/ExpandedWidgetEntries/gifs/", imagesDir)

directories = ["", "/Widgets/ExpandedWidgetEntries", "/Widgets/Properties"]
currentDir = os.path.abspath("../markdown")
for dir in directories[0:3]:
	
	print(dir)
	#print(sys.argv[1])
	#print(os.path.abspath(sys.argv[1]))
	print(currentDir+dir)
	docfiles = listdir(currentDir+dir)
	os.chdir(currentDir+dir)

	for filename in docfiles:
		if ".md" in filename:
			inputFile = open(filename)
			outputFile = open(outputDir+"/"+inputFile.name, 'w')
			lineText = ""

			# add YAML
			if "Properties" not in dir:
				print(inputFile.name)
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

				if "../images/" in line:
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

				if "(./beginners.md)" in line:
					line = line.replace("(./beginners.md)", "(../beginners/index.html)")

				if "(./introduction.md)" in line:
					line = line.replace("(./introduction.md)", "(../introduction/index.html)")

				if "(./form.md)" in line:
					line = line.replace("(./form.md)", "(../form/index.html)")

				if "(./macros_and_reserved_channels.md)" in line:
					line = line.replace("(./macros_and_reserved_channels.md)", "(../macros_and_reserved_channels/index.html)")

				if "(./controlling.md)" in line:
					line = line.replace("(./controlling.md)", "(../controlling/index.html)")

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

