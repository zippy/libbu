#!/bin/bash

TEMP=$(mktemp)
expand()
{
	local DIR="$1"
	for fl in $(svn ls "$DIR"); do
		if [ ${fl:(-1)} == "/" ]; then
			expand "${DIR}${fl}"
		else
			echo "${DIR}${fl}" >> $TEMP
		fi
	done
}

echo Building file lists from SVN...
expand ./

OUTNAME="libbu++-r$(svnversion).tar.bz2"
tar -vcT "${TEMP}" | bzip2 -9 > "$OUTNAME"

rm $TEMP

echo Produced "$OUTNAME"
