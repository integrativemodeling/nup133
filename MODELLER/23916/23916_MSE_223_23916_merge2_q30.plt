set terminal png;set output "23916_MSE_223_23916_merge2_q30.png";
set title "23916_MSE_223 23916_merge2_q30";set xlabel "q";set ylabel "I(q) log-scale"
plot '23916_MSE_223_23916_merge2_q30.dat' u 1:2 thru log(y) t "experimental", '23916_MSE_223_23916_merge2_q30.dat' u 1:3 thru log(y) t "FoXS" w lines lw 2 lt 19
