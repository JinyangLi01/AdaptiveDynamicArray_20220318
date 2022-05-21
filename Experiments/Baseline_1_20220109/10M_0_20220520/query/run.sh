#!/bin/sh
make
./query/query query1.csv query1Log.txt
#./query/query query2.csv query2Log.txt
#./query/query query3.csv query3Log.txt
#./query/query query4.csv query4Log.txt
#./query/query query5.csv query5Log.txt
./query/average query
python3 ./query/fig.py