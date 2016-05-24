#!/bin/bash

LOC=$(which llvm-config)

if [ -z $LOC ];
then 
	LOC=$(brew --prefix llvm)/bin; 
else
	LOC=$(llvm-config --prefix)/bin;
fi;

export LOC=$lOC
