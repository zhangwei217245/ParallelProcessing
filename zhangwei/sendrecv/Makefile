CC=mpicc
CFLAGS=-i_dynamic
OBJ=sendrecv.out
SRC=mpi_sendrecv.c
EXEC=mpirun


${OBJ}: ${SRC} 
	${CC} ${CFLAGS} -o $@ ${SRC}


.PHONY: run
run:
	${EXEC} -n ${NUM} ${OBJ}

.PHONY: clean
clean:
	-rm -rf ${OBJ}
	-rm -rf *.btr

.PHONY: all
all:	clean ${OBJ} run
	
