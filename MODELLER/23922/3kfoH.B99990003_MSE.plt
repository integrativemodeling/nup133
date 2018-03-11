set terminal png;set output "3kfoH.B99990003_MSE.png";
set title "3kfoH.B99990003_MSE";set xlabel "q";set ylabel "I(q) log-scale"
plot '3kfoH.B99990003_MSE.pdb.dat' u 1:2 thru log(y) t "theoretical" w lines lw 2 lt 8
