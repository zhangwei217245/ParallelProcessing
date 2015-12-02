#!/bin/bash
#PBS -V
#PBS -cwd
#PBS -S /bin/bash
#PBS -N mpi
#PBS -o $JOB_NAME.o$JOB_ID
#PBS -e $JOB_NAME.e$JOB_ID
#PBS -q normal
#PBS -l select=4:ncpus=6:mem=2gb:mpiprocs=1
#PBS
#PBS -P hrothgar

cmd="/lustre/work/newapps/openmpi-1.10.1.compile/bin/mpiexec -output-filename output/mpi_openmp_floyd  -$MFIL $SGE_CWD_PATH/machinefile.$JOB_ID $SGE_CWD_PATH/mpi_openmp_floyd.out  -msize 6"
echo cmd=$cmd
$cmd
