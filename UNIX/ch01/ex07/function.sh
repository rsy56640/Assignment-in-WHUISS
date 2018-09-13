#!/bin/sh
#####################
sum(){
	ret=`expr $1 + $2`
}

if [ $# -lt 2 ]
then
	echo Please input two parameters
else
	sum $1 $2
	echo " $1 + $2 = $ret"
fi

