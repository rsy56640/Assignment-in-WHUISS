#!/bin/sh

x=5
y='005'
[ $x = $y ]&&echo 'as strings, x is equal to y'
[ $x != $y ]&&echo 'as strings, x is not equal to y'
[ $x -eq $y ] && echo 'as numbers, x is equal to y'
[ $x -ne $y ] && echo 'as numbers, x is not equal to y'
