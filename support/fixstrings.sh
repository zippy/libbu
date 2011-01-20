#!/bin/bash

echo Automatically fixing all .cpp, .h, .unit, and .doxy files in this tree.
echo Files are updated in place, you have them in version control, right?
echo If this isn\'t what you want, press ctrl+c, otherwise, enter.
echo
read
for i in $(find . -iname '*.cpp' -o -iname '*.h' -o -iname '*.unit' -o -iname '*.doxy'); do
	echo -n $i...
	sed -i $i \
		-e 's@"fstring.h@"bu/string.h@g' \
		-e 's@"bu/fstring.h@"bu/string.h@g' \
		-e 's@<fstring.h@<bu/string.h@g' \
		-e 's@<bu/fstring.h@<bu/string.h@g' \
		-e 's@FString@String@g'
	echo done.
done
