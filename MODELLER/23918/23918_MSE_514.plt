set terminal png;set output "23918_MSE_514.png";
set title "23918_MSE_514";set xlabel "q";set ylabel "I(q) log-scale"
plot '23918_MSE_514.pdb.dat' u 1:2 thru log(y) t "theoretical" w lines lw 2 lt 6
