#! /bin/bash

for ((i=0 ; i<= $1 ; i++))
do
    IDADE=$(((RANDOM % 100)+1))
    # echo "./pessoas -u Pessoa${i} ${IDADE}"

    ./pessoas -u Pessoa${i} $IDADE
done