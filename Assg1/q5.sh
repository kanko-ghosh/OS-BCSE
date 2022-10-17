date_disp() {
    T=$1
    Y=$((T/60/60/24/365))
    Mo=$((T/60/60/24%365/30))
    D=$((T/60/60/24%365%30))
    H=$((T/60/60%24))
    M=$((T/60%60))
    S=$((T%60))

    (( $Y > 0 )) && res="$res$Y years, "
    (( $Mo > 0 )) && res="$res$Mo months, "
    (( $D > 0 )) && res="$res$D days, "
    (( $H > 0 )) && res="$res$H hours, "
    (( $M > 0 )) && res="$res$M minutes, "
    (( $S > 0 )) && res="$res$S seconds"
    echo $res
}


echo -n "enter date number 1(DD/MM/YYYY): "
read dt1
ddmm1=${dt1%/*}
dd1=${ddmm1%/*}
mm1=${ddmm1##*/}
yyyy1=${dt1##*/}
date1=`date -d $mm1/$dd1/$yyyy1`

echo -n "enter date number 2(DD/MM/YYYY): "
read dt2
ddmm2=${dt2%/*}
dd2=${ddmm2%/*}
mm2=${ddmm2##*/}
yyyy2=${dt2##*/}
date2=`date -d $mm2/$dd2/$yyyy2`




choice1=${date1:0:2}
case $choice1 in
    "Mo") echo Person one was born in: Monday ;;
    "Tu") echo Person one was born in: Tuesday ;;
    "We") echo Person one was born in: Wednesday ;;
    "Th") echo Person one was born in: Thursday ;;
    "Fr") echo Person one was born in: Friday ;;
    "Sa") echo Person one was born in: Saturday ;;
    "Su") echo Person one was born in: Sunday ;;
    "da") echo Date is invalid ;;
esac

choice2=${date2:0:2}
case $choice2 in
    "Mo") echo Person two was born in: Monday ;;
    "Tu") echo Person two was born in: Tuesday ;;
    "We") echo Person two was born in: Wednesday ;;
    "Th") echo Person two was born in: Thursday ;;
    "Fr") echo Person two was born in: Friday ;;
    "Sa") echo Person two was born in: Saturday ;;
    "Su") echo Person two was born in: Sunday ;;
    "da") echo Date is invalid ;;
esac

if [ $choice1 != "da" ] && [ $choice2 != "da" ]
then
    if [ $choice1 == $choice2 ]
    then 
        echo "Same day"
    else    
        echo "different day"
    fi
fi

if [ $choice1 != "da" ]
then
    echo -n "Age of person 1 is: "
    temp=`date +%s.%N -d $mm1/$dd1/$yyyy1`
    cur=`date +%s.%N`
    temp=$(echo "$cur - $temp" | bc)
    echo `date_disp ${temp%.*}`
fi

if [ $choice2 != "da" ]
then
    echo -n "Age of person 2 is: "
    temp=`date +%s.%N -d $mm2/$dd2/$yyyy2`
    cur=`date +%s.%N`
    temp=$(echo "$cur - $temp" | bc)
    echo `date_disp ${temp%.*}`
fi