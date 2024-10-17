#!/bin/sh

if [ $0 = "/usr/bin/bash" -o $0 = "/bin/bash" -o $0 = "bash" -o $0 = "-bash" ]; then
    COMPILER_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
elif [ $0 = "sh" ]; then
    COMPILER_DIR=$(realpath $(dirname $0))
else
    COMPILER_DIR=$(realpath $(dirname $0))
fi

rm -rf build
mkdir build;
pre=build/preproc.s

# eliminar todo después de numeral, lineas vacías
# y todo depués del punto y coma 
cpp $1 \
    | sed 's/\#.*//' \
    | sed 's/:/;/g' \
    | sed '/^$/d' \
    | sed 's/ //g' \
    | sed 's/\;.*/;/' \
    | python $COMPILER_DIR/charhelper.py \
    > $pre 
$COMPILER_DIR/build/compiler < $pre 

