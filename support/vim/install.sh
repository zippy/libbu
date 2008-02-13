#!/bin/bash

for type in {ftdetect,ftplugin,syntax}/taf.vim; do
	if [[ ! -e $type ]]; then
		echo "You don't seem to be in the right directory."
		echo "Run this from libbu++/support/vim"
		exit
	fi
done

if [[ $(whoami) != "root" ]]; then
	echo You should probably run this as root...
	exit
fi

for type in ftdetect ftplugin syntax; do
	for dest in /usr/share/vim/vim*; do
		if [[ ! -d $dest/$type ]]; then mkdir -v $dest/$type; fi
		if [[ -e $dest/$type/taf.vim ]]; then rm -v $dest/$type/taf.vim; fi
		cp -v $type/taf.vim $dest/$type
	done
done
