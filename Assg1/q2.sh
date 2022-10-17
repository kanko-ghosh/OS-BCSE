echo -n "Enter file name: "
read FNAME

if [ -f "$FNAME" ]
then
    echo -n "Number of lines in the file is: "
    echo `cat $FNAME | wc -l`
else
    echo "File $FNAME does not exist"
    echo "Creating a file with filename $FNAME"
    echo Enter Data, press $ to terminate
    touch $FNAME
    read data
    while [[ $data != "$" ]]
    do
        echo $data >> $FNAME
        read data
    done
fi