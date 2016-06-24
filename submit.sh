#!/bin/bash


eosfolder=/store/group/phys_susy/razor/HCALDPG/HcalRecoTesting/HighPtJet80/0000/
#eosfolder=/store/group/phys_susy/razor/HCALDPG/HcalRecoTesting/QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8/0000/

outfolder=/afs/cern.ch/work/j/jlawhorn/HighPtJet80v2/
#outfolder=/afs/cern.ch/work/j/jlawhorn/QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8/

for file in `/afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select ls ${eosfolder} | grep root`
do
    #if [[ -e ${outfolder}/${file} ]]; then
    #echo "Output file exists. Not submitting."
    #else
    #echo run.sh `pwd` $file $eosfolder $outfolder
    bsub -q 8nm run.sh ${CMSSW_BASE} $file $eosfolder $outfolder
    #fi
done