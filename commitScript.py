import os
from shutil import copyfile
import sys
import datetime
import ntpath
import time
import subprocess

print( "Usage: python commitScript branch \"commit string\"");

if len(sys.argv) < 1:
	print "Not enough parameters passed"
	exit();

##define CABBAGE_VERSION "Cabbage(64bit) v2.0.00"
patch_number = 0
patch_string=''
newFiletext = ''
with open('./Source/CabbageCommonHeaders.h') as f: 
    for line in f.readlines():
        if 'Cabbage(64bit) v2.0.' in line:
            patch_number = int(line[line.rfind('.')+1:].strip().replace('"', ''))
            patch_number+=1
            if patch_number<10:
                patch_string = str(patch_number).zfill(2)
                print(patch_string)
                line = '#define CABBAGE_VERSION "Cabbage(64bit) v2.0.'+patch_string+'"'
        newFiletext+=line
    
with open('./Source/CabbageCommonHeaders.h', "w") as f:
    f.write(newFiletext)

gitCommand = 'git add JuceLibraryCode/AppConfig.h'
print gitCommand
process = subprocess.Popen(gitCommand, shell=True, stdout=subprocess.PIPE)
process.wait()

gitCommand = 'git add Source'
print gitCommand
process = subprocess.Popen(gitCommand, shell=True, stdout=subprocess.PIPE)
process.wait()

print gitCommand
process = subprocess.Popen(gitCommand, shell=True, stdout=subprocess.PIPE)
process.wait()


gitCommand = 'git commit -m \"'+sys.argv[2]+'\"'
print gitCommand
process = subprocess.Popen(gitCommand, shell=True, stdout=subprocess.PIPE)
process.wait()

gitCommand = 'git push origin '+sys.argv[1]
process = subprocess.Popen(gitCommand, shell=True, stdout=subprocess.PIPE)
process.wait()