CC=mpicc
CFLAGS=-i_dynamic
OBJ=bigarray
SRC=mpi_bigarray.c
EXEC=mpirun


${OBJ}: ${SRC} 
	${CC} ${CFLAGS} -o $@ ${SRC}


.PHONY: run
run:	clean ${OBJ}
	${EXEC} -n ${NUM} ${OBJ}

.PHONY: clean
clean:
	rm -rf ${OBJ}
