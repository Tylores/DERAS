#!/bin/bash

# setup environment
export CPU=x86_64
export OS=linux
export VARIANT=debug
export AJ_ROOT=$HOME/src/core-alljoyn
export AJ_CPP=$AJ_ROOT/build/$OS/$CPU/$VARIANT/dist/cpp
export AJ_LIB=$AJ_CPP/lib
export AJ_INC=$AJ_CPP/inc
export LD_LIBRARY_PATH=$AJ_LIB:$LD_LIBRARY_PATH

# alljoyn router
# first kill any daemon's that are currently running, then start new daemon
# the last /dev/null 2>&1 & has all stdout set to null and runs in background
export AJ_ROUTER=stand-alone
killall alljoyn-daemon
sleep 1
$AJ_CPP/bin/alljoyn-daemon --config-file=../data/rn-config.xml > /dev/null 2>&1 &

# build
export SRC=deras
make -C ../build

# run
./../build/bin/debug/$SRC
