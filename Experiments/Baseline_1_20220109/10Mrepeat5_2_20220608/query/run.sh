#!/bin/sh
make
./query/query query4.csv query4Log.txt
./query/query query5.csv query5Log.txt
./query/average query
python3 ./query/fig.py