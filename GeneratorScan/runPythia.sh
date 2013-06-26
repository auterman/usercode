#!/bin/bash
rm -f pythia.out
#echo '...running Pythia'
echo $1 > para_pyt.tmp
echo $2 >> para_pyt.tmp
echo $3 >> para_pyt.tmp
echo $4 >> para_pyt.tmp
echo $5 >> para_pyt.tmp
./Pythia/main65.x < para_pyt.tmp > pythia.out
rm -f para_pyt.tmp
