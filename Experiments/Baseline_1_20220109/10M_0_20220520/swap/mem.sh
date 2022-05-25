#!/bin/bash
while true; do {
    pgrep -x swap | while read PID; do {
        echo -1000 > /proc/$PID/oom_score_adj; 
    } done;
} done;