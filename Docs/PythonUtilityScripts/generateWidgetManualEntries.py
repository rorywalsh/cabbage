from os import listdir
from shutil import copyfile
from distutils.dir_util import copy_tree
import os.path
import os
import sys
from os.path import isfile, join, basename


currentDir = os.path.abspath("../markdown/Widgets/")
docfiles = listdir(currentDir)
os.chdir(currentDir)

if os.path.exists('./ExpandedWidgetEntries') == False:
	os.mkdir("ExpandedWidgetEntries")

for filename in docfiles:
    if ".md" in filename:
        links = []
        identifierCode = []
        #search for properties and add anchor links to top of widget entry 
        inputFile = open(filename)
        print("Widget Properties for "+filename)
        for line in inputFile:
            if "{! ./markdown/Widgets/Properties/" in line:
                line = line.replace("{! ./markdown/Widgets/Properties/", "")
                line = line[0:line.find(" !}")]
                propFile = open("./Properties/"+line.rstrip())
                content = propFile.readlines()
                for propLine in content:
                    if "**" in propLine:
                        propLine = propLine[propLine.find("**")+2:-1]
                        params = propLine[propLine.find("("):propLine.find(")")+1]
                        propLine = propLine[0:propLine.find("(")]
                        linkText = "["+propLine+"](#" + line[0:line.find(".")] +")" + params +", "
                        links.append(linkText + "\n")
                    
                    #if we have an example, extract code and add to identifierCode list
                    if "<!--UPDATE WIDGET_IN_CSOUND" in propLine:
                        if "keyboard" in filename and ("bounds" in linkText or "visible" in linkText):
                            print("excluding" + filename + linkText)
                        else:
                            lineIndex = content.index(propLine)+1;           
                            while lineIndex < len(content)-1:
                                if "-->" in content[lineIndex]:
                                    lineIndex == len(content)+1
                                lineText = "\t"+content[lineIndex].lstrip(' ')
                                identifierCode.append(lineText)
                                lineIndex+=1

        #expand widget entry with generated Csound code        
        inputFile = open(filename)
        htmlText = ""
        for line in inputFile:
            if "WIDGET_SYNTAX" in line:
                line = line.replace("WIDGET_SYNTAX", ''.join(links))
        
            htmlText = htmlText+line;

            if ";WIDGET_ADVANCED_USAGE" in line:
                newLines = '''
instr 2
    if metro(1) == 1 then
        event "i", "ChangeAttributes", 0, 1
    endif
endin

instr ChangeAttributes
    SIdentifier init ""\n'''
                newLines = newLines.lstrip(' ')
                newLines +=''.join(identifierCode).lstrip(' ')      

                lastBit ='''    ;send identifier string to Cabbage
    chnset SIdentifier, "widgetIdent"           
endin
                '''
                newLines+=lastBit

                line = line.replace(";WIDGET_ADVANCED_USAGE", newLines)
                htmlText = htmlText+line;

        inputFile = open("./ExpandedWidgetEntries/"+filename, "w+")
        inputFile.write(htmlText)
        inputFile.close();

        #generate help .csd from expanded widget entry        
        inputFile = open("./ExpandedWidgetEntries/"+filename)
        outputFile = open("../../../Examples/Widgets/"+filename.lower().replace('.md', '.csd'), "w+")
        foundExampleCode = False
        for line in inputFile:
            if "<!--(Widget Example)/-->" in line:
                foundExampleCode = True
            if "<!--(End Widget Example)/-->" in line:
                foundExampleCode = False
            if foundExampleCode is True and "<!--(Widget Example)/-->" not in line and "```" not in line:
                outputFile.write(line)
        
        inputFile.close()
        outputFile.close()


