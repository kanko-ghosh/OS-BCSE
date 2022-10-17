argc=$#
if [ $argc -eq 0 ]
then
    echo "NO INPUT"
elif [ $argc -ge 2 ]
then
    echo "MORE THAN 1 INPUT"
else
    FNAME=$1
    if ! [ -f $FNAME ]
    then
        echo "File Doesnt Exist, invalid input"
    else        
        echo -n "Enter the pattern to find: "
        read str
        echo -n "Enter the word to replace with: "
        read new_str
        linecnt=`wc -l < $FNAME`
 
        echo "Number of lines in file: $linecnt"
        echo "Number of lines having string \"$str\": `grep $str $FNAME | wc -l`"
        cntTotal=0
        for (( i=1; i<=$linecnt; i++ ))
        do
            cnt=`awk "NR==$i {print}" $FNAME | fmt -w1  | grep $str | wc -l`
            cntTotal=$(($cntTotal + $cnt))
            if [ $cnt -ne 0 ]
            then
                echo "count in line $i: $cnt"
            fi
        done

        if [ $cntTotal -eq 0 ]
        then
            echo "'$str' not found"
        else
            echo "Total Count: $cntTotal"
            txt=`sed "s/$str/$new_str/g" $FNAME`
            echo "$txt" > $FNAME
            echo "File Saved!"
        fi
    fi
fi