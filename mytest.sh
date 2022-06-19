#!/bin/bash

echo "OS Final Project Team 4 Test"

gcc RRschedule.c -o schedular

./schedular
python3 gantt_chart.py
eog result_gantt.png
~                         
