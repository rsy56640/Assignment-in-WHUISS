#!/bin/sh

LOG=./error.log
date >> $LOG
if [ -r errfile ]; then
    cat errfile >> $LOG
    rm errfile
else
    echo "no error" >> $LOG
fi
