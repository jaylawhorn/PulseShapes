#!/bin/bash

builddir=$1
file=$2
eosfolder=$3
outfolder=$4

workDir=`pwd`
echo `hostname`
echo "args:    $*"

SCRAM_ARCH=slc6_amd64_gcc491

cd ${builddir}
eval `scramv1 runtime -sh`
#cmsenv
cd ${workDir}
which root

cp ${builddir}/src/PulseShapes/makeHists.C .
cp ${builddir}/src/PulseShapes/rootlogon.C .

root -l -q makeHists.C+\(\"root://eoscms//eos/cms/${eosfolder}${file}\",\"${outfolder}${file}\"\)

#cp Output.root ${outfolder}${file}

#xrdcp Output.root root://eoscms/${outfolder}${file}

status=`echo $?`
echo "Status - $status"

exit $status