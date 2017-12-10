from os import listdir
from shutil import copyfile
from distutils.dir_util import copy_tree
import os.path
import os
import sys
from os.path import isfile, join, basename

print("Usage: python generateWidgetManualEntries.py ./path_to_widget_md_files")

if len(sys.argv) != 2:
	print("You must supply a directory")
	exit(1)

currentDir = os.path.abspath(sys.argv[1])
docfiles = listdir(currentDir)
os.chdir(currentDir)

links = []

for filename in docfiles:
    if ".md" in filename:
        #print(filename)
        inputFile = open(filename)
        #outputFile = open(inputFile.name, 'w')
        #lineText = ""
        
        if filename == "button.md":
            print("Widget Properties for "+filename)
            for line in inputFile:
                if "{! ./markdown/Widgets/Properties/" in line:
                    line = line.replace("{! ./markdown/Widgets/Properties/", "")
                    line = line[0:line.find(" !}")]
                    #print(line)
                    propFile = open("./Properties/"+line)
                    for propLine in propFile:
                        if "**" in propLine:
                            propLine = propLine[propLine.find("**")+2:-1]
                            propLine = propLine[0:propLine.find("**")]
                            print (propLine)
                            links.append(propLine)


    #         <big><pre>
    # [bounds](#bounds)(x, y, width, height) 
    # [channel](#channel)("chan") 
    # [text](#text)("offCaption","onCaption")
    # [bounds](#bounds)(x, y, width, height) 
    # [channel](#channel)("chan") 
    # [text](#text)("offCaption","onCaption") 
    # [bounds](#bounds)(x, y, width, height) 
    # [channel](#channel)("chan") 
    # [text](#text)("offCaption","onCaption")
    # </pre></big>

        #     outputFile.write(line)

        # inputFile.close()
        # outputFile.close()

