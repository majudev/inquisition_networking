#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR
if [ -e bin ]; then
	rm -r bin
fi
if [ -e lib ]; then
	rm -r lib
fi
if [ -e obj ]; then
	rm -r obj
fi
rm *.deb
