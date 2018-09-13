#!/bin/sh
a=1;
b=2;
c=3;
((a=a+1));
echo $a;
a=$((a+1,b++,c++));
echo $a,$b,$c
