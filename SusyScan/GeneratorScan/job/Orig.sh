#!/bin/zsh
  exec > "$TMPDIR"/stdout.txt 2>"$TMPDIR"/stderr.txt
  JOBNAME=AUniquePlaceholderName
  RESULTPATH=/scratch/hh/lustre/cms/user/auterman/jobs/$JOBNAME

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

  cd $TMPDIR
  cp -r /afs/naf.desy.de/user/a/auterman/production/SusyScan/GeneratorScan .
  cd GeneratorScan
    
  mkdir job$SGE_TASK_ID
  mkdir job$SGE_TASK_ID/results
  mkdir job$SGE_TASK_ID/log
  mkdir job$SGE_TASK_ID/config

  ls -l 
  pwd
  echo "SGE_TASK_ID = $SGE_TASK_ID"
  cat job/$JOBNAME/GenScan_$JOB
  cp  job/$JOBNAME/GenScan_$JOB job$SGE_TASK_ID/config/.
  date
  #for i in `cat job$SGE_TASK_ID/config/GenScan_$JOB` 
  x=0
  file=job$SGE_TASK_ID/config/GenScan_$JOB
  while [ $x -lt $(wc -l <$file) ]
  do
    let x=x+1
    rm -f *.tmp *.out
    head -n $x $file | tail -n 1 > run_isa.tmp
    echo "calling ./runIsaPythia.sh `cat run_isa.tmp`"  
    ./runIsa.sh `cat run_isa.tmp`
    ./fill isa.out job$SGE_TASK_ID/results/scan_$JOB.dat
  done
  du -h --max-depth=1
  date

  cp "$TMPDIR"/stdout.txt job$SGE_TASK_ID/log/.
  cp "$TMPDIR"/stderr.txt job$SGE_TASK_ID/log/.
  cp -r job$SGE_TASK_ID $RESULTPATH/.
