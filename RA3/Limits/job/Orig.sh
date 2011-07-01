#!/bin/zsh
  exec > "$TMPDIR"/stdout.txt 2>"$TMPDIR"/stderr.txt
  JOBNAME=AUniquePlaceholderName
  LIMITDIR=LimitDirPlaceholderName
  RELEASEDIR=RELEASEDIRPlaceholderName
  RESULTPATH=/scratch/hh/lustre/cms/user/auterman/jobs/$JOBNAME
  #RESULTPATH=/afs/naf.desy.de/user/a/auterman/Jobs/$JOBNAME

  ZERO="" #$SGE_TASK_ID
  if [ $SGE_TASK_ID -lt 1000 ]; then
    ZERO="0"
  fi
  if [ $SGE_TASK_ID -lt 100 ]; then
    ZERO="00"
  fi
  if [ $SGE_TASK_ID -lt 10 ]; then
    ZERO="000"
  fi
  JOB="$ZERO$SGE_TASK_ID"

  # change to scratch directory (local, not lustre in this example)
  cd $TMPDIR
  # copy raw data to scratch directory (here from the afs)
  # DO NOT USE afscp UNTIL FURTHER NOTICE afscp /afs/naf.desy.de/group/vo/rawdata/bigfile bigfile
  # cp /afs/naf.desy.de/group/vo/rawdata/bigfile bigfile
  #sleep $SGE_TASK_ID
  
  cp -r $RELEASEDIR .
  WORKDIR=`basename $RELEASEDIR`
  cd $WORKDIR
  ini root527 
  
  mkdir job$SGE_TASK_ID
  mkdir job$SGE_TASK_ID/results
  mkdir job$SGE_TASK_ID/log
  mkdir job$SGE_TASK_ID/config

  pwd 
  echo "SGE_TASK_ID = $SGE_TASK_ID" 
  cat job/$JOBNAME/limit_$JOB
  cp  job/$JOBNAME/limit_$JOB job$SGE_TASK_ID/config/.
  date
  for i in `cat job$SGE_TASK_ID/config/limit_$JOB` 
  do
    echo $i
    cat $i
    file="${i##*/}"
    echo "copying $LIMITDIR/$file to $RESULTPATH/results/$file"
    cp $LIMITDIR/$file job$SGE_TASK_ID/results/$file  
    echo "calling ./Limit job$SGE_TASK_ID/results/$file"  
    ./Limit job$SGE_TASK_ID/results/$file
  done
  du -h --max-depth=1
  date

  #cp results/* job$SGE_TASK_ID/results/.
  cp "$TMPDIR"/stdout.txt job$SGE_TASK_ID/log/.
  cp "$TMPDIR"/stderr.txt job$SGE_TASK_ID/log/.
  cp -r job$SGE_TASK_ID $RESULTPATH/.

### qsub -t 1-3609 -l h_cpu=00:15:00 -l h_vmem=4000M job.sh
