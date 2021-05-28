from os import listdir
from shutil import copyfile
from distutils.dir_util import copy_tree
import os.path
import os
import sys
from os.path import isfile, join, basename

print("Add legacy styling to Iain's examples")
print("Usage: python addLegacyStyling.py ./path_to_files")

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
        if "Widgets" not in filename and "Miscellaneous" not in filename and ".csd" in filename:
            # print(filename)
            with open(filename, "rt", encoding="utf-8") as inputFile:
                for line in inputFile:
                    if "form" in line and "caption" in line and "size" in line:
                        line = line.replace('\n', '') +" style(\"legacy\")\n"

                    newFileText = newFileText+line

            with open(filename, "w") as f:
                    f.write(newFileText)

