#! /usr/local/bin/zsh

FILE=CFG_FILE
JOB=JOB_NUMBER

CASTOR_DIR=Calib-PhotonJet-1_5_2
CMSSW_VERSION=CMSSW_1_5_2
BASEDIR=`pwd`


uname -a
cp -r /afs/cern.ch/user/a/auterman/scratch0/coding/$CMSSW_VERSION .
cd $BASEDIR/$CMSSW_VERSION/src
eval `scramv1 runtime -sh`

cmsRun Calibration/test/job_$JOB.$FILE.cfg
rfcp $FILE.$JOB.root /castor/cern.ch/user/a/auterman/$CASTOR_DIR/$FILE.$JOB.root
rfcp $FILE.$JOB.trktow.root /castor/cern.ch/user/a/auterman/$CASTOR_DIR/$FILE.$JOB.trktow.root

cd $BASEDIR
rm -rf $BASEDIR/$CMSSW_VERSION
