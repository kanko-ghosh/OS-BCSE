echo "Program 1"

echo -n "Enter a variable: "
read uv1
echo -n "Enter another variable: "
read uv2

echo -n "Data Separated by comma: "
echo $uv1,$uv2
echo -n "Data Separated by comma: "
echo $uv2 and $uv1

len1=${#uv1}
len2=${#uv2}

for (( i=$len1-1; i>=0; i-- ));
do
    revstr=$revstr${uv1:$i:1}
done
for (( i=$len2-1; i>=0; i-- ));
do
    revstr2=$revstr2${uv2:$i:1}
done

echo Reverse of Input 1: $revstr
echo Reverse of Input 2: $revstr2

