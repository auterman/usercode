#!/bin/bash
MERGE=scan.dat
#MERGE=test
for file in scan_*.dat
do
	echo "Merge file $file to $MERGE"
	cat $file >> $MERGE
done
