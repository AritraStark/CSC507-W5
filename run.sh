#!/bin/bash
g++ -std=c++11 -pthread main_2.cpp -o out

# Case A: Fix N=1,000,000 vary M
echo "#M Time(s)" > res_1.dat
for M in 1 2 4; do
    ./out 1000000 $M | awk '{print $2, $3}' >> res_1.dat
done

# Case B: Fix M=4 vary N
echo "#N Time(s)" > res_2.dat
for N in 10 100 1000 10000 100000; do
    ./out $N 4 | awk '{print $1, $3}' >> res_2.dat
done

# Generate plots
gnuplot plot.gnu
