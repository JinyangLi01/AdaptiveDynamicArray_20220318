#!/bin/sh
make
./reorder reorder1.csv reorder1Log.txt
./reorder reorder2.csv reorder2Log.txt
./reorder reorder3.csv reorder3Log.txt
./reorder reorder4.csv reorder4Log.txt
./reorder reorder5.csv reorder5Log.txt
./reorder reorder6.csv reorder6Log.txt
./reorder reorder7.csv reorder7Log.txt
./reorder reorder8.csv reorder8Log.txt
./reorder reorder9.csv reorder9Log.txt
./reorder reorder10.csv reorder10Log.txt
./reorder reorder11.csv reorder11Log.txt
./reorder reorder12.csv reorder12Log.txt
./reorder reorder13.csv reorder13Log.txt
./reorder reorder14.csv reorder14Log.txt
./reorder reorder15.csv reorder15Log.txt
./reorder reorder16.csv reorder16Log.txt
./reorder reorder17.csv reorder17Log.txt
./reorder reorder18.csv reorder18Log.txt
./reorder reorder19.csv reorder19Log.txt
./reorder reorder20.csv reorder20Log.txt
./average reorder
python3 fig.py