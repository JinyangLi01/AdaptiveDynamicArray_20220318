#!/bin/sh
make
./swap/swap swap/swap16.csv swap/swap16Log.txt
./swap/swap swap/swap17.csv swap/swap17Log.txt
./swap/swap swap/swap18.csv swap/swap18Log.txt
./swap/swap swap/swap19.csv swap/swap19Log.txt
./swap/swap swap/swap20.csv swap/swap20Log.txt
./swap/average swap
