set terminal png;set output "23920.B99990067_MSE.png";
set title "23920.B99990067_MSE";set xlabel "q";set ylabel "I(q) log-scale"
plot '23920.B99990067_MSE.pdb.dat' u 1:2 thru log(y) t "theoretical" w lines lw 2 lt 68
