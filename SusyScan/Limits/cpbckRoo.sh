#!/bin/bash

for i in `ls ~/jobs/ | grep Roo` 
do
  DIR=`echo $i | cut -b 27-`
  #echo "cp $i to $DIR"
  cp ~/jobs/$i/job*/results/* $DIR/. 
  NUM=`grep -L RooMCMC $DIR/* | wc -l`
  echo "$NUM jobs in $DIR not finished; issue: ./Limit \`grep -L RooMCMC $DIR/*\`" 
done

