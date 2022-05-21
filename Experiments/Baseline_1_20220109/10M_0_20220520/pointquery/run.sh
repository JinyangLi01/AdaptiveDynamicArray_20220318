#!/bin/sh
make
# ./pointquery avg.csv pointqueryLog.txt
./pointquery/pointquery pointquery1.csv pointquery1Log.txt
#./pointquery/pointquery pointquery2.csv pointquery2Log.txt
#./pointquery/pointquery pointquery3.csv pointquery3Log.txt
#./pointquery/pointquery pointquery4.csv pointquery4Log.txt
#./pointquery/pointquery pointquery5.csv pointquery5Log.txt
./pointquery/average pointquery
python3 ./pointquery/fig.py