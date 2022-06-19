#!/bin/sh
make
./reorder/reorder reorder/reorder11.csv reorder/reorder11Log.txt
./reorder/reorder reorder/reorder12.csv reorder/reorder12Log.txt
./reorder/reorder reorder/reorder13.csv reorder/reorder13Log.txt
./reorder/reorder reorder/reorder14.csv reorder/reorder14Log.txt
./reorder/reorder reorder/reorder15.csv reorder/reorder15Log.txt
./reorder/average reorder
