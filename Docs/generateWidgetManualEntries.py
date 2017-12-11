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


for filename in docfiles:
    if ".md" in filename:
        #print(filename)
        links = []
        #outputFile = open("_"+filename, 'w+')
        #outputFile = open(inputFile.name, 'w')
        #lineText = ""
        os.chdir(currentDir)
        inputFile = open(filename)
        print("Widget Properties for "+filename)
        for line in inputFile:
            if "{! ./markdown/Widgets/Properties/" in line:
                line = line.replace("{! ./markdown/Widgets/Properties/", "")
                line = line[0:line.find(" !}")]
                #print(line)
                propFile = open("./Properties/"+line.rstrip())
                for propLine in propFile:
                    if "**" in propLine:
                        propLine = propLine[propLine.find("**")+2:-1]
                        params = propLine[propLine.find("("):propLine.find(")")+1]
                        propLine = propLine[0:propLine.find("(")]
                        linkText = "["+propLine+"](#" + propLine +")" + params +", "
                        #print (linkText)

                        links.append(linkText + "\n")


        
        inputFile = open(filename)
        htmlText = ""
        for line in inputFile:
            if "WIDGET_SYNTAX" in line:
                print("found in line")
                line = line.replace("WIDGET_SYNTAX", ''.join(links))
        
            htmlText = htmlText+line;

        inputFile = open("./GeneratedWidgetFiles/"+filename, "w+")
        inputFile.write(htmlText)
        inputFile.close();

#print htmlText
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

