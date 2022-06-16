#!/bin/sh
make
./reorder/reorder reorder/reorder1.csv reorder/reorder1Log.txt
./reorder/reorder reorder/reorder2.csv reorder/reorder2Log.txt
./reorder/reorder reorder/reorder3.csv reorder/reorder3Log.txt
./reorder/reorder reorder/reorder4.csv reorder/reorder4Log.txt
./reorder/reorder reorder/reorder5.csv reorder/reorder5Log.txt
./reorder/average reorder
