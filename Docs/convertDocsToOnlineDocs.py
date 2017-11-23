from os import listdir
import os.path
import os
import sys
from os.path import isfile, join, basename

if len(sys.argv) != 3:
	print("You must supply a directory")
	exit(1)

outputDir = os.path.abspath(sys.argv[2])

docfiles = listdir(os.path.abspath(sys.argv[1]))
os.chdir(os.path.abspath(sys.argv[1]))

for filename in docfiles:
	if ".md" in filename:
		inputFile = open(filename)
		outputFile = open(outputDir+"/"+inputFile.name, 'w')
		lineText = ""

		# add YAML
		if "Properties" not in sys.argv[1]:
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

			if "[Controlling widgets](./controlling.md)" in line:
				line = line.replace("[Controlling widgets](./controlling.md)", "[Controlling widgets](../controlling/index.html)")

			if "{! " in line:
				line = line.replace("{! ./markdown/Widgets/Properties/", "{% include_relative ")

			if " !}" in line:
				line = line.replace(" !}", " %}")

			outputFile.write(line)

		inputFile.close()
		outputFile.close()
