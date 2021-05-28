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

 

license = '''
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */
'''

newFileText = ""

for root, dirs, files in os.walk(currentDir):
    for f in files:
        filename = os.path.relpath(os.path.join(root, f), ".")
        newFileText = license + "\n"
        if "Widgets" not in filename and "Miscellaneous" not in filename and ".csd" in filename:
            # print(filename)
            with open(filename, "rt") as inputFile:
                newFileText += inputFile.read()

            with open(filename, "w") as f:
                    f.write(newFileText)

