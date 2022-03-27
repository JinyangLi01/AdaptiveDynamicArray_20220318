#!/bin/sh
make
./swap swap1.csv swap1Log.txt
./swap swap2.csv swap2Log.txt
./swap swap3.csv swap3Log.txt
./swap swap4.csv swap4Log.txt
./swap swap5.csv swap5Log.txt
./average swap
python3 fig.py
