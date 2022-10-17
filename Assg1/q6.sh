argc=$#
if [ $argc -eq 0 ]
then
    echo "NO INPUT"
elif [ $argc -eq 2 ]
then
    echo "MORE THAN 1 INPUT"
else
    FNAME=$1
    if ! [ -f $FNAME ]
    then
        echo "File Doesnt Exist, invalid input"
    else        
        echo -n "Enter the word to find: "
        read str
        linecnt=`wc -l < $FNAME`

        echo "Number of lines in file: $linecnt"
        echo "Number of lines having string \"$str\": `grep -w $str $FNAME | wc -l`"
        for (( i=1; i<=$linecnt; i++ ))
        do
            cnt=`awk "NR==$i {print}" $FNAME | grep -o -w $str | wc -l`
            if [ $cnt -ne 0 ]
            then
                echo "count in line $i: $cnt"
            fi
        done
    fi
fi
