#!/bin/bash

inputfile="$1"

function mkfunc()
{
	line=$(grep -nh "{%$1" "$inputfile")
	echo "#line ${line%%:*} \"$inputfile\" \\"
	echo "void $1() /**< expect ${2:-pass} */"
}

function mkadd()
{
	ex="expectPass"
	if [ "$2" == "fail" ]; then
		ex="expectFail"
	fi
	echo "\\\\tadd( static_cast<Bu::UnitSuite::Test>(\\&Unit::$1), \"$1\", Bu::UnitSuite::${ex} );\\\\n"
}

sedbits=""
init="#include \"bu/unitsuite.h\"\\\\n\\\\nclass Unit : public Bu::UnitSuite\\\\n{\\\\npublic:\\\\n\\\\nUnit()\\\\n{\\\\n\\\\tsetName(\"${1%.*}\");\\\\n"
for i in $(grep {% "$1"); do
	sedbits="${sedbits}s@${i}@$(mkfunc $(echo $i | sed -e 's/.*{%\(.*\)}/\1/' -e 's/:/ /g'))@\n"
	init="${init}$(mkadd $(echo $i | sed -e 's/.*{%\(.*\)}/\1/' -e 's/:/ /g'))"
done

init="${init}\\\\n}\\\\n\\\\nvirtual ~Unit() { }\\\\n\\\\n"

sedbits="${sedbits}s@.*{=Init}@${init}@\n"

temp=$(mktemp)
echo -e "$sedbits" > $temp
sed -f $temp $1 > $2
echo -e "\n};\n\nint main( int argc, char *argv[] )\n{\n\treturn Unit().run( argc, argv );\n}\n" >> $2

rm $temp

