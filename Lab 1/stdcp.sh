#OS Lab 1 dt. 9th Aug 2018
#For stdcp to be a custom command for cp, copy paste the code below in ~/.bashrc


function stdcp(){
  let num=2
  if test "$#" -ne $num #exactly 2 arguments required
  then
    if test "$#" < $num
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
        if test read -r line
        then
          echo $line > $2
        fi < $1
        while read -r line
        do
          echo $line >> $2
        done < $1
      else
        echo "File Not Found"
      fi
    fi
  fi
}
