#for FILES in pdb_selected
i=0
n=23
#for FILES in log{1..44}/*.log
for FILES in *.log
do
    #NEW_FILES=${FILES/log*'/'/}
    NEW_FILES=${FILES/.log/}

    awk '{print $1,"|",$3,"| + | | "}' $FILES > $NEW_FILES.txt
    recompute_zscore $NEW_FILES.txt > $NEW_FILES.zscore
    echo -e "\n"
done


## Z-Score conversion
#recompute_zscore

#~/imp/trunk/applications/integrative_docking/combine_scores
#awk '{print $1, $2, $3}' data_merged.dat > data_merged_extracted.dat
#awk '{print $1,"|",$3,"| + |"}' test.txt > test2.txt

#Z (P<0.25) = -0.67448975       #=NORMSINV(0.25) in excel
#Z (P<0.2) = -0.841621234       #=NORMSINV(0.2) in excel
#Z (P<0.15) = -1.036433389
#Z (P<0.1) = -1.281551566
#Z (P<0.05) = -1.644853627
