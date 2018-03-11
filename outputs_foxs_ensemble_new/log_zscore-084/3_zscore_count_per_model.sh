m_start=1
m_end=7071
#m_start=1170
#m_end=1180
FILE="class0.log"
#n=33

#Z (P<0.25) = -0.67448975       #=NORMSINV(0.25) in excel
#Z (P<0.2) = -0.841621234       #=NORMSINV(0.2) in excel
#Z (P<0.15) = -1.036433389
#Z (P<0.1) = -1.281551566
#Z (P<0.05) = -1.644853627

z_cutoff=-0.841621234

echo "# | FILE                           | hit counts | ZScore sum. | Hit score (decimal) | Hit score (binary)" > 3_z_hit_score$z_cutoff.txt

for ((i=$m_start; i<=$m_end; i++)); do
    # fetch a PDB FILE name
    PDB_FILES=$(awk '{print $1, $2}' $FILE | awk '{if ($1 == '$i') print $2}')
    NEW_FILES=${PDB_FILES/.log/}

    # variables initialization
    count=0
    z_score_sum=0
    let "bin_sum=2#0"
    
    for Z_HIT_FILES in class*.z_hit; do
        # ID Matching from *.z_hit files
        id=$(awk '{print $1,$5}' $Z_HIT_FILES | awk '{if ($1 == '$i') print $1}')

        # if a hit was found 
        if [ $id ]; then
            if [ "$id" -gt "0" ]; then
                # fetch matching class number
                class_id=${Z_HIT_FILES/class/}
                class_id=${class_id/.z_hit/}

                # binary bit operation
                let "bin=2#1 << $class_id"
                #echo "ibase=10;obase=2;$bin" | bc
                
                let "bin_sum=$bin_sum | $bin"
                #printf "%d \t binary_number = %d\n \t binary_sum %d\n" $class_id $bin $bin_sum

                z_score=$(awk '{print $1,$5}' $Z_HIT_FILES | awk '{if ($1 == '$i') print $2}')
                count=$(expr $count + 1)
                z_score_sum=$(echo $z_score_sum + $z_score | bc)
                
                printf "\t%d \t%s \tcount=%d \tZ_score=%.3f \tZ_score_sum=%.3f \t%d " $id $Z_HIT_FILES $count $z_score $z_score_sum $bin_sum
                echo "ibase=10;obase=2;$bin_sum" | bc
            fi
        fi
    done

    printf "%d | %s | %d | %.3f | %d | " $i $NEW_FILES.dat $count $z_score_sum $bin_sum
    echo "ibase=10;obase=2;$bin_sum" | bc
    
    printf "%d | %s | %d | %.3f | %d | " $i $NEW_FILES.dat $count $z_score_sum $bin_sum >> 3_z_hit_score$z_cutoff.txt
    echo "ibase=10;obase=2;$bin_sum" | bc >> 3_z_hit_score$z_cutoff.txt
done

