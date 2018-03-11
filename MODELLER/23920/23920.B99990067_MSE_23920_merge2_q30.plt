set terminal png;set output "23920.B99990067_MSE_23920_merge2_q30.png";
set title "23920.B99990067_MSE 23920_merge2_q30";set xlabel "q";set ylabel "I(q) log-scale"
plot '23920.B99990067_MSE_23920_merge2_q30.dat' u 1:2 thru log(y) t "experimental", '23920.B99990067_MSE_23920_merge2_q30.dat' u 1:3 thru log(y) t "FoXS" w lines lw 2 lt 68
