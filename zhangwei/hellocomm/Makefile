CC=mpicc
CFLAGS=-i_dynamic
OBJ=hellocomm.out
SRC=mpi_hellocomm.c
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
	
