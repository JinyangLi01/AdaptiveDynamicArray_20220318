#!/bin/sh
make
./query/query query/query16.csv query/query16Log.txt
./query/query query/query17.csv query/query17Log.txt
./query/query query/query18.csv query/query18Log.txt
./query/query query/query19.csv query/query19Log.txt
./query/query query/query20.csv query/query20Log.txt
./query/average query
