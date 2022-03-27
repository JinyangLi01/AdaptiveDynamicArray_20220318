#!/bin/sh
make
./query query1.csv query1Log.txt
./query query2.csv query2Log.txt
./query query3.csv query3Log.txt
./query query4.csv query4Log.txt
./query query5.csv query5Log.txt
./average query
python3 fig.py