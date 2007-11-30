#!/bin/bash
# Shell script to generate random seeds
# C.A., 16.4.2007
TIME=`date +%s`
SEED1=$(($TIME-1195555555))
SEED2=$(($TIME-1195544444))
SEED3=$(($TIME-1195533333))
SEED4=$(($TIME-1195522222))

if [ -z $1 ]; then
	FILE="crab.cfg"
else
	FILE=$1
fi

cp -f $FILE $FILE.bkp

if [ -e $FILE ]; then 
	PREV_PY=`grep pythia_seed $FILE`
	PREV_VT=`grep vtx_seed $FILE`
	PREV_GE=`grep g4_seed $FILE`
	PREV_MI=`grep mix_seed $FILE`
	sed -e "s/$PREV_PY/pythia_seed = $SEED1/"   $FILE.'bkp' > $FILE'.1'
	sed -e "s/$PREV_VT/vtx_seed = $SEED2/"      $FILE'.1'   > $FILE'.2'
	sed -e "s/$PREV_GE/g4_seed = $SEED3/"       $FILE'.2'   > $FILE'.3'
	sed -e "s/$PREV_MI/mix_seed = $SEED4/"      $FILE'.3'   > $FILE
	rm -f $FILE'.1' $FILE'.2'  $FILE'.3' $FILE.bkp 
	echo "SUCCESS! Used pythia_seed=$SEED1 , vtx_seed=$SEED2 , g4_seed=$SEED3 and mix_seed=$SEED4 in file $FILE"
else
	echo "ERROR: you have no '$FILE' file"
fi
