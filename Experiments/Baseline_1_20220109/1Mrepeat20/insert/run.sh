#!/bin/sh
make
./insert/insert insert/insert1.csv insert/insert1Log.txt
./insert/insert insert/insert2.csv insert/insert2Log.txt
./insert/insert insert/insert3.csv insert/insert3Log.txt
./insert/insert insert/insert4.csv insert/insert4Log.txt
./insert/insert insert/insert5.csv insert/insert5Log.txt
./insert/average insert

