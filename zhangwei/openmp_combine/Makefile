## Author: Wei Zhang
## Author: Ali Nosrati

CC=mpicc
CFLAGS=-i_dynamic -openmp
OBJ=hello.out
SRC=hello.c
EXEC=mpirun


${OBJ}: ${SRC} ${UTIL} 
	${CC} ${CFLAGS} -o $@ ${SRC} ${UTIL}


.PHONY: run
run:
		@${EXEC} --mca pls_rsh_agent ssh -np ${NUM} -x OMP_NUM_THREADS -output-filename output/$(subst .out,,${OBJ}) ${OBJ} ${ARGS}

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
