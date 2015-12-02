#!/bin/bash
#$ -V
#$ -cwd
#$ -S /bin/bash
#$ -N testjob
#$ -o $JOB_NAME.o$JOB_ID
#$ -e $JOB_NAME.e$JOB_ID
#$ -q qtemp
#$ -pe fill 48
#$ -P hrothgar
mpirun  -np 4 -machinefile machinefile.$JOB_ID mpi_openmp_floyd.out -msize 6 > $SGE_CWD_PATH/test.out
