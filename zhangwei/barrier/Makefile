CC=mpicc
CFLAGS=-i_dynamic
OBJ=mpi_barrier.out
UTIL=util.c 
SRC=mpi_barrier_test.c
EXEC=mpirun

OS=$(shell uname)
ifeq ($(OS),Darwin)
		UTIL=util_osx.c
else
		UTIL=util.c
endif

${OBJ}: ${SRC} ${ALGO}_barrier.c ${UTIL}
	${CC} ${CFLAGS} -o $@ ${SRC} ${ALGO}_barrier.c ${UTIL}


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
