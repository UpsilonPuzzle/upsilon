#!/bin/bash
apt-get install libsdl2-dev libsdl2-image-dev
cd src
wget http://www.lua.org/ftp/lua-5.3.2.tar.gz
tar zxf lua-5.3.2.tar.gz
rm lua-5.3.2.tar.gz
mv lua-5.3.2 lua
cd lua
make generic -j`nproc`
make install
cd ../..
