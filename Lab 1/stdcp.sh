#OS Lab 1 dt. 9th Aug 2018
#For stdcp to be a custom command for cp, copy paste the code below in ~/.bashrc
#----------ABDUL SATTAR MAPARA-------------#
#----------BT16CSE053----------------------#
function stdcp(){
  let num=2
  if test "$#" -ne $num #exactly 2 arguments required
  then
    if [ $# -lt $num ]
    then
      echo "Too few arguments to stdcp"
    else
      echo "Too many arguments to stdcp"
    fi
  else
    if [ "$1" == "$2" ]  #files can't be same
    then
      echo "Same files"
    else
      if [ -f $1 ] #check if 1st file exists
      then
		let i=0
		let zero=0
		let one=1
        while read -r line
        do
		  if [ $i == $zero ]
		  then
			echo $line > $2
			i=`expr $i+$one`
		  else	
          	echo $line >> $2
		  fi
        done < $1
      else
        echo "File Not Found"
      fi
    fi
  fi
}
