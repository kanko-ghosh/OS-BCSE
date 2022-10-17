date_disp()
{
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


