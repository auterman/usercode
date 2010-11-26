#!/bin/zsh
  #
  #(make sure the right shell will be used)
  #$ -S /bin/zsh
  #
  #(the cpu time for this job)
  #$ -l h_cpu=00:30:00
  #
  #(the maximum memory usage of this job)
  #$ -l h_vmem=1500M
  #
  #(stderr and stdout are merged together to stdout)
  #$ -j y
  #
  #(send mail on job's end and abort)
  #$ -m ae

  ZERO="" #$SGE_TASK_ID
  if [ $SGE_TASK_ID -lt 100 ]; then
    ZERO="0"
  fi
  if [ $SGE_TASK_ID -lt 10 ]; then
    ZERO="00"
  fi
  JOB="$ZERO$SGE_TASK_ID"

  # change to scratch directory (local, not lustre in this example)
  cd $TMPDIR
  # copy raw data to scratch directory (here from the afs)
  # DO NOT USE afscp UNTIL FURTHER NOTICE afscp /afs/naf.desy.de/group/vo/rawdata/bigfile bigfile
  # cp /afs/naf.desy.de/group/vo/rawdata/bigfile bigfile
  
  cp -r /afs/naf.desy.de/user/a/auterman/production/SusyScan/GeneratorScan .
  cd GeneratorScan
  rm -f *.tmp *.out *.dat
    
  # write calculated output also to scratch first
  #  /path/to/executable/which/works/on/bigfile -in bigfile -out output

  ls -l > log_$JOB.txt 
  pwd >>  log_$JOB.txt
  #make clean; make>> log_$JOB.txt
  echo "SGE_TASK_ID = $SGE_TASK_ID" >> log_$JOB.txt
  
  ## The real stuff to run:
  mtop=170.9
  tanb=10
  A_zero=0
  signmu=1
  m_zero=$((10*$SGE_TASK_ID))
  m_zero_max=$(($m_zero+10))
  while [ "$m_zero" -lt "$m_zero_max" ]
  do
    m_half=100
    while [ "$m_half" -le 500 ]
    do
       echo "mzero = $m_zero,  mhalf = $m_half" >> log_$JOB.txt
       ./runIsaPythia.sh $m_zero $m_half $tanb $A_zero $signmu
       ./fill isa.out scan_$JOB.dat
       #runPythia.sh $m_zero $m_half $tanb $A_zero $signmu
       #./fill_xsec pythia.out scan_xsec.dat
       m_half=$(($m_half+10))
    done

    m_zero=$(($m_zero+10))
  done

  
  # copy the output back into afs
  # DO NOT USE afscp UNTIL FURTHER NOTICE afscp output /afs/naf.desy.de/group/vo/output/

  cp scan_$JOB.dat /afs/naf.desy.de/user/a/auterman/production/SusyScan/GeneratorScan/job/.
  cp log_$JOB.txt /afs/naf.desy.de/user/a/auterman/production/SusyScan/GeneratorScan/job/.
