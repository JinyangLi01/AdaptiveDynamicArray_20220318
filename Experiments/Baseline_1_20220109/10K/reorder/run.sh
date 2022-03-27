#!/bin/sh
make
./reorder avg.csv reorderLog.txt
python3 fig.py