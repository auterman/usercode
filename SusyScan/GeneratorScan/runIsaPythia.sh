#!/bin/bash
rm -f isa.out
#echo '...running IsaJet'
echo \'isa.out\' > para_isa.tmp
echo '1' >> para_isa.tmp
echo $1 >> para_isa.tmp
echo $2 >> para_isa.tmp
echo $4 >> para_isa.tmp
echo $3 >> para_isa.tmp
echo $5 >> para_isa.tmp
echo '170.9' >> para_isa.tmp
./IsaJet771/isasugra.x < para_isa.tmp > isa.tmp
#echo '...running Pythia'
echo $1 > para_pyt.tmp
echo $2 >> para_pyt.tmp
echo $3 >> para_pyt.tmp
echo $4 >> para_pyt.tmp
echo $5 >> para_pyt.tmp
./Pythia/main65.x < para_pyt.tmp > pythia.out
rm -f para_pyt.tmp para_isa.tmp isa.tmp
