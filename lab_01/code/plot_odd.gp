set terminal pdfcairo enhanced size 8,5 font "Arial,14"
set output 'benchmarks_odd.pdf'

set title "Зависимость времени умножения матриц от размера для нечётных размеров"
set xlabel "Размер матрицы N (N × N)"
set ylabel "Время, мс"

set xrange [100:401]

set key top left
set grid

plot \
    'data_odd.txt' using 1:2 with linespoints lt 1 lw 2 pt 7 ps 1.0 lc rgb "red"   title "Стандартный", \
    'data_odd.txt' using 1:3 with linespoints lt 2 lw 2 pt 9 ps 1.0 lc rgb "blue"  title "Виноград", \
    'data_odd.txt' using 1:4 with linespoints lt 3 lw 2 pt 11 ps 1.0 lc rgb "green" title "Виноград опт."
