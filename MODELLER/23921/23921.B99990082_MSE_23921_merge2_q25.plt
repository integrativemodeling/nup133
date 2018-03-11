set terminal png;set output "23921.B99990082_MSE_23921_merge2_q25.png";
set title "23921.B99990082_MSE 23921_merge2_q25";set xlabel "q";set ylabel "I(q) log-scale"
plot '23921.B99990082_MSE_23921_merge2_q25.dat' u 1:2 thru log(y) t "experimental", '23921.B99990082_MSE_23921_merge2_q25.dat' u 1:3 thru log(y) t "FoXS" w lines lw 2 lt 83
