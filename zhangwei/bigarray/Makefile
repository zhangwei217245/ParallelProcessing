CC=mpicc
CFLAGS=-i_dynamic
OBJ=bigarray.out
SRC=mpi_bigarray.c
EXEC=mpirun


${OBJ}: ${SRC} 
	${CC} ${CFLAGS} -o $@ ${SRC}


.PHONY: run
run:
	${EXEC} -n ${NUM} ${OBJ}

.PHONY: clean
clean:
	rm -rf ${OBJ}

.PHONY: all
all:	clean ${OBJ} run
	
