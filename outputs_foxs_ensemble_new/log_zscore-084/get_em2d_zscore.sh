#5734 | Int325_6_B10770001_MSE.dat | 3 | -3.731 | 65920 | 10000000110000000

echo Int325_6_B10770001_MSE.dat
for ((i=0; i<=22; i++)); do
    ii=$(expr $i + 1)
    FILE=class$i.zscore
    echo $ii $FILE
    cat $FILE | grep 5734 
done
echo

#3618 | Basic350_5_B10820001_MSE.dat | 1 | -1.860 | 65536 | 10000000000000000

echo Basic350_5_B10820001_MSE.dat
for ((i=0; i<=22; i++)); do
    ii=$(expr $i + 1)
    FILE=class$i.zscore
    echo $ii $FILE
    cat $FILE | grep 3618 
done
echo

#5036 | Int300_9_B10860001_MSE.dat | 7 | -8.117 | 1084228 | 100001000101101000100

echo Int300_9_B10860001_MSE.dat
for ((i=0; i<=22; i++)); do
    ii=$(expr $i + 1)
    FILE=class$i.zscore
    echo $ii $FILE
    cat $FILE | grep 5036 
done
echo

#4253 | Int300_2_B10100001_MSE.dat | 12 | -12.472 | 3045560 | 1011100111100010111000

echo Int300_2_B10100001_MSE.dat
for ((i=0; i<=22; i++)); do
    ii=$(expr $i + 1)
    FILE=class$i.zscore
    echo $ii $FILE
    cat $FILE | grep 4253 
done

