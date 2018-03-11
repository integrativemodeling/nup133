set terminal png;set output "23917.B99990081_MSE.png";
set title "23917.B99990081_MSE";set xlabel "q";set ylabel "I(q) log-scale"
plot '23917.B99990081_MSE.pdb.dat' u 1:2 thru log(y) t "theoretical" w lines lw 2 lt 81
