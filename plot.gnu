# ---------- Case A ----------
set terminal pngcairo size 800,600
set output "Plot1.png"
set title "N = 10,00,000 vs Varying M"
set xlabel "Threads (M)"
set ylabel "Time (s)"
set grid
plot "res_1.dat" using 1:2 with linespoints lw 2 title "Execution Time"

# ---------- Case B ----------
set output "Plot2.png"
set title "M=4 vs Varying N"
set xlabel "Input Size (N)"
set ylabel "Time (s)"
set logscale x 10
set grid
plot "res_2.dat" using 1:2 with linespoints lw 2 title "Execution Time"
