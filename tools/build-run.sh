#!/bin/bash

# setup environment
export CPU=x86_64
export OS=linux
export VARIANT=debug
export AJ_ROOT=$HOME/src/core-alljoyn
export AJLIB=$AJ_ROOT/build/linux/$CPU/$VARIANT/dist/cpp/lib
export AJINC=$AJ_ROOT/build/linux/$CPU/$VARIANT/dist/cpp/inc
export LD_LIBRARY_PATH=$AJLIB:$LD_LIBRARY_PATH

# build
export SRC=dcs
make -C ../build

# run
./../build/bin/debug/$SRC -c ../data/config.ini -t 300
