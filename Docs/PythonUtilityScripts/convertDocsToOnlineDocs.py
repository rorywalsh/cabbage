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
					
				if "../images/" in line:
					line = "(../docs/images/" +line[line.index("images/")+7:]

				if "```html" in line:
					line = line.replace("```html", "```csharp")

				if "(./identchannels.html)" in line:
					line = line.replace("(./identchannels.html)", "(../identchannels/index.html)")

				if "(./widget_arrays.md)" in line:
					line = line.replace("(./widget_arrays.md)", "(../widget_arrays/index.html)")

				if "(./presets.md)" in line:
					line = line.replace("(./presets.md)", "(../presets/index.html)")

				if "(./plants.md)" in line:
					line = line.replace("(./plants.md)", "(../plants/index.html)")

				if "(./introduction.html)" in line:
					line = line.replace("(./introduction.html)", "(../introduction/index.html)")

				if "(./controlling.md)" in line:
					line = line.replace("(./controlling.md)", "(../controlling/index.html)")

				if "(./sliders.md)" in line:
					line = line.replace("(./sliders.md)", "(../sliders/index.html)")

				if "{! " in line:
					line = line.replace("{! ./markdown/Widgets/Properties/", "{% include_relative ")

				if " !}" in line:
					line = line.replace(" !}", " %}")

				outputFile.write(line)

			inputFile.close()
			outputFile.close()

