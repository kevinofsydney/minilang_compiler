#!/bin/bash

# Usage: ./run.sh <mode> <file>
# 	mode: scan|tokens|parse|pretty|symbol|typecheck|codegen
                                
# Check the command-line arguments are valid

if [ $# -lt 2 ]
then
	echo "Missing arguments"
	echo "Usage: $0 <mode> <file>"
	echo " + mode: scan|tokens|parse|pretty|symbol|typecheck|codegen"
	echo " + file: path to file (absolute or relative)"
	exit 1
fi

if [[ "|scan|tokens|parse|pretty|symbol|typecheck|codegen|" != *"|$1|"* ]]
then
	echo "Unknown mode \"$1\""
	echo "Usage: $0 <mode> <file>"
	echo " + mode: scan|tokens|parse|pretty|symbol|typecheck|codegen"
	echo " + file: path to file (absolute or relative)"
	exit 1

fi

# Invoke the compiler with the provided arguments: mode ($1) and file ($2)
#
# You MUST replace the following command with the command for invoking your compiler

./build.sh 1>/dev/null

if [[ $1 == "codegen" ]]
then
	if [[ ! -z $2 ]]
	then 
		tmp=`echo $2 | grep -oP "[a-z0-9]+.min"`
		./src/latte "$1" "$tmp" < "$2"
		exit 1
	fi
fi

./src/latte "$1" < "$2"
