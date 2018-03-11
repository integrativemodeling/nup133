#for FILES in *.*
i=0
for FILES in pdb/*.pdb
do
  #NEW_FILES=${FILES/_.model/}
  #NEW_FILES=${FILES/_23/}
  #NEW_FILES=${FILES/-1/}
  #NEW_FILES=${FILES/23904./23904_3.}
  #NEW_FILES=${FILES/_merged}
  #echo "$PDB: $NEW_PDB ${#NEW_PDB}"
  i=$(expr $i + 1)
  MOD10=$(($i % 10))
  NEW_FILES="pdb"$MOD10
  echo "$i: $MOD10 $FILES $NEW_FILES"
  cp $FILES $NEW_FILES
done

#ls -l

