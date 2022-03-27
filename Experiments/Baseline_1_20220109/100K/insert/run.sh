#!/bin/sh
make
./insert insert1.csv insert1Log.txt
./insert insert2.csv insert2Log.txt
./insert insert3.csv insert3Log.txt
./insert insert4.csv insert4Log.txt
./insert insert5.csv insert5Log.txt
./average insert
python3 fig.py