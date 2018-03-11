set terminal png;set output "23916_MSE_223.png";
set title "23916_MSE_223";set xlabel "q";set ylabel "I(q) log-scale"
plot '23916_MSE_223.pdb.dat' u 1:2 thru log(y) t "theoretical" w lines lw 2 lt 19
