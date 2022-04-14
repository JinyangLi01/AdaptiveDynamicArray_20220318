#!/bin/sh
make
./swap swap1.csv swap1Log.txt
./swap swap2.csv swap2Log.txt
./swap swap3.csv swap3Log.txt
./swap swap4.csv swap4Log.txt
./swap swap5.csv swap5Log.txt
./swap swap6.csv swap6Log.txt
./swap swap7.csv swap7Log.txt
./swap swap8.csv swap8Log.txt
./swap swap9.csv swap9Log.txt
./swap swap10.csv swap10Log.txt
./swap swap11.csv swap11Log.txt
./swap swap12.csv swap12Log.txt
./swap swap13.csv swap13Log.txt
./swap swap14.csv swap14Log.txt
./swap swap15.csv swap15Log.txt
./swap swap16.csv swap16Log.txt
./swap swap17.csv swap17Log.txt
./swap swap18.csv swap18Log.txt
./swap swap19.csv swap19Log.txt
./swap swap20.csv swap20Log.txt
./average swap
python3 fig.py