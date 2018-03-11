set terminal png;set output "23919_MSE_16_10.png";
set title "23919_MSE_16_10";set xlabel "q";set ylabel "I(q) log-scale"
plot '23919_MSE_16_10.pdb.dat' u 1:2 thru log(y) t "theoretical" w lines lw 2 lt 2
