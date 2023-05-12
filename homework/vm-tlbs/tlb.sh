#!/usr/bin/env zsh

num_pages=2
num_trials=10000

for i in {1..14}
do
    time=$(./tlb $num_pages $num_trials)
    echo $num_pages $time
    let "num_pages = num_pages * 2"
done
