#!/bin/sh
make
./reorder/reorder reorder/reorder6.csv reorder/reorder6Log.txt
./reorder/reorder reorder/reorder7.csv reorder/reorder7Log.txt
./reorder/reorder reorder/reorder8.csv reorder/reorder8Log.txt
./reorder/reorder reorder/reorder9.csv reorder/reorder9Log.txt
./reorder/reorder reorder/reorder10.csv reorder/reorder10Log.txt
./reorder/average reorder
