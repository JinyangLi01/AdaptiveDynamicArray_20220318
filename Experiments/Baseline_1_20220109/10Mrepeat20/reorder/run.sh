#!/bin/sh
make
./reorder/reorder reorder/reorder16.csv reorder/reorder16Log.txt
./reorder/reorder reorder/reorder17.csv reorder/reorder17Log.txt
./reorder/reorder reorder/reorder18.csv reorder/reorder18Log.txt
./reorder/reorder reorder/reorder19.csv reorder/reorder19Log.txt
./reorder/reorder reorder/reorder20.csv reorder/reorder20Log.txt
./reorder/average reorder
