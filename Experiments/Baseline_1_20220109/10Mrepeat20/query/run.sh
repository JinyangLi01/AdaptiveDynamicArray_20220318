#!/bin/sh
make
./query/query query/query6.csv query/query6Log.txt
./query/query query/query7.csv query/query7Log.txt
./query/query query/query8.csv query/query8Log.txt
./query/query query/query9.csv query/query9Log.txt
./query/query query/query10.csv query/query10Log.txt
./query/average query
