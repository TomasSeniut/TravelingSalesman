#!/usr/bin/python3

import numpy as np
import argparse
import os

parser = argparse.ArgumentParser(description='Generate test data for Traveling Salesman problem.')
parser.add_argument('N', type=int, help='Number of items')
parser.add_argument('-o', type=str, help='output file name')

args = parser.parse_args()

points = np.random.randint(0, 1000, size=(2, args.N))
indexes = np.arange(0, args.N)

data = np.column_stack((indexes, points[0, :], points[1, :]))

out = f'tsp_{args.N}.out'
if args.o:
    out = args.o

if os.path.isdir('tsp_data'):
    out = os.path.join('tsp_data', out)

np.savetxt(out, data, fmt='%4d', header=f'Cities: {args.N}\nIndex, X, Y')
