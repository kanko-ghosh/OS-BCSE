count_files()
{
    if [ $# -eq 0 ]
    then
        echo 0
    else
        PATH="$1" 
        no_of_files=0
        for file in "$PATH"/*
        do
            if [ -d "$file" ]
            then 
                rec_return=$(count_files "$file/")
                no_of_files=$(($no_of_files + $rec_return))
            else
                no_of_files=$(($no_of_files + 1))
            fi
        done
        
        echo $no_of_files
    fi
}

count_files_with_dir()
{
    if [ $# -eq 0 ]
    then
        echo 0
    else
        PATH="$1" 
        no_of_files=0
        for file in "$PATH"/*
        do
            no_of_files=$(($no_of_files + 1))
            if [ -d "$file" ]
            then 
                rec_return=$(count_files_with_dir "$file/")
                no_of_files=$(($no_of_files + $rec_return))
            fi
        done
        
        echo $no_of_files
    fi
}

ans=$(count_files ".")
echo "Number of files: $ans"
ans=$(count_files_with_dir ".")
echo "Number of files and directories: $ans"