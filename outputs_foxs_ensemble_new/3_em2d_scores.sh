#for FILES in pdb_selected
i=0
n=23
#for FILES in log{1..44}/*.log
for FILES in log/*.log
do
    i=$(expr $i + 1)
    #NEW_FILES=${FILES/log*'/'/}
    NEW_FILES=${FILES/log\//}
    GLOBAL_SCORE=$(awk "/Global score/" $FILES | awk '{print $3}')
    echo -e "$i\t $NEW_FILES\t $GLOBAL_SCORE" >> log_zscore/global_scores.log

    for ((j=0; j<$n; j++))
    do
        TEXT='Score for image '$j':'
        SCORE=$(awk "/$TEXT/" $FILES | awk '{print $5}')
        OUTPUT="class$j.log"
        echo -e "$i\t $NEW_FILES\t $SCORE" >> log_zscore/$OUTPUT
    done
#    break
done


## Z-Score conversion
#recompute_zscore

#~/imp/trunk/applications/integrative_docking/combine_scores
#awk '{print $1, $2, $3}' data_merged.dat > data_merged_extracted.dat
#awk '{print $1,"|",$3,"| + |"}' test.txt > test2.txt
