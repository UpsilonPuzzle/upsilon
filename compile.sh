#!/bin/bash
echo "Compiling upsilon..."
g++ src/*.cpp -llua -ldl -lSDL2 -lSDL2_image -g -std=c++0x -o upsilon
if [ $? == 0 ]; then
	echo "Done. You can now run upsilon by executing ./upsilon"
fi
