from xml.dom import minidom
import xml.etree.ElementTree as ET
import sys
import os

def increment_ver(version):
    version = version.split('.')
    version[2] = str(int(version[2]) + 1)
    return '.'.join(version)

newVersionNum = ""

if len(sys.argv) < 2:
    print("Please provide a commit message enclosed in quotation marks")
    exit()

if len(sys.argv) == 3:
    newVersionNum = sys.argv[2]

#bump jucer version numbers
newFileText = ""
with open("CMakeLists.txt", "rt") as inputFile:
    for line in inputFile:
        if "set" in line and "BUILD_VERSION" in line:
            if len(newVersionNum) > 0:
                line = "\tset(BUILD_VERSION "+newVersionNum+ ")\n"
            else:
                number = line.replace("set", "").replace("(", "").replace(")", ""). replace("BUILD_VERSION", "")
                newVersionNum = increment_ver(number.strip());
                
                line = "\tset(BUILD_VERSION "+ newVersionNum+")\n"
                

        newFileText = newFileText+line

with open("CMakeLists.txt", "w") as f:
    f.write(newFileText)

os.system('git add Source')
os.system('git add Examples')
os.system('git add Docs')
os.system('git add CMakeLists.txt')
os.system('git commit -m "'+sys.argv[1]+' - Version number:'+newVersionNum+'"')
