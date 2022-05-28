#!/bin/sh
make
./reorder reorder1.csv reorder1Log.txt
./reorder reorder2.csv reorder2Log.txt
./reorder reorder3.csv reorder3Log.txt
./reorder reorder4.csv reorder4Log.txt
./reorder reorder5.csv reorder5Log.txt
./average reorder
python3 fig.py