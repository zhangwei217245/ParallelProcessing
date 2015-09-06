#!/bin/bash
#$ -V
#$ -cwd
#$ -S /bin/bash
#$ -N mpi
#$ -o $JOB_NAME.o$JOB_ID
#$ -e $JOB_NAME.e$JOB_ID
#$ -q normal
#$ -pe fill 12
#$ -P hrothgar

cmd="$MCMD -np $NSLOTS -$MFIL $SGE_CWD_PATH/machinefile.$JOB_ID $SGE_CWD_PATH/helloworld"
echo cmd=$cmd
$cmd
