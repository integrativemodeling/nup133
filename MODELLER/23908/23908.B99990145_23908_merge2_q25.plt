set terminal png enhanced; set output "23908.B99990145_23908_merge2_q25.png";
set lmargin 2; set rmargin 2;set multiplot
set origin 0,0;set size 1,0.3; set tmargin 0; set bmargin 3;set ylabel '';set format y '';set xtics nomirror;set ytics nomirror; set border 3
set style line 11 lc rgb '#808080' lt 1;set border 3 back ls 11;f(x)=1
plot f(x) notitle lc rgb '#333333', '23908.B99990145_23908_merge2_q25.dat' u 1:($2/$3) notitle w lines lw 2.5 lc rgb '#e26261'
set origin 0,0.3;set size 1,0.69; set bmargin 0; set tmargin 1;set xlabel ''; set format x ''; set ylabel '';
plot '23908.B99990145_23908_merge2_q25.dat' u 1:2 thru log(y) notitle lc rgb '#333333' pt 6 ps 0.8, '23908.B99990145_23908_merge2_q25.dat' u 1:3 thru log(y) t 'FoXS chi = 1.11' w lines lw 2.5 lc rgb '#e26261'
unset multiplot
reset
