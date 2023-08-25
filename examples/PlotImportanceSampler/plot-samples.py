import json
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
import numpy as np
import math
import sys

"""
This python script is used to plot the samples writted to a json file by the main.cpp Radium example app in the PlotImportanceSampler folder.

To launch this script use the command
python3 plot-samples.py /path/to/jsonfile.json
"""

def plotDir(array, ax):
    for p in array:
        ax.scatter(p[0], p[1], p[2], c='#1f77b4', s=1)

with open(sys.argv[1], 'r') as f:
    data = json.load(f)

size = len(data)

fig = plt.figure(figsize=plt.figaspect(1/3))
gs = gridspec.GridSpec(math.ceil(size/3), 3)

i = 0
for l in data:
    ax = fig.add_subplot(gs[math.floor(i/3), i%3], projection='3d')
    ax.set_title(l)
    ax.set_zlim(0, 1)
    arr = np.array(data[l])
    plotDir(arr, ax)
    i += 1

plt.show()
