#!/bin/bash

if test "$#" -ne 2
then
	echo "Two args needed!"
	exit 1
fi

git mv "$1" "$2"
sed -i "s/$1/$2/g" wscript

