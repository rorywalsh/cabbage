from os import listdir
from shutil import copyfile
from distutils.dir_util import copy_tree
import os.path
import os
import sys
from os.path import isfile, join, basename

print("Usage: python convertDocsToOnlineDocs.py ./path_to_md_files ../path_to_cabbage_site_app_dir")

if len(sys.argv) != 3:
	print("You must supply a directory")
	exit(1)

outputDir = os.path.abspath(sys.argv[2])+"/_docs"
imagesDir = os.path.abspath(sys.argv[2])+"/images/docs"
copy_tree("./images/", imagesDir)

directories = ["", "/Widgets", "/Widgets/Properties"]
currentDir = os.path.abspath(sys.argv[1])
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

				if "(images/" in line:
					line = "![]({{ site.url }}/images/docs/"+line[line.index("(images/")+8:]

				if "```html" in line:
					line = line.replace("```html", "```csharp")

				if "[Identifier channels](./identchannels.html)" in line:
					line = line.replace("[Identifier channels](./identchannels.html)", "[Identifier channels](../identchannels/index.html)")

				if "[Widget arrays](./widget_arrays.md)" in line:
					line = line.replace("[Widget arrays](./widget_arrays.md)", "[Widget arrays](../widget_arrays/index.html)")

				if "[Presets](./presets.md)" in line:
					line = line.replace("[Presets](./presets.md)", "[Presets](../presets/index.html)")

				if "[Plants](./plants.md)" in line:
					line = line.replace("[Plants](./plants.md)", "[Plants](../plants/index.html)")

				if "[Cabbage](./introduction.html)" in line:
					line = line.replace("[Cabbage](./introduction.html)", "[Introduction](../introduction/index.html)")

				if "[Controlling widgets](./controlling.md)" in line:
					line = line.replace("[Controlling widgets](./controlling.md)", "[Controlling widgets](../controlling/index.html)")

				if "{! " in line:
					line = line.replace("{! ./markdown/Widgets/Properties/", "{% include_relative ")

				if " !}" in line:
					line = line.replace(" !}", " %}")

				outputFile.write(line)

			inputFile.close()
			outputFile.close()

