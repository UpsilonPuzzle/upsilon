#!/bin/sh
apt-get install libsdl2-dev libsdl2-image-dev -y
if [ $? != 0 ]; then
	echo "Error while executing apt-get."
	exit
fi

wget http://www.lua.org/ftp/lua-5.3.2.tar.gz
tar zxf lua-5.3.2.tar.gz -C lua
rm lua.tar.gz
cd lua
make generic -j`nproc`
make install
echo "Compiling upsilon..."
g++ *.cpp -llua -ldl -lSDL2 -lSDL2_image -g -std=c++0x -o upsilon
