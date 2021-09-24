import sys
import os

if len(sys.argv) != 2:
    print("Please pass a JUCER file to update")

jucerFile = sys.argv[1]
projucer = "~/sourcecode/JUCE/extras/Projucer/Builds/MacOSX/build/Debug/Projucer.app/Contents/MacOS/Projucer"

os.system(projucer + ' --resave ' + jucerFile);
os.system('git apply ./patches/*.patch');	