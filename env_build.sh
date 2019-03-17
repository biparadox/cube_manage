#!/bin/bash

if [ $CUBESYSPATH == "" ]
then
    echo "can't find CUBESYSPATH"
    exit
fi

echo "CUBESYSPATH is ${CUBESYSPATH}"

cd locallib
make
cd -

cd module
make 
cd -

