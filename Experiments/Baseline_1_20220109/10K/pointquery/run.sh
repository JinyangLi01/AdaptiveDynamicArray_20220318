#!/bin/sh
make
./pointquery pointquery1.csv pointquery1Log.txt
./pointquery pointquery2.csv pointquery2Log.txt
./pointquery pointquery3.csv pointquery3Log.txt
./pointquery pointquery4.csv pointquery4Log.txt
./pointquery pointquery5.csv pointquery5Log.txt
./average pointquery
python3 fig.py