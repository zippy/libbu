#!/bin/bash

CMD="ln -svf $PWD/bu /usr/include; ln -svf $PWD/libbu++.a /usr/lib"

if [ $UID == 0 ]; then
	bash -c "$CMD"
else
	echo This script needs root access...
	su root -c "$CMD"
fi
