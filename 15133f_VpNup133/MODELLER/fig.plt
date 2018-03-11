set terminal postscript eps size 3.5,2.62 color enhanced solid linewidth 2.5 font 'Helvetica,22'
set output 'mes2_final.eps'
set lmargin 2
set rmargin 2
set multiplot
f(x)=1
set xrange [:0.33]

############################################
# Residual Plot
############################################
set origin 0,0
set size 1,0.3
set tmargin 0
set bmargin 3
set ylabel ''
set format y ''
set xtics nomirror font 'Helvetica,18'
set ytics nomirror font 'Helvetica,18'
set border 3
set style line 11 lc rgb '#808080' lt 1
set border 3 back ls 11

#plot f(x) notitle lc rgb '#333333', 'Nup192N_chainA_final_Nup192_q31.dat' u 1:($2/$3) notitle w lines lw 2.5 lc rgb '#66ff00', 'Nup192N_chainA_final_Nup192_q31.dat' u 1:($2/$3) notitle w lines lw 2.5 lc rgb '#e26261'

#plot f(x) notitle lc rgb '#333333', '../../MODELLER/Nup192N_chainA_final_Nup192_q31.dat' u 1:($3/$2) notitle w lines lw 2.5 lc rgb '#e26261', '../../MODELLER/N24046M.B99990889_Nup192_q31.dat' u 1:($3/$2) notitle w lines lw 2.5 lc rgb '#66ff00', 'mes2.dat' u 1:($3/$2) notitle w lines lw 2.5 lc rgb '#0000ff'

plot f(x) notitle lc rgb '#333333', 'avg_optimal' u 1:($3/$2) notitle w lines lw 2.5 lc rgb '#0000ff'

############################################
# Main Plot
############################################
set origin 0,0.3
set size 1,0.69
set bmargin 0
set tmargin 1
set xlabel ''
set format x ''
set ylabel ''
#set xlabel 'q (A^{-1})' font "Tahoma, 30"
#set ylabel 'I(q) log-scale (a.u.)' font "Tahoma, 30"
set logscale y

#plot 'Nup192N_chainA_final_Nup192_q31.dat' u 1:2 thru log(y) notitle lc rgb '#333333' pt 6 ps 0.8, 'Nup192N_chainA_final_Nup192_q31.dat' u 1:3 thru log(y) t 'CRYSOL {/Symbol c} = 8.0' w lines lw 2.5 lc rgb '#66ff00', 'Nup192N_chainA_final_Nup192_q31.dat' u 1:3 thru log(y) t 'FoXS {/Symbol c} = 4.7' w lines lw 2.5 lc rgb '#e26261'

#plot '../../MODELLER/Nup192_q31.dat' u 1:2:3 with yerrorbars notitle lc rgb '#333333' pt 6 ps 0.8,'../../MODELLER/Nup192N_chainA_final_Nup192_q31.dat' u 1:3 t 'crystal structure {/Symbol c} = 4.027' w lines lw 2.5 lc rgb '#e26261', '../../MODELLER/N24046M.B99990889_Nup192_q31.dat' u 1:3 t 'MODELLER {/Symbol c} = 3.843' w lines lw 2.5 lc rgb '#66ff00', 'mes2.dat' u 1:3 t '2 MES models {/Symbol c} = 1.446' w lines lw 2.5 lc rgb '#0000ff'

plot '23904_merge2_q25.dat' u 1:2:3 with yerrorbars notitle lc rgb '#333333' pt 6 ps 0.8, 'avg_optimal' u 1:3 t '2 MES models {/Symbol c} = 1.446' w lines lw 2.5 lc rgb '#0000ff'

unset multiplot
