#!/bin/bash

gcc -Wall -std=c99 -pg integration.c -lm -o error
./error assignment_2.txt 10
gprof error gmon.out > profile.txt