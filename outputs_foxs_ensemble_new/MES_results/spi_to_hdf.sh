TARGET=23904

for FILE in 23904_*.spi
do
    echo $FILE
    e2proc2d.py $FILE $TARGET.hdf
done

