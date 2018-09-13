#!/bin/sh
a=1
b=2
c=3
x=`expr $a \* \( $b + $c \)`
y=`expr $x \> 20`
echo $y
[ `expr $x \> 20` = 0 ] && echo '$x = ' $x is less than 20

