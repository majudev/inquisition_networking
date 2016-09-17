#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR
if ! [ -e bin ]; then
	mkdir bin
fi
if ! [ -e lib ]; then
	mkdir lib
fi
if ! [ -e obj ]; then
	mkdir obj
fi
FLAGS="-O3 -Wall -fPIC -Iinclude -m32"
LIBS=""
g++ -c $FLAGS src/logmessage.cpp -o obj/logmessage.o
g++ -c $FLAGS src/data.cpp -o obj/data.o
g++ -c $FLAGS src/client.cpp -o obj/client.o
g++ -c $FLAGS src/server.cpp -o obj/server.o
g++ -Wall -shared -m32\
	obj/logmessage.o\
	obj/data.o\
	obj/client.o\
	obj/server.o\
	$LIBS \
	-o lib/libnetquisition.so
TESTFLAGS="-Wall -O3 -Ofast -Llib -linquisition -lnetquisition -Iinclude $LIBS -std=c++11 -lpthread -m32"
g++ \
	$TESTFLAGS \
	src/test.cpp \
	-o bin/netquisition
