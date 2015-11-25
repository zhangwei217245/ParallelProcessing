## Author: Wei Zhang
## Author: Ali Nosrati

MPI_PATH=/lustre/work/newapps/mpich-3.1.4
CC=${MPI_PATH}/bin/mpicc
#CFLAGS=-i_dynamic
CFLAGS=
OBJ=mpi_dijkstra.out
UTIL=util.c 
SRC=main.c dijkstra.c
EXEC=${MPI_PATH}/bin/mpirun

OS=$(shell uname)
ifeq ($(OS),Darwin)
		UTIL=util_osx.c
else
		UTIL=util.c
endif

${OBJ}: ${SRC} ${UTIL} 
	${CC} ${CFLAGS} -o $@ ${SRC} ${UTIL}


.PHONY: run
run:
	echo ##@${EXEC} -np ${NUM} -output-filename output/$(subst .out,,${OBJ}) ${OBJ} ${ARGS}
	@${EXEC} -np ${NUM} ${OBJ} ${ARGS}

.PHONY: clean
clean:
	-rm -rf ${OBJ}
	-rm -rf *.btr
	-rm -rf output/

.PHONY: all
all:	clean ${OBJ} run

.PHONY: viewrst
viewrst:	all
	-more output/*	
