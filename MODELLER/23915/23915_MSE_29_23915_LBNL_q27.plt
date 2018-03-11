set terminal png;set output "23915_MSE_29_23915_LBNL_q27.png";
set title "23915_MSE_29 23915_LBNL_q27";set xlabel "q";set ylabel "I(q) log-scale"
plot '23915_MSE_29_23915_LBNL_q27.dat' u 1:2 thru log(y) t "experimental", '23915_MSE_29_23915_LBNL_q27.dat' u 1:3 thru log(y) t "FoXS" w lines lw 2 lt 10
