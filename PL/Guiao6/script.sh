#! /bin/bash

./server &

for ((i=0 ; i<= $1 ; i++))
do
    ./client ${i}
done

./client exit