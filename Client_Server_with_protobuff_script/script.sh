#!/bin/bash

string=$*
total_letters=`echo $string | tr -d [:space:] | wc -m`
echo "The message has $total_letters letters." > result.txt
exit $total_letters