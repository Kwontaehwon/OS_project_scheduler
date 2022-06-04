#!/bin/bash

echo "OS Final Project Team 4 Test"

python3 init_input.py
gcc RRschedule.c -o schedular

./schedular
python3 gantt_chart.py
eog result_gantt.png
