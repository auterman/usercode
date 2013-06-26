#!/bin/bash


#rm -rf DemoPoint*
mkdir DemoPoint_$1

signaldir=signal_$1
./mkInputs -o DemoPoint_$1 -d DataCards/data.txt -q DataCards/qcd.txt -l DataCards/lostlepton.txt -t DataCards/tau.txt -z DataCards/zinvisible.txt -s DataCards/${signaldir}/mSUGRA_500_560_10_1_0_idx1105.dat DataCards/${signaldir}/mSUGRA_1000_400_10_1_0_idx2272.dat DataCards/${signaldir}/mSUGRA_2500_240_10_1_0_idx5789.dat
./mkInputs --single -o DemoPoint_$1 -d DataCards/data.txt -q DataCards/qcd.txt -l DataCards/lostlepton.txt -t DataCards/tau.txt -z DataCards/zinvisible.txt -s DataCards/${signaldir}/mSUGRA_500_560_10_1_0_idx1105.dat DataCards/${signaldir}/mSUGRA_1000_400_10_1_0_idx2272.dat DataCards/${signaldir}/mSUGRA_2500_240_10_1_0_idx5789.dat
