import subprocess
from os import listdir
from shutil import copyfile
from distutils.dir_util import copy_tree
import os.path
import time
import os
import sys
from os.path import isfile, join, basename
import signal
import subprocess

if os.path.exists('../markdown/Widgets/ExpandedWidgetEntries/gifs') == False:
	os.mkdir("../markdown/Widgets/ExpandedWidgetEntries/gifs")

#byzanz-record --width=400 --height=300 --duration=6 --delay=1  out.gif
for file in os.listdir(os.path.abspath("../markdown/Widgets/ExpandedWidgetEntries")):
    if file.endswith(".csd") and "listbox" not in file and "form" not in file and "sequencer" not in file:
        cabbageLiteCommand = "../../Builds/LinuxMakefile/build/CabbageLite "+os.path.abspath("../markdown/Widgets/ExpandedWidgetEntries/"+file)
        print (cabbageLiteCommand)
        pro1 = subprocess.Popen(cabbageLiteCommand, stdout=subprocess.PIPE, shell=True, preexec_fn=os.setsid)
        
        byzanzCommand = "byzanz-record --width=400 --x=1 --y=0 --height=300 --duration=6 --delay=3 "+ os.path.splitext(os.path.abspath("../markdown/Widgets/ExpandedWidgetEntries/gifs/"+file))[0]+'.gif'
        print(byzanzCommand)
        pro2 = subprocess.Popen(byzanzCommand, stdout=subprocess.PIPE, shell=True, preexec_fn=os.setsid)
        
        time.sleep(12)
        os.killpg(os.getpgid(pro1.pid), signal.SIGTERM) 
        #process = subprocess.Popen("../../Builds/LinuxMakefile/build/CabbageLite ", shell=True, stdout=subprocess.PIPE)
        #process.wait()
        #print process.returncode