set terminal png;set output "23915_MSE_29.png";
set title "23915_MSE_29";set xlabel "q";set ylabel "I(q) log-scale"
plot '23915_MSE_29.pdb.dat' u 1:2 thru log(y) t "theoretical" w lines lw 2 lt 10
