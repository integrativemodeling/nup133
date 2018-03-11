set terminal png;set output "3kfoH.B99990003_MSE_23922_merge.png";
set title "3kfoH.B99990003_MSE 23922_merge";set xlabel "q";set ylabel "I(q) log-scale"
plot '3kfoH.B99990003_MSE_23922_merge.dat' u 1:2 thru log(y) t "experimental", '3kfoH.B99990003_MSE_23922_merge.dat' u 1:3 thru log(y) t "FoXS" w lines lw 2 lt 8
