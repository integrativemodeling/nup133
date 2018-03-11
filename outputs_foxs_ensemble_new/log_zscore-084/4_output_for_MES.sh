#Z (P<0.25) = -0.67448975       #=NORMSINV(0.25) in excel
#Z (P<0.2) = -0.841621234       #=NORMSINV(0.2) in excel
#Z (P<0.15) = -1.036433389
#Z (P<0.1) = -1.281551566
#Z (P<0.05) = -1.644853627

z_cutoff=-0.841621234

awk '{print $3,$9}' 3_z_hit_score$z_cutoff.txt > filelist.txt

