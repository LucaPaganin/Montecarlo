#!/bin/bash

if [ -f main.cpp ]; then
    echo "rm main.cpp"
    rm main.cpp
fi

if [ -z $1 ]; then
    echo "Missing argument"
    exit 1
fi

echo "ln -s $1 main.cpp"
ln -s $1 main.cpp
