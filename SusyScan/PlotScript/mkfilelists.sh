#!/bin/bash
for i in `ls -d limits_*`
do
   echo $i
   ls $i/mSUGRA* > $i/filelist.txt
done
