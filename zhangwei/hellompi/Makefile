CC=mpicc
CFLAGS=-i_dynamic
OBJ=helloworld.out
SRC=mpi_hello.c
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
	
