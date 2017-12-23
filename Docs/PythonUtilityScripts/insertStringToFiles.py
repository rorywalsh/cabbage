from os import listdir
from shutil import copyfile
from distutils.dir_util import copy_tree
import os.path
import os
import sys
from os.path import isfile, join, basename

def insert(originalfile,string):
    with open(originalfile,'r') as f:
        with open('newfile.txt','w') as f2: 
            f2.write(string)
            f2.write(f.read())
    os.rename('newfile.txt',originalfile)

print("Usage: python insertStringToFiles.py ./path_to_widget_md_files")

if len(sys.argv) != 2:
	print("You must supply a directory")
	exit(1)

currentDir = os.path.abspath(sys.argv[1])
docfiles = listdir(currentDir)
os.chdir(currentDir)

for filename in docfiles:
    if ".md" in filename:
        #insert(filename, "inserted string")
        insert(filename, "<a name=\"" + filename[0:filename.find(".")] + "\"><h3 style=\"padding-top: 40px; margin-top: 40px;\"></h3></a>\n")

