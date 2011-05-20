#!/bin/bash
for i in `ls -d limits_*`
do
   echo $i
   #ls $i/mSUGRA* > $i/filelist.txt
   ls $i/mSUGRA_*_*_3_*_1_0.dat > $i/filelist_tb3.txt
   ls $i/mSUGRA_*_*_10_*_1_0.dat > $i/filelist_tb10.txt
   ls $i/mSUGRA_*_*_50_*_1_0.dat > $i/filelist_tb50.txt
done
