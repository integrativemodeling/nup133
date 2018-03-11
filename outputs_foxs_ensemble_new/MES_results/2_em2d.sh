#for FILES in pdb_selected
#rm -rf temp_pdb0
#mkdir temp_pdb0
#cd temp_pdb0
cp ../all-images.sel .

i=0
#for FILES in ../pdb0/*.pdb
for FILES in *.pdb
do
    i=$(expr $i + 1)
    NEW_FILES=${FILES/..'/'pdb0'/'/}
    NEW_FILES=${NEW_FILES/.pdb/}
#    echo "$i: $FILES $NEW_FILES"

    echo "$i: setup_environment.sh python ~/imp_git/imp-fast/bin/emagefit_score.py $FILES ./all-images.sel 2.28739 1000 15 100 > $NEW_FILES.log"
    setup_environment.sh python ~/imp_git/imp-fast/bin/emagefit_score.py $FILES ./all-images.sel 2.28739 1000 15 100 > $NEW_FILES.log

    mkdir $NEW_FILES
    mv $NEW_FILES.log $NEW_FILES
    mv coarse_match*.spi $NEW_FILES
    mv fine_match*.spi $NEW_FILES
    mv registration.params $NEW_FILES
    cp ../all-fine_images.sel $NEW_FILES

#    break   # Skip entire rest of loop.
done

#mv *.log ../log
#cd ..
#rm -rf temp_pdb0

