## Author: Wei Zhang
## Author: Ali Nosrati

CC=mpicc
CFLAGS=-i_dynamic -openmp
#EXFLAGS=--mca pls_rsh_agent ssh 
EXFLAGS=
OBJ=mpi_openmp_floyd.out
UTIL=timeutil.c
SRC=main.c floyd.c util.c
EXEC=mpirun

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
		@${EXEC} ${EXEFLAGS} -np ${NUM} -x OMP_NUM_THREADS -output-filename output/$(subst .out,,${OBJ}) ${OBJ} ${ARGS}

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
