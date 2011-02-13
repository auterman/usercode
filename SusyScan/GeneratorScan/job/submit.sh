echo "Submitting generatorscan $1"
DATESTR=`date +%F-%H-%M`-GeneratorScan-$1
WORKPATH=/scratch/hh/lustre/cms/user/auterman/jobs/$DATESTR
mkdir $WORKPATH
mkdir $DATESTR
TOPROCESS=$DATESTR/GenScan.txt
touch $TOPROCESS
  
## The stuff to run:
tanb=50
A_zero=0
signmu=1
m_zero=0
mtop=173.3
m_zero_max=1000
while [ "$m_zero" -lt "$m_zero_max" ]
do
  m_half=0
  while [ "$m_half" -le 500 ]
  do
     echo "$m_zero $m_half $tanb $A_zero $signmu  $mtop" >> $TOPROCESS
     m_half=$(($m_half+5))
  done
  m_zero=$(($m_zero+5))
done
split -a 4 -l 100 -d $TOPROCESS $DATESTR/GenScan_

#Job preperation
rm -f Job*.sh
rm -f .temp
sed "s/AUniquePlaceholderName/$DATESTR/g" Orig.sh > .temp
sed "s/LimitDirPlaceholderName/$1/g" .temp > Job$1.sh
chmod a+x Job$1.sh
echo "qsub -j y -o /dev/null -t 1-`ls -l $DATESTR/GenScan_* | wc -l` -l h_cpu=11:59:59 -l h_vmem=4000M Job$1.sh"
