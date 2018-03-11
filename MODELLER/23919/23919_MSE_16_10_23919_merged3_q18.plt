set terminal png;set output "23919_MSE_16_10_23919_merged3_q18.png";
set title "23919_MSE_16_10 23919_merged3_q18";set xlabel "q";set ylabel "I(q) log-scale"
plot '23919_MSE_16_10_23919_merged3_q18.dat' u 1:2 thru log(y) t "experimental", '23919_MSE_16_10_23919_merged3_q18.dat' u 1:3 thru log(y) t "FoXS" w lines lw 2 lt 2
