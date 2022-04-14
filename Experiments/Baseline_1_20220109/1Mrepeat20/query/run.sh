#!/bin/sh
make
./query query1.csv query1Log.txt
./query query2.csv query2Log.txt
./query query3.csv query3Log.txt
./query query4.csv query4Log.txt
./query query5.csv query5Log.txt
./query query6.csv query6Log.txt
./query query7.csv query7Log.txt
./query query8.csv query8Log.txt
./query query9.csv query9Log.txt
./query query10.csv query10Log.txt
./query query11.csv query11Log.txt
./query query12.csv query12Log.txt
./query query13.csv query13Log.txt
./query query14.csv query14Log.txt
./query query15.csv query15Log.txt
./query query16.csv query16Log.txt
./query query17.csv query17Log.txt
./query query18.csv query18Log.txt
./query query19.csv query19Log.txt
./query query20.csv query20Log.txt
./average query
python3 fig.py