#!/usr/bin/env python3

import sys
import os

def listdir(d):
    return [(os.path.join(d, f), f) for f in os.listdir(d)]

def imageClass(string):
    return string.split('-')[0]

if __name__ == '__main__':
    if len(sys.argv) != 2:
        sys.exit('Syntax: %s <directory>' % sys.argv[0])
    directory = sys.argv[1]

    for fileAndDirectory, filename in listdir(directory):
        newdir = os.path.join(directory, imageClass(filename))
        if not os.path.exists(newdir):
            os.makedirs(newdir)
        print(fileAndDirectory, '→', os.path.join(newdir, filename))
        os.rename(fileAndDirectory, os.path.join(newdir, filename))
