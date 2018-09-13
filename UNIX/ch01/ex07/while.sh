#!/bin/sh
#######################
# Name: while.sh
# Usage: Test while-do-done
# ……
#######################
a=10
while [ $a -gt 0 ]
do
	echo '$a = ' $a
	a=`expr $a - 1`
	sleep 1
done

