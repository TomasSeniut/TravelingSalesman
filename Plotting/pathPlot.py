#!/usr/bin/python3

from solution import *
import numpy as np
import matplotlib.pyplot as plt

fileName = dataFileName.split('/')[-1]
solution = np.array(solution)

_, x, y = np.loadtxt(dataFileName, unpack=True)

y_ordered = np.append(y[solution], y[solution[0]])
x_ordered = np.append(x[solution], x[solution[0]])

plt.scatter(x, y, c='k', alpha=0.8, lw=0)
plt.plot(x_ordered, y_ordered, c='b')

for solution_order, point_order, x_, y_ in zip(range(len(y)), solution, x_ordered, y_ordered):
    plt.annotate(solution_order, xy=(x_, y_), horizontalalignment='left', verticalalignment='bottom')
    plt.annotate(point_order, xy=(x_, y_), horizontalalignment='right', verticalalignment='top')

plt.title(fileName)

plt.savefig('PlotResults/' + fileName + '.png')
plt.show()
