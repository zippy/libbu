#!/bin/bash

IN=$1
OUT=$2

EXPR="s@00000000@null@g\n"
CNT=1
for PTR in $(grep -o -E -e '[0-9A-F]{8}' $IN | sort | uniq | grep -v 00000000); do
	EXPR="${EXPR}s@${PTR}@$(printf %4d ${CNT})@g\n"
	CNT=$(($CNT+1))
done

temp=$(mktemp)
echo -e "$EXPR" > $temp
sed -f $temp $IN > $OUT
rm $temp

