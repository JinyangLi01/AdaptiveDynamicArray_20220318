#!/bin/sh
while true; do {
    pgrep -x delete | while read PID; do {
        sudo echo -1000 > /proc/$PID/oom_score_adj; 
    } done;
} done;
