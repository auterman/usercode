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
echo $6 >> para_isa.tmp
./IsaJet771/isasugra.x < para_isa.tmp > isa.tmp
rm -f *.tmp
