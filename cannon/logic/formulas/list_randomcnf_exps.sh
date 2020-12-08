#!/bin/bash
# Takes argument folder containing CNFs
find $1 -type f -name "N150L*.cnf" | sort | 
  while IFS=. read name ext; 
    do ! [ -f "${name}_report.yaml" ] && echo "$PWD/$name.$ext"; 
  done;
