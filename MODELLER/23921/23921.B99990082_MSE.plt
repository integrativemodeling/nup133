set terminal png;set output "23921.B99990082_MSE.png";
set title "23921.B99990082_MSE";set xlabel "q";set ylabel "I(q) log-scale"
plot '23921.B99990082_MSE.pdb.dat' u 1:2 thru log(y) t "theoretical" w lines lw 2 lt 83
