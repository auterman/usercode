#!/bin/bash

for i in `ls ~/jobs/` 
do
  DIR=`echo $i | cut -b 24-`
  #echo "cp $i to $DIR"
  cp ~/jobs/$i/job*/results/* $DIR/. 
  NUM=`grep -L CLs $DIR/* | wc -l`
  echo "$NUM jobs in $DIR not finished: issue './Limit \`grep -L CLs $DIR/*\`'" 
done

