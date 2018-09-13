#!/bin/sh
case "$1" in
START|start)
    echo 'program started!'
;;
STOP|stop)
    echo 'program stopped!'
;;
RESTART|restart)
    echo 'program restarted!'
;;
*)
    echo 'usage: case.sh [start|stop|restart]'
;;
esac
