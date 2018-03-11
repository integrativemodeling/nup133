TARGET=Int300_9_B10860001_MSE

for FILE in fine_match-*.spi
do
    echo $FILE
    e2proc2d.py $FILE $TARGET.hdf
done

