#!/bin/sh
make
./pointquery/pointquery pointquery/pointquery1.csv pointquery/pointquery1Log.txt
./pointquery/pointquery pointquery/pointquery2.csv pointquery/pointquery2Log.txt
./pointquery/pointquery pointquery/pointquery3.csv pointquery/pointquery3Log.txt
./pointquery/pointquery pointquery/pointquery4.csv pointquery/pointquery4Log.txt
./pointquery/pointquery pointquery/pointquery5.csv pointquery/pointquery5Log.txt
./pointquery/average pointquery
