from os import listdir
from shutil import copyfile
from distutils.dir_util import copy_tree
import os.path
import os
import sys
from os.path import isfile, join, basename

print("Usage: python removeSampleRateAssignments.py ./path_to_files")

if len(sys.argv) != 2:
	print("You must supply a directory")
	exit(1)

currentDir = os.path.abspath(sys.argv[1])
exampleFiles = listdir(currentDir)
os.chdir(currentDir)
print(currentDir)

newFileText = ""

for root, dirs, files in os.walk(currentDir):
    for f in files:
        filename = os.path.relpath(os.path.join(root, f), ".")
        newFileText = ""
        if ".csd" in filename:
            # print(filename)
            with open(filename, "rt", encoding="utf-8") as inputFile:
                for line in inputFile:
                    if line.find("sr") > -1:
                        if line[0:line.find("=")].replace(' ', '').replace('\t', '') == "sr":
                            line = ";sr is set by the host\n"
                    # if "form" in line and "caption" in line and "size" in line:
                    #     line = line.replace('\n', '') +" style(\"legacy\")\n"

                    newFileText = newFileText+line

            with open(filename, "w") as f:
                     f.write(newFileText)

