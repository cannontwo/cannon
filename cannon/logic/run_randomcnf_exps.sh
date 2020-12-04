#!/bin/bash
# Only echo experiments for which a YAML report doesn't exist
find random_3sat -type f -name "*.cnf" | sort | 
  while IFS=. read name ext; 
    do ! [ -f "${name}_report.yaml" ] && echo "$name.$ext"; 
  done;
