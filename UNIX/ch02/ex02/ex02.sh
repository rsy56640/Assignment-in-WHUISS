#!/bin/sh
./prog || echo 'prog returns non-zero'
./prog && echo 'prog returns 0'
