#!/bin/sh

while true ; do
	../tools/fuzzxml -r 0.1 $1 fuzz.xml

	if ../src/lasem-render-0.4 fuzz.xml -o fuzz.png ; then
		echo "Fuzzed file processed correctly"
	else
		echo "Oops ! Crash file detected..."
		exit
	fi
done
