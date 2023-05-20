#!/bin/bash

# make clean
# make hex
# sudo -E make flash

if [ "$1" == "mm" ]; then
    app="_${1}"
elif [ "$1" == "add" ]; then
    app="_${1}"
elif [ "$1" == "mul" ]; then
    app="_${1}"
elif [ "$1" == "shr" ]; then
    app="_${1}"
elif [ "$1" == "xor" ]; then
    app="_${1}"
elif [ "$1" == "regread" ]; then
    app="_${1}"
elif [ "$1" == "regwrite" ]; then
    app="_${1}"
elif [ "$1" == "sramread" ]; then
    app="_${1}"
elif [ "$1" == "sramwrite" ]; then
    app="_${1}"
else
    app=""
    echo "Invalid first argument"
    exit
fi

if [ "$2" == "loop" ]; then
    loop="_loop"
else
    loop=""
fi

echo "tcgra_app${app}${loop}.c"

cp -f tcgra_app${app}${loop}.c tcgra_app.c
make clean
make hex
sudo -E make flash