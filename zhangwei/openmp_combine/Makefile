## Author: Wei Zhang
## Author: Ali Nosrati

#MPI_HOME=/home/zhang56/mpich-install
#MPI_HOME=/home/zhang56/openmpi_1.10.1
#MPI_HOME=/lustre/work/newapps/openmpi-1.10.1.compile
MPI_HOME=/lustre/work/newapps/mpich-3.1.4
CC=$(MPI_HOME)/bin/mpicc
#CFLAGS=-i_dynamic -openmp
CFLAGS=-openmp
#EXFLAGS=--mca pls_rsh_agent ssh 
EXFLAGS=
OBJ=mpi_openmp_floyd.out
UTIL=timeutil.c
SRC=hello.c
EXEC=$(MPI_HOME)/bin/mpiexec

WDIR=$(shell pwd)
OS=$(shell uname)
ifeq ($(OS),Darwin)
		UTIL=timeutil_osx.c
else
		UTIL=timeutil.c
endif

${OBJ}: ${SRC} ${UTIL} 
	${CC} ${CFLAGS} -o $@ ${SRC} ${UTIL}


.PHONY: run
run:
		echo ##@${EXEC} ${EXEFLAGS} -np ${NUM} -x OMP_NUM_THREADS -output-filename output/$(subst .out,,${OBJ}) ${OBJ} ${ARGS}
		@${EXEC} ${EXEFLAGS} -n ${NUM} ${WDIR}/${OBJ} ${ARGS}

.PHONY: clean
clean:
	-rm -rf ${OBJ}
	-rm -rf *.out
	-rm -rf *.btr
	-rm -rf output/

.PHONY: all
all:	clean ${OBJ} run

.PHONY: viewrst
viewrst:	all
	-more output/*	
