#!/bin/bash

export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH

rm -f stats
echo "1. getsections_dl RTLD_LAZY stats (in usec)" >> ./stats
let "sum = 0"
for i in $(seq 1 50)
do
    usec=`./getsections_dl getsyms RTLD_LAZY | grep dlopen_usecs | cut -d' ' -f2`
    echo $usec >> ./stats
    let "sum = $sum + $usec"
done
let "avg = $sum / 50"
echo "--------" >> ./stats
echo "average = $avg" >> ./stats
echo "--------" >> ./stats

echo "" >> ./stats
echo "2. getsections_dl RTLD_NOW stats (in usec)" >> ./stats
let "sum = 0"
for i in $(seq 1 50)
do
    usec=`./getsections_dl getsyms RTLD_NOW | grep dlopen_usecs | cut -d' ' -f2`
    echo $usec >> ./stats
    let "sum = $sum + $usec"
done
let "avg = $sum / 50"
echo "--------" >> ./stats
echo "average = $avg" >> ./stats
echo "--------" >> ./stats

echo "" >> ./stats
echo "3. getsyms_dl RTLD_LAZY stats (in usec)" >> ./stats
let "sum = 0"
for i in $(seq 1 50)
do
    usec=`./getsyms_dl getsyms RTLD_LAZY | grep dlopen_usecs | cut -d' ' -f2`
    echo $usec >> ./stats
    let "sum = $sum + $usec"
done
let "avg = $sum / 50"
echo "--------" >> ./stats
echo "average = $avg" >> ./stats
echo "--------" >> ./stats

echo "" >> ./stats
echo "4. getsyms_dl RTLD_NOW stats (in usec)" >> ./stats
let "sum = 0"
for i in $(seq 1 50)
do
    usec=`./getsyms_dl getsyms RTLD_NOW | grep dlopen_usecs | cut -d' ' -f2`
    echo $usec >> ./stats
    let "sum = $sum + $usec"
done
let "avg = $sum / 50"
echo "--------" >> ./stats
echo "average = $avg" >> ./stats
echo "--------" >> ./stats

echo "" >> ./stats


