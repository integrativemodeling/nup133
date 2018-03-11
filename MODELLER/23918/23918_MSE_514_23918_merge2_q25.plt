set terminal png;set output "23918_MSE_514_23918_merge2_q25.png";
set title "23918_MSE_514 23918_merge2_q25";set xlabel "q";set ylabel "I(q) log-scale"
plot '23918_MSE_514_23918_merge2_q25.dat' u 1:2 thru log(y) t "experimental", '23918_MSE_514_23918_merge2_q25.dat' u 1:3 thru log(y) t "FoXS" w lines lw 2 lt 6
