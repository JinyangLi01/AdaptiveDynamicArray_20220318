#!/bin/sh
make
./pointquery/pointquery pointquery1.csv pointquery1Log.txt
./pointquery/pointquery pointquery2.csv pointquery2Log.txt
./pointquery/pointquery pointquery3.csv pointquery3Log.txt
./pointquery/pointquery pointquery4.csv pointquery4Log.txt
./pointquery/pointquery pointquery5.csv pointquery5Log.txt
./pointquery/pointquery pointquery6.csv pointquery6Log.txt
./pointquery/pointquery pointquery7.csv pointquery7Log.txt
./pointquery/pointquery pointquery8.csv pointquery8Log.txt
./pointquery/pointquery pointquery9.csv pointquery9Log.txt
./pointquery/pointquery pointquery10.csv pointquery10Log.txt
./pointquery/pointquery pointquery11.csv pointquery11Log.txt
./pointquery/pointquery pointquery12.csv pointquery12Log.txt
./pointquery/pointquery pointquery13.csv pointquery13Log.txt
./pointquery/pointquery pointquery14.csv pointquery14Log.txt
./pointquery/pointquery pointquery15.csv pointquery15Log.txt
./pointquery/pointquery pointquery16.csv pointquery16Log.txt
./pointquery/pointquery pointquery17.csv pointquery17Log.txt
./pointquery/pointquery pointquery18.csv pointquery18Log.txt
./pointquery/pointquery pointquery19.csv pointquery19Log.txt
./pointquery/pointquery pointquery20.csv pointquery20Log.txt
./pointquery/average pointquery
python3 fig.py