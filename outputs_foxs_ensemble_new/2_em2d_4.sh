#for FILES in pdb_selected
rm -rf temp_pdb4
mkdir temp_pdb4
cd temp_pdb4
cp ../all-images.sel .

i=0
for FILES in ../pdb4/*.pdb
do
    i=$(expr $i + 1)
    NEW_FILES=${FILES/..'/'pdb4'/'/}
    NEW_FILES=${NEW_FILES/.pdb/}
#    echo "$i: $FILES $NEW_FILES"

    echo "$i: setup_environment.sh python ~/imp_git/imp-fast/bin/emagefit_score.py $FILES ./all-images.sel 2.28739 1000 15 100 > $NEW_FILES.log"

    setup_environment.sh python ~/imp_git/imp-fast/bin/emagefit_score.py $FILES ./all-images.sel 2.28739 1000 15 100 > $NEW_FILES.log

    rm -rf coarse_match*.spi
    rm -rf fine_match*.spi
    rm -rf registration.params

#    break   # Skip entire rest of loop.
done

mv *.log ../log
cd ..
rm -rf temp_pdb4

