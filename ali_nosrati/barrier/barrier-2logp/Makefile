CC=mpicc
CFLAGS=-i_dynamic
OBJ=barrier.out
SRC=mpi_barrier.c
EXEC=mpirun


${OBJ}: ${SRC} 
	${CC} ${CFLAGS} -o $@ ${SRC}


.PHONY: run
run:
	@${EXEC} -np ${NUM} -output-filename output/$(subst .out,,${OBJ}) ${OBJ} ${ARGS}

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
