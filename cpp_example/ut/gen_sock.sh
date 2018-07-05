#!/bin/sh

DIR=`pwd`

HFILE=$DIR/$*

cd /home/mininet/googletest-master/googlemock/scripts/generator/
./gmock_gen.py $HFILE

cd $DIR

