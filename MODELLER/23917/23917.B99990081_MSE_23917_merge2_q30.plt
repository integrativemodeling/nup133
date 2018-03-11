set terminal png;set output "23917.B99990081_MSE_23917_merge2_q30.png";
set title "23917.B99990081_MSE 23917_merge2_q30";set xlabel "q";set ylabel "I(q) log-scale"
plot '23917.B99990081_MSE_23917_merge2_q30.dat' u 1:2 thru log(y) t "experimental", '23917.B99990081_MSE_23917_merge2_q30.dat' u 1:3 thru log(y) t "FoXS" w lines lw 2 lt 81
