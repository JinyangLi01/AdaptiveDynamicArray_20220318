#!/bin/sh
make
./query/query query/query11.csv query/query11Log.txt
./query/query query/query12.csv query/query12Log.txt
./query/query query/query13.csv query/query13Log.txt
./query/query query/query14.csv query/query14Log.txt
./query/query query/query15.csv query/query15Log.txt
./query/average query
