# PulseShapes
fitting energy dependence for 2015/6 HBHE

(1) makeHists.C, using submit.sh and run.sh

(2) use hadd to combine all output files

(3) combineHists.C on combined file to make TGraph with proper errors / normalization

(4) simpleComp.C for plots shown in HCAL DPG week of Jun 13

(5) Analysis.C has unfinished fitting routine
