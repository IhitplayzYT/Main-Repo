s_dir="/home/IhitplayzYT/buildsystemplates"
proj_name=$1
c_dir=$PWD
cd $s_dir
echo "Select the template to be used : "
select x in *;do
    template=$x
    break
done
cd $c_dir
s_dir="$s_dir/$template"

    if [ -z proj_name ];then
        echo -e "\e[1;31m ERROR :\e[0m Input valid name for the buildsystem to create" >&2
        echo "FORMAT : $0 <proj_name>"
        exit 1
    elif [ -d $proj_name ];then
        echo -e "\e[1;31m ERROR :\e[0m Directory already exists in current directory!" >&2
        exit 2
    elif [ -z s_dir ];then
        echo -e "\e[1;31m ERROR :\e[0m The Template build system is unable to be found in $s_dir"
        exit 3
    fi


mkdir $proj_name
cd $proj_name
cp -R $s_dir/* .
for x in *;do
    if [[ $x == *PROJ_NAME* ]];then
        new=$(sed "s/PROJ_NAME/$proj_name/g" <<< "$x")
        sed "s/PROJ_NAME/$proj_name/g" < $x > $new
        rm -f $x
    else
        sed -i "s/PROJ_NAME/$proj_name/g" $x
    fi
    
done
echo -e "\e[1;32m Creation is successful!\e[0m"