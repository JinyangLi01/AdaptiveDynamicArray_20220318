#!/bin/sh
make
./insert/insert insert1.csv insert1Log.txt
#./insert/insert insert2.csv insert2Log.txt
#./insert/insert insert3.csv insert3Log.txt
#./insert/insert insert4.csv insert4Log.txt
#./insert/insert insert5.csv insert5Log.txt
./insert/average insert
python3 ./insert/fig.py